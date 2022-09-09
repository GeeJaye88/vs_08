////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tool_callback_imgptz.hpp ----------
/*!
\file r2_tool_callback_imgptz.hpp
\brief
\author Gareth Edwards
*/


using namespace review_02;


// -------- Functionality and Parameters for Tabbed Interface :Tint_ImgPtz_[...]

class Model::Tint_ImgPtz_Param
{

public:

	// ---- cdtor
		Tint_ImgPtz_Param::Tint_ImgPtz_Param() { ; };
		Tint_ImgPtz_Param::~Tint_ImgPtz_Param() { ; };

	// --- props
		BOOL _large_incr = TRUE;

	// --- 
		enum
		{
			NUM_AUTO_ITEMS = 10,

			ZOOM_ID = 9,

			AUTO_ID         = ZOOM_ID + 3,
			AUTO_PAUSE_ID   = AUTO_ID + 1,
			AUTO_SLIDE_ID   = AUTO_ID + 2,
			AUTO_SOSI_ID    = AUTO_ID + 3,
			AUTO_DIRECT_ID  = AUTO_ID + 4,

			AUTO_SET_ID     = AUTO_ID + 5,
			AUTO_SPEED_ID   = AUTO_ID + 6,
			AUTO_ON_ID      = AUTO_ID + 7,
			AUTO_OFF_ID     = AUTO_ID + 8,
			AUTO_SHUFFLE_ID = AUTO_ID + 9,

			SAVE_ID  = AUTO_ID + NUM_AUTO_ITEMS,
			RESET_ID = SAVE_ID + 1,
			INCR_ID  = SAVE_ID + 2,
		};

} tint_imgptz_param;


// ----------------------------------------------------------------------
INT Model::Tint_ImgPtz_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_ImgPtz_BuildList;
	tint_callback->callback_get_icon_info  = Tint_ImgPtz_GetIconInfo;
	tint_callback->callback_update_model   = Tint_ImgPtz_UpdateModel;
	tint_callback->callback_update_element = Tint_ImgPtz_UpdateElement;
	tint_callback->callback_add_actions    = Tint_ImgPtz_AddActions;
	tint_callback->callback_add_text       = Tint_ImgPtz_AddText;
	tint_callback->callback_add_value      = Tint_ImgPtz_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_ImgPtz_BuildList(
		vs_system::TintItem li[]
	)
{

	// ---- describe
		INT number_of_items = tint_imgptz_param.INCR_ID + 1;
		UINT gid = TINT_TAB_IMGPTZ_LID;
		INT i = -1;

		// ---- row 1
			li[++i].Set("Pan",  gid,  0);
			li[++i].Set("",     gid,  1);
			li[++i].Set("",     gid,  2);

		// ---- row 2
			li[++i].Set("",     gid,  3);
			li[++i].Set("",     gid,  4);
			li[++i].Set("",     gid,  5);

		// ---- row 3
			li[++i].Set("",     gid,  6);
			li[++i].Set("",     gid,  7);
			li[++i].Set("",     gid,  8);

		// ---- row 4
			li[++i].Set("Zoom", gid, 9);
			li[++i].Set("",     gid, 10);
			li[++i].Set("",     gid, 11);

		// ---- row 5
			li[++i].Set("Auto",      gid, tint_imgptz_param.AUTO_ID);
			li[++i].Set("Paus",      gid, tint_imgptz_param.AUTO_PAUSE_ID);
			li[++i].Set("Slid",      gid, tint_imgptz_param.AUTO_SLIDE_ID);
			li[++i].Set("Anim",      gid, tint_imgptz_param.AUTO_SOSI_ID);
			li[++i].Set("Drct",      gid, tint_imgptz_param.AUTO_DIRECT_ID);

		// ---- row 6
			li[++i].Set("Set",       gid, tint_imgptz_param.AUTO_SET_ID);
			li[++i].Set("Sped",      gid, tint_imgptz_param.AUTO_SPEED_ID);
			li[++i].Set("On",        gid, tint_imgptz_param.AUTO_ON_ID);
			li[++i].Set("Off",       gid, tint_imgptz_param.AUTO_OFF_ID);
			li[++i].Set("Shuf",      gid, tint_imgptz_param.AUTO_SHUFFLE_ID);

		// ---- row 7
			li[++i].Set("Save",      gid, tint_imgptz_param.SAVE_ID);
			li[++i].Set("Reset",     gid, tint_imgptz_param.RESET_ID);
			li[++i].Set("Increment", gid, tint_imgptz_param.INCR_ID);


	// --- detail
			i = -1;

		// ---- row 1
			li[++i].SetAtlas(3, 2, 0); 
			li[++i].SetAtlas(3, 2, 1);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(3, 2, 2);   li[i].SetXOff(2.2f);   li[i].SetLF(TRUE);

		// ---- row 2
			li[++i].SetAtlas(3, 3, 0);
			li[++i].SetAtlas(3, 3, 1);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(3, 3, 2);   li[i].SetXOff(2.2f);   li[i].SetLF(TRUE);

		// ---- row 3
			li[++i].SetAtlas(3, 4, 0);
			li[++i].SetAtlas(3, 4, 1);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(3, 4, 2);   li[i].SetXOff(2.2f);   li[i].SetLF(TRUE);   li[i].SetLFI(8);

		// ---- row 4 (zoom)
			li[++i].SetAtlas(3, 5, 0);
			li[++i].SetAtlas(3, 5, 1);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(3, 5, 2);   li[i].SetXOff(2.2f);   li[i].SetLF(TRUE);   li[i].SetLFI(12);

		// ---- row 5 (auto)
			li[++i].SetAtlas(4, 1, 0);
			li[++i].SetAtlas(4, 1, 1);   li[i].SetXOff(1.25f);
			li[++i].SetAtlas(4, 1, 2);   li[i].SetXOff(2.50f);
			li[++i].SetAtlas(4, 1, 3);   li[i].SetXOff(3.75f);
			li[++i].SetAtlas(4, 1, 4);   li[i].SetXOff(5.00f);   li[i].SetLF(TRUE);   li[i].SetLFI(8);

		// ---- row 6 (auto)
			li[++i].SetAtlas(4, 2, 0);
			li[++i].SetAtlas(4, 2, 1);   li[i].SetXOff(1.25f);
			li[++i].SetAtlas(4, 2, 2);   li[i].SetXOff(2.50f);
			li[++i].SetAtlas(4, 2, 3);   li[i].SetXOff(3.75f);
			li[++i].SetAtlas(4, 2, 4);   li[i].SetXOff(5.00f);   li[i].SetLF(TRUE);   li[i].SetLFI(8);

		// ---- row 7 (housekeeping)
			li[++i].SetAtlas(3, 3, 3);
			li[++i].SetAtlas(3, 3, 4);   li[i].SetXOff(1.5f);
			li[++i].SetAtlas(3, 4, 3);   li[i].SetXOff(3.0f);   li[i].SetLF(TRUE);   li[i].SetLFI(4);


		// ---- ptz pad line feed increment
			for (INT i = 0; i<8; i++) li[i].SetLFI(1);

	// ---- type

		// ---- set all type to CLICK
			for ( INT i = 0; i<number_of_items; i++) li[i].SetType(vs_system::Tint::ITEM_CLICK);

			// ---- set auto (row 5) to SWITCH
				li[tint_imgptz_param.AUTO_ID       ].SetType(vs_system::Tint::ITEM_SWITCH);
				li[tint_imgptz_param.AUTO_PAUSE_ID ].SetType(vs_system::Tint::ITEM_SWITCH);
				li[tint_imgptz_param.AUTO_SLIDE_ID ].SetType(vs_system::Tint::ITEM_SWITCH);
				li[tint_imgptz_param.AUTO_SOSI_ID  ].SetType(vs_system::Tint::ITEM_SWITCH);
				li[tint_imgptz_param.AUTO_DIRECT_ID].SetType(vs_system::Tint::ITEM_SWITCH);

			// ---- set some of auto (row 6) to SWITCH
				li[tint_imgptz_param.AUTO_SPEED_ID].SetType(vs_system::Tint::ITEM_SWITCH);
				li[tint_imgptz_param.AUTO_SHUFFLE_ID].SetType(vs_system::Tint::ITEM_SWITCH);

			// ---- set houskeeping INCREMENT to SWITCH
				li[tint_imgptz_param.INCR_ID].SetType(vs_system::Tint::ITEM_SWITCH);

	return number_of_items;
}


// ----------------------------------------------------------------------
INT Model::Tint_ImgPtz_GetIconInfo(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 3;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 0;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
		case vs_system::Tint::ICON_ATLAS_PAGE_ID : return 3;
		default:
			break;
	}
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_ImgPtz_UpdateModel(
		INT  handle_model,
		INT  element_id
	)
{
 
	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;

	// ---- get ptf
		UINT  ptf_id = 0;
		FLOAT pan, tlt, fov;
		FLOAT aspect;
		vs_system::ObjConfig *obj_config = model->GetObjConfig();
		result = obj_config->GetPanoPTFId(&ptf_id);
		result = obj_config->GetPanoPTFById(ptf_id, &pan, &tlt, &fov);
		result = obj_config->GetPanoAspectRatio(&aspect);

	// ---- state
		FLOAT inc = tint_imgptz_param._large_incr ? 0.1f : 0.01f;
		FLOAT min = -0.5f;
		FLOAT max = 0.5f;

	// --- pz
		switch ( element_id )
		{

			case 0:
				pan = pan - inc < min ? min : pan - inc;
				tlt = tlt + inc > max ? max : tlt + inc;
				break;
			case 1:
				tlt = tlt + inc > max ? max : tlt + inc;
				break;
			case 2:
				pan = pan + inc > max ? max : pan + inc;
				tlt = tlt + inc > max ? max : tlt + inc;
				break;
			case 3:
				pan = pan - inc < min ? min : pan - inc;
				break;
			case 4:
				obj_config->SetPanoDefault(TRUE);
				break;
			case 5:
				pan = pan + inc > max ? max : pan + inc;
				break;
			case 6:
				pan = pan - inc < min ? min : pan - inc;
				tlt = tlt - inc < min ? min : tlt - inc;
				break;
			case 7:
				tlt = tlt - inc < min ? min : tlt - inc;
				break;
			case 8:
				pan = pan + inc > max ? max : pan + inc;
				tlt = tlt - inc < min ? min : tlt - inc;
				break;

			case tint_imgptz_param.ZOOM_ID:
			case tint_imgptz_param.ZOOM_ID+2:
				{
					FLOAT zinc = inc * 20.0f;
					fov = element_id == tint_imgptz_param.ZOOM_ID ? fov + zinc : fov - zinc;
					fov = fov < 1 ? 1 : fov;
				}
				break;

			case tint_imgptz_param.ZOOM_ID+1:
				{
					obj_config->SetPanoDefault(FALSE);
				}
				break;

		// ---- switches ----------------------------------------------------------------

			case tint_imgptz_param.AUTO_DIRECT_ID:
			case tint_imgptz_param.AUTO_ON_ID:
			case tint_imgptz_param.AUTO_OFF_ID:
			case tint_imgptz_param.AUTO_SHUFFLE_ID:
				{

					switch (element_id)
					{
						case tint_imgptz_param.AUTO_DIRECT_ID:
							{
								obj_config->SetAutoPTFForward(obj_config->IsAutoPTFForward() ? FALSE : TRUE);
							}
							break;
						case tint_imgptz_param.AUTO_ON_ID:
							{
								model->Set_ImgPanels_SwitchedOn(TRUE);
							}
							break;
						case tint_imgptz_param.AUTO_OFF_ID:
							{
								model->Set_ImgPanels_SwitchedOn(FALSE);
							}
							break;
						case tint_imgptz_param.AUTO_SHUFFLE_ID:
							obj_config->SetAutoPTFShuffle(obj_config->IsAutoPTFShuffle() ? FALSE : TRUE);
							break;
						default:
							break;
					}

					if (obj_config->IsAutoPTFRunning())
					{
						// stop, ready to restart in run on below...
						obj_config->AutoPTFStop();
						obj_config->SetAutoPTFRunning(FALSE);
					}
					else
					{
						/* we are completely done, so... */ break;
					}
				}

			// ---- run on... ----------------------------------------------------------------

			case tint_imgptz_param.AUTO_ID:
				{
					if (!obj_config->IsAutoPTFRunning())
					{
						BOOL switched_on[9];
						INT num_switched_on = model->Get_ImgPanels_SwitchedOn(switched_on, 9);

						// should never happen...!
						if (num_switched_on <= 0)
						{
							::MessageBox(
								0,
								"Error : Can't Auto! No Image Panels Switched ON!",
								0, 0
								);
							// TBD : need to switch off auto
							obj_config->AutoPTFStop();
						}
						else
						{
							obj_config->AutoPTFSwitchedOn(switched_on);
							obj_config->AutoPTFStart();
							//OutputDebugString("+++ AutoPTF START\n");
						}
					}
					else
					{
						obj_config->AutoPTFStop();
						//OutputDebugString("+++ AutoPTF STOP\n");
					}
				}
				break;

			case tint_imgptz_param.AUTO_PAUSE_ID:
				{
					obj_config->SetAutoPTFPause(obj_config->IsAutoPTFPause() ? FALSE : TRUE);
				}
				break;

			case tint_imgptz_param.AUTO_SLIDE_ID:
				{
					obj_config->SetAutoPTFSlide(obj_config->IsAutoPTFSlide() ? FALSE : TRUE);
				}
				break;

			case tint_imgptz_param.AUTO_SOSI_ID:
				{
					obj_config->SetAutoPTFSosi(obj_config->IsAutoPTFSosi() ? FALSE : TRUE);
				}
				break;

			// ----------------------------------------------------------------

			case tint_imgptz_param.AUTO_SET_ID:
				{
					result = obj_config->AutoPTFSet();
				}
				break;

			case tint_imgptz_param.AUTO_SPEED_ID:
				{
					obj_config->SetAutoPTFSlow(obj_config->IsAutoPTFSlow() ? FALSE : TRUE);
				}
				break;

			// ----------------------------------------------------------------

			case tint_imgptz_param.SAVE_ID:
				{
					result = obj_config->WritePanoConfig();
				}
				break;

			case tint_imgptz_param.RESET_ID:
				{
					for (UINT i = 0; i < vs_system::ObjConfig::MAX_PANO_PTF_LUT; i++)
					{
						obj_config->SetPanoDefault(i, TRUE);
						obj_config->SetPanoDefault(i, FALSE);
					}
				}
				break;

			case tint_imgptz_param.INCR_ID:
				tint_imgptz_param._large_incr = tint_imgptz_param._large_incr ? FALSE : TRUE;
				break;

			default:
				CHAR msg[VS_MAXCHARNAME];
				sprintf(msg, ">>> Select %d\n", element_id);
				OutputDebugString(msg);
				break;

		}

	// ---- update ptf
		switch (element_id)
		{
			case tint_imgptz_param.AUTO_ID:
			case tint_imgptz_param.AUTO_SOSI_ID:
			case tint_imgptz_param.AUTO_SPEED_ID:
				break;

			case 4:  // default
			case tint_imgptz_param.ZOOM_ID + 1: // default (all)
				break;

			case tint_imgptz_param.SAVE_ID: // save
			case tint_imgptz_param.RESET_ID: // reset
			case tint_imgptz_param.INCR_ID: // incr
				break;

			default:
				obj_config->SetPanoPTFById(ptf_id, pan, tlt, fov);
				break;
		}

	// ---- update panel
		result = model->Update_ImgPanel_Config(ptf_id);

	return VS_ELM_SETUP_NONE;

}

// ----------------------------------------------------------------------
/*

We get here from:

	INT application::Display()

		-> INT Control::UpdateInput()

			-> INT AppControl::UpdateInput()

				-> INT Control::UpdatePanel(
						INT handle_control_obj,
						INT handle_panel_obj
					)

					-> INT Control::SetPanelActive(
							Control *control,
							vs_system::AppDependency *app_dependency,
							vs_system::AppPanel      *app_panel,
							vs_system::PanelContext  *panel_context
						)
					OR

					-> INT Control::SetMouseWheelFov(
							Control *control,
							vs_system::AppDependency *app_dependency,
							vs_system::PanelContext *panel_context
						)
					THEN

						-> INT Control::UpdateTintImgText(
								Control* control
							)

							-> INT Control::UpdateTintImgBody(  // note: callbacks
									Control *control,
									INT group_id,
									INT element_handle
								)

							THEN

								-> INT  Model::Tint_ImgPtz_UpdateModel ... see method above

								OR

								-> INT  Model::Tint_ImgPtz_UpdateElement ... se method below

								THEN

									-> TINT

										-> INT  Model::Tint_ImgPtz_AddText(

											.... where PTF values text is updated

											Simples!

*/
INT  Model::Tint_ImgPtz_UpdateElement(
		INT handle_model,
		INT element_id
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_img_setng;

	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_TAB_IMGPTZ_LID);

	// ---- select elem
		switch ( element_id )
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				result = tint->UpdateText(NULL, 0);

			case tint_imgptz_param.ZOOM_ID:
			case tint_imgptz_param.ZOOM_ID + 1:
			case tint_imgptz_param.ZOOM_ID + 2:
				result = tint->UpdateText(NULL, 9);

				result = tint->UpdateText(NULL, tint_imgptz_param.AUTO_SET_ID);

				break;

			case tint_imgptz_param.AUTO_ID:
			case tint_imgptz_param.AUTO_SET_ID:
				result = tint->UpdateText(NULL, tint_imgptz_param.AUTO_SET_ID);
				break;

			case tint_imgptz_param.RESET_ID:
				result = tint->UpdateText(NULL, 0);
				result = tint->UpdateText(NULL, 9);
				result = tint->UpdateText(NULL, tint_imgptz_param.AUTO_SET_ID);
				break;

			default:
				break;

		}

	return VS_OK;
}

// ----------------------------------------------------------------------
INT  Model::Tint_ImgPtz_AddActions(
	const INT  handle_model,
	const INT  element_index,
	FLOAT      action_values[]
)
{

	// ---- local
		INT num_actions = 0;

	// ---- local
		using namespace vs_system;
		Model *model = (Model *)handle_model;
		vs_system::ObjConfig *obj_config = model->GetObjConfig();

		//CHAR msg[128];
		//sprintf(msg, " -> %d\n", element_index);
		//OutputDebugString(msg);

	// ---- update ?
		switch ( element_index )
		{

			case tint_imgptz_param.AUTO_ID:
					//{

					//	num_actions = 3;

					//	BOOL auto_ptf_running = obj_config->IsAutoPTFRunning();

					//	FLOAT action_value = auto_ptf_running ? 1.0f : 0.0f;

					//	CHAR msg[128];
					//	sprintf(msg, "    %d %d\n", element_index, (INT)action_value);
					//	OutputDebugString(msg);

					//	FLOAT v[] =
					//		{
					//			(FLOAT)(tint_imgptz_param.AUTO_ON_ID), action_value,
					//			(FLOAT)(tint_imgptz_param.AUTO_OFF_ID), action_value,
					//			(FLOAT)(tint_imgptz_param.AUTO_SHUFFLE_ID), action_value
					//		};

					//	for (INT i = 0; i < num_actions*2; i++) action_values[i] = v[i];
					//	
					//}
					break;

				default:
					break;
		}

	return num_actions;
}

// ----------------------------------------------------------------------
INT  Model::Tint_ImgPtz_AddText(
		const INT  handle_obj_config,
		const INT  element_id,
		BOOL      *replace_text,
		BOOL      *add_text,
		CHAR      *text
	)
{

	// ---- init
		*replace_text = FALSE;
		*add_text = FALSE;

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_obj_config;

	// ---- get ptf
		UINT  ptf_id = 0;
		FLOAT pan=0, tlt=0, fov=0;
		vs_system::ObjConfig *obj_config = model->GetObjConfig();
		result = obj_config->GetPanoPTFId(&ptf_id);
		result = obj_config->GetPanoPTF(&pan, &tlt, &fov);

	// ---- get from auto ptf id info
		UINT  from_id = 0;
		FLOAT from_mu;
		result = obj_config->GetAutoPTFFromId(&from_id, &from_mu);

	// ---- select elem
		switch ( element_id )
		{
			case 0:
				{
					sprintf(text, "Pan %5.2f & Tilt %5.2f", pan, tlt);
					*replace_text = TRUE;
				}
				break;

			case tint_imgptz_param.ZOOM_ID:
				{ 
					FLOAT zoom = 1 / ( 1 + (fov - 1) / 10 );
					sprintf(text, "Zoom %5.2f", zoom);
					*replace_text = TRUE;
				}
				break;

			case tint_imgptz_param.AUTO_SET_ID:
				{
					FLOAT auto_id = from_id + from_mu;
					sprintf(text, "%1.2f", auto_id);
					*replace_text = TRUE;
				}
				break;

			default:
				break;

		}

	return VS_OK;
}

// ----------------------------------------------------------------------
INT  Model::Tint_ImgPtz_AddValue(
		const INT  handle_obj_config,
		const INT  element_id,
		BOOL      *add_value,
		FLOAT     *value,
		CHAR      *suffix
	)
{

	*add_value = FALSE;
	*value     = 0;
	sprintf(suffix, "2");

	return VS_OK;
}
