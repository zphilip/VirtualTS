#ifndef ___CCVVideoFrame
#define ___CCVVideoFrame

#include "CCVPictureFrame.h"
#include "CButton3D.h"
#include "CSlider.h"




class CVolumeFader : public CSlider
{
public :
			double		oriPos;
public :
						CVolumeFader(void);
	virtual				~CVolumeFader();

	virtual	void		Init(void);
	virtual	void		ItemDown(CComponent3D* item);
	virtual	void		ItemUp(CComponent3D* item);
	virtual	void		ItemClickDown(CComponent3D* item);
	virtual	void		ItemClickUp(CComponent3D* item);
};


class CCVVideoFrame : public CCVPictureFrame
{
public :
			CButton3D*	play;
			CButton3D*	stop;
			CButton3D*	pause;
			CButton3D*	blueBack;
			CVolumeFader*
						volume;
public :
						CCVVideoFrame(void);
	virtual				~CCVVideoFrame();

	virtual	void		Init(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);
	virtual	void		Precompute(void);
	virtual	void		ItemDown(CComponent3D* item);
	virtual	void		Reset(void);
};


#endif
