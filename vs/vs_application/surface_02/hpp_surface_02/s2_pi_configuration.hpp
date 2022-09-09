////////////////////////////////////////////////////////////////////////////////

// ---------- s2_pi_configuration.hpp ----------
/*!
\file s2_pi_configuration.hpp
\brief Surface_02 display configuration methods
\author Gareth Edwards
*/


// -------- Acquire Atlas Pages ------------------------------------------------

	VOID Surface_02::SetAcquireAtlasPageLid(UINT atlas_page_ref)
	{
		pi_surface_02->_configuration.acquire_atlas_page.ref = atlas_page_ref;
	}

	UINT Surface_02::GetAcquireAtlasPageLid() 
	{
		return pi_surface_02->_configuration.acquire_atlas_page.ref;
	}
		
	VOID  Surface_02::SetAcquireAtlasPageWidth(UINT width)
	{
		pi_surface_02->_configuration.acquire_atlas_page.width = width;
	}

	VOID  Surface_02::SetAcquireAtlasPageHeight(UINT height)
	{
		pi_surface_02->_configuration.acquire_atlas_page.height = height;
	}

	VOID  Surface_02::SetAcquireAtlasPageBytes(UINT bytes)
	{
		pi_surface_02->_configuration.acquire_atlas_page.bytes = bytes;
	}

	UINT  Surface_02::GetAcquireAtlasPageWidth()
	{
		return pi_surface_02->_configuration.acquire_atlas_page.width;
	}

	UINT  Surface_02::GetAcquireAtlasPageHeight()
	{
		return pi_surface_02->_configuration.acquire_atlas_page.height;
	}

	UINT  Surface_02::GetAcquireAtlasPageBytes()
	{
		return pi_surface_02->_configuration.acquire_atlas_page.bytes;
	}


// -------- Acquire Dimensions ------------------------------------------------

	VOID Surface_02::SetAcquireSurfaceDimensions(FLOAT width, FLOAT height, FLOAT depth)
	{
		pi_surface_02->_configuration.acquire_dimensions.width  = width;
		pi_surface_02->_configuration.acquire_dimensions.height = height; // hgt not used
		pi_surface_02->_configuration.acquire_dimensions.depth  = depth;
	}

	VOID Surface_02::GetAcquireSurfaceDimensions(FLOAT *width, FLOAT *height, FLOAT *depth)
	{
		*width  = pi_surface_02->_configuration.acquire_dimensions.width;
		*height = pi_surface_02->_configuration.acquire_dimensions.height; // hgt not used
		*depth  = pi_surface_02->_configuration.acquire_dimensions.depth;
	}

	VOID Surface_02::SetAcquireSurfaceSize(FLOAT size)
	{
		pi_surface_02->_configuration.acquire_dimensions.size  = size;
		pi_surface_02->_configuration.acquire_dimensions.width = size;
		pi_surface_02->_configuration.acquire_dimensions.depth = size;
	}

	FLOAT Surface_02::GetAcquireSurfaceSize()
	{
		return pi_surface_02->_configuration.acquire_dimensions.size;
	}

	VOID Surface_02::SetAcquireSurfaceYScalar(FLOAT y_scalar)
	{
		pi_surface_02->_configuration.acquire_dimensions.y_scalar = y_scalar;
	}

	FLOAT Surface_02::GetAcquireSurfaceYScalar()
	{
		return pi_surface_02->_configuration.acquire_dimensions.y_scalar;
	}

	FLOAT Surface_02::GetAcquireSetupSourceSurfaceSize(UINT index)
	{
		return pi_surface_02->_configuration.source_surface_table[index].size;
	}

	FLOAT Surface_02::GetAcquireSetupSourceSurfaceYScalar(UINT index)
	{
		return pi_surface_02->_configuration.source_surface_table[index].y_scalar;
	}


// -------- Acquire Camera ------------------------------------------------

	VOID Surface_02::SetAcquireCameraWidth(UINT width)
	{
		pi_surface_02->_configuration.acquire_camera.width = width;
	}

	VOID Surface_02::SetAcquireCameraHeight(UINT height)
	{
		pi_surface_02->_configuration.acquire_camera.height = height;
	}

	VOID Surface_02::SetAcquireCameraBitdepth(UINT bitdepth)
	{
		pi_surface_02->_configuration.acquire_camera.bitdepth = bitdepth;
	}

	VOID Surface_02::SetAcquireCameraRect(VsIntRectangle *rect)
	{
		pi_surface_02->_configuration.acquire_camera.rect_width = rect->right - rect->left + 1;
		pi_surface_02->_configuration.acquire_camera.rect_height = rect->bottom - rect->top + 1;
		pi_surface_02->_configuration.acquire_camera.rect = *rect;
	}

	UINT Surface_02::GetAcquireCameraWidth()
	{
		return pi_surface_02->_configuration.acquire_camera.width;
	}

	UINT Surface_02::GetAcquireCameraHeight()
	{
		return pi_surface_02->_configuration.acquire_camera.height;
	}

	UINT Surface_02::GetAcquireCameraBitdepth()
	{
		return pi_surface_02->_configuration.acquire_camera.bitdepth;
	}

	UINT Surface_02::GetAcquireCameraRectWidth()
	{
		return pi_surface_02->_configuration.acquire_camera.rect_width;
	}

	UINT Surface_02::GetAcquireCameraRectHeight()
	{
		return pi_surface_02->_configuration.acquire_camera.rect_height;
	}

	VOID Surface_02::GetAcquireCameraRect(VsIntRectangle *rect)
	{
		*rect = pi_surface_02->_configuration.acquire_camera.rect;
	}


// -------- Acquire Lumina Buffer ------------------------------------------------

	INT Surface_02::GetAcquireLuminaBuffer(UINT ush_bufer_index, vs_system::UnsignedShortBuffer **ush_buffer)
	{
		if ( ush_bufer_index >= 0 && ush_bufer_index < pi_surface_02->MAX_ACQ_IMG_BUFFER )
		{
			*ush_buffer = &pi_surface_02->_configuration.aquire_lumina_buffer[ush_bufer_index];
			return VS_OK;
		}
		return VS_ERROR;
	}

	INT Surface_02::SetAcquireLuminaBuffer(UINT ush_bufer_index, vs_system::UnsignedShortBuffer *ush_buffer)
	{
		if ( ush_bufer_index >= 0 && ush_bufer_index < pi_surface_02->MAX_ACQ_IMG_BUFFER )
		{
			pi_surface_02->_configuration.aquire_lumina_buffer[ush_bufer_index] = *ush_buffer;
			return VS_OK;
		}
		return VS_ERROR;
	}

	UINT Surface_02::GetAcquireLuminaBufferMax()
	{
		return pi_surface_02->MAX_ACQ_LIGHT;
	}


// -------- Acquire Shader Buffer ------------------------------------------------

	INT Surface_02::GetAcquireShaderBuffer(UINT img_bufer_index, vs_system::ImgBuffer **img_buffer)
	{
		if ( img_bufer_index >= 0 && img_bufer_index < pi_surface_02->MAX_ACQ_IMG_BUFFER )
		{
			*img_buffer = &pi_surface_02->_configuration.aquire_shader_buffer[img_bufer_index];
			return VS_OK;
		}
		return VS_ERROR;
	}

	INT Surface_02::SetAcquireShaderBuffer(UINT img_bufer_index, vs_system::ImgBuffer *img_buffer)
	{
		if ( img_bufer_index >= 0 && img_bufer_index < pi_surface_02->MAX_ACQ_IMG_BUFFER )
		{
			pi_surface_02->_configuration.aquire_shader_buffer[img_bufer_index] = *img_buffer;
			return VS_OK;
		}
		return VS_ERROR;
	}

	UINT Surface_02::GetAcquireShaderBufferMax()
	{
		return pi_surface_02->MAX_ACQ_IMG_BUFFER;
	}


// -------- Acquire Analysis Buffer ------------------------------------------------

	INT Surface_02::GetAcquireAnalysisBuffer(UINT img_bufer_index, vs_system::ImgBuffer **img_buffer)
	{
		if ( img_bufer_index >= 0 && img_bufer_index < pi_surface_02->MAX_ACQ_IMG_BUFFER )
		{
			*img_buffer = &pi_surface_02->_configuration.aquire_analysis_buffer[img_bufer_index];
			return VS_OK;
		}
		return VS_ERROR;
	}

	INT Surface_02::SetAcquireAnalysisBuffer(UINT img_bufer_index, vs_system::ImgBuffer *img_buffer)
	{
		if ( img_bufer_index >= 0 && img_bufer_index < pi_surface_02->MAX_ACQ_IMG_BUFFER )
		{
			pi_surface_02->_configuration.aquire_analysis_buffer[img_bufer_index] = *img_buffer;
			return VS_OK;
		}
		return VS_ERROR;
	}
	
	UINT Surface_02::GetAcquireAnalysisBufferMax()
	{
		return pi_surface_02->MAX_ACQ_IMG_BUFFER;
	}


// -------- Acquire Surface ------------------------------------------------

	UINT Surface_02::GetAcquireSurfaceVersionSelected()
	{
		return pi_surface_02->_configuration.acquire_surface.version;
	}

	VOID Surface_02::SetAcquireSurfaceVersionSelected(UINT version)
	{
		pi_surface_02->_configuration.acquire_surface.version = version;
	}

	UINT Surface_02::GetAcquireSurfaceModeSelected()
	{
		return pi_surface_02->_configuration.acquire_surface.selected;
	}

	VOID Surface_02::SetAcquireSurfaceModeSelected(UINT selected)
	{
		pi_surface_02->_configuration.acquire_surface.selected = selected;
	}


// -------- Acquire Light ------------------------------------------------

	VOID  Surface_02::SetAcquireLightParam(
			FLOAT intensity,
			FLOAT distance,
			FLOAT elevation,
			FLOAT azimuth,
			FLOAT scalar
		)
	{
		pi_surface_02->_configuration.acquire_light_param.intensity = intensity;
		pi_surface_02->_configuration.acquire_light_param.distance  = distance;
		pi_surface_02->_configuration.acquire_light_param.elevation = elevation;
		pi_surface_02->_configuration.acquire_light_param.azimuth   = azimuth;
		pi_surface_02->_configuration.acquire_light_param.scalar    = scalar;
	}
	FLOAT Surface_02::GetAcquireLightIntensityParam()
	{
		return pi_surface_02->_configuration.acquire_light_param.intensity;
	}
	FLOAT Surface_02::GetAcquireLightDistanceParam()
	{
		return pi_surface_02->_configuration.acquire_light_param.distance;
	}
	FLOAT Surface_02::GetAcquireLightElevationParam()
	{
		return pi_surface_02->_configuration.acquire_light_param.elevation;
	}
	FLOAT Surface_02::GetAcquireLightAzimuthParam()
	{
		return pi_surface_02->_configuration.acquire_light_param.azimuth;
	}
	FLOAT Surface_02::GetAcquireLightScalarValue()
	{
		return pi_surface_02->_configuration.acquire_light_param.scalar;
	}
	

// -------- Acquire Light ------------------------------------------------

	UINT Surface_02::GetAcquireLightMax()
	{
		return pi_surface_02->MAX_ACQ_LIGHT;
	}

	UINT Surface_02::GetAcquireLightIndex()
	{
		return pi_surface_02->_configuration.acquire_light_index;
	}

	INT Surface_02::SetAcquireLightIndex(UINT light_index)
	{
		if (light_index >= 0 && light_index < Surface_02::PI_Surface_02::MAX_ACQ_LIGHT)
		{
			pi_surface_02->_configuration.acquire_light_index = light_index;
			return VS_OK;
		}
		return VS_ERROR;
	}

	INT Surface_02::GetAcquireLight(UINT light_index, vs_system::VsLight **light)
	{
		if ( light_index >= 0 && light_index < Surface_02::PI_Surface_02::MAX_ACQ_LIGHT )
		{
			*light = &pi_surface_02->_configuration.acquire_light[light_index];
			return VS_OK;
		}
		return VS_ERROR;
	}

	INT Surface_02::SetAcquireLight(UINT light_index, vs_system::VsLight *light)
	{
		if ( light_index >= 0 && light_index < Surface_02::PI_Surface_02::MAX_ACQ_LIGHT )
		{
			pi_surface_02->_configuration.acquire_light[light_index] = *light;
			return VS_OK;
		}
		return VS_ERROR;
	}

	BOOL Surface_02::GetAcquireLightEnabled(UINT index)
	{
		return pi_surface_02->_configuration.acquire_light_enabled.state[index];
	}

	INT Surface_02::SetAcquireLightEnabled(UINT index)
	{
		if ( index >= PI_Surface_02::MAX_ACQ_LIGHT ) return VS_ERROR;
		for (INT i = 0; i < PI_Surface_02::MAX_ACQ_LIGHT; i++)
		{
			pi_surface_02->_configuration.acquire_light_enabled.state[i] =
				index == (UINT)i ? TRUE : FALSE;
		}
		return VS_OK;
	}
	
	BOOL Surface_02::GetAcquireLightColouredEnabled()
	{
		return pi_surface_02->_configuration.acquire_light_enabled.colour;
	}

	VOID Surface_02::SetAcquireLightColouredEnabled(BOOL colour_enabled)
	{
		pi_surface_02->_configuration.acquire_light_enabled.colour = colour_enabled;
	}

	BOOL Surface_02::GetAcquireLightSpecularEnabled()
	{
		return pi_surface_02->_configuration.acquire_light_enabled.specular;
	}

	VOID Surface_02::SetAcquireLightSpecularEnabled(BOOL specular_enabled)
	{
		pi_surface_02->_configuration.acquire_light_enabled.specular = specular_enabled;
	}


// -------- Acquire Material ------------------------------------------------

	INT Surface_02::GetAcquireMaterial(vs_system::VsMaterial **material)
	{
		*material = &pi_surface_02->_configuration.acquire_material;
		return VS_OK;
	}

	INT Surface_02::SetAcquireMaterial(vs_system::VsMaterial *material)
	{
		pi_surface_02->_configuration.acquire_material = *material;
		return VS_OK;
	}


// -------- Acquire Height Field Param ------------------------------------------------

	VOID Surface_02::SetAcquireHeightfieldParam(
			UINT   stride,
			FLOAT *buffer,
			FLOAT  scalar
		)
	{
		pi_surface_02->_configuration.acquire_height_field_param.stride = stride;
		pi_surface_02->_configuration.acquire_height_field_param.buffer = buffer;
		pi_surface_02->_configuration.acquire_height_field_param.scalar = scalar;
	}


// -------- Acquire Height Field Array Param ------------------------------------------------
	/*
		UINT   stride    = 0;
		UINT   width     = 0;
		UINT   extent    = 0;
		UINT   dimension = 0;
		FLOAT *array     = NULL;
	*/

	INT Surface_02::SetAcquireHeightFieldArray(FLOAT * array)
	{
		pi_surface_02->_configuration.aquire_height_field.array = array;
		return VS_OK;
	}

	INT Surface_02::SetAcquireHeightFieldDimension(UINT dimension)
	{
		pi_surface_02->_configuration.aquire_height_field.dimension = dimension;
		return VS_OK;
	}

	INT Surface_02::SetAcquireHeightFieldExtent(UINT extent)
	{
		pi_surface_02->_configuration.aquire_height_field.extent = extent;
		return VS_OK;
	}

	INT Surface_02::SetAcquireHeightFieldStride(UINT stride)
	{
		pi_surface_02->_configuration.aquire_height_field.stride = stride;
		return VS_OK;
	}

	INT Surface_02::SetAcquireHeightFieldWidth(UINT width)
	{
		pi_surface_02->_configuration.aquire_height_field.width = width;
		return VS_OK;
	}

	FLOAT * Surface_02::GetAcquireHeightFieldArray()
	{
		return pi_surface_02->_configuration.aquire_height_field.array;
	}

	UINT Surface_02::GetAcquireHeightFieldDimension()
	{
		return pi_surface_02->_configuration.aquire_height_field.dimension;
	}

	UINT Surface_02::GetAcquireHeightFieldExtent()
	{
		return pi_surface_02->_configuration.aquire_height_field.extent;
	}

	UINT Surface_02::GetAcquireHeightFieldStride()
	{
		return pi_surface_02->_configuration.aquire_height_field.stride;
	}

	UINT Surface_02::GetAcquireHeightFieldWidth()
	{
		return pi_surface_02->_configuration.aquire_height_field.width;
	}

	VOID  Surface_02::SetAcquireHeightFieldMin(FLOAT min)
	{
		pi_surface_02->_configuration.aquire_height_field.min = min;
	}

	FLOAT Surface_02::GetAcquireHeightFieldMin()
	{
		return pi_surface_02->_configuration.aquire_height_field.min;
	}

	VOID  Surface_02::SetAcquireHeightFieldMax(FLOAT max)
	{
		pi_surface_02->_configuration.aquire_height_field.max = max;
	}

	FLOAT Surface_02::GetAcquireHeightFieldMax()
	{
		return pi_surface_02->_configuration.aquire_height_field.max;
	}

	VOID  Surface_02::SetAcquireHeightFieldScalar(FLOAT scalar)
	{
		pi_surface_02->_configuration.aquire_height_field.scalar = scalar;
	}

	FLOAT Surface_02::GetAcquireHeightFieldScalar()
	{
		return pi_surface_02->_configuration.aquire_height_field.scalar;
	}



// -------- explore kamera heightfield ------------------------------------------------

	VOID Surface_02::SetExploreKameraMaxScalarSelected(BOOL selected)
	{
		pi_surface_02->_configuration.explore_kamera.max_scalar_selected = selected;
	}

	VOID Surface_02::SetExploreKameraMinScalar(FLOAT min)
	{
		pi_surface_02->_configuration.explore_kamera.min_scalar = min;
	}

	VOID Surface_02::SetExploreKameraMaxScalar(FLOAT max)
	{
		pi_surface_02->_configuration.explore_kamera.max_scalar = max;
	}

	BOOL Surface_02::GetExploreKameraMaxScalarSelected()
	{
		return pi_surface_02->_configuration.explore_kamera.max_scalar_selected;
	}

	FLOAT Surface_02::GetExploreKameraMinScalar()
	{
		return pi_surface_02->_configuration.explore_kamera.min_scalar;
	}

	FLOAT Surface_02::GetExploreKameraMaxScalar()
	{
		return pi_surface_02->_configuration.explore_kamera.max_scalar;
	}


// -------- Explore Light ------------------------------------------------

	/*
	INT Surface_02::GetExploreLightMax()
	{
		return PI_Surface_02::MAX_EXP_LGT;
	}

	INT Surface_02::GetExploreLight(UINT light_index, vs_system::VsLight **light)
	{
		if ( light_index >= 0 && light_index < PI_Surface_02::MAX_EXP_LGT )
		{
			*light = &pi_surface_02->_configuration.explore_light[light_index];
			return VS_OK;
		}
		return VS_ERROR;
	}

	INT Surface_02::SetExploreLight(UINT light_index, vs_system::VsLight *light)
	{
		if ( light_index >= 0 && light_index < PI_Surface_02::MAX_EXP_LGT )
		{ 
			pi_surface_02->_configuration.explore_light[light_index] = *light;
			return VS_OK;
		}
		return VS_ERROR;
	}

	// used

	BOOL Surface_02::GetExploreLightEnabled(UINT index)
	{
		return pi_surface_02->_configuration.explore_light_enabled.state[index];
	}

	// used

	INT Surface_02::SetExploreLightEnabled(UINT index, BOOL enabled)
	{
		pi_surface_02->_configuration.explore_light_enabled.state[index] = enabled;
		return VS_OK;
	}

	// used

	INT Surface_02::SetExploreLightEnabled(UINT index)
	{
		if ( index >= PI_Surface_02::MAX_EXP_LGT ) return VS_ERROR;
		for (INT i = 0; i < PI_Surface_02::MAX_EXP_LGT; i++)
		{
			pi_surface_02->_configuration.explore_light_enabled.state[i] =
				index == (UINT)i ? TRUE : FALSE;
		}
		return VS_OK;
	}

	VOID Surface_02::GetExploreLightsEnabled(BOOL light_enabled[])
	{
		for (INT i = 0; i < PI_Surface_02::MAX_EXP_LGT; i++)
		{
			light_enabled[i] = pi_surface_02->_configuration.explore_light_enabled.state[i];
		}
	}

	VOID Surface_02::SetExploreLightsEnabled(BOOL light_enabled[])
	{
		for (INT i = 0; i < PI_Surface_02::MAX_EXP_LGT; i++)
		{
			pi_surface_02->_configuration.explore_light_enabled.state[i] = light_enabled[i];
		}
	}

	BOOL Surface_02::GetExploreLightColouredEnabled()
	{
		return pi_surface_02->_configuration.explore_light_enabled.colour;
	}

	VOID Surface_02::SetExploreLightColouredEnabled(BOOL colour_enabled)
	{
		pi_surface_02->_configuration.explore_light_enabled.colour = colour_enabled;
	}

	BOOL Surface_02::GetExploreLightSpecularEnabled()
	{
		return pi_surface_02->_configuration.explore_light_enabled.specular;
	}

	VOID Surface_02::SetExploreLightSpecularEnabled(BOOL specular_enabled)
	{
		pi_surface_02->_configuration.explore_light_enabled.specular = specular_enabled;
	}
	*/

// -------- MESH ------------------------------------------------

	VOID Surface_02::SetMeshDensity(UINT mode)
	{
		UINT rows = 512, cols = 512;
		switch (mode)
		{
			case  1: rows = cols = 256; break;
			case  2: break;
			case  3: rows = cols = 1024; break;
			default: break;
		}
		pi_surface_02->_configuration.surface_mesh.cols = cols;
		pi_surface_02->_configuration.surface_mesh.rows = rows;
	}

	VOID Surface_02::SetMeshSolid(BOOL s)
	{
		pi_surface_02->_configuration.surface_mesh.solid = s ? TRUE : FALSE;
	}

	VOID Surface_02::SetMeshTextured(BOOL t)
	{
		pi_surface_02->_configuration.surface_mesh.textured = t ? TRUE : FALSE;
	}

	UINT Surface_02::GetMeshDisplayMode()
	{
		return pi_surface_02->_configuration.surface_mesh.display;
	}

	VOID Surface_02::SetMeshDisplayMode(UINT d)
	{
		pi_surface_02->_configuration.surface_mesh.display =
			d < MeshDisplayMode::WU ? MeshDisplayMode::WU :
				( d > MeshDisplayMode::ST ? MeshDisplayMode::ST : d);
	}

	UINT Surface_02::GetMeshDensityMode()
	{
		return pi_surface_02->_configuration.surface_mesh.density;
	}

	VOID Surface_02::SetMeshDensityMode(UINT d)
	{
		pi_surface_02->_configuration.surface_mesh.density =
			d < MeshDensityMode::LOW ? MeshDensityMode::LOW :
				(d > MeshDensityMode::HIGH ? MeshDensityMode::HIGH : d);
	}

	UINT Surface_02::GetMeshColumns()
	{
		return pi_surface_02->_configuration.surface_mesh.cols;
	}

	UINT Surface_02::GetMeshRows()
	{
		return pi_surface_02->_configuration.surface_mesh.rows;
	}
		
	BOOL Surface_02::GetMeshSolid()
	{
		return pi_surface_02->_configuration.surface_mesh.solid;
	}
		
	BOOL  Surface_02::GetMeshTextured()
	{
		return pi_surface_02->_configuration.surface_mesh.textured;
	}


// -------- TARGET ------------------------------------------------
	
	VOID Surface_02::InitTargetLoi()
	{
		pi_surface_02->SetupTargetLoi();
	}

	// ---- index
		VOID Surface_02::SetTargetLoiIndex(UINT index)
		{
			pi_surface_02->_configuration.target_loi_index = index;
		}
		UINT Surface_02::GetTargetLoiIndex()
		{
			return pi_surface_02->_configuration.target_loi_index;
		}

	// ---- init
		VOID Surface_02::SetTargetLoiInit(BOOL b)
		{
			PI_Surface_02::Surface_02_Config *conf = &pi_surface_02->_configuration;
			UINT index = conf->target_loi_index;
			pi_surface_02->_configuration.target_init[index] = b;
		}
		BOOL Surface_02::GetTargetLoiInit()
		{
			PI_Surface_02::Surface_02_Config *conf = &pi_surface_02->_configuration;
			UINT index = conf->target_loi_index;
			return pi_surface_02->_configuration.target_init[index];
		}

	// ---- location
		VsVector3 * Surface_02::GetTargetLoi()
		{
			PI_Surface_02::Surface_02_Config *conf = &pi_surface_02->_configuration;
			UINT index = conf->target_loi_index;
			return &conf->target_loi[index];
		}
		VsVector3 * Surface_02::GetTargetLoi(UINT index)
		{
			return &pi_surface_02->_configuration.target_loi[index];
		}

	// ---- rotation
		FLOAT Surface_02::GetTargetLoiRotation(UINT index)
		{
			return pi_surface_02->_configuration.target_rot[index];
		}


	// ---- ruler
		VOID Surface_02::SetTargetLoiRulerVisible(BOOL target_ruler_visible)
		{
			pi_surface_02->_configuration.target_ruler_visible = target_ruler_visible;
			pi_surface_02->SetObjectVisible(PI_Surface_02::GOB_EXP_LOI_RECT_IDX, target_ruler_visible);
			pi_surface_02->SetObjectVisible(PI_Surface_02::GOB_EXP_LOI_RULE_IDX, target_ruler_visible);

			ObjectLoiRect_HideAll(this);
			ObjectLoiRule_HideAll(this);

		}
		BOOL Surface_02::GetTargetLoiRulerVisible()
		{
			return pi_surface_02->_configuration.target_ruler_visible;
		}


	// ---- translate
		VOID Surface_02::SetTargetLoiTranslate(BOOL target_transform)
		{
			pi_surface_02->_configuration.target_transform = target_transform;
		}
		BOOL Surface_02::GetTargetLoiTranslate()
		{
			return pi_surface_02->_configuration.target_transform;
		}


	// ---- visible
		VOID Surface_02::SetTargetLoiVisible(BOOL target_visible)
		{
			pi_surface_02->_configuration.target_loi_visible = target_visible;
			pi_surface_02->SetObjectVisible(PI_Surface_02::GOB_EXP_LOI_SPOT_IDX,target_visible);
			pi_surface_02->SetObjectVisible(PI_Surface_02::GOB_EXP_LOI_AXIS_IDX, target_visible);
		}
		BOOL Surface_02::GetTargetLoiVisible()
		{
			return pi_surface_02->_configuration.target_loi_visible;
		}


	// --- substantive get & set

		FLOAT Surface_02::CalculateTargetLoi_Surface_Y(UINT index)
		{

			// ---- local
				PI_Surface_02::Surface_02_Config *conf = &pi_surface_02->_configuration;

			// ---- target
				FLOAT x = conf->target_loi[index].x;
				FLOAT y = conf->target_loi[index].y;
				FLOAT z = conf->target_loi[index].z;

			// ---- aquire dimensions
				FLOAT  width = conf->acquire_dimensions.width;
				FLOAT  depth = conf->acquire_dimensions.depth;

			// ---- aquire heightfield
				UINT   stride = conf->aquire_height_field.stride;
				UINT   hf_width  = conf->aquire_height_field.width;
				UINT   extent    = conf->aquire_height_field.extent;
				UINT   dimension = conf->aquire_height_field.dimension;
				FLOAT *array     = conf->aquire_height_field.array;

			// ---- quick test for no height data
				if ( extent == 0 ) return 0;

			// ---- note: no weighted sampling

			// ---- so, assuming legal x & z ordinates
				FLOAT z_ok = (z + depth / 2) / depth;
				FLOAT x_ok = (x + width / 2) / width;

				INT row = (INT)(z_ok * (FLOAT)stride);
					row = stride - row;
				INT col = (INT)(x_ok * (FLOAT)stride);

			// ---- Y!
				UINT new_extent = row * stride + col;
				new_extent = new_extent >= extent ? extent - 1 : new_extent;
				FLOAT height = *(array + new_extent);

			return height;
		}

		FLOAT Surface_02::CalculateTargetLoi_Surface_Y_Scalar()
		{

			INT surface_version_selected = GetAcquireSurfaceVersionSelected();
			FLOAT scalar = 1;
			switch ( surface_version_selected )
			{
				case vs_system::ObjConfig::MESH_VERSION_CAMERA:
				case vs_system::ObjConfig::MESH_VERSION_ACQUIRE:
				case vs_system::ObjConfig::MESH_VERSION_TERRAIN:
				case vs_system::ObjConfig::MESH_VERSION_RR:
					{
						scalar = 0.1f;
					}
					break;
				case vs_system::ObjConfig::MESH_VERSION_FUNCTION:
					break;
			}

			return scalar;
		}

		VOID Surface_02::SetTargetLoiIncr(FLOAT x, FLOAT y, FLOAT z)
		{

			// ---- local
				PI_Surface_02::Surface_02_Config *conf = &pi_surface_02->_configuration;
				UINT index = conf->target_loi_index;

			// ---- translate
				FLOAT w_max = conf->acquire_dimensions.width / 2;
				FLOAT w_min = -w_max;
				FLOAT x_loc = conf->target_loi[index].x + x;
				x_loc = x_loc < w_min ? w_min : (x_loc > w_max ? w_max : x_loc);

				FLOAT d_max = conf->acquire_dimensions.depth / 2;
				FLOAT d_min = -d_max;
				FLOAT z_loc = conf->target_loi[index].z + z;
				z_loc = z_loc < w_min ? w_min : (z_loc > w_max ? w_max : z_loc);

				conf->target_loi[index].x = x_loc;
				conf->target_loi[index].z = z_loc;

			// ---- rotate
				conf->target_rot[index] += y;

		}
	
		// ---- calculate intersection of two lines.
		//   - return true if found, false if not found or error

		inline double Det(double a, double b, double c, double d)
		{
			return a * d - b * c;
		}

		BOOL Surface_02::CalculateTargetLoi_Surface_Line_Edge_Intersect(
					DOUBLE x1, DOUBLE y1, // input - line 1 start
					DOUBLE x2, DOUBLE y2, //       - line 1 end
					DOUBLE x3, DOUBLE y3, //       - line 2 start
					DOUBLE x4, DOUBLE y4, //       - line 2 end
					DOUBLE &ixOut,        // output - start
					DOUBLE &iyOut         //        - end
				)
		{

			// http://mathworld.wolfram.com/Line-LineIntersection.html

			double detL1 = Det(x1, y1, x2, y2);
			double detL2 = Det(x3, y3, x4, y4);
			double x1mx2 = x1 - x2;
			double x3mx4 = x3 - x4;
			double y1my2 = y1 - y2;
			double y3my4 = y3 - y4;

			double xnom = Det(detL1, x1mx2, detL2, x3mx4);
			double ynom = Det(detL1, y1my2, detL2, y3my4);
			double denom = Det(x1mx2, y1my2, x3mx4, y3my4);
			if( denom == 0.0 ) // lines don't seem to cross
			{
				ixOut = NAN;
				iyOut = NAN;
				return false;
			}

			ixOut = xnom / denom;	
			iyOut = ynom / denom;
			if( !isfinite(ixOut ) || ! isfinite(iyOut) ) //Probably a numerical issue
				return false;

			return true; //All OK
		}

		VOID Surface_02::CalculateTargetLoi_Surface_Section_Intersect()
		{
			// ---- local
				PI_Surface_02::Surface_02_Config *conf = &pi_surface_02->_configuration;
				UINT index = conf->target_loi_index;

			// ---- uncalculated
				FLOAT width = conf->acquire_dimensions.width;
				FLOAT depth = conf->acquire_dimensions.depth;

			// ---- location
				FLOAT x_off = conf->target_loi[index].x;
				FLOAT z_off = conf->target_loi[index].z;

			// ---- other parameter data
				FLOAT angle = conf->target_rot[index];


			// ---- calculate the two intersects
				float rmin_x = -width / 2;
				float rmin_y = -depth / 2;
				float rmax_x =  width / 2;
				float rmax_y =  depth / 2;

			// ---- calculate two rect edge intersects

				// 2D point
				struct punt { float x; float y; };

				punt s_xy[5] = // outline of rect
				{
					{ rmin_x, rmin_y },
					{ rmin_x, rmax_y },
					{ rmax_x, rmax_y },
					{ rmax_x, rmin_y },
					{ rmin_x, rmin_y }
				};

				punt p_xy[2] = // zero angle line 
				{
					{ -900, 0 },
					{  900, 0 },
				};

				UINT r_index = 0;
				VsVector2 r_xy[2] = // intersect results
				{
					{ 0, 0 },
					{ 0, 0 },
				};


			// ---- adjust rect tolerance
				float tolr = 0.001f;
				rmin_x -= tolr;
				rmin_y -= tolr;
				rmax_x += tolr;
				rmax_y += tolr;


			// ---- edges
				FLOAT ar = angle * (FLOAT)VS_RADIANS_PER_DEGREE;
				double ix=0, iy=0;
				for (UINT e = 0; e < 4; e++)
				{ 

					// edge line
					float x1 = s_xy[e].x;
					float y1 = s_xy[e].y;
					float x2 = s_xy[e+1].x;
					float y2 = s_xy[e+1].y;

					// blade line
					for (UINT l = 0; l < 1; l++)
					{  

						// first point
						float x3 = x_off;
						float y3 = z_off;

						// 2nd point 
						// x' = y*sin q + x*cos q
						// y' = y*cos q - x*sin q
						float x4 = p_xy[l].y * sin(ar) + p_xy[l].x * cos(ar) + x_off;
						float y4 = p_xy[l].y * cos(ar) - p_xy[l].x * sin(ar) + z_off;

						// calc intersect
						bool lli = CalculateTargetLoi_Surface_Line_Edge_Intersect(
								(double)x1, (double)y1, (double)x2, (double)y2,
								(double)x3, (double)y3, (double)x4, (double)y4,
								ix,
								iy
							);

						// chuck "out of bounds" results
						if ( lli )
						{
							if ((ix < rmin_x || ix > rmax_x) || (iy < rmin_y || iy > rmax_y))
							{
								lli = false;
							}
						}

						// result
						if ( lli )
						{
							if ( r_index < 2 )
							{
								r_xy[r_index].x = (float)ix;
								r_xy[r_index].y = (float)iy;
								r_index++;
							}

						}
					}
				}

			// ---- copy
				conf->target_intersect[0] = r_xy[0];
				conf->target_intersect[1] = r_xy[1];
		}



