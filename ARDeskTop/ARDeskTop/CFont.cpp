#include <locale.h>
#include <stdlib.h>
#include <string.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif
#include <FTGL/ftgl.h>

#include "CFont.h"


CFont::CFont(void)
{
	SetFontSize(DEF_FONT_SIZE);
	SetFontName(DEF_FONT_NAME);
	ftFont		= 0;
}


CFont::~CFont()
{
	if(ftFont) delete ftFont;
}


void CFont::Init(void)
{
	ftFont		= new FTGLPolygonFont(fontName);

	if(ftFont->Error()) exit(1);
	if(!ftFont->FaceSize(fontSize)) exit(1);
	if(!ftFont->CharMap(ft_encoding_unicode)) exit(1);
}


void CFont::SetFontName(char* pFontName)
{
	strcpy(fontName, pFontName);
}


void CFont::SetFontSize(int pFontSize)
{
	fontSize = pFontSize;
}


void CFont::Draw(wchar_t* str)
{
	if(!ftFont) return;

	glNormal3f(0.0, 0.0, 1.0);
	ftFont->Render(str);
}


