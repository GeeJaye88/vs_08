////////////////////////////////////////////////////////////////////////////////

// ---------- r2_control_pimp.hpp ----------
/*!
\file r2_control_pimp.hpp
\brief 
\author Gareth Edwards
*/


#include "../header/r2_model.h"


using namespace review_02;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Application class ----------

class Control::PI_Application
{

public:

	PI_Application()
	{
		callback_to_setup_graphics   = NULL;
		callback_to_cleanup_graphics = NULL;
	};
	~PI_Application(){ };

	INT(*callback_to_setup_graphics)();
	INT(*callback_to_cleanup_graphics)();

};


////////////////////////////////////////////////////////////////////////////////

		
// ---------- Private Implementation of PI_Camera class ----------

class Control::PI_Camera
{

public:

	PI_Camera()
	{
		exposure_must_be_updated = FALSE;
		archive_save    = FALSE;
		dashboard_only  = FALSE;
		fx_boost        = 0;
		fx_mode         = 0;
		click           = FALSE;
		update_offset   = 0;
		live            = TRUE;
		loop_hard_start = FALSE;
		panophoto       = FALSE;
		sys_max_inc     = TRUE;
		aec_max_inc     = TRUE;
		pano_max_inc    = TRUE;
		iloc_max_inc    = TRUE;
		remote_control  = FALSE;
		stream          = FALSE;

		#if VS_LIDAR
		lidar_visible_mode = 1;
		lidar_rotate_mode  = 2;
		#endif

	};

	~PI_Camera(){ };

	BOOL  exposure_must_be_updated;
	BOOL  archive_save;
	BOOL  dashboard_only;
	UINT  fx_boost;
	UINT  fx_mode;
	BOOL  click;
	BOOL  update_offset;
	BOOL  live;
	BOOL  loop_hard_start;
	BOOL  panophoto;
	BOOL  sys_max_inc;
	BOOL  aec_max_inc;
	BOOL  pano_max_inc;
	BOOL  iloc_max_inc;
	BOOL  remote_control;
	BOOL  stream;

	#if VS_LIDAR
	UINT  lidar_visible_mode;
	UINT  lidar_rotate_mode;
	#endif
};


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Context class ----------

class Control::PI_Context
{

public:

	PI_Context() {}
	~PI_Context() {}

	BOOL pano_mode = FALSE;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Timeline class ----------

class Control::PI_Timeline
{

public:

	PI_Timeline()
	{
		timeline_command       = 0;
		timeline_live_mode     = FALSE;
		timeline_speed_mode    = 0;
		timeline_play_mode     = FALSE;
	};
	~PI_Timeline(){ };

	UINT timeline_command;
	BOOL timeline_live_mode;
	UINT timeline_speed_mode;
	BOOL timeline_play_mode;
};


////////////////////////////////////////////////////////////////////////////////


