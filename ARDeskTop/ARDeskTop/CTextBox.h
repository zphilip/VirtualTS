#ifndef ___CTextBox
#define ___CTextBox

#include "CLabeledCube.h"


class CTextBox : public CLabeledCube
{
public :
	virtual	void		Init(void);
	virtual	void		DropTo(CComponent3D* object, CMainCursor* cursor);
};


#endif


