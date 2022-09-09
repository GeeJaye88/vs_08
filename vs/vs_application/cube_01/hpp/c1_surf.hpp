////////////////////////////////////////////////////////////////////////////////

// ---------- c1_surf.hpp ----------
/*!
\file c1_surf.hpp
\brief Implementation of the Model surface methods
\author Gareth Edwards
*/


INT Model::SetupSurfKArray()
{

	// ---- allocate array ?
		for (int i = 0; i < pi_model->number_of_surf_elem; i++)
		{
			if ( pi_model->surf_kandinsky_array[i] == NULL )
			{
				pi_model->surf_kandinsky_array[i] = new FLOAT[4096];
			}
		}

	return VS_OK;
}


INT Model::SetupSurfKData(UINT index)
{

	// ---- created ?
		if ( pi_model->surf_kandinsky_array[index] == NULL )return VS_ERROR;

		// ---- init array
		switch (index)
		{
			default:
			case 0:
				{
					using namespace vs_system;
					FLOAT kube[88] = 
					{
						(FLOAT)ObjConfig::KANDINSKY_3D,
						(FLOAT)ObjConfig::KANDINSKY_GEOTYPE_FACE,
 						0, // # colours
						6, // # elements
					//  #  -  C  -  X   y   z  -  X   y   z  -  X   y   z  -  X   y   z
						4,    0,   -1, -1, -1,   -1,  1, -1,    1,  1, -1,    1, -1, -1,
						4,    0,    1, -1, -1,    1,  1, -1,    1,  1,  1,    1, -1,  1,
						4,    0,    1, -1,  1,    1,  1,  1,   -1,  1,  1,   -1, -1,  1,
						4,    0,   -1, -1,  1,   -1,  1,  1,   -1,  1, -1,   -1, -1, -1,
						4,    0,   -1,  1, -1,   -1,  1,  1,    1,  1,  1,    1,  1, -1,
						4,    0,    1, -1,  1,    1, -1, -1,   -1, -1, -1,   -1, -1,  1
					};
					memcpy(pi_model->surf_kandinsky_array[index], kube,  sizeof(kube));
					float fred = 2;
				}
				break;
			case 1:

				break;
		}

	return VS_OK;
}


INT Model::SetupSurfDisplay()
{

	// ---- local
		vs_system::VsMatrixStack *vms = pi_model->matrix_stack;


	// ---- push
		vms->Push();


	// ---- matrix stuff
		VsMatrix  *m = &vms->stack[vms->index];
		vms->Rotate( 0, 'Y');
		vms->Translate( 0, 0, 0);


	// ---- set world matrix
		for (int i = 0; i < pi_model->number_of_surf_elem; i++)
		{
			vs_system::ElmContext *elm_context = NULL;
			INT hr = pi_model->gfx->GetElementContext(pi_model->handle_surf_elem[i], &elm_context);
			elm_context->SetUseMatrix(TRUE);
			elm_context->SetMatrix(m);
			elm_context->SetViewMatrix(pi_model->gfx_view_matrix_index);
			elm_context->SetVisible(i == 0 ? TRUE : FALSE);//pi_model->show_surf);
		}


	// ---- pop
		vms->Pop();

	return VS_OK;
}


INT Model::SetupSurfObjects()
{

	// ---- create display list
		vs_system::GfxInterface *gfx = pi_model->gfx;


	// ---- create display list ?
		if ( pi_model->handle_dspl == NULL )
		{
			pi_model->handle_dspl = gfx->AddDisplayList("Scene", 1, 1);
		}


	// ---- for each
		for (int i = 0; i < pi_model->number_of_surf_elem; i ++)
		{

			// ---- create surf ?
				if ( pi_model->handle_surf_elem[i] == NULL )
				{
					pi_model->handle_surf_elem[i] = gfx->AddElement(
							pi_model->handle_dspl,
							"",
							1,
							1,
							pi_model->obj_config->Callback_Kandinsky_Configure
						);
				}


			// ---- context
				vs_system::ElmContext *gfx_context = NULL;
				INT hr = gfx->GetElementContext(pi_model->handle_surf_elem[i], &gfx_context);


			// ---- force geometry setup
				gfx_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
				gfx_context->SetConfigured(FALSE);


			// ---- init kandinsky parameters
				FLOAT *param = gfx_context->GetFloatParameter();
				{
					using namespace vs_system;
					param[ObjConfig::KANDINSKY_TYPE]   = 1;
					param[ObjConfig::KANDINSKY_AP_REF] = 100;
					param[ObjConfig::KANDINSKY_RED]    = 1;
					param[ObjConfig::KANDINSKY_GREEN]  = 1;
					param[ObjConfig::KANDINSKY_BLUE]   = 1;
					param[ObjConfig::KANDINSKY_XOFF]   = 0;
					param[ObjConfig::KANDINSKY_YOFF]   = 0;
					param[ObjConfig::KANDINSKY_ZOFF]   = 0;
					param[ObjConfig::KANDINSKY_XSCALE] = 20;
					param[ObjConfig::KANDINSKY_YSCALE] = 20;
					param[ObjConfig::KANDINSKY_ZSCALE] = 20;
				}

			// ---- kandinsky
				INT result = SetupSurfKData((UINT)i);
				LONG *l = gfx_context->GetLongParameter();
				*(l + 0) = (LONG)pi_model->surf_kandinsky_array[i];

				FLOAT *fred = (FLOAT *)*(l + 0);

			// ---- lighting and materials
				hr = gfx_context->SetLit(param[vs_system::ObjConfig::KANDINSKY_TYPE] == 0 ? FALSE : TRUE);
				hr = gfx_context->SetMaterialIndex(i);
				hr = gfx_context->SetCullMode(vs_system::ElmContext::CULL_CW);


		}

	return VS_OK;
}