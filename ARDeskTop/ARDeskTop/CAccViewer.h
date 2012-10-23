#ifndef ___CAccPictureViewer
#define ___CAccPictureViewer

#include "CAccessory.h"

class CCVVideoFrame;


class CAccViewer : public CAccessory
{
public :
	static	CAccViewer*	accViewer;
			CCVVideoFrame*
						viewer;
public :
						CAccViewer(void);
	virtual				~CAccViewer();

	virtual	void		Init(void);
	virtual	void		Precompute(void);
};


#endif
