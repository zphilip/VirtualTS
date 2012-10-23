#ifndef ___CCVEffect
#define ___CCVEffect

#include <cv.h>

#define PATT_NUM	7
#define MAX_POINT	100000

class PointTable {
public :
			CvPoint		p;
			CvScalar	color;
};




class CCVEffect
{
public :
						CCVEffect(void) {}
	virtual				~CCVEffect() {}

	virtual	void		Init(CvSize size) {}
	virtual	void		EffectImage(IplImage* back, IplImage* frame, IplImage* alpha, IplImage* mask, IplImage* res) = 0;
};


class CHSVToonEffect : public CCVEffect
{
public :
			IplImage*	hsv;  
			IplImage*	temp;  
public :
						CHSVToonEffect(void);
	virtual				~CHSVToonEffect();

	virtual	void		Init(CvSize size);
	virtual	void		EffectImage(IplImage* back, IplImage* frame, IplImage* alpha, IplImage* mask, IplImage* res);

			void		HSVToon(IplImage* frame, IplImage* base);
};


//�\�[�X�͂����炩�炨�؂肵�܂����B
//http://www.purple.dti.ne.jp/~t-ogura/animeEffect

class CHandDrawEffect : public CCVEffect
{
public :
			int			X;				// LineStep �傫���ƃK�N�K�N��
			int			Y;				// ���o���o�@�傫���Ɠ������͑a��
			int			Z;				// LineLength �}�C�i�X�œ_���ɁA�v���X�Ŗ��[���I�[�o�[
			int			lineWidth;		// ���̑���
			int			lineNoise;		// �ߐڃs�N�Z���Ƃ̔�r����臒l�B0�ȏ�ɂ���Ɛ����r�؂��
			double		DARK;			// �G�b�W�̐F�̋P�x

			int			VFlag;			// �K���}�␳�݂����ȁH
			int			hatchingType;	// 0..normal 1..inverse 2..white-base
			int			drawMode;
			int			patternOffset;
			IplImage*	hatching[PATT_NUM];
			PointTable	pointTable[MAX_POINT];

			uchar		posterizeFilter;

			IplImage*	hsv;  
			IplImage*	grayImage;
			IplImage*	binaryImage;
			IplImage*	imageA;
			IplImage*	imageB;
			IplImage*	imageC;
			IplImage*	imageD;
			IplImage*	imageE;
			IplImage*	scanningLine;
			CvMemStorage*
						memStorage0; 
public :
						CHandDrawEffect(void);
	virtual				~CHandDrawEffect();

	virtual	void		Init(CvSize size);
	virtual	void		EffectImage(IplImage* back, IplImage* frame, IplImage* alpha, IplImage* mask, IplImage* res);

			void		DrawEdge(IplImage* image, IplImage* image2, IplImage* base, int plane = 2);
			int			DrawHatching(IplImage* frame, IplImage* base);
			int			SetPoint(int index, CvPoint *point, CvScalar &color);
};

			void		AlphaBlend(IplImage* srcA, IplImage* srcB, IplImage* alpha, IplImage* dst);
			void		Posterize(uchar posterizeFilter, IplImage* frame, IplImage* base);

double		ColorLen(CvScalar& x, CvScalar& y);
CvScalar	GetColor(IplImage* image, int x, int y);
CvScalar	GetColorCheck(IplImage* image, int x, int y);
void		DivScaler(CvScalar& s, double d);
void		MulScaler(CvScalar& s, double d);
int			CheckPoint(IplImage* image, int x, int y, double th);
float		Clip(float x, float min, float max);



#endif