#ifndef ___CWindow
#define ___CWindow

#include <AR/gsub_lite.h>
#include <GL/glut.h>

class CWebCam;


class CWindow
{
public :
			CWebCam**	webCamList;
			int			webCamNum;

			int			gCallCountGetImage;

			int			width;
			int			height;
			double		aspectRatio;
			double		scale;
			double		pixelPitch;

			int			vpX, vpY, vpWidth, vpHeight; 

			int			showWipe;
			int			wipeRatio;

			ARGL_CONTEXT_SETTINGS_REF
						arglSettings;		// Settings from ARGL.

			GLuint		shdProg;

			bool		fullWindow;
public :
						CWindow(void);
	virtual				~CWindow();

	virtual	int			Init(void);
	virtual	int			SetupWindow(void);
	virtual	void		Display(void);
	virtual	void		Reshape(int currentWidth, int currentHeight);
	virtual	void		Keyboard(unsigned char key);
	virtual	void		Mouse(int button, int state, int x, int y) {}
	virtual	void		Quit(void);
	virtual	bool		DetectMarkerAll(void);
};


#endif
