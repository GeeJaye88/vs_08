////////////////////////////////////////////////////////////////////////////////

// ---------- vs_s3_acq_lgt_axis.hpp ----------
/*!
\file vs_s3_acq_lgt_axis.hpp
\brief
\author Gareth Edwards
\note

Create a line based graphical 3D XYZ box s3 camera rig based on box dimensions. 

*/


#pragma once


// ---------- Callback_AcqLgtAxis_Configure ----------
INT ObjConfig::Callback_S3_AcqLgtAxis_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling
		context->BeginConfig();

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();

	// ---- get supplied and parameter data

	// ---- hijack box
		UINT mode = (UINT)pd[BOX_FACE_MODE]; // not used
		UINT type = (UINT)pd[BOX_FACE_TYPE]; // all modes format = (UINT)(VS_XYZ | VS_DIFFUSE)

		UINT axis_flag   = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_1];
		UINT light_flag  = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_2];
		UINT radius_flag = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_3];

	// ---- num line pairs
		UINT num_lines =
			( axis_flag   ?   4 * 4 : 0 ) +
			( axis_flag   ? 180 * 1 : 0 ) +
			( light_flag  ?   4     : 0 ) +
			( light_flag  ?  16 * 4 : 0 ) +
			( radius_flag ? 360 * 3 : 0 );

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
		context->SetIndexBufferCallback(Callback_S3_AcqLgtAxis_GetIndexBuffer);

	// ---- primitive type
		context->SetPrimitiveType(VS_LINELIST);

	// ---- vertex buffer
		context->SetVertexCount(num_lines * 2);
		context->SetVertexFormat(format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_S3_AcqLgtAxis_GetVertexBuffer);


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


// ---------- Callback_AcqLgtAxis_GetVertexBuffer ----------
INT ObjConfig::Callback_S3_AcqLgtAxis_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- local
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- get supplied and parameter data
		UINT  mode     = (UINT)pd[BOX_FACE_MODE]; // not used
		UINT  type     = (UINT)pd[BOX_FACE_TYPE];

		FLOAT red      = pd[BOX_FACE_RED];
		FLOAT green    = pd[BOX_FACE_GREEN];
		FLOAT blue     = pd[BOX_FACE_BLUE];

		FLOAT a_ext    = pd[BOX_FACE_SCALE];                          // axis dimension scaling
		FLOAT width    = pd[vs_system::ObjConfig::BOX_FACE_WIDTH];
		FLOAT height   = pd[vs_system::ObjConfig::BOX_FACE_HEIGHT];   // hgt not used
		FLOAT depth    = pd[vs_system::ObjConfig::BOX_FACE_DEPTH];
		FLOAT x_off    = pd[vs_system::ObjConfig::BOX_FACE_X];        // +/- offset
		FLOAT y_off    = pd[vs_system::ObjConfig::BOX_FACE_Y];        //    "
		FLOAT z_off    = pd[vs_system::ObjConfig::BOX_FACE_Z];        //    "

	// ---- flags
		UINT axis_flag   = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_1];
		UINT light_flag  = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_2];
		UINT radius_flag = (UINT)pd[vs_system::ObjConfig::BOX_FLAG_3];

	// ----- light
		FLOAT intensity  = pd[vs_system::ObjConfig::BOX_FLAG_4];
		FLOAT distance   = pd[vs_system::ObjConfig::BOX_FLAG_5];
		FLOAT elevation  = pd[vs_system::ObjConfig::BOX_FLAG_6];
		FLOAT azimuth    = pd[vs_system::ObjConfig::BOX_FLAG_7];


	// ---- set light position

		// ---- light angle stuff
			FLOAT t2r = elevation * (FLOAT)VS_RADIANS_PER_DEGREE;
			FLOAT phi = azimuth;
			FLOAT p2r = phi * (FLOAT)VS_RADIANS_PER_DEGREE;

		// ---- light rotate about x axis
			// y' = y*cos q - z*sin q
			// z' = y*sin q + z*cos q
			FLOAT xt =  0;
			FLOAT yt =  distance * sinf(t2r);
			FLOAT zt = -distance * cosf(t2r);

		// ---- light rotate about y axis
			// z' = z*cos q - x*sin q
			// x' = z*sin q + x*cos q
			FLOAT zl = zt * cos(p2r) - xt * sin(p2r);
			FLOAT xl = zt * sin(p2r) + xt * cos(p2r);

		// ---- light position
			VsVector3 lp = { xl, yt, zl };


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


	// ---- AXIS
		if ( axis_flag )
		{

			// ---- axis stuff
				FLOAT dby2 = depth / 2 * 1.25f;
				FLOAT radius = (FLOAT)sqrt(dby2*dby2 + dby2 * dby2);

			// ---- circle
				for (UINT b = 0; b < 360; b += 2)
				{

					// ---- this and next angle
						FLOAT radian_1 = ( (FLOAT)b     + azimuth ) * (FLOAT)VS_RADIANS_PER_DEGREE;
						FLOAT radian_2 = ( (FLOAT)(b+2) + azimuth ) * (FLOAT)VS_RADIANS_PER_DEGREE;

					// ---- init
						vtx blip[2] =
							{
								{ 0, 0, radius, 0 },
								{ 0, 0, radius, 0 },
							};

					// ---- Y rotate
						// x' = z*sin q + x*cos q
						// z' = z*cos q - x*sin q
						vtx blip_rotated[2];
						for (UINT v = 0; v < 2; v++)
						{
							FLOAT radian = v == 0 ? radian_1 : radian_2;
							blip_rotated[v].x = blip[v].z * (FLOAT)sin(radian) + blip[v].x * (FLOAT)cos(radian) + x_off;
							blip_rotated[v].y = y_off;
							blip_rotated[v].z = blip[v].z * (FLOAT)cos(radian) - blip[v].x * (FLOAT)sin(radian) + z_off;
							blip_rotated[v].d = fd;
						}

					// ---- append
						size_t size_of_blip_rotated = sizeof(blip_rotated);
						memcpy((VOID *)f, (VOID *)blip_rotated, (INT)size_of_blip_rotated);
						f += (INT)size_of_blip_rotated / sizeof(FLOAT);
				}


			// ---- arrows
				for (UINT a = 0; a < 4; a++)
				{

					// ---- angle
						FLOAT angle = (FLOAT)a * 90 + azimuth;
						FLOAT radian = angle * (FLOAT)VS_RADIANS_PER_DEGREE;

					// ---- init
						FLOAT zi = depth/2;
						FLOAT zo = distance;
						vtx arrow[8] =
							{
								{   0, 0, radius, 0 },
								{   0, 0,  zo-aw, 0 },
								{ -aw, 0,  zo-aw, 0 },
								{   0, 0,     zo, 0 },
								{  aw, 0,  zo-aw, 0 },
								{   0, 0,     zo, 0 },
								{ -aw, 0,  zo-aw, 0 },
								{  aw, 0,  zo-aw, 0 },
							};

					// ---- Y rotate
						// x' = z*sin q + x*cos q
						// z' = z*cos q - x*sin q
						vtx arrow_rotated[8];
						for (UINT v = 0; v < 8; v++)
						{
							arrow_rotated[v].x = arrow[v].z * (FLOAT)sin(radian) + arrow[v].x * (FLOAT)cos(radian) + x_off;
							arrow_rotated[v].y = arrow[v].y + y_off;
							arrow_rotated[v].z = arrow[v].z * (FLOAT)cos(radian) - arrow[v].x * (FLOAT)sin(radian) + z_off;
							arrow_rotated[v].d = fd;
						}

					// ---- append
						size_t size_of_arrow_rotated = sizeof(arrow_rotated);
						memcpy((VOID *)f, (VOID *)arrow_rotated, (INT)size_of_arrow_rotated);
						f += (INT)size_of_arrow_rotated / sizeof(FLOAT);
				}

		}


	// ---- LIGHTS
		if ( light_flag )
		{

			// ---- lights @ l * 90 degrees
				for (UINT l = 0; l < 4; l++)
				{

					// ---- angle
						FLOAT angle = (FLOAT)l * 90 + azimuth;
						FLOAT radian = angle * (FLOAT)VS_RADIANS_PER_DEGREE;

					// ---- init
						FLOAT y  = lp.y;
						FLOAT z  = distance;
						vtx light[2] =
							{
								{ 0,    0, z, 0 },
								{ 0, y-aw, z, 0 }
							};

					// ---- rotate
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

					// ----- light
						vtx light_circle[32];
						for (UINT i = 0; i < 16; i++)
						{

							// ---- this and next angle
								FLOAT radian_1 = ( (FLOAT) i    * 22.5f ) * (FLOAT)VS_RADIANS_PER_DEGREE;
								FLOAT radian_2 = ( (FLOAT)(i+1) * 22.5f ) * (FLOAT)VS_RADIANS_PER_DEGREE;

							// ---- Z rotate (y == 0)
								FLOAT x1 = -aw * (FLOAT)sin(radian_1);
								FLOAT y1 =  aw * (FLOAT)cos(radian_1);
								FLOAT x2 = -aw * (FLOAT)sin(radian_2);
								FLOAT y2 =  aw * (FLOAT)cos(radian_2);

							// ---- Y rotate
								light_circle[i*2  ].x =  x1 * (FLOAT)cos(radian) + light_rotated[1].x;
								light_circle[i*2  ].y =  y1                        + y;
								light_circle[i*2  ].z = -x1 * (FLOAT)sin(radian) + light_rotated[1].z;
								light_circle[i*2  ].d =  fd;

								light_circle[i*2+1].x =  x2 * (FLOAT)cos(radian) + light_rotated[1].x;
								light_circle[i*2+1].y =  y2                        + y;
								light_circle[i*2+1].z = -x2 * (FLOAT)sin(radian) + light_rotated[1].z;
								light_circle[i*2+1].d =  fd;
						}

						size_t size_of_light_circle = sizeof(light_circle);
						memcpy((VOID *)f, (VOID *)light_circle, (INT)size_of_light_circle);
						f += (INT)size_of_light_circle / sizeof(FLOAT);

				}
			
		}

	// ---- RADII
		if ( radius_flag )
		{

			// ---- blips
				for (UINT b = 0; b < 360; b++)
				{

					// ---- angle
						FLOAT radian_1 = ( (FLOAT)b     + azimuth ) * (FLOAT)VS_RADIANS_PER_DEGREE;
						FLOAT radian_2 = ( (FLOAT)(b+1) + azimuth ) * (FLOAT)VS_RADIANS_PER_DEGREE;

					// ---- init
						FLOAT zi = distance;
						FLOAT zo = distance + (aw * 3);
						vtx blip[6] =
							{
								{   0,    0,   zi,   0 },
								{   0,    0,   zo,   0 },
								{   0,    0,   zi,   0 },
								{   0,    0,   zi,   0 },
								{   0,    0,   zo,   0 },
								{   0,    0,   zo,   0 },
							};

					// ---- Y rotate
						// x' = z*sin q + x*cos q
						// z' = z*cos q - x*sin q
						vtx blip_rotated[6];
						for (UINT v = 0; v < 6; v++)
						{
							FLOAT radian = ( v == 2 || v == 4 ) ? radian_2 : radian_1;
							blip_rotated[v].x = blip[v].z * (FLOAT)sin(radian) + blip[v].x * (FLOAT)cos(radian) + x_off;
							blip_rotated[v].y = y_off;
							blip_rotated[v].z = blip[v].z * (FLOAT)cos(radian) - blip[v].x * (FLOAT)sin(radian) + z_off;
							blip_rotated[v].d = fd;
						}

					// ---- append
						size_t size_of_blip_rotated = sizeof(blip_rotated);
						memcpy((VOID *)f, (VOID *)blip_rotated, (INT)size_of_blip_rotated);
						f += (INT)size_of_blip_rotated / sizeof(FLOAT);
				}

		}

	return VS_OK;
}


// ---------- Callback_AcqLgtAxis_GetIndexBuffer ----------
INT ObjConfig::Callback_S3_AcqLgtAxis_GetIndexBuffer(
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