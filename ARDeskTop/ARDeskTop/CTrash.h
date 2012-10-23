#ifndef ___CTrash
#define ___CTrash

#include "CLabeledCube.h"
#include "CList.h"



class CTrash : public CLabeledCube
{
public :
//	static	CComponent3D*
//						dust;
	static	CList		dust;
public :
						CTrash(void);
	virtual				~CTrash();

	virtual	void		Init(void);
	virtual	void		DropFrom(CComponent3D* object, CMainCursor* cursor);
public :
	static	void		Erase(void);
};


#endif
