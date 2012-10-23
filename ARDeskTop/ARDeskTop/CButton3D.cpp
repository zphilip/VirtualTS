#include "CButton3D.h"
#include "Color.h"


CButton3D::CButton3D(void)
: CLabeledCube()
{
	fix			= true;
	notifyParent= true;

	lockMode	= kButtonLock;
	buttonMode	= kButton;

	locked		= false;

	normalColor	= 0;
	hilightColor = 0;
}


CButton3D::~CButton3D()
{
}


void CButton3D::Init(void)
{
	CLabeledCube::Init();

	normalColor = this->color;
	hilightColor = frame->color;
	SetColor4va(Color::blue, 0.5f);
	frame->SetColor4va(Color::red, 0.5f);
	frame->SetCurrentColor(normalColor);
	label->SetColor3v(Color::white);
	label->SetAlignmentH(CLabel3D::kCenter);
}


void CButton3D::Precompute(void)
{
	CLabeledCube::Precompute();
}


void CButton3D::CursorEnter(CCursor* cursor)
{
	CLabeledCube::CursorEnter(cursor);

	if(buttonMode == kButton) {
		if(lockMode	== kButtonLock) {
			locked = !locked;
			if(locked)
				Select();
			else
				Unselect();
		} else if(lockMode	== kButtonUnlock) {
			Select();
		}
	} else if(buttonMode == kRadioButton) {
		if(!locked) {
			parent->Reset();
			Select();
		}
	}
}


void CButton3D::CursorLeave(CCursor* cursor)
{
	CLabeledCube::CursorLeave(cursor);

	if(buttonMode == kButton) {
		if(lockMode	== kButtonUnlock) {
			Unselect();
		}
	}
}


void CButton3D::Select(void)
{
	if(frame) {
		locked = true;
		if(hilightColor) frame->SetCurrentColor(hilightColor);
	}
}


void CButton3D::Unselect(void)
{
	if(frame) {
		locked = false;
		if(normalColor) frame->SetCurrentColor(normalColor);
	}
}


void CButton3D::SetButtonMode(int pButtonMode)
{
	buttonMode = pButtonMode;
}


void CButton3D::SetLockMode(int pLockMode)
{
	lockMode = pLockMode;
}


