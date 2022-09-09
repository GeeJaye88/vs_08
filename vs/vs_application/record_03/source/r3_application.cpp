////////////////////////////////////////////////////////////////////////////////

// ---------- r3_application.cpp ----------
/*!
\file r3_application.cpp
\brief Implementation of the r3_application class
\author Gareth Edwards
\note Vanilla C++
*/


////////////////////////////////////////////////////////////////////////////////


#ifdef APPLICATION_PI_PROPS
#endef APPLICATION_PI_PROPS
#else
#define APPLICATION_PI_PROPS
#endif

// ---------- Application include ----------
#ifdef APPLICATION_PI_PROPS
#include "../header/r3_application.h"
#endif

// ---------- Defintions for MS Shared Memory ----------
#ifdef APPLICATION_PI_PROPS
#include "../../../vs_project/shared_memory_04/header/vs_shared_memory.h"
#endif


////////////////////////////////////////////////////////////////////////////////


// ---------- namespace ----------
using namespace record_03;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Props class ----------
class application::PI_Props
{

public:

	// Constructor/destructor
	PI_Props();
	~PI_Props();

	// Methods
	INT Initialise();
	INT Setup();
	INT Configure();

	// Application dependency pointers
	vs_system::AppDependency *app_dependency;
	vs_system::SysContext    *sys_context;
	vs_system::SysOutput     *sys_output;

	// MVC object pointers
	vs_system::AppView       *app_view;
	record_03::Control       *control;
	static record_03::Model  *model;

	// Camera config objects pointers
	//
	// Managed by model
	//
	vs_system::CamModes      *cam_modes;
	vs_system::CamProps      *cam_props;

	// Camera object pointers
	vs_system::CamIds        *cam_ids;
	vs_system::CamJply       *cam_jply;
	vs_system::CamPic        *cam_pic;

	// Image archive object pointer
	vs_system::VS_ImageArchive *image_archive;

	// Counters
	ULONG  seconds_elapsed_recording;
	ULONG  seconds_elapsed_to_now;

	UINT   frames_per_second;
	UINT   images_counter;
	UINT   images_missed_count;
	UINT   images_duplicated_count;
	UINT   images_per_second;
	UINT   images_per_second_min;
	UINT   images_per_second_max;
	
	// Camera flags
	BOOL   camera_live;
	BOOL   camera_archive;

	// Camera luts
	CHAR   camera_ms_image_map[61];

	// ---- Set via "app_config.hdl" file ---

	// Config camera
	BOOL   camera_use_ini_file;
	BOOL   camera_use_config_hdl_file;

	// Config "Archive"
	CHAR   previous_filename[VS_MAXCHARPATHLEN];
	CHAR   archive_filename[VS_MAXCHARPATHLEN];
	CHAR   archive_folder[VS_MAXCHARPATHLEN];
	UINT   archive_mode;
	UINT   archive_extra_stack;
	UINT   archive_jpg_compression;

	// ---- Set using vs_system::SysContext ---

	// Files
	CHAR   app_setup_config_paf[VS_MAXCHARPATHLEN];
	CHAR   cam_modes_config_paf[VS_MAXCHARPATHLEN];
	CHAR   cam_props_config_paf[VS_MAXCHARPATHLEN];
	CHAR   obj_pano_config_paf[VS_MAXCHARPATHLEN];
	CHAR   ext_tex_map_image_paf[VS_MAXCHARPATHLEN];

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::PI_Props::PI_Props()
{
	// Initialise props
	Initialise();
}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::PI_Props::~PI_Props()
{
	// Delete MVC objects;
	if (app_view != NULL) delete app_view;
	if (control != NULL)  delete control;
	if (model != NULL)    delete model;

	// Delete camera objects
	if ( cam_ids != NULL )  delete cam_ids;
	if ( cam_jply != NULL ) delete cam_jply;
	if ( cam_pic != NULL )  delete cam_pic;

	// Delete image archive object 
	if ( image_archive != NULL ) delete image_archive;
}


// ---------- Initialise ----------
/*!
\brief Initialise
\author Gareth Edwards
*/
INT application::PI_Props::Initialise()
{
	// Application dependency pointers
	sys_context   = NULL;
	sys_output    = NULL;

	// MVC object pointers
	app_view      = NULL;
	control       = NULL;
	model         = NULL;

	// Camera config pointers
	cam_modes     = NULL;
	cam_props     = NULL;

	// Camera object pointers
	cam_ids       = NULL;
	cam_jply      = NULL;
	cam_pic       = NULL;

	// Image archive object pointer
	image_archive = NULL;

	// Counters
	seconds_elapsed_recording = 0;
	seconds_elapsed_to_now  = 0;
	frames_per_second       = 0;
	images_counter          = 0;
	images_missed_count     = 0;
	images_duplicated_count = 0;
	images_per_second       = 0;
	images_per_second_min   = 0;
	images_per_second_max   = 0;

	// Camera flags
	camera_live    = FALSE;
	camera_archive = FALSE;

	// Camera luts
	for (UINT i = 0; i<61; i++) camera_ms_image_map[i] = '_';
	camera_ms_image_map[60] = 0;

	// Camera config
	camera_use_ini_file        = FALSE;
	camera_use_config_hdl_file = TRUE;

#	ifdef VS_CAM_IDS_VIDEO_SDK
	camera_use_ini_file = FALSE;
	camera_use_config_hdl_file = TRUE;
#	endif

#	ifdef VS_CAM_JPLY_DVP_SDK
	camera_use_ini_file = FALSE;
	camera_use_config_hdl_file = TRUE;
#	endif

	// Config "Archive"
	strcpy(previous_filename, "");
	strcpy(archive_filename,  "");
	strcpy(archive_folder,    "");
	archive_mode            = vs_system::VS_ImageArchive::MODE;
	archive_extra_stack     = 0;
	archive_jpg_compression = vs_system::VS_ImageArchive::JPG_COMPRESSION;

	// Config path/file names
	strcpy(app_setup_config_paf,  "");
	strcpy(cam_modes_config_paf,  "");
	strcpy(cam_props_config_paf,  "");
	strcpy(obj_pano_config_paf,   "");
	strcpy(ext_tex_map_image_paf, "");

	// Done
	return VS_OK;
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
*/
INT application::PI_Props::Setup()
{
	// Local
	INT result = 0;

	// Done
	return VS_OK;
}


// ---------- Configure ----------
/*!
\brief Configure private implementation properties object
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT application::PI_Props::Configure()
{
	// Read application setup configuration file ( e.g. "app_config.hdl" )
	vs_system::VNodeIO vio;
	vs_system::VNode *config_hdl = vio.Read(app_setup_config_paf);

	// IF source file does NOT exist THEN
	if ( config_hdl == NULL )
	{
		sys_output->MessageError(
				"Could not READ application setup configuration file.",
				"File",
				app_setup_config_paf
			);
		return VS_ERROR;
	}

	// IF config data THEN
	if ((INT)config_hdl != VS_ERROR)
	{
		// Write application setup configuration file to log
		sys_output->WriteFile(app_setup_config_paf);

		// local
		vs_system::VNode *p = NULL;
		vs_system::VNode *c = NULL;

		// ========== IF "Camera" data THEN ==========
		if ( c = config_hdl->SearchName("Camera") )
		{
			// IF true THEN use [camera]/[camera]_config.ini NOT default camera setup
			if ( p = c->SearchName("Config File") ) camera_use_ini_file = p->GetBoolFromValue();

			// IF true THEN use settings in lighting cam_config.hdl
			if ( p = c->SearchName("Config Lighting") ) camera_use_config_hdl_file = p->GetBoolFromValue();
		}


		// ========== NO "Monitor" data ==========


		//  ========== IF "Archive" data THEN ==========
		if ( c = config_hdl->SearchName("Archive") )
		{
			// Set mode
			if ( p = c->SearchName("Mode") ) archive_mode = p->GetIntFromValue();

			// IF declared THEN set folder path
			if ( p = c->SearchName("Folder") )
			{
				strcpy(archive_folder, p->GetValue());
			}

			// Set jpeg compression (Effective range: 20 to 100)
			if ( p = c->SearchName("Compression") )
			{
				archive_jpg_compression = p->GetIntFromValue();
				archive_jpg_compression = archive_jpg_compression < 20 ?
					20 :
					(
						archive_jpg_compression > 100 ?
						100 :
						archive_jpg_compression
					);
			}

			// Stack size set to  permissable ( 2 * core ) + extra
			if ( p = c->SearchName("Extra Threads") )
			{
				archive_extra_stack = p->GetIntFromValue();
				archive_extra_stack = archive_extra_stack < 0 ? 0 : archive_extra_stack;
			}
		}


		// ========== NO "Stream" data ==========


		// ========== NO "ONVIF" data ==========

	}

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Camera class ----------
class application::PI_Camera
{

public:

	// Constructor/destructor
	PI_Camera();
	~PI_Camera();

	// Framework
	INT Setup();
	INT Cleanup();

	// Actions
	INT Start();
	INT Process();
	INT Stop();

	// Get
	INT GetExposure(FLOAT *);

	// HOffset
	INT GetHOffset();
	INT SetFPS(INT);
	INT SetHOffset(INT);

	// Time
	INT GetTimeStamp(INT *);

	// Update
	INT UpdateModeByGroup(INT);

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::PI_Camera::PI_Camera()
{
	;
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
application::PI_Camera::~PI_Camera()
{
	;
}


// ---------- Setup ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Camera::Setup(
		//PI_Props *pi_props
	)
{
	// Accelerate
	vs_system::AppDependency *app_dependency = pi_props->app_dependency;

	// Local
	INT result = 0;

#	ifdef VS_CAM_IDS_VIDEO_SDK

	// Allocate, construct and setup
	pi_props->cam_ids = new vs_system::CamIds();

	// Camera modes
	result = pi_props->model->GetCamModes(&pi_props->cam_modes);
	pi_props->cam_ids->SetCamModes(pi_props->cam_modes);

	// Camera properties
	result = pi_props->model->GetCamProps(&pi_props->cam_props);
	pi_props->cam_ids->SetCamProps(pi_props->cam_props);

	// Set camera serial number
	pi_props->cam_ids->SetSN(
			VS_LICENSE_SN_CLIENT,
			VS_LICENSE_SN_DEV_01,
			VS_LICENSE_SN_DEV_02
		);


	// Initialise
	pi_props->cam_ids->Initialise();
	pi_props->cam_ids->SetUseIdsIniFile(pi_props->camera_use_ini_file);
	pi_props->cam_ids->SetUseConfigHdlFile(pi_props->camera_use_config_hdl_file);
	pi_props->cam_ids->SetCallbackOnCaptureImage(CallbackOnCaptureImage);

	// Setup
	result = pi_props->cam_ids->Setup(app_dependency);
	if (result != VS_OK)
	{
		return VS_ERROR;
	}

	// Stop
	pi_props->cam_ids->Stop();

	// Get IDS specific camera setup filename AND then then write to log
	CHAR paf[VS_MAXCHARPATHLEN];
	pi_props->cam_ids->GetIdsSetupPAF(paf);
	pi_props->sys_output->WriteFile(paf);

	// Get IDS specific camera config filename AND then then write to log
	// Get note: already have this - see pi_props->cam_config
	pi_props->cam_ids->GetConfigPAF(paf);
	pi_props->sys_output->WriteFile(paf);

	// Set camera mode callbacks
	pi_props->cam_modes->SetLightCallback(pi_props->cam_ids->CallbackUpdateLight);
	pi_props->cam_modes->SetBrightCallback(pi_props->cam_ids->CallbackUpdateBright);
	pi_props->cam_modes->SetFrameRateCallback(pi_props->cam_ids->CallbackUpdateFrameRate);
	pi_props->cam_modes->SetWriteIniFileCallback(pi_props->cam_ids->CallbackWriteIniFile);

#	endif


#	ifdef VS_CAM_JPLY_DVP_SDK

	// Allocate, construct and setup
	pi_props->cam_jply = new vs_system::CamJply();
	result = pi_props->cam_jply->Setup(app_dependency);
	if (result != VS_OK)
	{
		return VS_ERROR;
	}

	// Initialise
	pi_props->cam_jply->Initialise(pi_props->cam_props_config_paf);
	pi_props->cam_jply->SetUseJplyIniFile(pi_props->camera_use_ini_file);
	pi_props->cam_jply->SetUseConfigHdlFile(pi_props->camera_use_config_hdl_file);
	pi_props->cam_jply->SetCallbackOnCaptureImage(CallbackOnCaptureImage);

	// Camera modes 
	result = pi_props->model->GetCamModes(&pi_props->cam_modes);
	pi_props->cam_jply->SetCamModes(pi_props->cam_modes);

	// Camera properties
	result = pi_props->model->GetCamProps(&pi_props->cam_props);
	pi_props->cam_jply->SetCamProps(pi_props->cam_props);

	// Get JPLY specific camera setup filename AND then then write to log
	CHAR paf[VS_MAXCHARPATHLEN];
	pi_props->cam_jply->GetJplySetupPAF(paf);
	pi_props->sys_output->WriteFile(paf);

	// Get JPLY specific camera config filename AND then then write to log
	// Get note: already have this - see pi_props->cam_config
	pi_props->cam_jply->GetConfigPAF(paf);
	pi_props->sys_output->WriteFile(paf);

	// Set camera mode callbacks
	pi_props->cam_modes->SetLightCallback(pi_props->cam_jply->CallbackUpdateLight);
	pi_props->cam_modes->SetBrightCallback(pi_props->cam_jply->CallbackUpdateBright);
	pi_props->cam_modes->SetFrameRateCallback(pi_props->cam_jply->CallbackUpdateFrameRate);

#	endif


#	ifdef VS_CAM_PIC_BUFFER

	// Allocate, construct and setup
	pi_props->cam_pic = new vs_system::CamPic();
	result = pi_props->cam_pic->Setup(app_dependency);

	// Initialise
	pi_props->cam_pic->Initialise();
	pi_props->cam_pic->SetCallbackOnCaptureImage(CallbackOnCaptureImage);
	pi_props->cam_pic->Stop();

	// Get camera cam_config.hdl
	result = pi_props->model->GetCamModes(&pi_props->cam_modes);

#	endif


	// Done
	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Camera::Cleanup()
{
	// Local
	INT result = VS_OK;

#	ifdef VS_CAM_IDS_VIDEO_SDK
	result = pi_props->cam_ids->Cleanup();
	if (pi_props->cam_ids != NULL)   delete pi_props->cam_ids;
#	endif

#	ifdef VS_CAM_JPLY_DVP_SDK
	result = pi_props->cam_jply->Cleanup();
	if (pi_props->cam_jply != NULL)  delete pi_props->cam_jply;
#	endif

#	ifdef VS_CAM_PIC_BUFFER
	result = pi_props->cam_pic->Cleanup();
	if (pi_props->cam_pic != NULL)   delete pi_props->cam_pic;
#	endif

	// Done
	return result;
}


// ---------- Start ----------
/*!
\brief Start
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Camera::Start()
{
	// Local
	INT result = VS_OK;

#	ifdef VS_CAM_IDS_VIDEO_SDK
	result = pi_props->cam_ids->Start();
#	endif

#	ifdef VS_CAM_JPLY_DVP_SDK
	result = pi_props->cam_jply->Start();
#	endif

#	ifdef VS_CAM_PIC_BUFFER
	result = pi_props->cam_pic->Start();
#	endif

	// Done
	return result;
}


// ---------- Process ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Camera::Process()
{
	// Local
	INT result = VS_OK;

#	ifdef VS_CAM_IDS_VIDEO_SDK
	result = pi_props->cam_ids->ProcessEvents();
#	endif

#	ifdef VS_CAM_PIC_BUFFER
	result = pi_props->cam_pic->ProcessEvents();
#	endif

	// Done
	return result;
}


// ---------- Stop ----------
/*!
\brief Start
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Camera::Stop()
{
	// Local
	INT result = VS_OK;

#	ifdef VS_CAM_IDS_VIDEO_SDK
	result = pi_props->cam_ids->Stop();
#	endif

#	ifdef VS_CAM_JPLY_DVP_SDK
	result = pi_props->cam_jply->Stop();
#	endif

#	ifdef VS_CAM_PIC_BUFFER
	result = pi_props->cam_pic->Stop();
#	endif

	// Done
	return result;
}


// ---------- GetHOffset ----------
/*!
\brief Update camera config
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Camera::GetHOffset()
{
	// Local
	INT result = VS_OK;
	INT h_offset = 0;

#	ifdef VS_CAM_IDS_VIDEO_SDK
	result = pi_props->cam_ids->GetHOffset(&h_offset);
	return h_offset;
#	endif

#	ifdef VS_CAM_JPLY_DVP_SDK
	result = pi_props->cam_jply->GetHOffset(&h_offset);
	return h_offset;
#	endif

#	ifdef VS_CAM_PIC_BUFFER
	;
#	endif

	// Done
	return 0;
}


// ---------- SetFPS ----------
/*!
\brief Set Frames Per Second
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Camera::SetFPS(INT fps)
{
	// Local
	INT result = VS_OK;

#	ifdef VS_CAM_IDS_VIDEO_SDK
	DOUBLE new_fps = pi_props->cam_ids->SetFPS((DOUBLE)fps);
#	endif

#	ifdef VS_CAM_JPLY_DVP_SDK
	;
#	endif

#	ifdef VS_CAM_PIC_BUFFER
	;
#	endif

	// Done
	return result;
}


// ---------- GetExposure ----------
/*!
\brief Get exposure in ms
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Camera::GetExposure(FLOAT *exposure)
{
	// Local
	INT result = VS_OK;

#	ifdef VS_CAM_IDS_VIDEO_SDK
	result = pi_props->cam_ids->GetExposure(exposure);
	return VS_OK;
#	endif

#	ifdef VS_CAM_JPLY_DVP_SDK
	return VS_ERROR;
#	endif

#	ifdef VS_CAM_PIC_BUFFER
	return VS_ERROR;
#	endif

	// Done
	return VS_ERROR;
}


// ---------- SetHOffset ----------
/*!
\brief Set camera horizontal offset
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Camera::SetHOffset(INT h_increment)
{
	// Local
	INT result = VS_OK;

#	ifdef VS_CAM_IDS_VIDEO_SDK
	result = pi_props->cam_ids->IncrementHOffset(h_increment);
#	endif

#	ifdef VS_CAM_JPLY_DVP_SDK
	result = pi_props->cam_jply->IncrementHOffset(h_increment);
#	endif

#	ifdef VS_CAM_PIC_BUFFER
	;
#	endif

	// Done
	return result;
}


// ---------- GetTimeStamp ----------
/*!
\brief Get current time stamp milliseconds
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Camera::GetTimeStamp(INT *timestamp)
{
	// Local
	INT result = VS_OK;

#	ifdef VS_CAM_IDS_VIDEO_SDK
	result = pi_props->cam_ids->GetTimeStamp(timestamp, 1);
#	endif

#	ifdef VS_CAM_JPLY_DVP_SDK
	result = VS_ERROR;
#	endif

#	ifdef VS_CAM_PIC_BUFFER
	result = VS_ERROR;
#	endif

	// Done
	return VS_OK;
}


// ---------- UpdateModeByGroup ----------
/*!
\brief Update camera mode by group id or ALL
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Camera::UpdateModeByGroup(INT group_id)
{
	// Local
	INT result = VS_OK;

#	ifdef VS_CAM_IDS_VIDEO_SDK
	result = pi_props->cam_modes->UpdateGroup(group_id);
#	endif

#	ifdef VS_CAM_JPLY_DVP_SDK
	result = pi_props->cam_modes->UpdateGroup(group_id);
#	endif

#	ifdef VS_CAM_PIC_BUFFER
	;
#	endif

	// Done
	return result;
}


////////////////////////////////////////////////////////////////////////////////


// Application static initialisation
application::PI_Camera * application::pi_camera = NULL;
application::PI_Props  * application::pi_props = NULL;


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::application()
{
	// Allocate and construct private implementations
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
	// Delete private implementations
	delete pi_camera;
	delete pi_props;
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
	// Local
	INT result = 0;

	// Store application dependency pointer
	pi_props->app_dependency = app_dep;

	// Set shortcut to an injected dependency property
	vs_system::GfxInterface *gfx = app_dep->GetGfxInterface();

	// ---------- Font structure ----------
	struct font_info
	{
		std::string name;
		std::string type;
		UINT height;
		UINT weight;
	};

	// ---------- Button names associated font list ----------
	font_info font_list[6] =
	{
		{ "CamModes", "Calibri",     36, 1 },
		{ "B",        "Courier New", 36, 1 },
		{ "C",        "Courier New", 18, 1 },
		{ "D",        "Courier New", 18, 1 },
		{ "About",    "Calibri",     18, 1 },
		{ "F",        "Courier New", 36, 1 }
	};

	// ---- Set managed FONTS ----
	INT total_fonts = sizeof(font_list) / sizeof(font_info);
	for (INT i = 0; i < total_fonts; i++)
	{
		INT font_ok = gfx->SetFont(
				i + 1,
				font_list[i].name,
				font_list[i].type,
				font_list[i].height,
				font_list[i].weight
			);
	}

	// SETUP sub-framework
	if ( result = SetupProperties()    == VS_ERROR ) return VS_ERROR;
	if ( result = SetupArchive()       == VS_ERROR ) return VS_ERROR;
	if ( result = SetupCamera()        == VS_ERROR ) return VS_ERROR;

	// Done
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
	// Local
	INT result = 0;

	// Display
	INT setup_frames = pi_props->model->SetupFrameset(pi_props->app_dependency, pi_props->app_view);

	// Graphics
	INT setup_graphics = pi_props->model->SetupGraphics();

	// Start live camera
	result = StartLiveCamera();

	// Done
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
	// Local
	INT result = 0;

	// Switch
	result = SwitchCameraLive();
	result = SwitchArchiveSave();

	// Process (pre-display) camera events (if any)
	result = ProcessCameraEvents();

	// Handle control events
	if ( pi_props->control->UpdateInput() >= VS_OK )
	{
		INT mouse_input    = pi_props->control->HandleMouseInput();
		INT keyboard_input = pi_props->control->HandleKeyboardInput();
	}

	// Display
	result = pi_props->model->DisplayBegin();
	result = pi_props->app_view->Display((INT)this, &CallbackDisplayFromView);
	result = pi_props->model->DisplayEnd();

	// Increment FPS
	pi_props->frames_per_second++;

	// Update (post display) camera config 
	result = UpdateCameraConfig();

	// Done
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
	// Stop live camera
	if ( pi_props->camera_live )
	{
		pi_camera->Stop();
		pi_props->sys_output->Message("Camera: Live Cleanup");
	}

	// Cleanup
	pi_props->model->CleanupGraphics();
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
	// Local
	INT result = 0;

	// Stop camera
	if ( pi_props->camera_live )
	{
		pi_camera->Stop();
		pi_props->sys_output->Message("Camera: Cleanup");
	}

	// Cleanup camera
	pi_camera->Cleanup();

	// Cleanup MVC
	result = pi_props->control->Cleanup();
	result = pi_props->model->Cleanup();
	result = pi_props->app_view->Cleanup();

	// Done
	return VS_OK;
}


// ---------- CreateSavePathFilename ----------
/*!
\brief CreateSavePathFilename
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT application::CreateSavePathFilename(
	std::string(&path)[6],
	std::string &filename,
	INT *timestamp
	)
{
	// Local
	INT result = VS_OK;

	// Now, set archive folder
	CHAR paf[IMAGE_ARCHIVE_PATHLEN];
	result = pi_props->image_archive->GetFolderPath(paf);

	// Create path stack "[archive name]/[year]/[month]/[day]/[hour]/[min]"
	char ps[IMAGE_ARCHIVE_PATHTOP][IMAGE_ARCHIVE_PATHLEN];
	sprintf(ps[1], "%4d", timestamp[0]);
	sprintf(ps[2], "%2d", timestamp[1]);
	sprintf(ps[3], "%2d", timestamp[2]);
	sprintf(ps[4], "%2d", timestamp[3]);
	sprintf(ps[5], "%2d", timestamp[4]);

	// Substitute ' ' for '0'
	path[0].assign(paf);
	for (WORD i = 1; i<6; i++)
	{
		for (WORD j = 0; j < IMAGE_ARCHIVE_PATHLEN; j++) ps[i][j] = ps[i][j] == ' ' ? '0' : ps[i][j];
		path[i].assign(ps[i]);
	}

	// Create image filename "year|month|day_hour|minute_millisec"
	char f[IMAGE_ARCHIVE_MAXCHAR];
	sprintf(
			f,
			"%4d%2d%2d_%2d%2d%2d_%3d.jpg",
			timestamp[0],
			timestamp[1],
			timestamp[2],
			timestamp[3],
			timestamp[4],
			timestamp[5],
			timestamp[6]
		);

	// Replace ' ' with '0'
	WORD len_filename = strlen(f);
	for (WORD i = 0; i<len_filename; i++)
		f[i] = f[i] == ' ' ? '0' : f[i];
	filename.assign(f);

	// Add sub archive to path
	path[0] += "/archive/";

	// Update image map
	INT i = (INT)((FLOAT)timestamp[6] / 16.66666f);
	pi_props->camera_ms_image_map[i] = 'X';

	// Copy
	strcpy(pi_props->previous_filename, pi_props->archive_filename);

	// Store
	strcpy(pi_props->archive_filename, filename.c_str());

	// Compare: IF same THEN return error
	if ( !strcmp(pi_props->archive_filename, pi_props->previous_filename) )
	{
		// Set 'D'uplicate
		pi_props->camera_ms_image_map[i] = 'D';

		// Increment # images missed
		pi_props->images_duplicated_count++;

		// Always skip saving this image - return error
		return VS_ERROR;
	}

	// Done
	return VS_OK;
}


// ---------- Report ----------
/*!
\brief Report image acquistion info
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT application::Report(INT *timestamp)
{
	// Local
	INT result = 0;

	// Increment IPS counter
	pi_props->images_counter++;

	// # seconds to wait before recording
	#define SECONDS_WAIT_TO_RECORD 1

	#ifdef VS_CAM_IDS_VIDEO_SDK
		#undef SECONDS_WAIT_TO_RECORD
		#define SECONDS_WAIT_TO_RECORD 1
		DOUBLE now_sec = (DOUBLE)timestamp[5];
	#endif
	
	#if defined(VS_CAM_JPLY_DVP_SDK) || defined(VS_CAM_PIC_BUFFER) 
		#undef SECONDS_WAIT_TO_RECORD
		#define SECONDS_WAIT_TO_RECORD 3
		DOUBLE now_ms = timeGetTime();
		ULONG now_sec = (LONG)(now_ms / 1000);
	#endif

	// IF total # of seconds changed THEN report
	if ( now_sec != pi_props->seconds_elapsed_to_now )
	{

		// IF zero recorded THEN re-initialise max ips
		if ( pi_props->seconds_elapsed_recording == 0 )
		{
			pi_props->images_missed_count     = 0;
			pi_props->images_duplicated_count = 0;
			pi_props->images_per_second_min   = 999;
			pi_props->images_per_second_max   = 0;
		}

		// IF NOT waiting before reporting THEN

		#ifdef VS_CAM_IDS_VIDEO_SDK
		if ( pi_props->seconds_elapsed_recording >= SECONDS_WAIT_TO_RECORD )
		#endif
		
	#if defined(VS_CAM_JPLY_DVP_SDK) || defined(VS_CAM_PIC_BUFFER) 
		if ( pi_props->seconds_elapsed_recording >= SECONDS_WAIT_TO_RECORD )
		#endif
		
		{
			// Get # of messages per second
			vs_system::SysContext* sys_context = pi_props->app_dependency->GetSysContext();
			DWORD euc = sys_context->GetDWord(sys_context->MSG_USER_EVENT_COUNT);
			sys_context->SetDWord(sys_context->MSG_USER_EVENT_COUNT, 0);

			// IF umc != ips THEN increment # images missed
			if (euc != pi_props->images_counter)
				pi_props->images_missed_count++;

			// IF archiving THEN report ips and last image
			CHAR msg[VS_MAXCHARLEN];
			if ( pi_props->camera_archive )
			{
				sprintf(
						msg,
						"Record %2dips : %s [ F:%d D:%d ]",
						(INT)pi_props->images_counter,
						pi_props->camera_ms_image_map,
						pi_props->images_missed_count,
						pi_props->images_duplicated_count
					);

				pi_props->sys_output->Message(msg);

				sprintf(
						msg,
						"Record: %2dips [ last image %s ] [ F:%d D:%d ]",
						(INT)pi_props->images_counter,
						pi_props->archive_filename,
						pi_props->images_missed_count,
						pi_props->images_duplicated_count
					);

				// Update report text elements
				result = pi_props->model->UpdateRecordText(msg);
				result = pi_props->model->UpdateFrameRateText(pi_props->images_counter);
			}
			else
			{
				sprintf(
						msg,
						"%2dips : %s [ F:%d D:%d ]",
						(INT)pi_props->images_counter,
						pi_props->camera_ms_image_map,
						pi_props->images_missed_count,
						pi_props->images_duplicated_count
					);
				pi_props->sys_output->Message(msg);

				// Update report text elements
				result = pi_props->model->UpdateRecordText("Record: STOP");
				result = pi_props->model->UpdateFrameRateText(pi_props->images_counter);
			}

			#ifdef VS_CAM_IDS_VIDEO_SDK
			
			// Update images per second minimax
			{
				// Update minimum # of images per second
				pi_props->images_per_second_min =
					pi_props->images_counter < pi_props->images_per_second_min ?
						pi_props->images_counter :
						pi_props->images_per_second_min;

				// Update maximum # of images per second
				pi_props->images_per_second_max =
					pi_props->images_counter > pi_props->images_per_second_max ?
						pi_props->images_counter :
						pi_props->images_per_second_max;
			}

			# endif
		}
		else
		{
			// Update report text elements
			result = pi_props->model->UpdateRecordText("Record: STOP");
			result = pi_props->model->UpdateFrameRateText(pi_props->images_counter);
		}

		// Update and re-initialise
		pi_props->images_per_second = pi_props->images_counter;
		pi_props->frames_per_second = 0;
		pi_props->images_counter    = 0;

		// Increment # seconds that have been recorded
		pi_props->seconds_elapsed_recording++;

		// Re-initialise image map
		for (UINT i = 0; i < 60; i++) pi_props->camera_ms_image_map[i] = '_';
		pi_props->camera_ms_image_map[60] = 0;
	}

	// Update total # of seconds
	pi_props->seconds_elapsed_to_now = (ULONG)now_sec;

	// Report
	/*
	CHAR msg[VS_MAXCHARLEN];
	sprintf(
			msg,
			"   %s",
			pi_props->archive_filename
		);
	pi_props->sys_output->Message(msg);
	*/

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Callback: Sub framework ----------


// ---------- CallbackDisplayFromView ----------
/*!
\brief Callback display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::CallbackDisplayFromView(
		INT app_cast_to_int,
		vs_system::AppPanel *panel
	)
{
	// Accelerate - system
	vs_system::GfxContext *gfx_context = panel->GetGfxContext();

	// Accelerate  - application
	application      *app   = (application *)app_cast_to_int;
	record_03::Model *model = app->pi_props->model;

	// Display
	app->pi_props->model->Display(gfx_context, NULL);

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Setup: sub framework from setup ----------


// ---------- SetupProperties ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SetupProperties()
{
	// Local
	INT result = 0;

	// Accelerate private dependency properties
	pi_props->sys_context = pi_props->app_dependency->GetSysContext();
	pi_props->sys_output = pi_props->app_dependency->GetSysOutput();


	// ----- MVC -----

	// Allocate, construct and setup application view object
	pi_props->app_view = new vs_system::AppView();
	result = pi_props->app_view->Setup(pi_props->app_dependency);

	// Allocate and construct other MVC objects
	pi_props->control = new record_03::Control();
	pi_props->model = new record_03::Model();


	// ----- Application and CV required config filenames -----

	// Create and setup config files
	record_03::ConfigFiles *config_files = NULL;
	result = pi_props->model->GetConfigFiles(&config_files);

	// Get app name
	CHAR camera_application_name[VS_MAXCHARNAME];
	result = config_files->GetCameraApplicationName(camera_application_name);

	// Setup config files
	result = config_files->Setup(pi_props->app_dependency, camera_application_name);
	if ( result == VS_ERROR ) return VS_ERROR;

	// Get config path filenames
	config_files->GetApplicationConfigHdlFilename (pi_props->app_setup_config_paf);
	config_files->GetCameraConfigHdlFilename      (pi_props->cam_modes_config_paf);
	config_files->GetPropsConfigHdlFilename       (pi_props->cam_props_config_paf);
	config_files->GetPanoramicConfigHdlFilename   (pi_props->obj_pano_config_paf);
	config_files->GetImageConfigJpgFilename       (pi_props->ext_tex_map_image_paf);


	// ----- Application: Setup and configure private properties -----
	result = pi_props->Setup();
	result = pi_props->Configure();

	// Read camera mode AND then then write to log
	result = pi_props->model->ReadCamModes(pi_props->cam_modes_config_paf);
	pi_props->sys_output->WriteFile(pi_props->cam_modes_config_paf);

	// Read camera properties AND then then write to log
	result = pi_props->model->ReadCamProps(pi_props->cam_props_config_paf);
	pi_props->sys_output->WriteFile(pi_props->cam_props_config_paf);

	// ONLY write pano paf to log - not required
	pi_props->sys_output->WriteFile(pi_props->obj_pano_config_paf);

	// ----- MC: Setup and configure -----
	result = pi_props->control->Setup(pi_props->app_dependency, pi_props->app_view, pi_props->model);
	result = pi_props->model->Setup(pi_props->app_dependency);

	// Initialise "now" in seconds

	#ifdef VS_CAM_IDS_VIDEO_SDK
	pi_props->seconds_elapsed_to_now = 0;
	#endif
	
	#ifdef	VS_CAM_JPLY_DVP_SDK
	pi_props->seconds_elapsed_to_now = (LONG)pi_props->sys_context->GetTimeNow() / 1000;
	#endif

	// Initialise # seconds that have been recorded
	pi_props->seconds_elapsed_recording = 0;

	// Done
	return VS_OK;
}


// ---------- SetupArchive ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SetupArchive()
{
	// Local
	INT result = 0;

	// Allocate and construct image archive
	pi_props->image_archive = new vs_system::VS_ImageArchive();

	// IF archive folder NOT NULL THEN use
	CHAR paf[VS_MAXCHARPATHLEN];
	if ( pi_props->archive_folder[0] != 0 )
	{
		sprintf(paf, "%s", pi_props->archive_folder);
	}
	// ELSE documents/observant folder
	else
	{
		sprintf(
			paf,
			"%sDocuments/Observant/",
			pi_props->sys_context->GetChar(pi_props->sys_context->PROJECT_FOLDER_PATH)
		);
	}

	// Now, set archive folder
	result = pi_props->image_archive->SetFolderPath(paf);

	// Set archive parameters
	result = pi_props->image_archive->SetMode(pi_props->archive_mode);
	result = pi_props->image_archive->SetCompression(pi_props->archive_jpg_compression);
	result = pi_props->image_archive->SetStackExtra(pi_props->archive_extra_stack);
	result = pi_props->image_archive->SetConfigPath(pi_props->obj_pano_config_paf);

	// Setup archive
	result = pi_props->image_archive->Setup(paf, pi_props->obj_pano_config_paf);
	if ( result == VS_ERROR ) return VS_ERROR;

	// Done
	return VS_OK;
}


// ---------- SetupCamera ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SetupCamera()
{
	// Local
	INT result = 0;

	// Set camera ON
	result = pi_props->control->SetCameraLive(TRUE);

	// Set archive ON
	result = pi_props->control->SetCameraArchive(TRUE);


	// Allocate, construct and setup
	result = pi_camera->Setup();
		if ( result <= VS_ERROR ) return result;

	// Update ALL camera modes
	result = pi_camera->UpdateModeByGroup(vs_system::CamModes::ALL_GROUPS);

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Start: sub framework - from SetupGraphics() ----------


// ---------- StartLiveCamera ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::StartLiveCamera()
{
	if ( pi_props->camera_live )
	{
		// Local
		INT result = VS_OK;

		// Start
		pi_camera->Start();
		pi_props->sys_output->Message("Camera: Live Start");

		// Important: The following is required to initialise
		// the camera config object passed to the camera and
		// required for management of the camera config panel.
		{
			// Update ALL camera modes
			result = pi_camera->UpdateModeByGroup(vs_system::CamModes::ALL_GROUPS);

			// Update model camera config element display list
			result = pi_props->model->UpdateCamModesDisplayList(FALSE);
		}

		// Re-initialise # seconds that have been recorded
		pi_props->seconds_elapsed_recording = 0;
	}

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Do: sub framework - from Display() ----------


// ---------- SwitchCameraLive ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SwitchCameraLive()
{
	// IF live switched THEN
	BOOL live = pi_props->control->GetCameraLive();
	if (live != pi_props->camera_live)
	{
		pi_props->camera_live = live;
		if (live)
		{
			INT result = StartLiveCamera();
		}
		else
		{
			pi_camera->Stop();
			pi_props->sys_output->Message("Camera: STOP");
		}

		// Re-initialise # seconds that have been recorded
		pi_props->seconds_elapsed_recording = 0;

	}

	// Done
	return VS_OK;
}


// ---------- SwitchArchiveSave ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SwitchArchiveSave()
{
	// IF camera save mode changed THEN 
	BOOL archive = pi_props->control->GetCameraArchive();
	if ( archive != pi_props->camera_archive )
	{
		pi_props->camera_archive = archive;
		INT result = pi_props->image_archive->SetSave(pi_props->camera_archive);
		if (archive)
		{
			// Update display
			pi_props->sys_output->Message("Record: START");
			result = pi_props->model->UpdateRecordText("Record: START");
		}
		else
		{
			pi_props->sys_output->Message("Record: STOP");
			result = pi_props->model->UpdateRecordText("Record: STOP");
		}

		{
			// Update camera mode for frame rate group
			result = pi_camera->UpdateModeByGroup(vs_system::CamModes::FRAME_RATE_GROUP);

			// Update model camera config element display list
			result = pi_props->model->UpdateCamModesDisplayList(FALSE);
		}

		// Re-initialise # seconds that have been recorded
		pi_props->seconds_elapsed_recording = 0;

	}

	// Done
	return VS_OK;
}


// ---------- ProcessCameraEvents ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::ProcessCameraEvents()
{
	pi_camera->Process();
	return VS_OK;
}


// ---------- UpdateCameraConfig ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::UpdateCameraConfig()
{
	// Note:
	//
	// I left this in so that future versions of Record
	// might provide interactive CamConfig update!

	// Get group identity
	INT group_id = pi_props->control->GetGroupEventID();

	// IF group id THEN
	if ( group_id > 0 )
	{
		// IF cam mode group id THEN
		UINT group_base = vs_system::CamModes::BASE_GROUP / 100 * 100;
		if ( group_base >= vs_system::CamModes::BASE_GROUP )
		{
			// Get element identity
			INT element_id = pi_props->control->GetElementEventID();

			// Update model camera config object
			INT result = VS_OK;
			result = pi_props->model->UpdateCamModes(group_id, element_id);

			// Update model camera config element display list
			// Note: FALSE == don't update cam mode elements
			// as they have already been updated via callback
			result = pi_props->model->UpdateCamModesDisplayList(FALSE);

			// Update camera mode by group id
			pi_camera->UpdateModeByGroup(group_id);
		}

		// Consume events
		pi_props->control->ResetEvents();

	}

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- CallbackOnCaptureImage ----------
/*!
\brief 
\author Gareth Edwards
\returns void
*/
INT application::CallbackOnCaptureImage(
		WORD   width,
		WORD   height,
		WORD   bytes,
		BYTE  *byte_data,
		INT    time_stamp[]
	)
{
	// Local
	INT result = VS_OK;
	INT create_save_path_result = VS_OK;
	INT report_result = VS_OK;
	INT timestamp[7];

	// ================== SET FILENAME ==================

	#ifdef VS_CAM_IDS_VIDEO_SDK
		// Get timestamp
		result = pi_camera->GetTimeStamp(timestamp);
		// Create save path stack and filename
		std::string path[IMAGE_ARCHIVE_PATHTOP];
		std::string filename;
		create_save_path_result = CreateSavePathFilename(path, filename, timestamp);
	#endif

	#if defined(VS_CAM_JPLY_DVP_SDK) || defined(VS_CAM_PIC_BUFFER) 
		// Create save path stack and filename
		std::string path[IMAGE_ARCHIVE_PATHTOP];
		std::string filename;
		INT milliseconds = 0;
		create_save_path_result = pi_props->image_archive->CreateSavePathFilename(path, filename, &milliseconds);
		pi_props->camera_ms_image_map[(INT)((FLOAT)milliseconds / 16.66666f)] = 'X';
	#endif

	// REPORT
	report_result = Report(timestamp);

	// IF failed THEN return
	if ( !create_save_path_result ) return VS_OK;

	// IF waiting to record THEN return
	if ( pi_props->seconds_elapsed_recording < SECONDS_WAIT_TO_RECORD ) return VS_OK;

	// SAVE

	// IF source image buffer (byte_data) THEN
	if ( byte_data != NULL )
	{

		// Set archive jpeg compression factor
		result = pi_props->image_archive->SetCompression(pi_props->archive_jpg_compression);

		// Save image INTO image archive (IF pi_props->camera_save THEN)
		result = pi_props->image_archive->Save(
				path,
				filename,
				width,
				height,
				bytes,
				byte_data
			);
	}

	// Done
	return VS_OK;
}


// ---------- ProcessCapturedImage ----------
/*!
\brief .... AND format and then copy into texture atlas
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/

// Static declaration
record_03::Model * application::PI_Props::model = NULL;

INT application::ProcessCapturedImage(
		std::string(&path)[IMAGE_ARCHIVE_PATHTOP],
		std::string &filename,
		WORD  w,
		WORD  h,
		WORD  bpp,
		BYTE *byte_data,
		INT   time_stamp[]
	)
{
	// Local
	INT result = 0;

	// IF source image buffer (byte_data) THEN
	if ( byte_data != NULL )
	{
		// Set archive jpeg compression factor
		result = pi_props->image_archive->SetCompression(pi_props->archive_jpg_compression);

		// Save image INTO image archive (IF pi_props->camera_save THEN)
		result = pi_props->image_archive->Save(
				path,
				filename,
				w,
				h,
				bpp,
				byte_data
			);

	}

	// Done
	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////
