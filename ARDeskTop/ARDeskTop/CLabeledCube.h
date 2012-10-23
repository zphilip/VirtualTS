#ifndef ___CLabeledCube
#define ___CLabeledCube

#include "CFrame.h"
#include "CLabel3D.h"

class CFont;



class CLabeledCube : public CFrame
{
public :
			CLabel3D*	label;
public :
						CLabeledCube(void);
	virtual				~CLabeledCube();

	virtual	void		Init(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);

			int			SetFont(CFont* font);
			void		SetAlignmentH(int pAlignment);
			void		SetString(wchar_t c);
	virtual	void		SetString(wchar_t* string);
			void		SetString(char* string);
};


#endif
