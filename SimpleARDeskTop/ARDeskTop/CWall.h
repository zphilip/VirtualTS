#ifndef ___CWall
#define ___CWall

#include "CGroup.h"



class CWall : public CGroup
{
public :
	static	CWall*		wall;
public :
						CWall(void);

	virtual	void		Init(void);
	virtual	void		Draw(void);

			void		SetCursorMode(int mode);
};


class CWall2 : public CWall
{
public :
	static	CWall*		wall;
public :
						CWall2(void);
	virtual				~CWall2();

	virtual	void		Init(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);
};


class CGrid : public CComponent3D
{
public :
			double		gridSize;
public :
	virtual	void		DrawShape(void);

			void		SetGridSize(double pGridSize);
};


class CWall3 : public CWall
{
public :
			CGrid*		grid;
public :
	virtual	void		Init(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);
};


#endif
