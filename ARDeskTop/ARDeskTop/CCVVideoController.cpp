#include "CCVVideoController.h"

#include "CButton3D.h"
#include "CCVVideoManager.h"
#include "Color.h"
#include "CCVPictureFrame.h"
#include "CCVVideoFrame.h"
#include "CLabel3D.h"
#include "CWall.h"




CCVVideoController::CCVVideoController(void)
: CFrame()
{
	pictureFrame	= 0;
	play			= 0;
	pause			= 0;
	stop			= 0;
	blueBack		= 0;
	volume			= 0;
	checkWithinBBOnly	= false;
}


CCVVideoController::~CCVVideoController()
{
}


void CCVVideoController::Init(void)
{
	CFrame::Init();

//	frame->SetColor4(1.0f, 1.0f, 1.0f, 0.2f);
//	frame->cardMode = kRound;
//	frame->wire = true;

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


void CCVVideoController::SetSize(double pWidth, double pHeight, double pDepth)
{
	CFrame::SetSize(pWidth, pHeight, pDepth);

	double pos[3];
	double btnWidth, btnHeight, d;
	double width2;

	btnWidth	= 15.0;
	btnHeight	= 15.0;

	width2 = (width - btnWidth * 5.0) / 6.0;
	d = btnWidth + width2;
	pos[0] = -width * 0.5 + btnWidth * 0.5 + width2;
	pos[1] = 0.0;
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


void CCVVideoController::ItemDown(CComponent3D* item)
{
	if(!pictureFrame) return;

	CCVVideoManager* mana = (CCVVideoManager*) pictureFrame->picture->animeManager;
	if(!mana) return;

	CFrame::ItemDown(item);

	if(item == play) {
		Play();
		Animate();
	} else if(item == blueBack) {
		pictureFrame->picture->bluebackMode = !blueBack->locked;
	} else if(item == pause) {
		Pause();
	} else if(item == stop) {
		Stop();
	}
}


void CCVVideoController::Reset(void)
{
	play->Unselect();
	pause->Unselect();
	stop->Unselect();
}


void CCVVideoController::Play(void)
{
	pictureFrame->Play();
}


void CCVVideoController::Replay(void)
{
	pictureFrame->Replay();
}


void CCVVideoController::Stop(void)
{
	pictureFrame->Stop();
}
	

void CCVVideoController::Pause(void)
{
	pictureFrame->Pause();
}


void CCVVideoController::Animate(void)
{
	pictureFrame->Animate();
}

