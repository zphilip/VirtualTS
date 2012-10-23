#ifndef ___CWorld
#define ___CWorld

#include "CGroup.h"
#include "CDesktop.h"



class CWorld : public CGroup
{
public :
						CWorld(void);
	virtual				~CWorld();

	virtual	void		Init(void);

			void		SortByDepth(CDesktop* desktop);
};

void	BubbleSortd(double a[], int id[], int n);


#endif
