////////////////////////////////////////////////////////////////////////////////

// ---------- s3_obj_loi_spot.hpp ----------
/*!
\file s3_obj_loi_spot.hpp
\brief 
\author Gareth Edwards
*/


// ---------- SetupLoiObject ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiSpot_Setup(INT gfx_object_index, Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;


	// ---- create display list
		vs_system::GfxInterface *gfx = pimp->gfx;


	// ---- elem
		for (int i = 0; i < PI_Surface_03::MAX_EXP_LOI_; i++)
		{

			// ---- create ? - or reconfig
				if ( pimp->runtime )
				{
					pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_SPOT_LIST_IDX][i] = gfx->AddElement(
							pimp->scene_dspl_list_handle,
							"",
							1,
							1,
							pimp->obj_config->Callback_Sphere_Configure
						);
				}
				else
				{
					pimp->gfx->SetElementConfigureBitmask(
							pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_SPOT_LIST_IDX][i],
							VS_ELM_SETUP_GEOMETRY
						);
				}


			// ---- context
				vs_system::ElmContext *gfx_context = NULL;
				INT hr = gfx->GetElementContext(
						pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_SPOT_LIST_IDX][i],
						&gfx_context
					);


			// ---- context
				hr = gfx_context->SetViewMatrix(1);
				hr = gfx_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NULL);
				hr = gfx_context->SetPick3D(FALSE);
				hr = gfx_context->SetFog(FALSE);
				hr = gfx_context->SetLit(TRUE);
				hr = gfx_context->SetMaterialIndex(5);
				hr = gfx_context->SetTextureMirror(FALSE);
				hr = gfx_context->SetCullMode(vs_system::ElmContext::CULL_NONE);


			// ---- get target loi
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
					p[ObjConfig::SPHERE_RADIUS]   =  pimp->_configuration.acquire_dimensions.depth * 0.0125f;
					p[ObjConfig::SPHERE_LAT_MIN]  = -90.0f;
					p[ObjConfig::SPHERE_LAT_MAX]  =  90.0f;
				}

				FLOAT *param = gfx_context->GetFloatParameter();
				memcpy(param, p, sizeof(FLOAT) * VS_MAX_ELM_FLOAT_PARAMETER);
				gfx_context->SetFloatParameter(sizeof(param), param);

		}

	return VS_OK;
}


// ---------- ObjectLoiSpot_Display ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiSpot_Display(
		INT gfx_object_index,
		Surface_03 *p_s_03,
		BOOL visible
	)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;
		UINT target_index = p_s_03->GetTargetLoiIndex();
		if (target_index == 999) return false;


	// ---- elem
		for (int i = 0; i < PI_Surface_03::MAX_EXP_LOI_; i++)
		{

			// ---- context
				vs_system::ElmContext *elm_context = NULL;
				INT hr = pimp->gfx->GetElementContext(
						pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_SPOT_LIST_IDX][i],
						&elm_context
					);

			// ---- using emissive materials
				elm_context->SetMaterialIndex( i == target_index ? 6 : 7);

			// ---- visible ?
				elm_context->SetVisible(visible);
				if ( visible )
				{

					// ---- push
						pimp->matrix_stack->Push();
						VsMatrix  *m = &pimp->matrix_stack->stack[pimp->matrix_stack->index];

					// ---- Y offset - only LOI sphere gets moved by target height!
						FLOAT y_off = p_s_03->GetTargetLoiLocationHeight(i);

					// ---- transform 
						VsVector3 *v = p_s_03->GetTargetLoi(i);
						pimp->matrix_stack->Translate(v->x, y_off, v->z);
						elm_context->SetUseMatrix(TRUE);
						elm_context->SetMatrix(m);

					// ---- pop
						pimp->matrix_stack->Pop();

				}
		}

	return VS_OK;
}


// ---------- ObjectLoiSpot_HideAll ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiSpot_HideAll(Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;

	// ---- hide all
		for (int i = 0; i < PI_Surface_03::MAX_EXP_LOI_; i++)
		{
			// ---- context
				vs_system::ElmContext *elm_context = NULL;
				INT result = pimp->gfx->GetElementContext(
					pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_SPOT_LIST_IDX][i],
					&elm_context
				);

			// ---- visible ?
				elm_context->SetVisible(FALSE);

		}

	return VS_OK;
}


// ---------- ObjectLoiSpot_Reset ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiSpot_Reset(Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;

	// ---- reset all Y
		for (int i = 0; i < PI_Surface_03::MAX_EXP_LOI_; i++)
		{
			pimp->_configuration.target_loi[i].y = 0;

		}

	return VS_OK;
}


// ---------- ObjectLoiSpot_Update ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiSpot_Update(Surface_03 *p_s_03)
{
	return VS_OK;
}
