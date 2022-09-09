////////////////////////////////////////////////////////////////////////////////

// ---------- vs_s3_surf_mount.hpp ----------
/*!
\file vs_s3_surf_mount.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_SurfMount_Configure ----------
INT ObjConfig::Callback_SurfMount_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get supplied and parameter data
		UINT cube_face_type   = (UINT)pd[BOX_FACE_TYPE];
		UINT cube_face_ap_ref = (UINT)pd[BOX_FACE_AP_REF];


	// ---- no texture - no atlas page
		context->SetAtlasPage(0, 0);


	// ---- overide ?
		UINT format = (UINT)(VS_XYZ | VS_NORMAL); // wireframe
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(false);
			context->SetColourVertex(true);
			context->SetMultiState(false);

			switch ( cube_face_type )
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
			}

			context->SetVisible(true);
		}


	// ---- index buffer
		context->SetIndexCount( 0);
		context->SetIndexBufferCallback(NULL);


	// ---- primitive type
		context->SetPrimitiveType(VS_TRIANGLELIST);


	// ---- vertex buffer ( 4 sections * 8 triangles * 3 vertices )
		context->SetVertexCount( 4 * 8 * 3 );
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_SurfMount_GetVertexBuffer);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
			);
	// ---- primitives ( 4 sections * 8 triangles )
		context->SetPrimitiveCount( 4 * 8 );


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ---------- Callback_SurfMount_GetVertexBuffer ----------
INT ObjConfig::Callback_SurfMount_GetVertexBuffer(vs_system::ElmContext *context)
{


	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get supplied and parameter data
		UINT  mode    = (UINT)pd[BOX_FACE_MODE];
		UINT  type    = (UINT)pd[BOX_FACE_TYPE];
		UINT  ap_ref  = (UINT)pd[BOX_FACE_AP_REF];
		FLOAT red     = pd[BOX_FACE_RED];
		FLOAT green   = pd[BOX_FACE_GREEN];
		FLOAT blue    = pd[BOX_FACE_BLUE];
		FLOAT scale   = pd[BOX_FACE_SCALE];
		FLOAT w       = pd[vs_system::ObjConfig::BOX_FACE_WIDTH]  * scale;
		FLOAT h       = pd[vs_system::ObjConfig::BOX_FACE_HEIGHT] * scale;
		FLOAT d       = pd[vs_system::ObjConfig::BOX_FACE_DEPTH]  * scale;
		FLOAT x       = pd[vs_system::ObjConfig::BOX_FACE_X];
		FLOAT y       = pd[vs_system::ObjConfig::BOX_FACE_Y];
		FLOAT z       = pd[vs_system::ObjConfig::BOX_FACE_Z];


	// ---- inner dimensions
		FLOAT w_inn = w / 2;
		FLOAT h_inn = h / 2;
		FLOAT d_inn = d / 2;


	// ---- outer dimensions (same height)
		FLOAT w_out = w_inn * 1.25f;
		FLOAT h_out = h_inn * 1.0f;
		FLOAT d_out = d_inn * 1.25f;


	// ---- vertex
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_count  = context->GetVertexCount();
		UINT vertex_format = context->GetVertexFormat();
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- diffuse
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} color;
		color.d = VS_D3DCOLOR_COLORVALUE(red,green,blue,1);


	// ---- vertex
		struct vtx
		{
			float x, y, z, xn, yn, zn;
		};


	// ---- triangle vertices
		vtx tri_vtx[24]=
		{

			{  -w_out, -h_out, -d_out,  0,  0, -1 },
			{  -w_out,  h_out, -d_out,  0,  0, -1 },
			{   w_out,  h_out, -d_out,  0,  0, -1 },

			{   w_out,  h_out, -d_out,  0,  0, -1 },
			{   w_out, -h_out, -d_out,  0,  0, -1 },
			{  -w_out, -h_out, -d_out,  0,  0, -1 },

			{  -w_out,  h_out, -d_out,  0,  1,  0 },
			{  -w_inn,  h_inn, -d_inn,  0,  1,  0 },
			{   w_inn,  h_inn, -d_inn,  0,  1,  0 },

			{   w_inn,  h_inn, -d_inn,  0,  1,  0 },
			{   w_out,  h_out, -d_out,  0,  1,  0 },
			{  -w_out,  h_out, -d_out,  0,  1,  0 },

			{  -w_inn,  h_inn, -d_inn,  0,  0,  1 },
			{  -w_inn, -h_inn, -d_inn,  0,  0,  1 },
			{   w_inn, -h_inn, -d_inn,  0,  0,  1 },

			{   w_inn, -h_inn, -d_inn,  0,  0,  1 },
			{   w_inn,  h_inn, -d_inn,  0,  0,  1 },
			{  -w_inn,  h_inn, -d_inn,  0,  0,  1 },

			{  -w_inn, -h_inn, -d_inn,  0, -1,  0 },
			{  -w_out, -h_out, -d_out,  0, -1,  0 },
			{   w_out, -h_out, -d_out,  0, -1,  0 },

			{   w_out, -h_out, -d_out,  0, -1,  0 },
			{   w_inn, -h_inn, -d_inn,  0, -1,  0 },
			{  -w_inn, -h_inn, -d_inn,  0, -1,  0 },

		};


	// ---- 4 sections with 8 triangles with 3 vertices
		for ( UINT sect=0; sect<4; sect++)
		{

			FLOAT ang = (FLOAT)sect * 90;
			FLOAT rad = ang * (FLOAT)VS_RADIANS_PER_DEGREE;
			FLOAT cosq = (FLOAT)cos(rad);
			FLOAT sinq = (FLOAT)sin(rad);

			UINT i = 0;
			for (UINT t=0; t<8; t++)
			{
				for (UINT v=0; v<3; v++)
				{
					
					if ( vertex_format & VS_XYZ )
					{
						*(f + 0) = x + ( tri_vtx[i].z * cosq - tri_vtx[i].x * sinq);
						*(f + 1) = y +   tri_vtx[i].y;
						*(f + 2) = z + ( tri_vtx[i].z * sinq + tri_vtx[i].x * cosq);
						f += 3;
					}

					if ( vertex_format & VS_NORMAL )
					{
						*(f + 0) = (tri_vtx[i].zn * cosq - tri_vtx[i].xn * sinq);
						*(f + 1) =  tri_vtx[i].yn;
						*(f + 2) = (tri_vtx[i].zn * sinq + tri_vtx[i].xn * cosq);
						f += 3;
					}

					if ( vertex_format & VS_DIFFUSE )
					{
						*(f) = color.f;
						f++;
					}

					i++;
		
				}
			}
		}

	return VS_OK;
}


// ---------- Callback_SurfMount_GetIndexBuffer ----------
INT ObjConfig::Callback_SurfMount_GetIndexBuffer(vs_system::ElmContext *context)
{

	return VS_OK;
}
