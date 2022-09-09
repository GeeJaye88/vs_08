////////////////////////////////////////////////////////////////////////////////

// ---------- s2_obj_surf_quad.hpp ----------
/*!
\file s2_obj_surf_quad.h
\brief 
\author Gareth Edwards
*/


// ---------- ObjectSurfQuad_Setup ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectSurfQuad_Setup(INT index, Surface_02 *surface_02)
{
	
	// ---- local
		PI_Surface_02 *pimp = surface_02->pi_surface_02;


	// ---- create display list
		vs_system::GfxInterface *gfx = pimp->gfx;


	// ---- create quad ?
		if ( pimp->runtime )
		{
			pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_QUAD_LIST_IDX][0] = gfx->AddElement(
					pimp->scene_dspl_list_handle,
					"",
					1,
					1,
					pimp->obj_config->Callback_Mesh_Quad_Configure
				);
		}
		else
		{
			pimp->gfx->SetElementConfigureBitmask(
					pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_QUAD_LIST_IDX][0],
					VS_ELM_SETUP_GEOMETRY
				);
		}


	// ---- context
		vs_system::ElmContext *elm_context = NULL;
		INT hr = gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_QUAD_LIST_IDX][0],
				&elm_context
			);


	// ---- set context
		hr = elm_context->SetViewMatrix(1);
		hr = elm_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NULL);
		hr = elm_context->SetPick3D(FALSE);
		hr = elm_context->SetFog(FALSE);
		hr = elm_context->SetLit(TRUE);
		hr = elm_context->SetAmbient(TRUE);
		hr = elm_context->SetMaterialIndex(0);
		hr = elm_context->SetTextureMirror(TRUE);
		hr = elm_context->SetCullMode(vs_system::ElmContext::CULL_NONE);


	// ---- shaders
		hr = elm_context->SetShaded(FALSE);
		hr = elm_context->SetShaderIndex(0);


	// ---- matrix
		vs_system::VsMatrixOp mop;
		VsMatrix m;
		mop.Identity(&m);
		hr = elm_context->SetMatrix(&m);
		hr = elm_context->SetUseMatrix(TRUE);


	// ---- setup/update common parameters
		INT result = ObjectSurfQuad_UpdateParam(index, surface_02);


	return VS_OK;
}


// ---------- ObjectSurfQuad_Display ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectSurfQuad_Display(INT index, Surface_02 *surface_02, BOOL visible)
{

	// ---- local
		PI_Surface_02 *pimp = surface_02->pi_surface_02;
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_QUAD_LIST_IDX][0],
				&elm_context
			);

	// ---- visible ?
		elm_context->SetVisible(visible);
		if ( !visible ) return VS_OK;


	// ---- push
		pimp->matrix_stack->Push();
		VsMatrix  *m = &pimp->matrix_stack->stack[pimp->matrix_stack->index];


	// ---- transform
		pimp->matrix_stack->Translate(0, 0, 0);
		elm_context->SetUseMatrix(TRUE);
		elm_context->SetMatrix(m);
		elm_context->SetViewMatrix(1);


	// ---- pop
		pimp->matrix_stack->Pop();

	return VS_OK;
}


// ---------- ObjectSurfEdge_Update ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectSurfQuad_Update(INT index, Surface_02 *surface_02)
{

	// ---- local
		PI_Surface_02 *pimp = surface_02->pi_surface_02;


	// ---- elem
		pimp->gfx->SetElementConfigureBitmask(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_QUAD_LIST_IDX][0],
				VS_ELM_SETUP_ALL
			);

	// ---- setup/update common parameters
		INT result = ObjectSurfQuad_UpdateParam(index, surface_02);

	return VS_OK;
}

// ---------- ObjectSurfEdge_UpdateParam ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectSurfQuad_UpdateParam(INT index, Surface_02 *surface_02)
{
	
	// ---- local
		PI_Surface_02 *pimp = surface_02->pi_surface_02;


	// ---- elem
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_QUAD_LIST_IDX][0],
				&elm_context
			);


	// ---- build  
		FLOAT *param = elm_context->GetFloatParameter();
		param[vs_system::ObjConfig::MESH_TYPE]   = 4;
		param[vs_system::ObjConfig::MESH_AP_REF] = (FLOAT)pimp->_configuration.acquire_atlas_page.ref;
		param[vs_system::ObjConfig::MESH_RED]    = 1;
		param[vs_system::ObjConfig::MESH_GREEN]  = 1;
		param[vs_system::ObjConfig::MESH_BLUE]   = 1;
		param[vs_system::ObjConfig::MESH_WIDTH]  = pimp->_configuration.acquire_dimensions.width;
		param[vs_system::ObjConfig::MESH_DEPTH]  = pimp->_configuration.acquire_dimensions.depth;
		param[vs_system::ObjConfig::MESH_HEIGHT] = pimp->_configuration.acquire_dimensions.height; // hgt not used
		param[vs_system::ObjConfig::MESH_XOFF]   = 0;
		param[vs_system::ObjConfig::MESH_YOFF]   = 0;
		param[vs_system::ObjConfig::MESH_ZOFF]   = 0;


	return VS_OK;
}