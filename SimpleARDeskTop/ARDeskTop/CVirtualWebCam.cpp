#include "CVirtualWebCam.h"

#include <stdio.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CARDeskTop.h"
#include "CWindow.h"
#include "CCursor.h"
#include "CList.h"
#include "CMarkerObject.h"
#include "CHeadTrackingMarker.h"
#include "CARDeskTop.h"
#include "CTrash.h"
#include "CDesktop.h"
#include "CMainWebCam.h"
#include "CWall.h"



CVirtualWebCam::CVirtualWebCam(void)
: CWebCam()
{
}


CVirtualWebCam::~CVirtualWebCam()
{
}


int CVirtualWebCam::SetupWebCam(const char* cparam_names, char* vconfs)
{
	ARTCparam.xsize = 640;
	ARTCparam.ysize = 480;

	zNear	= 30.0;
	zFar	= 100000.0;

	return(1);
}


void CVirtualWebCam::Display(ARGL_CONTEXT_SETTINGS_REF arglSettings)
{
	if(!CHeadTrackingMarker::HTMarker) {
		return;
	}

	if(CHeadTrackingMarker::HTMarker->visible) {
		double pos[3];

		pos[0] = CHeadTrackingMarker::HTMarker->trans[0][3] - CDesktop::desktop->trans[0][3];
		pos[1] = CHeadTrackingMarker::HTMarker->trans[1][3] - CDesktop::desktop->trans[1][3];
		pos[2] = CHeadTrackingMarker::HTMarker->trans[2][3] - CDesktop::desktop->trans[2][3];

		CDesktop::desktop->ProjectPos(pos, newPos);

		double left, right, top, bottom, k;

		k = zNear / newPos[2];

		double width2	= width * 0.5;
		double height2	= height * 0.5;

		left	= (-width2  - newPos[0]) * k;
		right	= ( width2  - newPos[0]) * k;
		bottom	= (-height2 - newPos[1]) * k;
		top		= ( height2 - newPos[1]) * k;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glFrustum(left, right, bottom, top, zNear, zFar);

		this->Draw();
	}
}


void CVirtualWebCam::Reshape(int currentWidth, int currentHeight)
{
	width	= CARDeskTop::arDeskTop->window->pixelPitch * currentWidth;
	height	= CARDeskTop::arDeskTop->window->pixelPitch * currentHeight;

	if(CWall::wall) {
		if(CARDeskTop::mode == CARDeskTop::kVR) {
			CWall::wall->SetSize(width, height, 300.0);
			CWall::wall->SetPosition(0.0, 0.0, -300.0 - 1.0);
		} else if(CARDeskTop::mode == CARDeskTop::kVR2) {
			CWall::wall->SetSize(width, height, 0.0);
			CWall::wall->SetPosition(0.0, 0.0, 0.0);
		}
	}
}


void CVirtualWebCam::Draw(void)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_COLOR_MATERIAL);

	CARDeskTop::arDeskTop->CheckClick();

	if(CWall::wall) CWall::wall->Precompute();
	CMainWebCam::markerObjectList.Precompute();

	CCursor::CheckAllWithin();
	CDesktop::desktop->SortByDepth();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(-newPos[0], -newPos[1], -newPos[2]);

	if(CWall::wall) CWall::wall->Draw();
	CMainWebCam::markerObjectList.DrawMarkerObject2();

	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_BLEND);
	glDisable(GL_NORMALIZE);
	glDisable(GL_DEPTH_TEST);

	CTrash::Erase();
}


void CVirtualWebCam::Reshape2(int currentWidth, int currentHeight)
{
	width	= currentWidth;
	height	= currentHeight;

	if(CWall::wall) {
		CWall::wall->SetSize(width, height, 300.0);
		CWall::wall->SetPosition(0.0, 0.0, -300.0 - 1.0);
	}
}

