////////////////////////////////////////////////////////////////////////////////

// ---------- s3_tint_8_callback_settings.hpp ----------
/*!
\file s3_tint_8_callback_settings.hpp
\brief
\author Gareth Edwards
*/

#pragma once

using namespace surface_03;


// -------- Functionality and Parameters for Tabbed Interface Acquire

class Model::Tint_Measure_Param
{

public:

	// ---- cdtor
		Tint_Measure_Param::Tint_Measure_Param() { ; };
		Tint_Measure_Param::~Tint_Measure_Param() { ; };


	// ---- enum
		enum
		{
			MAX_ITEMS = 17
		};


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
								(UINT)TINT_BODY_MEASURE_LID,	// [in]
								 vs_system::Tint::MAX_BODY_ITEM,// [in]
								element_lid_char,               // [in]
								group_lid_char,                 // [in]
								element_dspl_info,              // [out]
								hash_element_lid_to_dspl_index  // [out]
							);
						 float fred = 2;
				}

			return VS_OK;
		}


	// ---- properties


	// ---- element display list information
		INT  element_dspl_count = 0;
		vs_system::TintElemDsplInfo element_dspl_info[vs_system::Tint::MAX_BODY_ELM];


	// ---- element context
		// BOOL init_dspl_elm_context_list = TRUE;
		//INT element_id_index[MAX_ITEMS];
		//vs_system::ElmContext *element_id_context[MAX_ITEMS];


	// ---- codex
		CHAR element_lid_char[MAX_ITEMS] =
		{
			't',
			'0', '1', '2', '3', '4', '5', '6',
			'A', 'a',
			'B', 'b',
			'C', 'c',
			'D', 'd',
			'r',
		};

		CHAR group_lid_char[MAX_ITEMS] =
		{
			'T',
			'F', 'F', 'F', 'F', 'F', 'F', 'F',
			'P', 'P',
			'P', 'P',
			'P', 'P',
			'P', 'P',
			'R',
		};

		enum
		{
			ELEMENT_PARAM_FIRST = 'A',
			ELEMENT_PARAM_LAST = 'D',

			SOURCE_KEYPAD  = 1,
			SOURCE_BUTTON  = 2,
			SOURCE_MESSAGE = 3

		};

		// ---- source
		//
		// note: set in Tint_Measure_UpdateElement and
		//       used in Tint_Measure_AddText...
		//
		INT process_mode = SOURCE_BUTTON;


} tint_measure_param;


// ----------------------------------------------------------------------
INT Model::Tint_Measure_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_Measure_BuildList;
	tint_callback->callback_get_icon_info  = Tint_Measure_GetIconInfo;
	tint_callback->callback_update_model   = Tint_Measure_UpdateModel;
	tint_callback->callback_update_element = Tint_Measure_UpdateElement;
	tint_callback->callback_add_actions    = NULL;
	tint_callback->callback_add_text       = Tint_Measure_AddText;
	tint_callback->callback_add_value      = Tint_Measure_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Measure_BuildList(
		vs_system::TintItem li[]
	)
{

	// ---- local
		FLOAT lincr = 12;
		INT   result = 0;


	// ---- local for TINT_ macros
		UINT gid = TINT_BODY_MEASURE_LID;
		UINT noi = 0;

		
	// ---- mesh param
		SetParamGroup(surface_03::Model::PARAM_GROUP_MEASURE);
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
				TINT_SET_TARCX(Tint::ITEM_RADIO,   4, 6, 5, 5.5f)
				TINT_ADD_ICON()
				TINT_SET_TARCXLI(Tint::ITEM_RADIO, 4, 6, 6, 6.6f, TRUE, lincr)

			// ---- x offset
				FLOAT x_off = 1.5;
				gid = TINT_BODY_MEASURE_PARAM_LID;

			// ---- param 1 : Rotation
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 0)
				TINT_ADD_ITEM(char_param_array_labels[0])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr / 2)

			// ---- param 2 : X
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 1)
				TINT_ADD_ITEM(char_param_array_labels[1])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr / 2)

			// ---- param 3 : Y
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 2)
				TINT_ADD_ITEM(char_param_array_labels[2])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr / 2)

			// ---- param 4 : Z
				TINT_ADD_ICON()
				TINT_SET_TARC(Tint::ITEM_RADIO,     5, 7, 3)
				TINT_ADD_ITEM(char_param_array_labels[3])
				TINT_SET_TARCXLI(Tint::ITEM_TEXT,   0, 0, 0, x_off, TRUE, lincr / 2)

			// ---- reset
				TINT_ADD_ITEM("Reset")
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_CLICK, 5, 5, 6, LAST_LINE_SPACING_0, TRUE, lincr)

			// ---- set total number of items
				TINT_SET_TOTAL_NOI();

		}


		// ---- set item id's
			for (UINT i = 0; i < noi; i++)
			{
				li[i].elem_id = (UINT)tint_measure_param.element_lid_char[i];
			}
		

		// ---- set function & parameter status
			for (UINT i = 0; i < noi; i++)
			{
				CHAR group_id = tint_measure_param.group_lid_char[i];
				CHAR elem_id = tint_measure_param.element_lid_char[i];


				if ( group_id == 'F' )
				{
					INT group_index = elem_id - '0';
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
INT Model::Tint_Measure_GetIconInfo(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 4;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 7;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
		default:
			break;
	}
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Measure_UpdateModel(
		INT  handle_model,
		INT  element_id
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_setng;
		model::Surface_03 *surface_03 = s_pi_model->surface_03;


	// ---- get tint display list element info
		result = tint_measure_param.GetElementDsplParam(handle_model);


	// ---- get tint display list element contexts
		//if ( tint_measure_param.init_dspl_elm_context_list )
		//{
		//	result = tint->GetBodyDsplElmContext(
		//			(UINT)TINT_BODY_MEASURE_LID,
		//			tint_measure_param.MAX_ITEMS,
		//			tint_measure_param.element_lid_char,
		//			tint_measure_param.element_id_index,
		//			tint_measure_param.element_id_context
		//		);
		//	tint_measure_param.init_dspl_elm_context_list = FALSE;
		//}


	// ---- param
		SetParamGroup(surface_03::Model::PARAM_GROUP_MEASURE);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- process
		UINT number_of_element_ids = sizeof(tint_measure_param.element_lid_char) / sizeof(CHAR);
		for (UINT i = 0; i < number_of_element_ids; i++)
		{

			// ---- match ?
				if ( tint_measure_param.element_lid_char[i] == (CHAR)element_id )
				{

					// ---- group
						CHAR group = tint_measure_param.group_lid_char[i];

					// ---- update, either:
						//
						//    'F' - choose a different function
						//    'P' - send a new set of parameters to the keypad
						//    'R'eset  
						//
						switch ( group) 
						{
							// ---- parameter index
								case 'F' :
									{

										// ---- update the loi graphics (really important)
											result = Tint_Explore_UpdateModel(handle_model, element_id);

										INT group_index = element_id - '0';
										result = SetParamGroupIndex(group_index);
										for (INT i = 0; i < 7; i++)
										{
											INT param_id = i + 'A';
											result = Tint_Measure_UpdateElement(handle_model, param_id);
										}
										surface_03->UpdateHeightfieldObjects((INT)'M', surface_03, FALSE);
									}
									break;

							// ---- parameter
								case 'P':
									{
										INT param_index = element_id - 'A';
										result = SetParamArrayIndex(param_index);

										result = tint->SetSelectedBody(KPAD_BODY_INPUT_LID);
										Kpad_UpdateByParameter(handle_model, GetParamGroup());
										Kpad_Input_UpdateElement(handle_model, (INT)'P');
										result = tint->SetSelectedBody(TINT_BODY_MEASURE_LID);
									}
									break;

							// ---- reset
								case 'R':
									result = SetParamArrayDefaults(PARAM_GROUP_MEASURE);
									for (INT i = 0; i < 4; i++)
									{
										INT param_id = i + 'A';
										result = Tint_Measure_UpdateElement(handle_model, param_id);
									}
									surface_03->UpdateHeightfieldObjects((INT)'M', surface_03, FALSE);
									break;

								default:
									break;
						}
				}
		}

	// ---- param
		#include "../../surface_shared/hpp_param/param_cleanup.hpp"

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Measure_UpdateElement(
		INT handle_model,
		INT element_id
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_BODY_MEASURE_LID);

		 
	// ---- Source (invoked by method):
		//  - Kontrol::TintUpdateBody
		//  - Model::Tint_Measure_UpdateModel
		//
		tint_measure_param.process_mode = Tint_Measure_Param::SOURCE_BUTTON;


	// ---- if zero element_id, note source is Msg, and process all valid buttons
		if ( element_id == 0 )
		{
			// Model::SetMsgCameraControl
			tint_measure_param.process_mode = Tint_Measure_Param::SOURCE_MESSAGE;
		}
	// ---- else if negative, note source is keypad, and flip
		else if (element_id < 0)       
		{
			// Model::Kpad_SetParameter
			element_id = -element_id;
			tint_measure_param.process_mode = Tint_Measure_Param::SOURCE_KEYPAD;
		}
		

	// ---- shortcuts
		UINT process_mode = tint_measure_param.process_mode;

	// ---- process
		UINT number_of_element_ids = sizeof(tint_measure_param.element_lid_char) / sizeof(CHAR);
		for (UINT i = 0; i < number_of_element_ids; i++)
		{

			// ---- update all param text?
				CHAR eid = tint_measure_param.element_lid_char[i];


			// ---- process all ?
				if ( process_mode = Tint_Measure_Param::SOURCE_MESSAGE )
				{
					// if valid element_id (see logic below) process all valid buttons, else zap
					if ( eid >= tint_measure_param.ELEMENT_PARAM_FIRST &&
							eid <= tint_measure_param.ELEMENT_PARAM_LAST )
					{
						element_id = eid;
					}
					else
					{
						element_id = 0;
					}
				}


			// ---- update
				if ( tint_measure_param.element_lid_char[i] == (CHAR)element_id )
				{

					// ---- parameter group
						CHAR group = tint_measure_param.group_lid_char[i];
						switch (group)
						{
							case 'P':
								{
									// ---- valid element id
										switch ( element_id )
										{
											case 'A':
											case 'B':
											case 'C':
											case 'D':
												{
													INT id = tint_measure_param.element_lid_char[i + 1];
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

					// ---- update all ? ( source == Model::SetMsgCameraControl )
						if ( process_mode = Tint_Measure_Param::SOURCE_MESSAGE )
						{
							if ( element_id == tint_measure_param.ELEMENT_PARAM_LAST ) goto reset_source_and_return;
						}
						else
						{
							goto reset_source_and_return;
						}
				}
		}


	// ---- reset source
		reset_source_and_return:
			tint_measure_param.process_mode = Tint_Measure_Param::SOURCE_BUTTON;

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Measure_AddAction(
		const INT  handle_obj_config,
		const INT  element_id,
		FLOAT      action_values[]
	)
{ 
	return 0;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Measure_AddText(
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


	// ---- mesh param
		SetParamGroup(surface_03::Model::PARAM_GROUP_MEASURE);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- update
		Kpad_UpdateByParameter(handle_model, GetParamGroup());


	// ---- process
		UINT number_of_element_ids = sizeof(tint_measure_param.element_lid_char) / sizeof(CHAR);
		for (UINT i = 0; i < number_of_element_ids; i++)
		{
				if ( tint_measure_param.element_lid_char[i] == (CHAR)element_id )
				{
						CHAR group = tint_measure_param.group_lid_char[i];
						switch (group)
						{
							case 'P':
								{

									// ---- catch upper case
										INT char_id = tolower(element_id);
										INT index = char_id - (INT)'a';

									// ---- update;
										switch ( char_id )
										{
											case 'a':
											case 'b':
											case 'c':
											case 'd':
												{
													*replace_text = TRUE;
													sprintf(text, "%s: %.3f", char_param_array_labels[index], param_array_values[index]);
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
INT  Model::Tint_Measure_AddValue(
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


// ----------------------------------------------------------------------

INT Model::Tint_Measure_Switch(
		INT  handle_model,
		INT  group_lid,
		INT  element_lid,
		BOOL state
	)
{

	if ( group_lid == 0 ) return VS_ERROR;

	// ---- invoked from Tint_Explore_UpdateModel, so by-passes
	//       being invoked in Tint_Measure_UpdateModel
		INT result = tint_measure_param.GetElementDsplParam(handle_model);

	if ( element_lid == 0 )
	{
		for (int e = 0; e < tint_measure_param.element_dspl_count; e++)
		{
			if ( group_lid == tint_measure_param.element_dspl_info[e].group_lid )
				tint_measure_param.element_dspl_info[e].context->SetSwitchedOn(state);
		}
	}
	else
	{
		for (int e = 0; e < tint_measure_param.element_dspl_count; e++)
		{
			if (
					group_lid == tint_measure_param.element_dspl_info[e].group_lid &&
					element_lid == tint_measure_param.element_dspl_info[e].lid
				)
			{
				tint_measure_param.element_dspl_info[e].context->SetSwitchedOn(state);
			}
		}
	}

	return VS_OK;
}


// ----------------------------------------------------------------------

/*

	// ---- imaging
		FLOAT width, height, depth;
		surface_03->GetAcquireSurfaceDimensions(&width, &height, &depth);

	// ---- surface
		FLOAT y_min = surface_03->GetAcquireHeightFieldMin();
		FLOAT y_max = surface_03->GetAcquireHeightFieldMax();
		FLOAT y_hgt = y_max - y_min;

	// ---- section
		UINT  sect_index  = surface_03->GetTargetLoiIndex();
		FLOAT sect_findex = (FLOAT)sect_index;
		FLOAT sect_length = surface_03->GetTargetLoiSectionLength();
		FLOAT sect_height = surface_03->GetTargetLoiSectionHeight();
		FLOAT sect_rotate = surface_03->GetTargetLoiRotation(sect_index);
		VsVector3 sect_v; surface_03->GetTargetLoiForDisplay(&sect_v);


	// ---- local
		FLOAT value = 0;
		CHAR  ctext[128];  
		CHAR  *format = NULL;


	// ---- process
		UINT number_of_element_ids = sizeof(tint_measure_param.element_lid_char) / sizeof(CHAR);
		for (UINT i = 0; i < number_of_element_ids; i++)
		{

			CHAR group_lid = tint_measure_param.group_lid_char[i];;

			switch ( group_lid )
			{
				case 'M':
				case 'A':
				case 'B':
				case 'C':
				case 'D':
					{
						CHAR element_lid = tint_measure_param.element_lid_char[i];
						std::string name = tint_measure_param.element_dspl_info[i].context->GetName();
						INT len = name.length();
						if ( name.back() != ':' )
						{
							CHAR element_lid = tint_measure_param.element_lid_char[i];
							switch (element_lid)
							{
								case tint_measure_param.ELEMENT_LID_IMAGING_WIDTH    : value = depth / 10;       break;
								case tint_measure_param.ELEMENT_LID_IMAGING_HEIGHT   : value = depth / 10;       break;

								case tint_measure_param.ELEMENT_LID_SURFACE_HEIGHT   : value = y_hgt / 10;       break;
								case tint_measure_param.ELEMENT_LID_SURFACE_MIN      : value = y_min / 10;       break;
								case tint_measure_param.ELEMENT_LID_SURFACE_MAX      : value = y_max / 10;       break;

								case tint_measure_param.ELEMENT_LID_SECTION_INDEX    : value = sect_findex;      break;
								case tint_measure_param.ELEMENT_LID_SECTION_WIDTH    : value = sect_length / 10; break;
								case tint_measure_param.ELEMENT_LID_SECTION_HEIGHT   : value = sect_height / 10; break;
								case tint_measure_param.ELEMENT_LID_SECTION_ROTATION : value = sect_rotate;      break;
								case tint_measure_param.ELEMENT_LID_SECTION_X        : value = sect_v.x;         break;
								case tint_measure_param.ELEMENT_LID_SECTION_Y        : value = sect_v.y;         break;
								case tint_measure_param.ELEMENT_LID_SECTION_Z        : value = sect_v.z;         break;

								default: break;
							}

							CHAR *format = element_lid == tint_measure_param.ELEMENT_LID_SECTION_INDEX ? " %.0f" : " %.3f";
							sprintf(ctext, format, value);

							name += ctext;
							INT result = tint_measure_param.element_dspl_info[i].context->SetText(name);
						}
					}
					break;

				default:
						break;
			}
		}


*/


/*


	// ---- item element logical id's
		enum
		{
			ELEMENT_LID_IMAGING          = '0',
			ELEMENT_LID_IMAGING_WIDTH    = '1',
			ELEMENT_LID_IMAGING_HEIGHT   = '2',
			ELEMENT_LID_SURFACE          = '3',
			ELEMENT_LID_SURFACE_HEIGHT   = '4',
			ELEMENT_LID_SURFACE_MIN      = '5',
			ELEMENT_LID_SURFACE_MAX      = '6',
			ELEMENT_LID_SECTION          = '7',
			ELEMENT_LID_SECTION_INDEX    = '8',
			ELEMENT_LID_SECTION_WIDTH    = '9',
			ELEMENT_LID_SECTION_HEIGHT   = ';',
			ELEMENT_LID_PARAMETERS       = 'P',
			ELEMENT_LID_SECTLAB_ROTATION = 'A',
			ELEMENT_LID_SECTION_ROTATION = 'a',
			ELEMENT_LID_SECTLAB_X        = 'B',
			ELEMENT_LID_SECTION_X        = 'b',
			ELEMENT_LID_SECTLAB_Y        = 'C',
			ELEMENT_LID_SECTION_Y        = 'c',
			ELEMENT_LID_SECTLAB_Z        = 'D',
			ELEMENT_LID_SECTION_Z        = 'd'
		};
		CHAR element_lid_char[vs_system::Tint::MAX_BODY_ITEM] =
		{
			ELEMENT_LID_IMAGING,
			ELEMENT_LID_IMAGING_WIDTH,
			ELEMENT_LID_IMAGING_HEIGHT,
			ELEMENT_LID_SURFACE,
			ELEMENT_LID_SURFACE_HEIGHT,
			ELEMENT_LID_SURFACE_MIN,
			ELEMENT_LID_SURFACE_MAX,
			ELEMENT_LID_SECTION,
			ELEMENT_LID_SECTION_INDEX,
			ELEMENT_LID_SECTION_WIDTH,
			ELEMENT_LID_SECTION_HEIGHT,
			ELEMENT_LID_PARAMETERS,
			ELEMENT_LID_SECTLAB_ROTATION,
			ELEMENT_LID_SECTION_ROTATION,
			ELEMENT_LID_SECTLAB_X,
			ELEMENT_LID_SECTION_X,
			ELEMENT_LID_SECTLAB_Y,
			ELEMENT_LID_SECTION_Y,
			ELEMENT_LID_SECTLAB_Z,
			ELEMENT_LID_SECTION_Z
		};

	// ---- items group logical id's
		CHAR group_lid_char[vs_system::Tint::MAX_BODY_ITEM] =
		{
		//   I   W,  H,  S,  H,  M,  M,  S,  I,  W,  H,  P,  R,      X,      Y,      Z
			'M','M','M','M','M','M','M','M','M','M','M','P','A','a','B','b','C','c','D','d'
		};


*/


/*

// ---- local
FLOAT lincr = 12;
FLOAT sincr = lincr / 20;
INT   result = 0;
FLOAT x_off = 1.5;

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


		// ---- imaging
		TINT_ADD_ITEM("Imaging (Calibration):")
		TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 0.0f, TRUE, sincr)
		TINT_ADD_ITEM("Width")
		TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, x_off, TRUE, sincr)
		TINT_ADD_ITEM("Depth")
		TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, x_off, TRUE, lincr / 2)

		// ---- surface
		TINT_ADD_ITEM("Surface (Calculated):")
		TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 0.0f, TRUE, sincr)
		TINT_ADD_ITEM("Height")
		TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, x_off, TRUE, sincr)
		TINT_ADD_ITEM("Min")
		TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, x_off, TRUE, sincr)
		TINT_ADD_ITEM("Max")
		TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, x_off, TRUE, lincr / 2)

		// ---- section
		TINT_ADD_ITEM("Location of Interest:")
		TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 0.0f, TRUE, sincr)
		TINT_ADD_ITEM("Index")
		TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, x_off, TRUE, sincr)
		TINT_ADD_ITEM("Section Length")
		TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, x_off, TRUE, sincr)
		TINT_ADD_ITEM("Section Height")
		TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, x_off, TRUE, lincr / 2)

		// ---- parameter groups text
		TINT_ADD_ITEM("Location Parameters: ")
		TINT_SET_TARCXLI(vs_system::Tint::ITEM_TEXT, 0, 0, 0, 0.0f, TRUE, lincr / 8)

		// ---- param 1
		gid = TINT_BODY_ACQUIRE_PARAM_LID;
	TINT_ADD_ICON()
		TINT_SET_TARCX(Tint::ITEM_RADIO, 5, 7, 0, x_off)
		TINT_ADD_ITEM("Rotation")
		TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, x_off * 2, TRUE, lincr / 2)

		// ---- param 2
		TINT_ADD_ICON()
		TINT_SET_TARCX(Tint::ITEM_RADIO, 5, 7, 1, x_off)
		TINT_ADD_ITEM("X")
		TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, x_off * 2, TRUE, lincr / 2)

		// ---- param 2
		TINT_ADD_ICON()
		TINT_SET_TARCX(Tint::ITEM_RADIO, 5, 7, 2, x_off)
		TINT_ADD_ITEM("Y")
		TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, x_off * 2, TRUE, lincr / 2)

		// ---- param 3
		TINT_ADD_ICON()
		TINT_SET_TARCX(Tint::ITEM_RADIO, 5, 7, 3, x_off)
		TINT_ADD_ITEM("Z")
		TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, x_off * 2, TRUE, lincr)


		// ---- get 
		TINT_SET_TOTAL_NOI();

}

									INT id = element_id;
									switch ( element_id )
									{
										case 'A':
										case 'B':
										case 'C':
										case 'D':
											{
												id = tint_measure_param.element_lid_char[i + 1];
											}
										case 'a':
										case 'b':
										case 'c':
										case 'd':
											{
												*replace_text = TRUE;
												INT index = id - (INT)'a';
												if ( strcmp(char_param_array_labels[index], "NA") != 0 )
												{

													FLOAT value = param_array_values[index];
													switch (id)
													{
														case 'a': break;
														case 'b': value = value / 10 + 5;        break;
														case 'c': value = value / 10;            break;
														case 'd': value = 10 - (value / 10 + 5); break;
													}

													sprintf(text,
																"%s: %.3f",
																char_param_array_labels[index],
																value
															);

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

*/