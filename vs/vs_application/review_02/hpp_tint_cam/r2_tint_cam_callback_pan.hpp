////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_cam_callback_pan.hpp ----------
/*!
\file r2_tint_cam_callback_pan.hpp
\brief
\author Gareth Edwards
*/


using namespace review_02;


// -------- Functionality and Parameters for Tabbed Interface Pano

class Model::Tint_Pan_Param
{

	public:

	// ---- cdtor
		Tint_Pan_Param::Tint_Pan_Param(){;};
		Tint_Pan_Param::~Tint_Pan_Param(){;};


	// --- props
		BOOL _large_incr = TRUE;


	// --- logical id's
		enum
		{
			INNER          = 1,
			INNER_LESS     = 1 + INNER,
			INNER_MORE     = 2 + INNER,
			OUTER          = 4,
			OUTER_LESS     = 1 + OUTER,
			OUTER_MORE     = 2 + OUTER,
			AOI_INNER      = 7,
			AOI_INNER_LESS = 1 + AOI_INNER,
			AOI_INNER_MORE = 2 + AOI_INNER,
			AOI_OUTER      = 10,
			AOI_OUTER_LESS = 1 + AOI_OUTER,
			AOI_OUTER_MORE = 2 + AOI_OUTER,
			H              = 13,
			H_LESS         = 1 + H,
			H_MORE         = 2 + H,
			U              = 18,
			U_LESS         = 1 + U,
			U_MORE         = 2 + U,
			V              = 21,
			V_LESS         = 1 + V,
			V_MORE         = 2 + V,
			LU             = 24,
			LU_LESS        = 1 + LU,
			LU_MORE        = 2 + LU,
			SAVE           = 27,
			RESET          = 28,
			TOGGLEINC      = 29
		};


} tint_pano_param;


// ----------------------------------------------------------------------
INT Model::Tint_Pan_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_Pan_BuildList;
	tint_callback->callback_get_icon_info  = Tint_Pan_GetIconInfo;
	tint_callback->callback_update_model   = Tint_Pan_UpdateModel;
	tint_callback->callback_update_element = Tint_Pan_UpdateElement;
	tint_callback->callback_add_actions    = NULL;
	tint_callback->callback_add_text       = Tint_Pan_AddText;
	tint_callback->callback_add_value      = Tint_Pan_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Pan_BuildList(
		vs_system::TintItem li[]
	)
{

	// ---- local
		INT number_of_items = 15;
		UINT gid = ELEM_GROUP_PANO_LID;


		using namespace vs_system;


	// ---- describe
		INT i = -1;
		li[++i].Set("Radius Min",    gid, tint_pano_param.INNER_LESS);
		li[++i].Set("",              gid, tint_pano_param.INNER_MORE);
		li[++i].Set("Radius Max",    gid, tint_pano_param.OUTER_LESS);
		li[++i].Set("",              gid, tint_pano_param.OUTER_MORE);
		li[++i].Set("H Offset",      gid, tint_pano_param.H_LESS);
		li[++i].Set("",              gid, tint_pano_param.H_MORE);
		li[++i].Set("U Offset",      gid, tint_pano_param.U_LESS);
		li[++i].Set("",              gid, tint_pano_param.U_MORE);
		li[++i].Set("V Offset",      gid, tint_pano_param.V_LESS);
		li[++i].Set("",              gid, tint_pano_param.V_MORE);
		li[++i].Set("Look Up Angle", gid, tint_pano_param.LU_LESS);
		li[++i].Set("",              gid, tint_pano_param.LU_MORE);
		li[++i].Set("Save",          gid, tint_pano_param.SAVE);
		li[++i].Set("Reset",         gid, tint_pano_param.RESET);
		li[++i].Set("Increment",     gid, tint_pano_param.TOGGLEINC);


	// ---- detail
		i = -1;
		li[++i].SetAtlas(3, 5, 0);
		li[++i].SetAtlas(3, 5, 2); li[i].SetXOff(1.1f); li[i].SetLF(TRUE);
		li[++i].SetAtlas(3, 5, 0);
		li[++i].SetAtlas(3, 5, 2); li[i].SetXOff(1.1f); li[i].SetLF(TRUE);
		li[++i].SetAtlas(3, 5, 0);
		li[++i].SetAtlas(3, 5, 2); li[i].SetXOff(1.1f); li[i].SetLF(TRUE);
		li[++i].SetAtlas(3, 5, 0);
		li[++i].SetAtlas(3, 5, 2); li[i].SetXOff(1.1f); li[i].SetLF(TRUE);
		li[++i].SetAtlas(3, 5, 0);
		li[++i].SetAtlas(3, 5, 2); li[i].SetXOff(1.1f); li[i].SetLF(TRUE);
		li[++i].SetAtlas(3, 5, 0);
		li[++i].SetAtlas(3, 5, 2); li[i].SetXOff(1.1f); li[i].SetLF(TRUE);  li[i].SetLFI(8);

		li[++i].SetAtlas(3, 3, 3);
		li[++i].SetAtlas(3, 3, 4); li[i].SetXOff(1.5f);
		li[++i].SetAtlas(3, 4, 3); li[i].SetXOff(3.0f); li[i].SetLF(TRUE);  li[i].SetLFI(8);


	// ---- detail
		for (INT i = 0; i < number_of_items; i++)
		{
			if ( i == 14 )
				li[i].SetType(vs_system::Tint::ITEM_SWITCH);
			else
				li[i].SetType(vs_system::Tint::ITEM_CLICK);
		}

	return number_of_items;
}


// ----------------------------------------------------------------------
INT Model::Tint_Pan_GetIconInfo(INT id)
{
	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 1;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 3;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
		case vs_system::Tint::ICON_ATLAS_PAGE_ID : return 1;
		default:
			break;
	}
	return VS_ERROR;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Pan_UpdateModel(
		INT  handle_model,
		INT  element_id
	)
{

	// ---- local
		BYTE setup_bitmask = VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_INDEX | VS_ELM_SETUP_CONFIG;
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::ObjConfig *obj_config = model->GetObjConfig();
		vs_system::AppDependency *app_dependency = model->pi_model->app_dependency;
		BOOL   large_incr = tint_pano_param._large_incr ? TRUE : FALSE;


	// ---- select
		switch (element_id)
		{

			case tint_pano_param.INNER_LESS:
				{
					FLOAT min_radius = 0;
					result = obj_config->GetPanoRadiusMin(&min_radius);
					result = obj_config->SetPanoRadiusMin(min_radius - (large_incr ? 0.01f : 0.001f));
					result = obj_config->SetPanoUpdated(TRUE);
				}
				break;

			case tint_pano_param.INNER_MORE:
				{
					FLOAT min_radius = 0;
					result = obj_config->GetPanoRadiusMin(&min_radius);
					result = obj_config->SetPanoRadiusMin(min_radius + (large_incr ? 0.01f : 0.001f));
					result = obj_config->SetPanoUpdated(TRUE);
				}
				break;

			case tint_pano_param.OUTER_LESS:
				{
					FLOAT max_radius = 0;
					result = obj_config->GetPanoRadiusMax(&max_radius);
					result = obj_config->SetPanoRadiusMax(max_radius - (large_incr ? 0.01f : 0.001f));
					result = obj_config->SetPanoUpdated(TRUE);
				}
				break;

			case tint_pano_param.OUTER_MORE:
				{
					FLOAT max_radius = 0;
					result = obj_config->GetPanoRadiusMax(&max_radius);
					result = obj_config->SetPanoRadiusMax(max_radius + (large_incr ? 0.01f : 0.001f));
					result = obj_config->SetPanoUpdated(TRUE);
				}
				break;

			case tint_pano_param.H_LESS:
				{
					vs_system::CamProps *cam_props = NULL;
					result = model->GetCamProps(&cam_props);

					FLOAT h_offset = 0;
					result = cam_props->GetProperty(
							vs_system::CamProps::ROI,
							0,
							vs_system::CamProps::H_OFFSET,
							&h_offset
						);

					h_offset -= large_incr ? 24 : 8;
					h_offset  = h_offset < 0 ? 0 : h_offset;

					result = cam_props->SetProperty(
							vs_system::CamProps::ROI,
							0,
							vs_system::CamProps::H_OFFSET,
							h_offset
						);

					result = obj_config->SetPanoUpdated(TRUE);
				}
				break;

			case tint_pano_param.H_MORE:
				{
					vs_system::CamProps *cam_props = NULL;
					result = model->GetCamProps(&cam_props);

					FLOAT h_offset = 0;
					result = cam_props->GetProperty(
							vs_system::CamProps::ROI,
							0,
							vs_system::CamProps::H_OFFSET,
							&h_offset
						);

					h_offset += large_incr ? 24 : 8;

					result = cam_props->SetProperty(
							vs_system::CamProps::ROI,
							0,
							vs_system::CamProps::H_OFFSET,
							h_offset
						);

					result = obj_config->SetPanoUpdated(TRUE);
				}
				break;

			case tint_pano_param.U_LESS:
				{
					FLOAT u_offset = 0, v_offset = 0;
					result = obj_config->GetPanoUVOffset(&u_offset, &v_offset);
					result = obj_config->SetPanoOffset(u_offset - (large_incr ? 0.005f : 0.001f), v_offset);
					result = obj_config->SetPanoUpdated(TRUE);
				}
				break;

			case tint_pano_param.U_MORE:
				{
					FLOAT u_offset = 0, v_offset = 0;
					result = obj_config->GetPanoUVOffset(&u_offset, &v_offset);
					result = obj_config->SetPanoOffset(u_offset + (large_incr ? 0.005f : 0.001f), v_offset);
					result = obj_config->SetPanoUpdated(TRUE);
				}
				break;

			case tint_pano_param.V_LESS:
				{
					FLOAT u_offset = 0, v_offset = 0;
					result = obj_config->GetPanoUVOffset(&u_offset, &v_offset);
					result = obj_config->SetPanoOffset(u_offset, v_offset - (large_incr ? 0.005f : 0.001f));
					result = obj_config->SetPanoUpdated(TRUE);
				}
				break;

			case tint_pano_param.V_MORE:
				{
					FLOAT u_offset = 0, v_offset = 0;
					result = obj_config->GetPanoUVOffset(&u_offset, &v_offset);
					result = obj_config->SetPanoOffset(u_offset, v_offset + (large_incr ? 0.005f : 0.001f));
					result = obj_config->SetPanoUpdated(TRUE);
				}
				break;

			case tint_pano_param.LU_LESS:
				{
					FLOAT min_angle = 0, max_angle = 0;
					result = obj_config->GetPanoAngleMax(&max_angle);
					result = obj_config->SetPanoAngleMax(max_angle - (large_incr ? 1.0f : 0.1f));
					result = obj_config->SetPanoUpdated(TRUE);
				}
				break;

			case tint_pano_param.LU_MORE:
				{
					FLOAT max_angle = 0;
					result = obj_config->GetPanoAngleMax(&max_angle);
					result = obj_config->SetPanoAngleMax(max_angle + (large_incr ? 1.0f : 0.1f));
					result = obj_config->SetPanoUpdated(TRUE);
				}
				break;


			case tint_pano_param.SAVE:
				{
					result = model->SaveObjectAndCameraConfig();
					result = obj_config->SetPanoUpdated(FALSE);
				}
				break;

			case tint_pano_param.RESET:
				{
					obj_config->SetPanoRadii(0.5f, 1.0f);
					obj_config->SetPanoOffset(0, 0);
					obj_config->SetPanoAngleMax(8);
					result = obj_config->SetPanoUpdated(TRUE);
				}
				break;

			case tint_pano_param.TOGGLEINC:
				tint_pano_param._large_incr = tint_pano_param._large_incr ? FALSE : TRUE;
				break;

			default:
				setup_bitmask = VS_ELM_SETUP_NONE;
				break;

		}

	
	// ---- set dependent config bitmasks
		if ( setup_bitmask != VS_ELM_SETUP_NONE )
		{
			#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
				;
			#else
				vs_system::GfxInterface  *gfx_interface = app_dependency->GetGfxInterface();
				result = gfx_interface->SetGlobalConfigureBitmask(
						setup_bitmask,
						"Annular"   // "AOI"
					);
				result = gfx_interface->SetGlobalConfigureBitmask(
						setup_bitmask,
						"Exposure"  // "AOI"
					);
				result = gfx_interface->SetGlobalConfigureBitmask(
						setup_bitmask,
						"2D"        // "*"
					);
				result = gfx_interface->SetGlobalConfigureBitmask(
						setup_bitmask,
						"3D"        // "*"
					);
			#endif
		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Pan_UpdateElement(
		INT handle_model,
		INT element_id
	)
{

	// ---- local
		BYTE setup_bitmask = VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_INDEX | VS_ELM_SETUP_CONFIG;
		INT result = VS_OK;
		Model  *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_cam_setng;


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_HEAD_PANO_LID);


	// ---- select
		switch ( element_id )
		{
			case tint_pano_param.INNER_LESS:
			case tint_pano_param.INNER_MORE:
				{
					result = model->pi_model->tint_cam_setng->UpdateText(
							ELEM_GROUP_PANO_LID,
							tint_pano_param.INNER_LESS
						);
				}
				break;
			case tint_pano_param.OUTER_LESS:
			case tint_pano_param.OUTER_MORE:
				{
					result = model->pi_model->tint_cam_setng->UpdateText(
							ELEM_GROUP_PANO_LID,
							tint_pano_param.OUTER_LESS
						);
				}
				break;
			case tint_pano_param.H_LESS:
			case tint_pano_param.H_MORE:
				{
					result = model->pi_model->tint_cam_setng->UpdateText(
							ELEM_GROUP_PANO_LID,
							tint_pano_param.H_LESS
						);
				}
				break;
			case tint_pano_param.U_LESS:
			case tint_pano_param.U_MORE:
				{
					result = model->pi_model->tint_cam_setng->UpdateText(
							ELEM_GROUP_PANO_LID,
							tint_pano_param.U_LESS
						);
				}
				break;
			case tint_pano_param.V_LESS:
			case tint_pano_param.V_MORE:
				{
					result = model->pi_model->tint_cam_setng->UpdateText(
							ELEM_GROUP_PANO_LID,
							tint_pano_param.V_LESS
						);
				}
				break;
			case tint_pano_param.LU_LESS:
			case tint_pano_param.LU_MORE:
				{
					result = model->pi_model->tint_cam_setng->UpdateText(
							ELEM_GROUP_PANO_LID,
							tint_pano_param.LU_LESS
						);
				}
				break;
			case tint_pano_param.RESET:
				{
					result = model->pi_model->tint_cam_setng->UpdateText(ELEM_GROUP_PANO_LID, tint_pano_param.INNER_LESS);
					result = model->pi_model->tint_cam_setng->UpdateText(ELEM_GROUP_PANO_LID, tint_pano_param.OUTER_LESS);
					result = model->pi_model->tint_cam_setng->UpdateText(ELEM_GROUP_PANO_LID, tint_pano_param.U_LESS);
					result = model->pi_model->tint_cam_setng->UpdateText(ELEM_GROUP_PANO_LID, tint_pano_param.H_LESS);
					result = model->pi_model->tint_cam_setng->UpdateText(ELEM_GROUP_PANO_LID, tint_pano_param.V_LESS);
					result = model->pi_model->tint_cam_setng->UpdateText(ELEM_GROUP_PANO_LID, tint_pano_param.LU_LESS);
				}
				break;
			default:
				break;
		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Pan_AddText(
		const INT  handle_model,
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
INT Model::Tint_Pan_AddValue(
		const INT  handle_model,
		const INT  element_id,
		BOOL      *add_value,
		FLOAT     *value,
		CHAR      *suffix
	)
{

	// ---- init
		*add_value = TRUE;
		*value = 0;
		sprintf(suffix, "2");


	// ---- local
		BYTE setup_bitmask = VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_INDEX | VS_ELM_SETUP_CONFIG;
		INT result = VS_OK;
		Model  *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_cam_setng;
		vs_system::ObjConfig *obj_config = model->pi_model->obj_config;


	// ---- local ----
		FLOAT p = 0, q = 0;
		INT   i = 0;


	// ---- set ----
		switch (element_id)
		{
			case tint_pano_param.INNER_LESS:
				{
					result = obj_config->GetPanoRadii(&p, &q);
					*value = p;
				}
				break;
			case tint_pano_param.OUTER_LESS:
				{
					result = obj_config->GetPanoRadii(&p, &q);
					*value = q;
				}
				break;
			case tint_pano_param.H_LESS:
				{
					vs_system::CamProps *cam_props = NULL;
					result = model->GetCamProps(&cam_props);

					FLOAT h_offset = 0;
					result = cam_props->GetProperty(
							vs_system::CamProps::ROI,
							0,
							vs_system::CamProps::H_OFFSET,
							&h_offset
						);
					*value = h_offset;
				}
				break;
			case tint_pano_param.U_LESS:
				{
					result = obj_config->GetPanoUVOffset(&p, &q);
					*value = p;
				}
				break;
			case tint_pano_param.V_LESS:
				{
					result = obj_config->GetPanoUVOffset(&p, &q);
					*value = q;
				}
				break;
			case tint_pano_param.LU_LESS:
				{
					result = obj_config->GetPanoAngleMax(&p);
					*value = p;
				}
				break;

			default:
				*add_value = FALSE;
				break;
		}

	return VS_OK;
}

