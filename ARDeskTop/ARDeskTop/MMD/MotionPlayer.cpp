//****************
// モーション再生
//****************

#include	<stdio.h>
#include	<malloc.h>
#include	<string.h>
#include	"PMDModel.h"
#include	"MotionPlayer.h"


//================
// コンストラクタ
//================
cMotionPlayer::cMotionPlayer( void ) : m_pVMDMotion( NULL ), m_ppBoneList( NULL ), m_ppFaceList( NULL )
{
}

//==============
// デストラクタ
//==============
cMotionPlayer::~cMotionPlayer( void )
{
	clear();
}

//==========================
// 再生するモーションの設定
//==========================
void cMotionPlayer::setup( cPMDModel *pPMDModel, cVMDMotion *pMotion, bool bLoop )
{
	clear();


	m_pPMDModel = pPMDModel;
	m_pVMDMotion = pMotion;

	//---------------------------------------------------------
	// 操作対象ボーンのポインタを設定する
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
	// 操作対象表情のポインタを設定する
	FaceDataList	*pFaceDataList = m_pVMDMotion->getFaceDataList();
	m_ppFaceList = new cPMDFace *[m_pVMDMotion->getNumFaceNodes()];
	cPMDFace		**ppFace = m_ppFaceList;
	while( pFaceDataList )
	{
		(*ppFace) = getFaceByName( pFaceDataList->szFaceName, pPMDModel->m_pFaceArray, pPMDModel->m_unNumFaces );

		pFaceDataList = pFaceDataList->pNext;
		ppFace++;
	}

	// 変数初期値設定
	m_fOldFrame = m_fFrame = 0.0f;
	m_bLoop = bLoop;
}

//----------------------
// 指定名のボーンを返す
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
// 指定名の表情を返す
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
// モーション更新
//================
bool cMotionPlayer::update( float fFrame )
{
	if( !m_pVMDMotion )		return true;

	//---------------------------------------------------------
	// 指定フレームのデータでボーンを動かす
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

			// 補間なし
			(*ppBone)->m_vec3Position = vec3Position;
			(*ppBone)->m_vec4Rotate = vec4Rotate;
			// 追加：操作用ボーンにも反映

// 補間あり
//			Vector3Lerp( &((*pBone)->m_vec3Position), &((*pBone)->m_vec3Position), &vec3Position, fLerpValue );
//			QuaternionSlerp( &((*pBone)->m_vec4Rotate), &((*pBone)->m_vec4Rotate), &vec4Rotate, fLerpValue );
		}

		pMotionDataList = pMotionDataList->pNext;
		ppBone++;
	}

	//---------------------------------------------------------
	// 指定フレームのデータで表情を変形する
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
	// フレームを進める
	bool	bMotionFinshed = false;

//	m_fOldFrame = m_fFrame;
//	m_fFrame += fElapsedFrame;
	m_fOldFrame = m_fFrame = fFrame;	// 修正

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
// キーフレームを補間して位置と回転を返す
//----------------------------------------
void cMotionPlayer::getMotionPosRot( const MotionDataList *pMotionData, float fFrame, Vector3 *pvec3Pos, Vector4 *pvec4Rot )
{
	unsigned long	i;
	unsigned long	ulNumKeyFrame = pMotionData->ulNumKeyFrames;

	BoneKeyFrameList	*pKeyFrameTemp = pMotionData->pKeyFrameList;

	// 最後まで進む
	while( pKeyFrameTemp->pNext ) {
		pKeyFrameTemp = pKeyFrameTemp->pNext;
	}

	// 最終フレームを過ぎていた場合
	if( fFrame > pKeyFrameTemp->fFrameNo )
	{
		fFrame = pKeyFrameTemp->fFrameNo;
	}

	// 現在の時間がどのキー近辺にあるか
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

	// 前後のキーを設定
	if( !pKeyFrameTemp ) pKeyFrameTemp = pPrevKeyFrame;

	// 前後のキーの時間
	float	fTime0 = pPrevKeyFrame->fFrameNo;
	float	fTime1 = pKeyFrameTemp->fFrameNo;

	// 前後のキーの間でどの位置にいるか
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
// キーフレームを補間して表情ブレンド率を返す
//--------------------------------------------
float cMotionPlayer::getFaceRate( const FaceDataList *pFaceData, float fFrame )
{
	unsigned long	i;
	unsigned long	ulNumKeyFrame = pFaceData->ulNumKeyFrames;

	FaceKeyFrameList	*pKeyFrameTemp = pFaceData->pKeyFrameList;

	// 最後まで進む
	while( pKeyFrameTemp->pNext ) {
		pKeyFrameTemp = pKeyFrameTemp->pNext;
	}

	// 最終フレームを過ぎていた場合
	if( fFrame > pKeyFrameTemp->fFrameNo )
	{
		fFrame = pKeyFrameTemp->fFrameNo;
	}

	// 現在の時間がどのキー近辺にあるか
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

	// 前後のキーを設定
	if( !pKeyFrameTemp ) pKeyFrameTemp = pPrevKeyFrame;

	// 前後のキーの時間
	float	fTime0 = pPrevKeyFrame->fFrameNo;
	float	fTime1 = pKeyFrameTemp->fFrameNo;

	// 前後のキーの間でどの位置にいるか
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
// 設定されたモーションをクリアする
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
