//***************************
// PMD���f���p�e��\���̒�`
//***************************

// ���̍\���̒�`�́A�i�N�A�_��(http://www.geocities.jp/hatsune_no_mirai/)���͂��߂Ƃ���
// �uMMD�̃��f���f�[�^�ɂ��Č��X���v(http://jbbs.livedoor.jp/bbs/read.cgi/music/23040/1219738115/)
// �ɏ������܂ꂽPMD�t�@�C���t�H�[�}�b�g��͏��ƁA�x����������(http://yumin3123.at.webry.info/)��
// VMDConverter�̃\�[�X�t�@�C�����Q�l�ɂ����Ă��������܂���

#ifndef	_PMDTYPES_H_
#define	_PMDTYPES_H_

#include	"VecMatQuat.h"

#pragma pack( push, 1 )

// �t�@�C���w�b�_
struct PMD_Header
{
	char	szMagic[3];		// "Pmd"
	float	fVersion;		// PMD�o�[�W�����ԍ�
	char	szName[20];		// ���f����
	char	szComment[256];	// �R�����g(���쌠�\���Ȃ�)
};

// ���_�f�[�^
struct PMD_Vertex
{
	Vector3		vec3Pos;	// ���W
	Vector3		vec3Normal;	// �@���x�N�g��
	TexUV		uvTex;		// �e�N�X�`�����W

	unsigned short	unBoneNo[2];	// �{�[���ԍ�
	unsigned char	cbWeight;		// �u�����h�̏d�� (0�`100��)
	unsigned char	cbEdge;			// �G�b�W�t���O
};

// �}�e���A���f�[�^
struct PMD_Material
{
	Color4		col4Diffuse;
	float		fShininess;
	Color3		col3Specular,
				col3Ambient;

	unsigned short	unknown;
	unsigned long	ulNumIndices;			// ���̍ގ��ɑΉ����钸�_��
	char			szTextureFileName[20];	// �e�N�X�`���t�@�C����
};

// �{�[���f�[�^
struct PMD_Bone
{
	char	szName[20];			// �{�[���� (0x00 �I�[�C�]���� 0xFD)
	short	nParentNo;			// �e�{�[���ԍ� (�Ȃ���� -1)
	short	nChildNo;			// �q�{�[���ԍ�

	unsigned char	cbKind;		// �{�[���̎��
	unsigned short	unIKTarget;	// IK���̃^�[�Q�b�g�{�[��

	Vector3		vec3Position;	// ���f�����_����̈ʒu
};
/*
kind
	0x00�@�ʏ�{�[��(��]�̂�)
	0x01�@�ʏ�{�[��(�ړ��\)
	0x02�@IK�{�[��(�ړ��\)
	0x03�@�����Ȃ�(�I����)
	0x04�@IK�{�[��(��]�̂�)
	0x05�@��]�Ǐ](��]�̂�)
	0x06�@IK�ڑ���
	0x07�@�����Ȃ�(�I��s��)
	0x08�@�Ђ˂�
	0x09�@��]�^��
*/

// IK�f�[�^
struct PMD_IK
{
	short			nTargetNo;	// IK�^�[�Q�b�g�{�[���ԍ�
	short			nEffNo;		// IK��[�{�[���ԍ�

	unsigned char	cbNumLink;	// IK���\������{�[���̐�

	unsigned short	unCount;
	float			fFact;

	unsigned short	punLinkNo[1];// IK���\������{�[���̔z��
};

// �\��_
struct PMD_FACE_VTX
{
	unsigned long	ulIndex;
	Vector3			vec3Pos;
};

// �\��f�[�^
struct PMD_FACE
{
	char			szName[20];		// �\� (0x00 �I�[�C�]���� 0xFD)

	unsigned long	ulNumVertices;	// �\��_��
	unsigned char	cbType;			// ����

	PMD_FACE_VTX	pVertices[1];	// �\��_�f�[�^
};

#pragma pack( pop )

#endif	// _PMDTYPES_H_
