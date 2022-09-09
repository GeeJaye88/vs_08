////////////////////////////////////////////////////////////////////////////////

// ---------- vs_pano_annular_aoi.hpp ----------
/*!
\file vs_pano_annular_aoi.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_PanoAnnularAoi_Configure ----------
INT ObjConfig::Callback_PanoAnnularAoi_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- local
		FLOAT *p = context->GetFloatParameter();


	// ---- atlas page
		context->SetAtlasPage(0, 0);


	// ---- overide ?
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(true);
			context->SetColourVertex(true);
			context->SetMultiState(false);
			context->SetSolid(true);
			context->SetTextured(FALSE);
			context->SetVisible(true);
		}


	// ---- index buffer
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


	// ---- vertex buffer - 120 segments comprising 4 triangles of 3 vertices
		context->SetVertexCount( 120 * 12 );
		context->SetVertexFormat((UINT)( VS_XYZRHW | VS_DIFFUSE ));
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_PanoAnnularAoi_GetVertexBuffer);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
			);
		context->SetPrimitiveCount(120 * 4);
		context->SetPrimitiveType(VS_TRIANGLELIST);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ---------- Callback_PanoAnnularAoi_GetVertexBuffer ----------
INT ObjConfig::Callback_PanoAnnularAoi_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- local pano parameters
		FLOAT v_aoi_min  = pi_obj_pano->exposure_aoi_min;
		FLOAT v_aoi_max  = pi_obj_pano->exposure_aoi_max;
		FLOAT u_flip     = U_FLIP_LOGIC;
		FLOAT v_flip     = V_FLIP_LOGIC;


	// ---- geometry scalar to "frame" full annular ROI
		FLOAT scalar = 1 / (v_aoi_max  > 1.0f ? v_aoi_max : 1.0f);


	// ---- vertex
		FLOAT *sp = context->GetGfxFloatParameter();
		FLOAT left   = sp[VS_PANEL_LEFT];
		FLOAT top    = sp[VS_PANEL_TOP];
		FLOAT right  = sp[VS_PANEL_RIGHT];
		FLOAT bottom = sp[VS_PANEL_BOTTOM];
		FLOAT width  = right - left - 1;
		FLOAT height = bottom - top - 1;
		FLOAT hby2   = height/2;
		FLOAT wby2   = width/2;
		FLOAT cx     = width/2 + left;
		FLOAT cy     = height/2 + top;


	// ---- thickness of aoi line
		FLOAT tofl   = 0.005f * hby2;


	// ---- diffuse Colour
		union colour_union_t {
			DWORD d;
			FLOAT f; 
		} black;
		black.d = VS_D3DCOLOR_COLORVALUE( 1.0f,  1.0f,  1.0f,  0.2f);


	// ---- the "slice" generator, a radial xy 2 line list with AOI gap
		float xy[4][2] = {
			{ hby2  * v_aoi_min - tofl, 0 },
			{ hby2  * v_aoi_min,        0 },
			{ hby2  * v_aoi_max,        0 },
			{ hby2  * v_aoi_max + tofl, 0 },
		};
	

	// ---- buffer
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- vertex
		UINT vertex_size = context->GetVertexSize();
		UINT vertex_count = context->GetVertexCount();

	// ---- each segment
		FLOAT zed = 0.9f;
		FLOAT rhw = 1;
		for ( UINT segment=0; segment<120; segment++ )
		{

			// ---- "this" and "next" rotation in radians  
				FLOAT theta[2] =
				{ 
					(FLOAT)segment*6*(FLOAT)VS_RADIANS_PER_DEGREE,
					(FLOAT)(segment+1)*6 *(FLOAT)VS_RADIANS_PER_DEGREE
				};


			// ---- rotate, scale and then centre 2 copies of the "slice" generator
				FLOAT rxy[2][4][2];
				for ( UINT a=0; a<2; a++ )
				{
					for ( UINT i=0; i<4; i++ )
					{
						rxy[a][i][0] = ( cos(theta[a]) * xy[i][0] - sin(theta[a]) * xy[i][1]) * scalar + cx;
						rxy[a][i][1] = ( sin(theta[a]) * xy[i][0] + cos(theta[a]) * xy[i][1]) * scalar + cy;
					}
				}


			// ---- assemble 4 triangles

				// 1
				*(f) = rxy[1][0][0]; *(f+1) = rxy[1][0][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;
				*(f) = rxy[1][1][0]; *(f+1) = rxy[1][1][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;
				*(f) = rxy[0][0][0]; *(f+1) = rxy[0][0][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;

				// 2
				*(f) = rxy[1][1][0]; *(f+1) = rxy[1][1][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;
				*(f) = rxy[0][1][0]; *(f+1) = rxy[0][1][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;
				*(f) = rxy[0][0][0]; *(f+1) = rxy[0][0][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;

				// 3
				*(f) = rxy[1][2][0]; *(f+1) = rxy[1][2][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;
				*(f) = rxy[1][3][0]; *(f+1) = rxy[1][3][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;
				*(f) = rxy[0][2][0]; *(f+1) = rxy[0][2][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;

				// 4
				*(f) = rxy[1][3][0]; *(f+1) = rxy[1][3][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;
				*(f) = rxy[0][3][0]; *(f+1) = rxy[0][3][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;
				*(f) = rxy[0][2][0]; *(f+1) = rxy[0][2][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;

		}

	return VS_OK;
}
