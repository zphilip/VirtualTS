#ifndef ___CBone
#define ___CBone

#include "CGroup.h"

class cPMDBone;
class CMMDObject;



class CHandleCube : public CComponent3D
{
public :
			bool		drawWire;
public :
						CHandleCube(void);
	virtual				~CHandleCube();

	virtual	void		DrawShape(void);
	virtual	CComponent3D*
						IsWithin(double x, double y, double z);
	virtual	void		CursorEnter(CCursor* cursor);
	virtual	void		CursorLeave(CCursor* cursor);
public :
	static	double		boundSize;
	static	bool		drawDot;
	static	bool		drawWireMaster;
	static	bool		drawWireWithDepth;
};


class CBone : public CGroup
{
public :
			CHandleCube*
						handleCube;
			CMMDObject*	mmdObject;
public :
						CBone(void);
	virtual				~CBone();

	virtual	void		Init(void);
	virtual	void		Precompute(void);
	virtual	void		ClickDown(CMainCursor* cursor);
	virtual	void		ClickUp(CMainCursor* cursor);
	virtual	CComponent3D*
						GetHandleCube(void);
};


class CFKBone : public CBone
{
public :
			double		gMatrix[16];
			cPMDBone*	pmdBone;
public :
						CFKBone(void);
	virtual				~CFKBone();

	virtual	void		UpdateMatrix(void);
};

/*
class CIKBone : public CBone
{
public :
						CIKBone(void);
	virtual				~CIKBone();
};*/


#endif
