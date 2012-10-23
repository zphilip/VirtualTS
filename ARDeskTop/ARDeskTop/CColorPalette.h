#ifndef ___CColorPalette
#define ___CColorPalette

#include "CFrame.h"



class CColorCube : public CCube
{
public :
						CColorCube(void);

	virtual	void		DropTo(CComponent3D* object, CMainCursor* cursor);
};

class CColorPalette : public CFrame
{
public :
			CColorCube*	color01;
			CColorCube*	color02;
			CColorCube*	color03;
			CColorCube*	color04;
			CColorCube*	color05;
			CColorCube*	color06;
			CColorCube*	color07;
			CColorCube*	color08;
			CColorCube*	color09;
public :
						CColorPalette(void);
	virtual				~CColorPalette();

	virtual	void		Init(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);
};


#endif
