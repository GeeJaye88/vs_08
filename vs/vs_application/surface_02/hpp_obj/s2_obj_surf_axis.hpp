////////////////////////////////////////////////////////////////////////////////

// ---------- s2_obj_surf_axis.hpp ----------
/*!
\file s2_obj_surf_axis.hpp
\brief 
\author Gareth Edwards
*/


// ---------- ObjectSurfAxis_Setup ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectSurfAxis_Setup(INT index, Surface_02 *p_s_02)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;


	// ---- create display list
		vs_system::GfxInterface *gfx = pimp->gfx;


	// ---- add ? - or reconfig
		if ( pimp->runtime )
		{
			pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_AXIS_LIST_IDX][0] = gfx->AddElement(
					pimp->scene_dspl_list_handle,
					"",
					1,
					1,
					pimp->obj_config->Callback_SurfAxis_Configure
				);
		}
		else
		{
			pimp->gfx->SetElementConfigureBitmask(
					pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_AXIS_LIST_IDX][0],
					VS_ELM_SETUP_GEOMETRY
				);
		}


	// ---- context
		vs_system::ElmContext *gfx_context = NULL;
		INT hr = gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_AXIS_LIST_IDX][0],
				&gfx_context
			);


	// ---- force geometry setup
		gfx_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
		gfx_context->SetConfigured(FALSE);


	// ---- parameters
		FLOAT *param = gfx_context->GetFloatParameter();
		param[vs_system::ObjConfig::BOX_FACE_MODE]    = 0;
		param[vs_system::ObjConfig::BOX_FACE_TYPE]    = 1;
		param[vs_system::ObjConfig::BOX_FACE_AP_REF]  = 0;
		param[vs_system::ObjConfig::BOX_FACE_RED]     = 0.5f;
		param[vs_system::ObjConfig::BOX_FACE_GREEN]   = 0.5f;
		param[vs_system::ObjConfig::BOX_FACE_BLUE]    = 0.5f;
		param[vs_system::ObjConfig::BOX_FACE_SCALE]   = 1.15f;
		param[vs_system::ObjConfig::BOX_FACE_EXPLODE] = 0.0f;

	// ---- dimensions
		param[vs_system::ObjConfig::BOX_FACE_WIDTH]   = pimp->_configuration.acquire_dimensions.width;
		param[vs_system::ObjConfig::BOX_FACE_HEIGHT]  = pimp->_configuration.acquire_dimensions.height; // hgt not used
		param[vs_system::ObjConfig::BOX_FACE_DEPTH]   = pimp->_configuration.acquire_dimensions.depth;

	// ---- offset to avoid depth render z buffer conflict
		FLOAT axis_offset = pimp->_configuration.acquire_dimensions.depth * 0.001f;
		param[vs_system::ObjConfig::BOX_FACE_X] = axis_offset;
		param[vs_system::ObjConfig::BOX_FACE_Y] = axis_offset;
		param[vs_system::ObjConfig::BOX_FACE_Z] = axis_offset;

	// ---- flag
		param[vs_system::ObjConfig::BOX_FLAG_1] =  1; // axis flag
		param[vs_system::ObjConfig::BOX_FLAG_2] =  1; // ruler flag

	// ---- lighting, materials, etc...
		hr = gfx_context->SetLit(FALSE);
		hr = gfx_context->SetMaterialIndex(5);
		hr = gfx_context->SetCullMode(vs_system::ElmContext::CULL_NONE);

	return VS_OK;
}


// ---------- ObjectSurfAxis_Display ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectSurfAxis_Display(INT index, Surface_02 *p_s_02, BOOL visible)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_AXIS_LIST_IDX][0],
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
		elm_context->SetUseMatrix(TRUE);
		elm_context->SetMatrix(m);
		elm_context->SetViewMatrix(1);


	// ---- pop
		pimp->matrix_stack->Pop();

	return VS_OK;
}
