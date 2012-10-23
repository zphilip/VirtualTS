#include <stdio.h>

#include "CPictureViewer.h"
#include "CCVPicture.h"
#include "CButton3D.h"



CPictureViewer::CPictureViewer(void)
: CFrame()
{
	picture			= 0;
	prevPicture		= 0;
	nextPicture		= 0;
	rotatePicture	= 0;
	rotate			= 0;
}


CPictureViewer::~CPictureViewer()
{
}


void CPictureViewer::Init(void)
{
	CFrame::Init();

	frame->SetAlpha(0.0);

	picture			= new CCVPicture();
	picture->Init();
	AddObject(picture);

	rotatePicture	= new CButton3D();
	rotatePicture->Init();
	rotatePicture->lockMode	= CButton3D::kButtonUnlock;
	rotatePicture->SetID(0);
	rotatePicture->SetString(L"‰ñ");
	AddObject(rotatePicture);

/*	prevPicture		= new CButton3D();
	prevPicture->SetID(0);
	AddObject(prevPicture);

	nextPicture		= new CButton3D();
	nextPicture->SetID(1);
	AddObject(nextPicture);*/
}


void CPictureViewer::SetSize(double pWidth, double pHeight, double pDepth)
{
	CFrame::SetSize(pWidth, pHeight, pDepth);

	if(rotatePicture) {
		rotatePicture->SetSize(15.0, 15.0, depth);
		rotatePicture->SetPosition((width - 15.0) * 0.5, -(height - 15.0) * 0.5, 0.0);
	}
}


void CPictureViewer::ItemDown(CComponent3D* item)
{
	CFrame::ItemDown(item);
	
	if(item == rotatePicture && picture) {
	//	picture->Rotate(-90.0, 0.0, 0.0, 1.0);
		rotate = (rotate + 1) % 4;
		FitPicture();
	}
}


void CPictureViewer::OpenPicture(wchar_t* fileName)
{
	if(!picture) return;

	picture->OpenFile(fileName);
	rotate = 0;
	FitPicture();
}


void CPictureViewer::FitPicture(void)
{
	double widthRate, heightRate, minRate;

	if(rotate % 2 == 0) {
		widthRate	= width / picture->width;
		heightRate	= height / picture->height;
	} else {
		widthRate	= height / picture->width;
		heightRate	= width / picture->height;
	}
	minRate		= widthRate < heightRate ? widthRate : heightRate;

	LoadIdentity(picture->matrix);
	picture->SetSize(minRate, minRate, 1.0);
	if(rotate) picture->Rotate(-90.0 * rotate, 0.0, 0.0, 1.0);
	picture->SetPosition(0.0, 0.0, -depth * 0.5);
}
