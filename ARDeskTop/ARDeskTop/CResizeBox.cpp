#include "CResizeBox.h"
#include "CRegion.h"



CResizeBox::CResizeBox(void)
: CButton3D()
{
	dragging	= false;
	lockMode	= kButtonUnlock;

	FixAxis(false, false, true);
}


CResizeBox::~CResizeBox()
{
}


void CResizeBox::ClickDown(CMainCursor* cursor)
{
	CButton3D::ClickDown(cursor);
//	dragging = true;

	GetPosition(firstPos);

	parent->GetSize(regionSize);
}


/*
void CResizeBox::ClickUp(CMainCursor* cursor)
{
	CButton3D::ClickUp(cursor);
	dragging = false;
}
*/


void CResizeBox::Resize(CRegion* region)
{
	double pos[3];

	GetPosition(pos);

	switch(mode) {
	case kTopLeft :
		region->SetOrigin(pos[0] - CRegion::buttonSize, pos[1] + CRegion::buttonSize);
		region->SetSize(
			regionSize[0] - pos[0] + firstPos[0],
			regionSize[1] + pos[1] - firstPos[1],
			regionSize[2]
		);
		break;
	case kTopRight :
		region->orig[1] = pos[1] + CRegion::buttonSize;
		region->SetSize(
			regionSize[0] + pos[0] - firstPos[0],
			regionSize[1] + pos[1] - firstPos[1],
			regionSize[2]
		);
		break;
	case kBottomLeft :
		pos[1] -= CRegion::buttonSize;
		region->orig[0] = pos[0] - CRegion::buttonSize;
		region->SetSize(
			regionSize[0] - pos[0] + firstPos[0],
			-pos[1] + region->orig[1],
			regionSize[2]
		);
		break;
	case kBottomRight :
		pos[0] += CRegion::buttonSize;
		pos[1] -= CRegion::buttonSize;
		region->SetSize(
			pos[0] - region->orig[0],
			-pos[1] + region->orig[1],
			regionSize[2]
		);

		break;
	}
}

