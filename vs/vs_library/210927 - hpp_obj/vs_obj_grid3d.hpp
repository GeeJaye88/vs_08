////////////////////////////////////////////////////////////////////////////////

// ---------- vs_obj_grid3d.hpp ----------
/*!
\file vs_obj_grid3d.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_ObjGrid3D_Configure ----------
INT ObjConfig::Callback_ObjGrid3D_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- get supplied and parameter data
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- param
		UINT grid_cel_cols = (UINT)pd[GRID_CEL_COLS];
		UINT grid_cel_rows = (UINT)pd[GRID_CEL_ROWS];
		UINT grid_cols     = grid_cel_cols + 1;
		UINT grid_rows     = grid_cel_rows + 1;
		UINT grid_type     = (UINT)pd[GRID_TYPE];
		UINT grid_ap_ref   = (UINT)pd[GRID_AP_REF];


	// ---- atlas page
		context->SetAtlasPage(0, grid_ap_ref);


	// ---- not overide ?
		UINT format = (UINT)(VS_XYZ | VS_DIFFUSE); // wireframe
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(false);
			context->SetColourVertex(false);
			context->SetMultiState(false);
			context->SetSolid(true);
			context->SetTextured(TRUE);

			switch ( grid_type )
			{
				default:
				case 0: // wireframe
					format = (UINT)(VS_XYZ | VS_DIFFUSE);
					context->SetAlphaBlending(TRUE);
					context->SetColourVertex(TRUE);
					context->SetSolid(FALSE);
					context->SetTextured(FALSE);
					break;
				case 1: // solid
					format = (UINT)(VS_XYZ | VS_NORMAL);
					context->SetSolid(TRUE);
					context->SetTextured(FALSE);
					break;
				case 2: // textured
					format = (UINT)(VS_XYZ | VS_NORMAL | VS_TEX1);
					context->SetSolid(TRUE);
					context->SetTextured(grid_ap_ref == 0 ? FALSE : TRUE);
					break;
			}

			context->SetVisible(true);
		}


	// ---- no index buffer
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


	// ---- vertex buffer
		context->SetVertexCount(grid_cel_rows * ( (grid_cols+1) * 2 ) );
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_ObjGrid3D_GetVertexBuffer);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
			);
		context->SetPrimitiveCount(grid_cel_rows * ( (grid_cols+1) * 2) );
		context->SetPrimitiveType(VS_TRIANGLESTRIP);


		OutputDebugString("Callback_ObjGrid3D_Configure \n");


	// ---- end configuration profiling
		context->EndConfig();


	return VS_OK;
}


// ---------- Callback_ObjGrid3D_GetVertexBuffer ----------
INT ObjConfig::Callback_ObjGrid3D_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- get supplied and parameter data
		FLOAT *pd = context->GetFloatParameter();


	// ---- param
		UINT  grid_cel_cols = (UINT)pd[GRID_CEL_COLS];
		UINT  grid_cel_rows = (UINT)pd[GRID_CEL_ROWS];
		UINT  grid_cols     = grid_cel_cols + 1;
		UINT  grid_rows     = grid_cel_rows + 1;
		UINT  grid_type     = (UINT)pd[GRID_TYPE];
		UINT  grid_ap_ref   = (UINT)pd[GRID_AP_REF];
		UINT  grid_align    = (UINT)pd[GRID_ALIGN];
		FLOAT grid_red      = pd[GRID_RED];
		FLOAT grid_green    = pd[GRID_GREEN];
		FLOAT grid_blue     = pd[GRID_BLUE];
		FLOAT grid_width    = pd[GRID_WIDTH];
		FLOAT grid_depth    = pd[GRID_DEPTH];
		FLOAT grid_height   = pd[GRID_HEIGHT];

	// ---- vertex
		UINT  vertex_size   = context->GetVertexSize();
		UINT  vertex_format = context->GetVertexFormat();
		FLOAT incr_per_row  = 1 / (FLOAT)grid_cel_rows;
		FLOAT incr_per_col  = 1 / (FLOAT)grid_cel_cols;


	// ---- diffuse colour
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} diffuse;
		diffuse.d = VS_D3DCOLOR_COLORVALUE(grid_red, grid_green, grid_blue, 1.0f);


	// ---- normals
		FLOAT xn = 0, yn = 0, zn = 0;
		switch ( grid_align )
		{
			case 0 :  // xz
				yn = 1;
				break;
			case 1:  // xy
				zn = 1;
				break;
			case 2:  // yz
				xn = 1;
				break;
		}


	// ---- buffer
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- row

		struct pnt
		{
			FLOAT x, y, z, u, v;
		};
		pnt pnts[2][2048];
		UINT i = 0;

		for (UINT row = 0; row<=grid_cel_rows; row++)
		{
			
			FLOAT rinc = (FLOAT)row * incr_per_row;
			FLOAT edge = 0.001f;
			FLOAT v    = 1 - rinc - edge / 2;


			FLOAT x = 0, y = 0, z = 0;
			switch ( grid_align )
			{
				case 0:  // xz
					z = rinc * grid_depth;
					break;
				case 1:  // xy
					y = rinc * grid_depth;
					break;
				case 2:  // yz
					y = rinc * grid_depth;
					break;
			}


			// ---- calculate column
				for (UINT col = 0; col<grid_cols; col++)
				{
					FLOAT cinc = (FLOAT)col * incr_per_col;
					switch ( grid_align )
					{
						case 0 :  // xz
							pnts[i][col].x = cinc * grid_depth;
							pnts[i][col].y = y;
							pnts[i][col].z = z;
							break;
						case 1:  // xy
							pnts[i][col].x = cinc * grid_depth;
							pnts[i][col].y = y;
							pnts[i][col].z = z;
							break;
						case 2:  // yz
							pnts[i][col].x = x;
							pnts[i][col].y = y;
							pnts[i][col].z = cinc * grid_depth;
							break;
					}
					if ( vertex_format & VS_TEX1 )
					{
						FLOAT u = (1 - cinc) * (1 - edge) + edge / 2;
						pnts[i][col].u = 1 - u;
						pnts[i][col].v = v;
					}
				}


			// ---- build vertex list
				if ( row > 0 )
				{

					UINT h = i;
					UINT l = h == 0 ? 1 : 0;


					INT col_start = l < h ? 0         : grid_cols - 1;
					INT col_end   = l < h ? grid_cols : -1;
					INT col_incr  = l < h ? 1         : -1;


					for (INT col = col_start; col != col_end; col += col_incr)
					{

						pnt p[2] =
						{
							pnts[l < h ? l : h][col],
							pnts[l < h ? h : l][col]
						};


						for (UINT v = 0; v < 2; v++)
						{
							*(f    ) = p[v].x;
							*(f + 1) = p[v].y;
							*(f + 2) = p[v].z;
							f += 3;
							if ( vertex_format & VS_DIFFUSE )
							{
								*(f) = (FLOAT)diffuse.d;
								f++;
							}
							if ( vertex_format & VS_NORMAL )
							{
								*(f    ) = xn;
								*(f + 1) = yn;
								*(f + 2) = zn;
								f += 3;
							}
							if ( vertex_format & VS_TEX1 )
							{
								*(f    ) = p[v].u;
								*(f + 1) = p[v].v;
								f += 2;
							}
						}
					}


					{

						pnt p[2] =
						{
							pnts[l < h ? h : l][l < h ? grid_cols-1 : 0],
							pnts[l < h ? l : h][l < h ? grid_cols-1 : 0]
						};


						for (UINT v = 0; v < 2; v++)
						{

							*(f    ) = p[v].x;
							*(f + 1) = p[v].y;
							*(f + 2) = p[v].z;
							f += 3;

							if ( vertex_format & VS_DIFFUSE )
							{
								*(f) = (FLOAT)diffuse.d;
								f++;
							}

							if ( vertex_format & VS_NORMAL )
							{
								*(f    ) = xn;
								*(f + 1) = yn;
								*(f + 2) = zn;
								f += 3;
							}

							if ( vertex_format & VS_TEX1 )
							{
								*(f    ) = p[v].u;
								*(f + 1) = p[v].v;
								f += 2;
							}

						}
					}

				}


			// ---- swap
				i = i == 0 ? 1 : 0;
		}

		OutputDebugString("Callback_ObjGrid3D_GetVertexBuffer \n");

	return VS_OK;
}


// ---------- Callback_ObjGrid3D_GetIndexBuffer ----------
INT ObjConfig::Callback_ObjGrid3D_GetIndexBuffer(vs_system::ElmContext *context)
{

	// ---- get supplied and parameter data
		FLOAT *pd = context->GetFloatParameter();


	// ---- vertex
		INT grid_cel_cols = (UINT)pd[GRID_CEL_COLS];
		INT grid_cel_rows = (UINT)pd[GRID_CEL_ROWS];
		INT grid_cols     = grid_cel_cols + 1;


	// ---- buffer
		#if VS_INDEX_BUFFER_FORMAT == 32
		DWORD *i = context->GetIndexBufferPtr();
		#else
		WORD *i = context->GetIndexBufferPtr();
		#endif
		

	// ---- each row and column of the mesh
		for (INT row = 0; row<grid_cel_rows; row++)
		{
			INT row_offset_low  = row * grid_cols;
			INT row_offset_high = row_offset_low + grid_cols;
			for (INT col = 0; col<grid_cel_cols; col++)
			{
				INT i1 = col + row_offset_low;
				INT i2 = col + row_offset_high;
				*(i    ) = i1;
				*(i + 1) = i1 + 1;
				*(i + 2) = i2;
				*(i + 3) = i1 + 1;
				*(i + 4) = i2 + 1;
				*(i + 5) = i2;
				i += 6;
			}
		}

		OutputDebugString("Callback_ObjGrid3D_GetIndexBuffer \n");

	return VS_OK;
}


// ---------- Callback_ObjGrid3D_UpdateVertexBuffer ----------
INT ObjConfig::Callback_ObjGrid3D_UpdateVertexBuffer(
		vs_system::ElmContext *context,
		FLOAT *points,
		FLOAT *normals
	)
{

	// ---- get supplied and parameter data
		FLOAT *pd = context->GetFloatParameter();


	// ---- param
		UINT  grid_cel_cols = (UINT)pd[GRID_CEL_COLS];
		UINT  grid_cel_rows = (UINT)pd[GRID_CEL_ROWS];
		UINT  grid_cols     = grid_cel_cols + 1;
		UINT  grid_rows     = grid_cel_rows + 1;
		UINT  grid_type     = (UINT)pd[GRID_TYPE];
		UINT  grid_ap_ref   = (UINT)pd[GRID_AP_REF];



	// ---- vertex
		UINT  vertex_size      = context->GetVertexSize();
		UINT  vertex_format    = context->GetVertexFormat();
		FLOAT incr_vertex_rows = 1 / (FLOAT)grid_cel_rows;
		FLOAT incr_vertex_cols = 1 / (FLOAT)grid_cel_cols;


	// ---- buffer
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- each row of the mesh
		FLOAT *p = points;
		FLOAT *n = normals;
		for (UINT row = 0; row<grid_rows; row++)
		{

			// ---- for each column of the mesh
				for (UINT col = 0; col<grid_cols; col++)
				{

					// ---- point
						*(f    ) = *(p    );
						*(f + 1) = *(p + 1);
						*(f + 2) = *(p + 2);
						f += 3; p += 3;


					// ---- diffuse
						if ( vertex_format & VS_DIFFUSE )
						{
							f++; p++;
						}


					// ---- 3D normal
						if ( vertex_format & VS_NORMAL )
						{
							*(f    ) = *(n   );
							*(f + 1) = *(n + 1);
							*(f + 2) = *(n + 2);
							f += 3; p += 3;
						}


					// ---- 2D texture
						if ( vertex_format & VS_TEX1 )
						{
							f += 2; p += 2;
						}
				}
		}

	return VS_OK;
}
