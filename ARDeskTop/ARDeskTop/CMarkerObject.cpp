#include <stdio.h>

#include <AR/ar.h>

#include "CWebCam.h"
#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CMarkerObject.h"
#include "CComponent3D.h"



CMarkerObject::CMarkerObject(void)
: CObject()
{
	shape		= 0;
	fix			= false;
	visible		= false;
	drawCard	= true;
	drawRect	= false;
}


CMarkerObject::~CMarkerObject()
{
	if(shape) delete shape;
}


void CMarkerObject::Precompute(void)
{
	if(shape)
		if(shape->visible) shape->Precompute();
}


void CMarkerObject::Draw(void)
{
	if(!visible) return;

	double glPara[16];

	glMatrixMode(GL_MODELVIEW);
	arglCameraViewRH(trans, glPara, VIEW_SCALEFACTOR);

	glLoadMatrixd(glPara);
	this->DrawShape();
}


void CMarkerObject::DrawShape(void)
{
	if(drawCard) {
		glBlendFunc(GL_ZERO, GL_ONE);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBegin(GL_POLYGON);
			glVertex3d(cardRect[0][0] + markerCenter[0], cardRect[0][1] + markerCenter[1], 0.0);
			glVertex3d(cardRect[1][0] + markerCenter[0], cardRect[1][1] + markerCenter[1], 0.0);
			glVertex3d(cardRect[2][0] + markerCenter[0], cardRect[2][1] + markerCenter[1], 0.0);
			glVertex3d(cardRect[3][0] + markerCenter[0], cardRect[3][1] + markerCenter[1], 0.0);
		glEnd();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	if(drawRect) {
		double halfWidth = markerWidth * 0.5;

		glLineWidth(3);
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glNormal3d(0.0, 0.0, 1.0);
		glBegin(GL_LINE_LOOP);
			glVertex2d( halfWidth,  halfWidth);
			glVertex2d( halfWidth, -halfWidth);
			glVertex2d(-halfWidth, -halfWidth);
			glVertex2d(-halfWidth,  halfWidth);
		glEnd();
	}

	if(shape)
		shape->Draw();
}


void CMarkerObject::Read(char* name)
{
}


double CMarkerObject::GetTransMat(ARMarkerInfo* info)
{
	if(!visible) {
		return(arGetTransMat(info,
			markerCenter, markerWidth,
			trans));
	} else {
		return(arGetTransMatCont(info, trans,
			markerCenter, markerWidth,
			trans));
	}
}


void CMarkerObject::DrawRect(void)
{
	if(!visible) return;

	double glPara[16];

	glMatrixMode(GL_MODELVIEW);
	arglCameraViewRH(trans, glPara, VIEW_SCALEFACTOR);

	glLoadMatrixd(glPara);

	double halfWidth = markerWidth * 0.5;

	glLineWidth(3);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glNormal3d(0.0, 0.0, 1.0);
	glBegin(GL_LINE_LOOP);
		glVertex2d( halfWidth,  halfWidth);
		glVertex2d( halfWidth, -halfWidth);
		glVertex2d(-halfWidth, -halfWidth);
		glVertex2d(-halfWidth,  halfWidth);
	glEnd();
}

