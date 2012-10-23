#ifndef ___CClock
#define ___CClock

#include "CLabeledCube.h"


class CClock : public CLabeledCube
{
public :
						CClock(void);
	virtual				~CClock();

	virtual	void		Init(void);
	virtual	void		Precompute(void);
};


#endif
