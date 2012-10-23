#ifndef ___CProjection
#define ___CProjection

#include "CMarkerObject.h"
#include "CComponent3D.h"

class CCursor;


class CProjection : public CMarkerObject
{
public :
			double		cursorPos[3];
			double		invMat[3][4];

			double		HTMarkerInvMat[3][4];
public :
						CProjection(void);
	virtual				~CProjection();

	virtual	void		Init(void);
	virtual	void		Precompute(void);

			CComponent3D*
						IsWithin(double x, double y, double z);
			void		ProjectCursorPos(CCursor* cursor, double pos[3]);
			void		ProjectPos(double pos[3], double pCursorPos[3]);
			void		FlipFix(void);
};


#endif
