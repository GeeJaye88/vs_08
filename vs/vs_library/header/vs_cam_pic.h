////////////////////////////////////////////////////////////////////////////////

// ---------- vs_cam_pic.h ----------
/*!
\file vs_cam_pic.h
\brief Interface for the CamPic class.
\author Gareth Edwards
\note 2017
*/

#pragma once


// ---- preprocessor include
#include "vs_cam_define.h"


// ---- system include
#include "../../vs_system/header/vs_app_dependency.h"


// ---- library include
#include "vs_cam_props.h"
#include "vs_img_buffer.h"
#include "vs_img_jpeglib.h"


// ---- namespace ----
namespace vs_system
{


// ---- CamPic class ----
class CamPic
{

public:
	 
	// ---- cdtor
		CamPic();
		virtual ~CamPic();


	// ---- app framework
		INT Setup(vs_system::AppDependency *);
		INT Cleanup();


	// ---- framework
		INT Process();
		INT Start();
		INT Stop();


	// ---- set
		INT SetCamProps(vs_system::CamProps *);


	// ---- set callback
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
				INT(*callback_on_capture_image)(WORD, WORD, WORD, BYTE *, INT[])
			);

private:

	// ---- private implementation
		class  PI_CamPic;
		static PI_CamPic *pi_cam_pic;

	// ---- dependency injection
		vs_system::AppDependency *app_dependency;


}; // class


} // namespace

