////////////////////////////////////////////////////////////////////////////////

// ---------- vs_pano_cylinder.hpp ----------
/*!
\file vs_pano_cylinder.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_PanoCylinder_Configure ----------
INT ObjConfig::Callback_PanoCylinder_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling ----
	context->BeginConfig();

	// Accelerate pano parameters
	UINT rows = pi_obj_pano->cylinder_rows;
	UINT cols = pi_obj_pano->cylinder_columns;

	// ---- vertex ----
	UINT vertex_rows = (UINT)rows + 1;
	UINT vertex_cols = (UINT)cols + 1;

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
	context->SetIndexBufferCallback(&Callback_PanoCylinder_GetIndexBuffer);

	// ---- vertex buffer ----
	context->SetVertexCount(vertex_rows * vertex_cols);
	context->SetVertexFormat((UINT)(VS_XYZ | VS_TEX1));
	context->SetVertexSize(context->GetVertexFormat());
	context->SetVertexBufferCallback(&Callback_PanoCylinder_GetVertexBuffer);

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


// ----------  Callback_PanoCylinder_GetVertexBuffer ----------
/*!

*/
INT ObjConfig::Callback_PanoCylinder_GetVertexBuffer(vs_system::ElmContext *context)
{

	// Panel
	FLOAT *sp = context->GetGfxFloatParameter();
	FLOAT top = sp[VS_PANEL_TOP];
	FLOAT bottom = sp[VS_PANEL_BOTTOM];
	FLOAT panel_height = bottom - top - 1;

	// Accelerate pano parameters
	FLOAT rows = (FLOAT)pi_obj_pano->cylinder_rows;
	FLOAT cols = (FLOAT)pi_obj_pano->cylinder_columns;
	FLOAT cylinder_radius = (FLOAT)panel_height * pi_obj_pano->cylinder_scale;
	FLOAT cylinder_x_rotate = pi_obj_pano->cylinder_x_rotate;
	FLOAT latitude_min = pi_obj_pano->min_angle;
	FLOAT latitude_max = pi_obj_pano->max_angle;
	FLOAT aspect_ratio = pi_obj_pano->aspect_ratio;
	FLOAT u_flip = U_FLIP_LOGIC;
	FLOAT v_flip = V_FLIP_LOGIC;
	FLOAT u_centre = pi_obj_pano->u_offset + 0.5f;
	FLOAT v_centre = pi_obj_pano->v_offset + 0.5f;
	FLOAT v_radius_min = pi_obj_pano->min_radius;
	FLOAT v_radius_max = pi_obj_pano->max_radius;

	// Radii
	FLOAT v_radius_total = v_radius_max - v_radius_min;

	// Proportional height
	FLOAT circum = 2 * (FLOAT)VS_PI * cylinder_radius;
	FLOAT height = v_radius_total / v_radius_max * cylinder_radius * 2;
	FLOAT hby2 = height / 2;

	// ---- vertex ----
	UINT  vertex_size = context->GetVertexSize();
	UINT  vertex_format = context->GetVertexFormat();
	UINT  vertex_rows = (UINT)rows + 1;
	UINT  vertex_cols = (UINT)cols + 1;
	FLOAT incr_vertex_rows = 1 / (FLOAT)rows;
	FLOAT incr_vertex_cols = 1 / (FLOAT)cols;

	// Total latitude radians
	FLOAT latitiude_diff_radians = (latitude_max - latitude_min) * (FLOAT)VS_RADIANS_PER_DEGREE;
	FLOAT latitiude_dmin_radians = latitude_min * (FLOAT)VS_RADIANS_PER_DEGREE;

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
	for (UINT row = 0; row<vertex_rows; row++)
	{
		// X axis rotation scalar
		FLOAT x_axis_scalar = (FLOAT)row * (FLOAT)incr_vertex_rows;

		// 3D 
		FLOAT y = x_axis_scalar * height - hby2;
		FLOAT z = cylinder_radius;

		// 2D Unwrapping
		//FLOAT v = (x_axis_scalar * v_radius_total + v_radius_min) * 0.5f;


		// ---- 2D unwrapping
			if ( V_INSIDE_OUT )
				v = ((1-x_axis_scalar) * v_radius_total + v_radius_min) * 0.5f;
			else
				v = (x_axis_scalar * v_radius_total + v_radius_min) * 0.5f;
		

		// FOR each column of the mesh
		for (UINT col = 0; col<vertex_cols; col++)
		{
			// Y axis rotation scalar
			FLOAT y_axis_scalar = (FLOAT)col * incr_vertex_cols;

			// 3D Rotate RY,0,RZ_1 to RY,RX,RZ_2
			FLOAT y_rotation_radians =
				(y_axis_scalar * 360 + pi_obj_pano->pan + 180) *
				(FLOAT)VS_RADIANS_PER_DEGREE;
			*(f) = z * sinf(y_rotation_radians);
			*(f + 1) = y;
			*(f + 2) = -z * cosf(y_rotation_radians);
			f += 3;

			// Diffuse
			if (vertex_format & VS_DIFFUSE)
			{
				*(f) = white.f;
				f++;
			}

			// Normal
			if (vertex_format & VS_NORMAL)
			{
				FLOAT quotient = 1 / cylinder_radius;
				*(f) *= quotient; // * *(f);
				*(f + 1) *= quotient; // * *(f+1);
				*(f + 2) *= quotient; // * *(f+2);
				f += 3;
			}

			// 2D Unwrapping
			if (vertex_format & VS_TEX1)
			{
				FLOAT a = y_axis_scalar * -360.0f * (FLOAT)VS_RADIANS_PER_DEGREE;
				*(f) = u_flip * v * sinf(a) * aspect_ratio + u_centre;
				*(f + 1) = v_flip * v * cosf(a) + v_centre;
				f += 2;
			}
		}
	}

	// Buffer
	f = context->GetVertexBufferPtr();

	// Accelerate x rotation radian
	FLOAT x_rotation_radians = cylinder_x_rotate * (FLOAT)VS_RADIANS_PER_DEGREE;

	// ---- vertex ---- increment
	UINT vinc = 3;
	if (vertex_format & VS_DIFFUSE) vinc++;
	if (vertex_format & VS_NORMAL) vinc += 3;
	if (vertex_format & VS_TEX1) vinc += 2;

	// FOR each row of the mesh
	for (UINT row = 0; row<vertex_rows; row++)
	{
		// FOR each column of the mesh
		for (UINT col = 0; col<vertex_cols; col++)
		{
			FLOAT y = *(f + 1);
			FLOAT z = *(f + 2);
			*(f + 1) = y * cosf(x_rotation_radians) - z * sinf(x_rotation_radians);
			*(f + 2) = y * sinf(x_rotation_radians) + z * cosf(x_rotation_radians);
			f += vinc;
		}
	}

	// Done
	return VS_OK;
}


// ---------- Callback_PanoCylinder_GetIndexBuffer ----------
INT ObjConfig::Callback_PanoCylinder_GetIndexBuffer(vs_system::ElmContext *context)
{

	// Accelerate pano parameters
	UINT rows = pi_obj_pano->cylinder_rows;
	UINT cols = pi_obj_pano->cylinder_columns;

	// ---- vertex ----
	UINT  vertex_rows = rows+1;
	UINT  vertex_cols = cols+1;


	// ---- buffer
		#if VS_INDEX_BUFFER_FORMAT == 32
		DWORD *i = context->GetIndexBufferPtr();
		#else
		WORD *i = context->GetIndexBufferPtr();
		#endif


	// FOR each row and column of the mesh
	for ( UINT row=0; row<rows; row++ )
	{
		UINT row_offset_low = row * vertex_cols;
		UINT row_offset_high = row_offset_low + vertex_cols;
		for ( UINT col=0; col<cols; col++ )
		{
			UINT i1 = col + row_offset_low;
			UINT i2 = col + row_offset_high;
			*(i)    = i1;
			*(i+1)  = i1+1;
			*(i+2)  = i2;
			*(i+3)  = i1+1;
			*(i+4)  = i2+1;
			*(i+5)  = i2;
			i+=6;
		}
	}

	// Done
	return VS_OK;
}
