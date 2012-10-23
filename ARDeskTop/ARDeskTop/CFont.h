#ifndef ___CFont
#define ___CFont

//#define DEF_FONT_NAME "c:/windows/fonts/msgothic.ttc"
#define DEF_FONT_NAME "c:/windows/fonts/hgrgm.ttc"
#define DEF_FONT_SIZE 8

class FTFont;



class CFont
{
public :
			char		fontName[256];
			int			fontSize;
			FTFont*		ftFont;
public :
						CFont(void);
						~CFont();

			void		Init(void);
			void		SetFontName(char* pFontName);
			void		SetFontSize(int);
			void		Draw(wchar_t* str);
};


#endif
