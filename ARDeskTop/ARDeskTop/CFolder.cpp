#include <stdio.h>

#include "CFolder.h"
#include "CItemList.h"



CFolder::CFolder(void)
: CRegion()
{
	itemList	= 0;
}


CFolder::~CFolder()
{
}


void CFolder::Init(void)
{
	CRegion::Init();

	resize->mode = CResizeBox::kBottomRight;

	itemList	= new CItemList();
	itemList->Init();
	itemList->SetSize(10.0, 20.0, 20.0);
	itemList->SetOrigin(0.0, 20.0);
	itemList->Rotate(90.0, 1.0, 0.0, 0.0);
	AddObject(itemList);
}


void CFolder::SetSize(double pWidth, double pHeight, double pDepth)
{
	CRegion::SetSize(pWidth, pHeight, pDepth);

	double size[3];

	itemList->GetSize(size);
	itemList->SetSize(pWidth, size[1], CRegion::buttonSize);
//	itemList->SetPosition(orig[0], orig[1] - CRegion::buttonSize * 0.5, 0.0);
	itemList->SetPosition(orig[0], orig[1], 0.0);

	if(!resizing) {
		resize->SetSize(CRegion::buttonSize, CRegion::buttonSize, pDepth);
		resize->SetPosition(orig[0] + pWidth - CRegion::buttonSize, orig[1] - pHeight + CRegion::buttonSize, 0.0); // kBottomRight
	}
}


CComponent3D* CFolder::IsWithin(double x, double y, double z)
{
	if(!touchable || clickedCursor || !visible) return(0);

	double newPos[3];
	CComponent3D* hit = 0;
	CComponent3D* item;

	InvertPos(x, y, z, newPos);
//	if(boundingBox)
//		hit = boundingBox->IsWithin(newPos);
//	withinBB = withinBB || hit;

	hit = objectList.IsWithin(newPos);
	if(hit == region || !hit) {
		item = objectList2.IsWithin(newPos);
		if(item) hit = item;
	}

	return(hit);
}