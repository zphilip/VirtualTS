#include <time.h>

#include "CClock.h"


CClock::CClock(void)
: CLabeledCube()
{
}


CClock::~CClock()
{
}


void CClock::Init(void)
{
	CLabeledCube::Init();

	frame->SetColor4(0.0f, 0.0f, 1.0f, 0.5f);
	SetSize(65.0, 30.0, 30.0);
//	label->SetPosition(-20.0, -5.0, 16.0);
	label->SetColor3(1.0f, 1.0f, 1.0f);
	label->SetAlignmentH(CLabel3D::kCenter);
}


void CClock::Precompute(void)
{
	CLabeledCube::Precompute();

	time_t timer;
	struct tm* date;
	char str[256];

	timer = time(0);
	date = localtime(&timer);
	strftime(str, 255, "%H:%M:%S", date);
	SetString(str);
}

