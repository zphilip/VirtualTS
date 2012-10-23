#ifndef ___CResizeBox
#define ___CResizeBox

#include "CButton3D.h"

class CRegion;



class CResizeBox : public CButton3D
{
public :
	enum
	{
		kTopLeft,
		kTopRight,
		kBottomLeft,
		kBottomRight
	};
public :
			int			mode;
			bool		dragging;

			double		firstPos[3];
			double		regionSize[3];
public :
						CResizeBox(void);
	virtual				~CResizeBox();

	virtual	void		ClickDown(CMainCursor* cursor);
//	virtual	void		ClickUp(CMainCursor* cursor);

			void		Resize(CRegion* region);
};


#endif
