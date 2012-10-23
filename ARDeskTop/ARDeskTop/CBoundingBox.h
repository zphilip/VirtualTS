#ifndef ___CBoundingBox
#define ___CBoundingBox

#include "CPrimitive.h"


class CBoundingBox : public CPrimitive
{
public :
						CBoundingBox(void);
	virtual				~CBoundingBox();

	virtual	void		DrawShape(void);
};


#endif
