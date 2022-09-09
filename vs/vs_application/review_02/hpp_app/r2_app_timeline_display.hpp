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


// ---------- TimelineDisplay ----------
/*!
\brief Timeline
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT application::TimelineDisplay()
{

	// WATERFALL LOGIC FOR THIS METHOD
	// ===============================
	// 1. SET "IS" STATUS
	// 2. INITIALISE COMMAND
	// 3. HANDLE CREATE ARCHIVE COMMAND
	// 4. UPDATE
	// 4.1 SET MILLISECOND INCREMENT
	// 4.2 COMMAND
	// 4.2 GUARD THIS MINUTE
	// 4.3 UPDATE THIS MINUTE
	// 4.3.1 UPDATE
	// 4.3.1.1 REPORT
	// 4.3.1.2 SET "LIVE" INCREMENT DELAY GUARD
	// 4.3.1.3 INCREMENT
	// 4.3.2 QUEUE IMAGE
	// 4.3.3 READ IMAGE
	// 4.3.4 UPDATE DATE TIME LIST
	// 4.3.5 INITIALISE TIMELINE LISTS
	// 4.3.6 HIGHLIGHT CURRENT TIMELINE LIST
	// 4.3.7 SET PREVIOUS/NEXT BUTTON BITMASK
	// 4.3.8 SET FIRST/LAST BUTTON BITMASK
	// 4.3.9 UPDATE PREVIOUS/NEXT & FIRST/LAST BUTTON STATES
	// 4.3.10 UPDATE CLIP CLEAR/ARCHIVE BUTTON STATES
	// 4.3.11 UPDATE ARCHIVE EXPORT BUTTON STATE
	// 4.3.12 UPDATE PANOPHOTO BUTTON STATE
	// 4.3.14 "LIVE " ARCHIVE UPDATE

	// Local
	INT result = VS_OK;


	// ---- 1. SET "IS" STATUS ----

	BOOL timeline_is_available = img_timeline->IsTimelineAvailable();
	BOOL timeline_is_created =
		img_timeline->IsTimelineCreated() && timeline_is_available;
	BOOL this_minute_is_available = img_timeline->IsThisMinuteAvailable();


	// ---- 2. INITIALISE COMMAND ----

	// Get command and command parameters
	UINT command_param_count = 0;
	INT  command_param[COMMAND_BUFFER_MAX];
	UINT timeline_command = control->GetTimelineCommand(&command_param_count, command_param);


#	define ARCHIVE_UPDATING


	// ---- 3. HANDLE CREATE ARCHIVE COMMAND ----

	// IF -- last command was create new archive -- THEN
	if ( pi_props->timeline_new )
	{

		// Note: Do not invoke pi_props->Initialise();, as
		// current playback states, etc., are retained....
		//
		if (timeline_is_created && this_minute_is_available)
		{

			// zap archive
			pi_props->archive_last_filename = "";
			pi_props->archive_goto_last_frame = FALSE;

			// zap timeline
			pi_props->time_millisecond_increment = 0;
			pi_props->timeline_update = FALSE;

			pi_props->sys_output->Message("STOP: After creating a new timeline.");

			// zap clip
			pi_props->clip_inpoints_bitmask = 0;
			pi_props->clip_inpoints_set = FALSE;
			pi_props->ResetButtonStates(FALSE, control, model);

			// test - leave as a reminder for now...
			// model->ReportObjConfigPanoConfig();

			// zap command
			timeline_command = COMMAND_CREATE_TIMELINE;

			// Lastly - flip "new timeline" flag
			pi_props->timeline_new = FALSE;
		}

		// ELSE -- timeline is not ready, so return
		else
		{
			return VS_OK;
		}

	}

	// ELSE -- catch create archive command
	else
	{
		switch (timeline_command)
		{
			case COMMAND_CREATE_TIMELINE:
				{
					// get archive path
					CHAR archive_paf[VS_MAXCHARPATHLEN];
					result = GetArchivePath(archive_paf, VS_MAXCHARPATHLEN);

					// IF -- path ok -- THEN
					if ( result == VS_OK )
					{
						// store
						strcpy(pi_props->archive_paf, archive_paf);

						// create timeline
						INT create_timeline_result = CreateTimeline(archive_paf);

						// set "NEW timeline flag
						pi_props->timeline_new = TRUE;

						// return OK - Because archive is being created
						return VS_OK;
					}
				}
				break;

			default:
				break;
		}
	}


	// ---- 4. UPDATE ----
	if ( timeline_is_created && this_minute_is_available )
	{

		// IF -- timeline_created flag set -- THEN -- force GOTO
		//
		// Note: timeline_created is set TRUE at run time
		// if CreateTimeline is invoked ( logic works
		// even if run time CreateTimeline fails )
		//
		if ( pi_props->timeline_created )
		{
			timeline_command = COMMAND_CREATE_TIMELINE;
		}


		// ---- 4.1. SET MILLISECOND INCREMENT ----

		// (Re)set delay target already defined in ctor
		// For debug only - once all OK this block can be deleted

		BOOL last_minute = img_timeline->IsLastMinute();

		pi_props->time_millisecond_delay_target =
				pi_props->archive_updating && last_minute
					? 1000
						: 0;

		if (pi_props->time_millisecond_delay_target < 1)
			pi_props->time_millisecond_delay = 0;

		// set NOW
		pi_props->time_millisecond_last = pi_props->time_millisecond_now;
		pi_props->time_millisecond_now = timeGetTime();
		pi_props->time_millisecond_increment =
				pi_props->time_millisecond_now -
					pi_props->time_millisecond_last;


		// ---- 4.2 COMMAND ----


		// IF -- command -- THEN
		if ( timeline_command != COMMAND_NONE )
		{

			// flags for post command intialisation and actions
			BOOL post_command_init_goto     = FALSE;
			BOOL post_command_zero_counters = FALSE;
			BOOL post_command_init_update   = FALSE;
			BOOL post_command_stop_live     = FALSE;
			BOOL post_command_stop_play     = FALSE;


			// process command
			switch ( timeline_command )
			{

				// set timeline list index
				case COMMAND_DATETIME_GO_DOWN:
					{
						pi_props->datetime_index++;
						pi_props->datetime_index =
								pi_props->datetime_index < 8 ?
								pi_props->datetime_index :
								7;
						pi_props->minute_changed = TRUE;

						// NOTE: Update "navigation_button_list" radio button list
						// after the "goto" logic has been "handled"....!!!!

						model->UpdateDateTimeList(pi_props->datetime_index);

					}
					break;


				case COMMAND_DATETIME_DECADE:
				case COMMAND_DATETIME_YEAR:
				case COMMAND_DATETIME_MONTH:
				case COMMAND_DATETIME_DAY:
				case COMMAND_DATETIME_HOUR:
				case COMMAND_DATETIME_MINUTE:
				case COMMAND_DATETIME_SECOND:
				case COMMAND_DATETIME_MILLISECOND:
					{
						pi_props->datetime_index = timeline_command - COMMAND_DATETIME_DECADE + 1;
						pi_props->minute_changed = TRUE;
					}
					break;


				case COMMAND_CLIP_INPOINT:
				case COMMAND_CLIP_OUTPOINT:
					{

						// ---- nail IN or OUT ----
						if ( timeline_command == COMMAND_CLIP_INPOINT )
							model->UpdateButtonSwitch(
								NAVIGATION_BUTTON_GROUP_ID,
								NAVIGATION_CLIP_START,
								TRUE
							);
						else
								model->UpdateButtonSwitch(
								NAVIGATION_BUTTON_GROUP_ID,
								NAVIGATION_CLIP_END,
								TRUE
							);


						// ---- set start or end bit value ----
						UINT bitval = timeline_command == COMMAND_CLIP_INPOINT ?
							img_timeline->GOTO_CLIP_INPOINT :
								img_timeline->GOTO_CLIP_OUTPOINT;


						// IF -- clip START or END bit mask set -- THEN -- GOTO
						if ( pi_props->clip_inpoints_bitmask & bitval )
						{

							BOOL report_ms = FALSE;
							if ( report_ms )
							{
								for (UINT i = 1; i<DATETIME_MAX_STACK; i++)
										printf("%d%s",
											pi_props->clip_datetime_stack[bitval - 1][i],
												i < DATETIME_MAX_STACK - 1 ? ", " : "\n"
									);
							}


							// ---- goto in/out point ----
							result = TimelineClipGoto(bitval);
							if ( result == VS_OK )
							{

								// IF -- updating -- THEN -- bounce
								if ( pi_props->timeline_update )
								{

									// force minute update
									pi_props->minute_changed = TRUE;

									// delay "live" archive playback
									pi_props->time_millisecond_delay = pi_props->time_millisecond_delay_target;

								}
								// ELSE -- set post_comands to STOP --
								else
								{
									post_command_init_goto = TRUE;
									post_command_init_update = TRUE;
									post_command_zero_counters = TRUE;
									post_command_stop_play = TRUE;
									pi_props->minute_changed = TRUE;
								}

							}

							// ---- whatever -- STOP! ----
							post_command_stop_play = TRUE;

						}

						// ELSE -- set IN OR OUT point
						else
						{

							// index from 1 or 2 to 0 or 1, for either START or END
							UINT index = bitval - 1;

							// copy clip START or END from current datetime
							for (UINT i = 1; i<DATETIME_MAX_STACK; i++)
							{
								pi_props->clip_datetime_stack[index][i] = pi_props->datetime_value[i];
							}


							// ---- calculate and store clip milliseconds ----

							// store current datetime
							UINT tmp[DATETIME_MAX_STACK];
							img_timeline->GetDateTime(tmp);

							// set clip datetime
							img_timeline->SetDateTime(pi_props->clip_datetime_stack[index]);

							// get datetime ms
							LLONG ms = 0;
							result = img_timeline->GetDateTimeMilliseconds_3(
										7,
										&ms
									);

							// store datetime ms as current clip inpoint/outpoint ms
							pi_props->clip_ms_inpoints[index] = ms;

							// restore current datetime
							img_timeline->SetDateTime(tmp);

							// set clip bitmask
							pi_props->clip_inpoints_bitmask =
								pi_props->clip_inpoints_bitmask & bitval ?
									pi_props->clip_inpoints_bitmask :
										pi_props->clip_inpoints_bitmask + bitval;

						}


						// ---- set clip flag ----
						pi_props->clip_inpoints_set =
							pi_props->clip_inpoints_bitmask & img_timeline->GOTO_CLIP_INPOINT  &&
								pi_props->clip_inpoints_bitmask & img_timeline->GOTO_CLIP_OUTPOINT;


						// ---- set updated flag ----
						pi_props->clip_updated = TRUE;


						// IF -- clip set -- THEN nail loop icon --
						if (pi_props->clip_inpoints_set)
						{
							model->UpdateButtonSwitch(NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_CLIP_LOOP, FALSE);
						}

					}
					break;


				case COMMAND_CLIP_CLEAR:
					{
						model->UpdateButtonSwitch(NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_CLIP_START, FALSE);
						model->UpdateButtonSwitch(NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_CLIP_END, FALSE);
						model->UpdateButtonSwitch(NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_CLIP_LOOP, FALSE);
						pi_props->ResetClipProperties();
						pi_props->clip_updated = TRUE;
					}
					break;


				case COMMAND_CLIP_LOOP:
					if (pi_props->clip_inpoints_set)
					{
						pi_props->clip_loop = pi_props->clip_loop ? FALSE : TRUE;
						model->UpdateButtonSwitch(
							NAVIGATION_BUTTON_GROUP_ID,
							NAVIGATION_CLIP_LOOP,
							pi_props->clip_loop
							);
					}
					break;


				case COMMAND_CLIP_ARCHIVE:
					if (
							pi_props->clip_inpoints_set &&
							pi_props->archive_export_data.thread_complete
						)
					{
						pi_props->sys_output->Message("EXPORT: Begin...");
						result = CreateArchiveExportThread();
					}
					else
					{
						pi_props->sys_output->Message("EXPORT: Busy... ");
					}
					break;


				case COMMAND_BOOST_COLOR_FX:
					{
						#ifdef USE_IMG_ARKIVE
						img_arkive->Read_SetFxBoostMode(control->GetFXBoostMode());
						img_arkive->Read_SetFxColourMode(control->GetFXColourMode());
						#else
						img_archive->SetFxBoost(control->GetFXBoostMode());
						img_archive->SetFxMode(control->GetFXColourMode());
						#endif

						if (!pi_props->timeline_update)
						{
							post_command_init_goto = TRUE;
							post_command_init_update = TRUE;
						}
					}
					break;


				case COMMAND_GOTO_FIRST: // NOT USED
					{
						// Goto
						result = img_timeline->GotoContextualFirst(pi_props->datetime_index);

						// Post command
						post_command_init_goto = TRUE;
						post_command_init_update = TRUE;
						post_command_zero_counters = TRUE;
						post_command_stop_play = TRUE;
					}
					break;


				case COMMAND_GOTO_LAST: // NOT USED
					{
						// Goto
						result = img_timeline->GotoContextualLast(pi_props->datetime_index);

						// Post command
						post_command_init_goto = TRUE;
						post_command_init_update = TRUE;
						post_command_zero_counters = TRUE;
						post_command_stop_play = TRUE;
					}
					break;


				case COMMAND_GOTO_BACKWARD:  // NOT USED
				case COMMAND_GOTO_FORWARD:   // NOT USED
				case COMMAND_GOTO_PREVIOUS:
				case COMMAND_GOTO_NEXT:
				case COMMAND_GOTO:
					{

						// ---- select specific local action ---- 
						BOOL skip_post_command_stuff = FALSE;
						result = VS_ERROR;
						switch (timeline_command)
						{
						case COMMAND_GOTO_BACKWARD:
							result = TimelineGoto(img_timeline->GOTO_BACKWARD, -1);
							break;
						case COMMAND_GOTO_FORWARD:
							result = TimelineGoto(img_timeline->GOTO_FORWARD, -1);
							break;
						case COMMAND_GOTO_PREVIOUS:
							result = TimelineGoto(img_timeline->GOTO_PREVIOUS, -1);
							break;
						case COMMAND_GOTO_NEXT:
							result = TimelineGoto(img_timeline->GOTO_NEXT, -1);
							break;
						case COMMAND_GOTO:
							result = TimelineGoto(img_timeline->GOTO, command_param[0]);
							break;
						}


						// IF -- goto succeeds -- THEN
						if (result == VS_OK)
						{

							// IF -- updating -- THEN -- bounce
							if (pi_props->timeline_update)
							{
								// force minute update
								pi_props->minute_changed = TRUE;

								// delay "live" archive playback
								pi_props->time_millisecond_delay = pi_props->time_millisecond_delay_target;

							}
							// ELSE -- set post_comands to STOP
							else
							{
								// post command
								post_command_init_goto = TRUE;
								post_command_init_update = TRUE;
								post_command_zero_counters = TRUE;

								// minute (probably) changed - so force
								pi_props->minute_changed = TRUE;

								// stop play
								post_command_stop_play = TRUE;
							}
						}

					}
					break;

				case COMMAND_GOTO_BOUNCE: // NOT USED - now same as GOTO
					{
						// select
						result = TimelineGoto(img_timeline->GOTO, command_param[0]);

						// IF -- goto succeeds -- THEN -- update minute
						if (result == VS_OK)
						{
							pi_props->minute_changed = true;
						}

					}
					break;


				// Select "GOTO LAST MINUTE LAST FRAME"
				case COMMAND_GOTO_LAST_FRAME:
					{

						#ifdef ARCHIVE_UPDATING

						// IF -- goto last frame -- THEN
						if (pi_props->timeline_update)
						{

							// goto
							//result = img_timeline->GotoLastMinuteLastFrame(0);
							result = img_timeline->GotoFirstMinuteFirstFrame();

							// IF -- "live" archive -- THEN
							if (pi_props->archive_updating)
							{

								// initialise counters
								pi_props->playback_second_count = (LONG)pi_props->sys_context->GetTimeElapsed() / 1000;
								pi_props->frames_per_second = 0;
								pi_props->images_per_second = 0;

								// delay "live" archive playback
								pi_props->time_millisecond_delay = pi_props->time_millisecond_delay_target;
							}
							else
							{
								// post command
								post_command_init_goto = TRUE;
								post_command_init_update = TRUE;
								post_command_zero_counters = TRUE;
								post_command_stop_play = TRUE;
							}

						}
						// ELSE -- goto AND set post_commands to STOP
						else

						#endif

						{
							// goto
							result = img_timeline->GotoLastMinuteLastFrame(0);

							// post command
							post_command_init_goto = TRUE;
							post_command_init_update = TRUE;
							post_command_zero_counters = TRUE;
							post_command_stop_play = TRUE;
						}

					}
					break;


				// Timeline created...
				case COMMAND_CREATE_TIMELINE:
					{
						pi_props->timeline_created = FALSE;
						pi_props->minute_changed = true;
					}

					// THEN run ON to force "GOTO FIRST FRAME"
					//
					//    ===
					//    ===
					// =========
					//  =======
					//   =====
					//    ===
					//     =


				// select "GOTO FIRST FRAME"
				case COMMAND_GOTO_FIRST_MINUTE:
					{
						// goto
						result = img_timeline->GotoFirstMinuteFirstFrame();

						// IF -- goto succeeds -- THEN
						if (result == VS_OK)
						{

							// IF -- updating -- THEN -- bounce
							if (pi_props->timeline_update)
							{
								pi_props->minute_changed = TRUE;

								// delay "live" archive playback
								pi_props->time_millisecond_delay = pi_props->time_millisecond_delay_target;

							}
							// ELSE ---- set post_comands to STOP
							else
							{
								post_command_init_goto = TRUE;
								post_command_init_update = TRUE;
								post_command_zero_counters = TRUE;
								post_command_stop_play = TRUE;
							}
						}

					}
					break;

				// select "PLAY START"
				case COMMAND_PLAY_START:
					{
						// update...
						post_command_init_update = TRUE;

						// initialise counters
						pi_props->playback_second_count = (LONG)pi_props->sys_context->GetTimeElapsed() / 1000;
						pi_props->frames_per_second = 0;
						pi_props->images_per_second = 0;

						// initialise flags
						pi_props->archive_goto_last_frame = FALSE;

						// initialise "live" archive second count
						pi_props->archive_sec_update = 0;

					}
					break;

				case COMMAND_PANO_SAVE:
					{
						if ( pi_panophoto_data.thread_complete )
						{
							// flag to create thread ArchiveRead
							pi_props->create_pano_image = TRUE;

							// goto
							post_command_init_goto = TRUE;
							post_command_init_update = TRUE;
							post_command_zero_counters = TRUE;
							post_command_stop_play = TRUE;
						}
					}
					break;

				// select "PLAY STOP" - a "natural" (e.g. user selected) stop
				case COMMAND_PLAY_STOP:
					{
						pi_props->sys_output->Message("STOP: After STOP playback command.");
						pi_props->timeline_update = FALSE;
						pi_props->archive_last_filename = "";
						pi_props->archive_goto_last_frame = FALSE;
						pi_props->time_millisecond_increment = 0;
					}
					break;

				// select "LIVE START"
				case COMMAND_LIVE_START:
					{
						pi_props->sys_output->MessageOk(
									"Could not find a camera!\n\n"
									"Please connect a camera and restart this application.\n\n"
									"Click OK to exit."
								);
						// was exit(0)....
						return VS_ERROR;
					}
					break;

				// select "LIVE START"
				case COMMAND_LIVE_STOP:
					{
						;
					}
					break;

				// ignore
				default:
					break;

			}


			// ---- handle post command intialisation and actions ----
			//
			// Note: To avoid errors in repeatedly used instruction "groups" 
			//

			if (post_command_init_goto)
			{
				this_minute_is_available = FALSE;
				pi_props->timeline_goto = TRUE;
			}

			if (post_command_init_update)
			{
				pi_props->timeline_update = TRUE;
				pi_props->archive_last_filename = "";
				pi_props->time_millisecond_increment = 0;
			}

			if (post_command_zero_counters)
			{
				pi_props->playback_second_count = 0;
				pi_props->frames_per_second = 0;
				pi_props->images_per_second = 0;
			}

			if (post_command_stop_live)
			{
				if (control->GetTimelineLive())
				{
					model->UpdateButtonSwitch(NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_LIVE, FALSE);
					control->SetTimelineLive(FALSE);
				}
			}

			if (post_command_stop_play)
			{
				if (control->GetTimelinePlay())
				{
					model->UpdateButtonSwitch(NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_PLAY, FALSE);
					control->SetTimelinePlay(FALSE);
				}
			}

		}


		// ---- 4.3 GUARD THIS MINUTE !!!!! ----


		// IF -- commands result in "this" minute is not available -- THEN -- return OK
		//
		// SO: Refresh boolean variable this_minute_is_available!!!
		//
		// This logic is a VERY important - it "guards" all downstream logic
		// from trying to access a minute that is being updated...
		//
		this_minute_is_available = img_timeline->IsThisMinuteAvailable();
		if ( !this_minute_is_available ) return VS_OK;


		// ---- 4.3 UPDATE THIS MINUTE ----


		// IF -- this minute is available -- THEN
		//
		// Note: Requires this test as all goto commands WILL
		// make this minute unavailable!!!! As the flag state
		// is persistent, this logic block is executed when
		// the "this" minute IS available...
		//
		if ( this_minute_is_available )
		{

			// ---- 4.3.1 UPDATE ----
			BOOL archive_queue = FALSE;
			if ( pi_props->timeline_update )
			{

				// ---- 4.3.1.1 REPORT ----

				// IF -- next second -- THEN -- reset counters
				LONG now = (LONG)pi_props->sys_context->GetTimeElapsed() / 1000;
				if ( now != pi_props->playback_second_count )
				{

					// report
					{

						// chop off last part of image filename
						CHAR archive_last_filename[VS_MAXCHARLEN];
						CHAR char_src[VS_MAXCHARLEN];
						strcpy(archive_last_filename, "[ last image ");
						INT dest_len = strlen(archive_last_filename);
						if ( pi_props->archive_last_filename.compare("") != 0 )
						{

							// chop off last part of image
							strcpy(char_src, pi_props->archive_last_filename.c_str());
							INT len = strlen(char_src);
							BOOL copy_from_char_src = FALSE;
							for (INT si = 0; si < len; si++)
							{
								if (char_src[si] == '\\') { char_src[si] = '/';  copy_from_char_src = TRUE; }
								if (copy_from_char_src) archive_last_filename[dest_len++] = char_src[si];
							}
							archive_last_filename[dest_len++] = ' ';
							archive_last_filename[dest_len++] = ']';
							archive_last_filename[dest_len++] = '\0';
						}
						else
						{
							strcat(archive_last_filename, "none ]");
						}


						// report
						CHAR msg[VS_MAXCHARLEN];
						sprintf(
									msg,
									"Review: %dips %s",
									pi_props->images_per_second,
									archive_last_filename
								);
						pi_props->sys_output->Message(msg);

					}


					// reset
					pi_props->playback_second_count = now;
					pi_props->frames_per_second = 0;
					pi_props->images_per_second = 0;


					// IF -- archive starting AND archive updating -- THEN -- report archive stuff
					if (
							pi_props->archive_update_start &&
							pi_props->archive_updating
						)
					{

						// increment
						pi_props->archive_sec_update++;

						// report
						CHAR msg[VS_MAXCHARLEN];
						INT ms_difference = 0;
						INT result = img_timeline->GetLastMinuteMSDifference(&ms_difference);
						if ( result == VS_OK )
							sprintf(
								msg,
								"Archive:  E:%ds  DL:%dms  DF:%dms",
								pi_props->archive_sec_update,
								(INT)pi_props->time_millisecond_delay,
								ms_difference
							);
						else
							sprintf(
								msg,
								"Archive:  E:%ds  DL:%dms  DF:(not same minute)",
								pi_props->archive_sec_update,
								(INT)pi_props->time_millisecond_delay
							);
						pi_props->sys_output->Message(msg);

					}

				}


				// ---- 4.3.1.2 SET "LIVE" INCREMENT DELAY GUARD ----


#				ifdef ARCHIVE_UPDATING

				// IF -- updating a "live" archive -- THEN
				if ( pi_props->archive_updating )
				{

					// IF -- delay difference less than delay target -- THEN -- increment
					INT ms_difference = 0;
					INT result = img_timeline->GetLastMinuteMSDifference(&ms_difference);
					if ( result == VS_OK && ms_difference < pi_props->time_millisecond_delay_target )
					{
						pi_props->time_millisecond_delay += pi_props->time_millisecond_increment / 2;
					}


					// IF -- delayed -- THEN
					if ( pi_props->time_millisecond_delay > 0 )
					{

						// decrement delay
						pi_props->time_millisecond_delay -= pi_props->time_millisecond_increment;

						// catch negative delay
						pi_props->time_millisecond_delay =
							pi_props->time_millisecond_delay < 0
								? 0
									: pi_props->time_millisecond_delay;

						// zap increment
						pi_props->time_millisecond_increment = 0;
					}

				}

#				endif


				// ---- 4.3.1.3 INCREMENT ----


				// ---- increment  ----

				// get minute handle before increment
				INT this_minute_handle = NULL;
				INT get_handle_result = img_timeline->GetThisMinuteHandle(&this_minute_handle);


				// then...
				FLOAT speed[] = { 1.0f, 2.0f, 5.0f, 10.0f };
				FLOAT speed_mult = speed[ control->GetTimelineSpeedMode() ];
				INT increment_result = img_timeline->Increment(
							pi_props->timeline_goto
								? 0
									: (INT)(pi_props->time_millisecond_increment * speed_mult + 0.5)
							);


				// clip
				BOOL clip = pi_props->timeline_update &&
						pi_props->clip_inpoints_set &&
							pi_props->clip_loop;

				if ( clip )
				{

					// ---- current datetime ----

					// store
					UINT  stack[DATETIME_MAX_STACK];
					for (UINT i = 1; i<DATETIME_MAX_STACK; i++)
						stack[i] = pi_props->datetime_value[i];
					UINT tmp[DATETIME_MAX_STACK];
					img_timeline->GetDateTime(tmp);

					// set
					img_timeline->SetDateTime(stack);


					// ---- get ms ----
					LLONG ms = 0;
					INT get_result = img_timeline->GetDateTimeMilliseconds_3(
								7,
								&ms
							);


					// ---- loop ----
					if (
							increment_result == img_timeline->LAST_MINUTE_STOP ||
							ms >= pi_props->clip_ms_inpoints[1]
						)
					{

						// force timeline increment
						increment_result = img_timeline->INCREMENTED;

						// set datetime == inpoint, then goto
						for (UINT i = 1; i < DATETIME_MAX_STACK; i++)
							pi_props->datetime_value[i] = pi_props->clip_datetime_stack[0][i];
						UINT datetime_index = pi_props->datetime_index;
						pi_props->datetime_index = DATETIME_MSEC;
						INT res = TimelineGoto(img_timeline->GOTO, COMMAND_GOTO);

						// tidy, and notify that minute MIGHT have changed
						pi_props->datetime_index = datetime_index;
						pi_props->minute_changed = TRUE;

					}
					else
					{
						// restore current datetime
						img_timeline->SetDateTime(tmp);
					}

				}


				// ---- get minute handle after increment ----
				INT new_minute_handle = NULL;
				INT get_this_minute_handle = img_timeline->GetThisMinuteHandle(&new_minute_handle);


				// IF -- minute handle NOT same as before increment -- THEN -- minuted changed
				if ( new_minute_handle != this_minute_handle )
					pi_props->minute_changed = TRUE;


				// IF -- incremented -- THEN -- queue image
				if (increment_result == img_timeline->INCREMENTED)
				{
						archive_queue = TRUE;
				}
				// ELSE -- STOP -- in order of most likely occurence
				else if
					(
						// command to STOP ( Note: GOOD - Natural play stop )
						increment_result == img_timeline->STOP ||

						// clip OR last minute STOP ( Note: GOOD - Natural play end )
						increment_result == img_timeline->CLIP_END_STOP ||
						increment_result == img_timeline->LAST_MINUTE_STOP ||

						// timeline failed ( Note: BAD - NOT natural play end, as this OR next minute fails! )
						increment_result == img_timeline->THIS_NOT_AVAILABLE ||
						increment_result == img_timeline->NEXT_NOT_AVAILABLE
					)
				{

					// set button and control flag FALSE
					model->UpdateButtonSwitch(NAVIGATION_BUTTON_GROUP_ID, NAVIGATION_PLAY, FALSE);
					control->SetTimelinePlay(FALSE);

					// stop queue
					archive_queue = FALSE;

					// STOP update
					pi_props->timeline_update = FALSE;
					pi_props->ResetButtonStates(FALSE, control, model);

					// report
					CHAR msg[VS_MAXCHARLEN];
					sprintf(
								msg,
								"STOP: After timeline increment ends or fails (%d).",
								increment_result
							);
					pi_props->sys_output->Message(msg);
				}
			}


			// ---- 4.3.2 QUEUE IMAGE ----
			if ( archive_queue )
			{

				// IF -- filename ok -- THEN
				INT get_filename_result = img_timeline->GetFilename(pi_props->archive_queue_filename);
				if ( get_filename_result == VS_OK )
				{

					// IF -- filename not same as last -- THEN
					INT compare_result = pi_props->archive_queue_filename.compare(pi_props->archive_last_filename);
					if ( compare_result != 0 )
					{

						// IF -- queued ok -- THEN
						INT queue_result = ArchiveQueue();
						if ( queue_result == VS_OK )
						{

							// store "this" filename
							pi_props->archive_last_filename = pi_props->archive_queue_filename;

							// stop queuing
							archive_queue = FALSE;

							// IF -- goto -- THEN -- stop goto -- AND -- stop update ----
							if ( pi_props->timeline_goto )
							{

								// STOP goto
								pi_props->timeline_goto = FALSE;

								// STOP update
								pi_props->timeline_update = FALSE;
								pi_props->ResetButtonStates(FALSE, control, model);

								// report
								CHAR msg[VS_MAXCHARLEN];
								sprintf(msg, "STOP: After queued goto (%d).", result);
								pi_props->sys_output->Message(msg);

							}
						}
					}
				}
			}


			// ---- 4.3.3 READ IMAGE ----
			BOOL archive_read = pi_props->archive_read_index <= pi_props->archive_queue_index;
			if ( archive_read )
			{

				// IF -- read ok -- THEN -- increment counter
				INT read_result = ArchiveRead();
				if ( read_result == VS_OK )
				{
					pi_props->images_per_second++;
				}


				// IF -- equal -- THEN -- reset
				if ( pi_props->archive_read_index == pi_props->archive_queue_index )
				{
					pi_props->archive_queue_index = 0;
					pi_props->archive_read_index = 0;
				}
			}


			// ---- 4.3.4 UPDATE DATE TIME LIST ----
			{

				// IF -- read filename -- AND -- null -- THEN --
				if ( pi_props->archive_last_filename.compare("") != 0 )
				{

					// update date time text
					result = model->UpdateDateTimeList(
							pi_props->datetime_text,
							pi_props->archive_last_filename
						);


					// update date time value
					for (UINT i = 1; i<8; i++)
						pi_props->datetime_value[i] = atoi(pi_props->datetime_text[i].c_str());


					// IF -- timeline_this_second changed -- THEN -- copy and set flag
					INT timeline_this_second = atoi(pi_props->datetime_text[6].c_str());
					if ( timeline_this_second != pi_props->timeline_last_second )
					{
						pi_props->timeline_last_second = timeline_this_second;
						pi_props->second_changed = TRUE;
					}
				}
			}


			// ---- 4.3.5 INITIALISE TIMELINE LISTS ----
			if (pi_props->minute_changed || pi_props->second_changed || pi_props->clip_updated)
			{
				// reset
				pi_props->minute_changed = FALSE;
				pi_props->second_changed = FALSE;
				pi_props->clip_updated = FALSE;

				// IF -- get timeline date time list stride (e.g. always 4 at present !!!) -- THEN
				result = img_timeline->GetDateTimeStride(
								pi_props->datetime_index,
								&pi_props->datetime_stride
							);

				//BOOL this_minute_available = img_timeline->IsThisMinuteAvailable();

				if ( result == VS_OK )
				{
					// IF -- get date time list OK -- THEN -- set model timeline
					//
					// Note: # of items dependent on index, from Archive (0)
					// to 7 (MSEC), and stores id, value, mod and MSEC
					//
					// Note: This is where the values are modulus "whatever"
					// to create the displayed values.
					//
					result = img_timeline->GetDateTimeList(
									pi_props->datetime_index,
									&pi_props->datetime_items,
									pi_props->datetime_list
								);
					if ( result == VS_OK )
					{
						UINT value[8];
						result = img_timeline->GetDateTimeValue(value);

						// possible BUG BUG BUG - note: What bug??? GE, this is not very helpful!
						result = model->UpdateTimelineList(
										pi_props->datetime_index,
										value,
										pi_props->clip_inpoints_set,
										&pi_props->clip_ms_inpoints[0],
										pi_props->datetime_items,
										pi_props->datetime_stride,
										pi_props->datetime_list
									);

						pi_props->datetime_list_init = TRUE;

					}
				}
			}


			// ---- 4.3.6 HIGHLIGHT CURRENT TIMELINE LIST ----
			if ( pi_props->datetime_list_init )
			{
				result = model->HighlightTimelineList(
								pi_props->datetime_index,
								pi_props->datetime_value[pi_props->datetime_index],
								pi_props->datetime_items,
								pi_props->datetime_stride,
								pi_props->datetime_list
							);
			}


			// ---- 4.3.7 GET PREVIOUS/NEXT BUTTON BITMASK ----
			INT previous_next_state = 0;
			if ( pi_props->datetime_index > 0 )
				previous_next_state = img_timeline->GetPrevNextState(pi_props->datetime_index);


			// ---- 4.3.8 GET FIRST/LAST BUTTON BITMASK ----
			INT first_last_state = 0;
			if ( pi_props->datetime_index > 0 )
				first_last_state = img_timeline->GetFirstLastState();


			// ---- 4.3.9 UPDATE PREVIOUS/NEXT & FIRST/LAST BUTTON STATES ----
			model->UpdateContextualState(
						timeline_is_created,
						previous_next_state,
						first_last_state
					);


			// ---- 4.3.10 UPDATE CLIP CLEAR/ARCHIVE BUTTON STATES ----
			model->UpdateClipState(
						timeline_is_created,
						pi_props->clip_inpoints_bitmask
					);


			// ---- 4.3.11 UPDATE ARCHIVE EXPORT BUTTON STATE ----
			if (
					pi_props->archive_export_data.thread_complete &&
					pi_props->archive_export_data.button_on == FALSE
				)
			{
				INT handle = model->GetDisplayListHandleByName("Navigation");
				INT result = model->SetElementProperty(
						handle,
							NAVIGATION_BUTTON_GROUP_ID,
							NAVIGATION_CLIP_ARCHIVE,
							vs_system::ElmContext::PROPERTY_BEHAVIOUR,
							vs_system::ElmContext::BEHAVIOUR_UNCLUNK
						);
				result = model->UpdateProgBarDisplayList(FALSE, 0);
				pi_props->archive_export_data.button_on = TRUE;
			}
			else
			{
				if ( pi_props->archive_export_data.total_files != 0 )
				{
					result = model->UpdateProgBarDisplayList(
						TRUE,
							(FLOAT)pi_props->archive_export_data.total_processed /
								(FLOAT)pi_props->archive_export_data.total_files
						);
				}
			}


			// ---- 4.3.12 UPDATE PANOPHOTO BUTTON STATE ----
			if (
					pi_panophoto_data.thread_complete &&
					pi_panophoto_data.button_on == FALSE
				)
			{
				INT handle = model->GetDisplayListHandleByName("Frames");
				INT result = model->SetElementProperty(
							handle,
							NAVIGATION_BUTTON_GROUP_ID,
							NAVIGATION_PANO,
							vs_system::ElmContext::PROPERTY_BEHAVIOUR,
							vs_system::ElmContext::BEHAVIOUR_UNCLUNK
						);
				pi_panophoto_data.button_on = TRUE;
			}


			// ---- 4.3.14 "LIVE " ARCHIVE UPDATE ----
#			ifdef ARCHIVE_UPDATING
			{
				result = ArchiveUpdate();
			}
#			endif

		}

	}

	return VS_OK;
}
