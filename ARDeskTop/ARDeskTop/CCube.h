#ifndef ___CCube
#define ___CCube

#include "CPrimitive.h"



class CCube : public CPrimitive
{
public :
	enum
	{
		kRect,
		kRound,
		kCircle1,
		kCircle2,
		kSpeechBaloon1,
		kSpeechBaloon2
	};
public :
			int			cardMode;
			bool		wire;

			int			sbIndex;
			double		sbPos[2];
public :
						CCube(void);
	virtual				~CCube();

	virtual	void		DrawShape(void);
			void		DrawCard(int cradMode, bool wire);
public :
	static	int			divNum;
	static	double		roundPos[50][2];
	static	void		InitCube(void);
	static	double		sqrt2;
};


#endif
