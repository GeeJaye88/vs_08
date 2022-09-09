////////////////////////////////////////////////////////////////////////////////

// ---------- s3_obj_acq_lgt_axis.hpp ----------
/*!
\file s3_obj_acq_lgt_axis.hpp
\brief 
\author Gareth Edwards
*/

// ---------- ObjectAcqAxis_Setup ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectAcqAxis_Setup(INT index, Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;


	// ---- create or (re)config
		if ( pimp->runtime )
		{
			pimp->gfx_elem_handle[PI_Surface_03::ELM_ACQ_LGT_AXIS_LIST_IDX][0] =
				pimp->gfx->AddElement(
					pimp->scene_dspl_list_handle,
					"Rig",
					1,
					1,
					&pimp->obj_config->Callback_S3_AcqLgtAxis_Configure
				);
		}
		else
		{
			pimp->gfx->SetElementConfigureBitmask(
					pimp->gfx_elem_handle[PI_Surface_03::ELM_ACQ_LGT_AXIS_LIST_IDX][0],
					VS_ELM_SETUP_GEOMETRY
				);
		}


	// ---- get graphics context
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
					pimp->gfx_elem_handle[PI_Surface_03::ELM_ACQ_LGT_AXIS_LIST_IDX][0],
					&elm_context
				);

	// ---- force geometry setup
		hr = elm_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
		hr = elm_context->SetConfigured(FALSE);


	// ---- init func parameters
		FLOAT *param = elm_context->GetFloatParameter();
		param[vs_system::ObjConfig::BOX_FACE_MODE]    = 0; // not used
		param[vs_system::ObjConfig::BOX_FACE_TYPE]    = 1;
		param[vs_system::ObjConfig::BOX_FACE_AP_REF]  = 0;
		param[vs_system::ObjConfig::BOX_FACE_RED]     = 0.5f;
		param[vs_system::ObjConfig::BOX_FACE_GREEN]   = 0.5f;
		param[vs_system::ObjConfig::BOX_FACE_BLUE]    = 0.5f;
		param[vs_system::ObjConfig::BOX_FACE_SCALE]   = 1.0f;
		param[vs_system::ObjConfig::BOX_FACE_EXPLODE] = 0.0f;

		param[vs_system::ObjConfig::BOX_FACE_WIDTH]   = pimp->_configuration.acquire_dimensions.width;
		param[vs_system::ObjConfig::BOX_FACE_HEIGHT]  = pimp->_configuration.acquire_dimensions.height;
		param[vs_system::ObjConfig::BOX_FACE_DEPTH]   = pimp->_configuration.acquire_dimensions.depth;

		FLOAT func_axis_off = pimp->_configuration.acquire_dimensions.depth/2;
		param[vs_system::ObjConfig::BOX_FACE_X] =  func_axis_off;
		param[vs_system::ObjConfig::BOX_FACE_Y] =  0.0f;
		param[vs_system::ObjConfig::BOX_FACE_Z] = -func_axis_off;

		param[vs_system::ObjConfig::BOX_FLAG_1] = 1;  // axis flag
		param[vs_system::ObjConfig::BOX_FLAG_2] = 1;  // light flag
		param[vs_system::ObjConfig::BOX_FLAG_3] = 1;  // radius flag
		param[vs_system::ObjConfig::BOX_FLAG_4] = 0;
		param[vs_system::ObjConfig::BOX_FLAG_5] = 0;
		param[vs_system::ObjConfig::BOX_FLAG_6] = 0;
		param[vs_system::ObjConfig::BOX_FLAG_7] = 0;

	// ---- lighting, materials, etc...
		hr = elm_context->SetLit(FALSE);
		hr = elm_context->SetMaterialIndex(5);
		hr = elm_context->SetCullMode(vs_system::ElmContext::CULL_NONE);


	// ---- setup/update common parameters
		INT result = ObjectAcqAxis_UpdateParam(index, p_s_03);

	return VS_OK;
}


// ---------- ObjectAcqAxis_Display ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectAcqAxis_Display(INT index, Surface_03 *p_s_03, BOOL visible)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_03::ELM_ACQ_LGT_AXIS_LIST_IDX][0],
				&elm_context
			);


	// ---- visible ?
		elm_context->SetVisible(visible);
		if ( !visible ) return VS_OK;


	// ---- push
		pimp->matrix_stack->Push();
		VsMatrix  *m = &pimp->matrix_stack->stack[pimp->matrix_stack->index];


	// ---- transform
		pimp->matrix_stack->Translate(
				-pimp->_configuration.acquire_dimensions.width / 2,
				0, 
				pimp->_configuration.acquire_dimensions.depth / 2
			);

	// ---- transform
		elm_context->SetUseMatrix(TRUE);
		elm_context->SetMatrix(m);
		elm_context->SetViewMatrix(1);

	// ---- pop
		pimp->matrix_stack->Pop();

	return VS_OK;
}


// ---------- ObjectAcqAxis_Update ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note invoked at setup and when surface mode selected or param is updated

*/
INT Surface_03::ObjectAcqAxis_Update(INT index, Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;


	// ---- elem
		pimp->gfx->SetElementConfigureBitmask(
				pimp->gfx_elem_handle[PI_Surface_03::ELM_ACQ_LGT_AXIS_LIST_IDX][0],
				VS_ELM_SETUP_ALL
			);


	// ---- setup/update common parameters
		INT result = ObjectAcqAxis_UpdateParam(index, p_s_03);
	
	return VS_OK;
}


// ---------- ObjectAcqAxis_UpdateParam ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectAcqAxis_UpdateParam(INT index, Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;


	// ---- elem
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_03::ELM_ACQ_LGT_AXIS_LIST_IDX][0],
				&elm_context
			);

	// ---- param
		FLOAT *param = elm_context->GetFloatParameter();
		param[vs_system::ObjConfig::BOX_FLAG_4] = pimp->_configuration.acquire_light_param.intensity;
		param[vs_system::ObjConfig::BOX_FLAG_5] = pimp->_configuration.acquire_light_param.distance;
		param[vs_system::ObjConfig::BOX_FLAG_6] = pimp->_configuration.acquire_light_param.elevation;
		param[vs_system::ObjConfig::BOX_FLAG_7] = pimp->_configuration.acquire_light_param.azimuth;

	return VS_OK;
}

