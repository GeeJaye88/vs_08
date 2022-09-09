////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_img.hpp ----------
/*!
\file r2_tint_img.hpp
\brief
\author Gareth Edwards
*/


#pragma once

#include "../header/r2_model.h"

using namespace review_02;


// ---------- SetupTintImg ----------
/*!
\brief Setup tools interface
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupTintImg()
{

	// ---- local
		INT result = VS_OK;
		INT handle = 0;
		vs_system::Tint *tint = pi_model->tint_img_setng;
		vs_system::TintCallback tint_callback;


	// ---- init
		result = tint->Initialise(
					gfx,
					pi_model->obj_config,
					TintGetBuildParam
				);


	// ---- fonts
		pi_model->tint_img_setng->SetHeadFont("Dbz_Caption");
		pi_model->tint_img_setng->SetBodyFont("Dbz_Normal");


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


	// ---- add
		Tint_ImgPtz_GetCallbacks(&tint_callback);
		handle = tint->AddBody(
					(INT)this,
					"Pan & Zoom",
					TINT_TAB_IMGPTZ_LID,
					&tint_callback
				);
		tint->AddEnclose(handle);


	// ---- add
		Tint_ImgLoc_GetCallbacks(&tint_callback);
		handle = tint->AddBody(
					(INT)this,
					"Alignment & Presets",
					TINT_TAB_IMGLOC_LID,
					&tint_callback
				);
		tint->AddEnclose(handle);


	// ---- add
		Tint_ImgFx_GetCallbacks(&tint_callback);
		handle = tint->AddBody(
					(INT)this,
					"Imaging Effects",
					TINT_TAB_IMGFX_LID,
					&tint_callback
				);
		tint->AddEnclose(handle);


	// ---- add
		Tint_Fconfig_GetCallbacks(&tint_callback);
		handle = tint->AddBody(
					NULL,
					"Measurement",
					TINT_TAB_IMGUSER02_LID,
					&tint_callback
				);
		tint->AddEnclose(handle);


	// ---- locate head
		tint->SelectBody(TINT_TAB_IMGPTZ_LID);
		FLOAT head_yoffset = TintGetBuildParam(vs_system::Tint::HEAD_YOFFSET);

	// ---- add head
		tint->SetTabOffset(tint_xoffset, head_yoffset);
		handle = tint->AddHead(
					"vPTZ Manager",
					ELEM_GROUP_IMG_HEAD_LID
				);
		tint->AddEnclose(handle);


	// ---- set display list handles
		pi_model->handle_dspl_tint_img_head =
			tint->GetHeadHandle();
		pi_model->handle_dspl_tint_img_body =
			tint->GetBodyHandle(tint->GetSelectedBodyId());

	return VS_OK;
}

