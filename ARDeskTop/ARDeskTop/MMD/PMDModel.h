//***********
// PMD���f��
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
		char			m_szModelName[21];	// ���f����

		unsigned long	m_ulNumVertices;	// ���_��

		Vector3			*m_pvec3OrgPositionArray;	// ���W�z��
		Vector3			*m_pvec3OrgNormalArray;		// �@���z��
		TexUV			*m_puvOrgTexureUVArray;		// �e�N�X�`�����W�z��

		struct SkinInfo
		{
			float			fWeight;		// �E�F�C�g
			unsigned short	unBoneNo[2];	// �{�[���ԍ�
		};
		SkinInfo		*m_pOrgSkinInfoArray;

		Vector3			*m_pvec3PositionArray;
		Vector3			*m_pvec3NormalArray;

		unsigned long	m_ulNumIndices;		// ���_�C���f�b�N�X��
		unsigned short	*m_pIndices;		// ���_�C���f�b�N�X�z��

		struct Material
		{
			Color4			col4Diffuse,
							col4Specular,
							col4Ambient;
			float			fShininess;

			unsigned long	ulNumIndices;
			unsigned int	uiTexID;		// �e�N�X�`��ID
		};
		unsigned long	m_ulNumMaterials;	// �}�e���A����
		Material		*m_pMaterials;		// �}�e���A���z��

		unsigned short	m_unNumBones;		// �{�[����
		cPMDBone		*m_pBoneArray;		// �{�[���z��

		cPMDBone		*m_pNeckBone;		// ��̃{�[��
		bool			m_bLookAt;			// ����^�[�Q�b�g�֌����邩�ǂ���

		unsigned short	m_unNumIK;			// IK��
		cPMDIK			*m_pIKArray;		// IK�z��

		unsigned short	m_unNumFaces;		// �\�
		cPMDFace		*m_pFaceArray;		// �\��z��

		cMotionPlayer	m_clMotionPlayer;

	public :
		cPMDModel( void );
		~cPMDModel( void );

		bool load( const char *szFilePath, const char *szDirFilePath ); // �C��
		bool initialize( unsigned char *pData, const char *szDirFilePath ); // �C��

		void setMotion( cVMDMotion *pVMDMotion, bool bLoop );
		void updateMotion( float fFrame );
		void updateNeckBone( const Vector3 *pvec3LookTarget );
		void updateSkinning( void );
		void updateSkinning2( void );		// �ǉ�

		void render( void );

		void release( void );

		void getBB( Vector3 &min, Vector3 &max );				// �ǉ�

		inline void toggleLookAtFlag( void ){ m_bLookAt = !m_bLookAt; } 

		inline const char *getModelName( void ){ return m_szModelName; } 

	friend class cMotionPlayer;
	friend class CMMDObject;		// �ǉ�
};

#endif	// _PMDMODEL_H_
