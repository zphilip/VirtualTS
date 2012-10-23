#ifndef ___CCVVideoController
#define ___CCVVideoController

#include "CFrame.h"

class CButton3D;
class CCommentManager;
class CCVPictureFrame;
class CVolumeFader;



class CCVVideoController : public CFrame
{
public :
			CCVPictureFrame*
						pictureFrame;
			CButton3D*	play;
			CButton3D*	stop;
			CButton3D*	pause;
			CButton3D*	blueBack;
			CVolumeFader*
						volume;
public :
						CCVVideoController(void);
	virtual				~CCVVideoController();

	virtual	void		Init(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);
	virtual	void		ItemDown(CComponent3D* item);
	virtual	void		Reset(void);

	virtual	void		Play(void);
	virtual	void		Replay(void);
	virtual	void		Stop(void);
	virtual	void		Pause(void);
	virtual	void		Animate(void);
};


#endif
