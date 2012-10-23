#ifndef ___CMainWebCam2
#define ___CMainWebCam2

#include "CMainWebCam.h"


class CMainWebCam2 : public CMainWebCam
{
public :
						CMainWebCam2(void);
	virtual				~CMainWebCam2();

	virtual	bool		DetectMarker(void);
	virtual	void		Draw(void);
};


#endif


