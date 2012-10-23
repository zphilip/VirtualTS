#include <stdlib.h>
#include <math.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CCube.h"
#include "PI.h"

int		CCube::divNum	= 8;
double	CCube::roundPos[50][2];
double	CCube::sqrt2;



CCube::CCube(void)
: CPrimitive()
{
	cardMode	= kRect;
	wire		= false;

	sbIndex		= 0;
	sbPos[0]	= 1.0;
	sbPos[1]	= 1.0;
}


CCube::~CCube()
{
}


void CCube::DrawShape(void)
{
	if(card) {
		DrawCard(cardMode, wire);
	} else {
		glutSolidCube(1.0);
	}
}


void CCube::InitCube()
{
	double rad, radn = PI / (divNum * 2.0);

	for(int i = 0; i < divNum * 4; i++) {
		rad = radn * i;
		roundPos[i][0] = cos(rad);
		roundPos[i][1] = sin(rad);
	}

	sqrt2	= sqrt(2.0);
}


void CCube::DrawCard(int pCardMode, bool pWire)
{
	int i;
	double r;

	glNormal3d(0.0, 0.0, 1.0);

	if(pWire) {
		glLineWidth(4.0);
		if(pCardMode == kSpeechBaloon2)
			glBegin(GL_LINE_STRIP);
		else
			glBegin(GL_LINE_LOOP);
	} else {
		glBegin(GL_POLYGON);
	}

	if(pCardMode == kRect) {
		glVertex3d( 0.5,  0.5, 0.0);
		glVertex3d( 0.5, -0.5, 0.0);
		glVertex3d(-0.5, -0.5, 0.0);
		glVertex3d(-0.5,  0.5, 0.0);
	} else if(pCardMode == kRound) {
		double wRatio = 1.0, hRatio = 1.0;
		double x, y;

		r = 0.2;
		if(width > height) wRatio = height / width;
		else hRatio = width / height;

		x = ((1.0 - 2.0 * r) + 2.0 * r * (1.0 - wRatio)) * 0.5;
		y = ((1.0 - 2.0 * r) + 2.0 * r * (1.0 - hRatio)) * 0.5;

		wRatio *= r;
		hRatio *= r;

		//âEè„
		for(i = 0; i <= divNum; i++) {
			glVertex3d(wRatio * roundPos[i][0] + x, hRatio * roundPos[i][1] + y, 0.0);
		}

		//ç∂è„
		for(i--; i <= divNum * 2; i++) {
			glVertex3d(wRatio * roundPos[i][0] - x, hRatio * roundPos[i][1] + y, 0.0);
		}

		//ç∂â∫
		for(i--; i <= divNum * 3; i++) {
			glVertex3d(wRatio * roundPos[i][0] - x, hRatio * roundPos[i][1] - y, 0.0);
		}

		//âEâ∫
		for(i--; i < divNum * 4; i++) {
			glVertex3d(wRatio * roundPos[i][0] + x, hRatio * roundPos[i][1] - y, 0.0);
		}
	} else if(pCardMode == kCircle1 || pCardMode == kCircle2) {
		if(pCardMode == kCircle1) r = 0.5;
		else r = sqrt2 * 0.5;

		for(i = 0; i < divNum * 4; i++) {
			glVertex3d(r * roundPos[i][0], r * roundPos[i][1], 0.0);
		}
	} else if(pCardMode == kSpeechBaloon1) {
		r = sqrt2 * 0.5;

		for(i = 0; i < divNum * 4; i++) {
			glVertex3d(r * roundPos[i][0], r * roundPos[i][1], 0.0);
			if(i == sbIndex) {
				glVertex3d(sbPos[0], sbPos[1], 0.0);
			}
		}
	} else if(pCardMode == kSpeechBaloon2) {
		glVertex3d( 0.5, -0.5, 0.0);
		glVertex3d(-0.5, -0.5, 0.0);
		glVertex3d(sbPos[0], sbPos[1], 0.0);
	}

	glEnd();
}


