#include <locale.h>
#include <stdlib.h>
#include <string.h>


#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include <FTGL/ftgl.h>

#include "CText3D.h"
#include "CFont.h"


CFont CText3D::systemFont;



CText3D::CText3D(void)
: CComponent3D()
{
	touchable	= false;
	changedText	= true;
	font		= 0;
}


CText3D::~CText3D()
{
}


void CText3D::Init(void)
{
	CComponent3D::Init();

	SetFont(&systemFont);
	SetString(L"");
}


void CText3D::Precompute(void)
{
	CComponent3D::Precompute();
}


void CText3D::SetSize(double pWidth, double pHeight, double pDepth)
{
	width		= pWidth;
	height		= pHeight;
	depth		= pDepth;

	changedText = true;
}


void CText3D::DrawShape(void)
{
	if(font && str[0] != '\0' && visible) font->Draw(str);
}


int CText3D::SetFont(CFont* pFont)
{
	font		= pFont;
	changedText	= true;

	return(1);
}


int CText3D::SetFont(char* fontName)
{
	font->SetFontName(fontName);
	changedText = true;

	return(1);
}


void CText3D::SetString(wchar_t newChar)
{
	str[0] = newChar;
	str[1] = '\0';

	changedText = true;
}


void CText3D::SetString(wchar_t* newString)
{
	wcscpy(str, newString);

	changedText = true;
}


void CText3D::SetString(char* newString)
{
	int len;

	setlocale(LC_ALL, "Japanese");

	len = strlen(newString);
	if(len < 256)
		mbstowcs(str, newString, len + 1);

	changedText = true;
}


void CText3D::AddString(wchar_t newChar)
{
	int len = wcslen(str);

	if(len < 256) {
		str[len] = newChar;
		str[len + 1] = '\0';
	}

	changedText = true;
}


void CText3D::AddString(wchar_t* newString)
{
	wcscat(str, newString);

	changedText = true;
}


void CText3D::BackSpace(void)
{
	int len = wcslen(str);

	if(len > 0) {
		str[len - 1] = '\0';
	}
	changedText = true;
}


wchar_t* CText3D::GetString(void)
{
	return(str);
}


void CText3D::InitText(void)
{
	systemFont.Init();
}

