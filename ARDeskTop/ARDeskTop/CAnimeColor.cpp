#include "CAnimeColor.h"
#include "CComponent3D.h"
#include "Color.h"



CAnimeColor::CAnimeColor(CComponent3D* pComponent)
: CAnimeManager(pComponent)
{
	Color::SetColor4(colorA, 0.0f, 0.0f, 0.0f, 1.0f);
	Color::SetColor4(colorB, 1.0f, 1.0f, 1.0f, 1.0f);
}


CAnimeColor::~CAnimeColor()
{
}


void CAnimeColor::Init(void)
{
	currentColor = colorA;
	for(int i = 0; i < 4; i++)
		d[i] = colorB[i] - colorA[i];

	component->SetColor4v(currentColor);
}


void CAnimeColor::Replay(void)
{
	for(int i = 0; i < 4; i++)
		d[i] *= -1.0f;

	if(currentColor == colorA)
		currentColor = colorB;
	else
		currentColor = colorA;
}


int CAnimeColor::Animate(void)
{
	if(CAnimeManager::Animate()) return(1);

	float color[4];
	float a = (float) (elapsedTimeMS / animetionTimeMS);

	for(int i = 0; i < 4; i++)
		color[i] = a * d[i] + currentColor[i];

	component->SetColor4v(color);

	return(0);
}
