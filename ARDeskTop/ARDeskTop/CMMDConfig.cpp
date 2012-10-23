#include <stdio.h>

#include "CMMDConfig.h"
#include "CBone.h"
#include "CButton3D.h"
#include "Color.h"




CBoundSlider::CBoundSlider(void)
: CSlider()
{
}


CBoundSlider::~CBoundSlider()
{
}


void CBoundSlider::ItemClickUp(CComponent3D* item)
{
	CSlider::ItemClickUp(item);

	CHandleCube::boundSize = value;

	printf("size %f\n", CHandleCube::boundSize);
}





CMMDConfig::CMMDConfig(void)
: CFrame()
{
	drawDot				= 0;
	drawWireMaster		= 0;
	drawWireWithDepth	= 0;
	boundSize			= 0;
}


CMMDConfig::~CMMDConfig()
{
}


void CMMDConfig::Init(void)
{
	CFrame::Init();

	int i = 0;

	drawDot		= new CButton3D();
	drawDot->Init();
	drawDot->SetString(L"“_");
	drawDot->SetID(i++);
	AddObject(drawDot);

	drawWireMaster		= new CButton3D();
	drawWireMaster->Init();
	drawWireMaster->SetString(L"” ");
	drawWireMaster->SetID(i++);
	AddObject(drawWireMaster);

	drawWireWithDepth	= new CButton3D();
	drawWireWithDepth->Init();
	drawWireWithDepth->SetString(L"[");
	drawWireMaster->SetID(i++);
	AddObject(drawWireWithDepth);

	boundSize	= new CBoundSlider();
	boundSize->Init();
	boundSize->SetID(i++);
	boundSize->fix = true;
	boundSize->max = 4.0;
	boundSize->min = 0.2;
	AddObject(boundSize);
}


void CMMDConfig::Precompute(void)
{
	CFrame::Precompute();
}


void CMMDConfig::SetSize(double pWidth, double pHeight, double pDepth)
{
	CFrame::SetSize(pWidth, pHeight, pDepth);

	double pos[3];
	double btmWidth = 15.0;

	pos[0] = -(width + 6.0) * 0.5 + btmWidth;
	pos[1] = -(height - btmWidth - 6.0) * 0.5;
	pos[2] = 0.0;

	drawDot->SetSize(btmWidth, btmWidth, depth);
	drawDot->SetPosition(pos);

	pos[0] *= -1.0;
	drawWireWithDepth->SetSize(btmWidth, btmWidth, depth);
	drawWireWithDepth->SetPosition(pos);

	pos[0] = 0.0;
	drawWireMaster->SetSize(btmWidth, btmWidth, depth);
	drawWireMaster->SetPosition(pos);

	pos[1] *= -1.0;
	boundSize->axis = 0;
	boundSize->SetSize(width - 12.0, btmWidth, depth);
	boundSize->SetPosition(pos);
}


void CMMDConfig::ItemDown(CComponent3D* item)
{
	if(item == drawDot)					//“_
		CHandleCube::drawDot = !drawDot->locked;
	else if(item == drawWireMaster)		//” 
		CHandleCube::drawWireMaster = !drawWireMaster->locked;
	else if(item == drawWireWithDepth)	//[
		CHandleCube::drawWireWithDepth = !drawWireWithDepth->locked;
}


