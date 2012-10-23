#ifndef ___CRegion
#define ___CRegion

#include "CGroup.h"
#include "CResizeBox.h"

class CLabel3D;
class CCube;



class CRegion : public CGroup
{
public :
			double		orig[2];
			CLabel3D*	label;
			CCube*		region;
			CResizeBox*	resize;
			bool		resizing;

			CObjectList	objectList2;
public :
						CRegion(void);
	virtual				~CRegion();

	virtual	void		Init(void);
	virtual	void		Precompute(void);
	virtual	void		DrawShape(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);

	virtual	void		ItemClickDown(CComponent3D* item);
	virtual	void		ItemClickUp(CComponent3D* item);

			void		SetOrigin(double x, double y);
			void		AddObject2(CComponent3D* object);
public :
	static	double		buttonSize;
};


#endif
