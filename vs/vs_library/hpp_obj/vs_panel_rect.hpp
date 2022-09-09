////////////////////////////////////////////////////////////////////////////////

// ---------- vs_panel_rect.hpp ----------
/*!
\file vs_panel_rect.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_PanelRect_Configure ----------
INT ObjConfig::Callback_PanelRect_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling ----
	context->BeginConfig();


	// ---- param data ----
	FLOAT *p = context->GetFloatParameter();
	UINT  type   = (UINT)p[PANEL_RECT_TYPE];     // 1 == VS_TRIANGLESTRIP, else VS_LINESTRIP
	UINT  tid    = (UINT)p[PANEL_RECT_TEX_LID];  // e.g. 108
	FLOAT border = p[PANEL_RECT_BORDER];         // e.g. 40
	FLOAT aspect = p[PANEL_RECT_ASPECT];         // e.g. 1.333


	// ---- setup for all ----
	context->SetPrimitiveType(type == 1 ? VS_TRIANGLESTRIP : VS_LINESTRIP);
	context->SetAtlasPage(0, tid);
	context->SetIndexCount(0);
	context->SetIndexBufferCallback(NULL);
	context->SetVertexBufferCallback(&Callback_PanelRect_GetVertexBuffer);


	// ---- select by type ----
	switch ( context->GetPrimitiveType() )
	{

		default:
			return false;
	
		case VS_LINESTRIP:
		{

			if ( !context->GetOveride() )
			{
				context->SetColourVertex(true);
				context->SetSolid(false);
				context->SetTextured(FALSE);
				context->SetVisible(true);
			}
			context->SetVertexCount(5);
			context->SetVertexFormat((UINT)( VS_XYZRHW | VS_DIFFUSE ));
			context->SetVertexSize(context->GetVertexFormat());
			context->SetPrimitiveBytes(
					context->GetVertexSize() *
					context->GetVertexCount()  *
					sizeof(float)
				);
			context->SetPrimitiveCount(4);

		}
		break;

		case VS_TRIANGLESTRIP:
		{

			if ( !context->GetOveride() )
			{
				context->SetAlphaBlending(true);
				context->SetColourVertex(false);
				context->SetMultiState(false);
				context->SetSolid(true);
				context->SetTextured(TRUE);
				context->SetVisible(true);
			}
			context->SetVertexCount(4);
			context->SetVertexFormat( (UINT)( VS_XYZRHW | VS_TEX1 ));
			context->SetVertexSize(context->GetVertexFormat());
			context->SetPrimitiveBytes(
					context->GetVertexSize() *
					context->GetVertexCount()  *
					sizeof(float)
				);
			context->SetPrimitiveCount(2);

		}
		break;
	}


	// ---- end configuration profiling ----
	context->EndConfig();

	return VS_OK;
}


// ---------- Callback_PanelRect_GetVertexBuffer ----------
INT ObjConfig::Callback_PanelRect_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- param data ----
	FLOAT *p = context->GetFloatParameter();
	UINT  type   = (UINT)p[PANEL_RECT_TYPE];     // 1 == VS_TRIANGLESTRIP, else VS_LINESTRIP
	UINT  tid    = (UINT)p[PANEL_RECT_TEX_LID];  // e.g. 108
	FLOAT border = p[PANEL_RECT_BORDER];         // e.g. 40
	FLOAT aspect = p[PANEL_RECT_ASPECT];         // e.g. 1.333


	// ---- panel (with border) ----
	FLOAT *sp = context->GetGfxFloatParameter();
	FLOAT panel_left   = sp[VS_PANEL_LEFT]   + border;
	FLOAT panel_top    = sp[VS_PANEL_TOP]    + border;
	FLOAT panel_right  = sp[VS_PANEL_RIGHT]  - border - 1;
	FLOAT panel_bottom = sp[VS_PANEL_BOTTOM] - border - 1;
	FLOAT panel_width  = panel_right  - panel_left;
	FLOAT panel_height = panel_bottom - panel_top;


	// ---- rect (within bordered panel) ----
	FLOAT rect_width  = panel_height * aspect;
	FLOAT rect_height = panel_height;
	FLOAT rect_left   = -rect_width  / 2;
	FLOAT rect_top    = -rect_height / 2;
	FLOAT rect_right  =  rect_width  / 2;
	FLOAT rect_bottom =  rect_height / 2;


	// IF -- width too great -- THEN -- scale --
	if ( rect_width > panel_width )
	{
		FLOAT scalar = panel_width / rect_width;
		rect_left   *= scalar;
		rect_top    *= scalar;
		rect_right  *= scalar;
		rect_bottom *= scalar;
	}


	// ---- center ----
	FLOAT xc = panel_width  / 2 + panel_left;
	FLOAT yc = panel_height / 2 + panel_top;
	rect_left   += xc;
	rect_top    += yc;
	rect_right  += xc;
	rect_bottom += yc;


	// ---- select by type ----
	switch ( context->GetPrimitiveType() )
	{

		default:
		case VS_LINESTRIP:
			{

				struct XYZWD{ float x, y, z, w; DWORD color; };
				float zed = 0.01f;
				float rhw = 1;
				XYZWD elem[] =
				{
					{ rect_left,  rect_top,    zed, rhw, 0x00FFFFFF },
					{ rect_right, rect_top,    zed, rhw, 0x00FFFFFF },
					{ rect_right, rect_bottom, zed, rhw, 0x00FFFFFF },
					{ rect_left,  rect_bottom, zed, rhw, 0x00FF8800 },
					{ rect_left,  rect_top,    zed, rhw, 0x00FFFFFF }
				};
				memcpy( context->GetVertexBufferPtr(), elem, sizeof(elem));

			}
			break;

		case VS_TRIANGLESTRIP:
			{

				struct XYZWUV { float x, y, z, w; float u, v; };
				FLOAT zed = 0.01f;
				FLOAT rhw = 1;
				XYZWUV elem[] =
				{
					{ rect_left,  rect_top,    zed, rhw, 0, 0 },
					{ rect_right, rect_top,    zed, rhw, 1, 0 },
					{ rect_left,  rect_bottom, zed, rhw, 0, 1 },
					{ rect_right, rect_bottom, zed, rhw, 1, 1 },
				};
				memcpy( context->GetVertexBufferPtr(), elem, sizeof(elem) );

			}
			break;
	}

	return VS_OK;
}
