#include <stdio.h>
#include <stdlib.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif
#include <AR/gsub.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/video.h>

#include "CMainWebCam.h"
#include "CCursor.h"
#include "CList.h"
#include "CMarkerObject.h"
#include "CHeadTrackingMarker.h"
#include "CARDeskTop.h"
#include "CTrash.h"
#include "CDesktop.h"


CMarkerObjectList CMainWebCam::markerObjectList;



CMainWebCam::CMainWebCam(void)
: CWebCam()
{
}


CMainWebCam::~CMainWebCam()
{
	markerObjectList.DeleteAll();
}


bool CMainWebCam::DetectMarker(void)
{
	ARUint8*		image;

	pattFound = false;	
	image = ar2VideoGetImage(ARTVideo);
	if(!image) return(false);


	ARMarkerInfo*	markerInfo;
	int				markerNum, objectNum;
	int				i, j, k;
	CMarkerObject*	object;


	ARTImage = image;

	callCountMarkerDetect++;

	if(arDetectMarker(ARTImage, ARTThreshhold, &markerInfo, &markerNum) < 0) {
		exit(-1);
	}

	objectNum = markerObjectList.GetDegree();
	markerObjectList.GoToFirst();
	for(i = 0; i < objectNum; i++) {
		object = (CMarkerObject*) markerObjectList.GetKey();

		if(!object->fix) {
			k = -1;
			for(j = 0; j < markerNum; j++) {
				if(object->id == markerInfo[j].id) {
					if(k == -1) k = j;
					else if(markerInfo[k].cf < markerInfo[j].cf) k = j;
				}
			}
			if(k == -1) {
				object->visible = false;
			} else {
				pattFound = true;
				object->GetTransMat(&markerInfo[k]);
				object->visible = true;
			}
		} else {
			pattFound = true;
		}

		markerObjectList.GoToNext();
	}

	return(true);
}


void CMainWebCam::Draw(void)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_COLOR_MATERIAL);

	CARDeskTop::arDeskTop->CheckClick();
//	CCursor::CheckAllButton();

	if(CHeadTrackingMarker::HTMode && CHeadTrackingMarker::HTMarker)
		CHeadTrackingMarker::HTMarker->Precompute();
	markerObjectList.Precompute();
	CCursor::CheckAllWithin();
	CDesktop::desktop->SortByDepth();
	markerObjectList.DrawMarkerObject();

	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_BLEND);
	glDisable(GL_NORMALIZE);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	CTrash::Erase();
}

