////////////////////////////////////////////////////////////////////////////////

// ---------- r2_img_panels.hpp ----------
/*!
\file r2_ptf_panels.hpp
\brief
\author Gareth Edwards

\note

*/


#include "../header/r2_model.h"

using namespace review_02;


// ---------- Copy_ImgPanel_ByFrameId ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note ok - but nasty conditional spaghetti
*/
INT Model::Copy_ImgPanel_ByFrameId(
	UINT dest_frame,
	UINT src_frame
)
{

	// --- return ?
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
			;
		#else
			return VS_ERROR;
		#endif


	// ---- "IMG_PTZ_[...]" ---- OK


	// ---- local
		INT result = VS_OK;
		UINT ap_id = pi_model->active_panel_id;
		vs_system::ObjConfig  *obj_config  = pi_model->obj_config;
		vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();


	// ---- format (with extra navigation panels)
		DWORD format = sys_context->GetBool(vs_system::SysContext::FORMAT);


	// ---- valid "copy" frame id's
		UINT overview_frame_id  = vs_system::ObjConfig::IMG_PTZ_OVERVIEW_FRAME_ID;
		UINT multiview_frame_id = vs_system::ObjConfig::IMG_PTZ_MULTIVIEW_FRAME_ID;
		UINT soloview_frame_id  = vs_system::ObjConfig::IMG_PTZ_SOLOVIEW_FRAME_ID;
		UINT settings_frame_id  = vs_system::ObjConfig::IMG_PTZ_SETTINGS_FRAME_ID;


	// ---- valid frame id ? -- else return
		if (
				dest_frame == overview_frame_id  ||
				dest_frame == multiview_frame_id ||
				dest_frame == soloview_frame_id  ||
				dest_frame == settings_frame_id
			)
		{
			if (
					src_frame == overview_frame_id  ||
					src_frame == multiview_frame_id ||
					src_frame == soloview_frame_id  ||
					src_frame == settings_frame_id
				)
			{
				;
			}
			else
			{
				return VS_ERROR;
			}

		}
		else
		{
			return VS_ERROR;
		}


	// ---- panel id shortcut
		UINT soloview_panel_id = vs_system::ObjConfig::IMG_PTZ_SOLO_PANEL_INDEX + 1;
		UINT settings_panel_id = vs_system::ObjConfig::IMG_PTZ_SETTINGS_PANEL_INDEX + 1;


	// ---- not copied ? -- then ignore -- 
		if ( !pi_model->copyied_panel_ptf )
		{
			if ( dest_frame != overview_frame_id || dest_frame != settings_frame_id )
			{
				;
			}
			else
			{
				return VS_ERROR;
			}
			pi_model->copyied_panel_ptf = TRUE;
		}


	// ---- same src ? -- then ignore --
		if ( pi_model->copyied_src_frame == src_frame )
		{
			return VS_ERROR;
		}


	// --- source
		UINT src_id = 0;
		if ( src_frame == overview_frame_id || src_frame == multiview_frame_id )
		{
			src_id = ap_id;
		}
		else if ( src_frame == soloview_frame_id )
		{
			src_id = soloview_panel_id;
		}
		else if ( src_frame == settings_frame_id )
		{
			src_id = settings_panel_id;
		}


	// --- destination
		UINT dest_id = 0;
		if ( dest_frame == overview_frame_id )
		{
			dest_id = ap_id;
		}
		else if ( dest_frame == multiview_frame_id )
		{
			dest_id = ap_id;
		}
		else if ( dest_frame == soloview_frame_id )
		{
			dest_id = soloview_panel_id;
		}
		else if ( dest_frame == settings_frame_id )
		{
			dest_id = settings_panel_id;
		}


	// --- copy

		/*
		CHAR msg[32];
		sprintf(msg, "--- copy - %d from %d\n", (INT)dest_id, (INT)src_id);
		OutputDebugString(msg);
		*/

		{

			if ( dest_id != settings_panel_id && src_id != settings_panel_id )
			{
				result = obj_config->CopyPanoPTF(dest_id, src_id);
			}
			else
			{
				// 190107 - portrait (with extra navigation panels) ?
				if ( format == vs_system::SysContext::PORTRAIT )
					result = obj_config->CopyPanoPTF(dest_id, src_id);
			}

			result = obj_config->SetPanoPtfSelectedById(dest_id);
			result = obj_config->SetImagePanelActiveId(dest_id);
			result = Update_ImgPanel_Config(dest_id);

		}


	// ---- store src frame id
		pi_model->copyied_src_frame = src_frame;


	return VS_ERROR;
}


// ---------- Select_ImgPanel_ByFrameId ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note ok
*/
INT Model::Select_ImgPanel_ByFrameId(
	vs_system::AppFrame *frame,
	UINT frame_id,
	CHAR prefix
)
{

	CHAR name[5];
	sprintf(name, "%c%2d", prefix, frame_id);
	name[1] = name[1] == ' ' ? '0' : name[1];
	INT result = frame->SelectPanel(name, TRUE);

	return VS_ERROR;
}


// ---------- Get_ImgPanel_ActiveId ----------
/*!
\brief get current active panel id
\author Gareth Edwards
\return UINT - id
\note ok
*/
UINT Model::Get_ImgPanel_ActiveId()
{
	return pi_model->active_panel_id;
}


// ---------- Get_ImgPanel_GroupRow ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note ok
*/
INT Model::Get_ImgPanel_GroupRow(UINT *row)
{
	*row = pi_model->dspl_img_nav_panel_group_row;
	return VS_OK;
}


// ---------- Get_ImgPanel_HandleDisplayList ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note not ok from NO panel context (must be a navigation img rect)

\note display list

Name:Overview;
	Panel{
		Name:I01;
		Display:IRQ01;                  handle_dspl_ir_img_rect_quad[index]
		Display:IRL01;                  handle_dspl_ir_img_rect_line[index]
		Display:IML;                    handle_dspl_ir_img_rect_locate
		Display:IMB;                    handle_dspl_vm_img_button
	}
	$lib:NAV;


Name:Multiview (nine);
	Panel{
		Name:N02;
		Display:NRB02;                  handle_dspl_mv_img_button[index]
		Display:NRQ02;                  handle_dspl_mv_img_rect_quad[index]
		Display:NRL02;                  handle_dspl_mv_img_rect_line[index]
	}
	$lib:NAV;


Name:Solo;
	Panel{
		Name:I11;
		Display:IRQ11;                  handle_dspl_ir_img_rect_quad[index]
		Display:IRL11;                  handle_dspl_ir_img_rect_line[index]
		Display:IRG11;                  handle_dspl_ir_img_rect_view[index]
	}
	$lib:NAV;


Image Review (navigation):

	library{
		NAV{
			Panel{
				Name:I02;
				Display:IRQ02;          handle_dspl_ir_img_rect_quad[index]
				Display:IRL02;          handle_dspl_ir_img_rect_line[index]
				Display:IRG02;          handle_dspl_ir_img_rect_view[index]
				Display:IRB02;          handle_dspl_ir_img_button[index]
			}
		}
		...
	}

*/
INT Model::Get_ImgPanel_HandleDisplayList(
		vs_system::PanelContext *panel_context,
		CHAR *name
	)
{

	// ---- TBD - error checking


	// ---- local
		CHAR prefix[4] = { name[0], name[1], name[2], 0 };
		INT handle_display_list = 0;
		INT index = 0;


	//	---- use first char 'I' or 'N' as key
		switch ( name[0] )
		{

			// 'I'mage
			case 'I':
				{

					// return 'M'ap display list handle for
						//
						//   'L'ine and
						//   'B'uttons
						//
						if ( name[1] == 'M' )
						{
							// 'L'ine || 'B'uttons && return
							if ( name[2] == 'L' )
							{
								return pi_model->handle_dspl_ir_img_rect_locate;
							}
							else if ( name[2] == 'B' )
							{
								return pi_model->handle_dspl_vm_img_button;
							}
						}

					
					// set 'R'ect display list index for...
						//
						//   'Q'uad,
						//   'L'ine,
						//   'G'... view eh...!,
						//   'B'utton and
						//
						if ( name[1] == 'R' )
						{

							index = Get_ImgPanel_IndexDisplayList(name);
							if ( index == VS_ABORT ) return 0;

							index -= 1;
							if ( name[2] == 'Q' )
							{
								handle_display_list = pi_model->handle_dspl_ir_img_rect_quad[index];
							}
							else if ( name[2] == 'L' )
							{
								handle_display_list = pi_model->handle_dspl_ir_img_rect_line[index];
							}
							else if ( name[2] == 'G' )
							{
								handle_display_list = pi_model->handle_dspl_ir_img_rect_view[index];
							}
							else if ( name[2] == 'B' )
							{
								handle_display_list = pi_model->handle_dspl_ir_img_button[index];
							}
							else
							{
								return 0;
							}

					}
					else
					{
						return 0;
					}
				}
				break;

			case 'N':
				{

					// set 'N'avigation display list index for...
						//
						//   'Q'uad,
						//   'L'ine,
						//   'B'utton and
						//
						if ( name[1] == 'R' )
						{
							index = Get_ImgPanel_IndexDisplayList(name);
							if ( index == VS_ABORT ) return 0;

							index -= 1;
							if ( name[2] == 'Q' )
							{
								handle_display_list = pi_model->handle_dspl_mv_img_rect_quad[index];
							}
							else if ( name[2] == 'L' )
							{
								handle_display_list = pi_model->handle_dspl_mv_img_rect_line[index];
							}
							else if ( name[2] == 'B' )
							{
								handle_display_list = pi_model->handle_dspl_mv_img_button[index];
							}
							else
							{
								return 0;
							}

					}
					else
					{
						return 0;
					}

				}
				break;

			default:
				return 0;
		}


	// ---- note: continue from here ONLY with index && handle_display_list both set


	// ---- "live" update/set panel context for navigation img rect 
		//
		// note: could be set in Model::SetupPanels, but "live" update
		//       requires that each original set of img panels be
		//       used in each seperate frame to initialise the
		//       panel_context_img_rect array.
		//       
		// note: ONLY done once at runtime
		//
		if ( index > 0 )
		{

			// ---- NO panel context (must be a navigation img rect)
				if ( pi_model->panel_context_img_rect[index] == NULL )
				{
					pi_model->panel_context_img_rect[index] = panel_context;
				}

			// ---- otherwise ignore
				else
				{
					if ( pi_model->panel_context_img_rect[index] != panel_context)
					{
						;
					}
				}
		}


	return handle_display_list;
}


// ---------- Get_ImgPanel_Id ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note ok

*/
INT Model::Get_ImgPanel_Id(CHAR *name)
{

	// ---- return ptf id from "panel name"
		INT len = strlen(name);
		if (
				len     ==  3  &&
				name[0] == 'I' || name[0] == 'N' || name[0] == 'V' &&
				name[1] >= '0' && name[1] <= '1' &&
				name[2] >= '0' && name[2] <= '9'
			)
		{
			UINT id = ((name[1] - '0') * 10) + (name[2] - '0');
			return id;
		}

	return VS_ERROR;
}


// ---------- Get_ImgPanel_IndexDisplayList ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note ok

*/
INT Model::Get_ImgPanel_IndexDisplayList(CHAR *name)
{

	//
	// ---- return ptf id from "ptf name"
	//      image['I'aa##] or ['N'aa##]

	INT len = strlen(name);
	if (
			len == 5 &&
			name[0] == 'I' || name[0] == 'N' || name[0] == 'V' &&
			name[3] >= '0' && name[3] <= '1' &&
			name[4] >= '0' && name[4] <= '9'
		)
	{
		// BUG - too many selected
		UINT id = ( (name[3] - '0') * 10 ) + ( name[4] - '0' );
		return id;
	}

	return VS_ABORT;
}


// ---------- Set_ImgPanel_ActiveId ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note ok

*/
INT Model::Set_ImgPanel_ActiveId(UINT id)
{

	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)


		// ---- "IMG_PTZ_[...]" ---- OK

		UINT id_first = vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX + 1;
		UINT id_last  = vs_system::ObjConfig::IMG_PTZ_LAST_PANEL_INDEX + 1;

		if ( id >= id_first && id <= id_last )
		{
			pi_model->active_panel_id = id;
		}

	#else

		if ( id >= 2 && id <= 7 )
			pi_model->active_panel_id = id;

	#endif

	return VS_OK;
}


// ---------- Set_ImgPanel_ButtonsId ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note ONLY matches switched on status between the
      image review (navigation) buttons and (nine) multi-view buttons

\note ok, does what it says on the lid

\note see "vs_obj_config.h" for:
	#define VS_OBJ_IR_IMG_BUTTONS
	#define VS_OBJ_MV_IMG_BUTTONS
	#define VS_OBJ_VM_IMG_BUTTONS

*/
INT Model::Set_ImgPanel_ButtonsId(UINT group_id, UINT element_id)
{

	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

	// ---- report

	// ---- id of current image panel over ?
		//UINT panel_id = 0;
		//pi_model->obj_config->GetImagePanelOverId(&panel_id);

	// ---- NEW 220419 report toggle switched on
		//CHAR msg[128];
		//sprintf(msg, "--- Set_ImgPanel_ButtonsId - pid %d, gid %d & id %d\n", panel_id, group_id, element_id);
		//OutputDebugString(msg);

	// ---- set limits
		UINT id_first = vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX + 1;
		UINT id_last  = vs_system::ObjConfig::IMG_PTZ_LAST_PANEL_INDEX + 1;

	// ---- NEW 220419 toggle switched on

	// ---- NEW 220502 toggle switched on

	// ---- shortcut get and set
		INT hr_dspl_ir = pi_model->handle_dspl_ir_img_button[element_id - 1];
		INT hr_dspl_mv = pi_model->handle_dspl_mv_img_button[element_id - 1];
		INT hr_dspl_vm = pi_model->handle_dspl_vm_img_button;

	// ---- anonymous one get & two set
		INT  hr_dspl_get_and_set[3] = { 0, 0, 0 };

	// ---- select
		if (group_id == VS_OBJ_IR_IMG_BUTTONS)
		{
			hr_dspl_get_and_set[0] = hr_dspl_ir;
			hr_dspl_get_and_set[1] = hr_dspl_mv;
			hr_dspl_get_and_set[2] = hr_dspl_vm;
		}
		else if (group_id == VS_OBJ_MV_IMG_BUTTONS)
		{
			hr_dspl_get_and_set[0] = hr_dspl_mv;
			hr_dspl_get_and_set[1] = hr_dspl_ir;
			hr_dspl_get_and_set[2] = hr_dspl_vm;
		}
		else if (group_id == VS_OBJ_VM_IMG_BUTTONS)
		{
			hr_dspl_get_and_set[0] = hr_dspl_vm;
			hr_dspl_get_and_set[1] = hr_dspl_ir;
			hr_dspl_get_and_set[2] = hr_dspl_mv;
		}
		else
		{
			return VS_ERROR;
		}

	// ---- get and set display lists
		//
		//  note: get_active only used if e_id == element_id (see below) 
		//
		BOOL get_active = FALSE;
		for (INT dspl_list = 0; dspl_list < 3; dspl_list++)
		{
			INT hr_dspl = hr_dspl_get_and_set[dspl_list];

			// ---- panels
				for (UINT id = id_first; id <= id_last; id++)
				{
						INT index = id - 1;
						INT h_first = pi_model->gfx->GetFirst(hr_dspl);
						while (h_first)
						{

							// context id
							vs_system::ElmContext *elm_context = 0;
							INT result = gfx->GetElementContext(h_first, &elm_context);
							INT g_id = (INT)elm_context->GetGroupId();
							INT e_id = (INT)elm_context->GetId();

							// match ?
							if ( e_id == element_id )
							{
								if (dspl_list == 0)
								{
									get_active = elm_context->GetSwitchedOn();
									break;
								}
								else
								{
									result = elm_context->SetSwitchedOn(get_active);
								}
							}

							h_first = pi_model->gfx->GetNext(h_first);
						}
				}
		}

	// ---- check
		INT result = Set_ImgPanels_AtLeastOneOn();

	#endif

	return VS_OK;
}


// ---------- Set_ImgPanel_GroupDisplay ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note invoked by Control::UpdateImgPtzPanelsNavButtons

*/
INT Model::Set_ImgPanel_GroupDisplay(BOOL select_first)
{

	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

		// ---- local
			UINT group_row = pi_model->dspl_img_nav_panel_group_row;


		// ---- "IMG_PTZ_[...]" ---- OK


		// ---- for all panels
			UINT id_first = vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX + 1;
			UINT id_last  = vs_system::ObjConfig::IMG_PTZ_LAST_PANEL_INDEX + 1;
			for ( UINT i = id_first; i <= id_last; i++ )
			{

				// ---- panel ok ?
					UINT index = i - 1;
					if ( pi_model->panel_context_img_rect[index] != NULL )
					{

						// ---- get panel status
							UINT this_row =
								( index - vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX ) /
									vs_system::ObjConfig::IMG_PTZ_GROUP_SIZE;

							BOOL status = this_row == group_row ? TRUE : FALSE;

							INT  result = pi_model->panel_context_img_rect[index]->SetDisplayStatus(status);


						// ---- status ok ?
							if ( status )
							{

								if ( select_first )
								{

									// ---- position 0 == first in group
										UINT group_col =
											( index - vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX ) %
												vs_system::ObjConfig::IMG_PTZ_GROUP_SIZE;
								
									// ---- first ?
										if ( group_col == 0 )
										{
											// select rect
											result = pi_model->panel_context_img_rect[index]->SetSelected(TRUE);

											// get rect id
											UINT id = 0;
											result = pi_model->panel_context_img_rect[index]->GetId(&id);

											// update panel id
											result = Update_ImgPanel_Config(id);

										}
										else
										{
											// deselect
											result = pi_model->panel_context_img_rect[index]->SetSelected(FALSE);
										}
								}
								else
								{

									UINT a_id = Get_ImgPanel_ActiveId();
									if ( i == a_id )
									{
										result = pi_model->panel_context_img_rect[index]->SetSelected(TRUE);
										result = Update_ImgPanel_Config(a_id);
									}
									else
									{
										result = pi_model->panel_context_img_rect[index]->SetSelected(FALSE);
									}

								}

							}

					}

			}

	#endif

	return VS_OK;
}


// ---------- Set_ImgPanel_GroupRow ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note ok
*/
INT Model::Set_ImgPanel_GroupRow(UINT row)
{
	pi_model->dspl_img_nav_panel_group_row = row;
	return VS_OK;
}


// ---------- Set_ImgPanel_NavButtons ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note NOT USED ( - does nothing right now!!! )
*/
INT Model::Set_ImgPanel_NavButtons(UINT g_row)
{

	#if defined(VS_CAMERA_05_G) || defined(VS_REVIEW_04_G)


	// ---- "IMG_PTZ_[...]" ---- OK


	// ---- lut
		BOOL status[2] = { TRUE, TRUE };

	// ---- for each layer, button and element
		vs_system::ElmContext *elm_context = 0;
		UINT index = 0;
		for (UINT b = 0; b < 2; b++)
		{
			for (UINT i = 0; i < 3; i++)
			{

				// ---- update behaviour
					pi_model->gfx->GetElementContext(
							pi_model->handle_elem_img_nav_but[index],
							&elm_context
						);

					elm_context->SetBehaviour(
							status[b] ?
							vs_system::ElmContext::BEHAVIOUR_CLICK :
							vs_system::ElmContext::BEHAVIOUR_NONE
						);

					elm_context->SetSwitchedOn(status[b] ? TRUE : FALSE);

					elm_context->SetMultiActive(status[b] ? TRUE : FALSE, 0);

				// ---- increment
					index++;
			}
		}

	#endif

	return VS_OK;
}


// ---------- Update_ImgPanel_GroupRow ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note ok
*/
INT Model::Update_ImgPanel_GroupRow()
{

	INT a_id   = Get_ImgPanel_ActiveId();
	INT g_idx  = a_id - (vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX + 1);
	INT g_row  = g_idx / vs_system::ObjConfig::IMG_PTZ_GROUP_SIZE;

	INT result = Set_ImgPanel_GroupRow((UINT)g_row);
	BOOL select_first_panel = FALSE;

	result = Set_ImgPanel_GroupDisplay(select_first_panel);
	result = Set_ImgPanel_NavButtons(g_row); // ( - does nothing right now!!! )

	return VS_ERROR;
}


// ---------- Update_ImgPanel_OverviewMap ----------
/*!
\brief Update over nav in the multiview and overview
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value

\note so that a panel can echo the over mode of another

\note seems ok

*/
INT Model::Update_ImgPanel_OverviewMap()
{

	// ----- img only ?
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
		;
		#else
		return VS_OK;
		#endif


	// ---- local
		INT result = 0;
		//#define XREPORT_OVER_STUFF
		#if defined(XREPORT_OVER_STUFF) || defined(REPORT_OVER_STUFF) 
		CHAR msg[32];
		#endif


	// ---- gfx short cut
		vs_system::GfxInterface *gfx_interface =
			pi_model->app_dependency->GetGfxInterface();


	// --- object config
			vs_system::ObjConfig *obj_config = GetObjConfig();
			UINT over_id = 0;
			result = obj_config->GetImagePanelOverId(&over_id);
			result = obj_config->SetImagePanelOverId(0);


	// ---- gfx context ok ?
		vs_system::PanelContext *panel_context = NULL;
		result = gfx_interface->GetContextOverPanel(&panel_context);
		if ( panel_context )
		{

			// ---- panel id ok ?
				CHAR name[VS_MAXCHARNAME];
				result = panel_context->GetName(name);
				INT id = Get_ImgPanel_Id(name);
				if ( id != VS_ERROR )
				{

					// ---- over overview or ptf panels
						INT id_overview = vs_system::ObjConfig::IMG_PTZ_OVERVIEW_PANEL_INDEX + 1;
						INT id_first    = vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX    + 1;
						INT id_last     = vs_system::ObjConfig::IMG_PTZ_LAST_PANEL_INDEX     + 1;
						INT id_solo     = vs_system::ObjConfig::IMG_PTZ_SOLO_PANEL_INDEX     + 1;
						INT id_settings = vs_system::ObjConfig::IMG_PTZ_SETTINGS_PANEL_INDEX + 1;


					// ---- echo over
						UINT echo_over_id = 0;
						CHAR echo_over_mode = ' ';


					// ---- process
						switch ( name[0] )
						{

								case 'I':
								{

									// ---- img panels
										switch (id)
										{

											case vs_system::ObjConfig::IMG_PTZ_OVERVIEW_PANEL_INDEX + 1:
												#if defined(REPORT_OVER_STUFF)
												sprintf(msg, "--- over - %s with id %d\n", name, id); 
												#endif
												break;

											case vs_system::ObjConfig::IMG_PTZ_SOLO_PANEL_INDEX + 1:

												#if defined(REPORT_OVER_STUFF)
												sprintf(msg, "--- solo - %s with id %d (%d)\n", name, id, over_id);
												#endif

												echo_over_id = over_id;
												echo_over_mode = 'I';
												break;

											case vs_system::ObjConfig::IMG_PTZ_SETTINGS_PANEL_INDEX + 1:
												#if defined(REPORT_OVER_STUFF)
												sprintf(msg, "--- settings - %s with id %d\n", name, id);
												#endif
												break;

											default:
												if ( id >= id_overview && id <= id_last )
												{
													#if defined(REPORT_OVER_STUFF)
													sprintf(msg, "--- img - %s with id %d\n", name, id);
													#endif

													// set over
													obj_config->SetImagePanelOverId(id);

													// light up overview vmap
													for (INT i = id_first; i <= id_last; i++)
													{
														INT hr = GetHandleElemImgRect("ir", "vmap", i);
														result = gfx->SetElementConfigureBitmask(
															hr,
															(BYTE)(VS_ELM_SETUP_GEOMETRY |
																VS_ELM_SETUP_MAPPING)
														);
													}

													// bold
													echo_over_id = id;
													echo_over_mode = 'N';
												}
												break;
										}
									
									#if defined(REPORT_OVER_STUFF)
									OutputDebugString(msg);
									#endif	
								}
								break;

								case 'N':
									{
										#if defined(REPORT_OVER_STUFF)
										CHAR msg[32];
										sprintf(msg, "--- nono - %s with id %d\n", name, id);
										OutputDebugString(msg);
										#endif

										echo_over_id = id;
										echo_over_mode = 'I';
									}
									break;

								default:
									break;
						}

						

					// ---- echo over ?
						if (
								echo_over_id > 0 &&
									( echo_over_mode == 'I' || echo_over_mode == 'N' )
							)
						{

							CHAR echo_panel_name[32];
							sprintf(echo_panel_name, "%c%2d", echo_over_mode, echo_over_id);
							echo_panel_name[1] = echo_panel_name[1] == ' ' ? '0' : echo_panel_name[1];

							#if defined(REPORT_OVER_STUFF)
							sprintf(msg, "--- ECHO %s\n", echo_panel_name);
							OutputDebugString(msg);
							#endif

							vs_system::PanelContext *panel_context = NULL;
							switch ( echo_over_mode )
							{

								case 'I':
									panel_context = pi_model->panel_context_img_rect[echo_over_id-1];
									break;

								case 'N':
									panel_context = pi_model->panel_context_ptf[echo_over_id - 1];
									break;

								default:
									break;
							}


							// ---- so that a panel can echo the over mode of another
								//
								// note: gfx_interface resets echo over to FALSE
								//       when ViewRect methods are invoked... so
								//       does not be reset by application
								//
								if (panel_context != NULL)
								{
									panel_context->SetEchoOver(TRUE);
								}

								#if defined(REPORT_OVER_STUFF)
								result = panel_context->GetName(echo_panel_name);
								sprintf(msg, "---      %s\n", echo_panel_name);
								OutputDebugString(msg);
								#endif

						}
				}

		}


		// ---- reconfig...
			{

				// ---- oveview map
					UINT id_first = vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX + 1;
					UINT id_last = vs_system::ObjConfig::IMG_PTZ_LAST_PANEL_INDEX + 1;
					for (UINT i = id_first; i <= id_last; i++)
					{
						INT hr = GetHandleElemImgRect("ir", "vmap", i);
						result = gfx->SetElementConfigureBitmask(
								hr,
								(BYTE)(VS_ELM_SETUP_GEOMETRY |
									VS_ELM_SETUP_MAPPING)
							);
					}
			}
	
	return VS_ERROR;
}


// ---------- Update_ImgPanel_Config ----------
/*!
\brief Update ptf panel element config
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Model::Update_ImgPanel_Config(UINT id)
{

	// ---- local
		vs_system::GfxInterface *gfx = pi_model->app_dependency->GetGfxInterface();
		INT result = VS_OK;


	// ---- update "active" id
		vs_system::ObjConfig *obj_config = GetObjConfig();
		obj_config->SetImagePanelActiveId(id);


	// ---- reconfig "active" overview navigation and map view
		INT eh[5] = {
			GetHandleElemImgRect("ir", "quad", id),
			GetHandleElemImgRect("ir", "line", id),
			GetHandleElemImgRect("ir", "view", id),
			GetHandleElemImgRect("mv", "quad", id),
			GetHandleElemImgRect("mv", "line", id)
		};
		for (UINT h = 0; h < 5; h++)
		{
			result = gfx->SetElementConfigureBitmask(
				eh[h],
				(BYTE)(VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING)
			);
		}


	// ---- reconfig "solo" OR echo in navigation
		//
		// note: bit of a rats nest :-(
		//
		UINT reconfig_id = 0;
		UINT solo_id = vs_system::ObjConfig::IMG_PTZ_SOLO_PANEL_INDEX + 1;

		if ( id == solo_id )
		{
			// set previous navigation img without recalculating limits...
			result = obj_config->GetImagePanelPreviousId(&reconfig_id);
			FLOAT pan, tilt, fov;
			result = obj_config->GetPanoPTFById(solo_id, &pan, &tilt, &fov);
			result = obj_config->SetPanoPTFByIndex(reconfig_id - 1, pan, tilt, fov);
		}
		else
		{
			// force set previous id...  
			result = obj_config->SetImagePanelPreviousId(id);
			result = obj_config->CpyImagePanelActivePtfbyId(solo_id);
			reconfig_id = solo_id;
		}

		if ( reconfig_id != 0 )
		{

			INT ej[3] = {
				GetHandleElemImgRect("ir", "quad", reconfig_id),
				GetHandleElemImgRect("ir", "line", reconfig_id),
				GetHandleElemImgRect("ir", "view", reconfig_id)
			};
			for (UINT h = 0; h < 3; h++)
			{
				result = gfx->SetElementConfigureBitmask(
						ej[h],
						(BYTE)(VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING)
					);
			}

		}


	// ---- "IMG_PTZ_[...]" ---- OK


	// ---- reconfig "all" navigation view centre
		//
		// ok
		//
		UINT id_first = vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX + 1;
		UINT id_settings = vs_system::ObjConfig::IMG_PTZ_SETTINGS_PANEL_INDEX + 1;
		for ( UINT i = id_first; i <= id_settings; i++ )
		{
			INT hr = GetHandleElemImgRect("ir", "view", i);
			result = gfx->SetElementConfigureBitmask(
				hr,
				(BYTE)(VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING)
			);
		}


	// ---- update and reconfig a nav "row" in the overview map
		//
		// ok
		//
		UINT g_row = 0;
		result = Get_ImgPanel_GroupRow(&g_row);
		UINT id_last = vs_system::ObjConfig::IMG_PTZ_LAST_PANEL_INDEX + 1;
		for ( UINT i= id_first; i<=id_last; i++ )
		{

			// ---- row
				INT i_idx = i - (vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX + 1);
				INT i_row = i_idx / vs_system::ObjConfig::IMG_PTZ_GROUP_SIZE;


			// ---- overview vmap context
				INT hr = GetHandleElemImgRect("ir", "vmap", i);
				vs_system::ElmContext *elm_context = NULL;
				result = gfx->GetElementContext(hr, &elm_context);


			// ---- matched row ? -- then display:
				if ( i_row == g_row )
				{

					// ---- overview map
						elm_context->SetDisplay(TRUE);
						result = gfx->SetElementConfigureBitmask(
								hr,
								(BYTE)(VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING)
							);

					// ---- button number
						LONG *long_param = elm_context->GetLongParameter();
						for (UINT b = 0; b < 3; b++)
						{
							LONG long_val = *(long_param + b);
							if (long_val > 0)
							{
								vs_system::ElmContext *ec = (vs_system::ElmContext *)long_val;
								ec->SetDisplay(TRUE);
								CHAR idx[2] = "0";
								idx[0] = '0' + b;
								INT hr = GetHandleElemImgRect("vm", idx, i);
								if ( hr > 0 )
								{
									result = gfx->SetElementConfigureBitmask(
											hr,
											(BYTE)(VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING)
										);
								}
							}
						}

				}

			// ---- otherwise, don't display:
				else
				{

					// ---- overview map
						elm_context->SetDisplay(FALSE);

					// ---- button number
						LONG *long_param = elm_context->GetLongParameter();
						for (UINT i = 0; i < 3; i++)
						{
							LONG long_val = *(long_param + i);
							if (long_val > 0)
							{
								vs_system::ElmContext *ec = (vs_system::ElmContext *)long_val;
								ec->SetDisplay(FALSE);
							}
						}
				}

		}


	// ---- "stream" - copy active id to stream hidden and visible
		//
		// ok
		//
		UINT id_hid = vs_system::ObjConfig::IMG_PTZ_HIDDEN_PANEL_INDEX + 1;
		UINT id_vis = vs_system::ObjConfig::IMG_PTZ_VISIBLE_PANEL_INDEX + 1;
		for ( UINT i=id_hid; i<= id_vis; i++ )
		{
			obj_config->CpyImagePanelActivePtfbyId(i);
		}


	// ---- "stream" - reconfig hidden and visible
		//
		// ok
		//
		for (UINT i = 0; i < 4; i++)
		{
			result = gfx->SetElementConfigureBitmask(
					pi_model->handle_elem_stream_hidden[i],
					(BYTE)(VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING)
				);
			result = gfx->SetElementConfigureBitmask(
					pi_model->handle_elem_stream_visible[i],
					(BYTE)(VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING)
				);
		}

	return VS_OK;
}


// ---------- Get_ImgPanels_SwitchedOn ----------
/*!
\brief Get ptf panels switched On
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < num of switched on panels
*/
INT Model::Get_ImgPanels_SwitchedOn(BOOL switched_on[], UINT size)
{

	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

	UINT id_first = vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX + 1;
	UINT id_last = vs_system::ObjConfig::IMG_PTZ_LAST_PANEL_INDEX + 1;

	for (UINT i = 0; i < size; i++)
	{
		INT element_id = i + 2;

		INT hr_dspl_ir = pi_model->handle_dspl_ir_img_button[element_id - 1];

		for (UINT id = id_first; id <= id_last; id++)
		{

			INT h_first = pi_model->gfx->GetFirst(hr_dspl_ir);
			while (h_first)
			{

				// context id
				vs_system::ElmContext *elm_context = 0;
				INT result = gfx->GetElementContext(h_first, &elm_context);
				INT g_id = (INT)elm_context->GetGroupId();
				INT e_id = (INT)elm_context->GetId();

				// match ?
				if (e_id == element_id)
				{
					switched_on[i] = elm_context->GetSwitchedOn();
				}

				h_first = pi_model->gfx->GetNext(h_first);
			}
		}
	}

	INT num_switched_on = 0;
	for (UINT i = 0; i < size; i++)
		num_switched_on += switched_on[i] ? 1 : 0;

	return num_switched_on;

	#else

	return VS_OK;

	#endif

}

// ---------- Set_ImgPanels_SwitchedOn ----------
/*!
\brief Set ptf panels switched On
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Model::Set_ImgPanels_SwitchedOn(BOOL switched_on)
{

	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

	// ---- NEW 220422 toggle switched on

		UINT ptf_id;
		INT  result = pi_model->obj_config->GetPanoPTFId(&ptf_id);
		INT  ptf_active_index = ptf_id - 1;

	// ---- for each panel
		for (INT panel_index = 1; panel_index <=9; panel_index++)
		{
			// ---- store button display lists
				INT  handle_dspl_lists[2] =
				{
					pi_model->handle_dspl_ir_img_button[panel_index],
					pi_model->handle_dspl_mv_img_button[panel_index]
				};

			// ---- for each of the button display lists 
				for (INT button_dspl_list = 0; button_dspl_list < 2; button_dspl_list++)
				{
					// ---- for all the button elements
						INT hr_dspl = handle_dspl_lists[button_dspl_list];
						INT h_first = pi_model->gfx->GetFirst(hr_dspl);
						while (h_first)
						{
							vs_system::ElmContext *elm_context = 0;
							INT result = gfx->GetElementContext(h_first, &elm_context);
							INT g_id = (INT)elm_context->GetGroupId();
							INT e_id = (INT)elm_context->GetId();
							if (ptf_active_index == panel_index)
							{
								result = elm_context->SetSwitchedOn(TRUE);
							}
							else
							{
								result = elm_context->SetSwitchedOn(switched_on);
							}
							h_first = pi_model->gfx->GetNext(h_first);
						}
				}

		}

	// ---- for all the button elements
		INT hr_dspl = pi_model->handle_dspl_vm_img_button;
		INT count = 0;
		INT num_switched_on = 0;
		INT h_first = pi_model->gfx->GetFirst(hr_dspl);
		while (h_first)
		{
			vs_system::ElmContext *elm_context = 0;
			INT result = gfx->GetElementContext(h_first, &elm_context);
			INT g_id = (INT)elm_context->GetGroupId();
			INT e_id = (INT)elm_context->GetId();
			++count;
			if ( (count-1)/3 == ptf_active_index - 1)
			{
				++num_switched_on;
				result = elm_context->SetSwitchedOn(TRUE);
			}
			else
			{
				if (switched_on) ++num_switched_on;
				result = elm_context->SetSwitchedOn(switched_on);
			}
			h_first = pi_model->gfx->GetNext(h_first);
		}

	// ---- check
		result = Set_ImgPanels_AtLeastOneOn();

	#endif

	return VS_OK;
}


// ---------- Set_ImgPanels_AtLeastOneOn ----------
/*!
\brief Set at least one ptf panel switched On
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value

\note invoked by Set_ImgPanel _ButtonsId/_SwitchedOn 

*/
INT Model::Set_ImgPanels_AtLeastOneOn()
{

	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

	// ----
		INT num_switched_on = 0;
		INT h_first = pi_model->gfx->GetFirst(pi_model->handle_dspl_vm_img_button);
		while (h_first)
		{
			vs_system::ElmContext *elm_context = 0;
			INT result = gfx->GetElementContext(h_first, &elm_context);
			if (elm_context->GetSwitchedOn()) ++num_switched_on;
			h_first = pi_model->gfx->GetNext(h_first);
		}

		//CHAR msg[128];
		//sprintf(msg, " -> : num_switched_on %d\n", num_switched_on);
		//OutputDebugString(msg);

	// ---- finish if 1 or more switched ON 
		if (num_switched_on != 0) return VS_OK;

	// ---- force IR & MV button #1 ON
		{
			INT  handle_dspl_lists[2] =
			{
				pi_model->handle_dspl_ir_img_button[1],
				pi_model->handle_dspl_mv_img_button[1]
			};
			for (INT button_dspl_list = 0; button_dspl_list < 2; button_dspl_list++)
			{
				INT count = 0;
				INT hr_dspl = handle_dspl_lists[button_dspl_list];
				INT h_first = pi_model->gfx->GetFirst(hr_dspl);
				while (h_first)
				{
					vs_system::ElmContext *elm_context = 0;
					INT result = gfx->GetElementContext(h_first, &elm_context);
					result = elm_context->SetSwitchedOn(TRUE);
					if (++count >= 3) break;
					h_first = pi_model->gfx->GetNext(h_first);
				}
			}
		}

	// ---- force VM button #1 ON
		{
			INT hr_dspl = pi_model->handle_dspl_vm_img_button;
			h_first = pi_model->gfx->GetFirst(hr_dspl);
			INT count = 0;
			while (h_first)
			{
				vs_system::ElmContext *elm_context = 0;
				INT result = gfx->GetElementContext(h_first, &elm_context);
				result = elm_context->SetSwitchedOn(TRUE);
				if (++count >= 3) break;
				h_first = pi_model->gfx->GetNext(h_first);
			}
		}
	#endif

	return VS_OK;
}
