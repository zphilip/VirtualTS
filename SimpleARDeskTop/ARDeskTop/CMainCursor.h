#ifndef ___CMainCursor
#define ___CMainCursor

#include "CCursor.h"
#include "CClickInfo.h"

class CHand;



class CMainCursor : public CCursor
{
public :
			int			selectObjectIndex;
			CComponent3D*
						selectObject;
			CComponent3D*
						dropObject;
			CComponent3D*
						prevDropObject;

			CClickInfo	firstClickInfo;
			CClickInfo	currentClickInfo;
			double		angle;
			double		normal[3];
			double		missileVec[3];		//ÉQÅ[ÉÄóp
			double		missileStartPos[3];

			CClickInfo3	ci1;
			CClickInfo3	ci2;

			double		multMatrix[16];
public :
						CMainCursor(void);
	virtual				~CMainCursor();

	virtual	void		Init(void);
//	virtual	void		Precompute(void);
	virtual	int			Click(void);
			int			MultiClick(CHand*, int);

	virtual	void		Precompute(int id);
	virtual	CComponent3D*
						GetDropObject(void);

			void		RotateA(CComponent3D* object, bool fix = false);
			void		RotateB(CComponent3D* object, bool fix = false);
			void		Scale(CComponent3D* object, bool fix = false);
			void		Multi(CComponent3D* object, bool fix = false);
			void		Twist(CComponent3D* object, bool fix = false);

			void		CompTwistNormal(void);

			int			FindSelectObject(void);
};


#endif
