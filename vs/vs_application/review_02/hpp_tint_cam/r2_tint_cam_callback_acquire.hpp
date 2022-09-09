////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_cam_callback_acquire.hpp ----------
/*!
\file r2_tint_cam_callback_acquire.hpp
\brief
\author Gareth Edwards
*/


using namespace review_02;


// -------- Functionality and Parameters for Tabbed Interface Acquire

class Model::Tint_Acquire_Param
{

	public:

	// ---- cdtor
		Tint_Acquire_Param::Tint_Acquire_Param(){;};
		Tint_Acquire_Param::~Tint_Acquire_Param(){;};


	// --- props
		BOOL _large_incr = TRUE;
		INT _update_element_index = -1;


	// --- indices
		enum
		{
			EXP_AUTO_INDEX = 0,
			EXP_LMH_INDEX  = 1,
			EXP_USER_INDEX = 4,

			FRM_AUTO_INDEX = 7,
			FRM_LMH_INDEX  = 8,
			FRM_USER_INDEX = 11,

			ACQ_SRI_INDEX  = 15
		};


} tint_acquire_param;


// ----------------------------------------------------------------------
INT Model::Tint_Acquire_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_Acquire_BuildList;
	tint_callback->callback_get_icon_info  = Tint_Acquire_GetIconInfo;
	tint_callback->callback_update_model   = Tint_Acquire_UpdateModel;
	tint_callback->callback_update_element = Tint_Acquire_UpdateElement;
	tint_callback->callback_add_actions    = Tint_Acquire_AddActions;
	tint_callback->callback_add_text       = Tint_Acquire_AddText;
	tint_callback->callback_add_value      = Tint_Acquire_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Acquire_BuildList(
		vs_system::TintItem li[]
	)
{
	
	// ---- local
		FLOAT lincr  = 4;
		INT   result = 0;


	// ---- local for TINT_ macros
		UINT gid = ELEM_GROUP_ACQUIRE_LID;
		UINT noi = 0;


	// ---- build
		{

			// ---- scope tint def & enum
				using namespace vs_system;

			// ---- set
				TINT_SET_START_NOI()

			// auto
				TINT_ADD_ITEM("Exposure");
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_RADIO_LOCAL, 3, 6, 0, 0.0f, TRUE, lincr)

			// ---- low, medium & high
				TINT_ADD_ICON()
				TINT_SET_TARC(vs_system::Tint::ITEM_RADIO_LOCAL,    3, 4, 5)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO_LOCAL,   3, 4, 6, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_RADIO_LOCAL, 3, 4, 7, 2.2f, TRUE, lincr)

			// ---- user & <o>
				TINT_ADD_ICON()
				TINT_SET_TARC(vs_system::Tint::ITEM_RADIO_LOCAL,    3, 6, 4)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_CLICK,         3, 5, 0, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_CLICK,         3, 5, 1, 2.2f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_CLICK,       3, 5, 2, 3.3f, TRUE, lincr * 3)

			// ---- low, medium & high
				TINT_ADD_ITEM("Framerate")
				TINT_SET_TARC(vs_system::Tint::ITEM_RADIO_LOCAL,    3, 4, 5)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO_LOCAL,   3, 4, 6, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_RADIO_LOCAL, 3, 4, 7, 2.2f, TRUE, lincr)

			// ---- user & <o>
				TINT_ADD_ICON()
				TINT_SET_TARC(vs_system::Tint::ITEM_RADIO_LOCAL,    3, 6, 4)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_CLICK,         3, 5, 0, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_CLICK,         3, 5, 1, 2.2f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_CLICK,       3, 5, 2, 3.3f, TRUE, lincr * 3)

			// ---- save, reset & increment
				TINT_ADD_ITEM("Save")
				TINT_SET_TARC(vs_system::Tint::ITEM_CLICK,          3, 3, 3)
				TINT_ADD_ITEM("Reset")
				TINT_SET_TARCX(vs_system::Tint::ITEM_CLICK,         3, 3, 4, 1.5f)
				TINT_ADD_ITEM("Increment")
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_SWITCH,      3, 4, 3, 3.0f, TRUE, lincr)

			// ---- get 
				TINT_SET_TOTAL_NOI();

		}


	// ---- set by data
	

		// ---- to reference enum
			using namespace vs_system;


		// ---- cam props

			CamProps *cam_props = s_pi_model->cam_props;


			// ---- exposure - ALMHU
			INT exp_index = 0;
			result = cam_props->GetProperty(
					CamProps::SETUP,
					0,
					CamProps::EXPOSURE_INDEX,
					&exp_index
				);
			li[tint_acquire_param.EXP_AUTO_INDEX + exp_index].SetStatus(TRUE);


		// ---- framerate - ALMHU
			FLOAT frm_index = 0;
			result = cam_props->GetProperty(
					CamProps::SETUP,
					0,
					CamProps::FRAMERATE_INDEX,
					&frm_index
				);
			if ( frm_index < 1 )
			{
				frm_index = 1;
				result = cam_props->SetProperty(
						CamProps::SETUP,
						0,
						CamProps::FRAMERATE_INDEX,
						frm_index
					);
				}
			li[tint_acquire_param.FRM_AUTO_INDEX + (INT)frm_index].SetStatus(TRUE);


		// ---- framerate column LF
			li[tint_acquire_param.FRM_LMH_INDEX].SetColumnLF(TRUE);


		// ---- SRI column LF
			li[tint_acquire_param.ACQ_SRI_INDEX].SetColumnLF(TRUE);


		// ---- increment (large)
			li[tint_acquire_param.ACQ_SRI_INDEX+2].SetStatus(FALSE);


	return noi;
}


// ----------------------------------------------------------------------
INT Model::Tint_Acquire_GetIconInfo(INT id)
{

	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 3;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 3;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
		case vs_system::Tint::ICON_ATLAS_PAGE_ID : return 3;
		default:
			break;
	}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Acquire_UpdateModel(
		INT  handle_model,
		INT  element_index
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;


	// ---- to reference enum
		using namespace vs_system;


	// ---- cam props
		CamProps *cam_props = NULL;
		result = model->GetCamProps(&cam_props);
		BOOL edit_config = cam_props->GetEditConfig();


	// ---- get all callbacks - might as well...

		INT(*setup_callback)() = NULL;
		result = cam_props->GetSetupCallback(&setup_callback);

		INT(*exposure_callback)() = NULL;
		result = cam_props->GetExposureCallback(&exposure_callback);

		INT(*framerate_callback)() = NULL;
		result = cam_props->GetFramerateCallback(&framerate_callback);


	// ---- update ?
		switch ( element_index )
		{

			// ---- exposure : A
				case tint_acquire_param.EXP_AUTO_INDEX : // A
					{
						
						INT auto_exposure = 0;
						result = cam_props->GetProperty(
								CamProps::SETUP,
								0,
								CamProps::AUTO_EXPOSURE,
								&auto_exposure
							);
						auto_exposure = auto_exposure ? 0 : 1;
						result = cam_props->SetProperty(
								CamProps::SETUP,
								0,
								CamProps::AUTO_EXPOSURE,
								(FLOAT)auto_exposure
							);
						/*
						result = cam_props->SetProperty(
								CamProps::SETUP,
								0,
								CamProps::EXPOSURE_INDEX,
								0
							);
						*/
						result = setup_callback();
						result = exposure_callback();
					}
					break;


			// ---- exposure : LMHU
				case tint_acquire_param.EXP_LMH_INDEX     : // L
				case tint_acquire_param.EXP_LMH_INDEX + 1 : // M
				case tint_acquire_param.EXP_LMH_INDEX + 2 : // H
				case tint_acquire_param.EXP_USER_INDEX    : // U
					{
						/*
						result = cam_props->SetProperty(
								CamProps::SETUP,
								0,
								CamProps::AUTO_EXPOSURE,
								0
							);
						*/

						result = cam_props->SetProperty(
								CamProps::SETUP,
								0,
								CamProps::EXPOSURE_INDEX,
								(FLOAT)element_index - tint_acquire_param.EXP_AUTO_INDEX
							);

						result = setup_callback();
						result = exposure_callback();
					}
					break;


			// ---- exposure : < o >
				case tint_acquire_param.EXP_USER_INDEX + 1 : // <
				case tint_acquire_param.EXP_USER_INDEX + 2 : // o
				case tint_acquire_param.EXP_USER_INDEX + 3 : // >
					{

						INT incr = tint_acquire_param._large_incr ? 5 : 1;

						INT  exp_index = 0;
						if ( edit_config )
						{
							result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::EXPOSURE_INDEX, &exp_index);
						}
						else
						{
							exp_index = CamProps::NUM_EXPOSURE_CONFIGS - 1;
						}

						FLOAT exp_time_def = 0;
						result = cam_props->GetProperty(CamProps::EXPOSURE, exp_index, CamProps::TIME_EXP_DEF, &exp_time_def);

						FLOAT exp_time_max = 0;
						result = cam_props->GetProperty(CamProps::EXPOSURE, exp_index, CamProps::TIME_EXP_MAX, &exp_time_max);

						FLOAT exp_time_ms = 0;
						result = cam_props->GetProperty(CamProps::EXPOSURE, exp_index, CamProps::TIME_EXP_MS, &exp_time_ms);

						switch ( element_index )
						{
							case tint_acquire_param.EXP_USER_INDEX + 1: // <
								exp_time_ms = exp_time_ms - incr < 0 ? 1 : exp_time_ms - incr;
								break;
							case tint_acquire_param.EXP_USER_INDEX + 2: // o
								exp_time_ms = exp_time_def;
								break;
							case tint_acquire_param.EXP_USER_INDEX + 3: // >
								exp_time_ms = exp_time_ms + incr > exp_time_max ? exp_time_max : exp_time_ms + incr;
								break;
						}

						result = cam_props->SetProperty(CamProps::EXPOSURE, exp_index, CamProps::TIME_EXP_MS, exp_time_ms);
						result = exposure_callback();
					}
					break;


			// ----- framerate : A
				//
				// FRM_AUTO_INDEX not used at present
				//

			// ----- framerate : LMH
				case tint_acquire_param.FRM_LMH_INDEX     : // L
				case tint_acquire_param.FRM_LMH_INDEX + 1 : // M
				case tint_acquire_param.FRM_LMH_INDEX + 2 : // H
				case tint_acquire_param.FRM_USER_INDEX    : // U
					{
						result = cam_props->SetProperty(
								CamProps::SETUP,
								0,
								CamProps::FRAMERATE_INDEX,
								(FLOAT)element_index - tint_acquire_param.FRM_AUTO_INDEX
							);
						result = framerate_callback();
					}
					break;


			// ---- framerate : < o >
				case tint_acquire_param.FRM_USER_INDEX + 1 : // <
				case tint_acquire_param.FRM_USER_INDEX + 2 : // o
				case tint_acquire_param.FRM_USER_INDEX + 3 : // >
					{

						INT incr = tint_acquire_param._large_incr ? 3 : 1;

						INT  frm_index = 0;
						if ( edit_config )
						{
							result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::FRAMERATE_INDEX, &frm_index);
						}
						else
						{
							frm_index = CamProps::NUM_FRAMERATE_CONFIGS - 1;
						}

						FLOAT fps = 0;
						result = cam_props->GetProperty(CamProps::FRAMERATE, frm_index, CamProps::FPS, &fps);

						FLOAT fps_def = 0;
						result = cam_props->GetProperty(CamProps::FRAMERATE, frm_index, CamProps::FPS_DEF, &fps_def);

						FLOAT fps_max = 0;
						result = cam_props->GetProperty(CamProps::FRAMERATE, frm_index, CamProps::FPS_MAX, &fps_max);

						switch ( element_index )
						{
							case tint_acquire_param.FRM_USER_INDEX + 1: // <
								fps = fps - incr < 1 ? 1 : fps - incr;
								break;
							case tint_acquire_param.FRM_USER_INDEX + 2: // o
								fps = fps_def;
								break;
							case tint_acquire_param.FRM_USER_INDEX + 3: // >
								fps = fps + incr > fps_max ? fps_max : fps + incr;
								break;
						}

						result = cam_props->SetProperty(CamProps::FRAMERATE, frm_index, CamProps::FPS, fps);
						result = framerate_callback();
					}
					break;


			// ---- save
				case tint_acquire_param.ACQ_SRI_INDEX:
					result = model->SaveCamProps();
					break;


			// ---- reset
				case tint_acquire_param.ACQ_SRI_INDEX + 1:
					{
						// ---- reset and then update
							cam_props->ResetProperty(CamProps::EXPOSURE);
							cam_props->ResetProperty(CamProps::FRAMERATE);
							Tint *tint = model->pi_model->tint_cam_setng;
							tint->UpdateAllText();
							result = exposure_callback();
							result = framerate_callback();
					}
					break;
			

			// ---- increment
				case tint_acquire_param.ACQ_SRI_INDEX + 2:
						tint_acquire_param._large_incr =
							tint_acquire_param._large_incr ? FALSE : TRUE;
					break;

			default:
				OutputDebugString("--- huh: Tint_Acquire_UpdateModel\n");
				break;
		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Acquire_UpdateElement(
		INT handle_model,
		INT element_index
	)
{
	 
	// ---- store for "add" callbacks
		tint_acquire_param._update_element_index = element_index;


	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		PI_Model *pimp = model->pi_model;
		vs_system::Tint *tint = pimp->tint_cam_setng;


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_HEAD_ACQUIRE_LID);


	// ---- update ?
		switch ( element_index )
		{

			// ---- exposure : ALMHU
				case tint_acquire_param.EXP_AUTO_INDEX     : // A
				case tint_acquire_param.EXP_LMH_INDEX      : // L
				case tint_acquire_param.EXP_LMH_INDEX + 1  : // M
				case tint_acquire_param.EXP_LMH_INDEX + 2  : // H
				case tint_acquire_param.EXP_USER_INDEX     : // U
				case tint_acquire_param.EXP_USER_INDEX + 1 : // <
				case tint_acquire_param.EXP_USER_INDEX + 2 : // o
				case tint_acquire_param.EXP_USER_INDEX + 3 : // >
					{
						result = tint->UpdateText(NULL, tint_acquire_param.EXP_AUTO_INDEX);
					}
					break;


			// ----- framerate : A
				//
				// FRM_AUTO_INDEX not used at present
				//
			// ----- framerate : LMHU
				case tint_acquire_param.FRM_LMH_INDEX      : // L
				case tint_acquire_param.FRM_LMH_INDEX + 1  : // M
				case tint_acquire_param.FRM_LMH_INDEX + 2  : // H
				case tint_acquire_param.FRM_USER_INDEX     : // U
				case tint_acquire_param.FRM_USER_INDEX + 1 : // <
				case tint_acquire_param.FRM_USER_INDEX + 2 : // o
				case tint_acquire_param.FRM_USER_INDEX + 3 : // >
					{
						result = tint->UpdateText(NULL, tint_acquire_param.FRM_LMH_INDEX);
					}
					break;


			// ---- save, reset & increment - note: AddText & AddValue determine EXP text
				case tint_acquire_param.ACQ_SRI_INDEX + 1:
					{
						result = tint->UpdateText(NULL, tint_acquire_param.EXP_AUTO_INDEX);
						result = tint->UpdateText(NULL, tint_acquire_param.EXP_LMH_INDEX);
						result = tint->UpdateText(NULL, tint_acquire_param.FRM_LMH_INDEX);
					}
					break;

			default:
				break;
		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Acquire_AddActions(
		const INT  handle_obj_config,
		const INT  element_index,
		FLOAT      action_values[]
	)
{ 

	// ---- for enum
		using namespace vs_system;


	// ---- local
		CamProps *cam_props = (CamProps *)handle_obj_config;
		BOOL edit_config = cam_props->GetEditConfig();


	// ---- local
		INT num_actions = 0;


	// ---- update ?
		switch ( element_index )
		{

			// ---- exposure : ALMHU
				case tint_acquire_param.EXP_AUTO_INDEX : 
					{

						num_actions = 3;

						FLOAT action_value =
								edit_config ? 1.0f :
									cam_props->GetActionValue(
											tint_acquire_param._update_element_index,
											CamProps::AUTO_EXPOSURE,
											CamProps::EXPOSURE_INDEX,
											CamProps::NUM_EXPOSURE_CONFIGS,
											tint_acquire_param.EXP_USER_INDEX
										);

						FLOAT v[] =
							{
								(FLOAT)(tint_acquire_param.EXP_USER_INDEX + 1), action_value,
								(FLOAT)(tint_acquire_param.EXP_USER_INDEX + 2), action_value,
								(FLOAT)(tint_acquire_param.EXP_USER_INDEX + 3), action_value
							};

						for (INT i = 0; i < num_actions*2; i++) action_values[i] = v[i];
						
					}
					break;


			// ----- framerate : LMHU
				case tint_acquire_param.FRM_LMH_INDEX:
					{

						num_actions = 3;

						FLOAT action_value =
								edit_config ? 1.0f :
									cam_props->GetActionValue(
											tint_acquire_param._update_element_index,
											0,
											CamProps::FRAMERATE_INDEX,
											CamProps::NUM_FRAMERATE_CONFIGS,
											tint_acquire_param.FRM_USER_INDEX
										);

						FLOAT v[] =
							{
								(FLOAT)(tint_acquire_param.FRM_USER_INDEX + 1), action_value,
								(FLOAT)(tint_acquire_param.FRM_USER_INDEX + 2), action_value,
								(FLOAT)(tint_acquire_param.FRM_USER_INDEX + 3), action_value
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
INT  Model::Tint_Acquire_AddText(
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

			case tint_acquire_param.EXP_AUTO_INDEX:
				{

					INT auto_exposure = 0;
					result = cam_props->GetProperty(
							vs_system::CamProps::SETUP,
							0,
							vs_system::CamProps::AUTO_EXPOSURE,
							&auto_exposure
						);

					if ( auto_exposure )
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
INT  Model::Tint_Acquire_AddValue(
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


	// ---- for enum
		using namespace vs_system;


	// ---- cast to get camera properties object
		CamProps *cam_props = (vs_system::CamProps *)handle_obj_config;


	// ---- select
		switch ( element_index )
		{

			case tint_acquire_param.EXP_AUTO_INDEX:
				{

					INT auto_exposure = 0;
					result = cam_props->GetProperty(
							CamProps::SETUP,
							0,
							CamProps::AUTO_EXPOSURE,
							&auto_exposure
						);

					if ( ! auto_exposure )
					{
						*add_value = TRUE;
						INT exp_index = 0;
						result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::EXPOSURE_INDEX, &exp_index);
						FLOAT time_ms = 0;
						result = cam_props->GetProperty(CamProps::EXPOSURE, exp_index, CamProps::TIME_EXP_MS, &time_ms);
						*value = time_ms;
						strcpy(suffix, " MS");
					}

				}
				break;

			case tint_acquire_param.FRM_LMH_INDEX:
				{
					*add_value = TRUE;
					INT frm_index = 0;
					result = cam_props->GetProperty(CamProps::SETUP, 0, CamProps::FRAMERATE_INDEX, &frm_index);
					FLOAT fps = 0;
					result = cam_props->GetProperty(CamProps::FRAMERATE, frm_index, CamProps::FPS, &fps);
					*value = fps;
					strcpy(suffix, " FPS");
				}
				break;

			default:
				break;
		}

	return VS_OK;
}
