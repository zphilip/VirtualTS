#ifndef ___CHand
#define ___CHand


class CMainCursor;
class CSubCursor;
class CClickButton;
class CCursor;
class CComponent3D;


class CHand
{
public :
			CMainCursor*
						mainCursor;
			CSubCursor*
						subCursor;
			CClickButton*
						button;
			int			keyClick;
			float		clickedTime;
public :
						CHand(void);
						~CHand();

			void		Init(void);
			int			CheckClick(void);
			int			Click(void);
			CCursor*	FindWithinObject(CComponent3D* object);
			CCursor*	FindWithinObject(CComponent3D* object, int index);
			CCursor*	FindVisibleCursor(void);

			void		SetLockCursor(bool lock);
public :
	static	float		doubleClickTime;
};


#endif
