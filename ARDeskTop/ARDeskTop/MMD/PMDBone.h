//***********
// PMDボーン
//***********

#ifndef	_PMDBONE_H_
#define	_PMDBONE_H_

#include	"PMDTypes.h"

class CFKBone;	// 追加


class cPMDBone
{
	private :
		char			m_szName[21];

		bool			m_bIKLimitAngle;	// IK時に角度制限をするかどうか

		Vector3			m_vec3OrgPosition;
		Vector3			m_vec3Offset;

		Matrix			m_matInvTransform;	// 初期値のボーンを原点に移動させるような行列

		const cPMDBone	*m_pParentBone;
		cPMDBone		*m_pChildBone;

		// 以下は現在の値
		Vector3			m_vec3Position;
		Vector4			m_vec4Rotate;

		Matrix			m_matLocal;

	public :
		Matrix			m_matSkinning;		// 頂点デフォーム用行列
		CFKBone			*fkBone;			// 追加

		cPMDBone( void );
		~cPMDBone( void );

		void initialize( const PMD_Bone *pPMDBoneData, const cPMDBone pBoneArray[] );
		void recalcOffset( void );

		void reset( void );

		void updateMatrix( void );
		void lookAt( const Vector3 *pvecTargetPos );
		void updateSkinningMat( void );
		void updateSkinningMat2( void );	// 追加

		void debugDraw( void );

		inline const char *getName( void ){ return m_szName; }
		inline const void getOrgPos( Vector3 *pvec3Out ){ *pvec3Out = m_vec3OrgPosition; }

	friend class cMotionPlayer;
	friend class cPMDIK;
	friend class CMMDObject;	// 追加
};

#endif	// _PMDMODEL_H_
