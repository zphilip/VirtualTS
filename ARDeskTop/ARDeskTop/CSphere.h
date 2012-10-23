#ifndef ___CSphere
#define ___CSphere

#include "CComponent3D.h"



class CSphere : public CComponent3D
{
public :
						CSphere(void);
	virtual				~CSphere();

	virtual	void		Init(void);
	virtual	void		DrawShape(void);
};


#endif
