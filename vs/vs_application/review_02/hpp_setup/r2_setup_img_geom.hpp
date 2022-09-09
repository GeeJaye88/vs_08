////////////////////////////////////////////////////////////////////////////////

// ---------- r2_setup_img_geom.hpp ----------
/*!
\file r2_setup_img_geom.hpp
\brief
\author Gareth Edwards
*/


#pragma once

#include "../header/r2_model.h"

using namespace review_02;


// ---------- SetupImgGeometry ----------
/*!
\brief Setup image geometry display lists
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note 220419 - button behaviour update 

\note 220502 - Lord What Foolish Mortals We Are

\note 220502 - reactivate 'V' as active buttons

*/
INT Model::SetupImgGeometry()
{

	// ---- local
		INT hr;
		FLOAT button_wh = 0.2f;
		FLOAT button_y  = 0.01f;


	// ---- lambda ------------------------------------------------------------
		auto img_element = [&](INT handle)
		{
			hr = gfx->AddElement(
				handle,
				"2D Img",
				VS_OBJ_GEOMETRY,
				VS_OBJ_IMG,
				&pi_model->obj_config->Callback_Img_Rect_Configure
			);
		};


	// ---- add img list, NOTE: img NOT USED at present
		pi_model->handle_dspl_img = gfx->AddDisplayList("2D Img");
		img_element(pi_model->handle_dspl_img);


	// ---- lambda ------------------------------------------------------------
		//
		// note: display list name & element name the same
		//
		auto img_rect_quad_element = [&](CHAR prefix, INT id)
		{

			CHAR name[6];
				sprintf(name, "pRQ%2d", id);
				name[0] = prefix;
				name[3] = name[3] == ' ' ? '0' : name[3];

			INT index = id - 1;
			INT he = 0;

			if ( prefix == 'I' )
			{

				pi_model->handle_dspl_ir_img_rect_quad[index] =
						gfx->AddDisplayList(name);

				he = pi_model->handle_elem_ir_img_rect_quad[index] =
						gfx->AddElement(
								pi_model->handle_dspl_ir_img_rect_quad[index],
								name,
								VS_OBJ_GEOMETRY,
								VS_OBJ_IMG,
								&pi_model->obj_config->Callback_Img_Rect_Configure
							);
			}
			else
			{

				pi_model->handle_dspl_mv_img_rect_quad[index] =
						gfx->AddDisplayList(name);

				he = pi_model->handle_elem_mv_img_rect_quad[index] =
						gfx->AddElement(
								pi_model->handle_dspl_mv_img_rect_quad[index],
								name,
								VS_OBJ_GEOMETRY,
								VS_OBJ_IMG,
								&pi_model->obj_config->Callback_Img_Rect_Configure
							);
			}

			vs_system::ElmContext *elm_context;
				pi_model->gfx->GetElementContext(
						he,
						&elm_context
					);

			FLOAT *param = elm_context->GetFloatParameter();
				*(param + vs_system::ObjConfig::IMG_RECT_INDEX) = (FLOAT)index;
				*(param + vs_system::ObjConfig::IMG_RECT_TYPE) = vs_system::ObjConfig::IMG_RECT_TYPE_SOLID_PTZ_VIEW;
				*(param + vs_system::ObjConfig::IMG_RECT_MODE) = vs_system::ObjConfig::IMG_RECT_MODE_PTF_VIEW;
				*(param + vs_system::ObjConfig::IMG_RECT_ZOOM) = vs_system::ObjConfig::IMG_RECT_EXP_AOI_OFF;

			
			// ---- shader
				UINT p_id = (INT)id;
				INT p_val = 0;
				pi_model->obj_config->GetPanoParamById(p_id, 0, &p_val);
				UINT shader_index = p_val < 0 ? 0 : (  p_val > 3 ? 3 : p_val );
				he = elm_context->SetShaded(TRUE);
				he = elm_context->SetShaderIndex(shader_index);

		};


	// ---- lambda ------------------------------------------------------------
		auto img_rect_line_element = [&](CHAR prefix, INT id)
		{

			CHAR name[6];
				sprintf(name, "pRL%2d", id);
				name[0] = prefix;
				name[3] = name[3] == ' ' ? '0' : name[3];

			INT index = id - 1;
			INT he = 0;

			if ( prefix == 'I' )
			{

				pi_model->handle_dspl_ir_img_rect_line[index] =
						gfx->AddDisplayList(name);

				he = pi_model->handle_elem_ir_img_rect_line[index] =
						gfx->AddElement(
								pi_model->handle_dspl_ir_img_rect_line[index],
								name,
								VS_OBJ_GEOMETRY,
								VS_OBJ_IMG,
								&pi_model->obj_config->Callback_Img_Rect_Configure
							);
			}
			else
			{

				pi_model->handle_dspl_mv_img_rect_line[index] =
						gfx->AddDisplayList(name);

				he = pi_model->handle_elem_mv_img_rect_line[index] =
						gfx->AddElement(
								pi_model->handle_dspl_mv_img_rect_line[index],
								name,
								VS_OBJ_GEOMETRY,
								VS_OBJ_IMG,
								&pi_model->obj_config->Callback_Img_Rect_Configure
							);
			}
	

			vs_system::ElmContext *elm_context;
				pi_model->gfx->GetElementContext(
					he,
					&elm_context
				);

			FLOAT *param = elm_context->GetFloatParameter();
				*(param + vs_system::ObjConfig::IMG_RECT_INDEX) = (FLOAT)index;
				*(param + vs_system::ObjConfig::IMG_RECT_TYPE) = vs_system::ObjConfig::IMG_RECT_TYPE_LINE_PTZ_EDGE;
				*(param + vs_system::ObjConfig::IMG_RECT_MODE) = vs_system::ObjConfig::IMG_RECT_MODE_PTF_VIEW;
				*(param + vs_system::ObjConfig::IMG_RECT_ZOOM) = vs_system::ObjConfig::IMG_RECT_EXP_AOI_OFF;
		};


	// ---- lambda ------------------------------------------------------------
		auto img_rect_view_element = [&](INT id)
		{

			CHAR name[6];
				sprintf(name, "IRV%2d", id);
				name[3] = name[3] == ' ' ? '0' : name[3];

			INT index = id - 1;

			hr = pi_model->handle_dspl_ir_img_rect_view[index] =
				gfx->AddDisplayList(name);

			pi_model->handle_elem_ir_img_rect_view[index] =
				gfx->AddElement(
						pi_model->handle_dspl_ir_img_rect_view[index],
						name,
						VS_OBJ_GEOMETRY,
						VS_OBJ_IMG,
						&pi_model->obj_config->Callback_Img_Rect_Configure
					);

			vs_system::ElmContext *elm_context;
				pi_model->gfx->GetElementContext(
					pi_model->handle_elem_ir_img_rect_view[index],
					&elm_context
				);

			FLOAT *param = elm_context->GetFloatParameter();
				*(param + vs_system::ObjConfig::IMG_RECT_INDEX) = (FLOAT)index;
				*(param + vs_system::ObjConfig::IMG_RECT_TYPE)  = vs_system::ObjConfig::IMG_RECT_TYPE_SOLID_PTZ_CENTRE;
				*(param + vs_system::ObjConfig::IMG_RECT_MODE)  = vs_system::ObjConfig::IMG_RECT_MODE_PTF_CENTRE;
				*(param + vs_system::ObjConfig::IMG_RECT_ZOOM)  = vs_system::ObjConfig::IMG_RECT_EXP_AOI_OFF;


			// ---- view located relative to panel,
				//
				// note: unless panel in Settings frame,
				//       then located relative to img rect
				//
				if ( index == vs_system::ObjConfig::IMG_PTZ_SETTINGS_PANEL_INDEX )
				{
					*(param + vs_system::ObjConfig::IMG_RECT_MODE) = vs_system::ObjConfig::IMG_RECT_MODE_PTF_VIEW;
					*(param + vs_system::ObjConfig::IMG_RECT_ZOOM) = vs_system::ObjConfig::IMG_RECT_EXP_AOI_ON;
				}

		};


	// ---- lambda ------------------------------------------------------------
		auto img_rect_vmap_element = [&](INT id)
		{

			INT result = 0;

			CHAR name[6];
				sprintf(name, "IML%2d", id);
				name[3] = name[3] == ' ' ? '0' : name[3];

			INT index = id - 1;


			if ( index == vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX )
			{
				hr = pi_model->handle_dspl_ir_img_rect_locate =
					gfx->AddDisplayList(name);
			}


			pi_model->handle_elem_ir_img_rect_vmap[index] =
				gfx->AddElement(
						pi_model->handle_dspl_ir_img_rect_locate,
						name,
						VS_OBJ_GEOMETRY,
						VS_OBJ_IMG,
						&pi_model->obj_config->Callback_Img_Rect_Configure
					);


			vs_system::ElmContext *elm_context;
				result = pi_model->gfx->GetElementContext(
						pi_model->handle_elem_ir_img_rect_vmap[index],
						&elm_context
					);


			INT i_idx = id - (vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX + 1);
			INT i_row = i_idx / vs_system::ObjConfig::IMG_PTZ_GROUP_SIZE;
			result = elm_context->SetDisplay(i_row == 0 ? TRUE : FALSE);


			FLOAT *float_param = elm_context->GetFloatParameter();
				*(float_param + vs_system::ObjConfig::IMG_RECT_INDEX) = (FLOAT)index;
				*(float_param + vs_system::ObjConfig::IMG_RECT_TYPE) = vs_system::ObjConfig::IMG_RECT_TYPE_SOLID_PTZ_CENTRE;
				*(float_param + vs_system::ObjConfig::IMG_RECT_MODE) = vs_system::ObjConfig::IMG_RECT_MODE_PTF_OVERVIEW;
				*(float_param + vs_system::ObjConfig::IMG_RECT_ZOOM) = vs_system::ObjConfig::IMG_RECT_EXP_AOI_OFF;


			LONG *long_param = elm_context->GetLongParameter();
				*(long_param + 0) = 0;
				*(long_param + 1) = 0;
				*(long_param + 2) = 0;

		};


	// ---- lambda ------------------------------------------------------------
		auto img_rect_butt_element = [&](CHAR prefix, INT id)
		{

			// ---- local
				INT   result = 0;
				CHAR  name[6];
				CHAR *font_name = "";
				INT   index = id - 1;
				INT   he = 0;


			// ---- lut
				UINT num_elem = 3;
				UINT version_lut[3] =
				{
					vs_system::ElmContext::VERSION_BACKGROUND,
					vs_system::ElmContext::VERSION_TEXT,
					vs_system::ElmContext::VERSION_OUTLINE,
				};


			// ---- display list name - "IRB0[?]" or "NRB0[?]"
				sprintf(name, "pRB%2d", id);
				name[0] = prefix;
				name[3] = name[3] == ' ' ? '0' : name[3];

			// ---- add to display list - handle_dspl_ir_img_button & handle_dspl_mv_img_button

			// ---- init per display list
				//
				// Note:
				//   'I' : img nav
				//   'N' : multiview
				//   'V' : overview map
				//
				if ( prefix == 'I' )
				{ 
					pi_model->handle_dspl_ir_img_button[index] =
							gfx->AddDisplayList(name);
					font_name = "Ov_Button";
				}
				else if ( prefix == 'N' )
				{
					pi_model->handle_dspl_mv_img_button[index] =
							gfx->AddDisplayList(name);
					font_name = "Mv_Button";
				}
				else if ( prefix == 'V' )
				{
					// only if in overview mode
					if ( index == vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX )
					{
						pi_model->handle_dspl_vm_img_button =
								gfx->AddDisplayList(name);
					}
					font_name = "Vm_Button";

					// NEW 220502 - normal button
					// num_elem = 2;
					// version_lut[1] = vs_system::ElmContext::VERSION_TEXT;
				}


			// ---- shader
				//
				// note: for shaders
				//    FLOAT p_val = 0;
				//    UINT  p_id = id;
				//    pi_model->obj_config->GetPanoParamById(id, 0, &p_val);
				//    UINT shader_index = (INT)p_val < 0 ? 0 : (  (INT)p_val > 3 ? 3 : (INT)p_val );
				//


			// ---- set name
				//
				// note: for double digit, e.g. %2d, then add : 
				//    name[0] = name[0] == ' ' ? '0' : name[0];
				//
				// note: for shaders
				// sprintf(name, "%1d%c", id-1, 'A' + (INT)shader_index);
				//
				sprintf(name, "%1d", id - 1);


			// ---- create - handle_dspl_ir_img_button & handle_dspl_mv_img_button

			// ---- for each button element
				for ( UINT i = 0; i < num_elem; i++ )
				{

					// img nav
					if ( prefix == 'I' )
					{
						he = pi_model->handle_elem_ir_img_button[index][i] =
								gfx->AddElement(
										pi_model->handle_dspl_ir_img_button[index],
										name,
										VS_OBJ_IR_IMG_BUTTONS,
										id,
										&pi_model->obj_config->Callback_Button_Configure
									);
					}
					// multiview
					else if ( prefix == 'N' )
					{
						he = pi_model->handle_elem_mv_img_button[index][i] =
								gfx->AddElement(
										pi_model->handle_dspl_mv_img_button[index],
										name,
										VS_OBJ_MV_IMG_BUTTONS,
										id,
										&pi_model->obj_config->Callback_Button_Configure
									);
					}
					// overview map
					else if ( prefix == 'V' )
					{
						he = pi_model->handle_elem_vm_img_button[index][i] =
								gfx->AddElement(
										pi_model->handle_dspl_vm_img_button,
										name,
										VS_OBJ_VM_IMG_BUTTONS,
										id,
										&pi_model->obj_config->Callback_Button_Configure
									);
					}


					// ---- context
						vs_system::ElmContext *elm_context;
							pi_model->gfx->GetElementContext(
									he,
									&elm_context
								);


					// ---- set properties
						result = elm_context->SetFontName(font_name);
						result = elm_context->SetFontAlignment(vs_system::ElmContext::ALIGN_CENTRE);
						result = elm_context->SetText(name);
						result = elm_context->SetVersion(version_lut[i]);


						// overview map - deactivate 'V' // NEW 220502
						if ( prefix == 'v' )
						{
							result = elm_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NONE);
							result = elm_context->SetRGBA(1.0f, 1.0f, 1.0f, 1.0f);

							BOOL use_rgba = i == 2 ? TRUE : FALSE;
							result = elm_context->SetUseRGBA(use_rgba);

							BOOL mult_active = i == 2 ? FALSE : TRUE;
							result = elm_context->SetMultiActive(FALSE, mult_active);

						}
						// overview & multiview
						else
						{
							result = elm_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_SWITCH);
							result = elm_context->SetRGBA(1.0f, 1.0f, 1.0f, 1.0f);
							result = elm_context->SetUseRGBA(FALSE);

							// ---- set switched on when created
							result = elm_context->SetSwitchedOn(TRUE);

						}


					// ---- set parameters
						FLOAT *param = elm_context->GetFloatParameter();
						result = pi_model->obj_config->ButtonSetDPT(
								param,vs_system::ElmContext::VIEWPORT_WIDTH
							);
						result = pi_model->obj_config->ButtonSetLineAlpha(
								param,vs_system::ElmContext::LINE_ALPHA_OFF
							);


					// ---- location and dimension
						FLOAT wh = button_wh * 0.5f;
						FLOAT xy = button_y;
						if ( prefix == 'I')
						{
							;
						}
						else if ( prefix == 'N' )
						{
							xy *= 0.75f;
							wh *= 0.75f; 
						}
						else if ( prefix == 'V' )
						{
							xy *= 0.5f;
							wh *= 0.3f;
						}


					// ---- locate
						result = pi_model->obj_config->ButtonSetLocation(param, xy, xy, wh, wh);


					// ---- texture
						result = pi_model->obj_config->ButtonSetTexture(param, 0, 0, 0, 1, 0, 1, 1, 0.01f);


					// ---- overview map - display only one row
						if ( prefix == 'V' )
						{
							INT id_row_zero_min = vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX + 1;
							INT id_row_zero_max = id_row_zero_min + vs_system::ObjConfig::IMG_PTZ_GROUP_SIZE - 1;
							if ( id >= id_row_zero_min && id <= id_row_zero_max )
								result = elm_context->SetDisplay(TRUE);
							else
								result = elm_context->SetDisplay(FALSE);
						}


					// ---- overview map - store elm_context handles
						if ( prefix == 'V' )
						{
							vs_system::ElmContext *ov_elm_context;
								result = pi_model->gfx->GetElementContext(
										pi_model->handle_elem_ir_img_rect_vmap[index],
										&ov_elm_context
									);
							LONG *long_param = ov_elm_context->GetLongParameter();
								*(long_param+i) = (LONG)elm_context;
						}

				}

		};


	// ---- updated via:
	//
	// r2_ptf_panels.h - Model::Update_ImgPanel_Config(UINT id)
	//

	// ---- add img elements
		for (UINT i = 1; i <= vs_system::ObjConfig::MAX_PANO_PTF_LUT; i++)
		{

			// ---- multiview
				img_rect_quad_element('N', i); // bg
				img_rect_line_element('N', i); // edge
				img_rect_butt_element('N', i); // button

			// ---- overview
				img_rect_quad_element('I', i); // bg
				img_rect_line_element('I', i); // edge
				img_rect_view_element(i);      // view finder
				img_rect_butt_element('I', i); // button

		}


	// ----add oVerview map elements
		
		// "IMG_PTZ_[...]" ---- OK

		UINT id_first = vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX + 1;
		UINT id_last  = vs_system::ObjConfig::IMG_PTZ_LAST_PANEL_INDEX + 1;
		for (UINT i = id_first; i <= id_last; i++)
		{
			img_rect_vmap_element(i);      // edge
			img_rect_butt_element('V', i); // button
		}


	return VS_OK;
}


