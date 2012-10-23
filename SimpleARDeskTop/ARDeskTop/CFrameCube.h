#ifndef ___CFrameCube
#define ___CFrameCube

#include "CCube.h"



class CFrameCube : public CCube
{
public :
			bool		drawTransparentCard;
public :
						CFrameCube(void);
	virtual				~CFrameCube();

	virtual	void		DrawShape(void);
};


#endif
