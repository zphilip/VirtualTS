#ifndef ___CLine3D
#define ___CLine3D

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CComponent3D.h"
#include "CList.h"

class CVertex;


class CLine3D : public CComponent3D
{
public :
			GLenum		lineType;	
			float		lineWidth;
			CList		vertexList;
public :
						CLine3D(void);
	virtual				~CLine3D();

	virtual	void		Init(void);
	virtual	void		DrawShape(void);
	virtual	void		Click(void);

			void		AddVertex(CVertex* vertex);
			void		Bound(void);
			void		SetLineWidth(float pLineWidth);
};


#endif
