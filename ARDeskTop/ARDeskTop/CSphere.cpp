#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif
#include "CSphere.h"


CSphere::CSphere(void)
: CComponent3D()
{
}


CSphere::~CSphere()
{
}


void CSphere::Init(void)
{
	CComponent3D::Init();
}


void CSphere::DrawShape(void)
{
	glutSolidSphere(0.5, 12, 12);
}
