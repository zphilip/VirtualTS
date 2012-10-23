#include <stdio.h>
#include <stdlib.h>

#ifndef __APPLE__
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif
#include <AR/gsub.h>
#include <AR/param.h>
#include <AR/ar.h>
#include <AR/video.h>
#include <AR/gsub_lite.h>

#include "CList.h"
#include "CMarkerObject.h"
#include "CProjection.h"
#include "CComponent3D.h"
#include "CHeadTrackingMarker.h"
#include "CWebCam.h"



CList::CList(void)
{
	first	= 0;
	cursor	= 0;
	degree	= 0;
}


CList::~CList()
{
}


CLink* CList::Add(CObject* key)
{
	return(Add(new CLink(key)));
}


CLink* CList::Push(CObject* key)
{
	return(Push(new CLink(key)));
}


CLink* CList::Add(CLink* link)
{
	degree++;

	if(cursor) {
		cursor	= cursor->Insert(link);
	} else if(!first) {
		first	= link;
		first->next = 0;
		cursor	= first;
	}

	return(cursor);
}


CLink* CList::Push(CLink* link)
{
	degree++;

	if(!first) {
		first = link;
		first->next = 0;
	} else {
		link->next = first;
		first = link;
	}

	cursor = first;

	return(cursor);
}


CLink* CList::GoToFirst(void)
{
	cursor = first;

	return(cursor);
}


CLink* CList::GoToNext(void)
{
	if(cursor) {
		CLink* next = cursor->GetNext();
		if(next) cursor = next;
	}

	return(cursor);
}


CLink* CList::FindKey(CObject* key)
{
	if(!first) return(0);

	GoToFirst();
	for(int i = 0; i < degree; i++) {
		if(cursor->GetKey() == key)
			return(cursor);
		GoToNext();
	}

	return(0);
}


CLink* CList::Remove(void)
{
	return(cursor ? Remove(cursor->GetKey()) : 0);
}


CLink* CList::Remove(CObject* key)
{
	if(!first) return(0);

	CLink*	prev = GoToFirst();
	CLink*	ret;
	bool	found = false;

	for(int i = 0; i < degree; i++) {
		if(cursor->GetKey() == key) {
			found = true;
			break;
		}
		prev = cursor;
		GoToNext();
	}

	if(!found) {
		GoToFirst();
		return(0);
	} else if(cursor == first) {
		ret		= first;
		first	= first->next;
		cursor	= first;
	} else {
		ret		= cursor;
		prev->next = cursor->next;
		cursor	= prev;
	}

	degree--;
	ret->next = 0;

	return(ret);
}


int CList::Delete(void)
{
	return(cursor ? Delete(cursor->GetKey()) : 0);
}


int CList::Delete(CObject* key)
{
	CLink* link = Remove(key);
	if(link) {
		link->DeleteKey();
	//	link->Delete();
		delete link;
		return(1);
	}

	return(0);
}


void CList::DeleteAll(void)
{
	if(!GoToFirst()) return;

	CLink* pNext;

	for(int i = 0; i < degree; i++) {
		pNext = cursor->GetNext();
		cursor->DeleteKey();
		cursor->Delete();
		cursor = pNext;
	}

	first	= 0;
	cursor	= 0;
	degree	= 0;
}


int CList::MoveAllLinkTo(CList* that)
{
	if(!first) return(1);

	that->first = this->first;
	this->first = 0;

	that->GoToFirst();
	this->cursor = 0;

	that->degree = this->degree;
	this->degree = 0;

	return(0);
}


CObject* CList::GetKey(void)
{
	return(cursor ? cursor->GetKey() : 0);
}


int CList::IsLast(void) const
{
	return(cursor ? cursor->next == 0 : 0);
}




void CObjectList::Init(void)
{
	if(!first) return;

	GoToFirst();
	for(int i = 0; i < degree; i++) {
		cursor->GetKey()->Init();
		GoToNext();
	}
}


void CObjectList::Precompute(void)
{
	if(!first) return;

	CObject*	object;

	GoToFirst();
	for(int i = 0; i < degree; i++) {
		object = cursor->GetKey();
		if(object->visible) {
			object->Precompute();
		}
		GoToNext();
	}
}


void CObjectList::Draw(void)
{
	if(!first) return;
	CObject*	object;

	GoToFirst();
	for(int i = 0; i < degree; i++) {
		object = cursor->GetKey();
		if(object->visible) {
			object->Draw();
		}
		GoToNext();
	}
}


CComponent3D* CObjectList::IsWithin(double x, double y, double z)
{
	if(!first) return(0);

	CComponent3D* object;
	CComponent3D* hitObject;
	CComponent3D* ret = 0;

	GoToFirst();
	for(int i = 0; i < degree; i++) {
		object = (CComponent3D*) cursor->GetKey();
		if(object->visible && object->touchable && !object->clickedCursor) {
			hitObject = object->IsWithin(x, y, z);
			if(hitObject) ret = hitObject;
		}

		GoToNext();
	}

	return(ret);
}


void CMarkerObjectList::DrawMarkerObject(void)
{
	if(!first) return;

	CMarkerObject*	object;

	GoToFirst();
	for(int i = 0; i < degree; i++) {
		object = (CMarkerObject*) cursor->GetKey();
		object->Draw();

		GoToNext();
	}
}


void CMarkerObjectList::DrawMarkerObject2(void)
{
	if(!first) return;

	CMarkerObject*	object;

	GoToFirst();
	for(int i = 0; i < degree; i++) {
		object = (CMarkerObject*) cursor->GetKey();
		object->Draw2();

		GoToNext();
	}
}


void CMarkerObjectList::DrawRect(void)
{
	if(!first) return;

	CMarkerObject*	object;

	GoToFirst();
	for(int i = 0; i < degree; i++) {
		object = (CMarkerObject*) cursor->GetKey();
		object->DrawRect();

		GoToNext();
	}
}

