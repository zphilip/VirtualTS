//****************
// ���[�V�����Đ�
//****************

#include	<stdio.h>
#include	<malloc.h>
#include	<string.h>
#include	"PMDModel.h"
#include	"MotionPlayer.h"


//================
// �R���X�g���N�^
//================
cMotionPlayer::cMotionPlayer( void ) : m_pVMDMotion( NULL ), m_ppBoneList( NULL ), m_ppFaceList( NULL )
{
}

//==============
// �f�X�g���N�^
//==============
cMotionPlayer::~cMotionPlayer( void )
{
	clear();
}

//==========================
// �Đ����郂�[�V�����̐ݒ�
//==========================
void cMotionPlayer::setup( cPMDModel *pPMDModel, cVMDMotion *pMotion, bool bLoop )
{
	clear();


	m_pPMDModel = pPMDModel;
	m_pVMDMotion = pMotion;

	//---------------------------------------------------------
	// ����Ώۃ{�[���̃|�C���^��ݒ肷��
	MotionDataList	*pMotionDataList = m_pVMDMotion->getMotionDataList();
	m_ppBoneList = new cPMDBone *[m_pVMDMotion->getNumMotionNodes()];
	cPMDBone		**ppBone = m_ppBoneList;
	while( pMotionDataList )
	{
		(*ppBone) = getBoneByName( pMotionDataList->szBoneName, pPMDModel->m_pBoneArray, pPMDModel->m_unNumBones );

		pMotionDataList = pMotionDataList->pNext;
		ppBone++;
	}

	//---------------------------------------------------------
	// ����Ώە\��̃|�C���^��ݒ肷��
	FaceDataList	*pFaceDataList = m_pVMDMotion->getFaceDataList();
	m_ppFaceList = new cPMDFace *[m_pVMDMotion->getNumFaceNodes()];
	cPMDFace		**ppFace = m_ppFaceList;
	while( pFaceDataList )
	{
		(*ppFace) = getFaceByName( pFaceDataList->szFaceName, pPMDModel->m_pFaceArray, pPMDModel->m_unNumFaces );

		pFaceDataList = pFaceDataList->pNext;
		ppFace++;
	}

	// �ϐ������l�ݒ�
	m_fOldFrame = m_fFrame = 0.0f;
	m_bLoop = bLoop;
}

//----------------------
// �w�薼�̃{�[����Ԃ�
//----------------------
cPMDBone *cMotionPlayer::getBoneByName( const char *szBoneName, cPMDBone pBoneArray[], unsigned long ulNumBones )
{
	for( unsigned long i = 0 ; i < ulNumBones ; i++, pBoneArray++ )
	{
		if( strncmp( pBoneArray->getName(), szBoneName, 20 ) == 0 )
			return pBoneArray;
	}

	return NULL;
}

//--------------------
// �w�薼�̕\���Ԃ�
//--------------------
cPMDFace *cMotionPlayer::getFaceByName( const char *szFaceName, cPMDFace pFaceArray[], unsigned long ulNumFaces )
{
	for( unsigned long i = 0 ; i < ulNumFaces ; i++, pFaceArray++ )
	{
		if( strncmp( pFaceArray->getName(), szFaceName, 20 ) == 0 )
			return pFaceArray;
	}

	return NULL;
}

//================
// ���[�V�����X�V
//================
bool cMotionPlayer::update( float fFrame )
{
	if( !m_pVMDMotion )		return true;

	//---------------------------------------------------------
	// �w��t���[���̃f�[�^�Ń{�[���𓮂���
	MotionDataList	*pMotionDataList = m_pVMDMotion->getMotionDataList();
	cPMDBone		**ppBone = m_ppBoneList;
	Vector3			vec3Position;
	Vector4			vec4Rotate;

	while( pMotionDataList )
	{
		if( *ppBone )
		{
		//	getMotionPosRot( pMotionDataList, m_fFrame, &vec3Position, &vec4Rotate );
			getMotionPosRot( pMotionDataList, fFrame, &vec3Position, &vec4Rotate );

			// ��ԂȂ�
			(*ppBone)->m_vec3Position = vec3Position;
			(*ppBone)->m_vec4Rotate = vec4Rotate;
			// �ǉ��F����p�{�[���ɂ����f

// ��Ԃ���
//			Vector3Lerp( &((*pBone)->m_vec3Position), &((*pBone)->m_vec3Position), &vec3Position, fLerpValue );
//			QuaternionSlerp( &((*pBone)->m_vec4Rotate), &((*pBone)->m_vec4Rotate), &vec4Rotate, fLerpValue );
		}

		pMotionDataList = pMotionDataList->pNext;
		ppBone++;
	}

	//---------------------------------------------------------
	// �w��t���[���̃f�[�^�ŕ\���ό`����
	FaceDataList	*pFaceDataList = m_pVMDMotion->getFaceDataList();
	cPMDFace		**ppFace = m_ppFaceList;
	float			fFaceRate;

	while( pFaceDataList )
	{
		if( *ppFace )
		{
			fFaceRate = getFaceRate( pFaceDataList, m_fFrame );

			if( fFaceRate == 1.0f )			(*ppFace)->setFace(   m_pPMDModel->m_pvec3OrgPositionArray );
			else if( 0.001f < fFaceRate )	(*ppFace)->blendFace( m_pPMDModel->m_pvec3OrgPositionArray, fFaceRate );
		}

		pFaceDataList = pFaceDataList->pNext;
		ppFace++;
	}

	//---------------------------------------------------------
	// �t���[����i�߂�
	bool	bMotionFinshed = false;

//	m_fOldFrame = m_fFrame;
//	m_fFrame += fElapsedFrame;
	m_fOldFrame = m_fFrame = fFrame;	// �C��

	if( m_bLoop )
	{
		if( m_fOldFrame >= m_pVMDMotion->getMaxFrame() )
		{
			m_fOldFrame = 0.0f;
			m_fFrame = m_fFrame - m_pVMDMotion->getMaxFrame();
		}
	}

	if( m_fFrame >= m_pVMDMotion->getMaxFrame() )
	{
		m_fFrame = m_pVMDMotion->getMaxFrame();
		bMotionFinshed = true;
	}

	return bMotionFinshed;
}

//----------------------------------------
// �L�[�t���[�����Ԃ��Ĉʒu�Ɖ�]��Ԃ�
//----------------------------------------
void cMotionPlayer::getMotionPosRot( const MotionDataList *pMotionData, float fFrame, Vector3 *pvec3Pos, Vector4 *pvec4Rot )
{
	unsigned long	i;
	unsigned long	ulNumKeyFrame = pMotionData->ulNumKeyFrames;

	BoneKeyFrameList	*pKeyFrameTemp = pMotionData->pKeyFrameList;

	// �Ō�܂Ői��
	while( pKeyFrameTemp->pNext ) {
		pKeyFrameTemp = pKeyFrameTemp->pNext;
	}

	// �ŏI�t���[�����߂��Ă����ꍇ
	if( fFrame > pKeyFrameTemp->fFrameNo )
	{
		fFrame = pKeyFrameTemp->fFrameNo;
	}

	// ���݂̎��Ԃ��ǂ̃L�[�ߕӂɂ��邩
	BoneKeyFrameList	*pPrevKeyFrame;

	pKeyFrameTemp = pMotionData->pKeyFrameList;
	pPrevKeyFrame = pKeyFrameTemp;
	for( i = 0 ; i < ulNumKeyFrame ; i++ )
	{
		if( fFrame <= pKeyFrameTemp->fFrameNo )
		{
			break;
		}
		pPrevKeyFrame = pKeyFrameTemp;
		pKeyFrameTemp = pKeyFrameTemp->pNext;
	}

	// �O��̃L�[��ݒ�
	if( !pKeyFrameTemp ) pKeyFrameTemp = pPrevKeyFrame;

	// �O��̃L�[�̎���
	float	fTime0 = pPrevKeyFrame->fFrameNo;
	float	fTime1 = pKeyFrameTemp->fFrameNo;

	// �O��̃L�[�̊Ԃłǂ̈ʒu�ɂ��邩
	float	fLerpValue;
	if( pPrevKeyFrame != pKeyFrameTemp )
	{
		fLerpValue = (fFrame - fTime0) / (fTime1 - fTime0);
		Vector3Lerp( pvec3Pos, &(pPrevKeyFrame->vec3Position), &(pKeyFrameTemp->vec3Position), fLerpValue );
		QuaternionSlerp( pvec4Rot, &(pPrevKeyFrame->vec4Rotate), &(pKeyFrameTemp->vec4Rotate), fLerpValue );
		QuaternionNormalize( pvec4Rot, pvec4Rot );
	}
	else
	{
		*pvec3Pos = pPrevKeyFrame->vec3Position;
		*pvec4Rot = pPrevKeyFrame->vec4Rotate;
	}
}

//--------------------------------------------
// �L�[�t���[�����Ԃ��ĕ\��u�����h����Ԃ�
//--------------------------------------------
float cMotionPlayer::getFaceRate( const FaceDataList *pFaceData, float fFrame )
{
	unsigned long	i;
	unsigned long	ulNumKeyFrame = pFaceData->ulNumKeyFrames;

	FaceKeyFrameList	*pKeyFrameTemp = pFaceData->pKeyFrameList;

	// �Ō�܂Ői��
	while( pKeyFrameTemp->pNext ) {
		pKeyFrameTemp = pKeyFrameTemp->pNext;
	}

	// �ŏI�t���[�����߂��Ă����ꍇ
	if( fFrame > pKeyFrameTemp->fFrameNo )
	{
		fFrame = pKeyFrameTemp->fFrameNo;
	}

	// ���݂̎��Ԃ��ǂ̃L�[�ߕӂɂ��邩
	FaceKeyFrameList	*pPrevKeyFrame;

	pKeyFrameTemp = pFaceData->pKeyFrameList;
	pPrevKeyFrame = pKeyFrameTemp;

	for( i = 0 ; i < ulNumKeyFrame ; i++ )
	{
		if( fFrame <= pKeyFrameTemp->fFrameNo )
		{
			break;
		}
		pPrevKeyFrame = pKeyFrameTemp;
		pKeyFrameTemp = pKeyFrameTemp->pNext;
	}

	// �O��̃L�[��ݒ�
	if( !pKeyFrameTemp ) pKeyFrameTemp = pPrevKeyFrame;

	// �O��̃L�[�̎���
	float	fTime0 = pPrevKeyFrame->fFrameNo;
	float	fTime1 = pKeyFrameTemp->fFrameNo;

	// �O��̃L�[�̊Ԃłǂ̈ʒu�ɂ��邩
	float	fLerpValue;
	if( pPrevKeyFrame != pKeyFrameTemp )
	{
		fLerpValue = (fFrame - fTime0) / (fTime1 - fTime0);
		return (pPrevKeyFrame->fRate * (1.0f - fLerpValue)) + (pKeyFrameTemp->fRate * fLerpValue);
	}
	else
	{
		return pPrevKeyFrame->fRate;
	}
}

//==================================
// �ݒ肳�ꂽ���[�V�������N���A����
//==================================
void cMotionPlayer::clear( void )
{
	m_pVMDMotion = NULL;

	if( m_ppBoneList )
	{
		delete [] m_ppBoneList;
		m_ppBoneList = NULL;
	}

	if( m_ppFaceList )
	{
		delete [] m_ppFaceList;
		m_ppFaceList = NULL;
	}
}
