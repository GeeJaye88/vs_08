////////////////////////////////////////////////////////////////////////////////

// ---------- kpad_setup.hpp ----------
/*!
\file model_tint_setup.hpp
\brief Interface for Model class Tint setup method
\author Gareth Edwards
*/


// ---------- KpadSetup ----------
/*!
\brief Setup tabbed interface
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::KpadSetup()
{

	// ---- local
		INT result = VS_OK;
		INT handle = 0;
		vs_system::Tint *tint = pi_model->kpad_setng;
		vs_system::TintCallback tint_callback;


	// ---- init
		result = tint->Initialise(
					pi_model->gfx,
					pi_model->obj_config,
					KpadGetBuildParam
				);


	// ---- set fonts
		pi_model->kpad_setng->SetHeadFont("Kpad_Head");
		pi_model->kpad_setng->SetBodyFont("Kpad_Body");
			

	// ---- columns
		vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
		DWORD format = sys_context->GetDWord(vs_system::SysContext::FORMAT);
		result = tint->SetColumns(
					format == vs_system::SysContext::PORTRAIT ? TRUE : FALSE,
					3
				);


	// ---- locate body
		FLOAT calc_xoffset = KpadGetBuildParam(vs_system::Tint::TINT_XOFFSET);
		FLOAT body_yoffset = KpadGetBuildParam(vs_system::Tint::BODY_YOFFSET);
		tint->SetTabOffset(calc_xoffset, body_yoffset);


	// ---- add
		Kpad_Input_GetCallbacks(&tint_callback);
		handle = tint->AddBody(
					(INT)this,
					"# Keypad",
					KPAD_BODY_INPUT_LID,
					&tint_callback
				);
		tint->AddEnclose(handle);


	// ---- locate head
		tint->SelectBody(KPAD_BODY_INPUT_LID);
		FLOAT head_yoffset = KpadGetBuildParam(vs_system::Tint::HEAD_YOFFSET);


	// ---- add head
		tint->SetTabOffset(calc_xoffset, head_yoffset);
		handle = tint->AddHead(
					"Input",
					KPAD_HEAD_LID
				);
		tint->AddEnclose(handle);


	// ---- set display list handles
		pi_model->handle_dspl_kpad_head =
			tint->GetHeadHandle();
		pi_model->handle_dspl_kpad_body =
			tint->GetBodyHandle(tint->GetSelectedBodyId());

	return VS_OK;
}

