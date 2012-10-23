#include <stdio.h>

#include "CAccViewer.h"
#include "CCVVideoFrame.h"


CAccViewer*	CAccViewer::accViewer = 0;


CAccViewer::CAccViewer(void)
: CAccessory()
{
	accViewer	= this;
	viewer		= 0;
}


CAccViewer::~CAccViewer()
{
}


void CAccViewer::Init(void)
{
	CAccessory::Init();

	return;

	viewer = new CCVVideoFrame();
	viewer->Init();
	viewer->OpenFile(L"Data/NO_DATA.PNG");
//	viewer->OpenVideoFile(L"angel.mpeg");
	viewer->SetSize(140.0, 140.0, 40.0);
	shape		= viewer;
	drawCard	= false;
	viewer->OpenComment("Book1.csv");


/*	viewer		= new CPictureViewer();
	viewer->Init();
	viewer->SetSize(cardRect[0][0] - cardRect[3][0], cardRect[0][1] - cardRect[1][1], 15.0);
	viewer->SetPosition(0.0, 0.0, 7.5);
	shape		= viewer;
	drawCard	= false;*/
}


void CAccViewer::Precompute(void)
{
	CAccessory::Precompute();
}

