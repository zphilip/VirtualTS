//***********
// PMDボーン
//***********

#include	<gl/glut.h>
#include	<string.h>
#include	<math.h>
#include	"PMDBone.h"
#include	"../CBone.h"	// 追加


//================
// コンストラクタ
//================
cPMDBone::cPMDBone( void ) : m_pParentBone( NULL ), m_pChildBone( NULL )
{
	fkBone = NULL;	// 追加
}

//==============
// デストラクタ
//==============
cPMDBone::~cPMDBone( void )
{
}

//========
// 初期化
//========
void cPMDBone::initialize( const PMD_Bone *pPMDBoneData, const cPMDBone pBoneArray[] )
{
	// ボーン名のコピー
	strncpy( m_szName, pPMDBoneData->szName, 20 );
	m_szName[20] = '\0';

	// 位置のコピー
	m_vec3OrgPosition = pPMDBoneData->vec3Position;

	// 親ボーンの設定
	if( pPMDBoneData->nParentNo != -1 )
	{
		m_pParentBone = &(pBoneArray[pPMDBoneData->nParentNo]);
		Vector3Sub( &m_vec3Offset, &m_vec3OrgPosition, &m_pParentBone->m_vec3OrgPosition );
	}
	else
	{
		// 親なし
		m_vec3Offset = m_vec3OrgPosition;
	}

	// 子ボーンの設定
	if( pPMDBoneData->nChildNo != -1 )
	{
		m_pChildBone = (cPMDBone *)&(pBoneArray[pPMDBoneData->nChildNo]);
	}

	MatrixIdentity(	m_matInvTransform );
	m_matInvTransform[3][0] = -m_vec3OrgPosition.x; 
	m_matInvTransform[3][1] = -m_vec3OrgPosition.y; 
	m_matInvTransform[3][2] = -m_vec3OrgPosition.z; 

	m_bIKLimitAngle = false;

	// 各変数の初期値を設定
	reset();
}

//============================================================================
// 親のボーン番号のほうが大きい場合におかしくなるので初期化終了後に再計算する
//============================================================================
void cPMDBone::recalcOffset( void )
{
	if( m_pParentBone )	Vector3Sub( &m_vec3Offset, &m_vec3OrgPosition, &m_pParentBone->m_vec3OrgPosition );
}

//======================
// 各変数の初期値を設定
//======================
void cPMDBone::reset( void )
{
	m_vec3Position.x = m_vec3Position.y = m_vec3Position.z = 0.0f;
	m_vec4Rotate.x = m_vec4Rotate.y = m_vec4Rotate.z = 0.0f; m_vec4Rotate.w = 1.0f;

	MatrixIdentity(	m_matLocal );
	m_matLocal[3][0] = m_vec3OrgPosition.x; 
	m_matLocal[3][1] = m_vec3OrgPosition.y; 
	m_matLocal[3][2] = m_vec3OrgPosition.z; 
}

//====================
// ボーンの行列を更新
//====================
void cPMDBone::updateMatrix( void )
{
	// クォータニオンと移動値からボーンのローカルマトリックスを作成
	QuaternionToMatrix( m_matLocal, &m_vec4Rotate );
	m_matLocal[3][0] = m_vec3Position.x + m_vec3Offset.x;
	m_matLocal[3][1] = m_vec3Position.y + m_vec3Offset.y;
	m_matLocal[3][2] = m_vec3Position.z + m_vec3Offset.z;

	if( fkBone )
	{
		for( int i = 0; i < 4; i++ )
		{
			fkBone->matrix[i * 4 + 0] = m_matLocal[i][0];
			fkBone->matrix[i * 4 + 1] = m_matLocal[i][1];
			fkBone->matrix[i * 4 + 2] = m_matLocal[i][2];
			fkBone->matrix[i * 4 + 3] = m_matLocal[i][3];
			fkBone->uniMatrix = false;
		}
	}

	// 親があるなら親の回転を受け継ぐ
	if( m_pParentBone )	MatrixMultiply( m_matLocal, m_matLocal, m_pParentBone->m_matLocal );

/*	if( fkBone )
	{
		for( int i = 0; i < 4; i++ )
		{
			fkBone->gMatrix[i * 4 + 0] = m_matLocal[0][i];
			fkBone->gMatrix[i * 4 + 1] = m_matLocal[1][i];
			fkBone->gMatrix[i * 4 + 2] = m_matLocal[2][i];
			fkBone->gMatrix[i * 4 + 3] = m_matLocal[3][i];
		}
	}*/
}

#define		RAD(a)	(a * (3.1415926f / 180.0f))
#define		DEG(a)	(a * (180.0f / 3.1415926f))

//==========================
// ボーンを指定座標へ向ける
//==========================
void cPMDBone::lookAt( const Vector3 *pvecTargetPos )
{
	// どうもおかしいので要調整

	Matrix	matTemp;

	MatrixIdentity(	matTemp );
	matTemp[3][0] = m_vec3Position.x + m_vec3Offset.x; 
	matTemp[3][1] = m_vec3Position.y + m_vec3Offset.y; 
	matTemp[3][2] = m_vec3Position.z + m_vec3Offset.z;

	if( m_pParentBone )
	{
		Matrix	matInvTemp;
		MatrixInverse( matInvTemp, m_pParentBone->m_matLocal );
		matInvTemp[3][0] =  m_pParentBone->m_matLocal[3][0]; 
		matInvTemp[3][1] =  m_pParentBone->m_matLocal[3][1]; 
		matInvTemp[3][2] = -m_pParentBone->m_matLocal[3][2];
		MatrixMultiply( matTemp, matTemp, matInvTemp );
	}
	MatrixInverse( matTemp, matTemp );

	Vector3		vec3LocalTgtPosZY;
	Vector3		vec3LocalTgtPosXZ;

	Vector3Transform( &vec3LocalTgtPosZY, pvecTargetPos, matTemp );

	vec3LocalTgtPosXZ = vec3LocalTgtPosZY;
	vec3LocalTgtPosXZ.y = 0.0f;
	Vector3Normalize( &vec3LocalTgtPosXZ, &vec3LocalTgtPosXZ );

	vec3LocalTgtPosZY.x = 0.0f;
	Vector3Normalize( &vec3LocalTgtPosZY, &vec3LocalTgtPosZY );

	Vector3		vec3Angle = { 0.0f, 0.0f, 0.0f };

	vec3Angle.x =  asinf( vec3LocalTgtPosZY.y );
	if( vec3LocalTgtPosXZ.x < 0.0f )	vec3Angle.y =  acosf( vec3LocalTgtPosXZ.z );
	else								vec3Angle.y = -acosf( vec3LocalTgtPosXZ.z );

	if( vec3Angle.x < RAD(-25.0f) )	vec3Angle.x = RAD(-25.0f);
	if( RAD(45.0f) < vec3Angle.x  )	vec3Angle.x = RAD( 45.0f);

	if( vec3Angle.y < RAD(-80.0f) )	vec3Angle.y = RAD(-80.0f);
	if( RAD(80.0f) < vec3Angle.y  )	vec3Angle.y = RAD( 80.0f);

	QuaternionCreateEuler( &m_vec4Rotate, &vec3Angle );
}

//========================
// スキニング用行列を更新
//========================
void cPMDBone::updateSkinningMat( void )
{
	MatrixMultiply( m_matSkinning, m_matInvTransform, m_matLocal );
}


// 追加
void MatrixMultiply2( Matrix matOut, const Matrix matSrc1, const double matSrc2[16] )
{
	Matrix	matTemp;
	int		i;
	
	for( i = 0 ; i < 4 ; i++ )
	{
		matTemp[i][0] =	matSrc1[i][0] * matSrc2[ 0] + matSrc1[i][1] * matSrc2[ 4] + matSrc1[i][2] * matSrc2[ 8] + matSrc1[i][3] * matSrc2[12];
		matTemp[i][1] =	matSrc1[i][0] * matSrc2[ 1] + matSrc1[i][1] * matSrc2[ 5] + matSrc1[i][2] * matSrc2[ 9] + matSrc1[i][3] * matSrc2[13];
		matTemp[i][2] =	matSrc1[i][0] * matSrc2[ 2] + matSrc1[i][1] * matSrc2[ 6] + matSrc1[i][2] * matSrc2[10] + matSrc1[i][3] * matSrc2[14];
		matTemp[i][3] =	matSrc1[i][0] * matSrc2[ 3] + matSrc1[i][1] * matSrc2[ 7] + matSrc1[i][2] * matSrc2[11] + matSrc1[i][3] * matSrc2[15];
	}

	for( i = 0 ; i < 4 ; i++ )
	{
		matOut[i][0] = matTemp[i][0];
		matOut[i][1] = matTemp[i][1];
		matOut[i][2] = matTemp[i][2];
		matOut[i][3] = matTemp[i][3];
	}
}

// 追加
void cPMDBone::updateSkinningMat2( void )
{
	if(fkBone)
		MatrixMultiply2( m_matSkinning, m_matInvTransform, fkBone->gMatrix );
}

//==============
// デバッグ描画
//==============
void cPMDBone::debugDraw( void )
{
	glDisable( GL_TEXTURE_2D );

	glPushMatrix();

		glMultMatrixf( (const float *)m_matLocal );

		glColor4f( 1.0f, 0.0f, 1.0f, 1.0f );

		glutSolidCube( 0.3f );

	glPopMatrix();

	glPushMatrix();

		glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

		if( m_pParentBone )
		{
			glBegin( GL_LINES );
				glVertex3f( m_pParentBone->m_matLocal[3][0], m_pParentBone->m_matLocal[3][1], m_pParentBone->m_matLocal[3][2] );
				glVertex3f(                m_matLocal[3][0],                m_matLocal[3][1],                m_matLocal[3][2] );
			glEnd();
		}

	glPopMatrix();
}
