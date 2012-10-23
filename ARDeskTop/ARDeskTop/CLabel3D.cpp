#include <FTGL/ftgl.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CLabel3D.h"


CLabel3D::CLabel3D(void)
: CText3D()
{
	SetPosition(0.0, 0.0, 0.0); //LABEL_UKASHI);
//	SetSize(0.0, 0.0, 0.0);

	width3		= 0.0;
	height3		= 0.0;

	marginH		= LABEL_MARGIN;
	marginV		= LABEL_MARGIN;

	alignmentH	= kLeft;
	alignmentV	= kCenter;

	changedText = true;
}


CLabel3D::~CLabel3D()
{
}


void CLabel3D::Init(void)
{
	CText3D::Init();
}


void CLabel3D::Precompute(void)
{
	CText3D::Precompute();

	if(!font || !changedText) return;

	changedText = false;

	float	llx, lly, llz, urx, ury, urz;

	font->ftFont->BBox(str, llx, lly, llz, urx, ury, urz);

	width3	= font->ftFont->Advance(str);
	height3	= ury - lly;

	if(alignmentH == kLeft)
		delta[0] = -width * 0.5 + marginH;
	else if(alignmentH == kCenter)
		delta[0] = -width3 * 0.5;
	else if(alignmentH == kRight)
		delta[0] = width * 0.5 - marginH - width3;

	if(alignmentV == kTop)
		delta[1] = height * 0.5 - ury - marginV;
	else if(alignmentV == kCenter)
		delta[1] = -height3 * 0.5;
	else if(alignmentV == kBottom)
		delta[1] = -height * 0.5 + lly + marginV;

	if(card) {
		delta[2] = 0.0;
	} else {
		delta[2] = depth * 0.5;
	}
}


void CLabel3D::DrawShape(void)
{
	if(!visible) return;

	glTranslated(delta[0], delta[1], delta[2]);

	CText3D::DrawShape();
}


void CLabel3D::SetCardMode(bool pCard)
{
	CComponent3D::SetCardMode(pCard);

	changedText = true;
}


void CLabel3D::SetAlignmentH(int pAlignmentH)
{
	alignmentH	= pAlignmentH;
	changedText = true;
}


void CLabel3D::SetAlignmentV(int pAlignmentV)
{
	alignmentV	= pAlignmentV;
	changedText = true;
}


void CLabel3D::SetMarginH(double pMarginH)
{
	marginH		= pMarginH;
	changedText = true;
}


void CLabel3D::SetMarginV(double pMarginV)
{
	marginV		= pMarginV;
	changedText = true;
}

