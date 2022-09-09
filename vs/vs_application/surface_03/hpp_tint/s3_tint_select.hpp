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


	// ---- set measure  OFF
		model::Surface_03 *surface_03 = pi_model->surface_03;
		surface_03->SetTargetLoiMeasure(FALSE);


	// ---- set body param group
		int param_group = 999;
		switch (body_id)
		{
			case TINT_BODY_ACQUIRE_LID:
				param_group = PARAM_GROUP_ACQUIRE;
				break;

			case TINT_BODY_CALIBRATE_LID:
				param_group = PARAM_GROUP_CAMERA;
				break;

			case TINT_BODY_LIGHT_LID:
				param_group = PARAM_GROUP_LIGHTS;
				break;

			case TINT_BODY_EXPLORE_LID:
				{
					//INT tid = s_pi_model->GetTargetObjectId();
					//tid = tid == 999 ? '0' : tid + '0';
					//result = Tint_Explore_UpdateModel((INT)this, tid);
				}
				break;

			case TINT_BODY_MEASURE_LID:
				{

					//INT tid = s_pi_model->GetTargetObjectId() + '0';
					//result = Tint_Explore_UpdateModel((INT)this, tid);
					//result = Tint_Measure_UpdateModel((INT)this, tid);

					surface_03->SetTargetLoiMeasure(TRUE);
					param_group = PARAM_GROUP_MEASURE;

				}
				break;

			case TINT_BODY_ENHANCE_LID:
				break;

			case TINT_BODY_FUNCTION_LID:
				param_group = PARAM_GROUP_FUNCTION;
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


	// ---- NOT in use: setup kamera - especially for measure
		//result = TintBodyUpdateKamera(body_id);

	return VS_OK;
}

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


////////////////////////////////////////////////////////////////////////////////
