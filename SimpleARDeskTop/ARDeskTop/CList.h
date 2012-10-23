#ifndef ___CList
#define ___CList

#include "CLink.h"
#include "CObject.h"

class CComponent3D;


class CList
{
	friend	class		CLink;
protected :
			CLink*		first;
			CLink*		cursor;
			int			degree;
public :
						CList(void);
						~CList();
	
			CLink*		Add(CObject* key);
			CLink*		Push(CObject* key);
			CLink*		Add(CLink* link);
			CLink*		Push(CLink* link);
			CLink*		Remove(void);
			CLink*		Remove(CObject* key);
			int			Delete(void);
			int			Delete(CObject* key);
			void		DeleteAll(void);
			int			MoveAllLinkTo(CList* that);

			int			GetDegree(void) const { return(degree); }
			CObject*	GetKey(void);
			CLink*		GetFirst(void) const { return(first); }

			CLink*		GoToFirst(void);
			CLink*		GoToNext(void);
			CLink*		FindKey(CObject* key);
	
			int			IsEmpty(void) const { return(first == 0); }
			int			IsLast(void) const;
};


class CObjectList : public CList
{
public :
			void		Init(void);
			void		Precompute(void);
			void		Draw(void);
			CComponent3D*
						IsWithin(double x, double y, double z);
			CComponent3D*
						IsWithin(double pos[3])  { return(IsWithin(pos[0], pos[1], pos[2])); }
};


class CMarkerObjectList : public CObjectList
{
public :
			void		DrawMarkerObject(void);
			void		DrawMarkerObject2(void);
			void		DrawRect(void);
};


#endif
