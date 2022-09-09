////////////////////////////////////////////////////////////////////////////////

// ---------- model_tint_explore_set_text_incl.h ----------
/*!
\file model_tint_explore_set_text_incl.hpp
\brief Included Model Tint_Explore_SetTextStuff code
\author Gareth Edwards
*/


// ---- update control text
	start = tint_ms_camera_param.elm_context_control_index;
	tint_ms_camera_param.elem_context[start]->SetText(
			tint_ms_camera_param.control_text[control_update_mode][0]
		);
	tint_ms_camera_param.elem_context[start+4]->SetText(
			tint_ms_camera_param.control_text[control_update_mode][1]
		);


// ---- update speed 'i'
	vs_system::AppDependency *app_dep = model->GetAppDependency();
	vs_system::SysInput      *sys_input = app_dep->GetSysInput();
	INT msg_prev = sys_input->GetInt(vs_system::SysInput::ASCII_CODE_PREV);
	if ( msg_prev == 'i' )
	{

		// ---- flip state & set scalar
			INT result = Tint_Explore_UpdateSpeed();

		// ---- set bounds
			start  = tint_ms_camera_param.elm_context_sri_index + 4;
			end    = start + 4;

		// ---- flip text
			/*
			std::string txt = "Incr ";
			txt += tint_ms_camera_param.high_speed ? "Large" : "Small";
			tint_ms_camera_param.elem_context[start++]->SetText(txt);
			*/

		// ---- flip "on"
			for (UINT e = start; e < end; e++)
			{
				tint_ms_camera_param.elem_context[e]->SetSwitchedOn(tint_ms_camera_param.high_speed ? FALSE : TRUE);
			}

		// ---- restore msg (for persistant behaviour)
			sys_input->SetInt(vs_system::SysInput::ASCII_CODE, msg_prev);
	}


// ---- update animations 'j'. 'k' & 'l'
	if ( msg_prev == 'j' || msg_prev == 'k' || msg_prev == 'l' )
	{

		// ---- bounds
			start        = tint_ms_camera_param.elm_context_control_index;
			start_plus_3 = start + 3;
			end          = start + 26;

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
	}