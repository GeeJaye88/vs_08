////////////////////////////////////////////////////////////////////////////////

// ---------- r2_old_button_geometry.hpp ----------
/*!
\file r2_old_button_geometry.hpp
\brief
\author Gareth Edwards
*/


#ifndef REVIEW_02_BUTTON_GEOMETRY
#define REVIEW_02_BUTTON_GEOMETRY


// ---------- application include ----------
#include "../header/r2_model.h"


using namespace review_02;


// ---------- Callback_Button_Configure ----------
INT Model::Callback_Button_Configure(vs_system::ElmContext *context)
{

	// ---- begin configuration profiling ----
	context->BeginConfig();

	// Get version
	UINT version = context->GetVersion();

	// Set primitive type
	switch ( version )
	{
		default:
			return false;
		case 0: // Background
		case 1: // Icon
			context->SetPrimitiveType(VS_TRIANGLESTRIP);
			break;
		case 2: // Edge
			context->SetPrimitiveType(VS_LINESTRIP);
			break;
	}

	// ---- atlas pages ----
	context->SetAtlasPages(VS_ATLAS_PAGE_GROUP_TWO);

	// ---- index buffer ----
	context->SetIndexCount(0);
	context->SetIndexBufferCallback(NULL);

	// ---- vertex buffer ----
	context->SetVertexBufferCallback(&Callback_Button_GetVertexBuffer);

	// Select
	switch ( version )
	{
		default:
			return false;
	
		case 0: // Background (VS_TRIANGLESTRIP)
		case 1: // Icon (VS_TRIANGLESTRIP)
			{
				// Render flags
				context->SetAlphaBlending(true);
				context->SetColourVertex(false);
				context->SetMultiState(true);
				context->SetSolid(true);
				context->SetTextured(TRUE);
				context->SetVisible(true);

				// ---- vertex buffer ----
				context->SetVertexCount(4);
				context->SetVertexFormat( (UINT)( VS_XYZRHW | VS_TEX1 ));
				context->SetVertexSize(context->GetVertexFormat());

				// Set primitive parameters
				context->SetPrimitiveBytes(
						context->GetVertexSize() *
						context->GetVertexCount()  *
						sizeof(float)
					);
				context->SetPrimitiveCount(2);
			}
			break;

		case 2: // Edge (VS_LINESTRIP)
			{
				// Render flags
				context->SetAlphaBlending(true);
				context->SetColourVertex(true);
				context->SetMultiState(false);
				context->SetSolid(false);
				context->SetTextured(FALSE);
				context->SetVisible(true);

				// ---- vertex buffer ----
				context->SetVertexCount(5);
				context->SetVertexFormat((UINT)( VS_XYZRHW | VS_DIFFUSE ));
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


// ---------- Callback_Button_GetVertexBuffer ----------
INT Model::Callback_Button_GetVertexBuffer(vs_system::ElmContext *context)
{

	// ---- initialise ----

	// get version
	UINT version = context->GetVersion();

	// get supplied and parameter data
	FLOAT *sd = context->GetGfxFloatParameter();
	FLOAT *pd = context->GetFloatParameter();

	// calculate panel aspect
	FLOAT panel_width  = sd[VS_PANEL_RIGHT] - sd[VS_PANEL_LEFT];
	FLOAT panel_height = sd[VS_PANEL_BOTTOM] - sd[VS_PANEL_TOP];
	FLOAT panel_aspect = panel_width / panel_height;

	// access indices
	UINT index = (UINT)*(pd+0);      // index of referenced element
	UINT list_index = (UINT)*(pd+1); // index of referenced list


	// ---- atlas page
		UINT atlas_page = (UINT)*(pd + 2);
		context->SetAtlasPages(atlas_page);


	// select list
	button_element_descriptor *list = button_list[list_index];

	// runtime
	BOOL runtime = context->GetRuntime();

	// set element context
	if ( runtime )
	{
		context->SetRuntime(FALSE);
		context->SetName(list[index].name);
		context->SetText(list[index].text);
		context->SetBehaviour(list[index].behaviour);
		context->SetSwitchedOn(list[index].switched_on);
		context->SetFontName(font_list[list_index].name);
		context->SetFontAlignment(vs_system::ElmContext::ALIGN_CENTRE);
		context->SetGroupId(list[index].group_id);
		context->SetId(list[index].id);
		context->SetRGBA(1.0f, 1.0f, 1.0f, 0.4f);
	}

	// set visibility
	context->SetVisible((BOOL)list[index].visible); 

	// set handle for vs_system::ElmContext *
	list[index].handle_element_context[version] = (INT)context;

	// number of elements in list
	UINT number_of_elements = button_list_size[list_index];


	// ---- for ALL elements  ----

	BOOL  logo = FALSE;
	FLOAT list_width = 0;
	FLOAT button_left = 0;
	BOOL  align_left = FALSE;
	for ( UINT e=0; e<number_of_elements; e++ )
	{
		// IF element is current element THEN set button left
		if ( e == index )
			button_left = list_width;

		// set U width
		switch ( list[e].group_id )
		{ 

			case TIMELINE_BUTTON_GROUP_ID:
			case CLIPLINE_BUTTON_GROUP_ID:
				list[e].u_width = (panel_aspect - 2*ICWD - 2*SPWD) / (FLOAT)number_of_elements;
				break;

			case FRAMES_BUTTON_GROUP_ID:
				if ( list[e].id == 9 )
				{
					align_left = TRUE;
					logo = TRUE;
					list[e].u_width = 4.0f;
				}
				break;
				
			case TITLE_BUTTON_GROUP_ID:
				if ( list[e].u_block != 0 )
				{
					list[e].u_width = panel_aspect - 0.75f * ICWD;  // P + space + X
				}
				break;

			case CONTEXT_BUTTON_GROUP_ID:
				if ( list[e].u_block != 0 )
					list[e].u_width = panel_aspect - 2*ICWD - 2*SPWD;
				break;

			default:
				break;
		}

		// accumulate U width
		list_width += list[e].u_width + list[e].u_space;
	}


	// ---- set non-proportional button rect ----
	button_left = button_left / list_width;
	FLOAT button_right = button_left + list[index].u_width / list_width;
	FLOAT button_top = 0;
	FLOAT button_bottom = 1;


	// ---- IF ratio >= 1 (e.g. panel aspect is less than list width) THEN  ----
	FLOAT ratio = panel_aspect / list_width;
	if ( ratio >= 1 )
	{
		// scale DOWN HORIZONTAL button rect
		button_right = button_right / ratio;
		button_left = button_left / ratio;
		FLOAT button_offset = ( 1 - (1 / ratio) ) / 2;
		button_right += button_offset;
		button_left += button_offset;
	}
	else
	{
		// scale DOWN VERTICAL button rect
		if ( logo )
		{
			button_bottom = 1.0f;
		}
		else
		{
			button_bottom = ratio;
		}
	}


	//  ----vertex  ----
	FLOAT panel_left   =  (1-button_left)   * sd[VS_PANEL_LEFT] + button_left   * sd[VS_PANEL_RIGHT];
	FLOAT panel_top    =  (1-button_top)    * sd[VS_PANEL_TOP]  + button_top    * sd[VS_PANEL_BOTTOM];
	FLOAT panel_right  =  (1-button_right)  * sd[VS_PANEL_LEFT] + button_right  * sd[VS_PANEL_RIGHT] - 1;
	FLOAT panel_bottom =  (1-button_bottom) * sd[VS_PANEL_TOP]  + button_bottom * sd[VS_PANEL_BOTTOM] - 1;


	// ---- IF clipline list THEN shorten ----
	if (  list[index].group_id == CLIPLINE_BUTTON_GROUP_ID )
	{
		FLOAT clip_height = (panel_bottom - panel_top) * 0.2f;
		panel_top = list[index].group_id == CLIPLINE_BUTTON_GROUP_ID ? panel_bottom - clip_height : panel_top;
	}


	// ---- IF version == TEXT THEN (re)set font height ----
	if ( version == vs_system::ElmContext::X_VERSION_TEXT )
	{
		FLOAT scalar = list[index].group_id == TIMELINE_BUTTON_GROUP_ID ? 0.5f : 1.0f;
		context->SetFontHeight( (UINT)(scalar * panel_height * 0.75 + 0.5) );
	}


	// ---- select ----
	switch ( context->GetPrimitiveType() )
	{
		default:
			return false;

		case VS_TRIANGLESTRIP:
		{
			// texture vertex
			struct XYZWUV { FLOAT x, y, z, w; FLOAT u, v; };
			FLOAT texture_left_offset  = 
				align_left ? 0 :
					list[index].u_width < 1 ? (1 - list[index].u_width) * 0.125f / 2 : 0;
			FLOAT texture_right_offset =
					list[index].u_width * 0.125f;
			FLOAT texture_left   = 0;
			FLOAT texture_top    = 0;
			FLOAT texture_right  = 0;
			FLOAT texture_bottom = 0;
			FLOAT zed = 0;

			// select mapping
			switch ( version )
			{

				default:
					break;

				case 0: // background
					{
						texture_left   = (FLOAT)list[index].tex_background_index * 0.125f;
						texture_top    = 0.0f;
						texture_right  = texture_left + 0.125f;
						texture_bottom = 0.125f;
						zed = 0.2f;
					}
					break;

				case 1: // icon

					// IF timeline OR clipline THEN
					if ( list_index== 3 || list_index == 4 )
					{
						// locate "as is"
						texture_left   = (FLOAT)list[index].tex_icon_col * 0.125f + texture_left_offset;
						texture_top    = (FLOAT)list[index].tex_icon_row * 0.125f;
						texture_right  = texture_left + texture_right_offset;
						texture_bottom = texture_top + 0.125f;
					}
					// ELSE normal
					else
					{
						// locate
						texture_left   = (FLOAT)list[index].tex_icon_col * 0.125f + texture_left_offset;
						texture_top    = (FLOAT)list[index].tex_icon_row * 0.125f;
						texture_right  = texture_left + texture_right_offset;
						texture_bottom = texture_top + 0.125f;

						// trim
						FLOAT margin = 0.001f;
						texture_left   += margin;
						texture_top    += margin;
						texture_right  -= margin;
						texture_bottom -= margin;
					}
					zed = 0.1f;
					break;
			}

			// store
			FLOAT rhw = 1;
			XYZWUV elem[] =
			{
				{ panel_left,  panel_top,    zed, rhw, texture_left,  texture_top    },
				{ panel_right, panel_top,    zed, rhw, texture_right, texture_top    },
				{ panel_left,  panel_bottom, zed, rhw, texture_left,  texture_bottom },
				{ panel_right, panel_bottom, zed, rhw, texture_right, texture_bottom },
			};

			// Copy
			memcpy( context->GetVertexBufferPtr(), elem, sizeof(elem) );
		}
		break;

		case VS_LINESTRIP:
		{
			// colour vertex
			struct XYZWD { FLOAT x, y, z, w; DWORD color; };

			// fade colour from top to bottom of rect
			FLOAT r, b, g, a;
			context->GetRGBA(&r, &g, &b, &a);
			DWORD color_bottom = VS_D3DCOLOR_COLORVALUE(r*0.5, b*0.5, g*0.5, a*0.5);
			DWORD color_top = VS_D3DCOLOR_COLORVALUE(r, b, g, a);

			// store
			FLOAT zed = 0;
			FLOAT rhw = 1;
			XYZWD elem[] =
			{
				{ panel_left,  panel_top,    zed, rhw, color_top },
				{ panel_right, panel_top,    zed, rhw, color_top },
				{ panel_right, panel_bottom, zed, rhw, color_bottom },
				{ panel_left,  panel_bottom, zed, rhw, color_bottom },
				{ panel_left,  panel_top,    zed, rhw, color_top }
			};

			// copy
			memcpy( context->GetVertexBufferPtr(), elem, sizeof(elem));
		}
		break;

	}


	// done
	return VS_OK;
}


#endif // REVIEW_02_BUTTON_GEOMETRY


