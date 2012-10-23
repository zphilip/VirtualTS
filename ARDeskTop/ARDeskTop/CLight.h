#ifndef ___CLight
#define ___CLight

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CPrimitive.h"


class CLight : public CPrimitive
{
public :
			GLenum		lightNo;
			GLfloat		matAmbDiff[4];
			GLfloat		matSpecular[4];
			GLfloat		matShininess;
			GLfloat		lightAmbient[4];
			GLfloat		lightDiffuse[4];
			GLfloat		lightSpecular[4];
public :
						CLight(void);
	virtual				~CLight();

	virtual	void		Init(void);
	virtual	void		DrawShape(void);

			void		SetColor(GLfloat color[4], GLfloat r,  GLfloat g,  GLfloat b,  GLfloat a);
};


#endif
