#ifndef ___CLabel3D
#define ___CLabel3D

#include "CText3D.h"

#define LABEL_UKASHI	0.1
#define LABEL_MARGIN	5.0



class CLabel3D : public CText3D
{
public :
	enum
	{
		kLeft,
		kCenter,
		kRight,
		kTop,
		kBottom,
		kFront,
		kBack,
		kFix
	};
public :
			double		delta[3];
			double		width3, height3;
			double		marginH;
			double		marginV;
			int			alignmentH;
			int			alignmentV;
public :
						CLabel3D(void);
	virtual				~CLabel3D();

	virtual	void		Init(void);
	virtual	void		Precompute(void);
	virtual	void		DrawShape(void);
	virtual	void		SetCardMode(bool pCard);

			void		SetAlignmentH(int pAlignmentH);
			void		SetAlignmentV(int pAlignmentV);
			void		SetMarginH(double pMarginH);
			void		SetMarginV(double pMarginV);
};


#endif
