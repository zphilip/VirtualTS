#ifndef ___CWindow3D
#define ___CWindow3D

#include "CFrame.h"
#include "CTitleBar.h"

#define WINDOW_MARGIN	1.5
#define WINDOW_DEPTH	TITLEBAR_HEIGHT + WINDOW_MARGIN * 2.0

class CTitleBar;


class CWindow3D : public CFrame
{
public :
			CTitleBar*	titleBar;
public :
						CWindow3D(void);
	virtual				~CWindow3D();

	virtual	void		Init(void);

	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);
};


#endif
