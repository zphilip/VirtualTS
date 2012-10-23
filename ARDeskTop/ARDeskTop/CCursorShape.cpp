#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include <math.h>
#include "CCursorShape.h"


CCursorShape::CCursorShape(void)
: CComponent3D()
{
}


CCursorShape::~CCursorShape()
{
}


void CCursorShape::Init(void)
{
	CComponent3D::Init();

	SetColor4(0.0f, 0.0f, 1.0f, 0.7f);
}


void CCursorShape::DrawShape(void)
{
	glNormal3d(0.0, 0.0, 1.0);
	glBegin(GL_TRIANGLES);
		glVertex3dv(pos1);
		glVertex3dv(pos2);
		glVertex3dv(pos3);
	glEnd();
}


void CCursorShape::SetWidth(double w)
{
	pos1[0] = 0.0;		pos1[1] = 0.0;		pos1[2] = 1.0; 
	pos2[0] =  w * 0.5;	pos2[1] = -w;		pos2[2] = 1.0; 
	pos3[0] = w;		pos3[1] = -w * 0.5;	pos3[2] = 1.0; 
}
