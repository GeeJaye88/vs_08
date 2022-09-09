////////////////////////////////////////////////////////////////////////////////

// ---------- s2_tint_setup.hpp ----------
/*!
\file s2_tint_setup.hpp
\brief Interface for Model class Tint setup method
\author Gareth Edwards
*/

#pragma once

#include "../header/s2_model.h"

using namespace surface_02;


// ---------- TintSetup ----------
/*!
\brief Setup tabbed interface
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::TintSetup()
{

	// ---- local
		INT result = VS_OK;
		INT handle = 0;
		vs_system::Tint *tint = pi_model->tint_setng;
		vs_system::TintCallback tint_callback;


	// ---- init
		result = tint->Initialise(
					pi_model->gfx,
					pi_model->obj_config,
					TintGetBuildParam
				);


	// ---- set fonts
		pi_model->tint_setng->SetHeadFont("Tint_Head");
		pi_model->tint_setng->SetBodyFont("Tint_Body");
			

	// ---- columns
		vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
		DWORD format = sys_context->GetDWord(vs_system::SysContext::FORMAT);
		result = tint->SetColumns(
					format == vs_system::SysContext::PORTRAIT ? TRUE : FALSE,
					3
				);


	// ---- locate body
		FLOAT tint_xoffset = TintGetBuildParam(vs_system::Tint::TINT_XOFFSET);
		FLOAT body_yoffset = TintGetBuildParam(vs_system::Tint::BODY_YOFFSET);
		tint->SetTabOffset(tint_xoffset, body_yoffset);


	// ---- acquisition
		Tint_Acquire_GetCallbacks(&tint_callback);
		handle = tint->AddBody(
					(INT)this,
					"Acquire Surface",
					TINT_BODY_ACQUIRE_LID,
					&tint_callback
				);
		tint->AddEnclose(handle);


	// ---- explore
		Tint_Explore_GetCallbacks(&tint_callback);
		handle = tint->AddBody(
					(INT)this,
					"Explore Camera",
					TINT_BODY_EXPLORE_LID,
					&tint_callback
				);
		tint->AddEnclose(handle);


	// ---- functions
		Tint_Function_GetCallbacks(&tint_callback);
		handle = tint->AddBody(
					(INT)this,
					"Acquire Function",
					TINT_BODY_FUNCTION_LID,
					&tint_callback
				);
		tint->AddEnclose(handle);


	// ---- locate head
		tint->SelectBody(TINT_BODY_ACQUIRE_LID);
		FLOAT head_yoffset = TintGetBuildParam(vs_system::Tint::HEAD_YOFFSET);


	// ---- add head
		tint->SetTabOffset(tint_xoffset, head_yoffset);
		handle = tint->AddHead(
					"3DS Development",
					TINT_HEAD_2DS_LID
				);
		tint->AddEnclose(handle);


	// ---- set display list handles
		pi_model->handle_dspl_tint_head =
			tint->GetHeadHandle();
		pi_model->handle_dspl_tint_body =
			tint->GetBodyHandle(tint->GetSelectedBodyId());

	return VS_OK;
}

