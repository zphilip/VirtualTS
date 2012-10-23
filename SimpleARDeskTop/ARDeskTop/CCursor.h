#ifndef ___CCursor
#define ___CCursor

#include "CMarkerObject.h"

class CDesktop;
class CComponent3D;
class CProjection;
class CMarkerButton;



class CCursor : public CMarkerObject
{
public :
			double		cursorPos[3];
			CComponent3D**
						prevWithinObject;
			CComponent3D**
						withinObject;
			bool		lock;
			double		selectObjectMatInv[16];
public :
						CCursor(void);
	virtual				~CCursor();

	virtual	void		Init(void);
	virtual	void		Precompute(void);
	virtual	void		Draw2(void);
	virtual	int			Click(void);
	virtual	int			CheckClick(void);
	virtual	CComponent3D*
						GetDropObject(void) { return(0); }

	virtual	void		Precompute(int id);

			void		ProjectCursorPos(CProjection* pro, double pos[3]);
			CCursor*	FindWithinObject(CComponent3D* object);
			CCursor*	FindWithinObject(CComponent3D* object, int index);
			bool		IsWithinObject(void);
			void		ResetWithinObject(void);
public :
	static	int			projectionNum;
	static	int			cursorNum;
	static	int			buttonNum;
	static	CProjection**
						projectionList;
	static	CCursor**	cursorList;
	static	CMarkerButton**
						buttonList;

	static	void		CheckAllWithin(void);
	static	void		CheckAllButton(void);
	static	void		FixAll(void);
};


#endif
