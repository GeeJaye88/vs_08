////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_cam_callback_settings.hpp ----------
/*!
\file r2_tint_cam_callback_settings.hpp
\brief
\author Gareth Edwards
*/


using namespace review_02;


// -------- Functionality and Parameters for Tabbed Interface Acquire

class Model::Tint_Settings_Param
{

public:

	// ---- cdtor
		Tint_Settings_Param::Tint_Settings_Param() { ; };
		Tint_Settings_Param::~Tint_Settings_Param() { ; };


	// --- indices
		enum
		{
			SET_ROP_INDEX  = 0,
			#if defined(VS_CAMERA_05)
			SET_MODE_INDEX = 2,
			SET_EDIT_INDEX = 5,
			SET_SRR_INDEX  = 6
			#else
			SET_EDIT_INDEX = 2,
			SET_SRR_INDEX  = 3
			#endif
		};

} tint_settings_param;


// ----------------------------------------------------------------------
INT Model::Tint_Settings_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_Settings_BuildList;
	tint_callback->callback_get_icon_info  = Tint_Settings_GetIconInfo;
	tint_callback->callback_update_model   = Tint_Settings_UpdateModel;
	tint_callback->callback_update_element = Tint_Settings_UpdateElement;
	tint_callback->callback_add_actions    = NULL;
	tint_callback->callback_add_text       = Tint_Settings_AddText;
	tint_callback->callback_add_value      = Tint_Settings_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Settings_BuildList(
		vs_system::TintItem li[]
	)
{

	// ---- local
		FLOAT lincr  = 4;
		INT   result = 0;


	// ---- local for TINT_ macros
		UINT gid = ELEM_GROUP_SETTINGS_LID;
		UINT noi = 0;


	// ---- build
		{

			// ---- scope tint def & enum
				using namespace vs_system;

			// ---- set
				TINT_SET_START_NOI()

			// ---- rop
				TINT_ADD_ITEM("Raster Operations")
				TINT_SET_TARC(Tint::ITEM_SWITCH,    3, 6, 7)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_SWITCH, 3, 6, 6, 1.5f, TRUE, 12)

			// ---- mode
				#if defined(VS_CAMERA_05)
				TINT_ADD_ITEM("Image Size (requires restart)")
				TINT_SET_TARCX(Tint::ITEM_RADIO,    3, 5, 3, 0.0f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO,    3, 3, 6, 1.5f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_RADIO,  3, 3, 7, 3.0f, TRUE, 12)
				#endif

			// ---- edit
				TINT_ADD_ITEM("Edit Pre-Defined")
				TINT_SET_TARCXLI(Tint::ITEM_SWITCH, 3, 6, 5, 0.0f, TRUE, 12)

			// ---- save & reset
				TINT_ADD_ITEM("Save")
				TINT_SET_TARC(Tint::ITEM_CLICK,     3, 3, 3)
				TINT_ADD_ITEM("Reset Defaults")
				TINT_SET_TARCXLI(Tint::ITEM_CLICK,  3, 3, 4, 1.5f, TRUE, lincr)

			// ---- get 
				TINT_SET_TOTAL_NOI();

		}


	// ---- set by data
		{

			// ---- cam props
				using namespace vs_system;
				CamProps *cam_props = s_pi_model->cam_props;

			// ---- get rop
				INT h_flip = 0;
				result = cam_props->GetProperty(
						CamProps::ROP,
						0,
						CamProps::H_FLIP,
						&h_flip
					);
				INT v_flip = 0;
				result = cam_props->GetProperty(
						CamProps::ROP,
						0,
						CamProps::V_FLIP,
						&v_flip
					);

			// ---- flip
				li[tint_settings_param.SET_ROP_INDEX  ].SetStatus(h_flip ? TRUE : FALSE);
				li[tint_settings_param.SET_ROP_INDEX+1].SetStatus(v_flip ? TRUE : FALSE);

			// ---- camera mode
				#if defined(VS_CAMERA_05)
				INT mode = GetAppConfigCameraMode();
				if ( mode )
					li[tint_settings_param.SET_MODE_INDEX+mode-1].SetStatus(TRUE);
				#endif
		}


	// ---- column LF
		li[tint_settings_param.SET_EDIT_INDEX].SetColumnLF(TRUE);
		li[tint_settings_param.SET_SRR_INDEX].SetColumnLF(TRUE);

	return noi;
}


// ----------------------------------------------------------------------
INT Model::Tint_Settings_GetIconInfo(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 3;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 7;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 2;
		case vs_system::Tint::ICON_ATLAS_PAGE_ID : return 3;
		default:
			break;
	}
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Settings_UpdateModel(
		INT  handle_model,
		INT  element_index
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;


	// ---- cam props
		using namespace vs_system;
		CamProps *cam_props = NULL;
		result = model->GetCamProps(&cam_props);


	// ---- get callbacks - might as well...

		INT (*rop_callback)() = NULL;
		result = cam_props->GetRopCallback(&rop_callback);


	// ---- update ?
		switch ( element_index )
		{

			// ----- horizontal
				case tint_settings_param.SET_ROP_INDEX :
					{ 
						INT h_flip = 0;
						result = cam_props->GetProperty(
								CamProps::ROP,
								0,
								CamProps::H_FLIP,
								&h_flip
							);
						h_flip = h_flip ? 0 : 1;
						result = cam_props->SetProperty(
								CamProps::ROP,
								0,
								CamProps::H_FLIP,
								(FLOAT)h_flip
							);
						result = rop_callback();
					}
					break;

			// ----- vertical
				case tint_settings_param.SET_ROP_INDEX + 1 :
					{ 
						INT v_flip = 0;
						result = cam_props->GetProperty(
								CamProps::ROP,
								0,
								CamProps::V_FLIP,
								&v_flip
							);
						v_flip = v_flip ? 0 : 1;
						result = cam_props->SetProperty(
								CamProps::ROP,
								0,
								CamProps::V_FLIP,
								(FLOAT)v_flip
							);
						result = rop_callback();
					}
					break;

			// ----- imaging mode
				#if defined(VS_CAMERA_05)
				case tint_settings_param.SET_MODE_INDEX :
				case tint_settings_param.SET_MODE_INDEX + 1:
				case tint_settings_param.SET_MODE_INDEX + 2:
					{
						INT mode = element_index - tint_settings_param.SET_MODE_INDEX + 1;
						result = SetAppConfigCameraMode(mode);
					}
					break;
				#endif

			// ---- edit
				case tint_settings_param.SET_EDIT_INDEX:
					{

						// ---- flip state 
							BOOL edit_config = cam_props->GetEditConfig();
							edit_config = edit_config ? FALSE : TRUE;
							result = cam_props->SetEditConfig(edit_config);
							
						// ---- update all (yay - who's the man!)
							Tint *tint = model->pi_model->tint_cam_setng;
							tint->UpdateAllText();

					}
					break;

			// ---- save
				case tint_settings_param.SET_SRR_INDEX:
					result = model->SaveCamProps();
					break;

			// ---- reset
				case tint_settings_param.SET_SRR_INDEX + 1:
					{
						// ---- reset and then update
							cam_props->ResetCallbackProperties();
							Tint *tint = model->pi_model->tint_cam_setng;
							tint->UpdateAllText();
					}
					break;
			
				default:
					break;
		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Settings_UpdateElement(
		INT handle_model,
		INT element_index
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_cam_setng;


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_HEAD_SETTINGS_LID);


	// ---- update ?
		switch ( element_index )
		{
			case 0:
			default:
				break;
		}

	return VS_OK;
}



// ----------------------------------------------------------------------
INT  Model::Tint_Settings_AddActions(
	const INT  handle_obj_config,
	const INT  element_index,
	FLOAT      action_values[]
)
{
	return 0;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Settings_AddText(
		const INT  handle_obj_config,
		const INT  element_index,
		BOOL      *replace_text,
		BOOL      *add_text,
		CHAR      *text
	)
{

	// ---- init
		*add_text     = FALSE;
		*replace_text = FALSE;
		*text         = NULL;

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Settings_AddValue(
		const INT  handle_obj_config,
		const INT  element_index,
		BOOL      *add_value,
		FLOAT     *value,
		CHAR      *suffix
	)
{

	// ---- init
		*add_value = FALSE;

	return VS_OK;
}
