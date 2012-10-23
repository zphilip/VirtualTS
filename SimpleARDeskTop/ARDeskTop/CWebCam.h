#ifndef ___CWebCam
#define ___CWebCam


#include <AR/video.h>
#include <AR/gsub_lite.h>

#define VIEW_SCALEFACTOR		1.0
#define VIEW_DISTANCE_MIN		0.1			//0.1mm?
#define VIEW_DISTANCE_MAX		10000.0		//10m?


class CWebCam
{
public :
		//	int			apiContextIndex;	// API-specific index into an array of display contexts.
			ARParam		ARTCparam;			// Camera parameter.
			AR2VideoParamT*
						ARTVideo;			// Video parameters
			ARUint8*	ARTImage;			// Most recent image.
			int			ARTThreshhold;		// Threshold for marker detection.
			long		callCountMarkerDetect;	// Frames received.
			bool		pattFound;			// Whether marker transformation is valid.
			bool		dispImage;

			double		projectionMat[16];
public :
						CWebCam(void);
	virtual				~CWebCam();

	virtual	int			SetupWebCam(const char* cparam_names, char* vconfs);
	virtual	void		Quit(void);
	virtual	void		Display(ARGL_CONTEXT_SETTINGS_REF arglSettings);
	virtual	void		Reshape(int currentWidth, int currentHeight) {}
	virtual	bool		DetectMarker(void) = 0;
	virtual	void		Draw(void) = 0;
			void		SetZeroImage(void) { ARTImage = 0; }
};


#endif


