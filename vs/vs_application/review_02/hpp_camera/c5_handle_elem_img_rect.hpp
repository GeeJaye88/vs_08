////////////////////////////////////////////////////////////////////////////////

// ---------- c5_handle_elem_img_rect.hpp ----------
/*!
\file c5_handle_elem_img_rect.hpp
\brief
\author Gareth Edwards
*/


#pragma once

#include "../header/r2_model.h"

using namespace review_02;


////////////////////////////////////////////////////////////////////////////////


// ---------- GetHandleElemImgRect ----------
/*!
\brief Get element "img_rect_quad" handle
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/

// TBD - in wrong file

INT Model::GetHandleElemImgRect(CHAR * type, CHAR *name, UINT id)
{

	// ---- bounds
		if ( id < 1 ) return VS_ERROR;
		if ( id > vs_system::ObjConfig::MAX_PANO_PTF_LUT) return VS_ERROR;


	// ---- 'i'mage 'r'eview
		if ( !strcmp("ir", type) )
		{

			if ( !strcmp("quad", name) )
			{
				return pi_model->handle_elem_ir_img_rect_quad[id - 1];
			}
			else if ( !strcmp("line", name) )
			{
				return pi_model->handle_elem_ir_img_rect_line[id - 1];
			}
			else if ( !strcmp("view", name) )
			{
				return pi_model->handle_elem_ir_img_rect_view[id - 1];
			}
			else if ( !strcmp("vmap", name) )
			{
				return pi_model->handle_elem_ir_img_rect_vmap[id - 1];
			}
			else if ( !strcmp("butt", name) )
			{
				return pi_model->handle_elem_ir_img_rect_vmap[id - 1];
			}
			else
			{
				return VS_ERROR;
			}

		}

	// ---- 'm'ulti 'v'iew
		else if ( !strcmp("mv", type) )
		{

			if (!strcmp("quad", name))
			{
				return pi_model->handle_elem_mv_img_rect_quad[id - 1];

			}
			else if ( !strcmp("line", name) )
			{
				return pi_model->handle_elem_mv_img_rect_line[id - 1];
			}
			else
			{
				return VS_ERROR;
			}

		}

	// ---- over'v'iew 'm'ap
		else if ( !strcmp("vm", type) )
		{
			switch (name[0])
			{
				case '0':
					return pi_model->handle_elem_vm_img_button[id - 1][0];
					break;
				case '1':
					return pi_model->handle_elem_vm_img_button[id - 1][1];
					break;
				case '2':
					return pi_model->handle_elem_vm_img_button[id - 1][2];
					break;
				default:
					return VS_ERROR;
					break;
			}

		}

	return VS_ERROR;
}


vs_system::ElmContext * Model::GetElmContext(INT handle)
{
	vs_system::ElmContext *elm_context = NULL;
	pi_model->gfx->GetElementContext(handle, &elm_context);
	return elm_context;
}


////////////////////////////////////////////////////////////////////////////////
