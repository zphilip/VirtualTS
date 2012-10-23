#ifndef ___CButton3D
#define ___CButton3D

#include "CLabeledCube.h"



class CButton3D : public CLabeledCube
{
public :
	enum
	{
		kButtonUnlock = 0,
		kButtonLock
	};

	enum
	{
		kButton,
		kRadioButton
	};
public :
			int			lockMode;
			int			buttonMode;

			bool		locked;

			float*		normalColor;
			float*		hilightColor;
public :
						CButton3D(void);
	virtual				~CButton3D();

	virtual	void		Init(void);
	virtual	void		Precompute(void);

	virtual	void		CursorEnter(CCursor* cursor);
	virtual	void		CursorLeave(CCursor* cursor);

			void		Select(void);
			void		Unselect(void);
			void		SetButtonMode(int pButtonMode);
			void		SetLockMode(int pLockMode);
};


#endif
