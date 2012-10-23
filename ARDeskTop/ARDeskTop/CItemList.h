#ifndef ___CItemList
#define ___CItemList

#include "CRegion.h"
#include "CLabel3D.h"



class CTag : public CObject
{
public :
			wchar_t		str[256];
public :
	static	CList		tagList;
	static	void		AddTag(wchar_t* tag);
};



class CItem : public CLabel3D
{
public :
			CList		tagList;
public :
						CItem(void);
	virtual				~CItem();

	virtual	void		DrawShape(void);

	virtual	void		CursorEnter(CCursor* cursor);
	virtual	void		CursorLeave(CCursor* cursor);
public :
	static	CFont		fontBig;
public :
	static	void		InitItem(void);
};



class CItemList : public CRegion
{
public :
						CItemList(void);
	virtual				~CItemList();

	virtual	void		Init(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);
	virtual	CComponent3D*
						IsWithin(double x, double y, double z);

	virtual	void		ItemDown(CComponent3D* item);
	virtual	void		ItemUp(CComponent3D* item);

			void		SetItemSize(double pWidth, double pHeight, double pDepth);

			void		ReadFolder(wchar_t* dirName, bool readDir);
			void		FindItems(wchar_t* s, CItemList* itemList);
public :
	static	double		lineHeight;
	static	CGroup*		focus;
};


#endif
