////////////////////////////////////////////////////////////////////////////////

// ---------- vs_obj_mesh_edge.hpp ----------
/*!
\file vs_obj_mesh_edge.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_Mesh_Edge_Configure ----------
INT ObjConfig::Callback_Mesh_Edge_Configure(vs_system::ElmContext *context)
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
		UINT format = (UINT)(VS_XYZ | VS_NORMAL);
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(FALSE);
			context->SetColourVertex(FALSE);
			context->SetMultiState(FALSE);

			// ---- as per mesh
				switch ( mesh_type )
				{
					default:
					case 0: // wireframe
						format = (UINT)(VS_XYZ | VS_NORMAL);
						context->SetSolid(FALSE);
						context->SetTextured(FALSE);
						break;
					case 1: // solid
					case 2: // wireframe & texture
					case 3: // solid & textured
						format = (UINT)(VS_XYZ | VS_NORMAL);
						context->SetSolid(TRUE);
						context->SetTextured(FALSE);
						break;
				}

			context->SetVisible(TRUE);
		}


	// ---- primitive type
		context->SetPrimitiveType(VS_TRIANGLESTRIP);


	// ---- vertex buffer
		context->SetVertexCount (mesh_cols * 8 + 5);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_Mesh_Edge_GetVertexBuffer);


	// ---- index buffer
		UINT numIndices = 0;
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(FLOAT)
			);
		context->SetPrimitiveCount(mesh_cols * 8 + 3);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ---------- Callback_Mesh_Edge_GetVertexBuffer ----------
INT ObjConfig::Callback_Mesh_Edge_GetVertexBuffer(vs_system::ElmContext *context)
/*!
\brief create mesh edge from heightfield
\author Gareth Edwards

\note for demonstration and development purposes (e.g. function surface) the
      heightfield passed via this elements LongParameter array is dimensioned
	  +1 in rows & columns (e.g. mesh 1024*1024 & heightfield 1025*1025).

      This gives max heightfield column & row indices == max mesh column & row.

	  This results in an awkward - yet cunning - interpolation between the four
	  coreners to generate the edges.

	  Where sides 1 to 4, column & row increment value start, end, and increment are:

	  [   col start    row start    column end   row end    column incr    row incr ]
	  {   0,           0,           1,           mesh_row,  1,             1   },
	  {   0,           mesh_row-1,  mesh_col,    mesh_row,  1,             1   },
	  {   mesh_col-1,  mesh_row-1,  mesh_col,   -1,         1,            -1   },
	  {   mesh_col-1,  0,          -1,          -1,        -1,            -1   }

	  e.g. so increment for side:
		1) column == 0 & row == 0 until column == 1 & row == mesh_rows
		2) column == 0 & row == mesh_rows -1 until column == mesh_cols & row == mesh_rows
		3) etc.

*/
{

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();
		LONG  *ld = context->GetLongParameter();


	// ---- get parameter data
		UINT  mesh_type   = (UINT)pd[MESH_TYPE];
		UINT  mesh_ap_ref = (UINT)pd[MESH_AP_REF];
		INT   mesh_rows   = (INT )pd[MESH_ROWS];
		INT   mesh_cols   = (INT )pd[MESH_COLS];
		UINT  mesh_align  = (UINT)pd[MESH_ALIGN];
		FLOAT mesh_red    = (FLOAT)pd[MESH_RED];
		FLOAT mesh_green  = (FLOAT)pd[MESH_GREEN];
		FLOAT mesh_blue   = (FLOAT)pd[MESH_BLUE];
		FLOAT mesh_width  = (FLOAT)pd[MESH_WIDTH];
		FLOAT mesh_depth  = (FLOAT)pd[MESH_DEPTH];
		FLOAT mesh_height = (FLOAT)pd[MESH_HEIGHT]; // hgt not used
		FLOAT mesh_x_off  = (FLOAT)pd[MESH_XOFF];
		FLOAT mesh_y_off  = (FLOAT)pd[MESH_YOFF];
		FLOAT mesh_z_off  = (FLOAT)pd[MESH_ZOFF];


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


	// ---- local
		FLOAT u=0, v=0;
		INT   row=0, col=0;
		INT   row_str=0, row_end=0, row_inc=0;
		INT   col_str=0, col_end=0, col_inc=0;


	// ---- 4 * sides as per notes above
		col = mesh_cols;
		row = mesh_rows;
		INT rc[4][6] =
		{
			{   0,     0,     1,     row,   1,     1   },
			{   0,     row-1, col,   row,   1,     1   },
			{   col-1, row-1, col,  -1,     1,    -1   },
			{   col-1, 0,     -1,   -1,    -1,    -1   }
		};

	// ---- 4 * sides surface normals
		VsVector3 normal[4] =
		{
			{ -1,  0,  0 },
			{  0,  0, -1 },
			{  1,  0,  0 },
			{  0,  0,  1 }
		};


	// ---- assemble vertex buffer
		struct vertex
		{
			FLOAT xv = 0, yv = 0, zv = 0;
			FLOAT xn = 0, yn = 0, zn = 0;
		} vtx;


	// ---- side
		for (INT side = 0; side < 4; side++)
		{

			// ---- get start & end
				col_str = rc[side][0]; row_str = rc[side][1];
				col_end = rc[side][2]; row_end = rc[side][3];
				col_inc = rc[side][4]; row_inc = rc[side][5];
				
			// ---- surface normal
				vtx.xn = normal[side].x;
				vtx.yn = normal[side].y;
				vtx.zn = normal[side].z;

			// ---- row
				row = row_str;
				while ( row != row_end )
				{

					// ---- z
						y_heightfield_offset = row * y_heightfield_stride;
						v = (FLOAT)row / (FLOAT)(mesh_rows-1);
						vtx.zv = -v * mesh_depth + mesh_z_off;

					// ---- for each column
						col = col_str;
						while ( col != col_end )
						{

							// ---- z & y
								u = (FLOAT)col / (FLOAT)(mesh_cols-1);
								vtx.xv = u * mesh_width + mesh_x_off;
								vtx.yv = *(y_heightfield_buffer + y_heightfield_offset + col) + mesh_y_off;

							// ---- copy edge into buffer
								memcpy( (VOID *)(fvb), (VOID *)&vtx, sizeof(FLOAT)*vertex_size);
								fvb += vertex_size;

							// ---- copy base into buffer
								vtx.yv = mesh_y_off;
								memcpy((VOID *)(fvb), (VOID *)&vtx, sizeof(FLOAT)*vertex_size);
								fvb += vertex_size;

							// ---- update
								col += col_inc;

					}

					// ---- update
						row += row_inc;

				}

		}


	// ---- base vertex table
		vertex bvt[5] =
		{
			{ mesh_x_off,              mesh_y_off, mesh_z_off,               0, -1, 0 },
			{ mesh_x_off,              mesh_y_off, mesh_z_off,               0, -1, 0 },
			{ mesh_x_off,              mesh_y_off, mesh_z_off - mesh_depth,  0, -1, 0 },
			{ mesh_x_off + mesh_width, mesh_y_off, mesh_z_off,               0, -1, 0 },
			{ mesh_x_off + mesh_width, mesh_y_off, mesh_z_off - mesh_depth,  0, -1, 0 },
		};


	// ---- copy base vertices into buffer
		memcpy((VOID *)(fvb), (VOID *)&bvt[0], sizeof(FLOAT)*vertex_size * 5);


	// ---- copy base vertices into buffer
		//for (UINT bv = 0; bv < 5; bv++)
		//{
		//	memcpy((VOID *)(fvb), (VOID *)&bvt[bv], sizeof(FLOAT)*vertex_size);
		//	fvb += vertex_size;
		//}

	return VS_OK;
}

