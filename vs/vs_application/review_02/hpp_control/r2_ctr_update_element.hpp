////////////////////////////////////////////////////////////////////////////////

// ---------- r2_update_element.hpp ----------
/*!
\file r2_update_element.hpp
\brief 
\author Gareth Edwards
*/


#include "../header/r2_model.h"


using namespace review_02;


////////////////////////////////////////////////////////////////////////////////


// ---------- UpdateElement ----------
/*!
\brief Update picked element
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Control::UpdateElement(INT handle_control_obj, INT element_handle)
{

	// --- "this" instance
		Control* control = (Control*)handle_control_obj;


	// ---- pimp's
		PI_Timeline *pi_timeline = control->pi_timeline;
		PI_Camera   *pi_camera   = control->pi_camera;


	// ---- short cut
		vs_system::AppControl    *app_control    = control->app_control;
		vs_system::AppDependency *app_dependency = control->app_dependency;
		vs_system::GfxInterface  *gfx_interface  = app_dependency->GetGfxInterface();
		vs_system::SysOutput     *sys_output     = app_dependency->GetSysOutput();
		vs_system::SysInput      *sys_input      = app_dependency->GetSysInput();
		Model                    *model          = control->model;


	// ---- command parameters
		app_control->command_param_count = 0;


	// ---- element id'd
		app_control->group_id   = gfx_interface->GetElementGroupId(element_handle);
		app_control->element_id = gfx_interface->GetElementId(element_handle);


	// ---- local
		BOOL reset_id = TRUE;
		INT  result   = VS_OK;


	// ---- select event
		switch ( app_control->group_id )
		{

			case VS_OBJ_GEOMETRY: // For debug and testing
				switch ( app_control->element_id )
				{
					case VS_OBJ_PANO_RECT:
					case VS_OBJ_PANO_RECT_FRONT:
					case VS_OBJ_PANO_RECT_BACK:
						sys_output->Message("PANO_RECT");
						break;
					case VS_OBJ_PANO_ANNULAR:
						sys_output->Message("PANO_ANNULAR");
						break;
					case VS_OBJ_IMG:
						sys_output->Message("VS_OBJ_IMG");
						break;
					default:
					{
						CHAR msg[32];
						sprintf(msg, "VS_OBJ_GEOMETRY %d", app_control->element_id);
						sys_output->Message(msg);
						result = VS_ERROR;
					}
					break;
				}
				break;


			case TITLE_BUTTON_GROUP_ID:
				switch (app_control->element_id)
				{
					case TITLE_LOGO: // Launch web help page....
					{
						/*
						BOOL launch_web = FALSE;
						if (launch_web)
						{
							ShellExecute(
								NULL,
								"open",
								"http://www.observant-innovations.com",
								NULL,
								NULL,
								SW_SHOWNORMAL
							);
						}
						*/
					}
					break;
					case TITLE_EXIT: // Quit
						PostQuitMessage(0);
						break;
					default:
						result = VS_ERROR;
						break;
				}
				break;


			case STREAM_BUTTON_GROUP_ID:
				switch (app_control->element_id)
				{
					case STREAM_PLAY:

						pi_camera->stream = pi_camera->stream ? FALSE : TRUE;

						sys_output->Message(
								pi_camera->stream ?
								"STREAMING: Is ON" :
								"STREAMING: Is OFF"
							);

						if (pi_camera->remote_control)
						{
							pi_camera->remote_control = FALSE;
							model->UpdateStreamDisplayList(
									STREAM_BUTTON_GROUP_ID,
									STREAM_ONVIF_VPTZ,
									FALSE
								);
							sys_output->Message("ONVIF: PTZ is OFF");
						}

						break;

					case STREAM_ONVIF_VPTZ:

						pi_camera->remote_control = pi_camera->remote_control ? FALSE : TRUE;
						model->SetRemoteControlStatus(pi_camera->remote_control);

						sys_output->Message(
								pi_camera->remote_control ?
								"ONVIF: PTZ is ON" :
								"ONVIF: PTZ is OFF"
							);

						pi_camera->stream = pi_camera->remote_control;
						model->UpdateStreamDisplayList(
								STREAM_BUTTON_GROUP_ID,
								STREAM_PLAY,
								pi_camera->stream
							);

						sys_output->Message(
								pi_camera->stream ?
								"Streaming: ON" :
								"Streaming: OFF"
							);

						break;

					case STREAM_CLOSE:
					case STREAM_VIEW:
						{

							result = model->SetStreamDisplayStatus();
							if ( result == VS_OK )
							{
								control->app_view->SetPanelInteractionStatus(NULL, FALSE);
								control->app_view->SetPanelInteractionStatus("Stream", TRUE);
							}
							else
							{
								control->app_view->SetPanelInteractionStatus(NULL, TRUE);
								control->app_view->SetPanelInteractionStatus("Stream", FALSE);

								if (app_control->element_id == STREAM_CLOSE)
									gfx_interface->SetElementSwitch(
											STREAM_BUTTON_GROUP_ID,
											STREAM_VIEW,
											FALSE
										);

							}

						}
						break;

					case STREAM_1:
						result = model->SetStreamElementDisplay(STREAM_1);
						break;
					case STREAM_2:
						result = model->SetStreamElementDisplay(STREAM_2);
						break;
					case STREAM_3:
						result = model->SetStreamElementDisplay(STREAM_3);
						break;
					default:
						result = VS_ERROR;
						break;
				}
				break;


			case NEST_BUTTON_GROUP_ID:
				{

					// ---- switch all OFF
					INT handle = model->GetDisplayListHandleByName("Frames");
					model->SetGroupSwitchedOnOff(
						handle,
						FRAMES_BUTTON_GROUP_ID
					);


					// ---- select #2 
					UINT frame = 2;
					INT result = control->app_view->SelectFrame(frame);
					result = model->Set_PanoPanel_FrameSelected(frame);


					// ---- switch #2 ON
					model->SetElementProperty(
						handle,
						FRAMES_BUTTON_GROUP_ID,
						2,
						vs_system::ElmContext::PROPERTY_SWITCHED_ON,
						1
					);
				}
				break;


			case FRAMES_BUTTON_GROUP_ID:
				UpdateFrameGroup(handle_control_obj);
				break;


			case CAPTURE_BUTTON_GROUP_ID: // Capture
				switch (app_control->element_id)
				{
					case CAPTURE_LIVE:
						{
							pi_camera->live = pi_camera->live ? FALSE : TRUE;
						}
						break;
					case CAPTURE_SAVE:
						{
							pi_camera->archive_save = pi_camera->archive_save ? FALSE : TRUE;
						}
						break;
					case CAPTURE_REVIEW: // Launch review....
						{
							vs_system::SysContext *sys_context = app_dependency->GetSysContext();
							CHAR paf[VS_MAXCHARPATHLEN];
							sprintf(paf, "%s/Review.exe",
								sys_context->GetChar(sys_context->APP_FOLDER_PATH)
							);
							ShellExecute(
								NULL,
								"open",
								paf,
								NULL,
								NULL,
								SW_SHOWDEFAULT
							);
						}
						break;
					default:
						result = VS_ERROR;
						break;
				}
				break;


			case CONTEXT_BUTTON_GROUP_ID:
				switch (app_control->element_id)
				{
					case CONTEXT_PREVIOUS:
						pi_timeline->timeline_command = COMMAND_GOTO_PREVIOUS;
						break;
					case CONTEXT_NEXT:
						pi_timeline->timeline_command = COMMAND_GOTO_NEXT;
						break;
					case CONTEXT_FIRST:
						pi_timeline->timeline_command = COMMAND_GOTO_FIRST_MINUTE;
						break;
					case CONTEXT_LAST:
						pi_timeline->timeline_command = COMMAND_GOTO_LAST_FRAME;
						break;
					default:
						result = VS_ERROR;
						break;
				}
				break;


			case CREATE_BUTTON_GROUP_ID:
				switch (app_control->element_id)
				{
					case CREATE_TIMELINE: // Launch archive browser
						pi_timeline->timeline_command = COMMAND_CREATE_TIMELINE;
						break;
					default:
						result = VS_ERROR;
						break;
				}
				break;


			case TIMELINE_BUTTON_GROUP_ID: // Timeline segments
				switch (app_control->element_id)
				{
					case 1:
					default:
						{
							vs_system::ElmContext *element_context = NULL;
							INT result = gfx_interface->GetElementContext(element_handle, &element_context);
							BOOL switched_on = element_context->GetSwitchedOn();
							BOOL switched_on_prev = element_context->GetSwitchedOnPreviously();

							//CHAR msg[VS_MAXCHARLEN];
							//sprintf(msg, "switched_on %d %d", (INT)switched_on, (INT)switched_on_prev);
							//sys_output->Message(msg);

							if (switched_on_prev)
							{
								pi_timeline->timeline_command = COMMAND_DATETIME_GO_DOWN;
							}
							else
							{
								pi_timeline->timeline_command = COMMAND_GOTO;
								app_control->command_param_count = 1;
								app_control->command_param[0] = app_control->element_id;
							}

						}
						break;
				}
				break;


			case DATETIME_BUTTON_GROUP_ID: // Datetime
				switch (app_control->element_id)
				{
					case DATETIME_DECADE:
					case DATETIME_YEAR:
					case DATETIME_MONTH:
					case DATETIME_DAY:
					case DATETIME_HOUR:
					case DATETIME_MINUTE:
					case DATETIME_SECOND:
					case DATETIME_MSEC:
						pi_timeline->timeline_command = app_control->element_id + COMMAND_DATETIME_DECADE;
						break;
					default:
						result = VS_ERROR;
						break;
				}
				break;


			case NAVIGATION_BUTTON_GROUP_ID: // Navigation                                
				{
					switch (app_control->element_id)
					{
						case NAVIGATION_SPEED:
							pi_timeline->timeline_speed_mode =
								pi_timeline->timeline_speed_mode + 1 > 3 ?
								0 :
								pi_timeline->timeline_speed_mode + 1;
							model->Set_FrameIcons(
									NAVIGATION_BUTTON_INDEX,
									"SPEED",
									NAVIGATION_BUTTON_GROUP_ID,
									NAVIGATION_SPEED,
									0, 4 + pi_timeline->timeline_speed_mode
								);
							break;
						case NAVIGATION_PLAY:
							{
								pi_timeline->timeline_play_mode = pi_timeline->timeline_play_mode ? FALSE : TRUE;
								pi_timeline->timeline_command = pi_timeline->timeline_play_mode ? COMMAND_PLAY_START : COMMAND_PLAY_STOP;
							}
							break;
						case NAVIGATION_GOTO_FIRST:
							pi_timeline->timeline_command = COMMAND_GOTO_FIRST;
							break;
						case NAVIGATION_CLIP_START:
							pi_timeline->timeline_command = COMMAND_CLIP_INPOINT;
							break;
						case NAVIGATION_CLIP_END:
							pi_timeline->timeline_command = COMMAND_CLIP_OUTPOINT;
							break;
						case NAVIGATION_CLIP_CLEAR:
							pi_timeline->timeline_command = COMMAND_CLIP_CLEAR;
							break;
						case NAVIGATION_CLIP_LOOP:
							pi_timeline->timeline_command = COMMAND_CLIP_LOOP;
							break;
						case NAVIGATION_CLIP_ARCHIVE:
							pi_timeline->timeline_command = COMMAND_CLIP_ARCHIVE;
							break;
						case NAVIGATION_LAST_MINUTE:
							pi_timeline->timeline_command = COMMAND_GOTO_LAST_MINUTE;
							break;
						case NAVIGATION_PANO:
							{
								// For camera - pi_camera->panophoto = TRUE;
								pi_timeline->timeline_command = COMMAND_PANO_SAVE;
							}
							break;

						case NAVIGATION_LIVE:
							{
								pi_timeline->timeline_live_mode = pi_timeline->timeline_live_mode ? FALSE : TRUE;
								pi_timeline->timeline_command = pi_timeline->timeline_live_mode ? COMMAND_LIVE_START : COMMAND_LIVE_STOP;
							}
							break;
						default:
							result = VS_ERROR;
							break;
					}
				}
				break;


			case MONITOR_BUTTON_GROUP_ID: // Monitor Control                        
				{
					switch (app_control->element_id)
					{
						case 1:
							{
								pi_camera->live = pi_camera->live ? FALSE : TRUE;
							}
							break;
						case 2:
							{
								pi_camera->loop_hard_start = pi_camera->loop_hard_start ? FALSE : TRUE;
							}
							break;
						default:
							result = VS_ERROR;
							break;
					}
				}
				break;


			case LIDAR_BUTTON_GROUP_ID:                            
				{
					#if VS_LIDAR
					switch (app_control->element_id)
					{
						case 8:
							{
								model->Display_PanoLidar(&pi_camera->lidar_visible_mode);
							}
							break;
						case 9:
							{
								model->Rotate_PanoLidar(&pi_camera->lidar_rotate_mode);
							}
							break;
						default:
							result = VS_ERROR;
							break;
					}
					#endif
				}
				break;

			case PTF_BUTTON_GROUP_ID: // Pan, Tilt and FOV                                
				{
					result = UpdateElementPTF(handle_control_obj, app_control->element_id);
				}
				break;


			case IPP_BUTTON_BOOST_GROUP_ID:
				{
					pi_timeline->timeline_command = COMMAND_BOOST_COLOR_FX;
					switch (app_control->element_id)
					{

						case IPP_BOOST_BOX:
							pi_camera->fx_boost = pi_camera->fx_boost + 1 > 2 ? 0 : pi_camera->fx_boost + 1;
							model->Set_FrameIcons(
								FRAMES_BUTTON_INDEX,
								"BoostBox",
								IPP_BUTTON_BOOST_GROUP_ID,
								IPP_BOOST_BOX,
								4, 5 + pi_camera->fx_boost
							);
							break;

						case IPP_BOOST_01: pi_camera->fx_boost = 0; break;
						case IPP_BOOST_02: pi_camera->fx_boost = 1; break;
						case IPP_BOOST_03: pi_camera->fx_boost = 2; break;
						default:
							pi_timeline->timeline_command = COMMAND_NONE;
							result = VS_ERROR;
							break;
					}
				}
				break;


			case IPP_BUTTON_COLOR_GROUP_ID:
				{
					pi_timeline->timeline_command = COMMAND_BOOST_COLOR_FX;
					switch (app_control->element_id)
					{
						case IPP_COLOUR_BOX:
							pi_camera->fx_mode = pi_camera->fx_mode + 1 > 3 ? 0 : pi_camera->fx_mode + 1;
							model->Set_FrameIcons(
								FRAMES_BUTTON_INDEX,
								"ColourBox",
								IPP_BUTTON_COLOR_GROUP_ID,
								IPP_COLOUR_BOX,
								3, 4 + pi_camera->fx_mode
							);
							break;

						case IPP_COLOUR : pi_camera->fx_mode = vs_system::ImgLutProcess::COLOUR; break;
						case IPP_MONO   : pi_camera->fx_mode = vs_system::ImgLutProcess::MONO;   break;
						case IPP_GREEN  : pi_camera->fx_mode = vs_system::ImgLutProcess::GREEN;  break;
						case IPP_SEPIA  : pi_camera->fx_mode = vs_system::ImgLutProcess::SEPIA;  break;

						default:
							pi_timeline->timeline_command = COMMAND_NONE;
							result = VS_ERROR;
							break;
					}
				}
				break;


			case IMG_BUTTON_GROUP_ID:
				result = UpdateImgPtzPanelsNavButtons(control, app_control->element_id);
				break;


			case ELEM_GROUP_CAM_HEAD_LID:
			case ELEM_GROUP_IMG_HEAD_LID:
				result = UpdateTintHead(control);
				break;


			case ELEM_GROUP_ACQUIRE_LID:
			case ELEM_GROUP_COLOUR_LID:
			case ELEM_GROUP_GAINMAS_LID:
			case ELEM_GROUP_GAINCOL_LID:
			case ELEM_GROUP_SETTINGS_LID:
			case ELEM_GROUP_PANO_LID:
			case ELEM_GROUP_AEC_LID:
			case ELEM_GROUP_FOCUS_LID:
			#ifdef FREDFRED
			case TINT_HEAD_AEC_LID:
			#endif
				result = UpdateTintCamBody(control, (INT)app_control->group_id, element_handle);
				break;


			case TINT_TAB_IMGPTZ_LID:
			case TINT_TAB_IMGLOC_LID:
			case TINT_TAB_IMGFX_LID:
				result = UpdateTintImgBody(control, (INT)app_control->group_id, element_handle);
				break;


			case VS_OBJ_IR_IMG_BUTTONS:
			case VS_OBJ_MV_IMG_BUTTONS:
			case VS_OBJ_VM_IMG_BUTTONS:
				result = model->Set_ImgPanel_ButtonsId(app_control->group_id, app_control->element_id);
				break;


			default:
				reset_id = FALSE;
				break;
		}


	// ---- reset id ----
	if ( reset_id )
	{
		app_control->group_id   = 0;
		app_control->element_id = 0;
	}


	// ---- reset mouse ----
	sys_input->ResetMouse();


	return result;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- UpdateElementPTF ----------
/*!
\brief Update picked element
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK

// note

	Only invoked in Control::UpdateElement, e.g.

		case PTF_BUTTON_GROUP_ID: // Pan, Tilt and FOV
		{
			result = UpdateElementPTF(handle_control_obj, app_control->element_id);
		}
		break;

*/
INT Control::UpdateElementPTF(INT handle_control_obj, UINT element_id)
{

	// --- "this" instance
		Control* control = (Control*)handle_control_obj;


	// ---- local
		INT result = 0;
		Model                    *model          = control->model;
		vs_system::ObjConfig     *obj_config     = model->GetObjConfig();
		vs_system::AppDependency *app_dependency = control->app_dependency;


	// ---- ok
		result = VS_OK;
		switch ( element_id )
		{

			case PTF_ZOOM_IN:
			case PTF_ZOOM_OUT:
			{

				UINT ptf_id = 0;
				result = obj_config->GetPanoPTFId(&ptf_id);
				

				CHAR msg[32];
				sprintf(msg, "--- %d\n", ptf_id);
				OutputDebugString(msg);

				FLOAT pan, tilt, zoom;
				result = obj_config->GetPanoPTFById(ptf_id, &pan, &tilt, &zoom);

				// ---- ZOOM

				#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
				{
					zoom = element_id == PTF_ZOOM_IN ? zoom + 1 : zoom - 1;
					zoom = zoom < 1 ? 1 : zoom;
					result = obj_config->SetPanoPTFById(ptf_id, pan, tilt, zoom);
					result = model->Update_ImgPanel_Config(ptf_id);
				}
				#else
				{
					zoom = element_id == PTF_ZOOM_IN ? zoom - 1 : zoom + 1;
					zoom = zoom < 1 ? 1 : zoom;
					result = obj_config->SetPanoPTFById(ptf_id, pan, tilt, zoom);
				}
				#endif

			}
			break;

			case PTF_RESET:
				{
					result = obj_config->SetPanoDefault();
					UINT ptf_id = 0;
					result = obj_config->GetPanoPTFId(&ptf_id);

					#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
					result = model->Update_ImgPanel_Config(ptf_id);
					#endif
				}
				break;

			default:
				result = VS_ERROR;
				break;
	}


	return result;
}
