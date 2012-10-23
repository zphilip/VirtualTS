#ifndef ___CWall
#define ___CWall

#include "CGroup.h"


class CLabel3D;
class CCVPictureFrame;


class CWall : public CGroup
{
public :
	static	CWall*		wall;
public :
			CLabel3D*	modeDisp;
			CLabel3D*	ardesktopLogo;
			CCVPictureFrame*
						wallpaper;
public :
						CWall(void);
	virtual				~CWall();

	virtual	void		Init(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);

			void		SetCursorMode(int mode);
};


#endif
