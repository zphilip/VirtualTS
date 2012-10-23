#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include <stdio.h>

#include "CLine3D.h"
#include "CList.h"
#include "CVertex.h"
#include "CBoundingBox.h"
#include "CComponent3D.h"
#include "CMainCursor.h"
#include "CUser.h"


CLine3D::CLine3D(void)
: CComponent3D()
{
	lineType	= GL_LINE_STRIP;
	lineWidth	= 1.0;
}


CLine3D::~CLine3D()
{
	vertexList.DeleteAll();
	if(boundingBox) delete boundingBox;
}


void CLine3D::Init(void)
{
	CComponent3D::Init();
	SetColor3(1.0f, 1.0f, 0.0f);
	SetLineWidth(4.0);

	drawBB = false;
}


void CLine3D::DrawShape(void)
{
	if(!touchable) return;

	int			degree = vertexList.GetDegree();
	CVertex*	vertex;

	if(degree == 0) return;

	glLineWidth(lineWidth);
	glBegin(lineType);
		glNormal3d(0.0, 0.0, 1.0);
		vertexList.GoToFirst();
		for(int i = 0; i < degree; i++) {
			vertex = (CVertex*) vertexList.GetKey();
			glVertex3d(vertex->v[0], vertex->v[1], vertex->v[2]);
			vertexList.GoToNext();
		}
	glEnd();
}


void CLine3D::Click(void)
{
	if(clickedCursor) {
		if(CUser::cursorMode == CUser::kDraw) {
			Bound();
			changed = true;
			clickedCursor->selectObject = 0; // unselect object;
			clickedCursor = 0;
		} else
			CComponent3D::Click();
	}
}


void CLine3D::AddVertex(CVertex* vertex)
{
	vertexList.Push(vertex);
}


void CLine3D::Bound(void)
{
	int			i, degree = vertexList.GetDegree();
		
	if(degree == 0) return;

	CVertex*	vertex;
	double		maxPos[3], minPos[3];

	vertexList.GoToFirst();
	vertex = (CVertex*) vertexList.GetKey();
	maxPos[0] = vertex->v[0];	maxPos[1] = vertex->v[1];	maxPos[2] = vertex->v[2];
	minPos[0] = vertex->v[0];	minPos[1] = vertex->v[1];	minPos[2] = vertex->v[2];

	vertexList.GoToNext();
	for(i = 1; i < degree; i++) {
		vertex = (CVertex*) vertexList.GetKey();
		
		if(vertex->v[0] > maxPos[0]) maxPos[0] = vertex->v[0];
		if(vertex->v[1] > maxPos[1]) maxPos[1] = vertex->v[1];
		if(vertex->v[2] > maxPos[2]) maxPos[2] = vertex->v[2];

		if(vertex->v[0] < minPos[0]) minPos[0] = vertex->v[0];
		if(vertex->v[1] < minPos[1]) minPos[1] = vertex->v[1];
		if(vertex->v[2] < minPos[2]) minPos[2] = vertex->v[2];

		vertexList.GoToNext();
	}

	double width, height, depth;

//	if(boundingBox) delete boundingBox;
	boundingBox = new CBoundingBox();
	boundingBox->Init();

	width	= maxPos[0] - minPos[0];
	height	= maxPos[1] - minPos[1];
	depth	= maxPos[2] - minPos[2];
	boundingBox->SetSize(width, height, depth);

	width	= width * 0.5 + minPos[0];
	height	= height * 0.5 + minPos[1];
	depth	= depth * 0.5 + minPos[2];
	this->SetPosition(width, height, depth);

	vertexList.GoToFirst();
	for(i = 0; i < degree; i++) {
		vertex = (CVertex*) vertexList.GetKey();
		vertex->v[0] -= width;
		vertex->v[1] -= height;
		vertex->v[2] -= depth;
		vertexList.GoToNext();
	}

	drawBB = true;
}


void CLine3D::SetLineWidth(float pLineWidth)
{
	lineWidth = pLineWidth;
}


