////////////////////////////////////////////////////////////////////////////////

// ---------- s2_tint_select.hpp ----------
/*!
\file s2_tint_select.hpp
\brief Interface for Model class Tint select methods
\author Gareth Edwards
*/

#pragma once

using namespace surface_02;


// ---------- TintBodySelect ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK

\note Keypad focus set to selected Tint parameter

*/
INT Model::TintBodySelect( 
		UINT body_id
	)
{

	// ---- set selected body
		INT result = pi_model->tint_setng->SetSelectedBody(body_id);


	// ---- set body handle
		pi_model->handle_dspl_tint_body = pi_model->tint_setng->GetBodyHandle(body_id);


	// ---- set body param group
		INT param_group = 0;
		switch (body_id)
		{
			case TINT_BODY_ACQUIRE_LID:
				param_group = PARAM_GROUP_ACQUIRE;
				break;

			case TINT_BODY_EXPLORE_LID:
				break;

			case TINT_BODY_FUNCTION_LID:
				param_group = PARAM_GROUP_FUNCTION;
				break;

			default:
				return VS_ERROR;
		}

	// ---- parameter group ? - set keypad 
		if ( param_group != 0 )
		{
			SetParamGroup(param_group);
			Kpad_UpdateByParameter((INT)this, GetParamGroup());
		}

	return VS_OK;
}


// ---------- TintBodyUpdate ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK

note invoked (can only be!) after TintBodySelect as this
     method sets the selected body & body handle

*/
INT Model::TintBodyUpdate(
		UINT body_id
	)
{

	// ---- local
		INT result = 0;


	// ---- set visible objects per selected body
		model::Surface_02 *surface_02 = s_pi_model->surface_02;
		surface_02->SetVisibleObjects(body_id, surface_02);


	// ---- lambda


	// ---- iinitialise and select acquired last lumina (3D) surface
		auto update_acquire_model_lumina_last = [&]()
		{
					UINT element_id = Tint_Acquire_GetElemLidLuminaLast();
					result = Tint_Acquire_UpdateModel(
							(INT)this,
							element_id
						);
		};

	// ---- initialise tint acquire selected source surface and tint text label
		auto update_acquire_model_and_element = [&]()
		{
				result = Tint_Acquire_UpdateModel(
						(INT)this,
						Tint_Acquire_GetElemLidSourceSurf()
					);
				result = Tint_Acquire_UpdateElement(
						(INT)this,
						Tint_Acquire_GetElemLidSourceSurf()
					);
		};

	// ---- switch on selected loi
		auto update_explore = [&]()
		{
			UINT ccm = s_pi_model->GetCameraControlMode();
			if ( ccm != vs_system::AppKamera::Mode::TARGET )
			{
				UINT element_id = Tint_Explore_GetElemLidUpdateFirst() + ccm;
				result = Tint_Explore_UpdateModel(
							(INT)this,
							element_id
						);
			}
			else
			{
				UINT tid_object_id = s_pi_model->GetTargetObjectId();
				UINT target_loi_id = surface_02->GetTargetLoiIndex();
				UINT element_id = Tint_Explore_GetElemLidTargetFirst() + target_loi_id;
				result = Tint_Explore_UpdateModel(
							(INT)this,
							element_id
						);
			}
		};

	// ---- switch off all loi chrome
		auto update_no_target = [&]()
		{
			surface_02->SetTargetLoiVisible(FALSE);
			Tint_Explore_Switch((INT)this,
					Tint_Explore_Param::GROUP_LID_CAMERA_LOI,
					Tint_Explore_Param::ELEMENT_LID_TARGET_LOI,
					FALSE
				);
			surface_02->SetTargetLoiRulerVisible(FALSE);
			Tint_Explore_Switch((INT)this,
					Tint_Explore_Param::GROUP_LID_CAMERA_LOI,
					Tint_Explore_Param::ELEMENT_LID_TARGET_RULER,
					FALSE
				);
			surface_02->SetTargetLoiTranslate(FALSE);
			Tint_Explore_Switch((INT)this,
					Tint_Explore_Param::GROUP_LID_CAMERA_LOI,
					Tint_Explore_Param::ELEMENT_LID_TARGET_TFORM,
					FALSE
				);
		};

	// ---- initialise tint acquire selected source function and tint text label
		auto update_acquire_select_function_surface = [&]()
		{
				result = Tint_Acquire_UpdateModel(
						(INT)this,
						Tint_Acquire_GetElemLidSourceFunction()
					);
				result = Tint_Acquire_UpdateElement(
						(INT)this,
						Tint_Acquire_GetElemLidSourceSurf()
					);
		};


	// ---- Note: Keypad focus set to selected tint by the TintBodySelect method

	// ---- set body param group
		//
		//  * - logic changed such that only the
		//      acquire select source and acquire
		//      buttons can change textured surface
		//
		switch (body_id)
		{

			case TINT_BODY_SETUP_GFX_LID:
				{
					// * update_acquire_model_and_element();
					update_explore();
					update_no_target();
				}
				break;

			case TINT_BODY_ACQUIRE_LID:
				{
					// * update_acquire_model_lumina_last();
				}
				break;

			case TINT_BODY_EXPLORE_LID:
				{
					// * update_acquire_model_lumina_last();
					update_explore();
				}
				break;

			case TINT_BODY_FUNCTION_LID:
				{
					//update_acquire_select_function_surface();
					//Tint_Function_UpdateElement((INT)this, 'H');
				}
				break;

			default:
				return VS_ERROR;
		}

	return VS_OK;
}
