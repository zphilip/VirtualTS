#include "CFixButton.h"
#include "CCursor.h"



CFixButton::CFixButton(void)
: CMarkerButton()
{
}


CFixButton::~CFixButton()
{
}


int CFixButton::CheckButton(void)
{
	int click = CMarkerButton::CheckButton();

	if(click) CCursor::FixAll();

	return(click);
}

