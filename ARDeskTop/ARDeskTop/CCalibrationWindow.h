#ifndef ___CCalibrationWindow
#define ___CCalibrationWindow


#include "CWindow.h"

class CMarkerObject;
class CSimpleWebCam;
class CHTWebCam;


class CCalibrationWindow : public CWindow
{
public :
	static	CMarkerObject*
						calibMarker;

	static	int			calibMode;
public :
			CSimpleWebCam*
						simpleWebCam;
			CHTWebCam*	HTWebCam;

			double		camTrans[3][4];
public :
						CCalibrationWindow(void);
	virtual				~CCalibrationWindow();

	virtual	int			Init(void);
	virtual	void		Display(void);
	virtual	void		Reshape(int currentWidth, int currentHeight);
	virtual	void		Keyboard(unsigned char key);
};


#endif
