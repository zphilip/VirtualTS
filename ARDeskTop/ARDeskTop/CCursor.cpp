#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include <AR/ar.h>

#include "CCursor.h"
#include "CMainCursor.h"
#include "CDesktop.h"
#include "CComponent3D.h"
#include "CClickInfo.h"
#include "CProjection.h"
#include "CMarkerButton.h"
#include "CCursorShape.h"
#include "CUser.h"
#include "CARDeskTop.h"


int				CCursor::projectionNum	= 0;
int				CCursor::cursorNum		= 0;
int				CCursor::buttonNum		= 0;
CProjection**	CCursor::projectionList	= 0;
CCursor**		CCursor::cursorList		= 0;
CMarkerButton** CCursor::buttonList		= 0;



CCursor::CCursor(void)
: CMarkerObject()
{
	prevWithinObject	= 0;
	withinObject		= 0;
	lock				= false;
}


CCursor::~CCursor()
{
	if(prevWithinObject)
		delete prevWithinObject;

	if(withinObject)
		delete withinObject;
}


void CCursor::Init(void)
{
	CMarkerObject::Init();

	prevWithinObject	= new CComponent3D*[projectionNum];
	withinObject		= new CComponent3D*[projectionNum];
	for(int i = 0; i < projectionNum; i++) {
		prevWithinObject[i]	= 0;
		withinObject[i]		= 0;
	}

	double cardWidth;
	int rot;

	cardWidth = cardRect[0][0] - cardRect[3][0];
	if(cardRect[0][1] - cardRect[2][1] < cardWidth)
		cardWidth = cardRect[0][1] - cardRect[2][1];

	if(markerCenter[0] > 0.0) {
		if(markerCenter[1] > 0.0)
			rot = 3;
		else
			rot = 0;
	} else {
		if(markerCenter[1] > 0.0)
			rot = 2;
		else
			rot = 1;
	}

	CCursorShape* cursorShape = new CCursorShape();
	cursorShape->SetWidth(cardWidth);
	cursorShape->Init();
	if(rot) cursorShape->Rotate(-90.0 * rot, 0.0, 0.0, 1.0);

	shape = cursorShape;
}


void CCursor::Precompute(void)
{
	CMarkerObject::Precompute();
}


void CCursor::Draw2(void)
{
	if(!visible) return;
	glPushMatrix();
		shape->SetColor();
		glTranslated(cursorPos[0], cursorPos[1], cursorPos[2]);
		glutSolidSphere(5.0, 12, 12);
	glPopMatrix();
}


int CCursor::Click(void)
{
	return(0);
}


int CCursor::CheckClick(void)
{
	return(0);
}


void CCursor::Precompute(int id)
{
	if(!visible || !projectionList[id]->visible || lock) return;

	projectionList[id]->ProjectCursorPos(this, cursorPos);
	prevWithinObject[id]	= withinObject[id];
	withinObject[id]		= projectionList[id]->IsWithin(cursorPos[0], cursorPos[1], cursorPos[2]);

	if(prevWithinObject[id]) {
		if(prevWithinObject[id] == withinObject[id]) {
			withinObject[id]->CursorWithin(this);
		} else {
			prevWithinObject[id]->CursorLeave(this);
			if(withinObject[id])
				withinObject[id]->CursorEnter(this);
		}
	} else {
		if(withinObject[id]) {
			withinObject[id]->CursorEnter(this);
		} else {
		//Without;
		}
	}
}


void CCursor::ProjectCursorPos(CProjection* pro, double pos[3])
{
}


CCursor* CCursor::FindWithinObject(CComponent3D* object)
{
	if(!visible) return(0);

	for(int i = 0; i < projectionNum; i++) {
		if(withinObject[i] == object) return(this);
	}

	return(0);
}


CCursor* CCursor::FindWithinObject(CComponent3D* object, int index)
{
	return(visible && withinObject[index] == object ? this : 0);
}


bool CCursor::IsWithinObject(void)
{
	for(int i = 0; i < projectionNum; i++) {
		if(withinObject[i]) return(true);
	}

	return(false);
}


void CCursor::ResetWithinObject(void)
{
	for(int i = 0; i < CCursor::projectionNum; i++)
		withinObject[i] = 0;
}


void CCursor::CheckAllWithin(void)
{
	for(int i = 0; i < cursorNum; i++) {
		for(int j = 0; j < projectionNum; j++) {
			cursorList[i]->Precompute(j);
		}
	}
}

/*
void CCursor::CheckAllClick(void)
{
	if(cursorNum == 1) return;

	for(int i = 0; i < cursorNum; i++) {
		cursorList[i]->CheckClick();
	}
}*/


void CCursor::CheckAllButton(void)
{
	for(int i = 0; i < buttonNum; i++) {
		buttonList[i]->CheckButton();
	}
}


void CCursor::FixAll(void)
{
	for(int i = 0; i < projectionNum; i++) {
		if(projectionList[i]->visible && !projectionList[i]->fix) projectionList[i]->FlipFix();
	}
}

