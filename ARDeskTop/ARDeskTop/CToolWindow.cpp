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

	titleBar->SetString(L"�c�[��");

	int i = 0;

	buttonT = new CButton3D();
	buttonT->Init();
	buttonT->SetID(i++);
	buttonT->SetButtonMode(CButton3D::kRadioButton);
	buttonT->SetString(L"�ړ�");
	AddObject(buttonT);

	buttonR = new CButton3D();
	buttonR->Init();
	buttonR->SetID(i++);
	buttonR->SetButtonMode(CButton3D::kRadioButton);
	buttonR->SetString(L"��]");
	AddObject(buttonR);

	buttonS = new CButton3D();
	buttonS->Init();
	buttonS->SetID(i++);
	buttonS->SetButtonMode(CButton3D::kRadioButton);
	buttonS->SetString(L"�g��");
	AddObject(buttonS);

	buttonD = new CButton3D();
	buttonD->Init();
	buttonD->SetID(i++);
	buttonD->SetButtonMode(CButton3D::kRadioButton);
	buttonD->SetString(L"������");
	AddObject(buttonD);

	buttonM = new CButton3D();
	buttonM->Init();
	buttonM->SetID(i++);
	buttonM->SetButtonMode(CButton3D::kRadioButton);
	buttonM->SetString(L"�}���`");
	AddObject(buttonM);

	buttonW = new CButton3D();
	buttonW->Init();
	buttonW->SetID(i++);
	buttonW->SetButtonMode(CButton3D::kRadioButton);
	buttonW->SetString(L"�c�C�X�g");
	AddObject(buttonW);

	buttonG = new CButton3D();
	buttonG->Init();
	buttonG->SetID(i++);
	buttonG->SetButtonMode(CButton3D::kRadioButton);
	buttonG->SetString(L"�~�T�C��");
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

	if(item == buttonT)			//�ړ�
		mode = CUser::kTranslate;
	else if(item == buttonR)	//��]
		mode = CUser::kRotateA;
	else if(item == buttonS)	//�g��
		mode = CUser::kScale;
	else if(item == buttonD)	//������
		mode = CUser::kDraw;
	else if(item == buttonM)	//�}���`
		mode = CUser::kMulti;
	else if(item == buttonW)	//�c�C�X�g
		mode = CUser::kTwist;
	else if(item == buttonG)	//�~�T�C��
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

