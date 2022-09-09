////////////////////////////////////////////////////////////////////////////////

// ---------- r2_control_app_get_and_set.hpp ----------
/*!
\file r2_control_app_get_and_set.hpp
\brief 
\author Gareth Edwards
*/


#include "../header/r2_model.h"


using namespace review_02;


////////////////////////////////////////////////////////////////////////////////


// ---------- GetCameraExposureMustBeUpdated ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
BOOL Control::GetCameraExposureMustBeUpdated()
{
	BOOL aru = pi_camera->exposure_must_be_updated;
	pi_camera->exposure_must_be_updated = FALSE;
	return aru;
}


// ---------- GetCameraUpdateOffset ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
BOOL Control::GetCameraUpdateOffset()
{
	BOOL update_offset = pi_camera->update_offset;
	pi_camera->update_offset = 0;
	return update_offset;
}


// ---------- GetCameraLoopHardStart ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
BOOL Control::GetCameraLoopHardStart()
{
	return pi_camera->loop_hard_start;
}


// ---------- GetCameraLive ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
BOOL Control::GetCameraLive()
{
	return pi_camera->live;
}


// ---------- GetCameraPanophoto ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
BOOL Control::GetCameraPanophoto()
{
	BOOL status = pi_camera->panophoto;
	if ( status ) pi_camera->panophoto = FALSE;
	return status;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- GetArchiveSave ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
BOOL Control::GetArchiveSave()
{
	return pi_camera->archive_save;
}


// ---------- SetArchiveSave ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT Control::SetArchiveSave(BOOL save)
{
	pi_camera->archive_save = save;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- GetDashboardOnly ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
BOOL Control::GetDashboardOnly()
{
	return pi_camera->dashboard_only;
}

// ---------- SetDashboardOnly ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT Control::SetDashboardOnly(BOOL only)
{
	pi_camera->dashboard_only = only;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- GetFXBoostMode ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
UINT Control::GetFXBoostMode()
{
	return pi_camera->fx_boost;
}


// ---------- GetFXColourMode ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
UINT Control::GetFXColourMode()
{
	return pi_camera->fx_mode;
}


////////////////////////////////////////////////////////////////////////////////


// ========== STREAM ==========


// ---------- GetStreamFlag ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
BOOL Control::GetStreamFlag()
{
	return pi_camera->stream;
}

// ---------- GetRemoteControlFlag ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
BOOL Control::GetRemoteControlFlag()
{
	return pi_camera->remote_control;
}


// ---------- SetStreamStatus ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
BOOL Control::SetStreamStatus(BOOL stream)
{
	pi_camera->stream = stream;
	return VS_OK;
}


// ---------- SetRemoteControlStatus ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
BOOL Control::SetRemoteControlStatus(BOOL remote_control)
{
	pi_camera->remote_control = remote_control;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- GetTimelineCommand ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
UINT Control::GetTimelineCommand(UINT *count, INT param[])
{
	// Command parameters
	*count = app_control->command_param_count;
	for ( UINT i=0; i<app_control->command_param_count; i++ )
	{
		param[i] = app_control->command_param[i];
	}
	
	// Command
	UINT temp = pi_timeline->timeline_command;
	pi_timeline->timeline_command = COMMAND_NONE;
	return temp;
}


// ---------- GetTimelineLive ----------
/*!
\brief
\author Gareth Edwards
\return BOOL
*/
BOOL Control::GetTimelineLive()
{
	return pi_timeline->timeline_live_mode;
}


// ---------- GetTimelineSpeedMode----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
UINT Control::GetTimelineSpeedMode()
{
	return pi_timeline->timeline_speed_mode;
}


// ---------- GetTimelinePlay ----------
/*!
\brief
\author Gareth Edwards
\return BOOL
*/
BOOL Control::GetTimelinePlay()
{
	return pi_timeline->timeline_play_mode;
}


// ========== GET TIMELINE ==========


// ---------- SetTimelineLive ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
BOOL Control::SetTimelineLive(BOOL value)
{
	return pi_timeline->timeline_live_mode = value;
}


// ---------- SetTimelinePlay ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
BOOL Control::SetTimelinePlay(BOOL value)
{
	return pi_timeline->timeline_play_mode = value;
}


////////////////////////////////////////////////////////////////////////////////


