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
							vs_system::Tint::MAX_BODY_ITEM, // [in]
							element_item_lid_char,          // [in]
							group_lid_char,                 // [in]
							element_dspl_info,              // [out]
							hash_element_lid_to_dspl_index  // [out]
						);


				// ---- copy Text into Name to preserve "given" name for concatenation
					UINT number_of_element_ids = sizeof(tint_measure_param.element_item_lid_char) / sizeof(CHAR);
					for (UINT i = 0; i < number_of_element_ids; i++)
					{
						CHAR group_lid = tint_measure_param.group_lid_char[i];
						if ( group_lid != 'M' ) break;
						CHAR element_lid = tint_measure_param.element_item_lid_char[i];
						std::string text = tint_measure_param.element_dspl_info[i].context->GetText();
						INT result = tint_measure_param.element_dspl_info[i].context->SetName(text);
					}

			}

			return VS_OK;
		}

	// ---- properties


	// ---- element display list information
		INT  element_dspl_count = 0;
		vs_system::TintElemDsplInfo element_dspl_info[vs_system::Tint::MAX_BODY_ELM];


	// ---- item element logical id's
		CHAR element_item_lid_char[vs_system::Tint::MAX_BODY_ITEM] =
		{
			'a', // Imaging:
			'b', //    Width
			'c', //    Depth
			'd', // Surface:
			'e', //    Height
			'f', //    Min
			'g', //    Max
			'h', // Section:
			'i', //    Loi Id
			'j', //    Width
			'k', //    Height
			'l', //    Rotation
			'm', //    X
			'n', //    Y
			'o', //    Z
		};

	// ---- items group logical id's
		CHAR group_lid_char[vs_system::Tint::MAX_BODY_ITEM] =
		{
			'M'
		};

	// ----- kamera pan & tilt
		FLOAT pan = 0, tilt = 0;
		VsVector3 position  = { 0, 0, 0 };

	// ---- target
		BOOL target_status  = 0;
		BOOL target_measure = 0;
		UINT target_object_id = 0;

	// ---- target loi
		UINT target_loi_index = 0;


} tint_measure_param;


// ----------------------------------------------------------------------
INT Model::Tint_Measure_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_Measure_BuildList;
	tint_callback->callback_get_icon_info  = Tint_Measure_GetIconInfo;
	tint_callback->callback_update_model   = Tint_Measure_UpdateModel;
	tint_callback->callback_update_element = Tint_Measure_UpdateElement;
	tint_callback->callback_add_actions    = Tint_Measure_AddAction;
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
	FLOAT lincr = 10;
	FLOAT sincr = lincr / 8;
	INT   result = 0;
	INT   noi = 0;
	UINT  gid = TINT_BODY_EXPLORE_LID;


	// ---- build
	{

		// ---- scope tint def & enum
			using namespace vs_system;


		// ---- set
			TINT_SET_START_NOI()


		// ---- imaging
			TINT_ADD_ITEM("Imaging:")
			TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 0.0f, TRUE, sincr)
			TINT_ADD_ITEM("Width")
			TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 1.0f, TRUE, sincr)
			TINT_ADD_ITEM("Depth")
			TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 1.0f, TRUE, lincr)

		// ---- surface
			TINT_ADD_ITEM("Surface:")
			TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 0.0f, TRUE, sincr)
			TINT_ADD_ITEM("Height")
			TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 1.0f, TRUE, sincr)
			TINT_ADD_ITEM("Min")
			TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 1.0f, TRUE, sincr)
			TINT_ADD_ITEM("Max")
			TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 1.0f, TRUE, lincr)

		// ---- section
			TINT_ADD_ITEM("Section:")
			TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 0.0f, TRUE, sincr)
			TINT_ADD_ITEM("Index")
			TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 1.0f, TRUE, sincr)
			TINT_ADD_ITEM("Length")
			TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 1.0f, TRUE, sincr)
			TINT_ADD_ITEM("Height")
			TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 1.0f, TRUE, sincr)
			TINT_ADD_ITEM("Rotation")
			TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 1.0f, TRUE, sincr)
			TINT_ADD_ITEM("X")
			TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 1.0f, TRUE, sincr)
			TINT_ADD_ITEM("Y")
			TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 1.0f, TRUE, sincr)
			TINT_ADD_ITEM("Z")
			TINT_SET_TARCXLI(Tint::ITEM_TEXT, 0, 0, 0, 1.0f, TRUE, lincr)

		// ---- get 
			TINT_SET_TOTAL_NOI();

	}


	// ---- set item element id's & set all to belong to same group!!!
		for (INT i = 0; i < noi; i++)
		{
			li[i].elem_id = (UINT)tint_measure_param.element_item_lid_char[i];
			tint_measure_param.group_lid_char[i] = 'M';
		}

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
		model::Surface_03 *surface_03 = s_pi_model->surface_03;
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- get tint display list element info
		result = tint_measure_param.GetElementDsplParam(handle_model);


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
		VsVector3 *sect_v = surface_03->GetTargetLoi();
		FLOAT sect_y      = surface_03->GetTargetLoiLocationHeight(sect_index);

	// ---- local 
		FLOAT value = 0;
		CHAR  ctext[128];
		CHAR  *format = NULL;


	// ---- process
		UINT number_of_element_ids = sizeof(tint_measure_param.element_item_lid_char) / sizeof(CHAR);
		for (UINT i = 0; i < number_of_element_ids; i++)
		{

			CHAR group_lid = tint_measure_param.group_lid_char[i];
			if ( group_lid != 'M' ) break;

			switch ( group_lid ) 
			{
				case 'M':
					{
						CHAR element_lid = tint_measure_param.element_item_lid_char[i];
						std::string name = tint_measure_param.element_dspl_info[i].context->GetName();
						INT len = name.length();
						if ( name.back() != ':' )
						{
							CHAR element_lid = tint_measure_param.element_item_lid_char[i];
							switch (element_lid)
							{
								// ---- imaging
									case 'b': value = width/10;       break; // ok
									case 'c': value = depth/10;       break; // ok

								// ---- surface
									case 'e': value = y_hgt/10;       break; // ok
									case 'f': value = y_min/10;       break; // ok
									case 'g': value = y_max/10;       break; // ok

								// ---- section
									case 'i': value = sect_findex;    break; // ok
									case 'j': value = sect_length/10; break;
									case 'k': value = sect_height/10; break;
									case 'l': value = sect_rotate;    break; // ok
									case 'm': value = sect_v->x/10 + 5;   break; // ok
									case 'n': value = sect_y/10;      break;
									case 'o': value = 10 - ( sect_v->z/10 + 5);   break; // ok
							}

							CHAR *format = element_lid == 'i' ? " %.0f" : " %.2f";
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

	return VS_ELM_SETUP_NONE;

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
		const INT  handle_obj_config,
		const INT  element_id,
		BOOL      *replace_text,
		BOOL      *add_text,
		CHAR      *text
	)
{
	*add_text = FALSE;
	*replace_text = FALSE;
	*text = NULL;

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
