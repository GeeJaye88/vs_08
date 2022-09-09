////////////////////////////////////////////////////////////////////////////////

// ---------- model_panels_set_layout.hpp ----------
/*!
\file model_panels_set_layout.hpp
\brief Implementation of Model class set panel methods
\author Gareth Edwards
*/


////////////////////////////////////////////////////////////////////////////////


// ---------- SetPanelsLayoutDisplayStatus ----------
/*!
\brief Set panels layout display starus
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK

\note invoked by: application::SetupGraphics()

*/
INT Model::SetPanelsLayoutDisplayStatus(
	vs_system::AppDependency *app_dependency,
	vs_system::AppView *app_view
)
{

	// ---- local
		INT hr = VS_OK;
		INT result = VS_OK;

	// ---- store
		pi_model->SetAppView(app_view);


	// ---- layout
		vs_system::AppLayout app_layout;
		app_layout.Setup(app_dependency);


	// ---- view and control rects
		VsFloatRectangle rect_bounds[4];
		result = SetPanelsLayoutRectBounds(&app_layout, rect_bounds);


	// ---- pane lut
		vs_system::AppPane panel_lut[] =
		{
			{ "Main",        rect_bounds[0] },
			{ "Tint Head",   rect_bounds[1] },
			{ "Tint Body",   rect_bounds[1] },
			{ "Kpad Head",   rect_bounds[2] },
			{ "Kpad Body",   rect_bounds[2] },
			{ "Toolbar",     rect_bounds[3] },
		};
		UINT num_panel = sizeof(panel_lut) / sizeof(vs_system::AppPane);


	// ---- all off
		for (UINT i = 0; i < num_panel; i++)
		{
			app_view->SetPanelDisplayStatus(panel_lut[i].name, FALSE);
		}


	// ---- panel pane
		app_view->SetPanelPane(num_panel, panel_lut);


	// ---- fullscreen ?
		if ( pi_model->panels_main_fullscreen )
		{
			app_view->SetPanelDisplayStatus(panel_lut[0].name, TRUE);
		}
		else
		{

			// ---- on ?
				if ( pi_model->panels_visible & PI_Model::MAIN )
				{
					app_view->SetPanelDisplayStatus(panel_lut[0].name, TRUE);
				}

				if ( pi_model->panels_visible & PI_Model::TINT )
				{
					app_view->SetPanelDisplayStatus(panel_lut[1].name, TRUE);
					app_view->SetPanelDisplayStatus(panel_lut[2].name, TRUE);
				}

				if ( pi_model->panels_visible & PI_Model::KEYPAD )
				{
					app_view->SetPanelDisplayStatus(panel_lut[3].name, TRUE);
					app_view->SetPanelDisplayStatus(panel_lut[4].name, TRUE);
				}

				if ( pi_model->panels_visible & PI_Model::TOOLBAR )
				{
					app_view->SetPanelDisplayStatus(panel_lut[5].name, TRUE);
				}

		}

	// ---- store toolbar pane
		//
		// required to locate toolbar buttons
		//
		pi_model->toolbar_pane.rect = rect_bounds[3];


	// ---- reset keyboard
		pi_model->SetKeypadStatus(FALSE);



	return VS_OK;
}


// ---------- SetPanelsLayoutRectBounds ----------
/*! 
\brief Set panels layout rectangular boundaries
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK

\note ONLY sets param rect_bounds
\note invoked by: Model::SetPanelsLayoutDisplayStatus

\note work in progress:

	Assume aspect of:
		1.6000 (e.g. 1920x1200)  16:10 - common between 2003 and 2010
		1.7786 (e.g. 1366x768)   16:9  - common since 2008 (matches HDTV)
		1.7777 (e.g. 1920x1080)  16:9  - ditto

*/
INT Model::SetPanelsLayoutRectBounds(
		vs_system::AppLayout *app_layout,
		VsFloatRectangle rect_bounds[4]
	)
{

	// ---- fullscreen?
		vs_system::AppView *app_view = pi_model->GetAppView();
		if ( pi_model->panels_main_fullscreen )
		{
			app_view->SetPanelStatus("Main", vs_system::AppView::BORDER, FALSE);
			rect_bounds[0].left   = 0.0f;
			rect_bounds[0].right  = 1.0f;
			rect_bounds[0].top    = 0.0f;
			rect_bounds[0].bottom = 1.0f;
			return VS_OK;
		}
		else
		{
			app_view->SetPanelStatus("Main", vs_system::AppView::BORDER, TRUE);
		}


	// ---- system
		vs_system::AppDependency *app_dependency = app_layout->GetAppDependency();
		vs_system::SysContext    *sys_context = app_dependency->GetSysContext();
		UINT format = sys_context->GetDWord(vs_system::SysContext::FORMAT);


	// ---- get layout info
		VsFloatRectangle border_rect;
		INT   result = app_layout->GetBorderRect(&border_rect);
		FLOAT aspect = app_layout->GetValue(vs_system::AppLayout::ASPECT);
		FLOAT left   = border_rect.left;
		FLOAT right  = border_rect.right;


	// ---- local
		FLOAT space_x = app_layout->GetValue(vs_system::AppLayout::SPACE_X);
		FLOAT panels_toolbar_height = 0.06f;


	// ---- flags
		BOOL  tabs_on    = pi_model->panels_visible & PI_Model::TINT;
		BOOL  keypad_on  = pi_model->panels_visible & PI_Model::KEYPAD;


	// ---- choose format
		if ( format == vs_system::SysContext::LANDSCAPE )
		{

			// ---- main proportion (1/aspect * ?)
				FLOAT main_proportion = 1.0f;
				if ( aspect > 1.7 ) // both 16:9
				{
					main_proportion = 0.75f;
				}
				else if ( aspect > 1.5f ) // 16:10
				{
					main_proportion = 0.7f;
				}


			// ---- tint & keypad proportion
				FLOAT keypad_proportion = 0.16f;
				FLOAT tabs_proportion    = 1 - main_proportion;


			// ---- x divisions
				FLOAT x[5]  = { 0, 0, 0, 0, 0 };


			// ---- set rect division between view right and tint left
				if ( pi_model->panels_aligned == PI_Model::LEFT )
				{

					// ---- default is keypad off screen right 
						FLOAT rmu[4] = { 0.0f, main_proportion, main_proportion + tabs_proportion, 1.0f + keypad_proportion };

					// ---- keypad ? - shift left
						if ( keypad_on )
							for ( UINT i=1; i < 4; i++ ) rmu[i] -= keypad_proportion;

					// ---- x bounds
						for (UINT i = 0; i < 4; i++)
							x[i] = (1 - rmu[i]) * left + rmu[i] * right;

					// ---- rect
						for (UINT i = 0; i < 3; i++)
						{
							rect_bounds[i].left  = x[i];
							rect_bounds[i].right = x[i + 1];
						}

					// ---- main
						rect_bounds[0].left  = border_rect.left;
						rect_bounds[0].right = rmu[1];

				}
			// ---- set rect division between tint right and view left
				else
				{
					// ---- default is keypad off screen left
						FLOAT rmu[4] = { -keypad_proportion, 0, tabs_proportion, 1.0f };

					// ---- keypad ? - shift right
						if ( keypad_on )
							for ( UINT i=0; i < 3; i++ ) rmu[i] += keypad_proportion;

					// ---- x bounds
						for (UINT i = 0; i < 4; i++)
							x[i] = (1 - rmu[i]) * border_rect.left + rmu[i] * border_rect.right;

					// ---- rect
						UINT r = 2;
						for (UINT i = 0; i < 3; i++)
						{
							rect_bounds[r].left  = x[i];
							rect_bounds[r].right = x[i + 1];
							r--;
						}

					// ---- main
						rect_bounds[0].left  = rmu[2];
						rect_bounds[0].right = border_rect.right;

				}


			// ---- tint & keypad
				for (UINT i=0; i<3; i++)
				{
					rect_bounds[i].top    = border_rect.top + panels_toolbar_height;
					rect_bounds[i].bottom = border_rect.bottom;
				}

			// ---- main
				if ( !tabs_on && !keypad_on )
				{
					rect_bounds[0].left  = border_rect.left;
					rect_bounds[0].right = border_rect.right;
				}

			// ---- toolbar
				rect_bounds[3].left   = border_rect.left;
				rect_bounds[3].right  = border_rect.right;
				rect_bounds[3].top    = 0.0f;
				rect_bounds[3].bottom = 1 - panels_toolbar_height;
		}

	return VS_OK;
}


// ---------- SetPanelsLayoutKpadStatus ----------
/*!
\brief Set panels keypad panel layout
\author Gareth Edwards
\param BOOL [in] status, either TRUE (ON) or FALSE (OFF)
\return INT - VS_ERROR <= NULL < VS_OK

\note location of keypad as per main panel layout mode

\note invoked by Model::SetKeyPad & 

*/
INT Model::SetPanelsLayoutKpadStatus(BOOL status)
{
	return VS_OK;
}


// ---------- SwapPanelsLayoutAlignedSide ----------
/*!
\brief Set panels layout aligned side
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK

\note invoked by Model::SetMsgLayout ( s[...]_model_panel_set.hpp )

*/
INT Model::SwapPanelsLayoutAlignedSide()
{

	pi_model->panels_aligned =
		pi_model->panels_aligned == PI_Model::RIGHT ?
			PI_Model::LEFT : PI_Model::RIGHT;

	SetPanelsLayoutUpdate();

	return VS_OK;
}


// ---------- SetPanelsLayoutByElemId ----------
/*!
\brief Set panels layout visibility bitmap
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK

\note invoked by

	Kontrol::CallbackByPickElement ( [app space name]\source\[app prefix]_kontrol.cpp ) 

*/
INT Model::SetPanelsLayoutByElemId(UINT elem_id)
{

	// ---- process
		switch ( elem_id )
		{

			case TOOLBAR_ELEM_ID_SIDE:
			case TOOLBAR_ELEM_ID_SIDE+1:
				{
					SwapPanelsLayoutAlignedSide();
				}
				break;

			case TOOLBAR_ELEM_ID_LAYOUT:
			case TOOLBAR_ELEM_ID_LAYOUT+1:
			case TOOLBAR_ELEM_ID_LAYOUT+2:
				{

					// ---- create bitmap
						UINT index = elem_id - TOOLBAR_ELEM_ID_LAYOUT;
						switch (index)
						{
							case 0:
								pi_model->panels_visible = PI_Model::MAIN_COMPLETE;
								break;
							case 1:
								pi_model->panels_visible = PI_Model::TINT_COMPLETE;
								break;
							case 2:
								pi_model->panels_visible = PI_Model::KEYPAD_COMPLETE;
								break;
						}

					// --- update
						SetPanelsLayoutUpdate();

				}
				break;

			case TOOLBAR_ELEM_ID_EXIT:
				break;

			default:
				break;
		}

	return VS_OK;
}


// ---------- SetPanelsLayoutByCorner ----------
/*!
\brief Set panels layout by corner (see note)
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK

\note particularly required when only MAIN panel
is visible in full screen mode

\note invoked ONLY by Kontrol::CallbackByPickPanel

\note the panel layout is set:

Either landscape or potrait format-
	0: full

Landscape format-
	1: left
	2: right

Portrait format-
	1: top
	2: bottom

*/
INT Model::SetPanelsLayoutByCorner()
{

	// ---- local
		vs_system::AppDependency *app_dependency = pi_model->app_dependency;
		vs_system::GfxInterface  *gfx_interface = app_dependency->GetGfxInterface();
		vs_system::SysInput      *sys_input = app_dependency->GetSysInput();


	// ---- get Client Adjusted Viewport (CAV)
		VsFloatRectangle cav = { 0, 0, 0, 0 };
		gfx_interface->GetClientAdjustedViewport(&cav);


	// ---- pick with CAV
		POINT pick_point = { 0, 0 };
		pick_point   = *sys_input->GetPosition();
		FLOAT x_pick = (FLOAT)pick_point.x - cav.left;
		FLOAT y_pick = (FLOAT)pick_point.y - cav.top;
		FLOAT width  = cav.right  - cav.left;
		FLOAT height = cav.bottom - cav.top;
		FLOAT wby2   = width / 2;
		FLOAT hby2   = height / 2;
		FLOAT aspect = width / height;


	// ---- pick prop
		FLOAT y_prop = 0.1f;
		FLOAT x_prop = y_prop * aspect;
		FLOAT x = x_pick / width;
		FLOAT y = y_pick / height;


	// ---- which corner ?

		const UINT top_left = 1,
			bottom_left     = 2,
			top_right       = 3,
			bottom_right    = 4;

		INT corner = 0;
		if ( x < x_prop )
		{
			if ( y < y_prop )
			{
				corner = top_left;
			}
			else if ( y > 1 - y_prop )
			{
				corner = bottom_left;
			}
		}
		else if ( x > 1 - x_prop )
		{
			if ( y < y_prop )
			{
				corner = top_right;
			}
			else if ( y > 1 - y_prop )
			{
				corner = bottom_right;
			}
		}


	// ---- corner selected ?
		if ( corner )
		{

			// ---- flip
				pi_model->panels_main_fullscreen = pi_model->panels_main_fullscreen ? FALSE : TRUE;

			// --- re/store visibility
				if ( pi_model->panels_main_fullscreen )
				{
					pi_model->panels_visible_previously = pi_model->panels_visible;
				}
				else
				{
					pi_model->panels_visible = pi_model->panels_visible_previously;
				}

			// --- update
				SetPanelsLayoutUpdate();

			// ---- panel picked
				return VS_OK;
		}

	return VS_ERROR;
}


// ---------- SetPanelsLayoutUpdate ----------
/*!
\brief Set panels layout update
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK

\note invoked by Model::SetKeyPad & 

*/
INT Model::SetPanelsLayoutUpdate()
{

	// ---- local
		INT hr = VS_OK;
		vs_system::AppView       *app_view = pi_model->GetAppView();
		vs_system::AppDependency *app_dependency = pi_model->app_dependency;
		vs_system::SysContext    *sys_context = app_dependency->GetSysContext();
		vs_system::GfxInterface  *gfx = app_dependency->GetGfxInterface();


	// ---- set panel locations
		SetPanelsLayoutDisplayStatus(app_dependency, pi_model->app_view);


	// ---- configure tint display lists
		pi_model->tint_setng->SetDisplayListsConfigure();
		pi_model->kpad_setng->SetDisplayListsConfigure();


	// ---- configure toolbar display list
		gfx->SetDisplayListConfigure(pi_model->handle_dspl_toolbar, VS_ELM_SETUP_GEOMETRY);


	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////