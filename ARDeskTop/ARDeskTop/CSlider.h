#ifndef ___CSlider
#define ___CSlider

#include "CLabeledCube.h"



class CSliderControl : public CLabeledCube
{
public :
			int			axis;
			double		minPos, maxPos;
public :
						CSliderControl(void);
	virtual				~CSliderControl();

	virtual	void		Init(void);
	virtual	void		InterruptTranslate(double* pos, double* objectPos);

			double		GetValue(void);
};


class CSlider : public CLabeledCube
{
public :
			int			axis;
			double		min, max, value, margin, ratio;
			CSliderControl*
						sliderControl;
public :
						CSlider(void);
	virtual				~CSlider();

	virtual	void		Init(void);
	virtual	void		Precompute(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);
	virtual	void		ItemClickUp(CComponent3D* item);
};


#endif
