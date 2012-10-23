#ifndef ___CHeadTrackingMarker
#define ___CHeadTrackingMarker


#include "CMarkerObject.h"

#define MAX_SMOOTH_NUM	20



class CHeadTrackingMarker : public CMarkerObject
{
public :
	static	CHeadTrackingMarker*
						HTMarker;

	static	int			HTMode;
	static	int			smoothNum;
public :
			double		smoothMat[MAX_SMOOTH_NUM][3][4];
			double		smoothMatSum[3][4];
			double		smoothMatAve[3][4];
			int			smoothCursor;
			bool		startSmoothing;
public :
						CHeadTrackingMarker(void);
	virtual				~CHeadTrackingMarker();

	virtual	void		Init(void);
	virtual	void		Precompute(void);
};


#endif
