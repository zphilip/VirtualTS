#ifndef ___CCursorClick
#define ___CCursorClick

#include "CCursor.h"

#define CURSOR_COLLIDE	20.0f

class CMainCursor;


class CSubCursor : public CCursor
{
public :
			CMainCursor*
						mainCursor;
			bool		collide;
			bool		prevCollide;
			float		cursorCollide;
public :
						CSubCursor(void);
	virtual				~CSubCursor();

	virtual	void		Init(void);
	virtual	void		Precompute(int id);
	virtual	int			CheckClick(void);
};


#endif
