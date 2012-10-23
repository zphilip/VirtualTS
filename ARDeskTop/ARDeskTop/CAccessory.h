#ifndef ___CAccessory
#define ___CAccessory

#include "CProjection.h"



class CAccessory : public CProjection
{
public :
						CAccessory(void);
	virtual				~CAccessory();

	virtual	void		Init(void);
	virtual	void		Precompute(void);
};


#endif
