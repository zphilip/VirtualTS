#include <stdio.h>

#include "CMMDMotionManager.h"
#include "CMMDObject.h"



CMMDMotionManager::CMMDMotionManager(CMMDObject* pMMDObject)
: CAnimeManager(pMMDObject)
{
	mmdObject	= pMMDObject; 
}


CMMDMotionManager::~CMMDMotionManager()
{
}


void CMMDMotionManager::Init(void)
{
	CAnimeManager::Init();
}


void CMMDMotionManager::Stop(void)
{
	CAnimeManager::Stop();

	if(mmdObject) {
		mmdObject->g_clPMDModel.updateMotion(0.0f);
		mmdObject->g_clPMDModel.updateSkinning();
	}
}


void CMMDMotionManager::Pause(void)
{
	CAnimeManager::Pause();

	if(mmdObject) mmdObject->UpdateMatrix();
}


void CMMDMotionManager::Skip(double skipTimeMS)
{
	int frame = (int) ((elapsedTimeMS + skipTimeMS) * 30.0 + 0.49);
	skipTimeMS = ((float) frame - elapsedTimeMS * 30.0) / 30.0;
 
	CAnimeManager::Skip(skipTimeMS);
}



int CMMDMotionManager::Animate(void)
{
	if(!CAnimeManager::Animate()) return(0);

	if(mmdObject) {
		mmdObject->g_clPMDModel.updateMotion((float)elapsedTimeMS * 30.0f);
		mmdObject->g_clPMDModel.updateSkinning();
	}

	return(1);
}

