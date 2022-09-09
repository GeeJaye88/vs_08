////////////////////////////////////////////////////////////////////////////////

// ---------- s1_tint_callback_colour.hpp ----------
/*!
\file s1_tint_callback_colour.hpp
\brief
\author Gareth Edwards
*/


#pragma once

using namespace surface_01;


// ----------------------------------------------------------------------
INT Model::Tint_Colour_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_Colour_BuildList;
	tint_callback->callback_get_icon_info  = Tint_Colour_GetIconInfo;
	tint_callback->callback_update_model   = Tint_Colour_UpdateModel;
	tint_callback->callback_update_element = Tint_Colour_UpdateElement;
	tint_callback->callback_add_actions    = NULL;
	tint_callback->callback_add_text       = Tint_Colour_AddText;
	tint_callback->callback_add_value      = Tint_Colour_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Colour_BuildList(
		vs_system::TintItem li[]
	)
{

	// ---- describe
		INT number_of_items = 1;
		UINT gid = TINT_BODY_COLOUR_LID;


	// ---- row 1
		li[0].Set("",     gid,  1);
	

	// --- detail
		li[0].SetAtlas(4, 2, 0);   li[0].SetXOff(0);   li[0].SetLF(TRUE);


	// ---- type
		li[0].SetType(vs_system::Tint::ITEM_TEXT);
	
	return number_of_items;
}


// ----------------------------------------------------------------------
INT Model::Tint_Colour_GetIconInfo(INT id)
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
INT  Model::Tint_Colour_UpdateModel(
		INT  handle_model,
		INT  element_id
	)
{
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Colour_UpdateElement(
		INT handle_model,
		INT element_id
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_BODY_COLOUR_LID);

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Colour_AddText(
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
INT  Model::Tint_Colour_AddValue(
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
