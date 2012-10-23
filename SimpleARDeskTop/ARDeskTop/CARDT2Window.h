#ifndef ___CARDT2Window
#define ___CARDT2Window


#include "CWindow.h"



class CARDT2Window : public CWindow
{
public :
			int			vpWidth2, vpHeight2;
public :
						CARDT2Window(void);
	virtual				~CARDT2Window();

	virtual	int			Init(void);
	virtual	void		Display(void);
	virtual	void		Reshape(int currentWidth, int currentHeight);
	virtual	void		Keyboard(unsigned char key);
	virtual	void		Mouse(int button, int state, int x, int y);
};


#endif
