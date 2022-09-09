////////////////////////////////////////////////////////////////////////////////

// ---------- vs_s3_loi_axis.hpp ----------
/*!
\file vs_s3_loi_axis.hpp
\brief
\author Gareth Edwards
\note

*/


#pragma once


// ---------- Callback_LoiAxis_Configure ----------
INT ObjConfig::Callback_LoiAxis_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();

	// ---- get supplied and parameter data
		UINT mode      = (UINT)pd[BOX_FACE_MODE];
		UINT type      = (UINT)pd[BOX_FACE_TYPE];

		UINT axis_flag = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_1];
		UINT anot_flag = TRUE;

	// ---- num line pairs
		UINT num_lines = (axis_flag ? 4 * 3 * 2 : 0) +  // 3 axis * 2
							(anot_flag ? 2 * (3 + 3 + 2) : 0);  // XYZ annotation

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
		context->SetIndexBufferCallback(Callback_LoiAxis_GetIndexBuffer);

	// ---- primitive type
		context->SetPrimitiveType(VS_LINELIST);

	// ---- vertex buffer
		context->SetVertexCount(num_lines * 2);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_LoiAxis_GetVertexBuffer);

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


// ---------- Callback_LoiAxis_GetVertexBuffer ----------
INT ObjConfig::Callback_LoiAxis_GetVertexBuffer(vs_system::ElmContext *context)
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

		FLOAT width    = pd[vs_system::ObjConfig::BOX_FACE_WIDTH];    // not used
		FLOAT height   = pd[vs_system::ObjConfig::BOX_FACE_HEIGHT];   // not used
		FLOAT depth    = pd[vs_system::ObjConfig::BOX_FACE_DEPTH];    // used for size of arrow heads
		FLOAT x_off    = pd[vs_system::ObjConfig::BOX_FACE_X];        // +/- offset
		FLOAT y_off    = pd[vs_system::ObjConfig::BOX_FACE_Y];        //    "
		FLOAT z_off    = pd[vs_system::ObjConfig::BOX_FACE_Z];        //    "


	// ---- other parameter data
		UINT axis_flag = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_1];
		UINT anot_flag = TRUE;

		FLOAT y_top = pd[vs_system::ObjConfig::BOX_FLAG_2];
		FLOAT y_bot = pd[vs_system::ObjConfig::BOX_FLAG_3];


	// ---- vertex
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_count  = context->GetVertexCount();
		UINT vertex_format = context->GetVertexFormat();
		FLOAT *f = context->GetVertexBufferPtr();


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
		FLOAT aw = depth * 0.025f; // arrow width
		FLOAT ax = aw / 2;
		FLOAT zi = aw * 7;
		FLOAT zo = aw * 8;

	// ---- AXIS
		if ( axis_flag )
		{

			// ---- angle
				FLOAT angle  = 0;
				FLOAT radian = angle * (FLOAT)VS_RADIANS_PER_DEGREE;

			// ---- init
				FLOAT y = y_top;

			// ---- from
				vtx arrow[8] =
					{
						{   0, 0,      0, 0 },
						{   0, 0,  zo-aw, 0 },
						{ -ax, 0,  zo-aw, 0 },
						{   0, 0,     zo, 0 },
						{  ax, 0,  zo-aw, 0 },
						{   0, 0,     zo, 0 },
						{ -ax, 0,  zo-aw, 0 },
						{  ax, 0,  zo-aw, 0 },
					};

			// ---- too
				vtx axis[24];

			// ---- X
				for (UINT i = 0; i < 8; i++)
				{
					axis[i+0].x = arrow[i].z;
					axis[i+0].y = 0;
					axis[i+0].z = arrow[i].x;
					axis[i+0].d = f_red;
				}

			// ---- Y
				for (UINT i = 0; i < 8; i++)
				{
					axis[i+8].x = arrow[i].x;
					axis[i+8].y = arrow[i].z;
					axis[i+8].z = 0;
					axis[i+8].d = f_green;
				}
				axis[8].y = -y;

			// ---- Z
				for (UINT i = 0; i < 8; i++)
				{
					axis[i+16].x =  arrow[i].x;
					axis[i+16].z = -arrow[i].z;
					axis[i+16].y = 0;
					axis[i+16].d = f_orange;
				}

			// ---- Y rotate
				// x' = z*sin q + x*cos q
				// z' = z*cos q - x*sin q 
				for (UINT i = 0; i < 2; i++)
				{

						FLOAT jitter = aw * 0.01f;

						vtx axis_rotated[24];
						for (UINT v = 0; v < 24; v++)
						{
							axis_rotated[v].x = axis[v].z * (FLOAT)sin(radian) + axis[v].x * (FLOAT)cos(radian) + x_off;
							axis_rotated[v].y = axis[v].y + y;
							axis_rotated[v].z = axis[v].z * (FLOAT)cos(radian) - axis[v].x * (FLOAT)sin(radian) + z_off;
							axis_rotated[v].d = axis[v].d;

							axis_rotated[v].x += jitter;
							axis_rotated[v].y += jitter;
							axis_rotated[v].z += jitter;
						}

					// ---- append
						size_t size_of_axis_rotated = sizeof(axis_rotated);
						memcpy((VOID *)f, (VOID *)axis_rotated, (INT)size_of_axis_rotated);
						f += (INT)size_of_axis_rotated / sizeof(FLOAT);
				}
		}


	// ---- ANOTATION
		if ( anot_flag )
		{

			// ---- local
				FLOAT c_width  = 2;
				FLOAT c_wby2   = c_width / 2;
				FLOAT c_height = c_width * 1.33f;
				FLOAT c_hby2   = c_height / 2;
				FLOAT xof=0, yof=0, zof=0;

				UINT  num_lines;
				UINT  num_vert;
				CHAR *text = "XYZ";

			// ---- X
				xof = -c_wby2 + zo + 3;
				yof =  c_hby2 + 8.5f;
				zof =  0;
				Text3D_GetChar(
						(text+0),		// [in] char
						f_red,          // [in] colour
						'Z',            // [in] alignment
						c_width,	    // [in] width
						c_height,	    // [in] height
						xof,yof,zof,    // [in] offset
						&num_lines,		// [out] number of lines
						f	         	// [out] line xy pair data
					);
				num_vert = num_lines * 2;
				f += num_vert * 4;

			// ---- Y
				xof = -c_wby2;
				yof = -c_hby2 + zo + 13;
				zof =  0;
				Text3D_GetChar(
						(text+1),		// [in] char
						f_green,             // [in] colour
						'Z',            // [in] alignment
						c_width,	    // [in] width
						c_height,	    // [in] height
						xof,yof,zof,    // [in] offset
						&num_lines,		// [out] number of lines
						f	         	// [out] line xy pair data
					);
				num_vert = num_lines * 2;
				f += num_vert * 2 * 4;

			// ---- Z
				xof =  0;
				yof =  c_hby2 + 8.5f;
				zof = -c_wby2 - zo - 3;
				Text3D_GetChar(
						(text+2),		// [in] char
						f_orange,       // [in] colour
						'X',            // [in] alignment
						c_width,	    // [in] width
						c_height,	    // [in] height
						xof,yof,zof,    // [in] offset
						&num_lines,		// [out] number of lines
						f	         	// [out] line xy pair data
					);
				num_vert = num_lines * 2;
				f += num_vert * 4;

		}
		
		
	return VS_OK;
}


// ---------- Callback_LoiAxis_GetIndexBuffer ----------
INT ObjConfig::Callback_LoiAxis_GetIndexBuffer(
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