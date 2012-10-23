#ifndef ___CWristWatch
#define ___CWristWatch

#include "CFrame.h"

class CButton3D;
class CClock;
class CKeyboard;


class CWristWatch : public CFrame
{
public :
			int			buttonNum;
			CButton3D*	button;
			CClock*		clock;
			CKeyboard*	keyboard;
public :
						CWristWatch(void);
	virtual				~CWristWatch();

	virtual	void		Init(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);
	virtual	CComponent3D*
						IsWithin(double x, double y, double z);

	virtual	void		ItemDown(CComponent3D* item);
};


#endif
