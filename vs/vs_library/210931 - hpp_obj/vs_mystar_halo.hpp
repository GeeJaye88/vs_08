////////////////////////////////////////////////////////////////////////////////

// ---------- vs_mystar_halo.hpp ----------
/*!
\file vs_mystar_halo.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_MyStar_Halo_Configure ----------
INT ObjConfig::Callback_MyStar_Halo_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get parameter data
		UINT halo_edges = (UINT)pd[HALO_EDGES];


	// ---- overide ?
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(TRUE);
			context->SetColourVertex(TRUE);
			context->SetMultiState(FALSE);
			context->SetSolid(TRUE);
			context->SetTextured(FALSE);
			context->SetVisible(TRUE);
		}


	// ---- index buffer
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


	// ---- primitive type
		context->SetPrimitiveType(VS_TRIANGLELIST);


	// ---- vertex buffer
		context->SetVertexCount( halo_edges * 6 );
		context->SetVertexFormat((UINT)(VS_XYZ | VS_DIFFUSE));
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_MyStar_Halo_GetVertexBuffer);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
			);
		context->SetPrimitiveCount(halo_edges * 2);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ---------- Callback_MyStar_Halo_GetVertexBuffer ----------
INT ObjConfig::Callback_MyStar_Halo_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get supplied and parameter data
		UINT  halo_edges    = (UINT)pd[HALO_EDGES];
		FLOAT halo_red     = pd[HALO_RED];
		FLOAT halo_green   = pd[HALO_GREEN];
		FLOAT halo_blue    = pd[HALO_BLUE];
		FLOAT halo_alpha   = pd[HALO_ALPHA];
		FLOAT px           = pd[HALO_X];
		FLOAT py           = pd[HALO_Y];
		FLOAT pz           = pd[HALO_Z];
		FLOAT halo_degrees = pd[HALO_DEGREES];
		FLOAT halo_min_rad = pd[HALO_RAD_MIN];
		FLOAT halo_max_rad = pd[HALO_RAD_MAX];


	// ---- vertex
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_count  = context->GetVertexCount();
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- diffuse
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} dif_col;
		dif_col.d = VS_D3DCOLOR_COLORVALUE(halo_red, halo_green, halo_blue, halo_alpha);


	// ---- halo
		FLOAT x0 = 0, y0 = halo_min_rad;
		FLOAT x1 = 0, y1 = halo_max_rad;
		for (DWORD i = 1; i <= halo_edges; i++)
		{

			FLOAT rmu = (FLOAT)i / (FLOAT)halo_edges;
			FLOAT ang = rmu * 360.0f;
			float c = (FLOAT)cos(ang*VS_RADIANS_PER_DEGREE);
			float s = (float)sin(ang*VS_RADIANS_PER_DEGREE);
			FLOAT x2 = halo_min_rad * s;
			FLOAT y2 = halo_min_rad * c;
			FLOAT x3 = halo_max_rad * s;
			FLOAT y3 = halo_max_rad * c;

			*(f + 0) = x0; *(f + 1) = y0; *(f + 2) = 0; *(f + 3) = dif_col.f; f += 4;
			*(f + 0) = x1; *(f + 1) = y1; *(f + 2) = 0; *(f + 3) = dif_col.f; f += 4;
			*(f + 0) = x2; *(f + 1) = y2; *(f + 2) = 0; *(f + 3) = dif_col.f; f += 4;

			*(f + 0) = x2; *(f + 1) = y2; *(f + 2) = 0; *(f + 3) = dif_col.f; f += 4;
			*(f + 0) = x1; *(f + 1) = y1; *(f + 2) = 0; *(f + 3) = dif_col.f; f += 4;
			*(f + 0) = x3; *(f + 1) = y3; *(f + 2) = 0; *(f + 3) = dif_col.f; f += 4;

			x0 = x2; y0 = y2;
			x1 = x3; y1 = y3;

		}

	return VS_OK;
}

