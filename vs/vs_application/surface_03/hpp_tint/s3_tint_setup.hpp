////////////////////////////////////////////////////////////////////////////////

// ---------- s3_tint_setup.hpp ----------
/*!
\file s3_tint_setup.hpp
\brief Interface for Model class Tint setup method
\author Gareth Edwards
*/

#pragma once

#include "../header/s3_model.h"

using namespace surface_03;


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


	// ---- calibration
		Tint_Calibrate_GetCallbacks(&tint_callback);
		handle = tint->AddBody(
					(INT)this,
					"Acquire Calibrate",
					TINT_BODY_CALIBRATE_LID,
					&tint_callback
				);
		tint->AddEnclose(handle);


	// ---- light
		Tint_Light_GetCallbacks(&tint_callback);
		handle = tint->AddBody(
					(INT)this,
					"Explore Lighting",
					TINT_BODY_LIGHT_LID,
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


	// ---- measure
		Tint_Measure_GetCallbacks(&tint_callback);
		handle = tint->AddBody(
					(INT)this,
					"Explore Measure",
					TINT_BODY_MEASURE_LID,
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


	// ---- enhance
		/*
		Tint_Enhance_GetCallbacks(&tint_callback);
		handle = tint->AddBody(
					(INT)this,
					"Enhance",
					TINT_BODY_ENHANCE_LID,
					&tint_callback
				);
		tint->AddEnclose(handle);
		*/


	// ---- settings
		Tint_Settings_GetCallbacks(&tint_callback);
		handle = tint->AddBody(
					NULL,
					"Settings",
					TINT_BODY_SETTINGS_LID,
					&tint_callback
				);
		tint->AddEnclose(handle);


	// ---- locate head
		tint->SelectBody(TINT_BODY_ACQUIRE_LID);
		FLOAT head_yoffset = TintGetBuildParam(vs_system::Tint::HEAD_YOFFSET);


	// ---- add head
		tint->SetTabOffset(tint_xoffset, head_yoffset);
		handle = tint->AddHead(
					"Inspect 3D",
					TINT_HEAD_3DS_LID
				);
		tint->AddEnclose(handle);


	// ---- set display list handles
		pi_model->handle_dspl_tint_head =
			tint->GetHeadHandle();
		pi_model->handle_dspl_tint_body =
			tint->GetBodyHandle(tint->GetSelectedBodyId());

		/*
		INT head_dspl_elm_count = tint->GetHeadDsplElmCount();

		INT head_dspl_elm_handle[tint->MAX_BODY_ELM];
		result = tint->GetHeadDsplElmHandles(
				head_dspl_elm_handle
			);

		for (INT i = 0; i < head_dspl_elm_count; i++)
		{
			vs_system::ElmContext *elm_context = 0;
			pi_model->gfx->GetElementContext(
					head_dspl_elm_handle[i],
					&elm_context
				);
			char msg[128];
			sprintf(msg, "-> %d %d \n", elm_context->GetId(), elm_context->GetGroupId());
			OutputDebugString(msg);
			elm_context->SetMultiActive(FALSE,0);
		}
		*/

		TintHeadUpdateAnalysisTabButtons();

	return VS_OK;
}


// ---------- TintHeadUpdateAnalysisTabButtons ----------
/*!
\brief Update tint head analysis buttons 
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note requires tint_acquire_param.surface_acquired to be set TRUE/FALSE
*/
INT Model::TintHeadUpdateAnalysisTabButtons()
{

	// ---- local
		INT result = VS_OK;
		INT handle = 0;
		vs_system::Tint *tint = pi_model->tint_setng;


	// ---- get list of tint head display element handles 
		INT head_dspl_elm_count = tint->GetHeadDsplElmCount();
		INT head_dspl_elm_handle[tint->MAX_BODY_ELM];
			result = tint->GetHeadDsplElmHandles(
				head_dspl_elm_handle
			);

	// ---- update tab head icons
		for (INT i = 0; i < head_dspl_elm_count; i++)
		{

			// ---- get element gfx context
				vs_system::ElmContext *elm_context = 0;
				pi_model->gfx->GetElementContext(
						head_dspl_elm_handle[i],
						&elm_context
					);

			// ---- update ONLY analysis tabs
				switch ( elm_context->GetId() )
				{
					// ---- always on
						case TINT_BODY_ACQUIRE_LID:
						case TINT_BODY_CALIBRATE_LID:
						case TINT_BODY_FUNCTION_LID:
						case TINT_BODY_SETTINGS_LID:
							break;

					// ---- on if surface acquired
						case TINT_BODY_LIGHT_LID:
						case TINT_BODY_EXPLORE_LID:
						case TINT_BODY_MEASURE_LID:
						case TINT_BODY_ENHANCE_LID:
							if ( tint_acquire_param.surface_acquired )
							{
								elm_context->SetMultiActive(TRUE, 0);
								elm_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_RADIO);
							}
							else
							{
								elm_context->SetMultiActive(FALSE, 0);
								elm_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NULL);
							}
							break;
				}

		}

	return VS_OK;
}

