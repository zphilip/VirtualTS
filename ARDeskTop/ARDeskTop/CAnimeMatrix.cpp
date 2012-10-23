#include <stdio.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CAnimeMatrix.h"
#include "CComponent3D.h"



CAnimeMatrix::CAnimeMatrix(CComponent3D* pComponent)
: CAnimeManager(pComponent)
{
}


CAnimeMatrix::~CAnimeMatrix()
{
}


void CAnimeMatrix::Init(void)
{
	if(!component) return;

	CAnimeManager::Init();

	for(int i = 0; i < 16; i++)
		matrix[i] = component->matrix[i];

	pos[0]		= matrix[12];
	pos[1]		= matrix[13];
	pos[2]		= matrix[14];

	matrix[12]	= 0.0;
	matrix[13]	= 0.0;
	matrix[14]	= 0.0;

	SetVector(0.0, 0.0, 0.0);
}


int CAnimeMatrix::Animate(void)
{
	if(!CAnimeManager::Animate()) return(0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();
		glTranslated(
			pos[0] + elapsedTimeMS * vec[0],
			pos[1] + elapsedTimeMS * vec[1],
			pos[2] + elapsedTimeMS * vec[2]
		);

		PreAnimate();	// rotate + translate
		glMultMatrixd(matrix);
		PostAnimate();	// scale

		glGetDoublev(GL_MODELVIEW_MATRIX, component->matrix);
		component->uniMatrix = false;
	//	component->changed = true;
	glPopMatrix();

	return(1);
}


void CAnimeMatrix::PreAnimate(void)
{
}


void CAnimeMatrix::PostAnimate(void)
{
}


void CAnimeMatrix::SetVector(double x, double y, double z)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}




CAnimeRotate::CAnimeRotate(CComponent3D* pComponent)
: CAnimeMatrix(pComponent)
{
}


CAnimeRotate::~CAnimeRotate()
{
}


void CAnimeRotate::PreAnimate(void)
{
	CAnimeMatrix::PreAnimate();

	double angle = elapsedTimeMS * aps;
	int anglei = angle / 360.0;

	if(anglei) angle -= anglei * 360.0;

	glRotated(angle, axis[0], axis[1], axis[2]);
}


void CAnimeRotate::SetAxis(double x, double y, double z)
{
	axis[0] = x;
	axis[1] = y;
	axis[2] = z;
}
