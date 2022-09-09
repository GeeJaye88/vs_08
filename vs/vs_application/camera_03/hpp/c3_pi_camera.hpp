////////////////////////////////////////////////////////////////////////////////

// ---------- c3_pi_camera.hpp ----------
/*!
\file c3_pi_props.hpp
\brief Implementation of the private p_props class
\author Gareth Edwards
*/


#ifndef CAMERA_03_APP_CAMERA
#define CAMERA_03_APP_CAMERA


////////////////////////////////////////////////////////////////////////////////


// ---- system include ----
#include "../../../vs_system/header/vs_inc_defs.h"
#include "../../../vs_system/header/vs_app_dependency.h"


// ---- vs app include ----
#include "../../review_02/header/r2_model.h"


// ---- vs library include ----
#include "../../../vs_library/header/vs_cam_define.h"
#include "../../../vs_library/header/vs_cam_ids.h"
#include "../../../vs_library/header/vs_cam_pic.h"


////////////////////////////////////////////////////////////////////////////////


// ---------- Implementation of PI_Camera class ----------
class PI_Camera
{

public:

	// ---- cdtor ----
		PI_Camera();
		~PI_Camera();


	// ---- methods ----


		// ---- app framework
			INT Setup();
			INT Cleanup();

		// ---- actions
			INT Start();
			INT Process();
			INT Stop();

		// ---- get
			INT GetDimensions(INT *, INT *);
			INT GetExposure(FLOAT *);
			INT GetExposureMin(FLOAT *);
			INT GetExposureMax(FLOAT *);
			INT GetGain(FLOAT *);


		// ---- hOffset
			INT GetHOffset();
			INT SetHOffset();

		// ---- Set
			INT SetExposure();

		// ---- callback on capture image
			INT(*callback_on_capture_image)(WORD, WORD, WORD, BYTE *, INT[]);


	// ---- properties ----


		// ---- application dependency pointer
			vs_system::AppDependency *app_dependency;
			vs_system::SysOutput     *sys_output;

		// ---- MVC object pointers
			review_02::Model         *model;

		// ---- managed by model
			vs_system::CamProps      *cam_props;

		// ---- managed by PI_Camera
			vs_system::CamIds        *cam_ids;
			vs_system::CamPic        *cam_pic;

		// ---- camera
			UINT   camera_mode;
			BOOL   camera_use_ini_file;
			BOOL   camera_use_hdl_file;

		// ---- files - only used in jply Setup
			CHAR   cam_props_config_paf[VS_MAXCHARPATHLEN];

} static *pi_camera;


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
PI_Camera::PI_Camera()
{

	// ---- application dependency pointer
		app_dependency = NULL;
		sys_output     = NULL;

	// ---- MVC object pointers
		model          = NULL;

	// ---- managed
		cam_ids        = NULL;
		cam_pic        = NULL;

	// ---- camera
		camera_mode = 1;
		camera_use_ini_file = FALSE;
		camera_use_hdl_file = FALSE;

		#ifdef VS_CAM_IDS_VIDEO_SDK
		camera_use_ini_file = FALSE;
		camera_use_hdl_file = TRUE;
		#endif

	// ---- files
		strcpy(cam_props_config_paf, "");

}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
PI_Camera::~PI_Camera()
{
	;
}


// ---------- Setup ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
int PI_Camera::Setup()
{

	// ---- local ----
		INT result = 0;


	// ---- get camera properties
		result = model->GetCamProps(&cam_props);
	

	#ifdef VS_CAM_IDS_VIDEO_SDK


	// ---- allocate and construct
		cam_ids = new vs_system::CamIds();


	// ---- set camera properties
		cam_ids->SetCamProps(cam_props);


	// ---- set serial numbers
		cam_ids->SetSerialNumbers(
				VS_LICENSE_SN_CLIENT,
				VS_LICENSE_SN_DEV_01,
				VS_LICENSE_SN_DEV_02
			);


	// ---- set callbacks
		cam_props->SetBlackLevelCallback(cam_ids->CallbackUpdateBlackLevel);
		cam_props->SetColourCallback(cam_ids->CallbackUpdateColour);
		cam_props->SetExposureCallback(cam_ids->CallbackUpdateExposure);
		cam_props->SetFramerateCallback(cam_ids->CallbackUpdateFramerate);
		cam_props->SetGainColourCallback(cam_ids->CallbackUpdateGainColour);
		cam_props->SetGainMasterCallback(cam_ids->CallbackUpdateGainMaster);
		cam_props->SetGammaCallback(cam_ids->CallbackUpdateGamma);
		cam_props->SetRopCallback(cam_ids->CallbackUpdateRop);
		cam_props->SetSetupCallback(cam_ids->CallbackUpdateSetup);
		cam_props->SetWriteIniFileCallback(cam_ids->CallbackWriteIniFile);


	// ---- set camera housekeeping stuff
		cam_ids->SetMode(camera_mode);
		cam_ids->SetUseIdsInitialisation(camera_use_ini_file);
		cam_ids->SetUseCamProps(camera_use_hdl_file);
		cam_ids->SetCallbackOnCaptureImage(callback_on_capture_image);


	// ---- initialise camera hardware, its display and defaults
		result = cam_ids->Setup(app_dependency);
		if (result != VS_OK)
		{
			return result;
		}


	// ---- stop camera
		result = cam_ids->Stop();


	#endif


	#ifdef VS_CAM_JPLY_DVP_SDK

		;

	#endif


	#ifdef VS_CAM_PIC_BUFFER

	// ---- allocate, construct and setup
		cam_pic = new vs_system::CamPic();


	// ---- set camera properties
		cam_pic->SetCamProps(cam_props);


	// ---- set callbacks
		cam_props->SetBlackLevelCallback(cam_pic->CallbackUpdateBlackLevel);
		cam_props->SetColourCallback(cam_pic->CallbackUpdateColour);
		cam_props->SetExposureCallback(cam_pic->CallbackUpdateExposure);
		cam_props->SetFramerateCallback(cam_pic->CallbackUpdateFramerate);
		cam_props->SetGainColourCallback(cam_pic->CallbackUpdateGainColour);
		cam_props->SetGainMasterCallback(cam_pic->CallbackUpdateGainMaster);
		cam_props->SetGammaCallback(cam_pic->CallbackUpdateGamma);
		cam_props->SetRopCallback(cam_pic->CallbackUpdateRop);
		cam_props->SetSetupCallback(cam_pic->CallbackUpdateSetup);
		cam_props->SetWriteIniFileCallback(cam_pic->CallbackWriteIniFile);


	// ---- set housekeeping stuff
		cam_pic->SetCallbackOnCaptureImage(callback_on_capture_image);


	// ---- initialise
		result = cam_pic->Setup(app_dependency);


	// ---- stop camera
		cam_pic->Stop();

	#endif

	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT PI_Camera::Cleanup()
{

	INT result = VS_OK;

	#ifdef VS_CAM_IDS_VIDEO_SDK
	result = cam_ids->Cleanup();
	if ( cam_ids != NULL ) { delete cam_ids; cam_ids = NULL; }
	#endif

	#ifdef VS_CAM_JPLY_DVP_SDK
	;
	#endif

	#ifdef VS_CAM_PIC_BUFFER
	result = cam_pic->Cleanup();
	if ( cam_pic != NULL ) { delete cam_pic; cam_pic = NULL; }
	#endif

	return result;
}


// ---------- Start ----------
/*!
\brief Start
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT PI_Camera::Start()
{

	INT result = VS_OK;

	#ifdef VS_CAM_IDS_VIDEO_SDK
	result = cam_ids->Start();
	#endif

	#ifdef VS_CAM_JPLY_DVP_SDK
	;
	#endif

	#ifdef VS_CAM_PIC_BUFFER
	result = cam_pic->Start();
	#endif

	return result;
}


// ---------- Process ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT PI_Camera::Process()
{

	INT result = VS_OK;

	#ifdef VS_CAM_IDS_VIDEO_SDK
	result = cam_ids->Process(); 
	#endif

	#ifdef VS_CAM_PIC_BUFFER
	result = cam_pic->Process();
	#endif

	return result;
}


// ---------- Stop ----------
/*!
\brief Start
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
int PI_Camera::Stop()
{

	INT result = VS_OK;

	#ifdef VS_CAM_IDS_VIDEO_SDK
	result = cam_ids->Stop();
	#endif

	#ifdef VS_CAM_JPLY_DVP_SDK
	;
	#endif

	#ifdef VS_CAM_PIC_BUFFER
	result = cam_pic->Stop();
	#endif

	return result;
}


// ---------- GetAoi ----------
/*!
\brief Get AOI
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT PI_Camera::GetDimensions(INT *width, INT *height)
{

	INT result = VS_OK;

	#ifdef VS_CAM_IDS_VIDEO_SDK
	result = cam_ids->GetDimensions(width, height);
	return VS_OK;
	#endif

	#ifdef VS_CAM_JPLY_DVP_SDK
	return VS_ERROR;
	#endif

	#ifdef VS_CAM_PIC_BUFFER
	return VS_ERROR;
	#endif

	return VS_ERROR;
}


// ---------- GetExposure ----------
/*!
\brief Get exposure in ms
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT PI_Camera::GetExposure(FLOAT *exposure)
{

	INT result = VS_OK;

	#ifdef VS_CAM_IDS_VIDEO_SDK
	result = cam_ids->GetExposure(exposure);
	return VS_OK;
	#endif

	#ifdef VS_CAM_JPLY_DVP_SDK
	return VS_ERROR;
	#endif

	#ifdef VS_CAM_PIC_BUFFER
	exposure[0] = 75;
	return VS_ERROR;
	#endif

	return VS_ERROR;
}


// ---------- GetExposureRangeMin ----------
/*!
\brief Get exposure minimum
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT PI_Camera::GetExposureMin(FLOAT *min)
{

	INT result = VS_OK;

	#ifdef VS_CAM_IDS_VIDEO_SDK
	result = cam_ids->GetExposureMin(min);
	return VS_OK;
	#endif

	#ifdef VS_CAM_JPLY_DVP_SDK
	return VS_ERROR;
	#endif

	#ifdef VS_CAM_PIC_BUFFER
	*min = 0;
	return VS_ERROR;
	#endif

	return VS_ERROR;
}


// ---------- GetExposureRangeMax ----------
/*!
\brief Get exposure maximum
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT PI_Camera::GetExposureMax(FLOAT *max)
{

	INT result = VS_OK;

	#ifdef VS_CAM_IDS_VIDEO_SDK
	result = cam_ids->GetExposureMax(max);
	return VS_OK;
	#endif

	#ifdef VS_CAM_JPLY_DVP_SDK
	return VS_ERROR;
	#endif

	#ifdef VS_CAM_PIC_BUFFER
	*max = 150;
	return VS_ERROR;
	#endif

	return VS_ERROR;
}


// ---------- GetGain ----------
/*!
\brief Get Master and RGB gain
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT PI_Camera::GetGain(FLOAT *gain)
{

	INT result = VS_OK;

	#ifdef VS_CAM_IDS_VIDEO_SDK
	result = cam_ids->GetGain(gain);
	return VS_OK;
	#endif

	#ifdef VS_CAM_JPLY_DVP_SDK
	return VS_ERROR;
	#endif

	#ifdef VS_CAM_PIC_BUFFER
	gain[0] = 75;
	gain[1] = 25;
	gain[2] = 35;
	gain[3] = 25;
	return VS_ERROR;
	#endif

	return VS_ERROR;
}


// ---------- GetHOffset ----------
/*!
\brief Update camera horizontal offset
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT PI_Camera::GetHOffset()
{
	INT result = VS_OK;
	INT h_offset = 0;

	#ifdef VS_CAM_IDS_VIDEO_SDK
	result = cam_ids->GetHOffset(&h_offset);
	return h_offset;
	#endif

	#ifdef VS_CAM_JPLY_DVP_SDK
	result = 0;
	#endif

	#ifdef VS_CAM_PIC_BUFFER
	result = 0;
	#endif

	return result;
}


// ---------- SetHOffset ----------
/*!
\brief Set camera horizontal offset
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT PI_Camera::SetHOffset()
{
	INT result = VS_OK;

	#ifdef VS_CAM_IDS_VIDEO_SDK
	result = cam_ids->SetHOffset();
	#endif

	#ifdef VS_CAM_JPLY_DVP_SDK
	result = 0;
	#endif

	#ifdef VS_CAM_PIC_BUFFER
	result = 0;
	#endif

	return result;
}


// ---------- SetExposure ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

Call tree:

	INT application::SwitchCameraLoopHardStart()
	INT application::SwitchCameraMode()
	INT application::SetupGraphics()

		----> INT application::StartLiveCamera()

			----> INT PI_Camera::SetExposure()

				----> INT CamIds::SetExposure()

	INT application::Display()

		----> INT application::UpdateCameraExposure()

			----> INT PI_Camera::SetExposure()

				----> INT CamIds::SetExposure()

*/
INT PI_Camera::SetExposure()
{
	INT result = VS_OK;

	#ifdef VS_CAM_IDS_VIDEO_SDK
	result = cam_ids->SetExposure();
	#endif

	return result;
}


#endif CAMERA_03_APP_CAMERA
