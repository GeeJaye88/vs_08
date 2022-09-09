////////////////////////////////////////////////////////////////////////////////

// ---------- vs_obj_dashboard.hpp ----------
/*!
\file vs_obj_dashboard.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// *****************************************************************************


// ---------- Dbz_SetDefault ----------
INT ObjConfig::Dbz_SetDefault(
		FLOAT p[]
	)
{

	p[DBZ_CONFIG]       = 0.0f;
	p[DBZ_VISIBLE]      = 1.0f;
	p[DBZ_ART]          = DBZ_ART_CENTRE;
	p[DBZ_SCALE_X]      = 1.0f;
	p[DBZ_SCALE_Y]      = 1.0f;
	p[DBZ_SCALE_Z]      = 1.0f;
	p[DBZ_ROTATE_X]     = 0.0f;
	p[DBZ_ROTATE_Y]     = 0.0f;
	p[DBZ_ROTATE_Z]     = 0.0f;
	p[DBZ_TRANSLATE_X]  = 0.0f;
	p[DBZ_TRANSLATE_Y]  = 0.0f;
	p[DBZ_TRANSLATE_Z]  = 0.0f;

	return VS_OK;
}


// ---------- Dbz_SetArt ----------
INT ObjConfig::Dbz_SetArt(
		FLOAT param[],
		FLOAT art
	)
{
	param[DBZ_ART] = art;
	return VS_OK;
}


// ---------- Dbz_SetConfig ----------
INT ObjConfig::Dbz_SetConfig(
		FLOAT param[],
		FLOAT config
	)
{
	param[DBZ_CONFIG] = config;
	return VS_OK;
}


// ---------- Dbz_SetVisible ----------
INT ObjConfig::Dbz_SetVisible(
		FLOAT param[],
		FLOAT visible
	)
{
	param[DBZ_VISIBLE] = visible;
	return VS_OK;
}


// ---------- Dbz_SetScale ----------
INT ObjConfig::Dbz_SetScale(
		FLOAT param[],
		FLOAT x,
		FLOAT y
	)
{
	param[DBZ_SCALE_X] = x;
	param[DBZ_SCALE_Y] = y;
	return VS_OK;
}


// ---------- Dbz_SetRotate ----------
INT ObjConfig::Dbz_SetRotate(
		FLOAT param[],
		FLOAT rotation
	)
{
	param[DBZ_ROTATE_Z] = rotation;
	return VS_OK;
}


// ---------- Dbz_SetTranslate ----------
INT ObjConfig::Dbz_SetTranslate(
		FLOAT param[],
		FLOAT x,
		FLOAT y
	)
{
	param[DBZ_TRANSLATE_X] = x;
	param[DBZ_TRANSLATE_Y] = y;
	return VS_OK;
}


// *****************************************************************************


// ---------- Dbz_DialData ----------
struct Dbz_DialData
{

	// ---- base param ----
	UINT  _max_circ_seg = 120;
	UINT  _max_blips = 61;

	// ---- allocate for 60 x blips ----
	UINT  _num_blips_indx = 6 * _max_blips;
	UINT  _num_blips_vrtx = 4 * _max_blips;
	UINT  _num_blips_prim = 2 * _max_blips;

	// ---- allocate 2 x edge ----
	UINT  _num_edge_indx =   _max_circ_seg       * 6 * 2;
	UINT  _num_edge_vrtx = ( _max_circ_seg + 1 ) * 6 * 2;
	UINT  _num_edge_prim =   _max_circ_seg       * 2 * 2;

	// ---- allocate 1 x shape ----
	UINT  _num_shape_indx =   _max_circ_seg       * 6;
	UINT  _num_shape_vrtx = ( _max_circ_seg + 1 ) * 6;
	UINT  _num_shape_prim =   _max_circ_seg       * 2;

	// ----set in GetVertexBuffer ----
	UINT  _num_vertex_per_edge = 0;
	UINT  _num_blips      = 2;
	BOOL  _edge_min_flag = TRUE;
	BOOL  _edge_max_flag = TRUE;
	BOOL  _blips_flag    = TRUE;
	BOOL  _shape_flag    = TRUE;


} dashboard_dial_data;


// ---------- Dbz_Callback_Configure_Dial ----------
INT ObjConfig::Dbz_Callback_Configure_Dial(vs_system::ElmContext *context)
{

	context->BeginConfig();
	{

		// ---- param data ----
		FLOAT *pd = context->GetFloatParameter();
		BOOL   visible = (BOOL)((INT)pd[DBZ_VISIBLE]);


		// ---- atlas page ----
		context->SetAtlasPage(0, 0);


		// ---- IF overide THEN --
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(false);
			context->SetColourVertex(true);
			context->SetMultiState(false);
			context->SetSolid(true);
			context->SetTextured(FALSE);
			context->SetVisible(visible);
			context->SetCullMode(vs_system::ElmContext::CULL_NONE);
		}

		
		// ---- note: allocate max for index and vertex buffers ----


		// ---- index buffer ----
		context->SetIndexCount(
					dashboard_dial_data._num_blips_indx + 
					dashboard_dial_data._num_edge_indx +
					dashboard_dial_data._num_shape_indx
				);
		context->SetIndexBufferCallback(&Dbz_Callback_GetIndexBuffer_Dial);


		// ---- vertex buffer ----
		context->SetVertexCount(
					dashboard_dial_data._num_blips_vrtx +
					dashboard_dial_data._num_edge_vrtx +
					dashboard_dial_data._num_shape_vrtx
				);
		context->SetVertexFormat((UINT)(VS_XYZRHW | VS_DIFFUSE));
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Dbz_Callback_GetVertexBuffer_Dial);


		// ---- primitive parameters ----
		context->SetPrimitiveBytes(
					context->GetVertexSize() *
					context->GetVertexCount() *
					sizeof(float)
				);
		context->SetPrimitiveCount( 
					dashboard_dial_data._num_blips_prim +
					dashboard_dial_data._num_edge_prim +
					dashboard_dial_data._num_shape_prim
				);
		context->SetPrimitiveType(VS_TRIANGLELIST);

	}
	context->EndConfig();


	return VS_OK;
}


// ---------- Dbz_Callback_GetVertexBuffer_Dial ----------
INT ObjConfig::Dbz_Callback_GetVertexBuffer_Dial(vs_system::ElmContext *context)
{

	// ---- supplied object param ----
	FLOAT *pd = context->GetFloatParameter();

	FLOAT  pd_config_bitmask  = pd[DBZ_CONFIG];
	FLOAT  pd_visible         = pd[DBZ_VISIBLE];
	FLOAT  pd_art             = pd[DBZ_ART];
	FLOAT  pd_translate_x     = pd[DBZ_TRANSLATE_X];
	FLOAT  pd_translate_y     = pd[DBZ_TRANSLATE_Y];
	FLOAT  pd_scale_x         = pd[DBZ_SCALE_X];
	FLOAT  pd_scale_y         = pd[DBZ_SCALE_Y];
	FLOAT  pd_rotation        = pd[DBZ_ROTATE_Z];

	FLOAT  pd_min_radius      = pd[DBZ_DIAL_RADIUS_MIN];
	FLOAT  pd_max_radius      = pd[DBZ_DIAL_RADIUS_MAX];
	FLOAT  pd_min_angle       = pd[DBZ_DIAL_ANGLE_MIN];
	FLOAT  pd_max_angle       = pd[DBZ_DIAL_ANGLE_MAX];

	FLOAT  pd_edge_size       = pd[DBZ_DIAL_EDGE_SIZE];
	FLOAT  pd_edge_red_max    = pd[DBZ_DIAL_EDGE_RED_MAX];
	FLOAT  pd_edge_green_max  = pd[DBZ_DIAL_EDGE_GREEN_MAX];
	FLOAT  pd_edge_blue_max   = pd[DBZ_DIAL_EDGE_BLUE_MAX];
	FLOAT  pd_edge_red_min    = pd[DBZ_DIAL_EDGE_RED_MIN];
	FLOAT  pd_edge_green_min  = pd[DBZ_DIAL_EDGE_GREEN_MIN];
	FLOAT  pd_edge_blue_min   = pd[DBZ_DIAL_EDGE_BLUE_MIN];

	FLOAT  pd_shape_red_max   = pd[DBZ_DIAL_SHAPE_RED_MAX];
	FLOAT  pd_shape_green_max = pd[DBZ_DIAL_SHAPE_GREEN_MAX];
	FLOAT  pd_shape_blue_max  = pd[DBZ_DIAL_SHAPE_BLUE_MAX];
	FLOAT  pd_shape_red_min   = pd[DBZ_DIAL_SHAPE_RED_MIN];
	FLOAT  pd_shape_green_min = pd[DBZ_DIAL_SHAPE_GREEN_MIN];
	FLOAT  pd_shape_blue_min  = pd[DBZ_DIAL_SHAPE_BLUE_MIN];

	FLOAT  pd_blip_count      = pd[DBZ_DIAL_BLIP];
	FLOAT  pd_blip_edge_size  = pd[DBZ_DIAL_BLIP_EDGE_SIZE];


	// ---- derived sector param ----
	FLOAT sector_rotate = pd_rotation  + pd_min_angle + 180.0f;
	FLOAT sector_angle  = pd_max_angle - pd_min_angle;


	// ---- calculate # vertex proportional to sector angle ----
	UINT num_edge = (UINT)((FLOAT)dashboard_dial_data._max_circ_seg * sector_angle / 360.0f);
		num_edge = num_edge >  dashboard_dial_data._max_circ_seg ?
					dashboard_dial_data._max_circ_seg :
						num_edge;
	UINT num_vertex_per_edge = num_edge + 1;


	// ---- set # blips ----
	UINT num_blips = (UINT)pd_blip_count;
		num_blips = num_blips >  dashboard_dial_data._max_blips ?
						dashboard_dial_data._max_blips :
							num_blips;


	// ---- we need these later in GetIndex----
	dashboard_dial_data._num_vertex_per_edge = num_vertex_per_edge;
	dashboard_dial_data._num_blips = num_blips;


	// ---- derived logical component flags ----
	UINT cb = (INT)pd_config_bitmask;
	BOOL edge_min_flag = ( cb & DBZ_DIAL_CONFIG_BITMAP_EDGE_MIN ) ? 1 : 0;
	BOOL edge_max_flag = ( cb & DBZ_DIAL_CONFIG_BITMAP_EDGE_MAX ) ? 1 : 0;
	BOOL blips_flag    = ( cb & DBZ_DIAL_CONFIG_BITMAP_BARS     ) ? 1 : 0;
	BOOL shape_flag    = ( cb & DBZ_DIAL_CONFIG_BITMAP_SHAPE    ) ? 1 : 0;


	// ---- we need these later in GetIndex---
	dashboard_dial_data._edge_min_flag = edge_min_flag;
	dashboard_dial_data._edge_max_flag = edge_max_flag;
	dashboard_dial_data._blips_flag    = blips_flag;
	dashboard_dial_data._shape_flag    = shape_flag;


	// ---- supplied panel param ----
	FLOAT *sp    = context->GetGfxFloatParameter();
	FLOAT  left   = sp[VS_PANEL_LEFT];
	FLOAT  top    = sp[VS_PANEL_TOP];
	FLOAT  right  = sp[VS_PANEL_RIGHT];
	FLOAT  bottom = sp[VS_PANEL_BOTTOM];


	// ---- derived panel param ----
	FLOAT width  = right - left - 1;
	FLOAT height = bottom - top - 1;
	FLOAT hby2   = height / 2;
	FLOAT wby2   = width  / 2;
	FLOAT cx     = width  / 2 + left;
	FLOAT cy     = height / 2 + top;


	// ---- vertex ----
	struct XYZWD { float x, y, z, w; DWORD diffuse; };
	union colour_union_t {
		DWORD _d;
		FLOAT _f;
	} diffuse;


	// ---- y ----
	FLOAT ymin     = pd_min_radius     * hby2 * pd_scale_y;
	FLOAT ymax     = pd_max_radius     * hby2 * pd_scale_y;
	FLOAT edge_val = pd_edge_size      * hby2 * pd_scale_y;
	FLOAT blip_val = pd_blip_edge_size * hby2 * pd_scale_y;;


	// ---- translate ----
	FLOAT translate_x = hby2 * pd_translate_x;
	FLOAT translate_y = hby2 * pd_translate_y;


	// ---- get vertex stuff ----
	FLOAT *f = context->GetVertexBufferPtr();
	UINT  vertex_size = context->GetVertexSize();
	FLOAT vertex_zed = 0.75f;


	// ---- shape ----
	FLOAT ang_inc = sector_angle / (FLOAT)num_edge;
	if ( shape_flag )
	{

		FLOAT shape_y[] =
		{
			ymin,
			ymax
		};

		for (UINT iv = 0; iv < num_vertex_per_edge; iv++)
		{

			FLOAT a = ((FLOAT)iv * ang_inc + pd_min_angle) / pd_max_angle;
			diffuse._d = VS_D3DCOLOR_COLORVALUE(
					(1 - a) * pd_shape_red_min   + a * pd_shape_red_max,
					(1 - a) * pd_shape_green_min + a * pd_shape_green_max,
					(1 - a) * pd_shape_blue_min  + a * pd_shape_blue_max,
					1.0f
				);
			
			FLOAT theta = ((FLOAT)iv * ang_inc + sector_rotate) * (FLOAT)VS_RADIANS_PER_DEGREE;
			for (UINT iq = 0; iq < 2; iq++)
			{
				FLOAT rx = -sin(theta) * shape_y[iq] + cx;
				FLOAT ry = cos(theta) * shape_y[iq] + cy;
				*(f    ) = rx + translate_x;
				*(f + 1) = ry + translate_y;
				*(f + 2) = vertex_zed;
				*(f + 3) = 1;
				*(f + 4) = diffuse._f;
				f += vertex_size;
			}
		}
	}


	// ---- edges ----

	FLOAT y[2][2] =
	{
		{ ymin, ymin + edge_val },
		{ ymax - edge_val, ymax }
	};

	BOOL edge_flag[] = { edge_min_flag, edge_max_flag };
	for (UINT ic = 0; ic < 2; ic++)
	{
		if ( edge_flag[ic] )
		{
			for (UINT iv = 0; iv < num_vertex_per_edge; iv++)
			{

				FLOAT a = ((FLOAT)iv * ang_inc + pd_min_angle) / pd_max_angle;
				diffuse._d = VS_D3DCOLOR_COLORVALUE(
						(1 - a) * pd_edge_red_min   + a * pd_edge_red_max,
						(1 - a) * pd_edge_green_min + a * pd_edge_green_max,
						(1 - a) * pd_edge_blue_min  + a * pd_edge_blue_max,
						1.0f
					);
			
				FLOAT theta = ((FLOAT)iv * ang_inc + sector_rotate) * (FLOAT)VS_RADIANS_PER_DEGREE;
				for (UINT iq = 0; iq < 2; iq++)
				{
					FLOAT rx = -sin(theta) * y[ic][iq] + cx;
					FLOAT ry =  cos(theta) * y[ic][iq] + cy;
					*(f    ) = rx + translate_x;
					*(f + 1) = ry + translate_y;
					*(f + 2) = vertex_zed - 0.05f;
					*(f + 3) = 1;
					*(f + 4) = diffuse._f;
					f += vertex_size;
				}
			}
		}
	}


	// ---- blips ----
	if ( blips_flag )
	{

		FLOAT circum = 2 * 3.14159265f * ymax;
		FLOAT edge_width_ang = edge_val / circum * 360.0f;
		FLOAT blips_width_ang = blip_val / circum * 360.0f;

		FLOAT blip_inc_angle = 0;
		FLOAT blip_inc_width = 0;
		if ( num_blips == 1 )
		{
			blip_inc_angle = sector_angle - edge_width_ang;
			blip_inc_width = edge_width_ang;
		}
		else
		{
			blip_inc_angle = (sector_angle - blips_width_ang) / (FLOAT)num_blips;
			blip_inc_width = blips_width_ang;
		}

		BOOL blip_end_edge_rgb_flag = num_blips > 1 && edge_min_flag || edge_max_flag;

		for (UINT blip = 0; blip <= num_blips; blip++)
		{

			FLOAT blip_ang[2] =
			{
				blip * blip_inc_angle,
				blip * blip_inc_angle + blip_inc_width,
			};

			FLOAT blip_y[2] =
			{
				ymin,
				ymax
			};

			for (UINT ib = 0; ib < 2; ib++)
			{

				FLOAT a = (blip_ang[ib] + pd_min_angle) / pd_max_angle;

				if ( num_blips == 1 )
				{
					diffuse._d = VS_D3DCOLOR_COLORVALUE(
							(1 - a) * pd_edge_red_min   + a * pd_edge_red_max,
							(1 - a) * pd_edge_green_min + a * pd_edge_green_max,
							(1 - a) * pd_edge_blue_min  + a * pd_edge_blue_max,
							1.0f
						);
				}
				else
				{
					diffuse._d = VS_D3DCOLOR_COLORVALUE(
							(1 - a) * pd_shape_red_min   + a * pd_shape_red_max,
							(1 - a) * pd_shape_green_min + a * pd_shape_green_max,
							(1 - a) * pd_shape_blue_min  + a * pd_shape_blue_max,
							1.0f
						);
				}

				FLOAT theta = (blip_ang[ib] + sector_rotate) * (FLOAT)VS_RADIANS_PER_DEGREE;

				for (UINT ic = 0; ic < 2; ic++)
				{
					*(f    ) = -sin(theta) * blip_y[ic] + cx + translate_x;
					*(f + 1) =  cos(theta) * blip_y[ic] + cy + translate_y;
					*(f + 2) = vertex_zed;
					*(f + 3) = 1;
					*(f + 4) = diffuse._f;
					f += vertex_size;
				}
			}
		}
	}

	return VS_OK;
}


// ---------- Dbz_Callback_GetIndexBuffer_Dial ----------
INT ObjConfig::Dbz_Callback_GetIndexBuffer_Dial(
		vs_system::ElmContext *context
	)
{

	// ---- logical component flags - set in GetVertex ---
	BOOL edge_min_flag = dashboard_dial_data._edge_min_flag;
	BOOL edge_max_flag = dashboard_dial_data._edge_max_flag;
	BOOL blips_flag    = dashboard_dial_data._blips_flag;
	BOOL shape_flag    = dashboard_dial_data._shape_flag;


	// ---- calculated in GetVertex----
	UINT num_vertex_per_edge = dashboard_dial_data._num_vertex_per_edge;
	UINT num_quad = num_vertex_per_edge - 1;
	UINT num_blips = dashboard_dial_data._num_blips;


	// ---- set # component ----
	UINT shape_mult = shape_flag == TRUE ? 1 : 0;
	UINT edge_mult = edge_min_flag == TRUE ? 1 : 0;
			edge_mult += edge_max_flag == TRUE ? 1 : 0;
	UINT blips_mult = blips_flag == TRUE ? 1 : 0;


	// ---- calculate component param ----
	UINT sector_num_index =
		(num_vertex_per_edge - 1) * 6 * shape_mult +
			(num_vertex_per_edge - 1) * 6 * edge_mult +
				(dashboard_dial_data._num_blips+1) * 6 * blips_mult;

	UINT sector_num_vertex =
		num_vertex_per_edge * 2 * shape_mult +
			num_vertex_per_edge * 2 * edge_mult +
				(dashboard_dial_data._num_blips + 1) * 4 * blips_mult;

	UINT sector_num_prim =
		(num_vertex_per_edge - 1) * 2 * shape_mult +
			(num_vertex_per_edge - 1) * 2 * edge_mult +
				(dashboard_dial_data._num_blips + 1) * 2 * blips_mult;


	// ---- set ----
	context->SetIndexCount(sector_num_index);
	context->SetVertexCount(sector_num_vertex);
	context->SetPrimitiveCount(sector_num_prim);


	// ---- buffer
		#if VS_INDEX_BUFFER_FORMAT == 32
		DWORD *i = context->GetIndexBufferPtr();
		#else
		WORD *i = context->GetIndexBufferPtr();
		#endif


	// ---- shape ----
	if ( shape_flag)
	{
		for (UINT ie = 0; ie < num_quad; ie++)
		{
			UINT iq = ie * 2;
			*(i) = iq;
				*(i + 1) = iq + 1;
				*(i + 2) = iq + 3;
				i += 3;
			*(i) = iq;
				*(i + 1) = iq + 3;
				*(i + 2) = iq + 2;
				i += 3;
		}
	}


	// ---- edges ----
	BOOL edge_flag[] = { edge_min_flag, edge_max_flag };
	UINT i_offset = num_vertex_per_edge * 2 * edge_mult;
	for (UINT ic = 0; ic < 2; ic++)
	{
		if ( edge_flag[ic] )
		{
			for (UINT ie = 0; ie < num_quad; ie++)
			{
				UINT iq = i_offset + ie * 2;
				*(i) = iq;
					*(i + 1) = iq + 1;
					*(i + 2) = iq + 3;
					i += 3;
				*(i) = iq;
					*(i + 1) = iq + 3;
					*(i + 2) = iq + 2;
					i += 3;
			}
			i_offset += num_vertex_per_edge * 2;
		}
	}


	// ---- blips ----
	if ( blips_flag )
	{
		i_offset = num_vertex_per_edge * 2 * shape_mult +
			num_vertex_per_edge * 2 * edge_mult;

		WORD indices[] =
		{
			0, 1, 3,
			0, 3, 2
		};
		for (UINT blip = 0; blip <=num_blips; blip++)
		{
			for (UINT ib = 0; ib < 6; ib++)
			{
				*(i++) = i_offset + indices[ib];
			}
			i_offset += 4;
		}
	}

	return VS_OK;
}


// ---------- Dbz_SetDialDefault ----------
INT ObjConfig::Dbz_SetDialDefault(
		FLOAT pd[]
	)
{

	Dbz_SetDefault(pd);

	pd[DBZ_CONFIG] = (FLOAT)
		(
			vs_system::ObjConfig::DBZ_DIAL_CONFIG_BITMAP_EDGE_MAX |
			vs_system::ObjConfig::DBZ_DIAL_CONFIG_BITMAP_EDGE_MIN |
			vs_system::ObjConfig::DBZ_DIAL_CONFIG_BITMAP_BARS |
			vs_system::ObjConfig::DBZ_DIAL_CONFIG_BITMAP_SHAPE
		);

	pd[DBZ_DIAL_RADIUS_MIN]      = 0.8f;
	pd[DBZ_DIAL_RADIUS_MAX]      = 1.0f;
	pd[DBZ_DIAL_ANGLE_MIN]       = 0.0f;
	pd[DBZ_DIAL_ANGLE_MAX]       = 270;
	pd[DBZ_DIAL_EDGE_SIZE]       = 0.05f;
	pd[DBZ_DIAL_EDGE_RED_MAX]    = 0.8f;
	pd[DBZ_DIAL_EDGE_GREEN_MAX]  = 0.8f;
	pd[DBZ_DIAL_EDGE_BLUE_MAX]   = 0.8f;
	pd[DBZ_DIAL_EDGE_RED_MIN]    = 0.4f;
	pd[DBZ_DIAL_EDGE_GREEN_MIN]  = 0.4f;
	pd[DBZ_DIAL_EDGE_BLUE_MIN]   = 0.4f;
	pd[DBZ_DIAL_SHAPE_RED_MAX]   = 0.4f;
	pd[DBZ_DIAL_SHAPE_GREEN_MAX] = 0.6f;
	pd[DBZ_DIAL_SHAPE_BLUE_MAX]  = 0.8f;
	pd[DBZ_DIAL_SHAPE_RED_MIN]   = 0.2f;
	pd[DBZ_DIAL_SHAPE_GREEN_MIN] = 0.3f;
	pd[DBZ_DIAL_SHAPE_BLUE_MIN]  = 0.4f;
	pd[DBZ_DIAL_BLIP]            = 1.0f;
	pd[DBZ_DIAL_BLIP_EDGE_SIZE]  = 0.05f;
	pd[DBZ_DIAL_BLIP_RED_MAX]    = 0.8f;
	pd[DBZ_DIAL_BLIP_GREEN_MAX]  = 0.8f;
	pd[DBZ_DIAL_BLIP_BLUE_MAX]   = 0.8f;
	pd[DBZ_DIAL_BLIP_RED_MIN]    = 0.4f;
	pd[DBZ_DIAL_BLIP_GREEN_MIN]  = 0.4f;
	pd[DBZ_DIAL_BLIP_BLUE_MIN]   = 0.4f;

	return VS_OK;
}


// ---------- Dbz_SetDialAngle ----------
INT ObjConfig::Dbz_SetDialAngle(
		FLOAT pd[],
		FLOAT min_angle,
		FLOAT max_angle
	)
{
	pd[DBZ_DIAL_ANGLE_MIN] = min_angle;
	pd[DBZ_DIAL_ANGLE_MAX] = max_angle;
	return VS_OK;
}


// ---------- Dbz_SetDialRadii ----------
INT ObjConfig::Dbz_SetDialRadii(
	FLOAT pd[],
		FLOAT min_radius,
		FLOAT max_radius
	)
{
	pd[DBZ_DIAL_RADIUS_MIN] = min_radius;
	pd[DBZ_DIAL_RADIUS_MAX] = max_radius;
	return VS_OK;
}


// ---------- Dbz_SetDialBlipCount ----------
INT ObjConfig::Dbz_SetDialBlipCount(
	FLOAT pd[],
		FLOAT blip_count
	)
{

	pd[DBZ_DIAL_BLIP] =
		blip_count = blip_count > dashboard_dial_data._max_blips ?
			dashboard_dial_data._max_blips :
				blip_count;

	return VS_OK;
}


// ---------- Dbz_SetDialBlipEdgeSize ----------
INT ObjConfig::Dbz_SetDialBlipEdgeSize(
	FLOAT pd[],
		FLOAT blip_edge_size
	)
{

	pd[DBZ_DIAL_BLIP_EDGE_SIZE] =
		blip_edge_size = blip_edge_size > 0.001 ?
			blip_edge_size :
				0.001f;

	return VS_OK;
}


// ---------- Dbz_SetDialEdgeSize ----------
INT ObjConfig::Dbz_SetDialEdgeSize(
	FLOAT pd[],
		FLOAT edge_val
	)
{
	pd[DBZ_DIAL_EDGE_SIZE] = edge_val;
	return VS_OK;
}


// ---------- Dbz_SetDialEdgeRGBMin ----------
INT ObjConfig::Dbz_SetDialEdgeRGBMin(
	FLOAT pd[],
		FLOAT red,
		FLOAT green,
		FLOAT blue
	)
{
	pd[DBZ_DIAL_EDGE_RED_MIN]   = red;
	pd[DBZ_DIAL_EDGE_GREEN_MIN] = green;
	pd[DBZ_DIAL_EDGE_BLUE_MIN]  = blue;
	return VS_OK;
}


// ---------- Dbz_SetDialEdgeRGBMax ----------
INT ObjConfig::Dbz_SetDialEdgeRGBMax(
		FLOAT pd[],
		FLOAT red,
		FLOAT green,
		FLOAT blue
	)
{
	pd[DBZ_DIAL_EDGE_RED_MAX]   = red;
	pd[DBZ_DIAL_EDGE_GREEN_MAX] = green;
	pd[DBZ_DIAL_EDGE_BLUE_MAX]  = blue;
	return VS_OK;
}


// ---------- Dbz_SetDialShapeRGBMin ----------
INT ObjConfig::Dbz_SetDialShapeRGBMin(
		FLOAT pd[],
		FLOAT red,
		FLOAT green,
		FLOAT blue
	)
{
	pd[DBZ_DIAL_SHAPE_RED_MIN]   = red;
	pd[DBZ_DIAL_SHAPE_GREEN_MIN] = green;
	pd[DBZ_DIAL_SHAPE_BLUE_MIN]  = blue;
	return VS_OK;
}


// ---------- Dbz_SetDialShapeRGBMax ----------
INT ObjConfig::Dbz_SetDialShapeRGBMax(
		FLOAT pd[],
		FLOAT red,
		FLOAT green,
		FLOAT blue
	)
{
	pd[DBZ_DIAL_SHAPE_RED_MAX]   = red;
	pd[DBZ_DIAL_SHAPE_GREEN_MAX] = green;
	pd[DBZ_DIAL_SHAPE_BLUE_MAX]  = blue;
	return VS_OK;
}


// *****************************************************************************


// ---------- Dbz_Callback_Configure_Icon ----------
INT ObjConfig::Dbz_Callback_Configure_Icon(vs_system::ElmContext *context)
{

	context->BeginConfig();
	{

		// ---- param data ----
		FLOAT *pd = context->GetFloatParameter();
		BOOL   visible = (BOOL)((INT)pd[DBZ_VISIBLE]);


		// ---- atlas page ----
		// note: for standard texture - use - 
		//    context->SetAtlasPage(0, 101);
		context->SetAtlasPages(VS_ATLAS_PAGE_GROUP_ONE);


		// ---- IF overide THEN --
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(true);  //  note: for standard texture (false)
			context->SetColourVertex(false);
			context->SetMultiState(true);     //  note: for standard texture (false)
			context->SetSolid(true);
			context->SetTextured(TRUE);
			context->SetVisible(visible);
			context->SetCullMode(vs_system::ElmContext::CULL_NONE);
		}


		// ---- index buffer ----
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


		// ---- vertex buffer ----
		context->SetVertexCount(26);
		context->SetVertexFormat( (UINT)(VS_XYZRHW | VS_TEX1) );
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Dbz_Callback_GetVertexBuffer_Icon);


		// ---- primitive parameters ----
		context->SetPrimitiveBytes(
					context->GetVertexSize() *
					context->GetVertexCount() *
					sizeof(float)
				);
		context->SetPrimitiveCount(24);
		context->SetPrimitiveType(VS_TRIANGLEFAN);

	}
	context->EndConfig();


	return VS_OK;
}


// ---------- Dbz_Callback_GetVertexBuffer_Icon ----------
INT ObjConfig::Dbz_Callback_GetVertexBuffer_Icon(vs_system::ElmContext *context)
{

	// ---- local ----
	UINT  vertex_count = context->GetVertexCount();


	// ---- param data ----
	FLOAT *pd = context->GetFloatParameter();
	
	FLOAT  pd_art         = pd[DBZ_ART];
	FLOAT  pd_translate_x = pd[DBZ_TRANSLATE_X];
	FLOAT  pd_translate_y = pd[DBZ_TRANSLATE_Y];
	FLOAT  pd_size_x      = pd[DBZ_SCALE_X];
	FLOAT  pd_size_y      = pd[DBZ_SCALE_Y];

	FLOAT  pd_apg         = pd[DBZ_ICON_APG];
	FLOAT  pd_tex_col     = pd[DBZ_ICON_TEX_ROW];
	FLOAT  pd_tex_row     = pd[DBZ_ICON_TEX_COLUMN];
	FLOAT  pd_tex_width   = pd[DBZ_ICON_TEX_WIDTH];
	FLOAT  pd_tex_height  = pd[DBZ_ICON_TEX_HEIGHT];
	FLOAT  pd_tex_adjust  = pd[DBZ_ICON_TEX_ADJUST];


	// ---- set multi-state "Atlas Page Group" ----
	context->SetAtlasPages((UINT)pd_apg);


	// ---- set "Dimension Proportional To".. ----


	// ---- set tex location ----
	FLOAT texture_left   = pd_tex_col * 0.125f + pd_tex_adjust;
	FLOAT texture_top    = pd_tex_row * 0.125f + pd_tex_adjust;
	FLOAT texture_right  = texture_left + (0.125f * pd_tex_width)  - (pd_tex_adjust * 2);
	FLOAT texture_bottom = texture_top  + (0.125f * pd_tex_height) - (pd_tex_adjust * 2);

	FLOAT texture_cx = ( texture_left + texture_right ) / 2;
	FLOAT texture_cy = ( texture_top + texture_bottom ) / 2;
	FLOAT texture_radius = (0.125f + pd_tex_adjust) / 2;


	// ---- supplied panel param----
	FLOAT *sp    = context->GetGfxFloatParameter();
	FLOAT  left   = sp[VS_PANEL_LEFT];
	FLOAT  top    = sp[VS_PANEL_TOP];
	FLOAT  right  = sp[VS_PANEL_RIGHT];
	FLOAT  bottom = sp[VS_PANEL_BOTTOM];
	FLOAT  width  = right - left;
	FLOAT  height = bottom - top;
	FLOAT  hby2   = height / 2;
	FLOAT  cx     = width  / 2 + left;
	FLOAT  cy     = height / 2 + top;


	// ---- set XY param ----
	FLOAT translate_x = cx + pd_translate_x * hby2;
	FLOAT translate_y = cy + pd_translate_y * hby2;
	FLOAT radius      = pd_size_y * hby2;


	// ---- init and then copy triangle fan data ----
	struct XYZWUV
		{
			FLOAT _x, _y, _z, _w;
			FLOAT _u, _v;
		};
	FLOAT zed = 0.5f;
	FLOAT rhw = 1;

	/*
	XYZWUV v[4] =
	{
		{ -sx + tx + cx, -sy + ty + cy, zed, rhw, texture_left,  texture_top    },
		{  sx + tx + cx, -sy + ty + cy, zed, rhw, texture_right, texture_top    },
		{  sx + tx + cx,  sy + ty + cy, zed, rhw, texture_right, texture_bottom },
		{ -sx + tx + cx,  sy + ty + cy, zed, rhw, texture_left,  texture_bottom },
	};
	memcpy(context->GetVertexBufferPtr(), v, sizeof(v));
	*/

	{
		XYZWUV v[26];

		UINT  num_edge = 24;
		UINT  num_vrtx = num_edge + 1;
		FLOAT ang_inc = 360 / (FLOAT)num_edge;

		v[0] = { translate_x, translate_y, zed, rhw, texture_cx, texture_cy };

		for (UINT iv = 1; iv <= num_vrtx; iv++)
		{			
			FLOAT theta = (FLOAT)(iv-1) * ang_inc * (FLOAT)VS_RADIANS_PER_DEGREE;
			v[iv]._x = -sin(theta) * radius + translate_x;
			v[iv]._y = cos(theta) * radius + translate_y;
			v[iv]._z = zed;
			v[iv]._w = rhw;
			v[iv]._u = -sin(theta) * texture_radius + texture_cx;
			v[iv]._v = cos(theta) * texture_radius + texture_cy;
		}
		memcpy(context->GetVertexBufferPtr(), v, sizeof(v));
	}



	return VS_OK;
}


// ---------- Dbz_SetIconTexture ----------
INT ObjConfig::Dbz_SetIconTexture(
		FLOAT pd[],
		FLOAT apg,
		FLOAT row,
		FLOAT col,
		FLOAT width,
		FLOAT height,
		FLOAT adjust
	)
{
	pd[DBZ_ICON_APG]        = apg;
	pd[DBZ_ICON_TEX_COLUMN] = row;
	pd[DBZ_ICON_TEX_ROW]    = col;
	pd[DBZ_ICON_TEX_WIDTH]  = width;
	pd[DBZ_ICON_TEX_HEIGHT] = height;
	pd[DBZ_ICON_TEX_ADJUST] = adjust;
	return VS_OK;
}


// *****************************************************************************


// ---------- Dbz_Callback_Configure_Text ----------
INT ObjConfig::Dbz_Callback_Configure_Text(vs_system::ElmContext *context)
{

	context->BeginConfig();
	{

		// ---- param data ----
		FLOAT *pd = context->GetFloatParameter();
		BOOL   visible = (BOOL)((INT)pd[DBZ_VISIBLE]);


		// ---- atlas page ----
		context->SetAtlasPage(0, 0);


		// ---- IF overide THEN --
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(false);
			context->SetColourVertex(true);
			context->SetMultiState(false);
			context->SetSolid(false);
			context->SetTextured(FALSE);
			context->SetVisible(visible);
			context->SetCullMode(vs_system::ElmContext::CULL_NONE);
		}


		// ---- index buffer ----
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


		// ---- vertex buffer ----
		context->SetVertexCount(5);
		context->SetVertexFormat((UINT)(VS_XYZRHW | VS_DIFFUSE));
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Dbz_Callback_GetVertexBuffer_Text);


		// ---- primitive parameters ----
		context->SetPrimitiveBytes(
					context->GetVertexSize() *
					context->GetVertexCount() *
					sizeof(float)
				);
		context->SetPrimitiveCount(4);
		context->SetPrimitiveType(VS_LINESTRIP);

	}
	context->EndConfig();


	return VS_OK;
}


// ---------- Dbz_Callback_GetVertexBuffer_Text ----------
INT ObjConfig::Dbz_Callback_GetVertexBuffer_Text(vs_system::ElmContext *context)
{

	// ---- local ----
	UINT  vertex_count = context->GetVertexCount();


	// ---- param data ----
	FLOAT *pd = context->GetFloatParameter();
	FLOAT  pd_config      = pd[DBZ_CONFIG];
	FLOAT  pd_art         = pd[DBZ_ART];
	FLOAT  pd_translate_x = pd[DBZ_TRANSLATE_X];
	FLOAT  pd_translate_y = pd[DBZ_TRANSLATE_Y];
	FLOAT  pd_size_x      = pd[DBZ_SCALE_X];
	FLOAT  pd_size_y      = pd[DBZ_SCALE_Y];


	// ---- supplied panel param----
	FLOAT *sp    = context->GetGfxFloatParameter();
	FLOAT  left   = sp[VS_PANEL_LEFT];
	FLOAT  top    = sp[VS_PANEL_TOP];
	FLOAT  right  = sp[VS_PANEL_RIGHT];
	FLOAT  bottom = sp[VS_PANEL_BOTTOM];
	FLOAT  width  = right - left;
	FLOAT  height = bottom - top;
	FLOAT  hby2   = height / 2;
	FLOAT  cx     = width  / 2 + left;
	FLOAT  cy     = height / 2 + top;


	// ---- aligment relative to ----
	INT   art = (INT)pd_art;
	FLOAT dif = 0;
	switch (art)
	{
		case DBZ_ART_CENTRE:
			break;
		case DBZ_ART_LEFT:
			dif = (height - width) / 2;
			break;
		case DBZ_ART_RIGHT:
			break;
	}


	// ---- set XY param ----
	UINT  config = (UINT)pd_config;
	FLOAT tx = dif + pd_translate_x * hby2 + cx;
	FLOAT ty = pd_translate_y * hby2 + cy;
	FLOAT sx = pd_size_x * hby2;
	FLOAT sy = pd_size_y * hby2;


	// ---- colour ----
	struct XYZWD { FLOAT _x, _y, _z, _w; DWORD _color; };
	FLOAT r, b, g, a;
	context->GetRGBA(&r, &g, &b, &a);


	// ---- fade from bottom to top.... ----
	DWORD color_bottom = VS_D3DCOLOR_COLORVALUE(r*0.5f, b*0.5f, g*0.5f, a*0.5f);
	DWORD color_top = VS_D3DCOLOR_COLORVALUE(r, b, g, a);


	// ---- font ----
	context->SetVersion(vs_system::ElmContext::VERSION_TEXT);
	context->SetTextOutline(FALSE);
	context->SetUseRGBA(TRUE);
	context->SetFontHeight((UINT)(sy + 0.5f));


	// ---- font config ----
	switch ( config )
	{
		default:
			context->SetFontAlignment(vs_system::ElmContext::ALIGN_LEFT);
			break;
		case DBZ_TEXT_CAPTION_ALIGNED_CENTRE:
			context->SetFontName("Dbz_Caption");
			context->SetFontAlignment(vs_system::ElmContext::ALIGN_CENTRED_ON_TEXT);
			break;
		case DBZ_TEXT_CAPTION_ALIGNED_LEFT:
			context->SetFontName("Dbz_Caption");
			context->SetFontAlignment(vs_system::ElmContext::ALIGN_LEFT);
			break;
		case DBZ_TEXT_LABEL_ALIGNED_CENTRE:
			context->SetFontName("Dbz_Label");
			context->SetFontAlignment(vs_system::ElmContext::ALIGN_CENTRED_ON_TEXT);
			break;
		case DBZ_TEXT_LABEL_ALIGNED_LEFT:
			context->SetFontName("Dbz_Label");
			context->SetFontAlignment(vs_system::ElmContext::ALIGN_LEFT);
			break;
		case DBZ_TEXT_NORMAL_ALIGNED_CENTRE:
			context->SetFontName("Dbz_Normal");
			context->SetFontAlignment(vs_system::ElmContext::ALIGN_CENTRED_ON_TEXT);
			break;
		case DBZ_TEXT_NORMAL_ALIGNED_LEFT:
			context->SetFontName("Dbz_Normal");
			context->SetFontAlignment(vs_system::ElmContext::ALIGN_LEFT);
			break;
	}


	// So:-
	//
	//   Complicated. To be aligned centered on text,
	//   the bounds - therefore the elem below - must
	//   have zero width.
	//
	//   TBD
	//

	// ---- init and then copy line strip data ----
	FLOAT zed = 0;
	FLOAT rhw = 1;
	XYZWD elem[] =
	{
		{ -sx + tx, -sy + ty, zed, rhw, color_top    },
		{  sx + tx, -sy + ty, zed, rhw, color_top    },
		{  sx + tx,  sy + ty, zed, rhw, color_bottom },
		{ -sx + tx,  sy + ty, zed, rhw, color_bottom },
		{ -sx + tx, -sy + ty, zed, rhw, color_top    }
	};
	memcpy( context->GetVertexBufferPtr(), elem, sizeof(elem));

	return VS_OK;
}


// *****************************************************************************


// ---------- Dbz_Callback_Configure_Needle ----------
INT ObjConfig::Dbz_Callback_Configure_Needle(vs_system::ElmContext *context)
{

	context->BeginConfig();
	{

		// ---- param data ----
		FLOAT *pd = context->GetFloatParameter();
		BOOL   visible = (BOOL)((INT)pd[DBZ_VISIBLE]);


		// ---- atlas page ----
		context->SetAtlasPage(0, 0);


		// ---- IF overide THEN --
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(false);
			context->SetColourVertex(true);
			context->SetMultiState(false);
			context->SetSolid(true);
			context->SetTextured(FALSE);
			context->SetVisible(visible);
			context->SetCullMode(vs_system::ElmContext::CULL_NONE);
		}


		// ---- index buffer ----
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


		// ---- vertex buffer ----
		context->SetVertexCount(6);
		context->SetVertexFormat((UINT)(VS_XYZRHW | VS_DIFFUSE));
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Dbz_Callback_GetVertexBuffer_Needle);


		// ---- primitive parameters ----
		context->SetPrimitiveBytes(
					context->GetVertexSize() *
					context->GetVertexCount() *
					sizeof(float)
				);
		context->SetPrimitiveCount(4);
		context->SetPrimitiveType(VS_TRIANGLEFAN);

	}
	context->EndConfig();


	return VS_OK;
}


// ---------- Dbz_Callback_GetVertexBuffer_Needle ----------
INT ObjConfig::Dbz_Callback_GetVertexBuffer_Needle(vs_system::ElmContext *context)
{

	// ---- local ----
	UINT  vertex_count = context->GetVertexCount();


	// ---- param data ----
	FLOAT *pd = context->GetFloatParameter();
	FLOAT  pd_config      = pd[DBZ_CONFIG];
	FLOAT  pd_art         = pd[DBZ_ART];
	FLOAT  pd_translate_x = pd[DBZ_TRANSLATE_X];
	FLOAT  pd_translate_y = pd[DBZ_TRANSLATE_Y];
	FLOAT  pd_rotation    = pd[DBZ_ROTATE_Z];
	FLOAT  pd_scale_y     = pd[DBZ_SCALE_Y];
	FLOAT  pd_red_max     = pd[DBZ_NEEDLE_RED_MAX];
	FLOAT  pd_green_max   = pd[DBZ_NEEDLE_GREEN_MAX];
	FLOAT  pd_blue_max    = pd[DBZ_NEEDLE_BLUE_MAX];
	FLOAT  pd_red_min     = pd[DBZ_NEEDLE_RED_MIN];
	FLOAT  pd_green_min   = pd[DBZ_NEEDLE_GREEN_MIN];
	FLOAT  pd_blue_min    = pd[DBZ_NEEDLE_BLUE_MIN];


	// ---- supplied panel param----
	FLOAT *sp = context->GetGfxFloatParameter();
	FLOAT  left   = sp[VS_PANEL_LEFT];
	FLOAT  top    = sp[VS_PANEL_TOP];
	FLOAT  right  = sp[VS_PANEL_RIGHT];
	FLOAT  bottom = sp[VS_PANEL_BOTTOM];
	FLOAT  width  = right - left;
	FLOAT  height = bottom - top;
	FLOAT  hby2   = height / 2;
	FLOAT  cx     = width  / 2 + left;
	FLOAT  cy     = height / 2 + top;


	// ---- set XY param ----
	FLOAT tx = cx + pd_translate_x * hby2;
	FLOAT ty = cy + pd_translate_y * hby2;
	FLOAT rotation = pd_rotation;
	FLOAT radius   = pd_scale_y;


	// ---- vertex ----
	struct XYZWD { float x, y, z, w; DWORD diffuse; };


	// ---- diffuse ----
	DWORD dif_min = VS_D3DCOLOR_COLORVALUE(pd_red_min, pd_green_min, pd_blue_min, 1.0);
	DWORD dif_max = VS_D3DCOLOR_COLORVALUE(pd_red_max, pd_green_max, pd_blue_max, 1.0);


	// ---- hardwired ----
	FLOAT zed = 0.85f;
	FLOAT rhw = 1;


	// ---- proportional to radius (DBZ_NEEDLE_SCALE_Y) ----
	FLOAT r1 = radius * 0.95f;
	FLOAT r2 = radius * 1.00f;
	FLOAT d1 = radius * 0.05f;
	FLOAT d2 = radius * 0.02f;

	XYZWD v[6] =
	{
		{   0,  0, zed, rhw, dif_min },
		{ -d1,  0, zed, rhw, dif_min },
		{ -d2, r1, zed, rhw, dif_min },
		{   0, r2, zed, rhw, dif_max },
		{  d2, r1, zed, rhw, dif_min },
		{  d1,  0, zed, rhw, dif_min },
	};


	// ---- rotate -----
	FLOAT theta = rotation * (FLOAT)VS_RADIANS_PER_DEGREE;
	for (UINT iv=0; iv<6; iv++)
	{
		FLOAT x = v[iv].x * hby2;
		FLOAT y = v[iv].y * hby2;
		v[iv].x = cos(theta)*x - sin(theta)*y + tx;
		v[iv].y = sin(theta)*x + cos(theta)*y + ty;
	}
	memcpy(context->GetVertexBufferPtr(), v, sizeof(v));

	return VS_OK;
}


// ---------- Dbz_SetNeedleRGBMin ----------
INT ObjConfig::Dbz_SetNeedleRGBMin(
		FLOAT pd[],
		FLOAT red,
		FLOAT green,
		FLOAT blue
	)
{
	pd[DBZ_NEEDLE_RED_MIN]   = red;
	pd[DBZ_NEEDLE_GREEN_MIN] = green;
	pd[DBZ_NEEDLE_BLUE_MIN]  = blue;
	return VS_OK;
}


// ---------- Dbz_SetNeedleRGBMax ----------
INT ObjConfig::Dbz_SetNeedleRGBMax(
		FLOAT pd[],
		FLOAT red,
		FLOAT green,
		FLOAT blue
	)
{
	pd[DBZ_NEEDLE_RED_MAX]   = red;
	pd[DBZ_NEEDLE_GREEN_MAX] = green;
	pd[DBZ_NEEDLE_BLUE_MAX]  = blue;
	return VS_OK;
}


// *****************************************************************************


// ---------- Dbz_Callback_Configure_Rect ----------
INT ObjConfig::Dbz_Callback_Configure_Rect(vs_system::ElmContext *context)
{

	context->BeginConfig();
	{

		// ---- param data ----
		FLOAT *pd = context->GetFloatParameter();
		BOOL   visible = (BOOL)((INT)pd[DBZ_VISIBLE]);
		FLOAT  pd_num_edges = pd[DBZ_RECT_QUAD_NUM_EDGES];


		// ---- derive geometry param ----
		INT   num_edge = (INT)pd_num_edges;
		INT   num_vrtx = num_edge * 6 + 24;
		INT   num_prim = num_edge * 2 + 8;


		// ---- atlas page ----
		context->SetAtlasPage(0, 0);


		// ---- IF overide THEN --
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(false);
			context->SetColourVertex(true);
			context->SetMultiState(false);
			context->SetSolid(true);
			context->SetTextured(FALSE);
			context->SetVisible(visible);
			context->SetCullMode(vs_system::ElmContext::CULL_NONE);
		}


		// ---- index buffer ----
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


		// ---- vertex buffer ----
		context->SetVertexCount(num_vrtx);
		context->SetVertexFormat((UINT)(VS_XYZRHW | VS_DIFFUSE));
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Dbz_Callback_GetVertexBuffer_Rect);


		// ---- primitive parameters ----
		context->SetPrimitiveBytes(
					context->GetVertexSize() *
					context->GetVertexCount() *
					sizeof(float)
				);
		context->SetPrimitiveCount(num_prim);
		context->SetPrimitiveType(VS_TRIANGLELIST);

	}
	context->EndConfig();


	return VS_OK;
}


// ---------- Dbz_Callback_GetVertexBuffer_Rect ----------
INT ObjConfig::Dbz_Callback_GetVertexBuffer_Rect(vs_system::ElmContext *context)
{

	// ---- local ----
	UINT  vertex_count = context->GetVertexCount();


	// ---- param data ----
	FLOAT *pd = context->GetFloatParameter();
	FLOAT  pd_config      = pd[DBZ_CONFIG];
	FLOAT  pd_art         = pd[DBZ_ART];
	FLOAT  pd_translate_x = pd[DBZ_TRANSLATE_X];
	FLOAT  pd_translate_y = pd[DBZ_TRANSLATE_Y];
	FLOAT  pd_scale_x     = pd[DBZ_SCALE_X];
	FLOAT  pd_scale_y     = pd[DBZ_SCALE_Y];
	FLOAT  pd_rotation    = pd[DBZ_ROTATE_Z];

	FLOAT  pd_num_edges   = pd[DBZ_RECT_QUAD_NUM_EDGES];
	FLOAT  pd_rmax        = pd[DBZ_RECT_QUAD_RADIUS_MAX];
	FLOAT  pd_rmin        = pd[DBZ_RECT_QUAD_RADIUS_MIN];
	FLOAT  pd_width       = pd[DBZ_RECT_WIDTH];
	FLOAT  pd_height      = pd[DBZ_RECT_HEIGHT];
	FLOAT  pd_red_max     = pd[DBZ_RECT_RED_MAX];
	FLOAT  pd_green_max   = pd[DBZ_RECT_GREEN_MAX];
	FLOAT  pd_blue_max    = pd[DBZ_RECT_BLUE_MAX];
	FLOAT  pd_red_min     = pd[DBZ_RECT_RED_MIN];
	FLOAT  pd_green_min   = pd[DBZ_RECT_GREEN_MIN];
	FLOAT  pd_blue_min    = pd[DBZ_RECT_BLUE_MIN];


	// ---- supplied panel param ----
	FLOAT *sp = context->GetGfxFloatParameter();
	FLOAT  left   = sp[VS_PANEL_LEFT];
	FLOAT  top    = sp[VS_PANEL_TOP];
	FLOAT  right  = sp[VS_PANEL_RIGHT]; 
	FLOAT  bottom = sp[VS_PANEL_BOTTOM];
	FLOAT  width  = right - left;
	FLOAT  height = bottom - top;
	FLOAT  wby2   = width / 2;
	FLOAT  hby2   = height / 2;
	FLOAT  cx     = width  / 2 + left;
	FLOAT  cy     = height / 2 + top;


	// ---- aligment relative to ----
		INT   art = (INT)pd_art;
		FLOAT rotation = 0, translate_x = 0, translate_y = 0;
		switch (art)
		{
			case DBZ_ART_CENTRE:
				{
					// **** modified 20/10/18 for tabs ****
					//
					//      always proportional to height
					//
					wby2 = hby2 / 2;
					rotation    = pd_rotation;
					translate_x = cx + pd_translate_x * hby2;
					translate_y = cy + pd_translate_y * hby2;
				}
				break;
			case DBZ_ART_LEFT:
				{
					rotation = 0;
					translate_x = left + wby2;
					translate_y = cy + pd_translate_y * hby2;
				}
				break;
			case DBZ_ART_RIGHT:
				break;
		}
	

	// ---- derive geometry param ----
	INT   num_edge = (INT)pd_num_edges;
	INT   num_vrtx = num_edge * 6 + 24;
	INT   num_prim = num_edge * 2 + 8;


	// ---- derive screen radii ----
	FLOAT sc_rmax  = pd_rmax * hby2 * pd_scale_y;
	FLOAT r_scalar = pd_rmin / pd_rmax;


	// ---- derive screen mapping ----
	FLOAT sc_rwby2 = pd_width  * wby2     * pd_scale_x;  // **** modified 20/10/18 for tabs ****
	FLOAT sc_rhby2 = pd_height * hby2 / 2 * pd_scale_y;
	FLOAT sc_rwl   = sc_rwby2 - sc_rmax;
	FLOAT sc_rhl   = sc_rhby2 - sc_rmax;


	// ---- vertex ----
	struct XYZWD { float x, y, z, w; DWORD d; };
	FLOAT zed = 0.85f;
	FLOAT rhw = 1;


	// ---- diffuse ----
	DWORD dif_min = VS_D3DCOLOR_COLORVALUE(pd_red_min, pd_green_min, pd_blue_min, 1.0);
	DWORD dif_max = VS_D3DCOLOR_COLORVALUE(pd_red_max, pd_green_max, pd_blue_max, 1.0);


	// ---- quadrant location ----
	FLOAT sc_quadrent_loc[4][2] =
	{
		{ -sc_rwl,  sc_rhl },
		{ -sc_rwl, -sc_rhl },
		{  sc_rwl, -sc_rhl },
		{  sc_rwl,  sc_rhl },
	};


	// ---- ends ----
	XYZWD beg[4][2];
	XYZWD end[4][2];


	// ---- for each quadrant ----
	XYZWD *v = new XYZWD[num_vrtx];
	UINT   vindx = 0;
	FLOAT  ang_inc = 360 / (FLOAT)num_edge;
	for (UINT is = 0; is < 4; is++)
	{

		// ---- for each edge ----
		UINT  edge_vrtx_per_sector = num_edge/4;
		FLOAT ang_quadrant = (FLOAT)is * 90.0f;
		FLOAT xl=0, yl=0, xn=0, yn=0;
		for (UINT iv = 0; iv <= edge_vrtx_per_sector; iv++)
		{

			// ---- outer edge xy ----
			xl = xn;
			yl = yn;
			FLOAT theta = ((FLOAT)iv * ang_inc + ang_quadrant) * (FLOAT)VS_RADIANS_PER_DEGREE;
			xn = -sin(theta) * sc_rmax;
			yn =  cos(theta) * sc_rmax;

			// ---- buffer first vertex ----
			if ( iv > 0 )
			{

				// ---- quad ----
				v[vindx  ].x = xl;
				v[vindx  ].y = yl;
				v[vindx+1].x = xn;
				v[vindx+1].y = yn;
				v[vindx+2].x = xl * r_scalar;
				v[vindx+2].y = yl * r_scalar;

				v[vindx+3].x = xl * r_scalar;
				v[vindx+3].y = yl * r_scalar;
				v[vindx+4].x = xn;
				v[vindx+4].y = yn;
				v[vindx+5].x = xn * r_scalar;
				v[vindx+5].y = yn * r_scalar;

				// --- screen locate ----
				for (UINT j = 0; j < 6; j++)
				{
					v[vindx+j].x = v[vindx+j].x * pd_scale_x + translate_x + sc_quadrent_loc[is][0];
					v[vindx+j].y = v[vindx+j].y * pd_scale_y + translate_y + sc_quadrent_loc[is][1];
					v[vindx+j].z = zed;
					v[vindx+j].w = rhw;
					v[vindx+j].d = dif_min;
				}

				// ---- buffer ends ----
				if ( iv == 1 )
				{
					beg[is][0] = v[vindx+2];
					beg[is][1] = v[vindx];
				}
				else if ( iv == edge_vrtx_per_sector )
				{
					end[is][0] = v[vindx+4];
					end[is][1] = v[vindx+5];
				}

				// ---- increment ----
				vindx += 6;
			}
		}
	}
	
	
	// ---- use buffered ends to build sides -----
	for (UINT is = 0; is <= 3; is++)
	{
		UINT ib = is == 3 ? 0 : is + 1;
		v[vindx  ] = end[is][0];
		v[vindx+1] = beg[ib][1];
		v[vindx+2] = end[is][1];
		v[vindx+3] = end[is][1];
		v[vindx+4] = beg[ib][0];
		v[vindx+5] = beg[ib][1];
		vindx += 6;
	}


	// ---- copy -----
	memcpy(context->GetVertexBufferPtr(), v, sizeof(XYZWD)*(num_vrtx) );
	delete[] v;

	return VS_OK;
}


// ---------- Dbz_SetRectDimension ----------
INT ObjConfig::Dbz_SetRectDimension(
		FLOAT pd[],
		FLOAT width,
		FLOAT height
	)
{
	pd[DBZ_RECT_WIDTH] = width;
	pd[DBZ_RECT_HEIGHT] = height;
	return VS_OK;
}


// ---------- Dbz_SetRectQuad ----------
INT ObjConfig::Dbz_SetRectQuad(
		FLOAT pd[],
		FLOAT num_edges,
		FLOAT radius_max,
		FLOAT radius_min
	)
{
	pd[DBZ_RECT_QUAD_NUM_EDGES]  = num_edges;
	pd[DBZ_RECT_QUAD_RADIUS_MAX] = radius_max;
	pd[DBZ_RECT_QUAD_RADIUS_MIN] = radius_min;
	return VS_OK;
}


// ---------- Dbz_SetRectRGBMin ----------
INT ObjConfig::Dbz_SetRectRGBMin(
		FLOAT pd[],
		FLOAT red,
		FLOAT green,
		FLOAT blue
	)
{
	pd[DBZ_RECT_RED_MIN]   = red;
	pd[DBZ_RECT_GREEN_MIN] = green;
	pd[DBZ_RECT_BLUE_MIN]  = blue;
	return VS_OK;
}


// ---------- Dbz_SetRectRGBMax ----------
INT ObjConfig::Dbz_SetRectRGBMax(
		FLOAT pd[],
		FLOAT red,
		FLOAT green,
		FLOAT blue
	)
{
	pd[DBZ_RECT_RED_MAX]   = red;
	pd[DBZ_RECT_GREEN_MAX] = green;
	pd[DBZ_RECT_BLUE_MAX]  = blue;
	return VS_OK;
}



// *****************************************************************************


// ---------- Dbz_Callback_Configure_BarGraph ----------
INT ObjConfig::Dbz_Callback_Configure_BarGraph(vs_system::ElmContext *context)
{

	context->BeginConfig();
	{

		// ---- param data ----
		FLOAT *pd = context->GetFloatParameter();
		FLOAT pd_config   = pd[DBZ_CONFIG];
		FLOAT pd_visible  = pd[DBZ_VISIBLE];
		FLOAT pd_num_bars = pd[DBZ_GRAPH_BAR_NUM];


		// ---- config flags ----
		BOOL visible = (BOOL)((INT)pd_visible);
		BOOL edges   = (INT)pd_config & DBZ_CONFIG_EDGES ? TRUE : FALSE;
		BOOL shape   = (INT)pd_config & DBZ_CONFIG_SHAPE ? TRUE : FALSE;
		BOOL area    = (INT)pd_config & DBZ_CONFIG_AREA  ? TRUE : FALSE;
		BOOL bg      = (INT)pd_config & DBZ_CONFIG_BG    ? TRUE : FALSE;


		// ---- derive geometry param ----
		INT   num_bars = (INT)pd_num_bars;
		INT   num_vrtx_edges = edges ? (num_bars * 8 * 3) : 0;
		INT   num_vrtx_shape = shape ? (num_bars * 2 * 3) : 0;
		INT   num_vrtx_area  = area  ? 6 : 0;
		INT   num_vrtx_bg    = bg    ? 6 : 0;
		INT   num_prim_edges = edges ? (num_bars * 8) : 0;
		INT   num_prim_shape = shape ? (num_bars * 2) : 0;
		INT   num_prim_area  = area  ? 2 : 0;
		INT   num_prim_bg    = bg    ? 2 : 0;
		INT   num_vrtx       = num_vrtx_edges + num_vrtx_shape + num_vrtx_area + num_vrtx_bg;
		INT   num_prim       = num_prim_edges + num_prim_shape + num_prim_area + num_prim_bg;


		// ---- atlas page ----
		context->SetAtlasPage(0, 0);


		// ---- IF overide THEN --
		if ( !context->GetOveride() )
		{
			context->SetAlphaBlending(false);
			context->SetColourVertex(true);
			context->SetMultiState(false);
			context->SetSolid(true);
			context->SetTextured(FALSE);
			context->SetVisible(visible);
			context->SetCullMode(vs_system::ElmContext::CULL_NONE);
		}


		// ---- index buffer ----
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


		// ---- vertex buffer ----
		context->SetVertexCount(num_vrtx);
		context->SetVertexFormat((UINT)(VS_XYZRHW | VS_DIFFUSE));
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Dbz_Callback_GetVertexBuffer_BarGraph);


		// ---- primitive parameters ----
		context->SetPrimitiveBytes(
					context->GetVertexSize() *
					context->GetVertexCount() *
					sizeof(float)
				);
		context->SetPrimitiveCount(num_prim);
		context->SetPrimitiveType(VS_TRIANGLELIST);

	}
	context->EndConfig();

	//printf("+++ Configure_BarGraph +++ \n");

	return VS_OK;
}


// ---------- Dbz_Callback_GetVertexBuffer_BarGraph ----------
INT ObjConfig::Dbz_Callback_GetVertexBuffer_BarGraph(vs_system::ElmContext *context)
{

	// ---- param data ----
	FLOAT *pd = context->GetFloatParameter();
	FLOAT  pd_config       = pd[DBZ_CONFIG];
	FLOAT  pd_visible      = pd[DBZ_VISIBLE];
	FLOAT  pd_art          = pd[DBZ_ART];
	FLOAT  pd_translate_x  = pd[DBZ_TRANSLATE_X];
	FLOAT  pd_translate_y  = pd[DBZ_TRANSLATE_Y] * -1;  // flip for y up;;
	FLOAT  pd_scale_x      = pd[DBZ_SCALE_X];
	FLOAT  pd_scale_y      = pd[DBZ_SCALE_Y]     * -1;  // flip for y up;
	FLOAT  pd_rotation     = pd[DBZ_ROTATE_Z];

	FLOAT  pd_num_bars     = pd[DBZ_GRAPH_BAR_NUM];
	FLOAT  pd_width_edge   = pd[DBZ_GRAPH_BAR_WIDTH_EDGE];
	FLOAT  pd_width_space  = pd[DBZ_GRAPH_BAR_WIDTH_SPACE];
	FLOAT  pd_width_margin = pd[DBZ_GRAPH_BAR_WIDTH_MARGIN];
	FLOAT  pd_xarea_min    = pd[DBZ_GRAPH_BAR_XAREA_MIN];
	FLOAT  pd_xarea_max    = pd[DBZ_GRAPH_BAR_XAREA_MAX];
	FLOAT  pd_yoffset      = pd[DBZ_GRAPH_BAR_YOFFSET];
	FLOAT  pd_width        = pd[DBZ_GRAPH_BAR_WIDTH];
	FLOAT  pd_height       = pd[DBZ_GRAPH_BAR_HEIGHT];

	FLOAT  pd_edge_red     = pd[DBZ_GRAPH_BAR_EDGE_RED];
	FLOAT  pd_edge_green   = pd[DBZ_GRAPH_BAR_EDGE_GREEN];
	FLOAT  pd_edge_blue    = pd[DBZ_GRAPH_BAR_EDGE_BLUE];
	FLOAT  pd_edge_fade    = pd[DBZ_GRAPH_BAR_EDGE_FADE];

	FLOAT  pd_shape_red    = pd[DBZ_GRAPH_BAR_SHAPE_RED];
	FLOAT  pd_shape_green  = pd[DBZ_GRAPH_BAR_SHAPE_GREEN];
	FLOAT  pd_shape_blue   = pd[DBZ_GRAPH_BAR_SHAPE_BLUE];
	FLOAT  pd_shape_fade   = pd[DBZ_GRAPH_BAR_SHAPE_FADE];

	FLOAT  pd_area_red     = pd[DBZ_GRAPH_BAR_AREA_RED];
	FLOAT  pd_area_green   = pd[DBZ_GRAPH_BAR_AREA_GREEN];
	FLOAT  pd_area_blue    = pd[DBZ_GRAPH_BAR_AREA_BLUE];
	FLOAT  pd_area_fade    = pd[DBZ_GRAPH_BAR_AREA_FADE];

	FLOAT  pd_bg_red       = pd[DBZ_GRAPH_BAR_BG_RED];
	FLOAT  pd_bg_green     = pd[DBZ_GRAPH_BAR_BG_GREEN];
	FLOAT  pd_bg_blue      = pd[DBZ_GRAPH_BAR_BG_BLUE];
	FLOAT  pd_bg_fade      = pd[DBZ_GRAPH_BAR_BG_FADE];


	// ---- flags ---- 
	BOOL visible  = (BOOL)((INT)pd_visible);
	BOOL edges   = (INT)pd_config & DBZ_CONFIG_EDGES ? TRUE : FALSE;
	BOOL shape   = (INT)pd_config & DBZ_CONFIG_SHAPE ? TRUE : FALSE;
	BOOL area    = (INT)pd_config & DBZ_CONFIG_AREA  ? TRUE : FALSE;
	BOOL bg      = (INT)pd_config & DBZ_CONFIG_BG    ? TRUE : FALSE;


	// ---- supplied panel param ----
	FLOAT *sp = context->GetGfxFloatParameter();
	FLOAT  left   = sp[VS_PANEL_LEFT];
	FLOAT  top    = sp[VS_PANEL_TOP];
	FLOAT  right  = sp[VS_PANEL_RIGHT];
	FLOAT  bottom = sp[VS_PANEL_BOTTOM];
	FLOAT  width  = right - left;
	FLOAT  height = bottom - top;
	FLOAT  hby2   = height / 2;
	FLOAT  cx     = width  / 2 + left;
	FLOAT  cy     = height / 2 + top;


	// ---- aligment relative to ----
	INT   art = (INT)pd_art;
	FLOAT dif = 0;
	switch (art)
	{
		case DBZ_ART_CENTRE:
			break;
		case DBZ_ART_LEFT:
			dif = (height - width) / 2;
			break;
		case DBZ_ART_RIGHT:
			break;
	}


	// ---- set transform param ----
	FLOAT rotation    = pd_rotation;
	FLOAT translate_x = dif + cx + pd_translate_x * hby2;
	FLOAT translate_y = cy + pd_translate_y * hby2;


	// ---- get geometry param ----
	UINT  num_bars = (INT)pd_num_bars;
	UINT  num_vrtx = context->GetVertexCount();
	UINT  num_prim = context->GetPrimitiveCount();


	// ---- triangle indices ----
	INT tri_idx[10][3] =
	{
		{ 0, 1, 4 }, { 4, 1, 5 }, { 1, 2, 5 }, { 5, 2, 6 },
		{ 2, 3, 6 }, { 6, 3, 7 }, { 3, 0, 7 }, { 7, 0, 4 },
		{ 8, 9, 10 }, { 10, 8, 11 }
	};


	// ---- minimax ----
	UINT j_start = 0;
	UINT j_end = 10;
	if (edges && !shape)
	{
		j_end = 8;
	}
	else if (!edges && shape)
	{
		j_start = 8;
	}


	// ---- x, z, w and colour ----
	{

		// ---- vertex ----
		struct XYZWD { float x, y, z, w; DWORD d; };
		XYZWD *v = new XYZWD[num_vrtx];
		INT    size = sizeof(XYZWD);
		FLOAT  zed = 0.0f;
		FLOAT  rhw = 1;
		FLOAT  ex = pd_width_edge * hby2;


		// ---- diffuse colour ----
		DWORD e_max = VS_D3DCOLOR_COLORVALUE(pd_edge_red, pd_edge_green, pd_edge_blue, 1);
		DWORD e_min = VS_D3DCOLOR_COLORVALUE(
				pd_edge_red   * pd_edge_fade,
				pd_edge_green * pd_edge_fade,
				pd_edge_blue  * pd_edge_fade,
				1);

		DWORD s_max = VS_D3DCOLOR_COLORVALUE(pd_shape_red, pd_shape_green, pd_shape_blue, 1);
		DWORD s_min = VS_D3DCOLOR_COLORVALUE(
				pd_shape_red   * pd_shape_fade,
				pd_shape_green * pd_shape_fade,
				pd_shape_blue  * pd_shape_fade,
				1);

		DWORD a_max = VS_D3DCOLOR_COLORVALUE(pd_area_red,  pd_area_green,  pd_area_blue,  1);
		DWORD a_min = VS_D3DCOLOR_COLORVALUE(
				pd_area_red   * pd_area_fade,
				pd_area_green * pd_area_fade,
				pd_area_blue  * pd_area_fade,
				1);

		DWORD b_max = VS_D3DCOLOR_COLORVALUE(pd_bg_red,    pd_bg_green,    pd_bg_blue,    1);
		DWORD b_min = VS_D3DCOLOR_COLORVALUE(
				pd_bg_red   * pd_bg_fade,
				pd_bg_green * pd_bg_fade,
				pd_bg_blue  * pd_bg_fade,
				1);


		// ---- bar increment ----
		FLOAT bar_inc = 1 / pd_num_bars * (pd_width - 2 * pd_width_margin);
		FLOAT bar_max = bar_inc - pd_width_space;
		FLOAT bar_off = pd_width_margin;  // NOT USED ?
		INT   vidx  = 0;


		// ---- bars ----
		if ( edges || shape )
		{
			for (UINT b = 0; b < num_bars; b++)
			{
				FLOAT xmin = (FLOAT)b * bar_inc + pd_width_margin;
				FLOAT xmax = xmin + bar_max;

				xmin = xmin * hby2 * pd_scale_x + translate_x;
				xmax = xmax * hby2 * pd_scale_x + translate_x;

				XYZWD xyzwd[12] =
				{
					{ xmin,      0, zed, rhw, e_min },
					{ xmin,      0, zed, rhw, e_max },
					{ xmax,      0, zed, rhw, e_max },
					{ xmax,      0, zed, rhw, e_min },
					{ xmin + ex, 0, zed, rhw, e_min },
					{ xmin + ex, 0, zed, rhw, e_max },
					{ xmax - ex, 0, zed, rhw, e_max },
					{ xmax - ex, 0, zed, rhw, e_min },
					{ xmin + (edges ? ex : 0), 0, zed, rhw, s_min },
					{ xmin + (edges ? ex : 0), 0, zed, rhw, s_max },
					{ xmax - (edges ? ex : 0), 0, zed, rhw, s_max },
					{ xmax - (edges ? ex : 0), 0, zed, rhw, s_min },
				};
				for (UINT j = j_start; j < j_end; j++)
				{
					for (UINT i = 0; i < 3; i++)
					{
						v[vidx++] = xyzwd[tri_idx[j][i]];
					}
				}
			}
		}


		// ---- area ----
		if ( area )
		{
			INT bg_idx[10][3] =
			{
				{ 0, 1, 2 }, { 0, 2, 3 }
			};
			FLOAT xmin = (pd_xarea_min * pd_width * hby2) + translate_x;
			FLOAT xmax = (pd_xarea_max * pd_width * hby2) * pd_scale_x + translate_x;
			FLOAT ymin = translate_y;
			FLOAT ymax = pd_height * hby2 * pd_scale_y + translate_y;
			FLOAT  ey = pd_width_edge * hby2;  
			XYZWD bg[4] =
			{
				{ xmin, ymin, zed + 0.01f, rhw, a_min },
				{ xmin, ymax, zed + 0.01f, rhw, a_max },
				{ xmax, ymax, zed + 0.01f, rhw, a_max },
				{ xmax, ymin, zed + 0.01f, rhw, a_min },
			};
			for (UINT j = 0; j < 2; j++)
			{
				for (UINT i = 0; i < 3; i++)
				{
					v[vidx++] = bg[ bg_idx[j][i] ];
				}
			}
		}


		// ---- bg ----
		if ( bg )
		{
			INT bg_idx[10][3] =
			{
				{ 0, 1, 2 }, { 0, 2, 3 }
			};
			FLOAT xmin = translate_x;
			FLOAT xmax = pd_width * hby2 * pd_scale_x + translate_x;
			FLOAT ymin = translate_y;
			FLOAT ymax = pd_height * hby2 * pd_scale_y + translate_y;
			FLOAT ey   = pd_width_edge * hby2;
			XYZWD bg[4] =
			{
				{ xmin, ymin, zed + 0.02f, rhw, b_min },
				{ xmin, ymax, zed + 0.02f, rhw, b_max },
				{ xmax, ymax, zed + 0.02f, rhw, b_max },
				{ xmax, ymin, zed + 0.02f, rhw, b_min },
			};
			for (UINT j = 0; j < 2; j++)
			{
				for (UINT i = 0; i < 3; i++)
				{
					v[vidx++] = bg[ bg_idx[j][i] ];
				}
			}
		}


		// ---- copy all -----
		memcpy(context->GetVertexBufferPtr(), v, sizeof(XYZWD)*(num_vrtx));
		delete[] v;

	}


	// ---- y ----
	if ( edges || shape )
	{

		FLOAT *psfb = context->GetGfxFloatBuffer();
		FLOAT  yl = (pd_height - pd_width_margin) * hby2;
		FLOAT  ey = pd_width_edge * hby2;
		FLOAT *fb = context->GetVertexBufferPtr();
		for (UINT b = 0; b < num_bars; b++)
		{
			
			// ---- data ----
			FLOAT h = psfb[b];

			// ---- bar ----
			FLOAT ymin = translate_y - pd_width_margin * hby2 - pd_yoffset * hby2;
			FLOAT ymax = ymin - h * hby2;

			FLOAT y[12] =
			{
				ymin,
				ymax,
				ymax, 
				ymin, 
				ymin - ey,  // flip +/- ey for y up
				ymax + ey,  // ditto
				ymax + ey,  // ditto
				ymin - ey,  // ditto
				ymin - (edges ? ey : 0),  // flip +/- ey for y up
				ymax + (edges ? ey : 0),  // ditto
				ymax + (edges ? ey : 0),  // ditto
				ymin - (edges ? ey : 0)   // ditto
			};
			for (UINT j = j_start; j < j_end; j++)
			{
				for (UINT i = 0; i < 3; i++)
				{
					*(fb+1) = y[tri_idx[j][i]];
					fb += 5;
				}
			}
		}
	}
	 

	//printf("+++ GetVertexBuffer_BarGraph +++ \n");


	return VS_OK;
}


// ---------- Dbz_SetBarGraphNumBars ----------
INT ObjConfig::Dbz_SetBarGraphNum(
		FLOAT pd[],
		FLOAT num
	)
{
	pd[DBZ_GRAPH_BAR_NUM] = num;
	return VS_OK;
}


// ---------- Dbz_SetBarGraphWidthEdge ----------
INT ObjConfig::Dbz_SetBarGraphWidthEdge(
		FLOAT pd[],
		FLOAT edge
	)
{
	pd[DBZ_GRAPH_BAR_WIDTH_EDGE] = edge;
	return VS_OK;
}


// ---------- Dbz_SetBarGraphWidthSpace ----------
INT ObjConfig::Dbz_SetBarGraphWidthSpace(
		FLOAT pd[],
		FLOAT space
	)
{
	pd[DBZ_GRAPH_BAR_WIDTH_SPACE] = space;
	return VS_OK;
}


// ---------- Dbz_SetBarGraphWidthMargin ----------
INT ObjConfig::Dbz_SetBarGraphWidthMargin(
		FLOAT pd[],
		FLOAT margin
	)
{
	pd[DBZ_GRAPH_BAR_WIDTH_MARGIN] = margin;
	return VS_OK;
}


// ---------- Dbz_SetBarGraphXArea ----------
INT ObjConfig::Dbz_SetBarGraphXArea(
		FLOAT pd[],
		FLOAT min,
		FLOAT max
	)
{
	pd[DBZ_GRAPH_BAR_XAREA_MIN] = min;
	pd[DBZ_GRAPH_BAR_XAREA_MAX] = max;
	return VS_OK;
}


// ---------- Dbz_SetBarGraphYOffset ----------
INT ObjConfig::Dbz_SetBarGraphYOffset(
		FLOAT pd[],
		FLOAT offset
	)
{
	pd[DBZ_GRAPH_BAR_YOFFSET] = offset;
	return VS_OK;
}


// ---------- Dbz_SetBarGraphDimension ----------
INT ObjConfig::Dbz_SetBarGraphDimension(
		FLOAT pd[],
		FLOAT width,
		FLOAT height
	)
{
	pd[DBZ_GRAPH_BAR_WIDTH]  = width;
	pd[DBZ_GRAPH_BAR_HEIGHT] = height;
	return VS_OK;
}


// ---------- Dbz_SetBarGraphEdgeRGBF ----------
INT ObjConfig::Dbz_SetBarGraphEdgeRGBF(
		FLOAT pd[],
		FLOAT red,
		FLOAT green,
		FLOAT blue,
		FLOAT fade
	)
{
	pd[DBZ_GRAPH_BAR_EDGE_RED]   = red;
	pd[DBZ_GRAPH_BAR_EDGE_GREEN] = green;
	pd[DBZ_GRAPH_BAR_EDGE_BLUE]  = blue;;
	pd[DBZ_GRAPH_BAR_EDGE_FADE]  = fade;
	return VS_OK;
}


// ---------- Dbz_SetBarGraphShapeRGBF ----------
INT ObjConfig::Dbz_SetBarGraphShapeRGBF(
		FLOAT pd[],
		FLOAT red,
		FLOAT green,
		FLOAT blue,
		FLOAT fade
	)
{
	pd[DBZ_GRAPH_BAR_SHAPE_RED]   = red;
	pd[DBZ_GRAPH_BAR_SHAPE_GREEN] = green;
	pd[DBZ_GRAPH_BAR_SHAPE_BLUE]  = blue;;
	pd[DBZ_GRAPH_BAR_SHAPE_FADE]  = fade;
	return VS_OK;
}


// ---------- Dbz_SetBarGraphAreaRGBF ----------
INT ObjConfig::Dbz_SetBarGraphAreaRGBF(
		FLOAT pd[],
		FLOAT red,
		FLOAT green,
		FLOAT blue,
		FLOAT fade
	)
{
	pd[DBZ_GRAPH_BAR_AREA_RED]   = red;
	pd[DBZ_GRAPH_BAR_AREA_GREEN] = green;
	pd[DBZ_GRAPH_BAR_AREA_BLUE]  = blue;;
	pd[DBZ_GRAPH_BAR_AREA_FADE]  = fade;
	return VS_OK;
}


// ---------- Dbz_SetBarGraphBgRGBF ----------
INT ObjConfig::Dbz_SetBarGraphBgRGBF(
		FLOAT pd[],
		FLOAT red,
		FLOAT green,
		FLOAT blue,
		FLOAT fade
	)
{
	pd[DBZ_GRAPH_BAR_BG_RED]   = red;
	pd[DBZ_GRAPH_BAR_BG_GREEN] = green;
	pd[DBZ_GRAPH_BAR_BG_BLUE]  = blue;
	pd[DBZ_GRAPH_BAR_BG_FADE]  = fade;
	return VS_OK;
}


// *****************************************************************************

