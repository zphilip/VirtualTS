#include "CTitleBar.h"


CTitleBar::CTitleBar(void)
: CLabeledCube()
{
	fix = true;
}


CTitleBar::~CTitleBar()
{
}


void CTitleBar::Init(void)
{
	CLabeledCube::Init();

	frame->SetColor4(0.0f, 0.0f, 1.0f, 0.5f);
//	cube->SetSize(65.0, 30.0, 30.0);
//	label->SetPosition(-20.0, -5.0, 16.0);
	label->SetColor3(1.0f, 1.0f, 1.0f);
}

