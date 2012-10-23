#include <stdio.h>

#include "CTrash.h"
#include "CAnimeManager.h"
#include "Color.h"

//CComponent3D* CTrash::dust = 0;
CList CTrash::dust;


CTrash::CTrash(void)
: CLabeledCube()
{
}


CTrash::~CTrash()
{
}


void CTrash::Init(void)
{
	CLabeledCube::Init();

	SetAlignmentH(CLabel3D::kCenter);
//	SetColor3a(Color::white, 0.5f);
//	frame->SetCurrentColor(color);
	SetString(L"‚²‚Ý” ");
}


void CTrash::DropFrom(CComponent3D* object, CMainCursor* cursor)
{
//	dust = object;
//	dust->visible = false;

	dust.Add(object);
	object->visible = false;
}


void CTrash::Erase(void)
{
	dust.DeleteAll();
/*	if(dust) {
		CTrash::dust->Delete();
		CTrash::dust = 0;
	}*/
}
