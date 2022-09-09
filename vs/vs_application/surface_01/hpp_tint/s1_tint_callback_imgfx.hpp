////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tool_callback_setup.hpp ----------
/*!
\file r2_tool_callback_setup.hpp
\brief
\author Gareth Edwards
*/

#pragma once

using namespace surface_01;


// ----------------------------------------------------------------------

class Model::Tint_ImgFx_Param
{

public:

		Tint_ImgFx_Param::Tint_ImgFx_Param() { ; };
		Tint_ImgFx_Param::~Tint_ImgFx_Param() { ; };

		enum
		{
			SAVE_INDEX      = 16,
			RESET_INDEX     = 17,
			INCREMENT_INDEX = 18
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

	// ---- build
		UINT gid = TINT_BODY_IMGFX_LID;
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


	// ---- row 4
		#if VS_MAX_SHADERS == 1
		li[++i].Set("Configurations", gid);
		li[++i].Set("", gid);
		#else
		li[++i].Set("(Un)Set",     gid);
		li[++i].Set("(De)Select",  gid);
		#endif

	// ---- row 5
		#if VS_MAX_SHADERS == 1
		li[++i].Set("",            gid);
		#else
		li[++i].Set("Configurations", gid);
		#endif
		li[++i].Set("",            gid);
		li[++i].Set("",            gid);
		li[++i].Set("",            gid);

	// ---- row 6
		li[++i].Set("Save",        gid);
		li[++i].Set("Reset",       gid);
		li[++i].Set("Increment",   gid);


	// ---- set element id
		UINT number_of_items = i+1;
		for ( UINT i = 0; i<number_of_items; i++ ) li[i].elem_id = i+1;


	// --- detail
			i = -1;

		// ---- row 1
			li[++i].SetAtlas(4, 5, 0);
			li[++i].SetAtlas(4, 5, 1);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(4, 5, 2);   li[i].SetXOff(2.2f);   li[i].SetLF(TRUE);  li[i].SetLFI(4);

		// ---- row 2
			li[++i].SetAtlas(4, 5, 0);
			li[++i].SetAtlas(4, 5, 1);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(4, 5, 2);   li[i].SetXOff(2.2f);   li[i].SetLF(TRUE);  li[i].SetLFI(4);

		// ---- row 3
			li[++i].SetAtlas(4, 5, 0);
			li[++i].SetAtlas(4, 5, 1);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(4, 5, 2);   li[i].SetXOff(2.2f);   li[i].SetLF(TRUE);  li[i].SetLFI(12);

		// ---- row 4
			#if VS_MAX_SHADERS == 1
			li[++i].SetAtlas(4, 0, 0);
			li[++i].SetAtlas(4, 0, 0);
			#else
			li[++i].SetAtlas(4, 4, 4);
			li[++i].SetAtlas(4, 4, 4);   li[i].SetXOff(2.2f);   li[i].SetLF(TRUE);  li[i].SetLFI(12);
			#endif

		// ---- row 5
			li[++i].SetAtlas(4, 6, 0);
			li[++i].SetAtlas(4, 6, 1);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(4, 6, 2);   li[i].SetXOff(2.2f);
			li[++i].SetAtlas(4, 6, 3);   li[i].SetXOff(3.3f);   li[i].SetLF(TRUE);   li[i].SetLFI(12);

		// ---- row 6
			li[++i].SetAtlas(4, 3, 3);
			li[++i].SetAtlas(4, 3, 4);   li[i].SetXOff(1.5f);
			li[++i].SetAtlas(4, 4, 3);   li[i].SetXOff(3.0f); li[i].SetLF(TRUE);



	// ---- set type
		for ( UINT i = 0; i<number_of_items; i++ ) li[i].SetType(vs_system::Tint::ITEM_CLICK);
		
		UINT set_index = 9;
		#if VS_MAX_SHADERS == 1
		li[set_index].SetType(0);
		#else
		li[set_index].SetType(vs_system::Tint::ITEM_CLICK);
		#endif

		UINT unset_index = 10;
		#if VS_MAX_SHADERS == 1
		li[unset_index].SetType(0);
		#else
		li[set_index].SetType(vs_system::Tint::ITEM_CLICK);
		#endif

		UINT config_index = 11;
		li[config_index].SetType(vs_system::Tint::ITEM_RADIO);
		li[config_index].SetStatus(TRUE);
		li[config_index +1].SetType(vs_system::Tint::ITEM_RADIO);
		li[config_index +2].SetType(vs_system::Tint::ITEM_RADIO);
		li[config_index +3].SetType(vs_system::Tint::ITEM_RADIO);


		UINT incr_index = tint_imgfx_param.INCREMENT_INDEX;
		li[incr_index].SetType(vs_system::Tint::ITEM_SWITCH);
		li[incr_index].SetStatus(FALSE);


	// ---- column LF
		li[9].SetColumnLF(TRUE);
		li[15].SetColumnLF(TRUE);

	return number_of_items;
}


// ----------------------------------------------------------------------

INT Model::Tint_ImgFx_GetIconInfo(INT id)
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

INT  Model::Tint_ImgFx_UpdateModel(
		INT  handle_model,
		INT  element_id
	)
{

	// ---- local
		Model *model = (Model *)handle_model;
		UINT hr = 0;


	// ---- get image properties data
		vs_system::ImgProps *img_props = NULL;
		hr = model->GetImgProps(&img_props);

		using namespace vs_system;


	// ---- get image properties
		FLOAT float_value = 0;
		hr = img_props->GetProperty(ImgProps::GFX_SHADER,0,ImgProps::MODE, &float_value);
		UINT config_mode = (UINT)float_value;

		hr = img_props->GetProperty(ImgProps::GFX_SHADER,0,ImgProps::INCREMENT, &float_value);
		BOOL  incr_mode = float_value > 0 ? TRUE : FALSE;
		FLOAT incr = incr_mode ? 0.5f : 0.1f;

		FLOAT shader_param[8];
		FLOAT *b = &shader_param[0];
		FLOAT *c = &shader_param[1];
		FLOAT *s = &shader_param[2];
		hr = img_props->GetProperty(ImgProps::GFX_SHADER, config_mode, ImgProps::BRIGHTNESS, b);
		hr = img_props->GetProperty(ImgProps::GFX_SHADER, config_mode, ImgProps::CONTRAST,   c);
		hr = img_props->GetProperty(ImgProps::GFX_SHADER, config_mode, ImgProps::SATURATION, s);


	// --- fx
		switch ( element_id )
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

			case 10: // (Un)Set
				break;
			case 11: // (De)Select
				break; 

			case 12: // a
			case 13: // b
			case 14: // c
			case 15: // d
				{
					config_mode  = element_id - 12; // 0 to 3
					hr = img_props->GetProperty(ImgProps::GFX_SHADER, config_mode, ImgProps::BRIGHTNESS, b);
					hr = img_props->GetProperty(ImgProps::GFX_SHADER, config_mode, ImgProps::CONTRAST,   c);
					hr = img_props->GetProperty(ImgProps::GFX_SHADER, config_mode, ImgProps::SATURATION, s);
				}
				break;
			
			case tint_imgfx_param.SAVE_INDEX:
				hr = model->SaveImgProps();
				break;

			case tint_imgfx_param.RESET_INDEX:
				{
					*b = 1;
					*c = 0;
					*s = 0;
					for (UINT config_index = 0; config_index < 4; config_index++)
					{
						hr = img_props->SetProperty(ImgProps::GFX_SHADER, config_index, ImgProps::BRIGHTNESS, *b);
						hr = img_props->SetProperty(ImgProps::GFX_SHADER, config_index, ImgProps::CONTRAST,   *c);
						hr = img_props->SetProperty(ImgProps::GFX_SHADER, config_index, ImgProps::SATURATION, *s);
					}
				}
				break;

			case tint_imgfx_param.INCREMENT_INDEX:
				incr_mode = incr_mode ? FALSE : TRUE;
				break;

			default:
				break;

		}


	// ---- set image properties
		float_value = (FLOAT)config_mode;
		img_props->SetProperty(ImgProps::GFX_SHADER,0,ImgProps::MODE, float_value);

		float_value = (FLOAT)((INT)incr_mode);
		hr = img_props->SetProperty(ImgProps::GFX_SHADER,0,ImgProps::INCREMENT, float_value);

		hr = img_props->SetProperty(ImgProps::GFX_SHADER, config_mode, ImgProps::BRIGHTNESS, *b);
		hr = img_props->SetProperty(ImgProps::GFX_SHADER, config_mode, ImgProps::CONTRAST,   *c);
		hr = img_props->SetProperty(ImgProps::GFX_SHADER, config_mode, ImgProps::SATURATION, *s);

		CHAR msg[128];
		sprintf(msg, " +++ %d %d %6.4f %6.4f %6.4f \n", (INT)config_mode, (INT)incr_mode, *b, *c, *s);
		OutputDebugString(msg);


	// ---- set GfxShader BCS below

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
		vs_system::Tint *tint = model->pi_model->tint_setng;


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_BODY_IMGFX_LID);


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

			case 12: // a
			case 13: // b
			case 14: // c
			case 15: // d

			case tint_imgfx_param.SAVE_INDEX:
			case tint_imgfx_param.RESET_INDEX:
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


	// ---- get mode (from 1 to 4)
		FLOAT float_value = 0;
		result = img_props->GetProperty(ImgProps::GFX_SHADER, 0, ImgProps::MODE, &float_value);
		UINT mode = (UINT)float_value;


	// ---- set shader param
		FLOAT shader_param[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
		#if VS_MAX_SHADERS == 1
			FLOAT b, c, s;
			result = img_props->GetProperty(ImgProps::GFX_SHADER, mode, ImgProps::BRIGHTNESS, &b);
			result = img_props->GetProperty(ImgProps::GFX_SHADER, mode, ImgProps::CONTRAST,   &c);
			result = img_props->GetProperty(ImgProps::GFX_SHADER, mode, ImgProps::SATURATION, &s);
			shader_param[0] = b;
			shader_param[1] = c;
			shader_param[2] = s;
			result = model->SetGfxShaderParam(0, shader_param);
		#else
			for (UINT i = 0; i < VS_MAX_SHADERS; i++)
			{
				FLOAT b, c, s;
				result = img_props->GetProperty(ImgProps::GFX_SHADER, i, ImgProps::BRIGHTNESS, &b);
				result = img_props->GetProperty(ImgProps::GFX_SHADER, i, ImgProps::CONTRAST,   &c);
				result = img_props->GetProperty(ImgProps::GFX_SHADER, i, ImgProps::SATURATION, &s);
				shader_param[0] = b;
				shader_param[1] = c;
				shader_param[2] = s;
				result = model->SetGfxShaderParam(i, shader_param);
			}
		#endif

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
