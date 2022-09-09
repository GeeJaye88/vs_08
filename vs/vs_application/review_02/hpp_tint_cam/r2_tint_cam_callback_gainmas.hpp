////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_cam_callback_gainmas.hpp ----------
/*!
\file r2_tint_cam_callback_gainmas.hpp
\brief
\author Gareth Edwards
*/


// -------- Functionality and Parameters for Tabbed Interface Acquire

class Model::Tint_GainMas_Param
{

	public:

	// ---- cdtor
		Tint_GainMas_Param::Tint_GainMas_Param(){;};
		Tint_GainMas_Param::~Tint_GainMas_Param(){;};


	// --- props
		BOOL _large_incr = TRUE;
		INT  _update_element_index = -1;


	// --- indices
		enum
		{
			GM_AUTO_INDEX = 0,
			GM_LMH_INDEX  = 1, 
			GM_USER_INDEX = 4,

			BL_USER_INDEX = 8,

			GA_LMH_AUTO   = 10,
			GA_LMH_INDEX  = 11,
			GA_USER_INDEX = 14,

			IMG_SRI_INDEX = 18
		};


} tint_gainmas_param;


// ----------------------------------------------------------------------
INT Model::Tint_GainMas_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_GainMas_BuildList;
	tint_callback->callback_get_icon_info  = Tint_GainMas_GetIconInfo;
	tint_callback->callback_update_model   = Tint_GainMas_UpdateModel;
	tint_callback->callback_update_element = Tint_GainMas_UpdateElement;
	tint_callback->callback_add_actions    = Tint_GainMas_AddActions;
	tint_callback->callback_add_text       = Tint_GainMas_AddText;
	tint_callback->callback_add_value      = Tint_GainMas_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_GainMas_BuildList(
		vs_system::TintItem li[]
	)
{

	// ---- local
		FLOAT lincr  = 4;
		INT   result = 0;


	// ---- local for TINT_ macros
		UINT gid = ELEM_GROUP_GAINMAS_LID;
		UINT noi = 0;


	// ---- build
		{

			// ---- scope tint def & enum
				using namespace vs_system;

			// ---- set
				TINT_SET_START_NOI()

			// ---- gain: a
				TINT_ADD_ITEM("Gain")
				TINT_SET_TARCXLI(Tint::ITEM_RADIO_LOCAL, 3, 6, 0, 0.0f, TRUE, lincr)

			// ---- gain master: lmh
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO_LOCAL,    3, 4, 5)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   3, 4, 6, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_RADIO_LOCAL, 3, 4, 7, 2.2f, TRUE, lincr)

			// ---- gain master: u<o>
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO_LOCAL,    3, 6, 4)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_CLICK,         3, 5, 0, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_CLICK,         3, 5, 1, 2.2f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_CLICK,       3, 5, 2, 3.3f, TRUE, lincr * 3)

			// ---- black level: <o>
				TINT_ADD_ITEM("Black Level")
				TINT_SET_TARC(Tint::ITEM_CLICK,          3, 5, 0)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_CLICK,         3, 5, 1, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_CLICK,       3, 5, 2, 2.2f, TRUE, lincr * 3)

			// ---- gamma: lmh
				TINT_ADD_ITEM("Gamma")
				TINT_SET_TARC(Tint::ITEM_RADIO_LOCAL,    3, 4, 5)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   3, 4, 6, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_RADIO_LOCAL, 3, 4, 7, 2.2f, TRUE, lincr)

			// ---- gamma: u<o>
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO_LOCAL,    3, 6, 4)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_CLICK,         3, 5, 0, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_CLICK,         3, 5, 1, 2.2f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_CLICK,       3, 5, 2, 3.3f, TRUE, lincr * 3)

			// ---- sri
				TINT_ADD_ITEM("Save")
				TINT_SET_TARC(Tint::ITEM_CLICK,          3, 3, 3)
				TINT_ADD_ITEM("Reset")
				TINT_SET_TARCX(Tint::ITEM_CLICK,         3, 3, 4, 1.5f)
				TINT_ADD_ITEM("Increment")
				TINT_SET_TARCXLI(Tint::ITEM_SWITCH,      3, 4, 3, 3.0f, TRUE, lincr)

			// ---- get 
				TINT_SET_TOTAL_NOI()

		}


	// ---- set status by data


	// ---- cam props
		using namespace vs_system;
		CamProps *cam_props = s_pi_model->cam_props;


	// ---- gain - ALMHU
		INT gm_index = 0;
		result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::GAIN_MASTER_INDEX,
				&gm_index
			);
		li[tint_gainmas_param.GM_AUTO_INDEX + gm_index].SetStatus(TRUE);


	// ---- gamma - LMHU
		FLOAT ga_index = 0;
		result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::GAMMA_INDEX,
				&ga_index
			);
		if ( ga_index < 1 )
		{
			ga_index = 1;
			result = cam_props->SetProperty(
					CamProps::SETUP,
					0,
					CamProps::GAMMA_INDEX,
					ga_index
				);
		}
		li[tint_gainmas_param.GA_LMH_AUTO + (INT)ga_index].SetStatus(TRUE);


	// ---- Gamma column LF
		li[tint_gainmas_param.GA_LMH_INDEX].SetColumnLF(TRUE);


	// ---- SRI column LF
		li[tint_gainmas_param.IMG_SRI_INDEX].SetColumnLF(TRUE);


	// ---- increment (large)
		li[tint_gainmas_param.IMG_SRI_INDEX+2].SetStatus(FALSE);

	return noi;
}


// ----------------------------------------------------------------------
INT Model::Tint_GainMas_GetIconInfo(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 3;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 4;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
		case vs_system::Tint::ICON_ATLAS_PAGE_ID : return 3;
		default:
			break;
	}
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_GainMas_UpdateModel(
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
		BOOL edit_config = cam_props->GetEditConfig();


	// ---- get all callbacks - might as well...

		INT(*setup_callback)() = NULL;
		result = cam_props->GetSetupCallback(&setup_callback);

		INT(*gain_master_callback)() = NULL;
		result = cam_props->GetGainMasterCallback(&gain_master_callback);

		INT(*black_level_callback)() = NULL;
		result = cam_props->GetBlackLevelCallback(&black_level_callback);

		INT(*gamma_callback)() = NULL;
		result = cam_props->GetGammaCallback(&gamma_callback);


	// ---- update ?
		switch ( element_index )
		{

			// ---- gain : A
				case tint_gainmas_param.GM_AUTO_INDEX : // A
					{

						INT auto_gain = 0;
						result = cam_props->GetProperty(
								CamProps::SETUP,
								0,
								CamProps::AUTO_GAIN,
								&auto_gain
							);
						auto_gain = auto_gain ? 0 : 1;
						result = cam_props->SetProperty(
								CamProps::SETUP,
								0,
								CamProps::AUTO_GAIN,
								(FLOAT)auto_gain
							);
						/*
						result = cam_props->SetProperty(
								CamProps::SETUP,
								0,
								CamProps::GAIN_MASTER_INDEX,
								0
							);
						*/
						result = setup_callback();
						result = gain_master_callback();
					}
					break;


			// ---- gain : LMHU
				case tint_gainmas_param.GM_LMH_INDEX     : // L
				case tint_gainmas_param.GM_LMH_INDEX + 1 : // M
				case tint_gainmas_param.GM_LMH_INDEX + 2 : // H
				case tint_gainmas_param.GM_USER_INDEX    : // U
					{
						/*
						result = cam_props->SetProperty(
								CamProps::SETUP,
								0,
								CamProps::AUTO_GAIN,
								0
							);
						*/
						result = cam_props->SetProperty(
								CamProps::SETUP,
								0,
								CamProps::GAIN_MASTER_INDEX,
								(FLOAT)element_index - tint_gainmas_param.GM_LMH_INDEX + 1
							);

						result = setup_callback();  
						result = gain_master_callback();
						result = black_level_callback();
					}
					break;


			// ---- gain : < o >
				case tint_gainmas_param.GM_USER_INDEX + 1 : // <
				case tint_gainmas_param.GM_USER_INDEX + 2 : // o
				case tint_gainmas_param.GM_USER_INDEX + 3 : // >
					{

						FLOAT incr = tint_gainmas_param._large_incr ? 0.1f : 0.01f;

						INT  gm_index = 0;
						if ( edit_config )
						{
							result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::GAIN_MASTER_INDEX, &gm_index);
						}
						else
						{
							gm_index = CamProps::NUM_GAIN_MASTER_CONFIGS - 1;
						}

						FLOAT gm_gain = 0;
						result = cam_props->GetProperty(CamProps::GAIN_MASTER, gm_index, CamProps::GAIN, &gm_gain);

						switch ( element_index )
						{
							case tint_gainmas_param.GM_USER_INDEX + 1: // <
								gm_gain = gm_gain - incr < 0 ? 0 : gm_gain - incr;
								break;
							case tint_gainmas_param.GM_USER_INDEX + 2: // o
								gm_gain = 0;
								break;
							case tint_gainmas_param.GM_USER_INDEX + 3: // >
								gm_gain = gm_gain + incr > 1 ? 1 : gm_gain + incr;
								break;
						}

						result = cam_props->SetProperty(CamProps::GAIN_MASTER, gm_index, CamProps::GAIN, gm_gain);
						result = gain_master_callback();
						result = black_level_callback();
					}
					break;


			// ---- black level : < o >
				case tint_gainmas_param.BL_USER_INDEX     : // <
				case tint_gainmas_param.BL_USER_INDEX + 1 : // o
				case tint_gainmas_param.BL_USER_INDEX + 2 : // >
					{

						FLOAT incr = tint_gainmas_param._large_incr ? 0.1f : 0.01f;

						INT  gm_index = 0;
						result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::GAIN_MASTER_INDEX, &gm_index);

						FLOAT gm_black_level = 0;
						result = cam_props->GetProperty(CamProps::GAIN_MASTER, gm_index, CamProps::BLACK_LEVEL, &gm_black_level);

						switch ( element_index )
						{
							case tint_gainmas_param.BL_USER_INDEX    : // <
								gm_black_level = gm_black_level - incr < 0 ? 0 : gm_black_level - incr;
								break;
							case tint_gainmas_param.BL_USER_INDEX + 1 : // o
								gm_black_level = 0;
								break;
							case tint_gainmas_param.BL_USER_INDEX + 2 : // >
								gm_black_level = gm_black_level + incr > 1 ? 1 : gm_black_level + incr;
								break;
						}

						result = cam_props->SetProperty(CamProps::GAIN_MASTER, gm_index, CamProps::BLACK_LEVEL, gm_black_level);
						result = gain_master_callback();
						result = black_level_callback();
					}
					break;


			// ---- gamma : LMHU
				case tint_gainmas_param.GA_LMH_INDEX     : // L
				case tint_gainmas_param.GA_LMH_INDEX + 1 : // M
				case tint_gainmas_param.GA_LMH_INDEX + 2 : // H
				case tint_gainmas_param.GA_USER_INDEX    : // U
					{
						result = cam_props->SetProperty(
								CamProps::SETUP,
								0,
								CamProps::GAMMA_INDEX,
								(FLOAT)element_index - tint_gainmas_param.GA_LMH_AUTO
							);
						result = gamma_callback();
					}
					break;


			// ---- gamma : < o >
				case tint_gainmas_param.GA_USER_INDEX + 1 : // <
				case tint_gainmas_param.GA_USER_INDEX + 2 : // o
				case tint_gainmas_param.GA_USER_INDEX + 3 : // >
					{

						FLOAT incr = tint_gainmas_param._large_incr ? 0.1f : 0.01f;

						INT  ga_index = 0;
						if ( edit_config )
						{
							result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::GAMMA_INDEX, &ga_index);
						}
						else
						{
							ga_index = CamProps::NUM_GAIN_MASTER_CONFIGS - 1;
						}

						FLOAT ga_factor = 0;
						result = cam_props->GetProperty(CamProps::GAMMA, ga_index, CamProps::FACTOR, &ga_factor);

						switch ( element_index )
						{
							case tint_gainmas_param.GA_USER_INDEX + 1: // <
								ga_factor = ga_factor - incr < 0.01f ? 0.01f : ga_factor - incr;
								break;
							case tint_gainmas_param.GA_USER_INDEX + 2: // o
								ga_factor = 1.0f;
								break;
							case tint_gainmas_param.GA_USER_INDEX + 3: // >
								ga_factor = ga_factor + incr > 5.0f ? 5.0f : ga_factor + incr;
								break;
						}

						result = cam_props->SetProperty(CamProps::GAMMA, ga_index, CamProps::FACTOR, ga_factor);
						result = gamma_callback();
					}
					break;


			// ---- save
				case tint_gainmas_param.IMG_SRI_INDEX:
					result = model->SaveCamProps();
					break;


			// ---- reset
				case tint_gainmas_param.IMG_SRI_INDEX + 1:
					{
						// ---- reset and then update
							cam_props->ResetProperty(CamProps::GAIN_MASTER);
							Tint *tint = model->pi_model->tint_cam_setng;
							tint->UpdateAllText();
							result = gain_master_callback();
							result = black_level_callback();
					}
					break;


			// ---- increment
				case tint_gainmas_param.IMG_SRI_INDEX + 2:
						tint_gainmas_param._large_incr =
							tint_gainmas_param._large_incr ? FALSE : TRUE;
					break;

				default:
					OutputDebugString("--- huh: Tint_GainMas_UpdateModel\n");
					break;
		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_GainMas_UpdateElement(
		INT handle_model,
		INT element_index
	)
{

	// ---- store for "add" callbacks
		tint_gainmas_param._update_element_index = element_index;


	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_cam_setng;


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_HEAD_GAINMAS_LID);


	// ---- update ?
		switch ( element_index )
		{

			// ---- gain master : ALMHU
				case tint_gainmas_param.GM_AUTO_INDEX     : // A
				case tint_gainmas_param.GM_LMH_INDEX      : // L
				case tint_gainmas_param.GM_LMH_INDEX + 1  : // M
				case tint_gainmas_param.GM_LMH_INDEX + 2  : // H
				case tint_gainmas_param.GM_USER_INDEX     : // U
				case tint_gainmas_param.GM_USER_INDEX + 1 : // <
				case tint_gainmas_param.GM_USER_INDEX + 2 : // o
				case tint_gainmas_param.GM_USER_INDEX + 3 : // >
					{
						result = tint->UpdateText(NULL, tint_gainmas_param.GM_AUTO_INDEX);
						result = tint->UpdateText(NULL, tint_gainmas_param.BL_USER_INDEX);


						//
						// set to gain colour tabbed head, and invoke wb text update
						//
						// very sneaky, ... and only works due to the order in which the
						// vs_tint_cam_callback_gaincol and vs_tint_cam_gainmas.hpp
						// files are included.
						//
						result = tint->SetSelectedBody(TINT_HEAD_GAINCOL_LID);
						result = tint->UpdateText(NULL, tint_gaincol_param.GC_AUTO_INDEX);


						// reset to gain master tabbed head
						result = tint->SetSelectedBody(TINT_HEAD_GAINMAS_LID);

					}
					break;


			// ----- blacklevel : < o >
				case tint_gainmas_param.BL_USER_INDEX     : // <
				case tint_gainmas_param.BL_USER_INDEX + 1 : // o
				case tint_gainmas_param.BL_USER_INDEX + 2 : // >
					{
						result = tint->UpdateText(NULL, tint_gainmas_param.BL_USER_INDEX);
					}
					break;


			// ---- gamma : LMHU
				case tint_gainmas_param.GA_LMH_INDEX      : // L
				case tint_gainmas_param.GA_LMH_INDEX + 1  : // M
				case tint_gainmas_param.GA_LMH_INDEX + 2  : // H
				case tint_gainmas_param.GA_USER_INDEX     : // U
				case tint_gainmas_param.GA_USER_INDEX + 1 : // <
				case tint_gainmas_param.GA_USER_INDEX + 2 : // o
				case tint_gainmas_param.GA_USER_INDEX + 3 : // >
					{
						result = tint->UpdateText(NULL, tint_gainmas_param.GA_LMH_INDEX);
					}
					break;


			// ---- save, reset & increment - note: AddText & AddValue determine GM text
				case tint_gainmas_param.IMG_SRI_INDEX + 1:
					{
						result = tint->UpdateText(NULL, tint_gainmas_param.GM_AUTO_INDEX);
						result = tint->UpdateText(NULL, tint_gainmas_param.BL_USER_INDEX);
						result = tint->UpdateText(NULL, tint_gainmas_param.GA_LMH_INDEX);
					}
					break;


			default:
				break;
		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_GainMas_AddActions(
	const INT  handle_obj_config,
	const INT  element_index,
	FLOAT      action_values[]
)
{

	// ---- local
		using namespace vs_system;
		CamProps *cam_props = (CamProps *)handle_obj_config;
		BOOL edit_config = cam_props->GetEditConfig();


	// ---- local
		INT num_actions = 0;


	// ---- ignore < o >
		switch ( tint_gainmas_param._update_element_index )
		{
			case tint_gainmas_param.GM_USER_INDEX + 1: // <
			case tint_gainmas_param.GM_USER_INDEX + 2: // o
			case tint_gainmas_param.GM_USER_INDEX + 3: // >

			case tint_gainmas_param.BL_USER_INDEX    : // <
			case tint_gainmas_param.BL_USER_INDEX + 1: // o
			case tint_gainmas_param.BL_USER_INDEX + 2: // >

			case tint_gainmas_param.GA_USER_INDEX + 1: // <
			case tint_gainmas_param.GA_USER_INDEX + 2: // o
			case tint_gainmas_param.GA_USER_INDEX + 3: // >
				return 0;

			default:
				break;
		}


	// ---- update ?
		switch ( element_index )
		{

			// ---- gain master : auto | lmh | user
				case tint_gainmas_param.GM_AUTO_INDEX :
					{
						num_actions = 6;

						FLOAT action_value =
								edit_config ? 1.0f :
									 cam_props->GetActionValue(
											tint_gainmas_param._update_element_index,
											CamProps::AUTO_GAIN,
											CamProps::GAIN_MASTER_INDEX,
											CamProps::NUM_GAIN_MASTER_CONFIGS,
											tint_gainmas_param.GM_USER_INDEX
										);

						FLOAT v[] =
							{
								(FLOAT)(tint_gainmas_param.GM_USER_INDEX + 1), action_value,
								(FLOAT)(tint_gainmas_param.GM_USER_INDEX + 2), action_value,
								(FLOAT)(tint_gainmas_param.GM_USER_INDEX + 3), action_value,
								(FLOAT)(tint_gainmas_param.BL_USER_INDEX    ), action_value,
								(FLOAT)(tint_gainmas_param.BL_USER_INDEX + 1), action_value,
								(FLOAT)(tint_gainmas_param.BL_USER_INDEX + 2), action_value
							};

						for (INT i = 0; i < num_actions*2; i++) action_values[i] = v[i];
						
					}
					break;

			// ----- gamma : lmh | user
				case tint_gainmas_param.GA_LMH_INDEX :
					{

						num_actions = 3;

						FLOAT action_value =
								edit_config ? 1.0f :
									 cam_props->GetActionValue(
											tint_gainmas_param._update_element_index,
											0,
											CamProps::GAMMA_INDEX,
											CamProps::NUM_GAMMA_CONFIGS,
											tint_gainmas_param.GA_USER_INDEX
										);

						FLOAT v[] =
							{
								(FLOAT)(tint_gainmas_param.GA_USER_INDEX + 1), action_value,
								(FLOAT)(tint_gainmas_param.GA_USER_INDEX + 2), action_value,
								(FLOAT)(tint_gainmas_param.GA_USER_INDEX + 3), action_value
							};

						for (INT i = 0; i < num_actions*2; i++) action_values[i] = v[i];

					}
					break;

				default:
					break;
			}

	return num_actions;
}


// ----------------------------------------------------------------------
INT  Model::Tint_GainMas_AddText(
		const INT  handle_obj_config,
		const INT  element_index,
		BOOL      *replace_text,
		BOOL      *add_text,
		CHAR      *text
	)
{

	// ---- init
		*replace_text = FALSE;
		*add_text     = FALSE;
		*text         = NULL;


	// ---- local
		INT result = VS_OK;


	// ---- cast to get camera properties object
		vs_system::CamProps *cam_props = (vs_system::CamProps *)handle_obj_config;


	// ---- set
		switch ( element_index )
		{

			case tint_gainmas_param.GM_AUTO_INDEX:
				{

					INT auto_gm = 0;
					result = cam_props->GetProperty(
							vs_system::CamProps::SETUP,
							0,
							vs_system::CamProps::AUTO_GAIN,
							&auto_gm
						);

					if ( auto_gm )
					{
						*add_text = TRUE;
						strcpy(text, " AUTO");
					}

				}
				break;

			case tint_gainmas_param.BL_USER_INDEX:
				{

					INT auto_gm = 0;
					result = cam_props->GetProperty(
							vs_system::CamProps::SETUP,
							0,
							vs_system::CamProps::AUTO_GAIN,
							&auto_gm
						);

					if ( auto_gm )
					{
						*add_text = TRUE;
						strcpy(text, " AUTO");
					}

				}
				break;

			default:
				break;
		}
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_GainMas_AddValue(
		const INT  handle_obj_config,
		const INT  element_index,
		BOOL      *add_value,
		FLOAT     *value,
		CHAR      *suffix
	)
{

	// ---- init
		*add_value = FALSE;
		*value     = 0;
		sprintf(suffix, "2");


	// ---- local
		INT result = VS_OK;


	// ---- cast to get camera properties object
		using namespace vs_system;
		CamProps *cam_props = (vs_system::CamProps *)handle_obj_config;


	// ---- select
		switch ( element_index )
		{

			case tint_gainmas_param.GM_AUTO_INDEX:
				{

					INT auto_gain = 0;
					result = cam_props->GetProperty(
							CamProps::SETUP,
							0,
							CamProps::AUTO_GAIN,
							&auto_gain
						);

					if ( ! auto_gain )
					{
						*add_value = TRUE;
						INT gm_index = 0;
						result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::GAIN_MASTER_INDEX, &gm_index);
						FLOAT gain = 0;
						result = cam_props->GetProperty(CamProps::GAIN_MASTER, gm_index, CamProps::GAIN, &gain);
						*value = gain;
					}

				}
				break;

			case tint_gainmas_param.BL_USER_INDEX:
				{
				
					INT auto_gain = 0;
					result = cam_props->GetProperty(
							CamProps::SETUP,
							0,
							CamProps::AUTO_GAIN,
							&auto_gain
						);

					if ( !auto_gain )
					{
						*add_value = TRUE;
						INT gm_index = 0;
						result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::GAIN_MASTER_INDEX, &gm_index);
						FLOAT gm_black_level = 0;
						result = cam_props->GetProperty(CamProps::GAIN_MASTER, gm_index, CamProps::BLACK_LEVEL, &gm_black_level);
						*value = gm_black_level;
					}

				}
				break;


			case tint_gainmas_param.GA_LMH_INDEX:
				{
					*add_value = TRUE;
					INT ga_index = 0;
					result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::GAMMA_INDEX, &ga_index);
					FLOAT ga_factor = 0;
					result = cam_props->GetProperty(CamProps::GAMMA, ga_index, CamProps::FACTOR, &ga_factor);
					*value = ga_factor;
				}
				break;

			default:
				break;
		}

	return VS_OK;
}
