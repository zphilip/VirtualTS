#ifndef ___CAnimeManager
#define ___CAnimeManager

#include "CComponent3D.h"



class CAnimeManager
{
public :
	enum
	{
		kStop,
		kPlay,
		kPause
	};
public :
			int			state;
			double		startTime;
			double		animetionTimeMS;
			double		elapsedTimeMS;
			int			repeat;
			int			repeatTime;
			int			deleteAfterStopped;

			CComponent3D*
						component;
public :
						CAnimeManager(CComponent3D* pComponent = 0);
	virtual				~CAnimeManager();

	virtual	void		Init(void);
			void		SetAnimationTime(float ct);
			void		SetComponent(CComponent3D* pComponent);
	virtual	void		Play(void);
	virtual	void		Replay(void);
	virtual	void		Stop(void);
	virtual	void		Pause(void);
	virtual	void		Skip(double skipTimeMS);
	virtual	int			Animate(void);
public :
	static	double		currentTime;
	static	double		prevTime;
};


#endif
