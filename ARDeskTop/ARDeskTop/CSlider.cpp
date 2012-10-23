#include <stdio.h>
#include <wchar.h>

#include "CSlider.h"
#include "Color.h"




CSliderControl::CSliderControl(void)
: CLabeledCube()
{
	axis	= 0; //x = 0, y = 1, z = 2
	minPos	= 0.0;
	maxPos	= 0.0;
	notifyParent	= true;
}


CSliderControl::~CSliderControl()
{
}


void CSliderControl::Init(void)
{
	CLabeledCube::Init();

	frame->SetColor4va(Color::blue, 0.5f);
	frame->cardMode = CCube::kRound;
	label->SetColor3v(Color::white);
}


void CSliderControl::InterruptTranslate(double* pos, double* objectPos)
{
	if(pos[axis] > maxPos) pos[axis] = maxPos;
	else if(pos[axis] < minPos) pos[axis] = minPos;

	if(axis != 0) pos[0] = objectPos[0];
	if(axis != 1) pos[1] = objectPos[1];
	if(axis != 2) pos[2] = objectPos[2];
}


double CSliderControl::GetValue(void)
{
	return((matrix[12 + axis] - minPos) / (maxPos - minPos));
}





CSlider::CSlider(void)
: CLabeledCube()
{
	axis			= 0;
	min				= 0.0;
	max				= 0.0;
	value			= 0.0;
	margin			= 0.0;
	ratio			= 1.0;
	sliderControl	= 0;
	checkWithinBBOnly = false;
}


CSlider::~CSlider()
{
}


void CSlider::Init(void)
{
	CLabeledCube::Init();

	sliderControl	= new CSliderControl();
	sliderControl->Init();
//	SetString(L"Slider");
	label->SetColor3v(Color::white);
	AddObject(sliderControl);
}


void CSlider::Precompute(void)
{
	CLabeledCube::Precompute();

/*	wchar_t	 str[256];
	value = (max - min) * sliderControl->GetValue();
	swprintf(str, L"%f", value);
	sliderControl->SetString(str);*/
}


void CSlider::SetSize(double pWidth, double pHeight, double pDepth)
{
	CLabeledCube::SetSize(pWidth, pHeight, pDepth);

	sliderControl->axis = axis;

	if(axis == 0) {
		sliderControl->maxPos = (width - height) * 0.5;
		sliderControl->SetSize(height - margin, height * ratio - margin, depth - margin);
		sliderControl->SetPosition(sliderControl->maxPos, 0.0, 0.0);
	} else if(axis == 1) {
		sliderControl->maxPos = (height - width) * 0.5;
		sliderControl->SetSize(width - margin, width * ratio - margin, depth - margin);
		sliderControl->SetPosition(0.0, sliderControl->maxPos, 0.0);
	} else {
		sliderControl->maxPos = (depth - height) * 0.5;
		sliderControl->SetSize(width - margin, height * ratio - margin, height - margin);
		sliderControl->SetPosition(0.0, 0.0, sliderControl->maxPos);
	}

	sliderControl->minPos = -sliderControl->maxPos;



//	SetAlignmentH(kLeft);
}


void CSlider::ItemClickUp(CComponent3D* item)
{
	CLabeledCube::ItemClickUp(item);
//	wchar_t	 str[256];

	value = (max - min) * sliderControl->GetValue();
//	swprintf(str, L"%f", value);
//	SetString(str);
}


