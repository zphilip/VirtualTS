#include <stdio.h>
#include <stdlib.h>

#ifndef __APPLE__
#  include <GL/glew.h>
#  include <GL/wglew.h>
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CWindow.h"
#include "CARDeskTop.h"
#include "CWebCam.h"
#include "CMainWebCam.h"
#include "CMainEffectWebCam.h"
#include "CHTWebCam.h"
#include "CHeadTrackingMarker.h"
#include "CVirtualWebCam.h"



CWindow::CWindow(void)
{
	webCamList	= 0;
	webCamNum	= 0;

	gCallCountGetImage = 0;

	width		= 0;
	height		= 0;
	aspectRatio	= 0.0;
	scale		= 1.0;
	pixelPitch	= 0.2865;	//iiyama E2607WS
//	pixelPitch	= 0.282;	//io data LCD-AD222X

	showWipe	= 1; //0 - 2
	wipeRatio	= 4;

	arglSettings	= 0;

	fullWindow	= false;
}


CWindow::~CWindow()
{
}


int CWindow::Init(void)
{
	return(1);
}


int CWindow::SetupWindow(void)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(width * scale, height * scale);
	glutInitWindowPosition(10, 20);
	if(glutCreateWindow("ARDeskTop") < 1) {
		exit(-1);
	}

//	glewInit();

	glutDisplayFunc(::Display);
	glutReshapeFunc(::Reshape);
	glutVisibilityFunc(::Visibility);
	glutKeyboardFunc(::Keyboard);
	glutMouseFunc(::Mouse);

	if((arglSettings = arglSetupForCurrentContext()) == 0) {
		exit(-1);
	}

//	if(FUTL_LoadShader("simple.vert", "simple.frag", &shdProg) < 0) return(-1);
//	glUseProgram(shdProg);

	return(1);
}


void CWindow::Display(void)
{
}


void CWindow::Reshape(int currentWidth, int currentHeight)
{
	for(int i = 0; i < webCamNum; i++) {
		webCamList[i]->Reshape(currentWidth, currentHeight);
	}
}


void CWindow::Keyboard(unsigned char key)
{
}


void CWindow::Quit(void)
{
	arglCleanup(arglSettings);

	for(int i = 0; i < webCamNum; i++) {
		webCamList[i]->Quit();
		delete webCamList[i];
	}

	delete webCamList;

	webCamNum = 0;

	glutDestroyWindow(0);
}


bool CWindow::DetectMarkerAll(void)
{
	bool ret = false;

	gCallCountGetImage++;

	for(int i = 0; i < webCamNum; i++) {
		ret |= webCamList[i]->DetectMarker();
	}

	return(ret);
}

