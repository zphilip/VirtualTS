#include "CWindow3D.h"
#include "CTitleBar.h"
#include "CButton3D.h"
#include "CComponent3D.h"


CWindow3D::CWindow3D(void)
: CFrame()
{
	titleBar = 0;
}


CWindow3D::~CWindow3D()
{
}


void CWindow3D::Init(void)
{
	CFrame::Init();

	titleBar = new CTitleBar();
	titleBar->Init();
//	titleBar->SetSize(width - WINDOW_MARGIN * 2.0, TITLEBAR_HEIGHT, TITLEBAR_HEIGHT);
//	titleBar->SetPosition(0.0, (height - TITLEBAR_HEIGHT) * 0.5 - WINDOW_MARGIN, 0.0);
	AddObject(titleBar);
}


void CWindow3D::SetSize(double pWidth, double pHeight, double pDepth)
{
	CFrame::SetSize(pWidth, pHeight, pDepth);

	titleBar->SetSize(width - WINDOW_MARGIN * 2.0, TITLEBAR_HEIGHT, TITLEBAR_HEIGHT);
	titleBar->SetPosition(0.0, (height - TITLEBAR_HEIGHT) * 0.5 - WINDOW_MARGIN, 0.0);
}



