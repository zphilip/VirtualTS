#include "CHand.h"
#include "CMainCursor.h"
#include "CSubCursor.h"
#include "CComponent3D.h"
#include "CClickButton.h"
#include "CAnimeManager.h"
#include "Color.h"


float CHand::doubleClickTime = 0.1f;



CHand::CHand(void)
{
	mainCursor	= 0;
	subCursor	= 0;
	button		= 0;
	keyClick	= 0;
	clickedTime	= -(doubleClickTime + 0.1f);
}


CHand::~CHand()
{
}


int CHand::CheckClick(void)
{
	if(!mainCursor) return(0);

	int click = keyClick;

	if(!click && subCursor) {
		click = subCursor->CheckClick();
	}

	if(!click && button) {
		click = button->CheckButton();
		if(click) {
			if(!mainCursor->visible) {
				 CCursor::FixAll(); //臨時的
			}
		}
	}

	keyClick = 0;

	return(click);
}


void CHand::Init(void)
{
	if(mainCursor && subCursor) {
		subCursor->mainCursor = mainCursor;
	}
}


int CHand::Click(void)
{
	if(!mainCursor) return(0);

	int ret;

	if(mainCursor->selectObject) {
		if(CAnimeManager::currentTime - clickedTime <= doubleClickTime) {
			mainCursor->selectObject->DoubleClick();
			clickedTime = -(doubleClickTime + 0.1f);
		} else {
			mainCursor->selectObject->Click();
		}
		ret = 0;
	} else {
		ret = mainCursor->Click();
		clickedTime = (float) CAnimeManager::currentTime;
	}

	return(ret);
}


CCursor* CHand::FindWithinObject(CComponent3D* object)
{
	CCursor* ret = 0;

	 //人差し指優先
	if(subCursor) ret = subCursor->FindWithinObject(object);
	if(!ret && mainCursor) ret = mainCursor->FindWithinObject(object);

	return(ret);
}


CCursor* CHand::FindWithinObject(CComponent3D* object, int index)
{
	CCursor* ret = 0;

	 //人差し指優先
	if(subCursor) ret = subCursor->FindWithinObject(object, index);
	if(!ret && mainCursor) ret = mainCursor->FindWithinObject(object, index);

	return(ret);
}


CCursor* CHand::FindVisibleCursor(void)
{
	if(subCursor) { //人差し指優先
		if(subCursor->visible) return(subCursor);
	}
	if(mainCursor) {
		if(mainCursor->visible) return(mainCursor);
	}

	return(0);
}


void CHand::SetLockCursor(bool lock)
{
	if(mainCursor) {
		mainCursor->lock = lock;
		mainCursor->shape->SetColor3v(lock ? Color::red : (mainCursor->IsWithinObject() ? Color::yellow : Color::blue));
	}
	if(subCursor) {
		subCursor->lock = lock;
		subCursor->shape->SetColor3v(lock ? Color::red : (subCursor->IsWithinObject() ? Color::yellow : Color::blue));
	}
}

