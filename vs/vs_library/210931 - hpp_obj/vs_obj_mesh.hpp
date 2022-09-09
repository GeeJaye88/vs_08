////////////////////////////////////////////////////////////////////////////////

// ---------- vs_obj_mesh.hpp ----------
/*!
\file vs_obj_mesh.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_Mesh_Configure ----------
INT ObjConfig::Callback_Mesh_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get supplied and parameter data
		UINT mesh_type   = (UINT)pd[MESH_TYPE];
		UINT mesh_ap_ref = (UINT)pd[MESH_AP_REF];
		UINT mesh_rows   = (UINT)pd[MESH_ROWS];
		UINT mesh_cols   = (UINT)pd[MESH_COLS];


	// ---- atlas page
		context->SetAtlasPage(0, mesh_ap_ref);


	// ---- overide ?
		UINT format = (UINT)(VS_XYZ | VS_TEX1);
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(FALSE);
			context->SetColourVertex(FALSE);
			context->SetMultiState(FALSE);

			switch ( mesh_type )
			{
				default:
				case 0: // wireframe
					format = (UINT)(VS_XYZ | VS_NORMAL);
					context->SetSolid(FALSE);
					context->SetTextured(FALSE);
					break;
				case 1: // solid
					format = (UINT)(VS_XYZ | VS_NORMAL);
					context->SetSolid(TRUE);
					context->SetTextured(FALSE);
					break;
				case 2: // wireframe & texture
					format = (UINT)(VS_XYZ | VS_NORMAL | VS_TEX1);
					context->SetSolid(FALSE);
					context->SetTextured(TRUE);
					break;
				case 3: // solid & textured
					format = (UINT)(VS_XYZ | VS_NORMAL | VS_TEX1);
					context->SetSolid(TRUE);
					context->SetTextured(TRUE);
					break;
			}

			context->SetVisible(TRUE);
		}


	// ---- primitive type
		context->SetPrimitiveType(VS_TRIANGLESTRIP);


	// ---- vertex buffer
		context->SetVertexCount(mesh_cols * mesh_rows);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_Mesh_GetVertexBuffer);


	// ---- index buffer
		UINT numIndices = (mesh_cols * 2) * (mesh_rows - 1) + (mesh_rows - 2);
		context->SetIndexCount(numIndices);
		context->SetIndexBufferCallback(Callback_Mesh_GetIndexBuffer);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(FLOAT)
			);
		context->SetPrimitiveCount(numIndices - 2);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ---------- Callback_Mesh_GetVertexBuffer ----------
/*!
\brief get vertex buffer
\author Gareth Edwards

\note invokes method Mesh_Normals with height array

*/
INT ObjConfig::Callback_Mesh_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- local
		INT result = VS_OK;
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();
		LONG  *ld = context->GetLongParameter();


	// ---- get parameter data
		UINT  mesh_type    = (UINT)pd[MESH_TYPE];
		UINT  mesh_ap_ref  = (UINT)pd[MESH_AP_REF];
		UINT  mesh_rows    = (UINT)pd[MESH_ROWS];
		UINT  mesh_cols    = (UINT)pd[MESH_COLS];
		UINT  mesh_align   = (UINT)pd[MESH_ALIGN];
		FLOAT mesh_red     = (FLOAT)pd[MESH_RED];
		FLOAT mesh_green   = (FLOAT)pd[MESH_GREEN];
		FLOAT mesh_blue    = (FLOAT)pd[MESH_BLUE];
		FLOAT mesh_width   = (FLOAT)pd[MESH_WIDTH];
		FLOAT mesh_depth   = (FLOAT)pd[MESH_DEPTH];
		FLOAT mesh_height  = (FLOAT)pd[MESH_HEIGHT]; // hgt not used
		FLOAT mesh_x_off   = (FLOAT)pd[MESH_XOFF];
		FLOAT mesh_y_off   = (FLOAT)pd[MESH_YOFF];
		FLOAT mesh_z_off   = (FLOAT)pd[MESH_ZOFF];
		FLOAT mesh_scalar  = (FLOAT)pd[MESH_SCALAR];
		UINT  mesh_version = (UINT)pd[MESH_PARAM_0];


	// ---- vertex
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_count  = context->GetVertexCount();
		UINT vertex_format = context->GetVertexFormat();
		FLOAT *fvb = context->GetVertexBufferPtr();


	// ---- y_heightfield (note stride + 1 for subdivsion)
		//
		// note: *(ld + 0) = (e.g.) (LONG)pimp->terrain_bump_map->heightfield_data;
		//       *(ld + 1) = (e.g.) (LONG)pimp->terrain_bump_map->heightfield_extent;
		//       *(ld + 2) = 0 or scalar
		//       *(ld + 3) = (LONG)pimp->y_heightfield->GetBuffer();
		//       *(ld + 4) = (LONG)pimp->y_heightfield->GetExtent();
		//
		FLOAT *y_heightfield_buffer = (FLOAT*)*(ld + 3);
		INT    y_heightfield_extent = (INT)*(ld + 4);
		INT    y_heightfield_offset = 0;
		INT    y_heightfield_stride = mesh_cols + 1;


	// ---- select
		switch ( mesh_version )
		{

			// ---- default & null
				default:

			// ---- camera - not active
				case MESH_VERSION_CAMERA:
					{
						Heightfield heightfield;
						result = heightfield.Flatfield2D(
								y_heightfield_buffer,
								y_heightfield_stride
							);
					}
					break;

			// ---- demo (e.g. bump map)
				case MESH_VERSION_RR:
				case MESH_VERSION_TERRAIN:
					{
						Heightfield heightfield;
						result = heightfield.Heightfield2D(
								y_heightfield_buffer,
								y_heightfield_stride,
								mesh_scalar,
								(UCHAR *) *(ld+0),
								(INT) *(ld+1)
							);
					}
					break;

			// ---- functional surface (e.g. fractal)
				case MESH_VERSION_FUNCTION:
					{

						// ---- fractal - note: NO source - so created on the fly
							result = Mesh_Fractal(
									y_heightfield_buffer,
									y_heightfield_stride,
									&pd[MESH_PARAM_0]
								);

						// ---- y scale proportional to size (only surface_02)
							FLOAT y_scalar = mesh_scalar;
							result = Mesh_YScalar(
									y_heightfield_buffer,
									y_heightfield_stride,
									y_scalar
								);

					}
					break;

		}


	// ---- normals
		FLOAT *n_array = new FLOAT[y_heightfield_stride * y_heightfield_stride * 3];
		result = Mesh_Normals( // from ObjConfig::Callback_Mesh_GetVertexBuffer
				y_heightfield_buffer,
				y_heightfield_stride,
				n_array,
				mesh_width,
				mesh_depth
			);


	// ---- assemble vertex buffer
		struct vertex
		{
			FLOAT xv = 0, yv = 0, zv = 0;
			FLOAT xn = 0, yn = 0, zn = 0;
			FLOAT ut = 0, vt = 0;
		} vtx;


	// ---- local stuff
		FLOAT *n_in_array = n_array;
		FLOAT  u = 0, v = 0;
		INT    fvb_stride = vertex_format & VS_TEX1 ? 8 : 6;


	// ---- rows
		for ( UINT row = 0; row < mesh_rows; row++ )
		{

			// ---- interpolate v & z
				y_heightfield_offset = row * y_heightfield_stride;
				vtx.vt =  v = (FLOAT)row / (FLOAT)(mesh_rows - 1);
				vtx.zv = -v * mesh_depth + mesh_z_off;

			// ---- columns
				for ( UINT col = 0; col < mesh_cols; col++ )
				{

					// ---- interpolate u & x
						vtx.ut = u = (FLOAT)col / (FLOAT)(mesh_cols - 1);
						vtx.xv = u * mesh_width + mesh_x_off;
						vtx.yv = *(y_heightfield_buffer + y_heightfield_offset + col) + mesh_y_off;

					// ---- set normal
						vtx.xn = *(n_in_array + 0);
						vtx.yn = *(n_in_array + 1);
						vtx.zn = *(n_in_array + 2);
						n_in_array += 3;

					// copy into vertex buffer
						memcpy( (VOID *)(fvb), (VOID *)&vtx, sizeof(FLOAT)*fvb_stride);
						fvb += fvb_stride;
				
				}

			// ---- increment
				n_in_array += 3; // stride == mesh_cols + 1

		}


	// ---- cleanup
		delete[] n_array;

	return VS_OK;
}


// ---------- Mesh_Normals ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note invoked by ObjConfig::Callback_Mesh_GetVertexBuffer

*/
INT ObjConfig::Mesh_Normals( // heightfield buffer version
		FLOAT *y_array,
		UINT   stride,
		FLOAT *n_array,
		FLOAT  width,
		FLOAT  depth
	)
{

	// ---- buffer
		int cols       = stride;
		int rows       = stride;
		int row_stride = cols;
		int extent     = cols * rows;
		int norm_count = 0;
		float fnc      = 0;


	// ---- mesh spacing
		double x_inc = width / (FLOAT)(cols - 1);
		double z_inc = width / (FLOAT)(rows - 1);


	// ---- first surface normal 
		FLOAT *n2 = n_array;


	// ---- used to calc unit normal
		double x1, y1, z1;
		double x2, y2, z2;
		double x3, y3, z3;
		double v1x, v1y, v1z;
		double v2x, v2y, v2z;
		double xd, yd, zd;
		double len;


	// ---- tables
		int  ro[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // offsets
		bool gs[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // grid states


	// ---- sequential xz offset
		double x[] = { -x_inc, -x_inc, -x_inc,  0,     x_inc,  x_inc,  x_inc,  0,     -x_inc };
		double z[] = { -z_inc,  0,      z_inc,  z_inc, z_inc,  0,     -z_inc, -z_inc,  0     };


	//---- for each cel...
		int   last_row_offset, this_row_offset, next_row_offset;
		int   lro, tro, nro;


	//---- rows
		for (INT row = 0; row < rows; row++)
		{

			// ---- offsets
				last_row_offset = (row - 1) * row_stride;
				this_row_offset =  row      * row_stride;
				next_row_offset = (row + 1) * row_stride;

			// ---- columns
				for (INT col = 0; col < cols; col++)
				{

					// ---- row offsets
						lro = last_row_offset + col;
						tro = this_row_offset + col;
						nro = next_row_offset + col;

					// ---- sequential grid list
						ro[0] = lro-1; ro[1] = tro-1; ro[2] = nro-1;
						ro[7] = lro;                  ro[3] = nro;
						ro[6] = lro+1; ro[5] = tro+1; ro[4] = nro+1;
						ro[8] = ro[0];

					// ---- set grid state (gs) flag 'within'
						for (int i = 0; i < 9; i++)
						{
							gs[i] = ro[i] >= 0 && ro[i] < extent ? true : false;
						}

					// ---- init
						*(n2 + 0) = 0;
						*(n2 + 1) = 0;
						*(n2 + 2) = 0;

					// ---- current mesh location
						norm_count = 0;
						x2 = 0;
						y2 = *(y_array + tro);
						z2 = 0;

					// ---- sequentialy accumulate normals
						for (int i=0; i<8; i++)
						{

							// ---- 'within' ?
								if ( gs[i] && gs[i+1] )
								{

									// ---- sequential grid locations
										x1 = x[i];
										y1 = *(y_array + ro[i]  );
										z1 = z[i];
										x3 = x[i+1];
										y3 = *(y_array + ro[i+1]);
										z3 = z[i+1];

									// ---- cross product
										v1y = y1 - y2;
										v2y = y3 - y2;
										if ( fabs(v1y) < 0.001f && fabs(v2y) < 0.001f )
										{
											*(n2 + 1) += 1;
										}
										else
										{
											v1x = x1 - x2;
											v1z = z1 - z2;
											v2x = x3 - x2;
											v2z = z3 - z2;
											xd = v1y * v2z - v1z * v2y;
											yd = v1z * v2x - v1x * v2z;
											zd = v1x * v2y - v1y * v2x;
											len = (FLOAT)sqrt(xd*xd + yd*yd + zd*zd);
											*(n2 + 0) += (float)(xd / len);
											*(n2 + 1) += (float)(yd / len);
											*(n2 + 2) += (float)(zd / len);
										}

									// --- increment
										norm_count++;

								}
						}

					// ---- average unit normal (norm_count never zero!)
						fnc = (float)norm_count;

						*(n2 + 0) /= fnc;
						*(n2 + 1) /= fnc;
						*(n2 + 2) /= fnc;

					// ---- flip to left handed
						*(n2 + 2) *= -1;

					// --- increment
						n2 += 3;

				}

		}


	return VS_OK;
}


// ---------- Mesh_Normals ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note invoked by - 
	s2::Model::Acquire_CalculateDemoLuminafields
	s3::Model::AcquireCalculateDemoLuminafields

\note contains two verions of the calculate normal iterations

	The first is the old version - 
	
      in which the unit length needs to be calculated within inner
	  accumulate normal loop and when calculating actual normal as
	  the y difference results in variable normal magnitude.

*/
INT ObjConfig::Mesh_Normals( // VsVector3 version
	VsVector3 normal[],
	UINT      stride,
	VsVector3 position[],
	FLOAT     width,
	FLOAT     depth
)
{

	// ---- buffer
		int cols       = stride;
		int rows       = stride;
		int row_stride = cols;
		int extent     = cols * rows;


	// ---- current surface normal 
		VsVector3 *n2 = &normal[0];


	// ---- used to calc unit normal
		#ifdef VS_DEBUG_FLAG
		double x1, y1, z1;
		double x2, y2, z2;
		double x3, y3, z3;
		double v1x, v1y, v1z;
		double v2x, v2y, v2z;
		double xd, yd, zd;
		double len;
		#else
		VsVector3  v1d, v3d, vcp;
		VsVector3  v1, v2, v3;
		VsVector3  zero = { 0, 0, 0 };
		#endif
		double min_tolr = 0.0001f;


	// ---- tables
		int  ro[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // offsets
		bool gs[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // grid states


	//---- for each cel...
		int   last_row_offset, this_row_offset, next_row_offset;
		int   lro, tro, nro;


	//---- rows
		for (INT row = 0; row < rows; row++)
		{

				last_row_offset = (row - 1) * row_stride;
				this_row_offset = row       * row_stride;
				next_row_offset = (row + 1) * row_stride;

			// ---- columns
				for (INT col = 0; col < cols; col++)
				{

					// ---- row offsets
						lro = last_row_offset + col;
						tro = this_row_offset + col;
						nro = next_row_offset + col;

					// ---- sequential grid list
						ro[0] = lro - 1; ro[1] = tro - 1; ro[2] = nro - 1;
						ro[7] = lro;                      ro[3] = nro;
						ro[6] = lro + 1; ro[5] = tro + 1; ro[4] = nro + 1;
						ro[8] = ro[0];

					// ---- set grid state (gs) flag 'within'
						for (int i = 0; i < 9; i++)
						{
							gs[i] = ro[i] >= 0 && ro[i] < extent ? true : false;
						}

					#ifdef VS_DEBUG_FLAG

					// ---- zero normal
						n2->x = 0;
						n2->y = 0;
						n2->z = 0;
						double xn = 0, yn = 0, zn = 0;

					// ---- current mesh location
						x2 = position[tro].x;
						y2 = position[tro].y;
						z2 = position[tro].z;

					// ---- sequentialy accumulate normals
						for (int i = 0; i < 8; i ++)
						{
							// ---- 'within' ?
								if ( gs[i] && gs[i + 1] )
								{

									// ---- sequential grid locations
										x1 = position[ro[i]].x;
										y1 = position[ro[i]].y;
										z1 = position[ro[i]].z;

										x3 = position[ro[i+1]].x;
										y3 = position[ro[i+1]].y;
										z3 = position[ro[i+1]].z;

									// ---- cross product
										v1y = y1 - y2;
										v2y = y3 - y2;
										if ( fabs(v1y) < min_tolr && fabs(v2y) < min_tolr)
										{
											yn -= 1;
										}
										else
										{
											v1x = x1 - x2;
											v1z = z1 - z2;
											v2x = x3 - x2;
											v2z = z3 - z2;
											xd = v1y * v2z - v1z * v2y;
											yd = v1z * v2x - v1x * v2z;
											zd = v1x * v2y - v1y * v2x;
											len = (double)sqrt(xd*xd + yd*yd + zd*zd);
											xn += (float)(xd / len);
											yn += (float)(yd / len);
											zn += (float)(zd / len);
											float bert = 1;
										}
								}
						}

					// ---- normalise
						len = (double)sqrt(xn*xn + yn*yn + zn*zn);
						n2->x = (float)(xn / len);
						n2->y = (float)(yn / len);
						n2->z = (float)(zn / len);

					// ---- flip to match left handed build
						*n2 *= -1;

					#else

					// ---- zero normal
						*n2 = zero;

					// ---- sequentialy accumulate current location normals
						v2 = *(position + tro); 
						for (int i = 0; i < 8; i ++)
						{
							// ---- 'within' ?
								if ( gs[i] && gs[i + 1] )
								{

									// ---- sequential grid vectors
										v1 = *(position + ro[i]);
										v3 = *(position + ro[i + 1]);

									// ---- cross product
										v1d = v1 - v2;
										v3d = v3 - v2;
										v1d.Normalise();
										v3d.Normalise();
										vcp.CrossProduct(&v1d, &v3d);
										vcp.Normalise();

									// ---- add
										*n2 += vcp;
								}
						}

					// ---- normalise
						n2->Normalise();

					// ---- flip to match left handed build
						*n2 *= -1;

					#endif

					// ---- incr
						n2++;

				}

		}

	return VS_OK;
}


// ---------- Mesh_Filter ----------
INT ObjConfig::Mesh_Filter(
		FLOAT *y_array,
		UINT   y_stride,
		FLOAT *f_array,
		UINT   f_extent,
		FLOAT  f_rmu
	)
{

	// ---- ok extent
		switch (f_extent)
		{
			case 9:   // 3^2
			case 25:  // 5^2
			case 49:  // 7^2
				break;
			default:
				return VS_ERROR;
		}
	

	// ---- row/column size
		INT f_size = (int)sqrt((float)f_extent);


	// ---- y buffer
		INT y_extent = y_stride * y_stride;
		INT y_offset = 0;
		INT y_index  = 0;
		FLOAT y = 0;


	// ---- filter
		INT f_dby2  = (f_size - 1) / 2;
		INT f_index = 0;
		INT f_rows, f_row_index, f_row_offset;
		INT f_cols, f_col_offset;


	// ---- create filter array index offset table
		INT *f_offset = new INT[f_extent];
		for (f_rows = 0; f_rows < f_size; f_rows++)
		{
			f_row_index = f_rows * f_size;
			f_row_offset = (f_rows - f_dby2) * y_stride;
			for (f_cols = 0; f_cols < f_size; f_cols++)
			{
				f_index = f_row_index + f_cols;
				f_col_offset = f_row_offset + (f_cols - f_dby2);
				*(f_offset + f_index) = f_col_offset;
			}
		}


	// ---- for each y cel
		FLOAT *y_filtered = new FLOAT[y_extent];
		for (y_index = 0; y_index<y_extent; y_index++)
		{
			// ---- apply filter
				y = 0;
				for (f_index = 0; f_index < (int)f_extent; f_index++)
				{
					// ---- within ?
						y_offset = *(f_offset + f_index);
						if (y_offset >= 0 && y_offset < y_extent)
						{
							y += *(y_array + y_offset) * *(f_array + f_index);
						}
				}
				*(y_filtered + y_index) = 0;
		}


	// ---- interpolate with original
		for (y_index =0; y_index<y_extent; y_index++)
		{
			*(y_array + y_index) = (1-f_rmu) * *(y_array + y_index) +
										f_rmu * *(y_filtered + y_index);
		}


	// ---- delete
		delete [] y_filtered;
		delete [] f_offset;


	return VS_OK;
}


// ---------- Mesh_Fractal ----------
/*

note: Function mesh (e.g. fractal) have a n^2+1 width
      and the destination surface mesh has n^2 width, so
      the code below uses a different stride for get & put.

	  All other mesh are n^2.
*/
INT ObjConfig::Mesh_Fractal(
		FLOAT *dest_y_array,
		UINT   dest_stride,
		FLOAT *function_param_array
	)
{

	// ----- local
		INT result = VS_OK;


	// ---- init per mesh fractal
		if ( vs_system::ObjConfig::MESH_FRACTAL == vs_system::ObjConfig::MESH_FRACTAL_DS1 )
		{

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
						dest_stride-1,		// power of 2 ( +1 for subdivision )
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
						dest_stride,
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
		else // vs_system::ObjConfig::MESH_FRACTAL_DS2
		{


			// ---- fractal function parameters
				INT     random_seed    = (INT)*(function_param_array+1);
				FLOAT   range          = 100; // *(function_param_array + 2);
				FLOAT   rough          = *(function_param_array + 3);
				FLOAT   filter_scalar  = *(function_param_array + 4);
				FLOAT   nada           = *(function_param_array + 5);
				FLOAT   y_offset       = *(function_param_array + 6);
				FLOAT   y_peak         = *(function_param_array + 7);


			// ---- calc y fractal function
					vs_system::FractalDS2 fractal;
					fractal.Create(
						dest_y_array,
						dest_stride,
						random_seed,
						range,
						rough
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
						dest_stride,
						filter_array,
						9,
						filter_scalar
					);

			// ---- process
				unsigned int extent = dest_stride * dest_stride;
				unsigned int c;
				float y;

			// ---- minimax
				float min = 99999999.0f;
				float max = -min;
				for (c = 0; c < extent; c++)
				{
					y = *(dest_y_array + c);
					min = min > y ? y : min;
					max = max < y ? y : max;
				}
				float align = (max + min) / 2;


			// ---- offset
				for (c = 0; c < extent; c++)
				{
					*(dest_y_array + c) += y_offset - align;
				}


			// ---- clip
				max += y_peak - align;
				for (c = 0; c < extent; c++)
				{
					y = *(dest_y_array + c);
					*(dest_y_array + c) = y < 0 ? 0 : (y > max ? max : y);
				}

		}

	return VS_OK;
}


// ---- output test image
	/*
	BOOL test_image = FALSE;
	if ( test_image )
	{

		using namespace vs_system;

		float *map = dest_y_array;
		int   size = dest_stride;

		float scalar = 255 / (max - min);

		ImgJpegLib *img_jpeg_lib = new ImgJpegLib();
		ImgBuffer  *img_buffer   = new ImgBuffer(size, size, 3);
		img_buffer->CreateBuffer();

		unsigned char *buf_ptr = img_buffer->GetBuffer();
		for (int row = 0; row < size; row++)
		{
			for (int col = 0; col < size; col++)
			{

				int   map_index = row * size + col;
				float map_value = map[map_index] * scalar - (min * scalar);

				map_value = map_value < 0 ? 0 : (map_value > 255 ? 255 : map_value);

				unsigned char buf_value = (unsigned char)map_value;
				*(buf_ptr    ) = buf_value;
				*(buf_ptr + 1) = buf_value;
				*(buf_ptr + 2) = buf_value;
				buf_ptr += 3;

			}
		}

		img_jpeg_lib->WriteToDisk(img_buffer, "test.jpg");

		img_buffer->DestroyBuffer();
		delete img_buffer;
		delete img_jpeg_lib;

	}
	*/


// ---------- Mesh_YClip ----------
INT ObjConfig::Mesh_YClip(
	FLOAT *y_array,
	UINT   stride,
	FLOAT  y_min,
	FLOAT  y_max
	)
{

	// ---- check
		if ( y_min > y_max ) return VS_OK;


	// ---- local
		INT  rows   = stride;
		INT  cols   = stride;
		INT  extent = rows * cols;
		int  c;
		float y; 


	//---- minmax
		for ( c=0; c<int(extent); c++ )
		{
			y  = *(y_array + c);
			y  = y < y_min ? y_min : (y > y_max ? y_max : y);
			y -= y_min;
			*(y_array + c)  =  y;
		}

	return VS_OK;
}


// ---------- Mesh_YOffset ----------
INT ObjConfig::Mesh_YOffset(
	FLOAT *y_array,
	UINT   stride,
	FLOAT  y_offset
	)
{
	INT extent = stride * stride;
	for ( int c=0; c<int(extent); c++ )
	{
		*(y_array + c) -= y_offset; 
	}
	return VS_OK;
}


// ---------- Mesh_YScalar ----------
INT ObjConfig::Mesh_YScalar(
	FLOAT *y_array,
	UINT   stride,
	FLOAT  y_scalar
	)
{

	// ---- local
		INT  rows   = stride;
		INT  cols   = stride;
		INT  extent = rows * cols;
		int  c;


	//---- minmax
		for ( c=0; c<int(extent); c++ )
		{
			*(y_array + c) *= y_scalar;
		}

	return VS_OK;
}


// ---------- Callback_Mesh_GetIndexBuffer ----------
INT ObjConfig::Callback_Mesh_GetIndexBuffer(vs_system::ElmContext *context)
{

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();
		LONG  *ld = context->GetLongParameter();


	// ---- get supplied and parameter data
		UINT mesh_type = (UINT)pd[MESH_TYPE];
		UINT mesh_rows = (UINT)pd[MESH_ROWS];
		UINT mesh_cols = (UINT)pd[MESH_COLS];


	// ---- vertex
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_count  = context->GetVertexCount();
		UINT vertex_format = context->GetVertexFormat();


	// ---- buffer
		INT numIndices = (mesh_cols * 2) * (mesh_rows - 1) + (mesh_rows - 2);
		#if VS_INDEX_BUFFER_FORMAT == 32
		DWORD *i = context->GetIndexBufferPtr();
		#else
		WORD *i = context->GetIndexBufferPtr();
		#endif


	// ---- fill
		int index = 0;
		for ( UINT z = 0; z < mesh_rows - 1; z++ )
		{

			// ---- even rows move left to right, odd rows move right to left.
				if ( z % 2 == 0 )
				{

					// ---- even row
						INT x;
						for ( x = 0; x < (INT)mesh_cols; x++ )
						{
							*(i + index++) = x + (z * mesh_cols);
							*(i + index++) = x + (z * mesh_cols) + mesh_cols;
						}

					// ---- insert degenerate vertex if this isn't the last row
						if ( z != mesh_rows - 2)
						{
							*(i + index++) = --x + (z * mesh_cols);
						}
				} 
				else
				{
					// ---- odd row
						INT x;
						for ( x = (INT)mesh_cols - 1; x >= 0; x-- )
						{
							*(i + index++) = x + (z * mesh_cols);
							*(i + index++) = x + (z * mesh_cols) + mesh_cols;
						}

					// ---- insert degenerate vertex if this isn't the last row
						if ( z != mesh_rows - 2)
						{
							*(i + index++) = ++x + (z * mesh_cols);
						}
				}
		}

	return VS_OK;
}

