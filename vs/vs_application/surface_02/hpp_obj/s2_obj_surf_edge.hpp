////////////////////////////////////////////////////////////////////////////////

// ---------- s2_obj_surf_edge.hpp ----------
/*!
\file s2_obj_surf_edge.h
\brief 
\author Gareth Edwards
*/


// ---------- ObjectSurfEdge_Setup ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectSurfEdge_Setup(INT index, Surface_02 *p_s_02)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;


	// ---- edge


	// ---- add element ----
		if ( pimp->runtime )
		{
			pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_EDGE_LIST_IDX][0] =
				pimp->gfx->AddElement(
					pimp->scene_dspl_list_handle,
					"Edge",
					1,
					1,
					&pimp->obj_config->Callback_Mesh_Edge_Configure
				);
		}
		else
		{
			pimp->gfx->SetElementConfigureBitmask(
					pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_EDGE_LIST_IDX][0],
					VS_ELM_SETUP_GEOMETRY
				);
		}

	// ---- get graphics context
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
					pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_EDGE_LIST_IDX][0],
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
		hr = elm_context->SetCullMode(vs_system::ElmContext::CULL_CW);


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
		INT result = ObjectSurfEdge_UpdateParam(index, p_s_02);

	return VS_OK;
}


// ---------- ObjectSurfEdge_Display ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectSurfEdge_Display(INT index, Surface_02 *p_s_02, BOOL visible)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_EDGE_LIST_IDX][0],
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


// ---------- ObjectSurfEdge_Update ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note invoked at setup and when surface mode selected or param is updated

*/
INT Surface_02::ObjectSurfEdge_Update(INT index, Surface_02 *p_s_02)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;


	// ---- elem
		pimp->gfx->SetElementConfigureBitmask(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_EDGE_LIST_IDX][0],
				VS_ELM_SETUP_ALL
			);


	// ---- setup/update common parameters
		INT result = ObjectSurfEdge_UpdateParam(index, p_s_02);
	
	return VS_OK;
}


// ---------- ObjectSurfEdge_UpdateParam ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectSurfEdge_UpdateParam(INT index, Surface_02 *p_s_02)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;


	// ---- elem
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_EDGE_LIST_IDX][0],
				&elm_context
			);


	// ---- setup as per surface requirements
		INT surface_version_selected = p_s_02->GetAcquireSurfaceVersionSelected();


	// ---- parameters
		FLOAT rows = (FLOAT)pimp->_configuration.surface_mesh.rows;
		FLOAT cols = (FLOAT)pimp->_configuration.surface_mesh.cols;


		FLOAT *param = elm_context->GetFloatParameter();
		param[vs_system::ObjConfig::MESH_TYPE]   = (FLOAT)(pimp->_configuration.surface_mesh.solid ? 1 : 0);
		param[vs_system::ObjConfig::MESH_AP_REF] = 0;
		param[vs_system::ObjConfig::MESH_ROWS]   = rows;
		param[vs_system::ObjConfig::MESH_COLS]   = cols;
		param[vs_system::ObjConfig::MESH_ALIGN]  = 1;
		param[vs_system::ObjConfig::MESH_RED]    = 1;
		param[vs_system::ObjConfig::MESH_GREEN]  = 1;
		param[vs_system::ObjConfig::MESH_BLUE]   = 1;


	// ---- dimensions
		param[vs_system::ObjConfig::MESH_WIDTH]  = pimp->_configuration.acquire_dimensions.width;
		param[vs_system::ObjConfig::MESH_DEPTH]  = pimp->_configuration.acquire_dimensions.depth;
		param[vs_system::ObjConfig::MESH_HEIGHT] = pimp->_configuration.acquire_dimensions.height; // hgt not used


	// ---- location
		param[vs_system::ObjConfig::MESH_XOFF]   = 0;
		param[vs_system::ObjConfig::MESH_YOFF]   = 0;
		param[vs_system::ObjConfig::MESH_ZOFF]   = 0;


	// ---- pass heightfield data
		//
		//  note: float OR char heightfield info stored in l+0 to l+2
		//        y heightfield info stored in l+3 to l+4
		//
		LONG *l = elm_context->GetLongParameter();
		*(l + 3) = (LONG)pimp->y_heightfield->GetBuffer();
		*(l + 4) = (LONG)pimp->y_heightfield->GetExtent();

	return VS_OK;
}

