#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif
#include "CMarkerButton.h"



CMarkerButton::CMarkerButton(void)
: CMarkerObject()
{
	drawCard	= false;	//ÉJÅ[É\ÉãÇ…
	prevVisible	= false;
}


CMarkerButton::~CMarkerButton()
{
}


void CMarkerButton::DrawShape(void)
{
	glColor4f(1.0, 0.0, 0.0, 0.5);
	glBegin(GL_POLYGON);
		glVertex3d(cardRect[0][0] + markerCenter[0], cardRect[0][1] + markerCenter[1], 1.0);
		glVertex3d(cardRect[1][0] + markerCenter[0], cardRect[1][1] + markerCenter[1], 1.0);
		glVertex3d(cardRect[2][0] + markerCenter[0], cardRect[2][1] + markerCenter[1], 1.0);
		glVertex3d(cardRect[3][0] + markerCenter[0], cardRect[3][1] + markerCenter[1], 1.0);
	glEnd();
}


int CMarkerButton::CheckButton(void)
{
	int click = (!prevVisible && visible || prevVisible && !visible);

	prevVisible = visible;

	return(click);
}


