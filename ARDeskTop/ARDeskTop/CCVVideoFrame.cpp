#include <stdio.h>
#include <math.h>
#include <dshow.h>

#include "CCVVideoFrame.h"
#include "CCVVideoManager.h"
#include "Color.h"



CVolumeFader::CVolumeFader(void)
: CSlider()
{
	oriPos			= 0.0;
	notifyParent	= true;
}


CVolumeFader::~CVolumeFader()
{
}


void CVolumeFader::Init(void)
{
	CSlider::Init();

	max = 0.0;
	min = -10000.0;

	frame->wire = true;
	frame->SetColor4va(Color::white, 0.0f);

	sliderControl->frame->wire = false;
	sliderControl->frame->cardMode = CCube::kRound;
	sliderControl->frame->SetColor4va(Color::cyan, 0.5f);
	sliderControl->label->SetAlignmentH(CLabel3D::kCenter);
	sliderControl->label->SetColor3v(Color::white);
	sliderControl->label->SetString(L"‰¹—Ê");
}


void CVolumeFader::ItemDown(CComponent3D* item)
{
	CSlider::ItemDown(item);

	frame->SetAlpha(0.5f);
}


void CVolumeFader::ItemUp(CComponent3D* item)
{
	CSlider::ItemUp(item);

	frame->SetAlpha(0.0f);
}


void CVolumeFader::ItemClickDown(CComponent3D* item)
{
	CSlider::ItemClickDown(item);
}


void CVolumeFader::ItemClickUp(CComponent3D* item)
{
	CSlider::ItemClickUp(item);

	//ƒ|ƒWƒVƒ‡ƒ“C³
	double pos[3];
	GetPosition(pos);
	pos[axis] = oriPos - sliderControl->matrix[12 + axis];
	SetPosition(pos);

	CCVVideoManager* mana = (CCVVideoManager*) ((CCVVideoFrame*) parent)->picture->animeManager;

	if(mana->pBasicAudio) {
		int vol;
		vol = -10000 * (pow(10.0, 1.0 - sliderControl->GetValue()) - 1.0) / 9.0;
		mana->pBasicAudio->put_Volume(vol);
	}
}






CCVVideoFrame::CCVVideoFrame(void)
: CCVPictureFrame()
{
	play		= 0;
	pause		= 0;
	stop		= 0;
	blueBack	= 0;
	volume		= 0;
	checkWithinBBOnly	= false;
}


CCVVideoFrame::~CCVVideoFrame()
{
}


void CCVVideoFrame::Init(void)
{
	CCVPictureFrame::Init();

//	frame->SetColor4(1.0f, 1.0f, 1.0f, 0.2f);
//	frame->cardMode = kRound;
	frame->wire = true;

	label->SetColor3v(Color::black);
	label->SetAlignmentH(CLabel3D::kLeft);
	label->SetAlignmentV(CLabel3D::kTop);
	label->marginV = 4.0;
	label->SetString(L"ARDT Media Player");

	play = new CButton3D();
	play->Init();
	play->frame->wire = false;
	play->frame->cardMode = CCube::kRound;
	play->SetButtonMode(CButton3D::kRadioButton);
	play->SetColor4va(Color::white, 0.5f);
	Color::SetColor4va(play->hilightColor, Color::green, 0.5f);
	play->label->SetColor4va(Color::white, 1.0f);
	play->label->SetString(L"Ä¶");
	AddObject(play);

	pause = new CButton3D();
	pause->Init();
	pause->frame->wire = false;
	pause->frame->cardMode = CCube::kRound;
	pause->SetButtonMode(CButton3D::kRadioButton);
	pause->SetColor4va(Color::white, 0.5f);
	Color::SetColor4va(pause->hilightColor, Color::yellow, 0.5f);
	pause->label->SetColor4va(Color::white, 1.0f);
	pause->label->SetString(L"‹xŽ~");
	AddObject(pause);

	stop = new CButton3D();
	stop->Init();
	stop->frame->wire = false;
	stop->frame->cardMode = CCube::kRound;
	stop->SetButtonMode(CButton3D::kRadioButton);
	stop->SetColor4va(Color::white, 0.5f);
	Color::SetColor4va(stop->hilightColor, Color::red, 0.5f);
	stop->label->SetColor4va(Color::white, 1.0f);
	stop->label->SetString(L"’âŽ~");
	AddObject(stop);

	blueBack = new CButton3D();
	blueBack->Init();
	blueBack->frame->wire = false;
	blueBack->frame->cardMode = CCube::kRound;
	blueBack->SetColor4va(Color::white, 0.5f);
	Color::SetColor4va(blueBack->hilightColor, Color::blue, 0.5f);
	blueBack->label->SetColor4va(Color::white, 1.0f);
	blueBack->label->SetString(L"Â”w");
	AddObject(blueBack);

	volume = new CVolumeFader();
	volume->Init();
	volume->fix = true;
	AddObject(volume);

	SetCardMode(true);
}


void CCVVideoFrame::SetSize(double pWidth, double pHeight, double pDepth)
{
	CCVPictureFrame::SetSize(pWidth, pHeight, pDepth);

	double pos[3];
	double btnWidth, btnHeight, d;

	btnWidth	= 15.0;
	btnHeight	= 10.0;

	d = btnWidth + 4.0;
	pos[0] = -d * 2.0;
	pos[1] = -height * 0.5 + 6.5;
	pos[2] = 0.0;
	play->SetSize(btnWidth, btnHeight, depth);
	play->SetPosition(pos);

	pos[0] += d;
	pause->SetSize(btnWidth, btnHeight, depth);
	pause->SetPosition(pos);

	pos[0] += d;
	stop->SetSize(btnWidth, btnHeight, depth);
	stop->SetPosition(pos);

	pos[0] += d;
	blueBack->SetSize(btnWidth, btnHeight, depth);
	blueBack->SetPosition(pos);

	pos[0] += d;
	volume->axis = 1;
	volume->ratio = btnHeight / btnWidth;
	volume->SetSize(btnWidth, btnHeight * 5.0, depth);
	volume->oriPos = pos[volume->axis];
	pos[1] -= volume->sliderControl->maxPos;
	volume->SetPosition(pos);
}


void CCVVideoFrame::Precompute(void)
{
	CCVPictureFrame::Precompute();

	if(commentManager) {
		commentManager->Animate();
	}
}


void CCVVideoFrame::ItemDown(CComponent3D* item)
{
	if(!picture) return;

	CCVVideoManager* mana = (CCVVideoManager*) picture->animeManager;
	if(!mana) return;

	CCVPictureFrame::ItemDown(item);

	if(item == play) {
		Play();
		Animate();
	} else if(item == blueBack) {
		picture->bluebackMode = !blueBack->locked;
	} else if(item == pause) {
		Pause();
	} else if(item == stop) {
		Stop();
	}
}


void CCVVideoFrame::Reset(void)
{
	play->Unselect();
	pause->Unselect();
	stop->Unselect();
}

