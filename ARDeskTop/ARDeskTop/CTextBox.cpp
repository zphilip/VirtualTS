#include "CTextBox.h"
#include "CMainCursor.h"
#include "Color.h"



void CTextBox::Init(void)
{
	CLabeledCube::Init();

	frame->SetColor4va(Color::black, 1.0f);
	SetColor4va(Color::green, 1.0f);
	label->SetCurrentColor(currentColor);
	fix			= false;
	dropable	= true;
}


void CTextBox::DropTo(CComponent3D* object, CMainCursor* cursor)
{
	if(object) object->SetString(label->GetString());
	SetPosition(cursor->firstClickInfo.objectPos);
}

