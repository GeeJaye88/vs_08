////////////////////////////////////////////////////////////////////////////////

// ---------- vs_obj_wavefront.hpp ----------
/*!
\file vs_obj_wavefront.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_Obj_Configure ----------
INT ObjConfig::Callback_Obj_Configure(vs_system::ElmContext *context)
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


	// ---- vertex buffer ( 4 segments x 4 triangles * 3 vertices == 48 )
		context->SetVertexCount(48);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_Box_GetVertexBuffer);


	// ---- primitive parameters ( 4 segments x 4 triangles )
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
			);
		context->SetPrimitiveCount(16);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ---------- Callback_Obj_GetVertexBuffer ----------
INT ObjConfig::Callback_Obj_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get supplied and parameter data
		UINT  mode    = (UINT)pd[BOX_FACE_MODE];
		UINT  type    = (UINT)pd[BOX_FACE_TYPE];
		UINT  ap_ref  = (UINT)pd[BOX_FACE_AP_REF];
		UINT  ap_sect = (UINT)pd[BOX_FACE_AP_SECT];
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

		FLOAT vl = 1 - (ap_sect == 2 ? 0.0f : 0.5f);
		FLOAT vh = 1 - (ap_sect == 2 ? 0.5f : 1.0f);


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

	// ---- vertices
		struct vtx
		{
			float x, y, z, xn, yn, zn, u, v;
		};


	// ---- 5 col / 4 rows
		vtx v_cr[5][4];


	// ---- template
		vtx t[4] =
		{
			// x        y      z   xn  yn  zn  u   v
			{  0,       0.5f,  0,  0,  0,  0 , 0,  vl },
			{  0.707f,  0.5f,  0,  0,  0,  0,  0, (vh-vl) * 0.333f + vl },
			{  0.707f, -0.5f,  0,  0,  0,  0,  0, (vh-vl) * 0.666f + vl },
			{  0,      -0.5f,  0,  0,  0,  0,  0,  vh },
		};


	// ---- populate
		for (UINT col = 0; col< 5; col++)
		{
			FLOAT a2r = (FLOAT)col * 90 * (FLOAT)VS_RADIANS_PER_DEGREE;
			FLOAT u = (FLOAT)col / 4;
			for (UINT row = 0; row < 4; row++)
			{
				v_cr[col][row].x  =  t[row].x *  cos(a2r);
				v_cr[col][row].y  = t[row].y;
				v_cr[col][row].z  = t[row].x * -sin(a2r);
				v_cr[col][row].xn = v_cr[col][row].x;
				v_cr[col][row].yn = v_cr[col][row].y;
				v_cr[col][row].zn = v_cr[col][row].z;
				v_cr[col][row].u  = u;
				v_cr[col][row].v  = t[row].v;
			}
		}

	// ---- index
		UINT cr[4][3][2] = 
		{
			{ { 0, 0 }, { 1, 1 }, { 0, 1 } },
			{ { 0, 1 }, { 1, 1 }, { 1, 2 } },
			{ { 0, 1 }, { 1, 2 }, { 0, 2 } },
			{ { 0, 2 }, { 1, 2 }, { 0, 3 } }
		};


	// ---- segment
		for (UINT se = 0; se < 4; se++)
		{

			// ---- triangle
				for (UINT tr = 0; tr < 4; tr++)
				{

					// ---- vertex
						for (UINT ve = 0; ve < 3; ve++)
						{

							UINT c = cr[tr][ve][0] + se;
							UINT r = cr[tr][ve][1];

							vtx *h = &v_cr[c][r];

							if ( vertex_format & VS_XYZ )
							{
								*(f + 0) = h->x * scale; // +h->xn * (explode + h->x) * wby2;
								*(f + 1) = h->y * scale; // +h->yn * (explode + h->y) * hby2;
								*(f + 2) = h->z * scale; // +h->zn * (explode + h->z) * dby2;
								f += 3;
							}

							if ( vertex_format & VS_NORMAL )
							{
								*(f + 0) = h->xn;
								*(f + 1) = h->yn;
								*(f + 2) = h->zn;
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
								*(f + 0) = h->u;
								*(f + 1) = h->v;
								f+=2;
							}

						}
				}
		}

	return VS_OK;
}


// ---------- Callback_Obj_GetIndexBuffer ----------
INT ObjConfig::Callback_Obj_GetIndexBuffer(vs_system::ElmContext *context)
{


	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get supplied and parameter data
		UINT  mode    = (UINT)pd[BOX_FACE_MODE];
		UINT  type    = (UINT)pd[BOX_FACE_TYPE];
		UINT  ap_ref  = (UINT)pd[BOX_FACE_AP_REF];
		UINT  ap_sect = (UINT)pd[BOX_FACE_AP_SECT];
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


		FLOAT vl = 1 - (ap_sect == 2 ? 0.0f : 0.5f);
		FLOAT vh = 1 - (ap_sect == 2 ? 0.5f : 1.0f);

	// ---- 12 box quads
		struct vtx
		{
			float x, y, z, xn, yn, zn, u, v;
		};

		vtx q[48]=
		{

			// 0 - 
			{   wby2,  hby2,  0,  0,  0,  1,  0,  vl },
			{  -wby2,  hby2,  0,  0,  0,  1,  1,  vl },
			{  -wby2, -hby2,  0,  0,  0,  1,  1,  vh },
			{   wby2, -hby2,  0,  0,  0,  1,  0,  vh },

			// 1 - 
			{   0,  hby2, -dby2,  1,  0,  0,  0,  vl },
			{   0,  hby2,  dby2,  1,  0,  0,  1,  vl },
			{   0, -hby2,  dby2,  1,  0,  0,  1,  vh },
			{   0, -hby2, -dby2,  1,  0,  0,  0,  vh },

			// 2
			{  -wby2,  hby2,  0,  0,  0, -1,  0,  vl },
			{   wby2,  hby2,  0,  0,  0, -1,  1,  vl },
			{   wby2, -hby2,  0,  0,  0, -1,  1,  vh },
			{  -wby2, -hby2,  0,  0,  0, -1,  0,  vh },

			// 3
			{   0,  hby2,  dby2, -1,  0,  0,  0,  vl },
			{   0,  hby2, -dby2, -1,  0,  0,  1,  vl },
			{   0, -hby2, -dby2, -1,  0,  0,  1,  vh },
			{   0, -hby2,  dby2, -1,  0,  0,  0,  vh },

			// 4
			{  -wby2,  0,  dby2,  0,  1,  0,  0,  vl },
			{   wby2,  0,  dby2,  0,  1,  0,  1,  vl },
			{   wby2,  0, -dby2,  0,  1,  0,  1,  vh },
			{  -wby2,  0, -dby2,  0,  1,  0,  0,  vh },

			// 5
			{   wby2,  0,  dby2,  0, -1,  0,  0,  vl },
			{  -wby2,  0,  dby2,  0, -1,  0,  1,  vl },
			{  -wby2,  0, -dby2,  0, -1,  0,  1,  vh },
			{   wby2,  0, -dby2,  0, -1,  0,  0,  vh },

			// 6
			{  -wby2,  hby2,  0,  0,  0, -1,  0,  vl },
			{   wby2,  hby2,  0,  0,  0, -1,  1,  vl },
			{   wby2, -hby2,  0,  0,  0, -1,  1,  vh },
			{  -wby2, -hby2,  0,  0,  0, -1,  0,  vh },

			// 7
			{   0,  hby2,  dby2, -1,  0,  0,  0,  vl },
			{   0,  hby2, -dby2, -1,  0,  0,  1,  vl },
			{   0, -hby2, -dby2, -1,  0,  0,  1,  vh },
			{   0, -hby2,  dby2, -1,  0,  0,  0,  vh },

			// 8
			{   wby2,  hby2,  0,  0,  0,  1,  0,  vl },
			{  -wby2,  hby2,  0,  0,  0,  1,  1,  vl },
			{  -wby2, -hby2,  0,  0,  0,  1,  1,  vh },
			{   wby2, -hby2,  0,  0,  0,  1,  0,  vh },

			// 9
			{   0,  hby2, -dby2,  1,  0,  0,  0,  vl },
			{   0,  hby2,  dby2,  1,  0,  0,  1,  vl },
			{   0, -hby2,  dby2,  1,  0,  0,  1,  vh },
			{   0, -hby2, -dby2,  1,  0,  0,  0,  vh },

			// 10
			{   wby2,  0,  dby2,  0, -1,  0,  0,  vl },
			{  -wby2,  0,  dby2,  0, -1,  0,  1,  vl },
			{  -wby2,  0, -dby2,  0, -1,  0,  1,  vh },
			{   wby2,  0, -dby2,  0, -1,  0,  0,  vh },

			// 11
			{  -wby2,  0,  dby2,  0,  1,  0,  0,  vl },
			{   wby2,  0,  dby2,  0,  1,  0,  1,  vl },
			{   wby2,  0, -dby2,  0,  1,  0,  1,  vh },
			{  -wby2,  0, -dby2,  0,  1,  0,  0,  vh },

		};



	// ---- cube triangle vertex lut
		int cube_face_tri[2][3]
		{
			{0, 1, 2},
			{0, 2, 3}
		};


	// ---- 6 faces
		for ( UINT face = 0; face < 6; face++)
		{

			// ---- cube faces offset
				int off = (face < 0 ? 0 : face > 11 ? 11 : face) * 4;


			// ---- 2 triangles...
				for (int ti = 0; ti < 2; ti++)
				{

					// ---- 3 vertices,...
						for (int vi = 0; vi<3; vi++)
						{
			
							int vr = cube_face_tri[ti][vi] + off;

							if ( vertex_format & VS_XYZ )
							{
								*(f + 0) = q[vr].x + q[vr].xn * ( explode + q[vr].x ) * wby2;
								*(f + 1) = q[vr].y + q[vr].yn * ( explode + q[vr].y ) * hby2;
								*(f + 2) = q[vr].z + q[vr].zn * ( explode + q[vr].z ) * dby2;
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

		}

	return VS_OK;
}

