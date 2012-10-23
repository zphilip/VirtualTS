#include "CFrame.h"



CFrame::CFrame(void)
: CGroup()
{
	frame	= 0;
}


CFrame::~CFrame()
{
}


void CFrame::Init(void)
{
	CGroup::Init();

	frame = new CFrameCube();
	frame->Init();
	frame->fix = true;
	frame->SetColor4(1.0f, 1.0f, 1.0f, 0.2f);
	AddObject(frame);

	boundingBox = frame;
}
/*

void CFrame::Precompute(void)
{
	CGroup::Precompute();
}


CComponent3D* CFrame::IsWithin(double x, double y, double z)
{
	return(CGroup::IsWithin(x, y, z));

	if(!touchable || clickedCursor || !visible) return(0);
	if(!frame) return(0);

	double newPos[3];
	CComponent3D* hit;

	InvertPos(x, y, z, newPos);
	hit = frame->IsWithin(newPos);
	withinBB = withinBB || hit;

	if(hit) {
		if(checkWithinBBOnly) {
			hit = this;
		} else {
			CComponent3D* item;
			
			frame->touchable = false;
			item = objectList.IsWithin(newPos);
			if(item) hit = item;
			frame->touchable = true;
		}
	}

	return(hit);
}


void CFrame::SetSize(double pWidth, double pHeight, double pDepth)
{
	width	= pWidth;
	height	= pHeight;
	depth	= pDepth;

	if(frame)
		frame->SetSize(width, height, depth);
}
*/