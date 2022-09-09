////////////////////////////////////////////////////////////////////////////////

// ---------- r2_setup_ui.hpp ----------
/*!
\file r2_setup_geometry.hpp
\brief
\author Gareth Edwards
*/


#pragma once

#include "../header/r2_model.h"

using namespace review_02;


// ---------- SetupUIButtons ----------
/*!
\brief Setup UI ( old buttons ) display lists 
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupUIButtons()
{
	
	// ---- local
	INT hr = VS_OK;


	// ---- add "Title" ( TITLE_BUTTON_INDEX ) list
		pi_model->handle_dspl_title_button = gfx->AddDisplayList("Title");


	// ---- add "Frames" ( FRAMES_BUTTON_INDEX ) list
		pi_model->handle_dspl_frame_button = gfx->AddDisplayList("Frames");


	// ---- add "Context" ( CONTEXT_BUTTON_INDEX ) list
		pi_model->handle_dspl_context_button = gfx->AddDisplayList("Context");


	// ---- add "Timeline" ( TIMELINE_BUTTON_INDEX ) list
		{

			button_element_descriptor c = {
					"", "C",
					vs_system::ElmContext::BEHAVIOUR_CLICK, 0,
					0, 0,
					TIMELINE_BUTTON_GROUP_ID, 0,
					BACKROUND_VERSION | OUTLINE_VERSION,
					0, 0, 0, 0, 0, 0
				};
			for (UINT i = 0; i < button_list_size[TIMELINE_BUTTON_INDEX]; i++)
			{
				timeline_button_list[i] = c;
				timeline_button_list[i].handle_element_context[0] = NULL;
				timeline_button_list[i].handle_element_context[1] = NULL;
				timeline_button_list[i].handle_element_context[2] = NULL;
			}
			pi_model->handle_dspl_timeline_button = gfx->AddDisplayList("Timeline");


			// ---- set "pick switched on radio button" flag
				//
				// 	To allow for radio button parent timeline items to
				//  click to child timeline items...
				// 
				vs_system::ElmContext *dspl_list_context = NULL;
				gfx->GetElementContext(
					pi_model->handle_dspl_timeline_button,
					&dspl_list_context
				);
				dspl_list_context->SetPickOnRadioButton(FALSE);

		}


	// ---- add "Clipline" ( CLIPLINE_BUTTON_INDEX ) list
		{
			button_element_descriptor d = {
					"", "D",
					vs_system::ElmContext::BEHAVIOUR_NONE, 0,
					0, 0,
					CLIPLINE_BUTTON_GROUP_ID, 0,
					BACKROUND_VERSION | OUTLINE_VERSION,
					0, 0, 0, 0, 0, 0
				};
			for (UINT i = 0; i < button_list_size[CLIPLINE_BUTTON_INDEX]; i++)
			{
				clipline_button_list[i] = d;
			}
			pi_model->handle_dspl_clipline_button = gfx->AddDisplayList("Clipline");
		}


	// ---- add "Navigation" ( NAVIGATION_BUTTON_INDEX ) list
		pi_model->handle_dspl_control_button = gfx->AddDisplayList("Navigation");


	// ---- initialise HANDLES list
		INT h[BUTTON_MAX_LISTS] =
		{
			pi_model->handle_dspl_title_button,      // "Title"      ( TITLE_BUTTON_INDEX )
			pi_model->handle_dspl_frame_button,      // "Frames"     ( FRAMES_BUTTON_INDEX )
			pi_model->handle_dspl_context_button,    // "Context"    ( CONTEXT_BUTTON_INDEX )
			pi_model->handle_dspl_timeline_button,   // "Timeline"   ( TIMELINE_BUTTON_INDEX )
			pi_model->handle_dspl_clipline_button,   // "Clipline"   ( CLIPLINE_BUTTON_INDEX )
			pi_model->handle_dspl_control_button     // "Navigation" ( NAVIGATION_BUTTON_INDEX )
		};
		for ( UINT i=0; i<BUTTON_MAX_LISTS; i++ ) handles[i] = h[i];


	// ---- setup button display lists
		hr = SetupButtonDisplayLists();

	return VS_OK;
}


// ---------- SetupUIGeometry ----------
/*!
\brief Setup GUI geometry display lists
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupUIGeometry()
{

	// ---- local
	INT result;


	// ---- monitor list
		pi_model->handle_dspl_monitor = gfx->AddDisplayList("Monitor");
			result = SetupMonitorDisplayList();
			FLOAT lut[16]; for (INT i = 0; i < 16; i++) lut[i] = 0;
			result = UpdateMonitorDisplayList(lut, lut);


	// ---- monitor image list
		pi_model->handle_dspl_monitor_image = gfx->AddDisplayList("Monitor_Image");
			result = gfx->AddElement(
					pi_model->handle_dspl_monitor_image,
					"2D PanoAnnular",
					VS_OBJ_GEOMETRY,
					VS_OBJ_PANO_ANNULAR,
					&pi_model->obj_config->Callback_PanoAnnular_Configure
				);


	// ---- monitor control list
		pi_model->handle_dspl_monitor_control = gfx->AddDisplayList("Monitor_Control");
			result = SetupMonitorControlDisplayList();


	// ---- nest list
		pi_model->handle_dspl_nest = gfx->AddDisplayList("Nest");
			result = SetupNestDisplayList();


	// ---- tabbed interfaces
		result = SetupTintCam();
		result = SetupTintImg();

	// ---- image interfaces
		pi_model->handle_dspl_img_nav_but = gfx->AddDisplayList("Img Nav");
			result = SetupImgNavigation();


	// ---- progress bar list
		pi_model->handle_dspl_progress_bar = gfx->AddDisplayList("ProgressBar");
			result = SetupProgressBar();


	// ---- dashboard list
		pi_model->handle_dspl_dbz = gfx->AddDisplayList("Dashboard");
			result = SetupCameraDashboard();
			result = SetupCameraDashboardDials();
			result = SetupCameraDashboardIcons();
			result = SetupCameraDashboardText();
			result = SetupCameraDashboardGps();
			result = SetupCameraDashboardSys();


	// ---- about list
		pi_model->handle_dspl_about = gfx->AddDisplayList("About");
			result = SetupAboutDashboard();
			 

	// ---- stream list
		pi_model->handle_dspl_stream_ui = gfx->AddDisplayList("Stream");
			result = SetupStreamDisplayList();

	return VS_OK;
}

