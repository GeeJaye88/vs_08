////////////////////////////////////////////////////////////////////////////////

// ---------- s1_tint_callback_camera.hpp ----------
/*!
\file s1_tint_callback_camera.hpp
\brief
\author Gareth Edwards
*/

using namespace surface_01;

void ouput_int(CHAR *s, INT i)
{
	char msg[64];
	sprintf(msg, "%s %d \n", s, i);
	OutputDebugString(msg);
}

// -------- Functionality and Parameters for Tabbed Interface Acquire

class Model::Tint_Explore_Param
{

public:


	// ---- cdtor
		Tint_Explore_Param::Tint_Explore_Param() { ; };
		Tint_Explore_Param::~Tint_Explore_Param() { ; };


	// ---- methods
		INT Tint_Explore_Param::GetElementDsplParam(INT handle_model)
		{

			// ---- on time init
				if ( element_dspl_count == 0 )
				{
					
					// ---- tmp
						UINT hash_element_lid_to_dspl_index[vs_system::Tint::MAX_BODY_ELM];

					// ---- local
						INT result = VS_OK;
						Model *model = (Model *)handle_model;
						vs_system::Tint *tint = model->pi_model->tint_setng;

					// ---- init
						 element_dspl_count = tint->GetBodyElemDsplInfo(
								(UINT)TINT_BODY_EXPLORE_LID,	// [in]
								vs_system::Tint::MAX_BODY_ITEM, // [in]
								element_item_lid_char,          // [in]
								group_lid_char,                 // [in]
								element_dspl_info,              // [out]
								hash_element_lid_to_dspl_index  // [out]
							);
				}

			return VS_OK;
		}


	// ---- properties


	// ---- element display list (see Tint_Explore_Param::GetElementDsplParam asbove)
		INT  element_dspl_count = 0;
		vs_system::TintElemDsplInfo element_dspl_info[vs_system::Tint::MAX_BODY_ELM];


	// ---- item element logical id's
		CHAR element_item_lid_char[vs_system::Tint::MAX_BODY_ITEM] =
		{
			'd',      'm',
			'e', 'f', 'n', 'o', 'p',
			'u', 'v', 'w', 'x', 'y',
			'0', '1', '2', '3', '4', '5',
			'6', '7', '8', '9', ':', ';',
				 'W',      'R',
			'A', 'Q', 'D', 'E',
				 'S',      'F',
			'J', 'K', 'L', 'X', 'I'
		};

	// ---- item element logical id descriptors
		enum
		{
			ELEMENT_LID_STANDARD      = 'e',
			ELEMENT_LID_GRAPHIC       = 'f',
			ELEMENT_LID_LOW           = 'n',
			ELEMENT_LID_MEDIUM        = 'o',
			ELEMENT_LID_HIGH          = 'p',
			ELEMENT_LID_UPDATE_FIRST  = 'u',
			ELEMENT_LID_UPDATE_LAST   = 'y',
			ELEMENT_LID_TARGET_FIRST  = '0',
			ELEMENT_LID_TARGET_LAST   = ';',
			ELEMENT_LID_ANIMATE_FIRST = 'J',
			ELEMENT_LID_RESET         = 'X',
			ELEMENT_LID_SPEED         = 'I',
			ELEMENT_LID_LIGHT_TEXT    = 'd',
			ELEMENT_LID_VIEW_TEXT     = 'm',
			ELEMENT_LID_CONTROL_TEXT  = 'W',
		};


	// ---- items group logical id's
		CHAR group_lid_char[vs_system::Tint::MAX_BODY_ITEM] =
		{
			'D',      'M',
			'D', 'D', 'M', 'M', 'M',
			'U', 'U', 'U', 'U', 'U',
			'L', 'L', 'L', 'L', 'L', 'L',
			'L', 'L', 'L', 'L', 'L', 'L',
				 'C',      'C',
			'C', 'C', 'C', 'C',
				 'C',      'C',
			'C', 'C', 'C', 'S', 'S'
		};


	// ---- item group logical id descriptors
		enum
		{
			GROUP_LID_LIGHT             = 'D',
			GROUP_LID_VIEW              = 'M',
			GROUP_LID_CAMERA_TRANSFORM  = 'U',
			GROUP_LID_CAMERA_TARGET     = 'L',
			GROUP_LID_CAMERA_CONTROL    = 'C',
			GROUP_LID_ANIMATE           = 'A',
			GROUP_LID_SET               = 'S'
		};


	// --- control speed
		BOOL high_speed = TRUE;


		CHAR *light_text[3] =
			{
				"Star Light",
				"Gfx Light",
			};

		CHAR *view_text[3] =
			{
				"Standard View",
				"Large Object View",
				"Gfx Object View"
			};

		CHAR *transform_text[6] = 
			{
				"View aligned translation",
				"View direction translation",
				"View pan, tilt and zoom",
				"Global translation",
				"Global rotation",
				"Pan, tilt and zoom about target"
			};

		CHAR *control_text[6][2] =
			{
				{ "Z & X axis       Y axis",   "" },
				{ "Fly & Strafe     Climb",    "" },
				{ "Zoom & Pan    Tilt",        "" },
				{ "X & Z axis       Y axis",   "" },
				{ "Y & X axis       Z axis",   "" },
				{ "Zoom & Pan    Tilt",        "" }
			};

} tint_explore_param;


// ----------------------------------------------------------------------
INT Model::Tint_Explore_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_Explore_BuildList;
	tint_callback->callback_get_icon_info  = Tint_Explore_GetIconInfo;
	tint_callback->callback_update_model   = Tint_Explore_UpdateModel;
	tint_callback->callback_update_element = Tint_Explore_UpdateElement;
	tint_callback->callback_add_actions    = Tint_Explore_AddActions;
	tint_callback->callback_add_text       = Tint_Explore_AddText;
	tint_callback->callback_add_value      = Tint_Explore_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Explore_BuildList(
		vs_system::TintItem li[]
	)
{

	// ---- local
		FLOAT lincr = 10;
		INT   result = 0;
		INT   noi = 0;
		UINT  gid = TINT_BODY_EXPLORE_LID;


	// ---- build
		{

			// ---- scope tint def & enum
				using namespace vs_system;


			// ---- set
				TINT_SET_START_NOI()


			// ---- display & mode text
				TINT_ADD_ITEM("Light")
				TINT_SET_TARCX(Tint::ITEM_TEXT,          0, 0, 0, 0.0f)
				TINT_ADD_ITEM("Display")
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,        0, 0, 0, 3.0f, TRUE, lincr/8)


			// ---- display
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 4, 3, 0.0f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 4, 4, 1.1f)


			// ---- density
				gid = TINT_BODY_EXPLORE_LID_GROUP_1;
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 2, 5, 3.0f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 2, 6, 4.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_RADIO_LOCAL, 4, 2, 7, 5.2f, TRUE, lincr)


			// ---- select camera mode
				gid = TINT_BODY_EXPLORE_LID_GROUP_2;
				TINT_ADD_ITEM("Update")
				TINT_SET_TARC(Tint::ITEM_RADIO_LOCAL,    4, 5, 3)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 5, 4, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 5, 5, 2.2f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 5, 6, 3.3f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_RADIO_LOCAL, 4, 5, 7, 4.4f, TRUE, lincr)


			// ---- targets
					gid = TINT_BODY_EXPLORE_LID_GROUP_1;

				// ---- row 1
					TINT_ADD_ITEM("Target Objects")
					TINT_SET_TARC(Tint::ITEM_RADIO_LOCAL,    4, 6, 0)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 6, 1, 1.1f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 6, 2, 2.2f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 6, 3, 3.3f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 6, 4, 4.5f)
					TINT_ADD_ICON()
					TINT_SET_TARCXLI(Tint::ITEM_RADIO_LOCAL, 4, 6, 5, 5.6f, TRUE, lincr / 8)

				// ---- row 2
					TINT_ADD_ICON()
					TINT_SET_TARC(Tint::ITEM_RADIO_LOCAL,    4, 6, 6)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 6, 7, 1.1f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 7, 0, 2.2f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 7, 1, 3.3f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 7, 2, 4.4f)
					TINT_ADD_ICON()
					TINT_SET_TARCXLI(Tint::ITEM_RADIO_LOCAL, 4, 7, 3, 5.5f, TRUE, lincr)


			// ---- control

				// control _^_ - ^
					TINT_ADD_ITEM("Control")
					TINT_SET_TARCX(Tint::ITEM_CLICK,         4, 2, 1, 1.1f)
					TINT_ADD_ITEM(" ")
					TINT_SET_TARCXLI(Tint::ITEM_CLICK,       4, 2, 1, 4.4f, TRUE, lincr / 8)

				// control <o> - o
					TINT_ADD_ICON()
					TINT_SET_TARC(Tint::ITEM_CLICK,          4, 3, 0)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_SWITCH,        4, 3, 7, 1.1f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_CLICK,         4, 3, 2, 2.2f)

					TINT_ADD_ICON()
					TINT_SET_TARCXLI(Tint::ITEM_SWITCH,      4, 3, 7, 4.4f, TRUE, lincr / 8)

				// control _v_ - v 
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_CLICK,         4, 4, 1, 1.1f)
					TINT_ADD_ICON()
					TINT_SET_TARCXLI(Tint::ITEM_CLICK,       4, 4, 1, 4.4f, TRUE, lincr)

				// ---- animate
					TINT_ADD_ITEM("Animate")
					TINT_SET_TARC(Tint::ITEM_CLICK,          4, 0, 5)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_CLICK,         4, 0, 6, 1.1f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_CLICK,         4, 0, 7, 2.2f)


			// ---- set
				TINT_ADD_ITEM("Reset")
				TINT_SET_TARCX(Tint::ITEM_CLICK,         5, 5, 6, 4.0f) 
				TINT_ADD_ITEM("Speed")
				TINT_SET_TARCXLI(Tint::ITEM_SWITCH,      5, 5, 5, 5.5f, TRUE, lincr)


			// ---- get 
				TINT_SET_TOTAL_NOI();

		}


		// ---- set item element id's
			for (INT i = 0; i < noi; i++)
			{
				li[i].elem_id = (UINT)tint_explore_param.element_item_lid_char[i];
			}


		// ---- const
			UINT EXPLORE_TEXT_INDEX         = 0;    //  2 items
			UINT EXPLORE_LIGHT_INDEX        = 2;    //  2 items
			UINT EXPLORE_DISPLAY_INDEX      = 4;    //  3   "
			UINT EXPLORE_MOVE_INDEX         = 7;    //  5   "
			UINT EXPLORE_TARGET_INDEX       = 12;   // 12   "
			UINT EXPLORE_CONTROL_INDEX      = 24;   //  8   "
			UINT EXPLORE_ANIMATE_INDEX      = 32;   //  3   "
			UINT EXPLORE_RS_INDEX           = 35;   //  2   "


		// ---- system light
			{
				UINT slm = s_pi_model->system_param->GetSystemLightMode();
				switch (slm)
				{
					case SystemParam::SystemLightMode::NORMAL_COLOUR  :
					case SystemParam::SystemLightMode::GRAPHIC_COLOUR :
						li[EXPLORE_LIGHT_INDEX + slm].SetStatus(TRUE);
						break;
					default:
						{
							s_pi_model->system_param->SetSystemLightMode(SystemParam::SystemLightMode::GRAPHIC_COLOUR);
							li[EXPLORE_DISPLAY_INDEX + SystemParam::SystemLightMode::GRAPHIC_COLOUR].SetStatus(TRUE);
						}
						break;
				}
			}


		// ---- system view
			{
				UINT svm = s_pi_model->system_param->GetSystemViewMode();
				switch (svm)
				{
					case SystemParam::SystemViewMode::STANDARD_SIZE :
					case SystemParam::SystemViewMode::LARGE_SIZE    :
					case SystemParam::SystemViewMode::GRAPHIC_SIZE  :
						li[EXPLORE_DISPLAY_INDEX + svm].SetStatus(TRUE);
						break;
					default:
						{
							s_pi_model->system_param->SetSystemViewMode(SystemParam::SystemViewMode::GRAPHIC_SIZE);
							li[EXPLORE_DISPLAY_INDEX + SystemParam::SystemViewMode::GRAPHIC_SIZE].SetStatus(TRUE);
						}
						break;
				}
			}


		// ---- set camera mode
			UINT ccm = s_pi_model->GetCameraControlMode();
			{
				using namespace vs_system;
				switch ( ccm )
				{
					case AppKamera::Mode::XYZ       :
					case AppKamera::Mode::VECTOR    :
					case AppKamera::Mode::PTZ       :
					case AppKamera::Mode::TRANSLATE :
					case AppKamera::Mode::ROTATE    :
						li[EXPLORE_MOVE_INDEX + ccm].SetStatus(TRUE);
						s_pi_model->app_kamera[s_pi_model->app_kamera_index]->SetTargetStatus(FALSE);
						break;

					case AppKamera::Mode::TARGET:
						{
							UINT target_index = EXPLORE_TARGET_INDEX + s_pi_model->GetTargetObjectId();
							target_index = 
									target_index >= EXPLORE_TARGET_INDEX ? 
										( target_index < EXPLORE_CONTROL_INDEX ? target_index : EXPLORE_CONTROL_INDEX - 1 ) :
											target_index;
							li[target_index].SetStatus(TRUE);
						}
						break;

					default:
						break;
				}
			}

		// ---- set camera speed
			li[EXPLORE_RS_INDEX + 1].SetStatus(tint_explore_param.high_speed);

	return noi;
}


// ----------------------------------------------------------------------
INT Model::Tint_Explore_GetIconInfo(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 4;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 0;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
		default:
			break;
	}
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Explore_UpdateModel(
		INT  handle_model,
		INT  element_id
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- get tint display list element info
		result = tint_explore_param.GetElementDsplParam(handle_model);


	// ---- process
		UINT number_of_element_ids = sizeof(tint_explore_param.element_item_lid_char) / sizeof(CHAR);
		for (UINT i = 0; i < number_of_element_ids; i++)
		{

			if ( tint_explore_param.element_item_lid_char[i] == (CHAR)element_id )
			{

				CHAR group = tint_explore_param.group_lid_char[i];
				switch ( group ) 
				{

					case Tint_Explore_Param::GROUP_LID_LIGHT:
						{

							switch ( element_id )
							{

								case Tint_Explore_Param::ELEMENT_LID_STANDARD:
								case Tint_Explore_Param::ELEMENT_LID_GRAPHIC:
									{
										UINT slm = element_id - Tint_Explore_Param::ELEMENT_LID_STANDARD;
										s_pi_model->system_param->SetSystemLightMode(slm);
										#if defined(S1_EXPLODED_CUBE)
										INT result = model->InitExplodedCubeObject();
										#elif defined(S1_WAVEFRONT_OBJ)
										INT result = model->InitWavefrontObject();
										#else
										INT result = model->InitStarSystemObjects();
										#endif
									}
									return VS_ELM_SETUP_NONE;

								default:
									break;
							}

						}
						break;

					case Tint_Explore_Param::GROUP_LID_VIEW:
						{
							switch ( element_id )
							{
								case Tint_Explore_Param::ELEMENT_LID_LOW:
								case Tint_Explore_Param::ELEMENT_LID_MEDIUM:
								case Tint_Explore_Param::ELEMENT_LID_HIGH:
									{

										// ---- set view mode changed
											s_pi_model->system_param->SetSystemViewModeChanged(TRUE);
										
										// ---- set view mode
											UINT svm = element_id - Tint_Explore_Param::ELEMENT_LID_LOW;
											s_pi_model->system_param->SetSystemViewMode(svm);
											#if defined(S1_EXPLODED_CUBE)
											INT result = model->InitExplodedCubeObject();
											#elif defined(S1_WAVEFRONT_OBJ)
											INT result = model->InitWavefrontObject();
											#else
											INT result = model->InitStarSystemObjects();
											#endif
									}
									break;
							}
						}
						break;

					case Tint_Explore_Param::GROUP_LID_CAMERA_TRANSFORM:
						{

							// ---- zap target
								model->Tint_Explore_Switch(handle_model,
										Tint_Explore_Param::GROUP_LID_CAMERA_TARGET,
										0,
										FALSE
									);

							// ---- zap target #
								s_pi_model->SetTargetObjectId(CameraTarget::NONE);

							// ---- zap camera target
								vs_system::AppKamera *kamera = s_pi_model->app_kamera[s_pi_model->app_kamera_index];
								kamera->SetTargetStatus(FALSE);

							// ---- set camera control mode
								UINT ccm = element_id - Tint_Explore_Param::ELEMENT_LID_UPDATE_FIRST;
								s_pi_model->SetCameraControlMode(ccm);

						}
						return VS_ELM_SETUP_NONE;


					case Tint_Explore_Param::GROUP_LID_CAMERA_TARGET:
						{

							// ---- zap target
								model->Tint_Explore_Switch(handle_model,
										Tint_Explore_Param::GROUP_LID_CAMERA_TRANSFORM,
										0,
										FALSE
									);

							// ---- target
								UINT target_index = element_id - '0';
								s_pi_model->SetTargetObjectId(target_index);

							// ---- set camera target status
								vs_system::AppKamera *kamera = s_pi_model->app_kamera[s_pi_model->app_kamera_index];
								kamera->SetTargetStatus(TRUE);

							// ---- set camera control mode
								s_pi_model->SetCameraControlMode(5);

						}
						return VS_ELM_SETUP_NONE;


					case Tint_Explore_Param::GROUP_LID_CAMERA_CONTROL:
						{

							// ---- set message
								vs_system::SysContext *sys_context = model->pi_model->app_dependency->GetSysContext();
								FLOAT ms_delta = (FLOAT)sys_context->GetDouble(vs_system::SysContext::TIME_MS_DELTA);
								model->SetMsgCameraControl(element_id, ms_delta, 0);

							// ---- display
								result = model->Tint_Explore_UpdateControlByMsg(
										handle_model,
										element_id
									);

						}
						return VS_ELM_SETUP_NONE;


					case Tint_Explore_Param::GROUP_LID_SET:
						{
							switch ( element_id )
							{
								case Tint_Explore_Param::ELEMENT_LID_RESET:
									{
										result = model->SetMsgCameraControl('X', 0, 0);
									}
									return VS_ELM_SETUP_NONE;
								case Tint_Explore_Param::ELEMENT_LID_SPEED:
									{
										result = Tint_Explore_UpdateSpeed();
									}
									return VS_ELM_SETUP_NONE;

								default:
									break;
							}
						}
						break;


					default:
						break;

				}
			}
		}



	return VS_ELM_SETUP_NONE;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Explore_UpdateElement(
		INT handle_model,
		INT element_id
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_setng;

	// ---- action groups
		BOOL target_animation_actions = FALSE;


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_BODY_EXPLORE_LID);


	// ---- process
		UINT number_of_element_ids = sizeof(tint_explore_param.element_item_lid_char) / sizeof(CHAR);
		for (UINT i = 0; i < number_of_element_ids; i++)
		{

			if ( tint_explore_param.element_item_lid_char[i] == (CHAR)element_id )
			{

				CHAR group = tint_explore_param.group_lid_char[i];
				switch ( group ) 
				{

					case Tint_Explore_Param::GROUP_LID_LIGHT:
						{
							switch ( element_id )
							{

								case Tint_Explore_Param::ELEMENT_LID_STANDARD:
								case Tint_Explore_Param::ELEMENT_LID_GRAPHIC:
									{
										result = tint->UpdateText(NULL, Tint_Explore_Param::ELEMENT_LID_LIGHT_TEXT);
									}
								default:
									break;
							}
						}
						break;
						

					case Tint_Explore_Param::GROUP_LID_VIEW:
						{
							switch ( element_id )
							{
								case Tint_Explore_Param::ELEMENT_LID_LOW:
								case Tint_Explore_Param::ELEMENT_LID_MEDIUM:
								case Tint_Explore_Param::ELEMENT_LID_HIGH:
									{
										result = tint->UpdateText(NULL, Tint_Explore_Param::ELEMENT_LID_VIEW_TEXT);
									}
								default:
									break;
							}
						}
						break;


					case Tint_Explore_Param::GROUP_LID_CAMERA_TRANSFORM:
						{
							result = tint->UpdateText(NULL, Tint_Explore_Param::ELEMENT_LID_UPDATE_FIRST);
							result = tint->UpdateText(NULL, Tint_Explore_Param::ELEMENT_LID_CONTROL_TEXT);
						}
						break;


					case Tint_Explore_Param::GROUP_LID_CAMERA_TARGET:
						{

							// ---- zap camera update
								result = tint->UpdateText(NULL, Tint_Explore_Param::ELEMENT_LID_UPDATE_FIRST);
								result = tint->UpdateText(NULL, Tint_Explore_Param::ELEMENT_LID_TARGET_FIRST);

							// ---- update animation actions
								target_animation_actions = TRUE;
								result = tint->UpdateText(NULL, Tint_Explore_Param::ELEMENT_LID_TARGET_FIRST);
						}
						break;


					default:
						break;

				}

			}
		}


		// ---- force update of animation actions
			if ( ! target_animation_actions )
			{
				result = tint->UpdateText(NULL, Tint_Explore_Param::ELEMENT_LID_TARGET_FIRST);
			}


	return VS_OK;
}


// ----------------------------------------------------------------------

INT  Model::Tint_Explore_AddActions(
	const INT  handle_obj_config,
	const INT  element_index,
	FLOAT      action_values[]
)
{
	// ---- for enum
		using namespace vs_system;


	// ---- local
		INT num_actions = 0;


	// ---- update ?

		switch ( element_index )
		{

			// ---- animation : JKL
				case tint_explore_param.ELEMENT_LID_TARGET_FIRST:
					{

						vs_system::AppKamera *kamera = s_pi_model->app_kamera[s_pi_model->app_kamera_index];
						FLOAT action = (FLOAT)(kamera->GetTargetStatus());

						num_actions = 3;

						FLOAT v[] =
							{
								(FLOAT)(tint_explore_param.ELEMENT_LID_ANIMATE_FIRST + 0), action,
								(FLOAT)(tint_explore_param.ELEMENT_LID_ANIMATE_FIRST + 1), action,
								(FLOAT)(tint_explore_param.ELEMENT_LID_ANIMATE_FIRST + 2), action
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
INT  Model::Tint_Explore_AddText(
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

	// ---- select elem
		switch ( element_id )
		{

			case Tint_Explore_Param::ELEMENT_LID_LIGHT_TEXT:
				{
					*replace_text = TRUE;
					UINT slm = s_pi_model->system_param->GetSystemLightMode();
					strcpy(text, tint_explore_param.light_text[slm]);
				}
				break;

			case Tint_Explore_Param::ELEMENT_LID_VIEW_TEXT:
				{
				*replace_text = TRUE;
				UINT svm = s_pi_model->system_param->GetSystemViewMode();
				strcpy(text, tint_explore_param.view_text[svm]);
				}
				break;

			case Tint_Explore_Param::ELEMENT_LID_UPDATE_FIRST:
				{
						*replace_text = TRUE;
						UINT ccm = s_pi_model->GetCameraControlMode();
						using namespace vs_system;
						switch ( ccm )
						{
							case AppKamera::Mode::XYZ       :
							case AppKamera::Mode::VECTOR    :
							case AppKamera::Mode::PTZ       :
							case AppKamera::Mode::TRANSLATE :
							case AppKamera::Mode::ROTATE    :
								strcpy(text, tint_explore_param.transform_text[ccm]);
								break;
							case AppKamera::Mode::TARGET:
								{
									strcpy(text, tint_explore_param.transform_text[ccm]);
								}
								break;
						}
				}
				break;

			case Tint_Explore_Param::ELEMENT_LID_CONTROL_TEXT:
				{
					*replace_text = TRUE;
					UINT ccm = s_pi_model->GetCameraControlMode();
					sprintf(
							text,
							"%s",
							tint_explore_param.control_text[ccm][0]
						);
				}
				break;

			case Tint_Explore_Param::ELEMENT_LID_TARGET_FIRST:
				{
					*replace_text = TRUE;
					UINT toid = s_pi_model->GetTargetObjectId();
					if ( toid == CameraTarget::NONE)
					{
						sprintf(text, "No target object selected");
					}
					else
					{
						sprintf(text,"Target object # %d selected",toid + 1);
					}
				}
				break;

			default:
				break;

		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Explore_AddValue(
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


////////////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------
INT Model::Tint_Explore_UpdateSpeed()
{

	// ---- flip flag
		tint_explore_param.high_speed =
			tint_explore_param.high_speed ? FALSE : TRUE;


	// ---- set increment scalar
		vs_system::AppKamera *kamera =
			s_pi_model->app_kamera[s_pi_model->app_kamera_index];


	// ---- note - using this flag to stop rotation being proportional to ScalarIncr
		kamera->SetScalarRotation(TRUE);


	// ---- set translation and rotation scalar
		if ( tint_explore_param.high_speed )
		{
			kamera->SetScalarIncr(0.5f);
			kamera->SetScalarMode(TRUE);
		}
		else
		{
			kamera->SetScalarIncr(0.1f);
			kamera->SetScalarMode(FALSE);
		}

	return VS_OK;
}


// ----------------------------------------------------------------------

INT Model::Tint_Explore_Switch(
		INT  model_handle,
		INT  group_lid,
		INT  element_lid,
		BOOL state
	)
{

	if ( group_lid == 0 ) return VS_ERROR;

	if ( element_lid == 0 )
	{
		for (int e = 0; e < tint_explore_param.element_dspl_count; e++)
		{
			if ( group_lid == tint_explore_param.element_dspl_info[e].group_lid )
				tint_explore_param.element_dspl_info[e].context->SetSwitchedOn(state);
		}
	}
	else
	{
		for (int e = 0; e < tint_explore_param.element_dspl_count; e++)
		{
			if (
					group_lid == tint_explore_param.element_dspl_info[e].group_lid &&
					element_lid == tint_explore_param.element_dspl_info[e].lid
				)
			{
				tint_explore_param.element_dspl_info[e].context->SetSwitchedOn(state);
			}
		}
	}

	return VS_OK;
}


// ----------------------------------------------------------------------

INT Model::Tint_Explore_UpdateByMsg(
		INT  model_handle,
		INT  msg_lid
	)
{

	// ---- note: invoked from -
		//
		//    AppKeyboard ->
		//       AppKontrol ->
		//          project Kontrol ->
		//             project Kamera -> Model:Tint_Explore_UpdateByMessage
		//


	// ---- local
		INT   result = VS_OK;
		Model *model = (Model *)model_handle;


	// ---- targets
		INT adj_msg_lid = msg_lid;
		if ( msg_lid >= '0' && msg_lid <= ':')
		{
			adj_msg_lid = '0';
		}


	// ---- udate
		switch  ( adj_msg_lid )
		{


			// ---- system light mode
				case 'Z':
					{

						// ---- set
							Tint_Explore_UpdateElement(model_handle, Tint_Explore_Param::ELEMENT_LID_LIGHT_TEXT);

						// ---- zap
							Tint_Explore_Switch(model_handle,
								Tint_Explore_Param::GROUP_LID_LIGHT,
									0,
									FALSE
								);

						// ---- get mode
							UINT slm = s_pi_model->system_param->GetSystemLightMode();

						// ---- set element
							Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_LIGHT,
									Tint_Explore_Param::ELEMENT_LID_STANDARD + slm,
									1
								);
					}
					break;


			// ---- system view mode
				case 'C':
					{

						// ---- set
							Tint_Explore_UpdateElement(model_handle, Tint_Explore_Param::ELEMENT_LID_LOW);

						// ---- zap
							Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_VIEW,
									0,
									FALSE
								);

						// ---- get mode
							UINT vsm = s_pi_model->system_param->GetSystemViewMode();

						// ---- set element
							Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_VIEW,
									Tint_Explore_Param::ELEMENT_LID_LOW + vsm,
									1
								);
					}
					break;


			// ---- camera update control
				case 'G':
					{

						// ---- zap target group
							Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_CAMERA_TARGET,
									0,
									FALSE
								);

						// ---- set camera update text 
							Tint_Explore_UpdateElement(model_handle, Tint_Explore_Param::ELEMENT_LID_UPDATE_FIRST);

						// ---- zap camera update mode
							Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_CAMERA_TRANSFORM,
									0,
									FALSE
								);

						// ---- set camera control & element
							INT mode = s_pi_model->GetCameraControlMode();

							Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_CAMERA_TRANSFORM,
									Tint_Explore_Param::ELEMENT_LID_UPDATE_FIRST + mode,
									TRUE
								);

					}
					break;

			// ---- target
				case '0':
					{

						// ---- zap camera update
							Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_CAMERA_TRANSFORM,
									0,
									FALSE
								);

						// ---- set camera control mode "Update" + "around location of interest" 
							s_pi_model->SetCameraControlMode(5);

						// ---- set camera update target text (e.g. "Update" + "about location of interest")
							Tint_Explore_UpdateElement(model_handle, Tint_Explore_Param::ELEMENT_LID_UPDATE_FIRST);

						// ---- zap target group
							Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_CAMERA_TARGET,
									0,
									FALSE
								);

						// ---- set target element
							Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_CAMERA_TARGET,
									msg_lid,
									TRUE
								);
					}
					break;

			// ---- camera speed
				case 'I':
					{
						// ---- set speed element
							Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_SET,
									Tint_Explore_Param::ELEMENT_LID_SPEED,
									tint_explore_param.high_speed
								);
					}
					break;

			default:
				break;
		}

	return result;
}


// ----------------------------------------------------------------------

INT Model::Tint_Explore_UpdateControlByMsg(
			INT  model_handle,
			INT  msg_lid
		)
{

	// ---- local
		INT   result = VS_OK;
		Model *model = (Model *)model_handle;


	// ---- find start and end element
	//
	//      note: end is the first element of the next
	//            non-control button element group
	//
		INT start = -1;
		INT end   = -1;
		CHAR last_lid_char = tint_explore_param.element_dspl_info[0].lid;
		for (INT e = 0; e < tint_explore_param.element_dspl_count; e++)
		{
			CHAR lid_char = tint_explore_param.element_dspl_info[e].lid;
			if (
					last_lid_char == tint_explore_param.ELEMENT_LID_TARGET_LAST &&
					lid_char == tint_explore_param.ELEMENT_LID_CONTROL_TEXT
				)
			{
				start = e;
			}
			else if ( lid_char == tint_explore_param.ELEMENT_LID_RESET )
			{
				end = e;
				break;
			}
			last_lid_char = lid_char;
		}


	// ---- whoooooops...
		if ( start == -1 || end == -1 ) return VS_ERROR;


	// ---- note: end is the first element of the next non-control button element group 


	// ---- persistant ?
		BOOL persistant_on = model->GetMsgCameraControlPersistantOn();


	// ---- set persistant & switch off all other elements
		for (INT e = start; e < end; e++)
		{
			CHAR lid_char = tint_explore_param.element_dspl_info[e].lid;
			if ( lid_char == 'Q' || lid_char == 'E' )
			{
				tint_explore_param.element_dspl_info[e].context->SetSwitchedOn(persistant_on);
			}
			else
			{
				tint_explore_param.element_dspl_info[e].context->SetSwitchedOn(FALSE);
			}
		}


	// ---- persistant ? - set on active camera control
		if ( persistant_on  )
		{

			// ---- active
				BOOL active_on = model->GetMsgCameraControlPersistantActive();

			// ---- switch...
				for (INT e = start; e < end; e++)
				{
					CHAR lid_char = tint_explore_param.element_dspl_info[e].lid;
					switch ( lid_char )
					{
						case 'J':
						case 'K':
						case 'L':

							// ---- force action update
								model->Tint_Explore_UpdateElement(model_handle, Tint_Explore_Param::ELEMENT_LID_TARGET_FIRST);

						case 'W':
						case 'R':
						case 'A':
						case 'D':
						case 'S':
						case 'F':

							// ---- switch
								if ( msg_lid == (INT)lid_char )
								{
									tint_explore_param.element_dspl_info[e].context->SetSwitchedOn(active_on);
								}
								break;

						default:
							break;
					}
				}
		}

	return VS_OK;
}


// ----------------------------------------------------------------------

INT  Model::Tint_Explore_UpdateByMessage(
		INT   handle_model,
		INT   msg_lid
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- get tint display list element info
		result = tint_explore_param.GetElementDsplParam(handle_model);


	// ---- update everything EXCEPT camera control
		result = model->Tint_Explore_UpdateByMsg(handle_model, msg_lid);


	// ---- camera control msg's
		INT control_group_msg[11] = { 'W', 'R', 'A', 'Q', 'D', 'E', 'S', 'F', 'J', 'K', 'L' };


	//  ---- update camera control
		for (INT e = 0; e < 11; e++)
		{
			if ( control_group_msg[e] == msg_lid )
			{
				result = model->Tint_Explore_UpdateControlByMsg(handle_model, msg_lid);
				return VS_OK;
			}
		}


	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////