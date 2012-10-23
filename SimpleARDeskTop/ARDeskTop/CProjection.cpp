#include <AR/ar.h>
#include <AR/gsub.h>
#include "CWebCam.h"

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CProjection.h"
#include "CCursor.h"
#include "CHeadTrackingMarker.h"
#include "CARDeskTop.h"



CProjection::CProjection(void)
: CMarkerObject()
{
}


CProjection::~CProjection()
{
}


void CProjection::Init(void)
{
	CMarkerObject::Init();
}


void CProjection::Precompute(void)
{
	CMarkerObject::Precompute();

	if(CARDeskTop::mode == CARDeskTop::kAR && fix && CHeadTrackingMarker::HTMode && CHeadTrackingMarker::HTMarker) {
		if(!CHeadTrackingMarker::HTMarker->visible) return;

		arUtilMatMul(HTMarkerInvMat,
			CHeadTrackingMarker::HTMarker->startSmoothing ? CHeadTrackingMarker::HTMarker->smoothMatAve : CHeadTrackingMarker::HTMarker->trans,
			invMat);
		arUtilMatInv(invMat, trans);
	}
}


CComponent3D* CProjection::IsWithin(double x, double y, double z)
{
	return((visible && shape) ? shape->IsWithin(x, y, z) : 0);
}


void CProjection::ProjectCursorPos(CCursor* cursor, double pCursorPos[3])
{
	double pos[3];
	pos[0] = cursor->trans[0][3] - trans[0][3];
	pos[1] = cursor->trans[1][3] - trans[1][3];
	pos[2] = cursor->trans[2][3] - trans[2][3];

	if(!fix) arUtilMatInv(trans, invMat);
	pCursorPos[0] = invMat[0][0]*pos[0] + invMat[0][1]*pos[1] + invMat[0][2]*pos[2];
	pCursorPos[1] = invMat[1][0]*pos[0] + invMat[1][1]*pos[1] + invMat[1][2]*pos[2];
	pCursorPos[2] = invMat[2][0]*pos[0] + invMat[2][1]*pos[1] + invMat[2][2]*pos[2];
}


void CProjection::ProjectPos(double pos[3], double pCursorPos[3])
{
	if(!fix) arUtilMatInv(trans, invMat);
	pCursorPos[0] = invMat[0][0]*pos[0] + invMat[0][1]*pos[1] + invMat[0][2]*pos[2];
	pCursorPos[1] = invMat[1][0]*pos[0] + invMat[1][1]*pos[1] + invMat[1][2]*pos[2];
	pCursorPos[2] = invMat[2][0]*pos[0] + invMat[2][1]*pos[1] + invMat[2][2]*pos[2];
}


void CProjection::FlipFix(void)
{
	if(!visible) {
		return;
	} else if(CHeadTrackingMarker::HTMode && CHeadTrackingMarker::HTMarker) {
		if(!CHeadTrackingMarker::HTMarker->visible) return;
	}

	if(fix) {
		fix			= false;
		drawCard	= true;
	} else {
		fix			= true;
		drawCard	= false;

		arUtilMatInv(trans, invMat);

		if(CHeadTrackingMarker::HTMode && CHeadTrackingMarker::HTMarker) {
			if(CHeadTrackingMarker::HTMarker->visible) {
				double	m[3][4];

				arUtilMatInv(CHeadTrackingMarker::HTMarker->trans, m);
				arUtilMatMul(invMat, m, HTMarkerInvMat);
			} else {
				fix			= false; //ŒÅ’èŽ¸”s
				drawCard	= true;
			}
		}
	}
}

