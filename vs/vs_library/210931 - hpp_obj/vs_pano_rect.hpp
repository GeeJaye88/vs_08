////////////////////////////////////////////////////////////////////////////////

// ---------- vs_pano_rect.hpp ----------
/*!
\file vs_pano_rect.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_PanoRect_Configure ----------
INT ObjConfig::Callback_PanoRect_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling ----
	context->BeginConfig();

	// Accelerate pano parameters
	UINT rows = pi_obj_pano->sphere_rows;
	UINT cols = pi_obj_pano->sphere_columns;

	// ---- vertex ----
	UINT vertex_rows = rows + 1;
	UINT vertex_cols = cols + 1;

	// ---- atlas page ----
	context->SetAtlasPage(0, pi_obj_pano->pano_texture_lid);

	// IF -- overide -- THEN
	if (!context->GetOveride())
	{
		context->SetAlphaBlending(false);
		context->SetColourVertex(false);
		context->SetMultiState(false);
		context->SetSolid(true);
		context->SetTextured(TRUE);
		context->SetVisible(true);
	}

	// ---- index buffer ----
	context->SetIndexCount(rows * cols * 6);
	context->SetIndexBufferCallback(&Callback_PanoRect_GetIndexBuffer);

	// ---- vertex buffer ----
	context->SetVertexCount(vertex_rows * vertex_cols);
	context->SetVertexFormat((UINT)(VS_XYZRHW | VS_TEX1));
	context->SetVertexSize(context->GetVertexFormat());
	context->SetVertexBufferCallback(&Callback_PanoRect_GetVertexBuffer);

	// ---- primitive parameters ----
	context->SetPrimitiveBytes(
		context->GetVertexSize() *
		context->GetVertexCount()  *
		sizeof(float)
		);
	context->SetPrimitiveCount(rows * cols * 2);
	context->SetPrimitiveType(VS_TRIANGLELIST);

	// ---- end configuration profiling ----
	context->EndConfig();

	// Done
	return VS_OK;
}


// ---------- Callback_PanoRect_GetVertexBuffer ----------
INT ObjConfig::Callback_PanoRect_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- local ----
	FLOAT *p = context->GetFloatParameter();


	// ---- supplied ----
	FLOAT *sp = context->GetGfxFloatParameter();
	FLOAT left = sp[VS_PANEL_LEFT];
	FLOAT top = sp[VS_PANEL_TOP];
	FLOAT right = sp[VS_PANEL_RIGHT];
	FLOAT bottom = sp[VS_PANEL_BOTTOM];
	FLOAT width = right - left;
	FLOAT height = bottom - top;


	// ---- local pano parameters ----
	UINT  rows = pi_obj_pano->sphere_rows;
	UINT  cols = pi_obj_pano->sphere_columns;
	FLOAT sphere_radius = pi_obj_pano->sphere_radius;
	FLOAT latitude_min = pi_obj_pano->min_angle;
	FLOAT latitude_max = pi_obj_pano->max_angle;
	FLOAT aspect_ratio = pi_obj_pano->aspect_ratio;
	FLOAT u_flip = U_FLIP_LOGIC;
	FLOAT v_flip = V_FLIP_LOGIC;
	FLOAT u_centre = pi_obj_pano->u_offset + 0.5f;
	FLOAT v_centre = pi_obj_pano->v_offset + 0.5f;
	FLOAT v_radius_min = pi_obj_pano->min_radius;
	FLOAT v_radius_max = pi_obj_pano->max_radius;


	/// ---- radii
		FLOAT v_radius_total = v_radius_max - v_radius_min;


	// pan 2d vertex buffers ? -- then use pan value OR zero --
		FLOAT pmu =
			pi_obj_pano->pan_vertex_buffer ?
				pi_obj_pano->pan / 360 :
					-pi_obj_pano->pan_offset / 360;


	// ---- set pano geometry type
		UINT pano_type = context->GetId();
		BOOL demi = FALSE;


	// ---- see SetupSharedHiddenLists ----
	pano_type = pano_type == VS_OBJ_PANO_RECT_PART ? VS_OBJ_PANO_RECT_TOP : pano_type;


	// ---- demi param ----
	FLOAT x_offset = 0;
	FLOAT y_offset = 0;
	FLOAT demi_angle_xtra = 0;
	FLOAT radian_ang = 360.0f * (FLOAT)VS_RADIANS_PER_DEGREE;


	// ========== IF demi top OR demi bottom THEN ==========
	if (pano_type == VS_OBJ_PANO_RECT_TOP || pano_type == VS_OBJ_PANO_RECT_BOTTOM)
	{
		FLOAT x_middle = width / 2;
		FLOAT y_middle = height / 2;
		FLOAT pane_aspect = width / height;
		FLOAT demi_aspect = 180.0f / pi_obj_pano->max_fov / 2.0f;
		FLOAT demi_height = 0;
		FLOAT demi_width = 0;

		if (demi_aspect > pane_aspect)
		{
			demi_width = width;
			demi_height = width * pi_obj_pano->max_fov / 180.0f;
		}
		else
		{
			demi_height = height / 2.0f;
			demi_width = demi_height * 180.0f / pi_obj_pano->max_fov;
		}

		height = demi_height;
		width = demi_width;
		y_offset = pano_type == VS_OBJ_PANO_RECT_TOP ? -y_middle - 1 : -(y_middle - height);
	}


	// ========== Set demi parameters ==========
	switch (pano_type)
	{
	default:
		break;
	case VS_OBJ_PANO_RECT_TOP:
	case VS_OBJ_PANO_RECT_FRONT:
		cols /= 2;
		demi = TRUE;
		demi_angle_xtra = 0.25;
		break;
	case VS_OBJ_PANO_RECT_BOTTOM:
	case VS_OBJ_PANO_RECT_BACK:
		cols /= 2;
		demi = TRUE;
		demi_angle_xtra = 0.75;
		break;
	}


	// ========== IF equi(rectangular) THEN ==========
	if (pano_type == VS_OBJ_PANO_RECT_EQUI)
	{
		y_offset = -(90.0f + latitude_min) / 180.0f * height;
		FLOAT equi_aspect = pi_obj_pano->max_fov / 180.0f;
		height = height * equi_aspect;
	}


	// ========== FRFOM HERE CODE IS SAME FOR ALL PANORECT ==========

	// ---- vertex ----
	UINT  vertex_format = context->GetVertexFormat();
	UINT  vertex_rows = rows + 1;
	UINT  vertex_cols = cols + 1;
	FLOAT incr_vertex_rows = 1 / (FLOAT)rows;
	FLOAT incr_vertex_cols = 1 / (FLOAT)cols;

	// Diffuse Colour
	union colour_union_t {
		DWORD d;
		FLOAT f;
	} white;
	white.d = VS_D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);

	
	// ---- buffer
		FLOAT *f = context->GetVertexBufferPtr();
		FLOAT  v = 0;


	// FOR each row of the mesh
	for (UINT row=0; row<vertex_rows; row++)
	{
		// 2D Rect
		FLOAT rmu = (FLOAT)row * incr_vertex_rows;
		FLOAT y = -rmu * height + bottom + y_offset;


		// ---- 2D unwrapping
			if (V_INSIDE_OUT)
				v = ((1 - rmu) * v_radius_total + v_radius_min) * 0.5f;
			else
				v = (rmu * v_radius_total + v_radius_min) * 0.5f;


		// FOR each column of the mesh
		for (UINT col = 0; col<vertex_cols; col++)
		{
			// 2D Rect
			FLOAT cmu = (FLOAT)col * incr_vertex_cols;
			*(f) = (1 - cmu) * width + left;
			*(f + 1) = y;
			*(f + 2) = 0.1f; // zed
			*(f + 3) = 1; // rhw
			f += 4;

			// Diffuse
			if (vertex_format & VS_DIFFUSE)
			{
				*(f) = white.f;
				f++;
			}

			// 2D Unwrapping
			if (vertex_format & VS_TEX1)
			{
				FLOAT r = demi ? cmu * 0.5f : cmu;
				FLOAT a = (r + pmu + demi_angle_xtra) * radian_ang;
				*(f) = u_flip * v * sin(a) * aspect_ratio + u_centre;
				*(f + 1) = v_flip * v * cos(a) + v_centre;
				f += 2;
			}
		}
}


	// Done
	return VS_OK;
}


// ---------- Callback_PanoRect_GetIndexBuffer ----------
INT ObjConfig::Callback_PanoRect_GetIndexBuffer(vs_system::ElmContext *context)
{

	// ---- local
		FLOAT *p = context->GetFloatParameter();


	// ---- pano parameters
		UINT rows = pi_obj_pano->sphere_rows;
		UINT cols = pi_obj_pano->sphere_columns;


	// ---- select 2D panoramic geometry type
		UINT pano_type = context->GetId();
		switch (pano_type)
		{
		default:
			break;
		case VS_OBJ_PANO_RECT_TOP:
		case VS_OBJ_PANO_RECT_BOTTOM:
		case VS_OBJ_PANO_RECT_FRONT:
		case VS_OBJ_PANO_RECT_BACK:
			cols = cols / 2;
			break;
		}


	// ---- vertex
		UINT vertex_format = context->GetVertexFormat();
		UINT vertex_rows = rows + 1;
		UINT vertex_cols = cols + 1;


	// ---- buffer
		#if VS_INDEX_BUFFER_FORMAT == 32
		DWORD *i = context->GetIndexBufferPtr();
		#else
		WORD *i = context->GetIndexBufferPtr();
		#endif


	// ---- mesh row and column
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

