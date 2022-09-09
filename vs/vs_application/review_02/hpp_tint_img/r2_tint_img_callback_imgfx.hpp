////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tool_callback_setup.hpp ----------
/*!
\file r2_tool_callback_setup.hpp
\brief
\author Gareth Edwards
*/


#pragma once


using namespace review_02;


// -------- Functionality and Parameters for Tabbed Interface Acquire

class Model::Tint_ImgFx_Param
{

public:

	// ---- cdtor
		Tint_ImgFx_Param::Tint_ImgFx_Param() { ; };
		Tint_ImgFx_Param::~Tint_ImgFx_Param() { ; };

	// --- props
		enum
		{
			CONFIG_ID = 10,
			ABCD_ID   = CONFIG_ID,
			SAVE_ID   = ABCD_ID + 4,
			RESET_ID  = SAVE_ID + 1,
			INCR_ID   = SAVE_ID + 2
		};

} tint_imgfx_param;


// ----------------------------------------------------------------------
INT Model::Tint_ImgFx_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_ImgFx_BuildList;
	tint_callback->callback_get_icon_info  = Tint_ImgFx_GetIconInfo;
	tint_callback->callback_update_model   = Tint_ImgFx_UpdateModel;
	tint_callback->callback_update_element = Tint_ImgFx_UpdateElement;
	tint_callback->callback_add_actions    = NULL;
	tint_callback->callback_add_text       = Tint_ImgFx_AddText;
	tint_callback->callback_add_value      = Tint_ImgFx_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_ImgFx_BuildList(
		vs_system::TintItem li[]
	)
{

	// ---- SET
		UINT gid = TINT_TAB_IMGFX_LID;
		INT i = -1;

		// ---- row 1
			li[++i].Set("Brightness",  gid);
			li[++i].Set("",            gid);
			li[++i].Set("",            gid);

		// ---- row 2
			li[++i].Set("Contrast",    gid);
			li[++i].Set("",            gid);
			li[++i].Set("",            gid);

		// ---- row 3
			li[++i].Set("Saturation",  gid);
			li[++i].Set("",            gid);
			li[++i].Set("",            gid);

		// ---- row 4: ABCD Configurations
			li[++i].Set("Configurations", gid);
			li[++i].Set("",            gid);
			li[++i].Set("",            gid);
			li[++i].Set("",            gid);

		// ---- row 5
			li[++i].Set("Save",        gid);
			li[++i].Set("Reset",       gid);
			li[++i].Set("Increment",   gid);


	// ---- SET element_id = element_index + 1 (weird - TBD)
		UINT number_of_items = i+1;
		for ( UINT i = 0; i<number_of_items; i++ ) li[i].elem_id = i+1;


	// --- LAYOUT
			i = -1;

		// ---- row 1
			li[++i].SetAtlas(3, 5, 0);
			li[++i].SetAtlas(3, 5, 1);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(3, 5, 2);   li[i].SetXOff(2.2f);   li[i].SetLF(TRUE);  li[i].SetLFI(4);

		// ---- row 2
			li[++i].SetAtlas(3, 5, 0);
			li[++i].SetAtlas(3, 5, 1);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(3, 5, 2);   li[i].SetXOff(2.2f);   li[i].SetLF(TRUE);  li[i].SetLFI(4);

		// ---- row 3
			li[++i].SetAtlas(3, 5, 0);
			li[++i].SetAtlas(3, 5, 1);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(3, 5, 2);   li[i].SetXOff(2.2f);   li[i].SetLF(TRUE);  li[i].SetLFI(12);

		// ---- row 4: ABCD Configurations @ 10
			li[++i].SetAtlas(3, 6, 0);   li[i].SetXOff(0.0f); // 10
			li[++i].SetAtlas(3, 6, 1);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(3, 6, 2);   li[i].SetXOff(2.2f);
			li[++i].SetAtlas(3, 6, 3);   li[i].SetXOff(3.3f);   li[i].SetLF(TRUE);   li[i].SetLFI(12);

		// ---- row 5: Save, Reset & Increment @ 14, 15 & 16
			li[++i].SetAtlas(3, 3, 3);
			li[++i].SetAtlas(3, 3, 4);   li[i].SetXOff(1.5f);
			li[++i].SetAtlas(3, 4, 3);   li[i].SetXOff(3.0f); li[i].SetLF(TRUE); // 16

			li[i].SetLFI(4);


	// ---- TYPE
		for ( UINT i = 0; i<number_of_items; i++ ) li[i].SetType(vs_system::Tint::ITEM_CLICK);
		
		// ---- row 4: ABCD Configurations (note: gfx_shader_mode always saved == 1)
			li[tint_imgfx_param.ABCD_ID - 1    ].SetType(vs_system::Tint::ITEM_RADIO);
			li[tint_imgfx_param.ABCD_ID - 1    ].SetStatus(TRUE);
			li[tint_imgfx_param.ABCD_ID - 1 + 1].SetType(vs_system::Tint::ITEM_RADIO);
			li[tint_imgfx_param.ABCD_ID - 1 + 2].SetType(vs_system::Tint::ITEM_RADIO);
			li[tint_imgfx_param.ABCD_ID - 1 + 3].SetType(vs_system::Tint::ITEM_RADIO);

		// ---- row 5: Increment
			li[tint_imgfx_param.INCR_ID - 1].SetType(vs_system::Tint::ITEM_SWITCH);
			li[tint_imgfx_param.INCR_ID - 1].SetStatus(FALSE);


	// ---- COLUMNS - LF @ rows 4 & 5
		li[tint_imgfx_param.ABCD_ID - 1].SetColumnLF(TRUE); // Configurations
		li[tint_imgfx_param.SAVE_ID - 1].SetColumnLF(TRUE); // Save

	return number_of_items;
}


// ----------------------------------------------------------------------
INT Model::Tint_ImgFx_GetIconInfo(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 3;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 2;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
		case vs_system::Tint::ICON_ATLAS_PAGE_ID : return 3;
		default:
			break;
	}
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_ImgFx_UpdateModel(
		INT  handle_model,
		INT  element_index
	)
{

	// ---- local
		Model *model = (Model *)handle_model;
		UINT hr = 0;


	// ---- get image properties data
		vs_system::ImgProps *img_props = NULL;
		hr = model->GetImgProps(&img_props);

		using namespace vs_system;

	// ---- get image properties ( from 1 to 4 )
		FLOAT float_value = 0;
		hr = img_props->GetProperty(ImgProps::GFX_SHADER,0,ImgProps::MODE, &float_value);
		UINT mode = (UINT)float_value;

	// ---- get image increment
		hr = img_props->GetProperty(ImgProps::GFX_SHADER,0,ImgProps::INCREMENT, &float_value);
		BOOL  incr_mode = float_value > 0 ? TRUE : FALSE;
		FLOAT incr = incr_mode ? 0.5f : 0.1f;

	// ---- get bcs
		FLOAT shader_param[8];
		FLOAT *b = &shader_param[0];
		FLOAT *c = &shader_param[1];
		FLOAT *s = &shader_param[2];
		hr = img_props->GetProperty(ImgProps::GFX_SHADER, mode, ImgProps::BRIGHTNESS, b);
		hr = img_props->GetProperty(ImgProps::GFX_SHADER, mode, ImgProps::CONTRAST,   c);
		hr = img_props->GetProperty(ImgProps::GFX_SHADER, mode, ImgProps::SATURATION, s);

	// --- fx
		switch (element_index)
		{

			case 1: *b -= incr; *b = *b < 0 ? 0 : *b; break;
			case 2: *b  = 1;    break;
			case 3: *b += incr; break;
			case 4: *c -= incr; break;
			case 5: *c  = 0;    break;
			case 6: *c += incr; break;
			case 7: *s -= incr; break;
			case 8: *s  = 0;    break;
			case 9: *s += incr; break;

			case tint_imgfx_param.ABCD_ID    : // A
			case tint_imgfx_param.ABCD_ID + 1: // B
			case tint_imgfx_param.ABCD_ID + 2: // C
			case tint_imgfx_param.ABCD_ID + 3: // D
				{
					mode = element_index - tint_imgfx_param.ABCD_ID + 1; // from 1 to 4
					hr = img_props->GetProperty(ImgProps::GFX_SHADER, mode, ImgProps::BRIGHTNESS, b);
					hr = img_props->GetProperty(ImgProps::GFX_SHADER, mode, ImgProps::CONTRAST,   c);
					hr = img_props->GetProperty(ImgProps::GFX_SHADER, mode, ImgProps::SATURATION, s);
				}
				break;
			
			case tint_imgfx_param.SAVE_ID : // save
				hr = img_props->SetGfxShaderMode(1);
				hr = model->SaveImgProps();
				break;

			case tint_imgfx_param.RESET_ID : // reset
				{
					*b = 1;
					*c = 0;
					*s = 0;
					for (UINT mode = 1; mode <= 4; mode++)
					{
						hr = img_props->SetProperty(ImgProps::GFX_SHADER, mode, ImgProps::BRIGHTNESS, *b);
						hr = img_props->SetProperty(ImgProps::GFX_SHADER, mode, ImgProps::CONTRAST,   *c);
						hr = img_props->SetProperty(ImgProps::GFX_SHADER, mode, ImgProps::SATURATION, *s);
					}
				}
				break;

			case tint_imgfx_param.INCR_ID: // increment
				incr_mode = incr_mode ? FALSE : TRUE;
				break;

			default:
				break;

		}


	// ---- set image properties ( from 1 to 4 )
		float_value = (FLOAT)mode;
		img_props->SetProperty(ImgProps::GFX_SHADER, 0,ImgProps::MODE, float_value);

		float_value = (FLOAT)((INT)incr_mode);
		hr = img_props->SetProperty(ImgProps::GFX_SHADER, 0, ImgProps::INCREMENT, float_value);

		hr = img_props->SetProperty(ImgProps::GFX_SHADER, mode, ImgProps::BRIGHTNESS, *b);
		hr = img_props->SetProperty(ImgProps::GFX_SHADER, mode, ImgProps::CONTRAST,   *c);
		hr = img_props->SetProperty(ImgProps::GFX_SHADER, mode, ImgProps::SATURATION, *s);

		//CHAR msg[128];
		//sprintf(msg, " +++ %d %d %6.4f %6.4f %6.4f \n", (INT)mode, (INT)incr_mode, *b, *c, *s);
		//OutputDebugString(msg);


	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_ImgFx_UpdateElement(
		INT handle_model,
		INT element_id
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_img_setng;

	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_TAB_IMGFX_LID);

	// ---- select elem
		switch ( element_id )
		{

			case 1:
			case 2:
			case 3:
				result = tint->UpdateText(NULL, 1);
				break;

			case 4:
			case 5:
			case 6:
				result = tint->UpdateText(NULL, 4);
				break;

			case 7:
			case 8:
			case 9:
				result = tint->UpdateText(NULL, 7);
				break;

			case tint_imgfx_param.ABCD_ID    : // A
			case tint_imgfx_param.ABCD_ID + 1: // B
			case tint_imgfx_param.ABCD_ID + 2: // C
			case tint_imgfx_param.ABCD_ID + 3: // D
			case tint_imgfx_param.SAVE_ID    : // save
			case tint_imgfx_param.RESET_ID   : // reset
				result = tint->UpdateText(NULL, 1);
				result = tint->UpdateText(NULL, 4);
				result = tint->UpdateText(NULL, 7);
				break;

			default:
				break;

		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_ImgFx_AddText(
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
INT Model::Tint_ImgFx_AddValue(
		const INT  handle_obj_config,
		const INT  element_index,
		BOOL      *add_value,
		FLOAT     *value,
		CHAR      *suffix
	)
{


	// ---- local
		Model *model = (Model *)handle_obj_config;
		INT result = 0;

	// ---- get image properties data
		vs_system::ImgProps *img_props = NULL;
		result = model->GetImgProps(&img_props);

		using namespace vs_system;

	// ---- get image properties (from 1 to 4)
		FLOAT float_value = 0;
		result = img_props->GetProperty(ImgProps::GFX_SHADER, 1, ImgProps::MODE, &float_value);
		UINT mode = (UINT)float_value;

	// ---- set shader param
		FLOAT shader_param[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		FLOAT b, c, s;
		result = img_props->GetProperty(ImgProps::GFX_SHADER, mode, ImgProps::BRIGHTNESS, &b);
		result = img_props->GetProperty(ImgProps::GFX_SHADER, mode, ImgProps::CONTRAST,   &c);
		result = img_props->GetProperty(ImgProps::GFX_SHADER, mode, ImgProps::SATURATION, &s);
		shader_param[0] = b;
		shader_param[1] = c;
		shader_param[2] = s;
		result = model->SetGfxShaderParam(0, shader_param);

	// ---- select elem
		*add_value = TRUE;
		sprintf(suffix, "2");
		switch ( element_index )
		{
			case 1:
				*value = shader_param[0]; // brightness
				break;

			case 4:
				*value = shader_param[1]; // contrast
				break;

			case 7:
				*value = shader_param[2]; // value
				break;

			default:
				*add_value = FALSE;
				break;

		}

	return VS_OK;
}
