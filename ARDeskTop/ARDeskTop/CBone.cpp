#include <stdio.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CBone.h"
#include "CMMDObject.h"
#include "CBoundingBox.h"
#include "CUser.h"
#include "CMainCursor.h"
#include "MMD/PMDBone.h"
#include "Color.h"


double	CHandleCube::boundSize			= 2.0;
bool	CHandleCube::drawDot			= false;
bool	CHandleCube::drawWireMaster		= false;
bool	CHandleCube::drawWireWithDepth	= false;



CHandleCube::CHandleCube(void)
: CComponent3D()
{
	drawWire	= false;
}


CHandleCube::~CHandleCube()
{
}


void CHandleCube::DrawShape(void)
{
	if(drawWire && drawWireMaster) {
		if(drawWireWithDepth) {
			glEnable(GL_DEPTH_TEST);
			glutWireCube(boundSize);
			glDisable(GL_DEPTH_TEST);
		} else {
			glDisable(GL_DEPTH_TEST);
			glutWireCube(boundSize);
		}
	}

	if(drawDot) {
		glDisable(GL_LIGHTING);
		if(drawWire) {
			glPointSize(12.0f);
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		} else {
			glPointSize(3.0f);
			glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
		}
	//	glutSolidCube(0.2);
		glDisable(GL_DEPTH_TEST);
		glBegin(GL_POINTS);
			glVertex3d(0.0, 0.0, 0.0);
		glEnd();
		glEnable(GL_LIGHTING);
	}

	glEnable(GL_DEPTH_TEST);
}


CComponent3D* CHandleCube::IsWithin(double x, double y, double z)
{
	if(!touchable || clickedCursor || !visible) return(0);

	double newPos[3];

	InvertPos(x, y, z, newPos);

	return(	-boundSize * 0.5 <= newPos[0] && newPos[0] <= boundSize * 0.5 &&
			-boundSize * 0.5 <= newPos[1] && newPos[1] <= boundSize * 0.5 &&
			-boundSize * 0.5 <= newPos[2] && newPos[2] <= boundSize * 0.5 ? this : 0);
}


void CHandleCube::CursorEnter(CCursor* cursor)
{
	CComponent3D::CursorEnter(cursor);
	cursor->shape->SetColor3v(Color::green);
	drawWire	= true;
}


void CHandleCube::CursorLeave(CCursor* cursor)
{
	CComponent3D::CursorLeave(cursor);
	drawWire	= false;
}





CBone::CBone(void)
: CGroup()
{
	handleCube	= 0;
	mmdObject	= 0;
}


CBone::~CBone()
{
}


void CBone::Init(void)
{
	CGroup::Init();

	handleCube = new CHandleCube();
	handleCube->Init();
	handleCube->fix = true;
	handleCube->SetColor4(1.0f, 1.0f, 1.0f, 1.0f);

	AddObject(handleCube);
}


void CBone::Precompute(void)
{
	CGroup::Precompute();
}


void CBone::ClickDown(CMainCursor* cursor)
{
	if(	mmdObject && (
		CUser::cursorMode == CUser::kTranslate ||
		CUser::cursorMode == CUser::kRotateA ||
		CUser::cursorMode == CUser::kRotateB ||
		CUser::cursorMode == CUser::kScale ||
		CUser::cursorMode == CUser::kMulti ||
		CUser::cursorMode == CUser::kTwist)
	) mmdObject->isHandling = true;
}


void CBone::ClickUp(CMainCursor* cursor)
{
	CGroup::ClickUp(cursor);

	mmdObject->isHandling = false;
	mmdObject->BoundsModel();
}


CComponent3D* CBone::GetHandleCube(void)
{
	return(handleCube);
}





CFKBone::CFKBone(void)
: CBone()
{
	pmdBone	= 0;
}


CFKBone::~CFKBone()
{
}


void CFKBone::UpdateMatrix(void)
{
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
				) glMultMatrixd(clickedCursor->multMatrix);

				if(uniMatrix) {
					glTranslated(matrix[12], matrix[13], matrix[14]);
				} else {
					glMultMatrixd(matrix);
				}

				if(CUser::cursorMode == CUser::kScale)
					glMultMatrixd(clickedCursor->multMatrix);
			}

			SetPosition(pos);
		} else {
			if(uniMatrix) {
				glTranslated(matrix[12], matrix[13], matrix[14]);
			} else {
				glMultMatrixd(matrix);
			}
		}
		glGetDoublev(GL_MODELVIEW_MATRIX, gMatrix);


		int degree = objectList.GetDegree();

		objectList.GoToFirst();
		for(int i = 0; i < degree; i++) {
			((CComponent3D*) objectList.GetKey())->UpdateMatrix();
			objectList.GoToNext();
		}
	glPopMatrix();
}

