////////////////////////////////////////////////////////////////////////////////

// ---------- s3_tint_select.hpp ----------
/*!
\file s3_tint_select.hpp
\brief Interface for Model class Tint select methods
\author Gareth Edwards
*/

#pragma once

using namespace surface_03;


// ---------- TintBodySelect ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
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
		int param_group = 999;
		switch (body_id)
		{
			case TINT_BODY_ACQUIRE_LID:
				param_group = PARAM_GROUP_ACQUIRE;
				break;

			case TINT_BODY_CALIBRATE_LID:
				break;

			case TINT_BODY_LIGHT_LID:
				param_group = PARAM_GROUP_LIGHTS;
				break;

			case TINT_BODY_EXPLORE_LID:
				break;

			case TINT_BODY_MEASURE_LID:
				break;

			case TINT_BODY_ENHANCE_LID:
				break;

			case TINT_BODY_FUNCTION_LID:
				break;

			case TINT_BODY_SETTINGS_LID:
				break;

			default:
				return VS_ERROR;
		}


		if ( param_group != 999 )
		{
			SetParamGroup(param_group);
			Kpad_UpdateByParameter((INT)this, GetParamGroup());
		}


	// ---- setup kamera - especially for meausre
		result = TintBodyUpdateKamera(body_id);


	return VS_OK;
}


//result = Tint_Acquire_UpdateModel(
//	(INT)this,
//	Tint_Acquire_GetElemLidSourceSurf()
//);

// ---- unset acquire lumina & 3D buttons
/*
pi_model->tint_setng->ElemDsplInfoSwitch(
	tint_acquire_param.element_dspl_count,
	tint_acquire_param.element_dspl_info,
	Tint_Acquire_Param::GROUP_LID_LUMINA_FIELD,
	0,
	FALSE
);

param_group = PARAM_GROUP_FUNCTION;

// ---- deactivate analysis tabs
	tint_acquire_param.surface_acquired = FALSE;
	TintHeadUpdateAnalysisTabButtons();
*/


// ---------- TintBodyUpdateKamera ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK

\note 

*/
INT Model::TintBodyUpdateKamera(
		UINT  body_id
	)
{

	// ---- local
		INT result = VS_OK;
		vs_system::AppKamera *kamera = pi_model->app_kamera[0];
		Model::PI_Model      *pi_model = this->pi_model;
		model::Surface_03    *surface_03 = pi_model->surface_03;


	// ---- get loi rotation and target status (loi visible)
		UINT  target_loi_index    = surface_03->GetTargetLoiIndex();
		FLOAT target_loi_rotation = surface_03->GetTargetLoiRotation(target_loi_index);
		BOOL  target_status       = kamera->GetTargetStatus();


	// ---- get kamera pan, tilt & position
		FLOAT pan, tilt;
		VsVector3 position;
		kamera->GetPanTilt(&pan, &tilt);
		kamera->GetPosition(&position);


	// ---- surface
		surface_03->SetTargetLoiStatus(target_status);
		surface_03->SetTargetLoiMeasure(FALSE);


	// ---- store tint_measure_param stuff ?
		if ( body_id == TINT_BODY_MEASURE_LID || body_id == TINT_BODY_MEASURE_STORE )
		{
			// ---- kamera
				tint_measure_param.pan = pan;
				tint_measure_param.tilt = tilt;
				tint_measure_param.position = position;

			// ---- target
				tint_measure_param.target_status = target_status;
				tint_measure_param.target_loi_index = target_loi_index;
				tint_measure_param.target_object_id = s_pi_model->GetTargetObjectId();
		}


	// ---- setup measure view
		switch ( body_id )
		{

			case TINT_BODY_MEASURE_LID:
				{

						// ---- kamera
						//kamera->SetTargetStatus(FALSE);
						//kamera->SetPosition(0, 0, -39000);
						//kamera->SetPanTilt(0, 0);

						// ---- model
							//s_pi_model->SetTargetObjectId(CameraTarget::NONE);

						// ---- surface
							//surface_03->SetTargetLoiIndex(CameraTarget::NONE);
							surface_03->SetTargetLoiMeasureFov(40);
							surface_03->SetTargetLoiMeasure(TRUE);

					//kamera->SetPosition(0, 0, -300);
					//kamera->SetPanTilt(target_loi_rotation, 0);
				}
				break;

			default:
				{

					// ---- kamera
						kamera->SetTargetStatus(tint_measure_param.target_status);
						kamera->SetPosition(&tint_measure_param.position);
						kamera->SetPanTilt(tint_measure_param.pan, tint_measure_param.tilt);

					// ---- target
						s_pi_model->SetTargetObjectId(tint_measure_param.target_object_id);
						surface_03->SetTargetLoiIndex(tint_measure_param.target_loi_index);

				}
		}

		return result;
}


//char msg[128];
//sprintf(msg, "-> LOI rot %6.4f\n", loi_rotation);
//OutputDebugString(msg);


// ---------- TintBodyUpdateVisibleObjects ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK

\note invoked by INT Model::SetupGraphics()

*/
INT Model::TintBodyUpdateVisibleObjects(
		UINT  body_id
	)
{
	model::Surface_03 *surface_03 = s_pi_model->surface_03;
	surface_03->SetVisibleObjects(body_id, surface_03);
	return VS_OK;
}


// ---------- TintBodyUpdateText ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK

\note not invoked

*/
INT Model::TintBodyUpdateText(
		UINT  body_id,
		INT   group_id,
		INT   elem_id
	)
{

	INT result = pi_model->tint_setng->SetSelectedBody(body_id);

	result = pi_model->tint_setng->UpdateText(
			group_id,
			elem_id
		);

	return result;
}