#ifndef ___CAnimeLiner
#define ___CAnimeLiner

#include "CAnimeManager.h"



class CAnimeLiner : public CAnimeManager
{
public :
			double		vec[3];
			double		pos[3];
public :
						CAnimeLiner(CComponent3D* pComponent = 0);
	virtual				~CAnimeLiner();

	virtual	void		Init(void);
	virtual	int			Animate(void);

			void		SetVector(double x, double y, double z);
			void		SetPosition(double x, double y, double z);
};


class CAnimeGravity : public CAnimeLiner
{
public :
						CAnimeGravity(CComponent3D* pComponent = 0);
	virtual				~CAnimeGravity();

	virtual	int			Animate(void);
};


#endif
