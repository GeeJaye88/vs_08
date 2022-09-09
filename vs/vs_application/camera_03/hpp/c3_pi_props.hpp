////////////////////////////////////////////////////////////////////////////////

// ---------- c3_pi_props.hpp ----------
/*!
\file c3_pi_props.hpp
\brief Implementation of the application props class
\author Gareth Edwards
*/


#ifndef CAMERA_03_APP_PROPS
#define CAMERA_03_APP_PROPS


////////////////////////////////////////////////////////////////////////////////


// ---- system include ----
#include "../../../vs_system/header/vs_inc_defs.h"
#include "../../../vs_system/header/vs_app_dependency.h"


// ---- vs app include ----
#include "../../review_02/header/r2_control.h"
#include "../../review_02/header/r2_model.h"


// ---- sv library include ----
#include "../../../vs_library/header/vs_cam_define.h"
#include "../../../vs_library/header/vs_cam_ids.h"
#include "../../../vs_library/header/vs_cam_pic.h"


// ---- vs include ----
#include "../../../vs_include/vs_folder.h"


// ---- vs project include ----
#include "../../../vs_library/header/vs_img_arkive.h"


// ---- render to image ----
# define VS_MAX_RENDER_TO_IMG_BUFFERS 32


////////////////////////////////////////////////////////////////////////////////


// ---------- Implementation of PI_Props class ----------
class PI_Props

{

public:

	// ---- constructor/destructor
		PI_Props();
		~PI_Props();


	// ---- methods
		INT Initialise();
		INT Setup();
		INT Configure();


	// ---- pre-process
		INT PreGPUProcess(WORD, WORD, WORD, BYTE *);


	// ---- focus
		INT UpdateFocusBarGraph(WORD, WORD, WORD, BYTE *);


	// ---- monitor
		INT MonitorBuffer(WORD, WORD, WORD, BYTE *);
		INT MonitorAlert();


	// ---- acceleration
		vs_system::AppDependency *app_dependency;
		vs_system::SysContext    *sys_context;
		vs_system::SysOutput     *sys_output;


	// ---- MVC object pointers
		vs_system::AppView       *app_view;
		review_02::Control       *control;
		review_02::Model         *model;


	// ---- objects
		vs_system::ImgArkive       *img_arkive;
		vs_system::ImgColProcess   *img_col_process;
		vs_system::Folder          *scan_folder;
		vs_system::ImgBuffer       *dst_img_buffer;
		vs_system::FloatBuffer     *focus_accum_buffer;
		vs_system::FloatBuffer     *focus_average_buffer;
		vs_system::FloatBuffer     *focus_difference_buffer;
		vs_system::LidarOuster     *lidar_ouster;


	// ---- render to image
		vs_system::ImgBuffer *render_to_image_buffer[VS_MAX_RENDER_TO_IMG_BUFFERS];
		BOOL render_to_image_status[VS_MAX_RENDER_TO_IMG_BUFFERS];


	// ---- counters
		ULONG  seconds_playback_count;
		UINT   images_total_counter;
		ULONG  seconds_elapsed_recording;
		ULONG  seconds_elapsed_to_now;
		UINT   frames_per_second;
		ULONG  frames_total;
		UINT   images_counter;
		UINT   images_missed_count;
		UINT   images_duplicated_count;
		UINT   images_per_second;
		UINT   r2i_per_second;
		UINT   images_saved_per_second;
		DOUBLE loop_hard_start_second_count;


	// ---- camera
		BOOL   camera_live;
		BOOL   loop_hard_start;                 // Set FALSE at runtime - see MONITOR mode
		BOOL   camera_live_at_runtime;          // Set TRUE at runtime, then FALSE
		BOOL   camera_image_captured;           // Set TRUE when camera image aquired, then FALSE
		BOOL   camera_started;                  // Set TRUE when camera started, then FALSE


	// ---- camera image per second map ( TBD: sort out fps )
		CHAR   camera_ms_image_map_0[VS_MAXCHARLEN];
		CHAR   camera_ms_image_map_1[VS_MAXCHARLEN];


	// ---- camera exposure - EXXP
		UINT   exposure_zone;
		UINT   exposure_shape;


	#	ifdef VS_CAM_IDS_VIDEO_SDK

	// ---- IDS image grabber memory
		CHAR  *ids_image_grabber_memory;

	#	endif


	// ---- focus
		BOOL   focus_on;
		UINT   focus_accum_buf_index;
		UINT   focus_accum_buf_size;
		UINT   focus_accum_buf_max;
		BOOL   focus_accum_run_time;


	// ---- monitor
		BOOL   monitor_on;
		FLOAT  monitor_next_display_lut[16];
		FLOAT  monitor_prev_display_lut[16];
		FLOAT  monitor_threshold;
		FLOAT  monitor_restart;
		UINT   monitor_total_samples_changed;
		UINT   monitor_total_samples;
		UINT   monitor_next_samples_lut[16];
		UINT   monitor_prev_samples_lut[16];
		UINT   monitor_sample_incr;
		UINT   monitor_samples_size;
		BYTE  *monitor_samples_lut;


	//---- config camera flags --- Set via "app_config.hdl" file
		UINT   camera_mode;
		BOOL   camera_use_ini_file;
		BOOL   camera_use_hdl_file;


	// ---- archive
		CHAR   previous_filename[VS_MAXCHARPATHLEN];
		CHAR   archive_filename[VS_MAXCHARPATHLEN];
		CHAR   archive_folder[VS_MAXCHARPATHLEN];
		UINT   archive_mode;
		BOOL   archive_save;
		BOOL   archive_save_on_at_runtime;
		UINT   archive_extra_stack;
		UINT   archive_jpg_compression;


	// ---- dashboard
		BOOL   dashboard_only;


	// ---- config files ( set using vs_system::SysContext )
		CHAR   app_setup_config_paf[VS_MAXCHARPATHLEN];
		CHAR   cam_props_config_paf[VS_MAXCHARPATHLEN];
		CHAR   img_props_config_paf[VS_MAXCHARPATHLEN];
		CHAR   obj_config_paf[VS_MAXCHARPATHLEN];
		CHAR   ext_tex_map_image_paf[VS_MAXCHARPATHLEN];


	// ---- shared memory
		//#ifdef VS_CAMERA_03_SHARED_MEMORY
		//sm_handle smh = 0;
		//#endif


	// ---- shared memory - config application
		vs_system::ImgBuffer *shared_image_buffer;
		UINT   shared_image_width;
		UINT   shared_image_height;
		UINT   shared_image_blocks;
		UINT   shared_image_block_width;
		UINT   shared_image_block_height;
		UINT   shared_image_block_extent;
		UINT   shared_pixel_mesh_dim;
		INT    shared_counter;


	// ---- scavenger
		#ifdef VS_CAMERA_03_SCAVENGER
		BOOL   scavenging;
		UINT   scavenger_log;
		UINT   scavenger_minutes_max;
		UINT   scavenger_seconds_pause;
		CHAR   scavenger_app_path[VS_MAXCHARPATHLEN];
		CHAR   scavenger_app_name[VS_MAXCHARNAME];
		CHAR   scavenger_loc_path[VS_MAXCHARPATHLEN];
		#endif


	// ---- streaming and onvif wian remote control
		#ifdef VS_CAMERA_03_STREAMING
		BOOL   stream_on_at_runtime;
		BOOL   stream_and_onvif_on_at_runtime;
		#endif


	// ---- streaming
		#ifdef VS_CAMERA_03_STREAMING
		INT    MapPtzToVptf();
		INT    UpdateWianPtz();
		INT    UpdateVptf(UINT);
		#endif


	// ---- streaming - config application
		BOOL   streaming;
		USHORT stream_logical_id;
		USHORT stream_image_width;
		USHORT stream_image_height;
		INT    stream_frame_modulus;
		CHAR   stream_app_path[VS_MAXCHARPATHLEN];
		CHAR   stream_app_name[VS_MAXCHARNAME];
		USHORT stream_app_fps;


	// ---- ONVIF - config
		CHAR   onvif_ip_address[VS_MAXCHARNAME];
		CHAR   onvif_port_number[VS_MAXCHARNAME];
		FLOAT  onvif_rm_incr;
		FLOAT  onvif_cm_incr;


	// ---- ONVIF - Continuous Move (CM)
		BOOL   onvif_cm_on;
		BOOL   onvif_cm_zoom_mode;
		FLOAT  onvif_cm_pan;
		FLOAT  onvif_cm_tilt;
		FLOAT  onvif_cm_zoom;
		DWORD  onvif_cm_time_last;


	// ---- ONVIF PTZ direction status
		INT    onvif_pan_sign;
		INT    onvif_tilt_sign;
		INT    onvif_zoom_sign;


	// ---- Jerry's ONVIF
		#ifdef VS_CAMERA_03_STREAMING
		std::shared_ptr<OnvifHandler> onvif_handler;
		ONVIFHTTPHandler::Args onvif_handler_args;
		ONVIF::PTZ onvif_wian_ptz;
		#endif


	// ---- Jerry's GPS
		#ifdef VS_CAMERA_03_GPS
		GPSReader *pGPSReader;
		#endif

		UINT gps_mode;
		INT  gps_delay;

		enum {
			GPS_RUNTIME        = 1,
			GPS_TEST_DELAY     = 2,
			GPS_TEST_RUNNING   = 3,
			GPS_IS_RUNNING     = 4,
			GPS_IS_NOT_RUNNING = 5
		};

		std::string s_gps_port;
		std::string s_gps_lat;
		std::string s_gps_lon;
		std::string s_gps_date;
		std::string s_gps_time;

		CHAR gps_lat[VS_MAXCHARLEN];
		CHAR gps_lon[VS_MAXCHARLEN];
		CHAR gps_date[VS_MAXCHARLEN];
		CHAR gps_time[VS_MAXCHARLEN];


	// ---- fx
		UINT fx_mode = 0;
		UINT fx_boost = 0;


} static *pi_props;


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
PI_Props::PI_Props()
#ifdef VS_CAMERA_03_GPS
	: pGPSReader(0)	// initialize here, not in declaration...
#endif
{
	Initialise();
}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
PI_Props::~PI_Props()
{

	// ---- delete MVC objects  ----
		if ( app_view != NULL )
		{
			delete app_view;
			app_view = NULL;
		}
		if ( control != NULL )
		{
			delete control;
			control = NULL;
		}
		if ( model != NULL )
		{
			delete model;
			model = NULL;
		}


	// ---- delete image objects ----
		if ( img_arkive != NULL )
		{
			delete img_arkive;
			img_arkive = NULL;
		}
		if ( img_col_process != NULL )
		{
			delete img_col_process;
			img_col_process = NULL;
		}


	// ---- scan folder object pointer ----
		if ( scan_folder != NULL )
		{
			delete scan_folder;
			scan_folder = NULL;
		}


	// ---- delete image buffer object ----
		if ( dst_img_buffer != NULL )
		{
			delete dst_img_buffer;
			dst_img_buffer = NULL;
		}


	// ---- delete lidar ouster object ----
		if ( lidar_ouster != NULL )
		{
			delete lidar_ouster;
			lidar_ouster = NULL;
		}


	// ---- delete shared image buffer ----
		if ( shared_image_buffer )
		{
			delete shared_image_buffer;
			shared_image_buffer = NULL;
		}


	// ---- delete render to image buffers ----
		for ( UINT i = 0; i<VS_MAX_RENDER_TO_IMG_BUFFERS; i++ )
		{
			if ( render_to_image_buffer[i] != NULL )
			{
				delete render_to_image_buffer[i];
				render_to_image_buffer[i] = NULL;       // TBD should destroy buffer
				render_to_image_status[i] = FALSE;
			}
		}


	// ---- delete focus buffers ----
		if ( focus_accum_buffer )
		{
			delete focus_accum_buffer;
			focus_accum_buffer = NULL;
		}
		if ( focus_average_buffer )
		{
			delete focus_average_buffer;
			focus_average_buffer = NULL;
		}
		if ( focus_difference_buffer )
		{
			delete focus_difference_buffer;
			focus_difference_buffer = NULL;
		}


	// ---- monitor  ----
		if ( monitor_samples_lut != NULL )
		{
			delete[] monitor_samples_lut;
			monitor_samples_lut = NULL;
		}


	// ---- IDS image grabber memory ----
	#	ifdef VS_CAM_IDS_VIDEO_SDK
		if ( ids_image_grabber_memory != NULL )
		{
			delete[] ids_image_grabber_memory;
			ids_image_grabber_memory = NULL;
		}
	#	endif

}


// ---------- Initialise ----------
/*!
\brief Initialise
\author Gareth Edwards
*/
INT PI_Props::Initialise()
{

	// ---- acceleration ----
		sys_context = NULL;
		sys_output  = NULL;


	// ---- MVC object pointers ----
		app_view  = NULL;
		control   = NULL;
		model     = NULL;


	// ---- objects ----
		img_arkive      = NULL;
		img_col_process = NULL;
		scan_folder     = NULL;
		dst_img_buffer  = NULL;
		lidar_ouster    = NULL;


	// ---- focus objects ----
		focus_accum_buffer      = NULL;
		focus_average_buffer    = NULL;
		focus_difference_buffer = NULL;


	// ---- render to image ----
	for (UINT i = 0; i<VS_MAX_RENDER_TO_IMG_BUFFERS; i++)
	{
		render_to_image_buffer[i] = NULL;
		render_to_image_status[i] = FALSE;
	}


	// ---- counters ----
		seconds_playback_count    = 0;
		images_total_counter      = 0;
		seconds_elapsed_recording = 0;
		seconds_elapsed_to_now    = 0;
		frames_per_second         = 0;
		frames_total              = 0;
		images_counter            = 0;
		images_missed_count       = 0;
		images_duplicated_count   = 0;
		images_per_second         = 0;
		r2i_per_second            = 0;
		images_saved_per_second   = 0;
		loop_hard_start_second_count = 0;


	// ---- camera ----
		camera_live              = TRUE;
		loop_hard_start          = FALSE;
		camera_live_at_runtime   = TRUE;
		camera_image_captured    = FALSE;
		camera_started           = FALSE;


	// ---- camera image per second map ( TBD: sort out fps ) ----
		memset(camera_ms_image_map_0, '_', 60);
		camera_ms_image_map_0[60] = 0;
		INT len = strlen(camera_ms_image_map_0);
		strcpy(camera_ms_image_map_1, camera_ms_image_map_0);


	// ---- camera exposure - EXXP ----
		exposure_zone  = 0;
		exposure_shape = 0;


	#	ifdef VS_CAM_IDS_VIDEO_SDK

	// ---- IDS image grabber memory ----
		ids_image_grabber_memory = 0;

	#	endif


	// ---- focus ----
		focus_on              = FALSE;
		focus_accum_buf_index = 0;
		focus_accum_buf_size  = 1024;
		focus_accum_buf_max   = 8;
		focus_accum_run_time  = TRUE;


	// ---- monitor ----
		monitor_on = FALSE;
		for (UINT i = 0; i < 16; i++)
		{
			monitor_next_display_lut[i] = 0;
			monitor_prev_display_lut[i] = 0;
			monitor_next_samples_lut[i] = 0;
			monitor_prev_samples_lut[i] = 0;
		}
		monitor_threshold             = 5;
		monitor_restart               = 3.0f;
		monitor_total_samples_changed = 0;
		monitor_total_samples         = 0;
		monitor_sample_incr           = 16;
		monitor_samples_size          = 0;
		monitor_samples_lut           = NULL;


	//---- config camera flags --- Set via "app_config.hdl" file ----
		camera_mode                = 1;
		camera_use_ini_file        = FALSE;
		camera_use_hdl_file        = TRUE;


	// ---- archive ----
		strcpy(previous_filename,"");
		strcpy(archive_filename, "");
		strcpy(archive_folder, "");
		archive_mode = vs_system::ImgArkive::MODE;
		archive_save = FALSE;
		archive_save_on_at_runtime = FALSE;;
		archive_extra_stack = 0;
		archive_jpg_compression = vs_system::ImgArkive::JPG_QUALITY;


	// ---- dashboard ----
		dashboard_only = FALSE;


	// ---- config files ( set using vs_system::SysContext ) ----
		strcpy(app_setup_config_paf,  "");
		strcpy(cam_props_config_paf,  "");
		strcpy(obj_config_paf,   "");
		strcpy(ext_tex_map_image_paf, "");


	// ---- shared - config application ----
		shared_image_width    = 400;
		shared_image_height   = 1600;
		shared_image_blocks   = 4;
		shared_image_block_width  = shared_image_width;
		shared_image_block_height = shared_image_height / shared_image_blocks;
		shared_pixel_mesh_dim     = 50;
		shared_counter        = 1;


	// ---- shared  ----
		shared_image_buffer = NULL;


	// ---- streaming and onvif wian remote control ----
		#ifdef VS_CAMERA_03_STREAMING
		stream_on_at_runtime           = FALSE;
		stream_and_onvif_on_at_runtime = FALSE;
		#endif


	// ---- streaming - config application ----
		streaming            = FALSE;
		stream_logical_id    = 1;
		stream_image_width   = 800;
		stream_image_height  = 600;
		stream_frame_modulus = 0;
		strcpy(stream_app_path, "");
		strcpy(stream_app_name, "");
		USHORT stream_app_fps  = 5;


	// ---- scavenger - config application ----
		scavenging              = FALSE;
		scavenger_log           = 1;
		scavenger_minutes_max   = 60;
		scavenger_seconds_pause = 120;
		strcpy(scavenger_app_path, "");
		strcpy(scavenger_app_name, "Scavenger.exe");
		strcpy(scavenger_loc_path, "");


	// ---- ONVIF - config ----
		strcpy(onvif_ip_address,  "0");
		strcpy(onvif_port_number, "8001");
		onvif_rm_incr = 0.1f;
		onvif_cm_incr = 0.1f;


	// ---- ONVIF - Continuous Move (CM) ----
		onvif_cm_on = FALSE;
		onvif_cm_zoom_mode = FALSE;
		onvif_cm_pan  = 0;
		onvif_cm_tilt = 0;
		onvif_cm_zoom = 0;
		onvif_cm_time_last = timeGetTime();


	// ---- ONVIF PTZ direction status ----
		onvif_pan_sign  = 0;
		onvif_tilt_sign = 0;
		onvif_zoom_sign = 0;


	// ---- Jerry's ONVIF --
		#ifdef VS_CAMERA_03_STREAMING
		onvif_handler      = NULL;
		onvif_wian_ptz     = {};
		onvif_handler_args = {};
		#endif


	// ---- Jerry's GPS ----
		#ifdef VS_CAMERA_03_GPS
		pGPSReader = 0;
		#endif
	

	// ---- Jerry's GPS ----
		gps_mode = GPS_RUNTIME;
		gps_delay = 2000;

		s_gps_port = "no lock";
		s_gps_lat  = "no lock";
		s_gps_lon  = "no lock";
		s_gps_date = "no lock";
		s_gps_time = "no lock";

		strcpy( gps_lat,  "00° 00' 00.00 ?");
		strcpy( gps_lon,  "00° 00' 00.00 ?");
		strcpy( gps_date, "00-00-00");
		strcpy( gps_time, "00:00:00:000");

	return VS_OK;
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
*/
INT PI_Props::Setup()
{

	// ---- local
		INT result = 0;


	// ---- allocate and construct VS image buffer objects
		dst_img_buffer = new vs_system::ImgBuffer();


	// ---- allocate and construct VS lidar ouster objects
		lidar_ouster = new vs_system::LidarOuster();


	// ---- create, allocate and intialise shared image buffer
		shared_image_buffer = new vs_system::ImgBuffer;
		result = shared_image_buffer->Initialise(0, 0, 3);
		result = shared_image_buffer->SetQuality(90);


	// ---- create, allocate and intialise render to image buffers
		for (UINT i = 0; i<VS_MAX_RENDER_TO_IMG_BUFFERS; i++)
		{
			render_to_image_buffer[i] = new vs_system::ImgBuffer();
			result = render_to_image_buffer[i]->Initialise(0, 0, 3);
			result = render_to_image_buffer[i]->SetQuality(90);
		}


	// ---- default initialise destination image buffer
	//    - used in ProcessCapturedImage method
		result = dst_img_buffer->Initialise(0, 0, 0);
		result = dst_img_buffer->SetQuality(40);


	// ---- default initialise lidar ouster
		result = lidar_ouster->SetBeams(64);
		result = lidar_ouster->SetColumns(1024);


	// ---- allocate and construct VS float buffer objects
		focus_accum_buffer      = new vs_system::FloatBuffer();
		focus_average_buffer    = new vs_system::FloatBuffer();
		focus_difference_buffer = new vs_system::FloatBuffer();

	return VS_OK;
}


// ---------- Configure ----------
/*!
\brief Configure private implementation properties object
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT PI_Props::Configure()
{

	// ---- read application setup configuration file ( e.g. "app_config.hdl" )
		vs_system::VNodeIO vio;
		vs_system::VNode *config_hdl = vio.Read(app_setup_config_paf);


	// ---- application setup configuration file does NOT exist ?
		if (config_hdl == NULL)
		{
			INT result = sys_output->MessageError(
					"Could not READ application setup configuration file.",
					"File",
					app_setup_config_paf
				);
			return VS_ERROR;
		}


	// ---- config data ?
		if ((INT)config_hdl != VS_ERROR)
		{

			// ---- write application setup configuration file to log
				sys_output->WriteFile(app_setup_config_paf);


			// ---- local
				vs_system::VNode *p = NULL;
				vs_system::VNode *c = NULL;


			if ( c = config_hdl->SearchName("Camera") )
			{

				// ---- # - use [identity_#][...] config files
					if (p = c->SearchName("Config-Mode")     ) camera_mode = p->GetIntFromValue();

				// ---- 1 ? - use [identity_#]/[camera_name]_config.ini NOT default camera setup
					if (p = c->SearchName("Config-Ini-File") ) camera_use_ini_file = p->GetBoolFromValue();

				// ---- 1 ? - use [identity_#]/[identity]_config.hdl" file
					if (p = c->SearchName("Config-Hdl-File") ) camera_use_hdl_file = p->GetBoolFromValue();
			}


			if ( c = config_hdl->SearchName("Monitor") )
			{
				if (p = c->SearchName("Threshold")        ) monitor_threshold = p->GetFloatFromValue();
				if (p = c->SearchName("Restart-Duration") ) monitor_restart   = p->GetFloatFromValue();
			}


			if ( c = config_hdl->SearchName("Archive") )
			{

				// ---- 
					if (p = c->SearchName("Mode")   ) archive_mode = p->GetIntFromValue();
					if (p = c->SearchName("Folder") ) strcpy(archive_folder, p->GetValue());
					if (p = c->SearchName("Save")   ) archive_save_on_at_runtime = p->GetBoolFromValue();

				//---- jpeg permissable range 20 to 100
					if (p = c->SearchName("Compression") )
					{
						archive_jpg_compression = p->GetIntFromValue();
						archive_jpg_compression = archive_jpg_compression < 20 ?
								20 : (archive_jpg_compression > 100 ? 100 : archive_jpg_compression);
					}

				//---- stack size set to  permissable ( 2 * core ) + extra
					if (p = c->SearchName("Extra-Threads") )
					{
						archive_extra_stack = p->GetIntFromValue();
						archive_extra_stack = archive_extra_stack < 0 ? 0 : archive_extra_stack;
					}

			}


			if ( c = config_hdl->SearchName("Scavenger") )
			{
				if (p = c->SearchName("Log")           ) scavenger_log           = (UINT)p->GetIntFromValue();
				if (p = c->SearchName("Minutes-Max")   ) scavenger_minutes_max   = (UINT)p->GetIntFromValue();
				if (p = c->SearchName("Seconds-Pause") ) scavenger_seconds_pause = (UINT)p->GetIntFromValue();
			}


			if ( c = config_hdl->SearchName("GPS") )
			{
				if (p = c->SearchName("Port")  ) s_gps_port = p->GetValue();
				if (p = c->SearchName("Delay") ) gps_delay  = p->GetIntFromValue();
			}


			if (c = config_hdl->SearchName("Shared"))
			{
				// ---- get ----
				if (p = c->SearchName("Blocks")        ) shared_image_blocks   = p->GetIntFromValue();
				if (p = c->SearchName("Mesh")          ) shared_pixel_mesh_dim = p->GetIntFromValue();
				if (p = c->SearchName("Height")        ) shared_image_height   = p->GetIntFromValue();

			}


			if ( c = config_hdl->SearchName("Stream")  )
			{

				// ---- vptz
					if (p = c->SearchName("Vptz-ID")       ) stream_logical_id    = p->GetIntFromValue();
					if (p = c->SearchName("Vptz-Width")    ) stream_image_width   = p->GetIntFromValue();
					if (p = c->SearchName("Vptz-Height")   ) stream_image_height  = p->GetIntFromValue();
					if (p = c->SearchName("Vptz-Modulus")  ) stream_frame_modulus = p->GetIntFromValue();

				// ---- app
					if (p = c->SearchName("ID")            ) stream_logical_id    = p->GetIntFromValue();
					if (p = c->SearchName("Width")         ) stream_image_width   = p->GetIntFromValue();
					if (p = c->SearchName("Height")        ) stream_image_height  = p->GetIntFromValue();
					if (p = c->SearchName("Frame-Modulus") ) stream_frame_modulus = p->GetIntFromValue();
					if (p = c->SearchName("App-Path")      ) strcpy(stream_app_path, p->GetValue());
					if (p = c->SearchName("App-Name")      ) strcpy(stream_app_name, p->GetValue());
					if (p = c->SearchName("App-FPS")       ) stream_app_fps       = p->GetIntFromValue();

				// ---- flag
					#ifdef VS_CAMERA_03_STREAMING
					if (p = c->SearchName("On-At-Runtime") ) stream_on_at_runtime = p->GetBoolFromValue();
					#endif

			}


			if ( c = config_hdl->SearchName("ONVIF" ) )
			{
				// ---- vptz
					if (p = c->SearchName("IP-Address")    ) strcpy(onvif_ip_address, p->GetValue());
					if (p = c->SearchName("Port-Number")   ) strcpy(onvif_port_number, p->GetValue());
					if (p = c->SearchName("RM-Increment" ) ) onvif_rm_incr = p->GetFloatFromValue();
					if (p = c->SearchName("CM-Increment" ) ) onvif_cm_incr = p->GetFloatFromValue();

				// ---- flag
					#ifdef VS_CAMERA_03_STREAMING
					if (p = c->SearchName("On-At-Runtime") ) stream_and_onvif_on_at_runtime = p->GetBoolFromValue();
					#endif
			}

		}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


#include "c3_buf_preproc.hpp"
#include "c3_buf_monitor.hpp"
#include "c3_cam_focus_bar_graph.hpp"


#ifdef VS_CAMERA_03_STREAMING
#include "c3_pi_props_ptz_onvif.hpp"
#endif


////////////////////////////////////////////////////////////////////////////////


#endif CAMERA_03_APP_PROPS


////////////////////////////////////////////////////////////////////////////////
