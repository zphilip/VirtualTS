#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CVirtualMarker.h"



CVirtualMarker::CVirtualMarker(void)
: CProjection()
{
	visible	= true;
	fix		= true;
}


CVirtualMarker::~CVirtualMarker()
{
}


void CVirtualMarker::Init(void)
{
	CProjection::Init();

//	trans[0][0] =  1.0; trans[0][1] =  0.0; trans[0][2] =  0.0; trans[0][3] =  0.0;
//	trans[1][0] =  0.0; trans[1][1] = -1.0; trans[1][2] =  0.0; trans[1][3] =  0.0;
//	trans[2][0] =  0.0; trans[2][1] =  0.0; trans[2][2] = -1.0; trans[2][3] = 400.0;

	arUtilMatInv(trans, invMat);
}


void CVirtualMarker::Precompute(void)
{
	CMarkerObject::Precompute();
}


void CVirtualMarker::DrawShape(void)
{
//		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//		glutSolidCube(50.0);
		glScaled(10.0, 10.0, 10.0);
		glBegin(GL_POLYGON);
			glVertex3d(1.0, 1.0, 0.0);
			glVertex3d(1.0, -1.0, 0.0);
			glVertex3d(-1.0, -1.0, 0.0);
			glVertex3d(-1.0, 1.0, 0.0);
		glEnd();
}


double CVirtualMarker::GetTransMat(ARMarkerInfo* info)
{
	return(0.0);
}

