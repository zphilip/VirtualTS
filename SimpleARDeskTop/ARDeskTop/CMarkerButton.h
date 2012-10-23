#ifndef ___CMarkerButton
#define ___CMarkerButton

#include "CMarkerObject.h"



class CMarkerButton : public CMarkerObject
{
public :
			bool		prevVisible;
public :
						CMarkerButton(void);
	virtual				~CMarkerButton();

	virtual	void		DrawShape(void);

	virtual	int			CheckButton(void);
};


#endif
