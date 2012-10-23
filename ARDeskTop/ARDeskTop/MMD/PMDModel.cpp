//***********
// PMDモデル
//***********

#include	<stdio.h>
#include	<stdlib.h>
#include	<malloc.h>
#include	<string.h>
#include	<gl/glut.h>
#include	"PMDModel.h"
#include	"TextureList.h"

#define	_DBG_BONE_DRAW		(0)
#define	_DBG_IK_DRAW		(0)
#define	_DBG_TEXTURE_DRAW	(0)



//--------------------------
// IKデータソート用比較関数
//--------------------------
static int compareFunc( const void *pA, const void *pB )
{
	return (int)(((cPMDIK *)pA)->getSortVal() - ((cPMDIK *)pB)->getSortVal());
}

//================
// コンストラクタ
//================
cPMDModel::cPMDModel( void ) : m_pvec3OrgPositionArray( NULL ), m_pvec3OrgNormalArray( NULL ), m_puvOrgTexureUVArray( NULL ),
									m_pOrgSkinInfoArray( NULL ), m_pvec3PositionArray( NULL ), m_pvec3NormalArray( NULL ), m_pIndices( NULL ),
										m_pMaterials( NULL ), m_pBoneArray( NULL ), m_pNeckBone( NULL), m_pIKArray( NULL ), m_pFaceArray( NULL )
{
}

//==============
// デストラクタ
//==============
cPMDModel::~cPMDModel( void )
{
	release();
}

//====================
// ファイルの読み込み
//====================
//ちょっと修正
bool cPMDModel::load( const char *szFilePath, const char *szDirFilePath )
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

	// モデルデータ初期化
	bool	bRet = initialize( pData, szDirFilePath );

	free( pData );

	return bRet;
}

//======================
// モデルデータの初期化
//======================
//ちょっと修正
bool cPMDModel::initialize( unsigned char *pData, const char *szDirFilePath )
{
	release();
	m_clMotionPlayer.clear();

	// ヘッダのチェック
	PMD_Header	*pPMDHeader = (PMD_Header *)pData;
	if( pPMDHeader->szMagic[0] != 'P' || pPMDHeader->szMagic[1] != 'm' || pPMDHeader->szMagic[2] != 'd' || pPMDHeader->fVersion != 1.0f )
		return false;	// ファイル形式が違う

	// モデル名のコピー
	strncpy( m_szModelName, pPMDHeader->szName, 20 );
	m_szModelName[20] = '\0';

	pData += sizeof( PMD_Header );

	unsigned long	ulSize;

	//-----------------------------------------------------
	// 頂点数取得
	m_ulNumVertices = *((unsigned long *)pData);
	pData += sizeof( unsigned long );

	// 頂点配列をコピー
	m_pvec3OrgPositionArray =  (Vector3 *)malloc( sizeof(Vector3)  * m_ulNumVertices );
	m_pvec3OrgNormalArray   =  (Vector3 *)malloc( sizeof(Vector3)  * m_ulNumVertices );
	m_puvOrgTexureUVArray   =    (TexUV *)malloc( sizeof(TexUV)    * m_ulNumVertices );
	m_pOrgSkinInfoArray     = (SkinInfo *)malloc( sizeof(SkinInfo) * m_ulNumVertices ); 

	PMD_Vertex	*pVertex = (PMD_Vertex *)pData;
	for( unsigned long i = 0 ; i < m_ulNumVertices ; i++, pVertex++ )
	{
		m_pvec3OrgPositionArray[i] =  pVertex->vec3Pos;
		m_pvec3OrgNormalArray[i]   =  pVertex->vec3Normal;
		m_puvOrgTexureUVArray[i]   =  pVertex->uvTex;

		m_pOrgSkinInfoArray[i].fWeight     = pVertex->cbWeight / 100.0f; 
		m_pOrgSkinInfoArray[i].unBoneNo[0] = pVertex->unBoneNo[0]; 
		m_pOrgSkinInfoArray[i].unBoneNo[1] = pVertex->unBoneNo[1]; 
	}

	ulSize = sizeof( PMD_Vertex ) * m_ulNumVertices;
	pData += ulSize;

	// スキニング用頂点配列作成
	m_pvec3PositionArray = (Vector3 *)malloc( sizeof(Vector3) * m_ulNumVertices );
	m_pvec3NormalArray   = (Vector3 *)malloc( sizeof(Vector3) * m_ulNumVertices );

	//-----------------------------------------------------
	// 頂点インデックス数取得
	m_ulNumIndices = *((unsigned long *)pData);
	pData += sizeof( unsigned long );

	// 頂点インデックス配列をコピー
	ulSize = sizeof( unsigned short ) * m_ulNumIndices;
	m_pIndices = (unsigned short *)malloc( ulSize );

	memcpy( m_pIndices, pData, ulSize );
	pData += ulSize;

	//-----------------------------------------------------
	// マテリアル数取得
	m_ulNumMaterials = *((unsigned long *)pData);
	pData += sizeof( unsigned long );

	// マテリアル配列をコピー
	m_pMaterials = (Material *)malloc( sizeof(Material) * m_ulNumMaterials );

	PMD_Material	*pPMDMaterial = (PMD_Material *)pData;
	for( unsigned long i = 0 ; i < m_ulNumMaterials ; i++ )
	{
		m_pMaterials[i].col4Diffuse = pPMDMaterial->col4Diffuse;

		m_pMaterials[i].col4Specular.r = pPMDMaterial->col3Specular.r;
		m_pMaterials[i].col4Specular.g = pPMDMaterial->col3Specular.g;
		m_pMaterials[i].col4Specular.b = pPMDMaterial->col3Specular.b;
		m_pMaterials[i].col4Specular.a = 1.0f;

		m_pMaterials[i].col4Ambient.r = pPMDMaterial->col3Ambient.r;
		m_pMaterials[i].col4Ambient.g = pPMDMaterial->col3Ambient.g;
		m_pMaterials[i].col4Ambient.b = pPMDMaterial->col3Ambient.b;
		m_pMaterials[i].col4Ambient.a = 1.0f;

		m_pMaterials[i].fShininess = pPMDMaterial->fShininess;
		m_pMaterials[i].ulNumIndices = pPMDMaterial->ulNumIndices;

		if( pPMDMaterial->szTextureFileName[0] )
			m_pMaterials[i].uiTexID = g_clsTextureList.getTexture( szDirFilePath, pPMDMaterial->szTextureFileName );
		else
			m_pMaterials[i].uiTexID = 0xFFFFFFFF;

		pPMDMaterial++;
	}

	pData += sizeof(PMD_Material) * m_ulNumMaterials;

	//-----------------------------------------------------
	// ボーン数取得
	m_unNumBones = *((unsigned short *)pData);
	pData += sizeof( unsigned short );

	// ボーン配列を作成
	m_pBoneArray = new cPMDBone[m_unNumBones];

	for( unsigned short i = 0 ; i < m_unNumBones ; i++ )
	{
		m_pBoneArray[i].initialize( (const PMD_Bone *)pData, m_pBoneArray );

		// 首のボーンを保存
		if( strncmp( m_pBoneArray[i].getName(), "頭", 20 ) == 0 )	m_pNeckBone = &m_pBoneArray[i];

		pData += sizeof( PMD_Bone );	
	}
	for( unsigned short i = 0 ; i < m_unNumBones ; i++ )	m_pBoneArray[i].recalcOffset();

	//-----------------------------------------------------
	// IK数取得
	m_unNumIK = *((unsigned short *)pData);
	pData += sizeof( unsigned short );

	// IK配列を作成
	if( m_unNumIK )
	{
		m_pIKArray = new cPMDIK[m_unNumIK];

		for( unsigned short i = 0 ; i < m_unNumIK ; i++ )
		{
			m_pIKArray[i].initialize( (const PMD_IK *)pData, m_pBoneArray );
			pData += sizeof( PMD_IK ) + sizeof(unsigned short) * (((PMD_IK *)pData)->cbNumLink - 1);
		}
		qsort( m_pIKArray, m_unNumIK, sizeof(cPMDIK), compareFunc );
	}

	//-----------------------------------------------------
	// 表情数取得
	m_unNumFaces = *((unsigned short *)pData);
	pData += sizeof( unsigned short );

	// 表情配列を作成
	if( m_unNumFaces )
	{
		m_pFaceArray = new cPMDFace[m_unNumFaces];

		for( unsigned short i = 0 ; i < m_unNumFaces ; i++ )
		{
			m_pFaceArray[i].initialize( (const PMD_FACE *)pData, &m_pFaceArray[0] );
			pData += sizeof( PMD_FACE ) + sizeof(PMD_FACE_VTX) * (((PMD_FACE *)pData)->ulNumVertices - 1);
		}
	}

	m_bLookAt = false;

	return true;
}

//====================
// モーションのセット
//====================
void cPMDModel::setMotion( cVMDMotion *pVMDMotion, bool bLoop )
{
	m_clMotionPlayer.setup( this, pVMDMotion, bLoop );
}

//==============================
// モーションによるボーンの更新
//==============================
void cPMDModel::updateMotion( float fFrame )
{
	// モーション更新前に表情をリセット
	if( m_pFaceArray )	m_pFaceArray[0].setFace( m_pvec3OrgPositionArray );

	// モーション更新
	m_clMotionPlayer.update( fFrame );

	// ボーン行列の更新
	for( unsigned short i = 0 ; i < m_unNumBones ; i++ )
	{
		m_pBoneArray[i].updateMatrix();
	}

	// IKの更新
	for( unsigned short i = 0 ; i < m_unNumIK ; i++ )
	{
		m_pIKArray[i].update();
	}
}

//================================
// 首のボーンをターゲットに向ける
//================================
void cPMDModel::updateNeckBone( const Vector3 *pvec3LookTarget )
{
	if( m_pNeckBone && m_bLookAt )
	{
		m_pNeckBone->lookAt( pvec3LookTarget );

		unsigned short i;
		for( i = 0 ; i < m_unNumBones ; i++ )
		{
			if( m_pNeckBone == &m_pBoneArray[i] )	break;
		}

		for( ; i < m_unNumBones ; i++ )
		{
			m_pBoneArray[i].updateMatrix();
		}
	}
}

//====================
// 頂点スキニング処理
//====================
void cPMDModel::updateSkinning( void )
{
	// スキニング用行列の更新
	for( unsigned short i = 0 ; i < m_unNumBones ; i++ )
	{
		m_pBoneArray[i].updateSkinningMat();
	}

	// 頂点スキニング
	Matrix	matTemp;
	for( unsigned long i = 0 ; i < m_ulNumVertices ; i++ )
	{
		if( m_pOrgSkinInfoArray[i].fWeight == 0.0f )
		{
			Vector3Transform( &m_pvec3PositionArray[i], &m_pvec3OrgPositionArray[i], m_pBoneArray[m_pOrgSkinInfoArray[i].unBoneNo[1]].m_matSkinning );
			Vector3Rotate( &m_pvec3NormalArray[i], &m_pvec3OrgNormalArray[i], m_pBoneArray[m_pOrgSkinInfoArray[i].unBoneNo[1]].m_matSkinning );
		}
		else if( m_pOrgSkinInfoArray[i].fWeight >= 0.9999f )
		{
			Vector3Transform( &m_pvec3PositionArray[i], &m_pvec3OrgPositionArray[i], m_pBoneArray[m_pOrgSkinInfoArray[i].unBoneNo[0]].m_matSkinning );
			Vector3Rotate( &m_pvec3NormalArray[i], &m_pvec3OrgNormalArray[i], m_pBoneArray[m_pOrgSkinInfoArray[i].unBoneNo[0]].m_matSkinning );
		}
		else
		{
			MatrixLerp( matTemp,	m_pBoneArray[m_pOrgSkinInfoArray[i].unBoneNo[0]].m_matSkinning,
									m_pBoneArray[m_pOrgSkinInfoArray[i].unBoneNo[1]].m_matSkinning,		m_pOrgSkinInfoArray[i].fWeight );

			Vector3Transform( &m_pvec3PositionArray[i], &m_pvec3OrgPositionArray[i], matTemp );
			Vector3Rotate( &m_pvec3NormalArray[i], &m_pvec3OrgNormalArray[i], matTemp );
		}
	}
}

// 追加
void cPMDModel::updateSkinning2( void )
{
	// スキニング用行列の更新
	for( unsigned short i = 0 ; i < m_unNumBones ; i++ )
	{
		m_pBoneArray[i].updateSkinningMat2();
	}

	// 頂点スキニング
	Matrix	matTemp;
	for( unsigned long i = 0 ; i < m_ulNumVertices ; i++ )
	{
		if( m_pOrgSkinInfoArray[i].fWeight == 0.0f )
		{
			Vector3Transform( &m_pvec3PositionArray[i], &m_pvec3OrgPositionArray[i], m_pBoneArray[m_pOrgSkinInfoArray[i].unBoneNo[1]].m_matSkinning );
			Vector3Rotate( &m_pvec3NormalArray[i], &m_pvec3OrgNormalArray[i], m_pBoneArray[m_pOrgSkinInfoArray[i].unBoneNo[1]].m_matSkinning );
		}
		else if( m_pOrgSkinInfoArray[i].fWeight >= 0.9999f )
		{
			Vector3Transform( &m_pvec3PositionArray[i], &m_pvec3OrgPositionArray[i], m_pBoneArray[m_pOrgSkinInfoArray[i].unBoneNo[0]].m_matSkinning );
			Vector3Rotate( &m_pvec3NormalArray[i], &m_pvec3OrgNormalArray[i], m_pBoneArray[m_pOrgSkinInfoArray[i].unBoneNo[0]].m_matSkinning );
		}
		else
		{
			MatrixLerp( matTemp,	m_pBoneArray[m_pOrgSkinInfoArray[i].unBoneNo[0]].m_matSkinning,
									m_pBoneArray[m_pOrgSkinInfoArray[i].unBoneNo[1]].m_matSkinning,		m_pOrgSkinInfoArray[i].fWeight );

			Vector3Transform( &m_pvec3PositionArray[i], &m_pvec3OrgPositionArray[i], matTemp );
			Vector3Rotate( &m_pvec3NormalArray[i], &m_pvec3OrgNormalArray[i], matTemp );
		}
	}
}

//====================
// モデルデータの描画
//====================
void cPMDModel::render( void )
{
	if( !m_pvec3OrgPositionArray )	return;

#if (1)
	unsigned short	*pIndices = m_pIndices;

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	// 頂点座標、法線、テクスチャ座標の各配列をセット
	glVertexPointer( 3, GL_FLOAT, 0, m_pvec3PositionArray );
	glNormalPointer( GL_FLOAT, 0, m_pvec3NormalArray );
	glTexCoordPointer( 2, GL_FLOAT, 0, m_puvOrgTexureUVArray );

	for( unsigned long i = 0 ; i < m_ulNumMaterials ; i++ )
	{
		// マテリアル設定
		glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,  (float *)&(m_pMaterials[i].col4Diffuse)  );
		glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT,  (float *)&(m_pMaterials[i].col4Ambient)  );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, (float *)&(m_pMaterials[i].col4Specular) );
		glMaterialf(  GL_FRONT_AND_BACK, GL_SHININESS, m_pMaterials[i].fShininess );

		if( m_pMaterials[i].col4Diffuse.a < 1.0f )	glDisable( GL_CULL_FACE );
		else										glEnable( GL_CULL_FACE );

		if( m_pMaterials[i].uiTexID != 0xFFFFFFFF )
		{
			// テクスチャありならBindする
			glEnable( GL_TEXTURE_2D );
			glBindTexture( GL_TEXTURE_2D, m_pMaterials[i].uiTexID );
		}
		else
		{
			// テクスチャなし
			glDisable( GL_TEXTURE_2D );
		}	

		// 頂点インデックスを指定してポリゴン描画
		glDrawElements( GL_TRIANGLES, m_pMaterials[i].ulNumIndices, GL_UNSIGNED_SHORT, pIndices );

		pIndices += m_pMaterials[i].ulNumIndices;
	}

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
#endif

#if	_DBG_BONE_DRAW
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );

	for( unsigned short i = 0 ; i < m_unNumBones ; i++ )
	{
		m_pBoneArray[i].debugDraw();
	}

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
#endif

#if	_DBG_IK_DRAW
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );

	for( unsigned short i = 0 ; i < m_unNumIK ; i++ )
	{
		m_pIKArray[i].debugDraw();
	}

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_LIGHTING );
#endif

#if	_DBG_TEXTURE_DRAW
	g_clsTextureList.debugDraw();
#endif
}

//====================
// モデルデータの解放
//====================
void cPMDModel::release( void )
{
	if( m_pvec3OrgPositionArray )
	{
		free( m_pvec3OrgPositionArray );
		m_pvec3OrgPositionArray = NULL;
	}

	if( m_pvec3OrgNormalArray )
	{
		free( m_pvec3OrgNormalArray );
		m_pvec3OrgNormalArray = NULL;
	}

	if( m_puvOrgTexureUVArray )
	{
		free( m_puvOrgTexureUVArray );
		m_puvOrgTexureUVArray = NULL;
	}

	if( m_pOrgSkinInfoArray )
	{
		free( m_pOrgSkinInfoArray );
		m_pOrgSkinInfoArray = NULL;
	}

	if( m_pvec3PositionArray )
	{
		free( m_pvec3PositionArray );
		m_pvec3PositionArray = NULL;
	}

	if( m_pvec3NormalArray )
	{
		free( m_pvec3NormalArray );
		m_pvec3NormalArray = NULL;
	}

	if( m_pIndices )
	{
		free( m_pIndices );
		m_pIndices = NULL;
	}

	if( m_pMaterials )
	{
		for( unsigned long i = 0 ; i < m_ulNumMaterials ; i++ )
		{
			if( m_pMaterials[i].uiTexID != 0xFFFFFFFF )
				g_clsTextureList.releaseTexture( m_pMaterials[i].uiTexID );
		}

		free( m_pMaterials );
		m_pMaterials = NULL;
	}

	if( m_pBoneArray )
	{
		delete [] m_pBoneArray;
		m_pBoneArray = NULL;
		m_pNeckBone = NULL;
	}

	if( m_pIKArray )
	{
		delete [] m_pIKArray;
		m_pIKArray = NULL;
	}

	if( m_pFaceArray )
	{
		delete [] m_pFaceArray;
		m_pFaceArray = NULL;
	}
}



//追加
void cPMDModel::getBB( Vector3 &min, Vector3 &max )
{
	if( !m_pvec3PositionArray || m_ulNumVertices == 0 ) return;

	min = m_pvec3PositionArray[0];
	max = m_pvec3PositionArray[0];
	for( unsigned long i = 1 ; i < m_ulNumVertices ; i++ )
	{
		if( m_pvec3PositionArray[i].x < min.x) min.x = m_pvec3PositionArray[i].x;
		if( m_pvec3PositionArray[i].y < min.y) min.y = m_pvec3PositionArray[i].y;
		if( m_pvec3PositionArray[i].z < min.z) min.z = m_pvec3PositionArray[i].z;

		if( m_pvec3PositionArray[i].x > max.x) max.x = m_pvec3PositionArray[i].x;
		if( m_pvec3PositionArray[i].y > max.y) max.y = m_pvec3PositionArray[i].y;
		if( m_pvec3PositionArray[i].z > max.z) max.z = m_pvec3PositionArray[i].z;
	}
}

