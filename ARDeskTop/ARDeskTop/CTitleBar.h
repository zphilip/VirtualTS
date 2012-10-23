#ifndef ___CTitleBar
#define ___CTitleBar

#include "CLabeledCube.h"

#define TITLEBAR_HEIGHT	15.0


class CTitleBar : public CLabeledCube
{
public :
						CTitleBar(void);
	virtual				~CTitleBar();

	virtual	void		Init(void);
};


#endif
