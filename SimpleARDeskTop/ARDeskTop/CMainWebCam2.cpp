#include "CMainWebCam2.h"
#include "CHeadTrackingMarker.h"
#include "CARDeskTop.h"




CMainWebCam2::CMainWebCam2(void)
: CMainWebCam()
{
}


CMainWebCam2::~CMainWebCam2()
{
}


bool CMainWebCam2::DetectMarker(void)
{
	bool prevVisible = CHeadTrackingMarker::HTMarker->visible;
	bool ret = CMainWebCam::DetectMarker();

	if(CARDeskTop::mode == CARDeskTop::kVR) {
		if(prevVisible && !CHeadTrackingMarker::HTMarker->visible) {
			CHeadTrackingMarker::HTMarker->visible = true;
			pattFound = true;
		}
	}

	return(ret);
}


void CMainWebCam2::Draw(void)
{
	CMainWebCam::markerObjectList.DrawRect();
}

