#include "CWristWatch.h"
#include "CButton3D.h"
#include "CClock.h"
#include "CKeyboard.h"
#include "CFrame.h"


CWristWatch::CWristWatch(void)
: CFrame()
{
	buttonNum	= 0;
	button		= 0;
	clock		= 0;
	keyboard	= 0;
}


CWristWatch::~CWristWatch()
{
}


void CWristWatch::Init(void)
{
	CFrame::Init();

	int i = 0;

	buttonNum	= 1;
	button = new CButton3D[buttonNum];
	button[0].Init();
	button[0].SetID(i++);		// id = 0
	AddObject(&button[0]);

	clock = new CClock();
	clock->Init();
	clock->SetID(i++);			// id = 1
	AddObject(clock);

	keyboard = new CKeyboard();
	keyboard->Init();
	keyboard->SetID(i++);		// id = 2
	keyboard->visible = false; //hide object
	AddObject(keyboard);
}


void CWristWatch::SetSize(double pWidth, double pHeight, double pDepth)
{
	CFrame::SetSize(pWidth, pHeight, pDepth);

	double clockWidth = 35.0;
	double depthMargin = 0.0;

//	clock->label->on(0.0, 0.0, 0.0);
	clock->SetSize(clockWidth, clockWidth, depth - depthMargin);
	clock->SetPosition(0.0, 0.0, 0.0);

	button[0].SetSize(10.0, 10.0, depth - depthMargin);
	button[0].SetPosition(0.0, -(clockWidth * 0.5 + 10.0 + 5.0), 0.0);

	keyboard->SetSize(1.0, 1.0, depth);
	keyboard->SetPosition(-(keyboard->width + width) * 0.5, 0.0, 0.0);
}


CComponent3D* CWristWatch::IsWithin(double x, double y, double z)
{
	if(!touchable || clickedCursor || !visible) return(0);

	CComponent3D* hitObject = CFrame::IsWithin(x, y, z);

	if(!hitObject && keyboard->visible) {
		double newPos[3];

		InvertPos(x, y, z, newPos);
		hitObject = keyboard->IsWithin(newPos[0], newPos[1], newPos[2]);
	}

	return(hitObject);
}


void CWristWatch::ItemDown(CComponent3D* item)
{
	CFrame::ItemDown(item);

	if(item == &button[0]) {
		keyboard->visible = button[0].locked;
	}
}


