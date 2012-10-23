#include <math.h>
#include <stdio.h>

#ifdef WIN32
#	include <windows.h>
#endif


#include "CPictureRing.h"
#include "CPictureViewer.h"
#include "CCVVideoFrame.h"
#include "CComponent3D.h"
#include "CClickInfo.h"
#include "CMainCursor.h"
#include "CAccViewer.h"
#include "CUser.h"
#include "PI.h"



CPictureRing::CPictureRing(void)
: CFrame()
{
	viewer			= 0;
	pictureSize		= 0.0;
	pictureDepth	= 0.0;
	xMargin			= 0.0;
	yMargin			= 0.0;
}


CPictureRing::~CPictureRing()
{
}


void CPictureRing::Init(void)
{
	CFrame::Init();
	frame->SetAlpha(0.0);
}


void CPictureRing::InterruptClick(CMainCursor* cursor, int mode)
{
	if(CUser::cursorMode == CUser::kRotateA) {
		if(mode == CClickInfo::kFirst)
			cursor->firstClickInfo.vector[1] = 0.0;
		else if(mode == CClickInfo::kMiddle)
			cursor->currentClickInfo.vector[1] = 0.0;
	}
}


void CPictureRing::InterruptRotate(CMainCursor* cursor)
{
}


void CPictureRing::SetPictureSize(double pPictureSize)
{
	pictureSize = pPictureSize;
}


void CPictureRing::SetPictureDepth(double pPictureDepth)
{
	pictureDepth = pPictureDepth;
}


void CPictureRing::SetViewer(CPictureViewer* pViewer)
{
	viewer = pViewer;
}


void CPictureRing::AddTestPicture(wchar_t* dirName, int fileNum, double k)
{
#ifdef WIN32
	CCVPictureFrame*	photo;
	double			photoDepth = 15.0;
	int				i;

	WIN32_FIND_DATA	fd;
	HANDLE			h;
	wchar_t			fileName[512];
	int				len = wcslen(dirName);

	wcscpy(fileName, dirName);
	wcscat(fileName, L"*.jpg");

	h = FindFirstFileEx(fileName, FindExInfoStandard, &fd, FindExSearchNameMatch, NULL, 0);
	if(h == INVALID_HANDLE_VALUE) return;

	i = 0;
	do {
		if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			i++;
			photo = new CCVPictureFrame();
			photo->Init();
			photo->picture->SetColor4(1.0f, 1.0f, 1.0f, 1.0f);
			photo->frame->SetAlpha(0.0f);

			wcscpy(fileName, dirName);
			wcscat(fileName, fd.cFileName);

			photo->picture->OpenFile(fileName, pictureSize * k);
			photo->SetCardMode(true);
			photo->SetSize(pictureSize, pictureSize, pictureDepth);
			AddObject(photo);
		}
	} while(FindNextFile(h, &fd) && i < fileNum);

	FindClose(h);
#endif
}


void CPictureRing::SetSize(double pWidth, double pHeight, double pDepth)
{
	int h, v, i, hNum, vNum, degree;
	CComponent3D* object;
	double dAngle, rAngel, pos[3];

	xMargin			= 10.0;
	yMargin			= 10.0;

	degree	= objectList.GetDegree();
	if(degree == 1) return;

	hNum	= (pWidth - pictureDepth * 2.0) * PI / (pictureSize + xMargin);
	vNum	= (degree - 1) / (double) hNum;
	if((double) vNum != (double) (degree - 1) / (double) hNum) vNum++;

	CFrame::SetSize(pWidth, (double) vNum * (pictureSize + yMargin), pWidth);

	dAngle = 360.0 / hNum;
	rAngel = 2.0 * PI / hNum;
	pos[0] = 0.0;
	pos[1] = (double) (vNum - 1) * (pictureSize + yMargin) * 0.5;
	pos[2] = 0.0;

	//z軸まわりにリング状にオブジェクトを並べる
	i = 1;

	objectList.GoToFirst();
	objectList.GoToNext(); //最初のオブジェクはフレーム
	for(v = 0; v < vNum; v++) {
		for(h = 0; h < hNum; h++) {
			object = (CComponent3D*) objectList.GetKey();

			LoadIdentity(object->matrix);
			object->Rotate(dAngle * (double) h, 0.0, 1.0, 0.0);
			pos[0] = -(width - pictureDepth * 2.0) * 0.5 * sin(rAngel * (double) h);
			pos[2] = -(width - pictureDepth * 2.0) * 0.5 * cos(rAngel * (double) h); //反時計周り
			object->SetPosition(pos);

			if(++i == degree) return;
			objectList.GoToNext();
		}
		pos[1] -= pictureSize + xMargin;
	}
}


void CPictureRing::ItemDown(CComponent3D* item)
{
	CFrame::ItemDown(item);

	if(CAccViewer::accViewer) {
		if(CAccViewer::accViewer->viewer) {
			CAccViewer::accViewer->viewer->OpenFile(((CCVPictureFrame*)item)->picture->fileName);
		}
	}
}
