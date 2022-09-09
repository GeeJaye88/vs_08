////////////////////////////////////////////////////////////////////////////////

// ---------- s2_obj_exp_lgt_axis.hpp ----------
/*!
\file s2_obj_exp_lgt_axis.h
\brief
\author Gareth Edwards
*/


// ---------- ObjectExpLgtAxis_Setup ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectExpLgtAxis_Setup(INT index, Surface_02 *p_s_02)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;


	// ---- create or (re)config
		if ( pimp->runtime )
		{
			pimp->gfx_elem_handle[PI_Surface_02::ELM_EXP_LGT_RIG_LIST_IDX][index] =
				pimp->gfx->AddElement(
					pimp->scene_dspl_list_handle,
					"ExpLgtRig",
					1,
					1,
					&pimp->obj_config->Callback_ExpLgtAxis_Configure
				);
		}
		else
		{
			pimp->gfx->SetElementConfigureBitmask(
					pimp->gfx_elem_handle[PI_Surface_02::ELM_EXP_LGT_RIG_LIST_IDX][index],
					VS_ELM_SETUP_GEOMETRY
				);
		}


	// ---- set graphics context
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
					pimp->gfx_elem_handle[PI_Surface_02::ELM_EXP_LGT_RIG_LIST_IDX][index],
					&elm_context
				);


	// ---- force geometry setup
		hr = elm_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
		hr = elm_context->SetConfigured(FALSE);

		hr = elm_context->SetViewMatrix(1);
		hr = elm_context->SetLit(FALSE);
		hr = elm_context->SetMaterialIndex(5);
		hr = elm_context->SetCullMode(vs_system::ElmContext::CULL_NONE);


	// ---- init func parameters
		FLOAT *param = elm_context->GetFloatParameter();
		param[vs_system::ObjConfig::BOX_FACE_MODE]    = 0;
		param[vs_system::ObjConfig::BOX_FACE_TYPE]    = 1;
		param[vs_system::ObjConfig::BOX_FACE_AP_REF]  = 0;
		param[vs_system::ObjConfig::BOX_FACE_RED]     = 0.5f;
		param[vs_system::ObjConfig::BOX_FACE_GREEN]   = 0.5f;
		param[vs_system::ObjConfig::BOX_FACE_BLUE]    = 0.5f;
		param[vs_system::ObjConfig::BOX_FACE_SCALE]   = 1.0f;
		param[vs_system::ObjConfig::BOX_FACE_EXPLODE] = 0.0f;

		param[vs_system::ObjConfig::BOX_FACE_WIDTH]   = pimp->_configuration.acquire_dimensions.width;
		param[vs_system::ObjConfig::BOX_FACE_HEIGHT]  = pimp->_configuration.acquire_dimensions.height; // hgt  not used
		param[vs_system::ObjConfig::BOX_FACE_DEPTH]   = pimp->_configuration.acquire_dimensions.depth;

		FLOAT func_axis_off = pimp->_configuration.acquire_dimensions.depth/2;
		param[vs_system::ObjConfig::BOX_FACE_X] =  0;
		param[vs_system::ObjConfig::BOX_FACE_Y] =  0;
		param[vs_system::ObjConfig::BOX_FACE_Z] =  0;

		param[vs_system::ObjConfig::BOX_FLAG_1] = 1;  // axis flag
		param[vs_system::ObjConfig::BOX_FLAG_2] = 1;  // light flag

		param[vs_system::ObjConfig::BOX_FLAG_3] = (FLOAT)index;
		param[vs_system::ObjConfig::BOX_FLAG_4] = 0;  // x
		param[vs_system::ObjConfig::BOX_FLAG_5] = 0;  // y
		param[vs_system::ObjConfig::BOX_FLAG_6] = 0;  // z


	// ---- setup/update parameters
		INT result = ObjectExpLgtAxis_Update(index, p_s_02);

	return VS_OK;
}



// ---------- ObjectExpLgtAxis_Display ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectExpLgtAxis_Display(INT index, Surface_02 *p_s_02, BOOL visible)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_EXP_LGT_RIG_LIST_IDX][index],
				&elm_context
			);

	// ---- visible ?
		elm_context->SetVisible(visible);
		if ( !visible ) return VS_OK;


	// ---- push
		pimp->matrix_stack->Push();
		VsMatrix  *m = &pimp->matrix_stack->stack[pimp->matrix_stack->index];


	// ---- NO transform - already located relative to global origin & lights 
		vs_system::VsMatrixOp mop;
		VsMatrix i;
		mop.Identity(&i);
		mop.Translate(&i, 0, 0, 0);
		hr = elm_context->SetMatrix(&i);
		elm_context->SetUseMatrix(TRUE);
		elm_context->SetViewMatrix(1);


	// ---- set config material
		INT result = SetupExploreMaterials(index, p_s_02);


	// ---- set gfx material per lights
		UINT gfx_index = index + 1;
		elm_context->SetMaterialIndex(gfx_index);


	// ---- pop
		pimp->matrix_stack->Pop();

	return VS_OK;
}


// ---------- ObjectExpLgtAxis_UpdateAll ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note invoked when light locations are updated

*/
INT Surface_02::ObjectExpLgtAxis_UpdateAll(Surface_02 *p_s_02)
{

	// ---- setup lighting colour
		BOOL colour_enabled = p_s_02->GetAcquireLightColouredEnabled();

		INT result = p_s_02->SetupGraphicsLighting(
							colour_enabled ? 
								SceneSetupMode::DEFAULT :
									SceneSetupMode::ACQUIRE
						);

	// ---- setup/update parameters
		for (UINT index = 0; index < 4; index++)
		{
			result = ObjectExpLgtAxis_Update(index, p_s_02);
		}

	return VS_OK;
}


// ---------- ObjectExpLgtAxis_Update ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note invoked at setup and when surface mode selected or param is updated

*/
INT Surface_02::ObjectExpLgtAxis_Update(INT index, Surface_02 *p_s_02)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;

	// ---- elem
		pimp->gfx->SetElementConfigureBitmask(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_EXP_LGT_RIG_LIST_IDX][index],
				VS_ELM_SETUP_ALL
			);

	// ---- setup/update parameters
		INT result = ObjectExpLgtAxis_UpdateParam(index, p_s_02);


	return VS_OK;
}


// ---------- ObjectExpLgtAxis_UpdateParam ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectExpLgtAxis_UpdateParam(INT index, Surface_02 *p_s_02)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;


	// ---- elem
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_EXP_LGT_RIG_LIST_IDX][index],
				&elm_context
			);


	// ---- get & boost colour
		FLOAT scalar = 2;
		FLOAT red = pimp->_configuration.acquire_light[index].diffuse.red;
		FLOAT green = pimp->_configuration.acquire_light[index].diffuse.green;
		FLOAT blue = pimp->_configuration.acquire_light[index].diffuse.blue;
		red = red * scalar > 1 ? 1 : red * scalar;
		green = green * scalar > 1 ? 1 : green * scalar;
		blue = blue * scalar > 1 ? 1 : blue * scalar;


	// ---- param
		FLOAT *param = elm_context->GetFloatParameter();

		param[vs_system::ObjConfig::BOX_FACE_RED]   = red;
		param[vs_system::ObjConfig::BOX_FACE_GREEN] = green;
		param[vs_system::ObjConfig::BOX_FACE_BLUE]  = blue;

		param[vs_system::ObjConfig::BOX_FLAG_3] = (FLOAT)index;
		param[vs_system::ObjConfig::BOX_FLAG_4] = pimp->_configuration.acquire_light[index].position.x;
		param[vs_system::ObjConfig::BOX_FLAG_5] = pimp->_configuration.acquire_light[index].position.y;
		param[vs_system::ObjConfig::BOX_FLAG_6] = pimp->_configuration.acquire_light[index].position.z;


	// ---- force geometry setup
		hr = elm_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
		hr = elm_context->SetConfigured(FALSE);


		//CHAR msg[128];
		//sprintf(msg, " +-> %d %.3f \n", index, param[vs_system::ObjConfig::BOX_FACE_RED]);
		//OutputDebugString(msg);


	return VS_OK;
}

