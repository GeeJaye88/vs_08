////////////////////////////////////////////////////////////////////////////////

// ---------- vs_s3_loi_anno.hpp ----------
/*!
\file vs_s3_loi_anno.hpp
\brief
\author Gareth Edwards
\note

*/


#pragma once


// ---------- Callback_Loi_Annotation_Configure ----------
INT ObjConfig::Callback_S3_Loi_Annotation_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();

	// ---- get supplied and parameter data
		UINT mode   = (UINT)pd[BOX_FACE_MODE];
		UINT type   = (UINT)pd[BOX_FACE_TYPE];
		UINT ap_ref = (UINT)pd[BOX_FACE_AP_REF];

	// ---- loads of line pairs... 
		//
		// warning: potential memory violation
		//
		UINT num_lines = 200; // was 2 * (3 + 3 + 2);


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
		context->SetIndexBufferCallback(Callback_S3_Loi_Annotation_GetIndexBuffer);

	// ---- primitive type
		context->SetPrimitiveType(VS_LINELIST);

	// ---- vertex buffer
		context->SetVertexCount(num_lines * 2);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_S3_Loi_Annotation_GetVertexBuffer);

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


// ---------- Callback_Loi_Annotation_GetVertexBuffer ----------
INT ObjConfig::Callback_S3_Loi_Annotation_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get supplied and parameter data
		UINT  mode     = (UINT)pd[BOX_FACE_MODE];
		UINT  type     = (UINT)pd[BOX_FACE_TYPE];

		FLOAT red      = pd[BOX_FACE_RED];
		FLOAT green    = pd[BOX_FACE_GREEN];
		FLOAT blue     = pd[BOX_FACE_BLUE];

		FLOAT width    = pd[vs_system::ObjConfig::BOX_FACE_WIDTH];
		FLOAT height   = pd[vs_system::ObjConfig::BOX_FACE_HEIGHT];
		FLOAT depth    = pd[vs_system::ObjConfig::BOX_FACE_DEPTH];

		FLOAT x_off    = pd[vs_system::ObjConfig::BOX_FACE_X];
		FLOAT y_off    = pd[vs_system::ObjConfig::BOX_FACE_Y];
		FLOAT z_off    = pd[vs_system::ObjConfig::BOX_FACE_Z];

		FLOAT angle = pd[vs_system::ObjConfig::BOX_FLAG_1];
		FLOAT y_top = pd[vs_system::ObjConfig::BOX_FLAG_2];
		FLOAT y_bot = pd[vs_system::ObjConfig::BOX_FLAG_3];

		FLOAT x3 = pd[vs_system::ObjConfig::BOX_FLAG_4];
		FLOAT y3 = pd[vs_system::ObjConfig::BOX_FLAG_5];
		FLOAT x4 = pd[vs_system::ObjConfig::BOX_FLAG_6];
		FLOAT y4 = pd[vs_system::ObjConfig::BOX_FLAG_7];


	// ---- visibility
		UINT anot_flag = mode;


	// ---- vertex
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_count  = context->GetVertexCount();
		UINT vertex_format = context->GetVertexFormat();
		FLOAT *f = context->GetVertexBufferPtr();
		UINT used_vertex_count = 0;


	// ---- diffuse
		union colour_union_t {
			DWORD d;
			FLOAT f;
		};
		colour_union_t cu_red;
		cu_red.d    = VS_D3DCOLOR_RGBA( 255,  0,   0, 0 );
		colour_union_t cu_green;
		cu_green.d  = VS_D3DCOLOR_RGBA(  0, 255,   0, 0 );
		colour_union_t cu_orange;
		cu_orange.d = VS_D3DCOLOR_RGBA( 255,128,   0, 0 );
		FLOAT f_red    = cu_red.f;
		FLOAT f_green  = cu_green.f;
		FLOAT f_orange = cu_orange.f;


	// ---- vertices
		struct vtx
		{
			float x, y, z, d;
		};


	// ---- useful stuff
		FLOAT zo = 10;


	// ---- ANNOTATION ?
		if ( anot_flag )
		{

			// ---- local
				FLOAT c_width  = 2;
				FLOAT c_wby2   = c_width / 2;
				FLOAT c_height = c_width * 1.33f;
				FLOAT c_hby2   = c_height / 2;
				FLOAT xof=0, yof=0, zof=0;

				UINT   num_lines;
				UINT   num_vertex;
				FLOAT  width_modifier = 0;
				UCHAR *text = (UCHAR *)"XYZ";

			// ---- X ----
				xof = -c_wby2 + zo + 3;
				yof =  c_hby2 + 9.0f;
				zof =  0;
				Lines3D_GetAsciiChar(
						(text+0),
						'Z',
						f_red,
						c_width,
						c_height,
						xof,yof,zof,
						&num_lines,
						&width_modifier,
						f
					);
				num_vertex = num_lines * 2;
				f += num_vertex * 4;
				used_vertex_count += num_vertex;

			// ---- Y ----
				xof = -c_wby2;
				yof = -c_hby2 + zo + 13;
				zof =  0;
				Lines3D_GetAsciiChar(
						(text+1),
						'Z',
						f_green,
						c_width,
						c_height,
						xof,yof,zof,
						&num_lines,
						&width_modifier,
						f
					);
				num_vertex = num_lines * 2;
				f += num_vertex * 2 * 4;
				used_vertex_count += num_vertex;

			// ---- Z ----
				xof =  0;
				yof =  c_hby2 + 9.0f;
				zof = -c_wby2 - zo - 3;
				Lines3D_GetAsciiChar(
						(text+2),
						'X',
						f_orange,
						c_width,
						c_height,
						xof,yof,zof,
						&num_lines,
						&width_modifier,
						f
					);
				num_vertex = num_lines * 2;
				f += num_vertex * 4;
				used_vertex_count += num_vertex;

		}


	// ---- zero the unused vertices -10 dunno, TBD
		for (UINT i = used_vertex_count; i < vertex_count-10; i++)
		{
			*(f) = 0;
			*(f + 1) = 0;
			*(f + 2) = 0;
			*(f + 3) = 0;
			f += 4;
		}

	return VS_OK;
}


// ---------- Callback_Loi_Annotation_GetIndexBuffer ----------
INT ObjConfig::Callback_S3_Loi_Annotation_GetIndexBuffer(
		vs_system::ElmContext *context
	)
{

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();

	// ---- num line pairs
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