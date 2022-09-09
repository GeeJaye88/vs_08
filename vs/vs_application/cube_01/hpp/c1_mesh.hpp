////////////////////////////////////////////////////////////////////////////////

// ---------- c1_mesh.hpp ----------
/*!
\file c1_mesh.hpp
\brief Implementation of the Model mesh methods
\author Gareth Edwards
*/


INT Model::SetupMeshYArray()
{

	// ---- local
		INT result = VS_OK;


	// ---- allocate mesh array ?
		if ( pi_model->mesh_surface_y_array == NULL )
		{
			UINT dimension = 64 + 1;
			pi_model->mesh_surface_y_stride = dimension;
			pi_model->mesh_surface_y_array = new FLOAT[dimension * dimension];
		}


	// ---- mesh
		UINT   mesh_rows = pi_model->mesh_surface_y_stride - 1;


	// ---- y height field array ( stride = 1 & # width = mesh rows + 1 for subdivsion ) 
		UINT   y_array_pitch      = mesh_rows + 1;
		UINT   y_array_width      = y_array_pitch;
		UINT   y_array_extent     = y_array_pitch * y_array_pitch;
		UINT   y_array_offset     = 0;

		FLOAT *y_array            = pi_model->mesh_surface_y_array;
		UINT   y_array_dimension  = pi_model->mesh_surface_y_stride;


	// ---- fractal function parameters
		INT     random_seed      =  2001;
		FLOAT   height_scalar    = (FLOAT)pi_model->scene_dimension / 2.0f * 1.0f;
		FLOAT   height_roughness =   1.0f;
		FLOAT   filter_scalar    =   0.5f;
		FLOAT   y_min            = -10.0f;
		FLOAT   y_max            =  10.0f;


	// ---- calc y fractal function
		vs_system::Fractal fractal;
		result = fractal.Fractal2D(
				y_array,
				mesh_rows,
				random_seed,
				height_scalar,
				height_roughness
			);


	// ---- calc y post process
		float filter_array[9] =
		{
			0.0625, 0.1250, 0.0625,
			0.1250, 0.2500, 0.1250,
			0.0625, 0.1250, 0.0625
		};
		result = vs_system::ObjConfig::Mesh_Filter(
				y_array,
				y_array_width,
				filter_array,
				9,
				filter_scalar
			);
		result = vs_system::ObjConfig::Mesh_YOffset(
				y_array,
				y_array_width,
				y_min
			);
		result = vs_system::ObjConfig::Mesh_YClip(
				y_array,
				y_array_width,
				y_min,
				y_max
			);

	return VS_OK;
}


INT Model::SetupMeshDisplay()
{

	// ---- local
		vs_system::VsMatrixStack *vms = pi_model->matrix_stack;


	// ---- push
		vms->Push();


	// ---- matrix stuff
		VsMatrix  *m = &vms->stack[vms->index];
		vms->Translate( 0, 0, 0);
		vms->Rotate( 0.0f, 'Y');


	// ---- world matrix
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pi_model->gfx->GetElementContext(pi_model->handle_mesh_elem, &elm_context);
		elm_context->SetUseMatrix(TRUE);
		elm_context->SetMatrix(m);
		elm_context->SetViewMatrix(pi_model->gfx_view_matrix_index);
		elm_context->SetVisible(pi_model->show_mesh);

	// ---- pop
		vms->Pop();

	return VS_OK;
}


INT Model::SetupMeshObjects()
{

	// ---- create display list
		vs_system::GfxInterface *gfx = pi_model->gfx;


	// ---- create display list ?
		if (pi_model->handle_dspl == NULL )
		{
			pi_model->handle_dspl = gfx->AddDisplayList("Scene", 1, 1);
		}


	// ---- create mesh ?
		if ( pi_model->handle_mesh_elem == NULL )
		{
			pi_model->handle_mesh_elem = gfx->AddElement(
					pi_model->handle_dspl,
					"",
					1,
					1,
					pi_model->obj_config->Callback_Mesh_Configure
				);
		}


	// ---- context
		vs_system::ElmContext *gfx_context = NULL;
		INT hr = gfx->GetElementContext(pi_model->handle_mesh_elem, &gfx_context);


	// ---- force geometry setup
		gfx_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
		gfx_context->SetConfigured(FALSE);


	// ---- init mesh parameters
		UINT  mesh_rows = pi_model->mesh_surface_y_stride - 1;
		FLOAT mesh_size = 50;
		FLOAT mesh_sby2 = mesh_size / 2;
		FLOAT *param = gfx_context->GetFloatParameter();
		param[vs_system::ObjConfig::MESH_TYPE]    =  0;
		param[vs_system::ObjConfig::MESH_AP_REF]  =  109;
		param[vs_system::ObjConfig::MESH_ROWS]    =  (FLOAT)mesh_rows;
		param[vs_system::ObjConfig::MESH_COLS]    =  (FLOAT)mesh_rows;
		param[vs_system::ObjConfig::MESH_ALIGN]   =  1;
		param[vs_system::ObjConfig::MESH_RED]     =  1;
		param[vs_system::ObjConfig::MESH_GREEN]   =  1;
		param[vs_system::ObjConfig::MESH_BLUE]    =  1;
		param[vs_system::ObjConfig::MESH_WIDTH]   =  mesh_size;
		param[vs_system::ObjConfig::MESH_DEPTH]   =  mesh_size;
		param[vs_system::ObjConfig::MESH_HEIGHT]  =  mesh_size;
		param[vs_system::ObjConfig::MESH_XOFF]    = -mesh_sby2;
		param[vs_system::ObjConfig::MESH_YOFF]    =  0;
		param[vs_system::ObjConfig::MESH_ZOFF]    =  mesh_sby2;


	// ---- mesh type
		param[vs_system::ObjConfig::MESH_PARAM_0] = vs_system::ObjConfig::MESH_VERSION_CAMERA;


	// ---- capture height field 
		LONG *l = gfx_context->GetLongParameter();
		*(l + 0) = (LONG)pi_model->mesh_surface_y_array;
		*(l + 1) = (LONG)pi_model->mesh_surface_y_stride;
		*(l + 2) = (LONG)pi_model->mesh_surface_y_scalar;


	// ---- lighting and materials
		hr = gfx_context->SetLit(TRUE);
		hr = gfx_context->SetCullMode(vs_system::ElmContext::CULL_NONE);
		hr = gfx_context->SetMaterialIndex(0);

	return VS_OK;
}