#include <stdio.h>
#include <stdlib.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CHTWebCam.h"


CHTWebCam::CHTWebCam(void)
: CSimpleWebCam()
{
}


CHTWebCam::~CHTWebCam()
{
}


void CHTWebCam::Display(ARGL_CONTEXT_SETTINGS_REF arglSettings)
{
	CSimpleWebCam::Display(arglSettings);

	double lineLength = 0.1;

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glLineWidth(1);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex3d( lineLength, 0.0, -1.0);
		glVertex3d(-lineLength, 0.0, -1.0);
	glEnd();
	glBegin(GL_LINES);
		glVertex3d( 0.0,  lineLength, -1.0);
		glVertex3d( 0.0, -lineLength, -1.0);
	glEnd();
}

