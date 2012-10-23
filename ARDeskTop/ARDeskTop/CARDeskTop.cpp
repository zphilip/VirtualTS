#ifdef WIN32
#	include <windows.h>
#	include <dshow.h>
#endif

#include <stdio.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include <AR/gsub.h>

#include "CARDeskTop.h"
#include "CDesktop.h"
#include "CCursor.h"
#include "CSubCursor.h"
#include "CList.h"
#include "CMarkerObject.h"
#include "CComponent3D.h"
#include "CMainCursor.h"
#include "CText3D.h"
#include "CHeadTrackingMarker.h"
#include "CWebCam.h"
#include "CMainWebCam.h"
#include "CWorld.h"
#include "CARDTWindow.h"
#include "CAnimeManager.h"
#include "CComment.h"
#include "CAccViewer.h"
#include "CMarkerButton.h"
#include "CClickButton.h"
#include "CUser.h"
#include "CHand.h"
#include "CCube.h"
#include "CReverseCube.h"
#include "CItemList.h"
#include "CVirtualMarker.h"
#include "CCalibrationWindow.h"
#include "CARDT2Window.h"
#include "CWall.h"



CARDeskTop*	CARDeskTop::arDeskTop = 0;
int CARDeskTop::mode = kVR; //kCalibration



CARDeskTop::CARDeskTop(void)
{
	arDeskTop	= this;
	window		= 0;
	user		= 0;
}


CARDeskTop::~CARDeskTop()
{
	if(window) {
		window->Quit();
		delete window;
	}
	if(user) delete user;
}


int CARDeskTop::ReadData(char* filename)
{
	FILE*			fp;
	char			buf[256], buf1[256], className[256], name[256];
	int				i, c, p, b, objectnum;
	bool			doNotAdd, doNotScan, isVirtual;
	CMarkerObject*	object;

	printf("Opening Data File %s\n", filename);

	fp = fopen(filename, "r");
	if(!fp) {
		printf("Can't find the file - quitting\n");
		return(0);
	}

	GetBuff(buf, 256, fp);
	if(sscanf(buf, "%d", &window->webCamNum) != 1) { fclose(fp); return(0); }

	GetBuff(buf, 256, fp);
	if(sscanf(buf, "%d", &CCursor::buttonNum) != 1) { fclose(fp); return(0); }

	GetBuff(buf, 256, fp);
	if(sscanf(buf, "%d", &CCursor::cursorNum) != 1) { fclose(fp); return(0); }

	GetBuff(buf, 256, fp);
	if(sscanf(buf, "%d", &CCursor::projectionNum) != 1) { fclose(fp); return(0); }

	GetBuff(buf, 256, fp);
	if(sscanf(buf, "%d", &CHeadTrackingMarker::HTMode) != 1) { fclose(fp); return(0); }

	if(mode == kAR && window->webCamNum == 1) CHeadTrackingMarker::HTMode = 0;

	GetBuff(buf, 256, fp);
	if(sscanf(buf, "%d", &CHeadTrackingMarker::smoothNum) != 1) { fclose(fp); return(0); }


	CCursor::projectionList	= new CProjection*[CCursor::projectionNum];
	CCursor::cursorList		= new CCursor*[CCursor::cursorNum];
	if(CCursor::buttonNum) CCursor::buttonList		= new CMarkerButton*[CCursor::buttonNum];

	objectnum = CCursor::buttonNum + CCursor::cursorNum + CCursor::projectionNum;
	if(CHeadTrackingMarker::HTMode) objectnum++; //ht marker mode
	if(mode == kCalibration) objectnum++;

	printf("About to load %d Models\n", objectnum);

	b = 0;
	c = 1;
	p = 1;
	for(i = 0; i < objectnum; i++) {
		GetBuff(buf, 256, fp);
		if(sscanf(buf, "%s", className) != 1) { fclose(fp); return(0); }

		doNotAdd	= false;
		doNotScan	= false;
		isVirtual	= false;

		printf("%s\n", className);
		// creat class
		if(strcmp("CDesktop", className) == 0) {
			CDesktop::desktop = new CDesktop();
			object = (CMarkerObject*) CDesktop::desktop;
			CCursor::projectionList[0] = (CProjection*) CDesktop::desktop; //最初にデスクトップ

			if(mode == kVR) {
				doNotScan	= true;
				isVirtual	= true;
			}
		} else if(strcmp("CAccessory", className) == 0) {
			CAccessory*	accessory = new CAccessory();
			object = (CMarkerObject*) accessory;
			CCursor::projectionList[p++] = (CProjection*) accessory;
		} else if(strcmp("CAccViewer", className) == 0) {
			CAccessory*	accessory = new CAccViewer();
			object = (CMarkerObject*) accessory;
			CCursor::projectionList[p++] = (CProjection*) accessory;
		} else if(strcmp("CVirtualMarker", className) == 0) {
			CVirtualMarker*	virtualMarker = new CVirtualMarker();
			object = (CMarkerObject*) virtualMarker;
			CCursor::projectionList[p++] = (CProjection*) virtualMarker;
			doNotScan = true;
			isVirtual = true;
		} else if(strcmp("HandA.CMainCursor", className) == 0) {
			user->handA->mainCursor = new CMainCursor();
			object = (CMarkerObject*) user->handA->mainCursor;
			CCursor::cursorList[0] = (CCursor*) user->handA->mainCursor;
		} else if(strcmp("HandB.CMainCursor", className) == 0) {
			user->handB->mainCursor = new CMainCursor();
			object = (CMarkerObject*) user->handB->mainCursor;
			CCursor::cursorList[c++] = (CCursor*) user->handB->mainCursor;
		} else if(strcmp("HandA.CSubCursor", className) == 0) {
			user->handA->subCursor = new CSubCursor();
			object = (CMarkerObject*) user->handA->subCursor;
			CCursor::cursorList[c++] = (CCursor*) user->handA->subCursor;
		} else if(strcmp("HandB.CSubCursor", className) == 0) {
			user->handB->subCursor = new CSubCursor();
			object = (CMarkerObject*) user->handB->subCursor;
			CCursor::cursorList[c++] = (CCursor*) user->handB->subCursor;
		} else if(strcmp("HandA.CClickButton", className) == 0) {
			user->handA->button = new CClickButton();
			object = (CMarkerObject*) user->handA->button;
			CCursor::buttonList[b++] = user->handA->button;
		} else if(strcmp("HandB.CClickButton", className) == 0) {
			user->handB->button = new CClickButton();
			object = (CMarkerObject*) user->handB->button;
			CCursor::buttonList[b++] = user->handB->button;
		} else if(strcmp("CCursor", className) == 0) {
			CCursor* cursor = new CCursor();
			object = (CMarkerObject*) cursor;
			CCursor::cursorList[c++] = (CCursor*) cursor;
		} else if(strcmp("CHeadTrackingMarker", className) == 0) {
			CHeadTrackingMarker::HTMarker = new CHeadTrackingMarker();
			object = CHeadTrackingMarker::HTMarker;
			if(mode == kAR) doNotAdd = true;
		} else if(strcmp("CCalibrationMarker", className) == 0) {
			CCalibrationWindow::calibMarker = new CMarkerObject();
			object = CCalibrationWindow::calibMarker;
			object->drawRect	= true;
			doNotAdd = true;
		} else {
			printf("not found class : %s\n", className);
			fclose(fp); return(0);
		}

		if(isVirtual) {
			CProjection* proj = (CProjection*) object;

			object->visible	= true;
			object->fix		= true;

			for(int x = 0; x < 3; x++) {
				GetBuff(buf, 256, fp);
				if(sscanf(buf, "%lf %lf %lf %lf",
					&object->trans[x][0],
					&object->trans[x][1],
					&object->trans[x][2],
					&object->trans[x][3]
					) != 4
				) {
					fclose(fp); delete object; return(0);
				}
			}

			arUtilMatInv(object->trans, proj->invMat);
		}

		if(!doNotScan) {
			GetBuff(buf, 256, fp);
			if(sscanf(buf, "%s", name) != 1) {
				fclose(fp); delete object; return(0);
			}

			printf("Read in No.%d \n", i + 1);

			GetBuff(buf, 256, fp);
			if(sscanf(buf, "%s", buf1) != 1) {
				fclose(fp); delete object; return(0);
			}
			printf("patt file name : %s\n", buf1);

			if((object->id = arLoadPatt(buf1)) < 0) {
				fclose(fp); delete object; return(0);
			}
			printf("patt id : %d\n", object->id);

			GetBuff(buf, 256, fp);
			if(sscanf(buf, "%lf", &object->markerWidth) != 1) {
				fclose(fp); delete object; return(0);
			}

			GetBuff(buf, 256, fp);
			if(sscanf(buf, "%lf %lf", &object->cardRect[0][0],
				&object->cardRect[0][1]) != 2) {
				fclose(fp); delete object; return(0);
			}

			GetBuff(buf, 256, fp);
			if(sscanf(buf, "%lf %lf", &object->cardRect[1][0],
				&object->cardRect[1][1]) != 2) {
				fclose(fp); delete object; return(0);
			}

			GetBuff(buf, 256, fp);
			if(sscanf(buf, "%lf %lf", &object->cardRect[2][0],
				&object->cardRect[2][1]) != 2) {
				fclose(fp); delete object; return(0);
			}

			GetBuff(buf, 256, fp);
			if(sscanf(buf, "%lf %lf", &object->cardRect[3][0],
				&object->cardRect[3][1]) != 2) {
				fclose(fp); delete object; return(0);
			}

			GetBuff(buf, 256, fp);
			if(sscanf(buf, "%lf %lf", &object->markerCenter[0],
				&object->markerCenter[1]) != 2) {
				fclose(fp); delete object; return(0);
			}
		}

		if(!doNotAdd) CMainWebCam::markerObjectList.Add((CObject*) object);
	}

	fclose(fp);

	return(1);
}


int CARDeskTop::ReadConfig(char* filename)
{
	FILE*			fp;
	char			buf[256];

	if((fp = fopen(filename, "r")) == 0) {
		printf("Can't find the file - quitting\n");
		return(0);
	}

	fclose(fp);

	return(1);
}


void CARDeskTop::Run(int argc, char** argv, int pmode, char* filename)
{
	mode = pmode;

	glutInit(&argc, argv);

	//DirectShow
	CoInitialize(NULL);

	if(atexit(Quit) < 0) {
		exit(-1); // Bail out if we can't even register our exit function.
	}

	switch(mode) {
	case kAR :
		window	= new CARDTWindow();
		break;
	case kVR :
		window	= new CARDT2Window();
		break;
	case kCalibration :
		window	= new CCalibrationWindow();
		break;
	}

	user	= new CUser();
	user->Init();

	CAnimeManager::currentTime	= glutGet(GLUT_ELAPSED_TIME) * 0.001;

	if(!ReadData(filename)) return;

	user->Init2();
	window->Init();

	CCube::InitCube();
	CText3D::InitText();
	CComment::InitComment();
	CItem::InitItem();
	CReverseCube::InitReverseCube();

	if(mode == kVR) {
		CWall::wall	= new CWall();
		CWall::wall->Init();
	}

	if(CHeadTrackingMarker::HTMarker) CHeadTrackingMarker::HTMarker->Init();

	CMainWebCam::markerObjectList.Init();

	arUtilTimerReset();
	glutMainLoop();
}


void CARDeskTop::CheckClick(void)
{
	if(user) user->CheckClick();
}


void CARDeskTop::KeyboardFunc3D(unsigned char key)
{
	if(window) window->Keyboard(key);
}




char* GetBuff(char *buf, int n, FILE *fp)
{
	char* ret;

	for(;;) {
		ret = fgets(buf, n, fp);
		if(ret == 0) return(0);
		if(buf[0] != '\n' && buf[0] != '#') return(ret);
	}
}


void Quit(void)
{
	if(CCursor::projectionList) delete CCursor::projectionList;
	if(CCursor::cursorList)		delete CCursor::cursorList;
	if(CCursor::buttonList)		delete CCursor::buttonList;

	//DirectShow
	CoUninitialize();

	argCleanup();
	delete CARDeskTop::arDeskTop;

//	arVideoCapStop();
//	arVideoClose();
	exit(0);
}


void ARDTPlaySound(wchar_t* filename)
{
#ifdef WIN32
	PlaySound(filename, 0, SND_FILENAME | SND_ASYNC);
#endif
}


void Keyboard(unsigned char key, int x, int y)
{
	CARDeskTop::arDeskTop->KeyboardFunc3D(key);
}


void Idle(void)
{
	float	s_elapsed;

	CAnimeManager::currentTime = glutGet(GLUT_ELAPSED_TIME) * 0.001;

//	s_elapsed = (float) (CAnimeManager::currentTime - CAnimeManager::prevTime) * 0.001f;
	s_elapsed = CAnimeManager::currentTime - CAnimeManager::prevTime;
	if(s_elapsed < 0.01f) return; // Don't update more often than 100 Hz.

	CAnimeManager::prevTime = CAnimeManager::currentTime;

	if(CARDeskTop::arDeskTop->window) {
		if(CARDeskTop::arDeskTop->window->DetectMarkerAll())
			glutPostRedisplay();
	}
}


void Mouse(int button, int state, int x, int y)
{
	if(CARDeskTop::arDeskTop->window)
		CARDeskTop::arDeskTop->window->Mouse(button, state, x, y);
}


void Visibility(int visible)
{
	if(visible == GLUT_VISIBLE) {
		glutIdleFunc(Idle);
	} else {
		glutIdleFunc(0);
	}
}


void Reshape(int w, int h)
{
	if(CARDeskTop::arDeskTop->window)
		CARDeskTop::arDeskTop->window->Reshape(w, h);
}


void Display(void)
{
	if(CARDeskTop::arDeskTop->window) {
		CARDeskTop::arDeskTop->window->Display();
		glutSwapBuffers();
	}
}

