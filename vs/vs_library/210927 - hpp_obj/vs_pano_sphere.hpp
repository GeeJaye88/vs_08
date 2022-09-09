////////////////////////////////////////////////////////////////////////////////

// ---------- vs_pano_sphere.h ----------
/*!
\file vs_pano_sphere.h
\brief
\author Gareth Edwards
*/


#pragma once



// ---- initialise static private implementation ----
//ObjConfig::ObjVptf * ObjConfig::pi_obj_pano = NULL;
//ObjConfig::ImageRoi * ObjConfig::pi_image_roi = NULL;


// ---------- Callback_PanoSphere_Configure ----------
INT ObjConfig::Callback_PanoSphere_Configure(
		vs_system::ElmContext *context
	)
{

	// ---- begin configuration profiling ----
	context->BeginConfig();

	// Local
	//FLOAT *p = context->GetFloatParameter();


	// ---- local pano parameters ----
	UINT rows = pi_obj_pano->sphere_rows;
	UINT cols = pi_obj_pano->sphere_columns;


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
	context->SetIndexBufferCallback(&Callback_PanoSphere_GetIndexBuffer);


	// ---- vertex buffer ----
	context->SetVertexCount(vertex_rows * vertex_cols);
	context->SetVertexFormat((UINT)(VS_XYZ | VS_TEX1));
	context->SetVertexSize(context->GetVertexFormat());
	context->SetVertexBufferCallback(&Callback_PanoSphere_GetVertexBuffer);


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

	return VS_OK;
}


// ----------  Callback_PanoSphere_GetVertexBuffer ----------
/*!
\note Parameters:
u proportional centre (typically 0.5(ish), 0 <= u <= 1)
v proportional centre (typically 0.5(ish), 0 <= v <= 1)
u proportional radius minimum (typically 0.3(ish), 0 <= u <= 1)
u proportional radius maximum (typically 0.9(ish), 0 <= v <= 1)
minimum angle of latitude (typically -45(ish), -90 <= angle <= 0)
maximum angle of latitude (typically 45(ish), 0 <= angle <= 90)
maximum extent angle of latitude (typically 140(ish), 0 <= angle <= 90)
radius of spherical surface (typically 5(ish), radius > 1)
*/
INT ObjConfig::Callback_PanoSphere_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- local pano parameters
		FLOAT rows          = (FLOAT)pi_obj_pano->sphere_rows;
		FLOAT cols          = (FLOAT)pi_obj_pano->sphere_columns;
		FLOAT sphere_radius = pi_obj_pano->sphere_radius;
		FLOAT latitude_min  = pi_obj_pano->min_angle;
		FLOAT latitude_max  = pi_obj_pano->max_angle;
		FLOAT aspect_ratio  = pi_obj_pano->aspect_ratio;
		FLOAT u_flip        = U_FLIP_LOGIC;
		FLOAT v_flip        = V_FLIP_LOGIC;
		FLOAT u_centre      = pi_obj_pano->u_offset + 0.5f;
		FLOAT v_centre      = pi_obj_pano->v_offset + 0.5f;
		FLOAT v_radius_min  = pi_obj_pano->min_radius;
		FLOAT v_radius_max  = pi_obj_pano->max_radius;


	// ---- radii
		FLOAT v_radius_total = v_radius_max - v_radius_min;


	// ---- vertex 
		UINT  vertex_size = context->GetVertexSize();
		UINT  vertex_format = context->GetVertexFormat();
		UINT  vertex_rows = (UINT)rows + 1;
		UINT  vertex_cols = (UINT)cols + 1;
		FLOAT incr_vertex_rows = 1 / (FLOAT)rows;
		FLOAT incr_vertex_cols = 1 / (FLOAT)cols;


	// ---- total latitude radians ----
	FLOAT latitiude_diff_radians = (latitude_max - latitude_min) * (FLOAT)VS_RADIANS_PER_DEGREE;
	FLOAT latitiude_dmin_radians = latitude_min * (FLOAT)VS_RADIANS_PER_DEGREE;


	// ---- diffuse ----
	union colour_union_t {
		DWORD d;
		FLOAT f;
	} white;
	white.d = VS_D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);


	// ---- buffer
		FLOAT *f = context->GetVertexBufferPtr();
		FLOAT  v = 0;

	// FOR -- each row of the mesh --
	for (UINT row = 0; row<vertex_rows; row++)
	{

		// ---- X axis rotation scalar ----
		FLOAT x_axis_scalar = (FLOAT)row * (FLOAT)incr_vertex_rows;


		// ---- angle ----
		FLOAT x_rotation_radians = x_axis_scalar * latitiude_diff_radians + latitiude_dmin_radians;


		// ---- 3D Rotate 0,0,1 to RY,0,RZ_1 ----
		FLOAT y = sphere_radius * sin(x_rotation_radians);
		FLOAT z = sphere_radius * cos(x_rotation_radians);


		// ---- 2D unwrapping
			if ( V_INSIDE_OUT )
				v = ((1-x_axis_scalar) * v_radius_total + v_radius_min) * 0.5f;
			else
				v = (x_axis_scalar * v_radius_total + v_radius_min) * 0.5f;


		// FOR -- each column of the mesh --
		for (UINT col = 0; col<vertex_cols; col++)
		{

			// ---- Y axis rotation scalar ----
			FLOAT y_axis_scalar = (FLOAT)col * incr_vertex_cols;


			// ---- 3D Rotate RY,0,RZ_1 to RY,RX,RZ_2 ----
			FLOAT y_rotation_radians = y_axis_scalar * 360 * (FLOAT)VS_RADIANS_PER_DEGREE;
			*(f) = z * sinf(y_rotation_radians);
			*(f + 1) = y;
			*(f + 2) = -z * cosf(y_rotation_radians);
			f += 3;


			// ---- diffuse ----
			if (vertex_format & VS_DIFFUSE)
			{
				*(f) = white.f;
				f++;
			}


			// ---- normal ----
			if (vertex_format & VS_NORMAL)
			{
				FLOAT quotient = 1 / sphere_radius;
				*(f) *= quotient; // * *(f);
				*(f + 1) *= quotient; // * *(f+1);
				*(f + 2) *= quotient; // * *(f+2);
				f += 3;
			}


			// ---- 2D Unwrapping ----
			if (vertex_format & VS_TEX1)
			{
				FLOAT a = (1 - y_axis_scalar) * 360.0f * (FLOAT)VS_RADIANS_PER_DEGREE;
				*(f) = -u_flip * v * sinf(a) * aspect_ratio + u_centre;
				*(f + 1) = v_flip * v * cosf(a) + v_centre;
				f += 2;
			}

		}
	}

	return VS_OK;
}


// ---------- Callback_PanoSphere_GetIndexBuffer ----------
INT ObjConfig::Callback_PanoSphere_GetIndexBuffer(vs_system::ElmContext *context)
{

	// ---- local pano parameters ----
	INT rows = (INT)pi_obj_pano->sphere_rows;
	INT cols = (INT)pi_obj_pano->sphere_columns;


	// ---- vertex ----
	INT  vertex_rows = (INT)rows + 1;
	INT  vertex_cols = (INT)cols + 1;


	// ---- buffer
		#if VS_INDEX_BUFFER_FORMAT == 32
		DWORD *i = context->GetIndexBufferPtr();
		#else
		WORD *i = context->GetIndexBufferPtr();
		#endif


	// FOR --each row and column of the mesh --
	for (INT row = 0; row<rows; row++)
	{
		INT row_offset_low = row * vertex_cols;
		INT row_offset_high = row_offset_low + vertex_cols;
		for (INT col = 0; col<cols; col++)
		{
			INT i1 = col + row_offset_low;
			INT i2 = col + row_offset_high;
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

