#include <stdio.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CRegion.h"
#include "Color.h"



double CRegion::buttonSize	= 12.0;



CRegion::CRegion(void)
: CGroup()
{
	SetOrigin(0.0, 0.0);

	label		= 0;
	region		= 0;
	resize		= 0;
	resizing	= false;
}


CRegion::~CRegion()
{
	objectList2.DeleteAll();
}


void CRegion::Init(void)
{
	CGroup::Init();

	region		= new CCube();
	region->Init();
	region->SetCardMode(true);
	region->cardMode = CCube::kRect;
	region->wire = true;
	region->fix = true;
	region->notifyParent = true;
	region->SetColor4va(Color::blue, 1.0f);
	AddObject(region);


	label		= new CLabel3D();
	label->Init();
	label->SetCardMode(true);
	label->SetString(L"TEST");
	label->SetAlignmentH(CLabel3D::kCenter);
	label->SetAlignmentV(CLabel3D::kCenter);
	label->SetColor4va(Color::green, 1.0f);
	AddObject(label);


	resize		= new CResizeBox();
	resize->Init();
	resize->SetCardMode(true);
//	resize->mode = CResizeBox::kBottomRight;
//	resize->mode = CResizeBox::kBottomLeft;
	resize->label->SetColor4va(Color::green, 1.0f);
	resize->label->SetString(L" ");
	resize->label->SetAlignmentH(CLabel3D::kCenter);
	resize->label->SetAlignmentV(CLabel3D::kCenter);
	resize->frame->cardMode	= CCube::kRect;
	resize->frame->SetColor4va(Color::white, 0.5f);
	resize->fix	= false;
	AddObject(resize);
}


void CRegion::Precompute(void)
{
	if(!resizing) {
		CGroup::Precompute();
		objectList2.Precompute();
	}
}


void CRegion::DrawShape(void)
{
	if(resizing) {
		resize->Resize(this);
		CGroup::Precompute();
		objectList2.Precompute();
	}

	CGroup::DrawShape();
	objectList2.Draw();
}


void CRegion::SetSize(double pWidth, double pHeight, double pDepth)
{
	CGroup::SetSize(pWidth, pHeight, pDepth);

	label->SetSize(pWidth, buttonSize * 2.0, pWidth);
	label->SetPosition(orig[0] + pWidth * 0.5, orig[1] - pHeight + buttonSize, 0.0);

	region->SetSize(pWidth, pHeight, pDepth);
	region->SetPosition(orig[0] + pWidth * 0.5, orig[1] - pHeight * 0.5, 0.0);

	if(!resizing) {
	//	resize->SetSize(buttonSize, buttonSize, pDepth);
	//	resize->SetPosition(orig[0] + buttonSize, orig[1] - pHeight + buttonSize, 0.0); // kBottomLeft
	//	resize->SetPosition(orig[0] + pWidth - buttonSize, orig[1] - buttonSize, 0.0); // kTopRight
	//	resize->SetPosition(orig[0] + buttonSize, orig[1] - buttonSize, 0.0); // kTopLeft
	//	resize->SetPosition(orig[0] + pWidth - buttonSize, orig[1] - pHeight + buttonSize, 0.0); // kBottomRight
	}
}


void CRegion::ItemClickDown(CComponent3D* item)
{
	if(item == resize) {
		resizing = true;
	}
}


void CRegion::ItemClickUp(CComponent3D* item)
{
	if(item == resize) {
		resizing = false;
	}

}


void CRegion::SetOrigin(double x, double y)
{
	orig[0] = x;
	orig[1] = y;
}


void CRegion::AddObject2(CComponent3D* object)
{
	object->SetParent(this);
	objectList2.Add(object);
}

