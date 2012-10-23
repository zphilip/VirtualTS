#include <stdio.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CWall.h"
#include "CBoundingBox.h"
#include "CUser.h"
#include "color.h"

CWall* CWall::wall = 0;



CWall::CWall(void)
: CGroup()
{
}


void CWall::Init(void)
{
	CGroup::Init();

	SetCursorMode(CUser::kTranslate);
}


void CWall::Draw(void)
{
	glDisable(GL_LIGHTING);
	CGroup::Draw();
	glEnable(GL_LIGHTING);
}


void CWall::SetCursorMode(int mode)
{
}




CWall2::CWall2(void)
: CWall()
{
}


CWall2::~CWall2()
{
}


void CWall2::Init(void)
{
	CWall::Init();

	boundingBox = new CBoundingBox();
	boundingBox->Init();
	boundingBox->touchable = false;
	boundingBox->SetColor4va(Color::passionorange, 1.0f);
	AddObject(boundingBox);

}


void CWall2::SetSize(double pWidth, double pHeight, double pDepth)
{
	CGroup::SetSize(pWidth, pHeight, pDepth);

	boundingBox->SetSize(width, height, depth);
	boundingBox->SetPosition(0.0, 0.0, depth * 0.5);
}






void CGrid::DrawShape(void)
{
	double i, width2, height2;

	width2	= ((int) (width * 0.5 / gridSize)) * gridSize;
	height2	= ((int) (height * 0.5 / gridSize)) * gridSize;

	glLineWidth(1.0);
	glNormal3d(0.0, 0.0, 1.0);

	for(i = -width2; i <= width2; i += gridSize) {
		glBegin(GL_LINES);
			glVertex3d(i, -height2, 0.0);
			glVertex3d(i,  height2, 0.0);
		glEnd();
	}

	for(i = -height2; i <= height2; i += gridSize) {
		glBegin(GL_LINES);
			glVertex3d(-width2, i, 0.0);
			glVertex3d( width2, i, 0.0);
		glEnd();
	}
}

/*
void CGrid::DrawShape(void)
{
	double i, width2, height2;

	width2	= width * 0.5;
	height2	= height * 0.5;

	glLineWidth(1.0);
	glNormal3d(0.0, 0.0, 1.0);

	for(i = -width2; i <= width2; i += gridSize) {
		glBegin(GL_LINES);
			glVertex3d(i, -height2, 0.0);
			glVertex3d(i,  height2, 0.0);
		glEnd();
	}

	if(i != width2) {
		glBegin(GL_LINES);
			glVertex3d(width2, -height2, 0.0);
			glVertex3d(width2,  height2, 0.0);
		glEnd();
	}


	for(i = -height2; i <= height2; i += gridSize) {
		glBegin(GL_LINES);
			glVertex3d(-width2, i, 0.0);
			glVertex3d( width2, i, 0.0);
		glEnd();
	}

	if(i != height2) {
		glBegin(GL_LINES);
			glVertex3d(-width2, height2, 0.0);
			glVertex3d( width2, height2, 0.0);
		glEnd();
	}
}
*/

void CGrid::SetGridSize(double pGridSize)
{
	gridSize	= pGridSize;
}




void CWall3::Init(void)
{
	CWall::Init();

	grid	= new CGrid();
	grid->Init();
	grid->SetColor4va(Color::green, 1.0f);
	grid->SetGridSize(20.0);

	AddObject(grid);
}


void CWall3::SetSize(double pWidth, double pHeight, double pDepth)
{
	CWall::SetSize(pWidth, pHeight, pDepth);

	grid->SetPosition(0.0, 0.0, 0.0);
	grid->SetSize(pWidth, pHeight, pDepth);
}


