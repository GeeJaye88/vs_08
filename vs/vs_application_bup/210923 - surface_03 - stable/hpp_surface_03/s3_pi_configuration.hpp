////////////////////////////////////////////////////////////////////////////////

// ---------- s3_pi_configuration.hpp ----------
/*!
\file s3_pi_configuration.hpp
\brief surface_03 display configuration methods
\author Gareth Edwards
*/


// -------- Acquire Atlas Pages ------------------------------------------------

	VOID Surface_03::SetAcquireAtlasPageLid(UINT atlas_page_ref)
	{
		pi_surface_03->_configuration.acquire_atlas_page.ref = atlas_page_ref;
	}

	UINT Surface_03::GetAcquireAtlasPageLid() 
	{
		return pi_surface_03->_configuration.acquire_atlas_page.ref;
	}
		
	VOID  Surface_03::SetAcquireAtlasPageWidth(UINT width)
	{
		pi_surface_03->_configuration.acquire_atlas_page.width = width;
	}

	VOID  Surface_03::SetAcquireAtlasPageHeight(UINT height)
	{
		pi_surface_03->_configuration.acquire_atlas_page.height = height;
	}

	VOID  Surface_03::SetAcquireAtlasPageBytes(UINT stride)
	{
		pi_surface_03->_configuration.acquire_atlas_page.stride = stride;
	}

	UINT  Surface_03::GetAcquireAtlasPageWidth()
	{
		return pi_surface_03->_configuration.acquire_atlas_page.width;
	}

	UINT  Surface_03::GetAcquireAtlasPageHeight()
	{
		return pi_surface_03->_configuration.acquire_atlas_page.height;
	}

	UINT  Surface_03::GetAcquireAtlasPageBytes()
	{
		return pi_surface_03->_configuration.acquire_atlas_page.stride;
	}


// -------- Acquire Dimensions ------------------------------------------------

	VOID Surface_03::SetAcquireSurfaceDimensions(FLOAT width, FLOAT height, FLOAT depth)
	{
		pi_surface_03->_configuration.acquire_dimensions.width  = width;
		pi_surface_03->_configuration.acquire_dimensions.height = height;
		pi_surface_03->_configuration.acquire_dimensions.depth  = depth;
	}

	VOID Surface_03::GetAcquireSurfaceDimensions(FLOAT *width, FLOAT *height, FLOAT *depth)
	{
		*width  = pi_surface_03->_configuration.acquire_dimensions.width;
		*height = pi_surface_03->_configuration.acquire_dimensions.height;
		*depth  = pi_surface_03->_configuration.acquire_dimensions.depth;
	}


// -------- Acquire Light ------------------------------------------------

	VOID  Surface_03::SetAcquireLightParam(
			FLOAT intensity,
			FLOAT distance,
			FLOAT elevation,
			FLOAT azimuth
		)
	{
		pi_surface_03->_configuration.acquire_light_param.intensity = intensity;
		pi_surface_03->_configuration.acquire_light_param.distance  = distance;
		pi_surface_03->_configuration.acquire_light_param.elevation = elevation;
		pi_surface_03->_configuration.acquire_light_param.azimuth   = azimuth;
	}
	FLOAT Surface_03::GetAcquireLightIntensityParam()
	{
		return pi_surface_03->_configuration.acquire_light_param.intensity;
	}
	FLOAT Surface_03::GetAcquireLightDistanceParam()
	{
		return pi_surface_03->_configuration.acquire_light_param.distance;
	}
	FLOAT Surface_03::GetAcquireLightElevationParam()
	{
		return pi_surface_03->_configuration.acquire_light_param.elevation;
	}
	FLOAT Surface_03::GetAcquireLightAzimuthParam()
	{
		return pi_surface_03->_configuration.acquire_light_param.azimuth;
	}

	
// -------- Acquire Light ------------------------------------------------

	INT Surface_03::GetAcquireLight(UINT light_index, vs_system::VsLight **light)
	{
		if ( light_index >= 0 && light_index < 4 )
		{
			*light = &pi_surface_03->_configuration.acquire_light[light_index];
			return VS_OK;
		}
		return VS_ERROR;
	}

	INT Surface_03::SetAcquireLight(UINT light_index, vs_system::VsLight *light)
	{
		if (light_index >= 0 && light_index < 4)
		{
			pi_surface_03->_configuration.acquire_light[light_index] = *light;
			return VS_OK;
		}
		return VS_ERROR;
	}


// -------- Acquire Height Field Param ------------------------------------------------

	VOID Surface_03::SetAcquireHeightfieldParam(
			UINT   stride,
			FLOAT *buffer,
			FLOAT  scalar
		)
	{
		pi_surface_03->_configuration.acquire_height_field_param.stride = stride;
		pi_surface_03->_configuration.acquire_height_field_param.buffer = buffer;
		pi_surface_03->_configuration.acquire_height_field_param.scalar = scalar;
	}


// -------- Acquire Height Field Array Param ------------------------------------------------
	/*
		UINT   pitch     = 0;
		UINT   width     = 0;
		UINT   extent    = 0;
		UINT   offset    = 0;
		UINT   dimension = 0;
		FLOAT *array     = NULL;
	*/

	INT Surface_03::SetAcquireHeightFieldArray(FLOAT * array)
	{
		pi_surface_03->_configuration.aquire_height_field.array = array;
		return VS_OK;
	}

	INT Surface_03::SetAcquireHeightFieldDimension(UINT dimension)
	{
		pi_surface_03->_configuration.aquire_height_field.dimension = dimension;
		return VS_OK;
	}

	INT Surface_03::SetAcquireHeightFieldExtent(UINT extent)
	{
		pi_surface_03->_configuration.aquire_height_field.extent = extent;
		return VS_OK;
	}

	INT Surface_03::SetAcquireHeightFieldOffset(UINT offset)
	{
		pi_surface_03->_configuration.aquire_height_field.offset = offset;
		return VS_OK;
	}

	INT Surface_03::SetAcquireHeightFieldStride(UINT stride)
	{
		pi_surface_03->_configuration.aquire_height_field.stride = stride;
		return VS_OK;
	}

	INT Surface_03::SetAcquireHeightFieldWidth(UINT width)
	{
		pi_surface_03->_configuration.aquire_height_field.width = width;
		return VS_OK;
	}

	FLOAT * Surface_03::GetAcquireHeightFieldArray()
	{
		return pi_surface_03->_configuration.aquire_height_field.array;
	}

	UINT Surface_03::GetAcquireHeightFieldDimension()
	{
		return pi_surface_03->_configuration.aquire_height_field.dimension;
	}
	UINT Surface_03::GetAcquireHeightFieldExtent()
	{
		return pi_surface_03->_configuration.aquire_height_field.extent;
	}

	UINT Surface_03::GetAcquireHeightFieldOffset()
	{
		return pi_surface_03->_configuration.aquire_height_field.offset;
	}

	UINT Surface_03::GetAcquireHeightFieldStride()
	{
		return pi_surface_03->_configuration.aquire_height_field.stride;
	}

	UINT Surface_03::GetAcquireHeightFieldWidth()
	{
		return pi_surface_03->_configuration.aquire_height_field.width;
	}

	VOID  Surface_03::SetAcquireHeightFieldMin(FLOAT min)
	{
		pi_surface_03->_configuration.aquire_height_field.min = min;
	}

	FLOAT Surface_03::GetAcquireHeightFieldMin()
	{
		return pi_surface_03->_configuration.aquire_height_field.min;
	}

	VOID  Surface_03::SetAcquireHeightFieldMax(FLOAT max)
	{
		pi_surface_03->_configuration.aquire_height_field.max = max;
	}

	FLOAT Surface_03::GetAcquireHeightFieldMax()
	{
		return pi_surface_03->_configuration.aquire_height_field.max;
	}

	VOID  Surface_03::SetAcquireHeightFieldScalar(FLOAT scalar)
	{
		pi_surface_03->_configuration.aquire_height_field.scalar = scalar;
	}

	FLOAT Surface_03::GetAcquireHeightFieldScalar()
	{
		return pi_surface_03->_configuration.aquire_height_field.scalar;
	}


// -------- Acquire Image Buffer ------------------------------------------------

	INT Surface_03::GetAcquireImgBuffer(UINT img_bufer_index, vs_system::ImgBuffer **img_buffer)
	{
		if ( img_bufer_index >= 0 && img_bufer_index < pi_surface_03->MAX_ACQ_IMG_BUFFER )
		{
			*img_buffer = &pi_surface_03->_configuration.aquire_img_buffer[img_bufer_index];
			return VS_OK;
		}
		return VS_ERROR;
	}

	INT Surface_03::SetAcquireImgBuffer(UINT img_bufer_index, vs_system::ImgBuffer *img_buffer)
	{
		if ( img_bufer_index >= 0 && img_bufer_index < pi_surface_03->MAX_ACQ_IMG_BUFFER )
		{
			pi_surface_03->_configuration.aquire_img_buffer[img_bufer_index] = *img_buffer;
			return VS_OK;
		}
		return VS_ERROR;
	}

	UINT Surface_03::GetAcquireImgBufferMax()
	{
		return pi_surface_03->MAX_ACQ_IMG_BUFFER;
	}


// -------- Acquire Image Buffer ------------------------------------------------

	INT Surface_03::GetAnalyseImgBuffer(UINT img_bufer_index, vs_system::ImgBuffer **img_buffer)
	{
		if ( img_bufer_index >= 0 && img_bufer_index < pi_surface_03->MAX_ACQ_IMG_BUFFER )
		{
			*img_buffer = &pi_surface_03->_configuration.analyse_img_buffer[img_bufer_index];
			return VS_OK;
		}
		return VS_ERROR;
	}

	INT Surface_03::SetAnalyseImgBuffer(UINT img_bufer_index, vs_system::ImgBuffer *img_buffer)
	{
		if ( img_bufer_index >= 0 && img_bufer_index < pi_surface_03->MAX_ACQ_IMG_BUFFER )
		{
			pi_surface_03->_configuration.analyse_img_buffer[img_bufer_index] = *img_buffer;
			return VS_OK;
		}
		return VS_ERROR;
	}


// -------- Acquire Material ------------------------------------------------

	INT Surface_03::GetAcquireMaterial(vs_system::VsMaterial **material)
	{
		*material = &pi_surface_03->_configuration.acquire_material;
		return VS_OK;
	}

	INT Surface_03::SetAcquireMaterial(vs_system::VsMaterial *material)
	{
		pi_surface_03->_configuration.acquire_material = *material;
		return VS_OK;
	}


// -------- Explore Light ------------------------------------------------

	INT Surface_03::GetExploreLightMax()
	{
		return PI_Surface_03::MAX_EXP_LGT;
	}
		
	INT Surface_03::GetExploreLight(UINT light_index, vs_system::VsLight **light)
	{
		if ( light_index >= 0 && light_index < PI_Surface_03::MAX_EXP_LGT )
		{
			*light = &pi_surface_03->_configuration.explore_light[light_index];
			return VS_OK;
		}
		return VS_ERROR;
	}

	BOOL Surface_03::GetExploreLightEnabled(UINT index)
	{

		return pi_surface_03->_configuration.explore_light_enabled.state[index];
	}

	INT Surface_03::SetExploreLightEnabled(UINT index, BOOL enabled)
	{
		pi_surface_03->_configuration.explore_light_enabled.state[index] = enabled;
		return VS_OK;
	}

	VOID Surface_03::GetExploreLightsEnabled(BOOL light_enabled[])
	{
		for (INT i = 0; i < PI_Surface_03::MAX_EXP_LGT; i++)
		{
			light_enabled[i] = pi_surface_03->_configuration.explore_light_enabled.state[i];
		}
	}

	VOID Surface_03::SetExploreLightsEnabled(BOOL light_enabled[])
	{
		for (INT i = 0; i < PI_Surface_03::MAX_EXP_LGT; i++)
		{
			pi_surface_03->_configuration.explore_light_enabled.state[i] = light_enabled[i];
		}
	}

	BOOL Surface_03::GetExploreLightColouredEnabled()
	{
		return pi_surface_03->_configuration.explore_light_enabled.colour;
	}

	VOID Surface_03::SetExploreLightColouredEnabled(BOOL colour_enabled)
	{
		pi_surface_03->_configuration.explore_light_enabled.colour = colour_enabled;
	}

	BOOL Surface_03::GetExploreLightSpecularEnabled()
	{
		return pi_surface_03->_configuration.explore_light_enabled.specular;
	}

	VOID Surface_03::SetExploreLightSpecularEnabled(BOOL specular_enabled)
	{
		pi_surface_03->_configuration.explore_light_enabled.specular = specular_enabled;
	}


// -------- MESH ------------------------------------------------

	VOID Surface_03::SetMeshDensity(UINT mode)
	{
		UINT rows = 512, cols = 512;
		switch (mode)
		{
			case  1: rows = cols = 256; break;
			case  2: break;
			case  3: rows = cols = 1024; break;
			default: break;
		}
		pi_surface_03->_configuration.mesh.cols = cols;
		pi_surface_03->_configuration.mesh.rows = rows;
	}

	VOID Surface_03::SetMeshSolid(BOOL s)
	{
		pi_surface_03->_configuration.mesh.solid = s ? TRUE : FALSE;
	}

	VOID Surface_03::SetMeshTextured(BOOL t)
	{
		pi_surface_03->_configuration.mesh.textured = t ? TRUE : FALSE;
	}

	UINT Surface_03::GetMeshDisplayMode()
	{
		return pi_surface_03->_configuration.mesh.display;
	}

	VOID Surface_03::SetMeshDisplayMode(UINT d)
	{
		pi_surface_03->_configuration.mesh.display =
			d < MeshDisplayMode::WU ? MeshDisplayMode::WU :
				( d > MeshDisplayMode::ST ? MeshDisplayMode::ST : d);
	}

	UINT Surface_03::GetMeshDensityMode()
	{
		return pi_surface_03->_configuration.mesh.density;
	}

	VOID Surface_03::SetMeshDensityMode(UINT d)
	{
		pi_surface_03->_configuration.mesh.density = 
			d < MeshDensityMode::LOW ? MeshDensityMode::LOW :
				(d > MeshDensityMode::HIGH ? MeshDensityMode::HIGH : d);
	}

	UINT Surface_03::GetMeshMode()
	{
		return pi_surface_03->_configuration.mesh.mode;
	}

	VOID Surface_03::SetMeshMode(UINT surface_mode)
	{
		pi_surface_03->_configuration.mesh.mode = surface_mode;
	}

	UINT Surface_03::GetMeshColumns()
	{
		return pi_surface_03->_configuration.mesh.cols;
	}

	UINT Surface_03::GetMeshRows()
	{
		return pi_surface_03->_configuration.mesh.rows;
	}
		
	BOOL Surface_03::GetMeshSolid()
	{
		return pi_surface_03->_configuration.mesh.solid;
	}
		
	BOOL  Surface_03::GetMeshTextured()
	{
		return pi_surface_03->_configuration.mesh.textured;
	}


// -------- TARGET ------------------------------------------------
	
	VOID Surface_03::InitTargetLoi()
	{
		pi_surface_03->SetupTargetLoi();
	}

	// ---- index
		VOID Surface_03::SetTargetLoiIndex(UINT index)
		{
			pi_surface_03->_configuration.target_loi_index = index;
		}
		UINT Surface_03::GetTargetLoiIndex()
		{
			return pi_surface_03->_configuration.target_loi_index;
		}

	// ---- init
		VOID Surface_03::SetTargetLoiInit(BOOL b)
		{
			PI_Surface_03::Surface_03_Config *conf = &pi_surface_03->_configuration;
			UINT index = conf->target_loi_index;
			pi_surface_03->_configuration.target_init[index] = b;
		}
		BOOL Surface_03::GetTargetLoiInit()
		{
			PI_Surface_03::Surface_03_Config *conf = &pi_surface_03->_configuration;
			UINT index = conf->target_loi_index;
			return pi_surface_03->_configuration.target_init[index];
		}

	// ---- location
		VsVector3 * Surface_03::GetTargetLoi()
		{
			PI_Surface_03::Surface_03_Config *conf = &pi_surface_03->_configuration;
			UINT index = conf->target_loi_index;
			return &conf->target_loi[index];
		}
		VsVector3 * Surface_03::GetTargetLoi(UINT index)
		{
			return &pi_surface_03->_configuration.target_loi[index];
		}


	// ---- rotation
		FLOAT Surface_03::GetTargetLoiRotation(UINT index)
		{
			return pi_surface_03->_configuration.target_rot[index];
		}


	// ---- ruler
		VOID Surface_03::SetTargetLoiRulerVisible(BOOL target_ruler_visible)
		{
			pi_surface_03->_configuration.target_ruler_visible = target_ruler_visible;
			pi_surface_03->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_RECT_IDX, target_ruler_visible);
			pi_surface_03->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_RULE_IDX, target_ruler_visible);
			ObjectLoiRect_HideAll(this);
			ObjectLoiRule_HideAll(this);

		}
		BOOL Surface_03::GetTargetLoiRulerVisible()
		{
			return pi_surface_03->_configuration.target_ruler_visible;
		}


	// ---- section
		VOID Surface_03::SetTargetLoiSectionVisible(BOOL target_section_visible)
		{
			pi_surface_03->_configuration.target_section_visible = target_section_visible;
			pi_surface_03->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_SECT_IDX, target_section_visible);
			ObjectLoiSect_HideAll(this);
		}
		BOOL Surface_03::GetTargetLoiSectionVisible()
		{
			return pi_surface_03->_configuration.target_section_visible;
		}


	// ---- transform
		VOID Surface_03::SetTargetLoiTransform(BOOL target_transform)
		{
			pi_surface_03->_configuration.target_transform = target_transform;
		}
		BOOL Surface_03::GetTargetLoiTransform()
		{
			return pi_surface_03->_configuration.target_transform;
		}


	// ---- visible
		VOID Surface_03::SetTargetLoiVisible(BOOL target_visible)
		{
			pi_surface_03->_configuration.target_visible = target_visible;
			pi_surface_03->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_SPOT_IDX,target_visible);
			pi_surface_03->SetObjectVisible(PI_Surface_03::GOB_EXP_LOI_AXIS_IDX, target_visible);
		}
		BOOL Surface_03::GetTargetLoiVisible()
		{
			return pi_surface_03->_configuration.target_visible;
		}


	// --- substantive get & set

		FLOAT Surface_03::GetTargetLoiLocationHeight(UINT index)
		{

			// ---- local
				PI_Surface_03::Surface_03_Config *conf = &pi_surface_03->_configuration;

			// ---- target
				FLOAT x = conf->target_loi[index].x;
				FLOAT y = conf->target_loi[index].y;
				FLOAT z = conf->target_loi[index].z;

			// ---- aquire dimensions
				FLOAT  width  = conf->acquire_dimensions.width;
				FLOAT  depth = conf->acquire_dimensions.depth;

			// ---- aquire heightfield
				UINT   stride    = conf->aquire_height_field.stride;
				UINT   hf_width  = conf->aquire_height_field.width;
				UINT   extent    = conf->aquire_height_field.extent;
				//UINT   offset    = conf->aquire_height_field.offset;
				//UINT   dimension = conf->aquire_height_field.dimension;
				FLOAT *array     = conf->aquire_height_field.array;

			// ---- no data
				if ( extent == 0 ) return 0;

			// ---- note: no weighted sampling

			// ---- offset XZ to +XZ
				FLOAT z_plus = (z + depth / 2) / depth;
				FLOAT x_plus = (x + width / 2) / width;

			// ---- so, assuming legal x & z ordinates
				//FLOAT z_ok = (z + depth / 2) / depth;
				//FLOAT x_ok = (x + width / 2) / width;

				INT row = (INT)(z_plus * (FLOAT)hf_width);
					row = hf_width - row;
				INT col = (INT)(x_plus * (FLOAT)hf_width);

			// ---- Y!
				UINT sample_offset = row * stride + col;
				sample_offset = sample_offset >= extent ? extent - 1 : sample_offset;
				FLOAT height = *(array + sample_offset) * GetTargetLoiHeightScalar();

			return height;
		}


		INT Surface_03::GetTargetLoiSectionSamples(
				DOUBLE  x1, DOUBLE z1, // input  - start
				DOUBLE  x2, DOUBLE z2, //        - end
				UINT    num_samples,   //        - number of mesh samples
				FLOAT  *y_samples      // output - y height
			)
		{

			// ---- local
				PI_Surface_03::Surface_03_Config *conf = &pi_surface_03->_configuration;

			// ---- aquire dimensions
				FLOAT  width = conf->acquire_dimensions.width;
				FLOAT  depth = conf->acquire_dimensions.depth;

			// ---- aquire heightfield
				UINT   stride    = conf->aquire_height_field.stride;
				UINT   hf_width  = conf->aquire_height_field.width;
				UINT   extent    = conf->aquire_height_field.extent;
				//UINT   offset    = conf->aquire_height_field.offset;
				//UINT   dimension = conf->aquire_height_field.dimension;
				FLOAT *array     = conf->aquire_height_field.array;

			// ---- no data
				if ( extent == 0 ) return 0;

			// ---- note: no weighted sampling

			// ---- local vars
				UINT   sample, sample_offset;
				DOUBLE rmu;
				FLOAT  x, z;
				FLOAT  z_plus, x_plus;
				INT    row, col;

			// ---- sample
				for (sample=0; sample<num_samples; sample++)
				{

					// ---- interpolate XZ
						rmu = (FLOAT)sample / (FLOAT)(num_samples-1);

					// ---- nudge start/end to prevent Edge Error Condition (EEC)
						rmu = rmu * 0.99f + 0.005f;
						x = (FLOAT)( (1 - rmu)*x1 + rmu * x2 );
						z = (FLOAT)( (1 - rmu)*z1 + rmu * z2 );

					// ---- offset XZ to +XZ
						z_plus = (z + depth / 2) / depth;
						x_plus = (x + width / 2) / width;

					// ---- EEC
						//z_plus = z_plus < 0 ? 0 : (z_plus > depth ? depth : z_plus);
						//x_plus = x_plus < 0 ? 0 : (x_plus > width ? width : x_plus);

					// ---- convert +XZ to row col
						row = (INT)(z_plus * (FLOAT)hf_width);
						row = hf_width - row;
			/* -> */	row = row > hf_width - 1 ? hf_width - 1 : (row < 0 ? 0 : row);
						col = (INT)(x_plus * (FLOAT)hf_width);
			/* -> */	col = col > hf_width - 1 ? hf_width - 1 : (col < 0 ? 0 : col);

					// ---- acuaire height array offset
						sample_offset = row * stride + col;

					// ---- using EEC - (should be) no need to catch extent error
						//if (sample_offset > extent - 1 || sample_offset < 0)
						//{
						//	float fred = 2;
						//}
						//sample_offset = sample_offset > extent ? extent - 1 : (sample_offset < 0 ? 0 : sample_offset);
						
					// ---- get Y sample value
						*(y_samples+sample) = *(array + sample_offset) * GetTargetLoiHeightScalar();
				}

			return VS_OK;
		}


		FLOAT Surface_03::GetTargetLoiHeightScalar()
		{

			INT surface_mode = GetMeshMode();
			FLOAT scalar = 1;
			switch ( surface_mode )
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


		BOOL Surface_03::SetTargetLoiIncr(FLOAT x, FLOAT y, FLOAT z, BOOL persistant)
		{

			// ----- note:
			/*

					lazy programming - the x and z logic should be seperate, only
					used if it is an x OR z increment.... thus the returned flip
					direction logic is a tad dodgy - but works....
			*/

			// ---- local
				PI_Surface_03::Surface_03_Config *conf = &pi_surface_03->_configuration;
				UINT index = conf->target_loi_index;
				BOOL  flip_dir = FALSE;

			// ---- translate
				FLOAT w_max = conf->acquire_dimensions.width / 2;
				FLOAT w_min = -w_max;
				FLOAT x_loc = conf->target_loi[index].x + x;
				if ( persistant )
				{
					if ( x_loc < w_min )
					{
						float x_dif = x_loc - w_min;
						x_loc = w_min - x_dif;
						flip_dir = TRUE;
					}
					else if ( x_loc > w_max )
					{
						float x_dif = x_loc - w_max;
						x_loc = w_max - x_dif;
						flip_dir = TRUE;
					}
				}
				else
				{
					x_loc = x_loc < w_min ? w_min : (x_loc > w_max ? w_max : x_loc);
				}

				FLOAT d_max = conf->acquire_dimensions.depth / 2;
				FLOAT d_min = -d_max;
				FLOAT z_loc = conf->target_loi[index].z + z;
				if ( persistant )
				{
					if ( z_loc < d_min )
					{
						float z_dif = z_loc - d_min;
						z_loc = d_min - z_dif;
						flip_dir = TRUE;
					}
					else if ( z_loc > d_max )
					{
						float z_dif = z_loc - d_max;
						z_loc = d_max - z_dif;
						flip_dir = TRUE;
					}
				}
				else
				{
					z_loc = z_loc < d_min ? d_min : (z_loc > d_max ? d_max : z_loc);
				}

				conf->target_loi[index].x = x_loc;
				conf->target_loi[index].z = z_loc;

			// ---- rotate
				conf->target_rot[index] += y;


			// ---- flip
				return flip_dir;

		}
	

		// ---- calculate intersection of two lines.
		//   - return true if found, false if not found or error

		inline double Det(double a, double b, double c, double d)
		{
			return a * d - b * c;
		}

		BOOL Surface_03::SetTargetLoiLineLineIntersect(
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
			if( denom == 0.0 ) // lines don't seem to cross - beware tolerance!
			{
				ixOut = NAN;
				iyOut = NAN;
				return false;
			}

			ixOut = xnom / denom;	
			iyOut = ynom / denom;
			if( !isfinite(ixOut) || ! isfinite(iyOut) ) // probably a numerical issue
				return false;

			return true; //All OK
		}


		VOID Surface_03::SetTargetLoiLineRectIntersect()
		{
			// ---- local
				PI_Surface_03::Surface_03_Config *conf = &pi_surface_03->_configuration;
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

				punt s_xy[5] = // outline of reect
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


			// ---- adjust rect by a tolerance - avoids missing a corner...
				float tolr = 0.0001f;
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
						bool lli = SetTargetLoiLineLineIntersect(
								(double)x1, (double)y1, (double)x2, (double)y2,
								(double)x3, (double)y3, (double)x4, (double)y4,
								ix,
								iy
							);

						// chuck "out of bounds" results
						if ( lli )
						{
							if ( ( ix < rmin_x || ix > rmax_x ) || ( iy < rmin_y || iy > rmax_y ) )
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

			// ---- copy into configuration info
				conf->target_intersect[0] = r_xy[0];
				conf->target_intersect[1] = r_xy[1];
		}



