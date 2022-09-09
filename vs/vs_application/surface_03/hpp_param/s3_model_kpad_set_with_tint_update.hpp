////////////////////////////////////////////////////////////////////////////////

// ---------- s3_model_kpad_set_with_tint_update.hpp ----------
/*!
\file s3_model_kpad_set_with_tint_update.hpp
\brief
\author Gareth Edwards
*/


INT Model::Kpad_SetParameter(INT handle_model)
{

	// ---- defined ?
		INT param_group = kpad_input_float.GetParamGroup();
		if ( param_group == Model::PARAM_GROUP_NULL) return VS_ERROR;


	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;


	// ---- set group
		result = SetParamGroup(param_group);


	// ---- mesh param
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- get and set current param
		param_array_values[param_array_index] = kpad_input_float.GetFloatValue();
  
		// OutputDebugString("Kpad_SetParameter\n");
		//switch (param_group)
		//{
		//	case surface_03::Model::PARAM_GROUP_MEASURE:
		//		{
		//			FLOAT value = kpad_input_float.GetFloatValue();
		//			model::Surface_03 *surface_03 = s_pi_model->surface_03;
		//			FLOAT new_value = surface_03->GetTargetLoiInverseForDisplay(param_array_index, value);
		//			param_array_values[param_array_index] = new_value;
		//		}
		//		break;
		//	default:
		//		{
		//			param_array_values[param_array_index] = kpad_input_float.GetFloatValue();
		//		}
		//		break;
		//}

		result = SetParamArrayValues(
				param_group_index,
				param_group_id,
				param_array_count,
				param_array_values
			);

	// ---- report set keypad
		/*
			CHAR msg[128];
			sprintf(
				msg,
				"Set : %d - %d %d - %s - %s - %f \n",
				param_group,
				GetParamGroupIndex(),
				GetParamArrayIndex(),
				kpad_input_float.GetGroup(),
				kpad_input_float.GetName(),
				param_array_values[param_array_index]
			);
			OutputDebugString(msg);
		*/

	// ---- UpdateHeightfieldObjects action
		CHAR action = 0;

	// ---- update tint parameter text
		switch ( param_group )
		{ 
			case surface_03::Model::PARAM_GROUP_ACQUIRE:
				model->Tint_Acquire_UpdateElement(handle_model, param_array_index + 'A');
				break;   
			case surface_03::Model::PARAM_GROUP_FUNCTION:
				action = 'F';
				model->Tint_Function_UpdateElement(handle_model, param_array_index + 'A');
				break;
			case surface_03::Model::PARAM_GROUP_MEASURE:
				model->Tint_Measure_UpdateElement(handle_model, -( param_array_index + 'A') );
				break;
			case surface_03::Model::PARAM_GROUP_LIGHTS:
				model->Tint_Light_UpdateElement(handle_model, param_array_index + 'A');
				s_pi_model->surface_03->SetupExploreLighting(1);
				break;
		}


	// ---- ! acquisition ? - update objects
		if ( param_group != surface_03::Model::PARAM_GROUP_ACQUIRE )
		{
			model::Surface_03 *surface_03 = s_pi_model->surface_03;
			surface_03->UpdateHeightfieldObjects((INT)action, surface_03, FALSE);
		}


	// ---- mesh param
		#include "../../surface_shared/hpp_param/param_cleanup.hpp"

	return VS_OK;
}

