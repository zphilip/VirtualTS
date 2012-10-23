#include "CLink.h"
#include "CList.h"
#include "CObject.h"


CLink::CLink(void)
{
	next	= 0;
	key		= 0;
}

CLink::CLink(CObject* pKey)
{
	next	= 0;
	key		= pKey;
}


CLink::~CLink()
{
//	DeleteKey();
}


CObject* CLink::SetKey(CObject* pKey)
{
	DeleteKey();

	key = pKey;

	return(key);
}


void CLink::DeleteKey(void)
{
	if(key) {
		delete key;
		key = 0;
	}
}


CLink* CLink::Insert(CLink* that)
{
	if(!that) return(0);

	that->next	= this->next;
	this->next	= that;
	
	return(that);
}

CLink* CLink::Delete(void)
{
	CLink* pNext = next;

	delete this;

	return(pNext);
}
