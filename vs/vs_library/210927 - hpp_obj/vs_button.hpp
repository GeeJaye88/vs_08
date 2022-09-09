////////////////////////////////////////////////////////////////////////////////

// ---------- vs_button.hpp ----------
/*!
\file vs_button.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_Button_Configure ----------
INT ObjConfig::Callback_Button_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling ----
		context->BeginConfig();


	// ---- get version
		UINT version = context->GetVersion();


	// ---- set primitive type
		switch ( version )
		{
			default:
				return false;
			case ElmContext::VERSION_BACKGROUND:
			case ElmContext::VERSION_ICON:
			case ElmContext::VERSION_PANEL:
				context->SetPrimitiveType(VS_TRIANGLESTRIP);
				break;
			case ElmContext::VERSION_TEXT:
			case ElmContext::VERSION_OUTLINE:
				context->SetPrimitiveType(VS_LINESTRIP);
				break;
		}


	// ---- atlas pages ----
		context->SetAtlasPages(VS_ATLAS_PAGE_GROUP_ONE);


	// index Buffer
		context->SetIndexCount(0);
		context->SetIndexBufferCallback(NULL);


	// vertex Buffer
		context->SetVertexBufferCallback(&Callback_Button_GetVertexBuffer);


	// ---- select
		switch ( version )
		{
			default: 
				return false;
	
			case ElmContext::VERSION_BACKGROUND:
			case ElmContext::VERSION_ICON:
				{

					// overide ?
					if ( !context->GetOveride() )
					{
						context->SetAlphaBlending(true);
						context->SetColourVertex(false);
						context->SetMultiState(true);
						context->SetSolid(true);
						context->SetTextured(TRUE);
						context->SetVisible(true);
					}

					// vertex buffer
					context->SetVertexCount(4);
					context->SetVertexFormat( (UINT)( VS_XYZRHW | VS_TEX1 ));
					context->SetVertexSize(context->GetVertexFormat());

					// primitive parameters
					context->SetPrimitiveBytes(
							context->GetVertexSize() *
							context->GetVertexCount()  *
							sizeof(float)
						);
					context->SetPrimitiveCount(2);
				}
				break;

			case ElmContext::VERSION_PANEL:
				{

					// overide ?
					if ( !context->GetOveride() )
					{
						context->SetAlphaBlending(TRUE);
						context->SetColourVertex(TRUE);
						context->SetMultiState(FALSE);
						context->SetSolid(true);
						context->SetTextured(FALSE);
						context->SetVisible(true);
					}

					// vertex buffer
					context->SetVertexCount(4);
					context->SetVertexFormat((UINT)(VS_XYZRHW | VS_DIFFUSE));
					context->SetVertexSize(context->GetVertexFormat());

					// primitive parameters
					context->SetPrimitiveBytes(
							context->GetVertexSize() *
							context->GetVertexCount()  *
							sizeof(float)
						);
					context->SetPrimitiveCount(2);
				}
				break;

			case ElmContext::VERSION_TEXT:
			case ElmContext::VERSION_OUTLINE:

				{
					// overide ?
					if ( !context->GetOveride() )
					{
						context->SetAlphaBlending(true);
						context->SetColourVertex(true);
						context->SetMultiState(false);
						context->SetSolid(false);
						context->SetTextured(FALSE);
						context->SetVisible(true);
					}

					// vertex buffer
					context->SetVertexCount(5);
					context->SetVertexFormat((UINT)( VS_XYZRHW | VS_DIFFUSE ));
					context->SetVertexSize(context->GetVertexFormat());

					// primitive parameters
					context->SetPrimitiveBytes(
							context->GetVertexSize() *
							context->GetVertexCount()  *
							sizeof(float)
						);
					context->SetPrimitiveCount(4);

				}
				break;

		}


	// ---- end configuration profiling
		context->EndConfig();


	return VS_OK;
}


// ---------- Callback_Button_GetVertexBuffer ----------
INT ObjConfig::Callback_Button_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- get version
		UINT version = context->GetVersion();


	// ---- get both supplied and parameter data
		FLOAT *sd = context->GetGfxFloatParameter();
		FLOAT *pd = context->GetFloatParameter();


	// ---- aoi ?
		FLOAT sd_left=0, sd_top=0, sd_right=0, sd_bottom=0;
		INT AOI = (UINT)(pd[BUTTON_AOI]);
		if ( AOI == 1 )
		{
			FLOAT width  = sd[VS_PANEL_RIGHT] - sd[VS_PANEL_LEFT];
			FLOAT height = sd[VS_PANEL_BOTTOM] - sd[VS_PANEL_TOP];
			sd_left   = pd[BUTTON_LEFT];
			sd_top    = pd[BUTTON_TOP];
			sd_right  = pd[BUTTON_LEFT] + width;
			sd_bottom = pd[BUTTON_TOP]  + height;
		}
		else
		{
			sd_left   = sd[VS_PANEL_LEFT];
			sd_top    = sd[VS_PANEL_TOP];
			sd_right  = sd[VS_PANEL_RIGHT];
			sd_bottom = sd[VS_PANEL_BOTTOM];
		}


	// ---- set multi-state "Atlas Page Group" ----
		INT APG = (INT)(pd[BUTTON_APG]);
		context->SetAtlasPages(APG);


	// ---- set "Dimension Proportional To".. (e.g. width, height)
		INT DPT = (INT)(pd[BUTTON_DPT]);


	// ---- clip ?
		BOOL viewport_clip = FALSE;
		if ( DPT & vs_system::ElmContext::VIEWPORT_CLIP )
		{
			viewport_clip = TRUE;
			DPT -= vs_system::ElmContext::VIEWPORT_CLIP;
		}


	// ---- set x/y location proportional to a specific dimension ?
		BOOL use_viewport_width_loc = FALSE;
		if ( DPT & vs_system::ElmContext::VIEWPORT_WIDTH_XLOC )
		{
			use_viewport_width_loc = TRUE;
			DPT -= vs_system::ElmContext::VIEWPORT_WIDTH_XLOC;
		}
		BOOL use_viewport_height_loc = FALSE;
		if ( DPT & vs_system::ElmContext::VIEWPORT_HEIGHT_YLOC )
		{
			use_viewport_height_loc = TRUE;
			DPT -= vs_system::ElmContext::VIEWPORT_HEIGHT_YLOC;
		}


	// ---- set texture UV base
		FLOAT tex_bg_col    = pd[BUTTON_BG_COL];
		FLOAT tex_bg_row    = pd[BUTTON_BG_ROW];
		FLOAT tex_icon_col  = pd[BUTTON_FG_COL];
		FLOAT tex_icon_row  = pd[BUTTON_FG_ROW];
		FLOAT tex_dim_col   = pd[BUTTON_DIM_COL];
		FLOAT tex_dim_row   = pd[BUTTON_DIM_ROW];
		FLOAT tex_adjust    = pd[BUTTON_ADJUST] / 2.0f;


	// ---- temp
		/*
		if ( tex_icon_col == 3 && tex_icon_row == 3 )
		{
			float fred = 2;
		}
		*/

	// ---- viewport dimensions
		FLOAT vp_width   = sd_right  - sd_left;
		FLOAT vp_height  = sd_bottom - sd_top;

	
	// ---- viewport pixel ordinates, which are...
		FLOAT pixel_left = 0, pixel_top = 0, pixel_right = 0, pixel_bottom = 0;


	// ---- ... calculated proportional to viewport normalised button ordinates, whch are...
		FLOAT prop_but_left   = pd[BUTTON_X];
		FLOAT prop_but_right  = prop_but_left + pd[BUTTON_WIDTH];
		FLOAT prop_but_top    = pd[BUTTON_Y];
		FLOAT prop_but_bottom = prop_but_top + pd[BUTTON_HEIGHT];


	// ---- line shading
		UINT line_shade = (UINT)pd[BUTTON_LINE_SHADE];


	// ---- .... "Dimensionaly Proportional To" width, height or both.
		switch (DPT)
		{

			case vs_system::ElmContext::VIEWPORT_WIDTH:
				{
					pixel_left   = prop_but_left   * vp_width + sd_left;
					pixel_right  = prop_but_right  * vp_width + sd_left;
					pixel_top    = prop_but_top    * vp_width + sd_top;
					pixel_bottom = prop_but_bottom * vp_width + sd_top;
					if ( use_viewport_height_loc )
					{
						FLOAT pixel_height = pixel_bottom - pixel_top; // ---- CHANGED 22/05/18
						pixel_top    = prop_but_top * vp_height + sd_top;
						pixel_bottom = pixel_top + pixel_height;
					}

				}
				break;

			case vs_system::ElmContext::VIEWPORT_HEIGHT:
				{
					pixel_left   = prop_but_left   * vp_height + sd_left;
					pixel_right  = prop_but_right  * vp_height + sd_left;
					pixel_top    = prop_but_top    * vp_height + sd_top;
					pixel_bottom = prop_but_bottom * vp_height + sd_top;
					if ( use_viewport_width_loc )
					{
						FLOAT pixel_width = pixel_right - pixel_left;
						pixel_right = prop_but_right  * vp_width + sd_left;
						pixel_left  = pixel_right - pixel_width;
					}
				}
				break;

			default:

			case vs_system::ElmContext::VIEWPORT:
				{
					if ( viewport_clip )
					{
						FLOAT temp_but_left   = prop_but_left    * vp_width  + sd_left;
						FLOAT temp_but_right  = prop_but_right   * vp_width  + sd_left;
						FLOAT temp_but_top    = prop_but_top     * vp_height + sd_top;
						FLOAT temp_but_bottom = prop_but_bottom  * vp_height + sd_top;
						pixel_left   = temp_but_left   < sd_left       ? sd_left       : temp_but_left;
						pixel_right  = temp_but_right  > sd_right  - 1 ? sd_right  - 1 : temp_but_right;
						pixel_top    = temp_but_top    < sd_top        ? sd_top        : temp_but_top;
						pixel_bottom = temp_but_bottom > sd_bottom - 1 ? sd_bottom - 1 : temp_but_bottom;
					}
					else
					{
						pixel_left   = prop_but_left   * vp_width  + sd_left;
						pixel_right  = prop_but_right  * vp_width  + sd_left;
						pixel_top    = prop_but_top    * vp_height + sd_top;
						pixel_bottom = prop_but_bottom * vp_height + sd_top;
					}
				}
				break;

			case vs_system::ElmContext::VIEWPORT_CORNER:
				{
					INT   pixel_corner = (INT)pd[BUTTON_WIDTH];
					FLOAT pixel_prop   = pd[BUTTON_HEIGHT];
					FLOAT pixel_size   = vp_height * pixel_prop;
					
					pixel_left   = vp_width  * prop_but_left + sd_left;
					pixel_top    = vp_height * prop_but_top  + sd_top;
					switch (pixel_corner)
					{
						default:
						case 1:
							pixel_right  = pixel_left + pixel_size;
							pixel_bottom = pixel_top  + pixel_size;
							break;
						case 2:
							pixel_left  -= pixel_size;
							pixel_right  = pixel_left + pixel_size;
							pixel_bottom = pixel_top  + pixel_size;
							break;
						case 3:
							pixel_right  = pixel_left + pixel_size;
							pixel_top   -= pixel_size;
							pixel_bottom = pixel_top  + pixel_size;
							break;
						case 4:
							pixel_left  -= pixel_size;
							pixel_right  = pixel_left + pixel_size;
							pixel_top   -= pixel_size;
							pixel_bottom = pixel_top  + pixel_size;
							break;
					}

				}
				break;

		}


	// ---- text size proportionaly hardwired to element height
		if ( version == vs_system::ElmContext::VERSION_TEXT )
		{
			context->SetFontHeight((UINT)((pixel_bottom - pixel_top) * 0.75 + 0.5));
			context->SetTextOutline(FALSE);
		}


	// ---- select setup process by element context
		switch ( context->GetPrimitiveType() )
		{
			default:
				return false;

			case VS_TRIANGLESTRIP:
				{

					if ( version == vs_system::ElmContext::VERSION_PANEL )
					{

						// colour
						struct XYZWD { float x, y, z, w; DWORD d; };
						FLOAT r, b, g, a;
						context->GetRGBA(&r, &g, &b, &a);
						DWORD d = VS_D3DCOLOR_COLORVALUE(r, g, b, a);

						
						// initialise & copy
						FLOAT zed = 0.0f;
						FLOAT rhw = 1;
						XYZWD elem[] =
						{
							{ pixel_left,  pixel_top,    zed, rhw, d },
							{ pixel_right, pixel_top,    zed, rhw, d },
							{ pixel_left,  pixel_bottom, zed, rhw, d },
							{ pixel_right, pixel_bottom, zed, rhw, d },
						};
						memcpy( context->GetVertexBufferPtr(), elem, sizeof(elem));

					}
					else
					{

						// colour and texture
						struct XYZWUV { FLOAT x, y, z, w; FLOAT u, v; };
						FLOAT texture_left   = 0;
						FLOAT texture_top    = 0;
						FLOAT texture_right  = 0;
						FLOAT texture_bottom = 0;

						// painters algorithm: bg, icon/stamp and text
						FLOAT zed = 0;

						// select texture mapping
						switch ( version)
						{

							default:

							case vs_system::ElmContext::VERSION_BACKGROUND:
								zed = 0.02f;
								texture_left   = tex_bg_col   * 0.125f + tex_adjust;
								texture_top    = tex_bg_row   * 0.125f + tex_adjust;
								texture_right  = texture_left + (0.125f * tex_dim_col) - (tex_adjust * 2);
								texture_bottom = texture_top  + (0.125f * tex_dim_row) - (tex_adjust * 2);
								break;

							case vs_system::ElmContext::VERSION_ICON:
								texture_left   = tex_icon_col *  0.125f + tex_adjust;
								texture_top    = tex_icon_row *  0.125f + tex_adjust;
								texture_right  = texture_left + (0.125f * tex_dim_col) - (tex_adjust * 2);
								texture_bottom = texture_top  + (0.125f * tex_dim_row) - (tex_adjust * 2);
								break;

						}

						// initialise
						FLOAT rhw = 1;
						XYZWUV elem[] =
						{
							{ pixel_left,  pixel_top,    zed, rhw, texture_left,  texture_top    },
							{ pixel_right, pixel_top,    zed, rhw, texture_right, texture_top    },
							{ pixel_left,  pixel_bottom, zed, rhw, texture_left,  texture_bottom },
							{ pixel_right, pixel_bottom, zed, rhw, texture_right, texture_bottom },
						};

						// copy
						memcpy( context->GetVertexBufferPtr(), elem, sizeof(elem) );
					}
				}
				break;

			case VS_LINESTRIP:
				{

					// colour
					struct XYZWD { FLOAT x, y, z, w; DWORD color; };
					FLOAT r, b, g, a;
					context->GetRGBA(&r, &g, &b, &a);
				

					// fade from bottom to top....
					DWORD color_top = VS_D3DCOLOR_COLORVALUE(r, g, b, a);
					DWORD color_bottom = VS_D3DCOLOR_COLORVALUE(r*0.5f, g*0.5f, b*0.5f, a*0.5f);


					switch ( line_shade )
					{
						case vs_system::ElmContext::LINE_ALPHA_DEFAULT:
							break;
						case vs_system::ElmContext::LINE_ALPHA_ON:
							break;
						case vs_system::ElmContext::LINE_ALPHA_OFF:
							color_bottom = color_top;
							break;
					}


					// --- DWORD color = VS_D3DCOLOR_COLORVALUE(r, b, g, a);

					// initialise & copy
					FLOAT zed = 0;
					FLOAT rhw = 1;
					XYZWD elem[] =
					{
						{ pixel_left,  pixel_top,    zed, rhw, color_top    },
						{ pixel_right, pixel_top,    zed, rhw, color_top    },
						{ pixel_right, pixel_bottom, zed, rhw, color_bottom },
						{ pixel_left,  pixel_bottom, zed, rhw, color_bottom },
						{ pixel_left,  pixel_top,    zed, rhw, color_top    }
					};
					memcpy( context->GetVertexBufferPtr(), elem, sizeof(elem));
				}
				break;

		}

	return VS_OK;
}


INT ObjConfig::ButtonSetDefault(FLOAT param[])
{

	for (UINT i = 0; i < VS_MAX_ELM_FLOAT_PARAMETER; i++) *(param + i) = 0;

	param[BUTTON_DPT]     = vs_system::ElmContext::VIEWPORT_HEIGHT;
	param[BUTTON_X]       = 0;
	param[BUTTON_Y]       = 0;
	param[BUTTON_WIDTH]   = 0.1f;
	param[BUTTON_HEIGHT]  = 0.1f;
	param[BUTTON_BG_COL]  = 0;
	param[BUTTON_BG_ROW]  = 0;
	param[BUTTON_FG_COL]  = 0;
	param[BUTTON_FG_ROW]  = 0;
	param[BUTTON_DIM_COL] = 1;
	param[BUTTON_DIM_ROW] = 1;
	param[BUTTON_ADJUST]  = 0;
	param[BUTTON_APG]     = 1;
	param[BUTTON_LINE_SHADE] = vs_system::ElmContext::LINE_ALPHA_DEFAULT;

	return VS_OK;
}


INT ObjConfig::ButtonSetDPT(
		FLOAT param[],
		UINT dpt
	)
{
	param[BUTTON_DPT] = (FLOAT)dpt;
	return VS_OK;
}


INT ObjConfig::ButtonSetAoi(
		FLOAT param[],
		UINT  aoi,
		FLOAT left,
		FLOAT top,
		FLOAT right,
		FLOAT bottom
	)
{
	param[BUTTON_AOI]    = (FLOAT)aoi;
	param[BUTTON_LEFT]   = left;
	param[BUTTON_TOP]    = top;
	param[BUTTON_RIGHT]  = right;
	param[BUTTON_BOTTOM] = bottom;
	return VS_OK;
}


INT ObjConfig::ButtonSetLineAlpha(
		FLOAT param[],
		UINT shad_line
	)
{
	param[BUTTON_LINE_SHADE] = (FLOAT)shad_line;
	return VS_OK;
}


INT ObjConfig::ButtonSetLocation(
		FLOAT param[],
		FLOAT x,
		FLOAT y,
		FLOAT width,
		FLOAT height
	)
{
	param[BUTTON_X] = x;
	param[BUTTON_Y] = y;
	param[BUTTON_WIDTH]  = width;
	param[BUTTON_HEIGHT] = height;
	return VS_OK;
}


INT ObjConfig::ButtonSetTexture(
		FLOAT param[],
		UINT  apg,
		UINT  bg_col,
		UINT  bg_row,
		UINT  fg_col,
		UINT  fg_row,
		UINT  dim_col,
		UINT  dim_row,
		FLOAT adjust
	)
{
	param[BUTTON_APG]     = (FLOAT)apg;
	param[BUTTON_BG_COL]  = (FLOAT)bg_col;
	param[BUTTON_BG_ROW]  = (FLOAT)bg_row;
	param[BUTTON_FG_COL]  = (FLOAT)fg_col;
	param[BUTTON_FG_ROW]  = (FLOAT)fg_row;
	param[BUTTON_DIM_COL] = (FLOAT)dim_col;
	param[BUTTON_DIM_ROW] = (FLOAT)dim_row;
	param[BUTTON_ADJUST]  = (FLOAT)adjust;
	return VS_OK;
}


INT ObjConfig::ButtonSetTextureOff(FLOAT param[])
{
	param[BUTTON_APG] = -1;
	return VS_OK;
}
