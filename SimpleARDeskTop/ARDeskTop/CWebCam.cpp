#include <stdio.h>
#include <stdlib.h>

#include <GL/glut.h>

#include <AR/config.h>
#include <AR/video.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/gsub_lite.h>

#include "CWebCam.h"
#include "CHeadTrackingMarker.h"

#include "CMarkerObject.h"



CWebCam::CWebCam(void)
{
	callCountMarkerDetect	= 0;
	ARTImage				= 0;
	dispImage				= true;
}


CWebCam::~CWebCam()
{
}


int CWebCam::SetupWebCam(const char *cparam_names, char *vconfs)
{
	int			xsize, ysize;
	ARParam		wparam;

	if((ARTVideo = ar2VideoOpen(vconfs)) == 0) return(0);
	if(ar2VideoInqSize(ARTVideo, &xsize, &ysize) < 0) return(0);
	if(arParamLoad(cparam_names, 1, &wparam) < 0) return(0);

	arParamChangeSize(&wparam, xsize, ysize, &ARTCparam);
	arInitCparam(&ARTCparam);
	arParamDisp(&ARTCparam);
	ARTThreshhold = 100;

	arglCameraFrustumRH(&ARTCparam, VIEW_DISTANCE_MIN, VIEW_DISTANCE_MAX, projectionMat);

	if(ar2VideoCapStart(ARTVideo) != 0) return(0);

	ar2VideoCapNext(ARTVideo);

	return(1);
}


void CWebCam::Quit(void)
{
	ar2VideoCapStop(ARTVideo);
	ar2VideoClose(ARTVideo);
}


void CWebCam::Display(ARGL_CONTEXT_SETTINGS_REF arglSettings)
{
	if(!ARTImage) return;

	if(dispImage)
		arglDispImage(ARTImage, &ARTCparam, 1.0, arglSettings);

	if(pattFound) {
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(projectionMat);

		this->Draw();
	}

	ar2VideoCapNext(ARTVideo);

}

