#include "CMarkerDisplay.h"

#include <stdio.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CComponent3D.h"



CMarkerDisplay::CMarkerDisplay(void)
: CWebCam()
{
}


CMarkerDisplay::~CMarkerDisplay()
{
}


int CMarkerDisplay::SetupWebCam(const char* cparam_names, char* vconfs)
{
	ARTCparam.xsize = 640;
	ARTCparam.ysize = 480;

	LoadIdentity(projectionMat);
	projectionMat[0] = (double) ARTCparam.ysize / (double) ARTCparam.xsize;

	return(1);
}


void CMarkerDisplay::Display(ARGL_CONTEXT_SETTINGS_REF arglSettings)
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(projectionMat);

	this->Draw();
}


void CMarkerDisplay::Draw(void)
{
	double hWidth = 0.5;

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glLineWidth(1);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	DrawQuad(hWidth, 0.0, 0.0);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	DrawQuad(hWidth * 0.5, 0.0, 0.0);

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	DrawQuad(hWidth * 0.25, 0.125, -0.125);

}


void CMarkerDisplay::DrawQuad(double width, double x, double y)
{
	glBegin(GL_QUADS);
		glVertex2d( width + x,  width + y);
		glVertex2d( width + x, -width + y);
		glVertex2d(-width + x, -width + y);
		glVertex2d(-width + x,  width + y);
	glEnd();
}

