#ifndef ___CSimpleWebCam
#define ___CSimpleWebCam


#include "CWebCam.h"

class CMarkerObject;



class CSimpleWebCam : public CWebCam
{
public :
			CMarkerObject*
						targetMarker;
public :
						CSimpleWebCam(void);
	virtual				~CSimpleWebCam();

	virtual	bool		DetectMarker(void);
	virtual	void		Draw(void);

			void		SetTargetMarker(CMarkerObject* pTargetMarker);
};


#endif


