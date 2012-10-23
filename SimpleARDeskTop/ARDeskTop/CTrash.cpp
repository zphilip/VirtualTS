#include <stdio.h>

#include "CTrash.h"
#include "CAnimeManager.h"
#include "Color.h"

//CComponent3D* CTrash::dust = 0;
CList CTrash::dust;


CTrash::CTrash(void)
: CCube()
{
}


CTrash::~CTrash()
{
}


void CTrash::Init(void)
{
	CCube::Init();
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
