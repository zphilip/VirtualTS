
#include <stdio.h>

#include <highgui.h>

#include "CCVEffect.h"



CHSVToonEffect::CHSVToonEffect(void)
: CCVEffect()
{
	hsv			= 0;
	temp		= 0;
}


CHSVToonEffect::~CHSVToonEffect()
{
	if(hsv)			cvReleaseImage(&hsv);
	if(temp)		cvReleaseImage(&temp);
}


void CHSVToonEffect::Init(CvSize size)
{
	CCVEffect::Init(size);

	hsv			= cvCreateImage(size, IPL_DEPTH_8U, 3);  
	temp		= cvCreateImage(size, IPL_DEPTH_8U, 3);  
}


void CHSVToonEffect::EffectImage(IplImage* back, IplImage* frame, IplImage* alpha, IplImage* mask, IplImage* res)
{
	HSVToon(frame, temp);
	AlphaBlend(back, temp, alpha, res);
}


void CHSVToonEffect::HSVToon(IplImage* frame, IplImage* base)
{
	CvPixelPosition8u posA;
	uchar* pA;
	CvSize size;
	int x, y;
	int v = 2;

	size = cvGetSize(frame);

	cvCvtColor(frame, hsv, CV_BGR2HSV);


	x = 0; y = 0;
	CV_INIT_PIXEL_POS(posA, (unsigned char*) hsv->imageData,
						hsv->widthStep, size, x, y, hsv->origin);

	for(y = 0; y < size.height; y++) {
		for(x = 0; x < size.width; x++) {
			pA = CV_MOVE_TO(posA, x, y, 3);

			if(pA[v] >= 255 * 0.8) pA[v] = 255;
			else if(pA[v] >= 255 * 0.5) pA[v] = 204;
			else if(pA[v] >= 255 * 0.3) pA[v] = 127;
			else pA[v] = 63;
		}
	}

	cvCvtColor(hsv, base, CV_HSV2BGR);
}





//ソースはこちらからお借りしました。
//http://www.purple.dti.ne.jp/~t-ogura/animeEffect

CHandDrawEffect::CHandDrawEffect(void)
: CCVEffect()
{
	hsv			= 0;  
	grayImage	= 0;
	binaryImage	= 0;
	imageA		= 0;
	imageB		= 0;
	imageC		= 0;
	imageD		= 0;
	imageE		= 0;
	scanningLine = 0;
	memStorage0	= 0; 
}


CHandDrawEffect::~CHandDrawEffect()
{
	for(int i = 0; i < PATT_NUM; i++) {
		if(hatching[i])
			cvReleaseImage(&hatching[i]);
	}

	if(hsv)			cvReleaseImage(&hsv);
	if(grayImage)	cvReleaseImage(&grayImage);
	if(binaryImage)	cvReleaseImage(&binaryImage);
	if(imageA)		cvReleaseImage(&imageA);
	if(imageB)		cvReleaseImage(&imageB);
	if(imageC)		cvReleaseImage(&imageC);
	if(imageD)		cvReleaseImage(&imageD);
	if(imageE)		cvReleaseImage(&imageE);
	if(scanningLine) cvReleaseImage(&scanningLine);
	if(memStorage0)	cvReleaseMemStorage(&memStorage0);
}


void CHandDrawEffect::Init(CvSize size)
{
	CCVEffect::Init(size);

	X			= 5;
	Y			= 54;
	Z			= 0;
	lineWidth	= 1;
	lineNoise	= 15;
	DARK		= 0.7; //1.0;

	VFlag		= 0;
	hatchingType = 2; //0
	drawMode	= 3;
	patternOffset = 1; //0

	posterizeFilter = 0xD0;

	hsv			= cvCreateImage(size, IPL_DEPTH_8U, 3);  
	grayImage	= cvCreateImage(size, IPL_DEPTH_8U, 1);
	binaryImage	= cvCreateImage(size, IPL_DEPTH_8U, 1);
	imageA		= cvCreateImage(size, IPL_DEPTH_8U, 3);
	imageB		= cvCreateImage(size, IPL_DEPTH_8U, 3);

	CvPixelPosition8u posA;
	uchar* pA;
	uchar m;
	int x = 0, y = 0;

	imageC		= cvCreateImage(size, IPL_DEPTH_8U, 3);
	CV_INIT_PIXEL_POS(posA, (unsigned char*) imageC->imageData,
						imageC->widthStep, size, x, y, imageC->origin);

	imageD		= cvCreateImage(size, IPL_DEPTH_8U, 3);
	imageE		= cvCreateImage(size, IPL_DEPTH_8U, 3);

	for(y = 0; y < size.height; y++) {
		for(x = 0; x < size.width; x++) {
			pA = CV_MOVE_TO(posA, x, y, 3);
			pA[2] = 0x00; //R
			pA[1] = 0xFF; //G
			pA[0] = 0x00; //B
		}
	}


	scanningLine = cvCreateImage(size, IPL_DEPTH_8U, 3);
	CV_INIT_PIXEL_POS(posA, (unsigned char*) scanningLine->imageData,
						scanningLine->widthStep, size, x, y, scanningLine->origin);

	for(y = 0; y < size.height; y++) {
		m = y % 4 ? 255 : 200;
		for(x = 0; x < size.width; x++) {
			pA = CV_MOVE_TO(posA, x, y, 3);
			pA[0] = m;
			pA[1] = m;
			pA[2] = m;
		}
	}

	memStorage0	= cvCreateMemStorage(0);

	char buf[256];
	for(int i = 0; i < PATT_NUM; i++) {
		sprintf(buf, "Data/img/h%d.bmp", i);
		hatching[i] = cvLoadImage(buf);
	}
}


void CHandDrawEffect::EffectImage(IplImage* back, IplImage* frame, IplImage* alpha, IplImage* mask, IplImage* res)
{
	if(drawMode & 0x01) {
		//基本エフェクト
		Posterize(0xD0, frame, imageA);
	//	DrawHatching(frame, imageA);
		cvAnd(imageA, mask, imageB); //エフェクト処理後のCG部分のくりぬき

		//囲み
		cvNot(mask, imageA);
		cvDilate(imageA, imageD, 0, 1);
		cvDilate(mask, imageE, 0, 3);
		cvXor(imageE, imageD, mask);

		//アルファマスク更新
		cvNot(mask, imageA);
		cvConvertScale(imageA, imageA, 0.5);
		cvOr(alpha, imageA, alpha);

		//色付きの囲み
		cvNot(mask, imageA);
		cvAnd(imageA, imageC, imageA);
		cvOr(imageA, imageB, imageB);

		//走査線
		cvAnd(imageB, scanningLine, imageB);

		//アルファブレンド
		AlphaBlend(back, imageB, alpha, res);

		if(0) { //drawMode & 0x02) {
		//	DrawEdge(frame, imageB, res, 2);

			cvNot(mask, frame);
			cvDilate(frame, imageA, 0, 1);
			cvDilate(mask, imageB, 0, 3);
			cvXor(imageA, imageB, mask);
			cvAnd(mask, res, res);

			//色付きの線
			cvNot(mask, imageA);
			cvAnd(imageA, scanningLine, imageA);
			cvAnd(imageA, imageC, imageA);
			cvOr(res, imageA, res);
		}
	} else if(drawMode & 0x02) {
	//	DrawEdge(frame, imageB, res, 2);
	}
}


void AlphaBlend(IplImage* srcA, IplImage* srcB, IplImage* alpha, IplImage* dst)
{
	CvPixelPosition8u posA, posB, posC, posD;
	uchar* pA, *pB, *pC, *pD;
	CvSize size;
	int x, y;
	float a, b;

	size = cvGetSize(srcA);

	x = 0; y = 0;
	CV_INIT_PIXEL_POS(posA, (unsigned char*) srcA->imageData,
						srcA->widthStep, size, x, y, srcA->origin);
	CV_INIT_PIXEL_POS(posB, (unsigned char*) srcB->imageData,
						srcB->widthStep, size, x, y, srcB->origin);
	CV_INIT_PIXEL_POS(posC, (unsigned char*) alpha->imageData,
						alpha->widthStep, size, x, y, alpha->origin);
	CV_INIT_PIXEL_POS(posD, (unsigned char*) dst->imageData,
						dst->widthStep, size, x, y, dst->origin);

	for(y = 0; y < size.height; y++) {
		for(x = 0; x < size.width; x++) {
			pA = CV_MOVE_TO(posA, x, y, 3);
			pB = CV_MOVE_TO(posB, x, y, 3);
			pC = CV_MOVE_TO(posC, x, y, 3);
			pD = CV_MOVE_TO(posD, x, y, 3);

			b = pC[0] / 255.0f;
			a = 1.0f - b;
			pD[0] = (uchar) (pA[0] * a + pB[0] * b);
			pD[1] = (uchar) (pA[1] * a + pB[1] * b);
			pD[2] = (uchar) (pA[2] * a + pB[2] * b);
		}
	}
}


void Posterize(uchar posterizeFilter, IplImage* frame, IplImage* base)
{
	CvPixelPosition8u posA, posB;
	uchar* pA, *pB;
	CvSize size;
	int x, y;

	size = cvGetSize(frame);

	x = 0; y = 0;
	CV_INIT_PIXEL_POS(posA, (unsigned char*) frame->imageData,
						frame->widthStep, size, x, y, frame->origin);
	CV_INIT_PIXEL_POS(posB, (unsigned char*) base->imageData,
						base->widthStep, size, x, y, base->origin);

	for(y = 0; y < size.height; y++) {
		for(x = 0; x < size.width; x++) {
			pA = CV_MOVE_TO(posA, x, y, 3);
			pB = CV_MOVE_TO(posB, x, y, 3);

			pB[0] = pA[0] & posterizeFilter;
			pB[1] = pA[1] & posterizeFilter;
			pB[2] = pA[2] & posterizeFilter;
		}
	}
}


void CHandDrawEffect::DrawEdge(IplImage* image, IplImage* image2, IplImage* base, int plane)
{
	CvSeq* contourSeq0 = NULL;

	int height    = image->height;
	int width     = image->width;
	int step      = image->widthStep;
	int channels  = image->nChannels;
	uchar* data   = (uchar*)image->imageData;

	if(plane < 3) {
		cvCvtColor(image, hsv, CV_BGR2HSV);				// HSVのplaneを線画生成の元にする
		for(int i = 0; i < height * width; i++)
			grayImage->imageData[i] = hsv->imageData[i * 3 + plane];
	} else {
		cvCvtColor(image, grayImage, CV_BGR2GRAY);		// グレーイメージを作り線画生成の元にする
	}

	IplImage* target = base;					// 書き込むターゲットイメージ

	for(int x = 20; x < 240; x += Y) {
		cvThreshold(grayImage, binaryImage, x, 255, CV_THRESH_BINARY);	// x の値を境に２値化し輪郭を抽出
		contourSeq0 = 0;
		cvFindContours(binaryImage, memStorage0, &contourSeq0, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0, 0)); // 輪郭線探索

		if(lineNoise > 0) { // 不連続ラインの場合
			for(; contourSeq0 != 0; contourSeq0 = contourSeq0->h_next) {
				CvPoint *p;
				if(contourSeq0->total< X * 5) continue;		// 5角形以下の細かいのは排除

				int index = 0;
				for(int i = 0; i < contourSeq0->total; i += X) {
					p = CV_GET_SEQ_ELEM(CvPoint, contourSeq0, i);				// 点の場所と色を登録
					CvScalar color = GetColor(image2, p->x, p->y);
					MulScaler(color, DARK);										// 輝度を修正
					color.val[3] = CheckPoint(image, p->x, p->y, lineNoise);	// 有効点かどうかを近接ピクセルから判断して[3]へ格納
					SetPoint(index, p, color);									// pointTableへ保存
					index++;
					if(index > MAX_POINT) {
					//	printf("INDEX ERROR\n"); 
						index = 0;
					}
				}
				// ５連続以下の有効点は無効 （Pending:高速化）
				for(int i = 0; i < index; i++) {
					int p1 = i;
					int p2, p3, p4, p0;
					if(pointTable[p1].color.val[3]) {
						p2 = (p1 + 1) % index;
						p3 = (p1 + 2) % index;
						p4 = (p1 + 3) % index;
						p0 = (p1 - 1 + index) % index;
						if(pointTable[p0].color.val[3]) continue;
						if(!pointTable[p2].color.val[3] ||
							!pointTable[p3].color.val[3] ||
							!pointTable[p4].color.val[3]) {						
							pointTable[p1].color.val[3] = 0;
						}
					}
				}
				// 接続された有効点を描く
				for(int i = 0; i < index; i++) {
					int p1 = i;
					int p2 = (i + 1) % index;	// if (p2==index) p2 = 0;
					if(pointTable[p1].color.val[3] && pointTable[p2].color.val[3]) {
						CvScalar c = pointTable[p1].color;
						MulScaler(c, DARK);
						cvLine(target, pointTable[p1].p, pointTable[p2].p, c, lineWidth, CV_AA);
					}
				}
			}
		} else {
			// 全部描く場合
			for(; contourSeq0 != 0; contourSeq0 = contourSeq0->h_next) {

				CvPoint *p1 = 0;
				CvPoint *p2;

				if(contourSeq0->total < X * 5) continue;		

				for(int i = 0; i < contourSeq0->total; i += X) {
					p1 = CV_GET_SEQ_ELEM(CvPoint, contourSeq0, (i) % contourSeq0->total);//始点
					p2 = CV_GET_SEQ_ELEM(CvPoint, contourSeq0, (i + X + Z) % contourSeq0->total);// 終点
					CvScalar color = GetColor(image, p1->x, p1->y);
					MulScaler(color, DARK);
					cvLine(target, *p1, *p2, color, lineWidth, CV_AA);
				}
			}
		}
	}
	cvClearMemStorage(memStorage0);
}


int CHandDrawEffect::DrawHatching(IplImage* frame, IplImage* base)
{
//	IplImage* hsv = cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);  
	cvCvtColor(frame, hsv, CV_BGR2HSV);

	int hwidth = hatching[0]->width;
	int hheight = hatching[0]->height;

	int rx = rand() % hwidth;// texture offset
	int ry = rand() % hheight;

	float d = (float)(rand() % 1000) / 1000.0f - 1.5f;//傾き
	//d = 0;

	int height    = frame->height;
	int width     = frame->width;


	for(int y = 0; y < height; y++) { 
		for(int x = 0; x < width; x++) {
			int index = x * frame->nChannels + y * frame->widthStep;
			unsigned char v = hsv->imageData[index + 2];

			if(VFlag) {
				float V = v / 256.0f;
				V = sqrt(V) * 256.0f;
				v = (int)V;
			}

			int X = x + rx;
			int Y = y + ry + (int) (x * d);
			while(Y<0) Y += hheight * 100;
			X %= hwidth;
			Y %= hheight;
			float typeV;

			int maxV = PATT_NUM - 1 + patternOffset;
			
			typeV = (float) (255 - v) / (float) (256 / maxV) - patternOffset;
			typeV = maxV - typeV ;

			if(typeV < 0) typeV = 0;
			if(typeV > PATT_NUM-1) typeV = PATT_NUM - 1;

			float vf = 0.0;
			int a = (int) typeV;
			int b = a + 1;
			float w = typeV - a; // weight

			if(hatchingType >= 1) { // Inverse
				a = PATT_NUM-1 - a;
				b = PATT_NUM-1 - b;
			}
			vf += ((float) (unsigned char) hatching[a]->imageData[X*hatching[a]->nChannels + Y * hatching[a]->widthStep]) * (1.0f - w);
			if(b >= 0 && b <= 6){
				vf += ((float) (unsigned char) hatching[b]->imageData[X*hatching[b]->nChannels + Y * hatching[b]->widthStep]) * w;

			}

			if(hatchingType < 2)
				vf = 255 - vf;
			unsigned char vv = hsv->imageData[index + 2];
			if(hatchingType == 2){
				//int V = vv;V*=2;if (V>255)V=255;vv = V;// ベースを明るくしてみたり
				vv = (unsigned char) Clip(vv * 2.0f, 0.0f, 255.0f);
				vf = vf * 0.7f + 255.0f * 0.3f; // 線も黒すぎるのでちょっと明るく
			}
			float sf = 1.0f;
			if(hatchingType == 3) { // 白黒
				vv = 255;
				sf = 0.0;
				//vf=vf*0.7+255.0*0.3; // 線も黒すぎるのでちょっと明るく
			}
			

			base->imageData[index + 0] = hsv->imageData[index + 0];
			base->imageData[index + 1] = (char) (hsv->imageData[index + 1] * sf);
			base->imageData[index + 2] = (unsigned char) (vv * (float) vf / 255);

		}
	}
	cvCvtColor(base, base, CV_HSV2BGR);

	return(1);
}


int CHandDrawEffect::SetPoint(int index, CvPoint* point, CvScalar& color)
{
	if(index >= MAX_POINT) return(0);

	pointTable[index].p = *point;
	pointTable[index].color.val[0] = color.val[0];
	pointTable[index].color.val[1] = color.val[1];
	pointTable[index].color.val[2] = color.val[2];
	pointTable[index].color.val[3] = color.val[3];

	return(1);		
}






double ColorLen(CvScalar& x, CvScalar& y)
{
	double sum = 0;

	for(int i = 0; i < 3; i++) {
		sum += abs(x.val[i] - y.val[i]);
	}

	return(sum);
}


// imageの座標位置カラー
CvScalar GetColor(IplImage* image, int x, int y)
{
	int index = y * image->widthStep + x * image->nChannels;
	unsigned char* data =  (unsigned char*)image->imageData;

	return(CV_RGB(data[index + 2], data[index + 1], data[index + 0]));
}


// 座標がイメージ外にある可能性のある場合用
CvScalar GetColorCheck(IplImage* image, int x, int y)
{
	if(x < 0) x = 0;
	if(y < 0) y = 0;
	if(x >= image->width) x = image->width - 1;
	if(y >= image->height) y = image->height - 1;

	return(GetColor(image, x, y));
}


void DivScaler(CvScalar& s, double d)
{
	s.val[0] /= d;
	s.val[1] /= d;
	s.val[2] /= d;
	s.val[3] /= d;
}


void MulScaler(CvScalar& s, double d)
{
	s.val[0] *= d;
	s.val[1] *= d;
	s.val[2] *= d;
	s.val[3] *= d;
}


//近接ピクセルとの色の差の最大値と閾値を比較
int CheckPoint(IplImage* image, int x, int y, double th)
{
	struct { int x, y; } map[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

	CvScalar color = GetColor(image, x, y);

	for(int i = 0; i < 4; i++) {
		if(ColorLen(color, GetColorCheck(image, x + map[i].x, y + map[i].y)) > th) return(1);
	}

	return(0);
}


float Clip(float x, float min, float max)
{
	if(x > max) return(max);
	if(x < min) return(min);

	return(x);
}

