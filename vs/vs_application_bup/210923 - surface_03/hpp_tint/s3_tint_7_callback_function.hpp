////////////////////////////////////////////////////////////////////////////////

// ---------- s3_tint_4_callback_function.hpp ----------
/*!
\file s3_tint_4_callback_function.hpp
\brief
\author Gareth Edwards
*/

#pragma once

using namespace surface_03;


// -------- Functionality and Parameters for Tabbed Keypad Input --------

class Model::Tint_Function_Param
{

public:

	// ---- enum
		enum
		{
			MAX_ITEMS = 24
		};


	// ---- cdtor
		Tint_Function_Param()
		{
			;
		}
		~Tint_Function_Param()
		{
			;
		}


	// ---- element context
		BOOL init_dspl_elm_context_list = TRUE;
		INT element_id_index[MAX_ITEMS];
		vs_system::ElmContext *element_id_context[MAX_ITEMS];


	// ---- codex
		CHAR element_lid_char[MAX_ITEMS] =
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
		CHAR group_lid_char[MAX_ITEMS] =
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


	// ---- local for TINT_ macros
		UINT gid = TINT_BODY_FUNCTION_LID;
		UINT noi = 0;

		
	// ---- mesh param
		SetParamGroup(surface_03::Model::PARAM_GROUP_FUNCTION);
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
				TINT_SET_TARC(Tint::ITEM_RADIO,    4, 6, 0)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO,   4, 6, 1, 1.1f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO,   4, 6, 2, 2.2f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO,   4, 6, 3, 3.3f)
				TINT_ADD_ICON()
				TINT_SET_TARCX(Tint::ITEM_RADIO,   4, 6, 4, 4.4f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_RADIO, 4, 6, 5, 5.5f, TRUE, lincr)

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


		// ---- set item id's
			for (UINT i = 0; i < noi; i++)
			{
				li[i].elem_id = (UINT)tint_function_param.element_lid_char[i];
			}
		

		// ---- set function status
			for (UINT i = 0; i < noi; i++)
			{
				CHAR group_id = tint_function_param.group_lid_char[i];
				CHAR elem_id = tint_function_param.element_lid_char[i];


				if ( group_id == 'F' )
				{
					INT group_index = elem_id - '1';
					if ( group_index == param_group_index )
					{
						li[i].SetStatus(TRUE);
						// don't break... not done...
					}
				}
				else if ( group_id == 'P' )
				{
					if (strcmp(li[i + 1].text, "NA") == 0)
					{
						li[i].SetType(vs_system::Tint::ITEM_NULL);
					}
					else
					{
						INT param_index = elem_id - 'A';
						if (param_index == param_array_index)
						{
							li[i].SetStatus(TRUE);
						}
					}
				}
			}


	// ---- mesh param
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
		INT  element_id
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_setng;
		model::Surface_03 *surface_03 = s_pi_model->surface_03;


	// ---- get tint display list element contexts
		if (tint_function_param.init_dspl_elm_context_list )
		{
			result = tint->GetBodyDsplElmContext(
					(UINT)TINT_BODY_FUNCTION_LID,
					tint_function_param.MAX_ITEMS,
					tint_function_param.element_lid_char,
					tint_function_param.element_id_index,
					tint_function_param.element_id_context
				);
			tint_function_param.init_dspl_elm_context_list = FALSE;
		}


	// ---- mesh param
		SetParamGroup(surface_03::Model::PARAM_GROUP_FUNCTION);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- process
		UINT number_of_element_ids = sizeof(tint_function_param.element_lid_char) / sizeof(CHAR);
		for (UINT i = 0; i < number_of_element_ids; i++)
		{
			if ( tint_function_param.element_lid_char[i] == (CHAR)element_id )
			{
				CHAR group = tint_function_param.group_lid_char[i];

				// Update, either:
				//
				//    'F' - choose a different function
				//    'P' - send a new set of parameters to the keypad
				//    'S'ave, 'R'eset or 'I'ncrement
				//
				//tint_function_interface.Update(group, element_id);

				switch ( group) 
				{

					case 'F' :
						{
							INT group_index = element_id - '1';
							result = SetParamGroupIndex(group_index);
							for (INT i = 0; i < 6; i++)
							{
								INT param_id = i + 'A';
								result = Tint_Function_UpdateElement(handle_model, param_id);
							}
							surface_03->UpdateHeightfieldObjects(0, surface_03, FALSE);
						}
						break;

					case 'P':
						{
							INT param_index = element_id - 'A';
							result = SetParamArrayIndex(param_index);

							result = tint->SetSelectedBody(KPAD_BODY_INPUT_LID);
							Kpad_UpdateByParameter(handle_model, GetParamGroup());
							Kpad_Input_UpdateElement(handle_model, (INT)'P');
							result = tint->SetSelectedBody(TINT_BODY_FUNCTION_LID);
						}
						break;

					case 'R':
						result = SetParamArrayDefaults(PARAM_GROUP_FUNCTION);
						for (INT i = 0; i < 8; i++)
						{
							INT param_id = i + 'A';
							result = Tint_Function_UpdateElement(handle_model, param_id);
						}
						surface_03->UpdateHeightfieldObjects(0, surface_03, FALSE);
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
INT  Model::Tint_Function_UpdateElement(
		INT handle_model,
		INT element_id
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		model::Surface_03 *surface_03 = model->pi_model->surface_03;
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_BODY_FUNCTION_LID);


	// ---- process
		UINT number_of_element_ids = sizeof(tint_function_param.element_lid_char) / sizeof(CHAR);
		for (UINT i = 0; i < number_of_element_ids; i++)
		{
				if ( tint_function_param.element_lid_char[i] == (CHAR)element_id )
				{
						CHAR group = tint_function_param.group_lid_char[i];
						switch (group)
						{
							case 'P':
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
												INT id = tint_function_param.element_lid_char[i + 1];
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
		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Function_AddText(
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
		SetParamGroup(surface_03::Model::PARAM_GROUP_FUNCTION);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- update
		Kpad_UpdateByParameter(handle_model, GetParamGroup());


	// ---- process
		UINT number_of_element_ids = sizeof(tint_function_param.element_lid_char) / sizeof(CHAR);
		for (UINT i = 0; i < number_of_element_ids; i++)
		{
			if ( tint_function_param.element_lid_char[i] == (CHAR)element_id )
			{
				CHAR group = tint_function_param.group_lid_char[i];
				switch (group)
				{
					case 'P':
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
										id = tint_function_param.element_lid_char[i + 1];
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
										*replace_text = TRUE;
										INT index = id - (INT)'a';
										if ( strcmp(char_param_array_labels[index], "NA") != 0 )
										{
											sprintf(text, "%s: %f", char_param_array_labels[index], param_array_values[index]);
											std::string s = tostring(text);
											strcpy(text, s.c_str());
											INT len = strlen(text);
											if ( text[len - 1] == '.')
											{
												text[len] = '0';
												text[len+1] = 0;
											}
										}
										else
										{
											strcpy(text, "");
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

