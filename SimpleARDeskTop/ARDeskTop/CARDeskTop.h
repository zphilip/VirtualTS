#ifndef ___CARDeskTop
#define ___CARDeskTop

#include <stdio.h>

#include "CUser.h"

class CWindow;


class CARDeskTop
{
public :
	//ÉÇÅ[Éh
	enum
	{
		kAR	= 0,
		kVR,
		kVR2,
		kDisplayCalibration,
		kProjectorCalibration,
		kiPadVR,
	};
public :
	static	CARDeskTop*	arDeskTop;
			CWindow*	window;
			CUser*		user;
	static	int			mode;
public :
						CARDeskTop(void);
						~CARDeskTop();

			int			ReadData(char* filename);
			int			ReadConfig(char* filename);

			void		Run(int argc, char** argv, int pmode, char* filename);
			void		CheckClick(void);
			void		KeyboardFunc3D(unsigned char key);
};

char*		GetBuff(char* buf, int n, FILE* fp);

void		Quit(void);
void		ARDTPlaySound(wchar_t* filename);


void		Keyboard(unsigned char key, int x, int y);
void		Mouse(int button, int state, int x, int y);
void		Idle(void);
void		Visibility(int visible);
void		Reshape(int w, int h);
void		Display(void);


#endif


