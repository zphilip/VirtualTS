//***************
// VMDモーション
//***************

#include	<stdio.h>
#include	<stdlib.h>
#include	<malloc.h>
#include	<string.h>
#include	"VMDMotion.h"


//------------------------------
// ボーンキーフレームソート用比較関数
//------------------------------
static int boneCompareFunc( const void *pA, const void *pB )
{
//	return (int)(((BoneKeyFrame *)pA)->fFrameNo - ((BoneKeyFrame *)pB)->fFrameNo);
}

//------------------------------
// 表情キーフレームソート用比較関数
//------------------------------
static int faceCompareFunc( const void *pA, const void *pB )
{
//	return (int)(((FaceKeyFrame *)pA)->fFrameNo - ((FaceKeyFrame *)pB)->fFrameNo);
}

//================
// コンストラクタ
//================
cVMDMotion::cVMDMotion( void ) : m_pMotionDataList( NULL ), m_pFaceDataList( NULL )
{
}

//==============
// デストラクタ
//==============
cVMDMotion::~cVMDMotion( void )
{
	release();
}

//====================
// ファイルの読み込み
//====================
bool cVMDMotion::load( const char *szFilePath )
{
	FILE	*pFile;
	fpos_t	fposFileSize;
	unsigned char
			*pData;

	pFile = fopen( szFilePath, "rb" );
	if( !pFile )	return false;	// ファイルが開けない

	// ファイルサイズ取得
	fseek( pFile, 0, SEEK_END );
	fgetpos( pFile, &fposFileSize );

	// メモリ確保
	pData = (unsigned char *)malloc( (size_t)fposFileSize );

	// 読み込み
	fseek( pFile, 0, SEEK_SET );
	fread( pData, 1, (size_t)fposFileSize, pFile );

	fclose( pFile );

	// モーションデータ初期化
	bool	bRet = initialize( pData );

	free( pData );

	return bRet;
}

//==========================
// モーションデータの初期化
//==========================
bool cVMDMotion::initialize( unsigned char *pData )
{
	release();

	// ヘッダのチェック
	VMD_Header	*pVMDHeader = (VMD_Header *)pData;
	if( strncmp( pVMDHeader->szHeader, "Vocaloid Motion Data 0002", 30 ) != 0 )
		return false;	// ファイル形式が違う

	pData += sizeof( VMD_Header );

	//-----------------------------------------------------
	// ボーンのキーフレーム数を取得
	unsigned long	ulNumBoneKeyFrames = *((unsigned long *)pData);
	pData += sizeof( unsigned long );

	// まずはモーションデータ中のボーンごとのキーフレーム数をカウント
	VMD_Motion		*pVMDMotion = (VMD_Motion *)pData;
	MotionDataList	*pMotTemp;

	m_ulNumMotionNodes = 0;
	m_fMaxFrame = 0.0f;
	for( unsigned long i = 0 ; i < ulNumBoneKeyFrames ; i++, pVMDMotion++ )
	{
		if( m_fMaxFrame < (float)pVMDMotion->ulFrameNo )	m_fMaxFrame = (float)pVMDMotion->ulFrameNo;	// 最大フレーム更新

		pMotTemp = m_pMotionDataList;

		while( pMotTemp )
		{
			if( strncmp( pMotTemp->szBoneName, pVMDMotion->szBoneName, 15 ) == 0 ) break;
			pMotTemp = pMotTemp->pNext;
		}

		if( !pMotTemp )
		{
			// リストにない場合は新規ノードを追加
			MotionDataList	*pNew = new MotionDataList;

			strncpy( pNew->szBoneName, pVMDMotion->szBoneName, 15 );
			pNew->szBoneName[15]	= '\0';
			pNew->ulNumKeyFrames	= 0;
			pNew->pKeyFrameList		= NULL;

			pNew->pNext			= m_pMotionDataList;
			m_pMotionDataList	= pNew;
			pMotTemp			= pNew;

			m_ulNumMotionNodes++;
		}

		BoneKeyFrameList	*pKeyFrame = new BoneKeyFrameList;

		pKeyFrame->fFrameNo     = (float)pVMDMotion->ulFrameNo;
		pKeyFrame->vec3Position = pVMDMotion->vec3Position;
		pKeyFrame->pNext		= NULL;
		QuaternionNormalize( &pKeyFrame->vec4Rotate, &pVMDMotion->vec4Rotate );

		if( !pMotTemp->pKeyFrameList )
		{
			pMotTemp->pKeyFrameList = pKeyFrame;
		}
		else
		{
			pKeyFrame->pNext = pMotTemp->pKeyFrameList;
			pMotTemp->pKeyFrameList = pKeyFrame;
		}

		pMotTemp->ulNumKeyFrames++;
	}

	// キーフレーム配列を昇順にソート
	pMotTemp = m_pMotionDataList;

	while( pMotTemp )
	{
	//	qsort( pMotTemp->pKeyFrameList, pMotTemp->ulNumKeyFrames, sizeof( BoneKeyFrame ), boneCompareFunc );
		sortByFrame( pMotTemp );
		pMotTemp = pMotTemp->pNext;
	}

	pData += sizeof( VMD_Motion ) * ulNumBoneKeyFrames;

	//-----------------------------------------------------
	// 表情のキーフレーム数を取得
	unsigned long	ulNumFaceKeyFrames = *((unsigned long *)pData);
	pData += sizeof( unsigned long );

	// モーションデータ中の表情ごとのキーフレーム数をカウント
	VMD_Face		*pVMDFace = (VMD_Face *)pData;
	FaceDataList	*pFaceTemp;

	m_ulNumFaceNodes = 0;
	for( unsigned long i = 0 ; i < ulNumFaceKeyFrames ; i++, pVMDFace++ )
	{
		if( m_fMaxFrame < (float)pVMDFace->ulFrameNo )	m_fMaxFrame = (float)pVMDFace->ulFrameNo;	// 最大フレーム更新

		pFaceTemp = m_pFaceDataList;

		while( pFaceTemp )
		{
			if( strncmp( pFaceTemp->szFaceName, pVMDFace->szFaceName, 15 ) == 0 ) break;
			pFaceTemp = pFaceTemp->pNext;
		}

		if( !pFaceTemp )
		{
			// リストにない場合は新規ノードを追加
			FaceDataList	*pNew = new FaceDataList;

			strncpy( pNew->szFaceName, pVMDFace->szFaceName, 15 );
			pNew->szFaceName[15]	= '\0';
			pNew->ulNumKeyFrames	= 0;

			pNew->pNext				= m_pFaceDataList;
			m_pFaceDataList			= pNew;
			pFaceTemp				= pNew;

			m_ulNumFaceNodes++;
		}

		FaceKeyFrameList	*pKeyFrame = new FaceKeyFrameList;

		pKeyFrame->fFrameNo = (float)pVMDFace->ulFrameNo;
		pKeyFrame->fRate    =        pVMDFace->fFactor;
		pKeyFrame->pNext	= NULL;

		if( !pFaceTemp->pKeyFrameList )
		{
			pFaceTemp->pKeyFrameList = pKeyFrame;
		}
		else
		{
			pKeyFrame->pNext = pFaceTemp->pKeyFrameList;
			pFaceTemp->pKeyFrameList = pKeyFrame;
		}

		pFaceTemp->ulNumKeyFrames++;
	}

	// キーフレーム配列を昇順にソート
	pFaceTemp = m_pFaceDataList;

	while( pFaceTemp )
	{
	//	qsort( pFaceTemp->pKeyFrameList, pFaceTemp->ulNumKeyFrames, sizeof( FaceKeyFrame ), faceCompareFunc );
		sortByFrame( pFaceTemp );
		pFaceTemp = pFaceTemp->pNext;
	}

	return true;
}

//======
// 解放
//======
void cVMDMotion::release( void )
{
	// モーションデータの解放
	MotionDataList	*pMotTemp = m_pMotionDataList,
					*pNextMotTemp;

	BoneKeyFrameList *pBoneKeyTemp, *pNextBoneKeyTemp;

	while( pMotTemp )
	{
		pNextMotTemp = pMotTemp->pNext;

	//	delete [] pMotTemp->pKeyFrameList;
		pBoneKeyTemp = pMotTemp->pKeyFrameList;
		while( pMotTemp )
		{
			pNextBoneKeyTemp = pBoneKeyTemp->pNext;
			delete pBoneKeyTemp;
			pBoneKeyTemp = pNextBoneKeyTemp;
		}

		delete pMotTemp;

		pMotTemp = pNextMotTemp;
	}

	m_pMotionDataList = NULL;

	// 表情データの解放
	FaceDataList	*pFaceTemp = m_pFaceDataList,
					*pNextFaceTemp;

	FaceKeyFrameList	*pFaceKeyTemp, *pNextFaceKeyTemp;

	while( pFaceTemp )
	{
		pNextFaceTemp = pFaceTemp->pNext;

	//	delete [] pFaceTemp->pKeyFrameList;
		pFaceKeyTemp = pFaceTemp->pKeyFrameList;
		while( pFaceTemp )
		{
			pNextFaceKeyTemp = pFaceKeyTemp->pNext;
			delete pFaceKeyTemp;
			pFaceKeyTemp = pNextFaceKeyTemp;
		}

		delete pFaceTemp;

		pFaceTemp = pNextFaceTemp;
	}

	m_pFaceDataList = NULL;
}

void cVMDMotion::sortByFrame( MotionDataList *pMot )
{
	if( pMot->ulNumKeyFrames <= 1) return;

	BoneKeyFrameList	*pKeyFrame			= pMot->pKeyFrameList;
	BoneKeyFrameList	**ppKeyFrameList	= new BoneKeyFrameList *[pMot->ulNumKeyFrames];
	float				*pFrameList			= new float[pMot->ulNumKeyFrames];

	for( unsigned long i = 0 ; i < pMot->ulNumKeyFrames ; i++ )
	{
		pFrameList[i]		= pKeyFrame->fFrameNo;
		ppKeyFrameList[i]	= pKeyFrame;
		pKeyFrame			= pKeyFrame->pNext;
	}

	BubbleSortf( pFrameList, ppKeyFrameList, pMot->ulNumKeyFrames );

	pMot->pKeyFrameList = ppKeyFrameList[0];
	for( unsigned long i = 0 ; i < pMot->ulNumKeyFrames ; i++ )
	{
		ppKeyFrameList[i]->pNext = i < pMot->ulNumKeyFrames - 1 ? ppKeyFrameList[i + 1] : 0;
	}

	delete pFrameList;
	delete ppKeyFrameList;

/*	pKeyFrame = pMot->pKeyFrameList;
	while( pKeyFrame ) {
		printf("%f\n", pKeyFrame->fFrameNo);
		pKeyFrame = pKeyFrame->pNext;
	}
*/
}

void cVMDMotion::sortByFrame( FaceDataList *pFace )
{
	if( pFace->ulNumKeyFrames <= 1) return;

	FaceKeyFrameList	*pKeyFrame			= pFace->pKeyFrameList;
	FaceKeyFrameList	**ppKeyFrameList	= new FaceKeyFrameList *[pFace->ulNumKeyFrames];
	float				*pFrameList			= new float[pFace->ulNumKeyFrames];

	for( unsigned long i = 0 ; i < pFace->ulNumKeyFrames ; i++ )
	{
		pFrameList[i]		= pKeyFrame->fFrameNo;
		ppKeyFrameList[i]	= pKeyFrame;
		pKeyFrame			= pKeyFrame->pNext;
	}

	BubbleSortf( pFrameList, ppKeyFrameList, pFace->ulNumKeyFrames );

	pFace->pKeyFrameList = ppKeyFrameList[0];
	for( unsigned long i = 0 ; i < pFace->ulNumKeyFrames ; i++ )
	{
		ppKeyFrameList[i]->pNext = i < pFace->ulNumKeyFrames - 1 ? ppKeyFrameList[i + 1] : 0;
	}

	delete pFrameList;
	delete ppKeyFrameList;
}

void BubbleSortf( float a[], BoneKeyFrameList **ppKeyFrameList, int n )
{
	int i, j;
	float t;
	BoneKeyFrameList *tFrame;

	for(i = 0; i < n - 1; i++)
		for(j = n - 1; j > i; j--)
			if(a[j - 1] > a[j]) {
				t = a[j];
				a[j] = a[j - 1];
				a[j - 1] = t;

				tFrame = ppKeyFrameList[j];
				ppKeyFrameList[j] = ppKeyFrameList[j - 1];
				ppKeyFrameList[j - 1] = tFrame;
			}
}

void BubbleSortf( float a[], FaceKeyFrameList **ppKeyFrameList, int n )
{
	int i, j;
	float t;
	FaceKeyFrameList *tFrame;

	for(i = 0; i < n - 1; i++)
		for(j = n - 1; j > i; j--)
			if(a[j - 1] > a[j]) {
				t = a[j];
				a[j] = a[j - 1];
				a[j - 1] = t;

				tFrame = ppKeyFrameList[j];
				ppKeyFrameList[j] = ppKeyFrameList[j - 1];
				ppKeyFrameList[j - 1] = tFrame;
			}
}