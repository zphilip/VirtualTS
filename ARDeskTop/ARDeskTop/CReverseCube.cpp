#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include <stdio.h>

#include "CReverseCube.h"


CVertex		CReverseCube::vertex[8];
int			CReverseCube::face[6][4];
CVertex		CReverseCube::normal[6];




CReverseCube::CReverseCube(void)
: CComponent3D()
{
	hole		= 0.6;
	lighting	= true;
}


CReverseCube::~CReverseCube()
{
}


void CReverseCube::DrawShape(void)
{
	int i;

	if(!lighting)
		glDisable(GL_LIGHTING);

	glEnable(GL_CULL_FACE);

	glDepthMask(GL_TRUE);

	glBlendFunc(GL_ZERO, GL_ONE);
	glCullFace(GL_FRONT);

	for(i = 1; i < 6; i++) DrawFace(i, 1.0);
	DrawHoleFace(1.0);


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glCullFace(GL_BACK);

	if(GetAlpha() < 1.0f)
		glDepthMask(GL_FALSE);

	for(i = 1; i < 6; i++) DrawFace(i, -1.0);
	DrawHoleFace(-1.0);

	glDisable(GL_CULL_FACE);

	if(!lighting)
		glEnable(GL_LIGHTING);
}


void CReverseCube::DrawFace(int i, double n)
{
	glNormal3d(n * normal[i].v[0], n * normal[i].v[1], n * normal[i].v[2]);

	glBegin(GL_POLYGON);
	for(int j = 0; j < 4; j++) {
		glVertex3d(vertex[face[i][j]].v[0], vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);
	}
	glEnd();
}


void CReverseCube::DrawHoleFace(double n)
{
	if(hole <= 0.0 || 1.0 <= hole) return;

	double	hole2 = hole * 0.5;
	int		i = 0, j;

	glNormal3d(normal[i].v[0], normal[i].v[1], normal[i].v[2]);

	glBegin(GL_QUADS);
		j = 0;
		glVertex3d(vertex[face[i][j]].v[0], vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);
		j = 1;
		glVertex3d(vertex[face[i][j]].v[0], vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);
		glVertex3d(hole2 * vertex[face[i][j]].v[0], vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);
		j = 0;
		glVertex3d(hole2 * vertex[face[i][j]].v[0], vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);

		j = 2;
		glVertex3d(vertex[face[i][j]].v[0], vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);
		j = 3;
		glVertex3d(vertex[face[i][j]].v[0], vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);
		glVertex3d(hole2 * vertex[face[i][j]].v[0], vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);
		j = 2;
		glVertex3d(hole2 * vertex[face[i][j]].v[0], vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);

		j = 1;
		glVertex3d(hole2 * vertex[face[i][j]].v[0], hole2 * vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);
		glVertex3d(hole2 * vertex[face[i][j]].v[0], vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);
		j = 2;
		glVertex3d(hole2 * vertex[face[i][j]].v[0], vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);
		glVertex3d(hole2 * vertex[face[i][j]].v[0], hole2 * vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);

		j = 3;
		glVertex3d(hole2 * vertex[face[i][j]].v[0], hole2 * vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);
		glVertex3d(hole2 * vertex[face[i][j]].v[0], vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);
		j = 0;
		glVertex3d(hole2 * vertex[face[i][j]].v[0], vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);
		glVertex3d(hole2 * vertex[face[i][j]].v[0], hole2 * vertex[face[i][j]].v[1], vertex[face[i][j]].v[2]);		
	glEnd();
}


void CReverseCube::InitReverseCube(void)
{
	double x = 0.5, y = 0.5, z = 0.5;
	int i;

	//頂点上段
	i = 0;
	vertex[i].v[0] =  x;
	vertex[i].v[1] =  y;
	vertex[i].v[2] =  z;

	i++;
	vertex[i].v[0] =  x;
	vertex[i].v[1] = -y;
	vertex[i].v[2] =  z;

	i++;
	vertex[i].v[0] = -x;
	vertex[i].v[1] = -y;
	vertex[i].v[2] =  z;

	i++;
	vertex[i].v[0] = -x;
	vertex[i].v[1] =  y;
	vertex[i].v[2] =  z;


	//頂点下段
	z *= -1.0;

	i++;
	vertex[i].v[0] =  x;
	vertex[i].v[1] =  y;
	vertex[i].v[2] =  z;

	i++;
	vertex[i].v[0] =  x;
	vertex[i].v[1] = -y;
	vertex[i].v[2] =  z;

	i++;
	vertex[i].v[0] = -x;
	vertex[i].v[1] = -y;
	vertex[i].v[2] =  z;

	i++;
	vertex[i].v[0] = -x;
	vertex[i].v[1] =  y;
	vertex[i].v[2] =  z;


	//面
	i = 0;
	face[i][0] = 0;
	face[i][1] = 1;
	face[i][2] = 2;
	face[i][3] = 3;

	i++;
	face[i][0] = 3;
	face[i][1] = 7;
	face[i][2] = 4;
	face[i][3] = 0;

	i++;
	face[i][0] = 0;
	face[i][1] = 4;
	face[i][2] = 5;
	face[i][3] = 1;

	i++;
	face[i][0] = 1;
	face[i][1] = 5;
	face[i][2] = 6;
	face[i][3] = 2;

	i++;
	face[i][0] = 2;
	face[i][1] = 6;
	face[i][2] = 7;
	face[i][3] = 3;

	i++;
	face[i][0] = 7;
	face[i][1] = 6;
	face[i][2] = 5;
	face[i][3] = 4;

	//法線
	i = 0;
	normal[i].v[0] =  0.0;
	normal[i].v[1] =  0.0;
	normal[i].v[2] =  1.0;

	i++;
	normal[i].v[0] =  0.0;
	normal[i].v[1] =  1.0;
	normal[i].v[2] =  0.0;

	i++;
	normal[i].v[0] =  1.0;
	normal[i].v[1] =  0.0;
	normal[i].v[2] =  0.0;

	i++;
	normal[i].v[0] =  0.0;
	normal[i].v[1] = -1.0;
	normal[i].v[2] =  0.0;

	i++;
	normal[i].v[0] = -1.0;
	normal[i].v[1] =  0.0;
	normal[i].v[2] =  0.0;

	i++;
	normal[i].v[0] =  0.0;
	normal[i].v[1] =  0.0;
	normal[i].v[2] = -1.0;
}

