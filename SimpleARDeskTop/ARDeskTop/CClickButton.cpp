#include "CClickButton.h"
#include "CARDeskTop.h"
#include "CProjection.h"
#include "CCursor.h"
#include "CMainCursor.h"



CClickButton::CClickButton(void)
: CMarkerButton()
{
}


CClickButton::~CClickButton()
{
}


/*int CClickButton::CheckButton(void)
{
	CMainCursor* mainCursor = (CMainCursor*) CCursor::cursorList[0];

	if(!prevVisible && visible || prevVisible && !visible) {
		if(mainCursor->visible) {
			Keyboard(' ', 0, 0);
		} else {
			CCursor::FixAll();
		}
	}

	prevVisible = visible;
}
*/