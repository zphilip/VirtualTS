#ifndef ___CAnimeMatrix
#define ___CAnimeMatrix

#include "CAnimeManager.h"



class CAnimeMatrix : public CAnimeManager
{
public :
			double		matrix[16];
			double		pos[3];
			double		vec[3];
public :
						CAnimeMatrix(CComponent3D* pComponent = 0);
	virtual				~CAnimeMatrix();

	virtual	void		Init(void);
	virtual	int			Animate(void);
	virtual	void		PreAnimate(void);
	virtual	void		PostAnimate(void);

			void		SetVector(double x, double y, double z);
};


class CAnimeRotate : public CAnimeMatrix
{
public :
			double		axis[3];
			double		aps;
public :
						CAnimeRotate(CComponent3D* pComponent = 0);
	virtual				~CAnimeRotate();

	virtual	void		PreAnimate(void);

			void		SetAxis(double x, double y, double z);
};


#endif
