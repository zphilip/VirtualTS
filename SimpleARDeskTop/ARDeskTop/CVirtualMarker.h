#ifndef ___CVirtualMarker
#define ___CVirtualMarker

#include "CProjection.h"



class CVirtualMarker : public CProjection
{
public :
public :
						CVirtualMarker(void);
	virtual				~CVirtualMarker();

	virtual	void		Init(void);
	virtual	void		Precompute(void);

	virtual	void		DrawShape(void);
	virtual	double		GetTransMat(ARMarkerInfo* info);
};


#endif
