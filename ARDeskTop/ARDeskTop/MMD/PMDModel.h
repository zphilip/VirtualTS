//***********
// PMDモデル
//***********

#ifndef	_PMDMODEL_H_
#define	_PMDMODEL_H_

#include	"PMDTypes.h"
#include	"PMDBone.h"
#include	"PMDIK.h"
#include	"PMDFace.h"
#include	"MotionPlayer.h"




class cPMDModel
{
	private :
		char			m_szModelName[21];	// モデル名

		unsigned long	m_ulNumVertices;	// 頂点数

		Vector3			*m_pvec3OrgPositionArray;	// 座標配列
		Vector3			*m_pvec3OrgNormalArray;		// 法線配列
		TexUV			*m_puvOrgTexureUVArray;		// テクスチャ座標配列

		struct SkinInfo
		{
			float			fWeight;		// ウェイト
			unsigned short	unBoneNo[2];	// ボーン番号
		};
		SkinInfo		*m_pOrgSkinInfoArray;

		Vector3			*m_pvec3PositionArray;
		Vector3			*m_pvec3NormalArray;

		unsigned long	m_ulNumIndices;		// 頂点インデックス数
		unsigned short	*m_pIndices;		// 頂点インデックス配列

		struct Material
		{
			Color4			col4Diffuse,
							col4Specular,
							col4Ambient;
			float			fShininess;

			unsigned long	ulNumIndices;
			unsigned int	uiTexID;		// テクスチャID
		};
		unsigned long	m_ulNumMaterials;	// マテリアル数
		Material		*m_pMaterials;		// マテリアル配列

		unsigned short	m_unNumBones;		// ボーン数
		cPMDBone		*m_pBoneArray;		// ボーン配列

		cPMDBone		*m_pNeckBone;		// 首のボーン
		bool			m_bLookAt;			// 首をターゲットへ向けるかどうか

		unsigned short	m_unNumIK;			// IK数
		cPMDIK			*m_pIKArray;		// IK配列

		unsigned short	m_unNumFaces;		// 表情数
		cPMDFace		*m_pFaceArray;		// 表情配列

		cMotionPlayer	m_clMotionPlayer;

	public :
		cPMDModel( void );
		~cPMDModel( void );

		bool load( const char *szFilePath, const char *szDirFilePath ); // 修正
		bool initialize( unsigned char *pData, const char *szDirFilePath ); // 修正

		void setMotion( cVMDMotion *pVMDMotion, bool bLoop );
		void updateMotion( float fFrame );
		void updateNeckBone( const Vector3 *pvec3LookTarget );
		void updateSkinning( void );
		void updateSkinning2( void );		// 追加

		void render( void );

		void release( void );

		void getBB( Vector3 &min, Vector3 &max );				// 追加

		inline void toggleLookAtFlag( void ){ m_bLookAt = !m_bLookAt; } 

		inline const char *getModelName( void ){ return m_szModelName; } 

	friend class cMotionPlayer;
	friend class CMMDObject;		// 追加
};

#endif	// _PMDMODEL_H_
