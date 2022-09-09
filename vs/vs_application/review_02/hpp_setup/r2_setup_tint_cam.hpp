////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_cam.hpp ----------
/*!
\file r2_tint_cam.hpp
\brief
\author Gareth Edwards
*/


#pragma once

#include "../header/r2_model.h"

using namespace review_02;


// ---------- SetupTintCam ----------
/*!
\brief Setup camera tabs interface
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupTintCam()
{

	// ---- local
		INT result = VS_OK; 
		INT handle = 0;
		vs_system::Tint *tint = pi_model->tint_cam_setng;
		vs_system::TintCallback tint_callback;


	// ---- init
		result = tint->Initialise(
					gfx,
					pi_model->obj_config,
					TintGetBuildParam
				);


	// ---- set fonts
		pi_model->tint_cam_setng->SetHeadFont("Dbz_Caption");
		pi_model->tint_cam_setng->SetBodyFont("Dbz_Normal");


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
		result = Tint_Acquire_GetCallbacks(&tint_callback);
			handle = tint->AddBody(
					(INT)pi_model->cam_props,
					"Acquisition",
					TINT_HEAD_ACQUIRE_LID,
					&tint_callback
				);
			result = tint->AddEnclose(handle);


		result = Tint_GainMas_GetCallbacks(&tint_callback);
			handle = tint->AddBody(
					(INT)pi_model->cam_props,
					"Gain & Gamma",
					TINT_HEAD_GAINMAS_LID,
					&tint_callback
				);
			result = tint->AddEnclose(handle);


		result = Tint_GainCol_GetCallbacks(&tint_callback);
			handle = tint->AddBody(
					(INT)pi_model->cam_props,
					"White Balance",
					TINT_HEAD_GAINCOL_LID,
					&tint_callback
				);
			result = tint->AddEnclose(handle);


		result = Tint_Colour_GetCallbacks(&tint_callback);
			handle = tint->AddBody(
					(INT)pi_model->cam_props,
					"Colour",
					TINT_HEAD_COLOUR_LID,
					&tint_callback
				);
			result = tint->AddEnclose(handle);


		#ifdef FREDFRED
		result = Tint_ImgFx_GetCallbacks(&tint_callback);
			handle = tint->AddBody(
						(INT)this,
						"Image Effects",
						TINT_TAB_IMGFX_LID,
						&tint_callback
					);
			tint->AddEnclose(handle);
		#endif

		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
			;
		#else
			result = Tint_Pan_GetCallbacks(&tint_callback);
				handle = tint->AddBody(
						(INT)this,
						"Panorama",
						TINT_HEAD_PANO_LID,
						&tint_callback
					);
				result = tint->AddEnclose(handle);
		#endif


		result = Tint_Aec_GetCallbacks(&tint_callback);
			handle = tint->AddBody(
					(INT)pi_model->cam_props,
					"Control Zone",
					TINT_HEAD_AEC_LID,
					&tint_callback
				);
			result = tint->AddEnclose(handle);


		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
			;
		#else
			result = Tint_Focus_GetCallbacks(&tint_callback);
				handle = tint->AddBody(
						(INT)pi_model->cam_props,
						"Focus", 
						TINT_HEAD_SYSTEM_LID,
						&tint_callback
					);
				result = tint->AddEnclose(handle);
		#endif


		result = Tint_Settings_GetCallbacks(&tint_callback); 
			handle = tint->AddBody(
					(INT)pi_model->cam_props,
					"Settings",
					TINT_HEAD_SETTINGS_LID,
					&tint_callback
				);
			result = tint->AddEnclose(handle);


	// ---- head
		result = tint->SelectBody(TINT_HEAD_ACQUIRE_LID);
		FLOAT head_yoffset = TintGetBuildParam(vs_system::Tint::HEAD_YOFFSET);
			result = tint->SetTabOffset(tint_xoffset, head_yoffset);
			handle = tint->AddHead(
					"Camera",
					ELEM_GROUP_CAM_HEAD_LID
				);
			result = tint->AddEnclose(handle);


	// ---- set display list handles
		result = pi_model->handle_dspl_tint_cam_head = tint->GetHeadHandle();
		result = pi_model->handle_dspl_tint_cam_body = tint->GetBodyHandle(
						tint->GetSelectedBodyId()
					);

	return VS_OK;
}

