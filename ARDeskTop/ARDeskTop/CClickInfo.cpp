#include <math.h>
#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif
#include <stdio.h>

#include <AR/gsub.h>

#include "CClickInfo.h"
#include "CMainCursor.h"
#include "CComponent3D.h"
#include "CCursor.h"
#include "CGroup.h"
#include "CUser.h"



CClickInfo::CClickInfo(void)
{
}


CClickInfo::~CClickInfo(void)
{
}


void CClickInfo::Click(CMainCursor* cursor, CComponent3D* object, int mode)
{
	if(!cursor) return;

	clickPos[0] = cursor->cursorPos[0];
	clickPos[1] = cursor->cursorPos[1];
	clickPos[2] = cursor->cursorPos[2];

	vector[0] = clickPos[0];
	vector[1] = clickPos[1];
	vector[2] = clickPos[2];

	if( CUser::cursorMode == CUser::kTranslate ||
		CUser::cursorMode == CUser::kRotateA ||
		CUser::cursorMode == CUser::kRotateB ||
		CUser::cursorMode == CUser::kMulti ||
		CUser::cursorMode == CUser::kTwist
	) {
		if(object->parent) {
		//	MultiMatrix(cursor->selectObjectMatInv, clickPos, vector);
			object->parent->GlobalToLocal(vector);
		}
		if(CUser::cursorMode == CUser::kMulti && mode == kFirst) {
			object->InvertPos(vector[0], vector[1], vector[2], vector2);
		}

		object->GetPosition(objectPos);

		vector[0] -= objectPos[0];
		vector[1] -= objectPos[1];
		vector[2] -= objectPos[2];

		object->InterruptClick(cursor, mode);

		if(CUser::cursorMode == CUser::kTwist) {
			double temp[3] = { vector[0], vector[1], vector[2] };
			OuterProduct(cursor->normal, temp, vector);
		}
		if(CUser::cursorMode != CUser::kTranslate)
			len = sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
	} else if(CUser::cursorMode == CUser::kScale) {
	//	object->GlobalToLocal(vector);
		if(object->parent)
			MultiMatrix(cursor->selectObjectMatInv, clickPos, vector2);
		object->InvertPos(vector2[0], vector2[1], vector2[2], vector);
		object->InterruptClick(cursor, mode);
	}
}


void CClickInfo::Click2(CCursor* cursor, CComponent3D* object, int mode)
{
	if(!cursor) return;

	clickPos[0] = cursor->cursorPos[0];
	clickPos[1] = cursor->cursorPos[1];
	clickPos[2] = cursor->cursorPos[2];

	vector[0] = clickPos[0];
	vector[1] = clickPos[1];
	vector[2] = clickPos[2];
	
	if(object->parent) {
		MultiMatrix(cursor->selectObjectMatInv, clickPos, vector);
	//	object->parent->GlobalToLocal(vector);
	}
	if(CUser::cursorMode == CUser::kMulti) // && mode == kFirst)
		object->InvertPos(vector[0], vector[1], vector[2], vector2);


	object->GetPosition(objectPos);

	vector[0] -= objectPos[0];
	vector[1] -= objectPos[1];
	vector[2] -= objectPos[2];
}



CClickInfo3::CClickInfo3(void)
{
	cursor = 0;
}


void CClickInfo3::Precompute(double v[3])
{
	d[0] = v[0] - firstClickInfo.vector[0];
	d[1] = v[1] - firstClickInfo.vector[1];
	d[2] = v[2] - firstClickInfo.vector[2];
	dist = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
}



void MultiMatrix(double matrix[], double vector[], double vector2[])
{
	vector2[0] = matrix[ 0] * vector[0] + matrix[ 4] * vector[1] + matrix[ 8] * vector[2] + matrix[12];
	vector2[1] = matrix[ 1] * vector[0] + matrix[ 5] * vector[1] + matrix[ 9] * vector[2] + matrix[13];
	vector2[2] = matrix[ 2] * vector[0] + matrix[ 6] * vector[1] + matrix[10] * vector[2] + matrix[14];
}

double InnerProduct(double vector[], double vector2[])
{
	return(vector[0] * vector2[0] + vector[1] * vector2[1] + vector[2] * vector2[2]);
}


void OuterProduct(double vector[], double vector2[], double out[])
{
	out[0] = vector[1] * vector2[2] - vector[2] * vector2[1];
	out[1] = vector[2] * vector2[0] - vector[0] * vector2[2];
	out[2] = vector[0] * vector2[1] - vector[1] * vector2[0];
}

