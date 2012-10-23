#ifndef ___CARDTWindow
#define ___CARDTWindow


#include "CWindow.h"



class CARDTWindow : public CWindow
{
public :
						CARDTWindow(void);
	virtual				~CARDTWindow();

	virtual	int			Init(void);
	virtual	void		Display(void);
	virtual	void		Reshape(int currentWidth, int currentHeight);
	virtual	void		Keyboard(unsigned char key);

			void		FlipWipe(void);
};


#endif
