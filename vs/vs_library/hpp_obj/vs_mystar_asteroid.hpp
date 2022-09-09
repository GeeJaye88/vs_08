////////////////////////////////////////////////////////////////////////////////

// ---------- vs_mystar_asteroid.hpp ----------
/*!
\file vs_mystar_asteroid.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_MyStar_Asteroid_Configure ----------
INT ObjConfig::Callback_MyStar_Asteroid_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- get supplied and parameter data
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- param
		UINT sphere_cel_cols = (UINT)pd[SPHERE_CEL_COLS];
		UINT sphere_cel_rows = (UINT)pd[SPHERE_CEL_ROWS];
		UINT sphere_type     = (UINT)pd[SPHERE_TYPE];
		UINT sphere_ap_ref   = (UINT)pd[SPHERE_AP_REF];
		UINT sphere_number   = (UINT)pd[SPHERE_NUMBER];;


	// ---- atlas page
		context->SetAtlasPage(0, sphere_ap_ref);


	// IF -- not overide -- THEN
		UINT format = (UINT)(VS_XYZ | VS_DIFFUSE); // wireframe
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(FALSE);
			context->SetColourVertex(FALSE);
			context->SetMultiState(FALSE);

			switch ( sphere_type )
			{
				default:
				case 0: // wireframe
					format = (UINT)(VS_XYZ | VS_DIFFUSE);
					context->SetSolid(FALSE);
					context->SetTextured(FALSE);
					break;
				case 1: // solid
					format = (UINT)(VS_XYZ | VS_NORMAL);
					context->SetSolid(TRUE);
					context->SetTextured(FALSE);
					break;
				case 2: //textured
					format = (UINT)(VS_XYZ | VS_NORMAL | VS_TEX1);
					context->SetSolid(TRUE);
					context->SetTextured(sphere_ap_ref == 0 ? FALSE : TRUE);
					break;
			}

			context->SetVisible(TRUE);
		}


	// ---- index buffer
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


	// ---- vertex buffer
		context->SetVertexCount(sphere_cel_cols * sphere_cel_rows * 6 * sphere_number);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_MyStar_Asteroid_GetVertexBuffer);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(FLOAT)
			);
		context->SetPrimitiveCount(sphere_cel_cols * sphere_cel_rows * 2 * sphere_number);
		context->SetPrimitiveType(VS_TRIANGLELIST);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ----------  Callback_MyStar_Asteroid_GetVertexBuffer ----------
/*!

*/
INT ObjConfig::Callback_MyStar_Asteroid_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- lambda ------------------------------------------------------------

		auto random_value = [](FLOAT range)
		{
			return (FLOAT)(rand() % 10000) / (FLOAT)10000.0f * range;
		};


	// ---- get supplied and parameter data
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- sphere parameters
		UINT  sphere_cel_cols = (UINT)pd[SPHERE_CEL_COLS];
		UINT  sphere_cel_rows = (UINT)pd[SPHERE_CEL_ROWS];
		UINT  sphere_cols     = sphere_cel_cols + 1;
		UINT  sphere_rows     = sphere_cel_rows + 1;
		UINT  sphere_type     = (UINT)pd[SPHERE_TYPE];
		UINT  sphere_ap_ref   = (UINT)pd[SPHERE_AP_REF];
		UINT  sphere_ap_sect  = (UINT)pd[SPHERE_AP_SECT];
		FLOAT sphere_red      = pd[SPHERE_RED];
		FLOAT sphere_green    = pd[SPHERE_GREEN];
		FLOAT sphere_blue     = pd[SPHERE_BLUE];
		FLOAT sphere_radius   = pd[SPHERE_RADIUS];
		FLOAT sphere_lat_min  = -90.0f;
		FLOAT sphere_lat_max  =  90.0f;


	// ---- belt paremeters
		FLOAT sphere_rad_min  = pd[SPHERE_RAD_MIN];
		FLOAT sphere_rad_max  = pd[SPHERE_RAD_MAX];
		UINT  sphere_number   = (UINT)pd[SPHERE_NUMBER];


	// ---- vertex
		UINT  vertex_size      = context->GetVertexSize();
		UINT  vertex_format    = context->GetVertexFormat();
		FLOAT incr_vertex_rows = 1 / (FLOAT)sphere_cel_rows;
		FLOAT incr_vertex_cols = 1 / (FLOAT)sphere_cel_cols;


	// ---- total latitude radians
		FLOAT latitiude_diff_radians = (sphere_lat_max - sphere_lat_min) * (FLOAT)VS_RADIANS_PER_DEGREE;
		FLOAT latitiude_dmin_radians =  sphere_lat_min * (FLOAT)VS_RADIANS_PER_DEGREE;


	// ---- diffuse
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} white;
		white.d = VS_D3DCOLOR_COLORVALUE(sphere_red, sphere_green, sphere_blue, 1.0f);


	// ---- store
		struct vrtx
		{
			float x, y, z;
			float d;
			float xn, yn, zn;
			float u, v;
		};
		vrtx vr[11][21]; // ---- BUG


	// ---- init
		for (UINT row = 0; row < sphere_rows; row++)
		{
			FLOAT x_axis_scalar      = (FLOAT)row * (FLOAT)incr_vertex_rows;
			FLOAT x_rotation_radians = x_axis_scalar * latitiude_diff_radians + latitiude_dmin_radians;

			FLOAT y = sphere_radius * sin(x_rotation_radians);
			FLOAT z = sphere_radius * cos(x_rotation_radians);
			FLOAT v = 1 - ((FLOAT)row / (FLOAT)sphere_cel_rows) / 2 - (sphere_ap_sect == 2 ? 0 : 0.5f);

			for (UINT col = 0; col < sphere_cols; col++)
			{

				FLOAT y_axis_scalar = (FLOAT)col * incr_vertex_cols;
				FLOAT y_rotation_radians = y_axis_scalar * -360 * (FLOAT)VS_RADIANS_PER_DEGREE;
				vr[row][col].x = z * sinf(y_rotation_radians);
				vr[row][col].y = y;
				vr[row][col].z = -z * cosf(y_rotation_radians);

				if ( vertex_format & VS_DIFFUSE )
				{
					vr[row][col].d = white.f;
				}
				if ( vertex_format & VS_NORMAL )
				{
					FLOAT quotient = 1 / sphere_radius;
					vr[row][col].xn = vr[row][col].x * quotient;
					vr[row][col].yn = vr[row][col].y * quotient;
					vr[row][col].zn = vr[row][col].z * quotient;
				}
				if ( vertex_format & VS_TEX1 )
				{
					FLOAT u = (FLOAT)col / (FLOAT)sphere_cel_cols;
					vr[row][col].u = u * 0.998f + 0.001f;
					vr[row][col].v = v;
				}

			}
		}


	// ---- buffer
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- ring dimensions
		FLOAT height = sphere_lat_max - sphere_lat_min;
		FLOAT width = sphere_rad_max - sphere_rad_min;


	// ---- gaussian
		std::default_random_engine generator;

		FLOAT x_mean   = width;
		FLOAT x_stddev = x_mean / 2.0f;
		std::normal_distribution<float> x_distribution(x_mean, x_stddev);

		FLOAT y_mean   = sphere_radius * 10.0f;
		FLOAT y_stddev = y_mean / 2.0f;
		std::normal_distribution<float> y_distribution(y_mean, y_stddev);


	// ---- for each object

		for (UINT ast = 0; ast < sphere_number; ast++)
		{

			// ---- non gaussian scala and radius
				FLOAT scal =  random_value(8) / 32 + 0.2f;
				FLOAT xrad =  random_value(360) * (FLOAT)VS_RADIANS_PER_DEGREE;


			// ---- gaussian location
				FLOAT x = x_distribution(generator) + sphere_rad_min;
				FLOAT y = y_distribution(generator) - sphere_radius * 10.0f;


			// --- non gaussian rotation
				FLOAT xr = ( random_value(180) - 90 ) * (FLOAT)VS_RADIANS_PER_DEGREE;
				FLOAT yr =   random_value(360)        * (FLOAT)VS_RADIANS_PER_DEGREE;


			// ---- offset
				FLOAT zoff = -x * cos(xrad);
				FLOAT yoff =  y;
				FLOAT xoff =  x * sin(xrad);


			// ---- for each cel
				for (UINT row = 0; row < sphere_cel_rows; row++)
				{
					for (UINT col = 0; col < sphere_cel_cols; col++)
					{

						// --- load two triangles
							UINT idx[2][3][2] =
							{
								{ { row, col }, { row,     col + 1 }, { row + 1, col + 1 } },
								{ { row, col }, { row + 1, col + 1 }, { row + 1, col     } }
							};

							for (UINT i = 0; i < 2; i++)
							{
								for (UINT j = 0; j < 3; j++)
								{
									vrtx *v = &vr[idx[i][j][0]][idx[i][j][1]];

									FLOAT x1 = v->z * sinf(yr) + v->x * cosf(yr);
									//FLOAT y1 = v->y;
									FLOAT z1 = v->z * cosf(yr) - v->x * sinf(yr);

									FLOAT x2 = x1;
									FLOAT y2 = v->y * cosf(yr) - z1 * sinf(yr);
									FLOAT z2 = v->y * sinf(yr) + z1 * cosf(yr);


									*(f)     = x2 * scal + xoff;
									*(f + 1) = y2 * scal + yoff;
									*(f + 2) = z2 * scal + zoff;
									f += 3;

									if ( vertex_format & VS_DIFFUSE )
									{
										*(f) = v->d; f++;
									}

									if ( vertex_format & VS_NORMAL )
									{
										x1 = v->zn * sinf(yr) + v->xn * cosf(yr);
										//y1 = v->yn;
										z1 = v->zn * cosf(yr) - v->xn * sinf(yr);

										x2 = x1;
										y2 = v->yn * cosf(yr) - z1 * sinf(yr);
										z2 = v->yn * sinf(yr) + z1 * cosf(yr);

										*(f)     = x2;
										*(f + 1) = y2;
										*(f + 2) = z2;
										f += 3;
									}

									if ( vertex_format & VS_TEX1 )
									{
										*(f)     = v->u;
										*(f + 1) = v->v;
										f += 2;
									}
								}
							}

					}
				}
		}

	return VS_OK;
}

