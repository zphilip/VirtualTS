#ifndef ___CVirtualWebCam
#define ___CVirtualWebCam

#include "CWebCam.h"


class CVirtualWebCam : public CWebCam
{
public :
			double		zNear, zFar;
			double		width, height;
			double		newPos[3];
public :
						CVirtualWebCam(void);
	virtual				~CVirtualWebCam();

	virtual	int			SetupWebCam(const char* cparam_names, char* vconfs);
	virtual	void		Quit(void) {}
	virtual	void		Display(ARGL_CONTEXT_SETTINGS_REF arglSettings);
	virtual	void		Reshape(int currentWidth, int currentHeight);
	virtual	bool		DetectMarker(void) { return(true); }
	virtual	void		Draw(void);
};


#endif
