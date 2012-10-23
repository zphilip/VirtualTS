#include <stdio.h>
#include <stdlib.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CSimpleWebCam.h"
#include "CMarkerObject.h"



CSimpleWebCam::CSimpleWebCam(void)
: CWebCam()
{
	targetMarker	= 0;
}


CSimpleWebCam::~CSimpleWebCam()
{
	if(targetMarker) delete targetMarker;
}


bool CSimpleWebCam::DetectMarker(void)
{
	ARUint8*		image;

	pattFound = false;	
	image = ar2VideoGetImage(ARTVideo);

	if(!image) return(false);

	ARTImage = image;

	callCountMarkerDetect++;

	if(targetMarker) {
		ARMarkerInfo*	markerInfo;
		int				markerNum;
		int				j, k;

		if(arDetectMarker(ARTImage, ARTThreshhold, &markerInfo, &markerNum) < 0) {
			exit(-1);
		}

		k = -1;
		for(j = 0; j < markerNum; j++) {
			if(targetMarker->id == markerInfo[j].id) {
				if(k == -1) k = j;
				else if(markerInfo[k].cf < markerInfo[j].cf) k = j;
			}
		}
		if(k == -1) {
			targetMarker->visible = false;
		} else {
			targetMarker->GetTransMat(&markerInfo[k]);
			targetMarker->visible = true;
			pattFound = true;	
		}
	}

	return(true);
}


void CSimpleWebCam::Draw(void)
{
	if(targetMarker) {
		targetMarker->Draw();
	}
}


void CSimpleWebCam::SetTargetMarker(CMarkerObject* pTargetMarker)
{
	targetMarker	= pTargetMarker;
}

