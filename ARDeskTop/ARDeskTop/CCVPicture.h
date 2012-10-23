#ifndef ___CCVPicture
#define ___CCVPicture

#include <cv.h>
#include <highgui.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif


#include "CPrimitive.h"



class CCVPicture : public CPrimitive
{
public :
			GLuint		texName[1];
			int			width2;
			int			height2;
			wchar_t		fileName[256];

			bool		bluebackMode;
			bool		hitBlue;
public :
						CCVPicture(void);
	virtual				~CCVPicture();

	virtual	void		DrawShape(void);

			bool		OpenFile(wchar_t* pFileName, double size = 0.0);
			bool		OpenVideoFile(wchar_t* pFileName, double size = 0.0);
			IplImage*	ConvBlueback(IplImage* image);
			void		SetTexture(IplImage* image);
			void		SetTexture2(IplImage* image);

			void		FitToFrame(void);
};


#endif
