//****************
// モーション再生
//****************

#ifndef		_MOTIONPLAYER_H_
#define		_MOTIONPLAYER_H_

#include	"VMDMotion.h"

class cPMDModel;

class cMotionPlayer
{
	private :
		cPMDModel		*m_pPMDModel;
		cVMDMotion		*m_pVMDMotion;

		cPMDBone		**m_ppBoneList;
		cPMDFace		**m_ppFaceList;

		float			m_fOldFrame,
						m_fFrame;
		bool			m_bLoop;		// モーションをループするかどうか


		cPMDBone *getBoneByName( const char *szBoneName, cPMDBone pBoneArray[], unsigned long ulNumBones );
		void getMotionPosRot( const MotionDataList *pMotionData, float fFrame, Vector3 *pvec3Pos, Vector4 *pvec4Rot );

		cPMDFace *getFaceByName( const char *szFaceName, cPMDFace pFaceArray[], unsigned long ulNumFaces );
		float getFaceRate( const FaceDataList *pFaceData, float fFrame );

	public :
		cMotionPlayer( void );
		~cMotionPlayer( void );

		void setup( cPMDModel *pPMDModel, cVMDMotion *pMotion, bool bLoop );

		bool update( float fFrame );

		void clear( void );

};

#endif	// _MOTIONPLAYER_H_
