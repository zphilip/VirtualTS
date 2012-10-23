#ifndef ___CGroup
#define ___CGroup

#include "CComponent3D.h"
#include "CList.h"



class CGroup : public CComponent3D
{
public :
			bool		checkWithinBBOnly;
			bool		withinBB;

			CObjectList	objectList;
public :
						CGroup(void);
	virtual				~CGroup();

	virtual	void		Init(void);
	virtual	void		Precompute(void);
	virtual	void		DrawShape(void);
	virtual	CComponent3D*
						IsWithin(double x, double y, double z);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);

	virtual	void		AddObject(CComponent3D* object);
	virtual	CLink*		RemoveObject(CComponent3D* object);
	virtual	int			DeleteObject(CComponent3D* object);

	virtual	void		ItemDown(CComponent3D* item);
	virtual	void		ItemUp(CComponent3D* item);
	virtual	void		ItemClickDown(CComponent3D* item);
	virtual	void		ItemClickUp(CComponent3D* item);
	virtual	void		Reset(void);
	virtual	void		SetCardMode(bool pCard);

	virtual	void		Play(void);
	virtual	void		Replay(void);
	virtual	void		Stop(void);
	virtual	void		Pause(void);
	virtual	void		Animate(void);

	friend	CGroup*		FindNotFixed(CGroup* object);
};


#endif
