////////////////////////////////////////////////////////////////////////////////

// ---------- s1_model_kpad_set_with_tint_update.hpp ----------
/*!
\file s1_model_kpad_set_with_tint_update.hpp
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

	// ---- mesh param
		#include "../../surface_shared/hpp_param/param_cleanup.hpp"

	return VS_OK;
}

