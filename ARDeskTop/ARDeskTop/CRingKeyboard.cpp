#include <stdio.h>
#include <math.h>

#include "CRingKeyboard.h"
#include "Color.h"
#include "PI.h"



CRingKeyboard::CRingKeyboard(void)
: CFrame()
{
	key			= 0;
	easyDisplay	= 0;
}


CRingKeyboard::~CRingKeyboard()
{
	if(key) delete key;
}


void CRingKeyboard::Init(void)
{
	CFrame::Init();

	frame->notifyParent = true;
	key = new CKeyboardKey*[14];

	for(int i = 13; i >= 0; i--) {
//	for(int i = 0; i < 14; i++) {
		key[i] = new CKeyboardKey();
		key[i]->Init();
		key[i]->SetID(i);
		if(i < 10) {
			key[i]->normalColor = keyColor;
			key[i]->frame->SetCurrentColor(keyColor);
		}
		key[i]->SetAlignmentH(CLabel3D::kCenter);
		AddObject(key[i]);
	}

	Color::SetColor4va(keyColor, Color::green, 0.5f);
	key[12]->SetColor3(0.5f, 0.5f, 0.5f);
	key[12]->SetString(L"SP");
	key[13]->SetColor3(0.5f, 0.5f, 0.5f);
	key[13]->SetString(L"BS");

	easyDisplay = new CTextBox();
	easyDisplay->Init();
	AddObject(easyDisplay);
}


void CRingKeyboard::SetSize(double pWidth, double pHeight, double pDepth)
{
	double pos[3] = { 0.0, 0.0, 0.0 };
	double margin = 10.0, keyMargin = 1.0, keySize = 15.0, r = 30.0;
	double rad;
	int i;

	CFrame::SetSize(
		margin * 2.0 + keySize + r * 2.0,
		margin * 3.5 + keySize * 2.0 + r * 2.0,
		pDepth
	);

	easyDisplay->SetSize(keySize + r * 2.0, keySize, depth * 0.5);
	pos[1] = height * 0.5 - margin - keySize * 0.5;
	pos[2] = depth * 0.25;
	easyDisplay->SetPosition(pos);

	for(i = 0; i < 10; i++) {
		rad = -PI * 30.0 * (i < 5 ? i + 1 : 12 - (i - 4)) / 180.0;
		pos[0] = -r * cos(rad);
		pos[1] = -r * sin(rad) - (margin * 1.5 + keySize) * 0.5;
		key[i]->SetSize(keySize, keySize, depth * 0.5);
		key[i]->SetPosition(pos);
	}

	pos[1] = -(margin * 1.5 + keySize) * 0.5;

	pos[0] = -r;
	key[10]->SetSize(keySize, keySize, depth * 0.5);
	key[10]->SetPosition(pos);

	pos[0] = r;
	key[11]->SetSize(keySize, keySize, depth * 0.5);
	key[11]->SetPosition(pos);

	pos[0] = 0.0;
	pos[1] = r * 0.5 - (margin * 1.5 + keySize) * 0.5;
	pos[2] = -depth / 4.0;
	key[12]->SetSize(2.0 * r, r, depth * 0.5);
	key[12]->SetPosition(pos);

	pos[1] = -r * 0.5 - (margin * 1.5 + keySize) * 0.5;
	key[13]->SetSize(2.0 * r, r, depth * 0.5);
	key[13]->SetPosition(pos);

	ResetKeyChars();
}


void CRingKeyboard::Precompute(void)
{
	CFrame::Precompute();
}


void CRingKeyboard::ItemDown(CComponent3D* item)
{
	int keyID = item->GetID();

	if(keyID == -1) return;

	int page[11] = { 2, 3, 2, 3, 1, 4, 1, 1, 1, 2, 2 };

	Color::SetColor4va(keyColor, Color::blue, 0.5f);
	if(keyID <= 9) {
		if(index1 == 10 && index2 == 0) {
			index1 = keyID;
			index2 = 0;
			SetKeyChars();
		} else { //ï∂éöì¸óÕ
			wchar_t* s = ((CKeyboardKey*) item)->label->GetString();
			if(s[0] != '\0') easyDisplay->label->AddString(s);
		}
	} else if(keyID == 10) { //ëO
		if(index1 == 10) {
			if(index2 == 0) {
				index2 = 1;
				SetKeyChars();
			} else {
				ResetKeyChars();
			}
		} else {
			if(index2 == 0) {
				ResetKeyChars();
			} else {
				index2--;
				SetKeyChars();
			}
		}
	} else if(keyID == 11) { //éü
		if(index2 == page[index1] - 1) {
		/*	if(index1 == 10) {
				index2 = 0;
				SetKeyChars();
			} else {*/
				ResetKeyChars();
		//	}
		} else {
			index2++;
			SetKeyChars();
		}
	} else if(keyID == 12) { //ÉXÉyÅ[ÉX
		easyDisplay->label->AddString(L"Å@");
	} else if(keyID == 13) { //BS
		easyDisplay->label->BackSpace();
	}
}


void CRingKeyboard::ItemUp(CComponent3D* item)
{
	if(!withinBB) {
	//	ResetKeyChars();
	}
}


void CRingKeyboard::ResetKeyChars(void)
{
	index1 = 10;
	index2 = 0;
	Color::SetColor4va(keyColor, Color::green, 0.5f);
	SetKeyChars();
}


void CRingKeyboard::SetKeyChars(void)
{
	wchar_t* charMat[4][11][12] = {
		{
			{ L"Ç†", L"Ç¢", L"Ç§", L"Ç¶", L"Ç®", L"Çü", L"Ç°", L"Ç£", L"Ç•", L"Çß", L"ÅÉ", L"ÅÑ" },
			{ L"Ç©", L"Ç´", L"Ç≠", L"ÇØ", L"Ç±", L"Ç™", L"Ç¨", L"ÇÆ", L"Ç∞", L"Ç≤", L"ÅÉ", L"ÅÑ" },
			{ L"Ç≥", L"Çµ", L"Ç∑", L"Çπ", L"Çª", L"Ç¥", L"Ç∂", L"Ç∏", L"Ç∫", L"Çº", L"ÅÉ", L"ÅÑ" },
			{ L"ÇΩ", L"Çø", L"Ç¬", L"Çƒ", L"Ç∆", L"Çæ", L"Ç¿", L"Ç√", L"Ç≈", L"Ç«", L"ÅÉ", L"ÅÑ" },
			{ L"Ç»", L"Ç…", L"Ç ", L"ÇÀ", L"ÇÃ", L"Ç…Ç·", L"", L"Ç…Ç„", L"", L"Ç…ÇÂ", L"ÅÉ", L"ÅÑ" },
			{ L"ÇÕ", L"Ç–", L"Ç”", L"Ç÷", L"ÇŸ", L"ÇŒ", L"Ç—", L"Ç‘", L"Ç◊", L"Ç⁄", L"ÅÉ", L"ÅÑ" },
			{ L"Ç‹", L"Ç›", L"Çﬁ", L"Çﬂ", L"Ç‡", L"Ç›Ç·", L"", L"Ç›Ç„", L"", L"Ç›ÇÂ", L"ÅÉ", L"ÅÑ" },
			{ L"Ç‚", L"", L"Ç‰", L"", L"ÇÊ", L"Ç·", L"", L"Ç„", L"", L"ÇÂ", L"ÅÉ", L"ÅÑ" },
			{ L"ÇÁ", L"ÇË", L"ÇÈ", L"ÇÍ", L"ÇÎ", L"ÇËÇ·", L"", L"ÇËÇ„", L"", L"ÇËÇÂ", L"ÅÉ", L"ÅÑ" },
			{ L"ÇÌ", L"Ç", L"ÇÒ", L"Ç¡", L"Å[", L"ÇÏ", L"ÅA", L"ÅB", L"ÅI", L"ÅH", L"ÅÉ", L"ÅÑ" },
			{ L"Ç†", L"Ç©", L"Ç≥", L"ÇΩ", L"Ç»", L"ÇÕ", L"Ç‹", L"Ç‚", L"ÇÁ", L"ÇÌ", L"ÅÉ", L"ÅÑ" }
		},
		{
			{ L"Éî", L"Ç§Ç°", L"", L"Ç§Ç•", L"Ç§Çß", L"ÉîÇü", L"ÉîÇ°", L"ÉîÇ„", L"ÉîÇ•", L"ÉîÇß", L"ÅÉ", L"ÅÑ" },
			{ L"Ç´Ç·", L"", L"Ç´Ç„", L"", L"Ç´ÇÂ", L"Ç¨Ç·", L"", L"Ç¨Ç„", L"", L"Ç¨ÇÂ", L"ÅÉ", L"ÅÑ" },
			{ L"ÇµÇ·", L"", L"ÇµÇ„", L"ÇµÇ•", L"ÇµÇÂ", L"Ç∂Ç·", L"", L"Ç∂Ç„", L"Ç∂Ç•", L"Ç∂ÇÂ", L"ÅÉ", L"ÅÑ" },
			{ L"ÇøÇ·", L"", L"ÇøÇ„", L"ÇøÇ•", L"ÇøÇÂ", L"Ç¿Ç·", L"", L"Ç¿Ç„", L"", L"Ç¿ÇÂ", L"ÅÉ", L"ÅÑ" },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ L"Çœ", L"Ç“", L"Ç’", L"Çÿ", L"Ç€", L"Ç–Ç·", L"", L"Ç–Ç„", L"", L"Ç–ÇÂ", L"ÅÉ", L"ÅÑ" },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ L"Åu", L"Åv", L"Åw", L"Åx", L"Åh", L"Åi", L"Åj", L"Å{", L"Å|", L"ÅÅ", L"ÅÉ", L"ÅÑ" },
			{ L"ÇP", L"ÇQ", L"ÇR", L"ÇS", L"ÇT", L"ÇU", L"ÇV", L"ÇW", L"ÇX", L"ÇO", L"ÅÉ", L"ÅÑ" }
		},
		{
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ L"Ç≠Çü", L"Ç≠Ç°", L"", L"Ç≠Ç•", L"Ç≠Çß", L"ÇÆÇü", L"", L"", L"", L"", L"ÅÉ", L"ÅÑ" },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ L"Ç¬Çü", L"Ç¬Ç°", L"Ç¡", L"Ç¬Ç•", L"Ç¬Çß", L"ÇƒÇ°", L"Ç≈Ç°", L"Ç¿Ç„", L"Ç≈Ç„", L"", L"ÅÉ", L"ÅÑ" },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ L"Ç“Ç·", L"", L"Ç“Ç„", L"", L"Ç“ÇÂ", L"Ç—Ç·", L"", L"Ç—Ç„", L"", L"Ç—ÇÂ", L"ÅÉ", L"ÅÑ" },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
		},
		{
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ L"Ç”Çü", L"Ç”Ç°", L"Ç”Ç„", L"Ç”Ç•", L"Ç”Çß", L"", L"", L"", L"", L"", L"ÅÉ", L"ÅÑ" },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
		}
	};

	for(int i = 0; i < 12; i++) {
		if(charMat[index2][index1]) key[i]->SetString(charMat[index2][index1][i]);
	}
}

