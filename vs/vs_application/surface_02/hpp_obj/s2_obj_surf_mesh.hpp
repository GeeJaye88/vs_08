////////////////////////////////////////////////////////////////////////////////

// ---------- s2_obj_surf_mesh.hpp ----------
/*!
\file s2_obj_surf_mesh.h
\brief 
\author Gareth Edwards
*/


// ---------- ObjectSurfMesh_Setup ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectSurfMesh_Setup(INT index, Surface_02 *surface_02)
{

	// ---- local
		PI_Surface_02 *pimp = surface_02->pi_surface_02;


	// ---- surface


	// ---- runtime ? - add element, else force reconfig
		if ( pimp->runtime )
		{
			pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_MESH_LIST_IDX][0] =
				pimp->gfx->AddElement(
					pimp->scene_dspl_list_handle,
					"Surface",
					1,
					1,
					&pimp->obj_config->Callback_Mesh_Configure
				);
		}
		else
		{
			pimp->gfx->SetElementConfigureBitmask(
					pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_MESH_LIST_IDX][0],
					VS_ELM_SETUP_GEOMETRY
				);
		}


	// ---- get graphics context
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
					pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_MESH_LIST_IDX][0],
					&elm_context
				);


	// ---- set context
		hr = elm_context->SetViewMatrix(1);
		hr = elm_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NULL);
		hr = elm_context->SetPick3D(FALSE);
		hr = elm_context->SetFog(FALSE);
		hr = elm_context->SetLit(TRUE);
		hr = elm_context->SetAmbient(TRUE);
		hr = elm_context->SetMaterialIndex(11);
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
		INT result = ObjectSurfMesh_UpdateParam(index, surface_02);


	return VS_OK;
}


// ---------- ObjectSurfMesh_Display ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Surface_02::ObjectSurfMesh_Display(INT index, Surface_02 *surface_02, BOOL visible)
{

	// ---- local
		PI_Surface_02 *pimp = surface_02->pi_surface_02;
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_MESH_LIST_IDX][0],
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
		elm_context->SetUseMatrix(TRUE);
		elm_context->SetMatrix(m);
		elm_context->SetViewMatrix(1);

	// ---- pop
		pimp->matrix_stack->Pop();

	return VS_OK;
}


// ---------- SurfaceUpdate ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note invoked at setup and when surface mode selected or param is updated

*/
INT Surface_02::ObjectSurfMesh_ChangeTexture(INT index, Surface_02 *surface_02)
{

	// ---- local
		PI_Surface_02 *pimp = surface_02->pi_surface_02;
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_MESH_LIST_IDX][0],
				&elm_context
			);

	// ---- setup/update common parameters
		elm_context->SetAtlasPage(0, pimp->_configuration.acquire_atlas_page.ref);

	return VS_OK;
}


// ---------- ObjectSurfMesh_Update ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note invoked at setup and when surface mode selected or param is updated

*/
INT Surface_02::ObjectSurfMesh_Update(INT index, Surface_02 *surface_02)
{

	// ---- force surface reconfig
		PI_Surface_02 *pimp = surface_02->pi_surface_02;
		pimp->gfx->SetElementConfigureBitmask(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_MESH_LIST_IDX][0],
				VS_ELM_SETUP_ALL
			);

	// ---- setup/update common parameters
		INT result = ObjectSurfMesh_UpdateParam(index, surface_02);
	
	return VS_OK;
}


// ---------- ObjectSurfMesh_UpdateParam ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note - 1. get element context

        SOURCE mesh:
           2. setup LONG[0 to 2] param 
           3. init element context FLOAT param

		DESTINATION mesh:
		   4. setup LONG[3 & 4] param
		   5. init element context FLOAT param

\note - pass heightfield data via element context long parameter array


               MESH (e.g. MESH_VERSION_CAMERA)
       --------------------------------------------
  #    CAMERA or ACQUIRE  TERRAIN or WELD  FUNCTION
	   -----------------  ---------------  --------
  0         buffer         source buffer      na
  1         stride         source extent      na
  2         scalar              na            na

             ALL
	   	     ---
  3      dest buffer
  4      dest extent

            GROUP (e.g. PARAM_GROUP_CAMERA)
       ----------------------------------------
	   CAMERA  ACQUIRE  TERRAIN  WELD  FUNCTION
	   ------  -------  -------  ----  --------
       CAMERA  ACQUIRE   WELD    WELD  FUNCTION

*/
INT Surface_02::ObjectSurfMesh_UpdateParam(INT index, Surface_02 *surface_02)
{

	// ---- local
		PI_Surface_02 *pimp = surface_02->pi_surface_02;


	// ---- 1. get element context
		vs_system::ElmContext *elm_context = NULL;
		INT hr = pimp->gfx->GetElementContext(
				pimp->gfx_elem_handle[PI_Surface_02::ELM_SURF_MESH_LIST_IDX][0],
				&elm_context
			);
		LONG *l = elm_context->GetLongParameter();


	// ---- 2. setup LONG[0 to 2] param as per surface version
		INT surface_version_selected = surface_02->GetAcquireSurfaceVersionSelected();
		switch ( surface_version_selected )
		{

			case vs_system::ObjConfig::MESH_VERSION_CAMERA:
			case vs_system::ObjConfig::MESH_VERSION_ACQUIRE:
				{
					//
					// note, currently for development these are:
					//       *(ld + 0) = 0
					//       *(ld + 1) = 0
					//       *(ld + 2) = 1
					//
					*(l + 0) = (LONG)pimp->_configuration.acquire_height_field_param.buffer;
					*(l + 1) = (LONG)pimp->_configuration.acquire_height_field_param.stride;
					*(l + 2) = (LONG)pimp->_configuration.acquire_height_field_param.scalar;
				}
				break;

			case vs_system::ObjConfig::MESH_VERSION_TERRAIN:
				{

					if ( pimp->terrain_bump_map == NULL )
					{
						pimp->terrain_bump_map = new vs_system::Heightfield();
						UCHAR *terrain_bump_map = NULL;
						INT    terrain_bump_map_num_vertices = 0;
						BOOL ok = pimp->rr_bump_map[pimp->rr_bump_map_index]->Get(
										"terrain_1025.raw",
										&terrain_bump_map,
										&terrain_bump_map_num_vertices
									);

						if ( !ok )
						{
							delete pimp->terrain_bump_map;
							pimp->terrain_bump_map = NULL;
						}

						pimp->terrain_bump_map->heightfield_data   = terrain_bump_map;
						pimp->terrain_bump_map->heightfield_extent = terrain_bump_map_num_vertices;
					}

					*(l + 0) = (LONG)pimp->terrain_bump_map->heightfield_data;
					*(l + 1) = (LONG)pimp->terrain_bump_map->heightfield_extent;
					*(l + 2) = 0;
				}
				break;
		
			case vs_system::ObjConfig::MESH_VERSION_RR:
				{

					if ( pimp->rr_bump_map[pimp->rr_bump_map_index] == NULL )
					{
						char *rr_name = "";
						switch ( pimp->rr_bump_map_index )
						{
							case 0: rr_name = "00_rr_surface_1025.raw"; break;
							case 1: rr_name = "01_rr_surface_1025.raw"; break;
							case 2: rr_name = "02_rr_surface_1025.raw"; break;
							case 3: rr_name = "03_rr_surface_1025.raw"; break;
							default: break;
						}

						INT rr_bump_map_index = 0;

						pimp->rr_bump_map[pimp->rr_bump_map_index] = new vs_system::Heightfield();
						UCHAR *rr_bump_map = NULL;
						INT    rr_bump_map_num_vertices = 0;
						BOOL ok = pimp->rr_bump_map[pimp->rr_bump_map_index]->Get(
										rr_name,
										&rr_bump_map,
										&rr_bump_map_num_vertices
									);
						if ( !ok )
						{
							delete pimp->rr_bump_map[pimp->rr_bump_map_index];
							pimp->rr_bump_map[pimp->rr_bump_map_index] = NULL;
						}

						pimp->rr_bump_map[pimp->rr_bump_map_index]->heightfield_data   = rr_bump_map;
						pimp->rr_bump_map[pimp->rr_bump_map_index]->heightfield_extent = rr_bump_map_num_vertices;

					}

					*(l + 0) = (LONG)pimp->rr_bump_map[pimp->rr_bump_map_index]->heightfield_data;
					*(l + 1) = (LONG)pimp->rr_bump_map[pimp->rr_bump_map_index]->heightfield_extent;
					*(l + 2) = 0;
				}
				break;

			case vs_system::ObjConfig::MESH_VERSION_FUNCTION:
				{
					; // handled in Callback_Mesh_GetVertexBuffer
				}
				break;

		}


	// ---- update
		UINT mode = pimp->_configuration.surface_mesh.solid ?
			( pimp->_configuration.surface_mesh.textured ? 3 : 1 ) :
				( pimp->_configuration.surface_mesh.textured ? 2 : 0 );


	// ---- parameters
		FLOAT rows = (FLOAT)pimp->_configuration.surface_mesh.rows;
		FLOAT cols = (FLOAT)pimp->_configuration.surface_mesh.cols;


	// ---- 3. init param as per mesh requirments


	// ---- build  
		FLOAT *param = elm_context->GetFloatParameter();
		param[vs_system::ObjConfig::MESH_TYPE]   = (FLOAT)mode;
		param[vs_system::ObjConfig::MESH_AP_REF] = (FLOAT)pimp->_configuration.acquire_atlas_page.ref;
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


	// ---- y surface scalar
		//
		//  note: this calc is also used in the image processing code (see s2_model_acquire.hpp)
		//
		FLOAT y_scalar = surface_02->GetAcquireSurfaceSize() * surface_02->GetAcquireSurfaceYScalar();


	// ---- y scalar - as required this value is also multiplied to flatten
		//
		// note: scalar to allow pseudo acqusition of 3D surface, e.g. flat (0) or 3D (1)
		//
		y_scalar *= surface_02->GetAcquireHeightFieldScalar(); 
		param[vs_system::ObjConfig::MESH_SCALAR] = y_scalar;


	// ---- 4. setup LONG[3 & 4] param as per DESTINATION mesh requirements


	// ---- ! heightfield exists ?
		if ( pimp->y_heightfield == NULL )
		{
			pimp->y_heightfield = new vs_system::FloatBuffer();
		}


	// ---- ! heightfield initialised ?
		if ( pimp->y_heightfield->GetRows() != rows || pimp->y_heightfield->GetColumns() != cols )
		{
			pimp->y_heightfield->Initialise((INT)cols+1, (INT)rows+1, 1);
			pimp->y_heightfield->DestroyBuffer();
			pimp->y_heightfield->CreateBuffer();
		}


	// ---- store buffer & extent (0, 1 & 2 set above )
		//
		// note: *(ld + 0) = (e.g.) (LONG)pimp->terrain_bump_map->heightfield_data;
		//       *(ld + 1) = (e.g.) (LONG)pimp->terrain_bump_map->heightfield_extent;
		//       *(ld + 2) = 0 or scalar
		//
		*(l + 3) = (LONG)pimp->y_heightfield->GetBuffer();
		*(l + 4) = (LONG)pimp->y_heightfield->GetExtent();


	// ---- 5. setup FLOAT param as per DESTINATION mesh requirements


	// ---- store param group id & then set param group
		INT param_group = surface_02->parameter.GetParamGroup();
		INT surface_param_group = surface_02::Model::PARAM_GROUP_CAMERA;
		switch ( surface_version_selected )
		{
			default:
			case vs_system::ObjConfig::MESH_VERSION_CAMERA:
				surface_param_group = surface_02::Model::PARAM_GROUP_CAMERA;
				break;
			case vs_system::ObjConfig::MESH_VERSION_RR:
			case vs_system::ObjConfig::MESH_VERSION_TERRAIN:
				surface_param_group = surface_02::Model::PARAM_GROUP_BUMP_MAP;
				break;
			case vs_system::ObjConfig::MESH_VERSION_FUNCTION :
				surface_param_group = surface_02::Model::PARAM_GROUP_FUNCTION;
				break;
			case vs_system::ObjConfig::MESH_VERSION_ACQUIRE:
				surface_param_group = surface_02::Model::PARAM_GROUP_ACQUIRE;
				break;
		}
		surface_02->parameter.SetParamGroup(surface_param_group);


	// ---- set EIGHT parameters
		INT result = VS_OK;
		INT param_group_index = surface_02->parameter.GetParamGroupIndex();
		INT param_array_index = surface_02->parameter.GetParamArrayIndex();
		INT param_array_size  = surface_02->parameter.GetParamArraySize();
		FLOAT *param_array = new FLOAT[param_array_size];
		result = surface_02->parameter.GetParamArray(param_group_index, 9, param_array);
		for (UINT i = 0; i < 8; i++)
		{
			param[vs_system::ObjConfig::MESH_PARAM_0+i] = param_array[i];
		}
		delete[] param_array;
	

	// ---- restore param group id
		result = surface_02->parameter.SetParamGroup(param_group);

	return VS_OK;
}

