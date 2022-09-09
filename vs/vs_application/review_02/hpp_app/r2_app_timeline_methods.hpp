////////////////////////////////////////////////////////////////////////////////

// ---------- r2_empty.h ----------
/*!
\file r2_empty.h
\brief
\author Gareth Edwards
*/


#pragma once

#include "../header/r2_model.h"

using namespace review_02;


// ---------- TimelineGoto ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::TimelineGoto(INT direction, INT command)
{

	// Build GOTO stack from datetime_value stack
	// and command parameter zero....
	//
	// Note: context == timeline_list_index
	//
	// 1. From 0 to timeline_list_index - 1, copy from
	//    datetime_value stack to goto_stack
	// 2. At timeline_list_index, copy from datetime_value
	//    stack to goto_stack
	// 3. From timeline_list_index - 1 to top of stack,
	//    set goto_stack to zero
	//

	// Initialise goto stack
	INT datetime_goto_stack[DATETIME_MAX_STACK];


	// 1 - From 0 to timeline_list_index, copy from stack to goto_stack
	for ( UINT i=0; i<=pi_props->datetime_index; i++ )
		datetime_goto_stack[i] = pi_props->datetime_value[i];


	// 2 - At timeline_list_index, copy from datetime_value stack to goto_stack
	datetime_goto_stack[pi_props->datetime_index] = command;


	// IF -- not milliseconds -- THEN
	if ( pi_props->datetime_index < DATETIME_MSEC )
	{
		// 3 - From timeline_list_index - 1 to top of stack set goto_stack to zero
		for ( UINT i=pi_props->datetime_index+1; i<DATETIME_MAX_STACK; i++ )  
			datetime_goto_stack[i] = -1;
	}


	// NOW Goto!!!
	INT result = img_timeline->Goto(
			pi_props->datetime_index,
			direction,
			DATETIME_MAX_STACK,
			datetime_goto_stack
		);

	return result;
}


// ---------- TimelineClipGoto ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT application::TimelineClipGoto(INT mode)
{
	
	// initialise goto stack
	INT datetime_goto_stack[DATETIME_MAX_STACK];


	// copy clip goto datetime stack into goto datetime
	for ( UINT i=0; i<=7; i++ )
		datetime_goto_stack[i] = pi_props->clip_datetime_stack[mode - 1][i];


	// goto
	INT result = img_timeline->Goto(
			7,
			img_timeline->GOTO_CLIP,
			DATETIME_MAX_STACK,
			datetime_goto_stack
		);

	return result;
}


// ---------- CreateTimeline ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::CreateTimeline(CHAR *archive_path)
{

	// Blimey... wot a palaver
	CHAR path[VS_MAXCHARPATHLEN];
	CHAR temp[VS_MAXCHARPATHLEN];
	strcpy(path, archive_path);
	strcpy(temp, path);
	strcat(temp, "archive.hdl");

	// IF pano config THEN ...
	INT result = model->GetObjConfig()->ReadPanoConfig(temp);
	if ( result == VS_OK )
	{

		//... THEN report
		model->ReportObjConfigPanoConfig();

		// (re)create timeline
		strcpy(temp, path);
		strcat(temp, "archive/");
		result = img_timeline->Create(temp);

		// This block OK as timeline won't be accessed until
		// the thread created by img_timeline->Create thread
		// has completed.
		//
		if ( result )
		{
			// SET timeline_created flag TRUE
			pi_props->timeline_created = TRUE;

			// SET timeline_new flag TRUE
			pi_props->timeline_new = TRUE;

			// Set graphics update bitmasks flag TRUE
			pi_props->update_config_bitmask = TRUE;

			// Reset archive time
			pi_props->archive_ms_time_begin =
				pi_props->archive_ms_time_folder_count_changed =
					pi_props->archive_ms_time_image_count_changed =
						pi_props->archive_ms_time_last =
							pi_props->archive_ms_time_now = timeGetTime();

			// Stop goto last frame
			pi_props->archive_goto_last_frame = FALSE;

			// Start archive updating again
			//
			// Always re-start when creating a new timeline - so that
			// a "live" archive can be detected - or not... 
			//
			pi_props->archive_updating = TRUE;       // Do want to start "live" updating
			pi_props->archive_update_start = FALSE;  // Don't want to start interval testing
			pi_props->archive_sync_message = TRUE;   // Do want first sync message
		}

		// Hmmm.... should return VS_OK...

	}

	// ELSE read HDL file failed
	else
	{
		// Report error....
		switch (result)
		{
			case VS_VNODE_READERROR:
			case VS_VNODE_UNDERFLOW:
			case VS_VNODE_OVERFLOW:
				pi_props->sys_output->Message("Read HDL Failed: Internal file error.");
				break;
			case VS_VNODE_OPENFILEFAILED:
				pi_props->sys_output->Message("Read HDL Failed: File could not be opened.");
				break;
			case VS_VNODE_READFILEFAILED:
				pi_props->sys_output->Message("Read HDL Failed: File could not be read.");
				break;
		}

	}

	// ---- create timeline failed
	return VS_ERROR;
}
