#include "CVirtualWebCam.h"

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include <math.h>

#include "CFrameCube.h"
#include "CARDeskTop.h"
#include "CWindow.h"



CFrameCube::CFrameCube(void)
: CCube()
{
	drawTransparentCard = true;
}


CFrameCube::~CFrameCube()
{
}


void CFrameCube::DrawShape(void)
{
	if(!wire && GetAlpha() < 1.0f && drawTransparentCard) {
		double m[16], k = 1.0;

		glMatrixMode(GL_MODELVIEW);
		glGetDoublev(GL_MODELVIEW_MATRIX, m);

		if(CARDeskTop::mode == CARDeskTop::kAR) {
			if((m[8] * m[12] + m[9] * m[13] + m[10] * m[14]) /
				sqrt((m[8] * m[8] + m[9] * m[9] + m[10] * m[10]) * (m[12] * m[12] + m[13] * m[13] + m[14] * m[14])) < 0.0) k = -1.0;
		} else if(CARDeskTop::mode == CARDeskTop::kVR || CARDeskTop::mode == CARDeskTop::kVR2) {
			CVirtualWebCam* vWebCam = (CVirtualWebCam*) CARDeskTop::arDeskTop->window->webCamList[0];
			if((vWebCam->newPos[0] * m[8] + vWebCam->newPos[1] * m[9] + vWebCam->newPos[2] * m[10]) > 0.0) k = -1.0;
		}

		glPushMatrix();
			glDepthMask(GL_TRUE);
			glBlendFunc(GL_ZERO, GL_ONE);

			if(card) {
				glTranslated(0.0, 0.0, 0.05 * k);
				DrawCard(cardMode, false);
			} else {
				glTranslated(0.0, 0.0, 0.55 * k);
				DrawCard(kRect, false);
			}

			glDepthMask(GL_FALSE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPopMatrix();
	}

	CCube::DrawShape();

/*

	if(!wire && GetAlpha() < 1.0f && drawTransparentCard) {
		glDepthMask(GL_TRUE);
		glBlendFunc(GL_ZERO, GL_ONE);

		glEnable(GL_CULL_FACE);

		glPushMatrix();
			if(card) {
				glTranslated(0.0, 0.0, 0.05);
				glCullFace(GL_BACK);
				DrawCard(cardMode, false);

				glTranslated(0.0, 0.0, -0.1);
				glCullFace(GL_FRONT);
				DrawCard(cardMode, false);
			} else {
				glTranslated(0.0, 0.0, 0.55);
				glCullFace(GL_BACK);
				DrawCard(kRect, false);

				glTranslated(0.0, 0.0, -1.1);
				glCullFace(GL_FRONT);
				DrawCard(kRect, false);
			}
		glPopMatrix();

		glDisable(GL_CULL_FACE);

		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	CCube::DrawShape();
*/
}

