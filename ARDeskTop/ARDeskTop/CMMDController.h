#ifndef ___CMMDController
#define ___CMMDController

#include "CFrame.h"
#include "CButton3D.h"

class CMMDObject;
class CLabel3D;



class CJogWheel : public CButton3D
{
public :
			double		pos[3];
			int			prevIndex, index;
			int*		wheel;
			int			divNum;
public :
						CJogWheel(void);
	virtual				~CJogWheel();

	virtual	void		Init(void);
	virtual	CComponent3D*
						IsWithin(double x, double y, double z);
	virtual	void		CursorEnter(CCursor* cursor);
	virtual	void		CursorWithin(CCursor* cursor);

			int			CheckWheelArea(void);
			void		UpDataWheel(int id);
};


class CMMDController : public CFrame
{
public :
			CMMDObject*	mmdObject;

			CJogWheel*	jogWheel;
			CLabel3D*	counter;

			CButton3D*	play;
			CButton3D*	stop;
//			CButton3D*	pause;
public :
						CMMDController(void);
	virtual				~CMMDController();

	virtual	void		Init(void);
	virtual	void		Precompute(void);
	virtual	void		SetSize(double pWidth, double pHeight, double pDepth);

	virtual	void		Reset(void);
	virtual	void		ItemDown(CComponent3D* item);

	virtual	void		Play(void);
	virtual	void		Replay(void);
	virtual	void		Stop(void);
	virtual	void		Pause(void);
	virtual	void		Skip(double skitTimeMS);
	virtual	void		Animate(void);
public :
	static	CMMDController*
						mmdController;
};


#endif
