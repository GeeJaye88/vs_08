////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_rect.hpp ----------
/*!
\file vs_img_rect.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_Img_Rect_Configure ----------
INT ObjConfig::Callback_Img_Rect_Configure(vs_system::ElmContext *context)
{

	// ---- begin
	context->BeginConfig();


	// ---- parameter data
		FLOAT *pd = context->GetFloatParameter();
		UINT  index = (UINT)pd[IMG_RECT_INDEX];
		UINT  type  = (UINT)pd[IMG_RECT_TYPE];


	// ---- prim
		UINT prim_count;
		VS_PrimitiveType prim_type;
		switch ( type )
		{
			case IMG_RECT_TYPE_SOLID_PTZ_VIEW:
				prim_count = 2;
				prim_type  = VS_TRIANGLELIST;
				break;
			case IMG_RECT_TYPE_SOLID_PTZ_CENTRE:
				prim_count = 24;
				prim_type  = VS_TRIANGLELIST;
				break;
			default: // IMG_RECT_TYPE_LINE_PTZ_EDGE
				prim_count = 4;
				prim_type  = VS_LINESTRIP;
				break;
		}


	// ---- vrtx
		UINT vrtx_count, vrtx_format;
		switch ( type )
		{
			case IMG_RECT_TYPE_SOLID_PTZ_VIEW:
				vrtx_count  = prim_count * 3;
				vrtx_format = VS_XYZRHW | VS_TEX1;
				break;
			case IMG_RECT_TYPE_SOLID_PTZ_CENTRE:
				vrtx_count  = prim_count * 3;
				vrtx_format = VS_XYZRHW | VS_DIFFUSE;
				break;
			default: // IMG_RECT_TYPE_LINE_PTZ_EDGE
				vrtx_count  = 5;
				vrtx_format = VS_XYZRHW | VS_DIFFUSE;
				break;
		}


	// ---- atlas page
		context->SetAtlasPage(0, pi_obj_pano->pano_texture_lid);


	// IF -- overide -- THEN
		if ( !context->GetOveride() )
		{
			switch ( type )
			{
				case IMG_RECT_TYPE_SOLID_PTZ_VIEW:
					{
						context->SetAlphaBlending(false);
						context->SetColourVertex(false);
						context->SetMultiState(false);
						context->SetSolid(true);
						context->SetTextured(TRUE);
						context->SetVisible(true);
					}
					break;
				case IMG_RECT_TYPE_SOLID_PTZ_CENTRE:
					{
						context->SetAlphaBlending(true);
						context->SetColourVertex(true);
						context->SetMultiState(false);
						context->SetSolid(true);
						context->SetTextured(FALSE);
						context->SetVisible(true);
					}
					break;
				default: // IMG_RECT_TYPE_LINE_PTZ_EDGE
					{
						context->SetAlphaBlending(false);
						context->SetColourVertex(true);
						context->SetMultiState(false);
						context->SetSolid(false);
						context->SetTextured(FALSE);
						context->SetVisible(true);
					}
					break;
			}
		}


	// ---- index buffer
		context->SetIndexCount( 0 );
		context->SetIndexBufferCallback(NULL);


	// ---- vertex buffer
		context->SetVertexCount(vrtx_count);
		context->SetVertexFormat(vrtx_format);
		context->SetVertexSize(context->GetVertexFormat());
		context->SetVertexBufferCallback(&Callback_Img_Rect_GetVertexBuffer);


	// ---- primitive parameters
		context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
			);
		context->SetPrimitiveCount(prim_count);
		context->SetPrimitiveType(prim_type);


	// ---- end
		context->EndConfig();

	return VS_OK;
}


// ---------- Callback_Img_Rect_GetVertexBuffer ----------
INT ObjConfig::Callback_Img_Rect_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- supplied data
		FLOAT *sd    = context->GetGfxFloatParameter();
		FLOAT left   = sd[VS_PANEL_LEFT];
		FLOAT top    = sd[VS_PANEL_TOP];
		FLOAT right  = sd[VS_PANEL_RIGHT];
		FLOAT bottom = sd[VS_PANEL_BOTTOM];
		FLOAT width  = right - left;
		FLOAT height = bottom - top;


	// ---- parameter data
		FLOAT *pd = context->GetFloatParameter();
		UINT  index = (UINT)pd[IMG_RECT_INDEX];
		UINT  type  = (UINT)pd[IMG_RECT_TYPE];
		UINT  mode  = (UINT)pd[IMG_RECT_MODE];
		UINT  exposure = (UINT)pd[IMG_RECT_ZOOM];
		

	// ---- init aec
		pd[IMG_RECT_PARA_LEFT]   = 0;
		pd[IMG_RECT_PARA_RIGHT]  = 1;
		pd[IMG_RECT_PARA_BOTTOM] = 0;
		pd[IMG_RECT_PARA_TOP]    = 1;


	// ---- "CONFIG IMG PTZ" ---- TBD

	// ---- ptz flag
		BOOL no_ptz = index == IMG_PTZ_OVERVIEW_PANEL_INDEX ? TRUE : FALSE;


	// ---- image offset
		FLOAT u_off = pi_obj_pano->u_offset;
		FLOAT v_off = pi_obj_pano->v_offset;


	// ---- If zero index THEN no PTF ( note: F == "zoom" )
		FLOAT x_off = no_ptz ? 0 : -pi_obj_pano->pan_lut[index];
		FLOAT y_off = no_ptz ? 0 :  pi_obj_pano->tilt_lut[index];
		FLOAT fov   = no_ptz ? 1 :  pi_obj_pano->fov_lut[index];
		FLOAT zoom  = fov <= 1 ? 1 : fov;


	// ---- "zoom" calculation
		zoom = 1 + (zoom - 1) / 10;
	

	// ---- index
		UINT active_index = pi_obj_pano->ptf_active_id - 1;
		UINT over_index   = pi_obj_pano->ptf_over_id - 1;


	// ---- set state flags
		BOOL active = FALSE, over = FALSE, zoom_def = FALSE;
		if ( mode == IMG_RECT_MODE_PTF_VIEW &&
			index == IMG_PTZ_SETTINGS_PANEL_INDEX )
		{
			active   = TRUE;
			over     = TRUE;
			zoom_def = FALSE;
		}
		else
		{
			active   = active_index == index ? TRUE : FALSE;
			over     = over_index == index ? TRUE : FALSE;
			zoom_def = zoom < 1.001f;
		}


	// ---- local rect xy stuff
		FLOAT sxl, sxh, syl, syh;
		FLOAT sxo, syo;
		FLOAT sxc = width / 2;
		FLOAT syc = height / 2;


	// ---- panel, graticule or outline
		switch ( mode )
		{

			case IMG_RECT_MODE_PTF_VIEW:
				{

					// ---- offset
						FLOAT swp     = height *  1 / pi_obj_pano->aspect_ratio;
						FLOAT swp_by2 = swp / 2;
						sxo = x_off * swp;
						syo = y_off * height;


					// ---- exposure rect ?
						UINT exposure_rect = exposure == IMG_RECT_EXP_AOI_ON ?
								pi_obj_pano->exposure_aoi_zone :
									IMG_RECT_EXP_AOI_ALL;


					// ---- locate rect relative to panel
					switch ( exposure_rect )
					{

						case IMG_RECT_EXP_AOI_PTF_ACTIVE:
						case IMG_RECT_EXP_AOI_CENTRE:
							{

								#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

									FLOAT h = 0, w = 0, l = 0, t = 0;
									cam_props->GetProperty(CamProps::AOI, 0, CamProps::WIDTH,  &w);
									cam_props->GetProperty(CamProps::AOI, 0, CamProps::HEIGHT, &h);
									cam_props->GetProperty(CamProps::AOI, 0, CamProps::LEFT,   &l);
									cam_props->GetProperty(CamProps::AOI, 0, CamProps::TOP,    &t);

									FLOAT x_min = swp * l + (sxc - swp_by2);
									FLOAT x_max = x_min + swp * w;

									FLOAT y_min = height * t;
									FLOAT y_max = y_min + height * h;

									FLOAT aoi_x = swp_by2 * pi_obj_pano->exposure_aoi_max;
									FLOAT aoi_y = syc     * pi_obj_pano->exposure_aoi_min;

									sxl = (( x_min + sxo - sxc) * zoom ) + left + sxc;
									sxh = (( x_max + sxo - sxc) * zoom ) + left + sxc;
									syl = (( y_min + syo - syc) * zoom ) + top  + syc;
									syh = (( y_max + syo - syc) * zoom ) + top  + syc;

								#else

									FLOAT aoi_x = swp_by2 * pi_obj_pano->exposure_aoi_max;
									FLOAT aoi_y = syc * pi_obj_pano->exposure_aoi_min;
									sxl = ((-aoi_x + sxo) * zoom ) + left + sxc;
									sxh = (( aoi_x + sxo) * zoom ) + left + sxc;
									syl = ((-aoi_y + syo) * zoom ) + top  + syc;
									syh = (( aoi_y + syo) * zoom ) + top  + syc;

								#endif

							}
							break;


						case IMG_RECT_EXP_AOI_ALL:
						default:
							{
								sxl = ((-swp_by2 + sxo) * zoom ) + left + sxc;
								sxh = (( swp_by2 + sxo) * zoom ) + left + sxc;
								syl = ((-syc     + syo) * zoom ) + top  + syc;
								syh = ( (syc     + syo) * zoom ) + top  + syc;
							}
							break;
					}

				}
				break;

			case IMG_RECT_MODE_PTF_CENTRE:
				{
					// ---- "zoom"
						zoom = 1;

					// ---- locate centred on panel
						sxl = left;
						sxh = right;
						syl = top;
						syh = bottom;
				}
				break;

			case IMG_RECT_MODE_PTF_OVERVIEW:
			default:
				{
					// "zoom"
					zoom = 1 / zoom;

					// ---- offset
					FLOAT swp = height * 1 / pi_obj_pano->aspect_ratio;
					sxo = -x_off * swp;
					syo = -y_off * height;


					// ---- locate rect relative to panel
					FLOAT pwp = height / 2 * pi_obj_pano->ptf_aspect_ratio;
					sxl = ( -pwp * zoom ) + sxo + left + sxc;
					sxh = (  pwp * zoom ) + sxo + left + sxc;
					syl = ( -syc * zoom ) + syo + top  + syc;
					syh = (  syc * zoom ) + syo + top  + syc;


					// --- locate button elements relative to rect
					//
					//     flag aoi (param[BUTTON_AOI]= 1) and
					//     store rect bounds to be used   
					//
					//
					if ( mode == IMG_RECT_MODE_PTF_OVERVIEW )
					{
						LONG *long_param = context->GetLongParameter();
						for (UINT i = 0; i < 3; i++)
						{
							LONG long_val = *(long_param + i);
							if ( long_val > 0 )
							{
								ElmContext *elm_context = (ElmContext *)long_val;
								FLOAT *param = elm_context->GetFloatParameter();
									param[BUTTON_AOI]    = 1;
									param[BUTTON_LEFT]   = sxl;
									param[BUTTON_TOP]    = syl;
									param[BUTTON_RIGHT]  = sxh;
									param[BUTTON_BOTTOM] = syh;
							}
						}
					}

				}
				break;
		}


	// ---- screen space vertex list
		struct v2 { FLOAT x, y; };
		v2 ss[4] =
		{
			{ sxl,  syl },
			{ sxh,  syl },
			{ sxl,  syh },
			{ sxh,  syh }
		};


	// ---- quad, view or line
		switch ( type )
		{

			case IMG_RECT_TYPE_SOLID_PTZ_VIEW:
				{

					// ---- texture uv vertex list
						v2 tm[4] =
						{
							{ 0 + u_off,  0 + v_off },
							{ 1 + u_off,  0 + v_off },
							{ 0 + u_off,  1 + v_off },
							{ 1 + u_off,  1 + v_off }
						};


					// ---- vertices
						struct vertex { FLOAT x, y, u, v; };
						vertex v[6] = 
						{
							{ ss[0].x, ss[0].y, tm[0].x, tm[0].y }, // left,  top
							{ ss[1].x, ss[1].y, tm[1].x, tm[1].y }, // right, top
							{ ss[2].x, ss[2].y, tm[2].x, tm[2].y }, // left,  bottom
							{ ss[2].x, ss[2].y, tm[2].x, tm[2].y }, // left,  bottom
							{ ss[1].x, ss[1].y, tm[1].x, tm[1].y }, // right, top
							{ ss[3].x, ss[3].y, tm[3].x, tm[3].y }  // right, bottom
						};


					// ---- init
						FLOAT *f = context->GetVertexBufferPtr();
						FLOAT zed = 0.99f;
						FLOAT rhw = 1;
						UINT  vertex_size = context->GetVertexSize();
						UINT  vertex_count = context->GetVertexCount();
						for (UINT in =0; in<vertex_count; in++)
						{
							*(f)   = v[in].x;
							*(f+1) = v[in].y;
							*(f+2) = zed;
							*(f+3) = rhw;
							*(f+4) = v[in].u;
							*(f+5) = v[in].v;
							f += vertex_size;
					}
				}
				break; 

			case IMG_RECT_TYPE_SOLID_PTZ_CENTRE:
				{

					// ---- diffuse colour mode, on or off...
						UINT ci = active ? 0 : 1;


					// ---- diffuse colour
						struct rgba { FLOAT r, g, b, a; };
						rgba df[2][2] =
							{

								// ---- ON
								{	{ over ? 0.00f : 0.15f, over ? 0.40f : 0.40f, over ? 1.00f : 1.00f, over ? 1.00f : 0.8f },
									{ over ? 0.00f : 0.15f, over ? 0.40f : 0.40f, over ? 1.00f : 1.00f, over ? 1.00f : 0.8f }
								},

								// ---- OFF
								{	{ over ? 1.00f : 0.60f, over ? 1.00f : 0.60f, over ? 1.00f : 0.60f, over ? 1.00f : 1.0f },
									{ over ? 1.00f : 0.60f, over ? 1.00f : 0.60f, over ? 1.00f : 0.60f, over ? 1.00f : 1.0f }
								}
							};


					// ---- diffuse colour union
						union cut {
							DWORD d;
							FLOAT f;
						} od[8], vd[8];


					// ---- outline diffuse interpolation into colour union DWORD
						FLOAT om = 1.0;
						if ( !zoom_def )
						{
							if ( mode == IMG_RECT_MODE_PTF_CENTRE  )
							{
								om = 0.0f; // was 0.0, which reduced over emphasis
							}
							else
							{
								om = 1.0f;
							}
						}
						for (UINT i = 0; i <= 7; i++)
						{
							FLOAT rmu = i == 0 ? 0 : (FLOAT)i / 7.0f;
							FLOAT cr = (1 - rmu) * df[ci][0].r + rmu * df[ci][1].r;
							FLOAT cg = (1 - rmu) * df[ci][0].g + rmu * df[ci][1].g;
							FLOAT cb = (1 - rmu) * df[ci][0].b + rmu * df[ci][1].b;
							FLOAT ca = (1 - rmu) * df[ci][0].a + rmu * df[ci][1].a;
							od[i].d = VS_D3DCOLOR_COLORVALUE(cr, cg, cb, ca * om);
						};


					// ---- view diffuse interpolation into colour union DWORD
						FLOAT vm = active ? 1.0f : 0.0f;
						for (UINT i = 0; i <= 7; i++)
						{
							FLOAT rmu = i == 0 ? 0 : (FLOAT)i / 7.0f;
							FLOAT cr = (1 - rmu) * df[ci][0].r + rmu * df[ci][1].r;
							FLOAT cg = (1 - rmu) * df[ci][0].g + rmu * df[ci][1].g;
							FLOAT cb = (1 - rmu) * df[ci][0].b + rmu * df[ci][1].b;
							FLOAT ca = (1 - rmu) * df[ci][0].a + rmu * df[ci][1].a;
							vd[i].d = VS_D3DCOLOR_COLORVALUE(cr, cg, cb, ca * vm);
						};


					// ---- dimensions
						FLOAT d = height * 0.05f * zoom;
						FLOAT n = 2, w = 2;
						FLOAT t = over ? w+1 : w;


					// ---- size & location
						FLOAT xc  = (sxl + sxh) / 2;
						FLOAT yc  = (syl + syh) / 2;


					// ---- minimax
						FLOAT ff  = 1.5f;
						FLOAT lxl = xc - d * ff, lxh = xc + d * ff;
						FLOAT lyl = yc - d * ff, lyh = yc + d * ff;


					// ---- outline vertex data
						FLOAT ox[4] = { sxl, sxl + t, sxh - t, sxh };
						FLOAT oy[4] = { syl, syl + t, syh - t, syh };


					// ---- view vertex data
						FLOAT vx[8] = { lxl, lxl + w, lxl + d - n, lxl + d, lxh - d, lxh - d + n, lxh - w, lxh };
						FLOAT vy[8] = { lyl, lyl + w, lyl + d - n, lyl + d, lyh - d, lyh - d + n, lyh - w, lyh };


					// ---- vertices, with diffuse colour from colour union FLOAT
						struct v2d { FLOAT x, y, diffuse; };
						v2d v[72] =
						{

						// ---- border

							{ ox[0], oy[0], od[0].f }, { ox[3], oy[0], od[0].f }, { ox[2], oy[1], od[1].f },
							{ ox[2], oy[1], od[1].f }, { ox[1], oy[1], od[1].f }, { ox[0], oy[0], od[0].f },

							{ ox[1], oy[2], od[2].f }, { ox[2], oy[2], od[2].f }, { ox[3], oy[3], od[3].f },
							{ ox[3], oy[3], od[3].f }, { ox[0], oy[3], od[3].f }, { ox[1], oy[2], od[2].f },

							{ ox[0], oy[0], od[0].f }, { ox[1], oy[1], od[1].f }, { ox[1], oy[2], od[2].f },
							{ ox[1], oy[2], od[2].f }, { ox[0], oy[3], od[3].f }, { ox[0], oy[0], od[0].f },

							{ ox[2], oy[1], od[1].f }, { ox[3], oy[0], od[0].f }, { ox[2], oy[2], od[2].f },
							{ ox[2], oy[2], od[2].f }, { ox[3], oy[0], od[0].f }, { ox[3], oy[3], od[3].f },

						// ---- top grat 

							{ vx[0], vy[0], vd[0].f }, { vx[0], vy[3], vd[3].f }, { vx[1], vy[2], vd[2].f },
							{ vx[1], vy[2], vd[2].f }, { vx[1], vy[1], vd[1].f }, { vx[0], vy[0], vd[0].f },

							{ vx[0], vy[0], vd[0].f }, { vx[1], vy[1], vd[1].f }, { vx[2], vy[1], vd[1].f },
							{ vx[2], vy[1], vd[1].f }, { vx[3], vy[0], vd[0].f }, { vx[0], vy[0], vd[0].f },

							{ vx[4], vy[0], vd[0].f }, { vx[5], vy[1], vd[1].f }, { vx[6], vy[1], vd[1].f },
							{ vx[6], vy[1], vd[1].f }, { vx[7], vy[0], vd[0].f }, { vx[4], vy[0], vd[0].f },

							{ vx[7], vy[0], vd[0].f }, { vx[6], vy[1], vd[1].f }, { vx[6], vy[2], vd[2].f },
							{ vx[6], vy[2], vd[2].f }, { vx[7], vy[3], vd[3].f }, { vx[7], vy[0], vd[0].f },

						// ---- bottom grat

							{ vx[0], vy[4], vd[4].f }, { vx[1], vy[5], vd[5].f }, { vx[0], vy[7], vd[7].f },
							{ vx[0], vy[7], vd[7].f }, { vx[1], vy[5], vd[5].f }, { vx[1], vy[6], vd[6].f },

							{ vx[0], vy[7], vd[7].f }, { vx[1], vy[6], vd[6].f }, { vx[2], vy[6], vd[6].f },
							{ vx[2], vy[6], vd[6].f }, { vx[3], vy[7], vd[7].f }, { vx[0], vy[7], vd[7].f },

							{ vx[4], vy[7], vd[7].f }, { vx[5], vy[6], vd[6].f }, { vx[6], vy[6], vd[6].f },
							{ vx[6], vy[6], vd[6].f }, { vx[7], vy[7], vd[7].f }, { vx[4], vy[7], vd[7].f },

							{ vx[7], vy[7], vd[7].f }, { vx[6], vy[6], vd[6].f }, { vx[6], vy[5], vd[5].f },
							{ vx[6], vy[5], vd[5].f }, { vx[7], vy[4], vd[4].f }, { vx[7], vy[7], vd[7].f },

						};


						// ---- init
							FLOAT *f = context->GetVertexBufferPtr();
							FLOAT zed = over ? 0.0f : ( active ? 0.5f : 0.99f );
							FLOAT rhw = 1;
							UINT  vertex_size  = context->GetVertexSize();
							UINT  vertex_count = context->GetVertexCount();
							for (UINT in = 0; in<vertex_count; in++)
							{
								*(f)     = v[in].x;
								*(f + 1) = v[in].y;
								*(f + 2) = zed;
								*(f + 3) = rhw;
								*(f + 4) = v[in].diffuse;
								f += vertex_size;
							}

				}
				break;


			case IMG_RECT_TYPE_LINE_PTZ_EDGE:
			default:
				{

					// ---- colour union
						union cut {
							DWORD d;
							FLOAT f;
						} ctop, cbot;
						ctop.d = VS_D3DCOLOR_COLORVALUE( 0.5f, 0.5f, 0.5f, 1.0f);
						cbot.d = VS_D3DCOLOR_COLORVALUE( 0.3f, 0.3f, 0.3f, 1.0f);
				
				
					// ---- vertices
						struct vertex { FLOAT x, y, diffuse; };
						vertex v[5] = 
						{
							{ ss[0].x, ss[0].y, ctop.f }, // left,  top
							{ ss[1].x, ss[1].y, ctop.f }, // right, top
							{ ss[3].x, ss[3].y, cbot.f }, // right, bottom
							{ ss[2].x, ss[2].y, cbot.f }, // left,  bottom
							{ ss[0].x, ss[0].y, ctop.f }, // left,  top
						};


					// ---- init
						FLOAT *f = context->GetVertexBufferPtr();
						FLOAT zed = over ? 0.0f : (active ? 0.5f : 0.99f);
						FLOAT rhw = 1;
						UINT  vertex_size = context->GetVertexSize();
						UINT  vertex_count = context->GetVertexCount();
						for (UINT in =0; in<vertex_count; in++)
						{
							*(f)   = v[in].x;
							*(f+1) = v[in].y;
							*(f+2) = zed;
							*(f+3) = rhw;
							*(f+4) = v[in].diffuse;
							f += vertex_size;
						}

				}
		}

		/*
		CHAR msg[VS_MAXCHARNAME];
		sprintf(msg, "--- Get VB for %d\n", index);
		OutputDebugString(msg);
		*/

	return VS_OK;
}
