#ifndef ___CFrame
#define ___CFrame

#include "CGroup.h"
#include "CFrameCube.h"



class CFrame : public CGroup
{
public :
			CFrameCube*	frame;
public :
						CFrame(void);
	virtual				~CFrame();

	virtual	void		Init(void);
};


#endif
