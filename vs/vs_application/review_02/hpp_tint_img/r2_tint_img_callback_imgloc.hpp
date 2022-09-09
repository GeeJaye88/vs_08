////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_img_callback_imgloc.hpp ----------
/*!
\file r2_tint_img_callback_imgloc.hpp
\brief
\author Gareth Edwards
*/


#pragma once

using namespace review_02;


// -------- Tabbed Interface Image Location Funtionality and Parameters class

class Model::Tint_ImgLoc_Param
{

	public:


	// ---- enumerated values for MapRectToPtf method 
		enum DimensionBasis
		{
				WIDTH,
				HEIGHT,
				LEFT,
				RIGHT,
				BOTTOM,
				TOP
		};


	// ---- ctor
		Tint_ImgLoc_Param::Tint_ImgLoc_Param()
		{
			;
		};


	// ---- dtor
		Tint_ImgLoc_Param::~Tint_ImgLoc_Param()
		{
			;
		} ;


	// ---- border a VsFloatRectangle
		VOID BorderFloatRectangle(
				FLOAT border,
				VsFloatRectangle *rect
			)
		{
			rect->left   += border;
			rect->right  -= border;
			rect->top    += border;
			rect->bottom -= border;
		}


	// ---- copy VsFloatRectangle 'from' to VsFloatRectangle 'too'
		VOID CopyFloatRectangle(
				VsFloatRectangle *too,
				VsFloatRectangle *from
			)
		{
			too->left   = from->left;
			too->right  = from->right;
			too->bottom = from->bottom;
			too->top    = from->top;
		}


	// ---- map Pan, Tilt and Field of View (FOV) to a VsFloatRectangle
		INT MapPtfToRect(
				FLOAT pan,
				FLOAT tilt,
				FLOAT fov,
				VsFloatRectangle *rect
			)
		{

				FLOAT zoom = fov <= 1 ? 1 : fov;
				zoom = 1 + (zoom - 1) / 10;
				FLOAT scalar = 1 / zoom;

				FLOAT height = 1;
				FLOAT width  = 1;
				FLOAT xdim   = width / 2 * scalar;
				FLOAT ydim   = height / 2 * scalar;

				rect->left   = pan  - xdim;
				rect->right  = pan  + xdim;
				rect->top    = tilt - ydim;
				rect->bottom = tilt + ydim;

			return VS_OK;
		}


	// ---- map VsFloatRectangle to Pan, Tilt and Field of View (FOV)
		INT MapRectToPtf(
				DimensionBasis mode,
				FLOAT *pan,
				FLOAT *tilt,
				FLOAT *fov,
				VsFloatRectangle *rect
			)
		{
		 
			FLOAT scalar = 1;
			FLOAT width  = rect->right - rect->left;
			FLOAT height = rect->bottom - rect->top;
	
			switch (mode)
			{
				case LEFT:
					{
						*pan = rect->right - (width / 2);
						scalar = 1 / width;
					}
					break;

				case RIGHT:
					{
						*pan = rect->left + (width / 2);
						scalar = 1 / width;
					}
					break;

				case BOTTOM:
					{
						*tilt = rect->bottom - (height / 2);
						scalar = 1 / height;
					}
					break;

				case TOP:
					{
						*tilt = rect->top + (height / 2);
						scalar = 1 / height;
					}
					break;

				case WIDTH:
				case HEIGHT:
					{
						*pan = (rect->left + rect->right) / 2;
						*tilt = (rect->top + rect->bottom) / 2;
						scalar = mode == HEIGHT ?
							1 / height :
								1 / width;
					}
					break;

			}

			*fov = (scalar - 1) * 10 + 1;

			return VS_OK;
		}


	// ---- create full and quad location rects
		VsFloatRectangle single[5] =
		{
			{ -1, -1, 1, 1 },
			{ -1,  0, 0, 1 },
			{  0,  0, 1, 1 },
			{ -1, -1, 0, 0 },
			{  0, -1, 1, 0 },
		};
		VsFloatRectangle nono[9];
		VsFloatRectangle nono_with_quad[9];


	// --- border
		BOOL  border_all_flag = FALSE;
		BOOL  large_increment = TRUE;

} tint_imgloc_param;


// ---- const
	const UINT tint_img_all_id    = 15;
	const UINT tint_img_border_id = 17;
	#if defined(VS_CAMERA_05) || defined(VS_CAMERA_03)
		const UINT tint_img_save_id      = 20;
		const UINT tint_img_reset_id     = 21;
		const UINT tint_img_incr_id      = 22;
	#else
		const UINT tint_img_save_id      = 99;
		const UINT tint_img_reset_id     = 20;
		const UINT tint_img_incr_id      = 21;
	#endif 


// ----------------------------------------------------------------------
INT Model::Tint_ImgLoc_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_ImgLoc_BuildList;
	tint_callback->callback_get_icon_info  = Tint_ImgLoc_GetIconInfo;
	tint_callback->callback_update_model   = Tint_ImgLoc_UpdateModel;
	tint_callback->callback_update_element = Tint_ImgLoc_UpdateElement;
	tint_callback->callback_add_actions    = NULL;
	tint_callback->callback_add_text       = Tint_ImgLoc_AddText;
	tint_callback->callback_add_value      = Tint_ImgLoc_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_ImgLoc_BuildList(
		vs_system::TintItem li[]
	)
{

	// ---- describe
		UINT gid = TINT_TAB_IMGLOC_LID;
		INT i = -1;

		// ---- row 1
			li[++i].Set("Left",     gid); // 1
			li[++i].Set("",         gid);
			li[++i].Set("Right ",   gid);
			li[++i].Set("",         gid);

		// ---- row 2
			li[++i].Set("Top",      gid); // 5
			li[++i].Set("",         gid);
			li[++i].Set("Bottom",   gid);
			li[++i].Set("",         gid);

		// ---- row 3
			li[++i].Set("Single",   gid); // 9
			li[++i].Set("",         gid);
			li[++i].Set("",         gid);
			li[++i].Set("",         gid);
			li[++i].Set("",         gid);
			li[++i].Set("",         gid);

		// ---- row 4
			li[++i].Set("All",      gid); // 15
			li[++i].Set("",         gid);

		// ---- row 5
			li[++i].Set("Border",   gid); // border_flag_id
			li[++i].Set("",         gid);
			li[++i].Set("",         gid);

		// ---- row 6
			#if defined(VS_CAMERA_05) || defined(VS_CAMERA_03) 
			li[++i].Set("Save",     gid); // tint_img_save_id
			li[++i].Set("Reset",    gid); // tint_img_reset_id
			li[++i].Set("Increment",gid); // tint_img_incr_id
			#else
			li[++i].Set("Reset",    gid); // tint_img_reset_id
			li[++i].Set("Increment",gid); // tint_img_incr_id
			#endif


	// ---- set element id
		UINT number_of_items = i + 1;
		for (UINT i = 0; i<number_of_items; i++) li[i].elem_id = i + 1;


	// --- detail
			i = -1;

		// ---- row 1
			li[++i].SetAtlas(3, 5, 0);
			li[++i].SetAtlas(3, 5, 2);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(3, 5, 0);   li[i].SetXOff(2.7f);
			li[++i].SetAtlas(3, 5, 2);   li[i].SetXOff(3.8f);   li[i].SetLF(TRUE);

		// ---- row 2
			li[++i].SetAtlas(3, 5, 0);
			li[++i].SetAtlas(3, 5, 2);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(3, 5, 0);   li[i].SetXOff(2.7f);
			li[++i].SetAtlas(3, 5, 2);   li[i].SetXOff(3.8f);   li[i].SetLF(TRUE);   li[i].SetLFI(8);

		// ---- row 3
			li[++i].SetAtlas(3, 7, 0);
			li[++i].SetAtlas(3, 3, 5);   li[i].SetXOff(1.1f);   li[i].SetLF(TRUE);
			li[++i].SetAtlas(3, 7, 1);  
			li[++i].SetAtlas(3, 7, 2);   li[i].SetXOff(1.1f);
			li[++i].SetAtlas(3, 7, 3);   li[i].SetXOff(2.2f);
			li[++i].SetAtlas(3, 7, 4);   li[i].SetXOff(3.3f);   li[i].SetLF(TRUE);   li[i].SetLFI(8);

		// ---- row 4
			li[++i].SetAtlas(3, 7, 5);
			li[++i].SetAtlas(3, 7, 6);   li[i].SetXOff(1.1f);   li[i].SetLF(TRUE);   li[i].SetLFI(8);

		// ---- row 5
			li[++i].SetAtlas(3, 7, 7); 
			li[++i].SetAtlas(3, 2, 3);   li[i].SetXOff(1.5f);
			li[++i].SetAtlas(3, 2, 4);   li[i].SetXOff(2.6f);   li[i].SetLF(TRUE);   li[i].SetLFI(8);
	
		// ---- row 6
			#if defined(VS_CAMERA_05) || defined(VS_CAMERA_03)
			li[++i].SetAtlas(3, 3, 3);
			li[++i].SetAtlas(3, 3, 4);   li[i].SetXOff(1.5f);
			li[++i].SetAtlas(3, 4, 3);   li[i].SetXOff(3.0f); li[i].SetLF(TRUE);
			#else
			li[++i].SetAtlas(3, 3, 5);
			li[++i].SetAtlas(3, 4, 3);   li[i].SetXOff(2.6f); li[i].SetLF(TRUE);
			#endif
			li[i].SetLFI(4);


	// ---- set all type
		for ( UINT i = 0; i<number_of_items; i++ ) li[i].SetType(vs_system::Tint::ITEM_CLICK);
		

	// ---- set individual type
		li[tint_img_border_id - 1].SetType(vs_system::Tint::ITEM_SWITCH);
		li[tint_img_border_id - 1].SetStatus(FALSE);

		li[tint_img_incr_id   - 1].SetType(vs_system::Tint::ITEM_SWITCH);
		li[tint_img_incr_id   - 1].SetStatus(FALSE);


	// ---- init single
		for (UINT i = 0; i < 5; i++)
		{
			VsFloatRectangle *r = &tint_imgloc_param.single[i];
			r->left   *= 0.5f;
			r->top    *= 0.5f;
			r->right  *= 0.5f;
			r->bottom *= 0.5f;
		}


	// ---- init nono's
		UINT nono_index = 0;
		for (UINT iy = 0; iy < 3; iy++)
		{
			FLOAT yl = (FLOAT)(2-iy) * 0.3333f;
			FLOAT yh = yl + 0.3333f;
			for (UINT ix = 0; ix < 3; ix++)
			{
				FLOAT xl = (FLOAT)ix * 0.3333f;
				FLOAT xh = (FLOAT)(ix + 1) * 0.3333f;
				VsFloatRectangle *n = &tint_imgloc_param.nono[nono_index];
				VsFloatRectangle *nwq = &tint_imgloc_param.nono_with_quad[nono_index];
				nwq->left   = n->left   = xl - 0.5f;
				nwq->right  = n->right  = xh - 0.5f;
				nwq->top    = n->top    = yl - 0.5f;
				nwq->bottom = n->bottom = yh - 0.5f;
				nono_index++;
			}
		}

	// ---- init nono quads
		UINT ni[5] = { 0, 4, 5, 7, 8 };
		for (UINT i = 1; i < 5; i++)
		{
			UINT too_index = ni[i];
			tint_imgloc_param.CopyFloatRectangle(
					&tint_imgloc_param.nono_with_quad[too_index],
					&tint_imgloc_param.single[i]
				);
		}


	// ---- column LF
		li[8].SetColumnLF(TRUE);
		li[19].SetColumnLF(TRUE);


	return number_of_items;
}


// ----------------------------------------------------------------------
INT Model::Tint_ImgLoc_GetIconInfo(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 3;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 1;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
		case vs_system::Tint::ICON_ATLAS_PAGE_ID : return 3;
		default:
			break;
	}
	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_ImgLoc_UpdateModel(
		INT  handle_model,
		INT  element_id
	)
{

	// ---- 0 is not a valid update element id
		if ( element_id == 0 ) return VS_OK;


	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;


	// ---- get ptf
		UINT ptf_id = 0;
		FLOAT pan, tlt, fov;
		vs_system::ObjConfig *obj_config = model->GetObjConfig();

		result = obj_config->GetPanoPTFId(&ptf_id);
		result = obj_config->GetPanoPTFById(ptf_id, &pan, &tlt, &fov);


	// ---- state
		FLOAT inc = tint_imgloc_param.large_increment ? 0.1f : 0.01f;


	// ---- map ptf to rect
		VsFloatRectangle rect;
		tint_imgloc_param.MapPtfToRect(pan, tlt, fov, &rect);


	// ---- minimax dimensions
		FLOAT min_dim = 0.01f;
		FLOAT max_dim = 0.99f;


	// --- process
		switch ( element_id )
		{

			case 1: rect.left   -= inc;
					rect.left    = rect.left < -1 ? -1 : rect.left;
					break;
			case 2: rect.left   += inc;
					rect.left    = rect.left > rect.right ? rect.right - min_dim : rect.left;
					break;
			case 3: rect.right  -= inc;
					rect.right   = rect.right < rect.left ? rect.left + min_dim : rect.right;
					break;
			case 4: rect.right  += inc;
					rect.right   = rect.right > 1 ? 1 : rect.right;
					break;

			case 5: rect.bottom += inc;
					rect.bottom  = rect.bottom < rect.top ? rect.top - min_dim : rect.bottom;
					break;
			case 6: rect.bottom -= inc;
					rect.bottom  = rect.bottom < -1 ? -1 : rect.bottom;
					break;
			case 7: rect.top    += inc;
					rect.top     = rect.top > 1 ? 1 : rect.top;
					break;
			case 8: rect.top    -= inc;
					rect.top     = rect.top > rect.bottom ? rect.bottom + min_dim : rect.top;
					break;

			case 9:
				{
					tint_imgloc_param.CopyFloatRectangle(
							&rect,
							&tint_imgloc_param.single[element_id-9]
						);
				}
				break;

			case 10:
				{
					// ---- reset
						result = obj_config->SetPanoDefault(FALSE);
						result = obj_config->SetPanoDefault(TRUE);
						result = model->Update_ImgPanel_Config(ptf_id);
						result = obj_config->SetPanoPtfSelectedById(ptf_id);

					// ---- REALLY important !!!
						return VS_OK;
				}
				break;

			case 11:
			case 12:
			case 13:
			case 14:
				{
					tint_imgloc_param.CopyFloatRectangle(
							&rect,
							&tint_imgloc_param.single[element_id-10]
						);
				}
				break;

			case tint_img_all_id:
			case tint_img_all_id + 1:
				{

					for (UINT nono_index = 0; nono_index < 9; nono_index++)
					{
						VsFloatRectangle *n = element_id == tint_img_all_id ?
							&(tint_imgloc_param.nono[nono_index]) :
							&(tint_imgloc_param.nono_with_quad[nono_index]);

						UINT  ptf_index = nono_index + 2;
						FLOAT n_pan = pan, n_tilt = tlt, n_fov = fov;

						tint_imgloc_param.MapRectToPtf(
								tint_imgloc_param.HEIGHT,
								&n_pan,
								&n_tilt,
								&n_fov,
								n
							);

						result = obj_config->SetPanoPTFById(ptf_index, n_pan, n_tilt, n_fov);

					}
					
					// ---- reset current ptf
						for (UINT ptf_index = 2; ptf_index < 11; ptf_index++)
						{
							if (ptf_index != ptf_id)
								result = model->Update_ImgPanel_Config(ptf_index);
						}
						result = model->Update_ImgPanel_Config(ptf_id);


					// ---- reselect current
						result = obj_config->SetPanoPtfSelectedById(ptf_id);


					// ---- REALLY important !!!
						return VS_OK;

				}
				break;

			case tint_img_border_id:
				tint_imgloc_param.border_all_flag =
					tint_imgloc_param.border_all_flag ? FALSE : TRUE;
				break;

			case tint_img_border_id+1:
			case tint_img_border_id+2:
				{

					// ---- update border
						//FLOAT border = element_id == tint_img_border_id+1 ? -inc/5 : inc/5;

					 
					// ---- border
						UINT min = tint_imgloc_param.border_all_flag ? 0 : ptf_id - 2;
						UINT max = tint_imgloc_param.border_all_flag ? 9 : ptf_id - 1;
						for (UINT nono_index = min; nono_index < max; nono_index++)
						{

							UINT  ptf_index = nono_index + 2;
							FLOAT n_pan, n_tilt, n_fov;
							VsFloatRectangle rect;

							result = obj_config->GetPanoPTFById(ptf_index, &n_pan, &n_tilt, &n_fov);

							tint_imgloc_param.MapPtfToRect(
									n_pan,
									n_tilt,
									n_fov,
									&rect
								);

							//tint_imgloc_param.BorderFloatRectangle(
							//		element_id == 17 ? -0.01f : 0.01f ,
							//		&rect
							//	);

							UINT zout = tint_img_border_id + 1;
							FLOAT binc = element_id == zout ? -inc : inc;
							{
								rect.left   += binc;
								rect.right  -= binc;
								rect.top    += binc;
								rect.bottom -= binc;
							}

							tint_imgloc_param.MapRectToPtf(
									tint_imgloc_param.HEIGHT,
									&n_pan,
									&n_tilt,
									&n_fov,
									&rect
								);

							result = obj_config->SetPanoPTFById(ptf_index, n_pan, n_tilt, n_fov);

						}


					// ---- reset current ptf
						if ( tint_imgloc_param.border_all_flag )
						{
							for (UINT ptf_index = 2; ptf_index < 11; ptf_index++)
							{
								if (ptf_index != ptf_id)
									result = model->Update_ImgPanel_Config(ptf_index);
							}
						}
						result = model->Update_ImgPanel_Config(ptf_id);


					// ---- reselect current
						result = obj_config->SetPanoPtfSelectedById(ptf_id);


					// ---- REALLY important !!!
						return VS_OK;

				}
				break;

			case tint_img_save_id:
				result = obj_config->  WritePanoConfig();
				break;

			case tint_img_reset_id:
				{
					for (UINT nono_index = 0; nono_index < 9; nono_index++)
					{

						// ---- default
							UINT  ptf_index = nono_index + 2;
							result = obj_config->SetPanoPtfSelectedById(ptf_index);
							result = obj_config->SetPanoDefault(FALSE);
							result = obj_config->SetPanoDefault(TRUE);

						// ---- update all BUT selected
							if (ptf_index != ptf_id)
								result = model->Update_ImgPanel_Config(ptf_index);
					}

					// ---- update selected
						result = model->Update_ImgPanel_Config(ptf_id);
						result = obj_config->SetPanoPtfSelectedById(ptf_id);

					// ---- REALLY important !!!
						return VS_OK;
				}
				break;

			case tint_img_incr_id:
				tint_imgloc_param.large_increment = tint_imgloc_param.large_increment ? FALSE : TRUE;
				break;

			case 30: // all reset
				break;

			default:
				{
					CHAR msg[128];
					sprintf(msg, ">>> Select %d\n", element_id);
					OutputDebugString(msg);
				}
				break;

		}


	// --- update ?
		if ( element_id >= 1 && element_id <= 15)
		{

			// --- map rect to ptf
				FLOAT new_pan  = pan;
				FLOAT new_tilt = tlt;
				FLOAT new_fov  = fov;
				if ( element_id == 1 || element_id == 2 )
				{
					tint_imgloc_param.MapRectToPtf(
							tint_imgloc_param.LEFT,
							&new_pan,
							&new_tilt,
							&new_fov,
							&rect
						);
				}
				else if ( element_id == 3 || element_id == 4)
				{
					tint_imgloc_param.MapRectToPtf(
							tint_imgloc_param.RIGHT,
							&new_pan,
							&new_tilt,
							&new_fov,
							&rect
						);
				}
				else if ( element_id == 7 || element_id == 8 )
				{
					tint_imgloc_param.MapRectToPtf(
							tint_imgloc_param.TOP,
							&new_pan,
							&new_tilt,
							&new_fov,
							&rect
						);
				}
				else if ( element_id == 5 || element_id == 6 )
				{
					tint_imgloc_param.MapRectToPtf(
							tint_imgloc_param.BOTTOM,
							&new_pan,
							&new_tilt,
							&new_fov,
							&rect
						);
				} 
				else if ( element_id >= 9 && element_id <= 15 )
				{
					tint_imgloc_param.MapRectToPtf(
							tint_imgloc_param.HEIGHT,
							&new_pan,
							&new_tilt,
							&new_fov,
							&rect
						);
				}

			// ---- upate
				result = obj_config->SetPanoPTFById(ptf_id, new_pan, new_tilt, new_fov);
				result = model->Update_ImgPanel_Config(ptf_id);

		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_ImgLoc_UpdateElement(
		INT handle_model,
		INT element_id
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_img_setng;


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_TAB_IMGLOC_LID);


	// ---- select elem
		switch ( element_id )
		{
			case 0:

			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:

			case 9:
			case 10:
			case 11:
			case 12:
			case 13:

			case tint_img_all_id:
			case tint_img_all_id + 1:

			case tint_img_border_id:
			case tint_img_border_id + 1:
			case tint_img_border_id + 2:
				result = tint->UpdateText(NULL, tint_img_border_id);
				result = tint->UpdateText(NULL, 1);
				result = tint->UpdateText(NULL, 3);
				result = tint->UpdateText(NULL, 5);
				result = tint->UpdateText(NULL, 7);
				break;

			case 14:
			case tint_img_reset_id:
				result = tint->UpdateText(NULL, 1);
				result = tint->UpdateText(NULL, 3);
				result = tint->UpdateText(NULL, 5);
				result = tint->UpdateText(NULL, 7);
				break;

			default:
				break;

		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_ImgLoc_AddText(
		const INT  handle_obj_config,
		const INT  element_id,
		BOOL      *replace_text,
		BOOL      *add_text,
		CHAR      *text
	)
{

	// ---- init
		*add_text     = FALSE;
		*replace_text = FALSE;
		*text         = NULL;


	// ---- select elem
		switch ( element_id )
		{
			case tint_img_border_id:
				{
					if ( tint_imgloc_param.border_all_flag)
						sprintf(text, "Border: All");
					else
						sprintf(text, "Border: Single");
					*replace_text = TRUE;
				}
				break;

			default:
				break;

		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_ImgLoc_AddValue(
		const INT  handle_obj_config,
		const INT  element_id,
		BOOL      *add_value,
		FLOAT     *value,
		CHAR      *suffix
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_obj_config;


	// ---- get ptf
		UINT ptf_id = 0;
		VsFloatRectangle rect;
		FLOAT pan, tilt, fov;
		vs_system::ObjConfig *obj_config = model->GetObjConfig();
		result = obj_config->GetPanoPTFId(&ptf_id);
		result = obj_config->GetPanoPTFById(ptf_id, &pan, &tilt, &fov);


	// ---- cal rect
		tint_imgloc_param.MapPtfToRect(pan, tilt, fov, &rect);


	// ---- select elem
		*add_value = TRUE;
		sprintf(suffix, "2");
		switch ( element_id )
		{
			case 1:
				*value = rect.left;
				break;

			case 3:
				*value = rect.right;
				break;

			case 5:
				*value = rect.bottom;
				break;

			case 7:
				*value = rect.top;
				break;

			default:
				*add_value = FALSE;
				break;

		}

	return VS_OK;
}
