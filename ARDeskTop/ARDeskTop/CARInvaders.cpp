#include <math.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CARInvaders.h"
#include "CCursor.h"
#include "CLabel3D.h"
#include "CAnimeLiner.h"
#include "CDesktop.h"
#include "CUser.h"
#include "CARDeskTop.h"

#include "PI.h"

CARInvaders* CARInvaders::arInvaders = 0;



CMissile::CMissile(void)
: CCube()
{
}


CMissile::~CMissile()
{
}


void CMissile::Init()
{
	CCube::Init();
	SetColor3(1.0f, 0.0f, 0.0f);

	CAnimeLiner* mane = new CAnimeLiner(this);
	SetAnimeManager(mane);
	mane->SetVector(25.0 * matrix[4], 25.0 * matrix[5], 25.0 * matrix[6]);
	mane->SetAnimationTime(3.0);
	mane->repeat = 0;
	mane->deleteAfterStopped = 2;
	mane->Init();
	mane->Play();
}



CInvaderMissile::CInvaderMissile(void)
: CCube()
{
}


CInvaderMissile::~CInvaderMissile()
{
}


void CInvaderMissile::Init(void)
{
	CCube::Init();
}





CInvader::CInvader(void)
: CLabeledCube()
{
}


CInvader::~CInvader()
{
}


void CInvader::Init(void)
{
	CLabeledCube::Init();
	label->SetColor3(0.0f, 1.0f, 0.0f);
	SetString(L"‘ü");
}


void CInvader::Fire(void)
{
	double vec[3], pos[3], len;

//	GetPosition(pos);
//	parent->LocalToGlobal(pos);

	pos[0] = CDesktop::desktop->trans[0][3];
	pos[1] = CDesktop::desktop->trans[1][3];
	pos[2] = CDesktop::desktop->trans[2][3];
	CDesktop::desktop->ProjectPos(pos, vec);
	len = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	vec[0] /= len;
	vec[1] /= len;
	vec[2] /= len;

	CInvaderMissile* iMissile = new CInvaderMissile();
	iMissile->Init();

	iMissile->SetSize(5.0, 5.0, 5.0);

	CAnimeLiner* mane = new CAnimeLiner(iMissile);
	iMissile->SetAnimeManager(mane);
	mane->SetVector(50.0 * vec[0], 50.0 * vec[1], 50.0 * vec[2]);
	mane->SetAnimationTime(5.0);
	mane->repeat = 0;
	mane->deleteAfterStopped = 2;
//	GetPosition(vec);
//	iMissile->SetPosition(vec);
	mane->Init();
	mane->Play();

	CARInvaders::arInvaders->AddInvaderMissile(iMissile);
}





CARInvaders::CARInvaders(void)
: CGroup()
{
	arInvaders	= this;
	touchable	= false;
}


CARInvaders::~CARInvaders()
{
	missileList.DeleteAll();
	invaderList.DeleteAll();
	invaderMissileList.DeleteAll();
}


void CARInvaders::Init(void)
{
	CGroup::Init();

	invaderFont.SetFontSize(20);
	invaderFont.Init();

	CInvader*	invader;
	double		pos[3] = { 0.0, 0.0, 0.0 };

	for(int i = 0; i < 5; i++) {
		invader = new CInvader();
		invader->Init();
		invader->SetSize(30.0, 30.0, 30.0);
		invader->SetPosition(pos);
		invader->SetFont(&invaderFont);
		invader->SetAlignmentH(CLabel3D::kCenter);
		pos[0] += 40.0;
		CInvaderAnime* mane = new CInvaderAnime(invader);
		invader->SetAnimeManager(mane);
		mane->a = i * 0.5;
		mane->Init();
		mane->Play();
		AddInvader(invader);
	}
}


void CARInvaders::Precompute(void)
{
	CGroup::Precompute();
	missileList.Precompute();
	invaderList.Precompute();
	invaderMissileList.Precompute();

	CMissile*	missile;
	CInvader*	invader;
	double		missilePos[3];
	int			invaderNum, missileNum;

	missileNum = missileList.GetDegree();
	missileList.GoToFirst();

	for(int i = 0; i < missileNum; i++) {
		missile = (CMissile*) missileList.GetKey();
		missile->GetPosition(missilePos);

		invaderNum = invaderList.GetDegree();
		invaderList.GoToFirst();

		for(int j = 0; j < invaderNum; j++) {
			invader = (CInvader*) invaderList.GetKey();
			if(invader->IsWithin(missilePos[0], missilePos[1], missilePos[2])) {
				invader->Delete();
				missile->Stop();
				ARDTPlaySound(L"Data/explose.wav");
				break;
			}
			invaderList.GoToNext();
		}
		missileList.GoToNext();
	}
}


void CARInvaders::DrawShape(void)
{
	missileList.Draw();
	invaderList.Draw();
	invaderMissileList.Draw();
}


int CARInvaders::DeleteObject(CComponent3D* object)
{
	if(missileList.Delete(object)) return(1);
	if(invaderList.Delete(object)) return(1);

	return(invaderMissileList.Delete(object));
}


void CARInvaders::AddMissile(CMissile* missile)
{
	missile->SetParent(this);
	missileList.GoToFirst();
	missileList.Add(missile);
}


void CARInvaders::AddInvader(CInvader* invader)
{
	invader->SetParent(this);
	invaderList.GoToFirst();
	invaderList.Add(invader);
}


void CARInvaders::AddInvaderMissile(CInvaderMissile* iMissile)
{
	iMissile->SetParent(this);
	invaderMissileList.GoToFirst();
	invaderMissileList.Add(iMissile);
}





CInvaderAnime::CInvaderAnime(CComponent3D* pComponent)
: CAnimeMatrix(pComponent)
{
	a = 0.0;
}


void CInvaderAnime::Init(void)
{
	CAnimeMatrix::Init();
	animetionTimeMS = 2.0 * PI / 3.0;
	repeat		= 1;
	repeatTime	= -1;
}


void CInvaderAnime::Replay(void)
{
	CAnimeMatrix::Replay();
//	((CInvader*) component)->Fire();
}


void CInvaderAnime::PreAnimate(void)
{
	glTranslated(0.0, 70.0 * sin(3.0 * elapsedTimeMS + a), 0.0);
}
