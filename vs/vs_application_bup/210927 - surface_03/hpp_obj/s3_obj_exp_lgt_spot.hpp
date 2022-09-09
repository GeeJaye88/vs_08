////////////////////////////////////////////////////////////////////////////////

// ---------- s3_obj_exp_lgt_spot.hpp ----------
/*!
\file s3_obj_exp_lgt_spot.h
\brief 
\author Gareth Edwards
*/


// ---------- ObjectExpSpot_Setup ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectExpSpot_Setup(INT index, Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;


		// ---- create or (re)config sphere
			if (pimp->runtime )
			{
				pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LGT_SPOT_LIST_IDX][index] =
					pimp->gfx->AddElement(
						pimp->scene_dspl_list_handle,
						"Sphere",
						1,
						1,
						&pimp->obj_config->Callback_Sphere_Configure
					);
			}
			else
			{
				pimp->gfx->SetElementConfigureBitmask(
						pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LGT_SPOT_LIST_IDX][index],
						VS_ELM_SETUP_GEOMETRY
					);
			}


		// ---- set graphics context
			vs_system::ElmContext *gfx_context = NULL;
			INT hr = pimp->gfx->GetElementContext(
						pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LGT_SPOT_LIST_IDX][index],
						&gfx_context
					);
			hr = gfx_context->SetViewMatrix(1);
			hr = gfx_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NULL);
			hr = gfx_context->SetPick3D(FALSE);
			hr = gfx_context->SetFog(FALSE);
			hr = gfx_context->SetLit(TRUE);
			hr = gfx_context->SetMaterialIndex(1);
			hr = gfx_context->SetTextureMirror(TRUE);

			vs_system::VsMatrixOp mop;
			VsMatrix m;
			mop.Identity(&m);
			hr = gfx_context->SetMatrix(&m);
			hr = gfx_context->SetUseMatrix(TRUE);


		// ---- set parameters
			FLOAT p[VS_MAX_ELM_FLOAT_PARAMETER];
			{
				using namespace vs_system;
				p[ObjConfig::SPHERE_CEL_COLS] =  20;
				p[ObjConfig::SPHERE_CEL_ROWS] =  10;
				p[ObjConfig::SPHERE_TYPE]     =   1;
				p[ObjConfig::SPHERE_AP_REF]   =   0;
				p[ObjConfig::SPHERE_RED]      =   1.0f;
				p[ObjConfig::SPHERE_GREEN]    =   1.0f;
				p[ObjConfig::SPHERE_BLUE]     =   1.0f;
				p[ObjConfig::SPHERE_RADIUS]   =  pimp->_configuration.acquire_dimensions.depth * 0.025f;
				p[ObjConfig::SPHERE_LAT_MIN]  = -90.0f;
				p[ObjConfig::SPHERE_LAT_MAX]  =  90.0f;
			}

			FLOAT *param = gfx_context->GetFloatParameter();
			memcpy(param, p, sizeof(FLOAT) * VS_MAX_ELM_FLOAT_PARAMETER);
			gfx_context->SetFloatParameter(sizeof(param), param);

	return VS_OK;
}



// ---------- ObjectExpSpot_Display ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectExpSpot_Display(INT index, Surface_03 *p_s_03, BOOL visible)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LGT_SPOT_LIST_IDX][index],
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
				pimp->_configuration.explore_light[index].position.x,
				pimp->_configuration.explore_light[index].position.y,
				pimp->_configuration.explore_light[index].position.z
			);


	// ---- set config material
		INT result = SetupExploreMaterials(index, p_s_03);


	// ---- set gfx material
		UINT gfx_index = index + 1;
		elm_context->SetMaterialIndex(gfx_index);


	// ---- transform
		elm_context->SetUseMatrix(TRUE);
		elm_context->SetMatrix(m);
		elm_context->SetViewMatrix(1);


	// ---- pop
		pimp->matrix_stack->Pop();

	return VS_OK;
}
