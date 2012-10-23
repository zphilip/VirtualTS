#ifndef ___CFolder
#define ___CFolder

#include "CRegion.h"

class CItemList;


class CFolder : public CRegion
{
public :
			CItemList*	itemList;
	//		CButton3D*	minimize;
	//		CButton3D*	maximize;
public :
						CFolder(void);
	virtual				~CFolder();

	virtual	void		Init(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);
	virtual	CComponent3D*
						IsWithin(double x, double y, double z);
};


#endif
