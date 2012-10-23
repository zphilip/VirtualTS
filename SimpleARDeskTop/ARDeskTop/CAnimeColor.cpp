#include <stdio.h>

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
	if(!CAnimeManager::Animate()) return(0);

	float color[4];
	float a = (float) (elapsedTimeMS / animetionTimeMS);

	for(int i = 0; i < 4; i++)
		color[i] = a * d[i] + currentColor[i];

	component->SetColor4v(color);

	return(1);
}


/*sample


	CCube* rcube = new CCube();
	rcube->Init();
	rcube->SetSize(60.0, 60.0, 60.0);
	rcube->SetPosition(0.0, 0.0, 30.0);
	rcube->SetColor4va(Color::blue, 0.5f);
	rcube->SetID(objectID++);
	AddObject(rcube);

	CAnimeColor* aColor = new CAnimeColor(rcube);
	rcube->SetAnimeManager(aColor);
	Color::SetColor4(aColor->colorA, 1.0f, 0.0f, 0.0f, 1.0f);
	Color::SetColor4(aColor->colorB, 0.0f, 1.0f, 0.0f, 0.0f);
	aColor->Init();
	aColor->animetionTimeMS = 2.0;
	aColor->repeatTime = -1;
	rcube->Play();
*/

