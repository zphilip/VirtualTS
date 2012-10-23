#ifndef ___CLink
#define ___CLink

#include "CList.h"

class CObject;


class CLink
{
	friend	class		CList;
protected :
			CLink*		next;
			CObject*	key;
public :
						CLink(void);
						CLink(CObject*);
						~CLink();

			CLink*		GetNext(void) const { return(next); }
			CObject*	GetKey(void) const { return(key); }
			CObject*	SetKey(CObject* pKey);
			void		DeleteKey(void);
			CLink*		Delete(void);
	
			CLink*		Insert(CLink* that);
};


#endif
