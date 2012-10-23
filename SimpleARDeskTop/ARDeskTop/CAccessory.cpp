#include "CAccessory.h"
#include "CMainCursor.h"
#include "CCube.h"
#include "CComponent3D.h"

#include "Color.h"



CAccessory::CAccessory(void)
: CProjection()
{
	drawCard	= false;
}


CAccessory::~CAccessory()
{
}


void CAccessory::Init(void)
{
	CProjection::Init();
}


void CAccessory::Precompute(void)
{
	CProjection::Precompute();
}

