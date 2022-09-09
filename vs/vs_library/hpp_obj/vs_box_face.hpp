////////////////////////////////////////////////////////////////////////////////

// ---------- vs_box_face.hpp ----------
/*!
\file vs_box_face.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_BoxFace_Configure ----------
INT ObjConfig::Callback_BoxFace_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get supplied and parameter data
		UINT cube_face_type   = (UINT)pd[BOX_FACE_TYPE];
		UINT cube_face_ap_ref = (UINT)pd[BOX_FACE_AP_REF];


	// ---- atlas page
		//context->SetAtlasPages(cube_face_ap_ref);
		context->SetAtlasPage(0, cube_face_ap_ref);


	// ---- overide ?
		UINT format = (UINT)(VS_XYZ | VS_NORMAL); // wireframe
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(false);
			context->SetColourVertex(true);
			context->SetMultiState(false);

			switch (cube_face_type)
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
					context->SetTextured(TRUE);
					break;
			}

			context->SetVisible(true);
		}


	// ---- index buffer
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


	// ---- primitive type
		context->SetPrimitiveType(VS_TRIANGLELIST);


	// ---- vertex buffer
		context->SetVertexCount(6);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_BoxFace_GetVertexBuffer);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
			);
		context->SetPrimitiveCount(2);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ---------- Callback_BoxFace_GetVertexBuffer ----------
INT ObjConfig::Callback_BoxFace_GetVertexBuffer(vs_system::ElmContext *context)
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
		FLOAT explode = pd[BOX_FACE_EXPLODE] + 1;
			explode = mode < 6 ? explode : -explode;


		FLOAT wby2    = w / 2;
		FLOAT hby2    = h / 2;
		FLOAT dby2    = d / 2;


	// ---- vertex
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_count  = context->GetVertexCount();
		UINT vertex_format = context->GetVertexFormat();
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- diffuse
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} white;
		white.d = VS_D3DCOLOR_COLORVALUE(
				red,
				green,
				blue,
				1.0f
			);


	// ---- box faces lut
		struct vtx
		{
			float x, y, z, xn, yn, zn, u, v;
		};

		vtx q[48]=
		{

			// 0 - 
			{   wby2,  hby2,  0,  0,  0,  1,  0,  0 },
			{  -wby2,  hby2,  0,  0,  0,  1,  1,  0 },
			{  -wby2, -hby2,  0,  0,  0,  1,  1,  1 },
			{   wby2, -hby2,  0,  0,  0,  1,  0,  1 },

			// 1 - 
			{   0,  hby2, -dby2,  1,  0,  0,  0,  0 },
			{   0,  hby2,  dby2,  1,  0,  0,  1,  0 },
			{   0, -hby2,  dby2,  1,  0,  0,  1,  1 },
			{   0, -hby2, -dby2,  1,  0,  0,  0,  1 },

			// 2
			{  -wby2,  hby2,  0,  0,  0, -1,  0,  0 },
			{   wby2,  hby2,  0,  0,  0, -1,  1,  0 },
			{   wby2, -hby2,  0,  0,  0, -1,  1,  1 },
			{  -wby2, -hby2,  0,  0,  0, -1,  0,  1 },

			// 3
			{   0,  hby2,  dby2, -1,  0,  0,  0,  0 },
			{   0,  hby2, -dby2, -1,  0,  0,  1,  0 },
			{   0, -hby2, -dby2, -1,  0,  0,  1,  1 },
			{   0, -hby2,  dby2, -1,  0,  0,  0,  1 },

			// 4
			{  -wby2,  0,  dby2,  0,  1,  0,  0,  0 },
			{   wby2,  0,  dby2,  0,  1,  0,  1,  0 },
			{   wby2,  0, -dby2,  0,  1,  0,  1,  1 },
			{  -wby2,  0, -dby2,  0,  1,  0,  0,  1 },

			// 5
			{   wby2,  0,  dby2,  0, -1,  0,  0,  0 },
			{  -wby2,  0,  dby2,  0, -1,  0,  1,  0 },
			{  -wby2,  0, -dby2,  0, -1,  0,  1,  1 },
			{   wby2,  0, -dby2,  0, -1,  0,  0,  1 },

			// 6
			{  -wby2,  hby2,  0,  0,  0, -1,  0,  0 },
			{   wby2,  hby2,  0,  0,  0, -1,  1,  0 },
			{   wby2, -hby2,  0,  0,  0, -1,  1,  1 },
			{  -wby2, -hby2,  0,  0,  0, -1,  0,  1 },

			// 7
			{   0,  hby2,  dby2, -1,  0,  0,  0,  0 },
			{   0,  hby2, -dby2, -1,  0,  0,  1,  0 },
			{   0, -hby2, -dby2, -1,  0,  0,  1,  1 },
			{   0, -hby2,  dby2, -1,  0,  0,  0,  1 },

			// 8
			{   wby2,  hby2,  0,  0,  0,  1,  0,  0 },
			{  -wby2,  hby2,  0,  0,  0,  1,  1,  0 },
			{  -wby2, -hby2,  0,  0,  0,  1,  1,  1 },
			{   wby2, -hby2,  0,  0,  0,  1,  0,  1 },

			// 9
			{   0,  hby2, -dby2,  1,  0,  0,  0,  0 },
			{   0,  hby2,  dby2,  1,  0,  0,  1,  0 },
			{   0, -hby2,  dby2,  1,  0,  0,  1,  1 },
			{   0, -hby2, -dby2,  1,  0,  0,  0,  1 },

			// 10
			{   wby2,  0,  dby2,  0, -1,  0,  0,  0 },
			{  -wby2,  0,  dby2,  0, -1,  0,  1,  0 },
			{  -wby2,  0, -dby2,  0, -1,  0,  1,  1 },
			{   wby2,  0, -dby2,  0, -1,  0,  0,  1 },

			// 11
			{  -wby2,  0,  dby2,  0,  1,  0,  0,  0 },
			{   wby2,  0,  dby2,  0,  1,  0,  1,  0 },
			{   wby2,  0, -dby2,  0,  1,  0,  1,  1 },
			{  -wby2,  0, -dby2,  0,  1,  0,  0,  1 },

		};


	// ---- cube faces lut offset
		int off = ( mode < 0 ? 0 : mode > 11 ? 11 : mode ) * 4;


	// ---- cube triangle vertex lut
		int cube_face_tri[2][3]
		{
			{0, 1, 2},
			{0, 2, 3}
		};


	// ---- triangles...

		for (int ti = 0; ti < 2; ti++)
		{

			// ---- vertices,...
				for (int vi = 0; vi<3; vi++)
				{
			
					int vr = cube_face_tri[ti][vi] + off;

					if ( vertex_format & VS_XYZ )
					{
						*(f + 0) = x + q[vr].x + q[vr].xn * ( explode + q[vr].x ) * wby2;
						*(f + 1) = y + q[vr].y + q[vr].yn * ( explode + q[vr].y ) * hby2;
						*(f + 2) = z + q[vr].z + q[vr].zn * ( explode + q[vr].z ) * dby2;
						f += 3;
					}

					if ( vertex_format & VS_NORMAL )
					{
						*(f + 0) = q[vr].xn;
						*(f + 1) = q[vr].yn;
						*(f + 2) = q[vr].zn;
						f += 3;
					}

					if ( vertex_format & VS_DIFFUSE )
					{
						*(f) = white.f; f++;
					}

					if ( vertex_format & VS_SPECULAR )
					{
						*(f) = 1;
						f++;
					}

					if ( vertex_format & VS_TEX1 )
					{
						*(f + 0) = q[vr].u;
						*(f + 1) = q[vr].v;
						f+=2;
					}
		
				}
		}

	return VS_OK;
}

/*


		UINT ffv_format = context->GetVertexFormat();
		for (DWORD i = 0; i < 5; i++)
		{

			float xr = cube_face_x_rot * (float)VS_RADIANS_PER_DEGREE;
			float sx = q[i].x;
			float sy = q[i].y * cos(xr) - q[i].z * sin(xr);
			float sz = q[i].y * sin(xr) + q[i].z * cos(xr);

			float yr = cube_face_y_rot * (float)VS_RADIANS_PER_DEGREE;
			float tx =     sz * sin(yr) -     sx * cos(yr);
			float ty =     sy;
			float tz =     sz * cos(yr) +     sx * sin(yr);

			t[i].x   = tx;
			t[i].y   = ty;
			t[i].z   = tz;
		}

*/