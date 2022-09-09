////////////////////////////////////////////////////////////////////////////////

// ---------- c3_application.cpp ----------
/*!
\file c3_application.cpp
\brief Implementation of the c3_application class
\author Gareth Edwards
\note 2017
*/

////////////////////////////////////////////////////////////////////////////////


// ---- application
#include "../header/c3_variant.h"
#include "../header/c3_application.h"


// ---- namespace
using namespace camera_03;


// ---- props
#include "../hpp/c3_pi_props.hpp"
#include "../hpp/c3_pi_camera.hpp"


// ---- shared memory
#ifdef VS_CAMERA_03_SHARED_MEMORY
sm_handle shared_memory_handle = 0;
sm_config shared_memory_struct;
#endif



////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::application()
{
	pi_camera = new PI_Camera();
	pi_props = new PI_Props();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
application::~application()
{
	if ( pi_camera != NULL ) { delete pi_camera; pi_camera = NULL; }
	if ( pi_props  != NULL ) { delete pi_props;  pi_props  = NULL; }
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Framework ----------


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT application::Setup(vs_system::AppDependency *app_dep)
{

	// ---- local
		INT result = VS_OK;


	// ---- inject application dependency
		pi_props->app_dependency = app_dep;


	// ---- local
		vs_system::SysOutput *sys_output = pi_props->app_dependency->GetSysOutput();


	// ---- setup goodies
		result = SetupProperties();
			if (result != VS_OK) return result;

		result = SetupCamera();
			if (result != VS_OK) return result;

		result = SetupArchive();
			if (result != VS_OK) return result;
		
		result = SetupImgLutProcess();
			if (result != VS_OK) return result;

		result = SetupScanFolder();
			if (result != VS_OK) return result;


	// ---- report basic config file stuff, etc.
		result = ReportAppConfig(); 
			if (result != VS_OK) return result;

			 
	// ---- setup additional goodies
		result = SetupONVIF();
			if (result != VS_OK) return result;

		result = SetupGPS();
			if (result != VS_OK) return result;

		result = SetupIPC();
			if (result != VS_OK) return result;

		result = SetupStreaming();
			if (result != VS_OK) return result;

		result = SetupScavenger();
			if (result != VS_OK) return result;

	return VS_OK;
}


// ---------- SetupGraphics ----------
/*!
\brief Setup device
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SetupGraphics()
{

	// ---- setup
		INT setup_frames = pi_props->model->SetupPanels(
				pi_props->app_dependency,
				pi_props->app_view
			);
		INT setup_graphics = pi_props->model->SetupGraphics();


	// ---- long story - but can't be done in Setup
		INT setup_stream_rtsp_text = pi_props->model->SetStreamElementDisplay(STREAM_RTSP);


	// ---- force camera restart
		INT result = StartLiveCamera();


	// ---- force gps button status check
		pi_props->gps_mode = PI_Props::GPS_TEST_RUNNING;


	// ---- return state
		setup_graphics = TRUE;

	return setup_graphics == VS_OK && setup_frames == VS_OK ? VS_OK : VS_ERROR;
}


// ---------- Display ----------
/*!
\brief Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::Display()
{

	// ---- local
		INT result = 0;

	// ---- switch
		if ( pi_props->monitor_on )
		{
			result = SwitchCameraMode();
			result = SwitchCameraLoopHardStart();
		}
		result = SwitchDashboardOnly();
		result = SwitchArchiveSave();

	// ---- process
		#ifdef VS_CAMERA_03_GPS
		result = ProcessGpsReader();
		#endif
		result = pi_camera->Process();

	// ---- handle control events
		if ( pi_props->control->UpdateInput() >= VS_OK )
		{
			INT mouse_input    = pi_props->control->HandleMouseInput();
			INT keyboard_input = pi_props->control->HandleKeyboardInput();
		}

	// ---- handle auto ptz
		pi_props->model->UpdateAutoPTF();

	// ---- handle ptf panel over events
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
		pi_props->model->Update_ImgPanel_OverviewMap();
		#endif

	// ---- display 
		result = pi_props->model->DisplayBegin();
		result = pi_props->app_view->Display((INT)this, &CallbackDisplayFromView);
		result = pi_props->model->DisplayEnd();

	// ---- update...  
		result = UpdateSecondCount();
		result = UpdateCameraExposure();
		result = UpdateCameraHOffset();
		result = UpdateStreamIcon();

	return VS_OK;
}


// ---------- CleanupGraphics ----------
/*!
\brief Cleanup device
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::CleanupGraphics()
{
	// ---- local
		INT result = 0;


	// ---- stop live camera
		if ( pi_props->camera_live )
		{
			result = pi_camera->Stop();
			pi_props->sys_output->Message("Camera: Live Cleanup");
			pi_props->camera_started = FALSE;

		}


	// ---- cleanup
		result = pi_props->model->CleanupGraphics();

	return VS_OK;
}


// ---------- Cleanup------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::Cleanup()
{

	// ---- local
		INT result = 0;


	// ---- stop camera
		if ( pi_props->camera_live )
		{
			result = pi_camera->Stop();
			pi_props->sys_output->Message("Camera: Stop");
			pi_props->camera_started = FALSE;
		}
	

	// ---- cleanup
		pi_props->sys_output->Message("Cleanup: ");

		pi_props->sys_output->Message("   Camera");
		pi_camera->Cleanup();

		pi_props->sys_output->Message("   MVC");
			result = pi_props->control->Cleanup();
			result = pi_props->model->Cleanup();
			result = pi_props->app_view->Cleanup();

		pi_props->sys_output->Message("   ONVIF");
		result = CleanupONVIF();

		pi_props->sys_output->Message("   GPS");
		result = CleanupGPS();

		pi_props->sys_output->Message("   IPC");
		result = CleanupIPC();

		pi_props->sys_output->Message("   Shared Memory");
		result = CleanupSharedMemory();

		pi_props->sys_output->Message("   Streaming");
		result = CleanupStreaming();

		pi_props->sys_output->Message("   Scavenger");
		result = CleanupScavenger();

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---- framework gps utility
#include "../hpp/c3_app_gps_task_delay.hpp"


// ---- framework setup & cleanup
#include "../hpp/c3_app_sc_gps.hpp"
#include "../hpp/c3_app_sc_ipc.hpp"
#include "../hpp/c3_app_sc_onvif.hpp"
#include "../hpp/c3_app_sc_scavenger.hpp"
#include "../hpp/c3_app_sc_sharing.hpp"
#include "../hpp/c3_app_sc_streaming.hpp"


// ---- sub framework
#include "../hpp/c3_app_capture.hpp"
#include "../hpp/c3_app_display.hpp"
#include "../hpp/c3_app_gps.hpp"
#include "../hpp/c3_app_process.hpp"
#include "../hpp/c3_app_setup.hpp"
#include "../hpp/c3_app_start_live_cam.hpp"
#include "../hpp/c3_app_switch.hpp"
#include "../hpp/c3_app_update.hpp"


////////////////////////////////////////////////////////////////////////////////
