////////////////////////////////////////////////////////////////////////////////

// ---------- s2_tint_[#]_callback_explore.hpp ----------
/*!
\file s2_tint_[#]_callback_explore.hpp
\brief
\author Gareth Edwards

\note

    Interaction:

	// ---- Keyboard Cycle         GUI Elements
		// 'Z' cycle             - 'd' to 'e'
		// 'C' cycle             - 'm' to 'o'
		// 'G' cycle             - 'u' to 'y'


	// ---- Keyboard               GUI Element
		// '1' to '7' target     - apk
		// 'I' speed             - apk
		// 'T' texture           - apk
		// 'X' reset             - apk
		// 'Q' & 'E'             - apk
		// 'A' & 'D'             - apk
		// 'W' & 'S'             - apk
		// 'R' & 'F'             - apk

	// ---- Keyboard               GUI
		// 'V'                   - side toggle
		// 'B', 'N' & 'M'        - layout
		// '[' & ']'             - keypad +/-

	// ---- Mouse                  Keyboard & GUI Element
		// left & right          - 'A' & 'D'
		// up & down             - 'W' & 'S'
		// zoom                  - 'R' & 'F'

*/

#pragma once

using namespace surface_02;


// -------- Tint Explore Parameters

class Model::Tint_Explore_Param
{

public:


	// ---- cdtor
		Tint_Explore_Param()
		{

			// ---- element lid
				CHAR element_char[] =
				{
					'd',      'm',
					'e', 'f', 'n', 'o', 'p',
					'u', 'v', 'w', 'x', 'y',
					't', 'a', 'b', 'c',
					'1', '2', '3', '4', '5', '6', '7',
						 'W',      'R',
					'A', 'Q', 'D', 'E',
						 'S',      'F',
					'J', 'K', 'L', '&', 'X', 'I'
				};

			// ---- group lid
				CHAR group_char[] =
					{
						'D',      'M',
						'D', 'D', 'M', 'M', 'M',
						'U', 'U', 'U', 'U', 'U',
						'T', 'T', 'T', 'T',
						'L', 'L', 'L', 'L', 'L','L', 'L',
							 'C',      'C',
						'C', 'C', 'C', 'C',
							 'C',      'C',
						'C', 'C', 'C', 'S', 'S', 'S'
					};


			// ---- initialisation of group/element properties
				#include "../hpp_tint_include/s2_tint_initialise_group_element.hpp"

		}
		~Tint_Explore_Param()
		{
			;
		}


	// ---- methods

		// defintion of GetElementDsplInfo method
		#include "../hpp_tint_include/s2_tint_define_get_element_dspl_info.hpp"


		// declaration of group/element properties
		#include "../hpp_tint_include/s2_tint_declare_group_element.hpp"


	// ---- element item logical id descriptors
		enum
		{
			ELEMENT_LID_SOLID         = 'e',
			ELEMENT_LID_TEXTURED      = 'f',
			ELEMENT_LID_LOW           = 'n',
			ELEMENT_LID_MEDIUM        = 'o',
			ELEMENT_LID_HIGH          = 'p',
			ELEMENT_LID_UPDATE_FIRST  = 'u',
			ELEMENT_LID_UPDATE_LAST   = 'y',
			ELEMENT_LID_TARGET_LOI    = 't',
			ELEMENT_LID_TARGET_RULER  = 'a',
			ELEMENT_LID_TARGET_TFORM  = 'b',
			ELEMENT_LID_TARGET_NOFORM = 'c',
			ELEMENT_LID_TARGET_FIRST  = '1',
			ELEMENT_LID_TARGET_LAST   = '7',
			ELEMENT_LID_ANIMATE_FIRST = 'J',
			ELEMENT_LID_RNP           = '&',
			ELEMENT_LID_RAL           = 'X',
			ELEMENT_LID_SPEED         = 'I',
			ELEMENT_LID_DISPLAY_TEXT  = 'd',
			ELEMENT_LID_DENSITY_TEXT  = 'm',
			ELEMENT_LID_CONTROL_TEXT  = 'W',
		};


	// ---- group item logical id descriptors
		enum
		{
			GROUP_LID_MESH_DISPLAY   = 'D',
			GROUP_LID_MESH_DENSITY   = 'M',
			GROUP_LID_CAMERA_UPDATE  = 'U',
			GROUP_LID_CAMERA_LOI     = 'T',
			GROUP_LID_CAMERA_TARGET  = 'L',
			GROUP_LID_CAMERA_CONTROL = 'C',
			GROUP_LID_ANIMATE        = 'A',
			GROUP_LID_SET            = 'S'
		};


	// --- control speed
		BOOL high_speed = TRUE;
		BOOL low_speed = TRUE;

	// ---- display
		CHAR *mesh_display_text[2][2] =
			{
				{
					" W/U", " W/T" ,
				},
				{
					" S/U", " S/T" ,
				}
			};

		CHAR *mesh_density_text[3] =
			{
				" Low",
				" Medium",
				" High"
			};


	// ---- text
		CHAR *update_text[6] = 
			{
				" aligned with X, Y & Z axis",
				" Direction of View axis",
				" Pan, Tilt and Zoom",
				" global X, Z & Y translation",
				" global Y, X & Z rotation",
				" Locations of Interest"
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

INT Model::Tint_Explore_GetElemLidUpdateFirst()
{
	return (INT)Tint_Explore_Param::ELEMENT_LID_UPDATE_FIRST;
}

INT Model::Tint_Explore_GetElemLidTargetFirst()
{
	return (INT)Tint_Explore_Param::ELEMENT_LID_TARGET_FIRST;
}


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
				TINT_ADD_ITEM("Display")
				TINT_SET_TARCX(Tint::ITEM_TEXT,    0, 0, 0, 0.0f)
				TINT_ADD_ITEM("Density")
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,  0, 0, 0, 3.0f, TRUE, lincr/8)


			// ---- display
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_SWITCH,  4, 4, 3, 0.0f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_SWITCH,  4, 4, 4, 1.1f)


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
					TINT_ADD_ITEM("Locations of Interest")
					TINT_SET_TARCX(Tint::ITEM_SWITCH,        5, 0, 4, 0.0f);
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_SWITCH,        5, 0, 5, 1.1f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_SWITCH,        5, 0, 6, 2.5f)
					TINT_ADD_ICON()
					TINT_SET_TARCXLI(Tint::ITEM_SWITCH,      5, 0, 7, 3.6f, TRUE, lincr / 8)

				// ---- row 2
					TINT_ADD_ITEM("")
					TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 6, 1, 0.0f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 6, 2, 1.1f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 6, 3, 2.2f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 6, 4, 3.3f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 6, 5, 4.4f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_RADIO_LOCAL,   4, 6, 6, 5.5f)
					TINT_ADD_ICON()
					TINT_SET_TARCXLI(Tint::ITEM_RADIO_LOCAL, 4, 6, 7, 6.6f, TRUE, lincr)


			// ---- control

				// control _^_ - ^
					TINT_ADD_ITEM("Control")
					TINT_SET_TARCX(Tint::ITEM_CLICK,    4, 2, 1, 1.1f)
					TINT_ADD_ITEM(" ")
					TINT_SET_TARCXLI(Tint::ITEM_CLICK,  4, 2, 1, 4.4f, TRUE, lincr / 8)

				// control <o> - o
					TINT_ADD_ICON()
					TINT_SET_TARC(Tint::ITEM_CLICK,     4, 3, 0)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_SWITCH,   4, 3, 7, 1.1f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_CLICK,    4, 3, 2, 2.2f)

					TINT_ADD_ICON()
					TINT_SET_TARCXLI(Tint::ITEM_SWITCH, 4, 3, 7, 4.4f, TRUE, lincr / 8)

				// control _v_ - v 
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_CLICK,    4, 4, 1, 1.1f)
					TINT_ADD_ICON()
					TINT_SET_TARCXLI(Tint::ITEM_CLICK,  4, 4, 1, 4.4f, TRUE, lincr)

				// ---- animate
					TINT_ADD_ITEM("Animate")
					TINT_SET_TARC(Tint::ITEM_CLICK,        4, 0, 5)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_CLICK,       4, 0, 6, 1.1f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(Tint::ITEM_CLICK,       4, 0, 7, 2.2f)


			// ---- set
				TINT_ADD_ITEM("ZTR")
				TINT_SET_TARCX(Tint::ITEM_CLICK,    5, 5, 6, 3.7f)
				TINT_ADD_ITEM("Reset")
				TINT_SET_TARCX(Tint::ITEM_CLICK,    5, 5, 6, 5.1f) 

				TINT_ADD_ITEM("Speed")
				TINT_SET_TARCXLI(Tint::ITEM_SWITCH, 5, 5, 5, 6.8f, TRUE, lincr)


			// ---- get 
				TINT_SET_TOTAL_NOI();

		}


		// ---- set item element id's
			for (INT i = 0; i < noi; i++)
			{
				li[i].elem_id = (UINT)tint_explore_param.element_item_lid[i];
			}


		// ---- const
			UINT EXPLORE_MESH_TEXT_INDEX    = 0;    //  2 items
			UINT EXPLORE_MESH_DISPLAY_INDEX = 2;    //  2 items
			UINT EXPLORE_MESH_DENSITY_INDEX = 4;    //  3   "
			UINT EXPLORE_MOVE_INDEX         = 7;    //  8   "
			UINT EXPLORE_TARGET_INDEX       = 15;   //  7   "
			UINT EXPLORE_CONTROL_INDEX      = 22;   //  8   "
			UINT EXPLORE_ANIMATE_INDEX      = 30;   //  3   "
			UINT EXPLORE_RS_INDEX           = 33;   //  3   "


		// ---- shortcut
			model::Surface_02 *surface_02 = s_pi_model->surface_02;


		// ---- set surface solid & textured
			{
				BOOL solid = surface_02->GetMeshSolid();
				surface_02->SetMeshSolid(solid);
				li[EXPLORE_MESH_DISPLAY_INDEX + 0].SetStatus(solid);

				BOOL textured = surface_02->GetMeshTextured();
				surface_02->SetMeshTextured(textured);
				li[EXPLORE_MESH_DISPLAY_INDEX + 1].SetStatus(textured);
			}


		// ---- set mesh densisty mode
			{
				UINT mdm = surface_02->GetMeshDensityMode();
				switch ( mdm )
				{
					case surface_02->LOW:
					case surface_02->MEDIUM:
					case surface_02->HIGH:
						li[EXPLORE_MESH_DENSITY_INDEX + mdm].SetStatus(TRUE);
						break;
					default:
						{
							mdm = surface_02->LOW;
							surface_02->SetMeshDensityMode(mdm);
							li[EXPLORE_MESH_DENSITY_INDEX + mdm].SetStatus(TRUE);
						}
						break;
				}
				surface_02->SetMeshDensity(mdm + 1);
			}


		// ---- set camera mode
			{
				UINT ccm = s_pi_model->GetCameraControlMode();
				using namespace vs_system;
				switch ( ccm )
				{
					case AppKamera::Mode::XYZ       :
					case AppKamera::Mode::VECTOR    :
					case AppKamera::Mode::PTZ       :
					case AppKamera::Mode::TRANSLATE :
					case AppKamera::Mode::ROTATE    :
						li[EXPLORE_MOVE_INDEX + ccm].SetStatus(TRUE);
						break;

					case AppKamera::Mode::TARGET:
						{
							UINT target_index =
								s_pi_model->GetTargetObjectId() +
									EXPLORE_TARGET_INDEX;
							if (
									target_index >= EXPLORE_TARGET_INDEX &&
									target_index < EXPLORE_CONTROL_INDEX
								)
							{
								li[target_index].SetStatus(TRUE);
							}
						}
						break;

					default:
						break;
				}
			}

		// ---- set camera speed
			li[EXPLORE_RS_INDEX + 3].SetStatus(tint_explore_param.high_speed);


	return noi;
}


// ----------------------------------------------------------------------
INT Model::Tint_Explore_GetIconInfo(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 4;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 5;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
		default:
			break;
	}
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Explore_UpdateModel(
		INT  handle_model,
		INT  element_lid
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		model::Surface_02 *surface_02 = s_pi_model->surface_02;
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- get tint display list element info
		result = tint_explore_param.GetElementDsplInfo(handle_model, TINT_BODY_EXPLORE_LID);


	// ---- select action by group (using element id -> group id hash table)
		CHAR group = tint_explore_param.hash_element_lid_to_group_lid[element_lid];
		switch ( group ) 
		{

			case Tint_Explore_Param::GROUP_LID_MESH_DISPLAY:
				{
					switch ( element_lid )
					{
						case Tint_Explore_Param::ELEMENT_LID_SOLID:
							{
								BOOL solid = surface_02->GetMeshSolid();
								solid = solid ? FALSE : TRUE;
								surface_02->SetMeshSolid(solid);
								surface_02->UpdateHeightfieldObjects(0, surface_02, FALSE);
							}
							return VS_ELM_SETUP_NONE;

						case Tint_Explore_Param::ELEMENT_LID_TEXTURED:
							{
								BOOL textured = surface_02->GetMeshTextured();
								textured = textured ? FALSE : TRUE;
								surface_02->SetMeshTextured(textured);
								surface_02->UpdateHeightfieldObjects(0, surface_02, FALSE);
							}
							return VS_ELM_SETUP_NONE;

						default:
							break;
					}
				}
				break;
						

			case Tint_Explore_Param::GROUP_LID_MESH_DENSITY:
				{
					switch ( element_lid )
					{
						case Tint_Explore_Param::ELEMENT_LID_LOW:
						case Tint_Explore_Param::ELEMENT_LID_MEDIUM:
						case Tint_Explore_Param::ELEMENT_LID_HIGH:
							{

								// ---- model
									UINT mdm = element_lid - Tint_Explore_Param::ELEMENT_LID_LOW;
									surface_02->SetMeshDensityMode(mdm);

								// ---- surface
									surface_02->SetMeshDensity(mdm + 1);
									surface_02->UpdateHeightfieldObjects(0, surface_02, FALSE);
							}
							return VS_ELM_SETUP_NONE;

						default:
							break;
					}
				}
				break;


			case Tint_Explore_Param::GROUP_LID_CAMERA_UPDATE:
				{

					// ---- unset target rig buttons
						model->Tint_Explore_Switch(handle_model,
								Tint_Explore_Param::GROUP_LID_CAMERA_LOI,
								0,
								FALSE
							);

					// ---- don't zap loi state
						//surface_02->SetTargetLoiVisible(FALSE);
						//surface_02->SetTargetLoiRulerVisible(FALSE);
						//surface_02->SetTargetLoiTranslate(FALSE);

					// ---- hide loi chrome
						surface_02->ObjectLoiAxis_HideAll(surface_02);
						surface_02->ObjectLoiSpot_HideAll(surface_02);
						surface_02->ObjectLoiRect_HideAll(surface_02);
						surface_02->ObjectLoiRule_HideAll(surface_02);

					// ---- unset loi target
						model->Tint_Explore_Switch(handle_model,
								Tint_Explore_Param::GROUP_LID_CAMERA_TARGET,
								0,
								FALSE
							);

					// ---- unset loi target #
						s_pi_model->SetTargetObjectId(CameraTarget::NONE);
						s_pi_model->surface_02->SetTargetLoiIndex(CameraTarget::NONE);

					// ---- unset camera target
						vs_system::AppKamera *kamera = s_pi_model->app_kamera[s_pi_model->app_kamera_index];
						kamera->SetTargetStatus(FALSE);

					// --- set target camera control #
						UINT ccm = element_lid - Tint_Explore_Param::ELEMENT_LID_UPDATE_FIRST;
						s_pi_model->SetCameraControlMode(ccm);

				}
				return VS_ELM_SETUP_NONE;

			case Tint_Explore_Param::GROUP_LID_CAMERA_LOI:
			{
				switch ( element_lid )
				{
					case Tint_Explore_Param::ELEMENT_LID_TARGET_LOI:
						{
							BOOL loi_visible = surface_02->GetTargetLoiVisible();
							loi_visible = loi_visible ? FALSE : TRUE;
							surface_02->SetTargetLoiVisible(loi_visible);
						}
						break;

					case Tint_Explore_Param::ELEMENT_LID_TARGET_RULER:
						{
							BOOL ruler_visible = surface_02->GetTargetLoiRulerVisible();
							ruler_visible = ruler_visible ? FALSE : TRUE;
							surface_02->SetTargetLoiRulerVisible(ruler_visible);
						}
						break;

					case Tint_Explore_Param::ELEMENT_LID_TARGET_TFORM:
						{
							BOOL translate = surface_02->GetTargetLoiTranslate();
							translate = translate ? FALSE : TRUE;
							surface_02->SetTargetLoiTranslate(translate);
						}
						break;

					case Tint_Explore_Param::ELEMENT_LID_TARGET_NOFORM:
						{
							/*
							BOOL translate = surface_02->GetTargetLoiTranslate();
							translate = translate ? FALSE : TRUE;
							surface_02->SetTargetLoiTranslate(translate);
							*/
							;
						}
						break;

					default:
						break;
				}
			}
			break;

			case Tint_Explore_Param::GROUP_LID_CAMERA_TARGET:
				{

					// ---- zap camera update
						model->Tint_Explore_Switch(handle_model,
								Tint_Explore_Param::GROUP_LID_CAMERA_UPDATE,
								0,
								FALSE
							);

					// --- set target camera update #
						s_pi_model->SetCameraControlMode(5);

					// ---- set target #
						UINT tid = element_lid - Tint_Explore_Param::ELEMENT_LID_TARGET_FIRST;
						s_pi_model->SetTargetObjectId(tid);
						surface_02->SetTargetLoiIndex(tid);

					// ---- update target axis #
						surface_02->ObjectLoiAxis_Update(surface_02);

					// ---- set target camera mode
						vs_system::AppKamera *kamera = s_pi_model->app_kamera[s_pi_model->app_kamera_index];
						kamera->SetTargetStatus(TRUE);

					// --- set loi buttons & chrome
						BOOL target_loi_visible = surface_02->GetTargetLoiVisible();
						surface_02->SetTargetLoiVisible(target_loi_visible);
						model->Tint_Explore_Switch(handle_model,
								Tint_Explore_Param::GROUP_LID_CAMERA_LOI,
								Tint_Explore_Param::ELEMENT_LID_TARGET_LOI,
								target_loi_visible
							);

						BOOL target_ruler_visible = surface_02->GetTargetLoiRulerVisible();
						surface_02->SetTargetLoiRulerVisible(target_ruler_visible);
						model->Tint_Explore_Switch(handle_model,
								Tint_Explore_Param::GROUP_LID_CAMERA_LOI,
								Tint_Explore_Param::ELEMENT_LID_TARGET_RULER,
								target_ruler_visible
							);

						BOOL target_transform = surface_02->GetTargetLoiTranslate();
						surface_02->SetTargetLoiTranslate(target_transform);
						model->Tint_Explore_Switch(handle_model,
								Tint_Explore_Param::GROUP_LID_CAMERA_LOI,
								Tint_Explore_Param::ELEMENT_LID_TARGET_TFORM,
								target_transform
							);

				}
				return VS_ELM_SETUP_NONE;


			case Tint_Explore_Param::GROUP_LID_CAMERA_CONTROL:
				{

					// ---- camera
						FLOAT delta = tint_explore_param.high_speed ? 250.0f : 50.0f;
						model->SetMsgCameraControl(element_lid, delta, 0);

					// ---- display
						result = model->Tint_Explore_UpdateControlByMsg(
								handle_model,
								element_lid
							);

				}
				return VS_ELM_SETUP_NONE;


			case Tint_Explore_Param::GROUP_LID_SET:
				{
					switch ( element_lid )
					{
							case Tint_Explore_Param::ELEMENT_LID_RNP:
							{
								result = model->SetMsgCameraControl('&', 0, 0);
							}
							return VS_ELM_SETUP_NONE;
						case Tint_Explore_Param::ELEMENT_LID_RAL:
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

	return VS_ELM_SETUP_NONE;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Explore_UpdateElement(
		INT handle_model,
		INT element_lid
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


	// ---- select action by group (using element id -> group id hash table)
		CHAR group = tint_explore_param.hash_element_lid_to_group_lid[element_lid];
		switch ( group ) 
		{

			case Tint_Explore_Param::GROUP_LID_MESH_DISPLAY:
				{
					switch ( element_lid )
					{

						case Tint_Explore_Param::ELEMENT_LID_SOLID:
						case Tint_Explore_Param::ELEMENT_LID_TEXTURED:
							{
							result = tint->UpdateText(NULL, Tint_Explore_Param::ELEMENT_LID_DISPLAY_TEXT);
						}
						default:
							break;
					}
				}
				break;

			case Tint_Explore_Param::GROUP_LID_MESH_DENSITY:
				{
					switch ( element_lid )
					{
						case Tint_Explore_Param::ELEMENT_LID_LOW:
						case Tint_Explore_Param::ELEMENT_LID_MEDIUM:
						case Tint_Explore_Param::ELEMENT_LID_HIGH:
							{
								result = tint->UpdateText(NULL, Tint_Explore_Param::ELEMENT_LID_DENSITY_TEXT);
							}
						default:
							break;
					}
				}
				break;

			case Tint_Explore_Param::GROUP_LID_CAMERA_UPDATE:
				{
					result = tint->UpdateText(NULL, Tint_Explore_Param::ELEMENT_LID_UPDATE_FIRST);
					result = tint->UpdateText(NULL, Tint_Explore_Param::ELEMENT_LID_CONTROL_TEXT);
				}
				break;

			case Tint_Explore_Param::GROUP_LID_CAMERA_TARGET:
				{

					// ---- zap camera update
						result = tint->UpdateText(NULL, Tint_Explore_Param::ELEMENT_LID_UPDATE_FIRST);

					// ---- update animation actions
						target_animation_actions = TRUE;
						result = tint->UpdateText(NULL, Tint_Explore_Param::ELEMENT_LID_TARGET_LOI);
				}
				break;

			default:
				break;

		}


		// ---- force update of animation actions
			if ( ! target_animation_actions )
			{
				result = tint->UpdateText(NULL, Tint_Explore_Param::ELEMENT_LID_TARGET_LOI);
			}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Explore_AddActions(
	const INT  handle_obj_config,
	const INT  element_lid,
	FLOAT      action_values[]
)
{
	// ---- for enum
		using namespace vs_system;


	// ---- local
		INT num_actions = 0;


	// ---- update ?
		switch ( element_lid )
		{

			// ---- RNP : not availble in PTZ modes
				case tint_explore_param.ELEMENT_LID_UPDATE_FIRST:
				case tint_explore_param.ELEMENT_LID_UPDATE_FIRST + 1:
					{
						UINT ccm = s_pi_model->GetCameraControlMode();
						FLOAT rnp_action = (FLOAT)( ccm == 0 || ccm == 1 ? 0 : 1 );
						FLOAT v[] =
						{
							(FLOAT)(tint_explore_param.ELEMENT_LID_RNP), rnp_action
						};
						num_actions = ((INT)sizeof(v) / (INT)sizeof(FLOAT)) / 2;
						for (INT i = 0; i < num_actions * 2; i++) action_values[i] = v[i];
					}
					break;

			// ---- animation : JKL
				case tint_explore_param.ELEMENT_LID_TARGET_LOI:
					{
						vs_system::AppKamera *kamera = s_pi_model->app_kamera[s_pi_model->app_kamera_index];
						FLOAT animate_action = (FLOAT)(kamera->GetTargetStatus());
						FLOAT rnp_action     = (FLOAT)(kamera->GetTargetStatus() ? 0 : 1);
						FLOAT v[] =
							{
								(FLOAT)tint_explore_param.ELEMENT_LID_TARGET_LOI,          animate_action,
								(FLOAT)tint_explore_param.ELEMENT_LID_TARGET_RULER,        animate_action,
								(FLOAT)tint_explore_param.ELEMENT_LID_TARGET_TFORM,        animate_action,
								(FLOAT)tint_explore_param.ELEMENT_LID_TARGET_NOFORM,       animate_action,
								(FLOAT)(tint_explore_param.ELEMENT_LID_ANIMATE_FIRST + 0), animate_action,
								(FLOAT)(tint_explore_param.ELEMENT_LID_ANIMATE_FIRST + 1), animate_action,
								(FLOAT)(tint_explore_param.ELEMENT_LID_ANIMATE_FIRST + 2), animate_action,
								(FLOAT)(tint_explore_param.ELEMENT_LID_RNP),               rnp_action
							};
						num_actions = ( (INT)sizeof(v) / (INT)sizeof(FLOAT) ) / 2;
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
		const INT  element_lid,
		BOOL      *replace_text,
		BOOL      *add_text,
		CHAR      *text
	)
{

	// ---- init
		*replace_text = FALSE;
		*add_text = FALSE;
		*text = NULL;


	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_obj_config;
		model::Surface_02 *surface_02 = s_pi_model->surface_02;


	// ---- select elem
		switch ( element_lid )
		{

			case Tint_Explore_Param::ELEMENT_LID_DISPLAY_TEXT:
				{
					*add_text = TRUE;
					BOOL solid = surface_02->GetMeshSolid();
					BOOL textured = surface_02->GetMeshTextured();
					strcpy(text, tint_explore_param.mesh_display_text[solid][textured]);
				}
				break;

			case Tint_Explore_Param::ELEMENT_LID_DENSITY_TEXT:
				{
					*add_text = TRUE;
					UINT mdm = surface_02->GetMeshDensityMode();
					strcpy(text, tint_explore_param.mesh_density_text[mdm]);
				}
				break;

			case Tint_Explore_Param::ELEMENT_LID_UPDATE_FIRST:
				{
						*add_text = TRUE;
						UINT ccm = s_pi_model->GetCameraControlMode();
						using namespace vs_system;
						switch ( ccm )
						{
							case AppKamera::Mode::XYZ       :
							case AppKamera::Mode::VECTOR    :
							case AppKamera::Mode::PTZ       :
							case AppKamera::Mode::TRANSLATE :
							case AppKamera::Mode::ROTATE    :
								strcpy(text, tint_explore_param.update_text[ccm]);
								break;
							case AppKamera::Mode::TARGET:
								{
									UINT toi = s_pi_model->GetTargetObjectId();
									strcpy(text, tint_explore_param.update_text[ccm]); // BUG - shoulf be toi ???
								}
								break;
						}
				}
				break;

			case Tint_Explore_Param::ELEMENT_LID_CONTROL_TEXT:
				{
					*replace_text = TRUE;
					UINT control_update_mode = s_pi_model->GetCameraControlMode();
					sprintf(
							text,
							" %s",
							tint_explore_param.control_text[control_update_mode][0]
						);
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


// ========================================================================== //


// ----------------------------------------------------------------------
INT Model::Tint_Explore_UpdateSpeed()
{

	// ---- flip flag
		tint_explore_param.high_speed = tint_explore_param.high_speed ? 0 : 1;


	// ---- flip flag
		BOOL max_scalar_selected = s_pi_model->surface_02->GetExploreKameraMaxScalarSelected();
		max_scalar_selected = max_scalar_selected ? FALSE : TRUE;
		s_pi_model->surface_02->SetExploreKameraMaxScalarSelected(max_scalar_selected);


	// ---- set increment scalar
		vs_system::AppKamera *kamera =
			s_pi_model->app_kamera[s_pi_model->app_kamera_index];


	// ---- note - using this flag to enusre that rotation is not proportional to acquire size
		kamera->SetScalarRotation(FALSE);


	// ---- set translation and rotaion scalars 
		if ( max_scalar_selected )
		{ 
			FLOAT max = s_pi_model->surface_02->GetExploreKameraMaxScalar();
			kamera->SetScalarIncr(max);
			kamera->SetRotateIncr(0.02f);
			kamera->SetScalarMode(TRUE);
		}
		else
		{
			FLOAT min = s_pi_model->surface_02->GetExploreKameraMinScalar();
			kamera->SetScalarIncr(min);
			kamera->SetRotateIncr(0.005f);
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


// ========================================================================== //


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
		model::Surface_02 *surface_02 = s_pi_model->surface_02;


	// ---- target ? - use '0' as a catch all
		INT adj_msg_lid = msg_lid;
		if ( msg_lid >= '1' && msg_lid <= '7')
		{
			adj_msg_lid = '1';
		}


	// ---- update
		switch  ( adj_msg_lid )
		{

			// ---- targets
				case '1':
					{

						// ---- zap camera update
							result = Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_CAMERA_UPDATE,
									0,
									FALSE
								);

						// ---- set camera control mode "Update" + "around location of interest" 
							s_pi_model->SetCameraControlMode(5);

						// ---- set camera update target text (e.g. "Update" + "about location of interest")
							result = Tint_Explore_UpdateElement(model_handle, Tint_Explore_Param::ELEMENT_LID_UPDATE_FIRST);

						// ---- zap target group
							result = Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_CAMERA_TARGET,
									0,
									FALSE
								);

						// ---- set target element
							result = Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_CAMERA_TARGET,
									msg_lid,
									TRUE
								);

					}
					return VS_OK;


			// ---- mesh display modes
				case 'Z':
					{
						// ---- set display text 
							result = Tint_Explore_UpdateElement(model_handle, Tint_Explore_Param::ELEMENT_LID_SOLID);

						// ---- zap display group
							result = Tint_Explore_Switch(model_handle,
								Tint_Explore_Param::GROUP_LID_MESH_DISPLAY,
									0,
									FALSE
								);
						
						// ---- set mesh solid element
							result = Tint_Explore_Switch(model_handle,
								Tint_Explore_Param::GROUP_LID_MESH_DISPLAY,
								Tint_Explore_Param::ELEMENT_LID_SOLID,
								surface_02->GetMeshSolid()
							);

						// ---- set mesh texture element
							result = Tint_Explore_Switch(model_handle,
								Tint_Explore_Param::GROUP_LID_MESH_DISPLAY,
								Tint_Explore_Param::ELEMENT_LID_TEXTURED,
								surface_02->GetMeshTextured()
							);

					}
					return VS_OK;


			// ---- mesh density mode
				case 'C':
					{

						// ---- set density text 
							result = Tint_Explore_UpdateElement(model_handle, Tint_Explore_Param::ELEMENT_LID_LOW);

						// ---- zap density group 
							result = Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_MESH_DENSITY,
									0,
									FALSE
								);

						// ---- set density mode & text
							INT mdm = (INT)surface_02->GetMeshDensityMode();
							result = Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_MESH_DENSITY,
									Tint_Explore_Param::ELEMENT_LID_LOW + mdm,
									surface_02->GetMeshSolid()
								);

					}
					return VS_OK;


			// ---- camera update control
				case 'G':
					{

						// ---- zap target group
							result = Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_CAMERA_TARGET,
									0,
									FALSE
								);

						// ---- set camera update text 
							result = Tint_Explore_UpdateElement(model_handle, Tint_Explore_Param::ELEMENT_LID_UPDATE_FIRST);

						// ---- zap camera update mode
							result = Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_CAMERA_UPDATE,
									0,
									FALSE
								);

						// ---- set camera control & element
							INT mode = s_pi_model->GetCameraControlMode();

							result = Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_CAMERA_UPDATE,
									Tint_Explore_Param::ELEMENT_LID_UPDATE_FIRST + mode,
									surface_02->GetMeshSolid()
								);

					}
					return VS_OK;
			
			// ---- camera speed
				case 'I':
					{

						// ---- set speed element
							result = Tint_Explore_Switch(model_handle,
									Tint_Explore_Param::GROUP_LID_SET,
									Tint_Explore_Param::ELEMENT_LID_SPEED,
									tint_explore_param.high_speed
								);

					}
					return VS_OK;

			default:
				break;
		}

	return VS_ERROR;
}


// ----------------------------------------------------------------------
INT Model::Tint_Explore_UpdateControlByMsg(
			INT  model_handle,
			INT  msg_lid
		)
{

	// ---- note: invoked from -
		//
		//    Tint -> Model::Tint_Explore_UpdateModel
		//
		//    AppKeyboard ->
		//       AppKontrol ->
		//          project Kontrol ->
		//             project Kamera -> Model::Tint_Explore_UpdateByMessage
		//

	// ---- local
		INT   result = VS_OK;
		Model *model = (Model *)model_handle;


	// ---- find start and end element
		//
		//  - note: end is the first element of the next
		//    non-control button element group
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
			else if ( lid_char == tint_explore_param.ELEMENT_LID_RNP)
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
									result = model->Tint_Explore_UpdateElement(model_handle, Tint_Explore_Param::ELEMENT_LID_TARGET_FIRST);

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
INT Model::Tint_Explore_UpdateByMessage(
		INT   handle_model,
		INT   msg_lid
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- get tint display list element info
		result = tint_explore_param.GetElementDsplInfo(handle_model, TINT_BODY_EXPLORE_LID);


	// ---- update '0-9', 'Z', 'X', 'C', 'G' and 'I'
		result = model->Tint_Explore_UpdateByMsg(handle_model, msg_lid);
		if ( result == VS_OK )
		{
			return VS_OK;
		}


	//  ---- update camera control
		INT control_group_msg[11] = { 'W', 'R', 'A', 'Q', 'D', 'E', 'S', 'F', 'J', 'K', 'L' };
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


// ========================================================================== //