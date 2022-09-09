////////////////////////////////////////////////////////////////////////////////

// ---------- vs_mystar_rings.hpp ----------
/*!
\file vs_mystar_rings.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_MyStar_Rings_Configure ----------
INT ObjConfig::Callback_MyStar_Rings_Configure(vs_system::ElmContext *context)
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


	// IF -- not overide -- THEN
		UINT format = (UINT)(VS_XYZ | VS_DIFFUSE); // wireframe
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(true);
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
					format = (UINT)(VS_XYZ | VS_NORMAL);
					context->SetSolid(TRUE);
					context->SetTextured(FALSE);
					break;
				case 2: // textured
					format = (UINT)(VS_XYZ | VS_NORMAL | VS_TEX1);
					context->SetSolid(TRUE);
					context->SetTextured(sphere_ap_ref == 0 ? FALSE : TRUE);
					break;
			}

			context->SetVisible(TRUE);
		}


	// ---- index buffer
		context->SetIndexCount(sphere_cel_cols * sphere_cel_rows * 6);
		context->SetIndexBufferCallback(&Callback_MyStar_Rings_GetIndexBuffer);


	// ---- vertex buffer
		context->SetVertexCount( sphere_cols * sphere_rows );
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_MyStar_Rings_GetVertexBuffer);


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


// ----------  Callback_MyStar_Rings_GetVertexBuffer ----------
/*!

*/
INT ObjConfig::Callback_MyStar_Rings_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- get supplied and parameter data
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- parameters
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
		FLOAT sphere_lat_min  = pd[SPHERE_LAT_MIN];
		FLOAT sphere_lat_max  = pd[SPHERE_LAT_MAX];
		FLOAT sphere_rad_min  = pd[SPHERE_RAD_MIN];
		FLOAT sphere_rad_max  = pd[SPHERE_RAD_MAX];


	// ---- vertex
		UINT  vertex_size      = context->GetVertexSize();
		UINT  vertex_format    = context->GetVertexFormat();
		FLOAT incr_vertex_rows = 1 / (FLOAT)sphere_cel_rows;
		FLOAT incr_vertex_cols = 1 / (FLOAT)sphere_cel_cols;


	// ---- diffuse Colour
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} white;
		white.d = VS_D3DCOLOR_COLORVALUE(sphere_red, sphere_green, sphere_blue, 1.0f);


	// ---- buffer
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- each row of the mesh
	for (UINT row = 0; row<sphere_rows; row++)
	{

		// ---- X axis scalar
			FLOAT x_scalar = (FLOAT)row * (FLOAT)incr_vertex_rows;


		// ---- 
			FLOAT x = x_scalar * (sphere_rad_max - sphere_rad_min) + sphere_rad_min;
			FLOAT y = 0;
			FLOAT z = 0;


		// ---- 2D texture
			FLOAT v = 1 - ( (FLOAT)row / (FLOAT)sphere_cel_rows ) / 4 - ( sphere_ap_sect == 2 ? 0.0f : 0.5f);


		// ---- for each column of the mesh
		for (UINT col = 0; col<sphere_cols; col++)
		{

			// ---- Y axis rotation
				FLOAT y_rot = (FLOAT)col * incr_vertex_cols * 360;


			// ---- 2D y rotate
				FLOAT y_rot_rad = y_rot * (FLOAT)VS_RADIANS_PER_DEGREE;
				*(f)     =  -x * sinf(y_rot_rad);
				*(f + 1) =   y;
				*(f + 2) =   x * cosf(y_rot_rad);
				FLOAT *g = f;
				f += 3;


			// ---- diffuse
				if ( vertex_format & VS_DIFFUSE )
				{
					*(f) = white.f;
					f++;
				}


			// ---- 3D normal
				if ( vertex_format & VS_NORMAL )
				{
					*(f)     = 0;
					*(f + 1) = 1;
					*(f + 2) = 0;
					f += 3;
				}


			// ---- 2D texture
				if ( vertex_format & VS_TEX1 )
				{
					//FLOAT u = (FLOAT)col / (FLOAT)sphere_cel_cols;
					*(f) = 0.5f; //u * 0.998f + 0.001f;
					*(f + 1) = v;
					f += 2;
				}
		}
	}

	return VS_OK;
}


// ---------- Callback_MyStar_Rings_GetIndexBuffer ----------
INT ObjConfig::Callback_MyStar_Rings_GetIndexBuffer(vs_system::ElmContext *context)
{

	// ---- get supplied and parameter data
		FLOAT *sd = context->GetGfxFloatParameter();
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

