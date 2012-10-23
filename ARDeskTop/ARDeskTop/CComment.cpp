#include <string.h>
#include <stdio.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CComment.h"
#include "CCommentManager.h"
#include "CAnimeLiner.h"
#include "Color.h"


CFont	CComment::fontBig;
CFont	CComment::fontMedium;
CFont	CComment::fontSmall;
bool	CComment::fixedLayer[LAYER_NUM];
float	CComment::sizeList[FONT_NUM];




void CCommentData::SetSize(char* str)
{
	if(strcmp("b", str) == 0) {
		size = CComment::kBig;
	} else if(strcmp("m", str) == 0) {
		size = CComment::kMedium;
	} else if(strcmp("s", str) == 0) {
		size = CComment::kSmall;
	} else {
		size = CComment::kMedium; //デフォルト
	}
}


void CCommentData::SetColor(char* str)
{
	if(strcmp("black", str) == 0) {
		color = CComment::kBlack;
	} else if(strcmp("white", str) == 0) {
		color = CComment::kWhite;
	} else if(strcmp("red", str) == 0) {
		color = CComment::kRed;
	} else if(strcmp("green", str) == 0) {
		color = CComment::kGreen;
	} else if(strcmp("blue", str) == 0) {
		color = CComment::kBlue;
	} else if(strcmp("yellow", str) == 0) {
		color = CComment::kYellow;
	} else if(strcmp("cyan", str) == 0) {
		color = CComment::kCyan;
	} else if(strcmp("magenta", str) == 0) {
		color = CComment::kMagenta;
	} else if(strcmp("pink", str) == 0) {
		color = CComment::kPink;
	} else if(strcmp("purple", str) == 0) {
		color = CComment::kPurple;
	} else if(strcmp("truered", str) == 0) {
		color = CComment::kTruered;
	} else if(strcmp("elementalgreen", str) == 0) {
		color = CComment::kElementalgreen;
	} else if(strcmp("marineblue", str) == 0) {
		color = CComment::kMarineblue;
	} else if(strcmp("nobleviolet", str) == 0) {
		color = CComment::kNobleviolet;
	} else if(strcmp("madyellow", str) == 0) {
		color = CComment::kMadyellow;
	} else if(strcmp("passionorange", str) == 0) {
		color = CComment::kPassionorange;
	} else {
		color = CComment::kWhite; //デフォルト
	}
}


void CCommentData::SetLayer(char* str)
{
	if(strcmp("ue", str) == 0) {
		layer = CComment::kUe;
	} else if(strcmp("naka", str) == 0) {
		layer = CComment::kNaka;
	} else if(strcmp("shita", str) == 0) {
		layer = CComment::kShita;
	} else if(strcmp("hidari", str) == 0) {
		layer = CComment::kHidari;
	} else if(strcmp("migi", str) == 0) {
		layer = CComment::kMigi;
	} else if(strcmp("twist", str) == 0) {
		layer = CComment::kTwist;
	} else if(strcmp("tate", str) == 0) {
		layer = CComment::kTate;
	} else if(strcmp("pict", str) == 0) {
		layer = CComment::kPict;
	} else if(strcmp("sound", str) == 0) {
		layer = CComment::kSound;
	} else {
		layer = CComment::kNaka; //デフォルト
	}
}




CComment::CComment(void)
: CLabel3D()
{
}


CComment::~CComment()
{
}


void CComment::SetCommentSize(int size)
{
	switch(size) {
	case kBig :
		SetFont(&fontBig);
		break;
	case kMedium :
		SetFont(&fontMedium);
		break;
	case kSmall :
		SetFont(&fontSmall);
		break;
	}
}


void CComment::SetCommentColor(int color, float alpha)
{
	switch(color) {
	case kBlack :
		SetColor4va(Color::black, alpha);
		break;
	case kWhite :
		SetColor4va(Color::white, alpha);
		break;
	case kRed :
		SetColor4va(Color::red, alpha);
		break;
	case kGreen :
		SetColor4va(Color::green, alpha);
		break;
	case kBlue :
		SetColor4va(Color::blue, alpha);
		break;
	case kYellow :
		SetColor4va(Color::yellow, alpha);
		break;
	case kCyan :
		SetColor4va(Color::cyan, alpha);
		break;
	case kMagenta :
		SetColor4va(Color::magenta, alpha);
		break;
	case kPink :
		SetColor4va(Color::pink, alpha);
		break;
	case kOrenge :
		SetColor4va(Color::orenge, alpha);
		break;
	case kPurple :
		SetColor4va(Color::purple, alpha);
		break;
	case kTruered :
		SetColor4va(Color::truered, alpha);
		break;
	case kElementalgreen :
		SetColor4va(Color::elementalgreen, alpha);
		break;
	case kMarineblue :
		SetColor4va(Color::marineblue, alpha);
		break;
	case kNobleviolet :
		SetColor4va(Color::nobleviolet, alpha);
		break;
	case kMadyellow :
		SetColor4va(Color::madyellow, alpha);
		break;
	case kPassionorange :
		SetColor4va(Color::passionorange, alpha);
		break;
	}
}


void CComment::SetCommentAlignment(int layer)
{
	switch(layer) {
	case kNaka :
		SetAlignmentV(CLabel3D::kTop);
		SetAlignmentH(CLabel3D::kLeft);
		break;
	case kUe :
		SetAlignmentV(CLabel3D::kTop);
		SetAlignmentH(CLabel3D::kCenter);
		break;
	case kShita :
		SetAlignmentV(CLabel3D::kBottom);
		SetAlignmentH(CLabel3D::kCenter);
		break;
	case kHidari :
		SetAlignmentV(CLabel3D::kTop);
		SetAlignmentH(CLabel3D::kLeft);
		break;
	case kMigi :
		SetAlignmentV(CLabel3D::kTop);
		SetAlignmentH(CLabel3D::kRight);
		break;
	case kTwist :
		SetAlignmentV(CLabel3D::kCenter);
		SetAlignmentH(CLabel3D::kLeft);
		break;
	case kTate :
		SetAlignmentV(CLabel3D::kTop);
		SetAlignmentH(CLabel3D::kCenter);
		break;
	}
}


void CComment::SetCommentAnime(int layer, float animetionTime, double vec)
{
	CAnimeManager*	mana;

	if(layer == kNaka) {
		CAnimeLiner* liner = new CAnimeLiner(this);
		SetAnimeManager(liner);
		liner->Init();
		liner->SetVector(vec, 0.0, 0.0);
		mana = liner;
	} else if(layer == kTwist) {
		CAnimeRotate* rotate = new CAnimeRotate(this);
		SetAnimeManager(rotate);
		rotate->Init();
		rotate->SetVector(vec, 0.0, 0.0);
		rotate->SetAxis(1.0, 0.0, 0.0);
		rotate->aps = -360.0;
		this->Rotate(0.0, 0.0, 0.0, 1.0);
		mana = rotate;
	} else if(layer == kTate) {
		CAnimeLiner* liner = new CAnimeLiner(this);
		SetAnimeManager(liner);
		liner->Init();
		liner->SetVector(0.0, vec, 0.0);
		this->Rotate(90.0, 0.0, 0.0, 1.0);
		mana = liner;
	} else {
		mana = new CAnimeManager(this);
		SetAnimeManager(mana);
		mana->Init();
	}

	mana->repeat = 0;
	mana->animetionTimeMS = animetionTime;
	mana->deleteAfterStopped = 2;
	Precompute();
	mana->Play();
}


void CComment::InitComment(void)
{
//	char fontName[] = "c:/windows/fonts/hgrgm.ttc";
	char fontName[] = "c:/windows/fonts/hgrge.ttc";
//	char fontName[] = "c:/windows/fonts/hgrsgu.ttc";

	fontBig.SetFontName(fontName);
	fontBig.SetFontSize(13);
	fontBig.Init();

	fontMedium.SetFontName(fontName);
	fontMedium.SetFontSize(8);
	fontMedium.Init();

	fontSmall.SetFontName(fontName);
	fontSmall.SetFontSize(5);
	fontSmall.Init();


	CComment	comment;
	wchar_t		str[] = L"ABCDEFGHIJKLMNOPQRSTUVWXYZ<>?!@#$%^&*()|{}[]";

	comment.Init();
	comment.SetFont(&fontBig);
	comment.SetString(str);
	comment.Precompute();
	sizeList[kBig] = comment.height3 * 1.5f;

	comment.SetFont(&fontMedium);
	comment.Precompute();
	sizeList[kMedium] = comment.height3 * 1.5f;

	comment.SetFont(&fontSmall);
	comment.Precompute();
	sizeList[kSmall] = comment.height3 * 1.5f;

	printf("Big %f\nMedium %f\nSmall %f\n", sizeList[kBig], sizeList[kMedium], sizeList[kSmall]);


	fixedLayer[kUe]		= true;
	fixedLayer[kNaka]	= false;
	fixedLayer[kShita]	= true;
	fixedLayer[kHidari]	= true;
	fixedLayer[kMigi]	= true;
	fixedLayer[kTwist]	= false;
	fixedLayer[kTate]	= false;
	fixedLayer[kPict]	= false;
	fixedLayer[kSound]	= false;
}

