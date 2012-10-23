#ifndef ___CCommentManager
#define ___CCommentManager

#include "CAnimeManager.h"
#include "CComment.h"
#include "CFont.h"




class CCommentLine {
public :
			float		reachTime;
			float		endTime;
			float		height;
public :
			void		Init(void);
};


class CCommentLayer {
public :
			CCommentLine
						line[LINE_NUM];
public :
			void		Init(void);
			float		AddComment(CComment* comment, CCommentData* data, float reachTime, float endTime, float max);

};


class CCommentManager : public CAnimeManager
{
public :
			int			commentNum;
			CCommentData**
						commentData;
			CCommentLayer
						layerList[LAYER_NUM];
public :
						CCommentManager(CComponent3D* pComponent = 0);
	virtual				~CCommentManager();

	virtual	void		Init(void);
	virtual	int			Animate(void);

			void		Read(char* fileName);
			void		CleanUp(void);
};


#endif
