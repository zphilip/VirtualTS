//dandelion氏提供プログラムを参考に作成。助かりました。
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>

#include <qedit.h>

#include "CCVVideoManager.h"
#include "CCVPicture.h"



CCVVideoManager::CCVVideoManager(CComponent3D* pComponent)
: CAnimeManager(pComponent)
{
	Reset();
}


CCVVideoManager::~CCVVideoManager()
{
	Reset2();
}


void CCVVideoManager::Play(void)
{
	CAnimeManager::Play();
	if(pMediaControl) pMediaControl->Run();
}


void CCVVideoManager::Stop(void)
{
	CAnimeManager::Stop();
	if(pMediaControl) pMediaControl->Stop();
	OpenVideoFile2(fileName);
}


void CCVVideoManager::Pause(void)
{
	CAnimeManager::Pause();
	if(pMediaControl) pMediaControl->Pause();
}


int CCVVideoManager::Animate(void)
{
	if(!capture) return(0);
	if(!CAnimeManager::Animate()) return(0);

	int n;

	if(dsMode) {
		//DirectShowから現在の再生位置を取得して何フレーム進めるか決める
		LONGLONG pCurrent, pStop;
		pMediaSeeking->GetPositions(&pCurrent, &pStop);
		float current = pCurrent * 0.0000001;

		if(current * fps < counter) return(1);
		n = current * fps - counter + 1;
	} else {
		if(elapsedTimeMS * fps < counter) return(0);
		n = elapsedTimeMS * fps - counter + 1;
	}

	IplImage* image;

	for(int i = 0; i < n; i++) {
		image = cvQueryFrame(capture);
		if(!image) {
			Stop();
			return(1);
		}
	}

	counter += n;

//	if(!frame) frame = cvCreateImage(cvSize(image->width / 2, image->height / 2), IPL_DEPTH_8U, 3);
	cvResize(image, frame, CV_INTER_CUBIC);
	((CCVPicture*) component)->SetTexture(frame);

	return(1);
}


void CCVVideoManager::Reset(void)
{
	pGraphBuilder	= 0;
	pMediaControl	= 0;
	pMediaSeeking	= 0;
	pBasicAudio		= 0;


	capture			= 0;
	frame			= 0;
	fps				= 0.0f;
	counter			= 0;
	dsMode			= false;

	repeat			= 0;
}


void CCVVideoManager::Reset2(void)
{
	if(capture) cvReleaseCapture(&capture);
	if(frame) cvReleaseImage(&frame);

	if(pMediaControl) pMediaControl->Release();
	if(pGraphBuilder) pGraphBuilder->Release();
	if(pMediaSeeking) pMediaSeeking->Release();
	if(pBasicAudio)	pBasicAudio->Release();
}


bool CCVVideoManager::OpenVideoFile(wchar_t* pFileName, double size)
{
	Reset2();
	Reset();

	wcscpy(fileName, pFileName);
	OpenVideoFile2(pFileName);

	printf("fps = %.3f\n", fps);
	dsMode = fps <= 30.0f ? true : false;

	//DirectShow
	if(dsMode) {
		IMediaPosition*		pMediaPosition;

		CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID*) &pGraphBuilder);
		pGraphBuilder->QueryInterface(IID_IMediaControl, (LPVOID*) &pMediaControl);
		pGraphBuilder->QueryInterface(IID_IMediaPosition, (LPVOID*) &pMediaPosition);
		pGraphBuilder->QueryInterface(IID_IMediaSeeking, (LPVOID*) &pMediaSeeking);
		pGraphBuilder->QueryInterface(IID_IBasicAudio, (LPVOID*) &pBasicAudio);

		pGraphBuilder->RenderFile((BSTR) pFileName, NULL);

		TurnOffVideo();

		//再生時間取得
		REFTIME length;
		pMediaPosition->get_Duration(&length);
		animetionTimeMS	= (float) length;
		pMediaPosition->Release();

		if(length == 0.0f) {
			animetionTimeMS = 1000.0f;
			dsMode = false;
		}

	} else { //DirectShowでは再生不可能
		animetionTimeMS = 1000.0f; //適当
	}

	printf("length = %.3f\n", animetionTimeMS);

	IplImage* image = cvQueryFrame(capture);

	counter = 1;

	frame = cvCreateImage(cvSize(image->width * 0.5, image->height * 0.5), IPL_DEPTH_8U, 3);
	cvResize(image, frame, CV_INTER_CUBIC);
	((CCVPicture*) component)->SetTexture(frame);

	return(true);
}


bool CCVVideoManager::OpenVideoFile2(wchar_t* pFileName)
{
	char cFileName[256];

	setlocale(LC_ALL, "ja");
	wcstombs(cFileName, pFileName, 256);

	if(capture) cvReleaseCapture(&capture);

	//OpenCV
	capture = cvCaptureFromFile(cFileName);
	if(!capture) {
		return(false);
	}

	fps = (float) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);

	return(true);
}


void CCVVideoManager::TurnOffVideo(void)
{
	IBaseFilter*	pNullRenderer	= 0;
	IBaseFilter*	pFilter			= 0;
	IBaseFilter*	pVideoRenderer	= 0;
	IPin*			pinIn			= 0;
	IPin*			pinOut			= 0;

	//NULL Rendererを追加
	CoCreateInstance(CLSID_NullRenderer, NULL,CLSCTX_INPROC_SERVER,
		IID_IBaseFilter, (LPVOID *) &pNullRenderer);
	pGraphBuilder->AddFilter(pNullRenderer, L"Null Renderer");

	//Video Rendererを探す
	pGraphBuilder->FindFilterByName(L"Video Renderer", &pVideoRenderer);

	//Video Rendererの入力ピンを探す
	pVideoRenderer->FindPin(L"VMR Input0", &pinIn);

	//出力されているピンを探す
	pinIn->ConnectedTo(&pinOut);

	//NULL Rendererに接続
	pNullRenderer->FindPin(L"In", &pinIn);
	pinOut->Connect(pinIn, NULL);

	//Video Rendererを除去
	pGraphBuilder->RemoveFilter(pVideoRenderer);
	pVideoRenderer->Release();
}


