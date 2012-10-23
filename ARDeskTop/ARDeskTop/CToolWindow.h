#ifndef ___CToolWindow
#define ___CToolWindow

#include "CWindow3D.h"

class CButton3D;


class CToolWindow : public CWindow3D
{
public :
			CButton3D*	buttonT;
			CButton3D*	buttonR;
			CButton3D*	buttonS;
			CButton3D*	buttonD;
			CButton3D*	buttonM;
			CButton3D*	buttonW;
			CButton3D*	buttonG; //ÉQÅ[ÉÄóp
public :
						CToolWindow(void);
	virtual				~CToolWindow();

	virtual	void		Init(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);
	virtual	void		ItemDown(CComponent3D* item);
	virtual	void		Reset(void);
};


#endif
