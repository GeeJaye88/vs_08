////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_map.hpp ----------
/*!
\file vs_img_map.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_Img_Map_Configure ----------
INT ObjConfig::Callback_Img_Map_Configure(vs_system::ElmContext *context)
{

	// ---- begin
	context->BeginConfig();


	// ---- parameter data
		FLOAT *pd = context->GetParameterData();
		UINT  index = (UINT)pd[IMG_RECT_INDEX];
		UINT  type  = (UINT)pd[IMG_RECT_TYPE];


	// ---- prim
		UINT prim_count;
		VS_PrimitiveType prim_type;
		switch (type)
		{
			case IMG_RECT_TYPE_QUAD:
				prim_count = 2;
				prim_type  = VS_TRIANGLELIST;
				break;
			case IMG_RECT_TYPE_VIEW:
				prim_count = 16;
				prim_type  = VS_TRIANGLELIST;
				break;
			default: // IMG_RECT_TYPE_LINE
				prim_count = 4;
				prim_type  = VS_LINESTRIP;
				break;
		}


	// ---- vrtx
		UINT vrtx_count, vrtx_format;
		switch (type)
		{
			case IMG_RECT_TYPE_QUAD:
				vrtx_count  = prim_count * 3;
				vrtx_format = VS_XYZRHW | VS_TEX1;
				break;
			case IMG_RECT_TYPE_VIEW:
				vrtx_count  = prim_count * 3;
				vrtx_format = VS_XYZRHW | VS_DIFFUSE;
				break;
			default: // IMG_RECT_TYPE_LINE
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
				case IMG_RECT_TYPE_QUAD:
				case IMG_RECT_TYPE_VIEW:
					{
						context->SetAlphaBlending(false);
						context->SetColourVertex(false);
						context->SetMultiState(false);
						context->SetSolid(true);
						context->SetTexture(true);
						context->SetVisible(true);
					}
					break;
				default: // IMG_RECT_TYPE_LINE
					{
						context->SetAlphaBlending(false);
						context->SetColourVertex(true);
						context->SetMultiState(false);
						context->SetSolid(false);
						context->SetTexture(false);
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
		context->SetVertexBufferCallback(&Callback_Img_Map_GetVertexBuffer);


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


// ---------- Callback_Img_Map_GetVertexBuffer ----------
INT ObjConfig::Callback_Img_Map_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- supplied data
		FLOAT *sd    = context->GetSuppliedData();
		FLOAT left   = sd[VS_PANEL_LEFT];
		FLOAT top    = sd[VS_PANEL_TOP];
		FLOAT right  = sd[VS_PANEL_RIGHT];
		FLOAT bottom = sd[VS_PANEL_BOTTOM];
		FLOAT width  = right - left;
		FLOAT height = bottom - top;


	// ---- parameter data
		FLOAT *pd = context->GetParameterData();
		UINT  index = (UINT)pd[IMG_RECT_INDEX];
		UINT  type  = (UINT)pd[IMG_RECT_TYPE];


	// ---- ptf flag
		UINT active_id = pi_obj_pano->ptf_active_id;
		BOOL no_ptf = index == 0 ? TRUE : FALSE;


	// ---- If -- panel zero THEN no PTF else PTF
		FLOAT x_off = no_ptf ? 0 : pi_obj_pano->pan_lut[index];
		FLOAT y_off = no_ptf ? 0 : pi_obj_pano->tilt_lut[index];
		FLOAT fov   = no_ptf ? 1 : pi_obj_pano->fov_lut[index];
		FLOAT zoom = fov <= 1 ? 1 : fov;

		zoom = 1 / zoom;


	// ---- image parameters
		FLOAT img_aspect_ratio = pi_obj_pano->ptf_aspect_ratio;
		FLOAT u_off  = pi_obj_pano->u_offset;
		FLOAT v_off  = pi_obj_pano->v_offset;


	// ---- image width proportional to height
		FLOAT iwp = height * img_aspect_ratio;


	// ---- screen space centre
		FLOAT sxc = width / 2; 
		FLOAT syc = height / 2;


	// ---- screen space offset
		FLOAT sxo = -x_off * iwp;
		FLOAT syo = -y_off * height;


	// ---- panel width proportional to height
		FLOAT pwp = height / 2 * 1.7777f; //pi_obj_pano->ptf_aspect_ratio;


	// ---- screen space minimax
		FLOAT sxl = ( -pwp * zoom ) + sxo + left + sxc;
		FLOAT sxh = (  pwp * zoom ) + sxo + left + sxc;
		FLOAT syl = ( -syc * zoom ) + syo + top  + syc;
		FLOAT syh = (  syc * zoom ) + syo + top  + syc;


	// ---- screen space vertex list
		struct v2d { FLOAT x, y; };
		v2d ss[4] =
		{
			{ sxl,  syl },
			{ sxh,  syl },
			{ sxl,  syh },
			{ sxh,  syh }
		};


	// ---- quad or line
		if ( type == IMG_RECT_TYPE_QUAD )
		{

			// ---- texture uv vertex list
				v2d tm[4] =
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
				FLOAT zed = 1.0f;
				FLOAT rhw = 1;
				UINT  vertex_size = context->GetVertexSize();
				UINT  vertex_count = context->GetVertexCount();
				for (UINT idx =0; idx<vertex_count; idx++)
				{
					*(f)   = v[idx].x;
					*(f+1) = v[idx].y;
					*(f+2) = zed;
					*(f+3) = rhw;
					*(f+4) = v[idx].u;
					*(f+5) = v[idx].v;
					f += vertex_size;
				}
		}
		else
		{

			// ---- vertices

				union colour_union_t {
					DWORD d;
					FLOAT f;
				} ctop, cbot;

				ctop.d = index == active_id-1 ?
					VS_D3DCOLOR_COLORVALUE(0.0f, 0.0f, 1.0f, 1.0f) :
					VS_D3DCOLOR_COLORVALUE(0.9f, 0.9f, 0.9f, 1.0f);

				cbot.d = index == active_id-1 ?
					VS_D3DCOLOR_COLORVALUE(0.0f, 0.0f, 1.0f, 1.0f) :
					VS_D3DCOLOR_COLORVALUE(0.6f, 0.6f, 0.6f, 1.0f);


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
				FLOAT zed = index == active_id-1 ? 0.5f : 1.0f;
				FLOAT rhw = 1;
				UINT  vertex_size = context->GetVertexSize();
				UINT  vertex_count = context->GetVertexCount();
				for (UINT idx=0; idx<vertex_count; idx++)
				{
					*(f)   = v[idx].x;
					*(f+1) = v[idx].y;
					*(f+2) = zed;
					*(f+3) = rhw;
					*(f+4) = v[idx].diffuse;
					f += vertex_size;
				}

		}

	return VS_OK;
}
