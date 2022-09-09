////////////////////////////////////////////////////////////////////////////////

// ---------- model_tint_explore.hpp ----------
/*!
\file model_tint_explore.hpp
\brief Implementation of Model Tint Explore methods invoked by Kontrol
\author Gareth Edwards
*/


////////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------
INT Model::Tint_Explore_SetControlStuff(
		INT  handle_model,
		INT  msg
	)
{

	// ---- local
		INT   result = VS_OK;
		Model *model = (Model *)handle_model;


	// ---- control
		//
		//    "text"  0
		//    'W'     1 - 3
		//    "text"  4
		//    'R'     5 - 7
		//    'A'     8 - 10
		//    'Q'    11 - 13
		//    'D'    14 - 16
		//    'E'    17 - 19
		//    'S'    20 - 22
		//    'F'    23 - 25
		//
		UINT start = tint_ms_camera_param.elm_context_control_index;
		UINT start_plus_3 = start + 3;
		UINT end = start + 26;


	// ---- switch off
		for (UINT e = start; e < end; e++)
		{
			tint_ms_camera_param.elem_context[e]->SetSwitchedOn(FALSE);
		}


	// ---- ?
		BOOL switched_on = model->GetMsgCameraControlPersistantOn();
		if ( switched_on )
		{
			for (UINT e = start; e < start_plus_3; e++)
			{
				tint_ms_camera_param.elem_context[e + 11]->SetSwitchedOn(switched_on);
				tint_ms_camera_param.elem_context[e + 17]->SetSwitchedOn(switched_on);
			}
		}

		BOOL active = model->GetMsgCameraControlPersistantActive();
		if ( switched_on && active )
		{
			INT offset = 0;
			switch ( msg )
			{
				case 'W': offset =  1; break;
				case 'R': offset =  5; break;
				case 'A': offset =  8; break;
				case 'D': offset = 14; break;
				case 'S': offset = 20; break;
				case 'F': offset = 23; break;
				default:
					break;
			}
			if ( offset != 0 )
			{
				for (UINT e = start; e < start_plus_3; e++)
				{
					tint_ms_camera_param.elem_context[e + offset]->SetSwitchedOn(TRUE);
				}
			}

		}
		

	// ---- control mode text
		UINT control_mode = model->Tint_Explore_GetCameraControlMode();
		start = tint_ms_camera_param.elm_context_control_index;
		tint_ms_camera_param.elem_context[start]->SetText(tint_ms_camera_param.control_text[control_mode][0]);
		tint_ms_camera_param.elem_context[start + 4]->SetText(tint_ms_camera_param.control_text[control_mode][1]);

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Explore_UpdateByMessage(
		INT   handle_model,
		INT   msg_lid
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;


	// ---- get display list element context handles (runtime only)
		result = model->Tint_Explore_SetElementIndices(handle_model, 0);


	// ---- update
		result = model->Tint_Explore_SetTextStuff(handle_model, 0);
		result = model->Tint_Explore_SwitchStuff(handle_model, 0);
		for (INT e = 0; e < 8; e++)
		{
			if ( tint_ms_camera_param.control_group_msg[e] == msg_lid )
			{
				result = model->Tint_Explore_SetControlStuff(handle_model, msg_lid);
				return VS_OK;
			}
		}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////