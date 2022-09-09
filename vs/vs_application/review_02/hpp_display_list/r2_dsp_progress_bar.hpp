////////////////////////////////////////////////////////////////////////////////

// ---------- r2_dl_progress_bar.hpp ----------
/*!
\file r2_dl_progress_bar.hpp
\brief application include files.
\author Gareth Edwards
*/

#include "../header/r2_model.h"
using namespace review_02;


// ---------- SetupProgressBar ----------
/*!
\brief Setup progress bar display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/  
INT Model::SetupProgressBar()
{

	// ---- local
		INT result = 0;


	// ---- system
		using namespace vs_system;


	// ---- versions

		FLOAT param[VS_MAX_ELM_FLOAT_PARAMETER];
		UINT v[2] = {
			vs_system::ElmContext::VERSION_BACKGROUND,
			vs_system::ElmContext::VERSION_OUTLINE 
		};

	
	// ---- note: only bg currently added
		for (UINT i = 0; i < 1; i++)
		{
			INT bg_handle = gfx->AddElement(
					pi_model->handle_dspl_progress_bar,
					"ProgBar",
					0,
					0,
					&pi_model->obj_config->Callback_Button_Configure
				);
			vs_system::ElmContext *elm = NULL;
			INT get_element_result = gfx->GetElementContext(bg_handle, &elm);
			if ( get_element_result != NULL )
			{
				result = elm->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NONE);
				result = elm->SetVersion(v[i]);
				result = elm->SetRGBA(1.0f, 1.0f, 1.0f, 1.0f);
				result = elm->SetSwitchedOn(TRUE);
				result = elm->SetDisplay(FALSE);
				pi_model->obj_config->ButtonSetDPT(
						param,
						vs_system::ElmContext::VIEWPORT_WIDTH
					);
				pi_model->obj_config->ButtonSetLocation(
						param,
						0.0f, 0.0f, 1.0f, 0.95f
					);
				pi_model->obj_config->ButtonSetTexture(
						param,
						VS_ATLAS_PAGE_GROUP_TWO,
						0, 0, 0, 0, 1, 1, 0.01f
					);

				result = elm->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);
			}
		}


	return VS_OK;
}


// ---------- UpdateProgBarDisplayList ----------
/*!
\brief Setup progress bar display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::UpdateProgBarDisplayList(
		BOOL display,
		FLOAT percent
	)
{

	// ---- local
		INT result = 0;
		vs_system::ElmContext *element_context = NULL;


	// ---- element
		INT elm = gfx->GetFirst(pi_model->handle_dspl_progress_bar);
		while (elm != 0)
		{

			// ---- context ?
				result = gfx->GetElementContext(elm, &element_context);
				if ( result != NULL )
				{

					FLOAT *param = element_context->GetFloatParameter();
					pi_model->obj_config->ButtonSetLocation(
							param,
							0.0f, 0.0f, percent, 0.95f
						);
					result = element_context->SetDisplay(percent < 0.001 ? FALSE : TRUE);
					result = element_context->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);
					result = gfx->SetElementConfigureBitmask(elm,VS_ELM_SETUP_GEOMETRY);
			}

			// ---- next
				elm = gfx->GetNext(elm);
		}

	return VS_OK;
}

