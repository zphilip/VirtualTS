#ifndef ___CCVVideoManager
#define ___CCVVideoManager
//dandelion氏提供プログラムを参考に作成。助かりました。

#include <cv.h>
#include <highgui.h>
#include <dshow.h>


#include "CAnimeManager.h"



class CCVVideoManager : public CAnimeManager
{
public :
			wchar_t		fileName[256];
			IGraphBuilder*
						pGraphBuilder;
			IMediaControl*
						pMediaControl;
			IMediaSeeking*
						pMediaSeeking;
			IBasicAudio*
						pBasicAudio;

			CvCapture*	capture;
			IplImage*	frame;
			int			counter;
			float		fps;
			bool		dsMode;
public :
						CCVVideoManager(CComponent3D* pComponent = 0);
	virtual				~CCVVideoManager();

	virtual	void		Play(void);
	virtual	void		Stop(void);
	virtual	void		Pause(void);
	virtual	int			Animate(void);

			void		Reset(void);
			void		Reset2(void);
			bool		OpenVideoFile(wchar_t* pFileName, double size = 0.0);
			bool		OpenVideoFile2(wchar_t* pFileName);
			void		TurnOffVideo(void);
};


#endif
