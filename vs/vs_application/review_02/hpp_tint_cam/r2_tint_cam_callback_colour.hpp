////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_cam_callback_colour.hpp ----------
/*!
\file r2_tint_cam_callback_colour.hpp
\brief
\author Gareth Edwards
*/


using namespace review_02;


// -------- Functionality and Parameters for Tabbed Interface Acquire

class Model::Tint_Colour_Param
{

public:

	// ---- cdtor
		Tint_Colour_Param::Tint_Colour_Param() { ; };
		Tint_Colour_Param::~Tint_Colour_Param() { ; };


	// --- props
		BOOL _large_incr = TRUE;
		INT  _update_element_index = -1;


	// --- indices
	enum
	{
		SAT_LMH_INDEX  = 0,
		SAT_USER_INDEX = 3,
		SAT_SRI_INDEX  = 7
	};


} tint_colour_param;


// ----------------------------------------------------------------------
INT Model::Tint_Colour_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_Colour_BuildList;
	tint_callback->callback_get_icon_info  = Tint_Colour_GetIconInfo;
	tint_callback->callback_update_model   = Tint_Colour_UpdateModel;
	tint_callback->callback_update_element = Tint_Colour_UpdateElement;
	tint_callback->callback_add_actions    = Tint_Colour_AddActions;
	tint_callback->callback_add_text       = Tint_Colour_AddText;
	tint_callback->callback_add_value      = Tint_Colour_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Colour_BuildList(
		vs_system::TintItem li[]
	)
{

	// ---- local
		FLOAT lincr  = 4;
		INT   result = 0;


	// ---- local for TINT_ macros
		UINT gid = ELEM_GROUP_COLOUR_LID;
		UINT noi = 0;


	// ---- build
		{

			// ---- scope tint def & enum
				using namespace vs_system;

			// ---- set
				TINT_SET_START_NOI()

			// ---- LMH
				TINT_ADD_ITEM("Saturation")
				TINT_SET_TARC(Tint::ITEM_RADIO_LOCAL,    3, 4, 5)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   3, 4, 6, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_RADIO_LOCAL, 3, 4, 7, 2.2f, TRUE, lincr)

			// ---- U <o>
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO_LOCAL,    3, 6, 4)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_CLICK,         3, 5, 0, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_CLICK,         3, 5, 1, 2.2f)
				TINT_ADD_ICON();
				TINT_SET_TARCXLI(Tint::ITEM_CLICK,       3, 5, 2, 3.3f, TRUE, lincr * 3)

			// ---- save, reset & increment
				TINT_ADD_ITEM("Save")
				TINT_SET_TARC(Tint::ITEM_CLICK,          3, 3, 3)
				TINT_ADD_ITEM("Reset")
				TINT_SET_TARCX(Tint::ITEM_CLICK,         3, 3, 4, 1.5f)
				TINT_ADD_ITEM("Increment")
				TINT_SET_TARCXLI(Tint::ITEM_SWITCH,      3, 4, 3, 3.0f, TRUE, lincr)

			// ---- get 
				TINT_SET_TOTAL_NOI()

		}


	// ---- set by data
		

		// ---- cam props
			using namespace vs_system;
			CamProps *cam_props = s_pi_model->cam_props;


		// ---- get setup indices
			FLOAT col_index = 0;
			result = cam_props->GetProperty(
					CamProps::SETUP,
					0,
					CamProps::COLOUR_INDEX,
					&col_index
				);
			if ( col_index < 1 )
			{
				col_index = 1;
				result = cam_props->SetProperty(
					CamProps::SETUP,
					0,
					CamProps::COLOUR_INDEX,
					col_index
				);
			}


		// ---- saturation - lmh & user
			li[tint_colour_param.SAT_LMH_INDEX + (INT)col_index - 1].SetStatus(TRUE);


		// ---- SRI column LF
			li[tint_colour_param.SAT_SRI_INDEX].SetColumnLF(TRUE);


		// ---- increment (large)
			li[tint_colour_param.SAT_SRI_INDEX + 2].SetStatus(FALSE);


	return noi;
}


// ----------------------------------------------------------------------
INT Model::Tint_Colour_GetIconInfo(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 3;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 6;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
		case vs_system::Tint::ICON_ATLAS_PAGE_ID : return 3;
		default:
			break;
	}
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Colour_UpdateModel(
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


	// ---- get callbacks - might as well...
		INT(*colour_callback)() = NULL;
		result = cam_props->GetColourCallback(&colour_callback);


	// ---- update ?
		switch ( element_index )
		{

			// ----- saturation : lmh & user
				case tint_colour_param.SAT_LMH_INDEX     : // L
				case tint_colour_param.SAT_LMH_INDEX + 1 : // M
				case tint_colour_param.SAT_LMH_INDEX + 2 : // H
				case tint_colour_param.SAT_USER_INDEX    : // U
					{
					FLOAT new_index = (FLOAT)element_index - tint_colour_param.SAT_LMH_INDEX + 1;
						result = cam_props->SetProperty(
								CamProps::SETUP,
								0,
								CamProps::COLOUR_INDEX,
							new_index
							);

						result = colour_callback();
					}
					break;


			// ---- saturation : < o >
				case tint_colour_param.SAT_USER_INDEX + 1 : // <
				case tint_colour_param.SAT_USER_INDEX + 2 : // o
				case tint_colour_param.SAT_USER_INDEX + 3 : // >
					{

						FLOAT incr = tint_colour_param._large_incr ? 0.1f : 0.01f;

						INT col_index = 0;
						if ( edit_config )
						{
							result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::COLOUR_INDEX, &col_index);
						}
						else
						{
							col_index = CamProps::NUM_COLOUR_CONFIGS - 1;
						}

						FLOAT sat = 0;
						result = cam_props->GetProperty(CamProps::COLOUR, col_index, CamProps::SATURATION, &sat);

						switch ( element_index )
						{
							case tint_colour_param.SAT_USER_INDEX + 1: // <
								sat = sat - incr < 0 ? 0 : sat - incr;
								break;
							case tint_colour_param.SAT_USER_INDEX + 2: // o
								sat = 1;
								break;
							case tint_colour_param.SAT_USER_INDEX + 3: // >
								sat = sat + incr > 2 ? 2 : sat + incr;
								break;
						}

						result = cam_props->SetProperty(CamProps::COLOUR, col_index, CamProps::SATURATION, sat);
						result = colour_callback();
					}
					break;


			// ---- save
				case tint_colour_param.SAT_SRI_INDEX:
					result = model->SaveCamProps();
					break;


			// ---- reset
				case tint_colour_param.SAT_SRI_INDEX + 1:
					{   
						// ---- reset and then update
							cam_props->ResetProperty(CamProps::COLOUR);
							Tint *tint = model->pi_model->tint_cam_setng;
							tint->UpdateAllText();
					}
					break;
			

			// ---- increment
				case tint_colour_param.SAT_SRI_INDEX + 2:
					tint_colour_param._large_incr =
						tint_colour_param._large_incr ? FALSE : TRUE;
					break;

			default:
				OutputDebugString("--- huh: Tint_Colour_UpdateModel\n");
				break;
		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Colour_UpdateElement(
		INT handle_model,
		INT element_index
	)
{
	
	// ---- store for "add" callbacks
		tint_colour_param._update_element_index = element_index;


	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_cam_setng;


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_HEAD_COLOUR_LID);


	// ---- update ?
		switch ( element_index )
		{

			// ----- saturation : lmh & user
				case tint_colour_param.SAT_LMH_INDEX     : // L
				case tint_colour_param.SAT_LMH_INDEX + 1 : // M
				case tint_colour_param.SAT_LMH_INDEX + 2 : // H
				case tint_colour_param.SAT_USER_INDEX    : // U
				case tint_colour_param.SAT_USER_INDEX + 1: // <
				case tint_colour_param.SAT_USER_INDEX + 2: // o
				case tint_colour_param.SAT_USER_INDEX + 3: // >
					{
						result = tint->UpdateText(NULL, tint_colour_param.SAT_LMH_INDEX);
					}
					break;


			// ---- save, reset & increment - note: AddText & AddValue determine EXP text
				case tint_colour_param.SAT_SRI_INDEX + 1:
					{
						result = tint->UpdateText(NULL, tint_colour_param.SAT_LMH_INDEX);

					}
					break;

			default:
				break;
		}
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Colour_AddActions(
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
		switch (tint_colour_param._update_element_index)
		{
				case tint_colour_param.SAT_USER_INDEX + 1: // <
				case tint_colour_param.SAT_USER_INDEX + 2: // o
				case tint_colour_param.SAT_USER_INDEX + 3: // >
				return 0;

			default:
				break;
		}


	// ---- update ?
		switch ( element_index )
		{

			// ----- saturation : lmh & user
				case tint_colour_param.SAT_LMH_INDEX :
					{

						num_actions = 3;

						FLOAT action_value =
								edit_config ? 1.0f :
									cam_props->GetActionValue(
											tint_acquire_param._update_element_index,
											0,
											CamProps::COLOUR_INDEX,
											CamProps::NUM_COLOUR_CONFIGS,
											tint_colour_param.SAT_USER_INDEX
										);

						FLOAT v[] =
							{
								(FLOAT)(tint_colour_param.SAT_USER_INDEX + 1), action_value,
								(FLOAT)(tint_colour_param.SAT_USER_INDEX + 2), action_value,
								(FLOAT)(tint_colour_param.SAT_USER_INDEX + 3), action_value
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
INT  Model::Tint_Colour_AddText(
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
INT  Model::Tint_Colour_AddValue(
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

			case tint_colour_param.SAT_LMH_INDEX:
				{
					*add_value = TRUE;
					INT col_index = 0;
					result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::COLOUR_INDEX, &col_index);
					FLOAT sat = 0;
					result = cam_props->GetProperty(CamProps::COLOUR, col_index, CamProps::SATURATION, &sat);
					*value = (FLOAT)sat * 100;
					sprintf(suffix, " %%");
				}
				break;

			default:
				break;
		}
	return VS_OK;
}
