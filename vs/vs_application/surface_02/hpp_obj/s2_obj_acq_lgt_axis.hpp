////////////////////////////////////////////////////////////////////////////////

// ---------- s2_obj_acq_lgt_axis.hpp ----------
/*!
\file s2_obj_acq_lgt_axis.hpp
\brief 
\author Gareth Edwards
*/

// ---------- ObjectAcqAxis_Setup ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectAcqAxis_Setup(INT gfx_object_index, Surface_02 *p_s_02)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;

	// ---- for each rig
		for (UINT rig_index = 0; rig_index < 5; rig_index++)
		{

			// ---- create or (re)config
				if ( pimp->runtime )
				{
						pimp->gfx_elem_handle[PI_Surface_02::ELM_ACQ_LGT_AXIS_LIST_IDX][rig_index] =
							pimp->gfx->AddElement(
								pimp->scene_dspl_list_handle,
								"Rig",
								1,
								1,
								&pimp->obj_config->Callback_S2_AcqLgtAxis_Configure
							);
				}
				else
				{
					pimp->gfx->SetElementConfigureBitmask(
							pimp->gfx_elem_handle[PI_Surface_02::ELM_ACQ_LGT_AXIS_LIST_IDX][rig_index],
							VS_ELM_SETUP_GEOMETRY
						);
				}


			// ---- get graphics context
				vs_system::ElmContext *elm_context = NULL;
				INT hr = pimp->gfx->GetElementContext(
							pimp->gfx_elem_handle[PI_Surface_02::ELM_ACQ_LGT_AXIS_LIST_IDX][rig_index],
							&elm_context
						);


			// ---- force geometry setup
				hr = elm_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
				hr = elm_context->SetConfigured(FALSE);


			// ---- lighting, materials, etc...
				hr = elm_context->SetLit(FALSE);
				hr = elm_context->SetMaterialIndex(5);
				hr = elm_context->SetCullMode(vs_system::ElmContext::CULL_NONE);


			// ---- setup (& later update) parameters
				INT result = ObjectAcqAxis_UpdateParam(rig_index, p_s_02);

		}



	return VS_OK;
}


// ---------- ObjectAcqAxis_Display ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectAcqAxis_Display(INT gfx_object_index, Surface_02 *p_s_02, BOOL visible)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;


	// ---- for each rig
		for (UINT rig_index = 0; rig_index < 5; rig_index++)
		{

			// ---- context
				vs_system::ElmContext *elm_context = NULL;
				INT hr = pimp->gfx->GetElementContext(
						pimp->gfx_elem_handle[PI_Surface_02::ELM_ACQ_LGT_AXIS_LIST_IDX][rig_index],
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

		}

	return VS_OK;
}


// ---------- ObjectAcqAxis_Update ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note invoked at setup and when surface mode selected or param is updated

*/
INT Surface_02::ObjectAcqAxis_Update(INT gfx_object_index, Surface_02 *p_s_02)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;


	// ---- for each rig
		for (UINT rig_index = 0; rig_index < 5; rig_index++)
		{

			// ---- elem
				pimp->gfx->SetElementConfigureBitmask(
						pimp->gfx_elem_handle[PI_Surface_02::ELM_ACQ_LGT_AXIS_LIST_IDX][rig_index],
					VS_ELM_SETUP_ALL
					);


			// ---- setup/update common parameters
				INT result = ObjectAcqAxis_UpdateParam(rig_index, p_s_02);
		}

	return VS_OK;
}


// ---------- ObjectAcqAxis_UpdateParam ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note parameter data:

	MAX_MESH_AL_PARAMETERS = 36,
	MESH_AL_MODE   = 0,  // acquire or explore
	MESH_AL_ID     = 1,  // component
	MESH_AL_WIDTH  = 2,  // x dimension
	MESH_AL_HEIGHT = 3,  // y "
	MESH_AL_DEPTH  = 4,  // z "
	MESH_AL_XOFF   = 5,  // x offset
	MESH_AL_YOFF   = 6,  // y "
	MESH_AL_ZOFF   = 7,  // z "

	MESH_AL_MODE_ACQUIRE = 1,
	MESH_AL_MODE_EXPLORE = 2,

	MESH_AL_LIGHT_OFFSET = 8, // light data param array index
	MESH_AL_LIGHT_COUNT  = 4, // number of lights
	MESH_AL_LIGHT_SIZE   = 5, // parameters per light

*/
INT Surface_02::ObjectAcqAxis_UpdateParam(INT rig_index, Surface_02 *p_s_02)
{

	// ---- local
		PI_Surface_02 *pimp = p_s_02->pi_surface_02;


	// ---- elem
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_ACQ_LGT_AXIS_LIST_IDX][rig_index],
				&elm_context
			);


	// ---- required parameters
		using namespace vs_system;
		FLOAT *param = elm_context->GetFloatParameter();

	// ---- acquire (1:MESH_AL_MODE_ACQUIRE) or explore (2:MESH_AL_MODE_EXPLORE)
		param[ObjConfig::MESH_AL_MODE] = ObjConfig::MESH_AL_MODE_EXPLORE;

	// ---- rig component id
		//
		//  - 0 : compass + outer radii
		//  - 1 to 4 : lights L, B, R & F
		//
		param[ObjConfig::MESH_AL_ID] = (FLOAT)rig_index; 

	// ---- surface deimensions
		PI_Surface_02::Surface_02_Config::AcquireDimensions *ad= &pimp->_configuration.acquire_dimensions;
		param[ObjConfig::MESH_AL_WIDTH]  = ad->width;
		param[ObjConfig::MESH_AL_HEIGHT] = ad->height;
		param[ObjConfig::MESH_AL_DEPTH]  = ad->depth;

	// ---- offset rel to align with surface
		FLOAT func_axis_off = pimp->_configuration.acquire_dimensions.depth / 2;
		param[ObjConfig::MESH_AL_XOFF] = func_axis_off;
		param[ObjConfig::MESH_AL_YOFF] = 0.0f;
		param[ObjConfig::MESH_AL_ZOFF] = -func_axis_off;

		// use union to pack rgba into a float
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} diffuse;
		FLOAT red   = 0.5f;
		FLOAT green = 0.5f;
		FLOAT blue  = 0.5f;

		PI_Surface_02::Surface_02_Config::AcquireLightParam *alp = &pimp->_configuration.acquire_light_param;

	// ---- compass and radius ? - light data not required
		if ( rig_index == 0 )
		{
			// required
				diffuse.d = VS_D3DCOLOR_COLORVALUE(red, green, blue, 1.0f);
				param[ObjConfig::MESH_AL_LIGHT_OFFSET + 0] = diffuse.f;
			
			// required for outer radius
				param[ObjConfig::MESH_AL_LIGHT_OFFSET + 1] = alp->intensity;
				param[ObjConfig::MESH_AL_LIGHT_OFFSET + 2] = alp->distance;
				param[ObjConfig::MESH_AL_LIGHT_OFFSET + 3] = alp->elevation;

			// not required for outer radius
				param[ObjConfig::MESH_AL_LIGHT_OFFSET + 4] = 0;

		}
		else
		{

			switch ( rig_index )
			{
				case 1:  red = 0.0f; green = 1.0f; blue = 0.0f; break;
				case 2:  red = 1.0f; green = 0.5f; blue = 0.0f; break;
				case 3:  red = 1.0f; green = 0.0f; blue = 1.0f; break;
				case 4:  red = 0.0f; green = 0.5f; blue = 1.0f; break;
				default: red = 1.0f; green = 1.0f; blue = 1.0f; break;
			}
			diffuse.d = VS_D3DCOLOR_COLORVALUE(red, green, blue, 1.0f);

			param[ObjConfig::MESH_AL_LIGHT_OFFSET + 0] = diffuse.f;
			param[ObjConfig::MESH_AL_LIGHT_OFFSET + 1] = alp->intensity;
			param[ObjConfig::MESH_AL_LIGHT_OFFSET + 2] = alp->distance;
			param[ObjConfig::MESH_AL_LIGHT_OFFSET + 3] = alp->elevation;
			param[ObjConfig::MESH_AL_LIGHT_OFFSET + 4] = alp->azimuth + (FLOAT)rig_index * 90;

		}

	return VS_OK;
}

