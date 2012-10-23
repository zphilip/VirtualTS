#include "CVirtualWebCam.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include <AR/ar.h>

#include "CARDT2Window.h"
#include "CARDeskTop.h"
#include "CWebCam.h"
#include "CHTWebCam.h"
#include "CHeadTrackingMarker.h"
#include "CMainWebCam2.h"
#include "CHand.h"
#include "CMainCursor.h"
#include "CWall.h"
#include "Color.h"




CARDT2Window::CARDT2Window(void)
: CWindow()
{
}


CARDT2Window::~CARDT2Window()
{
}


int CARDT2Window::Init(void)
{
	if(webCamNum != 1) return(0);

	webCamNum		= 2;
	webCamList		= new CWebCam*[webCamNum];

	webCamList[0]	= new CVirtualWebCam();
	webCamList[1]	= new CMainWebCam2();

	scale = 1.0;

	const char* cparamNames[] = {
		"",
		"Data/camera_para.dat",
	};

	char* vconfs[] = {
		"",
		"Data/WDM_camera_flipV_02.xml",
	};

	for(int i = 0; i < webCamNum; i++) {
		if(!webCamList[i]->SetupWebCam(cparamNames[i], vconfs[i])) return(0);
	}
	webCamList[1]->dispImage = false;

	width		= webCamList[1]->ARTCparam.xsize;
	height		= webCamList[1]->ARTCparam.ysize;
	aspectRatio	= (double) width / (double) height;
	Reshape(width, height);

	vpX			= 0;
	vpY			= 0;

	Color::SetColor3v(bgColor, Color::black);
	bgColorID	= 0;

	SetupWindow();

	return(1);
}


void CARDT2Window::Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(bgColor[0], bgColor[1], bgColor[2], 0.0f);	//black

	glMatrixMode(GL_MODELVIEW);
	glViewport(vpX, vpY, vpWidth, vpHeight);
	webCamList[0]->Display(arglSettings);

	glMatrixMode(GL_MODELVIEW);
	glViewport(vpX, vpY, vpWidth2 / wipeRatio, vpHeight2 / wipeRatio);
	webCamList[1]->Display(arglSettings);

	webCamList[1]->SetZeroImage();
}


void CARDT2Window::Reshape(int currentWidth, int currentHeight)
{
	double currentAspectRatio = (double) currentWidth / (double) currentHeight;

	vpWidth		= currentWidth;
	vpHeight	= currentHeight;
	if(currentAspectRatio > aspectRatio) { //横長
		vpWidth2	= currentHeight * aspectRatio;
		vpHeight2	= currentHeight; 
	} else if(currentAspectRatio < aspectRatio) { //縦長
		vpWidth2	= currentWidth;
		vpHeight2	= currentWidth / aspectRatio;
	}

	CWindow::Reshape(vpWidth, vpHeight);
}


void CARDT2Window::Keyboard(unsigned char key)
{
	if(key == 0x1B) {
		Quit();
	} else if(key == 'g') {
		if(fullWindow) {
			glutReshapeWindow(640, 480);
			glutPositionWindow(10, 20);
		} else {
			glutFullScreen();
		}

		fullWindow = !fullWindow;
	} else if(key == ' ') { //handAをクリック
		if(CARDeskTop::arDeskTop->user->handA) {
			CARDeskTop::arDeskTop->user->handA->keyClick = 1;
		}
	} else if(key == 'b') {
		if(bgColorID == 0) Color::SetColor3v(bgColor, Color::white);
		else Color::SetColor3v(bgColor, Color::black);
		
		bgColorID = bgColorID % 2;
	} else {
		int mode;

		if(key == 't') mode = CUser::kTranslate;
		else if(key == 'r') mode = CUser::kRotateA;
		else if(key == 's') mode = CUser::kScale;
		else if(key == 'd') mode = CUser::kDraw;
		else if(key == 'w') mode = CUser::kTwist;

		CARDeskTop::arDeskTop->user->SetCursorMode(mode);
		CWall::wall->SetCursorMode(mode);
	}
}


void CARDT2Window::Mouse(int button, int state, int x, int y)
{
	if(button == 0) { //左クリック
		//state == 0 Down, state == 1 Up
		if(CARDeskTop::arDeskTop->user->handA)
			if(CARDeskTop::arDeskTop->user->handA->mainCursor->visible)
				CARDeskTop::arDeskTop->user->handA->keyClick = 1;
	} else if(button == 2) { //右クリック
	}
}


