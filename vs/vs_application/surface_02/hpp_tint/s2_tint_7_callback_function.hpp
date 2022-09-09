////////////////////////////////////////////////////////////////////////////////

// ---------- s2_tint_[#]_callback_function.hpp ----------
/*!
\file s2_tint_[#]_callback_function.hpp
\brief
\author Gareth Edwards
*/

#pragma once

using namespace surface_02;


// -------- Functionality and Parameters for Tabbed Keypad Input --------

class Model::Tint_Function_Param
{

public:


	// ---- cdtor
		Tint_Function_Param()
		{

			// ---- element lid
				CHAR element_char[] =
					{
						't',
						'1', '2', '3', '4', '5', '6',
						'A', 'a',
						'B', 'b',
						'C', 'c',
						'D', 'd',
						'E', 'e',
						'F', 'f',
						'G', 'g',
						'H', 'h',
						'r',
					};


			// ---- group lid
				CHAR group_char[] =
					{
						'T',
						'F', 'F', 'F', 'F', 'F', 'F',
						'P', 'P',
						'P', 'P',
						'P', 'P',
						'P', 'P',
						'P', 'P',
						'P', 'P',
						'P', 'P',
						'P', 'P',
						'R',
					};


			// ---- initialisation of group/element properties
				#include "../hpp_tint_include/s2_tint_initialise_group_element.hpp"

		}
		~Tint_Function_Param()
		{
			;
		}


	// ---- methods

		// defintion of GetElementDsplInfo method
		#include "../hpp_tint_include/s2_tint_define_get_element_dspl_info.hpp"


	// ---- properties

		// declaration of group/element properties
		#include "../hpp_tint_include/s2_tint_declare_group_element.hpp"

} tint_function_param;



// ----------------------------------------------------------------------
INT Model::Tint_Function_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_Function_BuildList;
	tint_callback->callback_get_icon_info  = Tint_Function_GetIconInfo;
	tint_callback->callback_update_model   = Tint_Function_UpdateModel;
	tint_callback->callback_update_element = Tint_Function_UpdateElement;
	tint_callback->callback_add_actions    = NULL;
	tint_callback->callback_add_text       = Tint_Function_AddText;
	tint_callback->callback_add_value      = Tint_Function_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Function_BuildList(
		vs_system::TintItem li[]
	)
{

	// ---- local
		FLOAT lincr = 12;
		INT   result = 0;


	// ---- local - required for TINT_ macros
		//  - gid: graphcs id
		//  - noi: number of items
		UINT gid = TINT_BODY_FUNCTION_LID;
		UINT noi = 0;

		
	// ---- setup parameters
		SetParamGroup(surface_02::Model::PARAM_GROUP_FUNCTION);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- build
		{

			// ---- scope tint def & enu
				using namespace vs_system;


			// ---- set
				TINT_SET_START_NOI()

			// ---- row 0
				TINT_ADD_ITEM(char_param_group_name)
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,  3, 0, 0, 0, TRUE, lincr/8)

			// ---- row 1
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,    4, 6, 1)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO,   4, 6, 2, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO,   4, 6, 3, 2.2f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO,   4, 6, 4, 3.3f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO,   4, 6, 5, 4.4f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_RADIO, 4, 6, 6, 5.5f, TRUE, lincr)

			// ---- x offset
				FLOAT x_off = 1.5;
				gid = TINT_BODY_FUNCTION_PARAM_LID;

			// ---- param 1
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 0)
				TINT_ADD_ITEM(char_param_array_labels[0])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr / 2)

			// ---- param 2
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 1)
				TINT_ADD_ITEM(char_param_array_labels[1])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr / 2)

			// ---- param 3
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 2)
				TINT_ADD_ITEM(char_param_array_labels[2])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr / 2)

			// ---- param 4
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 3)
				TINT_ADD_ITEM(char_param_array_labels[3])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr / 2)

			// ---- param 5
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 4)
				TINT_ADD_ITEM(char_param_array_labels[4])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr / 2)

			// ---- param 6
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 5)
				TINT_ADD_ITEM(char_param_array_labels[5])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr / 2)

			// ---- param 7
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 6)
				TINT_ADD_ITEM(char_param_array_labels[6])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr / 2)

			// ---- param 8
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 7)
				TINT_ADD_ITEM(char_param_array_labels[7])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr)

			// ---- reset
				TINT_ADD_ITEM("Reset")
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_CLICK, 5, 5, 6, LAST_LINE_SPACING_0, TRUE, lincr)

			// ---- set total number of items
				TINT_SET_TOTAL_NOI();

		}


		// ---- set item lid's
			for (UINT i = 0; i < noi; i++)
			{
				li[i].elem_id = (UINT)tint_function_param.element_item_lid[i];
			}
		

		// ---- set function status
			for (UINT i = 0; i < noi; i++)
			{

				// ---- logical id's
					CHAR group_lid = tint_function_param.group_item_lid[i];
					CHAR elem_lid = tint_function_param.element_item_lid[i];


				// ---- functions
					if ( group_lid == 'F' )
					{
						// group (default) 0 is set ON
						INT group_index = elem_lid - '1';
						if ( group_index == param_group_index )
						{
							li[i].SetStatus(TRUE);
							// don't break... not done...
						}
					}

				// ---- parameters
					else if ( group_lid == 'P' )
					{
						// if "NA" set OFF
						if (strcmp(li[i + 1].text, "NA") == 0)
						{
							li[i].SetType(vs_system::Tint::ITEM_NULL);
						}
						else
						{
							// paremeter (default) 0 is set ON
							INT param_index = elem_lid - 'A';
							if (param_index == param_array_index) // TBD
							{
								li[i].SetStatus(TRUE);
							}
						}
					}
			}


	// ---- cleanup parameters
		#include "../../surface_shared/hpp_param/param_cleanup.hpp"

		return noi;
}


// ----------------------------------------------------------------------
INT Model::Tint_Function_GetIconInfo(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 4;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 2;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
		default:
			break;
	}
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Function_UpdateModel(
		INT  handle_model,
		INT  element_lid
	)
{

	// Update, either:
	//
	//    'F' - choose a different function
	//    'P' - send a new set of parameters to the keypad
	//    'S'ave, 'R'eset or 'I'ncrement
	//

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_setng;
		model::Surface_02 *surface_02 = s_pi_model->surface_02;


	// ---- get tint display list element info
		result = tint_function_param.GetElementDsplInfo(handle_model, TINT_BODY_FUNCTION_LID);


	// ---- setup parameters
		SetParamGroup(surface_02::Model::PARAM_GROUP_FUNCTION);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- select action by group (using element id -> group id hash table)
		CHAR group = tint_function_param.hash_element_lid_to_group_lid[element_lid];
		switch ( group) 
		{

				case 'F' :
					//
					// ---- SELECT PARAMETER GROUP
					//

					{
						// select group
						INT group_index = element_lid - '1';
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
						for (INT i = 0; i < 8; i++)
						{
							INT param_id = i + 'A';
							result = Tint_Function_UpdateElement(handle_model, param_id);
						}

						// switch all parameter icons OFF
						tint->ElemDsplInfoSwitch(
								tint_function_param.element_dspl_count,
								tint_function_param.element_dspl_info,
								'P',
								0,
								FALSE
							);
						
						// switch selected parameter icon ON
						tint->ElemDsplInfoSwitch(
								tint_function_param.element_dspl_count,
								tint_function_param.element_dspl_info,
								'P',
							param_index + 'A',
								TRUE
							);

						// update height field
						surface_02->UpdateHeightfieldObjects(0, surface_02, FALSE);

						// report
						{
							SetParamGroup(surface_02::Model::PARAM_GROUP_FUNCTION);
							INT param_group_id = GetParamGroup();
							INT param_group_index = GetParamGroupIndex();

							char ods[128];
							sprintf(ods, " +-> F %d, %d, %d \n", param_group_id, param_group_index, param_index);
							OutputDebugString(ods);
						}

					}
					break;


				case 'P':
					//
					// ---- UPDATE PARAMETER
					//
					{
						// select parameter
						INT param_index = element_lid - 'A';

						// store index of selected parameter 
						{
							FLOAT *param_array = new FLOAT[param_array_size];
							result = GetParamArray(param_group_index, param_array_size, param_array);
							param_array[param_array_size-1] = (FLOAT)param_index;
							result = SetParamArray(param_group_index, param_array_size, param_array);
							delete[] param_array;
						}
						result = SetParamArrayIndex(param_index);

						// update keyboard
						result = tint->SetSelectedBody(KPAD_BODY_INPUT_LID);
						Kpad_UpdateByParameter(handle_model, GetParamGroup());
						Kpad_Input_UpdateElement(handle_model, (INT)'P');
						result = tint->SetSelectedBody(TINT_BODY_FUNCTION_LID);

						// report
						{
							SetParamGroup(surface_02::Model::PARAM_GROUP_FUNCTION);
							INT param_group_id = GetParamGroup();
							INT param_group_index = GetParamGroupIndex();

							char ods[128];
							sprintf(ods, " +-> f %d, %d, %d \n", param_group_id, param_group_index, param_index);
							OutputDebugString(ods);
						}

					}
					break;

				case 'R':
					//
					// ---- STUFF
					//
					{
						result = SetParamArrayDefaults(PARAM_GROUP_FUNCTION);
						for (INT i = 0; i < 8; i++)
						{
							INT param_id = i + 'A';
							result = Tint_Function_UpdateElement(handle_model, param_id);
						}
						surface_02->UpdateHeightfieldObjects(0, surface_02, FALSE);
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
INT Model::Tint_Function_UpdateElement(
		INT handle_model,
		INT element_lid
	)
{

	// ---- local 
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		model::Surface_02 *surface_02 = model->pi_model->surface_02;
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_BODY_FUNCTION_LID);


	// ---- select action by group (using element id -> group id hash table)
		CHAR group = tint_function_param.hash_element_lid_to_group_lid[element_lid];
		switch (group)
		{

			// ---- parameters
				case 'P':
				{
					switch ( element_lid )
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
								// update text next to the parameter icon (e.g. lid 'A' updates lid 'a')
								INT element_index = tint_function_param.hash_element_lid_to_index[element_lid];
								INT id = tint_function_param.element_item_lid[element_index + 1];
								result = tint->UpdateText(NULL, id);
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
INT  Model::Tint_Function_AddText(
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
		CHAR group = tint_function_param.hash_element_lid_to_group_lid[element_lid];
		INT element_index = tint_function_param.hash_element_lid_to_index[element_lid];
		switch (group)
		{
			// ---- parameters
				case 'P':
				{
						INT id = tolower(element_lid);
						switch ( id )
						{

							case 'a':  // note: parameter lid 'A' is lid 'a', etc.
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
										Model *model = (Model *)handle_model;
										vs_system::Tint *tint = model->pi_model->tint_setng;


									// ---- update label

										// setup parameters
										INT result = VS_OK;
										SetParamGroup(surface_02::Model::PARAM_GROUP_FUNCTION);
										#include "../../surface_shared/hpp_param/param_setup.hpp"

										// update keypad
										Kpad_UpdateByParameter(handle_model, GetParamGroup());

										// update parameter labels 'a' to 'h' (index == 0 to 7)
										INT index = id - (INT)'a';
										BOOL ms = tint->ElemDsplTextTidy(
													id,
													char_param_array_labels[index],
													param_array_values[index],
													text
												);

										// cleanup parameters
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
INT  Model::Tint_Function_AddValue(
		const INT  handle_obj_config, // (Model *) for _Function_ methods
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

