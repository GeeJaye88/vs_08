////////////////////////////////////////////////////////////////////////////////

// ---------- s3_tint_1_callback_acquire.hpp ----------
/*!
\file s3_tint_1_callback_acquire.hpp
\brief
\author Gareth Edwards
*/

#pragma once


// -------- Tint Process Parameters

class Model::Tint_Acquire_Param
{

public:
	
	// ---- cdtor
		Tint_Acquire_Param()
		{
			;
		}
		~Tint_Acquire_Param()
		{
			;
		}

	// ---- methods

		INT GetElementDsplParam(INT handle_model)
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
								(UINT)TINT_BODY_ACQUIRE_LID,	// [in]
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


	// ---- element display list information
		INT  element_dspl_count = 0;
		vs_system::TintElemDsplInfo element_dspl_info[vs_system::Tint::MAX_BODY_ELM];


	// ---- items group logical id's
		CHAR element_item_lid_char[vs_system::Tint::MAX_BODY_ITEM] =
		{
			't',                                // 116
			'f', 'g', 'h', 'i', 'j', 'k', 'l',
			'u',                                // 117
			'q', 'v', 'w', 'x', 'y', 'z',
			'p',                                // 112
			'1', '2', '3', '4', '5', '6',
			'A', 'a',
			'B', 'b',
			'C', 'c',
			'D', 'd',
			'e', 'r'                            // 114
		};


	// ---- item element logical id descriptors
		//   Tint_Acquire_Param::
		enum
		{
			ELEMENT_LID_SOURCE_TEXT        = 't',
			ELEMENT_LID_SOURCE_SURF        = 'f',
			ELEMENT_LID_ACQUIRE_TEXT       = 'u',
			ELEMENT_LID_ACQUIRE_GET        = 'q',
			ELEMENT_LID_LUMINA_FIRST_FIELD = 'v',
			ELEMENT_LID_PARAM_TEXT         = 'p',
			ELEMENT_LID_PARAM_FIRST_GROUP  = '1',
			ELEMENT_LID_PARAM_FIRST        = 'A',
			ELEMENT_LID_COPY_ACQUIRE       = 'e',
			ELEMENT_LID_RESET              = 'r'
		};


	// ---- item element logical id's
		CHAR group_lid_char[vs_system::Tint::MAX_BODY_ITEM] =
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
			'R', 'R'
		};


	// ---- item group logical id descriptors
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


	// ---- display
		CHAR *element_T_labels[7] =
		{
			"Source - Camera - NA",
			"Source - Weld",
			"Source - Scratch 1",
			"Source - Scratch 2",
			"Source - Scratch 3",
			"Source - Dent on Cylinder",
			"Source - Function",
		};


	// ---- display
		CHAR *element_V_labels[6] =
		{
			"Acquire",
			"Lumina Field 1",
			"Lumina Field 2",
			"Lumina Field 3",
			"Lumina Field 4",
			"3D Reconstruction",
		};

	// ---- acquire
		UINT lumina_field_index = 999;
		UINT lumina_field_3D_index = 4;

		BOOL view_analysis_img = FALSE;

		BOOL surface_acquired = FALSE;

} tint_acquire_param;


// ----------------------------------------------------------------------

INT Model::Tint_Acquire_GetElemLidSourceSurf()
{
	return Tint_Acquire_Param::ELEMENT_LID_SOURCE_SURF;
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


	// ---- local for TINT_ macros
		UINT gid = TINT_BODY_ACQUIRE_LID;
		UINT noi = 0;


	// ---- param
		SetParamGroup(surface_03::Model::PARAM_GROUP_ACQUIRE);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- build
		{

			// ---- scope tint def & enum
				using namespace vs_system;

			// ---- set
				TINT_SET_START_NOI()

			// ---- source mode
				TINT_ADD_ITEM("Mode: ")
				TINT_SET_TARCXLI(Tint::ITEM_TEXT ,             0, 0, 0, 0.0f, TRUE, lincr / 8)

			// ---- acquisition sources
				TINT_ADD_ICON()
				TINT_SET_TARC(vs_system::Tint::ITEM_RADIO_LOCAL,     5, 2, 3)

					TINT_ADD_ICON()
					TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO_LOCAL, 5, 1, 4, 1.1f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO_LOCAL, 5, 1, 5, 2.2f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO_LOCAL, 5, 1, 6, 3.3f)
					TINT_ADD_ICON()
					TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO_LOCAL, 5, 1, 7, 4.4f)

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
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr)


			// ---- reset
				TINT_ADD_ITEM("Eval")
				TINT_SET_TARCX(vs_system::Tint::ITEM_SWITCH,   5, 4, 7, 0)
				TINT_ADD_ITEM("Reset")
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_CLICK,  5, 5, 6, 2, TRUE, lincr)

			// ---- get 
				TINT_SET_TOTAL_NOI();

		}


		// ---- set item id's
			for (UINT i = 0; i < noi; i++)
			{
				li[i].elem_id = (UINT)tint_acquire_param.element_item_lid_char[i];
			}


		// ---- set acquire_mode
			INT surface_mode = s_pi_model->surface_03->GetMeshMode();


		// ---- set status
			for (UINT i = 0; i < noi; i++)
			{

				// ---- local
					CHAR group_id = tint_acquire_param.group_lid_char[i];
					CHAR elem_id = tint_acquire_param.element_item_lid_char[i];

				// ---- acquisition source label
					if ( group_id == Tint_Acquire_Param::GROUP_LID_SOURCE_TEXT )
					{
						strcpy(li[i].text, tint_acquire_param.element_T_labels[surface_mode]);
					}

				// ---- acquisition source surface
					else if ( group_id == Tint_Acquire_Param::GROUP_LID_SOURCE_SURF)
					{
						INT surface_index = elem_id - Tint_Acquire_Param::ELEMENT_LID_SOURCE_SURF;
						if ( surface_index == surface_mode )
						{
							li[i].SetStatus(TRUE);
						}
					}

				// ---- acquisition lumina fields
					else if ( group_id == Tint_Acquire_Param::GROUP_LID_LUMINA_FIELD)
					{
						INT capture_index = elem_id - Tint_Acquire_Param::ELEMENT_LID_LUMINA_FIRST_FIELD;
						/*
						if ( capture_index == 4 )
						{
							li[i].SetStatus(TRUE);
						}
						*/
					}

				// ---- parameter group label
					else if ( group_id == Tint_Acquire_Param::GROUP_LID_PARAM_TEXT )
					{
						strcpy(li[i].text, char_param_group_name);
					}

				// ---- parameter groups
					else if ( group_id == Tint_Acquire_Param::GROUP_LID_PARAM_GROUP )
					{
						INT param_index = elem_id - Tint_Acquire_Param::ELEMENT_LID_PARAM_FIRST_GROUP;
						if ( param_index == param_group_index )
						{
							li[i].SetStatus(TRUE);
						}
					}

				// ---- parameters
					else if ( group_id == Tint_Acquire_Param::GROUP_LID_PARAM )
					{
						if ( strcmp(li[i+1].text, "NA") == 0 )
						{
							li[i].SetType(vs_system::Tint::ITEM_NULL);
						}
						else
						{
							INT param_index = elem_id - Tint_Acquire_Param::ELEMENT_LID_PARAM_FIRST;
							if ( param_index == param_array_index )
							{
								li[i].SetStatus(TRUE);
							}
						}
					}

				// ---- set
					else if ( group_id == Tint_Acquire_Param::GROUP_LID_SET )
					{
						if ( elem_id == Tint_Acquire_Param::ELEMENT_LID_COPY_ACQUIRE )
						{
							li[i].SetStatus(tint_acquire_param.view_analysis_img);
						}
					}
			}


		// ---- param
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
		INT  element_id
	)
{
	
	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		model::Surface_03 *surface_03 = model->pi_model->surface_03;
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- get tint display list element info
		result = tint_acquire_param.GetElementDsplParam(handle_model);


	// ---- param
		SetParamGroup(surface_03::Model::PARAM_GROUP_ACQUIRE);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- process
		UINT number_of_element_ids = sizeof(tint_acquire_param.element_item_lid_char) / sizeof(CHAR);
		for (UINT i = 0; i < number_of_element_ids; i++)
		{

			if ( tint_acquire_param.element_item_lid_char[i] == (CHAR)element_id )
			{

				CHAR group = tint_acquire_param.group_lid_char[i];
				switch ( group )
				{

					case Tint_Acquire_Param::GROUP_LID_SOURCE_SURF:
						//
						// ---- select acquisition source (e.g. camera, weld demo, 2, 3, 4, terrain demo, function )
						//
						{

							// ---- deactivate analysis tabs
								tint_acquire_param.surface_acquired = FALSE;
								model->TintHeadUpdateAnalysisTabButtons();

							// ---- select & update surface model
								vs_system::GfxInterface *gfx = model->pi_model->gfx;

							// ---- set surface actual (all buttons) mode
								INT surface_mode_actual = element_id - Tint_Acquire_Param::ELEMENT_LID_SOURCE_SURF;
								INT atlas_page = surface_mode_actual + 100;
								atlas_page = atlas_page == 100 ? 107 : atlas_page; // PYE

							// ---- set surface (versions) mode
								INT surface_mode = 0;
								switch (surface_mode_actual)
								{
									case 0: surface_mode = 0; break;
									case 1: surface_mode = 1; break;
									case 2: surface_mode = 1; break;
									case 3: surface_mode = 1; break;
									case 4: surface_mode = 1; break;
									case 5: surface_mode = 2; break;
									case 6: surface_mode = 3; break;
								}

							// ---- set weld surface version index
								INT rr_bump_map_index = 0;
								switch (surface_mode_actual)
								{
									case 1: rr_bump_map_index = 0; break;
									case 2: rr_bump_map_index = 1; break;
									case 3: rr_bump_map_index = 2; break;
									case 4: rr_bump_map_index = 3; break;
									default: break;
								}
								surface_03->SetRRBumpMapIndex(rr_bump_map_index);

							// ---- set acquire heightfield scalar
								surface_03->SetAcquireHeightFieldScalar(0.1f);

							// ---- update heightfield objects
								model->AcquireFlatHeightField();
								surface_03->SetAcquireAtlasPageLid(atlas_page);
								surface_03->SetMeshMode(surface_mode);
								surface_03->UpdateHeightfieldObjects((INT)'A', surface_03, TRUE);

							// ---- relabel source text
								tint->ElemDsplInfoSetText(
										tint_acquire_param.element_dspl_count,
										tint_acquire_param.element_dspl_info,
										Tint_Acquire_Param::GROUP_LID_SOURCE_TEXT,
										tint_acquire_param.ELEMENT_LID_SOURCE_TEXT,
										tint_acquire_param.element_T_labels[surface_mode_actual]
									);

							// ---- relabel acquire text
								tint->ElemDsplInfoSetText(
										tint_acquire_param.element_dspl_count,
										tint_acquire_param.element_dspl_info,
										Tint_Acquire_Param::GROUP_LID_ACQUIRE_TEXT,
										Tint_Acquire_Param::ELEMENT_LID_ACQUIRE_TEXT,
										tint_acquire_param.element_V_labels[0]
									);

							// ---- unset acquire lumina & 3D buttons
								tint->ElemDsplInfoSwitch(
										tint_acquire_param.element_dspl_count,
										tint_acquire_param.element_dspl_info,
										Tint_Acquire_Param::GROUP_LID_LUMINA_FIELD,
										0,
										FALSE
									);
						}
						break;
						

					case Tint_Acquire_Param::GROUP_LID_ACQUIRE_GET:
						//
						// ---- acquisition 
						//
						{

							// ---- activate analysis tabs
								tint_acquire_param.surface_acquired = TRUE;
								model->TintHeadUpdateAnalysisTabButtons();

							// ---- set acquire 3D mesh scalar
								 surface_03->SetAcquireHeightFieldScalar(0.1f);

							// ---- acquire
								result = AcquireDemoCalculateLuminaFields();
								result = AcquireAnalyseLuminaFields();
								surface_03->SetAcquireAtlasPageLid(107); // PYE
								result = AcquireCopyLuminaFieldToAtlasPage(4, tint_acquire_param.view_analysis_img );

							// ---- update heightfield object parameters
								surface_03->SetAcquireHeightfieldParam(
										surface_03->GetAcquireHeightFieldDimension(),
										surface_03->GetAcquireHeightFieldArray(),
										1.0f
									);

							// ---- calculate heightfield
								result = AcquireCalculateHeightField();

							// ---- update objects
								UINT sm = surface_03->GetMeshMode();
								surface_03->SetMeshMode(sm);
								surface_03->UpdateHeightfieldObjects((INT)'A', surface_03, FALSE);

							// ---- unset acquire source buttons
								tint->ElemDsplInfoSwitch(
										tint_acquire_param.element_dspl_count,
										tint_acquire_param.element_dspl_info,
										Tint_Acquire_Param::GROUP_LID_SOURCE_SURF,
										0,
										FALSE
									);

							// ---- unset acquire lumina & 3D buttons
								tint->ElemDsplInfoSwitch(
										tint_acquire_param.element_dspl_count,
										tint_acquire_param.element_dspl_info,
										Tint_Acquire_Param::GROUP_LID_LUMINA_FIELD,
										0,
										FALSE
									);

							// ---- set acquire 3D reconstructed surface button
								tint->ElemDsplInfoSwitch(
										tint_acquire_param.element_dspl_count,
										tint_acquire_param.element_dspl_info,
										Tint_Acquire_Param::GROUP_LID_LUMINA_FIELD,
										'z',
										TRUE
									);

							// ---- relabel
								tint->ElemDsplInfoSetText(
										tint_acquire_param.element_dspl_count,
										tint_acquire_param.element_dspl_info,
										Tint_Acquire_Param::GROUP_LID_ACQUIRE_TEXT,
										Tint_Acquire_Param::ELEMENT_LID_ACQUIRE_TEXT,
										tint_acquire_param.element_V_labels[5]
									);
						}
						break;


					case Tint_Acquire_Param::GROUP_LID_LUMINA_FIELD:
						//
						// ---- view processed acquisition lumina fields & 3D
						//
						{

							// ---- lumina_field 3D texture is #107, all others #100  // PYE
								tint_acquire_param.lumina_field_index = element_id - Tint_Acquire_Param::ELEMENT_LID_LUMINA_FIRST_FIELD;
								BOOL lumina_field_3D =
									tint_acquire_param.lumina_field_index ==
										tint_acquire_param.lumina_field_3D_index;
								surface_03->SetAcquireAtlasPageLid(
										tint_acquire_param.lumina_field_index == tint_acquire_param.lumina_field_3D_index ?
										107 : 100 // PYE
									);
								AcquireCopyLuminaFieldToAtlasPage(
										tint_acquire_param.lumina_field_index,
										tint_acquire_param.view_analysis_img
									);

							// ---- flat or 3D : set acquired heightfield y data
								surface_03->SetAcquireHeightfieldParam(
										surface_03->GetAcquireHeightFieldDimension(),
										surface_03->GetAcquireHeightFieldArray(),
										lumina_field_3D ? 1.0f : 0.0f
									);

							// ---- update heightfield
								UINT sm = surface_03->GetMeshMode();
								surface_03->SetMeshMode(lumina_field_3D ? sm : vs_system::ObjConfig::MESH_VERSION_CAMERA);
								surface_03->UpdateHeightfieldObjects((INT)'A', surface_03, FALSE);
								surface_03->SetMeshMode(sm);

							// ---- relabel acquire text
								tint->ElemDsplInfoSetText(
										tint_acquire_param.element_dspl_count,
										tint_acquire_param.element_dspl_info,
										Tint_Acquire_Param::GROUP_LID_ACQUIRE_TEXT,
										Tint_Acquire_Param::ELEMENT_LID_ACQUIRE_TEXT,
										tint_acquire_param.element_V_labels[tint_acquire_param.lumina_field_index + 1]
									);
						}
						break;


					case Tint_Acquire_Param::GROUP_LID_PARAM_GROUP:
						{

							// ---- select param group
								INT process_index = element_id - Tint_Acquire_Param::ELEMENT_LID_PARAM_FIRST_GROUP;
								result = SetParamGroupIndex(process_index);
								for (INT i = 0; i < 6; i++)
								{
									INT param_id = i + Tint_Acquire_Param::ELEMENT_LID_PARAM_FIRST;
									result = Tint_Acquire_UpdateElement(handle_model, param_id);
								}

							// ---- get new param group name, &....
								std::string str_mesh_param_name;
								GetParamGroupName(str_mesh_param_name);

							// ---- relabel
								CHAR text[128];
								strcpy(text, str_mesh_param_name.c_str());
								tint->ElemDsplInfoSetText(
										tint_acquire_param.element_dspl_count,
										tint_acquire_param.element_dspl_info,
										Tint_Acquire_Param::GROUP_LID_PARAM_TEXT,
										Tint_Acquire_Param::ELEMENT_LID_PARAM_TEXT,
										text
									);

						}
						break;


					case Tint_Acquire_Param::GROUP_LID_PARAM:
						{

							INT param_index = element_id - Tint_Acquire_Param::ELEMENT_LID_PARAM_FIRST;
							result = SetParamArrayIndex(param_index);

							result = tint->SetSelectedBody(KPAD_BODY_INPUT_LID);
							Kpad_UpdateByParameter(handle_model, GetParamGroup());
							Kpad_Input_UpdateElement(handle_model, (INT)Tint_Acquire_Param::GROUP_LID_PARAM);
							result = tint->SetSelectedBody(TINT_BODY_ACQUIRE_LID);

						}
						break;


					case Tint_Acquire_Param::GROUP_LID_SET:
						{
							switch (element_id)
							{
								case Tint_Acquire_Param::ELEMENT_LID_COPY_ACQUIRE:
									{
										tint_acquire_param.view_analysis_img =
											tint_acquire_param.view_analysis_img ? FALSE : TRUE;

										if ( tint_acquire_param.lumina_field_index != 999 )
										{
											Tint_Acquire_UpdateModel(
													handle_model,
														tint_acquire_param.lumina_field_index +
															Tint_Acquire_Param::ELEMENT_LID_LUMINA_FIRST_FIELD
												);
										}
									}
									break;
								case Tint_Acquire_Param::ELEMENT_LID_RESET:
									{
										result = SetParamArrayDefaults(PARAM_GROUP_ACQUIRE);
										for (INT i = 0; i < 5; i++)
										{
											INT param_id = i + Tint_Acquire_Param::ELEMENT_LID_PARAM_FIRST;
											result = Tint_Acquire_UpdateElement(handle_model, param_id);
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

			}
		}


	// ---- mesh param
		#include "../../surface_shared/hpp_param/param_cleanup.hpp"

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Acquire_UpdateElement(
		INT handle_model,
		INT element_id
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- mesh param
		SetParamGroup(surface_03::Model::PARAM_GROUP_ACQUIRE);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_BODY_ACQUIRE_LID);

		//char msg[50];
		//sprintf(msg, "+++ Tint_Acquire_UpdateElement %d \n", (INT)element_id);
		//OutputDebugString(msg);

	// ---- process
		UINT element_dspl_count = (UINT)tint_acquire_param.element_dspl_count;

		for (UINT i = 0; i < element_dspl_count; i++)
		{
				INT lid = tint_acquire_param.element_dspl_info[i].lid;
				if ( lid == (CHAR)element_id )
				{
						CHAR group = tint_acquire_param.element_dspl_info[i].group_lid;
						switch (group)
						{

							case Tint_Acquire_Param::GROUP_LID_SOURCE_SURF:
								{
									tint->UpdateText(NULL, Tint_Acquire_Param::ELEMENT_LID_SOURCE_TEXT);
								}
								break;

							case Tint_Acquire_Param::GROUP_LID_ACQUIRE_GET:
								{
									tint->UpdateText(NULL, Tint_Acquire_Param::ELEMENT_LID_ACQUIRE_TEXT);
								}
								break;
								
							case Tint_Acquire_Param::GROUP_LID_PARAM:
								{
									switch ( element_id )
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

												UINT next_lid = lid;
												UINT j = i+1;

												do
												{
													next_lid = tint_acquire_param.element_dspl_info[j++].lid;
												} while ( next_lid == lid && j < element_dspl_count );

												if (next_lid != lid)
												{
													result = tint->UpdateText(NULL, next_lid);
												}

											// ---- update config
												model::Surface_03 *surface_03 = model->pi_model->surface_03;
												UINT index = surface_03->GetParamArrayIndex();
												for (UINT i = 0; i < 5; i++)
												{
													surface_03->SetParamArrayIndex(i);
													switch (i)
													{
														case 0 :
															{
																surface_03->GetParamArray(0, 9, param_array_values);
																surface_03->SetAcquireLightParam(
																		*(param_array_values + 1),
																		*(param_array_values + 2),
																		*(param_array_values + 3),
																		*(param_array_values + 4)
																	);
																surface_03->ObjectAcqAxis_Update(0, surface_03);
															}
															break;
														default:
															break;
													}
												}
												surface_03->SetParamArrayIndex(index);
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
		}


	// ---- mesh param
		#include "../../surface_shared/hpp_param/param_cleanup.hpp"

	return VS_OK;
}

// ----------------------------------------------------------------------

INT Model::Tint_Acquire_AddActions(
		const INT  handle_obj_config,
		const INT  element_index,
		FLOAT      action_values[]
	)
{

	// ---- lambda
		auto set_action_values = [](FLOAT av[], FLOAT v, INT n)
			{
				for (UINT i = 0; i < (UINT)n; i++)
				{
					UINT j = i * 2;
					av[j] = (FLOAT)(tint_acquire_param.ELEMENT_LID_LUMINA_FIRST_FIELD + i);
					av[j + 1] = v;;
				}
			};


	// ---- local
		INT num_actions = 0;


	// ---- update ?
		switch ( element_index )
		{
				//
				// ---- select acquisition source (e.g. camera, weld demo, terrain demo, function )
				//
				case Tint_Acquire_Param::ELEMENT_LID_SOURCE_TEXT:
					{								
						FLOAT action = 0;
						num_actions = 5;
						set_action_values(action_values, action, num_actions);
					}
					break;

				case Tint_Acquire_Param::ELEMENT_LID_ACQUIRE_TEXT:
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
		const INT  element_id,
		BOOL      *replace_text,
		BOOL      *add_text,
		CHAR      *text
	)
{

	// ---- init args
		*add_text     = FALSE;
		*replace_text = FALSE;
		*text         = NULL;


	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- mesh param
		SetParamGroup(surface_03::Model::PARAM_GROUP_ACQUIRE);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- update
		Kpad_UpdateByParameter(handle_model, GetParamGroup());


	// ---- process
		UINT number_of_element_ids = sizeof(tint_acquire_param.element_item_lid_char) / sizeof(CHAR);
		for (UINT i = 0; i < number_of_element_ids; i++)
		{
			if (tint_acquire_param.element_item_lid_char[i] == (CHAR)element_id )
			{
				CHAR group = tint_acquire_param.group_lid_char[i];
				switch ( group )
				{
					case Tint_Acquire_Param::GROUP_LID_PARAM:
						{
							INT id = element_id;
							switch ( element_id )
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
										id = tint_acquire_param.element_item_lid_char[i + 1];
									}
								case 'a':
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
										INT index = id - (INT)'a';
										if ( strcmp(char_param_array_labels[index], "NA") != 0 )
										{
											sprintf(text, "%s: %f", char_param_array_labels[index], param_array_values[index]);
											std::string s = tostring(text);
											strcpy(text, s.c_str());
											INT len = strlen(text);
											if (text[len - 1] == '.')
											{
												text[len] = '0';
												text[len + 1] = 0;
											}
										}
										else
										{
											strcpy(text, "");
											multi_state = FALSE;
										}

										INT elem_handles[tint->MAX_BODY_ELM];
										INT num_elem = tint->GetBodyDsplElmCount(TINT_BODY_ACQUIRE_LID);
										tint->GetBodyDsplElmHandles(TINT_BODY_ACQUIRE_LID, elem_handles);

										for (INT j = 0; j < num_elem; j++)
										{
											vs_system::ElmContext *elem_context = NULL;
											s_pi_model->gfx->GetElementContext(elem_handles[j], &elem_context);

											INT elem_group = elem_context->GetGroupId();
											if ( elem_group == TINT_BODY_ACQUIRE_PARAM_LID )
											{
												INT elem_id = elem_context->GetId();
												if ( elem_id == tint_acquire_param.element_item_lid_char[i - 1])
												{
													elem_context->SetState(multi_state);
													elem_context->SetMultiState(multi_state);
													elem_context->SetMultiActive(multi_state, 0);
													INT behaviour = multi_state ? vs_system::ElmContext::BEHAVIOUR_RADIO : vs_system::ElmContext::BEHAVIOUR_NULL;
													elem_context->SetBehaviour(behaviour);
												}
											}
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
				return VS_OK;
			}
		}


	// ---- mesh param
		#include "../../surface_shared/hpp_param/param_cleanup.hpp"

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Acquire_AddValue(
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
	return 0;
}
