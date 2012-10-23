#ifndef ___CPictureRing
#define ___CPictureRing

#include "CFrame.h"

class CPictureViewer;


class CPictureRing : public CFrame
{
public :
			CPictureViewer*
						viewer;
			double		pictureSize;
			double		pictureDepth;
			double		xMargin;
			double		yMargin;
public :
						CPictureRing(void);
	virtual				~CPictureRing();

	virtual	void		Init(void);
	virtual	void		InterruptClick(CMainCursor* cursor, int mode);
	virtual	void		InterruptRotate(CMainCursor* cursor);

			void		SetPictureSize(double pPictureSize);
			void		SetPictureDepth(double pPictureDepth);
			void		SetViewer(CPictureViewer* pViewer);
			void		AddTestPicture(wchar_t* dirName, int fileNum, double k = 2.0);

	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);

	virtual	void		ItemDown(CComponent3D* item);
};


#endif
