////////////////////////////////////////////////////////////////////////////////

// ---------- vs_panel_grid.hpp ----------
/*!
\file vs_panel_grid.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- Callback_PanelGrid_Configure ----------
INT ObjConfig::Callback_PanelGrid_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling ----
	context->BeginConfig();

	// Primitive type
	context->SetPrimitiveType(VS_LINESTRIP);

	// ---- index buffer ----
	context->SetIndexCount(0);
	context->SetIndexBufferCallback(NULL);

	// ---- vertex buffer ----
	context->SetVertexBufferCallback(&Callback_PanelGrid_GetVertexBuffer);

	// Select
	switch (context->GetPrimitiveType())
	{
		default:
			return false;

		case VS_LINESTRIP:
		{
			// IF -- overide -- THEN
			if (!context->GetOveride())
			{
				context->SetAlphaBlending(false);
				context->SetColourVertex(true);
				context->SetMultiState(false);
				context->SetSolid(false);
				context->SetTextured(FALSE);
				context->SetVisible(true);
			}

			// ---- vertex buffer ----
			context->SetVertexCount(5);
			context->SetVertexFormat((UINT)(VS_XYZRHW | VS_DIFFUSE));
			context->SetVertexSize(context->GetVertexFormat());

			// Primitive Parameters
			context->SetPrimitiveBytes(
				context->GetVertexSize() *
				context->GetVertexCount()  *
				sizeof(float)
				);
			context->SetPrimitiveCount(4);

		}
		break;
	}

	// ---- end configuration profiling ----
	context->EndConfig();

	// Done
	return VS_OK;
}


// ---------- Callback_PanelGrid_GetVertexBuffer ----------
INT ObjConfig::Callback_PanelGrid_GetVertexBuffer(vs_system::ElmContext *context)
{

	// Select
	switch (context->GetPrimitiveType())
	{
		default:
			return false;

		case VS_LINESTRIP:
		{
			// 2D Colour Vertex
			struct XYZWD { float x, y, z, w; DWORD color; };

			// ---- vertex ----
			float border = 20;
			FLOAT *sp = context->GetGfxFloatParameter();
			float left = sp[VS_PANEL_LEFT] + border;
			float top = sp[VS_PANEL_TOP] + border;
			float right = sp[VS_PANEL_RIGHT] - border - 1;
			float bottom = sp[VS_PANEL_BOTTOM] - border - 1;

			// Store
			float zed = 0.01f;
			float rhw = 1;
			XYZWD elem[] =
			{
				{ left, top, zed, rhw, 0x00FFFFFF },
				{ right, top, zed, rhw, 0x00FFFFFF },
				{ right, bottom, zed, rhw, 0x00FFFFFF },
				{ left, bottom, zed, rhw, 0x00FF8800 },
				{ left, top, zed, rhw, 0x00FFFFFF }
			};

			// Copy
			memcpy(context->GetVertexBufferPtr(), elem, sizeof(elem));
		}
		break;

	}

	// Done
	return VS_OK;
}
