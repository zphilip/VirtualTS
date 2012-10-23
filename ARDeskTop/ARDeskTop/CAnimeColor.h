#ifndef ___CAnimeLiner
#define ___CAnimeLiner

#include "CAnimeManager.h"



class CAnimeColor : public CAnimeManager
{
public :
			float*		currentColor;
			float		colorA[4];
			float		colorB[4];
			float		d[4];
public :
						CAnimeColor(CComponent3D* pComponent = 0);
	virtual				~CAnimeColor();

	virtual	void		Init(void);
	virtual	void		Replay(void);
	virtual	int			Animate(void);
};


#endif
