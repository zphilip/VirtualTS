#ifndef ___CMMDMotionManager
#define ___CMMDMotionManager

#include "CAnimeManager.h"

class CMMDObject;



class CMMDMotionManager : public CAnimeManager
{
public :
			CMMDObject*	mmdObject;
public :
						CMMDMotionManager(CMMDObject* pMMDObject = 0);
	virtual				~CMMDMotionManager();

	virtual	void		Init(void);
	virtual	void		Stop(void);
	virtual	void		Pause(void);
	virtual	void		Skip(double skipTimeMS);
	virtual	int			Animate(void);
};


#endif
