#include <locale.h>
#include <stdlib.h>
#include <stdio.h>

#include "CCVPicture.h"
#include "CCVVideoManager.h"
#include "CGroup.h"



CCVPicture::CCVPicture(void)
: CPrimitive()
{
	width2		= 0;
	height2		= 0;

	glGenTextures(1, &texName[0]);

	bluebackMode	= false;
	hitBlue			= false;
}


CCVPicture::~CCVPicture()
{
	if(texName[0]) glDeleteTextures(1, &texName[0]);
}


void CCVPicture::DrawShape(void)
{
	if(!texName[0]) return;

	if(bluebackMode && hitBlue) {
		glAlphaFunc(GL_GREATER, 0.5);
		glEnable(GL_ALPHA_TEST);
	}
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texName[0]);

	glColor4fv(color);
    glNormal3d(0.0, 0.0, 1.0);

	glBegin(GL_QUADS);
		glTexCoord2d(0.0, 1.0);
		glVertex3d(-width2 * 0.5,  height2 * 0.5,  0.0);

		glTexCoord2d(1.0, 1.0);
		glVertex3d( width2 * 0.5,  height2 * 0.5,  0.0);

		glTexCoord2d(1.0, 0.0);
		glVertex3d( width2 * 0.5, -height2 * 0.5,  0.0);

		glTexCoord2d(0.0, 0.0);
		glVertex3d(-width2 * 0.5, -height2 * 0.5,  0.0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	if(bluebackMode && hitBlue)
		glDisable(GL_ALPHA_TEST);
}


bool CCVPicture::OpenFile(wchar_t* pFileName, double size)
{
	IplImage* image;
	char cFileName[256];
	int comp;

	setlocale(LC_ALL, "ja");
	wcstombs(cFileName, pFileName, 256);

	image = cvLoadImage(cFileName, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR); //CV_LOAD_IMAGE_COLOR);
	if(!image) return(false);

	comp = image->width / size;

	if(size > 0.0 && comp) {
		IplImage* dst = cvCreateImage(cvSize(image->width / comp, image->height / comp), IPL_DEPTH_8U, 3);
		cvResize(image, dst, CV_INTER_CUBIC);
		SetTexture(dst);
		cvReleaseImage(&dst);
		cvReleaseImage(&image);
	} else {
		SetTexture(image);
		cvReleaseImage(&image);
	}

	return(true);
}


bool CCVPicture::OpenVideoFile(wchar_t* pFileName, double size)
{
	wcscpy(fileName, pFileName);

	bool ret;

	CCVVideoManager* mana = new CCVVideoManager(this);
	SetAnimeManager(mana);
	mana->Init();
	ret = mana->OpenVideoFile(fileName, size);

	return(true);
}


IplImage* CCVPicture::ConvBlueback(IplImage* image)
{
	IplImage* imageA = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 4);

	CvPixelPosition8u pos_src, pos_dst;
	uchar* p_src;
	uchar* p_dst;
	int x = 0, y = 0;

	if(image->origin == 0) cvFlip(image, NULL, 0);

	CV_INIT_PIXEL_POS(pos_src, (unsigned char*) image->imageData,
					   image->widthStep, cvGetSize(image), x, y, image->origin);

	CV_INIT_PIXEL_POS(pos_dst, (unsigned char*) imageA->imageData,
					   imageA->widthStep, cvGetSize(imageA), x, y, imageA->origin);

	hitBlue = false;
	for(y = 0; y < image->height; y++) {
		for(x = 0; x < image->width; x++) {
			p_src = CV_MOVE_TO(pos_src, x, y, 3); //BGR
			p_dst = CV_MOVE_TO(pos_dst, x, y, 4); //RGBA

			p_dst[0] = p_src[2];
			p_dst[1] = p_src[1];
			p_dst[2] = p_src[0];

	//		if(p_dst[0] == 0 && p_dst[1] == 0 && p_dst[2] == 255) {
	//		if(p_dst[0] < 30 && p_dst[1] < 30 && p_dst[2] > 225) {
			if(p_dst[0] < 40 && p_dst[1] < 40 && p_dst[2] > 215) {
				p_dst[3] = 0;
				hitBlue = true;
			} else {
				p_dst[3] = 255;
			}
		}
	}

	return(imageA);
}


void CCVPicture::SetTexture(IplImage* image)
{
	if(bluebackMode) {
		IplImage* imageA = ConvBlueback(image);
		SetTexture2(imageA);
		cvReleaseImage(&imageA);
	} else {
		cvCvtColor(image, image, CV_BGR2RGB);
		if(image->origin == 0) cvFlip(image, NULL, 0);
		SetTexture2(image);
	}
}


void CCVPicture::SetTexture2(IplImage* image)
{
	glBindTexture(GL_TEXTURE_2D, texName[0]);

//	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	bool unfit = (width == 0.0);

	width2	= image->width;
	height2	= image->height;

	if(unfit) FitToFrame();

	if(bluebackMode) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width2, height2, GL_RGBA, GL_UNSIGNED_BYTE, image->imageData);
	} else {
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width2, height2, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);
	}
}


void CCVPicture::FitToFrame(void)
{
	double widthRate, heightRate, minRate;

	widthRate	= parent->width / width2;
	heightRate	= parent->height / height2;

	minRate		= widthRate < heightRate ? widthRate : heightRate;

	SetSize(minRate, minRate, 1.0);
	if(card) {
		SetPosition(0.0, 0.0, 0.0);
	} else {
		SetPosition(0.0, 0.0, depth * 0.5);
	}
}
