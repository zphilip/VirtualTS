#include "CMarkerDisplay.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include <AR/ar.h>

#include "CCalibrationWindow.h"
#include "CARDeskTop.h"
#include "CWebCam.h"
#include "CHTWebCam.h"
#include "CHeadTrackingMarker.h"


CMarkerObject* CCalibrationWindow::calibMarker = 0;
int CCalibrationWindow::calibMode = 0;



CCalibrationWindow::CCalibrationWindow(void)
: CWindow()
{
	wipeRatio		= 6;

	simpleWebCam	= 0;
	HTWebCam		= 0;
}


CCalibrationWindow::~CCalibrationWindow()
{
}


int CCalibrationWindow::Init(void)
{
	if(webCamNum < 2) return(0);

	webCamNum		= 3;
	webCamList		= new CWebCam*[webCamNum];

	webCamList[0]	= new CMarkerDisplay();

	simpleWebCam	= new CSimpleWebCam();
	webCamList[1]	= simpleWebCam;
	simpleWebCam->SetTargetMarker(calibMarker);

	HTWebCam		= new CHTWebCam();
	webCamList[2]	= HTWebCam;
	HTWebCam->SetTargetMarker(CHeadTrackingMarker::HTMarker);

	scale = 1.0;

	const char* cparamNames[] = {
		"",
		"Data/camera_para.dat",
		"Data/camera_para.dat",
	};

	char* vconfs[] = {
		"",								//仮想カメラ用
		"Data/WDM_camera_flipV_01.xml", //キャリブレーション用
		"Data/WDM_camera_flipV_02.xml", //HT用
	};

	for(int i = 0; i < webCamNum; i++) {
		if(!webCamList[i]->SetupWebCam(cparamNames[i], vconfs[i])) return(0);
	}

	width		= webCamList[0]->ARTCparam.xsize;
	height		= webCamList[0]->ARTCparam.ysize;
	aspectRatio	= (double) width / (double) height;
	Reshape(width, height);

	SetupWindow();

	return(1);
}


void CCalibrationWindow::Display(void)
{
//	glDrawBuffer(GL_BACK);
	if(simpleWebCam->pattFound && HTWebCam->pattFound) {
		double invMat[3][4],invMat2[3][4], m[3][4];

		arUtilMatInv(HTWebCam->targetMarker->trans, invMat);
		arUtilMatInv(simpleWebCam->targetMarker->trans, invMat2);
		arUtilMatMul(invMat2, invMat, camTrans);
	/*	arUtilMatMul(invMat2, invMat, m);
		arUtilMatInv(m, camTrans);

		double pos[3], newPos[3];

		pos[0] = HTWebCam->targetMarker->trans[0][3] - camTrans[0][3];
		pos[1] = HTWebCam->targetMarker->trans[1][3] - camTrans[1][3];
		pos[2] = HTWebCam->targetMarker->trans[2][3] - camTrans[2][3];

		newPos[0] = camTrans[0][0]*pos[0] + camTrans[0][1]*pos[1] + camTrans[0][2]*pos[2];
		newPos[1] = camTrans[1][0]*pos[0] + camTrans[1][1]*pos[1] + camTrans[1][2]*pos[2];
		newPos[2] = camTrans[2][0]*pos[0] + camTrans[2][1]*pos[1] + camTrans[2][2]*pos[2];

		printf("%f, %f, %f\n", newPos[0], newPos[1], newPos[2]);*/
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glMatrixMode(GL_MODELVIEW);
	glViewport(vpX, vpY, vpWidth, vpHeight);
	webCamList[0]->Display(arglSettings);

	glMatrixMode(GL_MODELVIEW);
	glViewport(vpX, vpY, vpWidth / wipeRatio, vpHeight / wipeRatio);
	webCamList[1]->Display(arglSettings);

	glMatrixMode(GL_MODELVIEW);
	glViewport(vpX, vpY + vpHeight / wipeRatio, vpWidth / wipeRatio, vpHeight / wipeRatio);
	webCamList[2]->Display(arglSettings);

	webCamList[1]->SetZeroImage();
	webCamList[2]->SetZeroImage();
}


void CCalibrationWindow::Reshape(int currentWidth, int currentHeight)
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
		vpY			= (currentHeight - vpHeight) / 2;
	}
	
	if(calibMarker) {
		calibMarker->markerWidth = pixelPitch * vpHeight * 0.5;
		printf("marker width = %f\n", calibMarker->markerWidth);
	}
}


void CCalibrationWindow::Keyboard(unsigned char key)
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
	} else if(key == 'p') { //print
		printf("mult\n");
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 4; j++)
				printf("%f ", camTrans[i][j]);
			printf("\n");
		}
		printf("\n");
	} else if(key == 's') { //save
		printf("save\n");
		FILE*	fp;
		fp = fopen("cam.txt", "w");
		if(!fp) return;
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 4; j++)
				fprintf(fp, "%f ", camTrans[i][j]);
			fprintf(fp, "\n");
		}

		fclose(fp);
	} else {
	}
}
