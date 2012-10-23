#include "CVirtualWebCam.h"

#include <AR/gsub_lite.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CWorld.h"
#include "CARDeskTop.h"
#include "CLight.h"
#include "CAnimeMatrix.h"
#include "CAnimeColor.h"
#include "CColorPalette.h"
#include "CTrash.h"
#include "CAnimeColor.h"
#include "Color.h"
#include "CWebCam.h"
#include "CDesktop.h"
#include "CAnimeMatrix.h"
#include "CWindow.h"

#include "CWall.h"
#include "CSphere.h"



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




	CSphere* sphere = new CSphere();
	sphere->Init();
	sphere->Scale(60.0, 60.0, 60.0);
	sphere->SetPosition(0.0, 0.0, -30.0);
	sphere->SetColor4va(Color::green, 1.0f);
	sphere->SetID(objectID++);
	AddObject(sphere);
	
/*
	CCube* rcube = new CCube();
	rcube->Init();
	rcube->SetSize(60.0, 60.0, 60.0);
	rcube->SetPosition(0.0, 0.0, -30.0);
	rcube->SetColor4va(Color::green, 1.0f);
	rcube->SetID(objectID++);
	AddObject(rcube);

*/


/*	CCube* rcube = new CCube();
	rcube->Init();
	rcube->SetSize(60.0, 60.0, 60.0);
	rcube->SetPosition(0.0, 0.0, 30.0);
	rcube->SetColor4va(Color::blue, 0.5f);
	rcube->SetID(objectID++);
	AddObject(rcube);

	CAnimeColor* aColor = new CAnimeColor(rcube);
	rcube->SetAnimeManager(aColor);
	Color::SetColor4(aColor->colorA, 1.0f, 0.0f, 0.0f, 1.0f);
	Color::SetColor4(aColor->colorB, 0.0f, 1.0f, 0.0f, 0.0f);
	aColor->Init();
	aColor->animetionTimeMS = 2.0;
	aColor->repeatTime = -1;
	rcube->Play();

*/

/*
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
	light1->SetPosition(120.0, 150.0, 400.0);
	light1->SetSize(30.0, 30.0, 30.0);
	light1->SetID(objectID++);
	AddObject(light1);

	light1 = new CLight();
	light1->Init();
	light1->lightNo = GL_LIGHT1;
	light1->SetPosition(-20.0, 50.0, 300.0);
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
		} else if(CARDeskTop::mode == CARDeskTop::kVR || CARDeskTop::mode == CARDeskTop::kVR2) {
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
				} else if(CARDeskTop::mode == CARDeskTop::kVR || CARDeskTop::mode == CARDeskTop::kVR2) {
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

