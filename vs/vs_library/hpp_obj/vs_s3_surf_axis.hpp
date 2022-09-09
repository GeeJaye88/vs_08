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

// ---------- SurfAxis_GetDpuMajorMinor ----------
/*!

\returns max of the dimensions

\note Redundant logic

	// ---- maximum # of marks - note capped at 20 divisions per unit
		//
		// rule : 5 axis + extra line per axis
		//
		UINT max_marks = ((UINT)(max_dim) * 20 + 1) * 5;

*/
INT ObjConfig::SurfAxis_GetDpuMajorMinor(
		INT  width,
		INT  height,
		INT  depth,
		INT *dpu,
		INT *major,
		INT *minor
	)
{

	// ---- maximum dimension
		INT max_dim = width > height ?
				( width > depth ? width : depth ) :
				( height > depth ? height : depth );

	// ---- hard coded
		//
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
		*dpu   = 10;
		*major = 10;
		*minor = 10;
		if ( max_dim <= 5 )
		{
			*dpu   = 20;
			*major = 20;
			*minor = 5;
		}
		else if ( max_dim <= 10 )
		{
			*dpu   = 10;
			*major = 10;
			*minor = 5;
		}
		else if ( max_dim <= 25 )
		{
			*dpu   = 5;
			*major = 10;
			*minor = 5;
		}
		else
		{
			*dpu   = 1;
			*major = 10;
			*minor = 5;
		}

	// ---- maximum of the dimensions
		return max_dim;
}


// ---------- Callback_SurfAxis_Configure ----------
INT ObjConfig::Callback_SurfAxis_Configure(vs_system::ElmContext *context)
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

	// ---- dimensions
		FLOAT width  = pd[vs_system::ObjConfig::BOX_FACE_WIDTH];
		FLOAT height = pd[vs_system::ObjConfig::BOX_FACE_HEIGHT]; // not used
		FLOAT depth  = pd[vs_system::ObjConfig::BOX_FACE_DEPTH];

	// ---- height same as depth!
		height = depth;

	// ---- flags
		UINT axis_flag = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_1];
		UINT rule_flag = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_2];
		UINT anot_flag = TRUE;


	// ---- # of lines required for axis
		INT dpu = 0, major = 0, minor = 0;
		INT max_dim = SurfAxis_GetDpuMajorMinor(
				(INT)(width+0.5f),
				(INT)(height+0.5f),
				(INT)(depth+0.5f),
				&dpu, &major, &minor
			);
		INT line_div = ( max_dim * dpu + 1 ) * 5;
		INT line_char = ( major + 1 ) * 12 * 2 * 5;

	// ---- maximum num line pairs
		//
		// warning: low potential memory violation as count is hardcoded
		//
		UINT num_lines =
			( axis_flag ? 12        : 0 ) + // 3 axis (3) + 3 arrows (9)
			( rule_flag ? line_div  : 0 ) + // ( max_dim * dpu ) + 1 * 5
			( anot_flag ? (2+3+3)   : 0 ) + // X (2), Y (3), Z (3) 
			( TRUE      ? line_char : 0);   // ( major + 1 ) * 12 * 5;


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


		FLOAT wby2 = width / 2;
		FLOAT dby2 = width / 2;

	// ---- height === depth
		height = depth;


	// ---- flags
		UINT axis_flag = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_1];
		UINT rule_flag = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_2];
		UINT anot_flag = TRUE;
		UINT text_flag = FALSE;


	// ---- # of lines required for axis
		INT dpu = 0, major = 0, minor = 0;
		INT max_dim = SurfAxis_GetDpuMajorMinor(
				(INT)(width+0.5f),
				(INT)(height+0.5f),
				(INT)(depth+0.5f),
				&dpu, &major, &minor
			);
		INT line_div = major * dpu;
	

	// ---- vertex
		UINT vertex_size   = context->GetVertexSize();
		UINT vertex_count  = context->GetVertexCount();
		UINT vertex_format = context->GetVertexFormat();
		UINT vertex_used   = 0;
		FLOAT *f = context->GetVertexBufferPtr();


	// ---- diffuse
		union colour_union_t {
			DWORD d;
			FLOAT f;
		} diffuse;
		diffuse.d = VS_D3DCOLOR_COLORVALUE(red, green, blue, 0.5f);
		FLOAT f_grey = diffuse.f;


	// ---- vertices
		struct vtx
		{
			float x =0, y=0, z=0, d=0;
		};


	// ---- useful stuff
		FLOAT asl = 1.25f;          // axis shaft low
		FLOAT ash = 1.4f;           // axis shaft high
   		FLOAT aw = depth * 0.025f;  // arrow width


	// ---- char stuff

			// char width
			FLOAT c_width = aw * 1.5f;

			// metrics derived from char width
			FLOAT c_wby2   = c_width  / 2;
			FLOAT c_height = c_width  * 1.33f;
			FLOAT c_hby2   = c_height / 2;
			FLOAT c_space  = c_height / 2;

			// metrics derived from metrics
			FLOAT m_scalar = 4;
			FLOAT m_width = c_width   / m_scalar;
			FLOAT m_wby2   = c_wby2   / m_scalar;
			FLOAT m_height = c_height / m_scalar;
			FLOAT m_hby2   = c_hby2   / m_scalar;
			FLOAT m_space  = c_space  / m_scalar;

			// offset
			FLOAT xof = 0, yof = 0, zof = 0;

			// store
			UINT   num_char_lines;
			UINT   num_char_vert;
			UCHAR  txt[128];


	// ---- offset y axis
		FLOAT hby2 = height / 2;
		FLOAT Y    = y_off - hby2;


	// ----  AXIS
		if ( axis_flag )
		{

			// ---- mem stuff
				INT som = 0; // sizeof mem used for memcpy
				INT nof = 0; // number of floats
				INT nov = 0; // number of vertices

			// ---- shaft lines
				FLOAT x =  wby2 * asl;
				FLOAT y =  0;
				FLOAT z = -dby2 * asl;
				FLOAT w =  wby2 * ash;
				FLOAT h =  hby2 * ash;
				FLOAT d = -dby2 * ash;
				vtx axis[6] =
				{
					{ x,  0,  0,  f_grey },  // x axis
					{ w,  0,  0,  f_grey },
					{ 0,  0,  0,  f_grey },  // y axis - note: offset
					{ 0,  h,  0,  f_grey },
					{ 0,  0,  z,  f_grey },  // z axis - note: -ve
					{ 0,  0,  d,  f_grey }
				};
				som = sizeof(axis);         memcpy((VOID *)f, (VOID *)axis, som);
				nof = som / sizeof(FLOAT);  f += nof;
				nov = som / sizeof(vtx);    vertex_used += nov;


			// --- arrows
				FLOAT aw2 = aw * 3;
				FLOAT abx = aw;
				FLOAT W = w + aw2;
				FLOAT H = h + aw2;
				FLOAT D = d - aw2;
				vtx arrows[18] =
				{
					{ W-aw2, y-abx,  0,     f_grey },  // x axis
					{ W,     y,      0,     f_grey },
					{ W,     y,      0,     f_grey },
					{ W-aw2, y+abx,  0,     f_grey },
					{ W-aw2, y+abx,  0,     f_grey },
					{ W-aw2, y-abx,  0,     f_grey },

					{ -abx,  H-aw2,  0,     f_grey },  // y axis
					{  0,    H,      0,     f_grey },
					{  0,    H,      0,     f_grey },
					{  abx,  H-aw2,  0,     f_grey },
					{  abx,  H-aw2,  0,     f_grey },
					{ -abx,  H-aw2,  0,     f_grey },

					{ 0,     y-abx,  D+aw2, f_grey },  // z axis
					{ 0,     y,      D,     f_grey },
					{ 0,     y,      D,     f_grey },
					{ 0,     y+abx,  D+aw2, f_grey },
					{ 0,     y+abx,  D+aw2, f_grey },
					{ 0,     y-abx,  D+aw2, f_grey }
				};
				som = sizeof(arrows);       memcpy((VOID *)f, (VOID *)arrows, som);
				nof = som / sizeof(FLOAT);  f += nof;
				nov = som / sizeof(vtx);    vertex_used += nov;
		}


	// ---- RULERS
		if ( rule_flag )
		{

			// ---- axis dimensions -- TBD - all the same as max_dim??
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
							{ {  0,  0,  0, f_grey }, {  w,  0,  0, f_grey }, },  // XI - x axis
							{ {  0,  0, aw, f_grey }, {  w,  0, aw, f_grey }, },
						},
										{
							{ {  0,  Y,  0, f_grey }, {  0, -Y,  0, f_grey }, },  // Y  - y axis
							{ { aw,  Y,  0, f_grey }, { aw, -Y,  0, f_grey }, },
						},
						{
							{ {  0,  0, -d, f_grey }, {  0,  0,  0, f_grey }, },  // ZI - z axis
							{ {-aw,  0, -d, f_grey }, {-aw,  0,  0, f_grey }, },
						},
					// ---- outer axis
						{
							{ {  0,  0, -d, f_grey }, {  w,  0, -d, f_grey }, },  // XO - x axis
							{ {  0,  0, -D, f_grey }, {  w,  0, -D, f_grey }, },
						},
						{
							{ {  w,  0, -d, f_grey }, {  w,  0,  0, f_grey }, },  // ZO - z axis
							{ {  W,  0, -d, f_grey }, {  W,  0,  0, f_grey }, }, 
						},
				};

			// ---- mark anotation (mark width) offset
				//                XI    Y     ZI    XO    ZO
				FLOAT xao[5] = { 0.5f, 2.0f,-1.2f, 0.5f, 3.0f };
				FLOAT yao[5] = { 0.0f,-0.8f, 0.0f, 0.0f, 0.0f };
				FLOAT zao[5] = { 1.0f, 0.0f,-0.7f,-2.0f,-0.7f };

			// ---- axis...
				INT num_marks = max_dim * dpu + 1;

				vtx from, too;
				for (UINT axis = 0; axis < 5; axis++)
				{

					// ---- marks
						INT mark_counter = 0;
						for (INT mark = 0; mark < num_marks; mark++)
						{

							// ---- start start from and end too
								FLOAT rmu = (FLOAT)mark / (FLOAT)(num_marks-1);

								from.x = (1 - rmu)*mm[axis][0][0].x + rmu * mm[axis][0][1].x - wby2;
								from.y = (1 - rmu)*mm[axis][0][0].y + rmu * mm[axis][0][1].y;
								from.z = (1 - rmu)*mm[axis][0][0].z + rmu * mm[axis][0][1].z + dby2;

								too.x  = (1 - rmu)*mm[axis][1][0].x + rmu * mm[axis][1][1].x - wby2;
								too.y  = (1 - rmu)*mm[axis][1][0].y + rmu * mm[axis][1][1].y;
								too.z  = (1 - rmu)*mm[axis][1][0].z + rmu * mm[axis][1][1].z + dby2;

							// ---- note: pre-text, was 1.0, 0.66 & 0.33 respectively
								FLOAT smu = mark % major == 0 ? 0.65f : ( mark % minor == 0 ? 0.5f : 0.25f );

							// --- mod too
								too.x = (1 - smu)*from.x + smu * too.x;
								too.y = (1 - smu)*from.y + smu * too.y;
								too.z = (1 - smu)*from.z + smu * too.z;

							// ---- store
								*(f + 0) = from.x; *(f + 1) = from.y; *(f + 2) = from.z; *(f + 3) = f_grey; f += 4;
								*(f + 0) = too.x;  *(f + 1) = too.y;  *(f + 2) = too.z;  *(f + 3) = f_grey; f += 4;
								vertex_used += 2;

							// ---- major or minor mark
								if ( mark % dpu == 0 )
								{

									// ---- annotate ONLY major marks
										if ( mark % major == 0)
										{

											// ---- mod y axis value
												INT value = mark_counter - max_dim / 2;

											// ---- don't annotate if axis inner y or outer x & z && zero
												BOOL annotate = axis == 1 && value == 0 ? FALSE : TRUE;

											// ---- annotate
												if ( annotate )
												{
													smu = 1.1f;
													too.x = (1 - smu)*from.x + smu * too.x;
													too.y = (1 - smu)*from.y + smu * too.y;
													too.z = (1 - smu)*from.z + smu * too.z;

													CHAR t[10];
													sprintf(t, "%d", value);
													INT len = strlen(t);

													sprintf((CHAR *)txt, "%d", value);

													FLOAT xs = (FLOAT)len;

													FLOAT xoop = too.x - (m_width * xs) + (m_width  * xao[axis]);

													FLOAT yoop = too.y                  + (m_width  * yao[axis]);
													FLOAT zoop = too.z                  + (m_height * zao[axis]);

													Lines3D_GetAsciiText(
															txt,
															axis == 1 ? 'Z' : 'Y',
															f_grey,
															m_width,
															m_height,
															m_space,
															xoop, yoop, zoop,
															&num_char_lines,
															f
														);

													num_char_vert = num_char_lines * 2;
													f += num_char_vert * 4;
													vertex_used += num_char_vert;

												}
										}

									// ---- increment for both major & minor marks
										mark_counter++;
								}
						}
				}
		}


	// ---- ANNOTATION
		if ( anot_flag )
		{


			// ---- X
				xof = -c_wby2 + wby2 * ash + aw * 4.5f;
				yof = -c_hby2;
				zof =  0;
				sprintf((CHAR *)txt, "X");
				Lines3D_GetAsciiText(
						txt,
						'Z',
						f_grey,
						c_width,
						c_height,
						c_space,
						xof,yof,zof,
						&num_char_lines,
						f
					);
				num_char_vert = num_char_lines * 2;
				f += num_char_vert * 4;
				vertex_used += num_char_vert;

			// ---- Y
				xof = -c_wby2;
				yof = -c_hby2 + hby2 * ash + aw * 5.0f;
				zof =  0;
				sprintf((CHAR *)txt, "Y");
				Lines3D_GetAsciiText(
						txt,
						'Z',
						f_grey,
						c_width,
						c_height,
						c_space,
						xof,yof,zof,
						&num_char_lines,
						f
					);
				num_char_vert = num_char_lines * 2;
				f += num_char_vert * 4;
				vertex_used += num_char_vert;

			// ---- Z
				xof =  0;
				yof = -c_hby2;
				zof = -c_wby2 - dby2 * ash - aw * 4.5f;
				sprintf((CHAR *)txt, "Z");
				Lines3D_GetAsciiText(
						txt,
						'X',
						f_grey,
						c_width,
						c_height,
						c_space,
						xof,yof,zof,
						&num_char_lines,
						f
					);
				num_char_vert = num_char_lines * 2;
				f += num_char_vert * 4;
				vertex_used += num_char_vert;

		}


	// ---- zero the unused vertices
		for (UINT i = vertex_used; i < vertex_count; i++)
		{
			*(f) = 0;
			*(f + 1) = 0;
			*(f + 2) = 0;
			*(f + 3) = 0;
			f += 4;
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


////////////////////////////////////////////////////////////////////////////////


// ---------- Lines3D_GetAsciiText ----------
/*!
\brief get ASCII character text array line xy pairs
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/

INT ObjConfig::Lines3D_GetAsciiText(
		UCHAR *text,		// [in] text
		CHAR   align,		// [in] axis alignment
		FLOAT  colour,		// [in] colour
		FLOAT  width,		// [in] char width
		FLOAT  height,		// [in] char height
		FLOAT  space,		// [in] char spacing
		FLOAT  x,			// [in] x offset
		FLOAT  y,			// [in] y offset
		FLOAT  z,			// [in] z offset
		UINT  *num_lines,	// [out] number of lines
		FLOAT *line_xy_pairs// [out] line xy pairs
	)
{

	// ---- local
		UINT  text_len = (UINT)strlen((char*)text);
		UINT  num_new_lines = 0;
		UINT  total_new_lines = 0;
		FLOAT text_width = 0;
		FLOAT width_mod = 1;
		FLOAT xo = 0;

	// ---- for each char
		for (UINT i = 0; i < text_len; i++)
		{
			// ---- locate
				xo = x + text_width;
		
			// ---- get char line xy pairs
				Lines3D_GetAsciiChar(
						(text + i),		// [in] ASCII char from 0 to 255
						align,			// [in] axis alignment
						colour,			// [in] colour
						width,		    // [in] width
						height,			// [in] height
						xo, y, z,		// [in] offset
						&num_new_lines,	// [out] number of lines
						&width_mod,		// [out] width modifier
						line_xy_pairs	// [out] line xy pairs
					);

			// ---- update line count
				line_xy_pairs   += num_new_lines * 2 * 4;
				total_new_lines += num_new_lines;

			// ---- update text width
				text_width += (width * width_mod) + space;
		}

	// ---- update num_lines
		*num_lines = total_new_lines;

	return VS_OK;
}


// ---------- Lines3D_GetAsciiTextWidth ----------
/*!
\brief get ASCII character text array line xy pairs
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
FLOAT ObjConfig::Lines3D_GetAsciiTextWidth(
		UCHAR *text,	// [in] char text array
		FLOAT  width,	// [in] char width
		FLOAT  space	// [in[ char spacing
	)
{

	// ---- local
		UINT text_len = (UINT)strlen((char *)text);
		UINT num_new_lines = 0;
		UINT total_new_lines = 0;

		FLOAT text_width = 0;
		FLOAT width_mod = 1;

	// ---- for each char
		for (UINT i = 0; i < text_len; i++)
		{
			width_mod = Lines3D_GetAsciiCharWidth(*(text+i));
			text_width += (width * width_mod) + space;
		}

	return text_width - space;
}


// ---------- Lines3D_GetAsciiChar ----------
/*!
\brief get ASCII character line xy pairs
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT ObjConfig::Lines3D_GetAsciiChar(
		UCHAR *ch,			// [in] ASCII char from 0 to 255
		CHAR   align,		// [in] alignment
		FLOAT  colour,		// [in] colour
		FLOAT  width,		// [in] char width
		FLOAT  height,		// [in] char height
		FLOAT  x,			// [in] x offset
		FLOAT  y,			// [in] y offset
		FLOAT  z,			// [in] z offset
		UINT  *num_lines,	// [out] number of lines
		FLOAT *width_mod,	// [out] width modifier
		FLOAT *line_xy_pairs// [out] line xy pairs

	)
{

	// ---- local
		*width_mod = 1;
		FLOAT line_list[48];
		UINT  line_num = 0;


	// ---- hex column
		INT ich = (INT)*ch;
		ich = ich >= 0 ? ich : -ich;
		UINT hex_column = (UINT)ich / 16;


	// ---- init character line xy pairs & width modifier
		switch ( hex_column )
		{

			case 2: // *ch >= ' ' && *ch <= '/'
				{
					switch (*ch)
					{
						case ' ':
							{
								*width_mod = Lines3D_GetAsciiCharWidth(*ch);
								*num_lines = 0;
								return VS_OK;
							}
							break;
						case '-':
							{
								FLOAT xc = *width_mod * 3 / 2;
								FLOAT ys = 0.05f * 1.333f;
								FLOAT x1 = 0, x2 = 3;
								FLOAT y1 = 2-ys, y2 = 2+ys;
								FLOAT ll[16] = { x1,y1,x1,y2,  x1,y2,x2,y2,
												 x2,y2,x2,y1,  x2,y1,x1,y1 };
								line_num = sizeof(ll) / sizeof(FLOAT) / 4;
								memcpy((VOID *)line_list, (VOID *)ll, sizeof(ll));
							}
							break;
						case '.':
							{
								// note width mod is applied to the grid # of col width
								*width_mod = Lines3D_GetAsciiCharWidth(*ch);
								FLOAT xc = *width_mod * 3 / 2;
								FLOAT xt = 0.2f;
								FLOAT ys = xt * 1.333f;
								FLOAT x1 = xc-xt, x2 = xc+xt;
								FLOAT y1 = 0, y2 = ys;
								FLOAT ll[16] = { x1,y1,x1,y2,  x1,y2,x2,y2,
												 x2,y2,x2,y1,  x2,y1,x1,y1 };
								line_num = sizeof(ll) / sizeof(FLOAT) / 4;
								memcpy((VOID *)line_list, (VOID *)ll, sizeof(ll));
							}
							break;
						default:
							return VS_ERROR;
					}
				}
				break;

			case 3: // *ch >= '0' && *ch <= '?'
				{
					switch (*ch)
					{
						case '0':
							{
								FLOAT ll[32] = { 1,0,0,1,  0,1,0,3,  0,3,1,4,  1,4,2,4,
												 2,4,3,3,  3,3,3,1,  3,1,2,0,  2,0,1,0 };
								line_num = sizeof(ll) / sizeof(FLOAT) / 4;
								memcpy((VOID *)line_list, (VOID *)ll, sizeof(ll));
							}
							break;
						case '1':
							{
								FLOAT x1 = 0.5, x2 = 1.5, x3 = 2.5;
								FLOAT ll[12] = { x1,3,x2,4,  x2,4,x2,0,  x1,0,x3,0 };
								line_num = sizeof(ll) / sizeof(FLOAT) / 4;
								memcpy((VOID *)line_list, (VOID *)ll, sizeof(ll));
							}
							break;
						case '2':
							{
								FLOAT ll[24] = { 0,3,1,4,  1,4,2,4,  2,4,3,3,  3,3,3,2,
												3,2,0,0,  0,0,3,0 };
								line_num = sizeof(ll) / sizeof(FLOAT) / 4;
								memcpy((VOID *)line_list, (VOID *)ll, sizeof(ll));
							}
							break;
						case '3':
							{
								FLOAT ll[36] = { 0,3,1,4,  1,4,2,4,  2,4,3,3,  3,3,2,2,
												 2,2,1,2,  2,2,3,1,  3,1,2,0,  2,0,1,0,
												 1,0,0,1 };
								line_num = sizeof(ll) / sizeof(FLOAT) / 4;
								memcpy((VOID *)line_list, (VOID *)ll, sizeof(ll));
							}
							break;
						case '4':
							{
								FLOAT ll[12] = { 2,0,2,4,  2,4,0,1,  0,1,3,1 };
								line_num = sizeof(ll) / sizeof(FLOAT) / 4;
								memcpy((VOID *)line_list, (VOID *)ll, sizeof(ll));
							}
							break;
						case '5':
							{
								FLOAT ll[24] = { 3,4,0,4,  0,4,0,2,  0,2,2,2,  2,2,3,1,
												 3,1,2,0,  2,0,0,0 };
								line_num = sizeof(ll) / sizeof(FLOAT) / 4;
								memcpy((VOID *)line_list, (VOID *)ll, sizeof(ll));
							}
							break;
						case '6':
							{
								FLOAT ll[32] = { 2,4,1,4,  1,4,0,3,  0,3,0,1,  0,1,1,0,
												 1,0,2,0,  2,0,3,1,  3,1,2,2,  2,2,0,2 };
								line_num = sizeof(ll) / sizeof(FLOAT) / 4;
								memcpy((VOID *)line_list, (VOID *)ll, sizeof(ll));
							}
							break;
							case '7':
							{
								FLOAT ll[12] = { 0,4,3,4,  3,4,3,3,  3,3,0,0 };
								line_num = sizeof(ll) / sizeof(FLOAT) / 4;
								memcpy((VOID *)line_list, (VOID *)ll, sizeof(ll));
							}
							break;
						case '8':
							{
								FLOAT ll[44] = { 1,2,0,3,  0,3,1,4,  1,4,2,4,  2,4,3,3,
												 3,3,2,2,  2,2,1,2,  2,2,3,1,  3,1,2,0,
												 2,0,1,0,  1,0,0,1,  0,1,1,2 };
								line_num = sizeof(ll) / sizeof(FLOAT) / 4;
								memcpy((VOID *)line_list, (VOID *)ll, sizeof(ll));
							}
							break;
						case '9':
							{
								FLOAT ll[32] = { 3,2,1,2,  1,2,0,3,  0,3,1,4,  1,4,2,4,
												 2,4,3,3,  3,3,3,1,  3,1,2,0,  2,0,1,0 };
								line_num = sizeof(ll) / sizeof(FLOAT) / 4;
								memcpy((VOID *)line_list, (VOID *)ll, sizeof(ll));
							}
							break;
						default:
							return VS_ERROR;
					}
				}
				break;

			case 4: // *ch >= '@'  && *ch <= 'O'
				{
				}
				break;

			case 5: // *ch >= 'P'  && *ch <= '_'
				{
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
				}
				break;

			case 6: // *ch >= '`' && *ch <= 'o'
				return VS_ERROR;

			case 7: // *ch >= 'p' && *ch <= 127
				return VS_ERROR;

			case 8: // *ch >= 128 && *ch <= 143
				{
					switch (*ch)
					{
						case 128:
							{
								FLOAT ll[12] = { 0,0,0,4,  0,4,6,2,  6,2,0,0  };
								line_num = sizeof(ll) / sizeof(FLOAT) / 4;
								memcpy((VOID *)line_list, (VOID *)ll, sizeof(ll));
								*width_mod = Lines3D_GetAsciiCharWidth(*ch);
							}
							break;
						default:
							return VS_ERROR;
					}

				}
				break;

			default:
				return VS_ERROR;
		}


	// ---- add scaled & offset line xy pairs
		*num_lines =  line_num;
		FLOAT *lp_source = NULL;
		FLOAT *lp_dest = line_xy_pairs;
		FLOAT  x_s, y_s;
		for (UINT l = 0; l < line_num; l++)
		{
			lp_source = (line_list + l * 4);
			for (UINT o = 0; o < 2; o++)
			{

				// ---- based on a 4 rows * 3 cols grid with aspect 4/3
					x_s = *(lp_source + o * 2 + 0) / 3 * width;
					y_s = *(lp_source + o * 2 + 1) / 3 * height;

				// ---- axis == 0 alignment
					switch ( align )
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



// ---------- Lines3D_GetAsciiCharWidth ----------
/*!
\brief get ASCII character width modifier
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
FLOAT ObjConfig::Lines3D_GetAsciiCharWidth(
		UCHAR ch // [in] ASCII char code from 0 to 255
	)
{

	// ---- width modifier
		switch (ch)
		{
			case ' ': return  0.5f; // space
			case '.': return  0.5f; // half width
			case 128: return  2.0f; // double width
			default:
				break;
		}

	return 1;
}