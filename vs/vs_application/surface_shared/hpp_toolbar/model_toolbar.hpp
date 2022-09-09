////////////////////////////////////////////////////////////////////////////////

// ---------- model_toolbar.hpp ----------
/*!
\file model_panel.hpp
\brief 
\author Gareth Edwards
*/



// ---------- ToolbarButtonsSetup ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note only ONCE invoked at runtime by app SetupGraphics methods

*/
INT Model::ToolbarButtonsSetup()
{

	// ---- ?
		if (pi_model->runtime)
		{

			// ---- add display list
				pi_model->handle_dspl_toolbar = pi_model->gfx->AddDisplayList("Toolbar", 1, 1);

		}

	// ---- ?
		if (pi_model->runtime)
		{
				// ---- for each button
					INT handle_index = 0;
					INT num_buttons = 6;
					INT last_button = num_buttons-1;
					for (INT b = 0; b < num_buttons; b++)
					{

						// ---- exit ? - add outline
							INT num_elem = 2;
							BOOL exit_button = b == last_button ? TRUE : FALSE;
							if ( exit_button )
							{
								num_elem = 3;
							}

						// ---- for each version
							for (INT v = 0; v < num_elem; v++)
							{

								// ---- add element ----
									pi_model->handle_elem_toolbar[handle_index++] =
									pi_model->gfx->AddElement(
											pi_model->handle_dspl_toolbar,
											"Toolbar",
											1,
											1,
											&pi_model->obj_config->Callback_Button_Configure
										);
							}
					}
			}

			INT result = ToolbarButtonsConfig();

	return VS_OK;
}


// ---------- ToolbarButtonsConfig ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::ToolbarButtonsConfig()
{

	// ---- local
		INT   result = 0;
		FLOAT param[VS_MAX_ELM_FLOAT_PARAMETER];


	// ---- versions
		UINT element_version_lut[3] =
		{
			vs_system::ElmContext::VERSION_BACKGROUND,
			vs_system::ElmContext::VERSION_ICON,
			vs_system::ElmContext::VERSION_OUTLINE
		};

		 
	// ---- button_size equal to vs_system::Tint::ICON_WIDTH
		//
		//  see s[...]_model_tint_get.hpp
		//
		FLOAT button_size     = 0.05f;
		FLOAT button_space    = button_size  * 0.10f;
		FLOAT button_x_offset = button_size  * 0.85f;
		FLOAT button_y_offset = button_size  * 0.22f;


	// ---- application window layout
		vs_system::AppLayout app_layout;
		app_layout.Setup(pi_model->app_dependency);


	// --- NOT "Main" fullscreen ?
		VsFloatRectangle *panel_border_rect = &pi_model->toolbar_rect;
		if ( ! pi_model->panels_main_fullscreen )
		{
			// ---- get "Toolbar" context
				vs_system::PanelContext *panel_context;
				pi_model->app_view->GetPanelContext("Toolbar", &panel_context);

			// ---- get toolbar rect
				VsFloatRectangle *panel_border_rect = panel_context->GetViewrect();

			// ---- store (to preserve rect when "Main" fullscreen)
				pi_model->toolbar_rect = *panel_border_rect;
		}


	// ---- application window layout
		FLOAT aspect = app_layout.GetValue(vs_system::AppLayout::ASPECT);


	// ---- set panel dimensions & aspect
		FLOAT panel_width  = panel_border_rect->right - panel_border_rect->left;
		FLOAT panel_height = panel_border_rect->bottom - panel_border_rect->top;
		FLOAT panel_aspect = panel_width / panel_height;


	// ---- recalc panel width proportional to panel height
		panel_width *= panel_aspect * aspect;


	// ---- buttons
		struct button {
			FLOAT x    = 0;
			FLOAT y    = 0;
			UINT  gid  = 0;
			UINT  id   = 0;
			UINT  on   = 0;
			UINT  row  = 0;
			UINT  col  = 0;
		};

		auto x_left = [button_size, button_space, button_x_offset](FLOAT b)
		{
			return b * (button_size + button_space) + button_x_offset;
		};

		auto x_right = [panel_width, button_size, button_space, button_x_offset](FLOAT b)
		{
			return panel_width - b * (button_size + button_space) - button_x_offset;
		};

		button toolbar_buttons[6] =
		{
			{ x_left(0),  button_y_offset, TOOLBAR_GROUP_ID_SIDE,   TOOLBAR_ELEM_ID_SIDE,     0, 6, 3 },
			{ x_left(1),  button_y_offset, TOOLBAR_GROUP_ID_SIDE,   TOOLBAR_ELEM_ID_SIDE+1,   1, 6, 4 },
			{ x_right(4), button_y_offset, TOOLBAR_GROUP_ID_LAYOUT, TOOLBAR_ELEM_ID_LAYOUT,   0, 6, 5 },
			{ x_right(3), button_y_offset, TOOLBAR_GROUP_ID_LAYOUT, TOOLBAR_ELEM_ID_LAYOUT+1, 1, 6, 6 },
			{ x_right(2), button_y_offset, TOOLBAR_GROUP_ID_LAYOUT, TOOLBAR_ELEM_ID_LAYOUT+2, 0, 6, 7 },
			{ x_right(0), button_y_offset, TOOLBAR_GROUP_ID,        TOOLBAR_ELEM_ID_EXIT,     0, 5, 7 }
		};


	// ---- for each button
		INT handle_index = 0;
		INT num_buttons = 6;
		INT last_button = num_buttons-1;
		for (INT b = 0; b < num_buttons; b++)
		{

			// ---- exit ? - add outline
				INT num_elem = 2;
				BOOL exit_button = b == last_button ? TRUE : FALSE;
				if ( exit_button )
				{
					num_elem = 3;
				}

			// ---- for each version
				for (INT v = 0; v < num_elem; v++)
				{

					// ---- reconfig element
						result = pi_model->gfx->SetElementConfigureBitmask(
								pi_model->handle_elem_toolbar[handle_index],
								VS_ELM_SETUP_GEOMETRY
							);

					// ---- initialise
						vs_system::ElmContext *elm_context = NULL;
						INT get_element_context =
							pi_model->gfx->GetElementContext(
									pi_model->handle_elem_toolbar[handle_index++],
									&elm_context
								);
						if ( elm_context != NULL )
						{

							// ---- element properties
								result = elm_context->SetBehaviour(
									exit_button ?
										vs_system::ElmContext::BEHAVIOUR_CLICK :
											vs_system::ElmContext::BEHAVIOUR_RADIO);
								result = elm_context->SetVersion(element_version_lut[v]);
								result = elm_context->SetRGBA(1.0f, 1.0f, 1.0f, 0.5f);
								result = elm_context->SetGroupId(toolbar_buttons[b].gid);
								result = elm_context->SetId(toolbar_buttons[b].id);
								result = elm_context->SetSwitchedOn(toolbar_buttons[b].on);

							// ---- button configuration parameters
								result = pi_model->obj_config->ButtonSetDPT(
										param,
										vs_system::ElmContext::VIEWPORT_HEIGHT
									);
								result = pi_model->obj_config->ButtonSetLocation(
										param,
										toolbar_buttons[b].x,
										toolbar_buttons[b].y,
										button_size, button_size
									);
								result = pi_model->obj_config->ButtonSetTexture(
										param,
										VS_ATLAS_PAGE_GROUP_FIVE,
										0, 0,
										toolbar_buttons[b].col, toolbar_buttons[b].row,
										1, 1, 0.001f
									);
								result = elm_context->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);
						}
				}
		}

	return VS_OK;
}

