#include "CVirtualWebCam.h"

#include <AR/gsub_lite.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif
#include <cv.h>
#include <highgui.h>

#include "CWorld.h"
#include "CARDeskTop.h"
#include "CLabeledCube.h"
#include "CLight.h"
#include "CToolWindow.h"
#include "CKeyboard.h"
#include "CRingKeyboard.h"
#include "CWristWatch.h"
#include "CPictureRing.h"
#include "CAnimeMatrix.h"
#include "CSlider.h"
#include "CColorPalette.h"
#include "CTrash.h"
#include "CAnimeColor.h"
#include "Color.h"
#include "CWebCam.h"
#include "CDesktop.h"
#include "CCVPictureFrame.h"
#include "CCVVideoFrame.h"
#include "CAnimeMatrix.h"
#include "CWindow.h"

#include "CFolder.h"

#include "CARInvaders.h"
#include "CReverseCube.h"

#include "CMMDController.h"
#include "CMMDObject.h"
#include "CMMDConfig.h"
#include "CBone.h"

#include "CWall.h"
#include "CItemList.h"
#include "CCVVideoController.h"



CWorld::CWorld(void)
: CGroup()
{
	drawBB	= false;
}


CWorld::~CWorld()
{
}


void CWorld::Init(void)
{
	CGroup::Init();

	int objectID = 0;

/*
	CCVVideoFrame* viewer = new CCVVideoFrame();
	viewer->Init();
//	viewer->OpenFile(L"Data/NO_DATA.PNG");
	viewer->OpenVideoFile(L"angel.mpeg");
	viewer->SetSize(140.0, 140.0, 40.0);
	viewer->SetPosition(0.0, 100.0, 270.0);
	viewer->Scale(1.3);
	viewer->OpenComment("Book1.csv");
	viewer->SetID(objectID++);
	AddObject(viewer);
*/
/*
	CToolWindow* window = new CToolWindow();
	window->Init();
	window->SetSize(100.0, 150.0, WINDOW_DEPTH);
	window->SetPosition(130.0, 0.0, 0.0);
//	window->SetCardMode(true);
	window->SetID(objectID++);
	AddObject(window);
	*/
/*
	CCVVideoController* vCom = new CCVVideoController();
	vCom->Init();
	vCom->SetSize(100.0, 30.0, 30.0);
	vCom->SetPosition(0.0, 0.0, 0.0);
	vCom->SetCardMode(true);
	vCom->pictureFrame = CWall::wall->wallpaper;
	vCom->SetID(objectID++);
	AddObject(vCom);

	CFolder* folder = new CFolder();
	folder->Init();
	folder->SetOrigin(-50.0, 50.0);
	folder->itemList->SetItemSize(70.0, CItemList::lineHeight, 20.0);
	folder->SetSize(100.0, 200.0, 20.0);
	folder->Rotate(-90.0, 1.0, 0.0, 0.0);
	AddObject(folder);

	CFolder* folder2 = new CFolder();
	folder2->Init();
	folder2->SetOrigin(-35.0, 35.0);
	folder2->itemList->SetItemSize(50.0, CItemList::lineHeight, 20.0);
	folder2->SetSize(70.0, 70.0, 20.0);
	folder2->FixAxis(false, false, true);
	folder->AddObject2(folder2);

	folder2 = new CFolder();
	folder2->Init();
	folder2->SetOrigin(-35.0, 35.0);
	folder2->itemList->SetItemSize(50.0, CItemList::lineHeight, 20.0);
	folder2->SetSize(70.0, 70.0, 20.0);
	folder2->SetPosition(0.0, -100.0, 0.0);
	folder2->FixAxis(false, false, true);
	folder->AddObject2(folder2);

	CCube* rcube = new CCube();
	rcube->Init();
	rcube->SetSize(60.0, 60.0, 60.0);
	rcube->SetPosition(0.0, 0.0, -30.0);
	rcube->SetColor4va(Color::white, 1.0f);
	rcube->SetID(objectID++);
	rcube->SetCardMode(true);
	rcube->cardMode = CCube::kSpeechBaloon2;
	rcube->wire = true;
	AddObject(rcube);

	CMMDController* cntr = new CMMDController();
	cntr->Init();
	cntr->SetSize(120.0, 100.0, 20.0);
	cntr->SetPosition(100.0, 35.0, 80.0);
	cntr->SetID(objectID++);
	cntr->SetCardMode(true);
	AddObject(cntr);
*/
/*
	CMMDConfig* cnf = new CMMDConfig();
	cnf->Init();
	cnf->SetSize(80.0, 50.0, 20.0);
	cnf->SetPosition(-80.0, -35.0, 0.0);
	cnf->SetID(objectID++);
	cnf->SetCardMode(true);
	AddObject(cnf);


	CMMDObject* mmd = new CMMDObject();
	mmd->Init();
	mmd->OpenModel("C:/MikuMikuDance_v402/UserFile/Model/", "初音ミク.pmd");
	mmd->SetSize(12.0, 12.0, -12.0);
	mmd->SetPosition(0.0, -600 * 0.2865, -50.0);
	mmd->SetID(objectID++);
	mmd->flipZaxis = true;
//	CMMDController::mmdController->mmdObject = mmd;
	AddObject(mmd);
*/
/*
	CReverseCube* rcube = new CReverseCube();
	rcube->Init();
	rcube->SetSize(100.0, 100.0, 100.0);
	rcube->SetPosition(0.0, 0.0, -30.0);
	rcube->lighting = false;
	rcube->SetColor4va(Color::white, 1.0f);
	rcube->SetID(objectID++);
	AddObject(rcube);

	CCube* rcube = new CCube();
	rcube->Init();
	rcube->SetSize(60.0, 60.0, 60.0);
	rcube->SetPosition(0.0, 0.0, -30.0);
	rcube->SetColor4va(Color::green, 1.0f);
	rcube->SetID(objectID++);
	AddObject(rcube);

	new CARInvaders();
	CARInvaders::arInvaders->Init();
	AddObject(CARInvaders::arInvaders);
*/
/*
	CPictureRing* ring = new CPictureRing();
	ring->Init();
	ring->SetID(objectID++);
	ring->SetPictureSize(30.0);
	ring->SetPictureDepth(15.0);
	ring->AddTestPicture(L"Picture/", 160, 5.0);
	ring->SetSize(500.0, 0.0, 0.0); //後ろのはダミー
	ring->SetID(objectID++);
	AddObject(ring);
*/
/*
	rcube = new CCube();
	rcube->Init();
	rcube->SetSize(60.0, 60.0, 60.0);
	rcube->SetPosition(0.0, 0.0, 30.0);
	rcube->SetColor4va(Color::blue, 0.5f);
	rcube->SetID(objectID++);
	AddObject(rcube);

	CKeyboard* keyboard = new CKeyboard();
	keyboard->Init();
	keyboard->SetSize(100.0, 50.0, 20.0);
	keyboard->SetPosition(100.0, 50.0, 50.0);
	keyboard->Rotate(-45.0, 1.0, 0.0, 0.0);
	keyboard->SetID(objectID++);
	AddObject(keyboard);

	CRingKeyboard* keyboard2 = new CRingKeyboard();
	keyboard2->Init();
	keyboard2->SetSize(70.0, 50.0, 60.0);
	keyboard2->SetPosition(0.0, 50.0, 0.0);
	keyboard2->SetCardMode(true);
	AddObject(keyboard2);

	CSlider* slider = new CSlider();
	slider->Init();
	slider->axis = 2;
	slider->SetSize(20.0, 40.0, 150.0);
	slider->min = 0.0;
	slider->max = 50.0;
//	slider->SetCardMode(true);
	AddObject(slider);

	CTrash* trash = new CTrash();
	trash->Init();
	trash->SetPosition(-150.0, 20.0, 0.0);
	trash->SetSize(40.0, 40.0, 40.0);
	AddObject(trash);

	CColorPalette* palette = new CColorPalette();
	palette->Init();
	palette->SetSize(150.0, 150.0, 20.0);
	palette->SetPosition(150.0, 75.0, 0.0);
	palette->SetCardMode(true);
	palette->SetID(objectID++);
	AddObject(palette);
*/

	CLight* light1 = new CLight();
	light1->Init();
	light1->lightNo = GL_LIGHT0;
	light1->SetPosition(250.0, 220.0, 100.0);
	light1->SetSize(30.0, 30.0, 30.0);
	light1->SetID(objectID++);
	AddObject(light1);

	light1 = new CLight();
	light1->Init();
	light1->lightNo = GL_LIGHT1;
	light1->SetPosition(-120.0, 150.0, 700.0);
	light1->SetSize(30.0, 30.0, 30.0);
	light1->SetID(objectID++);
	AddObject(light1);
}


void CWorld::SortByDepth(CDesktop* desktop)
{
	int i, degree = objectList.GetDegree();

	if(!degree) return;

	CComponent3D* object;
	double m[16];
	double* depthList;
	CVirtualWebCam* vWebCam;

	depthList = new double[degree];
	int* idList = new int[degree];

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		if(CARDeskTop::mode == CARDeskTop::kAR) {
			arglCameraViewRH(desktop->trans, m, VIEW_SCALEFACTOR);
			glLoadMatrixd(m);
			glMultMatrixd(matrix);
		} else if(CARDeskTop::mode == CARDeskTop::kVR) {
			glLoadMatrixd(matrix);
			vWebCam = (CVirtualWebCam*) CARDeskTop::arDeskTop->window->webCamList[0];
		}

		objectList.GoToFirst();
		for(i = 0; i < degree; i++) {
			object = (CComponent3D*) objectList.GetKey();
			idList[i] = object->GetID();

			glPushMatrix();
				glMultMatrixd(object->matrix);
				glGetDoublev(GL_MODELVIEW_MATRIX, m);
				if(CARDeskTop::mode == CARDeskTop::kAR) {
					depthList[i] = m[12] * m[12] + m[13] * m[13] + m[14] * m[14];
				} else if(CARDeskTop::mode == CARDeskTop::kVR) {
					depthList[i] = -(vWebCam->newPos[0] * m[12] + vWebCam->newPos[1] * m[13] + vWebCam->newPos[2] * m[14]);
				//	depthList[i] = -m[14];
				}
			glPopMatrix();

			objectList.GoToNext();
		}
	glPopMatrix();


	BubbleSortd(depthList, idList, degree);

	CList list;
	int j, degree2;

	objectList.MoveAllLinkTo(&list);

	for(i = 0; i < degree; i++) {
		list.GoToFirst();
		degree2 = list.GetDegree();
		for(j = 0; j < degree2; j++) {
			object = (CComponent3D*) list.GetKey();
			if(idList[i] == object->GetID()) {
				objectList.Push(list.Remove(object));
				break;
			}
			list.GoToNext();
		}
	}

	delete depthList;
	delete idList;
}


void BubbleSortd(double a[], int id[], int n)
{
	int i, j, idt;
	double t;

	for(i = 0; i < n - 1; i++)
		for(j = n - 1; j > i; j--)
			if(a[j - 1] > a[j]) {
				t = a[j];
				a[j] = a[j - 1];
				a[j - 1] = t;

				idt = id[j];
				id[j] = id[j - 1];
				id[j - 1] = idt;
			}
}

