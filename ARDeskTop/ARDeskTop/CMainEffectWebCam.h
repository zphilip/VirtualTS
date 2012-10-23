#ifndef ___CMainEffectWebCam
#define ___CMainEffectWebCam

#include "CMainWebCam.h"
#include "CCVEffect.h"



class CMainEffectWebCam : public CMainWebCam
{
public :
			CCVEffect*	cvEffect;

			IplImage*	frame;
			IplImage*	frame2;
			IplImage*	temp;
			IplImage*	temp2;
			IplImage*	res;
			IplImage*	mask;
			IplImage*	alpha;
public :
						CMainEffectWebCam(void);
	virtual				~CMainEffectWebCam();

	virtual	int			SetupWebCam(const char* cparam_names, char* vconfs);
	virtual	void		Display(ARGL_CONTEXT_SETTINGS_REF arglSettings);
	virtual	void		Draw(void);
};


#endif
