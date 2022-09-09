////////////////////////////////////////////////////////////////////////////////

// ---------- c1_cube.hpp ----------
/*!
\file c1_cube.hpp
\brief Implementation of the Model cube methods
\author Gareth Edwards
*/


INT Model::SetupCubeDisplay()
{

	// ---- local
		vs_system::VsMatrixStack *vms = pi_model->matrix_stack;


	// ---- push
		vms->Push();


	// ---- matrix stuff
		VsMatrix  *m = &vms->stack[vms->index];
		vms->Rotate( 0, 'Y');
		vms->Translate( 0, 0, 0);
		vms->Rotate( 0, 'Y');


	// ---- world matrix
		for (int i = 0; i < pi_model->number_of_cube_elem; i++)
		{
			vs_system::ElmContext *elm_context = NULL;
			INT hr = pi_model->gfx->GetElementContext(pi_model->handle_cube_elem[i], &elm_context);
			elm_context->SetUseMatrix(TRUE);
			elm_context->SetMatrix(m);
			elm_context->SetViewMatrix(pi_model->gfx_view_matrix_index);
			elm_context->SetVisible(pi_model->show_cube);
		}


	// ---- pop
		vms->Pop();

	return VS_OK;
}


INT Model::SetupCubeObjects()
{

	// ---- create display list
		vs_system::GfxInterface *gfx = pi_model->gfx;


	// ---- create display list ?
		if ( pi_model->handle_dspl == NULL )
		{
			pi_model->handle_dspl = gfx->AddDisplayList("Scene", 1, 1);
		}


	// ---- for each face
		for (int i = 0; i < pi_model->number_of_cube_elem; i ++)
		{

			// ---- create quad ?
				if ( pi_model->handle_cube_elem[i] == NULL )
				{
					pi_model->handle_cube_elem[i] = gfx->AddElement(
							pi_model->handle_dspl,
							"",
							1,
							1,
							pi_model->obj_config->Callback_BoxFace_Configure
						);
				}


			// ---- context
				vs_system::ElmContext *gfx_context = NULL;
				INT hr = gfx->GetElementContext(pi_model->handle_cube_elem[i], &gfx_context);


			// ---- force geometry setup
				gfx_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
				gfx_context->SetConfigured(FALSE);


			// ---- init sphere parameters
				FLOAT *param = gfx_context->GetFloatParameter();
				param[vs_system::ObjConfig::BOX_FACE_MODE]    = (float)i;
				param[vs_system::ObjConfig::BOX_FACE_TYPE]    = 2;
				param[vs_system::ObjConfig::BOX_FACE_AP_REF]  = 108;
				param[vs_system::ObjConfig::BOX_FACE_RED]     = 1;
				param[vs_system::ObjConfig::BOX_FACE_GREEN]   = 1;
				param[vs_system::ObjConfig::BOX_FACE_BLUE]    = 1;
				param[vs_system::ObjConfig::BOX_FACE_SCALE]   = 20.0f;
				param[vs_system::ObjConfig::BOX_FACE_EXPLODE] = 0.5f;
				param[vs_system::ObjConfig::BOX_FACE_WIDTH]   = 1.0f;
				param[vs_system::ObjConfig::BOX_FACE_HEIGHT]  = 1.0f;
				param[vs_system::ObjConfig::BOX_FACE_DEPTH]   = 1.0f;


			// ---- lighting and materials
				hr = gfx_context->SetLit(TRUE);
				hr = gfx_context->SetMaterialIndex(i);

				hr = gfx_context->SetCullMode(vs_system::ElmContext::CULL_CCW);
				hr = gfx_context->SetMaterialIndex(i);

		}

	return VS_OK;
}