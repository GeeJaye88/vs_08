////////////////////////////////////////////////////////////////////////////////

// ---------- vs_pano_EXPOSURE_AOI_line.h ----------
/*!
\file vs_pano_EXPOSURE_AOI_line.h
\brief
\author Gareth Edwards
*/


#pragma once


INT ObjConfig::Callback_PanoExposureAec_Configure(vs_system::ElmContext *context)
{

	context->BeginConfig();
	{

		// ---- local ----
		FLOAT *p = context->GetFloatParameter();

		// ---- atlas page ----
		context->SetAtlasPage(0, 0);


		// ---- IF overide THEN --
		if (!context->GetOveride())
		{
			context->SetAlphaBlending(false);
			context->SetColourVertex(true);
			context->SetMultiState(false);
			context->SetSolid(false);
			context->SetTextured(FALSE);
			context->SetVisible(true);
		}


		// ---- provide for 2 x circle (cheat for rectangle) ----
		UINT num_circle = 2;
		UINT num_lines = 180;


		// ---- index buffer ----
		context->SetIndexCount(num_lines * 2 * num_circle);
		context->SetIndexBufferCallback(&Callback_PanoExposureAec_GetIndexBuffer);


		// ---- vertex buffer ----
		context->SetVertexCount((num_lines + 1) * num_circle);
		context->SetVertexFormat((UINT)(VS_XYZRHW | VS_DIFFUSE));
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_PanoExposureAec_GetVertexBuffer);


		// ---- primitive parameters ----
		context->SetPrimitiveBytes(
			context->GetVertexSize() *
			context->GetVertexCount()  *
			sizeof(float)
			);
		context->SetPrimitiveCount(num_lines* num_circle);
		context->SetPrimitiveType(VS_LINELIST);

	}
	context->EndConfig();

	return VS_OK;
}


INT ObjConfig::Callback_PanoExposureAec_GetVertexBuffer(vs_system::ElmContext *context)
{

	switch (pi_obj_pano->exposure_aoi_zone)
	{
		case AOI_ALL:
		case AOI_ANNULAR:
			{
				INT result = PanoExposureAec_GetAnnularVertexBuffer(
					context,
					180
					);
			}
			break;

		case AOI_TOP:
		case AOI_LEFT:
		case AOI_BOTTOM:
		case AOI_RIGHT:
			{
				INT result = PanoExposureAec_GetVertexBuffer_Rect(
					context
					);
			}
		break;
	}

	return VS_OK;
}


INT ObjConfig::PanoExposureAec_GetAnnularVertexBuffer(
	vs_system::ElmContext *context,
	UINT num_lines
	)
{

	// ---- local pano parameters ----
	FLOAT rad_min = pi_obj_pano->min_radius;
	FLOAT rad_max = pi_obj_pano->max_radius;
	FLOAT aoi_min = pi_obj_pano->exposure_aoi_min;
	FLOAT aoi_max = pi_obj_pano->exposure_aoi_max;


	// ---- vertex ----
	FLOAT *sp = context->GetGfxFloatParameter();
	FLOAT left = sp[VS_PANEL_LEFT];
	FLOAT top = sp[VS_PANEL_TOP];
	FLOAT right = sp[VS_PANEL_RIGHT];
	FLOAT bottom = sp[VS_PANEL_BOTTOM];
	FLOAT width = right - left - 1;
	FLOAT height = bottom - top - 1;
	FLOAT hby2 = height / 2;
	FLOAT wby2 = width / 2;
	FLOAT cx = width / 2 + left;
	FLOAT cy = height / 2 + top;


	// ---- vertex ----
	struct XYZWD { float x, y, z, w; DWORD diffuse; };
	union colour_union_t {
		DWORD d;
		FLOAT f;
	} diffuse;
	diffuse.d = VS_D3DCOLOR_COLORVALUE(1.0f, 0.0f, 0.0f, 1.0f);


	// ---- buffer ----
	FLOAT *f = context->GetVertexBufferPtr();


	// ---- circle param ----
	UINT num_circle = 2;
	UINT num_vertex = num_lines + 1;


	// ---- circles ----
	UINT vertex_size = context->GetVertexSize();
	FLOAT x = 0;
	BOOL  zall = pi_obj_pano->exposure_aoi_zone == AOI_ALL;
	FLOAT ymin = zall ? rad_min : aoi_min;
	FLOAT ymax = zall ? rad_max : aoi_max;
	FLOAT y[] = { hby2 * ymax, hby2 * ymin };
	float zed = 0.75;
	float rhw = 1;
	for (UINT ic = 0; ic < num_circle; ic++)
	{
		for (UINT iv = 0; iv < num_vertex; iv++)
		{
			FLOAT theta = (FLOAT)iv * 2 * (FLOAT)VS_RADIANS_PER_DEGREE;
			FLOAT rx = cos(theta) * x - sin(theta) * y[ic] + cx;
			FLOAT ry = sin(theta) * x + cos(theta) * y[ic] + cy;
			*(f) = rx;
			*(f + 1) = ry;
			*(f + 2) = zed;
			*(f + 3) = rhw;
			*(f + 4) = diffuse.f;
			f += vertex_size;
		}
	}

	return VS_OK;
}


INT ObjConfig::PanoExposureAec_GetVertexBuffer_Rect(vs_system::ElmContext *context)
{

	// ---- local pano parameters ----
	FLOAT u_flip = U_FLIP_LOGIC;
	FLOAT v_flip = V_FLIP_LOGIC;


	// ---- config parameters ----
	UINT shape = pi_obj_pano->exposure_aoi_shape;
	UINT zone = pi_obj_pano->exposure_aoi_zone;


	// ---- calculate AOI dimension with annular at origin ----
	FLOAT max_rad = pi_obj_pano->max_radius;
	FLOAT min_rad = pi_obj_pano->max_radius;
	FLOAT max_aoi = pi_obj_pano->exposure_aoi_max;
	FLOAT hgt_aoi = max_aoi - pi_obj_pano->exposure_aoi_min;
	FLOAT x = sqrt(max_rad*max_rad - max_aoi*max_aoi);


	// ---- panel parameters ----
	FLOAT *sp = context->GetGfxFloatParameter();
	FLOAT left = sp[VS_PANEL_LEFT];
	FLOAT top = sp[VS_PANEL_TOP];
	FLOAT right = sp[VS_PANEL_RIGHT];
	FLOAT bottom = sp[VS_PANEL_BOTTOM];


	// ---- derived panel parameters ----
	FLOAT width = right - left - 1;
	FLOAT height = bottom - top - 1;
	FLOAT hby2 = height / 2;
	FLOAT wby2 = width / 2;
	FLOAT cx = width / 2 + left;
	FLOAT cy = height / 2 + top;


	// ---- vertex ----
	struct XYZWD { float x, y, z, w; DWORD diffuse; };
	union colour_union_t {
		DWORD d;
		FLOAT f;
	} diffuse;
	diffuse.d = VS_D3DCOLOR_COLORVALUE(1.0f, 0.0f, 0.0f, 1.0f);


	// ---- store ----
	float xmin = x * hby2;
	float xmax = x * hby2;
	float ymin = max_aoi * hby2;
	float ymax = (max_aoi - hgt_aoi) * hby2;
	float zed = 0.9f;
	float rhw = 1;


	// ---- init shape ref xy ----
	float xx[4] = { -xmin, -xmin, xmax, xmax };
	float yy[4] = { ymin, ymax, ymax, ymin };


	// ---- lambda swap to rotate xy to yx ---- 
	auto swap2 = [](FLOAT *a, FLOAT *b)
	{
		FLOAT t = *a;
		*a = *b;
		*b = t;
	};


	// ---- locate ----
	switch (zone)
	{
		default:
		case AOI_TOP:
			break;
		case AOI_LEFT:
			for (UINT i = 0; i < 4; i++)
			{
				swap2(&xx[i], &yy[i]);
				yy[i] *= -1;
			}
			break;
		case AOI_BOTTOM:
			for (UINT i = 0; i < 4; i++)
			{
				yy[i] *= -1;
			}
			break;
		case AOI_RIGHT:
			for (UINT i = 0; i < 4; i++)
			{
				swap2(&xx[i], &yy[i]);
				xx[i] *= -1;
			}
			break;
	}


	// ---- finally, init vertex buffer struct arrays ----
	XYZWD elem[] =
	{
		{ cx - xx[0], cy - yy[0], zed, rhw, diffuse.d },
		{ cx - xx[1], cy - yy[1], zed, rhw, diffuse.d },
		{ cx - xx[2], cy - yy[2], zed, rhw, diffuse.d },
		{ cx - xx[3], cy - yy[3], zed, rhw, diffuse.d },
		{ cx - xx[0], cy - yy[0], zed, rhw, diffuse.d }
	};


	// ---- store ----
	memcpy(context->GetVertexBufferPtr(), elem, sizeof(elem));

	return VS_OK;
}


INT ObjConfig::Callback_PanoExposureAec_GetIndexBuffer(vs_system::ElmContext *context)
{
	printf(" + GetIndexBuffer\n");

	switch (pi_obj_pano->exposure_aoi_zone)
	{
		case AOI_ALL:
		case AOI_ANNULAR:
		{
			INT result = PanoExposureAec_GetAnnularIndexBuffer(
				context,
				180
				);
		}
		break;
		case AOI_TOP:
		case AOI_LEFT:
		case AOI_BOTTOM:
		case AOI_RIGHT:
		{
			INT result = PanoExposureAec_GetRectIndexBuffer(
				context
				);
		}
		break;
	}

	return VS_OK;
}


INT ObjConfig::PanoExposureAec_GetAnnularIndexBuffer(
	vs_system::ElmContext *context,
	UINT num_lines
	)
{

	printf("  - GetAnnularIndexBuffer\n");

	context->SetIndexCount(num_lines * 2 * 2);
	context->SetVertexCount((num_lines + 1) * 2);
	context->SetPrimitiveCount(num_lines * 2);


	// ---- buffer
		#if VS_INDEX_BUFFER_FORMAT == 32
		DWORD *i = context->GetIndexBufferPtr();
		#else
		WORD *i = context->GetIndexBufferPtr();
		#endif


	UINT index_base = 0;
	for (UINT ic = 0; ic < 2; ic++)
	{
		for (UINT il = 0; il < num_lines; il++)
		{
			*(i) = index_base + il;
			*(i + 1) = index_base + il + 1;
			i += 2;
		}
		index_base = num_lines + 1;
	}

	return VS_OK;
}


INT ObjConfig::PanoExposureAec_GetRectIndexBuffer(
	vs_system::ElmContext *context
	)
{

	printf("  - GetRectIndexBuffer\n");

	context->SetIndexCount(8);
	context->SetVertexCount(4);
	context->SetPrimitiveCount(4);

	// ---- buffer
		#if VS_INDEX_BUFFER_FORMAT == 32
		DWORD *i = context->GetIndexBufferPtr();
		#else
		WORD *i = context->GetIndexBufferPtr();
		#endif


	*(i++) = 0; *(i++) = 1;
	*(i++) = 1; *(i++) = 2;
	*(i++) = 2; *(i++) = 3;
	*(i++) = 3; *(i++) = 0;

	return VS_OK;
}

