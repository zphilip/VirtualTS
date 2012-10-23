#ifndef ___CText3D
#define ___CText3D

#include "CComponent3D.h"
#include "CFont.h"



class CText3D : public CComponent3D
{
public :
	static	CFont		systemFont;
protected :
			wchar_t		str[256];
			CFont*		font;
			bool		changedText;
public :
						CText3D(void);
	virtual				~CText3D();

	virtual	void		Init(void);
	virtual	void		Precompute(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);
	virtual	void		DrawShape(void);
	virtual	void		DrawBoudingBox(void) {}

			int			SetFont(CFont* pFont);
			int			SetFont(char* fontName);
			void		SetString(wchar_t c);
			void		SetString(wchar_t* newString);
			void		SetString(char* newString);
			void		AddString(wchar_t newChar);
			void		AddString(wchar_t* newString);
			void		BackSpace(void);
			wchar_t*	GetString(void);

	static	void		InitText(void);
};


#endif
