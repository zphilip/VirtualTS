#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CAnimeLiner.h"



CAnimeLiner::CAnimeLiner(CComponent3D* pComponent)
: CAnimeManager(pComponent)
{
	vec[0] = 0.0;
	vec[1] = 0.0;
	vec[2] = 0.0;

	pos[0] = 0.0;
	pos[1] = 0.0;
	pos[2] = 0.0;
}


CAnimeLiner::~CAnimeLiner()
{
}


void CAnimeLiner::Init(void)
{
	if(!component) return;

	CAnimeManager::Init();

//	component->GetPosition(pos);
}


int CAnimeLiner::Animate(void)
{
	if(!CAnimeManager::Animate()) return(0);

	component->SetPosition(
		pos[0] + elapsedTimeMS * vec[0],
		pos[1] + elapsedTimeMS * vec[1],
		pos[2] + elapsedTimeMS * vec[2]
	);

	return(1);
}


void CAnimeLiner::SetVector(double x, double y, double z)
{
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}


void CAnimeLiner::SetPosition(double x, double y, double z)
{
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
}







// class CAnimeGravity


CAnimeGravity::CAnimeGravity(CComponent3D* pComponent)
: CAnimeLiner(pComponent)
{
}


CAnimeGravity::~CAnimeGravity()
{
}


int CAnimeGravity::Animate(void)
{
	if(!CAnimeManager::Animate()) return(0);

	component->SetPosition(
		pos[0] + elapsedTimeMS * vec[0],
		pos[1] + elapsedTimeMS * vec[1] - 300.0 * elapsedTimeMS * elapsedTimeMS,
		pos[2] + elapsedTimeMS * vec[2]
	);

	return(1);
}
