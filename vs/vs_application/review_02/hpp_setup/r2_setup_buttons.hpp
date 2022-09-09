////////////////////////////////////////////////////////////////////////////////

// ---------- r2_setup_buttons.hpp ----------
/*!
\file r2_setup_buttons.hpp
\brief
\author Gareth Edwards
*/


#pragma once

#include "../header/r2_model.h"

using namespace review_02;


// ---------- SetupButtons ----------
/*!
\brief Setup button display lists
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupButtons()
{

	// This method is really important.
	//
	// The ptf_panel_id value is used to intialise the
	// selected PTF id, selected frame buttons status, 
	// button set and button icon texture.
	//
	// Note: The appropriate "home" frameset is selected
	// in the parent application->Setup() - which invokes
	// this method via the Setup() method - after the
	// this method has been invoked.


	// ---- local
		INT result = VS_OK;


	// ---- set "HOME" frame
		UINT home_frame_selected = Get_HomeFrameSelected() - 1;


	// ---- setup "PTF" panel id
		UINT id = 2;
		result = GetObjConfig()->SetPanoPtfSelectedById(id);
		result = Set_ImgPanel_ActiveId(id);
		result = Set_ImgOrPanoPanel_IconSelected(id);


	// ---- select button set
		#if defined(VS_REVIEW_02)

			review_02_frames_button_list[home_frame_selected].switched_on = 1;
			button_list_size[1] = sizeof(review_02_frames_button_list) / sizeof(button_element_descriptor);
			button_list[1]  = review_02_frames_button_list;

		#elif defined(VS_REVIEW_04)

			review_04_frames_button_list[home_frame_selected].switched_on = 1;
			button_list_size[1] = sizeof(review_04_frames_button_list) / sizeof(button_element_descriptor);
			button_list[1] = review_04_frames_button_list;

		#elif defined(VS_CAMERA_03)

			camera_03_frames_button_list[home_frame_selected].switched_on = 1;
			button_list_size[1] = sizeof(camera_03_frames_button_list) / sizeof(button_element_descriptor);
			button_list[1] = camera_03_frames_button_list;

		#elif defined(VS_CAMERA_05)

			camera_05_frames_button_list[home_frame_selected].switched_on = 1;
			button_list_size[1] = sizeof(camera_05_frames_button_list) / sizeof(button_element_descriptor);
			button_list[1] = camera_05_frames_button_list;

		#endif

	return VS_OK;
}


// ---------- SetupCameraFramesButtonsList ----------
/*!
\brief Setup button display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupCameraFramesButtonsList(
		UINT group_id,
		UINT elm_id,
		UINT property,
		UINT value
	)
{

	// ---- get application "name"
		CHAR app_name[VS_MAXCHARNAME];
		INT hr = pi_model->app_dependency->GetAppName(app_name);


	// ---- select button set
		#if defined(VS_CAMERA_03)

			button_element_descriptor *cfbl = camera_03_frames_button_list;
			UINT num_bed = sizeof(camera_03_frames_button_list) / sizeof(button_element_descriptor);

		#elif defined(VS_CAMERA_05)

			button_element_descriptor *cfbl = camera_05_frames_button_list;
			UINT num_bed = sizeof(camera_05_frames_button_list) / sizeof(button_element_descriptor);
		
		#else

			button_element_descriptor *cfbl = camera_05_frames_button_list;
			UINT num_bed = sizeof(camera_05_frames_button_list) / sizeof(button_element_descriptor);

		#endif


	// ---- process
		for (UINT i = 0; i < num_bed; i++)
		{

			UINT this_group_id = (cfbl+i)->group_id;
				this_group_id = group_id == 0 ? 0 : this_group_id;
		
			UINT this_elm_id = (cfbl + i)->id; 

			if ( this_group_id == group_id && this_elm_id == elm_id )
			{
				switch (property)
				{
					case 2: // behaviour
						break;
					case 3: // switched_on
						(cfbl + i)->switched_on = value;
						break;
					default:
						break;
				}
			}

		}

	return VS_OK;
}


// ---------- SetupButtonDisplayLists ----------
/*!
\brief Setup button display lists
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupButtonDisplayLists()
{

	// ---- local
		INT result;
		INT hr;


	// ---- FOR each list
	for ( UINT list_index=0; list_index<BUTTON_MAX_LISTS; list_index++ )
	{

		// ---- FOR each element ----
		UINT total_elements = button_list_size[list_index];
		for ( UINT element=0; element<total_elements; element++ )
		{

			// ---- param
				FLOAT p[] = {
					(FLOAT)element,
					(FLOAT)list_index,
					(FLOAT)button_list[list_index][element].tex_atlas_page
				};


			// ---- button param
				INT  list_handle = handles[list_index];
				BYTE element_version_bitmask = button_list[list_index][element].version_bitmask;
				BYTE bitmask  = 1;
				UINT group_id = button_list[list_index][element].group_id;
				UINT id       = button_list[list_index][element].id;


			// ---- archiving
				if ( group_id == CAPTURE_BUTTON_GROUP_ID && id == CAPTURE_SAVE )
				{
					button_list[list_index][element].switched_on = (UINT)pi_model->archive_save_on_at_runtime;
				}


			// ---- streaming
				if ( group_id == STREAM_BUTTON_GROUP_ID && id == STREAM_VIEW )
				{
					if ( pi_model->streaming_on_at_runtime || pi_model->streaming_and_onvif_on_at_runtime )
					{
						button_list[list_index][element].switched_on = TRUE;
						button_list[list_index][element].tex_icon_col = 2;
						button_list[list_index][element].tex_icon_row = 7;
					}
				}


			// ---- for each version...
				for ( UINT version = 0; version < 3; version++ )
				{

					// ---- version ?
						if ( element_version_bitmask & bitmask )
						{
							// Add Element - handle, name, group_id, id, version, callback
							//
							// Note: Parameter array p passed with element index and list_index
							// which are used to intitlialise elements from the buttons lists 
							//
							// Note version == bitmask, which is version ^2
							//
							CHAR t[256];
							strcpy(t, button_list[list_index][element].name.c_str());
							hr = gfx->AddElement(
									list_handle,
									t,
									group_id,
									id,           // was element,
									version,
									&Callback_Button_Configure
								);
							// "HERE"
							result = gfx->SetElementParameters(hr, sizeof(p), p);
						}

						bitmask *= 2;
				}

		}
	}

	return VS_OK;
}
