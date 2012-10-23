#ifndef ___CComment
#define ___CComment

#include "CLabel3D.h"
#include "CAnimeMatrix.h"

#define LAYER_NUM	7
#define LINE_NUM	30
#define FONT_NUM	3



class CCommentData {
public :
			int			id;
			int			command;
			int			layer;
			int			size;
			int			color;
			int			depth;

			int			time;
			char		str[256];
public :
			void		SetSize(char* str);
			void		SetColor(char* str);
			void		SetLayer(char* str);
};


class CComment : public CLabel3D
{
public :
	//レイヤー
	enum
	{
		kUe = 0,
		kNaka,
		kShita,
		kHidari,
		kMigi,
		kTwist,
		kTate,
		kPict,
		kSound,
	};
	//文字サイズ
	enum
	{
		kBig = 0,
		kMedium,
		kSmall
	};
	//色
	enum
	{
		kBlack,
		kWhite,
		kRed,
		kGreen,
		kBlue,
		kYellow,
		kCyan,
		kMagenta,
		kPink,
		kOrenge,
		kPurple,
		kTruered,
		kElementalgreen,
		kMarineblue,
		kNobleviolet,
		kMadyellow,
		kPassionorange,
	};
public :
						CComment(void);
	virtual				~CComment();

			void		SetCommentSize(int size);
			void		SetCommentColor(int color, float alpha = 1.0f);
			void		SetCommentAlignment(int layer);
			void		SetCommentAnime(int layer, float animetionTime, double vec);
public :
	static	CFont		fontBig;
	static	CFont		fontMedium;
	static	CFont		fontSmall;
	static	bool		fixedLayer[LAYER_NUM];
	static	float		sizeList[FONT_NUM];
public :
	static	void		InitComment(void);
};


#endif
