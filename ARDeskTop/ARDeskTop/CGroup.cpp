#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif
#include <AR/gsub.h>

#include "CGroup.h"
#include "CTrash.h"



CGroup::CGroup(void)
: CComponent3D()
{
	checkWithinBBOnly		= false;
	withinBB				= false;
}


CGroup::~CGroup()
{
	objectList.DeleteAll();
}


void CGroup::Init(void)
{
	CComponent3D::Init();
	objectList.Init();
}


void CGroup::Precompute(void)
{
	CComponent3D::Precompute();
	objectList.Precompute();
	withinBB = false;
}


void CGroup::DrawShape(void)
{
	objectList.Draw();
}


CComponent3D* CGroup::IsWithin(double x, double y, double z)
{
	if(!touchable || clickedCursor || !visible) return(0);

	double newPos[3];
	CComponent3D* hit = 0;

	InvertPos(x, y, z, newPos);
	if(boundingBox)
		hit = boundingBox->IsWithin(newPos);
	withinBB = withinBB || hit;

	if(hit) {
		if(checkWithinBBOnly) {
			hit = this;
		} else {
			CComponent3D* item;
			bool t = boundingBox->touchable;
			
			boundingBox->touchable = false;
			item = objectList.IsWithin(newPos);
			if(item) hit = item;
			boundingBox->touchable = t;
		}
	} else {
		hit = objectList.IsWithin(newPos);
	}

	return(hit);


/*	if(!touchable || clickedCursor || !visible) return(0);

	double newPos[3];

	InvertPos(x, y, z, newPos);

	return(objectList.IsWithin(newPos));
*/
}


void CGroup::SetSize(double pWidth, double pHeight, double pDepth)
{
	if(boundingBox) {
		width	= pWidth;
		height	= pHeight;
		depth	= pDepth;

		boundingBox->SetSize(width, height, depth);
	} else {
		CComponent3D::SetSize(pWidth, pHeight, pDepth);
	}
}


void CGroup::AddObject(CComponent3D* object)
{
	object->SetParent(this);
	objectList.Add(object);
}


CLink* CGroup::RemoveObject(CComponent3D* object)
{
	return(objectList.Remove(object));
}


int CGroup::DeleteObject(CComponent3D* object)
{
	CLink* link = RemoveObject(object);
	if(link) CTrash::dust.Add(link);
	return(1);
}


void CGroup::ItemDown(CComponent3D* item)
{
}


void CGroup::ItemUp(CComponent3D* item)
{
}


void CGroup::ItemClickDown(CComponent3D* item)
{
}


void CGroup::ItemClickUp(CComponent3D* item)
{
}


void CGroup::Reset(void)
{
}


void CGroup::SetCardMode(bool pCard)
{
	CComponent3D::SetCardMode(pCard);

	int degree = objectList.GetDegree();

	objectList.GoToFirst();
	for(int i = 0; i < degree; i++) {
		((CComponent3D*) objectList.GetKey())->SetCardMode(pCard);
		
		objectList.GoToNext();
	}
}


void CGroup::Play(void)
{
	CComponent3D::Play();

	int degree = objectList.GetDegree();

	objectList.GoToFirst();
	for(int i = 0; i < degree; i++) {
		((CComponent3D*) objectList.GetKey())->Play();

		objectList.GoToNext();
	}
}


void CGroup::Replay(void)
{
	CComponent3D::Replay();

	int degree = objectList.GetDegree();

	objectList.GoToFirst();
	for(int i = 0; i < degree; i++) {
		((CComponent3D*) objectList.GetKey())->Replay();

		objectList.GoToNext();
	}
}


void CGroup::Stop(void)
{
	CComponent3D::Stop();

	int degree = objectList.GetDegree();

	objectList.GoToFirst();
	for(int i = 0; i < degree; i++) {
		((CComponent3D*) objectList.GetKey())->Stop();

		objectList.GoToNext();
	}
}
	

void CGroup::Pause(void)
{
	CComponent3D::Pause();

	int degree = objectList.GetDegree();

	objectList.GoToFirst();
	for(int i = 0; i < degree; i++) {
		((CComponent3D*) objectList.GetKey())->Pause();

		objectList.GoToNext();
	}
}


void CGroup::Animate(void)
{
	CComponent3D::Animate();

	int degree = objectList.GetDegree();

	objectList.GoToFirst();
	for(int i = 0; i < degree; i++) {
		((CComponent3D*) objectList.GetKey())->Animate();

		objectList.GoToNext();
	}
}


CGroup* FindNotFixed(CGroup* object)
{
	if(!object)
		return(0);
	else if(!object->fix)
		return(object);
	else if(object->parent)
		return(FindNotFixed(object->parent));
	else
		return(0);
}

