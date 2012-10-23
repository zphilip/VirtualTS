#include <stdio.h>
#include <stdlib.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CARDTWindow.h"
#include "CARDeskTop.h"
#include "CWebCam.h"
#include "CMainWebCam.h"
#include "CHTWebCam.h"
#include "CHeadTrackingMarker.h"
#include "CARDeskTop.h"
#include "CCursor.h"
#include "CHand.h"

#include "CWorld.h"




CARDTWindow::CARDTWindow(void)
: CWindow()
{
}


CARDTWindow::~CARDTWindow()
{
}


int CARDTWindow::Init(void)
{
	webCamList		= new CWebCam*[webCamNum];
	webCamList[0]	= new CMainWebCam();

	if(webCamNum == 2) {
		webCamList[1]	= new CHTWebCam();
		((CHTWebCam*) webCamList[1])->SetTargetMarker(CHeadTrackingMarker::HTMarker);
	}

	scale = 1.0;

	const char* cparamNames[] = {
		"Data/camera_para.dat",
		"Data/camera_para.dat",
	};

	char* vconfs[] = {
		"Data/WDM_camera_flipV_1.xml", //HMD用
		"Data/WDM_camera_flipV_0.xml", //HT用
	};

	for(int i = 0; i < webCamNum; i++) {
		webCamList[i]->SetupWebCam(cparamNames[i], vconfs[i]);
	//	if(!webCamList[i]->SetupWebCam(cparamNames[i], vconfs[i])) return(0);
	}

	width		= webCamList[0]->ARTCparam.xsize;
	height		= webCamList[0]->ARTCparam.ysize;
	aspectRatio	= (double) width / (double) height;
	Reshape(width, height);

	SetupWindow();
	FlipWipe();

	return(1);
}


void CARDTWindow::Display(void)
{
//	glDrawBuffer(GL_BACK);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	if(showWipe == 0 || webCamNum == 1) {
		glMatrixMode(GL_MODELVIEW);
		glViewport(vpX, vpY, vpWidth, vpHeight);
		webCamList[0]->Display(arglSettings);
		webCamList[0]->SetZeroImage();
	} else {
		glMatrixMode(GL_MODELVIEW);
		glViewport(vpX, vpY, vpWidth, vpHeight);
		webCamList[0]->Display(arglSettings);

		glMatrixMode(GL_MODELVIEW);
		glViewport(vpX, vpY, vpWidth / wipeRatio, vpHeight / wipeRatio);
		webCamList[1]->Display(arglSettings);

		webCamList[0]->SetZeroImage();
		webCamList[1]->SetZeroImage();
	}
}


void CARDTWindow::Reshape(int currentWidth, int currentHeight)
{
	double currentAspectRatio = (double) currentWidth / (double) currentHeight;

	if(currentAspectRatio == aspectRatio) {
		vpWidth		= currentWidth;
		vpHeight	= currentHeight; 
		vpX			= 0;
		vpY			= 0;
	} else if(currentAspectRatio > aspectRatio) { //横長
		vpWidth		= currentHeight * aspectRatio;
		vpHeight	= currentHeight; 
		vpX			= (currentWidth - vpWidth) / 2;
		vpY			= 0;
	} else { //縦長
		vpWidth		= currentWidth;
		vpHeight	= currentWidth / aspectRatio; 
		vpX			= 0;
		vpY			= currentHeight - vpHeight;
	}
}


void CARDTWindow::Keyboard(unsigned char key)
{
	if(key == 0x1B) {
		Quit();
	} else if(key == 'w') {
		FlipWipe();
	} else if(key == 'g') {
		if(fullWindow) {
			glutReshapeWindow(640, 480);
			glutPositionWindow(10, 20);
		} else {
			glutFullScreen();
		}

		fullWindow = !fullWindow;
	} else if(key == 'f') {
		CCursor::FixAll();
	} else if(key == ' ') { //handAをクリック
		if(CARDeskTop::arDeskTop->user->handA)
			CARDeskTop::arDeskTop->user->handA->keyClick = 1;
	}
}


void CARDTWindow::FlipWipe(void)
{
	showWipe = (showWipe + 1) % 3;
	if(webCamNum == 2) {
		if(showWipe == 2) {
			webCamList[1]->dispImage = false;
		} else {
			webCamList[1]->dispImage = true;
		}
	}
}
