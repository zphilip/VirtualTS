#include <stdio.h>

#include "CUser.h"
#include "CHand.h"
#include "CCursor.h"
#include "CMainCursor.h"


int CUser::cursorMode	= CUser::kMulti;
int CUser::cursorMode2	= CUser::kTranslate;



CUser::CUser(void)
{
	multiCursorMode	= 0;
	handA			= 0;
	handB			= 0;
}


CUser::~CUser()
{
	if(handA) delete handA;
	if(handB) delete handB;	
}


void CUser::Init(void)
{
	handA = new CHand();
	handB = new CHand();
}


void CUser::Init2(void)
{
	handA->Init();
	handB->Init();

	multiCursorMode =	(handA->mainCursor && (handB->mainCursor || handB->subCursor)) ||
						(handB->mainCursor && (handA->mainCursor || handA->subCursor));

	CUser::cursorMode = multiCursorMode ? kMulti : kTranslate;
}


void CUser::CheckClick(void)
{
	int clickA, clickB;
	int clickM = 0;

	clickA = handA->CheckClick();
	clickB = handB->CheckClick();

	if(multiCursorMode) {
		if(clickA) {
			cursorMode = kMulti;
			clickM = handA->mainCursor->MultiClick(handB, clickB);
		} else if(clickB) {
			cursorMode = kMulti;
			clickM = handB->mainCursor->MultiClick(handA, clickA);
		}
	}

	if(clickM == 1) {
		handA->SetLockCursor(true);
		handB->SetLockCursor(true);
	} else {
		if(clickA && clickB && handA->mainCursor && handB->mainCursor) {
			if(handA->mainCursor->selectObject == handB->mainCursor->selectObject) {
				clickB = 0; //HandA—Dæ
			}
		}
		if(clickA) {
			if(clickM == 0) cursorMode = cursorMode2;
			handA->SetLockCursor(handA->Click() ? true : false);
		}
		if(clickB) {
			if(clickM == 0) cursorMode = cursorMode2;
			handB->SetLockCursor(handB->Click() ? true : false);
		}

		if(clickM == -1) {
			handA->SetLockCursor(false);
			handB->SetLockCursor(false);
		}
	}
}


void CUser::SetCursorMode(int mode)
{
	if(!multiCursorMode && mode == kMulti) return;
//	cursorMode = mode;
	if(mode != kMulti) cursorMode2 = mode;
}


CCursor* CUser::FindWithinObject(CComponent3D* object)
{
	CCursor* ret = 0;

	if(handA) ret = handA->FindWithinObject(object);
	if(!ret && handB) ret = handB->FindWithinObject(object);

	return(ret);
}


