#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include <stdio.h>

#include "CAnimeManager.h"
#include "CComponent3D.h"


double		CAnimeManager::currentTime	= 0.0;
double		CAnimeManager::prevTime		= 0.0;



CAnimeManager::CAnimeManager(CComponent3D* pComponent)
{
	state			= kStop;

	startTime		= 0.0;
	animetionTimeMS	= 0.0;
	elapsedTimeMS	= 0.0;
	repeat			= 1; // 0 - 1
	repeatTime		= 1; // -1 : repeat forever // repeatTime + 1‰ñŒJ‚è•Ô‚·
	deleteAfterStopped = 0; // 0 - 2

	component		= pComponent;
}


CAnimeManager::~CAnimeManager()
{
}


void CAnimeManager::Init(void)
{
}


void CAnimeManager::SetAnimationTime(float ct)
{
	animetionTimeMS	= ct;
}


void CAnimeManager::SetComponent(CComponent3D* pComponent)
{
	component	= pComponent;
}


void CAnimeManager::Play(void)
{
	if(state == kPlay) return;

	if(state == kPause) {
		startTime = currentTime - elapsedTimeMS;
	} else {
		startTime = currentTime;
	}

	state		= kPlay;
}


void CAnimeManager::Replay(void)
{
}


void CAnimeManager::Stop(void)
{
	state		= kStop;
	elapsedTimeMS = 0.0f;

	if(deleteAfterStopped == 2) deleteAfterStopped--;
}


void CAnimeManager::Pause(void)
{
	state		= kPause;
}


void CAnimeManager::Skip(double skipTimeMS)
{
	if(skipTimeMS == 0.0) return;

	elapsedTimeMS += skipTimeMS;

	if(!repeat) {
		if(elapsedTimeMS < 0.0f) elapsedTimeMS = 0.0;
		else if(animetionTimeMS < elapsedTimeMS) elapsedTimeMS = animetionTimeMS;
	} else {
		if(elapsedTimeMS < 0.0f) elapsedTimeMS += animetionTimeMS;
		else if(animetionTimeMS < elapsedTimeMS) elapsedTimeMS -= animetionTimeMS;
	}

	//Pause();
	state		= kPause;
	Play();
	Animate();
	Pause();
}


int CAnimeManager::Animate(void)
{
	if(!component) return(0);
	if(state == kStop && deleteAfterStopped == 1) {
		component->Delete();
		return(0);
	}
	if(state != kPlay) return(0);

	elapsedTimeMS = currentTime - startTime;

	if(elapsedTimeMS >= animetionTimeMS) {
		if(repeat && repeatTime != 0) {
			startTime += animetionTimeMS;
			elapsedTimeMS -= animetionTimeMS;
			if(repeatTime > 0) repeatTime--;
			Replay();
		} else {
			elapsedTimeMS = animetionTimeMS;
			Stop();

			return(0);
		}
	}

	return(1);
}

