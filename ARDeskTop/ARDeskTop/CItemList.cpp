#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
#	include <windows.h>
#endif

#include "CItemList.h"
#include "Color.h"


CList	CTag::tagList;

CFont	CItem::fontBig;

double	CItemList::lineHeight = 12.0;
CGroup*	CItemList::focus = 0;




void CTag::AddTag(wchar_t* tag)
{
}





CItem::CItem(void)
: CLabel3D()
{
	touchable	= true;
}


CItem::~CItem()
{
}


void CItem::DrawShape(void)
{
	if(!visible) return;

	float a;

	if(CItemList::focus == 0 || CItemList::focus == parent)
		a = 1.0f;
	else
		a = 0.2f;

	SetAlpha(a);

	CLabel3D::DrawShape();
}


void CItem::CursorEnter(CCursor* cursor)
{
	CLabel3D::CursorEnter(cursor);

	SetFont(&fontBig);
//	SetColor3v(Color::red);
	Precompute();
}


void CItem::CursorLeave(CCursor* cursor)
{
	CLabel3D::CursorLeave(cursor);

	SetFont(&CText3D::systemFont);
//	SetColor3v(Color::passionorange);
	Precompute();
}


void CItem::InitItem(void)
{
	fontBig.SetFontSize(13);
	fontBig.Init();
}






CItemList::CItemList(void)
: CRegion()
{
	fix = true;
}


CItemList::~CItemList()
{
}


void CItemList::Init(void)
{
	CRegion::Init();

	resize->mode = CResizeBox::kTopLeft;
	resize->FixAxis(true, false, true);

//	region->visible = false;
//	region->SetAlpha(0.0f);
//	region->fix = true;

	boundingBox = region;

	CItem* item;

	for(int i = 0; i < 10; i++) {
		item = new CItem();
		item->Init();
		item->SetCardMode(true);
		item->SetAlignmentH(CLabel3D::kLeft);
		item->SetAlignmentV(CLabel3D::kCenter);
		item->SetColor4va(Color::passionorange, 0.4f);
		item->SetString(L"ABCDEFG");
		item->notifyParent = true;
		AddObject2(item);
	}
}


void CItemList::SetSize(double pWidth, double pHeight, double pDepth)
{
	CRegion::SetSize(pWidth, pHeight, pDepth);

	if(!resizing) {
		resize->SetSize(buttonSize, buttonSize, pDepth);
		resize->SetPosition(orig[0] + CRegion::buttonSize, orig[1] - CRegion::buttonSize, 0.0); // kTopLeft
	}

	int visibleNum = (int) (pHeight / lineHeight);

	if(visibleNum) {
		CItem*	item;
		int		itemNum = objectList2.GetDegree();
		double	y;

		if(visibleNum > itemNum) visibleNum = itemNum;

		y = lineHeight * 0.5;
		objectList2.GoToFirst();
		for(int i = 0; i < itemNum; i++) {
			item = (CItem*) objectList2.GetKey();

			if(i < visibleNum) {
			//	item->SetSize(width, lineHeight, depth);
				item->SetPosition(orig[0] + CRegion::buttonSize * 2.0 + item->width * 0.5, orig[1] - y, 0.0);
				item->visible = true;
			} else {
				item->visible = false;
			}

			y += lineHeight;
			objectList2.GoToNext();
		}
	}
}


CComponent3D* CItemList::IsWithin(double x, double y, double z)
{
	if(!touchable || clickedCursor || !visible) return(0);

	double newPos[3];
	CComponent3D* hit = 0;
	CComponent3D* item;

	InvertPos(x, y, z, newPos);
	if(boundingBox)
		hit = boundingBox->IsWithin(newPos);
	withinBB = withinBB || hit;

	if(hit) {
		if(checkWithinBBOnly) {
			hit = this;
		} else {
			bool t = boundingBox->touchable;
			
			boundingBox->touchable = false;
			item = objectList.IsWithin(newPos);
			if(item) hit = item;
			else {
				item = objectList2.IsWithin(newPos);
				if(item) hit = item;
			}
			boundingBox->touchable = t;
		}
	}

	return(hit);
}


void CItemList::ItemDown(CComponent3D* item)
{
	CRegion::ItemDown(item);

	if(withinBB) {
		focus	= this;
	}
}


void CItemList::ItemUp(CComponent3D* item)
{
	CRegion::ItemUp(item);

	if(!withinBB) {
		focus	= 0;
	}
}


void CItemList::SetItemSize(double pWidth, double pHeight, double pDepth)
{
	CItem*	item;
	int		itemNum = objectList2.GetDegree();

	objectList2.GoToFirst();
	for(int i = 0; i < itemNum; i++) {
		item = (CItem*) objectList2.GetKey();

		item->SetSize(pWidth, lineHeight, pDepth);

		objectList2.GoToNext();
	}
}


void CItemList::ReadFolder(wchar_t* dirName, bool readDir)
{
#ifdef WIN32
	WIN32_FIND_DATA	fd;
	HANDLE			h;
	wchar_t			fileName[512];

	wcscpy(fileName, dirName);
	wcscat(fileName, L"*");

	h = FindFirstFileEx(fileName, FindExInfoStandard, &fd, FindExSearchNameMatch, NULL, 0);
	if(h == INVALID_HANDLE_VALUE) return;

	do {
		if((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && readDir) {
			if(wcscmp(fd.cFileName, L".") && wcscmp(fd.cFileName, L"..")) {
			//	foo( cnt + 1, dirname + fd.cFileName + "\\" );
			}
		} else {
			wcscpy(fileName, dirName);
			wcscat(fileName, fd.cFileName);

	//		AddObject(photo);
		}
	} while(FindNextFile(h, &fd));

	FindClose(h);
#endif
}


void CItemList::FindItems(wchar_t* s, CItemList* itemList)
{
	//int wcscmp(const wchar_t *s1, const wchar_t *s2);
}


