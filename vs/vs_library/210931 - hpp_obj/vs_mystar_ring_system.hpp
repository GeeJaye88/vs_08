////////////////////////////////////////////////////////////////////////////////

// ---------- vs_mystar_ring_system.hpp ----------
/*!
\file vs_mystar_ring_system.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_MyStar_Ring_System_Configure ----------
/*!

*/
INT ObjConfig::Callback_MyStar_Ring_System_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- get supplied and parameter data
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- param
		UINT  cols = (UINT)pd[RING_SYSTEM_COLS];        // per ring
		UINT  rows = (UINT)pd[RING_SYSTEM_ROWS];
		UINT  type = (UINT)pd[RING_SYSTEM_TYPE];        // 0 - wireframe, 1 - solid, 2 - etc.
		UINT  ap_ref = (UINT)pd[RING_SYSTEM_AP_REF];    // atlas page ref


		INT   number = (INT)pd[RING_SYSTEM_NUMBER];     // of active (non zero width rings)
		FLOAT rad_interp[5][2] =                         // geo / tex map interpolated radii
			{
				{
					pd[RING_SYSTEM_RAD_MIN_0],
					pd[RING_SYSTEM_RAD_MAX_0]
				},
				{
					pd[RING_SYSTEM_RAD_MIN_1],
					pd[RING_SYSTEM_RAD_MAX_1]
				},
				{
					pd[RING_SYSTEM_RAD_MIN_2],
					pd[RING_SYSTEM_RAD_MAX_2]
				},
				{
					pd[RING_SYSTEM_RAD_MIN_3],
					pd[RING_SYSTEM_RAD_MAX_3]
				},
				{
					pd[RING_SYSTEM_RAD_MIN_4],
					pd[RING_SYSTEM_RAD_MAX_4]
				}
			};


	// ---- atlas page
		context->SetAtlasPage(0, ap_ref);


	// IF -- not overide -- THEN
		UINT format = (UINT)(VS_XYZ | VS_DIFFUSE); // wireframe
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(TRUE);
			context->SetColourVertex(FALSE);
			context->SetMultiState(FALSE);

			switch ( type )
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
					context->SetTextured(ap_ref == 0 ? FALSE : TRUE);
					break;
			}

			context->SetVisible(TRUE);
		}


	// ---- index buffer
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


	// ---- count - and then store - non-zero rings
		number = 0;
		for (INT i = 0; i < 5; i++)
		{
			FLOAT width = rad_interp[i][1] - rad_interp[i][0];
			if (width > 0.01f) number++;
		}
		pd[RING_SYSTEM_NUMBER] = (FLOAT)number;


	// ---- vertex buffer
		context->SetVertexCount(cols * rows * 6 * number);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_MyStar_Ring_System_GetVertexBuffer);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(FLOAT)
			);
		context->SetPrimitiveCount(cols * rows * 2 * number);
		context->SetPrimitiveType(VS_TRIANGLELIST);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ----------  Callback_MyStar_Ring_System_GetVertexBuffer ----------
/*!

*/
INT ObjConfig::Callback_MyStar_Ring_System_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- lambda ------------------------------------------------------------

		auto random_value = [](FLOAT range)
		{
			return (FLOAT)(rand() % 100) / (FLOAT)100 * range;
		};


	// ---- get supplied and parameter data
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- param
		UINT  cols = (UINT)pd[RING_SYSTEM_COLS];        // per ring
		UINT  rows = (UINT)pd[RING_SYSTEM_ROWS];
		UINT  type = (UINT)pd[RING_SYSTEM_TYPE];        // 0 - wireframe, 1 - solid, 2 - etc.

		UINT  ap_ref  = (UINT)pd[RING_SYSTEM_AP_REF];   // atlas page ref
		UINT  ap_sect = (UINT)pd[RING_SYSTEM_AP_SECT];  // atlas page section

		FLOAT red    = pd[RING_SYSTEM_RED];             // tint
		FLOAT green  = pd[RING_SYSTEM_GREEN];
		FLOAT blue   = pd[RING_SYSTEM_BLUE];

		INT   number = (INT)pd[RING_SYSTEM_NUMBER];     // of active (non zero width rings)

		FLOAT rad_min = pd[RING_SYSTEM_RAD_MIN];        // geom radii
		FLOAT rad_max = pd[RING_SYSTEM_RAD_MAX];

		FLOAT rad_interp[5][2] =                         // geo / tex map interpolated radii
			{
				{
					pd[RING_SYSTEM_RAD_MIN_0],
					pd[RING_SYSTEM_RAD_MAX_0]
				},
				{
					pd[RING_SYSTEM_RAD_MIN_1],
					pd[RING_SYSTEM_RAD_MAX_1]
				},
				{
					pd[RING_SYSTEM_RAD_MIN_2],
					pd[RING_SYSTEM_RAD_MAX_2]
				},
				{
					pd[RING_SYSTEM_RAD_MIN_3],
					pd[RING_SYSTEM_RAD_MAX_3]
				},
				{
					pd[RING_SYSTEM_RAD_MIN_4],
					pd[RING_SYSTEM_RAD_MAX_4]
				}
			};


	// ---- vertex
		UINT  vertex_size   = context->GetVertexSize();
		UINT  vertex_format = context->GetVertexFormat();
		FLOAT incr_per_row = 1 / (FLOAT)rows;
		FLOAT incr_per_col = 1 / (FLOAT)cols;


	// ---- diffuse
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} dif_col;
		dif_col.d = VS_D3DCOLOR_COLORVALUE(red, green, blue, 1);


	// ---- store
		struct vrtx
		{
			float x, y, z;
			float d;
			float xn, yn, zn;
			float u, v;
		};


	// ---- buffer
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- for each ring
		for (INT ring = 0; ring < number; ring++)
		{

			// ---- ring exists ?
				FLOAT width = rad_interp[ring][1] - rad_interp[ring][0];
				if ( width > 0.01f )
				{

					// ---- init gen aligned with x axis
						FLOAT i_min = (FLOAT)rad_interp[ring][0];
						FLOAT i_max = (FLOAT)rad_interp[ring][1];

					// ---- v tex map offset
						FLOAT u_offset = ap_sect == 1 || ap_sect == 3 ? 0 : 0.505f;
						FLOAT v_offset = ap_sect > 2 ? 0.5f : 0;

					// ---- sweep gen		
						FLOAT row_width = 0;
						for (UINT row = 0; row < rows; row++)
						{

							// ---- generator
								FLOAT min = (FLOAT)row * incr_per_row;
								FLOAT max = min + incr_per_row;

								FLOAT row_i_min = (1 - min) * i_min + min * i_max;
								FLOAT row_i_max = (1 - max) * i_min + max * i_max;
								FLOAT row_x_min = (1 - row_i_min) * rad_min + row_i_min * rad_max;
								FLOAT row_x_max = (1 - row_i_max) * rad_min + row_i_max * rad_max;
								FLOAT row_v_min = row_i_min;
								FLOAT row_v_max = row_i_max;

								FLOAT x0 = row_x_min;
								FLOAT x1 = row_x_max;
								FLOAT z0 = 0;
								FLOAT z1 = 0;
								FLOAT u0 = u_offset;
								FLOAT v0 = row_v_min * 0.495f + v_offset;
								FLOAT v1 = row_v_max * 0.495f + v_offset;


							// ---- cels
								for (UINT col = 0; col < cols; col++)
								{

									// ---- forward stuff
										FLOAT ang_rad = ( (FLOAT)(col+1) * incr_per_col ) * 360 * (FLOAT)VS_RADIANS_PER_DEGREE;
										FLOAT x2 =  row_x_min * cosf(ang_rad);
										FLOAT x3 =  row_x_max * cosf(ang_rad);
										FLOAT z2 = -row_x_min * sinf(ang_rad);
										FLOAT z3 = -row_x_max * sinf(ang_rad);
										FLOAT u2 = ((FLOAT)(col + 1) * incr_per_col) * 0.495f + u_offset;

									// --- vertices
										FLOAT xzuv[4][4] =
										{
											{ x0, z0, u0, v0 },
											{ x1, z1, u0, v1 },
											{ x2, z2, u2, v0 },
											{ x3, z3, u2, v1 }
										};

									// ---- indices
										INT ind[2][3] = { { 0, 1, 2 }, { 1, 3, 2} };

										// ---- for each triangle
										for (UINT t = 0; t < 2; t++)
										{
												for (UINT i = 0; i < 3; i++)
												{
													// ---- geom
														*(f    ) = xzuv[ind[t][i]] [0];
														*(f + 1) = 0;
														*(f + 2) = xzuv[ind[t][i]] [1];
														f += 3;

													// ---- diffuse
														if ( vertex_format & VS_DIFFUSE )
														{
															*(f) = dif_col.f; f++;
														}

													// ---- normal
														if ( vertex_format & VS_NORMAL )
														{

															*(f)     = 0;
															*(f + 1) = 1;
															*(f + 2) = 0;
															f += 3;
														}

													// ---- uv
														if ( vertex_format & VS_TEX1 )
														{
															*(f)     = xzuv[ind[t][i]] [2];
															*(f + 1) = xzuv[ind[t][i]] [3];
															f += 2;
														}
												}
										}

										// ---- copy
											x0 = x2;
											x1 = x3;
											z0 = z2;
											z1 = z3;
											u0 = u2;
								}
						}

				}
		}

	return VS_OK;
}

