#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CBoundingBox.h"


CBoundingBox::CBoundingBox(void)
: CPrimitive()
{
}


CBoundingBox::~CBoundingBox()
{
}


void CBoundingBox::DrawShape(void)
{
	glLineWidth(4.0);
	glutWireCube(1.0);
}


