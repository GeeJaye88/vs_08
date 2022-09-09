////////////////////////////////////////////////////////////////////////////////

// ---------- r2_display_list_get_panel_handle.hpp ----------
/*!
\file r2_display_list_get_panel_handle.hpp
\brief
\author Gareth Edwards
*/


#include "../header/r2_model.h"


using namespace review_02;


////////////////////////////////////////////////////////////////////////////////


// ---------- GetDisplayListHandle ----------
/*!
\brief Setup a display list panel handle
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::InitPanelDisplayList(
		vs_system::PanelContext *panel_context
	)
{

	// ---- local
		INT result = 0;


	// ---- get display list index
		UINT handle_display_index;
		result = panel_context->GetDisplayListIndex(&handle_display_index);


	// ---- get display list handle
		INT handle_display_list;
		result = panel_context->GetDisplayListHandle(handle_display_index, &handle_display_list);


	// ---- lambda ------------------------------------------------------------
		auto set_display_status = [&](
				CHAR *cam_app_name,
				INT   handle_dspl_list,
				INT  *handle_gfx_con,
				UINT  group_id
			)
			{  
				CHAR app_name[VS_MAXCHARNAME];
				INT hr = pi_model->app_dependency->GetAppName(app_name);
				if ( !strcmp(app_name, cam_app_name) )
				{
					handle_display_list = handle_dspl_list;
					*handle_gfx_con = (INT)panel_context;
					INT display = 0; 
					result = pi_model->cam_props->GetProperty
							(
								group_id,
								0,
								vs_system::CamProps::DISPLAY,
								&display
							);
					if ( display == 0 ) panel_context->SetDisplayStatus(FALSE);
				}
			};


	// ---- Note: LOGIC only at RUNTIME


	// IF -- no display list handle -- THEN --
	if ( handle_display_list <= 0 )
	{

		// ---- get display list name
			CHAR display_name[VS_MAXCHARLEN];
			result = panel_context->GetDisplayListName(handle_display_index, display_name);


		// -------- set display list handle --------
		{

			// ---- UI
				if      ( !strcmp(display_name, "Title")          ) handle_display_list = pi_model->handle_dspl_title_button;
				else if ( !strcmp(display_name, "Frames")         ) handle_display_list = pi_model->handle_dspl_frame_button;
				else if ( !strcmp(display_name, "Context")        ) handle_display_list = pi_model->handle_dspl_context_button;
				else if ( !strcmp(display_name, "Timeline")       ) handle_display_list = pi_model->handle_dspl_timeline_button;
				else if ( !strcmp(display_name, "Clipline")       ) handle_display_list = pi_model->handle_dspl_clipline_button;
				else if ( !strcmp(display_name, "Navigation")     ) handle_display_list = pi_model->handle_dspl_control_button;


			// ---- objects
				else if ( !strcmp(display_name, "Annular")        ) handle_display_list = pi_model->handle_dspl_pano_annu;
				else if ( !strcmp(display_name, "Spherical")      ) handle_display_list = pi_model->handle_dspl_pano_spherical;
				else if ( !strcmp(display_name, "Lidar")          ) handle_display_list = pi_model->handle_dspl_pano_lidar;
				else if ( !strcmp(display_name, "Cylindrical")    ) handle_display_list = pi_model->handle_dspl_pano_cylindrical;
				else if ( !strcmp(display_name, "Panorama")       ) handle_display_list = pi_model->handle_dspl_pano_rect;
				else if ( !strcmp(display_name, "Panorama Front") ) handle_display_list = pi_model->handle_dspl_pano_front;
				else if ( !strcmp(display_name, "Panorama Back")  ) handle_display_list = pi_model->handle_dspl_pano_back;


			// ---- objects - toggled
				else if ( !strcmp(display_name, "Annular Exposure") )
					{
						set_display_status(
								"camera_03",
								pi_model->handle_dspl_pano_annu_exp_aoi,           // handle_display_list = 
								&pi_model->handle_pan_gfx_con_pano_annu_exp_aoi,
								vs_system::CamProps::ANNULAR_AOI
							);
					}

				else if ( !strcmp(display_name, "Annular Focus") )
					{
						set_display_status(
								"camera_03",
								pi_model->handle_dspl_pano_annu_foc_aoi,           // handle_display_list =
								&pi_model->handle_pan_gfx_con_pano_annu_foc_aoi,
								vs_system::CamProps::ANNULAR_FOCUS
							);
					}

				else if ( !strcmp(display_name, "Annular Graph") )
					{
						set_display_status(
								"camera_03",
								pi_model->handle_dspl_pano_annu_foc_grf,           // handle_display_list =
								&pi_model->handle_pan_gfx_con_pano_annu_foc_grf,
								vs_system::CamProps::ANNULAR_FOCUS
							);
					}


			// ---- img NOT USED at present
				else if ( !strcmp(display_name, "Img") ) handle_display_list = pi_model->handle_dspl_img;


			// --- 'I'mage || 'N'avigation
				else if ( result = Get_ImgPanel_HandleDisplayList(panel_context, display_name) )
				{
					handle_display_list = result;
				}


			// ---- animated export clip progress bar
				else if ( !strcmp(display_name, "ProgBar") )
				{
					handle_display_list = pi_model->handle_dspl_progress_bar;
					pi_model->progress_bar_panel_context = panel_context;
				}


			// ---- UI / info
				else if ( !strcmp(display_name, "Monitor")        ) handle_display_list = pi_model->handle_dspl_monitor;
				else if ( !strcmp(display_name, "Monitor_Image")  ) handle_display_list = pi_model->handle_dspl_monitor_image;
				else if ( !strcmp(display_name, "Monitor_Control")) handle_display_list = pi_model->handle_dspl_monitor_control;
				else if ( !strcmp(display_name, "Cam Head")       ) handle_display_list = pi_model->handle_dspl_tint_cam_head;
				else if ( !strcmp(display_name, "Cam Body")       ) handle_display_list = pi_model->handle_dspl_tint_cam_body;
				else if ( !strcmp(display_name, "Img Head")       ) handle_display_list = pi_model->handle_dspl_tint_img_head;
				else if ( !strcmp(display_name, "Img Body")       ) handle_display_list = pi_model->handle_dspl_tint_img_body;
				else if ( !strcmp(display_name, "Img Nav")        ) handle_display_list = pi_model->handle_dspl_img_nav_but;
				else if ( !strcmp(display_name, "Dashboard")      ) handle_display_list = pi_model->handle_dspl_dbz;
				else if ( !strcmp(display_name, "Spec")           ) handle_display_list = pi_model->handle_dspl_about;
				else if ( !strcmp(display_name, "Nest")           ) handle_display_list = pi_model->handle_dspl_nest;


			// ---- pop up
				else if (!strcmp(display_name, "GpsData"))
				{
					handle_display_list = pi_model->handle_dspl_gps_info;
					pi_model->gps_data_panel_context = panel_context;
					panel_context->SetDisplayStatus(FALSE);
				}
				else if (!strcmp(display_name, "Stream Gui"))
				{
					handle_display_list = pi_model->handle_dspl_stream_ui;
					pi_model->stream_control_panel_context = panel_context;
					panel_context->SetDisplayStatus(FALSE);
				}
				else if (!strcmp(display_name, "Stream Visible"))
				{
					handle_display_list = pi_model->handle_dspl_stream_visible;
					pi_model->stream_content_panel_context = panel_context;
					panel_context->SetDisplayStatus(FALSE);
				}


			// ---- hidden
				else if (!strcmp(display_name, "Stream Hidden"))
				{
					handle_display_list = pi_model->handle_dspl_stream_hidden;
				}
				else if (!strcmp(display_name, "Shared Hidden"))
				{
					handle_display_list = pi_model->handle_dspl_shared_hidden;
				}


			// ---- export clip progress bar
				else if (!strcmp(display_name, "ProgBar"))
				{
					handle_display_list = pi_model->handle_dspl_progress_bar;
					pi_model->progress_bar_panel_context = panel_context;
				}


			// ---- ooops
				else return VS_ERROR;

		} 

		// ---- set handle 
			result = panel_context->SetDisplayListHandle(
							handle_display_index,
							handle_display_list
						);


		// ---- set state
			vs_system::ElmContext *handle_display_list_context = NULL;
			gfx->GetElementContext(handle_display_list, &handle_display_list_context);

			result = panel_context->SetDisplayListState(
							handle_display_index,
							handle_display_list_context->GetState()
						);


		// ---- use panel name
			CHAR panel_name[VS_MAXCHARLEN];
			result = panel_context->GetName(panel_name);


		// ---- set panel group id
			// (e.g.  ""Cylindrical" == GROUP_ELEM_PANO_CYLIDRICAL)
			result = SetPanelGroupId(panel_context);


		// ---- set ptf panel id from "ptf name"
			// (e.g. "I09" == 9)
			result = Set_ImgOrPanoPanel_Selected(panel_context, panel_name);

	}


	// ELSE IF -- handle OK -- THEN
	/*
	else
	{
		// without this everything stops working....
		//
		// BUT don't understand why....
		// 
		INT display_list_handle;
		//result = panel_context->GetDisplayListHandle(0, &display_list_handle);
	}
	*/


	// ---- SPECIAL CASES ----


	// IF -- hidden -- THEN -- don't display --
		if (
				handle_display_list != pi_model->handle_dspl_stream_ui &&
				handle_display_list != pi_model->handle_dspl_stream_hidden &&
				handle_display_list != pi_model->handle_dspl_stream_visible
			)
		{
			if ( pi_model->stream_control_panel_context != NULL )
			{
				BOOL status = FALSE;
				pi_model->stream_control_panel_context->GetDisplayStatus(&status);
				if ( status ) return VS_ERROR;
			}
		}

	return handle_display_list;
}


// ---------- SetPanelGroupId ----------
/*!
\brief Set panel group id
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/

INT Model::SetPanelGroupId(
		vs_system::PanelContext *panel_context
	)
{

	// ---- local
		INT result = 0;


	// ---- get panel name
		CHAR panel_name[VS_MAXCHARLEN];
		result = panel_context->GetName(panel_name);
	

	// ---- get display list name
		CHAR display_name[VS_MAXCHARLEN];
		result = panel_context->GetDisplayListName(0, display_name);


	// ---- default group id
		panel_context->SetGroupId(GROUP_UNDEFINED);


	// ---- pano
		if (
					#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
					panel_name[0] == 'n'           ||  // ---- TBD - ignored - change to text ---
					#else
					panel_name[0] == 'N'           ||  // "Pano Nine"
					#endif
					panel_name[0] == 'Q'           ||  // "Quad"
					panel_name[0] == 'H'           ||  // "Hex"
					!strcmp(panel_name, "3D")      ||
					!strcmp(panel_name, "3D Full") ||
					!strcmp(panel_name, "3D Small")
			)
		{
			panel_context->SetGroupId(GROUP_ELEM_PANO_SPHERICAL);
		}
		else if ( panel_name[0] == 'S' )
		{
			panel_context->SetGroupId(GROUP_STREAM);
		}
		else if (
					!strcmp(panel_name, "2D") ||
					!strcmp(panel_name, "2D Solo") ||
					!strcmp(panel_name, "2D Nono")
				)
		{
			panel_context->SetGroupId(GROUP_ELEM_PANO_FULL);
		}
		else if (!strcmp(panel_name, "2D Front"))
		{
			panel_context->SetGroupId(GROUP_ELEM_PANO_FRONT);
		}
		else if (!strcmp(panel_name, "2D Back"))
		{
			panel_context->SetGroupId(GROUP_ELEM_PANO_BACK);
		}
		else if (
					!strcmp(panel_name, "Annular") ||
					!strcmp(panel_name, "Annular Small")
				)
		{
			panel_context->SetGroupId(GROUP_ELEM_PANO_ANNULAR);
		}
		else if (!strcmp(panel_name, "Cylindrical"))
		{
			panel_context->SetGroupId(GROUP_ELEM_PANO_CYLIDRICAL);
		}


	// ---- image ['I'aa##] or ['N'aa##]
		else if
			(
				( result = Get_ImgPanel_IndexDisplayList(display_name) ) != VS_ERROR
			)
		{
			panel_context->SetGroupId(GROUP_ELEM_IMAGE);
		}

	return VS_OK;
}

