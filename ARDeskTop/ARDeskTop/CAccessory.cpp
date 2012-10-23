#include "CAccessory.h"
#include "CMainCursor.h"
#include "CCube.h"
#include "CWristWatch.h"
#include "CPictureViewer.h"
#include "CComponent3D.h"

#include "CReverseCube.h"
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

/*	CReverseCube* rcube = new CReverseCube();
	rcube->Init();
	rcube->SetSize(60.0, 60.0, 60.0);
	rcube->SetPosition(0.0, 0.0, -30.0);
	rcube->SetColor4va(Color::green, 1.0f);
	shape = rcube;

	CWristWatch* watch = new CWristWatch();
	watch->Init();
	watch->SetSize(50.0, 80.0, 30.0);
	watch->SetPosition(0.0, 0.0, 15.0);
	shape = watch;*/
}


void CAccessory::Precompute(void)
{
	CProjection::Precompute();
}

