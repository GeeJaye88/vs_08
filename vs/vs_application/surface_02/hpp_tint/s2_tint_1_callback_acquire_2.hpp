////////////////////////////////////////////////////////////////////////////////

// ---------- s2_tint_[#]_callback_acquire.hpp ----------
/*!
\file s2_tint_[#]_callback_acquire.hpp
\brief
\author Gareth Edwards
*/
 
#pragma once

using namespace surface_02;


// -------- Tint Acquisition Parameters

class Model::Tint_Acquire_Param_New_2
{

public:
	
	// ---- cdtor
		Tint_Acquire_Param_New_2()
		{

			// ---- element lid
				CHAR element_char[] =
					{
						't',                            // 116
						'f', 'g', 'h', 'i', 'j', 'k', 'l',
						'u',                            // 117
						'q', 'v', 'w', 'x', 'y', 'z',
						'p',                            // 112
						'1', '2', '3', '4', '5', '6',
						'A', 'a',
						'B', 'b',
						'C', 'c',
						'D', 'd',
						'E', 'e',
						's', 'r'                        // 114
					};


			// ---- group lid
				CHAR group_char[]=
					{
						'T',
						'L', 'L', 'L', 'L', 'L', 'L', 'L',
						'U',
						'Q', 'V', 'V', 'V', 'V', 'V',
						'J',
						'F', 'F', 'F', 'F', 'F', 'F',
						'P', 'P',
						'P', 'P',
						'P', 'P',
						'P', 'P',
						'P', 'P',
						'R', 'R'
					};


			// ---- initialisation of group/element properties
				#include "../hpp_tint_include/s2_tint_initialise_group_element.hpp"

		}

		~Tint_Acquire_Param_New_2()
		{
			;
		}


	// ---- methods

		// defintion of GetElementDsplInfo method
		#include "../hpp_tint_include/s2_tint_define_get_element_dspl_info.hpp"


	// ---- properties

		// declaration of group/element properties
		#include "../hpp_tint_include/s2_tint_declare_group_element.hpp"


	// ---- element item logical id descriptors
		enum
		{
			ELEMENT_LID_SOURCE_TEXT        = 't',
			ELEMENT_LID_SOURCE_SURF        = 'f',
			ELEMENT_LID_SOURCE_FUNCTION    = 'l',
			ELEMENT_LID_ACQUIRE_TEXT       = 'u',
			ELEMENT_LID_ACQUIRE_GET        = 'q',
			ELEMENT_LID_LUMINA_FIRST_FIELD = 'v',
			ELEMENT_LID_LUMINA_LAST_FIELD  = 'z',
			ELEMENT_LID_PARAM_TEXT         = 'p',
			ELEMENT_LID_PARAM_FIRST_GROUP  = '1',
			ELEMENT_LID_PARAM_FIRST        = 'A',
			ELEMENT_LID_VIEW_ANALYSYS      = 's',
			ELEMENT_LID_RESET              = 'r'
		};


	// ---- group item logical id descriptors
		enum
		{
			GROUP_LID_SOURCE_TEXT           = 'T',
			GROUP_LID_SOURCE_SURF           = 'L',
			GROUP_LID_ACQUIRE_TEXT          = 'U',
			GROUP_LID_ACQUIRE_GET           = 'Q',
			GROUP_LID_LUMINA_FIELD          = 'V',
			GROUP_LID_PARAM_TEXT            = 'J',
			GROUP_LID_PARAM_GROUP           = 'F',
			GROUP_LID_PARAM                 = 'P',
			GROUP_LID_SET                   = 'R',
		};


	// ---- source labels
		CHAR *element_source_labels[7] =
		{
			"Source - Camera - NA",
			"Source - Weld",
			"Source - Scratch 1",
			"Source - Scratch 2",
			"Source - Scratch 3",
			"Source - Terrain",
			"Source - Function",
		};


	// ---- acquire labels
		CHAR *element_acquire_labels[6] =
		{
			"Acquire",
			"Lumina Field 1",
			"Lumina Field 2",
			"Lumina Field 3",
			"Lumina Field 4",
			"3D Reconstruction",
		};

	// ---- acquire
		BOOL view_analysis_img = FALSE; 

} tint_acquire_param_new_2;



// ----------------------------------------------------------------------
/*
\note invoked at runtime in Model::SetupGraphics so
      that Tint_Acquire_UpdateModel can be invoked.
*/

INT Model::Tint_Acquire_GetElemLidSourceSurf()
{
	return (INT)Tint_Acquire_Param_New_2::ELEMENT_LID_SOURCE_SURF;
}

INT Model::Tint_Acquire_GetElemLidSourceFunction()
{
	return (INT)Tint_Acquire_Param_New_2::ELEMENT_LID_SOURCE_FUNCTION;
}

INT Model::Tint_Acquire_GetElemLidLuminaFirst()
{
	return (INT)Tint_Acquire_Param_New_2::ELEMENT_LID_LUMINA_FIRST_FIELD;
}

INT Model::Tint_Acquire_GetElemLidLuminaLast()
{
	return (INT)Tint_Acquire_Param_New_2::ELEMENT_LID_LUMINA_LAST_FIELD;
}


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
		FLOAT lincr = 12;
		INT  result = 0;


	// ---- local - required for TINT_ macros
		//  - gid: graphcs id
		//  - noi: number of items
		UINT gid = TINT_BODY_ACQUIRE_LID;
		UINT noi = 0;


	// ---- setup parameters
		SetParamGroup(surface_02::Model::PARAM_GROUP_ACQUIRE);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- build
		{

			// ---- scope tint def & enum
				using namespace vs_system;

			// ---- set
				TINT_SET_START_NOI()

			// ---- source mode
				TINT_ADD_ITEM("Mode: ")
				TINT_SET_TARCXLI(Tint::ITEM_TEXT , 0, 0, 0, 0.0f, TRUE, lincr / 8)

			// ---- acquisition sources
				TINT_ADD_ICON()
				TINT_SET_TARC(vs_system::Tint::ITEM_RADIO_LOCAL,     5, 2, 3)

				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO_LOCAL,    5, 1, 4, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO_LOCAL,    5, 1, 5, 2.2f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO_LOCAL,    5, 1, 6, 3.3f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO_LOCAL,    5, 1, 7, 4.4f)

				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO_LOCAL,    5, 2, 5, 5.5f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_RADIO_LOCAL,  5, 2, 5, 6.6f, TRUE, lincr / 2)

			// ---- acquisition
				TINT_ADD_ITEM("Acquire")
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_TEXT,         0, 0, 0, 0.0f, TRUE, lincr / 8)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_CLICK,          4, 1, 0, 0.0f)
					
			// ---- acquisition lumina fields
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO_LOCAL,    4, 6, 1, 1.7f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO_LOCAL,    4, 6, 2, 2.8f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO_LOCAL,    4, 6, 3, 3.9f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO_LOCAL,    4, 6, 4, 5.0f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_RADIO_LOCAL,  5, 2, 3, 6.1f, TRUE, lincr / 2)

			// ---- parameter groups text
				TINT_ADD_ITEM("Parameter: ")
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_TEXT,   0, 0, 0, 0.0f, TRUE, lincr/8)
				
			// ---- parameter groups
				gid = TINT_BODY_ACQUIRE_ARRAY_LID;
				TINT_ADD_ICON()
				TINT_SET_TARC(vs_system::Tint::ITEM_RADIO,     4, 6, 1)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO,    4, 6, 2, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO,    4, 6, 3, 2.2f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO,    4, 6, 4, 3.3f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO,    4, 6, 5, 4.4f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_RADIO,  4, 6, 6, 5.5f, TRUE, lincr/2)

			// ---- x offset
				FLOAT x_off = 1.5;

			// ---- param 1
				gid = TINT_BODY_ACQUIRE_PARAM_LID;
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 0)
				TINT_ADD_ITEM(char_param_array_labels[0])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr / 2)

			// ---- param 2
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 1)
				TINT_ADD_ITEM(char_param_array_labels[1])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr / 2)

			// ---- param 2
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 2)
				TINT_ADD_ITEM(char_param_array_labels[2])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr / 2)

			// ---- param 3
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 3)
				TINT_ADD_ITEM(char_param_array_labels[3])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr/2)

			// ---- param 4
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 4)
				TINT_ADD_ITEM(char_param_array_labels[4])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr)

			// ---- reset
				TINT_ADD_ITEM("Eval")
				TINT_SET_TARCX(vs_system::Tint::ITEM_SWITCH,   5, 4, 7, 0)
				TINT_ADD_ITEM("Reset")
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_CLICK,  5, 5, 6, 2, TRUE, lincr)

			// ---- get 
				TINT_SET_TOTAL_NOI();

		}


		// ---- set item lid's
			for (UINT i = 0; i < noi; i++)
			{
				li[i].elem_id = (UINT)tint_acquire_param_new_2.element_item_lid[i];
			}


		// ---- set acquire surface version selected
			INT surface_version_selected = s_pi_model->surface_02->GetAcquireSurfaceVersionSelected();


		// ---- set status
			for (UINT i = 0; i < noi; i++)
			{

				// ---- logical id's
					CHAR group_lid = tint_acquire_param_new_2.group_item_lid[i];
					CHAR elem_lid = tint_acquire_param_new_2.element_item_lid[i];

				// ---- acquisition source label
					if ( group_lid == Tint_Acquire_Param_New_2::GROUP_LID_SOURCE_TEXT )
					{
						strcpy(li[i].text, tint_acquire_param_new_2.element_source_labels[surface_version_selected]);
					}

				// ---- acquisition source surface
					else if ( group_lid == Tint_Acquire_Param_New_2::GROUP_LID_SOURCE_SURF)
					{
						INT surface_index = elem_lid - Tint_Acquire_Param_New_2::ELEMENT_LID_SOURCE_SURF;
						if ( surface_index == surface_version_selected )
						{
							li[i].SetStatus(TRUE);
						}
					}

				// ---- acquisition lumina fields
					else if ( group_lid == Tint_Acquire_Param_New_2::GROUP_LID_LUMINA_FIELD)
					{
						INT capture_index = elem_lid - Tint_Acquire_Param_New_2::ELEMENT_LID_LUMINA_FIRST_FIELD;
					}

				// ---- parameter group label
					else if ( group_lid == Tint_Acquire_Param_New_2::GROUP_LID_PARAM_TEXT )
					{
						strcpy(li[i].text, char_param_group_name);
					}

				// ---- parameter groups
					else if ( group_lid == Tint_Acquire_Param_New_2::GROUP_LID_PARAM_GROUP )
					{
						INT param_index = elem_lid - Tint_Acquire_Param_New_2::ELEMENT_LID_PARAM_FIRST_GROUP;
						if ( param_index == param_group_index )
						{
							li[i].SetStatus(TRUE);
						}
					}

				// ---- parameters
					else if ( group_lid == Tint_Acquire_Param_New_2::GROUP_LID_PARAM )
					{
						if ( strcmp(li[i+1].text, "NA") == 0 )
						{
							li[i].SetType(vs_system::Tint::ITEM_NULL);
						}
						else
						{
							INT param_index = elem_lid - Tint_Acquire_Param_New_2::ELEMENT_LID_PARAM_FIRST;
							if ( param_index == param_array_index ) // TBD
							{
								li[i].SetStatus(TRUE);
							}
						}
					}

				// ---- set
					else if ( group_lid == Tint_Acquire_Param_New_2::GROUP_LID_SET )
					{
						if ( elem_lid == Tint_Acquire_Param_New_2::ELEMENT_LID_VIEW_ANALYSYS )
						{
							li[i].SetStatus(tint_acquire_param_new_2.view_analysis_img);
						}
					}
			}


		// ---- cleanup parameters
			#include "../../surface_shared/hpp_param/param_cleanup.hpp"

	return noi;
}


// ----------------------------------------------------------------------
INT Model::Tint_Acquire_GetIconInfo(INT id)
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
INT  Model::Tint_Acquire_UpdateModel(
		INT  handle_model,
		INT  element_lid
	)
{
	
	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		model::Surface_02 *surface_02 = model->pi_model->surface_02;
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- get tint display list element info
		result = tint_acquire_param_new_2.GetElementDsplInfo(handle_model, TINT_BODY_ACQUIRE_LID);


	// ---- setup parameters
		SetParamGroup(surface_02::Model::PARAM_GROUP_ACQUIRE);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- select action by group (using element id -> group id hash table)
		CHAR group = tint_acquire_param_new_2.hash_element_lid_to_group_lid[element_lid];
		switch ( group )
		{

			case Tint_Acquire_Param_New_2::GROUP_LID_SOURCE_SURF:

				//
				// ---- SELECT ACQUISITION SOURCE
				//
				//  - camera (e.g. IDS)
				//  - 1 x SONY imaged RR weld
				//  - 3 x RR turbine disc scratches  
				//  - 1 x terrain
				//  - 1 x functional surface (fractal)
				//
				{

					// ---- SET

						// set mesh type visible - flat or 3D
						surface_02->SetVisibleMeshType(0, surface_02, TRUE);

						// set surface mode selected (sms)
						INT sms = element_lid - Tint_Acquire_Param_New_2::ELEMENT_LID_SOURCE_SURF;
						INT atlas_page = sms + 100;
						atlas_page = atlas_page == 100 ? 107 : atlas_page; // PYE

						// set surface version selected (svs)
						INT svs = 0;
						switch ( sms )
						{
							case 0: svs = vs_system::ObjConfig::MESH_VERSION_CAMERA;   break;
							case 1: svs = vs_system::ObjConfig::MESH_VERSION_RR;       break;
							case 2: svs = vs_system::ObjConfig::MESH_VERSION_RR;       break;
							case 3: svs = vs_system::ObjConfig::MESH_VERSION_RR;       break;
							case 4: svs = vs_system::ObjConfig::MESH_VERSION_RR;       break;
							case 5: svs = vs_system::ObjConfig::MESH_VERSION_TERRAIN;  break;
							case 6: svs = vs_system::ObjConfig::MESH_VERSION_FUNCTION; break;
						}
						surface_02->SetAcquireSurfaceVersionSelected(svs);

						// set RR surface version index
						INT rr_bump_map_index = 0;
						switch ( sms )
						{
							case 1: rr_bump_map_index = 0; break;
							case 2: rr_bump_map_index = 1; break;
							case 3: rr_bump_map_index = 2; break;
							case 4: rr_bump_map_index = 3; break;
							default: break;
						}
						surface_02->SetRRBumpMapIndex(rr_bump_map_index);

						// set heightfield scalar (default NOT flat)
						FLOAT scalar = 1;
						switch ( svs )
						{
							case vs_system::ObjConfig::MESH_VERSION_RR:
								scalar = 1;
								break;
							default:
								break;
						}
						surface_02->SetAcquireHeightFieldScalar(scalar);


					// ---- SET SURFACE MODE SELECTED

						// set current surface mode selected (csms) 
						UINT csms = surface_02->GetAcquireSurfaceModeSelected();

						// copy parameter set into parameter store
						surface_02->parameter_store[csms].CopyParamSet(&surface_02->parameter);

						// set surface mode selected (sms)
						surface_02->SetAcquireSurfaceModeSelected(sms);


					// ---- SETUP SCENE
						//
						//  - set sms size & height as per acquire setup table
						//  - copy sms store into parameter set
						//  - setup scene & objects
						//
						surface_02->SetAcquireSurfaceSize(surface_02->GetAcquireSetupSourceSurfaceSize(sms));
						surface_02->SetAcquireSurfaceYScalar(surface_02->GetAcquireSetupSourceSurfaceYScalar(sms));
						surface_02->parameter.CopyParamSet(&surface_02->parameter_store[sms]);
						surface_02->SetupAcquire(s_pi_model->app_kamera[0]);


					// ---- PARAMETER

						// get copied group indices
						INT group = GetParamGroup();
						INT group_index = GetParamGroupIndex();
						INT param_index = 0;
						{
							FLOAT *param_array = new FLOAT[param_array_size];
							result = GetParamArray(group_index, param_array_size, param_array);
							param_index = (INT)param_array[param_array_size - 1];
							delete[] param_array;
						}
						SetParamArrayIndex(param_index);


						// set parameter gui button & labels
						//
						//  - update all elements in parameter group
						//  - set first parameter keypad
						//
						INT new_group_lid = group_index + Tint_Acquire_Param_New_2::ELEMENT_LID_PARAM_FIRST_GROUP;
						Tint_Acquire_UpdateModel(handle_model, new_group_lid);
						INT new_element_lid = param_index + Tint_Acquire_Param_New_2::ELEMENT_LID_PARAM_FIRST;
						Tint_Acquire_UpdateElement(handle_model, new_element_lid);


					// ---- SURFACE
						//
						//  - re-init (zero) heightfield
						//  - set acquistion source atlas page 
						//  - re-init target loi
						model->Acquire_InitialiseHeightfield();
						surface_02->SetAcquireAtlasPageLid(atlas_page);
						surface_02->InitTargetLoi();

						//  - update surface version heightfield dependant objects,
						//    invokes Surface_02::ObjectSurfMesh_Setup
						//       which invokes ObjectSurfMesh_UpdateParam
						//          which loads source bump maps, etc.
						//
						surface_02->UpdateHeightfieldObjects(0, surface_02, TRUE);


					// ---- SWITCH PARAMETER
						//
						//  - parameter group all OFF
						//  - group ON
						//  - parameter array all OFF
						//  - parameter ON
						//

						tint->ElemDsplInfoSwitch(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_PARAM_GROUP,
								0,
								FALSE
							);
						tint->ElemDsplInfoSwitch(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_PARAM_GROUP,
								group_index + Tint_Acquire_Param_New_2::ELEMENT_LID_PARAM_FIRST_GROUP,
								TRUE
							);
						tint->ElemDsplInfoSwitch(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_PARAM,
								0,
								FALSE
							);
						tint->ElemDsplInfoSwitch(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_PARAM,
								param_index + Tint_Acquire_Param_New_2::ELEMENT_LID_PARAM_FIRST,
								TRUE
							);


					// ---- RELABEL
						//
						//  - source text
						//  - acquire text
						//

						tint->ElemDsplInfoSetText(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_SOURCE_TEXT,
								tint_acquire_param_new_2.ELEMENT_LID_SOURCE_TEXT,
								tint_acquire_param_new_2.element_source_labels[sms]
							);
						tint->ElemDsplInfoSetText(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_ACQUIRE_TEXT,
								Tint_Acquire_Param_New_2::ELEMENT_LID_ACQUIRE_TEXT,
								tint_acquire_param_new_2.element_acquire_labels[0]
							);


					// ---- SWITCH BUTTONS
						//
						//  - all source buttons OFF
						//  - selected source button ON
						//  - all lumina field buttons OFF

						tint->ElemDsplInfoSwitch(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_SOURCE_SURF,
								0,
								FALSE
							);
						tint->ElemDsplInfoSwitch(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_SOURCE_SURF,
								Tint_Acquire_Param_New_2::ELEMENT_LID_SOURCE_SURF + sms,
								TRUE
							);
						//tint->ElemDsplInfoSwitch(
						//		tint_acquire_param_new_2.element_dspl_count,
						//		tint_acquire_param_new_2.element_dspl_info,
						//		Tint_Acquire_Param_New_2::GROUP_LID_LUMINA_FIELD,
						//		Tint_Acquire_Param_New_2::ELEMENT_LID_LUMINA_FIRST_FIELD,
						//		FALSE
						//	);
						tint->ElemDsplInfoSwitch(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_LUMINA_FIELD,
								0,
								FALSE
							);
				}
				break;
						

			case Tint_Acquire_Param_New_2::GROUP_LID_ACQUIRE_GET:
				//
				// ---- ACQUISITION
				//
				{

					// ---- set mesh visible & mesh type 3D
						surface_02->SetVisibleMeshType(0, surface_02, TRUE);


					// ---- DEMO

						OutputDebugString("MF: Tint_Acquire_UpdateModel -> Acquire_GetDemoHeightfield\n");

						//  - simulate multi image acquisition
						//    by generating source test heighfields
						result = Acquire_GetDemoHeightfield();

						//  - simulate lumina field acquisition
						//    from a source test heighfield
						result = Acquire_CalculateDemoLuminafields();


					// ---- ANALYSE
						//
						//  - calculate image analyse overlays
						//  - set image analyse overlay destination atlas page lid
						//  - copy #4 (white)lumina field image to atlas page for the #4 (3D) surface
						//
						result = Acquire_AnalyseLuminafields();
						surface_02->SetAcquireAtlasPageLid(107);
						result = Acquire_CopyImageBufferToAtlasPage(4, tint_acquire_param_new_2.view_analysis_img);


					// ---- HEIGHTFIELD
						//
						//  - set heightfield heightfield parameters
						//  - calculate heightfield (aka "the big one")
						//  - update all heightfield dependant objects
						//
						surface_02->SetAcquireHeightfieldParam(
								surface_02->GetAcquireHeightFieldDimension(),
								surface_02->GetAcquireHeightFieldArray(),
								1.0f
							);
						result = Acquire_CalculateHeightfield();
						surface_02->UpdateHeightfieldObjects(0, surface_02, FALSE);

					// ---- LIGHTING
						//
						//  - set light # 0 ON
						//
						surface_02->SetAcquireLightIndex(0);


					// ---- SWITCH
						//
						//  - acqusition source buttons OFF
						//  - lumina & 3D buttons OFF
						//  - 3D button ON
						//

						tint->ElemDsplInfoSwitch(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_SOURCE_SURF,
								0,
								FALSE
							);
						tint->ElemDsplInfoSwitch(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_LUMINA_FIELD,
								0,
								FALSE
							);
						tint->ElemDsplInfoSwitch(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_LUMINA_FIELD,
								'z',
								TRUE
							);


					// ---- RELABEL as "3D Reconstruction"
						tint->ElemDsplInfoSetText(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_ACQUIRE_TEXT,
								Tint_Acquire_Param_New_2::ELEMENT_LID_ACQUIRE_TEXT,
								tint_acquire_param_new_2.element_acquire_labels[5]
							);
				}
				break;


			case Tint_Acquire_Param_New_2::GROUP_LID_LUMINA_FIELD:
				//
				// ---- SELECT LUMINA FIELD or 3D RECONSTRUCTION
				//
				{

					// ---- SET
						//
						//  - heightfield scalar (all 3D)
						//
						surface_02->SetAcquireHeightFieldScalar(1);

						// set lumina_field index
						UINT lfi = element_lid - Tint_Acquire_Param_New_2::ELEMENT_LID_LUMINA_FIRST_FIELD;
						result = surface_02->SetAcquireLightIndex(lfi);

						// set lumina_field index
						BOOL lumina_field_3D = lfi == 4 ? TRUE : FALSE;

						// set texture atlas page - note 3D texture is #107, all others #100
						surface_02->SetAcquireAtlasPageLid( lfi == 4 ? 107 : 100 );

						// copy acquire or analysis lumina field image to atlas page
						result = Acquire_CopyImageBufferToAtlasPage(lfi, tint_acquire_param_new_2.view_analysis_img);


					// ---- SURFACE
						//
						//  - set acquired heightfield y scaling ( lumina_field_3D ? 1 : 0 )
						//  - set visible mesh type either 3D or flat
						//  - store current surface version selected
						//  - set surface version selected
						//  - update heightfield dependant objects
						//  - re-store surface version selected
						//
						surface_02->SetAcquireHeightfieldParam(
								surface_02->GetAcquireHeightFieldDimension(),
								surface_02->GetAcquireHeightFieldArray(),
								lumina_field_3D ? 1.0f : 0.0f
							);
						result = surface_02->SetVisibleMeshType(0, surface_02, lumina_field_3D);
						
						// store
						UINT surface_version_selected = surface_02->GetAcquireSurfaceVersionSelected();
						surface_02->SetAcquireSurfaceVersionSelected(
								lumina_field_3D ? surface_version_selected :
									vs_system::ObjConfig::MESH_VERSION_CAMERA);
						
						// update
						result = surface_02->UpdateHeightfieldObjects(0, surface_02, FALSE);
						
						// restore
						surface_02->SetAcquireSurfaceVersionSelected(surface_version_selected);


					// ---- LIGHTING
						//
						//  - set light # lfi ON
						//
						if ( lfi != surface_02->GetAcquireLightMax() )
						{
							result = Acquire_SetupLighting();
							result = surface_02->SetAcquireLightIndex(lfi);
							result = surface_02->SetupGraphicsLighting(model::Surface_02::SceneSetupMode::ACQUIRE);

							// NOT 3D ? - then update spot & axis graphics
							if ( !lumina_field_3D )
							{
								surface_02->ObjectAcqAxis_Update(lfi, surface_02);
								surface_02->ObjectAcqSpot_Update(lfi, surface_02);
								surface_02->SetupGraphicsLighting(model::Surface_02::SceneSetupMode::ACQUIRE);
							}
						}


					// ---- SWITCH
						//
						//  - unset acquire lumina buttons
						//  - set acquire lumina button
						//
						tint->ElemDsplInfoSwitch(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_LUMINA_FIELD,
								0,
								FALSE
							);
						UINT elem_lid = element_lid + Tint_Acquire_Param_New_2::ELEMENT_LID_LUMINA_FIRST_FIELD;
						tint->ElemDsplInfoSwitch(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_LUMINA_FIELD,
								element_lid,
								TRUE
							);


					// ---- RELABEL
						//
						//  - acquire
						//
						tint->ElemDsplInfoSetText(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_ACQUIRE_TEXT,
								Tint_Acquire_Param_New_2::ELEMENT_LID_ACQUIRE_TEXT,
								tint_acquire_param_new_2.element_acquire_labels[lfi + 1]
							);

				}
				break;


			case Tint_Acquire_Param_New_2::GROUP_LID_PARAM_GROUP:
				//
				// ---- SELECT PARAMETER GROUP
				//
				{
						// select group
						INT group_index = element_lid - Tint_Acquire_Param_New_2::ELEMENT_LID_PARAM_FIRST_GROUP;
						result = SetParamGroupIndex(group_index);

						// get index of this group's selected parameter
						INT param_index = 0;
						{
							FLOAT *param_array = new FLOAT[param_array_size];
							result = GetParamArray(group_index, param_array_size, param_array);
							param_index = (INT)param_array[param_array_size-1];
							delete[] param_array;
						}
						result = SetParamArrayIndex(param_index);

						// update parameter text elements
						for (INT p = 0; p < 6; p++)
						{
							INT param_lid = p + Tint_Acquire_Param_New_2::ELEMENT_LID_PARAM_FIRST;
							result = Tint_Acquire_UpdateElement(handle_model, param_lid);
						}

						// get new parameter group name, &....
						std::string str_mesh_param_name;
						GetParamGroupName(str_mesh_param_name);

						// ... relabel group
						CHAR text[128];
						strcpy(text, str_mesh_param_name.c_str());
						tint->ElemDsplInfoSetText(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_PARAM_TEXT,
								Tint_Acquire_Param_New_2::ELEMENT_LID_PARAM_TEXT,
								text
							);

						// switch all parameter icons OFF
						tint->ElemDsplInfoSwitch(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_PARAM,
								0,
								FALSE
							);
						
						// switch selected parameter icon ON
						tint->ElemDsplInfoSwitch(
								tint_acquire_param_new_2.element_dspl_count,
								tint_acquire_param_new_2.element_dspl_info,
								Tint_Acquire_Param_New_2::GROUP_LID_PARAM,
								param_index + Tint_Acquire_Param_New_2::ELEMENT_LID_PARAM_FIRST,
								TRUE
							);

						// report
						{
							SetParamGroup(surface_02::Model::PARAM_GROUP_ACQUIRE);
							INT param_group_id = GetParamGroup();
							INT param_group_index = GetParamGroupIndex();

							char ods[128];
							sprintf(ods, " +-> A %d, %d, %d \n", param_group_id, param_group_index, param_index);
							OutputDebugString(ods);
						}

				}
				break;


			case Tint_Acquire_Param_New_2::GROUP_LID_PARAM:
				//
				// ---- UPDATE PARAMETER
				//
				{
						// select parameter
						INT param_index = element_lid - Tint_Acquire_Param_New_2::ELEMENT_LID_PARAM_FIRST;

						// store index of selected parameter
						{
							FLOAT *param_array = new FLOAT[param_array_size];
							result = GetParamArray(param_group_index, param_array_size, param_array);
							param_array[param_array_size - 1] = (FLOAT)param_index;
							result = SetParamArray(param_group_index, param_array_size, param_array);
							delete[] param_array;
						}
						result = SetParamArrayIndex(param_index);

						// update keypad
						//
						//  - select tint keypadbody
						//  - get parameter to be updated
						//  - update keypad button
						//  - select tint acquire body
						//
						result = tint->SetSelectedBody(KPAD_BODY_INPUT_LID);
						Kpad_UpdateByParameter(handle_model, GetParamGroup());
						Kpad_Input_UpdateElement(handle_model, (INT)Tint_Acquire_Param_New_2::GROUP_LID_PARAM);
						result = tint->SetSelectedBody(TINT_BODY_ACQUIRE_LID);

						// report
						{
							SetParamGroup(surface_02::Model::PARAM_GROUP_ACQUIRE);
							INT param_group_id    = GetParamGroup();
							INT param_group_index = GetParamGroupIndex();

							char ods[128];
							sprintf(ods, " +-> a %d, %d, %d \n", param_group_id, param_group_index, param_index);
							OutputDebugString(ods);
						}
				}
				break;


			case Tint_Acquire_Param_New_2::GROUP_LID_SET:
				//
				// ---- STUFF
				//
				{
					switch (element_lid)
					{

						case Tint_Acquire_Param_New_2::ELEMENT_LID_VIEW_ANALYSYS:
							//
							// ---- VIEW LUMINA FIELD WITH OR WITHOUT ANALYSYS
							//
							{
								// flip
								tint_acquire_param_new_2.view_analysis_img =
									tint_acquire_param_new_2.view_analysis_img ? FALSE : TRUE;

								// update
								Tint_Acquire_UpdateModel(
											handle_model,
												surface_02->GetAcquireLightIndex() + 
													Tint_Acquire_Param_New_2::ELEMENT_LID_LUMINA_FIRST_FIELD
										);
							}
							break;
						case Tint_Acquire_Param_New_2::ELEMENT_LID_RESET:
							//
							// ---- RESET SELECTED PARAMETER SET
							//
							{
								result = SetParamArrayDefaults(PARAM_GROUP_ACQUIRE);
								for (INT i = 0; i < 6; i++)
								{
									INT param_lid = i + Tint_Acquire_Param_New_2::ELEMENT_LID_PARAM_FIRST;
									result = Tint_Acquire_UpdateElement(handle_model, param_lid);
								}
							}
							break;
						default:

							break;
					}

				}
				break;

			default:
				break;

		}


	// ---- cleanup parameters
		#include "../../surface_shared/hpp_param/param_cleanup.hpp"

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Acquire_UpdateElement(
		INT handle_model,
		INT element_lid
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- select action by group (using element id -> group id hash table)
		CHAR group = tint_acquire_param_new_2.hash_element_lid_to_group_lid[element_lid];
		INT element_index = tint_acquire_param_new_2.hash_element_lid_to_index[element_lid];
		switch (group)
		{

			case Tint_Acquire_Param_New_2::GROUP_LID_SOURCE_SURF:
				{
					tint->UpdateText(NULL, Tint_Acquire_Param_New_2::ELEMENT_LID_SOURCE_TEXT);
				}
				break;

			case Tint_Acquire_Param_New_2::GROUP_LID_ACQUIRE_GET:
				{
					tint->UpdateText(NULL, Tint_Acquire_Param_New_2::ELEMENT_LID_ACQUIRE_TEXT);
				}
				break;
								
			case Tint_Acquire_Param_New_2::GROUP_LID_PARAM:
				{

					switch (element_lid)
					{
						case 'A':
						case 'B':
						case 'C':
						case 'D':
						case 'E':
						case 'F':
						case 'G':
						case 'H':
							{

								// ---- setup parameters
									INT result = VS_OK;
									SetParamGroup(surface_02::Model::PARAM_GROUP_ACQUIRE);
									#include "../../surface_shared/hpp_param/param_setup.hpp"


								// ---- update parameter text
									//
									//  - element logical identifiers
									//  - display indices
									//
										UINT last_element_lid = element_lid;
										UINT first_dspl_index = tint_acquire_param_new_2.hash_element_lid_to_dspl_index[element_lid];
										UINT last_dspl_index  = first_dspl_index + 1;

									// last element with same lid is text
										Tint_Acquire_Param_New_2 *tap = &tint_acquire_param_new_2;
										UINT dspl_count = (UINT)tap->element_dspl_count;
										do
										{
											last_element_lid = tap->element_dspl_info[last_dspl_index++].lid;
										}
										while ( last_element_lid == element_lid && last_dspl_index < dspl_count );

									// first & last not same ? - then update text  
										if ( last_element_lid != element_lid )
										{
											result = tint->UpdateText(NULL, last_element_lid );
										}


							// ---- update parameter values
									result = tint->SetSelectedBody(TINT_BODY_ACQUIRE_LID);
									model::Surface_02 *surface_02 = model->pi_model->surface_02;
									surface_02->GetParamArray(0, 9, param_array_values);
									surface_02->SetAcquireLightParam(
												*(param_array_values + 1),
												*(param_array_values + 2),
												*(param_array_values + 3),
												*(param_array_values + 4),
												*(param_array_values + 5)
											);
									surface_02->ObjectAcqAxis_Update(0, surface_02);
									surface_02->ObjectAcqSpot_Update(0, surface_02);
									surface_02->SetupGraphicsLighting(model::Surface_02::SceneSetupMode::ACQUIRE);


							// ---- cleanup parameters
								#include "../../surface_shared/hpp_param/param_cleanup.hpp"

							}
							break;
						default:
							break;
					}
				}
				break;

			default:
				break;

		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Acquire_AddActions(
		const INT  handle_obj_config,
		const INT  element_lid,
		FLOAT      action_values[]
	)
{

	// ---- lambda
		auto set_action_values = [](FLOAT av[], FLOAT v, INT n)
			{
				for (UINT i = 0; i < (UINT)n; i++)
				{
					UINT j = i * 2;
					av[j] = (FLOAT)(tint_acquire_param_new_2.ELEMENT_LID_LUMINA_FIRST_FIELD + i);
					av[j + 1] = v;;
				}
			};


	// ---- actions ?
		INT num_actions = 0;
		switch (element_lid)
		{
			//
			// ---- select acquisition source (e.g. camera, weld demo, terrain demo, function )
			//
			case Tint_Acquire_Param_New_2::ELEMENT_LID_SOURCE_TEXT:
				{								
					FLOAT action = 0;
					num_actions = 5;
					set_action_values(action_values, action, num_actions);
				}
				break;

			case Tint_Acquire_Param_New_2::ELEMENT_LID_ACQUIRE_TEXT:
				{			

					Model *model = (Model *)handle_obj_config;
					BOOL runtime = model->pi_model->runtime;
					if ( runtime ) return 0;

					FLOAT action = 1;
					num_actions = 5;
					set_action_values(action_values, action, num_actions);
				}
				break;

			default:
				break;

		}

	return num_actions;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Acquire_AddText(
		const INT  handle_model,
		const INT  element_lid,
		BOOL      *replace_text,
		BOOL      *add_text,
		CHAR      *text
	)
{

	// ---- init args
		*add_text     = FALSE;
		*replace_text = FALSE;
		*text         = NULL;


	// ---- select action by group (using element id -> group id hash table)
		CHAR group = tint_acquire_param_new_2.hash_element_lid_to_group_lid[element_lid];
		INT element_index = tint_acquire_param_new_2.hash_element_lid_to_index[element_lid];
		switch ( group )
		{

			case Tint_Acquire_Param_New_2::GROUP_LID_PARAM:
				{
					
						INT id = tolower(element_lid);
						switch ( id )
						{

							case 'a':  // note: parameter lid is 'A', value lid is 'a'
							case 'b':
							case 'c':
							case 'd':
							case 'e':
							case 'f':
							case 'g':
							case 'h':
								{

									// ---- local
										*replace_text = TRUE;
										BOOL multi_state = TRUE;
										Model *model = (Model *)handle_model;
										vs_system::Tint *tint = model->pi_model->tint_setng;


									// ---- update label

										// setup parameters
										INT result = VS_OK;
										SetParamGroup(surface_02::Model::PARAM_GROUP_ACQUIRE);
										#include "../../surface_shared/hpp_param/param_setup.hpp"

										// update keypad
										Kpad_UpdateByParameter(handle_model, GetParamGroup());

										// update parameter labels 'a' to 'h' (index == 0 to 7)
										INT index = id - (INT)'a';
										multi_state = tint->ElemDsplTextTidy(
													id,
													char_param_array_labels[index],
													param_array_values[index],
													text
												);

										//char ods[128];
										//sprintf(ods, " +-> A Param Group %d \n", (INT)GetParamGroup());
										//OutputDebugString(ods);

										// cleanup parameters
										#include "../../surface_shared/hpp_param/param_cleanup.hpp"


									// ---- switch elements of param selected (e.g. 'a' -> 'A', that's the parameter icon lid)
										result = tint->ElemDsplSwitch(
													TINT_BODY_ACQUIRE_LID,
													TINT_BODY_ACQUIRE_PARAM_LID,
													toupper(id), 
													& multi_state
												);
								}
								break;

							default:
								break;
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
		const INT  element_lid,
		BOOL      *add_value,
		FLOAT     *value,
		CHAR      *suffix
	)
{
	*add_value = FALSE;
	*value     = 0;
	sprintf(suffix, "2");
	return 0;
}
