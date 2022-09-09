////////////////////////////////////////////////////////////////////////////////

// ---------- s3_model_img_enhance.hpp ----------
/*!
\file s3_model_img_enhance.hpp
\brief Implementation of the Camera methods
\author Gareth Edwards
*/


// ---------- AcquireSetup ----------
/*!
\brief setup acquisition buffers
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::AcquireSetup()
{

	// ---- local
		INT result = VS_OK;
		vs_system::GfxInterface *gfx = s_pi_model->app_dependency->GetGfxInterface();
		model::Surface_03 *surface_03 = s_pi_model->surface_03;


	// ---- destination atlas page handle ?
		UINT page_handle = gfx->GetAtlasPageHandle(surface_03->GetAcquireAtlasPageLid());
		if ( page_handle != VS_ERROR )
		{

			// ---- destination atlas page lock ?
				UCHAR *page_buffer = gfx->PageLock(page_handle);
				if ( page_buffer != NULL )
				{

					// ---- dimensions
						UINT width = 0, height = 0;
						gfx->GetAtlasPageTextureDimensions(surface_03->GetAcquireAtlasPageLid(), &width, &height);

					// ---- initialise image buffers
						UINT img_buffer_max = surface_03->GetAcquireImgBufferMax();
						for (UINT i = 0; i < img_buffer_max; i++)
						{
							// ---- acquisition
								vs_system::ImgBuffer *img_buffer = NULL;
								surface_03->GetAcquireImgBuffer(i, &img_buffer);
								img_buffer->Initialise(width,height,4);
								img_buffer->CreateBuffer();

							// ---- analysis
								surface_03->GetAnalyseImgBuffer(i, &img_buffer);
								img_buffer->Initialise(width, height, 4);
								img_buffer->CreateBuffer();
						}

					// ---- initialise heightfield buffer array
						UINT dimension = width + 1;
						surface_03->SetAcquireHeightFieldDimension(dimension);
						FLOAT *array = new FLOAT[dimension * dimension];
						surface_03->SetAcquireHeightFieldArray(array);
						
				}

			// ---- destination atlas page unlock
				INT result = gfx->PageUnlock(page_handle);

		}

	return VS_OK;
}


// ---------- AcquireCleanup ----------
/*!
\brief cleanup acquisition buffers
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::AcquireCleanup()
{

	// ---- local
		INT result = VS_OK;
		vs_system::GfxInterface *gfx = s_pi_model->app_dependency->GetGfxInterface();
		model::Surface_03 *surface_03 = s_pi_model->surface_03;


	// ---- delete image buffers
		UINT img_buffer_max = surface_03->GetAcquireImgBufferMax();
		for (UINT i = 0; i < img_buffer_max; i++)
		{
			// ---- acquisition
				vs_system::ImgBuffer *img_buffer = NULL;
				surface_03->GetAcquireImgBuffer(i, &img_buffer);
				img_buffer->DestroyBuffer();
				delete img_buffer;

			// ---- analysis
				surface_03->GetAnalyseImgBuffer(i, &img_buffer);
				img_buffer->DestroyBuffer();
				delete img_buffer;
		}


	// ---- delete heightfield buffer array
		FLOAT *array = surface_03->GetAcquireHeightFieldArray();
		delete[] array;


	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- AcquireDemoSetLights ----------
/*!
\brief set acquisition demo lights
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::AcquireDemoSetLights()
{
	
	// ---- local
		INT result = 0;
		model::Surface_03 *surface_03 = s_pi_model->surface_03;


	// ---- get aquire param
		INT current_param_group = surface_03->parameter.GetParamGroup();
		surface_03->parameter.SetParamGroup(PARAM_GROUP_ACQUIRE);
		INT acquire_param_group_index = surface_03->parameter.GetParamGroupIndex();
		INT acquire_param_group_size  = surface_03->parameter.GetParamGroupSize();
		INT acquire_param_array_index = surface_03->parameter.GetParamArrayIndex();
		INT acquire_param_array_size  = surface_03->parameter.GetParamArraySize();
		FLOAT *acquire_param_array    = new FLOAT[acquire_param_array_size];


	// ---- get aquire light parameters
		result = surface_03->parameter.GetParamArray(0, 9, acquire_param_array);

	// ---- store
		surface_03->SetAcquireLightParam(
				*(acquire_param_array + 1),
				*(acquire_param_array + 2),
				*(acquire_param_array + 3),
				*(acquire_param_array + 4)
			);


	// ---- get ambient light intensity
		result = surface_03->parameter.GetParamArray(1, 9, acquire_param_array);
		FLOAT intensity = *(acquire_param_array + 1);
		VsColour light_ambient = { intensity, intensity, intensity, 0.0f };


	// ---- set diffuse light
		VsColour light_diffuse = { 1, 1, 1, 0 };


	// ---- calc lights
		for (UINT l=0; l<4; l++)
		{

			// ---- angle stuff
				FLOAT t2r = surface_03->GetAcquireLightElevationParam() * (FLOAT)VS_RADIANS_PER_DEGREE;
				FLOAT phi = surface_03->GetAcquireLightAzimuthParam() + (FLOAT)l * 90.0f + 90.0f;
				FLOAT p2r = phi * (FLOAT)VS_RADIANS_PER_DEGREE;

			// ---- rotate about x axis
				// y' = y*cos q - z*sin q
				// z' = y*sin q + z*cos q
				FLOAT xt =  0;
				FLOAT yt =  surface_03->GetAcquireLightDistanceParam() * sinf(t2r);
				FLOAT zt = -surface_03->GetAcquireLightDistanceParam() * cosf(t2r);

			// ---- rotate about y axis
				// z' = z*cos q - x*sin q
				// x' = z*sin q + x*cos q
				FLOAT zl = zt * cos(p2r) - xt * sin(p2r);
				FLOAT xl = zt * sin(p2r) + xt * cos(p2r);

			// ---- set
				VsVector3 acquire_light_position = { xl, yt, zl };
				vs_system::VsLight *light;
				surface_03->GetAcquireLight(l, &light);
				result = light->SetPosition(acquire_light_position);
				result = light->SetAmbient(light_ambient);
				result = light->SetDiffuse(light_diffuse);
		}


	// ---- reset
		surface_03->parameter.SetParamGroup(current_param_group);


	// ---- cleanup
		delete[] acquire_param_array;

	return VS_OK;
}


// ---------- AcquireDemoSetMaterials ----------
/*!
\brief set acquisition demo materials
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::AcquireDemoSetMaterials()
{

	// ---- local
		INT result = 0;
		model::Surface_03 *surface_03 = s_pi_model->surface_03;

	// ---- get acquire material
		vs_system::VsMaterial *material = NULL;
		surface_03->GetAcquireMaterial(&material);
		material->diffuse  = { 1, 1, 1, 0 };
		material->specular = { 0, 0, 0, 0 };

	return VS_OK;
}


// ---------- AcquireDemoCalculateLuminaFields ----------
/*!
\brief calculate acqusition demo lumina fields
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::AcquireDemoCalculateLuminaFields()
{

	// ---- auto shader ( psuedo inline )
		auto shader = [](
				FLOAT                  intensity,  // [in]
				vs_system::VsLight    *light,      // [in]
				vs_system::VsMaterial *mtl,        // [in]
				FLOAT                  position[], // [in] 
				FLOAT                  normal[],   // [in]
				VsColour              *colour      // [out] 
			)

		{

			// ---- ambient
				float red   = light->ambient.red;

			// ---- distance
				float xd = light->position.x - position[0];
				float yd = light->position.y - position[1];
				float zd = light->position.z - position[2];
				float dist = sqrt(xd * xd + yd * yd + zd * zd);

			// ---- incident ?
				if ( dist > 0.0f )
				{
					float attenuation = intensity * 1.0f  / (dist * dist);
					float dva = 0.0;
					xd = xd * attenuation; dva += normal[0] * xd;
					yd = yd * attenuation; dva += normal[1] * yd;
					zd = zd * attenuation; dva += normal[2] * zd;
					if ( dva > 0.00001f )
					{
						dva = dva > 1 ? 1 : dva;
						red += mtl->diffuse.red  * dva * light->diffuse.red;
					}
				}

			// ---- clamp
				colour->red = red < 0 ? 0 : (red > 1 ? 1 : red  );

		};


	// ---- local
		INT result = 0;
		model::Surface_03 *surface_03 = s_pi_model->surface_03;


	// ---- calculate acquisition demo destination (y) heightfield array
		result = AcquireDemoCalculateHeightfield();


	// ---- get y heightfield parameters
		UINT y_array_pitch  = surface_03->GetAcquireHeightFieldStride();
		UINT y_array_width  = surface_03->GetAcquireHeightFieldWidth();
		UINT y_array_extent = surface_03->GetAcquireHeightFieldExtent();
		UINT y_array_offset = surface_03->GetAcquireHeightFieldOffset();


	// ---- get acquisition dimensions
		FLOAT acquire_width, acquire_height, acquire_depth;
		surface_03->GetAcquireSurfaceDimensions(
				&acquire_width,
				&acquire_height,
				&acquire_depth
			);


	// ---- shortcut initialised y surface param
		UINT atlas_page_width  = surface_03->GetAcquireAtlasPageWidth();
		UINT atlas_page_height = surface_03->GetAcquireAtlasPageHeight();
		UINT atlas_page_stride = surface_03->GetAcquireAtlasPageBytes();


	// ---- shortcut initialised y array
		FLOAT *y_array = surface_03->GetAcquireHeightFieldArray();
		UINT   y_array_dimension = surface_03->GetAcquireHeightFieldDimension();


	// ---- calc xyz normals ( stride = 3 )
		UINT   norm_array_pitch  = y_array_width * 3;
		UINT   norm_array_extent = norm_array_pitch * y_array_width;
		UINT   norm_array_offset = 0;
		FLOAT *norm_array        = new FLOAT[norm_array_extent];
		result = vs_system::ObjConfig::Mesh_Normals( // from s3::AcquireCalculateDemoLuminafields
				y_array,
				y_array_width,
				norm_array,
				acquire_width,
				acquire_depth
			);


	// ---- calc surface xYz ( stride = 3 ) ( Y from y_array )
		UINT   surf_array_pitch  = y_array_width  * 3;
		UINT   surf_array_extent = surf_array_pitch * y_array_width;
		UINT   surf_array_offset = 0;
		FLOAT *surf_array        = new FLOAT[y_array_extent * 3];
		FLOAT  x_inc = acquire_width / (FLOAT)atlas_page_width;
		FLOAT  z_inc = acquire_width / (FLOAT)atlas_page_width;
		FLOAT  x_by2 = acquire_width / 2;
		FLOAT  z_by2 = acquire_width / 2;
		FLOAT  y_min =  99999; // to big to fail!?
		FLOAT  y_max = -y_min;
		for (UINT r = 0; r < atlas_page_width; r++)
		{
			FLOAT z = -(FLOAT)r * z_inc + z_by2;
			y_array_offset = r * y_array_pitch;
			surf_array_offset = r * surf_array_pitch;
			for (UINT c = 0; c < atlas_page_width; c++)
			{
				FLOAT  y = *(y_array + y_array_offset + c);
				y_min = y_min < y ? y_min : y;
				y_max = y_max > y ? y_max : y;

				FLOAT *s = surf_array + surf_array_offset + c * 3;
				*(s    ) = (FLOAT)c * x_inc - x_by2;
				*(s + 1) = y;
				*(s + 2) = z;
			}
		}

		surface_03->SetAcquireHeightFieldMin(y_min);
		surface_03->SetAcquireHeightFieldMax(y_max);


	// ---- initialise lights
		result = AcquireDemoSetLights();


	// ---- initialise materials
		result = AcquireDemoSetMaterials();


	// ---- calc lumina fields
		//
		// note: shade FLOAT y fractal to BYTE y height map
		//
		UINT  byte_array_pitch  = atlas_page_width * 4;
		UINT  byte_array_extent = byte_array_pitch * atlas_page_width;
		UINT  byte_array_offset = 0;
		BYTE *byte_array        = new BYTE[byte_array_extent];
		UINT  byte_col_offset   = 0;


	// ---- shader 
		VsColour colour = { 0, 0, 0, 0 };
		vs_system::VsMaterial *material = NULL;
		surface_03->GetAcquireMaterial(&material);


	// ---- calculate lumina fields
		for (UINT lumina_field = 0; lumina_field <= 4; lumina_field++)
		{

			// ---- 3d surface ?
				if ( lumina_field == 4 )
				{
					BYTE red = (BYTE)(material->diffuse.red * 255);
					for (UINT r = 0; r < atlas_page_width; r++)
					{
						byte_array_offset = r * byte_array_pitch;
						for (UINT c = 0; c < atlas_page_width; c++)
						{
							byte_col_offset = byte_array_offset + c * 4;
							*(byte_array + byte_col_offset + 0) = 255;
							*(byte_array + byte_col_offset + 1) = 255;
							*(byte_array + byte_col_offset + 2) = 255;;
							*(byte_array + byte_col_offset + 3) = 0;
						}
					}
				}
				else
				{
					vs_system::VsLight *light = NULL;
					result = surface_03->GetAcquireLight(lumina_field > 3 ? 3 : lumina_field, &light);

					for (UINT r = 0; r < atlas_page_width; r++)
					{
						surf_array_offset = r * surf_array_pitch;
						norm_array_offset = r * norm_array_pitch;
						byte_array_offset = r * byte_array_pitch;
						for (UINT c = 0; c < atlas_page_width; c++)
						{

							// ---- basic shader
								shader(
										surface_03->GetAcquireLightIntensityParam(), // [in] light intensity
										light,                                       // [in] light
										material,                                    // [in] material
										(surf_array + surf_array_offset + c * 3),    // [in] surface
										(norm_array + norm_array_offset + c * 3),    // [in] normal
										&colour                                      // [out] result
									);

							// ---- monochrome
								byte_col_offset = byte_array_offset + c * 4;
								BYTE red = (BYTE)(colour.red * 255);
								*(byte_array + byte_col_offset + 0) = red;
								*(byte_array + byte_col_offset + 1) = red;
								*(byte_array + byte_col_offset + 2) = red;
								*(byte_array + byte_col_offset + 3) = 0;

						}
					}
				}


			// ---- store
				INT time_stamp[10];
				result = AcquireStoreLuminaField(
								lumina_field,
								atlas_page_width,
								atlas_page_height,
								atlas_page_stride,
								byte_array,
								time_stamp
							);

		}


	// ---- cleanup (in reverse order)
		delete[] byte_array;
		delete[] surf_array;
		delete[] norm_array;

	return VS_OK;
}


// ---------- AcquireDemoCalculateHeightfield ----------
/*!
\brief calculate acquisition demo destination y heightfield array
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::AcquireDemoCalculateHeightfield()
{
	
	// ---- local
		INT result = 0;
		model::Surface_03 *surface_03 = s_pi_model->surface_03;
		UINT mesh_mode = surface_03->GetMeshMode();


	// ---- get acquisition destination dimensions
		FLOAT dest_width, acquire_height, acquire_depth;
		surface_03->GetAcquireSurfaceDimensions(
				&dest_width,
				&acquire_height,
				&acquire_depth
			);


	// ---- get & then set acquisition atlas page values
		UINT atlas_page_width, atlas_page_height, atlas_page_stride;
		AcquireGetImageDimensions(
				&atlas_page_width,
				&atlas_page_height,
				&atlas_page_stride
			);
		surface_03->SetAcquireAtlasPageWidth(atlas_page_width);
		surface_03->SetAcquireAtlasPageHeight(atlas_page_height);
		surface_03->SetAcquireAtlasPageBytes(atlas_page_stride);


	// ---- image buffer
		UINT   dest_stride = atlas_page_width;


	// ---- y height field array ( stride = 1 & # width = mesh rows + 1 for subdivsion ) 
		UINT   dest_y_array_pitch  = dest_stride + 1;
		UINT   dest_y_array_width  = dest_y_array_pitch;
		UINT   dest_y_array_extent = dest_y_array_pitch * dest_y_array_pitch;
		UINT   dest_y_array_offset = 0;

		FLOAT *dest_y_array = surface_03->GetAcquireHeightFieldArray();
		UINT   dest_y_array_dimension = surface_03->GetAcquireHeightFieldDimension();


	// ---- get param group
		INT current_param_group = surface_03->parameter.GetParamGroup();


	// ---- process y
		switch ( mesh_mode )
		{

			case vs_system::ObjConfig::MESH_VERSION_CAMERA:
				{
					vs_system::Heightfield heightfield;
					heightfield.Flatfield2D(
						dest_y_array,
						dest_stride + 1
					);
				}
				break;

			case vs_system::ObjConfig::MESH_VERSION_TERRAIN:
				{
					UCHAR *terrain_bump_map = NULL;
					INT terrain_bump_map_extent = 0;
					surface_03->GetTerainBumpMapData(
							&terrain_bump_map,
							&terrain_bump_map_extent
						);

					vs_system::Heightfield heightfield;
					heightfield.Heightfield2D(
							dest_y_array,
							dest_stride + 1,
							dest_width,
							terrain_bump_map,
							terrain_bump_map_extent
						);
				}
				break;

			case vs_system::ObjConfig::MESH_VERSION_RR:
				{
					UCHAR *rr_bump_map = NULL;
					INT rr_bump_map_extent = 0;
					surface_03->GetRRBumpMapData(
							&rr_bump_map,
							&rr_bump_map_extent
						);

					vs_system::Heightfield heightfield;
					heightfield.Heightfield2D(
							dest_y_array,
							dest_stride + 1,
							dest_width	,
							rr_bump_map,
							rr_bump_map_extent
						);
				}
				break;

			case vs_system::ObjConfig::MESH_VERSION_FUNCTION:
				{

					// ---- has NO physical form - transintory - created on the fly ....
					

					// ---- get param
						surface_03->parameter.SetParamGroup(PARAM_GROUP_FUNCTION);
						INT function_param_group_index = surface_03->parameter.GetParamGroupIndex();
						INT function_param_array_index = surface_03->parameter.GetParamArrayIndex();
						INT function_param_array_size  = surface_03->parameter.GetParamArraySize();
						FLOAT *function_param_array    = new FLOAT[function_param_array_size];
						result = surface_03->parameter.GetParamArray(function_param_group_index, 9, function_param_array);
						 

					 // ---- fractal function parameters
						INT     random_seed      = (INT)*(function_param_array+1);
						FLOAT   height_scalar    = *(function_param_array + 2);
						FLOAT   height_roughness = *(function_param_array + 3);
						FLOAT   filter_scalar    = *(function_param_array + 4);
						FLOAT   y_clip_percent   = *(function_param_array + 5);


					// ---- height proportional to size of surface 
						FLOAT   width = 100; // mm, set in mesh param
						height_scalar *= width;


					// ---- calc y fractal function
						vs_system::Fractal fractal;
						result = fractal.Fractal2D(
								dest_y_array,		// extent (dest_stride+1) x (dest_stride+1)
								dest_stride,		// power of 2 (already -1 for subdivsion)
								random_seed,
								height_scalar,
								height_roughness
							);


					// ---- y post process
						float filter_array[9] =
						{
							0.0625, 0.1250, 0.0625,
							0.1250, 0.2500, 0.1250,
							0.0625, 0.1250, 0.0625
						};
						result = vs_system::ObjConfig::Mesh_Filter(
								dest_y_array,
								dest_y_array_width,
								filter_array,
								9,
								filter_scalar
							);
						result = vs_system::ObjConfig::Mesh_YClip(
								dest_y_array,
								dest_stride,
								0,
								y_clip_percent * height_scalar
							);

				}
				break;
		}


	// ---- scale
		/*
		FLOAT y_scalar = surface_03->parameter.GetAcquireHeightfieldScalar();
		result = vs_system::ObjConfig::Mesh_YScalar(
				y_array,
				y_array_width,
				y_scalar
			);
		*/


	// ---- set destination y heightfield parameters
		surface_03->SetAcquireHeightFieldStride(dest_y_array_pitch);
		surface_03->SetAcquireHeightFieldWidth(dest_y_array_width);
		surface_03->SetAcquireHeightFieldExtent(dest_y_array_extent);
		surface_03->SetAcquireHeightFieldOffset(dest_y_array_offset);


	// ---- restore
		surface_03->parameter.SetParamGroup(current_param_group);


	// ---- write data
	/*
		std::ofstream myfile("surf.csv");
		if (myfile.is_open())
		{
			for (UINT row = 0; row < 64; row++)
			{
				UINT row_offset = row * y_array_pitch;
				for (UINT col = 0; col < 64; col++)
				{
					UINT col_offset = row_offset + col;
					CHAR msg[64];
					sprintf(msg, "%6.3f", *(y_array + col_offset));
					myfile << msg;
					myfile << ",";
				}
				myfile << "\n";
			}

			myfile.close();
		}
	*/

	return VS_OK;
}


// ---------- AcquireAnalyseLuminaFields ----------
/*!
\brief analyse acquisition lumina fields
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::AcquireAnalyseLuminaFields()
{

	// ---- lambda
		auto float_to_byte = [](FLOAT f)
		{
			f = f < 0 ? 0 : (f > 1 ? 1 : f);
			BYTE b = (BYTE)(f * 255);
			return b;
		};


	// ---- local
		INT result = VS_OK;
		model::Surface_03 *surface_03 = s_pi_model->surface_03;


	// ---- get aquire param
		INT current_param_group = surface_03->parameter.GetParamGroup();
		surface_03->parameter.SetParamGroup(PARAM_GROUP_ACQUIRE);
		INT acquire_param_group_index = surface_03->parameter.GetParamGroupIndex();
		INT acquire_param_group_size  = surface_03->parameter.GetParamGroupSize();
		INT acquire_param_array_index = surface_03->parameter.GetParamArrayIndex();
		INT acquire_param_array_size  = surface_03->parameter.GetParamArraySize();
		FLOAT *acquire_param_array = new FLOAT[acquire_param_array_size];


	// ---- get min "resolvalibity"  value parameters
		result = surface_03->parameter.GetParamArray(2, 9, acquire_param_array);
		BYTE min_value = float_to_byte(*(acquire_param_array + 1));
		BYTE min_red   = float_to_byte(*(acquire_param_array + 2));
		BYTE min_green = float_to_byte(*(acquire_param_array + 3));
		BYTE min_blue  = float_to_byte(*(acquire_param_array + 4));


	// ---- get max "resolvalibity" value parameters
		result = surface_03->parameter.GetParamArray(3, 9, acquire_param_array);
		BYTE max_value = float_to_byte(*(acquire_param_array + 1));
		BYTE max_red   = float_to_byte(*(acquire_param_array + 2));
		BYTE max_green = float_to_byte(*(acquire_param_array + 3));
		BYTE max_blue  = float_to_byte(*(acquire_param_array + 4));


	// ---- shortcut initialised image light field acqusition buffers
		UINT img_width = surface_03->GetAcquireAtlasPageWidth();
		UINT img_height = surface_03->GetAcquireAtlasPageHeight();
		UINT img_bytes  = surface_03->GetAcquireAtlasPageBytes();


	// ---- pre store all image buffers
		UINT img_buffer_max = surface_03->GetAcquireImgBufferMax();
		BYTE *aquire_img_buffer[5]; // TBD - 5 is a cheat, may not future proof
		BYTE *analyse_img_buffer[5];
		for (UINT i = 0; i < img_buffer_max; i++)
		{

			// ---- acquire
				vs_system::ImgBuffer *img_buffer = NULL;
				surface_03->GetAcquireImgBuffer(i, &img_buffer);
				aquire_img_buffer[i] = img_buffer->GetBuffer();

			// ---- analysis
				surface_03->GetAnalyseImgBuffer(i, &img_buffer);
				analyse_img_buffer[i] = img_buffer->GetBuffer();
		}


	// ---- rows
		UINT row_pitch = img_width * img_bytes;
		for (UINT row = 0; row < img_height; row++)
		{

			// ---- columns
				UINT row_offset = row_pitch * row;
				for (UINT col = 0; col < img_width; col++)
				{

					// ---- test individual "too dark" or "too light"
						UINT col_offset   = row_offset + img_bytes * col;

					// ---- unusable ?
						UINT too_dark = 0;
						UINT too_light = 0;
						for (UINT i = 0; i < img_buffer_max; i++)
						{

							if ( i != img_buffer_max-1 )
							{

								BYTE src_value = *(aquire_img_buffer[i] + col_offset);
								BYTE *b = analyse_img_buffer[i] + col_offset;

								if ( src_value < min_value )
								{
									too_dark++;
									*(b + 0) = min_blue;
									*(b + 1) = min_green;
									*(b + 2) = min_red;
								}
								else if ( src_value > max_value )
								{
									too_light++;
									*(b + 0) = max_blue;
									*(b + 1) = max_green;
									*(b + 2) = max_red;
								}
								else
								{
									BYTE *f = aquire_img_buffer[i] + col_offset;
									*(b + 0) = *(f + 0);
									*(b + 1) = *(f + 1);
									*(b + 2) = *(f + 2);
								}

							}
							else
							{

								BYTE *b = analyse_img_buffer[i] + col_offset;
								if ( too_dark > 2 )
								{
									*(b + 0) = min_blue;
									*(b + 1) = min_green;
									*(b + 2) = min_red;
								}
								else if( too_light > 2 )
								{
									*(b + 0) = max_blue;
									*(b + 1) = max_green;
									*(b + 2) = max_red;
								}
								else
								{
									BYTE *f = aquire_img_buffer[i] + col_offset;
									*(b + 0) = *(f + 0);
									*(b + 1) = *(f + 1);
									*(b + 2) = *(f + 2);
								}

							}
						}

				}

		}

	// ---- reset
		surface_03->parameter.SetParamGroup(current_param_group);

	// ---- cleanup
		delete[] acquire_param_array;

	return VS_OK;
}


// ---------- AcquireFlatHeightField ----------
/*!
\brief init flat (y == 0) acqusition height field
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::AcquireFlatHeightField()
{

	// ---- local
		INT result = 0;
		model::Surface_03 *surface_03 = s_pi_model->surface_03;

	// ---- get heighfield stuff
		FLOAT *dest_y_array = surface_03->GetAcquireHeightFieldArray();
		UINT   dest_y_array_extent = surface_03->GetAcquireHeightFieldExtent();

	// ---- zap minimax
		surface_03->SetAcquireHeightFieldMin(0);
		surface_03->SetAcquireHeightFieldMax(0);

	// ---- zap
		for (UINT i = 0; i < dest_y_array_extent; i++)
		{
			*(dest_y_array + i) = 0;
		}

	return VS_OK;
}


// ---------- AcquireCalculateHeightField ----------
/*!
\brief calculate acqusition height field
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::AcquireCalculateHeightField()
{
	return VS_OK;
}


// ---------- AcquireGetImageDimensions ----------
/*!
\brief get acqusition image dimensions
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::AcquireGetImageDimensions(
		UINT  *width,
		UINT  *height,
		UINT  *bytes
	)
{

	// ---- local
		INT result = VS_OK;
		vs_system::GfxInterface *gfx = s_pi_model->app_dependency->GetGfxInterface();
		model::Surface_03 *surface_03 = s_pi_model->surface_03;


	// ---- destination atlas page handle ?
		UINT page_handle = gfx->GetAtlasPageHandle(surface_03->GetAcquireAtlasPageLid());
		if ( page_handle != VS_ERROR )
		{

			// ---- destination atlas page lock ?
				UCHAR *dest_buffer_char = gfx->PageLock(page_handle);
				if ( dest_buffer_char != NULL )
				{

					// ---- get dimensions
						gfx->GetAtlasPageTextureDimensions(
								surface_03->GetAcquireAtlasPageLid(),
								width,
								height
							);

					// ---- get bytes
						vs_system::ImgBuffer *img_buffer = NULL;
						surface_03->GetAcquireImgBuffer(0, &img_buffer);
						*bytes = img_buffer->GetBytes();

					// ---- destination atlas page unlock ?
						INT result = gfx->PageUnlock(page_handle);
				}

		}

	return VS_OK;
}


// ---------- AcquireStoreLuminaField ----------
/*!
\brief store acqusition lumina field
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::AcquireStoreLuminaField(
		UINT   acquire_img_index,
		UINT   width,
		UINT   height,
		UINT   bytes,
		BYTE  *byte_data,
		INT    timestamp[]
	)
{

	// ---- local
		INT result = VS_OK;
		model::Surface_03 *surface_03 = s_pi_model->surface_03;


	// ---- destination acquire buffer
		vs_system::ImgBuffer *acquire_img_buffer = NULL;
		surface_03->GetAcquireImgBuffer(acquire_img_index, &acquire_img_buffer);
		BYTE *acquire_buffer = acquire_img_buffer->GetBuffer();


	// ---- copy byte data to acquire buffer ( assume same extent )
		memcpy((VOID*)acquire_buffer, (VOID *)byte_data, (size_t)(width * height * bytes));

	return VS_OK;
}


// ---------- AcquireCopyLuminaFieldToAtlasPage ----------
/*!
\brief copy acqusition lumina field to atlas page image buffer
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::AcquireCopyLuminaFieldToAtlasPage(
		UINT acquire_img_index,
		BOOL view_analysis_img
	)
{

	// ---- local
		INT result = VS_OK;
		vs_system::GfxInterface *gfx = s_pi_model->app_dependency->GetGfxInterface();
		model::Surface_03 *surface_03 = s_pi_model->surface_03;


	// ---- source is a process buffer
		vs_system::ImgBuffer *src_img_buffer = NULL;
		if ( view_analysis_img )
		{
			surface_03->GetAnalyseImgBuffer(acquire_img_index, &src_img_buffer);
		}
		else
		{
			surface_03->GetAcquireImgBuffer(acquire_img_index, &src_img_buffer);
		}


	// ---- destination atlas page handle ?
		UINT page_handle = gfx->GetAtlasPageHandle(surface_03->GetAcquireAtlasPageLid());
		if ( page_handle != VS_ERROR )
		{

			// ---- destination atlas page lock ?
				UCHAR *dest_buffer_char = gfx->PageLock(page_handle);
				if ( dest_buffer_char != NULL )
				{

					// ---- destination and source dimension match ?
						UINT dest_width = 0, dest_height = 0, dest_bytes = 4;
						gfx->GetAtlasPageTextureDimensions(
								surface_03->GetAcquireAtlasPageLid(),
								&dest_width,
								&dest_height
							);

						if (
								dest_width  == src_img_buffer->GetWidth() &&
								dest_height == src_img_buffer->GetHeight() &&
								dest_bytes  == src_img_buffer->GetBytes()
							)
						{
								memcpy(
										(VOID *)dest_buffer_char,
										(VOID *)src_img_buffer->GetBuffer(),
										src_img_buffer->GetExtent()
									);
							}

					// ---- destination atlas page unlock
						INT result = gfx->PageUnlock(page_handle);
				}

		}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
