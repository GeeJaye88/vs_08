 ////////////////////////////////////////////////////////////////////////////////

// ---------- s2_model_acquire.hpp ----------
/*!
\file s2_model_acquire.hpp
\brief Implementation of the Image Acquisition methods
\author Gareth Edwards
*/


////////////////////////////////////////////////////////////////////////////////


// ---------- Acquire_SetupBuffers ----------
/*!
\brief setup acquisition lumina, image & heightfield buffers
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note - for demo use width & height of a designated source atlas page

\note - pixels are cels each of which is bounded by a row, next row,
        column & next column... therefore heighfield dimension is
		equal to source atlas page dimension

*/
INT Model::Acquire_SetupBuffers()
{

	// ---- local
		INT result = VS_OK;
		vs_system::GfxInterface *gfx = s_pi_model->app_dependency->GetGfxInterface();
		model::Surface_02 *surface_02 = s_pi_model->surface_02;


	// ---- get atlas page dimensions
		UINT atlas_page_width = 0, atlas_page_height = 0;
		UINT page_handle = gfx->GetAtlasPageHandle(surface_02->GetAcquireAtlasPageLid());
		if ( page_handle != VS_ERROR )
		{

			// ---- designated atlas page lock ?
				UCHAR *page_buffer = gfx->PageLock(page_handle);
				if ( page_buffer != NULL )
				{

					// ---- dimensions of designated atlas page
						gfx->GetAtlasPageTextureDimensions(
								surface_02->GetAcquireAtlasPageLid(),
								&atlas_page_width,
								&atlas_page_height
							);

					// ---- destination atlas page unlock
						INT result = gfx->PageUnlock(page_handle);
				}
				else
				{
					return VS_ERROR;
				}
		}
		else
		{
			return VS_ERROR;
		}


	// ---- store atlas page to image buffer dimensions
		surface_02->SetAcquireAtlasPageWidth(atlas_page_width);
		surface_02->SetAcquireAtlasPageHeight(atlas_page_height);
		surface_02->SetAcquireAtlasPageBytes(4);


	// ---- camera default to atlas dimensions for demo only
		surface_02->SetAcquireCameraWidth(atlas_page_width);
		surface_02->SetAcquireCameraHeight(atlas_page_height);
		surface_02->SetAcquireCameraBitdepth(8);
		VsIntRectangle rect = { 0, 0, atlas_page_width-1, atlas_page_height-1 };
		surface_02->SetAcquireCameraRect(&rect);
	

	// ---- get camera rect dimensions
		UINT camera_rect_width  = atlas_page_width;
		UINT camera_rect_height = atlas_page_height;


	// ---- initialise acquisition buffers
		UINT img_buffer_max = surface_02->GetAcquireShaderBufferMax();
		for (UINT i = 0; i < img_buffer_max; i++)
		{

			// ---- lumina - 1 unsigned short V
				vs_system::UnsignedShortBuffer *ush_buffer = NULL;
				surface_02->GetAcquireLuminaBuffer(i, &ush_buffer);
				ush_buffer->Initialise(camera_rect_width, camera_rect_height,1);
				ush_buffer->CreateBuffer();

			// ---- acquire - 4 bytes RGBA
				vs_system::ImgBuffer *img_buffer = NULL;
				surface_02->GetAcquireShaderBuffer(i, &img_buffer);
				img_buffer->Initialise(camera_rect_width, camera_rect_height,4);
				img_buffer->CreateBuffer();

			// ---- analysis - 4 bytes RGBA
				surface_02->GetAcquireAnalysisBuffer(i, &img_buffer);
				img_buffer->Initialise(camera_rect_width, camera_rect_height, 4);
				img_buffer->CreateBuffer();
		}


	// ---- initialise heightfield to image buffer dimensions PLUS 1 
		UINT hf_width  = camera_rect_width + 1;
		UINT hf_height = camera_rect_height + 1;
		surface_02->SetAcquireHeightFieldDimension(hf_width);
		surface_02->SetAcquireHeightFieldStride(hf_width);
		surface_02->SetAcquireHeightFieldWidth(hf_width);

		UINT hf_extent = hf_width * hf_width;
		FLOAT *array = new FLOAT[hf_extent];
		surface_02->SetAcquireHeightFieldExtent(hf_extent);
		surface_02->SetAcquireHeightFieldArray(array);
						


	return VS_OK;
}


// ---------- Acquire_CleanupBuffers ----------
/*!
\brief setup acquisition image & heightfield buffers
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::Acquire_CleanupBuffers()
{

	// ---- local
		INT result = VS_OK;
		vs_system::GfxInterface *gfx = s_pi_model->app_dependency->GetGfxInterface();
		model::Surface_02 *surface_02 = s_pi_model->surface_02;


	// ---- delete allocated buffers - note : but not buffer objects
		UINT img_buffer_max = surface_02->GetAcquireShaderBufferMax();
		for (UINT i = 0; i < img_buffer_max; i++)
		{

			// ---- lumina - 1 unsigned short V
				vs_system::UnsignedShortBuffer *ush_buffer = NULL;
				surface_02->GetAcquireLuminaBuffer(i, &ush_buffer);
				ush_buffer->DestroyBuffer();

			// ---- acquire - 4 bytes RGBA
				vs_system::ImgBuffer *img_buffer = NULL;
				surface_02->GetAcquireShaderBuffer(i, &img_buffer);
				img_buffer->DestroyBuffer();

			// ---- analysis - 4 bytes RGBA
				surface_02->GetAcquireAnalysisBuffer(i, &img_buffer);
				img_buffer->DestroyBuffer();

		}


	// ---- delete heightfield buffer array
		FLOAT *array = surface_02->GetAcquireHeightFieldArray();
		delete[] array;


	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Acquire_SetupLighting ----------
/*!
\brief setup acquisition scene lighting
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note light positioning calculations same as SetupGraphicsLighting

\note light positions used in Acquire_CalculateDemoLuminafields()

*/
INT Model::Acquire_SetupLighting()
{
	
	// ---- local
		INT result = 0;
		model::Surface_02 *surface_02 = s_pi_model->surface_02;
		vs_system::Parameter *parameter = &surface_02->parameter;


	// ---- param
		parameter->SetParamGroup(surface_02::Model::PARAM_GROUP_ACQUIRE);
		#include "../../surface_shared/hpp_param/param_setup.hpp"


	// ---- get acquire light parameters
		INT    acquire_param_array_size = parameter->GetParamArraySize();
		FLOAT *acquire_param_array = new FLOAT[acquire_param_array_size];
		result = parameter->GetParamArray(0, acquire_param_array_size, acquire_param_array);

		FLOAT param_intensity = acquire_param_array[1];
		FLOAT param_dist      = acquire_param_array[2];
		FLOAT param_elevation = acquire_param_array[3];
		FLOAT param_azimuth   = acquire_param_array[4];
		FLOAT param_range     = acquire_param_array[5];
		FLOAT param_height    = 0;


	// ---- 4 * point lights
		VsColour light_amb_col = { 0, 0, 0, 1 };
		VsColour light_dif_col = { param_intensity, param_intensity, param_intensity, 1 };
		VsColour light_spc_col = { 1, 1, 1, 1 };

		vs_system::VsLight *light = NULL;

		for (UINT i=0; i<surface_02->GetAcquireLightMax(); i++)
		{

			// ---- position
				FLOAT t2r   = param_elevation * (FLOAT)VS_RADIANS_PER_DEGREE;
				FLOAT phi   = param_azimuth   + (FLOAT)i * 90.0f + 90.0f;
				FLOAT p2r   = phi             * (FLOAT)VS_RADIANS_PER_DEGREE;

				// y' = y*cos q - z*sin q
				// z' = y*sin q + z*cos q
					FLOAT xt =  0;
					FLOAT yt =  param_dist * sinf(t2r);
					FLOAT zt = -param_dist * cosf(t2r);

				// z' = z*cos q - x*sin q
				// x' = z*sin q + x*cos q
					FLOAT zl = zt * cos(p2r) - xt * sin(p2r);
					FLOAT xl = zt * sin(p2r) + xt * cos(p2r);

			// ---- set
				VsVector3 acquire_light_position = { xl, yt, zl };
				surface_02->GetAcquireLight(i, &light);
					result = light->SetType((vs_system::VsLightType)model::Surface_02::LIGHT_TYPE);
					result = light->SetPosition(acquire_light_position);
					result = light->SetAmbient(light_amb_col);
					result = light->SetDiffuse(light_dif_col);
					result = light->SetSpecular(light_spc_col);
					// note: only applied if point light type
					result = light->SetRange(param_range);
					result = light->SetAttConstant(0);
					result = light->SetAttLinear(0);
					result = light->SetAttQuadratic(1);
				surface_02->SetAcquireLight(i, light);

		}

	// ---- param
		#include "../../surface_shared/hpp_param/param_cleanup.hpp"

	return VS_OK;
}


// ---------- Acquire_SetupMaterials ----------
/*!
\brief setip acquisition scene materials
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note material used in AcquireDemoCalculateLuminaFields()

*/
INT Model::Acquire_SetupMaterials()
{

	// ---- local
		INT result = 0;
		model::Surface_02 *surface_02 = s_pi_model->surface_02;

	// ---- get acquire material
		vs_system::VsMaterial *material = NULL;
		surface_02->GetAcquireMaterial(&material);
		material->diffuse  = { 1, 1, 1, 0 };
		material->specular = { 0, 0, 0, 0 };

	return VS_OK;
}


// ---------- Acquire_GetDemoHeightfield ----------
/*!
\brief get demo source heightfield arrays
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note
   - atlas page dimensions initialised
   - get demo BYTE heightfield
   - converted into FLOAT heightfield
   - range 0 to acquire depth

*/
INT Model::Acquire_GetDemoHeightfield()
{

	// ---- local
		INT result = 0;
		model::Surface_02 *surface_02 = s_pi_model->surface_02;
		UINT surface_version_selected = surface_02->GetAcquireSurfaceVersionSelected();


	// ---- y height field array ( + 1 for subdivsion )
		FLOAT *y_array = surface_02->GetAcquireHeightFieldArray();
		UINT   y_array_stride = surface_02->GetAcquireHeightFieldStride();
		UINT   y_array_extent = surface_02->GetAcquireHeightFieldExtent();
		UINT   y_array_offset = 0;


	// ---- store param group
		INT current_param_group = surface_02->parameter.GetParamGroup();


	// ---- y surface scalar
		//
		//  note: this calc is also used in the mesh object
		//        generation code (see s2_obj_surf_mesh.hpp)
		//
		FLOAT y_scalar = surface_02->GetAcquireSurfaceSize() * surface_02->GetAcquireSurfaceYScalar();


	// ---- y initialisation dependant on surface version
		switch ( surface_version_selected )
		{

			case vs_system::ObjConfig::MESH_VERSION_CAMERA:
				{
					vs_system::Heightfield heightfield;
					result = heightfield.Flatfield2D(
						y_array,
						y_array_stride
					);
				}
				break;

			case vs_system::ObjConfig::MESH_VERSION_TERRAIN:
				{
					UCHAR *terrain_bump_map = NULL;
					INT terrain_bump_map_extent = 0;
					result = surface_02->GetTerainBumpMapData(
							&terrain_bump_map,
							&terrain_bump_map_extent
						);

					vs_system::Heightfield heightfield;
					result = heightfield.Heightfield2D(
							y_array,
							y_array_stride,
							y_scalar, // HGT
							terrain_bump_map,
							terrain_bump_map_extent
						);
				}
				break;

			case vs_system::ObjConfig::MESH_VERSION_RR:
				{
					UCHAR *rr_bump_map = NULL;
					INT rr_bump_map_extent = 0;
					result = surface_02->GetRRBumpMapData(
							&rr_bump_map,
							&rr_bump_map_extent
						);

					vs_system::Heightfield heightfield;
					result = heightfield.Heightfield2D(
							y_array,
							y_array_stride,
							y_scalar, // HGT
							rr_bump_map,
							rr_bump_map_extent
						);
				}
				break;

			case vs_system::ObjConfig::MESH_VERSION_FUNCTION:
				{


					OutputDebugString("MF: Acquire_GetDemoHeightfield -> Mesh_Fractal\n");

					// ---- store param group
						INT param_group = surface_02->parameter.GetParamGroup();

					// ---- get parameter function array
						surface_02->parameter.SetParamGroup(surface_02::Model::PARAM_GROUP_FUNCTION);
						INT param_array_index = surface_02->parameter.GetParamGroupIndex();
						INT param_array_size = surface_02->parameter.GetParamArraySize();
						FLOAT *param_array = new FLOAT[param_array_size];
						result = surface_02->parameter.GetParamArray(param_array_index, 9, param_array);

					// ---- fractal - note: NO bump map source - so created on the fly
						result = s_pi_model->obj_config->Mesh_Fractal(
								y_array,
								y_array_stride,
								param_array
							);

					// ---- delete tmp array & restore param group
						delete[] param_array;
						result = surface_02->parameter.SetParamGroup(param_group);

					// ---- y scale - set to 1!
						result = vs_system::ObjConfig::Mesh_YScalar(
								y_array,
								y_array_stride,
								y_scalar
							);
				}
				break;
		}

	// ---- restore parmeter group
		surface_02->parameter.SetParamGroup(current_param_group);

	return VS_OK;
}


// ---------- Acquire_CalculateDemoSurface ----------
/*!
\brief calculate acqusition surface
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::Acquire_CalculateDemoSurface(VsVector3 position[])
{

	// ---- local
		INT result = 0;
		model::Surface_02 *surface_02 = s_pi_model->surface_02;


	// ---- get heightfield
		UINT   y_array_stride = surface_02->GetAcquireHeightFieldStride();               // e.g. 1025
		FLOAT *y_array = surface_02->GetAcquireHeightFieldArray();
		UINT   y_array_offset = 0;


	// ---- get surface dimensions
		FLOAT acquire_width, acquire_height, acquire_depth;                              // e.g. 6.0
		surface_02->GetAcquireSurfaceDimensions(
				&acquire_width,
				&acquire_height,
				&acquire_depth
			);


	// ---- get camera rect dimensions
		UINT camera_rect_width  = surface_02->GetAcquireCameraRectWidth();               // e.g. 1024
		UINT camera_rect_height = surface_02->GetAcquireAtlasPageHeight();


	// ---- init source surf width, depth & y minimax
		FLOAT  x_inc = acquire_width / (FLOAT)(camera_rect_width-1);                     // e.g. 1023 divsions
		FLOAT  z_inc = acquire_width / (FLOAT)(camera_rect_height-1);
		FLOAT  x_by2 = acquire_width / 2;
		FLOAT  z_by2 = acquire_width / 2;
		FLOAT  y_min =  99999; // too big to fail!?
		FLOAT  y_max = -y_min;


	// ---- calc position ( Y from y_array )
		//
		//  - note: for demo only using 1024 of the 1025 y array values
		//
		VsVector3 *p = position;
		for (UINT r = 0; r < camera_rect_width; r++)
		{
			FLOAT z = -(FLOAT)r * z_inc + z_by2;
			y_array_offset = r * y_array_stride;
			for (UINT c = 0; c < camera_rect_height; c++)
			{
				FLOAT  y = *(y_array + y_array_offset + c);
				y_min = y_min < y ? y_min : y;
				y_max = y_max > y ? y_max : y;
				p->x = (FLOAT)c * x_inc - x_by2;
				p->y = y;
				p->z = z;
				p++;
			}
		}


	// ---- store y minimax
		surface_02->SetAcquireHeightFieldMin(y_min);
		surface_02->SetAcquireHeightFieldMax(y_max);

	return VS_OK;
}


// ---------- Acquire_CalculateDemoLuminafields ----------
/*!
\brief calculate acqusition demo lumina fields
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/

template <typename T, typename S, typename D>
void convert_to_lumina(T *output_lumin, S input_value, D bitdepth )
{
	FLOAT p2 = (FLOAT)pow(2, (FLOAT)bitdepth);
	*output_lumin = (T)((FLOAT)input_value * p2 + 0.5f);
}

template <typename T, typename S, typename D>
void convert_from_lumina(T *output_value, S input_lumin, D bitdepth)
{
	FLOAT p2 = (FLOAT)pow(2, (FLOAT)bitdepth);
	*output_value = (T)((FLOAT)input_lumin / p2);
}

INT Model::Acquire_CalculateDemoLuminafields()
{

	FLOAT  iv = 0.75f;
	USHORT ov;
	convert_to_lumina(&ov, iv, 10);

	FLOAT  vv;
	convert_from_lumina(&vv, ov, 10);

	float fred = 2;



	// ---- auto


	// ---- shaders

		//
		// define a function pointer for a capture-less lambda shader
		//
		//  - If you have a lambda with an empty capture specification,
		//    then it can be treated like a regular function and assigned
		//    to a function pointer.
		//
		//  - This works because a lambda that doesn't have a capture group
		//    doesn't need its own class--it can be compiled to a regular old
		//    function, allowing it to be passed around just like a normal function.
		//
		typedef void(*func)(
					vs_system::VsLight    *light,      // [in]
					vs_system::VsMaterial *mtl,        // [in]
					VsVector3             *view,       // [in]
					VsVector3             *position,   // [in] 
					VsVector3             *normal,     // [in]
					VsColour              *colour      // [out]
				);

		//
		// note: - standard = global ambient + ( diffuse + specular + ambient )
		//
		func spot_standard_shader = [](
					vs_system::VsLight    *light,      // [in]
					vs_system::VsMaterial *mtl,        // [in]
					VsVector3             *view,       // [in]
					VsVector3             *position,   // [in] 
					VsVector3             *normal,     // [in]
					VsColour              *colour      // [out] 
				)
			{

				// ---- light direction
					VsVector3 light_dir = light->position - *position;

				// ---- normalise light source direction & return distance to the light source
					FLOAT distance = light_dir.Normalise();

				// ---- view direction
					VsVector3  view_dir = *view - *position;

				// ---- normalise light source direction & return distance to the light source
					view_dir.Normalise();

				// ---- global ambient
					*colour = { 0, 0, 0, 0 };

				// ---- diffuse reflectance ? - calc the dot product between light direction & normal
					FLOAT ndotl = light_dir.DotProduct(*normal);
					if ( ndotl > 0 )
					{
						FLOAT att = 1 / (light->attenuation2 * distance * distance);
						*colour += light->diffuse * att * (mtl->diffuse * ndotl + mtl->ambient);
					}

				// ---- specular reflectance ? - calc the dot product between view direction & normal
					FLOAT ndotv = view_dir.DotProduct(*normal);
					if ( ndotv > 0 )
					{
						*colour += mtl->specular * light->specular  * pow(ndotv, mtl->power);
					}

				// ---- ambient
					*colour += mtl->ambient;

				// ---- max limit
					colour->Clamp();

			};

		//
		// note: - greyscale shader = ( diffuse + ambient )
		//       - greyscale value returned in colour->red
		//
		func spot_greyscale_shader = [](
					vs_system::VsLight    *light,      // [in]
					vs_system::VsMaterial *mtl,        // [in]
					VsVector3             *view,       // [in]
					VsVector3             *position,   // [in] 
					VsVector3             *normal,     // [in]
					VsColour              *colour      // [out] 
				)
			{
				// ---- light direction
					VsVector3 light_dir = light->position - *position;

				// ---- normalise light source direction & return distance to the light source
					FLOAT distance = light_dir.Normalise();

				// ---- calc the dot product between light direction & normal
					FLOAT ndotl = light_dir.DotProduct(*normal);

				// ---- diffuse reflect
					colour->red = mtl->ambient.red;
					if ( ndotl > 0 )
					{
						FLOAT att = 1 / (light->attenuation2 * distance * distance);
						colour->red = light->diffuse.red * att *
										( mtl->diffuse.red * ndotl + mtl->ambient.red );
					}

				// ---- max limit
					colour->red = colour->red > 1 ? 1 : colour->red;
			};
		
		//
		// note: - directional shader = ( diffuse + ambient )
		//       - light directional
		//       - greyscale value returned in colour->red
		//
		func directional_greyscale_shader = [](
					vs_system::VsLight    *light,      // [in]
					vs_system::VsMaterial *mtl,        // [in]
					VsVector3             *view,       // [in]
					VsVector3             *position,   // [in] 
					VsVector3             *normal,     // [in]
					VsColour              *colour      // [out] 
				)
			{

				// ---- light direction
					VsVector3 light_dir = light->position;

				// ---- normalise light source direction & return distance to the light source
					FLOAT distance = light_dir.Normalise();

				// ---- calc the dot product between light direction & normal
					FLOAT ndotl = light_dir.DotProduct(*normal);

				// ---- diffuse reflect
					colour->red = mtl->ambient.red;
					if ( ndotl > 0 )
					{
						colour->red = (mtl->diffuse.red * ndotl + mtl->ambient.red);
					}

				// ---- max limit
					colour->red = colour->red > 1 ? 1 : colour->red;
		};

		//
		// init array of capture-less lambda shader function pointers
		//
		func p_shader[3] =
		{
			spot_standard_shader,
			spot_greyscale_shader,
			directional_greyscale_shader
		};

	// ---- local
		INT result = 0;
		model::Surface_02 *surface_02 = s_pi_model->surface_02;
		

	// ---- get camera rect dimensions
		UINT camera_rect_width = surface_02->GetAcquireCameraRectWidth();                // e.g. 1024
		UINT camera_rect_height = surface_02->GetAcquireAtlasPageHeight();


	// ---- get atlas page pixel stride
		UINT atlas_page_bytes  = surface_02->GetAcquireAtlasPageBytes();                 // e.g. 1024


	// ---- heightfield extent
		UINT extent = (camera_rect_width * camera_rect_height );                         // e.g. 1024 x 1024


	// ---- calculate surface
		VsVector3 *position = new VsVector3[extent];
		Acquire_CalculateDemoSurface(position);


	// ---- get surface dimensions
		FLOAT surface_width, surface_height, surface_depth;                              // e.g. 6.0
		surface_02->GetAcquireSurfaceDimensions(
				&surface_width,
				&surface_height,
				&surface_depth
			);


	// ---- calculate surface normals
		VsVector3 *normal = new VsVector3[extent];
		result = vs_system::ObjConfig::Mesh_Normals( // from s2::Acquire_CalculateDemoLuminafields
				normal,
				camera_rect_width, // assuming square TBD
				position,
				surface_width,     // assuming square TBD
				surface_width
			);


	// ---- setup virtual scene lights
		result = Acquire_SetupLighting();


	// ---- setup virtual scene materials
		result = Acquire_SetupMaterials();


	// ---- get view
		vs_system::AppKamera *kamera = s_pi_model->app_kamera[s_pi_model->app_kamera_index];
		VsVector3 view ;
		kamera->GetPosition(&view);


	// ---- get material
		VsColour colour = { 0, 0, 0, 0};
		vs_system::VsMaterial *material = NULL;
		surface_02->GetAcquireMaterial(&material);


	// ---- calculate lumina fields (include <= to +1 for flat white 3D surface)
		UINT lf_max = surface_02->GetAcquireLuminaBufferMax();
		for (UINT lf_index = 0; lf_index <= lf_max; lf_index++)
		{
			
			// ---- 3d surface ?
				if ( lf_index == 4 )
				{
					VsByteRGBA rgba = { 255, 255, 255, 0 };
					Acquire_CopyRGBAToImageBuffer(lf_index, rgba);
				}
				else
				{

					// ---- for this lumina field...
						vs_system::UnsignedShortBuffer *p = NULL;
						result = surface_02->GetAcquireLuminaBuffer(lf_index, &p);
						USHORT *lf_buffer = p->GetBuffer();
						UINT lf_block  = p->GetBlock();
						UINT lf_stride = p->GetStride();
						UINT lf_row_offset = 0;
						UINT lf_col_offset = 0;

					// ---- get light
						vs_system::VsLight *light = NULL;
						result = surface_02->GetAcquireLight(lf_index, &light);

					// ---- per position/normal cels
						UINT row_offset = 0;
						UINT col_offset = 0;

					// ---- per row
						for (UINT row = 0; row < camera_rect_height; row++)
						{
								row_offset = row * camera_rect_width;
								lf_row_offset = row * lf_stride;

							// ---- per column
								for (UINT column = 0; column < camera_rect_width; column++)
								{
										col_offset = row_offset + column;

									// ---- shader ( STANDARD, GREYSCALE or DIRECTIONAL )
										(*p_shader[model::Surface_02::SurfaceShader::SHADER_TYPE])(
													light,                   // [in] light
													material,                // [in] material
													&view,                   // [in] view location
													&position[col_offset],   // [in] position on surface
													&normal[col_offset],     // [in] surface normal to position
													&colour                  // [out] result
												);

									// ---- calculate FLOAT to bitdepth
										USHORT red = (USHORT)(colour.red * 255);

									// ---- store
										lf_col_offset = lf_row_offset + column * lf_block;
										*(lf_buffer + lf_col_offset) = red;

								}
						}

					// ---- copy lumina field into lumina field image buffer
						result = Acquire_CopyLuminafieldToImageBuffer(lf_index,p);

				}

		}


	// ---- cleanup (in reverse order)
		delete[] normal;
		delete[] position;

	return VS_OK;
}


// ---------- Acquire_AnalyseLuminafields ----------
/*!
\brief analyse acquisition lumina fields
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/

INT Model::Acquire_AnalyseLuminafields()
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
		model::Surface_02 *surface_02 = s_pi_model->surface_02;


	// ---- get aquire param
		INT current_param_group = surface_02->parameter.GetParamGroup();
		surface_02->parameter.SetParamGroup(PARAM_GROUP_ACQUIRE);
		INT acquire_param_group_index = surface_02->parameter.GetParamGroupIndex();
		INT acquire_param_group_size  = surface_02->parameter.GetParamGroupSize();
		INT acquire_param_array_index = surface_02->parameter.GetParamArrayIndex();
		INT acquire_param_array_size  = surface_02->parameter.GetParamArraySize();
		FLOAT *acquire_param_array = new FLOAT[acquire_param_array_size];


	// ---- get "resolvalibity" value parameter array
		result = surface_02->parameter.GetParamArray(2, 9, acquire_param_array);


	// ---- set min "resolvalibity"  value parameters
		BYTE min_resolve_value = float_to_byte(*(acquire_param_array + 1));
		BYTE min_red   = float_to_byte(0.0f);
		BYTE min_green = float_to_byte(0.2f);
		BYTE min_blue  = float_to_byte(1.0f);


	// ---- set max "resolvalibity" value parameters
		BYTE max_resolve_value = float_to_byte(*(acquire_param_array + 2));
		BYTE max_red   = float_to_byte(1.0f);
		BYTE max_green = float_to_byte(0.0f);
		BYTE max_blue  = float_to_byte(0.0f);


	// ---- get shader buffers
		UINT shader_max = surface_02->GetAcquireShaderBufferMax();
		BYTE **aquire_shader_buffer = new BYTE *[shader_max];
		vs_system::ImgBuffer *img_buffer = NULL;
		for (UINT i = 0; i < shader_max; i++)
		{
			surface_02->GetAcquireShaderBuffer(i, &img_buffer);
			aquire_shader_buffer[i] = img_buffer->GetBuffer();
		}


	// ---- get analysis buffers
		UINT analysis_max = surface_02->GetAcquireAnalysisBufferMax();
		BYTE **aquire_analysis_buffer = new BYTE * [analysis_max];
		for (UINT i = 0; i < analysis_max; i++)
		{
			surface_02->GetAcquireAnalysisBuffer(i, &img_buffer);
			aquire_analysis_buffer[i]= img_buffer->GetBuffer();
		}


	// ---- get lumina buffers
		UINT lumina_max = surface_02->GetAcquireLuminaBufferMax();
		USHORT **aquire_lumina_buffer = new USHORT *[lumina_max];
		vs_system::UnsignedShortBuffer *lumina_buffer = NULL;
		for (UINT i = 0; i < lumina_max; i++)
		{
			surface_02->GetAcquireLuminaBuffer(i, &lumina_buffer);
			aquire_lumina_buffer[i] = lumina_buffer->GetBuffer();
		}


	// ---- get camera rect dimensions - note: same for atlas, analysis & lumina
		UINT camera_rect_width = surface_02->GetAcquireCameraRectWidth();                // e.g. 1024
		UINT camera_rect_height = surface_02->GetAcquireAtlasPageHeight();


	// ---- shader line and pixel stride
		surface_02->GetAcquireShaderBuffer(0, &img_buffer);
		UINT shader_stride = img_buffer->GetStryde(); // TBF ImgBuffer GetStride wrong
		UINT shader_bytes  = img_buffer->GetBytes();


	// ---- analysis line and pixel stride
		surface_02->GetAcquireAnalysisBuffer(0, &img_buffer);
		UINT analysis_stride = img_buffer->GetStryde(); // TBF ImgBuffer GetStride wrong
		UINT analysis_bytes  = img_buffer->GetBytes();


	// ---- lumina line and block stride
		surface_02->GetAcquireLuminaBuffer(0, &lumina_buffer);
		UINT lumina_stride = lumina_buffer->GetStride();
		UINT lumina_block  = lumina_buffer->GetBlock();


	// ---- eval minimax
		UINT max_lumin_value = 0, min_lumin_value = 255;
		UINT too_dark_count = 0;
		UINT too_light_count = 0;
		UINT resolve_count = 0;

	// ---- rows
		for (UINT row = 0; row < camera_rect_height; row++)
		{
				UINT shader_row_offset   = shader_stride   * row;
				UINT analysis_row_offset = analysis_stride * row;
				UINT lumina_row_offset   = lumina_stride   * row;

			// ---- columns
				for (UINT col = 0; col < camera_rect_width; col++)
				{

					// ---- test individual "too dark" or "too light"
						UINT shader_col_offset   = shader_row_offset   + shader_bytes   * col;
						UINT analysis_col_offset = analysis_row_offset + analysis_bytes * col;
						UINT lumina_col_offset   = lumina_row_offset   + lumina_block   * col;


					// ---- test individual "too dark" or "too light"
						BOOL too_dark[4]  = { 0, 0, 0, 0 };
						BOOL too_light[4] = { 0, 0, 0, 0 };
						for (UINT i = 0; i < 4; i++)
						{
							BYTE value = (BYTE)*(aquire_lumina_buffer[i] + lumina_col_offset);
							max_lumin_value = value > max_lumin_value ? value : max_lumin_value;
							min_lumin_value = value < min_lumin_value ? value : min_lumin_value;
							if ( value >= max_resolve_value )
								too_light[i] = 1;
							else if ( value <= min_resolve_value )
								too_dark[i] = 1;
						}


					// ---- set
						for (UINT i = 0; i < 2; i++)
						{

							// ---- analysis
								BYTE *ana_1 = aquire_analysis_buffer[i    ] + analysis_col_offset;
								BYTE *ana_2 = aquire_analysis_buffer[i + 2] + analysis_col_offset;

							// ---- too dark ? - set this and opposite min colour
								if ( too_dark[i] || too_dark[i+2] )
								{
									*(ana_2 + 0) = *(ana_1 + 0) = min_blue;
									*(ana_2 + 1) = *(ana_1 + 1) = min_green;
									*(ana_2 + 2) = *(ana_1 + 2) = min_red;
									too_dark_count++;
								}
							// ---- too light ? - set this and opposite max colour
								else if ( too_light[i] || too_light[i+2] )
								{
									*(ana_2 + 0) = *(ana_1 + 0) = max_blue;
									*(ana_2 + 1) = *(ana_1 + 1) = max_green;
									*(ana_2 + 2) = *(ana_1 + 2) = max_red;
									too_light_count++;
								}
							// ---- ok? - set this and opposite == shader
								else
								{
									BYTE *sha_1 = aquire_shader_buffer[i    ] + shader_col_offset;
									BYTE *sha_2 = aquire_shader_buffer[i + 2] + shader_col_offset;
									*(ana_1 + 0) = *(sha_1 + 0);
									*(ana_1 + 1) = *(sha_1 + 1);
									*(ana_1 + 2) = *(sha_1 + 2);
									*(ana_2 + 0) = *(sha_2 + 0);
									*(ana_2 + 1) = *(sha_2 + 1);
									*(ana_2 + 2) = *(sha_2 + 2);
									resolve_count++;
								}

						}
				}
		}


	// ---- restore parmeter group
		surface_02->parameter.SetParamGroup(current_param_group);


	// ---- cleanup
		delete [] aquire_lumina_buffer;
		delete [] aquire_analysis_buffer;
		delete [] aquire_shader_buffer;


	// ---- report
		char ods[128];
		sprintf(ods, " +-> minimax: %d %d \n", (INT)min_lumin_value, (UINT)max_lumin_value);
		OutputDebugString(ods);
		FLOAT extent = (FLOAT)(camera_rect_height*camera_rect_width);
		sprintf(ods, "     resolvable: %% %.3f \n", (FLOAT)resolve_count/2 / extent );
		OutputDebugString(ods);
		sprintf(ods, "     too dark  : %% %.3f \n", (FLOAT)too_dark_count / 2 / extent);
		OutputDebugString(ods);
		sprintf(ods, "     too light : %% %.3f \n", (FLOAT)too_light_count / 2 / extent);
		OutputDebugString(ods);

	return VS_OK;
}


// ---------- Acquire_InitialiseHeightfield ----------
/*!
\brief init flat (y == 0) acqusition height field
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::Acquire_InitialiseHeightfield()
{

	// ---- local
		INT result = 0;
		model::Surface_02 *surface_02 = s_pi_model->surface_02;

	// ---- get heighfield stuff
		FLOAT *dest_y_array = surface_02->GetAcquireHeightFieldArray();
		UINT   dest_y_array_extent = surface_02->GetAcquireHeightFieldExtent();

	// ---- zap minimax
		surface_02->SetAcquireHeightFieldMin(0);
		surface_02->SetAcquireHeightFieldMax(0);

	// ---- zap
		for (UINT i = 0; i < dest_y_array_extent; i++)
		{
			*(dest_y_array + i) = 0;
		}

	return VS_OK;
}


// ---------- Acquire_CalculateHeightfield ----------
/*!
\brief calculate acqusition height field
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::Acquire_CalculateHeightfield()
{
	return VS_OK;
}


// ---------- Acquire_CopyLuminafieldToImageBuffer ----------
/*!
\brief store acqusition lumina field
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::Acquire_CopyLuminafieldToImageBuffer(
		UINT    lf_index,
		UINT    lf_width,
		UINT    lf_height,
		UINT    lf_stride,
		UINT    lf_block,
		USHORT *lf_array
	)
{

	// ---- local
		INT result = VS_OK;
		model::Surface_02 *surface_02 = s_pi_model->surface_02;


	// ---- create source lf buffer
		vs_system::ImgBuffer img_buffer;
		img_buffer.Initialise(lf_width, lf_height, 4);
		img_buffer.CreateBuffer();
		BYTE *ib = img_buffer.GetBuffer();


	// ---- fill source buffer 
		UINT ib_row_offset   = 0;
		UINT ib_col_offset   = 0;
		UINT ib_stride       = img_buffer.GetPitch();
		UINT ib_pixel_stride = img_buffer.GetBytes();

		UINT lf_row_offset = 0;
		UINT lf_col_offset = 0;

		for (UINT r = 0; r < lf_height; r++)
		{
			ib_row_offset = r * ib_stride;
			lf_row_offset = r * lf_stride;
			for (UINT c = 0; c < lf_width; c++)
			{
				ib_col_offset = ib_row_offset + c * ib_pixel_stride;
				lf_col_offset = lf_row_offset + c * lf_block;
				*(ib + ib_col_offset    ) =
					*(ib + ib_col_offset + 1) =
						*(ib + ib_col_offset + 2) = (BYTE)*(lf_array + lf_col_offset );
				*(ib + ib_col_offset + 3) = 0;
			}
		}


	// ---- destination acquire buffer
		vs_system::ImgBuffer *acquire_img_buffer = NULL;
		surface_02->GetAcquireShaderBuffer(lf_index, &acquire_img_buffer);
		BYTE *acquire_buffer = acquire_img_buffer->GetBuffer();


	// ---- copy lumina image buffer to acquire buffer ( assume same extent )
		memcpy((VOID*)acquire_buffer,
				(VOID *)ib,
				(size_t)(lf_width * lf_height * ib_pixel_stride)
			);

	return VS_OK;
}


INT Model::Acquire_CopyLuminafieldToImageBuffer(
		UINT lfb_index,
		vs_system::UnsignedShortBuffer *bu
	)
{

	// ---- local
		INT result = VS_OK;
		model::Surface_02 *surface_02 = s_pi_model->surface_02;

	// ---- lumina field
		USHORT*lfb_buffer = bu->GetBuffer();
		UINT lfb_width  = bu->GetColumns();
		UINT lfb_height = bu->GetRows();
		UINT lfb_block  = bu->GetBlock();
		UINT lfb_stride = bu->GetStride();
		UINT lfb_row_offset   = 0;
		UINT lfb_col_offset   = 0;


	// ---- create source lf buffer
		vs_system::ImgBuffer img_buffer;
		img_buffer.Initialise(lfb_width, lfb_height, 4);
		img_buffer.CreateBuffer();
		BYTE *ib = img_buffer.GetBuffer();


	// ---- fill source buffer
		UINT ib_row_offset   = 0;
		UINT ib_col_offset   = 0;
		UINT ib_stride       = img_buffer.GetPitch(); // !!!
		UINT ib_pixel_stride = img_buffer.GetBytes();

		UINT lf_row_offset = 0;
		UINT lf_col_offset = 0;

		for (UINT r = 0; r < lfb_height; r++)
		{
			ib_row_offset = r * ib_stride;
			lf_row_offset = r * lfb_stride;
			for (UINT c = 0; c < lfb_width; c++)
			{
				ib_col_offset = ib_row_offset + c * ib_pixel_stride;
				lf_col_offset = lf_row_offset + c * lfb_block;

				BYTE fred = (BYTE)*(lfb_buffer + lf_col_offset);
				*(ib + ib_col_offset    ) =
					*(ib + ib_col_offset + 1) =
						*(ib + ib_col_offset + 2) = fred;
				*(ib + ib_col_offset + 3) = 0;
			}
		}


	// ---- destination acquire buffer
		vs_system::ImgBuffer *acquire_img_buffer = NULL;
		surface_02->GetAcquireShaderBuffer(lfb_index, &acquire_img_buffer);
		BYTE *acquire_buffer = acquire_img_buffer->GetBuffer();


	// ---- copy lumina image buffer to acquire buffer ( assume same extent )
		memcpy((VOID*)acquire_buffer,
				(VOID *)ib,
				(size_t)(lfb_width * lfb_height * ib_pixel_stride)
			);


	return VS_OK;
}


INT Model::Acquire_CopyRGBAToImageBuffer(
		UINT lfb_index,
		VsByteRGBA rgba
	)
{
	
	// ---- local
		INT result = VS_OK;
		model::Surface_02 *surface_02 = s_pi_model->surface_02;


	// ---- destination acquire buffer
		vs_system::ImgBuffer *acquire_img_buffer = NULL;
		surface_02->GetAcquireShaderBuffer(lfb_index, &acquire_img_buffer);
		BYTE *acquire_buffer = acquire_img_buffer->GetBuffer();

		UINT width = acquire_img_buffer->GetWidth();
		UINT height = acquire_img_buffer->GetHeight();
		UINT stride = acquire_img_buffer->GetPitch(); // !!!
		UINT pixel_stride = acquire_img_buffer->GetBytes();

		UINT row_offset = 0;
		UINT col_offset = 0;


	// ---- copy
		for (UINT r = 0; r < height; r++)
		{
			row_offset = r * stride;
			for (UINT c = 0; c < width; c++)
			{
				col_offset = row_offset + c * pixel_stride;
				*(acquire_buffer + col_offset)     = rgba.red;
				*(acquire_buffer + col_offset + 1) = rgba.green;
				*(acquire_buffer + col_offset + 2) = rgba.blue;;
				*(acquire_buffer + col_offset + 3) = rgba.alpha;;
			}
		}

	return VS_OK;
}

// ---------- Acquire_CopyImageBufferToAtlasPage ----------
/*!
\brief copy image buffer to atlas page image buffer
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
INT Model::Acquire_CopyImageBufferToAtlasPage(
		UINT ib_index,
		BOOL view_analysis
	)
{

	// ---- local
		INT result = VS_OK;
		vs_system::GfxInterface *gfx = s_pi_model->app_dependency->GetGfxInterface();
		model::Surface_02 *surface_02 = s_pi_model->surface_02;


	// ---- source is a process buffer
		vs_system::ImgBuffer *src_img_buffer = NULL;
		if ( view_analysis )
		{
			surface_02->GetAcquireAnalysisBuffer(ib_index, &src_img_buffer);
		}
		else
		{
			surface_02->GetAcquireShaderBuffer(ib_index, &src_img_buffer);
		}


	// ---- destination atlas page handle ?
		UINT page_handle = gfx->GetAtlasPageHandle(surface_02->GetAcquireAtlasPageLid());
		if ( page_handle != VS_ERROR )
		{

			// ---- destination atlas page lock ?
				UCHAR *dest_buffer_char = gfx->PageLock(page_handle);
				if ( dest_buffer_char != NULL )
				{

					// ---- destination and source dimension match ?
						UINT dest_width = 0, dest_height = 0, dest_bytes = 4;
						gfx->GetAtlasPageTextureDimensions(
								surface_02->GetAcquireAtlasPageLid(),
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
