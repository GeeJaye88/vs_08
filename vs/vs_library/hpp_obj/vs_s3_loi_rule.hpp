////////////////////////////////////////////////////////////////////////////////

// ---------- vs_s3_loi_rule.hpp ----------
/*!
\file vs_s3_loi_rule.hpp
\brief
\author Gareth Edwards
\note

*/


#pragma once


// ---------- Callback_S3_Loi_Ruler_Configure ----------
INT ObjConfig::Callback_S3_Loi_Ruler_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();

	// ---- get supplied and parameter data
		UINT mode      = (UINT)pd[BOX_FACE_MODE];
		UINT type      = (UINT)pd[BOX_FACE_TYPE];


	// ---- number of lines
		//
		// warning: potential memory violation
		//
		//  -- based on max width of 100, max ruler length is 142
		//     so max 600 number of lines allowed.
		//
		UINT num_lines = 700;


	// ---- atlas page
		context->SetAtlasPage(0, 0);


	// ---- overide ?
		UINT format = (UINT)(VS_XYZ | VS_DIFFUSE);
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(FALSE);
			context->SetColourVertex(TRUE);
			context->SetMultiState(FALSE);

			switch ( type )
			{
				default:
				case 0:
					format = (UINT)(VS_XYZ | VS_DIFFUSE);
					context->SetSolid(FALSE);
					context->SetTextured(FALSE);
					break;
			}
			context->SetVisible(TRUE);
		}

	// ---- index buffer
		context->SetIndexCount(num_lines * 2);
		context->SetIndexBufferCallback(Callback_S3_Loi_Ruler_GetIndexBuffer);

	// ---- primitive type
		context->SetPrimitiveType(VS_LINELIST);

	// ---- vertex buffer
		context->SetVertexCount(num_lines * 2);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_S3_Loi_Ruler_GetVertexBuffer);

	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
			);
		context->SetPrimitiveCount(num_lines);


	// ---- end configuration profiling
		context->EndConfig();

	return VS_OK;
}


// ---------- Callback_S3_Loi_Ruler_GetVertexBuffer ----------
INT ObjConfig::Callback_S3_Loi_Ruler_GetVertexBuffer(vs_system::ElmContext *context)
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

	// ---- uncalculated
		FLOAT width = pd[vs_system::ObjConfig::BOX_FACE_WIDTH];
		FLOAT depth = pd[vs_system::ObjConfig::BOX_FACE_DEPTH];

	// ---- location
		FLOAT x_off = pd[vs_system::ObjConfig::BOX_FACE_X];
		FLOAT z_off = pd[vs_system::ObjConfig::BOX_FACE_Z];

	// ---- other parameter data
		FLOAT angle = pd[vs_system::ObjConfig::BOX_FLAG_1];
		FLOAT y_top = pd[vs_system::ObjConfig::BOX_FLAG_2];
		FLOAT y_bot = pd[vs_system::ObjConfig::BOX_FLAG_3];

		FLOAT x3 = pd[vs_system::ObjConfig::BOX_FLAG_4];
		FLOAT y3 = pd[vs_system::ObjConfig::BOX_FLAG_5];
		FLOAT x4 = pd[vs_system::ObjConfig::BOX_FLAG_6];
		FLOAT y4 = pd[vs_system::ObjConfig::BOX_FLAG_7];

		FLOAT marks_per_unit = pd[vs_system::ObjConfig::BOX_FLAG_8];


	// ---- vertex
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_count  = context->GetVertexCount();
		UINT vertex_format = context->GetVertexFormat();
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- num lines
		UINT num_lines = context->GetPrimitiveCount();


	// ---- diffuse
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} grey;
		grey.d = VS_D3DCOLOR_COLORVALUE( red, green, blue, 1.0f );


	// ---- vertices
		struct vtx
		{
			float x, y, z, d;
		};


	// ---- from allocated
		UINT used_vertex_count = 0;


	// ---- proportional to depth
		FLOAT aw = depth * 0.025f;


	// ---- add location lines with arrow heads
		FLOAT x  = width / 2;
		FLOAT y  = y_bot;
		FLOAT z  = depth / 2;
		FLOAT xo = x_off;
		FLOAT zo = z_off;
		FLOAT w  = aw;
		FLOAT w2 = w/2;

		FLOAT zoa[10][2] =
		{
			{ -x+w, zo    }, { -x+w, zo-w2 }, { -x,   zo    }, { -x+w, zo+w2 }, { -x+w, zo    },
			{  x-w, zo    }, {  x-w, zo+w2 }, {  x,   zo    }, {  x-w, zo-w2 }, {  x-w, zo    }
		};

		FLOAT xoa[10][2] =
		{
			{  xo,   -z+w }, {  xo+w2,-z+w }, {  xo,   -z   }, {  xo-w2,-z+w }, {  xo,   -z+w },
			{  xo,    z-w }, {  xo-w2, z-w }, {  xo,    z   }, {  xo+w2, z-w }, {  xo,    z-w }
		};

		for (UINT a = 0; a < 2; a++)
		{
			FLOAT *ad = a == 0 ? &zoa[0][0] : &xoa[0][0];
			for (UINT i = 0; i < 9; i++)
			{
				*(f    ) = *(ad);
				*(f + 1) = y;
				*(f + 2) = *(ad+1);
				*(f + 3) = grey.f;
				f += 4;
				*(f    ) = *(ad+2);
				*(f + 1) = y;
				*(f + 2) = *(ad+3);
				*(f + 3) = grey.f;
				f += 4;
				ad += 2;
				used_vertex_count += 2;
			}
		}


	// ---- height of rule
		FLOAT rule_hgt = y_top - y_bot;


	// ---- offset: front (fn) & back (bn) surface normals
		FLOAT vl = ap_sect == 1 ? 0.5f : 0;
		FLOAT vh = ap_sect == 1 ? 1 : 0.5f;
		VsVector3 fn = { 0, 0, 0 };
		VsVector3 bn = { 0, 0, 0 };
		{
			VsVector3 a = { 0, rule_hgt, 0 };
			VsVector3 b = { x4 - x3, 0, y4 - y3 };
			a.Normalise();
			b.Normalise();
			fn.CrossProduct(&a, &b);
			bn.x = -fn.x; bn.y = fn.y; bn.z = -fn.z;
			FLOAT scalar = aw * 0.01f;
			fn.x *= scalar;
			fn.z *= scalar;
			bn.x *= scalar;
			bn.z *= scalar;
		}

	// ---- y sample offset to "see" surface intersect, proportional to surface size
		FLOAT y_offset = 0.015f * aw;


	// ---- height
		FLOAT rule_hgts[3] =
			{
				rule_hgt,
				rule_hgt * 0.4f,
				rule_hgt * 0.1f
			};


	// ---- in each "direction"
		for (UINT d=0; d<2; d++)
		{

			// ---- ends
				FLOAT xe = d == 0 ? x3 : x4;
				FLOAT ze = d == 0 ? y3 : y4;

			// ---- # of mm width
				FLOAT xd = xe - x_off;
				FLOAT zd = ze - z_off;
				FLOAT dist = (FLOAT)sqrt(xd*xd + zd*zd) * marks_per_unit;
				UINT  idist = (UINT)dist;

			// ---- mm
				for (UINT i = 0; i<= idist; i++)
				{

					// ---- avoid overflow
						if ( used_vertex_count+4 < vertex_count )
						{

							// ---- interpolate
								FLOAT rmu = (FLOAT)i / idist;
								FLOAT x = (1 - rmu)*x_off + rmu * xe;
								FLOAT z = (1 - rmu)*z_off + rmu * ze;

							// ---- mark size
								UINT ih = i % 10 == 0 ? 0 : (i % 5 == 0 ? 1 : 2);

							// ---- front
								*(f  ) = x + fn.x;
								*(f+1) = y_top + y_offset;
								*(f+2) = z + fn.z;
								*(f+3) = grey.f;
								f += 4;
								*(f  ) = x + fn.x;
								*(f+1) = y_top - rule_hgts[ih];
								*(f+2) = z + fn.z;
								*(f+3) = grey.f;
								f += 4;

							// ---- back
								*(f  ) = x + bn.x;
								*(f+1) = y_top + y_offset;
								*(f+2) = z + bn.z;
								*(f+3) = grey.f;
								f += 4;
								*(f  ) = x + bn.x;
								*(f+1) = y_top - rule_hgts[ih];
								*(f+2) = z + bn.z;
								*(f+3) = grey.f;
								f += 4;

							// ---- increment 
								used_vertex_count += 4;
						}

				}

		}


	// ---- zero the unused vertices
		for (UINT i = used_vertex_count; i < vertex_count; i++)
		{
			*(f) = 0;
			*(f + 1) = 0;
			*(f + 2) = 0;
			*(f + 3) = 0;
			f += 4;
		}

	return VS_OK;
}


// ---------- Callback_S3_Loi_Ruler_GetIndexBuffer ----------
INT ObjConfig::Callback_S3_Loi_Ruler_GetIndexBuffer(
		vs_system::ElmContext *context
	)
{

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();

	// ---- num lines
		UINT num_lines = context->GetPrimitiveCount();

	// ---- buffer
		DWORD *ib = context->GetIndexBufferPtr();
		DWORD  ic = 0;

	// ----
		for (UINT line = 0; line < num_lines; line++)
		{
			*(ib+0) = ic++;
			*(ib+1) = ic++;
			ib += 2;
		}

	return VS_OK;
}
