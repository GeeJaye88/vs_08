////////////////////////////////////////////////////////////////////////////////

// ---------- kpad_kontrol.hpp ----------
/*!
\file kpad_kontrol.hpp
\brief 
\author Gareth Edwards
*/


// ---------- KpadSetDisplayListHandles ----------
/*!
\brief Set tint display list handles
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Kontrol::KpadSetDisplayListHandles()
{

	// ---- local
		INT result = 0;
		vs_system::AppKontrol *app_kontrol = pi_kontrol->app_kontrol;
		Model *model = (Model *)app_kontrol->GetHandleModel();
		vs_system::AppView *app_view = app_kontrol->GetAppView();


	// ---- Tint - required
		//
		// To init head and body display lists, see:
		//    s3_model_tint_get.hpp
		//
		app_view->SetPanelDisplayListHandle(
				"Kpad Head",
				model->KpadGetHeadDisplayListHandle()
			);
		app_view->SetPanelDisplayListHandle(
				"Kpad Body",
				model->KpadGetBodyDisplayListHandle()
			);

	return VS_OK;
}


// ---------- KpadUpdateHead ----------
/*!
\brief Update a Tint Head
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value

\note

*/
INT Kontrol::KpadUpdateHead(
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
			case KPAD_HEAD_LID:
				{
					if ( handle_element != 0 )
					{

						// ---- has to be ok.... to get to here!!!
							vs_system::ElmContext *elem_context = NULL;
							result = app_dependency->GetGfxInterface()->GetElementContext(handle_element, &elem_context);

						// ---- select
							INT body_id = elem_context->GetId();
							model->KpadBodySelect( body_id );

						// ---- re-init body display list to selected body display list
							app_view->SetPanelDisplayListHandle(
									"Kpad Body",
									model->KpadGetBodyDisplayListHandle()
								);
					}
				}
				break;
			default:
				break;
		}

	return VS_OK;
}



// ---------- KpadUpdateBody ----------
/*!
\brief Update a Kpad Body
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Kontrol::KpadUpdateBody(
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
			case KPAD_BODY_INPUT_LID:
				model->Kpad_Input_GetCallbacks(&tint_callbacks);
				break;
			default:
				return VS_ERROR;
		}


	// ---- has to be ok.... to get to here!!!
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


// ---------- KpadUpdateText ----------
/*!
\brief Update tabbed interface text and values
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Kontrol::KpadUpdateText(
		vs_system::AppKontrol *app_kontrol
	)
{

	INT result = 0;

	result = KpadUpdateBody(app_kontrol, KPAD_BODY_INPUT_LID, 0);

	return VS_OK;
}
