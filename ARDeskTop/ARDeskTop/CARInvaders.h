#ifndef ___CARInvaders
#define ___CARInvaders

#include "CGroup.h"
#include "CCube.h"
#include "CAnimeMatrix.h"
#include "CLabeledCube.h"
#include "CFont.h"


class CMissile : public CCube
{
public :
						CMissile(void);
	virtual				~CMissile();

	virtual	void		Init(void);
};


class CInvaderMissile : public CCube
{
public :
						CInvaderMissile(void);
	virtual				~CInvaderMissile();

	virtual	void		Init(void);
};


class CInvader : public CLabeledCube
{
public :
						CInvader(void);
	virtual				~CInvader();

	virtual	void		Init(void);
			void		Fire(void);
};


class CARInvaders : public CGroup
{
public :
	static	CARInvaders*
						arInvaders;
public :
			CObjectList	missileList;
			CObjectList	invaderList;
			CObjectList	invaderMissileList;
			CFont		invaderFont;
public :
						CARInvaders(void);
	virtual				~CARInvaders();

	virtual	void		Init(void);
	virtual	void		Precompute(void);
	virtual	void		DrawShape(void);
	virtual	int			DeleteObject(CComponent3D* object);

			void		AddMissile(CMissile* missile);
			void		AddInvader(CInvader* invader);
			void		AddInvaderMissile(CInvaderMissile* iMissile);
};


class CInvaderAnime : public CAnimeMatrix
{
public :
			float		a;
public :
						CInvaderAnime(CComponent3D* pComponent = 0);
	virtual	void		Init(void);
	virtual	void		Replay(void);
	virtual	void		PreAnimate(void);
};


#endif
