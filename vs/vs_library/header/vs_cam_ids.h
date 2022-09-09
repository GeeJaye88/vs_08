////////////////////////////////////////////////////////////////////////////////

// ---------- vs_cam_ids.h ----------
/*!
\file vs_cam_ids.h
\brief Interface for the CamIds class.
\author Gareth Edwards
\note 2017
*/

#pragma once


// ---- preprocessor include
#include "vs_cam_define.h"


// ---- IDS API include
#ifdef VS_CAM_IDS_VIDEO_SDK
#include <uEye.h>
#else
#define HIDS DWORD
#endif


// ---- system include
#include "../../vs_system/header/vs_app_dependency.h"


// ---- application include
#include "../../vs_application/header/vs_app_setup.h"


// ---- library include
#include "vs_cam_props.h"
#include "vs_img_buffer.h"
#include "vs_img_jpeglib.h"
#include "vs_img_process.h"
#include "vs_obj_config.h"


// ---------- namespace
namespace vs_system
{


// ---------- CamIds class
class CamIds
{

public:
	 
	// ---- cdtor
		CamIds();
		virtual ~CamIds();


	// ---- app framework
		INT Setup(vs_system::AppDependency *);
		INT Cleanup();


	// ---- framework
		INT Process();
		INT Start();
		INT Stop();


	// ---- get
		INT GetDimensions(INT*, INT*);
		INT GetExposure(FLOAT *);
		INT GetExposureMin(FLOAT *);
		INT GetExposureMax(FLOAT *);
		INT GetExposureRange(FLOAT []);
		INT GetFPS(DOUBLE *);
		INT GetGain(FLOAT []);
		INT GetHOffset(INT *);
		INT GetTimeStamp(INT [], const INT);


	// ---- set
		INT SetCamProps(vs_system::CamProps *);
		INT SetExposure();
		INT SetHOffset();
		INT SetSerialNumbers(CHAR *, CHAR *, CHAR *);
		INT SetMode(UINT);
		INT SetUseIdsInitialisation(BOOL);
		INT SetUseCamProps(BOOL);


	// ---- callback
		static INT CallbackUpdateBlackLevel();
		static INT CallbackUpdateColour();
		static INT CallbackUpdateExposure();
		static INT CallbackUpdateFramerate();
		static INT CallbackUpdateGainColour();
		static INT CallbackUpdateGainMaster();
		static INT CallbackUpdateGamma();
		static INT CallbackUpdateRop();
		static INT CallbackUpdateSetup();
		static INT CallbackWriteIniFile();


	// ---- callback
		INT SetCallbackOnCaptureImage(
				INT(*callback_on_capture_image)(WORD, WORD, WORD, BYTE *, INT [])
			);

private:

	// ---- private implementation
		class  PI_CamIds;
		static PI_CamIds *pi_cam_ids;


}; // class


} // namespace

