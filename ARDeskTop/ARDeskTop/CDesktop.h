#ifndef ___CDesktop
#define ___CDesktop

#include "CProjection.h"

class CWorld;


class CDesktop : public CProjection
{
public :
	static	CDesktop*	desktop;
public :
			CWorld*		world;
public :
						CDesktop(void);
	virtual				~CDesktop();

	virtual	void		Init(void);
	virtual	void		Draw2(void);

			void		SortByDepth(void);
};


#endif
