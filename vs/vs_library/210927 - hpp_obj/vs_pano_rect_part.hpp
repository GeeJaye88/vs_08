////////////////////////////////////////////////////////////////////////////////

// ---------- vs_pano_rect_part.hpp ----------
/*!
\file vs_pano_rect_part.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_PanoRectPart_Configure ----------
INT ObjConfig::Callback_PanoRectPart_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling ----
	context->BeginConfig();


	// ---- supplied data ----
	FLOAT *sp = context->GetGfxFloatParameter();
	FLOAT left = sp[VS_PANEL_LEFT];
	FLOAT top = sp[VS_PANEL_TOP];
	FLOAT right = sp[VS_PANEL_RIGHT];
	FLOAT bottom = sp[VS_PANEL_BOTTOM];
	FLOAT width = right - left;
	FLOAT height = bottom - top;


	// ---- parameter data ----
	FLOAT *p = context->GetFloatParameter();
	FLOAT block = p[0];
	FLOAT number_of_blocks = p[1];
	FLOAT pixel_mesh_dim = p[2];


	// ---- was pano parameters ----
	// e.g. rows = pi_obj_pano->sphere_rows;
	//      cols = pi_obj_pano->sphere_columns;
	UINT rows = (UINT)(width / pixel_mesh_dim + 0.5);
	UINT cols = (UINT)(height / pixel_mesh_dim + 0.5);

	UINT vertex_rows = rows + 1;
	UINT vertex_cols = cols + 1;


	// ---- atlas page ----
	context->SetAtlasPage(0, pi_obj_pano->pano_texture_lid);


	// IF -- overide -- THEN
	if (!context->GetOveride())
	{
		context->SetAlphaBlending(FALSE);
		context->SetColourVertex(FALSE);
		context->SetMultiState(FALSE);
		context->SetSolid(TRUE);
		context->SetTextured(TRUE);
		context->SetVisible(TRUE);
	}


	// ---- index buffer ---- ----
	context->SetIndexCount(rows * cols * 6);
	context->SetIndexBufferCallback(&Callback_PanoRectPart_GetIndexBuffer);


	// ---- vertex buffer ----
	context->SetVertexCount(vertex_rows * vertex_cols);
	context->SetVertexFormat((UINT)(VS_XYZRHW | VS_TEX1));
	context->SetVertexSize(context->GetVertexFormat());
	context->SetVertexBufferCallback(&Callback_PanoRectPart_GetVertexBuffer);


	// ----primitive parameters ----
	context->SetPrimitiveBytes(
		context->GetVertexSize() *
		context->GetVertexCount()  *
		sizeof(float)
		);
	context->SetPrimitiveCount(rows * cols * 2);
	context->SetPrimitiveType(VS_TRIANGLELIST);


	// ---- end configuration profiling ----
	context->EndConfig();

	return VS_OK;
}


// ---------- Callback_PanoRectPart_GetVertexBuffer ----------
INT ObjConfig::Callback_PanoRectPart_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- supplied data ----
	FLOAT *sp = context->GetGfxFloatParameter();
	FLOAT left = sp[VS_PANEL_LEFT];
	FLOAT top = sp[VS_PANEL_TOP];
	FLOAT right = sp[VS_PANEL_RIGHT];
	FLOAT bottom = sp[VS_PANEL_BOTTOM];
	FLOAT width = right - left;
	FLOAT height = bottom - top;


	// ---- parameter data ----
	FLOAT *p = context->GetFloatParameter();
	FLOAT block = p[0];
	FLOAT number_of_blocks = p[1];
	FLOAT pixel_mesh_dim = p[2];


	// ---- was pano parameters ----
	// e.g. rows = pi_obj_pano->sphere_rows;
	//      cols = pi_obj_pano->sphere_columns;
	UINT rows = (UINT)(width / pixel_mesh_dim + 0.5);
	UINT cols = (UINT)(height / pixel_mesh_dim + 0.5);


	// ---- local pano parameters ----
	FLOAT sphere_radius = pi_obj_pano->sphere_radius;
	FLOAT latitude_min = pi_obj_pano->min_angle;
	FLOAT latitude_max = pi_obj_pano->max_angle;
	FLOAT aspect_ratio = pi_obj_pano->aspect_ratio;


	// ---- uv ----
	FLOAT u_flip = U_FLIP_LOGIC;
	FLOAT v_flip = V_FLIP_LOGIC;
	FLOAT u_centre = pi_obj_pano->u_offset + 0.5f;
	FLOAT v_centre = pi_obj_pano->v_offset + 0.5f;
	FLOAT v_radius_min = pi_obj_pano->min_radius;
	FLOAT v_radius_max = pi_obj_pano->max_radius;


	/// ---- radii ----
	FLOAT v_radius_total = v_radius_max - v_radius_min;


	// ---- part param ----
	FLOAT radian_ang = 360.0f * (FLOAT)VS_RADIANS_PER_DEGREE;
	FLOAT radian_xtra = block / number_of_blocks;
	FLOAT radian_span = 1.0f / number_of_blocks;


	// ---- vertex ----
	UINT  vertex_format = context->GetVertexFormat();
	UINT  vertex_rows = rows + 1;
	UINT  vertex_cols = cols + 1;
	FLOAT incr_vertex_rows = 1 / (FLOAT)rows;
	FLOAT incr_vertex_cols = 1 / (FLOAT)cols;


	// ---- diffuse ----
	union colour_union_t {
		DWORD d;
		FLOAT f;
	} white;
	white.d = VS_D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);


	// FOR -- each row --
	FLOAT *f = context->GetVertexBufferPtr();
	for (UINT row = 0; row<vertex_rows; row++)
	{
		// 2D rect
		FLOAT rmu = (FLOAT)row * incr_vertex_rows;
		FLOAT y = -rmu * width + right;

		// 2D unwrapping
		FLOAT v = (rmu * v_radius_total + v_radius_min) * 0.5f;

		// FOR -- each column --
		for (UINT col = 0; col<vertex_cols; col++)
		{
			// 2D rect
			FLOAT cmu = (FLOAT)col * incr_vertex_cols;
			*(f + 1) = cmu * height;
			*(f + 0) = y;
			*(f + 2) = 0.1f; // zed
			*(f + 3) = 1;    // rhw
			f += 4;

			// diffuse
			if (vertex_format & VS_DIFFUSE)
			{
				*(f) = white.f;
				f++;
			}

			// 2D unwrapping
			if (vertex_format & VS_TEX1)
			{
				FLOAT a = (cmu * radian_span + radian_xtra) * radian_ang;
				*(f + 0) = u_flip * v * sin(a) * aspect_ratio + u_centre;
				*(f + 1) = v_flip * v * cos(a) + v_centre;
				f += 2;
			}
		}
	}

	return VS_OK;
}


// ---------- Callback_PanoRectPart_GetIndexBuffer ----------
INT ObjConfig::Callback_PanoRectPart_GetIndexBuffer(
	vs_system::ElmContext *context
	)
{

	// ---- supplied data ----
	FLOAT *sp = context->GetGfxFloatParameter();
	FLOAT left = sp[VS_PANEL_LEFT];
	FLOAT top = sp[VS_PANEL_TOP];
	FLOAT right = sp[VS_PANEL_RIGHT];
	FLOAT bottom = sp[VS_PANEL_BOTTOM];
	FLOAT width = right - left;
	FLOAT height = bottom - top;


	// ---- parameter data ----
	FLOAT *p = context->GetFloatParameter();
	FLOAT block = p[0];
	FLOAT number_of_blocks = p[1];
	FLOAT pixel_mesh_dim = p[2];


	// ---- was pano parameters ----
	// e.g. rows = pi_obj_pano->sphere_rows;
	//      cols = pi_obj_pano->sphere_columns;
	UINT rows = (UINT)(width / pixel_mesh_dim + 0.5);
	UINT cols = (UINT)(height / pixel_mesh_dim + 0.5);


	// ---- vertex ----
	UINT vertex_format = context->GetVertexFormat();
	UINT vertex_rows = rows + 1;
	UINT vertex_cols = cols + 1;


	// ---- buffer
		#if VS_INDEX_BUFFER_FORMAT == 32
		DWORD *i = context->GetIndexBufferPtr();
		#else
		WORD *i = context->GetIndexBufferPtr();
		#endif


	// ---- row and column
		for (UINT row = 0; row<rows; row++)
		{
			UINT row_offset_low = row * vertex_cols;
			UINT row_offset_high = row_offset_low + vertex_cols;
			for (UINT col = 0; col<cols; col++)
			{
				UINT i1 = col + row_offset_low;
				UINT i2 = col + row_offset_high;
				*(i) = i1;
				*(i + 1) = i1 + 1;
				*(i + 2) = i2;
				*(i + 3) = i1 + 1;
				*(i + 4) = i2 + 1;
				*(i + 5) = i2;
				i += 6;
			}
		}

	return VS_OK;
}

