#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CLight.h"


CLight::CLight(void)
: CPrimitive()
{
}


CLight::~CLight()
{
	glDisable(lightNo);
}


void CLight::Init(void)
{
	CPrimitive::Init();

	CPrimitive::SetColor3(1.0f, 1.0f, 1.0f);
//	SetAlpha(0.7);

	lightNo = GL_LIGHT0;
	SetColor(matAmbDiff, 0.9, 0.9, 0.0, 1.0);
	SetColor(matSpecular, 0.5, 0.5, 0.5, 1.0);
	matShininess = 10.0;
	SetColor(lightAmbient, 0.01, 0.01, 0.01, 1.0);
	SetColor(lightDiffuse, 1.0, 1.0, 1.0, 1.0);
	SetColor(lightSpecular, 1.0, 1.0, 1.0, 1.0);
}


void CLight::DrawShape(void)
{
	glEnable(lightNo);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matAmbDiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, &matShininess);	
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
//	glEnable(GL_COLOR_MATERIAL);

	glLightfv(lightNo, GL_AMBIENT, lightAmbient);
//	glLightfv(lightNo, GL_DIFFUSE, lightDiffuse);
	glLightfv(lightNo, GL_DIFFUSE, currentColor);
	glLightfv(lightNo, GL_SPECULAR, currentColor);
//	glLightfv(lightNo, GL_SPECULAR, lightSpecular);

	GLfloat lightPosition[] = {0.0, 0.0, 0.0, 1.0};
	glLightfv(lightNo, GL_POSITION, lightPosition);

	glPushMatrix();
		glLineWidth(3.0);
		glScaled(0.5, 0.5, 0.5);
		glutSolidOctahedron();
	glPopMatrix();
}


void CLight::SetColor(GLfloat color[4], GLfloat r,  GLfloat g,  GLfloat b,  GLfloat a)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}


