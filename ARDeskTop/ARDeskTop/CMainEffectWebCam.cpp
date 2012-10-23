#include <cv.h>
#include <highgui.h>
#include <stdlib.h>

#include "CMainEffectWebCam.h"


CMainEffectWebCam::CMainEffectWebCam(void)
: CMainWebCam()
{
	cvEffect	= 0;

	frame		= 0;
	frame2		= 0;
	temp		= 0;
	temp2		= 0;
	res			= 0;
	mask		= 0;
	alpha		= 0;

	cvNamedWindow("test", CV_WINDOW_AUTOSIZE);
}


CMainEffectWebCam::~CMainEffectWebCam()
{
	if(cvEffect) delete cvEffect;
	cvDestroyWindow("test");

	if(frame)	cvReleaseImage(&frame);
	if(frame2)	cvReleaseImage(&frame2);
	if(res)		cvReleaseImage(&res);
	if(mask)	cvReleaseImage(&mask);
	if(alpha)	cvReleaseImage(&alpha);
	if(temp)	cvReleaseImage(&temp);
	if(temp2)	cvReleaseImage(&temp2);
}


int CMainEffectWebCam::SetupWebCam(const char* cparam_names, char* vconfs)
{
	if(!CMainWebCam::SetupWebCam(cparam_names, vconfs)) return(0);


	CvSize size;

	size.width	= ARTCparam.xsize;
	size.height	= ARTCparam.ysize;

//	cvEffect = new CHandDrawEffect();
	cvEffect = new CHSVToonEffect();
	cvEffect->Init(size);

	frame		= cvCreateImage(size, IPL_DEPTH_8U, 3);
	frame2		= cvCreateImage(size, IPL_DEPTH_8U, 3);
	temp		= cvCreateImage(size, IPL_DEPTH_8U, 3);
	temp2		= cvCreateImage(size, IPL_DEPTH_8U, 1);
	res			= cvCreateImage(size, IPL_DEPTH_8U, 3);
	mask		= cvCreateImage(size, IPL_DEPTH_8U, 3);
	alpha		= cvCreateImage(size, IPL_DEPTH_8U, 3);

	return(1);
}


void CMainEffectWebCam::Display(ARGL_CONTEXT_SETTINGS_REF arglSettings)
{
	if(!ARTImage) return;

	CvSize size;
	CvPixelPosition8u posA, posB, posC, posD, posE;
	uchar* pA, *pB, *pC, *pD, *pE;
	uchar m;
	int x, y, z;

	size.width	= ARTCparam.xsize;
	size.height	= ARTCparam.ysize;

	x = 0; y = 0;
	CV_INIT_PIXEL_POS(posA, (unsigned char*) frame->imageData,
						frame->widthStep, size, x, y, frame->origin);

	//ARunit8‚ðIplImage‚É•ÏŠ·
	for(y = 0; y < size.height; y++) {
		for (x = 0; x < size.width; x++) {
			z = (y * size.width + x) * 4;
			pA = CV_MOVE_TO(posA, x, y, 3);

			pA[0] = ARTImage[z + 0];
			pA[1] = ARTImage[z + 1];
			pA[2] = ARTImage[z + 2];
		}
	}


	if(pattFound) {
		double p[16];

		arglCameraFrustumRH(&ARTCparam, VIEW_DISTANCE_MIN, VIEW_DISTANCE_MAX, p);
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixd(p);

		this->Draw();

		glReadPixels(0, 0, size.width, size.height, GL_RGB, GL_UNSIGNED_BYTE, temp->imageData);
		glReadPixels(0, 0, size.width, size.height, GL_ALPHA, GL_UNSIGNED_BYTE, temp2->imageData);

		x = 0; y = 0;
		CV_INIT_PIXEL_POS(posA, (unsigned char*) temp->imageData,
							temp->widthStep, size, x, y, temp->origin);
		CV_INIT_PIXEL_POS(posB, (unsigned char*) frame2->imageData,
							frame2->widthStep, size, x, y, frame2->origin);
		CV_INIT_PIXEL_POS(posC, (unsigned char*) alpha->imageData,
							alpha->widthStep, size, x, y, alpha->origin);
		CV_INIT_PIXEL_POS(posD, (unsigned char*) mask->imageData,
							mask->widthStep, size, x, y, mask->origin);
		CV_INIT_PIXEL_POS(posE, (unsigned char*) temp2->imageData,
							temp2->widthStep, size, x, y, temp2->origin);

		for(y = 0; y < size.height; y++) {
			for(x = 0; x < size.width; x++) {
				//frame2
				pA = CV_MOVE_TO(posA, x, y, 3); //BGRA
				pB = CV_MOVE_TO(posB, x, y, 3);
				pB[0] = pA[2];
				pB[1] = pA[1];
				pB[2] = pA[0];

				//alpha
				pC = CV_MOVE_TO(posC, x, y, 3);
				pE = CV_MOVE_TO(posE, x, y, 1);
				pC[0] = pE[0];
				pC[1] = pE[0];
				pC[2] = pE[0];

				//mask
				m = pE[0] == 0.0f ? 0 : 255;
				pD = CV_MOVE_TO(posD, x, y, 3);
				pD[0] = m;
				pD[1] = m;
				pD[2] = m;
			}
		}

		if(temp->origin == 0) {
			cvFlip(frame2, NULL, 0);
			cvFlip(mask, NULL, 0);
			cvFlip(alpha, NULL, 0);
		}

		cvEffect->EffectImage(frame, frame2, alpha, mask, res);
		cvShowImage("test", res);
	} else {
		cvShowImage("test", frame);
	}

	ar2VideoCapNext(ARTVideo);
}


void CMainEffectWebCam::Draw(void)
{
	CMainWebCam::Draw();
}

