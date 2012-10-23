#ifndef ___CUser
#define ___CUser

class CHand;
class CCursor;
class CComponent3D;




class CUser
{
public :
	enum
	{
		kSelect,
		kTranslate,
		kRotateA,
		kRotateB,
		kScale,
		kDraw,
		kGame,
		kMulti,
		kTwist,
		kSub
	};
public :
	static	int			cursorMode;
	static	int			cursorMode2;
			int			multiCursorMode;
			CHand*		handA;
			CHand*		handB;
public :
						CUser(void);
						~CUser();

			void		Init(void);
			void		Init2(void);
			void		CheckClick(void);
			void		SetCursorMode(int mode);
			CCursor*	FindWithinObject(CComponent3D* object);
};


#endif
