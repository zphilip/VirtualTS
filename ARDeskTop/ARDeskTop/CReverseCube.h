#ifndef ___CReverseCube
#define ___CReverseCube

#include "CComponent3D.h"
#include "CVertex.h"


class CReverseCube : public CComponent3D
{
public :
			double		hole;
			bool		lighting;
public :
						CReverseCube(void);
	virtual				~CReverseCube();

	virtual	void		DrawShape(void);
			void		DrawFace(int i, double n);
			void		DrawHoleFace(double n);
public :
	static	CVertex		vertex[8];
	static	int			face[6][4];
	static	CVertex		normal[6];
public :
	static	void		InitReverseCube(void);
};


#endif
