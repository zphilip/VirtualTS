//***********
// PMD�{�[��
//***********

#ifndef	_PMDBONE_H_
#define	_PMDBONE_H_

#include	"PMDTypes.h"

class CFKBone;	// �ǉ�


class cPMDBone
{
	private :
		char			m_szName[21];

		bool			m_bIKLimitAngle;	// IK���Ɋp�x���������邩�ǂ���

		Vector3			m_vec3OrgPosition;
		Vector3			m_vec3Offset;

		Matrix			m_matInvTransform;	// �����l�̃{�[�������_�Ɉړ�������悤�ȍs��

		const cPMDBone	*m_pParentBone;
		cPMDBone		*m_pChildBone;

		// �ȉ��͌��݂̒l
		Vector3			m_vec3Position;
		Vector4			m_vec4Rotate;

		Matrix			m_matLocal;

	public :
		Matrix			m_matSkinning;		// ���_�f�t�H�[���p�s��
		CFKBone			*fkBone;			// �ǉ�

		cPMDBone( void );
		~cPMDBone( void );

		void initialize( const PMD_Bone *pPMDBoneData, const cPMDBone pBoneArray[] );
		void recalcOffset( void );

		void reset( void );

		void updateMatrix( void );
		void lookAt( const Vector3 *pvecTargetPos );
		void updateSkinningMat( void );
		void updateSkinningMat2( void );	// �ǉ�

		void debugDraw( void );

		inline const char *getName( void ){ return m_szName; }
		inline const void getOrgPos( Vector3 *pvec3Out ){ *pvec3Out = m_vec3OrgPosition; }

	friend class cMotionPlayer;
	friend class cPMDIK;
	friend class CMMDObject;	// �ǉ�
};

#endif	// _PMDMODEL_H_
