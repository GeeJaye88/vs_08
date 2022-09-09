////////////////////////////////////////////////////////////////////////////////

// ---------- r2_application.cpp ----------
/*!
\file r2_application.cpp
\brief Implementation of the application class
\author Gareth Edwards
*/


// ---- include ----
#include "../header/r2_application.h"


// ---- pimps
#include "../hpp_pi/r2_pi_panophoto.hpp"
#include "../hpp_pi/r2_pi_props.hpp"


// ---- namespace ----
using namespace review_02;
 

////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::application()
{

	// ---- create private properties
		pi_props = new PI_Props();


	// ---- initialise
		app_view       = NULL;
		control        = NULL;
		model          = NULL;
		img_buffer     = NULL;
		img_timeline   = NULL;

		#ifdef USE_IMG_ARKIVE
		img_arkive     = NULL;
		#else
		img_archive    = NULL;
		#endif

		#if VS_LIDAR
		lidar_ouster   = NULL;
		csv_buffer     = NULL;
		#endif

}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
application::~application()
{

	// ---- delete private properties
		if ( pi_props != NULL ) { delete pi_props; pi_props = NULL; }


	// ---- delete allocated objects
		if ( app_view     != NULL ) { delete app_view;     app_view    = NULL; }
		if ( control      != NULL ) { delete control;      control     = NULL; }
		if ( model        != NULL ) { delete model;        model       = NULL; }
		if ( img_buffer   != NULL ) { delete img_buffer;   img_buffer  = NULL; }
		if ( img_timeline != NULL ) { delete img_timeline; app_view    = NULL; }

		#ifdef USE_IMG_ARKIVE
		if ( img_arkive   != NULL ) { delete img_arkive;  img_arkive = NULL;   }
		#else
		if ( img_archive  != NULL )  { delete img_archive; img_archive = NULL;   }
		#endif

		#if VS_LIDAR
		if ( lidar_ouster != NULL ) { delete lidar_ouster; lidar_ouster = NULL; }
		if ( csv_buffer   != NULL ) { delete csv_buffer;   csv_buffer   = NULL; }
		#endif

}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT application::Setup(vs_system::AppDependency *app_dependency)
{

	// ---- local
		INT result = 0;
	

	// ---- initialise application dependency pointer
		pi_props->app_dependency = app_dependency;


	// ---- initialise dependency system pointers
		pi_props->sys_context = pi_props->app_dependency->GetSysContext();
		pi_props->sys_output = pi_props->app_dependency->GetSysOutput();


	// ---- license - not required
		pi_props->sys_context->SetBool(vs_system::SysContext::APP_LICENSED, FALSE);


	// ---- MVC
		app_view = new vs_system::AppView();
			app_view->Setup(pi_props->app_dependency);
		control  = new Control();
		model    = new Model();


	// ---- set pano mode flag
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
			BOOL pano_mode = FALSE;
		#else
			BOOL pano_mode = TRUE;
		#endif


	// ---- application and CV required config filenames -----
	

		// ---- path
			vs_system::SysContext *sys_context = app_dependency->GetSysContext();
			strcpy(
					pi_props->app_setup_config_paf,
					sys_context->GetChar(vs_system::SysContext::APP_FOLDER_PATH)
				);
			CHAR config_name[VS_MAXCHARNAME];
			strcpy(
					config_name,
					sys_context->GetChar(vs_system::SysContext::CONFIG_NAME)
				);
			strcat(pi_props->app_setup_config_paf, "data/");
			strcat(pi_props->app_setup_config_paf, config_name);
			strcat(pi_props->app_setup_config_paf, "/app_config.hdl");


		// ---- application: setup and configure private properties
			result = pi_props->Setup(pi_props->app_setup_config_paf);


	// ---- MC: Setup and configure
		result = control->Setup(pi_props->app_dependency, app_view, model);
		result = model->Setup(pi_props->app_dependency);


	// ---- camera "aoi" - don't display
		vs_system::CamProps *cam_props;
		model->GetCamProps(&cam_props);
		result = cam_props->SetProperty
				(
					vs_system::CamProps::AOI,
					0,
					vs_system::CamProps::DISPLAY,
					0
				);


	// ---- select "home" frame

		// Note: The PTF panoramic panel index is initialised
		// in model->Setup()->SetupButtons(). Based on this
		// this method also sets the appropriate frame button
		// status, button set and button icon texture.


		// ---- get PTF panoramic panel index
			UINT ptf_panel_id = 0;
			model->GetObjConfig()->GetPanoPTFId(&ptf_panel_id);


		// ---- set ptf frame index 
			UINT ptf_frame_index = ptf_panel_id == 1 ? 1 : 2;


		// ---- LOOP for each frame
			vs_system::AppFrame *frame = app_view->GetFirstFrame();
			UINT frame_index = 1;
			while (frame)
			{
				frame->SetHome( frame_index == ptf_frame_index ? TRUE : FALSE );
				if ( frame->GetHome() ) app_view->SelectFrame(frame);
				frame_index++;
				frame = frame->GetNext();
			}


	// ---- new - stuff
		img_timeline = new vs_system::ImgTimeline();
		img_buffer   = new vs_system::ImgBuffer();

		#ifdef USE_IMG_ARKIVE
		img_arkive   = new vs_system::ImgArkive();
		#else
		img_archive  = new vs_system::ImgArchive();
		#endif

		#if VS_LIDAR
		lidar_ouster = new vs_system::LidarOuster();
		csv_buffer   = new vs_system::CsvObject();
		#endif



	// ----- AT RUNTIIME LOAD DEFAULT ARCHIVE -----
		//
		// -> Get run time default archive path
		// -> Store archive path
		// -> Create new timeline
		// -> Start archive updating to detect "live" archive
		//
		CHAR archive_paf[VS_MAXCHARLEN];

		result = GetRunTimeArchivePath(archive_paf, VS_MAXCHARLEN);

		INT create_timeline_result = VS_ERROR;
		if ( result == VS_OK )
		{
			strcpy(pi_props->archive_paf, archive_paf);
			create_timeline_result = CreateTimeline(archive_paf);
		}


	// ----- arkive - read application config file
		{
			#ifdef USE_IMG_ARKIVE
			result = img_arkive->SetMaxStack(10);
			result = img_arkive->ReadSetup(pi_props->app_setup_config_paf);
			#else
			vs_system::SysOutput *sys_output = pi_props->app_dependency->GetSysOutput();
			result = img_archive->SetSysOutput(sys_output);
			result = img_archive->SetMaxStack(10);
			result = img_archive->Configure(pi_props->app_setup_config_paf);
			#endif
		}


	// ---- lidar
		#if VS_LIDAR

		// ---- lidar - init
			result = lidar_ouster->SetBeams(64);
			result = lidar_ouster->SetColumns(1024);
			result = model->SetLidarOuster(lidar_ouster);

		// ---- csv object - init (derived from lidar data)
			result = csv_buffer->Initialise(lidar_ouster->GetBeams() * lidar_ouster->GetColumns(), 5);
			result = csv_buffer->CreateBuffer();
			result = csv_buffer->SetRowsPerRecord(lidar_ouster->GetBeams());
			result = model->SetCsvObject(csv_buffer);

		// ---- model - update pano lidar csv parameters
			result = model->Update_PanoLidar(csv_buffer);

		// ---- arkive - update csv parametrs
			result = img_arkive->Setup_Csv(
					csv_buffer->GetRows(),
					csv_buffer->GetColumns(),
					csv_buffer->GetRowsPerRecord()
				);

		#endif


	// ----- image buffer - initialise quality
		result = img_buffer->SetQuality(85);


	// ----- set "NOW"
		pi_props->time_millisecond_now = timeGetTime();

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

	// ---- display
		INT setup_frames = model->SetupPanels(pi_props->app_dependency, app_view);


	// ---- graphics
		INT setup_graphics = model->SetupGraphics();


	// ---- done
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

	// ---- display timeline
		INT result = TimelineDisplay();


	// ---- input?
		if ( control->UpdateInput() >= VS_OK )
		{
			INT mouse_input = control->HandleMouseInput();
			INT keyboard_input = control->HandleKeyboardInput();
		}


	// ---- display current view
		model->DisplayBegin();
		app_view->Display((INT)this, &callbackDisplay);
		model->DisplayEnd();

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
	model->CleanupGraphics();
	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::Cleanup()
{
	control->Cleanup();
	model->Cleanup();
	app_view->Cleanup();
	return VS_OK;
}



// ---------- callbackDisplay ----------
/*!
\brief callback Model Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::callbackDisplay(
		INT handle_app_obj,
		vs_system::AppPanel *p
	)
{
	application* app = (application *)handle_app_obj;
	app->model->Display(p->GetPanelContext(), NULL);
	return VS_OK;
} 


////////////////////////////////////////////////////////////////////////////////


#include "../hpp_app/r2_app_archive_methods.hpp"
#include "../hpp_app/r2_app_archive_export.hpp"
#include "../hpp_app/r2_app_panophoto.hpp"
#include "../hpp_app/r2_app_timeline_display.hpp"
#include "../hpp_app/r2_app_timeline_methods.hpp"


////////////////////////////////////////////////////////////////////////////////

