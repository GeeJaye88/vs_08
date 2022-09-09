////////////////////////////////////////////////////////////////////////////////

// ---------- r2_control_pimp.hpp ----------
/*!
\file r2_control_pimp.hpp
\brief 
\author Gareth Edwards
*/


#include "../header/r2_model.h"


using namespace review_02;


////////////////////////////////////////////////////////////////////////////////


// ---------- UpdatePanel ----------
/*!
\brief Update panel
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK

\note

	Context:

		IF panel is NOT NULL THEN
		   context == "immediate" panel context
		ELSE
		   context == "persistant" panel context

*/
INT Control::UpdatePanel(
		INT handle_control_obj,
		INT handle_panel_obj
	)
{

	// ---- local
		INT result = VS_OK;
		Control* control = (Control*)handle_control_obj;


	// ---- IF NO interaction THEN return
		BOOL interaction_status = TRUE;
		result = control->app_view->GetPanelInteractionStatus(NULL, &interaction_status);
		if ( !interaction_status ) return VS_OK;


	// ---- local
		vs_system::AppControl *app_control = control->app_control;
		vs_system::AppPanel *app_panel = (vs_system::AppPanel *)handle_panel_obj;


	// ---- set context
		vs_system::PanelContext *panel_context = NULL;
		if ( app_panel != NULL )
			panel_context = app_panel->GetPanelContext();
		else
			panel_context = app_control->persistant_panel_context;


	// ---- set FOV
		result = SetMouseWheelFov(
				control,
				control->app_dependency,
				panel_context
			);
		if (result == VS_OK) return VS_OK;


	// ---- select frame
		result = SetPanelActive(
				control,
				control->app_dependency,
				app_panel,
				panel_context
			);
		if ( result == VS_OK ) return VS_OK;


	// ---- get panel group id
		UINT panel_group_id = 0;
		if ( panel_context != NULL )
			panel_context->GetGroupId(&panel_group_id);


	// ---- select panel
		switch ( panel_group_id )
		{
			case GROUP_ELEM_IMAGE:
				UpdateElemImage(
						control,
						app_control,
						app_panel,
						panel_context,
						panel_group_id
					);
				break;
			default:
				UpdateElemPano(
						control,
						app_control,
						app_panel,
						panel_context,
						panel_group_id
					);
				break;
		}

	return VS_OK;
}


// ---------- SetPanelActive ----------
/*!
\brief set active panel
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK

\note

	Active:

		If panel NOT active THEN
			set panel active and return VS_OK
		ELSE
			return VS_ERROR

*/
INT Control::SetPanelActive(
		Control *control,
		vs_system::AppDependency *app_dependency,
		vs_system::AppPanel      *app_panel,
		vs_system::PanelContext  *panel_context
	)
{

	// ---- context is NULL ?
	if ( panel_context != NULL )
	{

		// ---- local
			vs_system::AppControl   *app_control = control->app_control;
			vs_system::SysInput     *sys_input = app_dependency->GetSysInput();
			vs_system::GfxInterface *gfx_interface = app_dependency->GetGfxInterface();


		// ---- only select via gui panel pick
			if ( app_panel != NULL )
			{

				// ---- non-zero id ?
					UINT id = 0;
					INT result = panel_context->GetId(&id);
					if ( id != 0 )
					{

						// ---- temp
						char msg[64];
						sprintf(msg, "--- Click  lid %d\n", id);
						OutputDebugString(msg);


						// ---- NOT selected
							BOOL selected = FALSE;
							result = panel_context->GetSelected(&selected);
							if ( ! selected )
							{ 

								// ---- select - "IMG_PTZ_[...]" ---- OK

								// ---- ERROR default
									result = VS_ERROR;

									#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

									Model *model = control->model;
									UINT id_overview = vs_system::ObjConfig::IMG_PTZ_OVERVIEW_PANEL_INDEX + 1;
									UINT id_first    = vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX + 1;
									UINT id_last     = vs_system::ObjConfig::IMG_PTZ_LAST_PANEL_INDEX + 1;

									if ( id == id_overview)
									{
										;
									}
									else if ( id >= id_first && id <= id_last )
									{
										result = control->app_view->SelectPanel(app_panel);
										result = model->Update_ImgPanel_Config(id);
									}
									else
									{
										result = control->app_view->SelectPanel(app_panel);
									}

									#else
									
									result = control->app_view->SelectPanel(app_panel);
									
									#endif

									 
								// ---- selected
									if ( result == VS_OK )
									{

										// ---- temp
											char msg[64];
											sprintf(msg, "--- Select lid %d\n", id);
											OutputDebugString(msg);
											 

										// ---- id
											Model *model = control->model;
											vs_system::ObjConfig *obj_config = model->GetObjConfig();


										// ---- set previous
											//UINT active_id = 0;
											//result = obj_config->GetImagePanelActiveId(&active_id);
											//result = obj_config->SetImagePanelPreviousId(active_id);


										// ---- select
											result = obj_config->SetPanoPtfSelectedById(id);

											#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)


												// ---- ONLY "IMG_PTZ_[...]" ?
													UINT id_first = vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX + 1;
													UINT id_last  = vs_system::ObjConfig::IMG_PTZ_LAST_PANEL_INDEX + 1;
													if ( id >= id_first && id <= id_last)
													{
														// ONLY select between first and last
														result  = model->Set_ImgPanel_ActiveId(id);

														// ONLY does what it says on the tin...
														result = model->Set_ImgOrPanoPanel_IconSelected(id);
													}


												// ---- update tabbed image interface text and values (e.g. PTF)
													UpdateTintImgText(control);

											#else

												// ---- set img stuff
													result = model->Set_ImgPanel_ActiveId(id);             // 1 to 7
													result = model->Set_ImgOrPanoPanel_IconSelected(id);   // 1 to 8

												// ---- set all "2D" prefixed elements to be re-configured
													BOOL pan_2d_vertex_buffer;
													INT result = obj_config->GetPanoPanVertexBuffer(&pan_2d_vertex_buffer);
													if ( pan_2d_vertex_buffer )
														result = gfx_interface->SetGlobalConfigureBitmask(
																VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING,
																"2D"
															);

											#endif


										// --- tidy - then done...
											sys_input->ResetMouse();
											app_control->persistant_panel_context = NULL;
											return VS_OK;

									}

							} // ! selected
					}
			}
	}

	return VS_ERROR;
}


// ---------- SetMouseWheelFov ----------
/*!
\brief Set mouse wheel FOV value
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Control::SetMouseWheelFov(
		Control *control,
		vs_system::AppDependency *app_dependency,
		vs_system::PanelContext *panel_context
	)
{

	//
	// Note: Interaction status gated by invoking UpdatePanel method
	//

	// IF -- context is NULL -- THEN
	//    IF -- wheel change -- THEN
	//       IF -- context = "over" panel -- THEN
	//          Update FOV
	// ResetMouse
	// Return
	//

	if ( panel_context == NULL )
	{

		// ---- local
			vs_system::AppControl *app_control = control->app_control;


		// ---- wheel change ?
			//    Note: already tested if invoked from
			//    AppControl->UpdateInput method ) 
			//
			vs_system::SysInput *sys_input = app_dependency->GetSysInput();
			BOOL wheel_change = sys_input->GetBool(sys_input->WHEEL_CHANGE);
			if ( wheel_change )
			{

				// ---- context = "over" panel ?
					vs_system::GfxInterface  *gfx_interface = app_dependency->GetGfxInterface();
					INT result = gfx_interface->GetContextOverPanel(&panel_context);
					if ( panel_context )
					{

						// ---- get PTF id
							UINT ptf_id = 0;
							Model *model = control->model;
							vs_system::ObjConfig *obj_config = model->GetObjConfig();
							INT result = obj_config->GetPanoPTFId(&ptf_id);


						// ---- system context stuff
							vs_system::SysContext *sys_context = app_dependency->GetSysContext();
							char *config_name = sys_context->GetChar(vs_system::SysContext::CONFIG_NAME);
							BOOL touch        = sys_context->GetBool(vs_system::SysContext::TOUCH);
							INT  wheel_clicks = sys_input->GetLong(sys_input->WHEEL_CLICKS);


						// ---- ZOOM: mouse wheel get & set PTF(or Z) parameters
							FLOAT pan, tilt, fov;
							obj_config->GetPanoPTFById(ptf_id, &pan, &tilt, &fov);
								fov += (FLOAT)app_control->wheel_clicks * ( touch ? -5 : 1 );
								fov = fov <= 1 ? 1 : fov;
								obj_config->SetPanoPTFById(ptf_id, pan, tilt, fov);


						// --- yay - special case

							#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)


								// ---- update
									result = model->Update_ImgPanel_Config(ptf_id);


								// ---- report
									
									CHAR msg[VS_MAXCHARLEN];
									sprintf(msg, "--- F %d %6.3f\n", ptf_id, fov);
									OutputDebugString(msg);
									

								// ---- update tabbed interface image text and value
									UpdateTintImgText(control);

							#endif

					}
			}

		// ---- reset mouse and zap context (if any)
			sys_input->ResetMouse();
			app_control->persistant_panel_context = NULL;

		return VS_OK;
	}

	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////


