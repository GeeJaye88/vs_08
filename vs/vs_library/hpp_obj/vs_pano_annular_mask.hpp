////////////////////////////////////////////////////////////////////////////////

// ---------- vs_pano_annular_mask.hpp ----------
/*!
\file vs_pano_annular_mask.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_PanoAnnularMask_Configure ----------
INT ObjConfig::Callback_PanoAnnularMask_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- local
		FLOAT *p = context->GetFloatParameter();


	// ---- atlas page
		context->SetAtlasPage(0, 0);


	// ---- overide -- ?
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(false);
			context->SetColourVertex(true);
			context->SetMultiState(false);
			context->SetSolid(true);
			context->SetTextured(FALSE);
			context->SetVisible(true);
		}


	// ---- index buffer
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


	// ---- vertex buffer ---- - 60 segments comprising 3 triangles of 3 vertices
		context->SetVertexCount( 60 * 9 );
		context->SetVertexFormat((UINT)( VS_XYZRHW | VS_DIFFUSE ));
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_PanoAnnularMask_GetVertexBuffer);


	// ---- primitive parameters ----
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
			);
		context->SetPrimitiveCount( 180 );
		context->SetPrimitiveType(VS_TRIANGLELIST);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ---------- Callback_PanoAnnularMask_GetVertexBuffer ----------
INT ObjConfig::Callback_PanoAnnularMask_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- pano parameters
		FLOAT v_radius_min  = pi_obj_pano->min_radius;
		FLOAT v_radius_max  = pi_obj_pano->max_radius;
		FLOAT u_flip        = U_FLIP_LOGIC;
		FLOAT v_flip        = V_FLIP_LOGIC;


	// ----- scalar to "frame" full annular ROI
		FLOAT scalar = 1 / (v_radius_max  > 1.0f ? v_radius_max : 1.0f);


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
		FLOAT outer  = sqrt(wby2*wby2 + hby2*hby2);
		FLOAT cx     = width/2 + left;
		FLOAT cy     = height/2 + top;


	// ---- diffuse solid colour
		union colour_union_t {
			DWORD d;
			FLOAT f; 
		} black;
		black.d = VS_D3DCOLOR_COLORVALUE( 0.0f, 0.0f, 0.0f, 1.0f);


	// ---- "slice" generator, a radial xy 2 line list with ROI radii gap
		float xy[4][2] = {
			{ 0, 0 },
			{ hby2  * v_radius_min, 0 },
			{ hby2  * v_radius_max, 0 },
			{ outer * v_radius_max, 0 },
		};


	// ----buffer pointer
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- vertex
		UINT vertex_size = context->GetVertexSize();
		UINT vertex_count = context->GetVertexCount();


	// ---- each segment....
	FLOAT zed = 0.9f;
	FLOAT rhw = 1;
    for ( UINT segment=0; segment<60; segment++ )
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


		// ---- assemble 3 triangles (centre to inner radius triangle, and outer radius to outer quad)

			// 1
			*(f) = rxy[0][0][0]; *(f+1) = rxy[0][0][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;
			*(f) = rxy[1][1][0]; *(f+1) = rxy[1][1][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;
			*(f) = rxy[0][1][0]; *(f+1) = rxy[0][1][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;

			// 2
			*(f) = rxy[1][2][0]; *(f+1) = rxy[1][2][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;
			*(f) = rxy[1][3][0]; *(f+1) = rxy[1][3][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;
			*(f) = rxy[0][2][0]; *(f+1) = rxy[0][2][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;

			// 3
			*(f) = rxy[1][3][0]; *(f+1) = rxy[1][3][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;
			*(f) = rxy[0][3][0]; *(f+1) = rxy[0][3][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;
			*(f) = rxy[0][2][0]; *(f+1) = rxy[0][2][1]; *(f+2) = zed; *(f+3) = rhw; *(f+4) = black.f; f+=vertex_size;

	}

	return VS_OK;
}
