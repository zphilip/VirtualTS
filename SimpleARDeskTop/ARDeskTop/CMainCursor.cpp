#include <math.h>
#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif
#include <stdio.h>

#include <AR/gsub.h>

#include "CMainCursor.h"
#include "CComponent3D.h"
#include "CDesktop.h"
#include "CLine3D.h"
#include "CVertex.h"
#include "CClickInfo.h"
#include "CWorld.h"
#include "CHand.h"
#include "CSubCursor.h"
#include "CUser.h"
#include "CARDeskTop.h"

#include "PI.h"



CMainCursor::CMainCursor(void)
: CCursor()
{
	selectObject		= 0;
	dropObject			= 0;
	prevDropObject		= 0;
	selectObjectIndex	= -1;
}


CMainCursor::~CMainCursor()
{
}


void CMainCursor::Init(void)
{
	CCursor::Init();
}


void CMainCursor::Precompute(int id)
{
	if(!visible || !projectionList[id]->visible) return;

	double localPos[3];

	if(selectObject) {
		if(selectObjectIndex != id) return;

		projectionList[id]->ProjectCursorPos(this, cursorPos);

		if(CUser::cursorMode == CUser::kTranslate) {
			localPos[0] = cursorPos[0];
			localPos[1] = cursorPos[1];
			localPos[2] = cursorPos[2];

			if(selectObject->parent)
		//		MultiMatrix(selectObjectMatInv, cursorPos, localPos);
				selectObject->parent->GlobalToLocal(localPos);


			localPos[0] -= firstClickInfo.vector[0];
			localPos[1] -= firstClickInfo.vector[1];
			localPos[2] -= firstClickInfo.vector[2];

			selectObject->InterruptTranslate(localPos, firstClickInfo.objectPos);
			selectObject->SetPosition(localPos[0], localPos[1], localPos[2]);

			//ドロップ
			if(selectObject->dropable) {
				CComponent3D* comp = 0;
				prevDropObject = dropObject;
				dropObject = 0;
				for(int i = 0; i < CCursor::projectionNum; i++) {
					if(projectionList[i]->visible) {
						projectionList[i]->ProjectCursorPos(this, cursorPos);
						comp = projectionList[i]->IsWithin(cursorPos[0], cursorPos[1], cursorPos[2]);
						if(comp) dropObject = comp;
					}
				}
				if(prevDropObject) {
					prevDropObject->withinCursor = 0;
				}
				if(dropObject) {
					dropObject->withinCursor = this;
				}
			}
		} else if(CUser::cursorMode == CUser::kDraw) {
			localPos[0]	= cursorPos[0];
			localPos[1]	= cursorPos[1];
			localPos[2]	= cursorPos[2];

			selectObject->GlobalToLocal(localPos);
			//selectObject->InvertPos(localPos[0], localPos[1], localPos[2], localPos);

			CVertex* vertex = new CVertex();
			vertex->Init();
			vertex->v[0] = localPos[0];
			vertex->v[1] = localPos[1];
			vertex->v[2] = localPos[2];
			((CLine3D*) selectObject)->AddVertex(vertex);
		} else if(
			CUser::cursorMode == CUser::kRotateA ||
			CUser::cursorMode == CUser::kRotateB ||
			CUser::cursorMode == CUser::kScale ||
			CUser::cursorMode == CUser::kTwist
		) {
			currentClickInfo.Click(this, selectObject, CClickInfo::kMiddle);
		} else if(CUser::cursorMode == CUser::kMulti && ci1.cursor) {
			currentClickInfo.Click2(this, selectObject, CClickInfo::kMiddle);
			projectionList[id]->ProjectCursorPos(ci1.cursor, ci1.cursor->cursorPos);
			ci1.currentClickInfo.Click2(ci1.cursor, selectObject, CClickInfo::kMiddle);

			if(ci2.cursor) {
				projectionList[id]->ProjectCursorPos(ci2.cursor, ci2.cursor->cursorPos);
				ci2.currentClickInfo.Click2(ci2.cursor, selectObject, CClickInfo::kMiddle);
			}
		}

		if(CUser::cursorMode == CUser::kMulti)
			Multi(selectObject);
		else if(CUser::cursorMode == CUser::kRotateA)
			RotateA(selectObject);
		else if(CUser::cursorMode == CUser::kRotateB)
			RotateB(selectObject);
		else if(CUser::cursorMode == CUser::kScale)
			Scale(selectObject);
		else if(CUser::cursorMode == CUser::kTwist)
			Twist(selectObject);
	} else {
		CCursor::Precompute(id);
	}
}


CComponent3D* CMainCursor::GetDropObject(void)
{
	return(dropObject);
}


int CMainCursor::Click(void)
{
	selectObjectIndex = FindSelectObject();

	if(selectObjectIndex == -1) { //何もないところでクリック
		if(CUser::cursorMode == CUser::kTranslate) { //移動なら世界を選択
			selectObjectIndex = 0;
			selectObject = CDesktop::desktop->world;
		}
	} else {
		if(CUser::cursorMode != CUser::kDraw) {
			selectObject = !withinObject[selectObjectIndex]->fix ? withinObject[selectObjectIndex] : FindNotFixed(withinObject[selectObjectIndex]->parent);
		}
	}

	if(CUser::cursorMode == CUser::kDraw) {
		CGroup* group;

		if(selectObjectIndex == -1) {
			selectObjectIndex = 0;
			group = CDesktop::desktop->world;
		} else if(withinObject[selectObjectIndex]) {
			group = FindNotFixed(withinObject[selectObjectIndex]->parent);
		}

		if(group) {
			CLine3D* line = new CLine3D();
			line->Init();

		//	group->objectList.GoToFirst(); //バグるので
			group->AddObject(line);
			selectObject = line;
		}
	}

	if(selectObject) {
		selectObject->clickedCursor = this;
		CCursor::projectionList[selectObjectIndex]->ProjectCursorPos(this, cursorPos);
		if(selectObject->parent && CUser::cursorMode != CUser::kDraw)
			selectObject->parent->GetGlobalMatrix(selectObjectMatInv);
		if(CUser::cursorMode == CUser::kTwist)
			CompTwistNormal();
		firstClickInfo.Click(this, selectObject);
		selectObject->ClickDown(this);
	}

	return(selectObject ? 1 : 0);
}


int CMainCursor::MultiClick(CHand* hand, int handClick)
{
	if(ci1.cursor) {
		ci1.cursor	= 0;
		return(-1); //Unselect
	}

	selectObjectIndex = FindSelectObject();

	if(selectObjectIndex == -1) {
//		CUser::cursorMode = CUser::kTranslate;
		return(0); //Nonselect
	}

	//別の手のカーソルがどちらか見えているか
	bool visible1 = false, visible2 = false;

	if(hand->subCursor) {
		if(hand->subCursor->visible) visible1 = true;
	}
	if(hand->mainCursor) {
		if(hand->mainCursor->visible) visible2 = true;
	}
	if(!visible1 && !visible2) {
//		CUser::cursorMode = CUser::kTranslate;
		return(0); //Nonselect
	}

	ci1.cursor = hand->FindWithinObject(withinObject[selectObjectIndex], selectObjectIndex);
	if(!ci1.cursor) {
//		CUser::cursorMode = CUser::kTranslate;
		return(0); //Nonselect
	}

	ci2.cursor = 0;
	if(ci1.cursor == hand->subCursor && hand->mainCursor) {
		ci2.cursor = hand->mainCursor->FindWithinObject(withinObject[selectObjectIndex], selectObjectIndex);
	}

	selectObject = !withinObject[selectObjectIndex]->fix ?
		withinObject[selectObjectIndex] : FindNotFixed(withinObject[selectObjectIndex]->parent);
	if(!selectObject) {
//		CUser::cursorMode = CUser::kTranslate;
		return(0); //Nonselect
	}

	selectObject->clickedCursor = this;
	CCursor::projectionList[selectObjectIndex]->ProjectCursorPos(this, cursorPos);

	if(selectObject->parent) {
		selectObject->parent->GetGlobalMatrix(selectObjectMatInv);
		for(int i = 0; i < 16; i++)
			ci1.cursor->selectObjectMatInv[i] = selectObjectMatInv[i];
	}

	firstClickInfo.Click(this, selectObject);
	ci1.firstClickInfo.Click2(ci1.cursor, selectObject);
	ci1.Precompute(firstClickInfo.vector);

	if(ci2.cursor) {
		for(int i = 0; i < 16; i++)
			ci2.cursor->selectObjectMatInv[i] = selectObjectMatInv[i];
		ci2.firstClickInfo.Click2(ci2.cursor, selectObject);
		ci2.Precompute(ci1.firstClickInfo.vector);
	}


	selectObject->ClickDown(this);

	return(1); // Select
}


void CMainCursor::RotateA(CComponent3D* object, bool fix)
{
	double cosa;

	object->InterruptRotate(this);

	OuterProduct(firstClickInfo.vector, currentClickInfo.vector, normal);
	cosa = InnerProduct(firstClickInfo.vector, currentClickInfo.vector);
	cosa /= firstClickInfo.len * currentClickInfo.len;

	if(cosa >= 1.0 - 0.0000001) {
		if(!fix) LoadIdentity(multMatrix);
		return;
	}
	
	angle = acos(cosa) * 180.0 / PI;

	if(fix) {
		double pos[3];
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			object->GetPosition(pos);
			object->SetPosition(0.0, 0.0, 0.0);

			glLoadIdentity();
			glTranslated(pos[0], pos[1], pos[2]);
			glRotated(angle, normal[0], normal[1], normal[2]);
			glMultMatrixd(object->matrix);

			glGetDoublev(GL_MODELVIEW_MATRIX, object->matrix);
		glPopMatrix();
		object->uniMatrix	= false;
	} else {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			glRotated(angle, normal[0], normal[1], normal[2]);
			glGetDoublev(GL_MODELVIEW_MATRIX, multMatrix);
		glPopMatrix();

	//	glRotated(angle, normal[0], normal[1], normal[2]);
	}
}


void CMainCursor::RotateB(CComponent3D* object, bool fix)
{
}


void CMainCursor::Scale(CComponent3D* object, bool fix)
{
	double scaleX, scaleY, scaleZ;

	scaleX = currentClickInfo.vector[0] / firstClickInfo.vector[0];
	scaleY = currentClickInfo.vector[1] / firstClickInfo.vector[1];
	scaleZ = currentClickInfo.vector[2] / firstClickInfo.vector[2];
	if(scaleX < 0.0) scaleX *= -1.0;
	if(scaleY < 0.0) scaleY *= -1.0;
	if(scaleZ < 0.0) scaleZ *= -1.0;

	if(0) {
		double min;

		min = scaleX;
		if(scaleY < min) min = scaleX;
		if(scaleZ < min) min = scaleZ;

		scaleX = min;
		scaleY = min;
		scaleZ = min;
	}

	if(fix) {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadMatrixd(object->matrix);
			glScaled(scaleX, scaleY, scaleZ);
			glGetDoublev(GL_MODELVIEW_MATRIX, object->matrix);
		glPopMatrix();
		object->uniMatrix	= false;
	} else {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			glScaled(scaleX, scaleY, scaleZ);
			glGetDoublev(GL_MODELVIEW_MATRIX, multMatrix);
		glPopMatrix();

	//	glScaled(scaleX, scaleY, scaleZ);
	}
}


void CMainCursor::Multi(CComponent3D* object, bool fix)
{
	double d2[3], dist2, s, normal[3], cosa, angle2;

	d2[0] = currentClickInfo.vector[0] - ci1.currentClickInfo.vector[0];
	d2[1] = currentClickInfo.vector[1] - ci1.currentClickInfo.vector[1];
	d2[2] = currentClickInfo.vector[2] - ci1.currentClickInfo.vector[2];
	dist2 = InnerProduct(d2, d2);

	s = sqrt(dist2 / ci1.dist);

	OuterProduct(ci1.d, d2, normal);
	cosa = InnerProduct(ci1.d, d2);
	cosa /= sqrt(ci1.dist * dist2);
	angle = acos(cosa) * 180.0 / PI;

	if(ci2.cursor) {
		double mat[3][4], matrixInv[3][4];
		double len;

		//z
		len = sqrt(dist2);
		mat[0][2] = d2[0] / len;
		mat[1][2] = d2[1] / len;
		mat[2][2] = d2[2] / len;

		//y
		len = sqrt(InnerProduct(normal, normal));
		mat[0][1] = normal[0] / len;
		mat[1][1] = normal[1] / len;
		mat[2][1] = normal[2] / len;

		//x
		mat[0][0] = mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2];
		mat[1][0] = mat[2][1] * mat[0][2] - mat[0][1] * mat[2][2];
		mat[2][0] = mat[0][1] * mat[1][2] - mat[1][1] * mat[0][2];

		//pos
		mat[0][3] = 0.0;
		mat[1][3] = 0.0;
		mat[2][3] = 0.0;

		arUtilMatInv(mat, matrixInv);

		double x, y, z;
		double firstPos[3], currentPos[3];

		x = ci2.firstClickInfo.vector2[0] - ci1.firstClickInfo.vector2[0];
		y = ci2.firstClickInfo.vector2[1] - ci1.firstClickInfo.vector2[1];
		z = ci2.firstClickInfo.vector2[2] - ci1.firstClickInfo.vector2[2];

		firstPos[0] = matrixInv[0][0] * x + matrixInv[0][1] * y + matrixInv[0][2] * z;
		firstPos[1] = matrixInv[1][0] * x + matrixInv[1][1] * y + matrixInv[1][2] * z;
	//	firstPos[2] = matrixInv[2][0] * x + matrixInv[2][1] * y + matrixInv[2][2] * z;

		x = ci2.currentClickInfo.vector2[0] - ci1.currentClickInfo.vector2[0];
		y = ci2.currentClickInfo.vector2[1] - ci1.currentClickInfo.vector2[1];
		z = ci2.currentClickInfo.vector2[2] - ci1.currentClickInfo.vector2[2];

		currentPos[0] = matrixInv[0][0] * x + matrixInv[0][1] * y + matrixInv[0][2] * z;
		currentPos[1] = matrixInv[1][0] * x + matrixInv[1][1] * y + matrixInv[1][2] * z;
	//	currentPos[2] = matrixInv[2][0] * x + matrixInv[2][1] * y + matrixInv[2][2] * z;

		int f, c;

		if(selectObject->flipZaxis) {
			f = 0; c = 1;
		} else {
			f = 1; c = 0;
		}

		cosa = firstPos[f] * currentPos[f] + firstPos[c] * currentPos[c];
		cosa /= sqrt((firstPos[f] * firstPos[f] + firstPos[c] * firstPos[c]) * (currentPos[f] * currentPos[f] + currentPos[c] * currentPos[c]));
		angle2 = acos(cosa) * 180.0 / PI;

		if((firstPos[c] * currentPos[f] - firstPos[f] * currentPos[c]) < 0.0) angle2 *= -1.0;


	/*	cosa = firstPos[0] * currentPos[0] + firstPos[2] * currentPos[2];
		cosa /= sqrt((firstPos[0] * firstPos[0] + firstPos[2] * firstPos[2]) * (currentPos[0] * currentPos[0] + currentPos[2] * currentPos[2]));
		angle2 = acos(cosa) * 180.0 / PI;

		if((firstPos[2] * currentPos[0] - firstPos[0] * currentPos[2]) < 0.0) angle2 *= -1.0;
		*/
	}

	double pos[3];
	if(fix) {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			object->GetPosition(pos);
			object->SetPosition(0.0, 0.0, 0.0);

			glLoadIdentity();
			glTranslated(pos[0], pos[1], pos[2]);

			glTranslated(currentClickInfo.vector[0], currentClickInfo.vector[1], currentClickInfo.vector[2]);
			if(ci2.cursor) {
			//	ci2.cursor->ResetWithinObject();
				ci2.cursor = 0;
				glRotated(angle2, d2[0], d2[1], d2[2]);
			}
			glRotated(angle, normal[0], normal[1], normal[2]);
			glMultMatrixd(object->matrix);

			glScaled(s, s, s);
			glTranslated(-firstClickInfo.vector2[0], -firstClickInfo.vector2[1], -firstClickInfo.vector2[2]);

			glGetDoublev(GL_MODELVIEW_MATRIX, object->matrix);
		glPopMatrix();
		object->uniMatrix	= false;
	} else {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			object->GetPosition(pos);
			object->SetPosition(0.0, 0.0, 0.0);

			glLoadIdentity();
			glTranslated(ci1.currentClickInfo.vector[0], ci1.currentClickInfo.vector[1], ci1.currentClickInfo.vector[2]);
			if(ci2.cursor)
				glRotated(angle2, d2[0], d2[1], d2[2]);
			glRotated(angle, normal[0], normal[1], normal[2]);
			glMultMatrixd(object->matrix);

			glScaled(s, s, s);
			glTranslated(-ci1.firstClickInfo.vector2[0], -ci1.firstClickInfo.vector2[1], -ci1.firstClickInfo.vector2[2]);

			glGetDoublev(GL_MODELVIEW_MATRIX, multMatrix);

			object->SetPosition(pos);
		glPopMatrix();
	}
}


void CMainCursor::Twist(CComponent3D* object, bool fix)
{
	double cosa;

	cosa = InnerProduct(firstClickInfo.vector, currentClickInfo.vector);
	cosa /= firstClickInfo.len * currentClickInfo.len;

	if(cosa >= 1.0 - 0.0000001) {
		if(!fix) LoadIdentity(multMatrix);
		return;
	}

	angle = acos(cosa) * 180.0 / PI;

	double out[3];

	OuterProduct(firstClickInfo.vector, currentClickInfo.vector, out);

	if(InnerProduct(out, normal) < 0.0) angle *= -1.0;

	if(fix) {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			object->GetPosition(out);
			object->SetPosition(0.0, 0.0, 0.0);

			glLoadIdentity();
			glTranslated(out[0], out[1], out[2]);
			glRotated(angle, normal[0], normal[1], normal[2]);
			glMultMatrixd(object->matrix);

			glGetDoublev(GL_MODELVIEW_MATRIX, object->matrix);
		glPopMatrix();

		object->uniMatrix	= false;
	} else {
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			glRotated(angle, normal[0], normal[1], normal[2]);
			glGetDoublev(GL_MODELVIEW_MATRIX, multMatrix);
		glPopMatrix();
	}
}


void CMainCursor::CompTwistNormal(void)
{
	if(!selectObject) return;

	CComponent3D* handle = selectObject->GetHandleCube();

	if(handle) {
		double pos[3];
		handle->GetPosition(pos);

		normal[0] = selectObject->matrix[ 0] * pos[0] + selectObject->matrix[ 4] * pos[1] + selectObject->matrix[ 8] * pos[2];
		normal[1] = selectObject->matrix[ 1] * pos[0] + selectObject->matrix[ 5] * pos[1] + selectObject->matrix[ 9] * pos[2];
		normal[2] = selectObject->matrix[ 2] * pos[0] + selectObject->matrix[ 6] * pos[1] + selectObject->matrix[10] * pos[2];
	} else {
		//y軸
		normal[0] = selectObject->matrix[ 4];
		normal[1] = selectObject->matrix[ 5];
		normal[2] = selectObject->matrix[ 6];
	}
}


int CMainCursor::FindSelectObject(void)
{
	int ret = -1;
	for(int i = 0; i < CCursor::projectionNum; i++) {
		if(withinObject[i]) ret = i;
	}

	return(ret);
}

