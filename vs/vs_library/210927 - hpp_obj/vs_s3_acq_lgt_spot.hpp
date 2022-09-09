////////////////////////////////////////////////////////////////////////////////

// ---------- vs_sphere.hpp ----------
/*!
\file vs_sphere.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_S3_AcqLgtSpot_Configure ----------
INT ObjConfig::Callback_S3_AcqLgtSpot_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- get supplied and parameter data
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- param
		UINT sphere_cel_cols = (UINT)pd[SPHERE_CEL_COLS];
		UINT sphere_cel_rows = (UINT)pd[SPHERE_CEL_ROWS];
		UINT sphere_cols     = sphere_cel_cols + 1;
		UINT sphere_rows     = sphere_cel_rows + 1;
		UINT sphere_type     = (UINT)pd[SPHERE_TYPE];
		UINT sphere_ap_ref   = (UINT)pd[SPHERE_AP_REF];


	// ---- atlas page
		context->SetAtlasPage(0, sphere_ap_ref);


	// ---- not overide ?
		UINT format = (UINT)(VS_XYZ | VS_DIFFUSE); // wireframe
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(FALSE);
			context->SetColourVertex(FALSE);
			context->SetMultiState(FALSE);

			switch (sphere_type)
			{
				default:
				case 0: // wireframe
					format = (UINT)(VS_XYZ | VS_DIFFUSE);
					context->SetSolid(FALSE);
					context->SetTextured(FALSE);
					break;
				case 1: // solid
					format = (UINT)(VS_XYZ | VS_NORMAL | VS_DIFFUSE);
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
		context->SetIndexCount(sphere_cel_cols * sphere_cel_rows * 6);
		context->SetIndexBufferCallback(&Callback_S3_AcqLgtSpot_GetIndexBuffer);


	// ---- vertex buffer
		context->SetVertexCount( sphere_cols * sphere_rows );
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_S3_AcqLgtSpot_GetVertexBuffer);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(FLOAT)
			);
		context->SetPrimitiveCount(sphere_cel_cols * sphere_cel_rows * 2);
		context->SetPrimitiveType(VS_TRIANGLELIST);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ----------  Callback_S3_AcqLgtSpot_GetVertexBuffer ----------
/*!

*/
INT ObjConfig::Callback_S3_AcqLgtSpot_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- get supplied and parameter data
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- sphere parameters
		UINT  sphere_cel_cols = (UINT)pd[SPHERE_CEL_COLS];
		UINT  sphere_cel_rows = (UINT)pd[SPHERE_CEL_ROWS];
		UINT  sphere_cols     = sphere_cel_cols + 1;
		UINT  sphere_rows     = sphere_cel_rows + 1;
		UINT  sphere_type     = 0; // not used in this method
		UINT  sphere_ap_ref   = 0; // ditto
		UINT  sphere_ap_sect  = 0; // ditto
		FLOAT sphere_red      = pd[SPHERE_RED];
		FLOAT sphere_green    = pd[SPHERE_GREEN];
		FLOAT sphere_blue     = pd[SPHERE_BLUE];
		FLOAT sphere_radius   = pd[SPHERE_RADIUS];
		FLOAT sphere_lat_min  = -90;
		FLOAT sphere_lat_max  = 90;

		
	// ---- hijack box dimensions
		FLOAT width    = pd[vs_system::ObjConfig::BOX_FACE_WIDTH];
		FLOAT height   = pd[vs_system::ObjConfig::BOX_FACE_HEIGHT];   // hgt not used
		FLOAT depth    = pd[vs_system::ObjConfig::BOX_FACE_DEPTH];

	// ---- hijack box position
		FLOAT x_off    = pd[vs_system::ObjConfig::BOX_FACE_X];        // +/- offset
		FLOAT y_off    = pd[vs_system::ObjConfig::BOX_FACE_Y];        //    "
		FLOAT z_off    = pd[vs_system::ObjConfig::BOX_FACE_Z];        //    "

	// ----- hijack box light
		FLOAT param_intensity  = pd[vs_system::ObjConfig::BOX_FLAG_4];
		FLOAT param_distance   = pd[vs_system::ObjConfig::BOX_FLAG_5];
		FLOAT param_elevation  = pd[vs_system::ObjConfig::BOX_FLAG_6];
		FLOAT param_azimuth    = pd[vs_system::ObjConfig::BOX_FLAG_7];


	// ---- set light position

		// ---- light angle stuff
			FLOAT t2r = param_elevation * (FLOAT)VS_RADIANS_PER_DEGREE;
			FLOAT phi = param_azimuth + 90;
			FLOAT p2r = phi * (FLOAT)VS_RADIANS_PER_DEGREE;

		// ---- light rotate about x axis
			// y' = y*cos q - z*sin q
			// z' = y*sin q + z*cos q
			FLOAT x_lgt_pos =  0;
			FLOAT y_lgt_pos =  param_distance * sinf(t2r);
			FLOAT z_lgt_pos = -param_distance * cosf(t2r);

		// ---- Y rotation
			// x' = z*sin q + x*cos q
			// z' = z*cos q - x*sin q
			//FLOAT x_lgt_pos = z * sin(p2r) + x * cos(p2r);
			//FLOAT y_lgt_pos = y;
			//FLOAT z_lgt_pos = z * cos(p2r) - x * sin(p2r);

	// ---- vertex
		UINT  vertex_size      = context->GetVertexSize();
		UINT  vertex_format    = context->GetVertexFormat();
		FLOAT incr_vertex_rows = 1 / (FLOAT)sphere_cel_rows;
		FLOAT incr_vertex_cols = 1 / (FLOAT)sphere_cel_cols;


	// ---- total latitude radians
		FLOAT latitiude_diff_radians = (sphere_lat_max - sphere_lat_min) * (FLOAT)VS_RADIANS_PER_DEGREE;
		FLOAT latitiude_dmin_radians = sphere_lat_min * (FLOAT)VS_RADIANS_PER_DEGREE;


	// ---- diffuse colour
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} diffuse;
		diffuse.d = VS_D3DCOLOR_COLORVALUE(sphere_red, sphere_green, sphere_blue, 1.0f);


	// ---- buffer
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- each row of the mesh
		for (UINT row = 0; row<sphere_rows; row++)
		{

			// ---- X axis rotation scalar
				FLOAT x_axis_scalar = (FLOAT)row * (FLOAT)incr_vertex_rows;


			// ---- angle
				FLOAT x_rotation_radians = x_axis_scalar * latitiude_diff_radians + latitiude_dmin_radians;


			// ---- 3D rotate 0,0,1 to RY,0,RZ_1
				FLOAT y = sphere_radius * sin(x_rotation_radians);
				FLOAT z = sphere_radius * cos(x_rotation_radians);


			// ---- 2D texture
				FLOAT edge = 0.01f;
				FLOAT v = 1 - ( (FLOAT)row / (FLOAT)sphere_cel_rows ) - edge;


			// ---- for each column of the mesh
				for (UINT col = 0; col<sphere_cols; col++)
				{

					// ---- Y axis rotation scalar
						FLOAT y_axis_scalar = (FLOAT)col * incr_vertex_cols;

					// ---- 3D rotate RY,0,RZ_1 to RY,RX,RZ_2
						FLOAT y_rotation_radians = y_axis_scalar * -360 * (FLOAT)VS_RADIANS_PER_DEGREE;
						*(f)     =  z * sinf(y_rotation_radians);
						*(f + 1) =  y;
						*(f + 2) = -z * cosf(y_rotation_radians);
						FLOAT *g = f;
						f += 3;

					// ---- 3D normal TBD - add rotation
						if ( vertex_format & VS_NORMAL )
						{
							FLOAT quotient = 1 / sphere_radius;
							*(f)     = *(g    ) * quotient;
							*(f + 1) = *(g + 1) * quotient;
							*(f + 2) = *(g + 2) * quotient;
							f += 3;
						}

					// ---- position
						FLOAT x = *(g)     + x_lgt_pos;
						FLOAT y = *(g + 1) + y_lgt_pos;
						FLOAT z = *(g + 2) + z_lgt_pos;

					// ---- Y rotation
						// x' = z*sin q + x*cos q
						// z' = z*cos q - x*sin q
						*(g)     = z * sin(p2r) + x * cos(p2r) + x_off;
						*(g + 1) = y                           + y_off;
						*(g + 2) = z * cos(p2r) - x * sin(p2r) + z_off;

					// ---- diffuse
						if ( vertex_format & VS_DIFFUSE )
						{
							*(f) = diffuse.f;
							f++;
						}

					// ---- 2D texture
						if ( vertex_format & VS_TEX1 )
						{
							FLOAT u = (FLOAT)col / (FLOAT)sphere_cel_cols * (1 - edge) + edge / 2;
							*(f    ) = 1 - u;
							*(f + 1) = v - edge / 2;
							f += 2;
						}
				}
		}

	return VS_OK;
}


// ---------- Callback_S3_AcqLgtSpot_GetIndexBuffer ----------
INT ObjConfig::Callback_S3_AcqLgtSpot_GetIndexBuffer(vs_system::ElmContext *context)
{

	// ---- get supplied and parameter data
		FLOAT *pd = context->GetFloatParameter();


	// ---- vertex
		INT sphere_cel_cols = (UINT)pd[SPHERE_CEL_COLS];
		INT sphere_cel_rows = (UINT)pd[SPHERE_CEL_ROWS];
		INT sphere_cols     = sphere_cel_cols + 1;


	// ---- buffer
		#if VS_INDEX_BUFFER_FORMAT == 32
		DWORD *i = context->GetIndexBufferPtr();
		#else
		WORD *i = context->GetIndexBufferPtr();
		#endif


	// ---- each row and column of the mesh
		for (INT row = 0; row<sphere_cel_rows; row++)
		{
			INT row_offset_low = row * sphere_cols;
			INT row_offset_high = row_offset_low + sphere_cols;
			for (INT col = 0; col<sphere_cel_cols; col++)
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

	return VS_OK;
}

