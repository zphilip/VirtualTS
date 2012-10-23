#ifndef ___CMainWebCam
#define ___CMainWebCam

#include "CWebCam.h"
#include "CList.h"


class CMainWebCam : public CWebCam
{
public :
	static	CMarkerObjectList
						markerObjectList;
public :
						CMainWebCam(void);
	virtual				~CMainWebCam();

	virtual	bool		DetectMarker(void);
	virtual	void		Draw(void);
};


#endif


