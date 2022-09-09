////////////////////////////////////////////////////////////////////////////////

// ---------- s3_obj_loi_rule.hpp ----------
/*!
\file s3_obj_loi_rule.hpp
\brief 
\author Gareth Edwards
*/


// ---------- ObjectLoiRule_Setup ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiRule_Setup(INT gfx_object_index, Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;


	// ---- create display list
		vs_system::GfxInterface *gfx = pimp->gfx;

	
	// ---- elem
		for (int i = 0; i < PI_Surface_03::MAX_EXP_LOI_; i++)
		{

			// ---- create ? - or reconfig
				if ( pimp->runtime )
				{
					pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_RULE_LIST_IDX][i] = gfx->AddElement(
							pimp->scene_dspl_list_handle,
							"",
							1,
							1,
							pimp->obj_config->Callback_S3_Loi_Ruler_Configure
						);
				}
				else
				{
					pimp->gfx->SetElementConfigureBitmask(
							pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_RULE_LIST_IDX][i],
							VS_ELM_SETUP_GEOMETRY
						);
				}


			// ---- context
				vs_system::ElmContext *elm_context = NULL;
				INT hr = gfx->GetElementContext(
						pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_RULE_LIST_IDX][i],
						&elm_context
					);


			// ---- context
				hr = elm_context->SetViewMatrix(1);
				hr = elm_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NULL);
				hr = elm_context->SetPick3D(FALSE);
				hr = elm_context->SetFog(FALSE);
				hr = elm_context->SetLit(FALSE);
				hr = elm_context->SetMaterialIndex(6);
				hr = elm_context->SetCullMode(vs_system::ElmContext::CULL_NONE);


			// ---- get target loi
				hr = elm_context->SetUseMatrix(TRUE);


			// ---- param
				FLOAT *param = elm_context->GetFloatParameter();
				param[vs_system::ObjConfig::BOX_FACE_MODE]    = 0;
				param[vs_system::ObjConfig::BOX_FACE_TYPE]    = 1;
				param[vs_system::ObjConfig::BOX_FACE_AP_REF]  = 0;
				param[vs_system::ObjConfig::BOX_FACE_RED]     = 0.5f;
				param[vs_system::ObjConfig::BOX_FACE_GREEN]   = 0.5f;
				param[vs_system::ObjConfig::BOX_FACE_BLUE]    = 0.5f;

			// ---- uncalculated
				param[vs_system::ObjConfig::BOX_FACE_WIDTH] = pimp->_configuration.acquire_dimensions.width;
				param[vs_system::ObjConfig::BOX_FACE_DEPTH] = pimp->_configuration.acquire_dimensions.depth;

			// ---- location
				param[vs_system::ObjConfig::BOX_FACE_X] = 0;
				param[vs_system::ObjConfig::BOX_FACE_Y] = 0;
				param[vs_system::ObjConfig::BOX_FACE_Z] = 0;

				param[vs_system::ObjConfig::BOX_FLAG_1] =   0;  // angle
				param[vs_system::ObjConfig::BOX_FLAG_2] =  10;  // y_top
				param[vs_system::ObjConfig::BOX_FLAG_3] = -10;  // y_bot

				param[vs_system::ObjConfig::BOX_FLAG_4] = 0;  // x3
				param[vs_system::ObjConfig::BOX_FLAG_5] = 0;  // y3
				param[vs_system::ObjConfig::BOX_FLAG_6] = 0;  // x4
				param[vs_system::ObjConfig::BOX_FLAG_7] = 0;  // y4

			// ---- marks_per_unit
				param[vs_system::ObjConfig::BOX_FLAG_8] = 1;
		}

	return VS_OK;
}


// ---------- ObjectLoiRule_Display ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiRule_Display(INT gfx_object_index, Surface_03 *p_s_03, BOOL visible)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;
		UINT target_index = p_s_03->GetTargetLoiIndex();
		if (target_index == 999) return false;


	// ----- target init ?
		BOOL init = p_s_03->GetTargetLoiInit();


	// ---- elem
		for (int i = 0; i < PI_Surface_03::MAX_EXP_LOI_; i++)
		{

			// ---- context
				vs_system::ElmContext *elm_context = NULL;
				INT result = pimp->gfx->GetElementContext(
						pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_RULE_LIST_IDX][i],
						&elm_context
					);

			// ---- visible ?
				BOOL elm_visible = (i == target_index ? TRUE : FALSE) && visible;
				elm_context->SetVisible(elm_visible);
				if ( elm_visible )
				{

					// ---- push
						pimp->matrix_stack->Push();
						VsMatrix  *m = &pimp->matrix_stack->stack[pimp->matrix_stack->index];
						
					// ---- object y offset just above top of surface
						FLOAT yoff = 1;

					// ---- transform
						VsVector3 *v = p_s_03->GetTargetLoi(i);
						pimp->matrix_stack->Translate(0, v->y + yoff, 0);
						result = elm_context->SetUseMatrix(TRUE);
						result = elm_context->SetMatrix(m);

					// ---- pop
						pimp->matrix_stack->Pop();

					// ---- NOT line rect intersect initialised ?
						if ( !init )
						{
							result = ObjectLoiRule_Update(p_s_03);
						}

				}
		}

	return VS_OK;
}


// ---------- ObjectLoiRule_HideAll ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiRule_HideAll(Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;

	// ---- hide all
		for (int i = 0; i < PI_Surface_03::MAX_EXP_LOI_; i++)
		{
			// ---- context
				vs_system::ElmContext *elm_context = NULL;
				INT result = pimp->gfx->GetElementContext(
						pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_RULE_LIST_IDX][i],
						&elm_context
					);

			// ---- visible ?
				elm_context->SetVisible(FALSE);

		}

	return VS_OK;
}


// ---------- ObjectLoiRule_Update ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiRule_Update(Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;
		UINT target_index = p_s_03->GetTargetLoiIndex();
		if (target_index == 999) return false;


	// ---- reconfigure
		INT hr = pimp->gfx->SetElementConfigureBitmask(
				pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_RULE_LIST_IDX][target_index],
				VS_ELM_SETUP_GEOMETRY
			);


	// ---- context
		vs_system::ElmContext *elm_context = NULL;
		hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_RULE_LIST_IDX][target_index],
				&elm_context
			);

	// ---- y parameters
		FLOAT y_loc = pimp->_configuration.target_loi[target_index].y;
		FLOAT y_sca = p_s_03->GetTargetLoiHeightScalar();
		FLOAT y_min = p_s_03->GetAcquireHeightFieldMin() * y_sca;
		FLOAT y_max = p_s_03->GetAcquireHeightFieldMax() * y_sca;


	// ---- y_top & y_bot
		FLOAT y_ext = y_max - y_loc;
		FLOAT y_hgt = y_ext + ( y_loc - y_min);


	// ---- store parameters
		FLOAT *param = elm_context->GetFloatParameter();

		// ---- where
			param[vs_system::ObjConfig::BOX_FACE_X] = pimp->_configuration.target_loi[target_index].x;
			param[vs_system::ObjConfig::BOX_FACE_Z] = pimp->_configuration.target_loi[target_index].z;

		// ---- at
			param[vs_system::ObjConfig::BOX_FLAG_1] = pimp->_configuration.target_rot[target_index]; // angle
			param[vs_system::ObjConfig::BOX_FLAG_2] =  y_ext; // y_top 
			param[vs_system::ObjConfig::BOX_FLAG_3] = -y_hgt; // y_bot

		// ---- between
			param[vs_system::ObjConfig::BOX_FLAG_4] = pimp->_configuration.target_intersect[0].x;  // x3
			param[vs_system::ObjConfig::BOX_FLAG_5] = pimp->_configuration.target_intersect[0].y;  // y3
			param[vs_system::ObjConfig::BOX_FLAG_6] = pimp->_configuration.target_intersect[1].x;  // x4
			param[vs_system::ObjConfig::BOX_FLAG_7] = pimp->_configuration.target_intersect[1].y;  // y4

		// ---- marks_per_unit
			param[vs_system::ObjConfig::BOX_FLAG_8] = 1;

	return VS_OK;
}
