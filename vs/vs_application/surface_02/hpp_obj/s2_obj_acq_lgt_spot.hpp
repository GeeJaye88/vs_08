////////////////////////////////////////////////////////////////////////////////

// ---------- s2_obj_acq_lgt_spot.hpp ----------
/*!
\file s2_obj_acq_lgt_spot.hpp
\brief 
\author Gareth Edwards
*/

// ---------- ObjectAcqSpot_Setup ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectAcqSpot_Setup(INT index, Surface_02 *p_s_02)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;


	// ---- create or (re)config
		if ( pimp->runtime )
		{
			pimp->gfx_elem_handle[PI_Surface_02::ELM_ACQ_LGT_SPOT_LIST_IDX][0] =
				pimp->gfx->AddElement(
					pimp->scene_dspl_list_handle,
					"Rig",
					1,
					1,
					&pimp->obj_config->Callback_S3_AcqLgtSpot_Configure
				);
		}
		else
		{
			pimp->gfx->SetElementConfigureBitmask(
					pimp->gfx_elem_handle[PI_Surface_02::ELM_ACQ_LGT_SPOT_LIST_IDX][0],
					VS_ELM_SETUP_GEOMETRY
				);
		}


	// ---- get graphics context
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
					pimp->gfx_elem_handle[PI_Surface_02::ELM_ACQ_LGT_SPOT_LIST_IDX][0],
					&elm_context
				);

	// ---- force geometry setup
		hr = elm_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
		hr = elm_context->SetConfigured(FALSE);


	// ---- init func parameters
		FLOAT *param = elm_context->GetFloatParameter();


	// ---- hijack sphere
		param[vs_system::ObjConfig::SPHERE_CEL_COLS] =  20;
		param[vs_system::ObjConfig::SPHERE_CEL_ROWS] =  10;
		param[vs_system::ObjConfig::SPHERE_TYPE]     =   1;
		param[vs_system::ObjConfig::SPHERE_AP_REF]   =   0;
		param[vs_system::ObjConfig::SPHERE_RED]      =   1.0f;
		param[vs_system::ObjConfig::SPHERE_GREEN]    =   1.0f;
		param[vs_system::ObjConfig::SPHERE_BLUE]     =   1.0f;
		param[vs_system::ObjConfig::SPHERE_RADIUS]   =  pimp->_configuration.acquire_dimensions.depth * 0.025f;


	// ---- hijack box
		param[vs_system::ObjConfig::BOX_FACE_WIDTH]  = pimp->_configuration.acquire_dimensions.width;
		param[vs_system::ObjConfig::BOX_FACE_HEIGHT] = pimp->_configuration.acquire_dimensions.height; // hgt not used
		param[vs_system::ObjConfig::BOX_FACE_DEPTH]  = pimp->_configuration.acquire_dimensions.depth;

		FLOAT func_axis_off = pimp->_configuration.acquire_dimensions.depth/2;
		param[vs_system::ObjConfig::BOX_FACE_X] =  func_axis_off;
		param[vs_system::ObjConfig::BOX_FACE_Y] =  0.0f;
		param[vs_system::ObjConfig::BOX_FACE_Z] = -func_axis_off;

		param[vs_system::ObjConfig::BOX_FLAG_4] = 0;
		param[vs_system::ObjConfig::BOX_FLAG_5] = 0;
		param[vs_system::ObjConfig::BOX_FLAG_6] = 0;
		param[vs_system::ObjConfig::BOX_FLAG_7] = 0;


	// ---- lighting, materials, etc...
		hr = elm_context->SetLit(FALSE);
		hr = elm_context->SetMaterialIndex(5);
		hr = elm_context->SetCullMode(vs_system::ElmContext::CULL_NONE);


	// ---- setup/update common parameters
		INT result = ObjectAcqSpot_UpdateParam(index, p_s_02);

	return VS_OK;
}


// ---------- ObjectAcqSpot_Display ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectAcqSpot_Display(INT index, Surface_02 *p_s_02, BOOL visible)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_ACQ_LGT_SPOT_LIST_IDX][0],
				&elm_context
			);

	// ---- visible ?
		elm_context->SetVisible(visible);
		if ( !visible ) return VS_OK;

	// ---- push
		pimp->matrix_stack->Push();
		VsMatrix  *m = &pimp->matrix_stack->stack[pimp->matrix_stack->index];

		// ---- NO parent transform
			vs_system::VsMatrixOp mop;
			VsMatrix i;
			mop.Identity(&i);
			pimp->matrix_stack->stack[pimp->matrix_stack->index] = i;

		// ---- transform into quadrent bounded by +x & -z axis
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


// ---------- ObjectAcqSpot_Update ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note invoked at setup and when surface mode selected or param is updated

*/
INT Surface_02::ObjectAcqSpot_Update(INT index, Surface_02 *p_s_02)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;


	// ---- elem
		pimp->gfx->SetElementConfigureBitmask(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_ACQ_LGT_SPOT_LIST_IDX][0],
				VS_ELM_SETUP_ALL
			);


	// ---- setup/update common parameters
		INT result = ObjectAcqSpot_UpdateParam(index, p_s_02);
	
	return VS_OK;
}


// ---------- ObjectAcqSpot_UpdateParam ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectAcqSpot_UpdateParam(INT index, Surface_02 *p_s_02)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;


	// ---- elem
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_ACQ_LGT_SPOT_LIST_IDX][0],
				&elm_context
			);


	// ---- azimuth angle offset of the active light ?
		UINT lfi = p_s_02->GetAcquireLightIndex();
		FLOAT aao = (FLOAT)lfi * 90.0f;


	// ---- param
		FLOAT *param = elm_context->GetFloatParameter();
		param[vs_system::ObjConfig::BOX_FLAG_4] = pimp->_configuration.acquire_light_param.intensity;
		param[vs_system::ObjConfig::BOX_FLAG_5] = pimp->_configuration.acquire_light_param.distance;
		param[vs_system::ObjConfig::BOX_FLAG_6] = pimp->_configuration.acquire_light_param.elevation;
		param[vs_system::ObjConfig::BOX_FLAG_7] = pimp->_configuration.acquire_light_param.azimuth + aao;

	return VS_OK;
}

