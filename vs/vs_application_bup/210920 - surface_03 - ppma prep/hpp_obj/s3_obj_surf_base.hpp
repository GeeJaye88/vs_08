////////////////////////////////////////////////////////////////////////////////

// ---------- s3_obj_surf_base.hpp ----------
/*!
\file s3_obj_surf_base.h
\brief 
\author Gareth Edwards
*/


// ---------- ObjectSurfBase_Setup ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectSurfBase_Setup(INT index, Surface_03 *p_s_03)
{
	
	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;


	// ---- create display list
		vs_system::GfxInterface *gfx = pimp->gfx;


	// ---- create quad ?
		if ( pimp->runtime )
		{
			pimp->gfx_elem_handle[PI_Surface_03::ELM_SURF_BASE_LIST_IDX][0] = gfx->AddElement(
					pimp->scene_dspl_list_handle,
					"",
					1,
					1,
					pimp->obj_config->Callback_SurfMount_Configure
				);
		}
		else
		{
			pimp->gfx->SetElementConfigureBitmask(
					pimp->gfx_elem_handle[PI_Surface_03::ELM_SURF_BASE_LIST_IDX][0],
					VS_ELM_SETUP_GEOMETRY
				);
		}


	// ---- context
		vs_system::ElmContext *elm_context = NULL;
		INT hr = gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_03::ELM_SURF_BASE_LIST_IDX][0],
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


	// ---- init box parameters
		FLOAT *param = elm_context->GetFloatParameter();
		param[vs_system::ObjConfig::BOX_FACE_MODE]    =  (float)0;
		param[vs_system::ObjConfig::BOX_FACE_TYPE]    =  1;
		param[vs_system::ObjConfig::BOX_FACE_AP_REF]  =  108;
		param[vs_system::ObjConfig::BOX_FACE_RED]     =  0.2f;
		param[vs_system::ObjConfig::BOX_FACE_GREEN]   =  0.2f;
		param[vs_system::ObjConfig::BOX_FACE_BLUE]    =  0.2f;
		param[vs_system::ObjConfig::BOX_FACE_SCALE]   =  1.0f;
		param[vs_system::ObjConfig::BOX_FACE_EXPLODE] =  0.0f;

		FLOAT height = pimp->_configuration.acquire_dimensions.depth / 20.0f;
		param[vs_system::ObjConfig::BOX_FACE_WIDTH]   =  pimp->_configuration.acquire_dimensions.width * 1.0f;
		param[vs_system::ObjConfig::BOX_FACE_HEIGHT]  =  height;
		param[vs_system::ObjConfig::BOX_FACE_DEPTH]   =  pimp->_configuration.acquire_dimensions.depth * 1.0f;

		param[vs_system::ObjConfig::BOX_FACE_X]       =  0.0f;
		param[vs_system::ObjConfig::BOX_FACE_Y]       = -height/2 - height/50;
		param[vs_system::ObjConfig::BOX_FACE_Z]       =  0.0f;

	return VS_OK;
}



// ---------- ObjectSurfBase_Display ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectSurfBase_Display(INT index, Surface_03 *p_s_03, BOOL visible)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_03::ELM_SURF_BASE_LIST_IDX][0],
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
