////////////////////////////////////////////////////////////////////////////////

// ---------- s2_tint_kontrol.hpp ----------
/*!
\file s2_tint_kontrol.hpp
\brief 
\author Gareth Edwards
*/

#pragma once

using namespace surface_02;


// ---------- TintSetDisplayListHandles ----------
/*!
\brief Set tint display list handles
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value

\note invoked by application::SetupGraphics()

*/
INT Kontrol::TintSetDisplayListHandles()
{

	// ---- local
		INT result = 0;
		vs_system::AppKontrol *app_kontrol = pi_kontrol->app_kontrol;
		Model *model = (Model *)app_kontrol->GetHandleModel();
		vs_system::AppView *app_view = app_kontrol->GetAppView();


	// ---- Tint - required
		//
		// To init head and body display lists, see:
		//    s2_model_tint_get.hpp
		//
		app_view->SetPanelDisplayListHandle(
				"Tint Head",
				model->TintGetHeadDisplayListHandle()
			);
		app_view->SetPanelDisplayListHandle(
				"Tint Body",
				model->TintGetBodyDisplayListHandle()
			);

	return VS_OK;
}


// ---------- TintUpdateHead ----------
/*!
\brief Update a Tint Head
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value

\note invoked by Kontrol::CallbackByPickElement

*/
INT Kontrol::TintUpdateHead(
		vs_system::AppKontrol *app_kontrol,
		INT group_id,
		INT handle_element
	)
{

	// ---- local
		INT result = 0;
		Model *model = (Model   *)app_kontrol->GetHandleModel();
		vs_system::AppView       *app_view = app_kontrol->GetAppView();
		vs_system::AppDependency *app_dependency = app_kontrol->GetAppDependency();


	// ---- select
		//
		//  if selected then
		//   - zero zap named display list forcing display list re-initialisation
		//   - select associated tab body
		//
		switch ( group_id )
		{
			case TINT_HEAD_2DS_LID:
				{
					if ( handle_element != 0 )
					{

						// ---- has to be ok.... to get to here!!!
							vs_system::ElmContext *elem_context = NULL;
							result = app_dependency->GetGfxInterface()->GetElementContext(handle_element, &elem_context);

						// ---- get tint body id
							INT body_id = elem_context->GetId(); 

						// ---- select & update
							model->TintBodySelect(body_id);
							model->TintBodyUpdate(body_id);

						// ---- re-init body display list to selected body display list
							app_view->SetPanelDisplayListHandle(
									"Tint Body",
									model->TintGetBodyDisplayListHandle()
								);
					}
				}
				break;
			default:
				break;
		}

	return VS_OK;
}



// ---------- TintUpdateBody ----------
/*!
\brief Update a Tint Body
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Kontrol::TintUpdateBody(
		vs_system::AppKontrol *app_kontrol,
		INT group_id,
		INT handle_element
	)
{

	// ---- local
		INT result = 0;
		BOOL reset_mouse = TRUE;
		vs_system::AppDependency *app_dependency = app_kontrol->GetAppDependency();
		Model *model = (Model *)app_kontrol->GetHandleModel();


	// ---- get callbacks
		vs_system::TintCallback tint_callbacks;
		switch ( group_id )
		{

			case TINT_BODY_ACQUIRE_LID:
			case TINT_BODY_ACQUIRE_ARRAY_LID:
			case TINT_BODY_ACQUIRE_PARAM_LID:
				model->Tint_Acquire_GetCallbacks(&tint_callbacks);
				break;

			case TINT_BODY_EXPLORE_LID:
			case TINT_BODY_EXPLORE_LID_GROUP_1:
			case TINT_BODY_EXPLORE_LID_GROUP_2:
				model->Tint_Explore_GetCallbacks(&tint_callbacks);
				break;

			case TINT_BODY_FUNCTION_LID:
			case TINT_BODY_FUNCTION_PARAM_LID:
				model->Tint_Function_GetCallbacks(&tint_callbacks);
				break;

			default:
				return VS_ERROR;
		}


	// ---- has to ( / should )be ok.... to get to here!!!
		vs_system::ElmContext *elem_context = NULL;
		result = app_dependency->GetGfxInterface()->GetElementContext(handle_element, &elem_context);
		INT element_id = elem_context->GetId();


	// ---- invoke model update
		if ( handle_element != 0 )
		{
			tint_callbacks.callback_update_model(
				(INT)model,
					element_id
				);
		}


	// ---- invoke text update ----
		tint_callbacks.callback_update_element(
				(INT)model,
				handle_element == 0 ? 0 : element_id
			);


	return VS_ELM_SETUP_NONE;
}


// ---------- UpdateTintText ----------
/*!
\brief Update tabbed interface text and values
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Kontrol::TintUpdateText(
		vs_system::AppKontrol *app_kontrol
	)
{

	INT result = 0;

	result = TintUpdateBody(app_kontrol, TINT_BODY_ACQUIRE_LID, 0);
	result = TintUpdateBody(app_kontrol, TINT_BODY_ACQUIRE_ARRAY_LID, 0);
	result = TintUpdateBody(app_kontrol, TINT_BODY_ACQUIRE_PARAM_LID, 0);

	result = TintUpdateBody(app_kontrol, TINT_BODY_EXPLORE_LID, 0);
	result = TintUpdateBody(app_kontrol, TINT_BODY_EXPLORE_LID_GROUP_1, 0);
	result = TintUpdateBody(app_kontrol, TINT_BODY_EXPLORE_LID_GROUP_2, 0);

	result = TintUpdateBody(app_kontrol, TINT_BODY_FUNCTION_LID, 0);
	result = TintUpdateBody(app_kontrol, TINT_BODY_FUNCTION_PARAM_LID, 0);

	return VS_OK;
}
