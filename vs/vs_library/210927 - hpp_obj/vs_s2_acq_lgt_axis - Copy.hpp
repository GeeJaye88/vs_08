////////////////////////////////////////////////////////////////////////////////

// ---------- vs_s2_acq_lgt_axis.hpp ----------
/*!
\file vs_s2_acq_lgt_axis.hpp
\brief
\author Gareth Edwards
\note

Create a line based graphical 3D XYZ box s3 camera rig based on box dimensions. 

*/


#pragma once


// ---------- Callback_S2_AcqLgtAxis_Configure ----------
INT ObjConfig::Callback_S2_AcqLgtAxis_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *param = context->GetFloatParameter();

	// ---- get supplied parameter data

	// ---- flags
		UINT axis_flag = (UINT)param[ObjConfig::MESH_AL_FLAG_1]; 
		UINT light_flag = (UINT)param[ObjConfig::MESH_AL_FLAG_2]; 
		UINT radius_flag = (UINT)param[ObjConfig::MESH_AL_FLAG_3];

	// ---- lines
		UINT num_lines =
			( axis_flag   ? 360 * 3                   : 0 ) +  // 360 * 3 line 360 degree compass
			( axis_flag   ?   5 * MESH_AL_LIGHT_COUNT : 0 ) +  // 5 line arrows
			( axis_flag   ?  51 * MESH_AL_LIGHT_COUNT : 0 ) +  // 45 line 45 degree arcs plus 2 * 3 line arrows
			( light_flag  ?   1 * MESH_AL_LIGHT_COUNT : 0 ) +  // 1 line to circle
			( light_flag  ?  16 * MESH_AL_LIGHT_COUNT : 0 ) +  // 16 line light circle
			( radius_flag ? 180 * 1                   : 0 );   // 180 * 1 line 360 degree outer circle

	// ---- atlas page
		context->SetAtlasPage(0, 0);

	// ---- overide ?
		UINT format = (UINT)(VS_XYZ | VS_DIFFUSE);
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(FALSE);
			context->SetColourVertex(TRUE);
			context->SetMultiState(FALSE);
			context->SetSolid(FALSE);
			context->SetTextured(FALSE);
			context->SetVisible(TRUE);
		}

	// ---- index buffer
		context->SetIndexCount(num_lines * 2);
		context->SetIndexBufferCallback(Callback_S2_AcqLgtAxis_GetIndexBuffer);

	// ---- primitive type
		context->SetPrimitiveType(VS_LINELIST);

	// ---- vertex buffer
		context->SetVertexCount(num_lines * 2);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_S2_AcqLgtAxis_GetVertexBuffer);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
			);
		context->SetPrimitiveCount(num_lines);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ---------- Callback_S2_AcqLgtAxis_GetVertexBuffer ----------
INT ObjConfig::Callback_S2_AcqLgtAxis_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *param = context->GetFloatParameter();

	// ---- acquire (1) or explore (2)
		INT mode = (INT)param[ObjConfig::MESH_AL_MODE];

	// ---- surface deimensions
		FLOAT width  = param[ObjConfig::MESH_AL_WIDTH];
		FLOAT height = param[ObjConfig::MESH_AL_HEIGHT];
		FLOAT depth  = param[ObjConfig::MESH_AL_DEPTH];

	// ---- offset rel to align with surface
		FLOAT x_off  = param[ObjConfig::MESH_AL_XOFF];
		FLOAT y_off  = param[ObjConfig::MESH_AL_YOFF];
		FLOAT z_off  = param[ObjConfig::MESH_AL_ZOFF];

	// ---- flags
		UINT axis_flag   = (UINT)param[ObjConfig::MESH_AL_FLAG_1];
		UINT light_flag  = (UINT)param[ObjConfig::MESH_AL_FLAG_2];
		UINT radius_flag = (UINT)param[ObjConfig::MESH_AL_FLAG_3];

	// ----- acquire light
		UINT  light_offset = ObjConfig::MESH_AL_LIGHT_OFFSET;
		FLOAT rgba_colour      = param[light_offset];
		FLOAT param_intensity  = param[light_offset + 1];
		FLOAT param_distance   = param[light_offset + 2];
		FLOAT param_elevation  = param[light_offset + 3];
		FLOAT param_azimuth    = param[light_offset + 4];


	// ---- set light position

		// ---- light angle stuff
			FLOAT t2r = param_elevation * (FLOAT)VS_RADIANS_PER_DEGREE;
			FLOAT phi = param_azimuth + 90.0f;
			FLOAT p2r = phi * (FLOAT)VS_RADIANS_PER_DEGREE;

		// ---- light rotate about x axis
			// y' = y*cos q - z*sin q
			// z' = y*sin q + z*cos q
			FLOAT xt =  0;
			FLOAT yt =  param_distance * sinf(t2r);
			FLOAT zt = -param_distance * cosf(t2r);

		// ---- light position
			VsVector3 lp = { xt, yt, zt };


	// ---- vertex
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_count  = context->GetVertexCount();
		UINT vertex_format = context->GetVertexFormat();
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- vertices
		struct vtx
		{
			float x, y, z, d;
		};


	// ---- gfx proportional to arrow width
		FLOAT aw = depth * 0.1f;
		FLOAT awby2 = aw / 2;
		FLOAT awby3 = aw / 3;

	// ---- AXIS
		if ( axis_flag )
		{

			// ---- 360 * 3 line 360 degree compass - proportional to size of surface
				FLOAT dby2   = depth / 2 * 1.25f;
				FLOAT radius = (FLOAT)sqrt(dby2*dby2 + dby2 * dby2);
				FLOAT zi = radius;
				FLOAT zo = radius + awby2;
				for (UINT b = 0; b < 360; b++)
				{

					// ---- inner & outer radii, and per angle blip

						// angle
						FLOAT fb = (FLOAT)b;
						FLOAT rad[6] = { fb-0.5f, fb+0.5f, fb, fb, fb-0.5f, fb+0.5f };
						// note: was also rotated by param_azimuth
						for (UINT i = 0; i < 6; i++) rad[i] *= (FLOAT)VS_RADIANS_PER_DEGREE;

						// z - blips @ 10, 5 & 1 degrees
						FLOAT mu = b % 10 == 0 ? 1 : (b % 5 == 0 ? 0.66f : 0.33f);
						FLOAT z[6] = { zi, zi, zi, (1-mu)*zi+mu*zo, zo, zo };

						// Y rotate
						// x' = z*sin q + x*cos q
						// z' = z*cos q - x*sin q
						vtx blip_rotated[6];
						for (UINT v = 0; v < 6; v++)
						{
							FLOAT radian = rad[v];
							blip_rotated[v].x = z[v] * (FLOAT)sin(radian) + x_off;
							blip_rotated[v].y = y_off;
							blip_rotated[v].z = z[v] * (FLOAT)cos(radian) + z_off;
							blip_rotated[v].d = rgba_colour;
						}

						// append
						size_t size_of_blip_rotated = sizeof(blip_rotated);
						memcpy((VOID *)f, (VOID *)blip_rotated, (INT)size_of_blip_rotated);
						f += (INT)size_of_blip_rotated / sizeof(FLOAT);
				}


			// ---- 5 line arrows - proportional to size of surface
				zi = zo;
				zo = -lp.z;
				float zi_1 = zi + depth / 3;
				float zi_2 = zi_1 + awby2 * 1.25f;
				for (UINT a = 0; a < MESH_AL_LIGHT_COUNT; a++)
				{

					// ---- arrow

						// angle
						FLOAT angle = (FLOAT)a * 90 + param_azimuth;
						FLOAT radian = angle * (FLOAT)VS_RADIANS_PER_DEGREE;

						// init
						vtx arrow[10] =
							{
								{      0, 0,     zi, 0 },
								{      0, 0,   zi_1, 0 },
								{ -awby3, 0,   zi_1, 0 },
								{      0, 0,   zi_2, 0 },
								{  awby3, 0,   zi_1, 0 },
								{      0, 0,   zi_2, 0 },
								{ -awby3, 0,   zi_1, 0 },
								{  awby3, 0,   zi_1, 0 },
								{      0, 0,   zi_2, 0 },
								{      0, 0,     zo, 0 }
							};

						// Y rotate
						// x' = z*sin q + x*cos q
						// z' = z*cos q - x*sin q
						vtx arrow_rotated[10];
						for (UINT v = 0; v < 10; v++)
						{
							arrow_rotated[v].x = arrow[v].z * (FLOAT)sin(radian) + arrow[v].x * (FLOAT)cos(radian) + x_off;
							arrow_rotated[v].y = arrow[v].y + y_off;
							arrow_rotated[v].z = arrow[v].z * (FLOAT)cos(radian) - arrow[v].x * (FLOAT)sin(radian) + z_off;
							arrow_rotated[v].d = rgba_colour;
						}

						// append
						size_t size_of_arrow_rotated = sizeof(arrow_rotated);
						memcpy((VOID *)f, (VOID *)arrow_rotated, (INT)size_of_arrow_rotated);
						f += (INT)size_of_arrow_rotated / sizeof(FLOAT);
				}


			// ---- 45 line 45 degree arcs plus 2 * 3 line arrows
				float zi_3 = zi + depth / 6 - awby2 * 0.625f;
				float zi_4 = zi + depth / 6 + awby2 * 0.625f;
				float zi_5 = zi + depth / 6;
				vtx arc_seg[2] =
				{
					{ 0, 0, zi_5, 0 },
					{ 0, 0, zi_5, 0 },
				};
				for (UINT a = 0; a < MESH_AL_LIGHT_COUNT; a++)
				{
					FLOAT angle = (FLOAT)a * 90 + param_azimuth - 22.5f;

					// ---- arc
						for (UINT b = 0; b < 45; b++)
						{

								// this and next angles
								FLOAT radian_1 = ( (FLOAT) b    + angle) * (FLOAT)VS_RADIANS_PER_DEGREE;
								FLOAT radian_2 = ( (FLOAT)(b+1) + angle) * (FLOAT)VS_RADIANS_PER_DEGREE;

								// Y rotate
								// x' = z*sin q + x*cos q
								// z' = z*cos q - x*sin q
								vtx blip_rotated[2];
								for (UINT v = 0; v < 2; v++)
								{
									FLOAT radian = v == 0 ? radian_1 : radian_2;
									blip_rotated[v].x = arc_seg[v].z * (FLOAT)sin(radian) + arc_seg[v].x * (FLOAT)cos(radian) + x_off;
									blip_rotated[v].y = y_off;
									blip_rotated[v].z = arc_seg[v].z * (FLOAT)cos(radian) - arc_seg[v].x * (FLOAT)sin(radian) + z_off;
									blip_rotated[v].d = rgba_colour;
								}

								// append
								size_t size_of_blip_rotated = sizeof(blip_rotated);
								memcpy((VOID *)f, (VOID *)blip_rotated, (INT)size_of_blip_rotated);
								f += (INT)size_of_blip_rotated / sizeof(FLOAT);
						}

					// ---- arrows
						vtx arrow[12] =
						{
								{      0, 0,   zi_3, 0 },
								{      0, 0,   zi_4, 0 },
								{      0, 0,   zi_4, 0 },
								{  awby3, 0,   zi_5, 0 },
								{  awby3, 0,   zi_5, 0 },
								{      0, 0,   zi_3, 0 },
								{      0, 0,   zi_3, 0 },
								{      0, 0,   zi_4, 0 },
								{      0, 0,   zi_4, 0 },
								{ -awby3, 0,   zi_5, 0 },
								{ -awby3, 0,   zi_5, 0 },
								{      0, 0,   zi_3, 0 },
						};

						// Y rotate
						vtx arrow_rotated[12];
						for (UINT v = 0; v < 12; v++)
						{
							FLOAT angle = v < 6 ? (FLOAT)a * 90 + param_azimuth + 22.5f : (FLOAT)a * 90 + param_azimuth - 22.5f;
							FLOAT radian = angle * (FLOAT)VS_RADIANS_PER_DEGREE;
							arrow_rotated[v].x = arrow[v].z * (FLOAT)sin(radian) + arrow[v].x * (FLOAT)cos(radian) + x_off;
							arrow_rotated[v].y = y_off;
							arrow_rotated[v].z = arrow[v].z * (FLOAT)cos(radian) - arrow[v].x * (FLOAT)sin(radian) + z_off;
							arrow_rotated[v].d = rgba_colour;
						}
						// append
						size_t size_of_arrow_rotated = sizeof(arrow_rotated);
						memcpy((VOID *)f, (VOID *)arrow_rotated, (INT)size_of_arrow_rotated);
						f += (INT)size_of_arrow_rotated / sizeof(FLOAT);
				}

		}


	// ---- LIGHTS
		if ( light_flag )
		{

			// ---- lights @ l * 90 degrees
				FLOAT y = lp.y;
				FLOAT z = lp.z;
				for (UINT l = 0; l < MESH_AL_LIGHT_COUNT; l++)
				{

					// ---- 1 * line to circle

						// angle
						FLOAT angle = (FLOAT)l * 90 + param_azimuth;
						FLOAT radian = angle * (FLOAT)VS_RADIANS_PER_DEGREE;

						// init
						vtx light[2] =
							{
								{ 0, 0, z, 0 },
								{ 0, y, z, 0 }
							};

						// rotate
						vtx light_rotated[2];
						for (UINT v = 0; v < 2; v++)
						{
							light_rotated[v].x = light[v].z * (FLOAT)sin(radian) + light[v].x * (FLOAT)cos(radian) + x_off;
							light_rotated[v].y = light[v].y + y_off;
							light_rotated[v].z = light[v].z * (FLOAT)cos(radian) - light[v].x * (FLOAT)sin(radian) + z_off;
							light_rotated[v].d = rgba_colour;
						}

						// append
						size_t size_of_light_rotated = sizeof(light_rotated);
						memcpy((VOID *)f, (VOID *)light_rotated, (INT)size_of_light_rotated);
						f += (INT)size_of_light_rotated / sizeof(FLOAT);


					// ----- 16 line light circle
						y = lp.y;
						FLOAT radius = awby2;
						vtx light_circle[32];
						for (UINT i = 0; i < 16; i++)
						{

								// this and next angle
								FLOAT radian_1 = ( (FLOAT) i    * 22.5f ) * (FLOAT)VS_RADIANS_PER_DEGREE;
								FLOAT radian_2 = ( (FLOAT)(i+1) * 22.5f ) * (FLOAT)VS_RADIANS_PER_DEGREE;

								// Z rotate (y == 0)
								FLOAT x1 = -radius * (FLOAT)sin(radian_1);
								FLOAT y1 =  radius * (FLOAT)cos(radian_1);
								FLOAT x2 = -radius * (FLOAT)sin(radian_2);
								FLOAT y2 =  radius * (FLOAT)cos(radian_2);

								// Y rotate
								light_circle[i*2  ].x =  x1 * (FLOAT)cos(radian) + light_rotated[1].x;
								light_circle[i*2  ].y =  y1                        + y;
								light_circle[i*2  ].z = -x1 * (FLOAT)sin(radian) + light_rotated[1].z;
								light_circle[i*2  ].d =  rgba_colour;

								light_circle[i*2+1].x =  x2 * (FLOAT)cos(radian) + light_rotated[1].x;
								light_circle[i*2+1].y =  y2                        + y;
								light_circle[i*2+1].z = -x2 * (FLOAT)sin(radian) + light_rotated[1].z;
								light_circle[i*2+1].d =  rgba_colour;
						}

						// append
						size_t size_of_light_circle = sizeof(light_circle);
						memcpy((VOID *)f, (VOID *)light_circle, (INT)size_of_light_circle);
						f += (INT)size_of_light_circle / sizeof(FLOAT);

				}
			
		}

	// ---- radius
		if ( radius_flag )
		{

			// ---- 180 * 1 line 360 degree outer circle
				for (UINT b = 0; b < 360; b += 2)
				{

						// this and next angles
						FLOAT radian_1 = ( (FLOAT)b     + param_azimuth ) * (FLOAT)VS_RADIANS_PER_DEGREE;
						FLOAT radian_2 = ( (FLOAT)(b+2) + param_azimuth ) * (FLOAT)VS_RADIANS_PER_DEGREE;

						// init
						vtx blip[2] =
							{
								{ 0, 0, lp.z, 0 },
								{ 0, 0, lp.z, 0 },
							};

						// Y rotate
						// x' = z*sin q + x*cos q
						// z' = z*cos q - x*sin q
						vtx blip_rotated[2];
						for (UINT v = 0; v < 2; v++)
						{
							FLOAT radian = v == 0 ? radian_1 : radian_2;
							blip_rotated[v].x = blip[v].z * (FLOAT)sin(radian) + blip[v].x * (FLOAT)cos(radian) + x_off;
							blip_rotated[v].y = y_off;
							blip_rotated[v].z = blip[v].z * (FLOAT)cos(radian) - blip[v].x * (FLOAT)sin(radian) + z_off;
							blip_rotated[v].d = rgba_colour;
						}

						// append
						size_t size_of_blip_rotated = sizeof(blip_rotated);
						memcpy((VOID *)f, (VOID *)blip_rotated, (INT)size_of_blip_rotated);
						f += (INT)size_of_blip_rotated / sizeof(FLOAT);
				}

		}

	return VS_OK;
}


// ---------- Callback_S2_AcqLgtAxis_GetIndexBuffer ----------
INT ObjConfig::Callback_S2_AcqLgtAxis_GetIndexBuffer(
		vs_system::ElmContext *context
	)
{
	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();

	// ---- num line pairs
		UINT num_lines = context->GetPrimitiveCount();

	// ---- buffer
		DWORD *ib = context->GetIndexBufferPtr();
		DWORD  ic = 0;

	// ----
		for (UINT line = 0; line < num_lines; line++)
		{
			*(ib+0) = ic++;
			*(ib+1) = ic++;
			ib += 2;
		}

	return VS_OK;
}