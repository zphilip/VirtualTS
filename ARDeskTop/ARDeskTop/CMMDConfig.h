#ifndef ___CMMDConfig
#define ___CMMDConfig

#include "CFrame.h"
#include "CSlider.h"

class CButton3D;



class CBoundSlider : public CSlider
{
public :
						CBoundSlider(void);
	virtual				~CBoundSlider();

	virtual	void		ItemClickUp(CComponent3D* item);
};



class CMMDConfig : public CFrame
{
public :
			CButton3D*	drawDot;
			CButton3D*	drawWireMaster;
			CButton3D*	drawWireWithDepth;
			CBoundSlider*
						boundSize;
public :
						CMMDConfig(void);
	virtual				~CMMDConfig();

	virtual	void		Init(void);
	virtual	void		Precompute(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);
	virtual	void		ItemDown(CComponent3D* item);
};


#endif
