////////////////////////////////////////////////////////////////////////////////

// ---------- s3_tint_7_callback_bcs.hpp ----------
/*!
\file s3_tint_7_callback_bcs.hpp
\brief
\author Gareth Edwards
*/

#pragma once

using namespace surface_03;


// ----------------------------------------------------------------------

class Model::Tint_Enhance_Param
{

public:

	// ---- cdtor
		Tint_Enhance_Param::Tint_Enhance_Param()
		{
			;
		}
		Tint_Enhance_Param::~Tint_Enhance_Param()
		{
			;
		}

} tint_enhance_param;


// ----------------------------------------------------------------------
INT Model::Tint_Enhance_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_Enhance_BuildList;
	tint_callback->callback_get_icon_info  = Tint_Enhance_GetIconInfo;
	tint_callback->callback_update_model   = Tint_Enhance_UpdateModel;
	tint_callback->callback_update_element = Tint_Enhance_UpdateElement;
	tint_callback->callback_add_actions    = NULL;
	tint_callback->callback_add_text       = Tint_Enhance_AddText;
	tint_callback->callback_add_value      = Tint_Enhance_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Enhance_BuildList(
		vs_system::TintItem li[]
	)
{
	return 0;
}


// ----------------------------------------------------------------------
INT Model::Tint_Enhance_GetIconInfo(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 4;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 6;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
		default:
			break;
	}
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Enhance_UpdateModel(
		INT  handle_model,
		INT  element_id
	)
{
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Enhance_UpdateElement(
		INT handle_model,
		INT element_id
	)
{
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Enhance_AddText(
		const INT  handle_obj_config,
		const INT  element_id,
		BOOL      *replace_text,
		BOOL      *add_text,
		CHAR      *text
	)
{
	*add_text     = FALSE;
	*replace_text = FALSE;
	*text         = NULL;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Enhance_AddValue(
		const INT  handle_obj_config,
		const INT  element_id,
		BOOL      *add_value,
		FLOAT     *value,
		CHAR      *suffix
	)
{
	*add_value = FALSE;
	return VS_OK;
}
