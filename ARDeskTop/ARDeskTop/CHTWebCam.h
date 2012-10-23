#ifndef ___CHTWebCam
#define ___CHTWebCam


#include "CSimpleWebCam.h"



class CHTWebCam : public CSimpleWebCam
{
public :
						CHTWebCam(void);
	virtual				~CHTWebCam();

	virtual	void		Display(ARGL_CONTEXT_SETTINGS_REF arglSettings);
};


#endif


