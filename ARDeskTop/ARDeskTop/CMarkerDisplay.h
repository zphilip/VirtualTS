#ifndef ___CMarkerDisplay
#define ___CMarkerDisplay

#include "CWebCam.h"


class CMarkerDisplay : public CWebCam
{
public :
						CMarkerDisplay(void);
	virtual				~CMarkerDisplay();

	virtual	int			SetupWebCam(const char* cparam_names, char* vconfs);
	virtual	void		Quit(void) {}
	virtual	void		Display(ARGL_CONTEXT_SETTINGS_REF arglSettings);
	virtual	bool		DetectMarker(void) { return(true); }
	virtual	void		Draw(void);

			void		DrawQuad(double width, double x, double y);
};


#endif
