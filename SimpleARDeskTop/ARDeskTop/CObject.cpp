#include "CObject.h"


CObject::CObject(void)
{
	id			= -1;
	visible		= true;
}


/*CObject::CObject(const CObject& that)
{
	visible = that.visible;
}*/


CObject::~CObject()
{
}


void CObject::Init(void)
{
}


void CObject::Precompute(void)
{
}


void CObject::Draw(void)
{
}


void CObject::KeyboardFunc3D(unsigned char)
{
}


void CObject::SetID(int pID)
{
	id = pID;
}
