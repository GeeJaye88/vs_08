////////////////////////////////////////////////////////////////////////////////

// ---------- vs_obj_mesh_edge.hpp ----------
/*!
\file vs_obj_mesh_edge.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_Mesh_Quad_Configure ----------
INT ObjConfig::Callback_Mesh_Quad_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get supplied and parameter data
		UINT type   = (UINT)pd[MESH_TYPE];
		UINT ap_ref = (UINT)pd[MESH_AP_REF];


	// ---- atlas page
		context->SetAtlasPage(0, ap_ref);


	// ---- overide ?
		UINT format = (UINT)(VS_XYZ | VS_NORMAL);
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(FALSE);
			context->SetColourVertex(FALSE);
			context->SetMultiState(FALSE);

			// ---- as per mesh
				switch ( type )
				{
					default:
					case 0: // wireframe
						context->SetSolid(FALSE);
						context->SetTextured(FALSE);
						break;
					case 1: // wireframe & texture
						format = (UINT)(VS_XYZ | VS_NORMAL | VS_TEX1);
						context->SetSolid(FALSE);
						context->SetTextured(TRUE);
						break;
					case 2: // solid
						format = (UINT)(VS_XYZ | VS_NORMAL);
						context->SetSolid(TRUE);
						context->SetTextured(FALSE);
						break;
					case 3: // solid & textured
						format = (UINT)(VS_XYZ | VS_NORMAL | VS_TEX1);
						context->SetSolid(TRUE);
						context->SetTextured(FALSE);
						break;
					case 4: // solid & not shaded & textured
						format = (UINT)(VS_XYZ | VS_TEX1);
						context->SetLit(FALSE);
						context->SetSolid(TRUE);
						context->SetTextured(TRUE);
				}

			context->SetVisible(TRUE);
		}


	// ---- primitive type
		context->SetPrimitiveType(VS_TRIANGLELIST);


	// ---- vertex buffer
		context->SetVertexCount (6);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_Mesh_Quad_GetVertexBuffer);


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
		context->SetPrimitiveCount(2);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ---------- Callback_Mesh_Quad_GetVertexBuffer ----------
INT ObjConfig::Callback_Mesh_Quad_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get parameter data
		UINT type    = (UINT)pd[MESH_TYPE];
		UINT ap_ref  = (UINT)pd[MESH_AP_REF];

		FLOAT red    = (FLOAT)pd[MESH_RED];
		FLOAT green  = (FLOAT)pd[MESH_GREEN];
		FLOAT blue   = (FLOAT)pd[MESH_BLUE];
		FLOAT width  = (FLOAT)pd[MESH_WIDTH];
		FLOAT depth  = (FLOAT)pd[MESH_DEPTH];
		FLOAT height = (FLOAT)pd[MESH_HEIGHT]; // hgt not used
		FLOAT x_off  = (FLOAT)pd[MESH_XOFF];
		FLOAT y_off  = (FLOAT)pd[MESH_YOFF];
		FLOAT z_off  = (FLOAT)pd[MESH_ZOFF];


	// ---- dimensions by 2
		FLOAT wby2 = width / 2;
		FLOAT hby2 = height / 2;              // hgt not used
		FLOAT dby2 = depth / 2;


	// ---- vertex
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_count  = context->GetVertexCount();
		UINT vertex_format = context->GetVertexFormat();
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- diffuse
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} colour;
		colour.d = VS_D3DCOLOR_COLORVALUE(
				red,
				green,
				blue,
				1.0f
			);


	// ---- quad vertices
		struct vtx
		{
			float x, y, z, xn, yn, zn, u, v;
		};
		vtx v[6]
		{
			{ -1,  0, -1,  0,  1,  0,  0,  1 },
			{ -1,  0,  1,  0,  1,  0,  0,  0 },
			{  1,  0,  1,  0,  1,  0,  1,  0 },
			{  1,  0,  1,  0,  1,  0,  1,  0 },
			{  1,  0, -1,  0,  1,  0,  1,  1 },
			{ -1,  0, -1,  0,  1,  0,  0,  1 },
		};


	// ---- vertices,...
		for (int i = 0; i<6; i++)
		{

			if ( vertex_format & VS_XYZ )
			{
				*(f + 0) = v[i].x * wby2 + x_off;
				*(f + 1) = v[i].y        + y_off;
				*(f + 2) = v[i].z * dby2 + z_off;
				f += 3;
			}

			if ( vertex_format & VS_NORMAL )
			{
				*(f + 0) = v[i].xn;
				*(f + 1) = v[i].yn;
				*(f + 2) = v[i].zn;
				f += 3;
			}

			if ( vertex_format & VS_DIFFUSE )
			{
				*(f) = colour.f; f++;
			}

			if ( vertex_format & VS_SPECULAR )
			{
				*(f) = 1;
				f++;
			}

			if ( vertex_format & VS_TEX1 )
			{
				*(f + 0) = v[i].u;
				*(f + 1) = v[i].v;
				f+=2;
			}
		
		}


	return VS_OK;
}