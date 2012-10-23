#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include <stdio.h>
#include <math.h>

#include "CSubCursor.h"
#include "CCursor.h"
#include "CMainCursor.h"
#include "CComponent3D.h"

#include "Color.h"
#include "PI.h"



CSubCursor::CSubCursor(void)
: CCursor()
{
	mainCursor		= 0;
	collide			= false;
	prevCollide		= false;
}


CSubCursor::~CSubCursor()
{
}


void CSubCursor::Init(void)
{
	CCursor::Init();

	cursorCollide = CURSOR_COLLIDE * CURSOR_COLLIDE;
}


void CSubCursor::Precompute(int id)
{
	CCursor::Precompute(id);
	return;


	if(!mainCursor) return;

	if(!mainCursor->selectObject && mainCursor->visible) CCursor::Precompute(id);
}


int CSubCursor::CheckClick(void)
{
	if(!mainCursor) return(0);
	if(!mainCursor->visible) return(0);

	double dist[3];

	dist[0] = mainCursor->trans[0][3] - trans[0][3];
	dist[1] = mainCursor->trans[1][3] - trans[1][3];
	dist[2] = mainCursor->trans[2][3] - trans[2][3];

	double cosaX, angleX;
	cosaX	= mainCursor->trans[0][0] * trans[0][0] + mainCursor->trans[1][0] * trans[1][0] + mainCursor->trans[2][0] * trans[2][0];
	angleX	= acos(cosaX) * 180.0 / PI;

/*	double cosaY, angleY;
	cosaY	= mainCursor->trans[0][1] * trans[0][1] + mainCursor->trans[1][1] * trans[1][1] + mainCursor->trans[2][1] * trans[2][1];
	angleY	= acos(cosaY) * 180.0 / PI;
*/

	prevCollide = collide;
//	printf("X:%f, Y:%f\n", angleX, angleY);

	if(prevCollide) //—£‚ê‚É‚­‚­‚·‚é
		collide = (angleX <= 15.0) || (dist[0]*dist[0] + dist[1]*dist[1] + dist[2]*dist[2] <= 30.0 * 30.0); //18.0 32.0
	else
		collide = (angleX <= 10.0) && (dist[0]*dist[0] + dist[1]*dist[1] + dist[2]*dist[2] <= 20.0 * 20.0);

	if(collide && !prevCollide)
		shape->SetColor3v(Color::red);
	else if(!collide && prevCollide)
		shape->SetColor3v(Color::blue);

	return(collide && !prevCollide || !collide && prevCollide);
}

