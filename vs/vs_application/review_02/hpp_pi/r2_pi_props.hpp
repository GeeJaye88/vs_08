////////////////////////////////////////////////////////////////////////////////

// ---------- r2_pi_props.hpp ----------
/*!
\file r2_pi_props.hpp
\brief
\author Gareth Edwards
*/


// ---------- Private Implementation of PI_Props class ----------

using namespace review_02;


class application::PI_Props
{

public:

	// ---- constructor/destructor
	PI_Props();
	~PI_Props();


	// ---- methods ----
	void  Initialise();
	INT   Setup(CHAR *);
	INT   Pano();
	void  ResetClipProperties();
	void  ResetButtonStates(BOOL, Control *, Model*);
	

	// ---- VS object pointers ----
	vs_system::AppDependency *app_dependency;
	vs_system::SysContext    *sys_context;
	vs_system::SysOutput     *sys_output;


	// ---- files ----
	CHAR  archive_paf[VS_MAXCHARPATHLEN];
	CHAR  app_setup_config_paf[VS_MAXCHARPATHLEN];
	CHAR  app_archive_paf[VS_MAXCHARPATHLEN];


	// ---- application counters ----
	LONG  playback_second_count;
	UINT  images_per_second;
	UINT  frames_per_second;


	// ---- change flags ----
	BOOL  minute_changed;
	BOOL  second_changed;


	// ---- datetime ----
	BOOL  set_datetime_value;
	BOOL  datetime_list_init;
	UINT  datetime_items;
	UINT  datetime_stride;
	LLONG datetime_list[400];
	std::string datetime_text[DATETIME_MAX_STACK];
	UINT  datetime_value[DATETIME_MAX_STACK];
	UINT  datetime_index;


	// ---- clip ----
	LLONG clip_ms_inpoints[2];
	UINT  clip_datetime_stack[2][DATETIME_MAX_STACK];
	BYTE  clip_inpoints_bitmask;
	BOOL  clip_inpoints_set;
	BOOL  clip_loop;
	BOOL  clip_updated;


	// ---- time ----
	DOUBLE time_millisecond_now;
	DOUBLE time_millisecond_last;
	DOUBLE time_millisecond_increment;
	DOUBLE time_millisecond_delay;
	DOUBLE time_millisecond_delay_target;


	// ---- timeline ----
	BOOL  timeline_created;
	BOOL  timeline_new;
	BOOL  timeline_goto;
	BOOL  timeline_update;
	FLOAT timeline_speed;
	UINT  timeline_command;
	INT   timeline_last_minute;
	INT   timeline_last_second;


	// ---- archive flags ----
	BOOL  archive_run_time;
	BOOL  archive_goto_last_frame;
	BOOL  archive_updating;
	BOOL  archive_update_start;
	BOOL  archive_sync_message;


	// ---- archive IO ----
	std::string archive_last_filename;
	std::string archive_queue_filename;
	std::string archive_read_filename;
	UINT        archive_queue_index;
	UINT        archive_read_index;


	// ---- archive time ----
	DOUBLE archive_ms_time_begin;
	DOUBLE archive_ms_time_folder_count_changed;
	DOUBLE archive_ms_time_image_count_changed;
	DOUBLE archive_ms_time_last;
	DOUBLE archive_ms_time_now;
	DOUBLE archive_ms_time_pause;
	ULONG  archive_sec_update;


	// ---- archive last image ----
	INT    archive_last_image_width;
	INT    archive_last_image_height;


	// ---- archive export ----
	BOOL     archive_export;
	HANDLE   archive_export_thread_handle;
	ArchData archive_export_data;


	// ---- pano photo ----
	BOOL   create_pano_image;
	BOOL   panophoto_fullsize;
	FLOAT  panophoto_basis;
	FLOAT  panophoto_gamma;
	FLOAT  panophoto_sharpness;
	UINT   panophoto_sample;
	UINT   panophoto_width;


	// ---- pano photo thread ----
	HANDLE  pano_thread_handle;


	// ---- update model config bitmasks flag ----
	BOOL  update_config_bitmask;

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::PI_Props::PI_Props()
{
	Initialise();
}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::PI_Props::~PI_Props()
{
	;
}


// ---------- Initialise ----------
/*!
\brief Initialise
\author Gareth Edwards
*/
void application::PI_Props::Initialise()
{

	// ---- VS object pointers ----
	app_dependency = NULL;
	sys_context    = NULL;
	sys_output     = NULL;


	// ---- config path/file names ----
	strcpy(archive_paf, "");
	strcpy(app_setup_config_paf, "");
	strcpy(app_archive_paf, "");


	// ---- application counters ----
	frames_per_second = 0;
	playback_second_count = 0;
	images_per_second = 0;


	// ---- change ----
	minute_changed  = FALSE;
	second_changed  = FALSE;


	// ---- datetime ----
	set_datetime_value = FALSE;
	datetime_list_init = FALSE;
	datetime_items = 0;
	datetime_stride = 0;
	for ( UINT i=0; i<DATETIME_MAX_STACK; i++ )
	{
		datetime_text[i] = "";
		datetime_value[i] = 0;
	}


	// ---- runtime datetime index ----
	datetime_index= COMMAND_DATETIME_RUNTIME;


	// ---- clip ----
	ResetClipProperties();


	// ---- time ----
	time_millisecond_now = 0;
	time_millisecond_last = 0;
	time_millisecond_increment = 0;
	time_millisecond_delay = 0;
	time_millisecond_delay_target = 1000;


	// ---- timeline ----
	timeline_created = FALSE;
	timeline_new = FALSE;
	timeline_goto = FALSE;
	timeline_update = FALSE;
	timeline_speed = 1;
	timeline_command = COMMAND_NONE;
	timeline_last_minute = -1;
	timeline_last_second = -1;


	// ---- archive flags ----
	archive_run_time = TRUE;           // NOT USED
	archive_goto_last_frame = FALSE;   // Don't want to go to last frame as default start state
	archive_updating = TRUE;           // Do want updating to be default state
	archive_update_start = TRUE;       // Do want start to be detected
	archive_sync_message = TRUE;       // Do want first sync message


	// ---- archive IO ----
	archive_last_filename = "";
	archive_queue_filename = "";
	archive_read_filename = "";
	archive_queue_index = 0;
	archive_read_index = 0;


	// ---- archive Time ----
	archive_ms_time_begin = 0;
	archive_ms_time_folder_count_changed = 0;
	archive_ms_time_image_count_changed = 0;
	archive_ms_time_last = 0;
	archive_ms_time_now = 0;
	archive_ms_time_pause = 0;
	archive_sec_update = 0;


	// ---- archive Last Image ----
	archive_last_image_width  = -1;
	archive_last_image_height = -1;


	// ---- create pano image ----
	create_pano_image   = FALSE;        // Internal status flag
	panophoto_fullsize  = FALSE;        // Fullsize or config specified wifht/height 
	panophoto_basis     = 0.0f;         // Interpolant, where 0 == inner and 1 == outer circumference
	panophoto_gamma     = 1.2f;         // Where 1 has no effect
	panophoto_sharpness = 0.0f;
	panophoto_sample    = 3;            // sample >= 1, where 1 is point fast point sampling
	panophoto_width     = 8000;         // Default width


	// ---- pano photo thread ----
	pano_thread_handle    = NULL;


	// ---- update model config bitmasks flag ----
	//    - Set TRUE in CreateTimeline
	//    - Set FALSE in ArchiveRead
	update_config_bitmask = FALSE;
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
*/
INT application::PI_Props::Setup(CHAR *filename)
{

	// ---- read config fle ----
	vs_system::VNodeIO vio;
	vs_system::VNode *config_hdl = vio.Read(filename);


	// IF -- config data READ -- THEN
	if ( (INT)config_hdl != VS_ERROR )
	{

		// local
		vs_system::VNode *p = NULL;

		// IF -- "Archive" config data -- THEN
		if ( config_hdl->SearchName("Panophoto") )
		{
			if (p = config_hdl->SearchName("Full-Size") )
				panophoto_fullsize  = p->GetBoolFromValue();
			if (p = config_hdl->SearchName("Basis")     )
				panophoto_basis     = p->GetFloatFromValue();
			if (p = config_hdl->SearchName("Gamma")     ) panophoto_gamma     = p->GetFloatFromValue();
			if (p = config_hdl->SearchName("Sharpness") ) panophoto_sharpness = p->GetFloatFromValue();
			if (p = config_hdl->SearchName("Sample")    )
				panophoto_sample    = p->GetIntFromValue();
			if (p = config_hdl->SearchName("Width")     ) panophoto_width     = p->GetIntFromValue();
		}
	}

	return VS_OK;
}


// ---------- Pano ----------
/*!
\brief Pano
\author Gareth Edwards
*/
INT application::PI_Props::Pano()
{

	// IF -- open test file -- THEN -- write
	std::ofstream outfile("test.txt");
	if (outfile.is_open())
	{
		outfile.write("GARETH", 6);
		outfile.close();
	}

	return VS_OK;
}


// ---------- ResetButtonStates ----------
/*!
\brief Reset button staes and control flags
\author Gareth Edwards
*/
void application::PI_Props::ResetButtonStates(
		BOOL setup,
		Control *control,
		Model *model
	)
{

	// ---- datetime - resets on-the-fly ----


	// ---- clip ----
	//
	// Note: ONLY resets if NO inpoints set.
	//
	if ( !clip_inpoints_bitmask )
	{
		model->UpdateButtonSwitch(NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_CLIP_START, FALSE);
		model->UpdateButtonSwitch(NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_CLIP_END,   FALSE);
		model->UpdateButtonSwitch(NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_CLIP_LOOP,  FALSE);
		ResetClipProperties();
	}


	// ---- live ----
	//
	// NOTE: Concteptualy deprecated by NEW live archive code!
	//
	//       Candidate to be deleted!
	//
	model->UpdateButtonSwitch(NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_LIVE, FALSE);
	control->SetTimelineLive(FALSE);
	

	// ---- play ----
	model->UpdateButtonSwitch(NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_PLAY, FALSE);
	control->SetTimelinePlay(FALSE);

}


// ---------- ResetClipProperties ----------
/*!
\brief Reset clip properties
\author Gareth Edwards
*/
void application::PI_Props::ResetClipProperties()
{

	// reset clip IN/OUT datetime stacks
	for ( UINT i=0; i<DATETIME_MAX_STACK; i++ )
	{

		// old comment:
		//    TBD Index '7' is out of valid index range '0' to '1' for
		//    possibly stack allocated buffer 'clip_ms_inpoints'.
		// 08/05/2016 - no idea what tjhe above meant
		//
		clip_ms_inpoints[i] = 0;
		for ( UINT j=0; j<2; j++ )
		{
			clip_datetime_stack[j][i] = 0;
		}

	}

	clip_inpoints_bitmask = 0;
	clip_inpoints_set = FALSE;
	clip_loop         = FALSE;
	clip_updated      = FALSE;

}


