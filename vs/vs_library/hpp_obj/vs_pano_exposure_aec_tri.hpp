////////////////////////////////////////////////////////////////////////////////

// ---------- vs_pano_EXPOSURE_AOI_tri.h ----------
/*!
\file vs_pano_EXPOSURE_AOI_tri.h
\brief
\author Gareth Edwards
*/


#pragma once


//#define VS_PEA_RECT_MODE

#define VS_PEA_TRI_NUM_CIRC   2
#define VS_PEA_TRI_NUM_EDGE   180
#define VS_PEA_TRI_NUM_INDX   (  VS_PEA_TRI_NUM_EDGE      * 6 * VS_PEA_TRI_NUM_CIRC )
#define VS_PEA_TRI_NUM_VRTX   ( (VS_PEA_TRI_NUM_EDGE + 1) * 2 * VS_PEA_TRI_NUM_CIRC )
#define VS_PEA_TRI_NUM_PRIM   (  VS_PEA_TRI_NUM_EDGE      * 2 * VS_PEA_TRI_NUM_CIRC )
#define VS_PEA_TRI_E_VAL      0.0075f
#define VS_PEA_TRI_COLOR_MAX  VS_D3DCOLOR_COLORVALUE(0.16f, 0.4f, 1.00f, 1.0f);
#define VS_PEA_TRI_COLOR_MIN  VS_D3DCOLOR_COLORVALUE(0.12f, 0.3f, 0.75f, 1.0f);
/*

Model::SetupPanoGeometry()
            |
			V
   Callback_PanoExposureAec_T_Configure
			   |
			   |--- set -> Callback_PanoExposureAec_T_GetVertexBuffer
			   |--- set -> Callback_PanoExposureAec_T_GetIndexBuffer



GfxInterface::RenderViewport
            |
            V
   GfxInterface::PI_Element::Setup
               |
               V

      ElmContext::VertexBufferCallback
                  |
                  V
         Callback_PanoExposureAec_T_GetVertexBuffer
                     |

            switch (pi_obj_pano->aoi_zone)
            {
   		       case AOI_ALL:
		       case AOI_ANNULAR:
   		          PanoExposureAec_T_GetAnnularVertexBuffer
		          break;

  		       case AOI_TOP:
		       case AOI_LEFT:
		       case AOI_BOTTOM:
		       case AOI_RIGHT:
			      ( PanoExposureAec_T_GetRectVertexBuffer )
		          PanoExposureAec_T_GetSectorVertexBuffer
		          break;
		    }

      ElmContext::IndexBufferCallback
                  |
                  V
         Callback_PanoExposureAec_T_GetIndexBuffer
                     |

            switch (pi_obj_pano->aoi_zone)
            {
   		       case AOI_ALL:
		       case AOI_ANNULAR:
   		          PanoExposureAec_T_GetAnnularIndexBuffer
		          break;

  		       case AOI_TOP:
		       case AOI_LEFT:
		       case AOI_BOTTOM:
		       case AOI_RIGHT:
			      ( PanoExposureAec_T_GetAnnularIndexBuffer )
		          PanoExposureAec_T_GetSectorIndexBuffer
		          break;
		    }

*/

INT ObjConfig::Callback_PanoExposureAec_T_Configure(
		vs_system::ElmContext *context
	)
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
			context->SetSolid(true);
			context->SetTextured(FALSE);
			context->SetVisible(true);
			context->SetCullMode(vs_system::ElmContext::CULL_NONE);
		}


		// ---- index buffer ----
		context->SetIndexCount( VS_PEA_TRI_NUM_INDX );
		context->SetIndexBufferCallback(&Callback_PanoExposureAec_T_GetIndexBuffer);


		// ---- vertex buffer ----
		context->SetVertexCount( VS_PEA_TRI_NUM_VRTX );
		context->SetVertexFormat((UINT)(VS_XYZRHW | VS_DIFFUSE));
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_PanoExposureAec_T_GetVertexBuffer);


		// ---- primitive parameters ----
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
			);
		context->SetPrimitiveCount( VS_PEA_TRI_NUM_PRIM );
		context->SetPrimitiveType(VS_TRIANGLELIST);

	}
	context->EndConfig();

	return VS_OK;
}


INT ObjConfig::Callback_PanoExposureAec_T_GetVertexBuffer(
		vs_system::ElmContext *context
	)
{

	switch ( context->GetId() )
	{

		case VS_OBJ_PANO_ANNULAR_FOCUS_AOI:
			{
				INT result = PanoExposureAec_T_GetSectorVertexBuffer(context);
			}
			break;
		
		default:

			switch (pi_obj_pano->exposure_aoi_zone)
			{
				case AOI_ALL:
				case AOI_ANNULAR:
					{
						#ifdef VS_PEA_RECT_MODE
						INT result = PanoExposureAec_T_GetRectVertexBuffer(context);
						#else
						INT result = PanoExposureAec_T_GetAnnularVertexBuffer(context);
						#endif
					}
					break;
				case AOI_TOP:
				case AOI_LEFT:
				case AOI_BOTTOM:
				case AOI_RIGHT:
				{
						#ifdef VS_PEA_RECT_MODE
						INT result = PanoExposureAec_T_GetRectVertexBuffer(context);
						#else
						INT result = PanoExposureAec_T_GetSectorVertexBuffer(context);
						#endif
					}
					break;
			}
			break;
	}

	return VS_OK;
}


INT ObjConfig::PanoExposureAec_T_GetAnnularVertexBuffer(
		vs_system::ElmContext *context
	)
{

	// ---- local pano parameters ----
	FLOAT rad_min = pi_obj_pano->min_radius;
	FLOAT rad_max = pi_obj_pano->max_radius;
	FLOAT aoi_min = pi_obj_pano->exposure_aoi_min;
	FLOAT aoi_max = pi_obj_pano->exposure_aoi_max;


	// ---- local ----
	FLOAT *sp = context->GetGfxFloatParameter();
	FLOAT left   = sp[VS_PANEL_LEFT];
	FLOAT top    = sp[VS_PANEL_TOP];
	FLOAT right  = sp[VS_PANEL_RIGHT];
	FLOAT bottom = sp[VS_PANEL_BOTTOM];

	FLOAT width  = right - left - 1;
	FLOAT height = bottom - top - 1;
	FLOAT hby2   = height/2;
	FLOAT wby2   = width/2;
	FLOAT cx     = width/2 + left;
	FLOAT cy     = height/2 + top;
	FLOAT e_val  = VS_PEA_TRI_E_VAL * (FLOAT)hby2;


	// ---- vertex ----
	struct XYZWD { float x, y, z, w; DWORD diffuse; };
	union colour_union_t {
		DWORD d;
		FLOAT f;
	}  min, max;
	min.d = VS_PEA_TRI_COLOR_MIN;
	max.d = VS_PEA_TRI_COLOR_MAX;



	// ---- buffer ----
	FLOAT *f = context->GetVertexBufferPtr();


	// ---- local param ----
	UINT num_vertex = VS_PEA_TRI_NUM_EDGE+1;


	// ---- circles ----
	UINT vertex_size = context->GetVertexSize();
	FLOAT x    = 0;
	BOOL  zall = pi_obj_pano->exposure_aoi_zone == AOI_ALL;
	FLOAT ymin = zall ? rad_min : aoi_min;
	FLOAT ymax = zall ? rad_max : aoi_max;
	FLOAT y[VS_PEA_TRI_NUM_CIRC] = { hby2 * ymin, hby2 * ymax };
	float zed = 0.75;
	float rhw = 1;

	for (UINT ic = 0; ic < VS_PEA_TRI_NUM_CIRC; ic++)
	{
		FLOAT e[VS_PEA_TRI_NUM_CIRC] = { ic == 0 ? -e_val : 0, ic == 0 ? 0 : e_val };
		for (UINT iv = 0; iv < num_vertex; iv++)
		{
			for (UINT iq = 0; iq < 2; iq++)
			{
				FLOAT theta = (FLOAT)iv * 2 * (FLOAT)VS_RADIANS_PER_DEGREE;
				FLOAT rx = -sin(theta) * (y[ic] + e[iq]) + cx;
				FLOAT ry =  cos(theta) * (y[ic] + e[iq]) + cy;
				*(f) = rx;
				*(f + 1) = ry;
				*(f + 2) = zed;
				*(f + 3) = rhw;
				*(f + 4) = ic == 0 ? min.f : max.f;
				f += vertex_size;
			}
		}
	}

	return VS_OK;
}


INT ObjConfig::PanoExposureAec_T_GetRectVertexBuffer(vs_system::ElmContext *context)
{

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
	FLOAT left   = sp[VS_PANEL_LEFT];
	FLOAT top    = sp[VS_PANEL_TOP];
	FLOAT right  = sp[VS_PANEL_RIGHT];
	FLOAT bottom = sp[VS_PANEL_BOTTOM];


	// ---- derived panel parameters ----
	FLOAT width  = right - left - 1;
	FLOAT height = bottom - top - 1;
	FLOAT hby2   = height/2;
	FLOAT wby2   = width/2;
	FLOAT cx     = width/2 + left;
	FLOAT cy     = height/2 + top;


	// ---- fvf ----
	struct XYZWD { float x, y, z, w; DWORD diffuse; };
	union colour_union_t {
		DWORD d;
		FLOAT f;
	}  min, max;
	min.d = VS_PEA_TRI_COLOR_MIN;
	max.d = VS_PEA_TRI_COLOR_MAX;


	// ---- store ----
	FLOAT xmin  =  x * hby2;
	FLOAT xmax  =  x * hby2;
	FLOAT ymax  =  max_aoi * hby2;
	FLOAT ymin  = (max_aoi - hgt_aoi) * hby2;
	FLOAT zed   =  0.5f;
	FLOAT rhw   =  1;
	FLOAT e_val =  VS_PEA_TRI_E_VAL * (FLOAT)hby2;


	// ---- init default "top" xy ----
	FLOAT xi[4] = { -xmin,   -xmin,   xmax,   xmax   };
	FLOAT yi[4] = {  ymin,    ymax,   ymax,   ymin   };
	FLOAT xo[4] = { -xmin - e_val, -xmin - e_val, xmax + e_val, xmax + e_val };
	FLOAT yo[4] = {  ymin - e_val,  ymax + e_val, ymax + e_val, ymin - e_val };


	// ---- exception ----
	if (zone == AOI_ALL)
	{
		xo[0] = -hby2;  yo[0] = -hby2;
		xo[1] = -hby2;  yo[1] =  hby2;
		xo[2] =  hby2;  yo[2] =  hby2;
		xo[3] =  hby2;  yo[3] = -hby2;
		xi[0] = xo[0] + e_val; yi[0] = yo[0] + e_val;
		xi[1] = xo[1] + e_val; yi[1] = yo[1] - e_val;
		xi[2] = xo[2] - e_val; yi[2] = yo[2] - e_val;
		xi[3] = xo[3] - e_val; yi[3] = yo[3] + e_val;
	}
	else if (zone == AOI_ANNULAR)
	{
		xo[0] = -ymax;  yo[0] = -ymax;
		xo[1] = -ymax;  yo[1] =  ymax;
		xo[2] =  ymax;  yo[2] =  ymax;
		xo[3] =  ymax;  yo[3] = -ymax;
		xi[0] =  xo[0] + e_val; yi[0] = yo[0] + e_val;
		xi[1] =  xo[1] + e_val; yi[1] = yo[1] - e_val;
		xi[2] =  xo[2] - e_val; yi[2] = yo[2] - e_val;
		xi[3] =  xo[3] - e_val; yi[3] = yo[3] + e_val;
	}


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
		case AOI_ALL:
		case AOI_ANNULAR:
			break;
		case AOI_TOP:
			break;
		case AOI_LEFT:
			for (UINT i = 0; i < 4; i++)
			{
				swap2(&xi[i], &yi[i]);  yi[i] *= -1;
				swap2(&xo[i], &yo[i]);  yo[i] *= -1;
			}
			break;
		case AOI_BOTTOM:
			for (UINT i = 0; i < 4; i++)
			{
				yi[i] *= -1;
				yo[i] *= -1;
			}
			break;
		case AOI_RIGHT:
			for (UINT i = 0; i < 4; i++)
			{
				swap2(&xi[i], &yi[i]); xi[i] *= -1;
				swap2(&xo[i], &yo[i]); xo[i] *= -1;
			}
			break;
	}


	// ---- order ----
	XYZWD elem[] =
	{
		{ cx - xi[0], cy - yi[0], zed, rhw, max.d },
		{ cx - xi[1], cy - yi[1], zed, rhw, max.d },
		{ cx - xi[2], cy - yi[2], zed, rhw, max.d },
		{ cx - xi[3], cy - yi[3], zed, rhw, max.d },
		{ cx - xo[0], cy - yo[0], zed, rhw, max.d },
		{ cx - xo[1], cy - yo[1], zed, rhw, max.d },
		{ cx - xo[2], cy - yo[2], zed, rhw, max.d },
		{ cx - xo[3], cy - yo[3], zed, rhw, max.d },
	};


	// ---- store ----
	memcpy(context->GetVertexBufferPtr(), elem, sizeof(elem));

	return VS_OK;
}


INT ObjConfig::PanoExposureAec_T_GetSectorVertexBuffer(vs_system::ElmContext *context)
{

	// ---- zone parameters ----
	UINT  zone = context->GetId() == VS_OBJ_PANO_ANNULAR_FOCUS_AOI ?
			AOI_FOCUS : pi_obj_pano->exposure_aoi_zone;


	FLOAT sector_rotate = 0;
	FLOAT sector_angle  = 90.0f;
	switch ( zone )
	{
		default:
		case AOI_TOP:
			break;
		case AOI_LEFT:
			sector_rotate = 270;
			break;
		case AOI_BOTTOM:
			sector_rotate = 180;
			break;
		case AOI_RIGHT:
			sector_rotate = 90;
			break;
		case AOI_FOCUS:
		{
			sector_rotate = pi_obj_pano->focus_aoi_rotate;
			sector_angle  = pi_obj_pano->focus_aoi_sector;
		}
		break;
	}
	FLOAT sector_min_angle = sector_rotate - sector_angle / 2 + 180.f;



	// ---- panel parameters ----
	FLOAT *sp    = context->GetGfxFloatParameter();
	FLOAT left   = sp[VS_PANEL_LEFT];
	FLOAT top    = sp[VS_PANEL_TOP];
	FLOAT right  = sp[VS_PANEL_RIGHT];
	FLOAT bottom = sp[VS_PANEL_BOTTOM];


	// ---- derived panel parameters ----
	FLOAT width  = right - left - 1;
	FLOAT height = bottom - top - 1;
	FLOAT hby2   = height / 2;
	FLOAT wby2   = width / 2;
	FLOAT cx     = width / 2 + left;
	FLOAT cy     = height / 2 + top;


	// ---- vertex ----
	struct XYZWD { float x, y, z, w; DWORD diffuse; };
	union colour_union_t {
		DWORD d; 
		FLOAT f;
	} min, max;
	min.d = VS_PEA_TRI_COLOR_MIN;
	max.d = VS_PEA_TRI_COLOR_MAX;

	// ---- y ----
	FLOAT ymin = 0;
	FLOAT ymax = 1;
	if ( zone == AOI_FOCUS )
	{
		ymin = pi_obj_pano->focus_aoi_min * hby2;
		ymax = pi_obj_pano->focus_aoi_max * hby2;
	}
	else
	{
		ymin = pi_obj_pano->exposure_aoi_min * hby2;
		ymax = pi_obj_pano->exposure_aoi_max * hby2;
	}
	FLOAT e_val = VS_PEA_TRI_E_VAL * (FLOAT)hby2;


	// ---- init "rib" y ----
	FLOAT y[2][2] =
	{
		{ ymin - e_val, ymin },
		{ ymax, ymax + e_val }
	};


	// ---- local param ----
	UINT  num_vertex = (UINT)((FLOAT)VS_PEA_TRI_NUM_EDGE * sector_angle / 360.0f);
	FLOAT ang_inc = sector_angle / (FLOAT)num_vertex;
	UINT  num_vertex_per_rib = num_vertex + 1;


	// ---- store ribs ----
	FLOAT *f = context->GetVertexBufferPtr();
	UINT vertex_size = context->GetVertexSize();
	float zed = 0.75;
	float rhw = 1;
	float count = 0;
	for (UINT ic = 0; ic < VS_PEA_TRI_NUM_CIRC; ic++)
	{
		for (UINT iv = 0; iv < num_vertex_per_rib; iv++)
		{
			for (UINT iq = 0; iq < 2; iq++)
			{
				FLOAT theta = ( (FLOAT)iv * ang_inc + sector_min_angle) * (FLOAT)VS_RADIANS_PER_DEGREE;
				FLOAT rx = -sin(theta) * y[ic][iq] + cx;
				FLOAT ry = cos(theta) * y[ic][iq] + cy;
				*(f) = rx;
				*(f + 1) = ry;
				*(f + 2) = zed;
				*(f + 3) = rhw;
				*(f + 4) = ic == 0 ? min.f : max.f;
				f += vertex_size;
				count++;
			}
		}
	}


	// ---- store bars ( +8 vertices ) ----
	FLOAT circum = 2 * 3.14159265f * ymax;
	FLOAT bar_rot = e_val / circum * 360.0f;
	FLOAT bar_ang[] =
	{
		sector_min_angle,
		sector_min_angle + bar_rot,
		sector_min_angle + sector_angle - bar_rot,
		sector_min_angle + sector_angle
	};
	for (UINT ib = 0; ib < 4; ib++)
	{
		FLOAT y[2] = { ymin - e_val / 2, ymax + e_val / 2 };
		for (UINT ic = 0; ic < VS_PEA_TRI_NUM_CIRC; ic++)
		{
			FLOAT theta = bar_ang[ib] * (FLOAT)VS_RADIANS_PER_DEGREE;
			*(f)     = -sin(theta) * y[ic] + cx;
			*(f + 1) =  cos(theta) * y[ic] + cy;
			*(f + 2) = zed;
			*(f + 3) = rhw;
			*(f + 4) = ic == 0 ? min.f : max.f;
			f += vertex_size;
		}
	}


	// ---- cheat; we need this later ----
	context->SetVertexCount( num_vertex_per_rib );

	return VS_OK;
}


INT ObjConfig::Callback_PanoExposureAec_T_GetIndexBuffer(vs_system::ElmContext *context)
{

	switch ( context->GetId() )
	{

		case VS_OBJ_PANO_ANNULAR_FOCUS_AOI:
			{
				INT result = PanoExposureAec_T_GetSectorIndexBuffer(context);
			}
			break;
		
		default:

			switch (pi_obj_pano->exposure_aoi_zone)
			{
				case AOI_ALL:
				case AOI_ANNULAR:
					{
						#ifdef VS_PEA_RECT_MODE
						INT result = PanoExposureAec_T_GetRectIndexBuffer(context);
						#else
						INT result = PanoExposureAec_T_GetAnnularIndexBuffer(context);
						#endif
					}
					break;
				case AOI_TOP:
				case AOI_LEFT:
				case AOI_BOTTOM:
				case AOI_RIGHT:
				case AOI_FOCUS:
					{
						#ifdef VS_PEA_RECT_MODE
						INT result = PanoExposureAec_T_GetRectIndexBuffer(context);
						#else
						INT result = PanoExposureAec_T_GetSectorIndexBuffer(context);
						#endif
					}
					break;
			}
	}

	return VS_OK;
}


INT ObjConfig::PanoExposureAec_T_GetAnnularIndexBuffer(
		vs_system::ElmContext *context
	)
{

	context->SetIndexCount( VS_PEA_TRI_NUM_INDX );
	context->SetVertexCount( VS_PEA_TRI_NUM_VRTX );
	context->SetPrimitiveCount( VS_PEA_TRI_NUM_PRIM );


	// ---- buffer
		#if VS_INDEX_BUFFER_FORMAT == 32
		DWORD *i = context->GetIndexBufferPtr();
		#else
		WORD *i = context->GetIndexBufferPtr();
		#endif


	UINT ib = 0;
	for (UINT ic = 0; ic < VS_PEA_TRI_NUM_CIRC; ic++)
	{
		for (UINT ie = 0; ie < VS_PEA_TRI_NUM_EDGE; ie++)
		{
			UINT iq  = ib + ie * 2;
			*(i)     = iq;
			*(i + 1) = iq + 1;
			*(i + 2) = iq + 3;
			i += 3;
			*(i)     = iq;
			*(i + 1) = iq + 3;
			*(i + 2) = iq + 2;
			i += 3;
		}
		ib = ( VS_PEA_TRI_NUM_EDGE + 1 ) * 2;
	}

	return VS_OK;
}


INT ObjConfig::PanoExposureAec_T_GetRectIndexBuffer(
		vs_system::ElmContext *context
	)
{

	context->SetIndexCount(24);
	context->SetVertexCount(8);
	context->SetPrimitiveCount(8);


	// ---- buffer
		#if VS_INDEX_BUFFER_FORMAT == 32
		DWORD *i = context->GetIndexBufferPtr();
		#else
		WORD *i = context->GetIndexBufferPtr();
		#endif


	WORD indices[] =
	{
		0, 4, 5,
		0, 5, 1,
		1, 5, 6,
		1, 6, 2,
		2, 6, 7,
		2, 7, 3,
		3, 7, 4,
		3, 4, 0
	};

	VOID *pVoid = (VOID*)i;
	memcpy(pVoid, indices, sizeof(indices));

	return VS_OK;
}


INT ObjConfig::PanoExposureAec_T_GetSectorIndexBuffer(
		vs_system::ElmContext *context
	)
{

	UINT num_vertex_per_rib = context->GetVertexCount();
	UINT sector_num_index   = (num_vertex_per_rib-1) * 6 * VS_PEA_TRI_NUM_CIRC + 12;
	UINT sector_num_vertex  = num_vertex_per_rib     * 2 * VS_PEA_TRI_NUM_CIRC + 8;
	UINT sector_num_prim    = (num_vertex_per_rib-1) * 2 * VS_PEA_TRI_NUM_CIRC + 4;


	context->SetIndexCount(sector_num_index);
	context->SetVertexCount(sector_num_vertex);
	context->SetPrimitiveCount(sector_num_prim);


	// ---- buffer
		#if VS_INDEX_BUFFER_FORMAT == 32
		DWORD *i = context->GetIndexBufferPtr();
		#else
		WORD *i = context->GetIndexBufferPtr();
		#endif


	UINT ib = 0;
	UINT num_vertex = num_vertex_per_rib-1;
	for (UINT ic = 0; ic < VS_PEA_TRI_NUM_CIRC; ic++)
	{
		for (UINT ie = 0; ie < num_vertex; ie++)
		{
			UINT iq  = ib + ie * 2;
			*(i)     = iq;
			*(i + 1) = iq + 1;
			*(i + 2) = iq + 3;
			i += 3;
			*(i)     = iq;
			*(i + 1) = iq + 3;
			*(i + 2) = iq + 2;
			i += 3;
		}
		ib = num_vertex_per_rib * 2;
	}


	WORD indices[] =
	{
		0, 1, 3,
		0, 3, 2,
		4, 5, 7,
		4, 7, 6
	};
	ib = num_vertex_per_rib * 4;
	for (UINT ii = 0; ii < 12; ii++)
	{
		*(i++) = indices[ii] + ib;
	}

	return VS_OK;
}



