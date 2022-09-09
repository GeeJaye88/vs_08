////////////////////////////////////////////////////////////////////////////////

// ---------- s3_obj_loi_axis.hpp ----------
/*!
\file s3_obj_loi_axis.hpp
\brief 
\author Gareth Edwards
*/


// ---------- ObjectLoiAxis_Setup ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiAxis_Setup(INT gfx_object_index, Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;


	// ---- create display list
		vs_system::GfxInterface *gfx = pimp->gfx;


	#define LOI_RIGG


	// ---- elem
		for (int i = 0; i < PI_Surface_03::MAX_EXP_LOI_; i++)
		{

			// ---- create ? - or reconfig
				if ( pimp->runtime )
				{
					pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_AXIS_LIST_IDX][i] = gfx->AddElement(
							pimp->scene_dspl_list_handle,
							"",
							1,
							1,
							#if defined(LOI_RIGG)
							pimp->obj_config->Callback_LoiAxis_Configure
							#else
							pimp->obj_config->Callback_Sphere_Configure
							#endif
						);
				}
				else
				{
					pimp->gfx->SetElementConfigureBitmask(
							pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_AXIS_LIST_IDX][i],
							VS_ELM_SETUP_GEOMETRY
						);
				}


			// ---- context
				vs_system::ElmContext *elm_context = NULL;
				INT hr = gfx->GetElementContext(
						pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_AXIS_LIST_IDX][i],
						&elm_context
					);


			// ---- context
				hr = elm_context->SetViewMatrix(1);
				hr = elm_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NULL);
				hr = elm_context->SetPick3D(FALSE);
				hr = elm_context->SetFog(FALSE);
				hr = elm_context->SetLit(FALSE);
				hr = elm_context->SetMaterialIndex(6);
				hr = elm_context->SetCullMode(vs_system::ElmContext::CULL_NONE);


			// ---- get target loi
				hr = elm_context->SetUseMatrix(TRUE);

			#if defined(LOI_RIGG)

				FLOAT *param = elm_context->GetFloatParameter();
				param[vs_system::ObjConfig::BOX_FACE_MODE]    = 0;
				param[vs_system::ObjConfig::BOX_FACE_TYPE]    = 1;
				param[vs_system::ObjConfig::BOX_FACE_AP_REF]  = 0;
				param[vs_system::ObjConfig::BOX_FACE_RED]     = 0.5f;
				param[vs_system::ObjConfig::BOX_FACE_GREEN]   = 0.5f;
				param[vs_system::ObjConfig::BOX_FACE_BLUE]    = 0.5f;
				param[vs_system::ObjConfig::BOX_FACE_SCALE]   = 1.15f;
				param[vs_system::ObjConfig::BOX_FACE_EXPLODE] = 0.0f;

				param[vs_system::ObjConfig::BOX_FACE_WIDTH]   = pimp->_configuration.acquire_dimensions.width;
				param[vs_system::ObjConfig::BOX_FACE_HEIGHT]  = pimp->_configuration.acquire_dimensions.height;
				param[vs_system::ObjConfig::BOX_FACE_DEPTH]   = pimp->_configuration.acquire_dimensions.depth;

				param[vs_system::ObjConfig::BOX_FACE_X] = 0;
				param[vs_system::ObjConfig::BOX_FACE_Y] = 0;
				param[vs_system::ObjConfig::BOX_FACE_Z] = 0;

				param[vs_system::ObjConfig::BOX_FLAG_1] =   1;  // axis flag
				param[vs_system::ObjConfig::BOX_FLAG_2] =  10;  // y_top
				param[vs_system::ObjConfig::BOX_FLAG_3] = -10;  // y_bot

			#else

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

				FLOAT *param = elm_context->GetFloatParameter();
				memcpy(param, p, sizeof(FLOAT) * VS_MAX_ELM_FLOAT_PARAMETER);
				elm_context->SetFloatParameter(sizeof(param), param);
			#endif
		}

	return VS_OK;
}


// ---------- ObjectLoiAxis_Display ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiAxis_Display(INT gfx_object_index, Surface_03 *p_s_03, BOOL visible)
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
						pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_AXIS_LIST_IDX][i],
						&elm_context
					);

			// ---- visible ?
				BOOL elm_visible = (i == target_index ? TRUE : FALSE) && visible;
				elm_context->SetVisible(elm_visible);
				if ( elm_visible )
				{

					// ---- push
						pimp->matrix_stack->Push();
						VsMatrix  *m = &pimp->matrix_stack->stack[pimp->matrix_stack->index];

					// ---- y object offset to "see" just above surface
						FLOAT yoff = 0.1f;

					// ---- transform
						VsVector3 *v = p_s_03->GetTargetLoi(i);
						FLOAT angle = p_s_03->GetTargetLoiRotation(i);
						FLOAT ymax = p_s_03->GetAcquireHeightFieldMax();

						pimp->matrix_stack->Translate(v->x, v->y+yoff, v->z - 0.05f);
						pimp->matrix_stack->Rotate(angle, 'Y');
						elm_context->SetUseMatrix(TRUE);
						elm_context->SetMatrix(m);

					// ---- pop
						pimp->matrix_stack->Pop();

				}
		}

	return VS_OK;
}


// ---------- ObjectLoiAxis_HideAll ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiAxis_HideAll(Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;

	// ---- hide all
		for (int i = 0; i < PI_Surface_03::MAX_EXP_LOI_; i++)
		{
			// ---- context
				vs_system::ElmContext *elm_context = NULL;
				INT result = pimp->gfx->GetElementContext(
					pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_AXIS_LIST_IDX][i],
					&elm_context
				);

			// ---- visible ?
				elm_context->SetVisible(FALSE);

		}

	return VS_OK;
}


// ---------- ObjectLoiAxis_Reset ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiAxis_Reset(Surface_03 *p_s_03)
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


// ---------- ObjectLoiAxis_Update ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_03::ObjectLoiAxis_Update(Surface_03 *p_s_03)
{

	// ---- local
		PI_Surface_03 *pimp = p_s_03->pi_surface_03;
		UINT target_index = p_s_03->GetTargetLoiIndex();
		if (target_index == 999) return false;


	// ---- reconfigure
		INT hr = pimp->gfx->SetElementConfigureBitmask(
				pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_AXIS_LIST_IDX][target_index],
				VS_ELM_SETUP_GEOMETRY
			);


	// ---- context
		vs_system::ElmContext *elm_context = NULL;
		hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_03::ELM_EXP_LOI_AXIS_LIST_IDX][target_index],
				&elm_context
			);


	// ---- vb param
		FLOAT y_loc = pimp->_configuration.target_loi[target_index].y;

		FLOAT y_sca = p_s_03->GetTargetLoiHeightScalar();
		FLOAT y_min = p_s_03->GetAcquireHeightFieldMin() * y_sca;
		FLOAT y_max = p_s_03->GetAcquireHeightFieldMax() * y_sca;

		FLOAT y_top = y_max - y_loc;
		FLOAT y_bot = y_top + (y_loc - y_min);

		FLOAT *param = elm_context->GetFloatParameter();
		param[vs_system::ObjConfig::BOX_FLAG_2] =  y_top; // y_top 
		param[vs_system::ObjConfig::BOX_FLAG_3] = -y_bot; // y_bot
		param[vs_system::ObjConfig::BOX_FLAG_4] = -y_loc; // y_loc

	return VS_OK;
}
