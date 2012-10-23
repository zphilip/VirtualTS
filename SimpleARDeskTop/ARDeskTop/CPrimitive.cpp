#include "CPrimitive.h"



CPrimitive::CPrimitive(void)
: CComponent3D()
{
}


CPrimitive::~CPrimitive()
{
}


void CPrimitive::SetSize(double w, double h, double d)
{
	CComponent3D::SetSize(w, h, d);

	matrix[ 0] =   w; matrix[ 4] = 0.0; matrix[ 8] = 0.0;
	matrix[ 1] = 0.0; matrix[ 5] =   h; matrix[ 9] = 0.0;
	matrix[ 2] = 0.0; matrix[ 6] = 0.0; matrix[10] =   d;
	matrix[ 3] = 0.0; matrix[ 7] = 0.0; matrix[11] = 0.0; matrix[15] = 1.0;

	uniMatrix	= false;
}


CComponent3D* CPrimitive::IsWithin(double x, double y, double z)
{
	if(!touchable || clickedCursor || !visible) return(0);

	double newPos[3];

	InvertPos(x, y, z, newPos);

	return(	-0.5 <= newPos[0] && newPos[0] <= 0.5 &&
			-0.5 <= newPos[1] && newPos[1] <= 0.5 &&
			-0.5 <= newPos[2] && newPos[2] <= 0.5 ? this : 0);
}

