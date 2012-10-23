#include <stdio.h>
#include <stdlib.h>

#ifndef __APPLE__
#  include <GL/glew.h>
#  include <GL/wglew.h>
#  include <GL/glut.h>
#else
#  include <GLUT/glut.h>
#endif

#include "CARDTWindow.h"
#include "CARDeskTop.h"
#include "CWebCam.h"
#include "CMainWebCam.h"
#include "CMainEffectWebCam.h"
#include "CHTWebCam.h"
#include "CHeadTrackingMarker.h"
#include "CARDeskTop.h"
#include "CCursor.h"
#include "CHand.h"




CARDTWindow::CARDTWindow(void)
: CWindow()
{
}


CARDTWindow::~CARDTWindow()
{
}


int CARDTWindow::Init(void)
{
	webCamList		= new CWebCam*[webCamNum];
	webCamList[0]	= new CMainWebCam();
//	webCamList[0]	= new CMainEffectWebCam();

	if(webCamNum == 2) {
		webCamList[1]	= new CHTWebCam();
		((CHTWebCam*) webCamList[1])->SetTargetMarker(CHeadTrackingMarker::HTMarker);
	}

	scale = 1.0;

	const char* cparamNames[] = {
		"Data/camera_para.dat",
		"Data/camera_para.dat",
	};

	char* vconfs[] = {
		"Data/WDM_camera_flipV_01.xml", //HMD�p
		"Data/WDM_camera_flipV_02.xml", //HT�p
	};

	for(int i = 0; i < webCamNum; i++) {
		if(!webCamList[i]->SetupWebCam(cparamNames[i], vconfs[i])) return(0);
	}

	width		= webCamList[0]->ARTCparam.xsize;
	height		= webCamList[0]->ARTCparam.ysize;
	aspectRatio	= (double) width / (double) height;
	Reshape(width, height);

	SetupWindow();
	FlipWipe();

	return(1);
}




//http://www.arakin.dyndns.org/ ���񂩂炨�؂肵�܂����B
/*
 * �V�F�[�_�[�v���O���������[�h���A�R���p�C��
 */
static int loadShader(
	GLuint shader, 
	char *name
)
{
	errno_t err;
	FILE *fp;
	void *buf;
	int size;
	GLint compiled;

	if ((err = fopen_s(&fp, name, "rb")) != 0)
	{
		fprintf(stderr, "%s is not found!!\n", name);
		return -1;
	}
  
	/* �t�@�C���T�C�Y�̎擾 */
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);

	/* �V�F�[�_�v���O�����̓ǂݍ��ݗ̈���m�� */
	if ((buf = (void *)malloc(size)) == NULL)
	{
		fprintf(stderr, "Memory is not enough for %s\n", name);
		fclose(fp);
		return -1;
	}
  
	/* �t�@�C����擪����ǂݍ��� */
	fseek(fp, 0, SEEK_SET);
	fread(buf, 1, size, fp);

	/* �V�F�[�_�I�u�W�F�N�g�Ƀv���O�������Z�b�g */
	glShaderSource(shader, 1, (const GLchar **)&buf, &size);
  
	/* �V�F�[�_�ǂݍ��ݗ̈�̉�� */
	free(buf);
	fclose(fp);

	/* �V�F�[�_�̃R���p�C�� */
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
//	printShaderInfoLog(shader);		/* �R���p�C�����O�̏o�� */
	if (compiled == GL_FALSE)
	{
		fprintf(stderr, "Compile error in %s!!\n", name);
		return -1;
	}

	return 0;
}



/*
 * �V�F�[�_�v���O�����̃��[�h
 */
int FUTL_LoadShader(
	char *vtxShdName,
	char *frgShdName,
	GLuint *lpProg
)
{
	GLuint vtxShader;
	GLuint frgShader;
	GLuint prog;
	GLint linked;

	/* �V�F�[�_�I�u�W�F�N�g�̍쐬 */
	vtxShader = glCreateShader(GL_VERTEX_SHADER);
	frgShader = glCreateShader(GL_FRAGMENT_SHADER);

	/* �o�[�e�b�N�X�V�F�[�_�̃��[�h�ƃR���p�C�� */
	if (loadShader(vtxShader, vtxShdName) < 0)
	{
		return -1;
	}

	/* �t���O�����g�V�F�[�_�̃��[�h�ƃR���p�C�� */
	if (loadShader(frgShader, frgShdName) < 0)
	{
		return -1;
	}

	/* �v���O�����I�u�W�F�N�g�̍쐬 */
	prog = glCreateProgram();

	/* �V�F�[�_�I�u�W�F�N�g�̃V�F�[�_�v���O�����ւ̓o�^ */
	glAttachShader(prog, vtxShader);
	glAttachShader(prog, frgShader);

	/* �V�F�[�_�I�u�W�F�N�g�̍폜 */
	glDeleteShader(vtxShader);
	glDeleteShader(frgShader);

	/* �V�F�[�_�v���O�����̃����N */
	glLinkProgram(prog);
	glGetProgramiv(prog, GL_LINK_STATUS, &linked);
//	printProgramInfoLog(prog);
	if (linked == GL_FALSE)
	{
		fprintf(stderr, "Link error of %s & %s!!\n", vtxShdName, frgShdName);
		return -1;
	}

	*lpProg = prog;

	return 0;
}




void CARDTWindow::Display(void)
{
//	glDrawBuffer(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	if(showWipe == 0 || webCamNum == 1) {
		glMatrixMode(GL_MODELVIEW);
		glViewport(vpX, vpY, vpWidth, vpHeight);
		webCamList[0]->Display(arglSettings);
		webCamList[0]->SetZeroImage();
	} else {
		glMatrixMode(GL_MODELVIEW);
		glViewport(vpX, vpY, vpWidth, vpHeight);
		webCamList[0]->Display(arglSettings);

		glMatrixMode(GL_MODELVIEW);
		glViewport(vpX, vpY, vpWidth / wipeRatio, vpHeight / wipeRatio);
		webCamList[1]->Display(arglSettings);

		webCamList[0]->SetZeroImage();
		webCamList[1]->SetZeroImage();
	}
}


void CARDTWindow::Reshape(int currentWidth, int currentHeight)
{
	double currentAspectRatio = (double) currentWidth / (double) currentHeight;

	if(currentAspectRatio == aspectRatio) {
		vpWidth		= currentWidth;
		vpHeight	= currentHeight; 
		vpX			= 0;
		vpY			= 0;
	} else if(currentAspectRatio > aspectRatio) { //����
		vpWidth		= currentHeight * aspectRatio;
		vpHeight	= currentHeight; 
		vpX			= (currentWidth - vpWidth) / 2;
		vpY			= 0;
	} else { //�c��
		vpWidth		= currentWidth;
		vpHeight	= currentWidth / aspectRatio; 
		vpX			= 0;
		vpY			= currentHeight - vpHeight;
	}
}


void CARDTWindow::Keyboard(unsigned char key)
{
	if(key == 0x1B) {
		Quit();
	} else if(key == 'w') {
		FlipWipe();
	} else if(key == 'g') {
		if(fullWindow) {
			glutReshapeWindow(640, 480);
			glutPositionWindow(10, 20);
		} else {
			glutFullScreen();
		}

		fullWindow = !fullWindow;
	} else if(key == 'f') {
		CCursor::FixAll();
	} else if(key == ' ') { //handA���N���b�N
		if(CARDeskTop::arDeskTop->user->handA)
			CARDeskTop::arDeskTop->user->handA->keyClick = 1;
	} else {
	}
}


void CARDTWindow::FlipWipe(void)
{
	showWipe = (showWipe + 1) % 3;
	if(webCamNum == 2) {
		if(showWipe == 2) {
			webCamList[1]->dispImage = false;
		} else {
			webCamList[1]->dispImage = true;
		}
	}
}
