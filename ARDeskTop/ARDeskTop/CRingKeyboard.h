#ifndef ___CKeyBoard3
#define ___CKeyBoard3

#include "CFrame.h"
#include "CKeyboardKey.h"
#include "CTextBox.h"


class CRingKeyboard : public CFrame
{
public :
			int			index1, index2;
			float		keyColor[4];
			CKeyboardKey**
						key;
			CTextBox*	easyDisplay;
public :
						CRingKeyboard(void);
	virtual				~CRingKeyboard();

	virtual	void		Init(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);
	virtual	void		Precompute(void);

	virtual	void		ItemDown(CComponent3D* item);
	virtual	void		ItemUp(CComponent3D* item);

			void		ResetKeyChars(void);
			void		SetKeyChars(void);
};


#endif
