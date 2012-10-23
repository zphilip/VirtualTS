#ifndef ___CKeyboard
#define ___CKeyboard

#include "CFrame.h"
#include "CKeyboardKey.h"
#include "CTextBox.h"


class CKeyboard : public CFrame
{
public :
			int			keyNum;
			int*		keyIndex;
			int*		keyShiftIndex;
			int*		modifierKey;
			wchar_t		charMatrix[8 * 16];

			int			shiftKeyDown;
			int			capsLockDown;

			CKeyboardKey**
						key;
			float		keyColor[4];

			double		topMargin;
			double		leftMargin;
			double		depthMargin;
			double		xKeyMargin;
			double		yKeyMargin;
			double		xKeySize;
			double		yKeySize;

			CTextBox*	easyDisplay;

			CButton3D*	wireButton;
			CButton3D*	rectButton;
			CButton3D*	roundButton;
			CButton3D*	circle1Button;
			CButton3D*	circle2Button;
			CButton3D*	threeDButton;
public :
						CKeyboard(void);
	virtual				~CKeyboard();

	virtual	void		Init(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);

	virtual	void		ItemDown(CComponent3D* item);
	virtual	void		ItemUp(CComponent3D* item);
	virtual	void		Reset(void);

			wchar_t		GetChar(int id);
			void		Reflesh(int shift);
			void		SetStyle1(int pCradMode);
			void		SetStyle2(bool pWire);
	virtual	void		SetCardMode(bool pCard);
};


#endif
