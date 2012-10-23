#ifndef ___CPrimitive
#define ___CPrimitive

#include "CComponent3D.h"



class CPrimitive : public CComponent3D
{
public :
						CPrimitive(void);
	virtual				~CPrimitive();

	virtual	void		SetSize(double w, double h, double d);
	virtual	CComponent3D*
						IsWithin(double x, double y, double z);
};


#endif
