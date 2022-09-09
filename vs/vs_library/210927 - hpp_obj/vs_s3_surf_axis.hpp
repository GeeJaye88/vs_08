////////////////////////////////////////////////////////////////////////////////

// ---------- vs_s3_surf_axis.hpp ----------
/*!
\file vs_s3_surf_axis.hpp
\brief
\author Gareth Edwards
\note

Create a line based graphical 3D XYZ box axis with rulers
located in the XZ plane on the outer sides bounded by the box. 

*/


#pragma once


// ---------- Callback_SurfAxis_Configure ----------
INT ObjConfig::Callback_SurfAxis_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();


	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();

	// ---- get supplied and parameter data
		UINT mode = (UINT)pd[BOX_FACE_MODE];
		UINT type = (UINT)pd[BOX_FACE_TYPE];

	// ---- dimensions
		FLOAT width  = pd[vs_system::ObjConfig::BOX_FACE_WIDTH];
		FLOAT height = pd[vs_system::ObjConfig::BOX_FACE_HEIGHT]; // not used
		FLOAT depth  = pd[vs_system::ObjConfig::BOX_FACE_DEPTH];

	// ---- height === depth
		height = depth;

	// ---- flags
		UINT axis_flag = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_1];
		UINT rule_flag = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_2];
		UINT anot_flag = TRUE;

	// ---- maximum dimension
		FLOAT max_dim = width > height ? (width > depth ? width : depth) : (height > depth ? height : depth);

	// ---- maximum # of blips - note: capped at 20 divisions per unit
		UINT max_blip = ( (UINT)(max_dim) * 20 + 1 ) * 5;

	// ---- maximum num line pairs
		UINT num_lines =
			( axis_flag ? 12       : 0 ) +
			( rule_flag ? max_blip : 0 ) +
			( anot_flag ? (2+3+3)  : 0 );

	// ---- atlas page
		context->SetAtlasPage(0, 0);


	// ---- overide ?
		UINT format = (UINT)(VS_XYZ | VS_NORMAL); // wireframe
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
		context->SetIndexBufferCallback(Callback_SurfAxis_GetIndexBuffer);

	// ---- primitive type
		context->SetPrimitiveType(VS_LINELIST);

	// ---- vertex buffer
		context->SetVertexCount(num_lines * 2);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_SurfAxis_GetVertexBuffer);

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


// ---------- Callback_SurfAxis_GetVertexBuffer ----------
INT ObjConfig::Callback_SurfAxis_GetVertexBuffer(vs_system::ElmContext *context)
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

		FLOAT a_ext    = pd[BOX_FACE_SCALE];                          // axis scaling
		FLOAT width    = pd[vs_system::ObjConfig::BOX_FACE_WIDTH];
		FLOAT height   = pd[vs_system::ObjConfig::BOX_FACE_HEIGHT];   // hgt is used
		FLOAT depth    = pd[vs_system::ObjConfig::BOX_FACE_DEPTH];
		FLOAT x_off    = pd[vs_system::ObjConfig::BOX_FACE_X];        // =/- offset to avoid depth render z buffer conflict
		FLOAT y_off    = pd[vs_system::ObjConfig::BOX_FACE_Y];        //    "
		FLOAT z_off    = pd[vs_system::ObjConfig::BOX_FACE_Z];        //    "


	// ---- height === depth
		height = depth;


	// ---- flags
		UINT axis_flag = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_1];
		UINT rule_flag = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_2];
		UINT anot_flag = TRUE;


	// ---- maximum dimension
		FLOAT max_dim = width > height ? (width > depth ? width : depth) : (height > depth ? height : depth);


	// ---- maximum # of blips - note capped at 20 divisions per unit
		//
		// rule : 5 axis + extra line per axis
		//
		UINT max_blip = ((UINT)(max_dim) * 20 + 1) * 5;


	// ---- hard coded
		// --- note: divisions per unit marker param
		//
		//   upto
		//   size  dpu  major minor
		//    100   1    10    5
		//    50    5    10    5
		//    10    10   10    5
		//    5     20   20    5
		//
		// --- divisions per unit, major # & minor # markers
		UINT dpu = 10, major = 10, minor = 10;
		if ( max_dim <= 5 )
		{
			dpu   = 20;
			major = 20;
			minor = 5;
		}
		else if ( max_dim <= 10 )
		{
			dpu   = 10;
			major = 10;
			minor = 5;
		}
		else if ( max_dim <= 50 )
		{
			dpu   = 5;
			major = 10;
			minor = 5;
		}
		else
		{
			dpu   = 1;
			major = 10;
			minor = 5;
		}


	// ---- vertex
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_count  = context->GetVertexCount();
		UINT vertex_format = context->GetVertexFormat();
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- diffuse
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} diffuse;
		diffuse.d = VS_D3DCOLOR_COLORVALUE(red, green, blue, 0.5f);
		FLOAT fd = diffuse.f;


	// ---- vertices
		struct vtx
		{
			float x =0, y=0, z=0, d=0;
		};


	// ---- offset y axis
		FLOAT hby2 = height / 2;
		FLOAT Y    = y_off - hby2;


	// ---- zap everything
		UINT num_lines = context->GetPrimitiveCount();
		FLOAT *g = f;
		for (UINT line = 0; line < num_lines; line++)
		{
			for (UINT o = 0; o < 8; o++)
			{
				*(g++) = 0;
			}
		}


	// ----  AXIS
		if ( axis_flag )
		{

			// ---- shaft lines
				FLOAT x = -x_off;
				FLOAT y = -y_off;
				FLOAT z = -z_off;
				FLOAT w = x_off + width  * a_ext;
				FLOAT h = Y     + height * a_ext;
				FLOAT d = z_off + depth  * a_ext;
				vtx axis[6] =
				{
					{ x,  y,  z, fd },  // x axis
					{ w,  y,  z, fd },
					{ x,  Y,  z, fd },  // y axis - note: offset
					{ x,  h,  z, fd },
					{ x,  y,  z, fd },  // z axis - note: -ve
					{ x,  y, -d, fd }
				};
				memcpy((VOID *)f, (VOID *)axis, sizeof(axis));
				f += sizeof(axis) / sizeof(FLOAT);

			// --- arrows
				FLOAT aw = d * 0.025f * 2; // arrow width
				FLOAT ax = aw / 2;
				FLOAT W = w + aw;
				FLOAT H = h + aw;
				FLOAT D = d + aw;
				vtx arrows[18] =
				{
					{ W-aw,  y-ax, z,    fd },  // x axis
					{ W,     y,    z,    fd },
					{ W,     y,    z,    fd },
					{ W-aw,  y+ax, z,    fd },
					{ W-aw,  y+ax, z,    fd },
					{ W-aw,  y-ax, z,    fd },

					{ x-ax,  H-aw, z,    fd },  // y axis
					{ x,     H,    z,    fd },
					{ x,     H,    z,    fd }, 
					{ x+ax,  H-aw, z,    fd },
					{ x+ax,  H-aw, z,    fd },
					{ x-ax,  H-aw, z,    fd },

					{ x,     y-ax,-D+aw, fd },  // z axis
					{ x,     y,   -D,    fd },
					{ x,     y,   -D,    fd },
					{ x,     y+ax,-D+aw, fd },
					{ x,     y+ax,-D+aw, fd },
					{ x,     y-ax,-D+aw, fd }
				};
				memcpy((VOID *)f, (VOID *)arrows, sizeof(arrows));
				f += sizeof(arrows) / sizeof(FLOAT);

		}


	// ---- RULERS
		if ( rule_flag )
		{

			// ---- number of ruler blips (single line) 
				UINT num_blip_per_axis[5] =
				{
					(UINT)width  * dpu + 1,
					(UINT)height * dpu + 1,
					(UINT)depth  * dpu + 1,
					(UINT)width  * dpu + 1,
					(UINT)depth  * dpu + 1
				};

			// ---- axis dimensions
				FLOAT aw = depth * 0.1f;
				FLOAT w  = width;
				FLOAT h  = height;
				FLOAT d  = depth;
				FLOAT W  = width  + aw;
				FLOAT H  = height + aw;
				FLOAT D  = depth  + aw;

			// ---- interpolation (from too) vertex table
				vtx mm[5][2][4] =
				{
					// ---- inner axis
						{
							{ {  0,  0,  0, fd }, {  w,  0,  0, fd }, },  // x axis
							{ {  0,  0, aw, fd }, {  w,  0, aw, fd }, },
						},
										{
							{ {  0, -Y,  0, fd }, {  0,  Y,  0, fd }, },  // y axis
							{ { aw, -Y,  0, fd }, { aw,  Y,  0, fd }, },
						},
						{
							{ {  0,  0,  0, fd }, {  0,  0, -d, fd }, },  // z axis
							{ {-aw,  0,  0, fd }, {-aw,  0, -d, fd }, },
						},
					// ---- outer axis
						{
							{ {  0,  0, -d, fd }, {  w,  0, -d, fd }, },  // x axis
							{ {  0,  0, -D, fd }, {  w,  0, -D, fd }, },
						},
						{
							{ {  w,  0,  0, fd }, {  w,  0, -d, fd }, },  // z axis
							{ {  W,  0,  0, fd }, {  W,  0, -d, fd }, },
						},
				};

			// ---- axis...
				vtx from, too;
				UINT blip_count = 0;
				for (UINT axis = 0; axis < 5; axis++)
				{

					// ---- blips
						UINT num_blip = num_blip_per_axis[axis];
						for (UINT blip = 0; blip < num_blip; blip++)
						{
							FLOAT rmu = (FLOAT)blip / (FLOAT)(num_blip - 1);

							from.x = (1 - rmu)*mm[axis][0][0].x + rmu * mm[axis][0][1].x;
							from.y = (1 - rmu)*mm[axis][0][0].y + rmu * mm[axis][0][1].y;
							from.z = (1 - rmu)*mm[axis][0][0].z + rmu * mm[axis][0][1].z;

							too.x  = (1 - rmu)*mm[axis][1][0].x + rmu * mm[axis][1][1].x;
							too.y  = (1 - rmu)*mm[axis][1][0].y + rmu * mm[axis][1][1].y;
							too.z  = (1 - rmu)*mm[axis][1][0].z + rmu * mm[axis][1][1].z;

							FLOAT smu = blip % major == 0 ? 1.0f : ( blip % minor == 0 ? 0.66f : 0.33f );
							too.x = (1 - smu)*from.x + smu * too.x;
							too.y = (1 - smu)*from.y + smu * too.y;
							too.z = (1 - smu)*from.z + smu * too.z;

							*(f + 0) = from.x; *(f + 1) = from.y; *(f + 2) = from.z; *(f + 3) = fd; f += 4;
							*(f + 0) = too.x;  *(f + 1) = too.y;  *(f + 2) = too.z;  *(f + 3) = fd; f += 4;

							blip_count++;
						}
				}
		}

	// ---- ANOTATION
		if ( anot_flag )
		{

			// ---- local
				FLOAT c_width  = 5;
				FLOAT c_wby2   = c_width / 2;
				FLOAT c_height = c_width * 1.33f;
				FLOAT c_hby2   = c_height / 2;
				FLOAT xo=0, yo=0, zo=0;

				UINT  num_lines;
				CHAR *text = "XYZ";

			// ---- X
				xo = -c_wby2 + width + 25;
				yo = -c_hby2;
				zo =  0;
				Text3D_GetChar(
						(text+0),		// [in] char
						fd,             // [in] colour
						'Z',            // [in] alignment
						c_width,	    // [in] width
						c_height,	    // [in] height
						xo,yo,zo,       // [in] offset
						&num_lines,		// [out] number of lines
						f	         	// [out] line xy pair data
					);
				f += num_lines * 2 * 4;

			// ---- Y
				xo = -c_wby2;
				yo = -c_hby2 + height/2 + 25;
				zo =  0;
				Text3D_GetChar(
						(text+1),		// [in] char
						fd,             // [in] colour
						'Z',            // [in] alignment
						c_width,	    // [in] width
						c_height,	    // [in] height
						xo,yo,zo,       // [in] offset
						&num_lines,		// [out] number of lines
						f	         	// [out] line xy pair data
					);
				f += num_lines * 2 * 4;

			// ---- Z
				xo =  0;
				yo = -c_hby2;
				zo = -c_wby2 - depth - 25;
				Text3D_GetChar(
						(text+2),		// [in] char
						fd,             // [in] colour
						'X',            // [in] alignment
						c_width,	    // [in] width
						c_height,	    // [in] height
						xo,yo,zo,       // [in] offset
						&num_lines,		// [out] number of lines
						f	         	// [out] line xy pair data
					);
				f += num_lines * 2 * 4;

		}

	return VS_OK;
}



// ---------- Callback_SurfAxis_GetIndexBuffer ----------
INT ObjConfig::Callback_SurfAxis_GetIndexBuffer(
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


INT ObjConfig::Text3D_GetInfo(
		CHAR  *te,		// [in] text
		FLOAT  bh,		// [in] base height
		UINT  *nl,		// [out] number of lines
		FLOAT *wi,		// [out] width
		FLOAT *he		// [out] height
	)
{


	return VS_OK;
}


INT ObjConfig::Text3D_GetLines(
		CHAR  *te,		// [in] text
		FLOAT  bh,		// [in] base height
		UINT  *nl,		// [out] number of lines
		FLOAT *lp		// [out] line xy pair data
	)
{
	return VS_OK;
}


INT ObjConfig::Text3D_GetChar(
		CHAR  *ch,		// [in] char
		FLOAT  colour,  // [in] colour
		CHAR   axis,    // [in] alignment
		FLOAT  width,	// [in] char width
		FLOAT  height,	// [in] char height
		FLOAT  x,		// [in] x offset
		FLOAT  y,		// [in] y offset
		FLOAT  z,		// [in] z offset
		UINT  *nl,		// [out] number of lines
		FLOAT *lp		// [out] line xy pair data
	)
{
	
	// ---- local
		FLOAT line_list[48];
		UINT  line_num = 0;


	// ---- line pair ords 
		switch (*ch)
		{
			case 'X':
				{
				FLOAT ll[8] = { 0,0,3,4,  3,0,0,4 };
				line_num = sizeof(ll) / sizeof(FLOAT) / 4;
				memcpy((VOID *)line_list, (VOID *)ll, sizeof(ll));
				}
				break;
			case 'Y':
				{
				FLOAT ll[12] = { 0,4,1.5f,2,  3,4,1.5f,2,  1.5f,2,1.5f,0 };
				line_num = sizeof(ll) / sizeof(FLOAT) / 4;
				memcpy((VOID *)line_list, (VOID *)ll, sizeof(ll));
				}
				break;
			case 'Z':
				{
				FLOAT ll[12] = { 0,4,3,4,  3,4,0,0,  0,0,3,0 };
				line_num = sizeof(ll) / sizeof(FLOAT) / 4;
				memcpy((VOID *)line_list, (VOID *)ll, sizeof(ll));
				}
				break;
			default:
				return VS_ERROR;
		}


	// ---- line pair data
		*nl =  line_num;
		FLOAT *lp_source = NULL;
		FLOAT *lp_dest = lp;
		FLOAT  x_s, y_s;
		for (UINT l = 0; l < line_num; l++)
		{
			lp_source = (line_list + l * 4);
			for (UINT o = 0; o < 2; o++)
			{
				x_s = *(lp_source + o*2 + 0) / 3 * width;
				y_s = *(lp_source + o*2 + 1) / 3 * height;
				switch (axis)
				{
					case 'X':
						{
							*(lp_dest + 0) = x + 0;
							*(lp_dest + 1) = y + y_s;
							*(lp_dest + 2) = z + x_s;
						}
						break;
					default:
					case 'Y':
						{
							*(lp_dest + 0) = x + x_s;
							*(lp_dest + 1) = y + 0;
							*(lp_dest + 2) = z + y_s;
						}
						break;
					case 'Z':
						{
							*(lp_dest + 0) = x + x_s;
							*(lp_dest + 1) = y + y_s;
							*(lp_dest + 2) = z + 0;
						}
						break;
				}
				*(lp_dest + 3) = (FLOAT)colour;
				lp_dest += 4;
			}
		}

	return VS_OK;
}
