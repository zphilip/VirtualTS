#ifndef ___CKeyboardKey
#define ___CKeyboardKey

#include "CButton3D.h"

#define BACK_SPACE_KEY		13
#define TAB_KEY				14
#define CAPS_LOCK_KEY		28
#define ENTER_KEY			40
#define SHIFT_KEY_A			41
#define SHIFT_KEY_B			52

#define CAPS_LOCK_KEY_CODE	0x000E
#define SHIFT_KEY_CODE		0x000F


class CKeyboardKey : public CButton3D
{
public :
						CKeyboardKey(void);
	virtual				~CKeyboardKey();

	virtual	void		Init(void);
};


#endif
