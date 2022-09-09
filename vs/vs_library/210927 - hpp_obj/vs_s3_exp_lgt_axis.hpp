////////////////////////////////////////////////////////////////////////////////

// ---------- vs_s3_exp_lgt_axis.hpp ----------
/*!
\file vs_s3_exp_lgt_axis.hpp
\brief
\author Gareth Edwards
\note

Create a line based graphical 3D XYZ box s3 camera rig based on box dimensions. 

*/


#pragma once


// ---------- Callback_ExpLgtAxis_Configure ----------
INT ObjConfig::Callback_ExpLgtAxis_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();

	// ---- get supplied and parameter data
		UINT mode      = (UINT)pd[BOX_FACE_MODE];
		UINT type      = (UINT)pd[BOX_FACE_TYPE];

		FLOAT w = pd[vs_system::ObjConfig::BOX_FACE_WIDTH];
		FLOAT h = pd[vs_system::ObjConfig::BOX_FACE_HEIGHT];
		FLOAT d = pd[vs_system::ObjConfig::BOX_FACE_DEPTH];

		UINT axis_flag   = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_1];
		UINT light_flag  = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_2];

	// ---- num line pairs
		UINT num_lines =
			(axis_flag  ? 60 : 0) +
			(axis_flag  ? 4 : 0) +
			(light_flag ? 3 : 0);

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
		context->SetIndexBufferCallback(Callback_ExpLgtAxis_GetIndexBuffer);

	// ---- primitive type
		context->SetPrimitiveType(VS_LINELIST);

	// ---- vertex buffer
		context->SetVertexCount(num_lines * 2);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_ExpLgtAxis_GetVertexBuffer);

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


// ---------- Callback_ExpLgtAxis_GetVertexBuffer ----------
INT ObjConfig::Callback_ExpLgtAxis_GetVertexBuffer(vs_system::ElmContext *context)
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

		FLOAT a_ext    = pd[BOX_FACE_SCALE];                          // not used
		FLOAT width    = pd[vs_system::ObjConfig::BOX_FACE_WIDTH];
		FLOAT height   = pd[vs_system::ObjConfig::BOX_FACE_HEIGHT];   // hgt not used
		FLOAT depth    = pd[vs_system::ObjConfig::BOX_FACE_DEPTH];
		FLOAT x_off    = pd[vs_system::ObjConfig::BOX_FACE_X];        // +/- offset
		FLOAT y_off    = pd[vs_system::ObjConfig::BOX_FACE_Y];        //    "
		FLOAT z_off    = pd[vs_system::ObjConfig::BOX_FACE_Z];        //    "

	// ---- flags
		UINT axis_flag  = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_1];
		UINT light_flag = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_2];


	// ----- light
		UINT  lgt_index = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_3];
		FLOAT lgt_x = pd[vs_system::ObjConfig::BOX_FLAG_4];
		FLOAT lgt_y = pd[vs_system::ObjConfig::BOX_FLAG_5];
		FLOAT lgt_z = pd[vs_system::ObjConfig::BOX_FLAG_6];


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
		diffuse.d = VS_D3DCOLOR_COLORVALUE(red, green, blue, 1.0f);
		FLOAT fd = diffuse.f;


	// ---- vertices
		struct vtx
		{
			float x, y, z, d;
		};


	// ---- useful stuff
		FLOAT aw = depth * 0.025f; // arrow width
		FLOAT azimuth  = (FLOAT)atan2(lgt_z,lgt_x) * (FLOAT)VS_RADIAN;
		FLOAT radian   = -azimuth * (FLOAT)VS_RADIANS_PER_DEGREE;
		FLOAT distance = (FLOAT)sqrt(lgt_x*lgt_x + lgt_z*lgt_z);


	// ---- AXIS
		if ( axis_flag )
		{

			// ---- axis stuff
				FLOAT dby2   = depth / 2 * 1.25f;
				FLOAT radius = (FLOAT)sqrt(dby2*dby2 + dby2*dby2);
				UINT  angle  = 60;
				FLOAT aby2   = (FLOAT)angle / 2 + azimuth;

			// ---- circle
				for (UINT a = 0; a < angle; a ++)
				{

					// ---- this and next angle
						FLOAT radian_1 = (FLOAT)(a     - aby2) * (FLOAT)VS_RADIANS_PER_DEGREE;
						FLOAT radian_2 = (FLOAT)(a + 1 - aby2) * (FLOAT)VS_RADIANS_PER_DEGREE;

					// ---- init
						vtx line[2] =
							{
								{ radius, 0, 0, 0 },
								{ radius, 0, 0, 0 },
							};

					// ---- Y rotate
						// x' = z*sin q + x*cos q
						// z' = z*cos q - x*sin q
						vtx line_rotated[2];
						for (UINT v = 0; v < 2; v++)
						{
							FLOAT radian = v == 0 ? radian_1 : radian_2;
							line_rotated[v].x = line[v].z * (FLOAT)sin(radian) + line[v].x * (FLOAT)cos(radian) + x_off;
							line_rotated[v].y = y_off;
							line_rotated[v].z = line[v].z * (FLOAT)cos(radian) - line[v].x * (FLOAT)sin(radian) + z_off;
							line_rotated[v].d = fd;
						}

					// ---- append
						size_t size_of_line_rotated = sizeof(line_rotated);
						memcpy((VOID *)f, (VOID *)line_rotated, (INT)size_of_line_rotated);
						f += (INT)size_of_line_rotated / sizeof(FLOAT);
				}


			// ---- arrow
				FLOAT x = distance;
				vtx arrow[8] =
					{
						{  radius, 0,   0, fd },
						{    x-aw, 0,   0, fd },
						{    x-aw, 0,  aw, fd },
						{       x, 0,   0, fd },
						{    x-aw, 0, -aw, fd },
						{       x, 0,   0, fd },
						{    x-aw, 0, -aw, fd },
						{    x-aw, 0,  aw, fd },
					};

				// ---- Y rotate
					// x' = z*sin q + x*cos q
					// z' = z*cos q - x*sin q
					vtx arrow_rotated[8];
					for (UINT v = 0; v < 8; v++)
					{
						arrow_rotated[v].x = arrow[v].z * (FLOAT)sin(radian) + arrow[v].x * (FLOAT)cos(radian) + x_off;
						arrow_rotated[v].y = y_off;
						arrow_rotated[v].z = arrow[v].z * (FLOAT)cos(radian) - arrow[v].x * (FLOAT)sin(radian) + z_off;
						arrow_rotated[v].d = fd;
					}

			// ---- append
				size_t size_of_arrow_rotated = sizeof(arrow_rotated);
				memcpy((VOID *)f, (VOID *)arrow_rotated, (INT)size_of_arrow_rotated);
				f += (INT)size_of_arrow_rotated / sizeof(FLOAT);

			}



	// ---- LIGHT
		if ( light_flag )
		{

			// ---- light
				vtx light[2] =
					{
						{ distance,     0, 0, fd },
						{ distance, lgt_y, 0, fd },
					};

				// ---- Y rotate
					// x' = z*sin q + x*cos q
					// z' = z*cos q - x*sin q
					vtx light_rotated[2];
					for (UINT v = 0; v < 2; v++)
					{
						light_rotated[v].x = light[v].z * (FLOAT)sin(radian) + light[v].x * (FLOAT)cos(radian) + x_off;
						light_rotated[v].y = light[v].y + y_off;
						light_rotated[v].z = light[v].z * (FLOAT)cos(radian) - light[v].x * (FLOAT)sin(radian) + z_off;
						light_rotated[v].d = fd;
					}

			// ---- append
				size_t size_of_light_rotated = sizeof(light_rotated);
				memcpy((VOID *)f, (VOID *)light_rotated, (INT)size_of_light_rotated);
				f += (INT)size_of_light_rotated / sizeof(FLOAT);


		}

	return VS_OK;
}


// ---------- Callback_ExpLgtAxis_GetIndexBuffer ----------
INT ObjConfig::Callback_ExpLgtAxis_GetIndexBuffer(
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