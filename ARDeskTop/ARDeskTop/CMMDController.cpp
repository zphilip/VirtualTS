#include <math.h>
#include <stdio.h>
#include <wchar.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CMMDController.h"
#include "CMMDObject.h"
#include "CMMDMotionManager.h"
#include "CLabel3D.h"
#include "CCube.h"
#include "PI.h"
#include "Color.h"


CMMDController* CMMDController::mmdController = 0;




CJogWheel::CJogWheel(void)
: CButton3D()
{
	wheel	= 0;
	divNum	= 4;
}


CJogWheel::~CJogWheel()
{
	if(wheel) delete wheel;
}


void CJogWheel::Init(void)
{
	CButton3D::Init();

	frame->cardMode = CCube::kCircle1;
	wheel	= new int[divNum * 4];
}


CComponent3D* CJogWheel::IsWithin(double x, double y, double z)
{
	if(!touchable || clickedCursor || !visible) return(0);

	InvertPos(x, y, z, pos);

	if(boundingBox)
		return(boundingBox->IsWithin(pos) ? this : 0);
	else 
		return(	-0.5 <= pos[0] && pos[0] <= 0.5 &&
				-0.5 <= pos[1] && pos[1] <= 0.5 &&
				-0.5 <= pos[2] && pos[2] <= 0.5 ? this : 0);
}


void CJogWheel::CursorEnter(CCursor* cursor)
{
	CButton3D::CursorEnter(cursor);

	index	= CheckWheelArea();
	UpDataWheel(index);
}


void CJogWheel::CursorWithin(CCursor* cursor)
{
	CButton3D::CursorWithin(cursor);

	prevIndex	= index;
	index		= CheckWheelArea();

	if(prevIndex != index) {
		if(CMMDController::mmdController) {
			CMMDController::mmdController->Skip(wheel[index] / 30.0f);
		}

		UpDataWheel(index);
	}
}


int CJogWheel::CheckWheelArea(void)
{
	double angle;

	if(-0.001 < pos[0] && pos[0] < 0.001) {
		angle = pos[1] >= 0 ? 90.0 : 270.0;
	} else {
		angle = atan(pos[1] / pos[0]) * 180.0 / PI;
		
		if(pos[0] < 0.0) angle += 180.0;
		else if(pos[1] < 0.0) angle += 360.0;
	}

	return((int)(divNum * angle / 90.0));
}


void CJogWheel::UpDataWheel(int id)
{
	int x = 0;

	for(int i = 0; i < divNum * 4; i++) {
		if(i  == divNum * 2) x  = divNum * 2;
		if(id == divNum * 4) id = 0;
		wheel[id] = x;

		x--;
		id++;
	}
}





CMMDController::CMMDController(void)
: CFrame()
{
	mmdObject	= 0;

	jogWheel	= 0;
	counter		= 0;
	play		= 0;
	stop		= 0;
//	pause		= 0;

	mmdController	= this;
}


CMMDController::~CMMDController()
{
}


void CMMDController::Init(void)
{
	CFrame::Init();

	jogWheel = new CJogWheel();
	jogWheel->Init();
	jogWheel->SetButtonMode(CButton3D::kRadioButton);
	jogWheel->SetColor4va(Color::white, 0.5f);
	Color::SetColor4va(jogWheel->hilightColor, Color::yellow, 0.5f);
	AddObject(jogWheel);

	counter	= new CLabel3D();
	counter->Init();
	counter->SetColor4va(Color::white, 1.0f);
	counter->SetAlignmentH(CLabel3D::kRight);
	counter->SetAlignmentV(CLabel3D::kCenter);
	AddObject(counter);


	play = new CButton3D();
	play->Init();
	play->frame->wire = false;
	play->frame->cardMode = CCube::kRect;
	play->SetButtonMode(CButton3D::kRadioButton);
	play->SetColor4va(Color::white, 0.5f);
	Color::SetColor4va(play->hilightColor, Color::green, 0.5f);
	play->label->SetColor4va(Color::white, 1.0f);
	play->label->SetString(L"Ä¶");
	AddObject(play);

/*	pause = new CButton3D();
	pause->Init();
	pause->frame->wire = false;
	pause->frame->cardMode = CCube::kRect;
	pause->SetButtonMode(CButton3D::kRadioButton);
	pause->SetColor4va(Color::white, 0.5f);
	Color::SetColor4va(pause->hilightColor, Color::yellow, 0.5f);
	pause->label->SetColor4va(Color::white, 1.0f);
	pause->label->SetString(L"‹xŽ~");
	AddObject(pause);
*/
	stop = new CButton3D();
	stop->Init();
	stop->frame->wire = false;
	stop->frame->cardMode = CCube::kRect;
	stop->SetButtonMode(CButton3D::kRadioButton);
	stop->SetColor4va(Color::white, 0.5f);
	Color::SetColor4va(stop->hilightColor, Color::red, 0.5f);
	stop->label->SetColor4va(Color::white, 1.0f);
	stop->label->SetString(L"’âŽ~");
	AddObject(stop);
}


void CMMDController::SetSize(double pWidth, double pHeight, double pDepth)
{
	CFrame::SetSize(pWidth, pHeight, pDepth);

	jogWheel->SetSize(width * 0.5, width * 0.5, depth);

	double pos[3];
	double btnWidth, btnHeight;

	btnWidth	= 15.0;
	btnHeight	= 10.0;

	pos[0] = -width * 0.5 + btnWidth;
	pos[1] = 0.0;
	pos[2] = 0.0;
	play->SetSize(btnWidth, btnHeight * 4.0, depth);
	play->SetPosition(pos);

	pos[0] *= -1.0;
	stop->SetSize(btnWidth, btnHeight * 4.0, depth);
	stop->SetPosition(pos);

	pos[0] = 0.0;
	pos[1] = height * 0.5 - btnHeight;
	counter->SetSize(width, height, depth);
	counter->SetPosition(pos);

/*	pos[0] = -btnWidth;
	pos[1] = height * 0.5 - btnHeight;
	pause->SetSize(btnWidth, btnHeight, depth);
	pause->SetPosition(pos);

	pos[0] *= -1.0;
	stop->SetSize(btnWidth, btnHeight, depth);
	stop->SetPosition(pos);*/
}


void CMMDController::Precompute(void)
{
	CFrame::Precompute();

	wchar_t	str[256];
	double	elapsedTimeMS = mmdObject->animeManager->elapsedTimeMS;
	int		frame =  (int) (elapsedTimeMS * 30.0);
//	int		frameMax = (int) mmdObject->g_clVMDMotion.getMaxFrame();

	swprintf(str, L"%f, %d", elapsedTimeMS * 30.0, frame);
	counter->SetString(str);
}


void CMMDController::ItemDown(CComponent3D* item)
{
	CFrame::ItemDown(item);

	if(item == play) {
		Play();
		Animate();
	} else if(item == jogWheel) {
		Pause();
	} else if(item == stop) {
		Stop();
	}
}


void CMMDController::Reset(void)
{
	CFrame::Reset();

	play->Unselect();
	jogWheel->Unselect();
	stop->Unselect();
}



void CMMDController::Play(void)
{
	CComponent3D::Play();

	if(mmdObject) mmdObject->Play();
}


void CMMDController::Replay(void)
{
	CComponent3D::Replay();

	if(mmdObject) mmdObject->Replay();
}


void CMMDController::Stop(void)
{
	CComponent3D::Stop();

	if(mmdObject) mmdObject->Stop();
}
	

void CMMDController::Pause(void)
{
	CComponent3D::Pause();

	if(mmdObject) mmdObject->Pause();
}


void CMMDController::Skip(double skitTimeMS)
{
	CComponent3D::Skip(skitTimeMS);

	if(mmdObject) mmdObject->Skip(skitTimeMS);
}


void CMMDController::Animate(void)
{
	CComponent3D::Animate();

	if(mmdObject) mmdObject->Animate();
}
