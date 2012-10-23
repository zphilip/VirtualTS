#include "CLabeledCube.h"


CLabeledCube::CLabeledCube(void)
: CFrame()
{
	label	= 0;
	checkWithinBBOnly	= true;
}


CLabeledCube::~CLabeledCube()
{
}


void CLabeledCube::Init(void)
{
	CFrame::Init();

	label	= new CLabel3D();
	label->Init();
	AddObject(label);
}


void CLabeledCube::SetSize(double pWidth, double pHeight, double pDepth)
{
	CFrame::SetSize(pWidth, pHeight, pDepth);

	if(label) label->SetSize(pWidth, pHeight, pDepth);
}


int CLabeledCube::SetFont(CFont* font)
{
	if(label) {
		label->SetFont(font);
		return(0);
	}

	return(1);
}


void CLabeledCube::SetAlignmentH(int pAlignment)
{
	if(label) label->SetAlignmentH(pAlignment);
}


void CLabeledCube::SetString(wchar_t c)
{
	if(label) label->SetString(c);
}


void CLabeledCube::SetString(wchar_t* string)
{
	if(label) label->SetString(string);
}


void CLabeledCube::SetString(char* string)
{
	if(label) label->SetString(string);
}

