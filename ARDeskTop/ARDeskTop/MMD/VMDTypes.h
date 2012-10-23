//*******************************
// VMDモーション用各種構造体定義
//*******************************

// この構造体定義は、MMDのモデルデータについて語るスレ」(http://jbbs.livedoor.jp/bbs/read.cgi/music/23040/1219738115/)や
// Ｙｕｍｉｎ氏(http://yumin3123.at.webry.info/)のVMDConverterのソースファイルを参考にさせていただきました

#ifndef	_VMDTYPES_H_
#define	_VMDTYPES_H_

#include	"VecMatQuat.h"

#pragma pack( push, 1 )

// ファイルヘッダ
struct VMD_Header
{
	char	szHeader[30];			// "Vocaloid Motion Data 0002"
	char	szModelName[20];		// 対象モデル名
};

// モーションデータ
struct VMD_Motion
{
	char	szBoneName[15];			// ボーン名

	unsigned long	ulFrameNo;		// フレーム番号

	Vector3	vec3Position;			// 位置
	Vector4	vec4Rotate;				// 回転(クォータニオン)

	char	cInterpolation1[16];	// 補間情報
	char	cInterpolation2[16];
	char	cInterpolation3[16];
	char	cInterpolation4[16];
};

// 表情データ
struct VMD_Face
{
	char	szFaceName[15];		// 表情名

	unsigned long	ulFrameNo;	// フレーム番号

	float	fFactor;			// ブレンド率
};

#pragma pack( pop )

#endif	// _VMDTYPES_H_
