#ifndef ___CMarkerObject
#define ___CMarkerObject

#include <AR/ar.h>
#include "CObject.h"
#include "CList.h"

class CComponent3D;


class CMarkerObject : public CObject
{
public :
			double		markerCoord[4][2];
			double		trans[3][4];
			double		markerWidth;
			double		markerCenter[2];
			double		cardRect[4][2];

			CComponent3D*
						shape;
			bool		drawCard;
			bool		drawRect;
			bool		fix;
			bool		stable;
public :
						CMarkerObject(void);
	virtual				~CMarkerObject();

	virtual	void		Precompute(void);
	virtual	void		Draw(void);
	virtual	void		Draw2(void) {}
	virtual	void		DrawShape(void);
	virtual	void		Read(char*);

	virtual	double		GetTransMat(ARMarkerInfo* info);

			void		DrawRect(void);
};


#endif
