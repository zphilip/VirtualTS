#include "CColorPalette.h"
#include "CMainCursor.h"
#include "Color.h"



CColorCube::CColorCube(void)
: CCube()
{
	drawBB		= true;
	dropable	= true;
}


void CColorCube::DropTo(CComponent3D* object, CMainCursor* cursor)
{
	if(object) object->SetColor3v(currentColor);
	SetPosition(cursor->firstClickInfo.objectPos);
}



CColorPalette::CColorPalette(void)
: CFrame()
{
	color01 = 0;
	color02 = 0;
	color03 = 0;
	color04 = 0;
	color05 = 0;
	color06 = 0;
	color07 = 0;
	color08 = 0;
	color09 = 0;
}


CColorPalette::~CColorPalette()
{
}

void CColorPalette::Init(void)
{
	CFrame::Init();

	color01 = new CColorCube();
	color02 = new CColorCube();
	color03 = new CColorCube();
	color04 = new CColorCube();
	color05 = new CColorCube();
	color06 = new CColorCube();
	color07 = new CColorCube();
	color08 = new CColorCube();
	color09 = new CColorCube();

	color01->Init();
	color02->Init();
	color03->Init();
	color04->Init();
	color05->Init();
	color06->Init();
	color07->Init();
	color08->Init();
	color09->Init();

	color01->SetColor3v(Color::black);
	color02->SetColor3v(Color::white);
	color03->SetColor3v(Color::red);
	color04->SetColor3v(Color::green);
	color05->SetColor3v(Color::blue);
	color06->SetColor3v(Color::yellow);
	color07->SetColor3v(Color::cyan);
	color08->SetColor3v(Color::magenta);
	color09->SetColor3v(Color::pink);

	AddObject(color01);
	AddObject(color02);
	AddObject(color03);
	AddObject(color04);
	AddObject(color05);
	AddObject(color06);
	AddObject(color07);
	AddObject(color08);
	AddObject(color09);
}


void CColorPalette::SetSize(double pWidth, double pHeight, double pDepth)
{
	CFrame::SetSize(pWidth, pHeight, pDepth);

	double margin, cWidth, cHeight, cDepth, x, y, z;

	margin	= 12.0;
	cWidth	= (width - margin * 4.0) / 3.0;
	cHeight	= (height - margin * 4.0) / 3.0;
	cDepth	= depth;

	color01->SetSize(cWidth, cHeight, cDepth);
	color02->SetSize(cWidth, cHeight, cDepth);
	color03->SetSize(cWidth, cHeight, cDepth);
	color04->SetSize(cWidth, cHeight, cDepth);
	color05->SetSize(cWidth, cHeight, cDepth);
	color06->SetSize(cWidth, cHeight, cDepth);
	color07->SetSize(cWidth, cHeight, cDepth);
	color08->SetSize(cWidth, cHeight, cDepth);
	color09->SetSize(cWidth, cHeight, cDepth);

	x = -(cWidth + margin);
	y = cHeight + margin;
	z = 0.0;

	color01->SetPosition(x, y, z);
	x += (cWidth + margin);
	color02->SetPosition(x, y, z);
	x += (cWidth + margin);
	color03->SetPosition(x, y, z);

	x = -(cWidth + margin);
	y = 0.0;

	color04->SetPosition(x, y, z);
	x += (cWidth + margin);
	color05->SetPosition(x, y, z);
	x += (cWidth + margin);
	color06->SetPosition(x, y, z);

	x = -(cWidth + margin);
	y = -(cHeight + margin);

	color07->SetPosition(x, y, z);
	x += (cWidth + margin);
	color08->SetPosition(x, y, z);
	x += (cWidth + margin);
	color09->SetPosition(x, y, z);
}

