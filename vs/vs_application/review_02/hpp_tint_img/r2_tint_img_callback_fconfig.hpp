////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tool_callback_fconfig.hpp ----------
/*!
\file r2_tool_callback_fconfig.hpp
\brief
\author Gareth Edwards
*/


#pragma once

using namespace review_02;


// -------- Functionality and Parameters for Tabbed Interface :Tint_ImgPtz_[...]

class Model::Tint_Fconfig_Param
{

public:

	// ---- cdtor
		Tint_Fconfig_Param::Tint_Fconfig_Param() { ; };
		Tint_Fconfig_Param::~Tint_Fconfig_Param() { ; };

	// --- 
		enum
		{
			NUM_KEYPAD_ITEMS = 12,

			NUM_MARK_GROUPS = 4,
			NUM_MARK_ITEMS = 3,

			SAVE_ID  = NUM_KEYPAD_ITEMS + (NUM_MARK_GROUPS * NUM_MARK_ITEMS),
			RESET_ID = SAVE_ID + 1,
			INCR_ID  = SAVE_ID + 2,

		};

} tint_fconfig_param;


// ----------------------------------------------------------------------
INT Model::Tint_Fconfig_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_Fconfig_BuildList;
	tint_callback->callback_get_icon_info  = Tint_Fconfig_GetIconInfo;
	tint_callback->callback_update_model   = Tint_Fconfig_UpdateModel;
	tint_callback->callback_update_element = Tint_Fconfig_UpdateElement;
	tint_callback->callback_add_actions    = NULL;
	tint_callback->callback_add_text       = Tint_Fconfig_AddText;
	tint_callback->callback_add_value      = Tint_Fconfig_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Fconfig_BuildList(
		vs_system::TintItem li[]
	)
{

	// ---- describe
		INT number_of_items = (4 * 3) + (4 * 3) + 3;
		UINT gid = TINT_TAB_IMGUSER02_LID;
		INT i = -1;

		// ---- row 1
			li[++i].Set("Keypad", gid,  0);
			li[++i].Set("",     gid,  1);
			li[++i].Set("",     gid,  2);

		// ---- row 2
			li[++i].Set("",     gid,  3);
			li[++i].Set("",     gid,  4);
			li[++i].Set("",     gid,  5);

		// ---- row 3
			li[++i].Set("",     gid,  6);
			li[++i].Set("",     gid,  7);
			li[++i].Set("",     gid,  8);

		// ---- row 4
			li[++i].Set("",     gid, 9);
			li[++i].Set("",     gid, 10);
			li[++i].Set("",     gid, 11);

		// ---- row 5, 6, 7 & 8,
			li[++i].Set("", gid, 9); li[++i].Set("0.000", gid, 10); li[++i].Set("Origin",       gid, 10);
			li[++i].Set("", gid, 9); li[++i].Set("0.000", gid, 10); li[++i].Set("Origin Ref",   gid, 10);
			li[++i].Set("", gid, 9); li[++i].Set("0.000", gid, 10); li[++i].Set("Baseline",     gid, 10);
			li[++i].Set("", gid, 9); li[++i].Set("0.000", gid, 10); li[++i].Set("Baseline Ref", gid, 10);

		// ---- row 9
			li[++i].Set("Save",      gid, tint_fconfig_param.SAVE_ID);
			li[++i].Set("Reset",     gid, tint_fconfig_param.RESET_ID);
			li[++i].Set("Increment", gid, tint_fconfig_param.INCR_ID);

	// --- detail
			i = -1;

		// ---- row 1
			li[++i].SetAtlas(3, 2, 0); 
			li[++i].SetAtlas(3, 2, 1);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(3, 2, 2);   li[i].SetXOff(2.2f);   li[i].SetLF(TRUE);

		// ---- row 2
			li[++i].SetAtlas(3, 3, 0);
			li[++i].SetAtlas(3, 3, 1);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(3, 3, 2);   li[i].SetXOff(2.2f);   li[i].SetLF(TRUE);

		// ---- row 3
			li[++i].SetAtlas(3, 4, 0);
			li[++i].SetAtlas(3, 4, 1);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(3, 4, 2);   li[i].SetXOff(2.2f);   li[i].SetLF(TRUE);   li[i].SetLFI(6);

		// ---- row 4 (zoom)
			li[++i].SetAtlas(3, 5, 0);
			li[++i].SetAtlas(3, 5, 1);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(3, 5, 2);   li[i].SetXOff(2.2f);   li[i].SetLF(TRUE);   li[i].SetLFI(12);

		// ---- row 5, 6, 7 & 8,
			li[++i].SetAtlas(4, 7, 3);   li[++i].SetXOff(1.3f);  li[++i].SetXOff(3.0f);  li[i].SetLF(TRUE);   li[i].SetLFI(8);
			li[++i].SetAtlas(4, 7, 4);   li[++i].SetXOff(1.3f);  li[++i].SetXOff(3.0f);  li[i].SetLF(TRUE);   li[i].SetLFI(8);
			li[++i].SetAtlas(4, 7, 5);   li[++i].SetXOff(1.3f);  li[++i].SetXOff(3.0f);  li[i].SetLF(TRUE);   li[i].SetLFI(8);
			li[++i].SetAtlas(4, 7, 6);   li[++i].SetXOff(1.3f);  li[++i].SetXOff(3.0f);  li[i].SetLF(TRUE);   li[i].SetLFI(12);

		// ---- row 9 (housekeeping)
			li[++i].SetAtlas(3, 3, 3);
			li[++i].SetAtlas(3, 3, 4);   li[i].SetXOff(1.5f);
			li[++i].SetAtlas(3, 4, 3);   li[i].SetXOff(3.0f);   li[i].SetLF(TRUE);   li[i].SetLFI(4);

	// ---- type

		// ---- set all type to CLICK
			for ( INT i = 0; i<number_of_items; i++) li[i].SetType(vs_system::Tint::ITEM_CLICK);

		// ---- set mark to TEXT

			INT text_items = 12;
			li[++text_items].SetType(vs_system::Tint::ITEM_TEXT);
			li[++text_items].SetType(vs_system::Tint::ITEM_TEXT);

			++text_items;
			li[++text_items].SetType(vs_system::Tint::ITEM_TEXT);
			li[++text_items].SetType(vs_system::Tint::ITEM_TEXT);

			++text_items;
			li[++text_items].SetType(vs_system::Tint::ITEM_TEXT);
			li[++text_items].SetType(vs_system::Tint::ITEM_TEXT);

			++text_items;
			li[++text_items].SetType(vs_system::Tint::ITEM_TEXT);
			li[++text_items].SetType(vs_system::Tint::ITEM_TEXT);

		// ---- set houskeeping INCREMENT to SWITCH
			li[tint_imgptz_param.INCR_ID].SetType(vs_system::Tint::ITEM_SWITCH);
	
	return number_of_items;
}


// ----------------------------------------------------------------------
INT Model::Tint_Fconfig_GetIconInfo(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 3;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 7;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 2;
		case vs_system::Tint::ICON_ATLAS_PAGE_ID : return 3;
		default:
			break;
	}
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Fconfig_UpdateModel(
		INT  handle_model,
		INT  element_id
	)
{
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Fconfig_UpdateElement(
		INT handle_model,
		INT element_id
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_cam_setng;


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_TAB_IMGUSER02_LID);

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Fconfig_AddText(
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
INT  Model::Tint_Fconfig_AddValue(
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
