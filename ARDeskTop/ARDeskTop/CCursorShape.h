#ifndef ___CCursorShape
#define ___CCursorShape

#include "CComponent3D.h"



class CCursorShape : public CComponent3D
{
protected :
			double		pos1[3], pos2[3], pos3[3];
public :
						CCursorShape(void);
	virtual				~CCursorShape();

	virtual	void		Init(void);
	virtual	void		DrawShape(void);

			void		SetWidth(double w);
};


#endif
