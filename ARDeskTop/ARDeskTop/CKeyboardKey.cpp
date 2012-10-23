#include "CKeyboardKey.h"
#include "CKeyboard.h"


CKeyboardKey::CKeyboardKey(void)
: CButton3D()
{
	lockMode	= kButtonUnlock;
}


CKeyboardKey::~CKeyboardKey()
{
}


void CKeyboardKey::Init(void)
{
	CButton3D::Init();
//	SetAlpha(1.0);
//	cube->SetAlpha(1.0);
}

