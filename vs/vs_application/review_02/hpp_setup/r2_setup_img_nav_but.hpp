////////////////////////////////////////////////////////////////////////////////

// ---------- r2_setup_img_nav_but.hpp ----------
/*!
\file r2_setup_img_nav_but.hpp
\brief
\author Gareth Edwards
*/


#pragma once

#include "../header/r2_model.h"

using namespace review_02;


// ---------- SetupImgNavigation----------
/*!
\brief Setup image navigation display lists
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupImgNavigation()
{

	// ---- local
		INT result;
		FLOAT param[VS_MAX_ELM_FLOAT_PARAMETER];


	// ---- parameters
		FLOAT text_dim = 0.025f;


	// ---- versions
		UINT element_version_lut[3] =
		{
			vs_system::ElmContext::VERSION_BACKGROUND,
			vs_system::ElmContext::VERSION_ICON,
			vs_system::ElmContext::VERSION_OUTLINE
		};


	// ---- for each layer, button and element
		vs_system::ElmContext *elm_context = NULL;
		UINT elem_index = 0;

		for (UINT b = 0; b < 2; b++)
		{
			for (INT i = 0; i < 3; i++)
			{

				// ---- add
					pi_model->handle_elem_img_nav_but[elem_index] =
							gfx->AddElement(
									pi_model->handle_dspl_img_nav_but,
									"Img",
									IMG_BUTTON_GROUP_ID,
									b,
									&pi_model->obj_config->Callback_Button_Configure
								);


				// ---- get context
					INT get_element_result =
						gfx->GetElementContext(
									pi_model->handle_elem_img_nav_but[elem_index],
									&elm_context
								);

				// ---- init
					if ( get_element_result != NULL )
					{

						// ---- click
							elm_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_CLICK);


						// ---- both ON at runtime ?
							//elm_context->SetSwitchedOn(TRUE);
							//elm_context->SetMultiActive(TRUE, 0);


						// --- rest is standard 
							result = elm_context->SetVersion(element_version_lut[i]);
							result = elm_context->SetRGBA(1.0f, 1.0f, 1.0f, 0.5f);


						// ---- param
							result = pi_model->obj_config->ButtonSetDPT(
										param,
										vs_system::ElmContext::VIEWPORT_WIDTH |
										vs_system::ElmContext::VIEWPORT_HEIGHT_YLOC
									);
							result = pi_model->obj_config->ButtonSetLineAlpha(
									param,
									vs_system::ElmContext::LINE_ALPHA_OFF
								);
					
							FLOAT xloc = b == 0 ? 0 : 1 - text_dim;
							FLOAT yloc = 0.1f;
							FLOAT yhgt = text_dim * 2.5f;

							result = pi_model->obj_config->ButtonSetLocation(
										param,
										xloc,
										yloc,
										text_dim, yhgt
									);
							result = pi_model->obj_config->ButtonSetTexture(
										param,
										VS_ATLAS_PAGE_GROUP_THREE,
										0, 0, b == 0 ? 3 : 4, 2, 1, 1, 0.001f
									);
							result = elm_context->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);

					}


					// ---- increment array index
						elem_index++;

			}
		}


	return VS_OK;
}

