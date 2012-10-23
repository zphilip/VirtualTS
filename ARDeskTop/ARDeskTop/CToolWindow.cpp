#include <stdio.h>

#include "CToolWindow.h"
#include "CButton3D.h"
#include "CComponent3D.h"
#include "CMainCursor.h"
#include "CDesktop.h"
#include "CARDeskTop.h"
#include "CUser.h"



CToolWindow::CToolWindow(void)
: CWindow3D()
{
	buttonT = 0;
	buttonR = 0;
	buttonS = 0;
	buttonD = 0;
	buttonM = 0;
	buttonW = 0;
	buttonG = 0;
}


CToolWindow::~CToolWindow()
{
}


void CToolWindow::Init(void)
{
	CWindow3D::Init();

	titleBar->SetString(L"ツール");

	int i = 0;

	buttonT = new CButton3D();
	buttonT->Init();
	buttonT->SetID(i++);
	buttonT->SetButtonMode(CButton3D::kRadioButton);
	buttonT->SetString(L"移動");
	AddObject(buttonT);

	buttonR = new CButton3D();
	buttonR->Init();
	buttonR->SetID(i++);
	buttonR->SetButtonMode(CButton3D::kRadioButton);
	buttonR->SetString(L"回転");
	AddObject(buttonR);

	buttonS = new CButton3D();
	buttonS->Init();
	buttonS->SetID(i++);
	buttonS->SetButtonMode(CButton3D::kRadioButton);
	buttonS->SetString(L"拡大");
	AddObject(buttonS);

	buttonD = new CButton3D();
	buttonD->Init();
	buttonD->SetID(i++);
	buttonD->SetButtonMode(CButton3D::kRadioButton);
	buttonD->SetString(L"落書き");
	AddObject(buttonD);

	buttonM = new CButton3D();
	buttonM->Init();
	buttonM->SetID(i++);
	buttonM->SetButtonMode(CButton3D::kRadioButton);
	buttonM->SetString(L"マルチ");
	AddObject(buttonM);

	buttonW = new CButton3D();
	buttonW->Init();
	buttonW->SetID(i++);
	buttonW->SetButtonMode(CButton3D::kRadioButton);
	buttonW->SetString(L"ツイスト");
	AddObject(buttonW);

	buttonG = new CButton3D();
	buttonG->Init();
	buttonG->SetID(i++);
	buttonG->SetButtonMode(CButton3D::kRadioButton);
	buttonG->SetString(L"ミサイル");
	AddObject(buttonG);

	buttonT->Select();
}


void CToolWindow::SetSize(double pWidth, double pHeight, double pDepth)
{
	CWindow3D::SetSize(pWidth, pHeight, pDepth);

	double	yPos;

	yPos = (height - TITLEBAR_HEIGHT) * 0.5 - WINDOW_MARGIN;

	buttonT->SetSize(70.0, TITLEBAR_HEIGHT, TITLEBAR_HEIGHT);
	yPos -= TITLEBAR_HEIGHT + WINDOW_MARGIN;
	buttonT->SetPosition(0.0, yPos, 0.0);

	buttonR->SetSize(70.0, TITLEBAR_HEIGHT, TITLEBAR_HEIGHT);
	yPos -= TITLEBAR_HEIGHT + WINDOW_MARGIN;
	buttonR->SetPosition(0.0, yPos, 0.0);

	buttonS->SetSize(70.0, TITLEBAR_HEIGHT, TITLEBAR_HEIGHT);
	yPos -= TITLEBAR_HEIGHT + WINDOW_MARGIN;
	buttonS->SetPosition(0.0, yPos, 0.0);

	buttonD->SetSize(70.0, TITLEBAR_HEIGHT, TITLEBAR_HEIGHT);
	yPos -= TITLEBAR_HEIGHT + WINDOW_MARGIN;
	buttonD->SetPosition(0.0, yPos, 0.0);

	buttonM->SetSize(70.0, TITLEBAR_HEIGHT, TITLEBAR_HEIGHT);
	yPos -= TITLEBAR_HEIGHT + WINDOW_MARGIN;
	buttonM->SetPosition(0.0, yPos, 0.0);

	buttonW->SetSize(70.0, TITLEBAR_HEIGHT, TITLEBAR_HEIGHT);
	yPos -= TITLEBAR_HEIGHT + WINDOW_MARGIN;
	buttonW->SetPosition(0.0, yPos, 0.0);

	buttonG->SetSize(70.0, TITLEBAR_HEIGHT, TITLEBAR_HEIGHT);
	yPos -= TITLEBAR_HEIGHT + WINDOW_MARGIN;
	buttonG->SetPosition(0.0, yPos, 0.0);
}


void CToolWindow::ItemDown(CComponent3D* item)
{
	int mode = CUser::kTranslate;

	CWindow3D::ItemDown(item);

	if(item == buttonT)			//移動
		mode = CUser::kTranslate;
	else if(item == buttonR)	//回転
		mode = CUser::kRotateA;
	else if(item == buttonS)	//拡大
		mode = CUser::kScale;
	else if(item == buttonD)	//落書き
		mode = CUser::kDraw;
	else if(item == buttonM)	//マルチ
		mode = CUser::kMulti;
	else if(item == buttonW)	//ツイスト
		mode = CUser::kTwist;
	else if(item == buttonG)	//ミサイル
		mode = CUser::kGame;

	CARDeskTop::arDeskTop->user->SetCursorMode(mode);
}


void CToolWindow::Reset(void)
{
	buttonT->Unselect();
	buttonR->Unselect();
	buttonS->Unselect();
	buttonD->Unselect();
	buttonM->Unselect();
	buttonW->Unselect();
	buttonG->Unselect();
}

