////////////////////////////////////////////////////////////////////////////////

// ---------- r2_ctr_tint.hpp ----------
/*!
\file r2_ctr_tint.hpp
\brief 
\author Gareth Edwards
*/


#pragma once

using namespace review_02;


// ---------- UpdateTintHead ----------
/*!
\brief Update a Tint Head
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Control::UpdateTintHead(
		Control *control
	)
{

	// --- local
		INT result = VS_OK;


	// ---- select
		//
		//  if selected then
		//   - zap named display list forcing display list re-initialisation
		//   - select associated tab body
		//
		switch (control->app_control->group_id)
		{
			case ELEM_GROUP_CAM_HEAD_LID:
				{
					control->app_view->SetPanelDisplayListHandle("Cam Body", 0);
					control->model->Tint_Cam_UpdateBody(control->app_control->element_id);
				}
				break;
			case ELEM_GROUP_IMG_HEAD_LID:
				{
					control->app_view->SetPanelDisplayListHandle("Img Body", 0);
					control->model->Tint_Img_UpdateBody(control->app_control->element_id);
				}
				break;
			default:
				break;
		}

	return VS_OK;
}


// ---------- UpdateTintCamBody ----------
/*!
\brief Update a Tint Img Body
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Control::UpdateTintCamBody(
		Control *control,
		INT group_id,
		INT element_handle
	)
{

	// ---- local objects
		vs_system::AppDependency *app_dependency = control->app_dependency;
		review_02::Model         *model          = control->model;
		vs_system::AppControl    *app_control    = control->app_control;
		INT result = VS_OK;


	// ---- get callbacks
		vs_system::TintCallback dmc;
		switch ( group_id )
		{
			case ELEM_GROUP_ACQUIRE_LID:
				result = model->Tint_Acquire_GetCallbacks(&dmc);
				break;
			case ELEM_GROUP_COLOUR_LID:
				result = model->Tint_Colour_GetCallbacks(&dmc);
				break;
			case ELEM_GROUP_GAINMAS_LID:
				result = model->Tint_GainMas_GetCallbacks(&dmc);
				break;
			case ELEM_GROUP_GAINCOL_LID:
				result = model->Tint_GainCol_GetCallbacks(&dmc);
				break;
			case ELEM_GROUP_SETTINGS_LID:
				result = model->Tint_Settings_GetCallbacks(&dmc);
				break;
			case ELEM_GROUP_PANO_LID:
				result = model->Tint_Pan_GetCallbacks(&dmc);
				break;
			case ELEM_GROUP_AEC_LID:
				result = model->Tint_Aec_GetCallbacks(&dmc);
				break;
			case ELEM_GROUP_FOCUS_LID:
				result = model->Tint_Focus_GetCallbacks(&dmc);
				break;
				
			#ifdef FREDFRED
			case TINT_HEAD_AEC_LID:
				{
					float fred = 2;
				}
				return VS_ERROR;
			#endif

			default:
				return VS_ERROR;
		}


	// ---- local cam props object
		vs_system::CamProps *cam_props = NULL;
		result = model->GetCamProps(&cam_props);
		FLOAT h_offset_before = 0;


	// ---- local
		UINT element_id = app_control->element_id;


	// ---- pre
		switch ( group_id )
		{
			case ELEM_GROUP_PANO_LID:
				{
					result = cam_props->GetProperty(
							vs_system::CamProps::ROI,
							0,
							vs_system::CamProps::H_OFFSET,
							&h_offset_before
						);
				}
				break;
			case ELEM_GROUP_AEC_LID:
				{
					;
				}
				break;
			case ELEM_GROUP_FOCUS_LID:
				{
					;
				}
				break;
			default:
				break;
		}


	// ---- invoke model update
		if ( element_handle != 0 )
			dmc.callback_update_model(
					(INT)model,
					element_id
				);
		else
			float fred = 2;


	// ---- invoke text update ----
		dmc.callback_update_element(
				(INT)model,
				element_id
			);


	// ---- post
		switch ( group_id )
		{
			case ELEM_GROUP_PANO_LID:
				{
					FLOAT h_offset_after = 0;
					result = cam_props->GetProperty(
							vs_system::CamProps::ROI,
							0,
							vs_system::CamProps::H_OFFSET,
							&h_offset_after
						);
					if (h_offset_after != h_offset_before)
					{
						control->pi_camera->update_offset = TRUE;
					}
					control->pi_camera->exposure_must_be_updated = TRUE;
				}
				break;
			case ELEM_GROUP_AEC_LID:
				{
					control->pi_camera->exposure_must_be_updated = TRUE;
				}
				break;
			case ELEM_GROUP_FOCUS_LID:
				{
					;
				}
				break;
			default:
				break;
		}

	return VS_ELM_SETUP_NONE;
}


// ---------- UpdateTintImgBody ----------
/*!
\brief Update a Tint Img Body
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Control::UpdateTintImgBody(
		Control *control,
		INT group_id,
		INT element_handle
	)
{

	// ---- local objects
		vs_system::AppDependency *app_dependency = control->app_dependency;
		review_02::Model         *model          = control->model;
		vs_system::AppControl    *app_control    = control->app_control;

	// ---- local
		UINT element_id = app_control->element_id;
		INT  result = VS_OK;

	// ---- get callbacks
		vs_system::TintCallback dmc;
		switch ( group_id )
		{
			case TINT_TAB_IMGPTZ_LID:
				model->Tint_ImgPtz_GetCallbacks(&dmc);
				break;
			case TINT_TAB_IMGLOC_LID:
				model->Tint_ImgLoc_GetCallbacks(&dmc);
				break;
			case TINT_TAB_IMGFX_LID:
				model->Tint_ImgFx_GetCallbacks(&dmc);
				break;
			default:
				return VS_ERROR;
		}

	// ---- invoke model update
		if (element_handle != 0)
		{
			dmc.callback_update_model(
				(INT)model,
				element_id
			);
		}
		else
		{
			// TBD warn!
			;
		}

	// ---- invoke elment (& therefore text) update ----
		dmc.callback_update_element(
				(INT)model,
				element_id
			);

	return VS_ELM_SETUP_NONE;
}


// ---------- UpdateTintImgText ----------
/*!
\brief Update tabbed interface image text and values
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Control::UpdateTintImgText(
		Control* control
	)
{
	INT result = UpdateTintImgBody(control, TINT_TAB_IMGPTZ_LID, 0);
	    result = UpdateTintImgBody(control, TINT_TAB_IMGLOC_LID, 0);
	return VS_OK;
}
