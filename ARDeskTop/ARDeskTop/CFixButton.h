#ifndef ___CFixButton
#define ___CFixButton

#include "CMarkerButton.h"



class CFixButton : public CMarkerButton
{
public :
						CFixButton(void);
	virtual				~CFixButton();

	virtual	int			CheckButton(void);
};


#endif
