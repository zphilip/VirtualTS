#include "CDesktop.h"
#include "CWorld.h"
#include "CARDeskTop.h"


CDesktop* CDesktop::desktop = 0;


CDesktop::CDesktop(void)
: CProjection()
{
	world	= 0;
}


CDesktop::~CDesktop()
{
}


void CDesktop::Init(void)
{
	CProjection::Init();

	world = new CWorld();
	world->Init();

//	world->Rotate(90.0, 1.0, 0.0, 0.0);

	shape = world;
}


void CDesktop::Draw2(void)
{
	if(shape)
		if(shape->visible) shape->Draw();
}


void CDesktop::SortByDepth(void)
{
	if(world && visible) world->SortByDepth(this);
}


