#include "CCVPictureFrame.h"



CCVPictureFrame::CCVPictureFrame(void)
: CLabeledCube()
{
	picture		= 0;
//	lockMode	= kButtonUnlock;
	fix			= false;

	commentManager	= 0;
}


CCVPictureFrame::~CCVPictureFrame()
{
	if(commentManager) delete commentManager;
}


void CCVPictureFrame::Init(void)
{
	CLabeledCube::Init();
	frame->SetColor4(1.0f, 1.0f, 1.0f, 0.2f);

	picture = new CCVPicture();
	picture->Init();
//	picture->SetColor4(1.0f, 1.0f, 1.0f, 0.5f);
	AddObject(picture);
}


void CCVPictureFrame::SetSize(double pWidth, double pHeight, double pDepth)
{
	CLabeledCube::SetSize(pWidth, pHeight, pDepth);
	if(picture) picture->FitToFrame();
}


void CCVPictureFrame::Precompute(void)
{
	CLabeledCube::Precompute();

	if(commentManager) {
		commentManager->Animate();
	}
}


bool CCVPictureFrame::OpenFile(wchar_t* pFileName, double size)
{
	if(picture) return(picture->OpenFile(pFileName, size));
	return(false);
}


bool CCVPictureFrame::OpenVideoFile(wchar_t* pFileName, double size)
{
	if(picture) return(picture->OpenVideoFile(pFileName, size));
	return(false);
}


void CCVPictureFrame::Play(void)
{
	CLabeledCube::Play();
	if(commentManager) commentManager->Play();
}


void CCVPictureFrame::Replay(void)
{
	CLabeledCube::Replay();
	if(commentManager) commentManager->Replay();
}


void CCVPictureFrame::Stop(void)
{
	CLabeledCube::Stop();
	if(commentManager) commentManager->Stop();
}
	

void CCVPictureFrame::Pause(void)
{
	CLabeledCube::Pause();
	if(commentManager) commentManager->Pause();
}


void CCVPictureFrame::Animate(void)
{
	CLabeledCube::Animate();
	if(commentManager) commentManager->Animate();
}


void CCVPictureFrame::OpenComment(char* pFileName)
{
	commentManager = new CCommentManager(this);

	commentManager->Init();
	commentManager->Read(pFileName);
	if(animeManager) commentManager->animetionTimeMS = commentManager->animetionTimeMS;
}

