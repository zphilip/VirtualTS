#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CMMDObject.h"
#include "CMMDMotionManager.h"
#include "CBoundingBox.h"
#include "CBone.h"




CMMDObject::CMMDObject(void)
: CFrame()
{
	flipZaxis		= true;
	isHandling		= false;
}


CMMDObject::~CMMDObject()
{
	g_clPMDModel.release();
	g_clVMDMotion.release();
}


void CMMDObject::Init(void)
{
	CFrame::Init();
	SetColor4(1.0f, 1.0f, 1.0f, 1.0f);
	frame->SetColor4(1.0f, 1.0f, 1.0f, 0.0f);
	frame->drawTransparentCard = false;

	CMMDMotionManager* mana = new CMMDMotionManager(this);
	mana->Init();
	SetAnimeManager(mana);
}


void CMMDObject::Precompute(void)
{
	CFrame::Precompute();
}


void CMMDObject::DrawShape(void)
{
	if(isHandling) {
		UpdateMatrix();
		g_clPMDModel.updateSkinning2();
	}

	glDisable(GL_COLOR_MATERIAL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GEQUAL, 0.05f);

	g_clPMDModel.render();

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	CFrame::DrawShape();
}


void CMMDObject::SetSize(double w, double h, double d)
{
//	CComponent3D::SetSize(w, h, d);
	Scale(w, h, d);
}


void CMMDObject::OpenModel(char* dirName, char* fileName)
{
	char fullPath[256];

	strcpy(fullPath, dirName);
	strcat(fullPath, fileName);
	g_clPMDModel.load(fullPath, dirName);

	g_clPMDModel.updateSkinning();

//	printf("Bone = %d\n", g_clPMDModel.m_unNumBones);
//	printf("IK = %d\n", g_clPMDModel.m_unNumIK);

	BoundsModel();
	SetBones();
	UpdateMatrix();
}


void CMMDObject::OpenMotion(char* fileName)
{
	g_clVMDMotion.load(fileName);
	g_clPMDModel.setMotion(&g_clVMDMotion, false);

	g_clPMDModel.updateMotion(0.0f);
//	printf("Motion = %d\n", g_clVMDMotion.getNumMotionNodes());

	if(animeManager) {
		animeManager->SetAnimationTime(g_clVMDMotion.getMaxFrame() * 30.0f);
	}
}


void CMMDObject::BoundsModel(void)
{
	Vector3 min, max;
	double	width2, height2, depth2;
	double	pos[3];

	g_clPMDModel.getBB(min, max);

	width2	= max.x - min.x;
	height2	= max.y - min.y;
	depth2	= max.z - min.z;
	frame->SetSize(width2, height2, depth2);

	pos[0]	= width2 * 0.5 + min.x;
	pos[1]	= height2 * 0.5 + min.y;
	pos[2]	= depth2 * 0.5 + min.z;
	frame->SetPosition(pos);
}


void CMMDObject::SetBones(void)
{
	if(!g_clPMDModel.m_pBoneArray || !g_clPMDModel.m_pIKArray) return;

	unsigned short	i;
	CFKBone**		bone = new CFKBone*[g_clPMDModel.m_unNumBones];

	for(i = 0; i < g_clPMDModel.m_unNumBones; i++) {
		bone[i] = new CFKBone();
		bone[i]->Init();
		bone[i]->SetColor4(1.0f, 1.0f, 1.0f, 1.0f);
		bone[i]->mmdObject = this;
		bone[i]->pmdBone = &g_clPMDModel.m_pBoneArray[i];
		g_clPMDModel.m_pBoneArray[i].fkBone = bone[i];
	}


	const cPMDBone*	pBone;
	double			pos[3], pos2[3];

	for(i = 0; i < g_clPMDModel.m_unNumBones; i++) {
		pos[0] = g_clPMDModel.m_pBoneArray[i].m_matLocal[3][0];
		pos[1] = g_clPMDModel.m_pBoneArray[i].m_matLocal[3][1];
		pos[2] = g_clPMDModel.m_pBoneArray[i].m_matLocal[3][2];

		pBone = g_clPMDModel.m_pBoneArray[i].m_pParentBone;

		if(!pBone) {
			bone[i]->SetPosition(pos);
			AddObject(bone[i]);
		} else {
			pBone->fkBone->AddObject(bone[i]);

			pos2[0] = pBone->m_matLocal[3][0];
			pos2[1] = pBone->m_matLocal[3][1];
			pos2[2] = pBone->m_matLocal[3][2];

			bone[i]->SetPosition(pos[0] - pos2[0], pos[1] - pos2[1], pos[2] - pos2[2]);

			pBone = g_clPMDModel.m_pBoneArray[i].m_pChildBone;
			if(pBone) {
				pos2[0] = pBone->m_matLocal[3][0];
				pos2[1] = pBone->m_matLocal[3][1];
				pos2[2] = pBone->m_matLocal[3][2];

				bone[i]->handleCube->SetPosition(pos2[0] - pos[0], pos2[1] - pos[1], pos2[2] - pos[2]);
			}
		}
	}

	delete bone;
}


void CMMDObject::UpdateMatrix(void)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glLoadIdentity();

		int degree = objectList.GetDegree();

		objectList.GoToFirst();
		for(int i = 0; i < degree; i++) {
			((CComponent3D*) objectList.GetKey())->UpdateMatrix();
			objectList.GoToNext();
		}
	glPopMatrix();
}

