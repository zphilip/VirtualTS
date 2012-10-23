#ifndef ___CMMDObject
#define ___CMMDObject

#include "CFrame.h"
#include "MMD/PMDModel.h"
#include "MMD/VMDMotion.h"


class CMMDObject : public CFrame
{
public :
			cPMDModel	g_clPMDModel;
			cVMDMotion	g_clVMDMotion;
			bool		isHandling;
public :
						CMMDObject(void);
	virtual				~CMMDObject();

	virtual	void		Init(void);
	virtual	void		Precompute(void);
	virtual	void		DrawShape(void);
	virtual	void		SetSize(double w, double h, double d);

			void		OpenModel(char* dirName, char* fileName);
			void		OpenMotion(char* fileName);
			void		BoundsModel(void);
			void		SetBones(void);
			void		UpdateMatrix(void);
};


#endif
