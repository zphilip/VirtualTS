#include <stdio.h>
#include <stdlib.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif
#include <AR/gsub.h>


#include "CComponent3D.h"
#include "CClickInfo.h"
#include "CGroup.h"
#include "CMainCursor.h"
#include "CAnimeManager.h"
#include "CARDeskTop.h"
#include "CUser.h"
#include "Color.h"



CComponent3D::CComponent3D(void)
: CObject()
{
	LoadIdentity(matrix);
	uniMatrix		= true;

	color[0]		= 0.0f;
	color[1]		= 0.0f;
	color[2]		= 0.0f;
	color[3]		= 1.0f;
	currentColor	= color;

	width			= 1.0;
	height			= 1.0;
	depth			= 1.0;

	touchable		= true;
	dropable		= false;
	changed			= true;
	fix				= false;
	drawBB			= false;
	notifyParent	= false;
	card			= false;
	flipZaxis		= false;
	FixAxis(false, false, false);

	parent			= 0;
	boundingBox		= 0;
	withinCursor	= 0;
	clickedCursor	= 0;
	animeManager	= 0;
}


CComponent3D::~CComponent3D()
{
	if(animeManager) delete animeManager;
}


CLink* CComponent3D::Remove(void)
{
	return(parent ? parent->RemoveObject(this) : 0);
}


void CComponent3D::Delete(void)
{
	if(parent)
		parent->DeleteObject(this);
	else
		delete this;
}


void CComponent3D::Precompute(void)
{
	CObject::Precompute();

	if(animeManager) {
		animeManager->Animate();
	}
}


void CComponent3D::Draw(void)
{
	if(!visible) return;

	float	alpha;

	alpha = GetAlpha();

	if(alpha == 0.0f) return;
	if(alpha < 1.0f)
		glDepthMask(GL_FALSE);

	glPushMatrix();
		if(clickedCursor) {
			double	pos[3];

			GetPosition(pos);
			SetPosition(0.0, 0.0, 0.0);
			glTranslated(pos[0], pos[1], pos[2]);

			if(CUser::cursorMode == CUser::kMulti) {
				glMultMatrixd(clickedCursor->multMatrix);
			} else {
				if(	CUser::cursorMode == CUser::kRotateA ||
					CUser::cursorMode == CUser::kRotateB ||
					CUser::cursorMode == CUser::kTwist
				) {
					glMultMatrixd(clickedCursor->multMatrix);
				}

				if(uniMatrix) {
					glTranslated(matrix[12], matrix[13], matrix[14]);
				} else {
					glMultMatrixd(matrix);
				}

				if(CUser::cursorMode == CUser::kScale)
					glMultMatrixd(clickedCursor->multMatrix);
			}

			SetColor();
			this->DrawShape();
			this->DrawBoundingBox();

			SetPosition(pos);
		} else {
			if(uniMatrix) {
				glTranslated(matrix[12], matrix[13], matrix[14]);
			} else {
				glMultMatrixd(matrix);
			}

			SetColor();
			this->DrawShape();
			this->DrawBoundingBox();

		}
	glPopMatrix();

	if(alpha < 1.0f)
		glDepthMask(GL_TRUE);
}


void CComponent3D::KeyboardFunc3D(unsigned char key)
{
}


CComponent3D* CComponent3D::IsWithin(double x, double y, double z)
{
	if(!touchable || clickedCursor || !visible) return(0);

	double newPos[3];

	InvertPos(x, y, z, newPos);

	if(boundingBox)
		return(boundingBox->IsWithin(newPos) ? this : 0);
	else 
		return(	-0.5 * width  <= newPos[0] && newPos[0] <= 0.5 * width  &&
				-0.5 * height <= newPos[1] && newPos[1] <= 0.5 * height &&
				-0.5 * depth  <= newPos[2] && newPos[2] <= 0.5 * depth  ? this : 0);
}


void CComponent3D::DrawBoundingBox(void)
{
	if(!touchable) return;

	bool dropping = withinCursor ? withinCursor->GetDropObject() == this : false;

	if(!drawBB && !dropping) return;

	if(clickedCursor || withinCursor) {
		if(boundingBox) {
			glPushMatrix();
			glMultMatrixd(boundingBox->matrix);
		}

		glColor3f(1.0f, 1.0f, 1.0f);
		glLineWidth(4.0);
		if(card) {
			glNormal3d(0.0, 0.0, 1.0);
			glBegin(GL_LINE_LOOP);
				glVertex3d( 0.5,  0.5, 0.0);
				glVertex3d( 0.5, -0.5, 0.0);
				glVertex3d(-0.5, -0.5, 0.0);
				glVertex3d(-0.5,  0.5, 0.0);
			glEnd();
		} else {
			glutWireCube(1.0);
		}

		if(boundingBox) {
			glPopMatrix();
		}
	}
}


void CComponent3D::Click(void)
{
	if(!clickedCursor) return;

	switch(CUser::cursorMode) {
	case CUser::kTranslate :
		if(clickedCursor->dropObject) {
			clickedCursor->dropObject->withinCursor = 0;
			clickedCursor->dropObject->DropFrom(this, clickedCursor);
		}
		this->DropTo(clickedCursor->dropObject, clickedCursor);
		clickedCursor->dropObject = 0;

	//	CARDeskTop::arDeskTop->user->SetCursorMode(CUser::kMulti);
		break;
	case CUser::kRotateA :
		clickedCursor->RotateA(this, true);
		break;
	case CUser::kRotateB :
		clickedCursor->RotateB(this, true);
		break;
	case CUser::kScale :
		clickedCursor->Scale(this, true);
		break;
	case CUser::kMulti :
		clickedCursor->Multi(this, true);
		break;
	case CUser::kTwist :
		clickedCursor->Twist(this, true);
		break;
	}

	this->ClickUp(clickedCursor);
	changed = true;
	clickedCursor->selectObject = 0; // unselect object;
	clickedCursor = 0;
}


void CComponent3D::DoubleClick(void)
{
	printf("double click!\n");

	clickedCursor->selectObject = 0; // unselect object;
	clickedCursor = 0;
}


void CComponent3D::InterruptClick(CMainCursor* cursor, int mode)
{
}


void CComponent3D::InterruptTranslate(double* pos, double* objectPos)
{
	if(fixedAxis[0]) pos[0] = objectPos[0];
	if(fixedAxis[1]) pos[1] = objectPos[1];
	if(fixedAxis[2]) pos[2] = objectPos[2];
}


void CComponent3D::InterruptRotate(CMainCursor* cursor)
{
}


void CComponent3D::CursorEnter(CCursor* cursor)
{
	withinCursor = cursor;
	withinCursor->shape->SetColor3v(Color::yellow);

	if(parent && notifyParent)
		parent->ItemDown(this);
}


void CComponent3D::CursorLeave(CCursor* cursor)
{
	cursor->shape->SetColor3v(Color::blue);
	withinCursor = 0;

	if(parent && notifyParent)
		parent->ItemUp(this);
}


void CComponent3D::CursorWithin(CCursor* cursor)
{
}


void CComponent3D::ClickDown(CMainCursor* cursor)
{
	if(parent && notifyParent)
		parent->ItemClickDown(this);
}


void CComponent3D::ClickUp(CMainCursor* cursor)
{
	if(parent && notifyParent)
		parent->ItemClickUp(this);
}


void CComponent3D::DropFrom(CComponent3D* object, CMainCursor* cursor)
{
}


void CComponent3D::DropTo(CComponent3D* object, CMainCursor* cursor)
{
}


void CComponent3D::SetString(wchar_t* string)
{
}


void CComponent3D::SetCardMode(bool pCard)
{
	card		= pCard;
}


void CComponent3D::SetPosition(double x, double y, double z)
{
	matrix[12]	= x;
	matrix[13]	= y;
	matrix[14]	= z;
	changed		= true;
}


void CComponent3D::GetPosition(double pos[3])
{
	pos[0] = matrix[12];
	pos[1] = matrix[13];
	pos[2] = matrix[14];
}


void CComponent3D::SetSize(double w, double h, double d)
{
//	matrix[ 0] =   w; matrix[ 4] = 0.0; matrix[ 8] = 0.0;
//	matrix[ 1] = 0.0; matrix[ 5] =   h; matrix[ 9] = 0.0;
//	matrix[ 2] = 0.0; matrix[ 6] = 0.0; matrix[10] =   d;
//	matrix[ 3] = 0.0; matrix[ 7] = 0.0; matrix[11] = 0.0; matrix[15] = 1.0;

//	SetSize2(w, h, d);
	width		= w;
	height		= h;
	depth		= d;

	changed		= true;
//	uniMatrix	= false;
}


void CComponent3D::GetSize(double size[3])
{
	size[0]		= width;
	size[1]		= height;
	size[2]		= depth;
}


void CComponent3D::Scale(double x, double y, double z)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadMatrixd(matrix);
		glScaled(x, y, z);
		glGetDoublev(GL_MODELVIEW_MATRIX, matrix);
	glPopMatrix();
	changed		= true;
	uniMatrix	= false;
}


void CComponent3D::Rotate(double angle, double x, double y, double z)
{
	double	pos[3];

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

		GetPosition(pos);
		SetPosition(0.0, 0.0, 0.0);
		glTranslated(pos[0], pos[1], pos[2]);
		glRotated(angle, x, y, z);
		glMultMatrixd(matrix);

		glGetDoublev(GL_MODELVIEW_MATRIX, matrix);
	glPopMatrix();
	changed		= true;
	uniMatrix	= false;
}


void CComponent3D::Multiply(double m[16])
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadMatrixd(matrix);
		glMultMatrixd(m);
		glGetDoublev(GL_MODELVIEW_MATRIX, matrix);
	glPopMatrix();
	changed		= true;
	uniMatrix	= false;
}


void CComponent3D::SetCurrentColor(float* pCurrentColor)
{
	currentColor = pCurrentColor;
}


void CComponent3D::SetColor3(float r, float g, float b)
{
	if(currentColor) Color::SetColor3(currentColor, r, g, b);
}


void CComponent3D::SetColor4(float r, float g, float b, float a)
{
	if(currentColor) Color::SetColor4(currentColor, r, g, b, a);
}


void CComponent3D::SetColor(void)
{
	glColor4fv(currentColor);
}


void CComponent3D::SetAlpha(float a)
{
	if(!currentColor) return;
	currentColor[3] = a;
}


float CComponent3D::GetAlpha(void)
{
	return(currentColor ? currentColor[3] : -1.0f);
}


void CComponent3D::SetParent(CGroup* pParent)
{
	parent = pParent;
}


void CComponent3D::GlobalToLocal(double pos[3])
{
	if(parent)
		parent->GlobalToLocal(pos);

	InvertPos(pos[0], pos[1], pos[2], pos);
}


void CComponent3D::LocalToGlobal(double pos[3])
{
	ConvertPos(pos[0], pos[1], pos[2], pos);

	if(parent)
		parent->LocalToGlobal(pos);
}


void CComponent3D::GlobalToLocal(void)
{
	if(parent)
		parent->GlobalToLocal();

	double mat[16];
	if(changed) {
		double	mat2[3][4];
		ConvertToARMat(matrix, mat2);
		arUtilMatInv(mat2, matrixInv);
		changed = false;
	}
	argConvGlpara(matrixInv, mat);
	glMultMatrixd(mat);
}


void CComponent3D::LocalToGlobal(void)
{
	glMultMatrixd(matrix);

	if(parent)
		parent->LocalToGlobal();
}


void CComponent3D::GetGlobalMatrix(double mat[])
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();
		GlobalToLocal();
		glGetDoublev(GL_MODELVIEW_MATRIX, mat);
	glPopMatrix();
}


void CComponent3D::GetLocalMatrix(double mat[])
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();
		LocalToGlobal();
		glGetDoublev(GL_MODELVIEW_MATRIX, mat);
	glPopMatrix();
}


void CComponent3D::SetAnimeManager(CAnimeManager* pAnimeManager)
{
	if(animeManager) delete animeManager;
	animeManager = pAnimeManager;
}


void CComponent3D::Play(void)
{
	if(animeManager) animeManager->Play();
}


void CComponent3D::Replay(void)
{
	if(animeManager) animeManager->Replay();
}


void CComponent3D::Stop(void)
{
	if(animeManager) animeManager->Stop();
}


void CComponent3D::Pause(void)
{
	if(animeManager) animeManager->Pause();
}


void CComponent3D::Skip(double skipTimeMS)
{
	if(animeManager) animeManager->Skip(skipTimeMS);
}


void CComponent3D::Animate(void)
{
	if(animeManager) animeManager->Animate();
}


void CComponent3D::InvertPos(double x, double y, double z, double pos[3])
{
	x -= matrix[12];
	y -= matrix[13];
	z -= matrix[14];

	if(uniMatrix) {
		pos[0] = x;
		pos[1] = y;
		pos[2] = z;

		return;
	}

	if(changed) {
		double	mat[3][4];
		ConvertToARMat(matrix, mat);
		arUtilMatInv(mat, matrixInv);
		changed = false;
	}
	pos[0] = matrixInv[0][0] * x + matrixInv[0][1] * y + matrixInv[0][2] * z;
	pos[1] = matrixInv[1][0] * x + matrixInv[1][1] * y + matrixInv[1][2] * z;
	pos[2] = matrixInv[2][0] * x + matrixInv[2][1] * y + matrixInv[2][2] * z;
}


void CComponent3D::ConvertPos(double x, double y, double z, double pos[3])
{
	double newPos[3];

	newPos[0] = matrix[ 0] * pos[0] + matrix[ 4] * pos[1] + matrix[ 8] * pos[2] + matrix[12];
	newPos[1] = matrix[ 1] * pos[0] + matrix[ 5] * pos[1] + matrix[ 9] * pos[2] + matrix[13];
	newPos[2] = matrix[ 2] * pos[0] + matrix[ 6] * pos[1] + matrix[10] * pos[2] + matrix[14];

	pos[0] += newPos[0];
	pos[1] += newPos[1];
	pos[2] += newPos[2];
}


void CComponent3D::FixAxis(bool x, bool y, bool z)
{
	fixedAxis[0] = x;
	fixedAxis[1] = y;
	fixedAxis[2] = z;
}


void LoadIdentity(double matrix[16])
{
	matrix[ 0] = 1.0; matrix[ 4] = 0.0; matrix[ 8] = 0.0; matrix[12] = 0.0;
	matrix[ 1] = 0.0; matrix[ 5] = 1.0; matrix[ 9] = 0.0; matrix[13] = 0.0;
	matrix[ 2] = 0.0; matrix[ 6] = 0.0; matrix[10] = 1.0; matrix[14] = 0.0;
	matrix[ 3] = 0.0; matrix[ 7] = 0.0; matrix[11] = 0.0; matrix[15] = 1.0;
}


void ConvertToARMat(double matrix[16], double trans[3][4])
{
	trans[0][0] = matrix[ 0]; trans[0][1] = matrix[ 4]; trans[0][2] = matrix[ 8]; trans[0][3] = matrix[12];
	trans[1][0] = matrix[ 1]; trans[1][1] = matrix[ 5]; trans[1][2] = matrix[ 9]; trans[1][3] = matrix[13];
	trans[2][0] = matrix[ 2]; trans[2][1] = matrix[ 6]; trans[2][2] = matrix[10]; trans[2][3] = matrix[14];
}


void CalcShadowMatrix(double* plane, double* lightPos, double* shadowMatrix)
{
	double dot = plane[0] * lightPos[0] + plane[1] * lightPos[1] + plane[2] * lightPos[2] 
		+ plane[3] * lightPos[3];

	for(int i = 0; i < 4; i++) {
		for(int j = 0; j < 4; j++) {
			shadowMatrix[i * 4 + j] = -plane[i] * lightPos[j];
			if(i == j) {
				shadowMatrix[i * 4 + j] += dot;
			}
		}
	}
}
