////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_cam_callback_rroi.hpp ----------
/*!
\file r2_tint_cam_callback_rroi.hpp
\brief
\author Gareth Edwards
*/


using namespace review_02;


// ----------------------------------------------------------------------
INT Model::Tint_Rroi_GetCallbacks(TintCallback *tint_callback)
{
	tint_callback->callback_build_list    = Tint_Rroi_BuildList;
	tint_callback->callback_get_icon_info = Tint_Rroi_GetIconInfo;
	tint_callback->callback_update_model  = Tint_Rroi_UpdateModel;
	tint_callback->callback_update_text   = Tint_Rroi_UpdateText;
	tint_callback->callback_add_text      = Tint_Rroi_AddText;
	tint_callback->callback_add_value     = Tint_Rroi_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Rroi_BuildList(
		TintItem li[]
	)
{

	// ---- build item list
		const INT number_of_items = 4;
		li[0].Set("Horizontal Offset",    ELEM_GROUP_PANOPARAM_LID,  RROI_PARAM_U,         Tint::ITEM_INCDEC,  Tint::ICON_INCDEC  );
		li[1].Set("Vertical Offset",      ELEM_GROUP_PANOPARAM_LID,  RROI_PARAM_V,         Tint::ITEM_INCDEC,  Tint::ICON_INCDEC  );
		li[2].Set("Vertical Zoom Ratio",  ELEM_GROUP_PANOPARAM_LID,  RROI_PARAM_Z,         Tint::ITEM_INCDEC,  Tint::ICON_INCDEC  );
		li[3].Set("Aspect ratio",         ELEM_GROUP_PANOPARAM_LID,  RROI_PARAM_A,         Tint::ITEM_INCDEC,  Tint::ICON_INCDEC  );
		li[4].Set("Save",                 ELEM_GROUP_PANOPARAM_LID,  RROI_PARAM_SAVE,      Tint::ITEM_BUTTON,  Tint::ICON_SAVE    );
		li[5].Set("Reset",                ELEM_GROUP_PANOPARAM_LID,  RROI_PARAM_RESET,     Tint::ITEM_BUTTON,  Tint::ICON_RESET   );
		li[6].Set("Increment",            ELEM_GROUP_PANOPARAM_LID,  RROI_PARAM_TOGGLEINC, Tint::ITEM_BUTTON,  Tint::ICON_TOGGLE  );


	// ---- set offset
		li[7].SetXOff(1.5f);
		li[8].SetXOff(3.0f);

	return number_of_items;
}


// ----------------------------------------------------------------------
INT Model::Tint_Rroi_GetIconInfo(INT id)
{
	switch (id)
	{
		case Tint::TAB_ATLAS_PAGE_ID  : return 1;
		case Tint::TAB_ATLAS_PAGE_COL : return 3;
		case Tint::TAB_ATLAS_PAGE_ROW : return 1;
		case Tint::ICON_ATLAS_PAGE_ID : return 1;
		default:
			break;
	}
	return VS_ERROR;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Rroi_UpdateModel(
		INT  handle_model,
		INT  element_id,
		BOOL large_incr
	)
{

	INT result = VS_OK;
	Model   *model = (Model *)handle_model;


	switch (element_id)
	{

		case PANO_PARAM_INNER_LESS:
			{
				FLOAT min_radius = 0;
				result = model->GetPanoRadiusMin(&min_radius);
				result = model->SetPanoRadiusMin(min_radius - (large_incr ? 0.01f : 0.001f));
				result = model->SetPanoUpdated(TRUE);
			}
			break;

		case PANO_PARAM_INNER_MORE:
			{
				FLOAT min_radius = 0;
				result = model->GetPanoRadiusMin(&min_radius);
				result = model->SetPanoRadiusMin(min_radius + (large_incr ? 0.01f : 0.001f));
				result = model->SetPanoUpdated(TRUE);
			}
			break;

		case PANO_PARAM_OUTER_LESS:
			{
				FLOAT max_radius = 0;
				result = model->GetPanoRadiusMax(&max_radius);
				result = model->SetPanoRadiusMax(max_radius - (large_incr ? 0.01f : 0.001f));
				result = model->SetPanoUpdated(TRUE);
			}
			break;

		case PANO_PARAM_OUTER_MORE:
			{
				FLOAT max_radius = 0;
				result = model->GetPanoRadiusMax(&max_radius);
				result = model->SetPanoRadiusMax(max_radius + (large_incr ? 0.01f : 0.001f));
				result = model->SetPanoUpdated(TRUE);
			}
			break;

		case PANO_PARAM_H_LESS:
			{
				result = model->SetPanoUpdated(TRUE);
			}
			break;

		case PANO_PARAM_H_MORE:
			{
				result = model->SetPanoUpdated(TRUE);
			}
			break;

		case PANO_PARAM_U_LESS:
			{
				FLOAT u_offset = 0, v_offset = 0;
				result = model->GetPanoUVOffset(&u_offset, &v_offset);
				result = model->SetPanoOffset(u_offset - (large_incr ? 0.005f : 0.001f), v_offset);
				result = model->SetPanoUpdated(TRUE);
			}
			break;

		case PANO_PARAM_U_MORE:
			{
				FLOAT u_offset = 0, v_offset = 0;
				result = model->GetPanoUVOffset(&u_offset, &v_offset);
				result = model->SetPanoOffset(u_offset + (large_incr ? 0.005f : 0.001f), v_offset);
				result = model->SetPanoUpdated(TRUE);
			}
			break;

		case PANO_PARAM_V_LESS:
			{
				FLOAT u_offset = 0, v_offset = 0;
				result = model->GetPanoUVOffset(&u_offset, &v_offset);
				result = model->SetPanoOffset(u_offset, v_offset - (large_incr ? 0.005f : 0.001f));
				result = model->SetPanoUpdated(TRUE);
			}
			break;

		case PANO_PARAM_V_MORE:
			{
				FLOAT u_offset = 0, v_offset = 0;
				result = model->GetPanoUVOffset(&u_offset, &v_offset);
				result = model->SetPanoOffset(u_offset, v_offset + (large_incr ? 0.005f : 0.001f));
				result = model->SetPanoUpdated(TRUE);
			}
			break;

		case PANO_PARAM_LU_LESS:
			{
				FLOAT min_angle = 0, max_angle = 0;
				result = model->GetPanoAngleMax(&max_angle);
				result = model->SetPanoAngleMax(max_angle - (large_incr ? 1.0f : 0.1f));
				result = model->SetPanoUpdated(TRUE);
			}
			break;

		case PANO_PARAM_LU_MORE:
			{
				FLOAT max_angle = 0;
				result = model->GetPanoAngleMax(&max_angle);
				result = model->SetPanoAngleMax(max_angle + (large_incr ? 1.0f : 0.1f));
				result = model->SetPanoUpdated(TRUE);
			}
			break;


		case PANO_PARAM_SAVE:
			{
				result = model->SaveObjectAndCameraConfig();
				result = model->SetPanoUpdated(FALSE);
			}
			break;

		case PANO_PARAM_RESET:
			{

				model->SetPanoRadii(0.5f, 1.0f);
				model->SetPanoOffset(0, 0);
				model->SetPanoAngleMax(8);
				result = model->SetPanoUpdated(TRUE);

				{
					INT setup_bitmask = VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING | VS_ELM_SETUP_INDEX | VS_ELM_SETUP_CONFIG,
					result = model->gfx->SetGlobalConfigureBitmask(
							setup_bitmask,
							"Annular"   // "AOI"
						);
					result = model->gfx->SetGlobalConfigureBitmask(
							setup_bitmask,
							"Exposure"  // "AOI"
						);
					result = model->gfx->SetGlobalConfigureBitmask(
							setup_bitmask,
							"2D"        // "*"
						);
					result = model->gfx->SetGlobalConfigureBitmask(
							setup_bitmask,
							"3D"        // "*"
						);
				}

			}
			break;

		case PANO_PARAM_TOGGLEINC:
			break;

		default:
			break;

	}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Rroi_UpdateText(
		INT handle_model,
		INT element_id
	)
{

	INT result = VS_OK;


	Model *model = (Model *)handle_model;
	result = model->pi_model->tint_cam_setng->SetSelectedBody(TINT_HEAD_PANO_LID);


	switch ( element_id )
	{
		case PANO_PARAM_INNER_LESS:
		case PANO_PARAM_INNER_MORE:
			{
				result = model->pi_model->tint_cam_setng->UpdateText(
						ELEM_GROUP_PANOPARAM_LID,
						PANO_PARAM_INNER
					);
			}
			break;
		case PANO_PARAM_OUTER_LESS:
		case PANO_PARAM_OUTER_MORE:
			{
				result = model->pi_model->tint_cam_setng->UpdateText(
						ELEM_GROUP_PANOPARAM_LID,
						PANO_PARAM_OUTER
					);
			}
			break;
		case PANO_PARAM_AOI_INNER_LESS:
		case PANO_PARAM_AOI_INNER_MORE:
			{
				result = model->pi_model->tint_cam_setng->UpdateText(
						ELEM_GROUP_PANOPARAM_LID,
						PANO_PARAM_AOI_INNER
					);
			}
			break;
		case PANO_PARAM_AOI_OUTER_LESS:
		case PANO_PARAM_AOI_OUTER_MORE:
			{
				result = model->pi_model->tint_cam_setng->UpdateText(
						ELEM_GROUP_PANOPARAM_LID,
						PANO_PARAM_AOI_OUTER
					);
			}
			break;
		case PANO_PARAM_H_LESS:
		case PANO_PARAM_H_MORE:
			{
				result = model->pi_model->tint_cam_setng->UpdateText(
						ELEM_GROUP_PANOPARAM_LID,
						PANO_PARAM_H
					);
			}
			break;
		case PANO_PARAM_U_LESS:
		case PANO_PARAM_U_MORE:
			{
				result = model->pi_model->tint_cam_setng->UpdateText(
						ELEM_GROUP_PANOPARAM_LID,
						PANO_PARAM_U
					);
			}
			break;
		case PANO_PARAM_V_LESS:
		case PANO_PARAM_V_MORE:
			{
				result = model->pi_model->tint_cam_setng->UpdateText(
						ELEM_GROUP_PANOPARAM_LID,
						PANO_PARAM_V
					);
			}
			break;
		case PANO_PARAM_LU_LESS:
		case PANO_PARAM_LU_MORE:
			{
				result = model->pi_model->tint_cam_setng->UpdateText(
						ELEM_GROUP_PANOPARAM_LID,
						PANO_PARAM_LU
					);
			}
			break;
		case PANO_PARAM_RESET:
			{
				result = model->pi_model->tint_cam_setng->UpdateText(ELEM_GROUP_PANOPARAM_LID, PANO_PARAM_INNER);
				result = model->pi_model->tint_cam_setng->UpdateText(ELEM_GROUP_PANOPARAM_LID, PANO_PARAM_OUTER);
				result = model->pi_model->tint_cam_setng->UpdateText(ELEM_GROUP_PANOPARAM_LID, PANO_PARAM_U);
				result = model->pi_model->tint_cam_setng->UpdateText(ELEM_GROUP_PANOPARAM_LID, PANO_PARAM_V);
				result = model->pi_model->tint_cam_setng->UpdateText(ELEM_GROUP_PANOPARAM_LID, PANO_PARAM_LU);
			}
			break;
		default:
			break;
	}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Rroi_AddText(
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
INT Model::Tint_Rroi_AddValue(
		const INT  handle_obj_config,
		const INT  element_id,
		BOOL      *add_value,
		FLOAT     *value,
		CHAR      *suffix
	)
{

	// ---- init ----
	*add_value = TRUE;
	*value = 0;


	// ---- local ----
	INT result = VS_OK;
	vs_system::ObjConfig *obj_config = (vs_system::ObjConfig *)handle_obj_config;
	FLOAT p = 0, q = 0;
	INT   i = 0;


	// ---- set ----
	switch (element_id)
	{
		case PANO_PARAM_INNER:
			{
				result = obj_config->GetPanoRadii(&p, &q);
				*value = p;
			}
			break;
		case PANO_PARAM_OUTER:
			{
				result = obj_config->GetPanoRadii(&p, &q);
				*value = q;
			}
			break;

		#ifdef GET_PANO_EXPOSURE
		case PANO_PARAM_AOI_INNER:
			{
				result = obj_config->GetPanoExposureAoi(&p, &q);
				*value = p;
			}
			break;
		case PANO_PARAM_AOI_OUTER:
			{
				result = obj_config->GetPanoExposureAoi(&p, &q);
				*value = q;
			}
			break;
		#endif

		case PANO_PARAM_H:
			{
				result = obj_config->GetPanoCamHOffset(&i);
				*value = (FLOAT)i;
			}
			break;
		case PANO_PARAM_U:
			{
				result = obj_config->GetPanoUVOffset(&p, &q);
				*value = p;
			}
			break;
		case PANO_PARAM_V:
			{
				result = obj_config->GetPanoUVOffset(&p, &q);
				*value = q;
			}
			break;
		case PANO_PARAM_LU:
			{
				result = obj_config->GetPanoAngleMax(&p);
				*value = p;
			}
			break;

		case PANO_PARAM_SAVE:
		case PANO_PARAM_RESET:
		case PANO_PARAM_TOGGLEINC:
			*add_value = FALSE;
			break;

		default:
			break;
	}

	return VS_OK;
}

