#ifndef ___CPictureViewer
#define ___CPictureViewer

#include "CFrame.h"

class CCVPicture;
class CButton3D;
class CImage;


class CPictureViewer : public CFrame
{
public :
			CCVPicture*
						picture;

			CButton3D*	prevPicture;
			CButton3D*	nextPicture;
			CButton3D*	rotatePicture;
			int			rotate;
public :
						CPictureViewer(void);
	virtual				~CPictureViewer();

	virtual	void		Init(void);

	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);
	virtual	void		ItemDown(CComponent3D* item);

			void		OpenPicture(wchar_t* fileName);
			void		FitPicture(void);
};


#endif
