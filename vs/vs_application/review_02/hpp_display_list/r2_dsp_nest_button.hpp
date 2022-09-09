////////////////////////////////////////////////////////////////////////////////

// ---------- r2_dl_nest_button.hpp ----------
/*!
\file r2_dl_nest_button.hpp
\brief application include files.
\author Gareth Edwards
*/


#include "../header/r2_model.h"
using namespace review_02;


// ---------- SetupNestDisplayList ----------
/*!
\brief Setup camera "nest" display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupNestDisplayList()
{

	// ---- local ----
		INT result = 0;
		FLOAT param[VS_MAX_ELM_FLOAT_PARAMETER];


	// ---- parameters
		FLOAT text_dim = 0.05f;


	// ---- versions
		UINT element_version_lut[3] =
		{
			vs_system::ElmContext::VERSION_BACKGROUND,
			vs_system::ElmContext::VERSION_ICON,
			vs_system::ElmContext::VERSION_OUTLINE
		};

	// ---- build display list
		for (INT i = 0; i < 3; i++)
		{

			// ---- add
			INT button_handle = gfx->AddElement(
						pi_model->handle_dspl_nest,
						"Close",
						NEST_BUTTON_GROUP_ID,
						1,
						&pi_model->obj_config->Callback_Button_Configure
					);


			// ---- initialise
			vs_system::ElmContext *elm = NULL;
			INT get_element_result = gfx->GetElementContext(button_handle, &elm);
			if (get_element_result != NULL)
			{

				// elem
				result = elm->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_CLICK);
				result = elm->SetVersion(element_version_lut[i]);
				result = elm->SetRGBA(1.0f, 1.0f, 1.0f, 0.5f);

				// param
				result = pi_model->obj_config->ButtonSetDPT(
						param,
						vs_system::ElmContext::VIEWPORT_HEIGHT |
						vs_system::ElmContext::VIEWPORT_WIDTH_XLOC
					);
				result = pi_model->obj_config->ButtonSetLocation(
						param,
						1 - text_dim - text_dim/8, text_dim/8,
						text_dim, text_dim
					);
				result = pi_model->obj_config->ButtonSetTexture(
						param,
						VS_ATLAS_PAGE_GROUP_TWO,
						0, 0, 1, 6, 1, 1, 0.001f
					);
				result = elm->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);
			}

		}

	return VS_OK;
}


// ---------- UpdateNestDisplayList ----------
/*!
\brief Update camera "nest" display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::UpdateNestDisplayList()
{

	return VS_OK;
}

