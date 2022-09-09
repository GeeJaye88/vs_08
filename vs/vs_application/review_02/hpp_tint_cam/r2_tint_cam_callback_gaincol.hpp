////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_cam_callback_gaincol.hpp ----------
/*!
\file r2_tint_cam_callback_gaincol.hpp
\brief
\author Gareth Edwards
*/


// -------- Functionality and Parameters for Tabbed Interface Gain Colour

class Model::Tint_GainCol_Param
{

	public:

	// ---- cdtor
		Tint_GainCol_Param::Tint_GainCol_Param(){;};
		Tint_GainCol_Param::~Tint_GainCol_Param(){;};


	// --- props
		BOOL _large_incr   = TRUE;
		INT  _update_element_index = -1;


	// --- indices
		enum
		{
			GC_AUTO_INDEX = 0,
			GC_LMH_INDEX   = 1,
			GC_USER_INDEX  = 6,
			GC_RED_INDEX   = 7,
			GC_GREEN_INDEX = 10,
			GC_BLUE_INDEX  = 13,

			GC_SRI_INDEX   = 16
		};


} tint_gaincol_param;


// ----------------------------------------------------------------------
INT Model::Tint_GainCol_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_GainCol_BuildList;
	tint_callback->callback_get_icon_info  = Tint_GainCol_GetIconInfo;
	tint_callback->callback_update_model   = Tint_GainCol_UpdateModel;
	tint_callback->callback_update_element = Tint_GainCol_UpdateElement;
	tint_callback->callback_add_actions    = Tint_GainCol_AddActions;
	tint_callback->callback_add_text       = Tint_GainCol_AddText;
	tint_callback->callback_add_value      = Tint_GainCol_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_GainCol_BuildList(
		vs_system::TintItem li[]
	)
{

	// ---- local
		FLOAT lincr = 4;
		INT   result = 0;


	// ---- local for TINT_ macros
		UINT gid = ELEM_GROUP_GAINCOL_LID;
		UINT noi = 0;


	// ---- build
		{

			// ---- scope tint def & enum
				using namespace vs_system;

			// ---- set
				TINT_SET_START_NOI()

			// ---- wb : A
				TINT_ADD_ITEM("WB")
				TINT_SET_TARCXLI(Tint::ITEM_RADIO, 3, 6, 0, 0.0f, TRUE, lincr)

			// ---- gain colour : (1)
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO, 1, 2, 0)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO, 1, 2, 1, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO, 1, 2, 2, 2.2f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO, 1, 2, 3, 3.3f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_RADIO, 1, 2, 4, 4.4f, TRUE, lincr * 3)

			// ---- gain colour : M
				TINT_ADD_ITEM("User")
				TINT_SET_TARC(Tint::ITEM_RADIO, 3, 6, 4)

			// ---- gain colour : <o>
				TINT_ADD_ITEM("Red")
				TINT_SET_TARCX(Tint::ITEM_CLICK, 3, 5, 0, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_CLICK, 3, 5, 1, 2.2f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_CLICK, 3, 5, 2, 3.3f, TRUE, lincr)

			// ---- gain colour : <o>
				TINT_ADD_ITEM("Green")
				TINT_SET_TARCX(Tint::ITEM_CLICK, 3, 5, 0, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_CLICK, 3, 5, 1, 2.2f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_CLICK, 3, 5, 2, 3.3f, TRUE, lincr)

			// ---- gain colour : <o>
				TINT_ADD_ITEM("Blue")
				TINT_SET_TARCX(Tint::ITEM_CLICK, 3, 5, 0, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_CLICK, 3, 5, 1, 2.2f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_CLICK, 3, 5, 2, 3.3f, TRUE, lincr)

			// ---- sri
				TINT_ADD_ITEM("Save")
				TINT_SET_TARC(Tint::ITEM_CLICK, 3, 3, 3)
				TINT_ADD_ITEM("Reset")
				TINT_SET_TARCX(Tint::ITEM_CLICK, 3, 3, 4, 1.5f)
				TINT_ADD_ITEM("Increment")
				TINT_SET_TARCXLI(Tint::ITEM_SWITCH, 3, 4, 3, 3.0f, TRUE, lincr)

			// ---- get 
				TINT_SET_TOTAL_NOI()

		}


	// ---- set by data


	// ---- cam props
		using namespace vs_system;
		CamProps *cam_props = s_pi_model->cam_props;


	// ---- wb - ALMHU
		INT wb_index = 0;
		result = cam_props->GetProperty(
				CamProps::SETUP,
				0,
				CamProps::GAIN_COLOUR_INDEX,
				&wb_index
			);
		li[tint_gaincol_param.GC_AUTO_INDEX + wb_index].SetStatus(TRUE);

		
	// ---- USER column LF
		li[tint_gaincol_param.GC_USER_INDEX].SetColumnLF(TRUE);


	// ---- SRI column LF
		li[tint_gaincol_param.GC_SRI_INDEX].SetColumnLF(TRUE);


	// ---- increment (large)
		li[tint_gaincol_param.GC_SRI_INDEX+2].SetStatus(FALSE);

	return noi;
}


// ----------------------------------------------------------------------
INT Model::Tint_GainCol_GetIconInfo(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 3;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 5;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
		case vs_system::Tint::ICON_ATLAS_PAGE_ID : return 3;
		default:
			break;
	}
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_GainCol_UpdateModel(
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

		INT(*gain_colour_callback)() = NULL;
		result = cam_props->GetGainColourCallback(&gain_colour_callback);

		INT(*gain_master_callback)() = NULL;
		result = cam_props->GetGainMasterCallback(&gain_master_callback);


	// ---- update ?
		switch ( element_index )
		{

			// ---- gain : A
				case tint_gaincol_param.GC_AUTO_INDEX : // A
					{

						INT auto_wb = 0;
						result = cam_props->GetProperty(
								CamProps::SETUP,
								0,
								CamProps::AUTO_WB,
								&auto_wb
							);
						auto_wb = auto_wb ? 0 : 1;
						result = cam_props->SetProperty(
								CamProps::SETUP,
								0,
								CamProps::AUTO_WB,
								(FLOAT)auto_wb
							);
						/*
						result = cam_props->SetProperty(
								CamProps::SETUP,
								0,
								CamProps::GAIN_COLOUR_INDEX,
								0
							);
						*/
						result = setup_callback();
						result = gain_colour_callback();
						result = gain_master_callback();
					}
					break;


			// ---- gain : LMHU
				case tint_gaincol_param.GC_LMH_INDEX     : // Sunshine
				case tint_gaincol_param.GC_LMH_INDEX + 1 : // Cloudy
				case tint_gaincol_param.GC_LMH_INDEX + 2 : // Overcast
				case tint_gaincol_param.GC_LMH_INDEX + 3 : // Bulb
				case tint_gaincol_param.GC_LMH_INDEX + 4 : // Fluo
				case tint_gaincol_param.GC_USER_INDEX    : // User
					{

						result = cam_props->SetProperty(
								CamProps::SETUP,
								0,
								CamProps::AUTO_WB,
								0
							);

						result = cam_props->SetProperty(
								CamProps::SETUP,
								0,
								CamProps::GAIN_COLOUR_INDEX,
								(FLOAT)element_index - tint_gaincol_param.GC_AUTO_INDEX
							);

						result = setup_callback();
						result = gain_colour_callback();
						result = gain_master_callback();
					}
					break;


			// ---- gain colours : < o >
				case tint_gaincol_param.GC_RED_INDEX       : // <
				case tint_gaincol_param.GC_RED_INDEX + 1   : // o
				case tint_gaincol_param.GC_RED_INDEX + 2   : // >
				case tint_gaincol_param.GC_GREEN_INDEX     : // <
				case tint_gaincol_param.GC_GREEN_INDEX + 1 : // o
				case tint_gaincol_param.GC_GREEN_INDEX + 2 : // >
				case tint_gaincol_param.GC_BLUE_INDEX      : // <
				case tint_gaincol_param.GC_BLUE_INDEX + 1  : // o
				case tint_gaincol_param.GC_BLUE_INDEX + 2  : // >
					{

						FLOAT incr = tint_gaincol_param._large_incr ? 0.1f : 0.01f;

						INT  gc_index = 0;
						if ( edit_config )
						{
							result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::GAIN_COLOUR_INDEX, &gc_index);
						}
						else
						{
							gc_index = CamProps::NUM_GAIN_COLOUR_CONFIGS - 1;
						}

						FLOAT gm_red = 0;
						result = cam_props->GetProperty(CamProps::GAIN_COLOUR, gc_index, CamProps::RED,   &gm_red);
						FLOAT gm_green = 0;
						result = cam_props->GetProperty(CamProps::GAIN_COLOUR, gc_index, CamProps::GREEN, &gm_green);
						FLOAT gm_blue = 0;
						result = cam_props->GetProperty(CamProps::GAIN_COLOUR, gc_index, CamProps::BLUE,  &gm_blue);

						switch ( element_index )
						{
							case tint_gaincol_param.GC_RED_INDEX       : // <
								gm_red = gm_red - incr < 0 ? 0 : gm_red - incr;
								break;
							case tint_gaincol_param.GC_RED_INDEX + 1   : // o
								gm_red = 0.1f;
								break;
							case tint_gaincol_param.GC_RED_INDEX + 2   : // >
								gm_red = gm_red + incr > 1 ? 1 : gm_red + incr;
								break;

							case tint_gaincol_param.GC_GREEN_INDEX     : // <
								gm_green = gm_green - incr < 0 ? 0 : gm_green - incr;
								break;
							case tint_gaincol_param.GC_GREEN_INDEX + 1 : // o
								gm_green = 0.0f;
								break;
							case tint_gaincol_param.GC_GREEN_INDEX + 2 : // >
								gm_green = gm_green + incr > 1 ? 1 : gm_green + incr;
								break;

							case tint_gaincol_param.GC_BLUE_INDEX      : // <
								gm_blue = gm_blue - incr < 0 ? 0 : gm_blue - incr;
								break;
							case tint_gaincol_param.GC_BLUE_INDEX + 1  : // o
								gm_blue = 0.2f;
								break;
							case tint_gaincol_param.GC_BLUE_INDEX + 2  : // >
								gm_blue = gm_blue + incr > 1 ? 1 : gm_blue + incr;
								break;
						}

						result = cam_props->SetProperty(CamProps::GAIN_COLOUR, gc_index, CamProps::RED,   gm_red);
						result = cam_props->SetProperty(CamProps::GAIN_COLOUR, gc_index, CamProps::GREEN, gm_green);
						result = cam_props->SetProperty(CamProps::GAIN_COLOUR, gc_index, CamProps::BLUE,  gm_blue);
						result = gain_colour_callback();
					}
					break;


			// ---- save
				case tint_gaincol_param.GC_SRI_INDEX:
					result = model->SaveCamProps();
					break;


			// ---- reset
				case tint_gaincol_param.GC_SRI_INDEX + 1:
					{
						// ---- reset and then update
							cam_props->ResetProperty(CamProps::GAIN_COLOUR);
							Tint *tint = model->pi_model->tint_cam_setng;
							tint->UpdateAllText();
							result = gain_colour_callback();
							result = gain_master_callback();
					}
					break;


			// ---- increment
				case tint_gaincol_param.GC_SRI_INDEX + 2:
						tint_gaincol_param._large_incr =
							tint_gaincol_param._large_incr ? FALSE : TRUE;
					break;

				default:
					break;
		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_GainCol_UpdateElement(
		INT handle_model,
		INT element_index
	)
{

	// ---- store for "add" callbacks
		tint_gaincol_param._update_element_index = element_index;


	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_cam_setng;


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_HEAD_GAINCOL_LID);


	// ---- update ?
		switch ( element_index )
		{

			// ---- gain colour : auto | lmh | user
				case tint_gaincol_param.GC_AUTO_INDEX    : // Sunshine
				case tint_gaincol_param.GC_LMH_INDEX     : // Sunshine
				case tint_gaincol_param.GC_LMH_INDEX + 1 : // Cloudy
				case tint_gaincol_param.GC_LMH_INDEX + 2 : // Overcast
				case tint_gaincol_param.GC_LMH_INDEX + 3 : // Bulb
				case tint_gaincol_param.GC_LMH_INDEX + 4 : // Fluo
				case tint_gaincol_param.GC_USER_INDEX    : // User
					{
						result = tint->UpdateText(NULL, tint_gaincol_param.GC_AUTO_INDEX);
						result = tint->UpdateText(NULL, tint_gaincol_param.GC_RED_INDEX);
						result = tint->UpdateText(NULL, tint_gaincol_param.GC_GREEN_INDEX);
						result = tint->UpdateText(NULL, tint_gaincol_param.GC_BLUE_INDEX);
					}
					break;


			// ----- gain red : user
				case tint_gaincol_param.GC_RED_INDEX     : // <
				case tint_gaincol_param.GC_RED_INDEX + 1 : // o
				case tint_gaincol_param.GC_RED_INDEX + 2 : // >
					{
						result = tint->UpdateText(NULL, tint_gaincol_param.GC_RED_INDEX);
					}
					break;


			// ----- gain green : user
				case tint_gaincol_param.GC_GREEN_INDEX     : // <
				case tint_gaincol_param.GC_GREEN_INDEX + 1 : // o
				case tint_gaincol_param.GC_GREEN_INDEX + 2 : // >
					{
						result = tint->UpdateText(NULL, tint_gaincol_param.GC_GREEN_INDEX);
					}
					break;


			// ----- gain blue : user
				case tint_gaincol_param.GC_BLUE_INDEX     : // <
				case tint_gaincol_param.GC_BLUE_INDEX + 1 : // o
				case tint_gaincol_param.GC_BLUE_INDEX + 2 : // >
					{
						result = tint->UpdateText(NULL, tint_gaincol_param.GC_BLUE_INDEX);
					}
					break;


			// ---- save, reset & increment - note: AddText & AddValue determine GM text
				case tint_gaincol_param.GC_SRI_INDEX + 1:
					{
						result = tint->UpdateText(NULL, tint_gaincol_param.GC_AUTO_INDEX);
						result = tint->UpdateText(NULL, tint_gaincol_param.GC_LMH_INDEX);
						result = tint->UpdateText(NULL, tint_gaincol_param.GC_RED_INDEX);
						result = tint->UpdateText(NULL, tint_gaincol_param.GC_GREEN_INDEX);
						result = tint->UpdateText(NULL, tint_gaincol_param.GC_BLUE_INDEX);
					}
					break;


			default:
				break;
		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_GainCol_AddActions(
	const INT  handle_obj_config,
	const INT  element_index,
	FLOAT      action_values[]
)
{

	// ---- local
		using namespace vs_system;
		CamProps *cam_props = (CamProps *)handle_obj_config;
		BOOL edit_config    = cam_props->GetEditConfig();


	// ---- local
		INT num_actions = 0;


	// ---- ignore < o >
		switch ( tint_gaincol_param._update_element_index )
		{
			case tint_gaincol_param.GC_RED_INDEX       : // <
			case tint_gaincol_param.GC_RED_INDEX   + 1 : // o
			case tint_gaincol_param.GC_RED_INDEX   + 2 : // >

			case tint_gaincol_param.GC_GREEN_INDEX     : // <
			case tint_gaincol_param.GC_GREEN_INDEX + 1 : // o
			case tint_gaincol_param.GC_GREEN_INDEX + 2 : // >

			case tint_gaincol_param.GC_BLUE_INDEX      : // <
			case tint_gaincol_param.GC_BLUE_INDEX  + 1 : // o
			case tint_gaincol_param.GC_BLUE_INDEX  + 2 : // >
				return 0;

			default:
				break;
		}


	// ---- update ?
		switch ( element_index )
		{

			// ---- gain master : auto | lmh | user
				case tint_gaincol_param.GC_AUTO_INDEX : 
					{

						num_actions = 9;

						FLOAT action_value =
								edit_config ? 1.0f :
									cam_props->GetActionValue(
										tint_gaincol_param._update_element_index,
										CamProps::AUTO_WB,
										CamProps::GAIN_COLOUR_INDEX,
										CamProps::NUM_GAIN_COLOUR_CONFIGS,
										tint_gaincol_param.GC_USER_INDEX
									);

						FLOAT v[] =
							{
								(FLOAT)(tint_gaincol_param.GC_RED_INDEX      ), action_value,
								(FLOAT)(tint_gaincol_param.GC_RED_INDEX   + 1), action_value,
								(FLOAT)(tint_gaincol_param.GC_RED_INDEX   + 2), action_value,
								(FLOAT)(tint_gaincol_param.GC_GREEN_INDEX    ), action_value,
								(FLOAT)(tint_gaincol_param.GC_GREEN_INDEX + 1), action_value,
								(FLOAT)(tint_gaincol_param.GC_GREEN_INDEX + 2), action_value,
								(FLOAT)(tint_gaincol_param.GC_BLUE_INDEX     ), action_value,
								(FLOAT)(tint_gaincol_param.GC_BLUE_INDEX  + 1), action_value,
								(FLOAT)(tint_gaincol_param.GC_BLUE_INDEX  + 2), action_value,
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
INT  Model::Tint_GainCol_AddText(
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
		using namespace vs_system;
		CamProps *cam_props = (CamProps *)handle_obj_config;


	// ---- set
		switch ( element_index )
		{

			case tint_gaincol_param.GC_AUTO_INDEX:
				{
				
					// ---- gain master label

						INT gm_index = 0;
						result = cam_props->GetProperty(
								CamProps::SETUP,
								0,
								CamProps::GAIN_MASTER_INDEX,
								&gm_index
							);
							gm_index = gm_index == 0 ? 4 : gm_index;
						CHAR *gain_name[CamProps::NUM_GAIN_MASTER_CONFIGS] =
							{
								"NA",
								"( Gain Low )",
								"( Gain Medium )",
								"( Gain High )",
								"( Gain User )",
							};

					// ---- gain colour label
						INT gc_index = 0;
						result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::GAIN_COLOUR_INDEX, &gc_index);
						CHAR *wb_name[CamProps::NUM_GAIN_COLOUR_CONFIGS] =
							{
								"AUTO",
								"Sunshine",
								"Cloudy",
								"Overcast",
								"Bulb",
								"Fluorescent",
								"User",
							};

					// ---- append - only in non-AUTO mode
						CHAR all[128];
						sprintf(all, " %s %s", wb_name[gc_index], gc_index == 0 ? "" : gain_name[gm_index]);

					// ---- set
						*add_text = TRUE;
						strcpy(text, all);

				}
				break;

			case tint_gaincol_param.GC_RED_INDEX:
			case tint_gaincol_param.GC_GREEN_INDEX:
			case tint_gaincol_param.GC_BLUE_INDEX:
				{
					INT gc_index = 0;
					result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::GAIN_COLOUR_INDEX, &gc_index);
					if ( gc_index == 0 )
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
INT  Model::Tint_GainCol_AddValue(
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

			case tint_gaincol_param.GC_AUTO_INDEX:
				{
					;
				}
				break;

			case tint_gaincol_param.GC_RED_INDEX:
				{
					INT gc_index = 0;
					result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::GAIN_COLOUR_INDEX, &gc_index);
					if ( gc_index != 0 )
					{
						*add_value = TRUE;
						FLOAT gc_red = 0;
						result = cam_props->GetProperty(CamProps::GAIN_COLOUR, gc_index, CamProps::RED, &gc_red);
						*value = gc_red;
					}
				}
				break;

			case tint_gaincol_param.GC_GREEN_INDEX:
				{
					INT gc_index = 0;
					result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::GAIN_COLOUR_INDEX, &gc_index);
					if ( gc_index != 0 )
					{
						*add_value = TRUE;
						FLOAT gc_green = 0;
						result = cam_props->GetProperty(CamProps::GAIN_COLOUR, gc_index, CamProps::GREEN, &gc_green);
						*value = gc_green;
					}
				}
				break;

			case tint_gaincol_param.GC_BLUE_INDEX:
				{
					INT gc_index = 0;
					result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::GAIN_COLOUR_INDEX, &gc_index);
					if ( gc_index != 0 )
					{
						*add_value = TRUE;
						FLOAT gc_blue = 0;
						result = cam_props->GetProperty(CamProps::GAIN_COLOUR, gc_index, CamProps::BLUE, &gc_blue);
						*value = gc_blue;
					}
				}
				break;

			default:
				break;
		}

	return VS_OK;
}
