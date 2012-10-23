#include <stdio.h>

#include "CWall.h"
#include "CBoundingBox.h"
#include "CLabel3D.h"
#include "CCVPictureFrame.h"
#include "CUser.h"
#include "color.h"

CWall* CWall::wall = 0;



CWall::CWall(void)
: CGroup()
{
	modeDisp		= 0;
	ardesktopLogo	= 0;
	wallpaper		= 0;
}


CWall::~CWall()
{
}


void CWall::Init(void)
{
	CGroup::Init();

	boundingBox = new CBoundingBox();
	boundingBox->Init();
	boundingBox->touchable = false;
	boundingBox->SetColor4va(Color::green, 1.0f);
	AddObject(boundingBox);

	modeDisp = new CLabel3D();
	modeDisp->Init();
	modeDisp->touchable = false;
	modeDisp->SetColor4va(Color::green, 1.0f);
	modeDisp->SetAlignmentV(CLabel3D::kTop);
	modeDisp->SetAlignmentH(CLabel3D::kLeft);
	modeDisp->SetMarginV(20.0);
	modeDisp->SetMarginH(20.0);
	AddObject(modeDisp);
	SetCursorMode(CUser::kTranslate);

	ardesktopLogo = new CLabel3D();
	ardesktopLogo->Init();
//	ardesktopLogo->touchable = false;
	ardesktopLogo->SetColor4va(Color::green, 1.0f);
	ardesktopLogo->SetAlignmentV(CLabel3D::kCenter);
	ardesktopLogo->SetAlignmentH(CLabel3D::kCenter);
	ardesktopLogo->SetMarginV(0.0);
	ardesktopLogo->SetMarginH(20.0);
	ardesktopLogo->SetString(L"ARDeskTop");
	ardesktopLogo->Rotate(-90.0, 0.0, 0.0, 1.0);
	ardesktopLogo->Rotate(90.0, 0.0, 1.0, 0.0);
	ardesktopLogo->Scale(5.0, 5.0, 1.0);
	AddObject(ardesktopLogo);


/*	wallpaper = new CCVPictureFrame();
	wallpaper->Init();
	wallpaper->touchable = false;
	wallpaper->SetCardMode(true);
	wallpaper->frame->SetAlpha(0.0f);

	wallpaper->OpenVideoFile(L"angel.mpeg");
	wallpaper->OpenComment("Book1.csv");

	AddObject(wallpaper);*/
}


void CWall::SetSize(double pWidth, double pHeight, double pDepth)
{
	CGroup::SetSize(pWidth, pHeight, pDepth);

	boundingBox->SetSize(width, height, depth);
	boundingBox->SetPosition(0.0, 0.0, depth * 0.5);

	modeDisp->SetPosition(-width * 0.5, height * 0.5, 0.0);

	ardesktopLogo->SetPosition(-width * 0.5, 0.0, depth * 0.5);

//	wallpaper->SetSize(width, height, 10.0);
//	wallpaper->SetPosition(0.0, 0.0, 0.0);
}


void CWall::SetCursorMode(int mode)
{
	if(!modeDisp) return;

	switch(mode) {
	case CUser::kTranslate :
		modeDisp->SetString(L"curosr mode: translate");
		break;
	case CUser::kRotateA :
		modeDisp->SetString(L"curosr mode: rotate");
		break;
	case CUser::kScale :
		modeDisp->SetString(L"curosr mode: scale");
		break;
	case CUser::kMulti :
		modeDisp->SetString(L"curosr mode: multi");
		break;
	case CUser::kTwist :
		modeDisp->SetString(L"curosr mode: twist");
		break;
	case CUser::kDraw:
		modeDisp->SetString(L"curosr mode: draw");
		break;
	}
}

