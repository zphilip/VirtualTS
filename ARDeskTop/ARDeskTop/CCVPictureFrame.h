#ifndef ___CCVPictureFrame
#define ___CCVPictureFrame

#include "CLabeledCube.h"
#include "CCVPicture.h"

#include "CCommentManager.h"



class CCVPictureFrame : public CLabeledCube
{
public :
			CCVPicture*	picture;
			CCommentManager*
						commentManager;
public :
						CCVPictureFrame(void);
	virtual				~CCVPictureFrame();

	virtual	void		Init(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);
	virtual	void		Precompute(void);

	virtual	bool		OpenFile(wchar_t* pFileName, double size = 0.0);
	virtual	bool		OpenVideoFile(wchar_t* pFileName, double size = 0.0);

	virtual	void		Play(void);
	virtual	void		Replay(void);
	virtual	void		Stop(void);
	virtual	void		Pause(void);
	virtual	void		Animate(void);

			void		OpenComment(char* pFileName);
};


#endif
