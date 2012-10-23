#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CHeadTrackingMarker.h"
#include "CCube.h"


CHeadTrackingMarker* CHeadTrackingMarker::HTMarker = 0;
int CHeadTrackingMarker::HTMode = 0;
int CHeadTrackingMarker::smoothNum = 0;



CHeadTrackingMarker::CHeadTrackingMarker(void)
: CMarkerObject()
{
	drawCard		= false;
	drawRect		= true;

	smoothCursor	= 0;
	startSmoothing	= false;
}


CHeadTrackingMarker::~CHeadTrackingMarker()
{
}


void CHeadTrackingMarker::Init(void)
{
	CMarkerObject::Init();

	if(smoothNum > MAX_SMOOTH_NUM) smoothNum = MAX_SMOOTH_NUM;
	else if(smoothNum < 2) smoothNum = 2;

	if(HTMode && smoothNum) {
		int i, j;
		int n;

		smoothCursor	= 0;
		startSmoothing	= 0;

		for(i = 0; i < 3; i++)
			for(j = 0; j < 4; j++)
				smoothMatSum[i][j]	= 0.0;

		for(n = 0; n < smoothNum; n++) {
			for(i = 0; i < 3; i++)
				for(j = 0; j < 4; j++)
					smoothMat[n][i][j] = 0.0;
		}
	}
}


void CHeadTrackingMarker::Precompute(void)
{
	CMarkerObject::Precompute();

	if(!HTMode || !smoothNum) return;

	int i, j;

	if(smoothCursor == smoothNum - 1) startSmoothing = true;
	if(0) {
		for(i = 0; i < 3; i++) {
			for(j = 0; j < 4; j++) {
				smoothMatSum[i][j] += trans[i][j] - smoothMat[smoothCursor][i][j];
				smoothMat[smoothCursor][i][j] = trans[i][j];
				if(startSmoothing) smoothMatAve[i][j] = smoothMatSum[i][j] / (double) smoothNum;
			}
		}
	} else if (1) {
		for(i = 0; i < 3; i++) {
			for(j = 0; j < 4; j++) {
				smoothMat[smoothCursor][i][j] = trans[i][j];
			}
		}

		if(startSmoothing) {
			int k, cursor;
			double weight;

			for(i = 0; i < 3; i++) {
				for(j = 0; j < 4; j++) {
					smoothMatAve[i][j] = 0.0;
					weight = 1.0;
					cursor = smoothCursor;
					for(k = 0; k < smoothNum; k++) {
						if(k < smoothNum - 1) weight *= 0.5;
						smoothMatAve[i][j] += weight * smoothMat[cursor][i][j];
						cursor--;
						if(cursor < 0)  cursor = smoothNum - 1;
					}
				}
			}
		}
	} else {
		if(!startSmoothing) {
			for(i = 0; i < 3; i++) {
				for(j = 0; j < 4; j++) {
					smoothMatAve[i][j] = trans[i][j];
				}
			}
			startSmoothing = 1;
		}

		for(i = 0; i < 3; i++) {
			for(j = 0; j < 4; j++) {
				smoothMatAve[i][j] = (trans[i][j] + smoothMatAve[i][j]) * 0.5;
			}
		}
	}

	smoothCursor = (smoothCursor + 1) % smoothNum;
}


