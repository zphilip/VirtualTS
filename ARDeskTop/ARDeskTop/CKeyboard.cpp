#include <stdio.h>

#include "CKeyboard.h"
#include "CComponent3D.h"
#include "CLabeledCube.h"
#include "CMainCursor.h"
#include "Color.h"



CKeyboard::CKeyboard(void)
: CFrame()
{
	keyNum			= 0;
	keyIndex		= 0;
	keyShiftIndex	= 0;
	key				= 0;
	shiftKeyDown	= 0;
	capsLockDown	= 0;
	topMargin		= 0.0;
	leftMargin		= 0.0;
	depthMargin		= 0.0;

	easyDisplay		= 0;

	wireButton		= 0;
	rectButton		= 0;
	roundButton		= 0;
	circle1Button	= 0;
	circle2Button	= 0;
	threeDButton	= 0;

	Color::SetColor4va(keyColor, Color::yellow, 0.5f);
}


CKeyboard::~CKeyboard()
{
	if(keyIndex)
		delete keyIndex;
	if(keyShiftIndex)
		delete keyShiftIndex;
	if(key)
		delete key;
}


void CKeyboard::Init(void)
{
	CFrame::Init();

	int i, row = 16;

	wchar_t c = 0x0000;
	for(i = 0; i < 8 * 16; i++) {
		charMatrix[i] = c;
		c += 0x0001;
	}

	keyNum = 53 + 1;//+space
	keyIndex = new int[keyNum];

	keyIndex[ 0] = 7 * row + 14;
	keyIndex[ 1] = 3 * row +  1;
	keyIndex[ 2] = 3 * row +  2;
	keyIndex[ 3] = 3 * row +  3;
	keyIndex[ 4] = 3 * row +  4;
	keyIndex[ 5] = 3 * row +  5;
	keyIndex[ 6] = 3 * row +  6;
	keyIndex[ 7] = 3 * row +  7;
	keyIndex[ 8] = 3 * row +  8;
	keyIndex[ 9] = 3 * row +  9;
	keyIndex[10] = 3 * row +  0;
	keyIndex[11] = 2 * row + 13;
	keyIndex[12] = 2 * row + 11;
	keyIndex[13] = 0 * row +  8; //BS

	keyIndex[14] = 0 * row +  0; //tab?
	keyIndex[15] = 7 * row +  1;
	keyIndex[16] = 7 * row +  7;
	keyIndex[17] = 6 * row +  5;
	keyIndex[18] = 7 * row +  2;
	keyIndex[19] = 7 * row +  4;
	keyIndex[20] = 7 * row +  9;
	keyIndex[21] = 7 * row +  5;
	keyIndex[22] = 6 * row +  9;
	keyIndex[23] = 6 * row + 15;
	keyIndex[24] = 7 * row +  0;
	keyIndex[25] = 5 * row + 11;
	keyIndex[26] = 5 * row + 13;
	keyIndex[27] = 5 * row + 12;

	keyIndex[28] = CAPS_LOCK_KEY_CODE; //caps lock
	keyIndex[29] = 6 * row +  1;
	keyIndex[30] = 7 * row +  3;
	keyIndex[31] = 6 * row +  4;
	keyIndex[32] = 6 * row +  6;
	keyIndex[33] = 6 * row +  7;
	keyIndex[34] = 6 * row +  8;
	keyIndex[35] = 6 * row + 10;
	keyIndex[36] = 6 * row + 11;
	keyIndex[37] = 6 * row + 12;
	keyIndex[38] = 3 * row + 11;
	keyIndex[39] = 2 * row +  7;
	keyIndex[40] = 0 * row + 10; //enter

	keyIndex[41] = SHIFT_KEY_CODE; //shift
	keyIndex[42] = 7 * row + 10;
	keyIndex[43] = 7 * row +  8;
	keyIndex[44] = 6 * row +  3;
	keyIndex[45] = 7 * row +  6;
	keyIndex[46] = 6 * row +  2;
	keyIndex[47] = 6 * row + 14;
	keyIndex[48] = 6 * row + 13;
	keyIndex[49] = 2 * row + 12; //,
	keyIndex[50] = 2 * row + 14; //.
	keyIndex[51] = 2 * row + 15;
	keyIndex[52] = SHIFT_KEY_CODE; //shift

	keyIndex[53] = 2 * row +  0; //space


	keyShiftIndex = new int[keyNum];

	keyShiftIndex[ 0] = 6 * row +  0;
	keyShiftIndex[ 1] = 2 * row +  1;
	keyShiftIndex[ 2] = 4 * row +  0;
	keyShiftIndex[ 3] = 2 * row +  3;
	keyShiftIndex[ 4] = 2 * row +  4;
	keyShiftIndex[ 5] = 2 * row +  5;
	keyShiftIndex[ 6] = 5 * row + 14;
	keyShiftIndex[ 7] = 2 * row +  6;
	keyShiftIndex[ 8] = 2 * row + 10;
	keyShiftIndex[ 9] = 2 * row +  8;
	keyShiftIndex[10] = 2 * row +  9;
	keyShiftIndex[11] = 5 * row + 11;
	keyShiftIndex[12] = 3 * row + 13;
	keyShiftIndex[13] = 0 * row +  8; //BS

	keyShiftIndex[14] = 0 * row +  0; //tab
	keyShiftIndex[15] = 5 * row +  1;
	keyShiftIndex[16] = 5 * row +  7;
	keyShiftIndex[17] = 4 * row +  5;
	keyShiftIndex[18] = 5 * row +  2;
	keyShiftIndex[19] = 5 * row +  4;
	keyShiftIndex[20] = 5 * row +  9;
	keyShiftIndex[21] = 5 * row +  5;
	keyShiftIndex[22] = 4 * row +  9;
	keyShiftIndex[23] = 4 * row + 15;
	keyShiftIndex[24] = 5 * row +  0;
	keyShiftIndex[25] = 7 * row + 11;
	keyShiftIndex[26] = 7 * row + 13;
	keyShiftIndex[27] = 7 * row + 12;

	keyShiftIndex[28] = CAPS_LOCK_KEY_CODE;
	keyShiftIndex[29] = 4 * row +  1;
	keyShiftIndex[30] = 5 * row +  3;
	keyShiftIndex[31] = 4 * row +  4;
	keyShiftIndex[32] = 4 * row +  6;
	keyShiftIndex[33] = 4 * row +  7;
	keyShiftIndex[34] = 4 * row +  8;
	keyShiftIndex[35] = 4 * row + 10;
	keyShiftIndex[36] = 4 * row + 11;
	keyShiftIndex[37] = 4 * row + 12;
	keyShiftIndex[38] = 3 * row + 10;
	keyShiftIndex[39] = 2 * row +  2;
	keyShiftIndex[40] = 0 * row + 10; //enter

	keyShiftIndex[41] = SHIFT_KEY_CODE; //shift
	keyShiftIndex[42] = 5 * row + 10;
	keyShiftIndex[43] = 5 * row +  8;
	keyShiftIndex[44] = 4 * row +  3;
	keyShiftIndex[45] = 5 * row +  6;
	keyShiftIndex[46] = 4 * row +  2;
	keyShiftIndex[47] = 4 * row + 14;
	keyShiftIndex[48] = 4 * row + 13;
	keyShiftIndex[49] = 3 * row + 12;
	keyShiftIndex[50] = 3 * row + 14;
	keyShiftIndex[51] = 2 * row + 15;
	keyShiftIndex[52] = SHIFT_KEY_CODE; //shift

	keyShiftIndex[53] = 2 * row +  0; //space


	key = new CKeyboardKey*[keyNum];
	modifierKey = new int[keyNum];
	for(i = 0; i < keyNum; i++) {
		key[i] = new CKeyboardKey();
		key[i]->Init();
		key[i]->SetID(i);
		key[i]->SetAlignmentH(CLabel3D::kCenter);
		key[i]->normalColor = keyColor;
		key[i]->frame->SetCurrentColor(keyColor);
		AddObject(key[i]);

		modifierKey[i] = 0;
	}

	modifierKey[BACK_SPACE_KEY] = 1; //BS
	key[BACK_SPACE_KEY]->SetString(L"BS");

	modifierKey[TAB_KEY] = 1; //tab
	key[TAB_KEY]->SetString(L"Tab");

	modifierKey[CAPS_LOCK_KEY] = 1; //caps lock
	key[CAPS_LOCK_KEY]->SetString(L"CL");
	key[CAPS_LOCK_KEY]->SetLockMode(CButton3D::kButtonLock);

	modifierKey[ENTER_KEY] = 1; //enter
	key[ENTER_KEY]->SetString(L"Enter");

	modifierKey[SHIFT_KEY_A] = 1; //shift
	key[SHIFT_KEY_A]->SetString(L"Shift");

	modifierKey[SHIFT_KEY_B] = 1; //shift
	key[SHIFT_KEY_B]->SetString(L"Shift");

	Reflesh(0);

	easyDisplay		= new CTextBox();
	easyDisplay->Init();
	easyDisplay->drawBB = true;
	easyDisplay->frame->cardMode = CCube::kRound;
	easyDisplay->frame->SetAlpha(0.9f);
	AddObject(easyDisplay);

	wireButton		= new CButton3D();
	rectButton		= new CButton3D();
	roundButton		= new CButton3D();
	circle1Button	= new CButton3D();
	circle2Button	= new CButton3D();
	threeDButton	= new CButton3D();

	wireButton->Init();
	rectButton->Init();
	roundButton->Init();
	circle1Button->Init();
	circle2Button->Init();
	threeDButton->Init();

	wireButton->SetLockMode(CButton3D::kButtonLock);
	rectButton->SetButtonMode(CButton3D::kRadioButton);
	roundButton->SetButtonMode(CButton3D::kRadioButton);
	circle1Button->SetButtonMode(CButton3D::kRadioButton);
	circle2Button->SetButtonMode(CButton3D::kRadioButton);
	threeDButton->SetButtonMode(CButton3D::kRadioButton);

	frame->cardMode = CCube::kRound;
	wireButton->frame->wire = true;
	rectButton->frame->cardMode = CCube::kRect;
	roundButton->frame->cardMode = CCube::kRound;
	circle1Button->frame->cardMode = CCube::kCircle1;
	circle2Button->frame->cardMode = CCube::kCircle2;
	threeDButton->frame->cardMode = CCube::kRect;

	wireButton->normalColor = keyColor;
	rectButton->normalColor = keyColor;
	roundButton->normalColor = keyColor;
	circle1Button->normalColor = keyColor;
	circle2Button->normalColor = keyColor;
	threeDButton->normalColor = keyColor;

	wireButton->frame->SetCurrentColor(keyColor);
	rectButton->frame->SetCurrentColor(keyColor);
	roundButton->frame->SetCurrentColor(keyColor);
	circle1Button->frame->SetCurrentColor(keyColor);
	circle2Button->frame->SetCurrentColor(keyColor);
	threeDButton->frame->SetCurrentColor(keyColor);

	wireButton->SetCardMode(true);
	rectButton->SetCardMode(true);
	roundButton->SetCardMode(true);
	circle1Button->SetCardMode(true);
	circle2Button->SetCardMode(true);

	threeDButton->Select();

	AddObject(wireButton);
	AddObject(rectButton);
	AddObject(roundButton);
	AddObject(circle1Button);
	AddObject(circle2Button);
	AddObject(threeDButton);
}


void CKeyboard::SetSize(double pWidth, double pHeight, double pDepth)
{
	topMargin	= 20.0;
	leftMargin	= 20.0;
	depthMargin	= 0.0;
	xKeyMargin	= 5.0;
	yKeyMargin	= 5.0;
	xKeySize	= 15.0;
	yKeySize	= 15.0;

	pWidth		= leftMargin * 2.0 + xKeySize * 15.0;
	pHeight		= topMargin * 2.0 + yKeySize * 5.0;
	if(easyDisplay) pHeight += yKeySize;

	CFrame::SetSize(pWidth, pHeight, pDepth);

	int		i, num;
	double	pos[3], keyWidth;

	if(easyDisplay) {
		pos[0] = 0.0;
		pos[1] = height * 0.5 - (topMargin + yKeySize) * 0.5;
		pos[2] = 0.0;
		easyDisplay->SetSize(xKeySize * 8.0 - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
	//	pos[2] = depth * 3.0 / 8.0;
	//	easyDisplay->SetSize(xKeySize * 8.0 - xKeyMargin, yKeySize - yKeyMargin, depth / 4.0);
		easyDisplay->SetPosition(pos);
	}

	if(!key) return;

	//first line
	pos[0] = -width * 0.5 + leftMargin + xKeySize * 0.5;
	pos[1] = height * 0.5 - topMargin - yKeySize * 0.5;
	pos[2] = 0.0;
	if(easyDisplay) pos[1] -= yKeySize;

	//~ to 0
	num = 13;
	for(i = 0; i < num; i++) {
		key[i]->SetSize(xKeySize - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
		key[i]->SetPosition(pos);
		pos[0] += xKeySize;
	}

	//back space
	keyWidth = 2.0;
	pos[0] += (keyWidth - 1.0) * xKeySize * 0.5;
	key[i]->SetSize(xKeySize * keyWidth - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
	key[i]->SetPosition(pos);
	i++;






	//next line
	keyWidth = 1.5;
	pos[0] = -width * 0.5 + leftMargin + xKeySize * keyWidth * 0.5;
	pos[1] -= yKeySize;

	//tab
	key[i]->SetSize(xKeySize * keyWidth - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
	key[i]->SetPosition(pos);
	i++;

	//q to }
	num = i + 12;
	pos[0] = -width * 0.5 + leftMargin + xKeySize * (keyWidth + 0.5);
	for(; i < num; i++) {
		key[i]->SetSize(xKeySize - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
		key[i]->SetPosition(pos);
		pos[0] += xKeySize;
	}

	//|
	keyWidth = 1.5;
	pos[0] += (keyWidth - 1.0) * xKeySize * 0.5;
	key[i]->SetSize(xKeySize * keyWidth - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
	key[i]->SetPosition(pos);
	i++;







	//next line
	keyWidth = 1.75;
	pos[0] = -width * 0.5 + leftMargin + xKeySize * keyWidth * 0.5;
	pos[1] -= yKeySize;

	//caps lock
	key[i]->SetSize(xKeySize * keyWidth - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
	key[i]->SetPosition(pos);
	i++;

	//a to "
	num = i + 11;
	pos[0] = -width * 0.5 + leftMargin + xKeySize * (keyWidth + 0.5);
	for(; i < num; i++) {
		key[i]->SetSize(xKeySize - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
		key[i]->SetPosition(pos);
		pos[0] += xKeySize;
	}

	//enter
	keyWidth = 2.25;
	pos[0] += (keyWidth - 1.0) * xKeySize * 0.5;
	key[i]->SetSize(xKeySize * keyWidth - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
	key[i]->SetPosition(pos);
	i++;






	//next line
	keyWidth = 2.25;
	pos[0] = -width * 0.5 + leftMargin + xKeySize * keyWidth * 0.5;
	pos[1] -= yKeySize;

	//shift
	key[i]->SetSize(xKeySize * keyWidth - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
	key[i]->SetPosition(pos);
	i++;

	//z to ?
	num = i + 10;
	pos[0] = -width * 0.5 + leftMargin + xKeySize * (keyWidth + 0.5);
	for(; i < num; i++) {
		key[i]->SetSize(xKeySize - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
		key[i]->SetPosition(pos);
		pos[0] += xKeySize;
	}

	//shift
	keyWidth = 2.75;
	pos[0] += (keyWidth - 1.0) * xKeySize * 0.5;
	key[i]->SetSize(xKeySize * keyWidth - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
	key[i]->SetPosition(pos);
	i++;





	//next line
	keyWidth = 6.0;
	pos[0] = 0.0;
	pos[1] -= yKeySize;

	//space
	key[i]->SetSize(xKeySize * keyWidth - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
	key[i]->SetPosition(pos);
	i++;





	//design
	wireButton->SetSize(xKeySize - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
	rectButton->SetSize(xKeySize - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
	roundButton->SetSize(xKeySize - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
	circle1Button->SetSize(xKeySize - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
	circle2Button->SetSize(xKeySize - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);
	threeDButton->SetSize(xKeySize - xKeyMargin, yKeySize - yKeyMargin, depth - depthMargin);

	pos[1] -= yKeySize;

	pos[0] = -xKeySize * 2.5;
	wireButton->SetPosition(pos);
	pos[0] += xKeySize;
	rectButton->SetPosition(pos);
	pos[0] += xKeySize;
	roundButton->SetPosition(pos);
	pos[0] += xKeySize;
	circle1Button->SetPosition(pos);
	pos[0] += xKeySize;
	circle2Button->SetPosition(pos);
	pos[0] += xKeySize;
	threeDButton->SetPosition(pos);
}


void CKeyboard::ItemDown(CComponent3D* item)
{
	if(item == wireButton) {
		SetStyle2(!(wireButton->locked));
		SetCardMode(true);
	} else if(item == rectButton) {
		SetStyle1(CCube::kRect);
		SetCardMode(true);
	} else if(item == roundButton) {
		SetStyle1(CCube::kRound);
		SetCardMode(true);
	} else if(item == circle1Button) {
		SetStyle1(CCube::kCircle1);
		SetCardMode(true);
	} else if(item == circle2Button) {
		SetStyle1(CCube::kCircle2);
		SetCardMode(true);
	} else if(item == threeDButton) {
		SetCardMode(false);
	} 

	int itemID = item->GetID();
	if(itemID == -1) return;
	
	wchar_t c = GetChar(itemID);
	int newShift, prevShift = (capsLockDown + shiftKeyDown) % 2;

	switch(itemID) {
		case CAPS_LOCK_KEY :
			capsLockDown = (capsLockDown + 1) % 2;
			break;
		case ENTER_KEY :
			if(easyDisplay) {
				easyDisplay->SetString(L"");
			}
			break;
		case SHIFT_KEY_A :
		case SHIFT_KEY_B :
			shiftKeyDown = 1;
			break;
		case BACK_SPACE_KEY :
			if(easyDisplay) {
				easyDisplay->label->BackSpace();
			}
			break;
		default :
			if(easyDisplay) {
				easyDisplay->label->AddString(c);
			}
			break;
	}

	newShift = (capsLockDown + shiftKeyDown) % 2;
	if(prevShift != newShift)
		Reflesh(newShift);
}


void CKeyboard::ItemUp(CComponent3D* item)
{
	int itemID = item->GetID();
	if(itemID == -1) return;

//	wchar_t c = GetChar(id);
	int newShift, prevShift = (capsLockDown + shiftKeyDown) % 2;

	switch(itemID) {
		case SHIFT_KEY_A :
		case SHIFT_KEY_B :
			shiftKeyDown = 0;
			break;
		default :
			break;
	}

	newShift = (capsLockDown + shiftKeyDown) % 2;
	if(prevShift != newShift)
		Reflesh(newShift);
}


void CKeyboard::Reset(void)
{
	rectButton->Unselect();
	roundButton->Unselect();
	circle1Button->Unselect();
	circle2Button->Unselect();
	threeDButton->Unselect();
}


wchar_t CKeyboard::GetChar(int id)
{
	if((capsLockDown + shiftKeyDown) % 2)
		return(charMatrix[keyShiftIndex[id]]);
	else
		return(charMatrix[keyIndex[id]]);
}


void CKeyboard::Reflesh(int shift)
{
	if(!shift)
		for(int i = 0; i < keyNum; i++) {
			if(!modifierKey[i])
				key[i]->SetString(charMatrix[keyIndex[i]]);
		}
	else
		for(int i = 0; i < keyNum; i++) {
			if(!modifierKey[i])
				key[i]->SetString(charMatrix[keyShiftIndex[i]]);
		}
}


void CKeyboard::SetStyle1(int pCradMode)
{
	for(int i = 0; i < keyNum; i++) {
		key[i]->frame->cardMode = pCradMode;
	}

	wireButton->frame->cardMode = pCradMode;
}


void CKeyboard::SetStyle2(bool pWire)
{
	frame->wire					= pWire;

	for(int i = 0; i < keyNum; i++) {
		key[i]->frame->wire		= pWire;
	}

	rectButton->frame->wire		= pWire;
	roundButton->frame->wire	= pWire;
	circle1Button->frame->wire	= pWire;
	circle2Button->frame->wire	= pWire;
}


void CKeyboard::SetCardMode(bool pCard)
{
	frame->SetCardMode(pCard);

	for(int i = 0; i < keyNum; i++) {
		key[i]->SetCardMode(pCard);
	}

	easyDisplay->SetCardMode(pCard);
}
