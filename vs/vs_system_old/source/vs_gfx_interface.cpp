////////////////////////////////////////////////////////////////////////////////

// ---------- vs_gfx_interface.cpp ----------
/*!
\file vs_gfx_interface.cpp
\brief Implementation of the Gfx(/raphics)Interface class
\author Gareth Edwards
\note TAINTED - REQUIRES COMPILE FLAGS
*/

// ---- include
#include "../header/vs_gfx_interface.h"


// ---- namespace
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- debug definitions ----------
// #define DISPLAY_LIST_DEBUG


// ---------- global ( SetupFromJpegTexture ) ----------
//
// RGB  : 4912 x 3684 x 3 = 54287424
// RGBA : 4912 x 3684 x 4 = 72383232
//

//#define VS_USE_BIG_BUFFER

#if defined(VS_USE_BIG_BUFFER)
UCHAR global_very_big_image_buffer[54287424];
#endif

////////////////////////////////////////////////////////////////////////////////


// ---- gfx classes
	#include "../hpp/vs_gfx_page_dimension.hpp"
	#include "../hpp/vs_gfx_page.hpp"
	#include "../hpp/vs_gfx_atlas.hpp"
	#include "../hpp/vs_gfx_device.hpp"
	#include "../hpp/vs_gfx_font.hpp"
	#include "../hpp/vs_gfx_fonts.hpp"
	#include "../hpp/vs_gfx_shaders.hpp"
	#include "../hpp/vs_gfx_lams.hpp"
	#include "../hpp/vs_gfx_render_to_image.hpp"
	#include "../hpp/vs_gfx_pi_element.hpp"
	#include "../hpp/vs_gfx_pi_props.hpp"


// ---- gfx implementation
	#include "../hpp/vs_gfx_imp_fonts.hpp"
	#include "../hpp/vs_gfx_imp_lams.hpp"
	#include "../hpp/vs_gfx_imp_pick_ray.hpp"
	#include "../hpp/vs_gfx_imp_pages.hpp"
	#include "../hpp/vs_gfx_imp_shaders.hpp"
	#include "../hpp/vs_gfx_imp_get_and_set.hpp"


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
GfxInterface::GfxInterface()
{
	pi_props  = new PI_Props;
	pi_display_list = new PI_Element();
}


// ---------- Destructor ----------
GfxInterface::~GfxInterface(){
	if ( pi_props        != NULL ) { delete pi_props; pi_props               = NULL; }
	if ( pi_display_list != NULL ) { delete pi_display_list; pi_display_list = NULL; }
}


////////////////////////////////////////////////////////////////////////////////


// ---------- RenderBegin ----------
/*!
\brief Begin render invoking RenderViewrect and RenderViewport methods
\author Gareth Edwards
\return INT - ERROR # < VS_ERROR < VS_OK 
*/
int GfxInterface::RenderBegin()
{

	// ---- local
		GfxDevice *props_gfx_device = pi_props->gfx_device;
		IDirect3DDevice9 *d3d9_device = props_gfx_device->d3d9_device;


	// ---- local
		HRESULT hr;


	// ---- clear buffers (done below)
		//hr = d3d9_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		//hr = d3d9_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);


	// ---- set 3d render states
		hr = d3d9_device->SetRenderState(D3DRS_AMBIENT, RGB(255, 255, 255));
		hr = d3d9_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		hr = d3d9_device->SetRenderState(D3DRS_LIGHTING, FALSE);
		hr = d3d9_device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		hr = d3d9_device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		hr = d3d9_device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
		hr = d3d9_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		hr = d3d9_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);


	// ---- set general render states
		hr = d3d9_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		hr = d3d9_device->SetRenderState(D3DRS_COLORVERTEX, true);
		hr = d3d9_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, pi_props->aaq != 0);


	// ---- set sampler states
		for (unsigned i = 0; i < 8; i++)
		{
			hr = d3d9_device->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); // D3DTEXF_LINEAR, D3DTEXF_POINT
			hr = d3d9_device->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); // D3DTEXF_LINEAR, D3DTEXF_POINT
			hr = d3d9_device->SetSamplerState(i, D3DSAMP_MIPFILTER, D3DTEXF_ANISOTROPIC);
			hr = d3d9_device->SetSamplerState(i, D3DSAMP_MAXANISOTROPY, 1);
			hr = d3d9_device->SetSamplerState(i, D3DSAMP_SRGBTEXTURE, 0);
		}


	// ---- set NO texture
		hr = d3d9_device->SetTexture(0, 0);


	// ---- set world matrix
		D3DXMATRIX matrix;
		D3DXMatrixIdentity(&matrix);
		hr = d3d9_device->SetTransform(D3DTS_WORLD, &matrix);


	// ---- store viewport
		d3d9_device->GetViewport(&props_gfx_device->d3d9_viewport);


	// ---- clear buffers
		hr = props_gfx_device->ClearViewportBackground(pi_props->colour[pi_props->BG]);


	// ---- outline entire window with a line one pixel wide
		// VsFloatRectangle margin = { 1, 1, 1, 1 };
		// hr = props_gfx_device->DrawViewportBorder( &margin, pi_props->colour[pi_props->EDGE]);


	// ---- set NULL "over panel" context
		pi_props->context_over_panel = NULL;


	// ---- begin
		hr = d3d9_device->BeginScene();

	return VS_OK;
}


INT GfxInterface::RenderViewrectX(PanelContext *gfx_context, BOOL border)
{

	// ---- accelerate
		GfxDevice *props_gfx_device = pi_props->gfx_device;
		IDirect3DDevice9 *d3d9_device = props_gfx_device->d3d9_device;


	// ---- get client adjusted viewrect
		VsFloatRectangle viewrect;
		GetClientAdjustedViewrect(&viewrect);


	// ---- set viewport
		D3DVIEWPORT9 fvp;
		fvp.X      = (DWORD)(viewrect.left+0.5);
		fvp.Y      = (DWORD)(viewrect.top+0.5);
		fvp.Width  = (DWORD)(viewrect.right - viewrect.left + 0.5);
		fvp.Height = (DWORD)(viewrect.bottom - viewrect.top + 0.5);
		fvp.MinZ   =  0;
		fvp.MaxZ   =  1;

		d3d9_device->SetViewport(&fvp);


	// INT r = gfx_context->SetInteractionStatus(FALSE);


	// ---- get user interaction with ViewRect status
		BOOL user_interaction_status = TRUE;
		INT result = gfx_context->GetInteractionStatus(&user_interaction_status);


	// IF -- user interaction -- THEN -- test and set against viewrect OR set FALSE
		BOOL over_viewrect = user_interaction_status ? pi_props->isOverViewrect() : FALSE;


	// ---- display list index
		UINT index = 0;
		gfx_context->GetDisplayListIndex(&index);
		BOOL first_panel = index == 0 ? TRUE : FALSE;


	// ---- display list top
		UINT top = 0;
		gfx_context->GetDisplayListTop(&top);
		BOOL top_panel = index == top-1 ? TRUE : FALSE;


	// ---- background
		BOOL solid = gfx_context->GetBackground();
		if ( first_panel && solid && !border )
		{
			HRESULT hr = props_gfx_device->ClearViewportBackground(
				over_viewrect ?
					pi_props->colour[pi_props->VIEWRECT_OVER] :
						pi_props->colour[pi_props->VIEWRECT]);
		}


	// ---- border
		if ( top_panel && solid && border )
		{

			BOOL selected = FALSE;
			INT result = gfx_context->GetSelected(&selected);


			// ---- get border colour
			D3DCOLOR colour = 
				selected ?
					pi_props->colour[pi_props->BORDER_ON] :
					(
						over_viewrect ?
							pi_props->colour[pi_props->BORDER_OVER] :
								pi_props->colour[pi_props->BORDER]
					);


			// ---- get border width
			VsFloatRectangle *widths =
				selected ?
					gfx_context->GetBorderOver() :
					(
						over_viewrect ?
							gfx_context->GetBorderOver() :
								gfx_context->GetBorder()
					);


			// ---- draw border
			props_gfx_device->DrawViewportBorder(widths, colour);


			// ---- draw grid (if any)
			if ( pi_props->grid_size > 0 )
				props_gfx_device->DrawViewportGrid(
					pi_props->grid_rows,
					pi_props->grid_cols,
					pi_props->grid_size,
					colour
				);

		}

	return VS_OK;
}


// ---------- RenderViewrect ----------
/*!
\brief Render Viewrect background and foreground (e.g. border, grid)
\author Gareth Edwards
\return INT - ERROR # < VS_ERROR < VS_OK
*/
INT GfxInterface::RenderViewrect(
		PanelContext *panel_context,
		BOOL render_border
	)
{

	// ---- local
		GfxDevice *props_gfx_device = pi_props->gfx_device;
		IDirect3DDevice9 *d3d9_device = props_gfx_device->d3d9_device;


	// ---- get client adjusted viewrect
		VsFloatRectangle viewrect;
		GetClientAdjustedViewrect(&viewrect);


	// ---- set viewport
		D3DVIEWPORT9 fvp;
		fvp.X      = (DWORD)(viewrect.left+0.5);
		fvp.Y      = (DWORD)(viewrect.top+0.5);
		fvp.Width  = (DWORD)(viewrect.right - viewrect.left + 0.5);
		fvp.Height = (DWORD)(viewrect.bottom - viewrect.top + 0.5);
		fvp.MinZ   =  0;
		fvp.MaxZ   =  1;
		d3d9_device->SetViewport(&fvp);


	// ---- get user interaction with ViewRect status
		BOOL user_interaction_status = TRUE;
		INT result = panel_context->GetInteractionStatus(&user_interaction_status);

		 
	// ---- user interaction -- ? test and set against viewrect OR set FALSE
		BOOL over_viewrect = user_interaction_status ? pi_props->isOverViewrect() : FALSE;


	// ---- lambda
		auto outline = [&]()
		{

			// ---- selected
				BOOL selected = FALSE;
				INT result = panel_context->GetSelected(&selected);

			// ---- echo over
				//
				// such that a panel can echo the over mode of another
				//
				BOOL echo_over = panel_context->GetEchoOver();
				result = panel_context->SetEchoOver(FALSE);

			// ---- get border colour
				D3DCOLOR colour = 
					selected ?
						pi_props->colour[pi_props->BORDER_ON] :
						(
							( over_viewrect || echo_over ) ?
								pi_props->colour[pi_props->BORDER_OVER] :
									pi_props->colour[pi_props->BORDER]
						);

			// ---- get border width
				//
				// IF selected
				//   width = over border width
				// ELSE
				//   width =
				//      IF ( over rect ) THEN
				//         over border width
				//      ELSE
				//         border width
				//
				VsFloatRectangle *width =
					selected ?
						panel_context->GetBorderOver() :
							(
								( over_viewrect || echo_over ) ?
									panel_context->GetBorderOver() :
										panel_context->GetBorder()
							);

			// ---- render border
				props_gfx_device->DrawViewportBorder(width, colour);

			// ---- render grid (if any)
				if ( pi_props->grid_size > 0 )
					props_gfx_device->DrawViewportGrid(
						pi_props->grid_rows,
						pi_props->grid_cols,
						pi_props->grid_size,
						colour
					);
		};


	// ---- display list
		UINT dspl_list_top = 0;
			panel_context->GetDisplayListTop(&dspl_list_top);
		UINT dspl_list_index = 0;   
			panel_context->GetDisplayListIndex(&dspl_list_index);

		BOOL dspl_list_first = dspl_list_index == 0 ? TRUE : FALSE;
		BOOL dspl_list_last  = dspl_list_index == dspl_list_top - 1 ? TRUE : FALSE;


	// ---- draw panel background ?
		//
		// note: gates both border and background
		//
		BOOL render_background = panel_context->GetBackground();


	// ---- draw border around view rect ?
		if ( render_border )
		{

			// ---- ONLY if last display list...
				if ( dspl_list_last )
				{

						if ( render_background )
						{
							outline();
						}

						BOOL render_outline = panel_context->GetOutline();
						if ( render_outline )
						{
							outline();
						}

				}
		}

		else
		{

			// ---- ONLY if first display list ....
				if ( dspl_list_first && render_background )
				{

					// ---- render background
						HRESULT hr = props_gfx_device->ClearViewportBackground(
							over_viewrect ?
								pi_props->colour[pi_props->VIEWRECT_OVER] :
									pi_props->colour[pi_props->VIEWRECT]);

				}

		}

	return VS_OK;
}


#ifdef VS_GFX_REPORT_LOOP
DOUBLE time_elapsed_ms_last = 0;
#endif


#ifdef VS_GFX_REPORT_LOOP
BOOL report = FALSE;
CHAR name[256];
panel_context->GetName(name);
if (!strcmp(name, "Stream"))
{
	DOUBLE time_elapsed_ms = (DOUBLE)timeGetTime();
	INT second = (INT)(time_elapsed_ms / 1000);
	INT second_last = (INT)(time_elapsed_ms_last / 1000);
	if (second != second_last)
	{
		CHAR str[128];
		sprintf(str, " ---- Stream \n");
		OutputDebugString(str);
		report = TRUE;
	}
	time_elapsed_ms_last = time_elapsed_ms;
}
#endif


// ---------- RenderViewport ----------
/*!
\brief Render Viewport background and current display list
\author Gareth Edwards
\param INT - display list handle
\param PanelContext * - parent panel context
\return INT - ERROR # < VS_ERROR < VS_OK
*/   
INT GfxInterface::RenderViewport(
		INT dspl_list_handle,
		PanelContext *panel_context
	)
{

	// =============================================================================


	// 1. check args ===============================================================


	// ---- check legal display list
	if ( dspl_list_handle <= 0 )
	{
		pi_props->sys_output->Message("RenderViewport: Illegal display list handle!");
		return VS_ERROR;
	}


	// 2. local ====================================================================


	// ---- local
	INT vs_result = 0;
	HRESULT hr = NULL;


	// ---- local object *
	GfxDevice        *props_gfx_device = pi_props->gfx_device;
	IDirect3DDevice9 *d3d9_device = props_gfx_device->d3d9_device;


	// ---- temp
	//d3d9_device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);


	// 1. empty ====================================================================


	// ---- check empty display list
		if ( dspl_list_handle == VS_OK )
		{

			// --- interaction ?
				BOOL user_interaction_status = TRUE;
				INT result = panel_context->GetInteractionStatus(&user_interaction_status);
				BOOL over_viewport = user_interaction_status ? pi_props->isOverViewport() : FALSE;


			// ---- clear background ?
				BOOL solid = panel_context->GetBackground();
				if ( solid )
				{

					// ---- set color
						D3DCOLOR panel_background_color =
							over_viewport ?
								( panel_context->GetUseViewportColor() ?
									panel_context->GetViewportOverD3dColor() :
										pi_props->colour[pi_props->VIEWPORT_OVER]
								) :
								( panel_context->GetUseViewportColor() ?
									panel_context->GetViewportD3dColor() :
										pi_props->colour[pi_props->VIEWPORT]
								) ;


					// ---- over viewport ?
						if ( over_viewport )
							pi_props->context_over_panel = panel_context;


					// ---- clear
						hr = props_gfx_device->ClearViewportBackground(panel_background_color);

				}

			// ---- pick
				BOOL picked =
						over_viewport ?
							pi_props->isPickPanel(panel_context->GetHandle()) :
								FALSE;

			return VS_OK;
		}


	// 3. setup ====================================================================


	// ---- default viewport, projection, view matrix and world matrix
		vs_result = RenderSetupForViewport(panel_context);


	// ---- user interaction ? -- test and set against viewport OR set FALSE
		BOOL user_interaction_status = TRUE;
		vs_result = panel_context->GetInteractionStatus(&user_interaction_status);
		BOOL over_viewport = user_interaction_status ? pi_props->isOverViewport() : FALSE;


	// ---- render to image ? -- never OVER viewport, so always set FALSE
		BOOL render_to_image_flag = TRUE;
		vs_result = panel_context->GetRenderToImageFlag(&render_to_image_flag);
		over_viewport = over_viewport ? ( render_to_image_flag ? FALSE : TRUE ) : FALSE;


	// ---- display list
		UINT dspl_list_top = 0;
			panel_context->GetDisplayListTop(&dspl_list_top);
		UINT dspl_list_index = 0;
			panel_context->GetDisplayListIndex(&dspl_list_index);
		
		BOOL dspl_list_first = dspl_list_index == 0 ? TRUE :  FALSE;
		BOOL dspl_list_last  = dspl_list_index == dspl_list_top - 1 ? TRUE : FALSE; 


	// ---- clear background ?
		BOOL solid = panel_context->GetBackground();
		if ( dspl_list_first && solid )
		{

			// ---- set color
				D3DCOLOR panel_background_color =
					over_viewport ?
						( panel_context->GetUseViewportColor() ?
							panel_context->GetViewportOverD3dColor() :
								pi_props->colour[pi_props->VIEWPORT_OVER]
						) :
						( panel_context->GetUseViewportColor() ?
							panel_context->GetViewportD3dColor() :
								pi_props->colour[pi_props->VIEWPORT]
						) ;


			// ---- over viewport ?
				//
				// ? set gfx_contex "over" panel
				//
				// Note: Provides for focus that is neither immediate (e.g. pick
				// viewport, pick element) OR persistant (e.g. mouse down on
				// viewport, mouse down on element), such as mouse wheel click
				// (or roll-over) that requires over viewport gfx context.
				//
				if ( over_viewport )
					pi_props->context_over_panel = panel_context;


			// ---- clear
				hr = props_gfx_device->ClearViewportBackground(panel_background_color);

		}


	// ---- frame texture_border ?
		D3DCOLOR frame_texture_border_color = 0x00000000;
		if ( pi_props->frame_texture_border )
		{
			// ---- over viewport -- ? colour over -- ELSE colour on
			frame_texture_border_color = over_viewport ?
				pi_props->frame_texture_border_color_over :
					pi_props->frame_texture_border_color;
		}


	// ---- texture flag
		BOOL sys_textures_flag = pi_props->sys_context->GetBool(SysContext::TEXTURES);


	// ---- output to log file
		BOOL log = pi_props->sys_output != NULL ? pi_props->sys_output->GetLog() : FALSE;


	// ---- NO updated font
		BOOL updated_font = FALSE;


	// ---- NO picked element
		BOOL picked_an_element = FALSE;



	// 4. 3D states ================================================================


		// ---- lights
			hr = SetAmbient(
					pi_props->gfx_ambient->color.r,
					pi_props->gfx_ambient->color.g,
					pi_props->gfx_ambient->color.b
				);


		// ---- fog
			hr = SetFog();


		// ---- shaders
			FLOAT shader_param[8];
			INT shader_index = 0;
			#if VS_MAX_SHADERS == 1
			hr = pi_props->gfx_shaders[shader_index]->GetParam(shader_param);
			hr = pi_props->gfx_shaders[shader_index]->SetBCS(shader_param[0], shader_param[1], shader_param[2]);
			hr = pi_props->gfx_shaders[shader_index]->SetMatrices();
			#endif


		// ---- view matrix
			UINT display_list_view_matrix_index = 999;


	// 5. display list ? ===========================================================

	PI_Element *display_list = (PI_Element*)dspl_list_handle;
	if ( display_list->type == PI_Element::LIST )
	{

		// ---- pick switched on radio (or channel) flag
			BOOL pick_switched_on_radio_button =
				display_list->element_context.GetPickOnRadioButton();


		// ---- log ? ? pre-report LIST setup (if any)
			# if defined(DISPLAY_LIST_DEBUG)
			if ( log )
			{
				PI_Element *element = display_list->first_child;
				CHAR text[VS_MAXCHARLEN];
				while ( element )
				{
					if ( !element->element_context_configured )
					{
						sprintf(
								text,
								"Setup display List %s ( %d )",
								display_list->element_context.GetName().c_str(),
								(INT)display_list
							);
						OutputDebugString(text);
						break;
					}
					element = element->next;
				}

			}
			#endif


		// 5.1 init supplied element info ==============================================

		// ---- initialise SUPPLIED client adjusted viewport bounds
			VsFloatRectangle viewport;
			vs_result = GetClientAdjustedViewport(&viewport);
			pi_props->supplied_gfx_float_parameter[VS_PANEL_LEFT]      = viewport.left;
			pi_props->supplied_gfx_float_parameter[VS_PANEL_TOP]       = viewport.top;
			pi_props->supplied_gfx_float_parameter[VS_PANEL_RIGHT]     = viewport.right;
			pi_props->supplied_gfx_float_parameter[VS_PANEL_BOTTOM]    = viewport.bottom;
			pi_props->supplied_gfx_float_parameter[VS_BUFFER_WIDTH]    = (FLOAT)GetBackBufferWidth();
			pi_props->supplied_gfx_float_parameter[VS_BUFFER_HEIGHT]   = (FLOAT)GetBackBufferHeight();


		// 5.2 process display lists ===================================================

		//
		// Painters algorithm: Render first to last so that in a 2d element stack -
		// such as in a button - only the first element (e.g. background) is picked
		// and only this event invokes a callback via PI_Props isPickElement method
		//

		PI_Element *element = display_list->first_child;
		while ( element )
		{

			// ---- context
				ElmContext *element_context = &element->element_context;


			// 5.2.1 visible ? =============================================================

			if ( element->element_context.GetVisible() && element->element_context.GetDisplay() )
			{

				// 5.2.2.1 apply tint actions ==================================================

					//   ONLY tint (tabbed interface) text buttons generate actions
					//
					//   So, if a Tint button is picked in this function it invokes a callback
					//   that invokes Tint body callback functions UpdateModel and UpdateText.
					//
					//   The latter UpdateText function then invokes the Tint body
					//   functions UpdateActions, UpdateText, UpdateValue.
					// 
					//   These actions are processed the next time this function is invoked.
					//
					//   Then the action list is zeroed.
					//

					if (
							element_context->GetVersion() == ElmContext::VERSION_TEXT &&
							element_context->GetActions() > 0
						)
					{

						// ---- previously picked
							//
							//   Next in the display list is the background element,
							//   the element that WAS previously picked!!!
							//
							//   Button display list element sequence is:
							//
							//   ElmContext::VERSION_TEXT         text       (2) indexed for Tint body update functions
							//   ElmContext::VERSION_BACKGROUND   background (0)
							//   ElmContext::VERSION_ICON         icon       (1)
							//   ElmContext::VERSION_OUTLINE      outline    (3)
							//
							PI_Element *element_previously_picked = element->next;
							ElmContext *element_previously_contex = &element_previously_picked->element_context;


						// ---- action data appended to parameter data
							FLOAT *float_param = element_context->GetFloatParameter();
							float_param += 20;


						// ---- apply actions [index/id, value], [], ...
							//   
							UINT index = 0;
							UINT num_actions = element_context->GetActions();
							for ( UINT action = 0; action < num_actions; action++ )
							{

								// data
								FLOAT *data = &float_param[index];

								// !! --- per previously picked --- !!
								switch ( element_previously_contex->GetBehaviour() )
								{
									case ElmContext::BEHAVIOUR_RADIO:
									case ElmContext::BEHAVIOUR_RADIO_ALL:
									case ElmContext::BEHAVIOUR_CLUNK: // TBC
										{
											UINT action_index = (UINT)*(data + 0);
											UINT action_value = (UINT)*(data + 1);
											PI_Element *e = display_list->first_child;
											while ( e )
											{
												vs_system::ElmContext *e_context = &e->element_context;
												if ( e_context->GetId() == action_index )
												{
													switch ( e_context->GetBehaviour() )
													{
														case ElmContext::BEHAVIOUR_CLICK:
															e->element_context.SetMultiActive(action_value == 1 ? TRUE : FALSE, 0);
															break;
														default:
															break;
													}
												}
												e = e->next;
											}
										}
										break;
									default:
										break;
								}

								// increment
								index += 2;
							}

							// zero # of actions
							element_context->SetActions(0);

					}   // ---- apply tint actions


				// 5.2.2.2 setup 3D lighting ===================================================
	
					BOOL lit = element_context->GetLit();
					if ( lit )
					{
						hr = SetLighting(TRUE);
						UINT u = element_context->GetMaterialIndex();
						UINT use_rgba = element_context->GetUseRGBA();
						hr = SetMaterial((INT)u);
					}
					else
					{
						hr = SetLighting(FALSE);
					}


				// 5.2.2.3 setup 3D view matrix ================================================

					//
					// TBD change only if changed, ditto matrix, etc...
					//

					UINT element_view_matrix_index = element_context->GetViewMatrixIndex();
					if ( element_view_matrix_index != display_list_view_matrix_index )
					{
						hr = SetViewMatrix(element_view_matrix_index);
						hr = CreateWorldSpacePickRay();
						display_list_view_matrix_index = element_view_matrix_index;
					}


				// 5.2.2.4 setup 3D world matrix ===============================================

					BOOL use_matrix = FALSE;
					vs_result = element_context->GetUseMatrix(&use_matrix);
					if ( use_matrix )
					{
						hr = element_context->GetMatrix(&pi_props->world_matrix);
							memcpy(
									&pi_props->d3d_world_matrix.m[0][0],
									&pi_props->world_matrix.m[0][0],
									16 * sizeof(FLOAT)
								);
						hr = d3d9_device->SetTransform(D3DTS_WORLD, &pi_props->d3d_world_matrix);
					}


				// 5.2.2.5 (re)configure ? =====================================================

					if ( !element->element_context_configured )
					{

						// ---- set supplied data
							vs_result = element_context->SetGfxFloatParameter(pi_props->supplied_gfx_float_parameter);
							vs_result = element_context->SetGfxFloatBuffer(pi_props->supplied_gfx_float_buffer);


						// ---- (re)setup element
							vs_result = element->Setup(d3d9_device);
							# if defined(DISPLAY_LIST_DEBUG)
							if ( log )
							{
								CHAR text[VS_MAXCHARLEN];
								sprintf(text, "              Element %s ( %d )", element_context->GetName().c_str(), (INT)element);
								OutputDebugString(text);
							}
							#endif


						// ---- font height changed ? -- update font height
							UINT font_height = element_context->GetFontHeight();
							if ( font_height != 0 )
							{
								GfxFont *font = pi_props->gfx_fonts->GetFont(element_context->GetFontName());
								if ( font != NULL) { font->height = font_height; updated_font=TRUE; }
								element_context->SetFontHeight(0);
							}

					}


				// 5.2.2.6 behaviour ===========================================================

					UINT behaviour = element_context->GetBehaviour();


				// 5.2.2.7 over interaction ? ==================================================

					element->over = FALSE;
					if ( over_viewport && behaviour != ElmContext::BEHAVIOUR_NULL )
					{
						switch ( element->dimension )
						{
							case 2:
								if ( element_context->GetPick2D() )
								{
									element->over = pi_props->IsOver2DElement(element);
								}
								break;
							case 3:
								if ( element_context->GetPick3D() )
								{
									element->over = pi_props->IsOver3DElement(element);
								}
								break;
							default:
								break;
						}
					}


				// 5.2.2.8 picked ? ============================================================


				// ---- over viewport (and user interaction) ?
					//if ( over_viewport && behaviour > ElmContext::BEHAVIOUR_NONE )


				// ---- over and possible interaction ?
					element->picked = FALSE;
					if ( element->over && behaviour > ElmContext::BEHAVIOUR_NONE )
					{

						// ---- picked ?
							element->picked = pi_props->isPickElement((INT)element);
							if (
									element->picked &&
									! pick_switched_on_radio_button &&
									! picked_an_element
								)
							{

								// ---- really important tidy up ---- !!!
									//
									//   if behaviour == RADIO/RADIO_ALL/CLICK then cancel pick
									//      as these are "do once" actions
									//
									//   if behaviour == CLICK and NOT MultiActive then cancel pick
									//      as this button has been set off via Tint actions
									//
									//   note: element picked callback below via
									//      pi_props->CallbackElement((INT)element)
									//
									switch ( behaviour )
									{
										case ElmContext::BEHAVIOUR_RADIO:
										case ElmContext::BEHAVIOUR_RADIO_ALL:
											if ( element_context->GetSwitchedOn() )
											{
												if (
													element_context->GetSubBehaviour()
													!= ElmContext::SUB_BEHAVIOUR_RADIO_SWITCH
													)
												{
													element->picked = FALSE;
												}
											}
											break;
										case ElmContext::BEHAVIOUR_CLICK:
											if ( !element_context->GetMultiActive() )
											{
												element->picked = FALSE;
											}
											break;
										default:
											break;
									}


								// ---- NOT picked - BUT element had been picked... "HERE"
									if ( ! element->picked )
										picked_an_element = TRUE;

							}
					}


				// 5.2.2.9 on ? ================================================================

				// ---- already ON ?
					BOOL switched_on_status = element_context->GetSwitchedOn();
					BOOL switched_on_prev_status = switched_on_status;


				// 5.2.2.10 update on behaviour ================================================

					switch ( behaviour )
					{
						// does nothing, which is not the same as...
						case ElmContext::BEHAVIOUR_NULL:
							break;

						// ...does not a lot
						case ElmContext::BEHAVIOUR_NONE:
							if ( element->picked )
							{
								switched_on_status = TRUE;
								element_context->SetSwitchedOn(switched_on_status);
							}
							break;

						// ON & OFF with no intermediate state
						case ElmContext::BEHAVIOUR_CLUNK:
							if ( element->picked )
							{
								switched_on_status = TRUE;
								element_context->SetSwitchedOn(switched_on_status);
							}
							break;

						// BUTTON down
						case ElmContext::BEHAVIOUR_UNCLUNK:
							{
								element_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_CLUNK);
								switched_on_status = FALSE;
								element_context->SetSwitchedOn(switched_on_status);
							}
							break;

						// RADIO all elements within a display list
						case ElmContext::BEHAVIOUR_RADIO:

							if ( element->picked )
							{
								if ( switched_on_status )
								{
									UINT counter = 0;
									UINT group_id = element_context->GetGroupId();
									UINT id = element_context->GetId();
									PI_Element *e = display_list->first_child;
									while ( e )
									{
										vs_system::ElmContext *e_context = &e->element_context;
										if ( e_context->GetGroupId() == group_id )
										{
											if ( e_context->GetId() != id )
											{
												if (e_context->GetBehaviour() == ElmContext::BEHAVIOUR_RADIO)
												{
													counter++;
												}
											}
										}
										e = e->next;
									}
									if ( counter == 0 )
									{
										switched_on_status = FALSE;
										element_context->SetSwitchedOn(switched_on_status);
									}
								}
								else
								{
									switched_on_status = TRUE;
									element_context->SetSwitchedOn(switched_on_status);

									UINT group_id = element_context->GetGroupId();
									UINT id = element_context->GetId();
									PI_Element *e = display_list->first_child;
									while ( e )
									{
										vs_system::ElmContext *e_context = &e->element_context;
										if ( e_context->GetGroupId() == group_id )
										{
											if ( e_context->GetId() != id )
											{
												if ( e_context->GetBehaviour() == ElmContext::BEHAVIOUR_RADIO )
												{
													e->element_context.SetSwitchedOn(FALSE);
												}
											}
										}
										e = e->next;
									}
								}
							}
							break;


						// RADIO a channel (group) within a display list
						case ElmContext::BEHAVIOUR_RADIO_ALL:

							if ( element->picked )
							{
								if ( switched_on_status )
								{

									// count # of other radio buttons
									UINT counter = 0;
									UINT group_id = element_context->GetGroupId();
									UINT id = element_context->GetId();
									PI_Element *e = display_list->first_child;
									while ( e )
									{
										vs_system::ElmContext *e_context = &e->element_context;
										if ( e_context->GetGroupId() == group_id )
										{
											if ( e_context->GetId() != id )
											{
												if (e_context->GetBehaviour() == ElmContext::BEHAVIOUR_RADIO_ALL)
												{
													counter++;
												}
											}
										}
										e = e->next;
									}

									// if none then treat as a switch
									if ( counter == 0 )
									{
										switched_on_status = FALSE;
										element_context->SetSwitchedOn(switched_on_status);
									}

								}
								else
								{

									// switch ON
									switched_on_status = TRUE;
									element_context->SetSwitchedOn(switched_on_status);

									// find
									UINT group_id = element_context->GetGroupId();
									UINT id = element_context->GetId();

									PI_Element *e = display_list->first_child;
									while ( e )
									{
										vs_system::ElmContext *e_context = &e->element_context;
										if ( e_context->GetGroupId() == group_id )
										{
											if ( e_context->GetId() == id )
											{

												// last
												PI_Element *el = e->last;
												while ( el )
												{
													vs_system::ElmContext *el_context = &el->element_context;
													if ( el_context->GetBehaviour() == ElmContext::BEHAVIOUR_RADIO_ALL)
													{
														if ( el_context->GetId() != id )
															el_context->SetSwitchedOn(FALSE);
													}
													else
													{
														break;
													}
													el = el->last;
												}

												// next
												PI_Element *en = e->next;
												while  (en )
												{
													vs_system::ElmContext *en_context = &en->element_context;
													if ( en_context->GetBehaviour() == ElmContext::BEHAVIOUR_RADIO_ALL)
													{
														if ( en_context->GetId() != id )
															en_context->SetSwitchedOn(FALSE);
													}
													else
													{
														break;
													}
													en = en->next;
												}

												break;

												//if ( e_context->GetBehaviour() == ElmContext::BEHAVIOUR_RADIO )
												//{
												//	e->element_context.SetSwitchedOn(FALSE);
												//}

											}
										}
										
										e = e->next;
									
									}
								}
							}
							break;

						// ON or OFF
						case ElmContext::BEHAVIOUR_SWITCH:

							if ( element->picked )
							{
								switched_on_status = switched_on_status ? FALSE : TRUE;
								UINT group_id = element_context->GetGroupId();
								UINT id = element_context->GetId();
								PI_Element *e = display_list->first_child;
								while ( e )
								{
									vs_system::ElmContext *e_context = &e->element_context;
									if ( e_context->GetGroupId() == group_id )
										if ( e_context->GetId() == id )
											if ( e_context->GetBehaviour() == ElmContext::BEHAVIOUR_SWITCH )
											{
												e->element_context.SetSwitchedOn(switched_on_status);
											}
									e = e->next;
								}
							}
							break;

						default:
							break;

					}


				// 5.2.2.11 pick element callback ==============================================

					// ---- element picked ?
					//    IF callback THEN
					//       invoke callback
					//
					// note: after updating switched on status
					//
					if ( behaviour != ElmContext::BEHAVIOUR_NULL && element->picked )
					{
						pi_props->CallbackElement((INT)element);

						// ---- swi tched on previuosly -- ? flip
						//
						// note: allows callback functions to test for a
						// second pick of an element...
						//
						if ( switched_on_prev_status )
							element_context->SetSwitchedOnPreviously(FALSE);
						else
							element_context->SetSwitchedOnPreviously(TRUE);
					}


				// 5.2.2.12 set solid OR wireframe
					hr = d3d9_device->SetRenderState(
							D3DRS_FILLMODE, element_context->GetSolid() ?
								D3DFILL_SOLID :
									  D3DFILL_WIREFRAME);


				// 5.2.2.14 set colour vertex TRUE OR FALSE
					hr = d3d9_device->SetRenderState(D3DRS_COLORVERTEX,
							element_context->GetColourVertex() );


				// 5.2.2.15 texture ? ==========================================================

				BOOL element_on = TRUE;
				BOOL texture_on = element_context->GetTextured() && sys_textures_flag;
				if ( texture_on )
				{

					// 5.2.2.15.1 setup texture multi state  =======================================

					// ---- multi-state texturing ?
						UINT texture_page_reference = VS_ERROR;
						if ( element_context->GetMultiState() )
						{

							// ---- get multi-state atlas page references
								UINT pr[VS_MAX_ELM_ATLAS_PAGE_REFERENCES];
								element_context->GetAtlasPages(pr);

							// ---- multi state ?
								// IF multi state texturing is active
								//    IF switched on THEN
								//       select texture ON index (#2)
								//    ELSE 
								//       IF over THEN
								//          select texture OVER index (#3)
								//       ELSE
								//          select texture OFF index (#1)
								//       ENDIF
								//    ENDIF
								// ELSE
								//    select multi state NULL texture index (#0)
								// ENDIF

								texture_page_reference =
									element_context->GetMultiActive() ?
										(
											switched_on_status ? pr[2] : ( element->over ? pr[3] : pr[1] )
										) : pr[element_context->GetMultiNull()];

						}
						// ---- get single-state atlas page reference
						else
						{
							element_context->GetAtlasPage(0, &texture_page_reference);
						}


					// 5.2.2.15.2 set texture states and blending ==================================

					// ---- get page texture from atlas
						IDirect3DTexture9 *d3d9_texture = pi_props->gfx_atlas->GetTexture(texture_page_reference);


					// ---- valid texture ?
					if ( d3d9_texture != NULL )
					{

						// ---- shader ?
						if ( element_context->GetShaded() )
						{
							#if VS_MAX_SHADERS == 1
								shader_index = 0;
							#else
								shader_index = element_context->GetShaderIndex();
							#endif
							hr = pi_props->gfx_shaders[shader_index]->SetMatrices();
							hr = pi_props->gfx_shaders[shader_index]->GetParam(shader_param);
							hr = pi_props->gfx_shaders[shader_index]->SetBCS(shader_param[0], shader_param[1], shader_param[2]);
							hr = pi_props->gfx_shaders[shader_index]->SetBCSEnabled(&pi_props->d3d_world_matrix);
						}
						 

						// ---- set texture stage 0:
							hr = d3d9_device->SetTexture (0, d3d9_texture);


						// ---- set Texture States 
							//
							//      Also see: D3DTADDRESS_BORDER, D3DTADDRESS_WRAP,
							//      D3DTADDRESS_CLAMP and D3DTADDRESS_MIRROR
							//

							if (element_context->GetTextureMirror())
							{
								hr = d3d9_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
								hr = d3d9_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
							}

							// ---- ELSE transparent borders AND NO repeat
							else
							{
								hr = d3d9_device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
								hr = d3d9_device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
								hr = d3d9_device->SetSamplerState(0, D3DSAMP_BORDERCOLOR, frame_texture_border_color);
							}


						// ---- texture AND alpha blended
							if ( element_context->GetAlphaBlending() )
							{

								// ---- final Color = (Source Color x Source Factor)
								//                      Operation (Dest Color x Dest Factor)


								// ---- set stage 0:
									hr = d3d9_device->SetTextureStageState(0,D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
									hr = d3d9_device->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTA_TEXTURE);
									hr = d3d9_device->SetTextureStageState(0,D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
									hr = d3d9_device->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTA_TEXTURE);


								// ---- set alpha blending
									hr = d3d9_device->SetRenderState(D3DRS_SRCBLEND,           D3DBLEND_SRCALPHA);
									hr = d3d9_device->SetRenderState(D3DRS_DESTBLEND,          D3DBLEND_INVSRCALPHA);


								// ---- turn ON alpha blending
									hr = d3d9_device->SetRenderState(D3DRS_ALPHABLENDENABLE,   TRUE);

								//
								//hr = d3d9_device->SetRenderState(D3DRS_BLENDOP,            D3DBLENDOP_ADD);
							}

						// ---- set non-blended states
						else
						{


							DWORD lighting = 0;
							hr = d3d9_device->GetRenderState(D3DRS_LIGHTING, &lighting);
							if ( lighting )
							{

								// ---- Stage 0:
									hr = d3d9_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
									hr = d3d9_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
									hr = d3d9_device->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);

								// ---- Stage 1:
									hr = d3d9_device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
									hr = d3d9_device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
									hr = d3d9_device->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_ADD);
							}
							else
							{

								// ---- Stage 0:
									hr = d3d9_device->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
									hr = d3d9_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
									hr = d3d9_device->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
									hr = d3d9_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
							}


							// ---- turn OFF alpha blending
								hr = d3d9_device->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
						}

					}

					// ---- NULL texture
					else
					{
						hr = d3d9_device->SetTexture (0, NULL);
						element_on = FALSE;
					}

				}

				// ---- texture is OFF
				else
				{


					// ---- set NULL texture
						hr = d3d9_device->SetTexture (0, NULL);


					// ---- disable Texture States
						hr = d3d9_device->SetTextureStageState(0,D3DTSS_COLOROP,   D3DTOP_DISABLE);
						hr = d3d9_device->SetTextureStageState(0,D3DTSS_COLORARG1, D3DTOP_DISABLE);
						hr = d3d9_device->SetTextureStageState(0,D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
						hr = d3d9_device->SetTextureStageState(0,D3DTSS_ALPHAARG1, D3DTOP_DISABLE);


					// ---- turn alpha blending ON ?
						if ( element_context->GetAlphaBlending() )
						{
							//hr = d3d9_device->SetRenderState(D3DRS_ZENABLE,           FALSE);
							hr = d3d9_device->SetRenderState(D3DRS_ALPHABLENDENABLE,  TRUE);
							hr = d3d9_device->SetRenderState(D3DRS_BLENDOP,           D3DBLENDOP_ADD);
							hr = d3d9_device->SetRenderState(D3DRS_SRCBLEND,          D3DBLEND_SRCALPHA);
							hr = d3d9_device->SetRenderState(D3DRS_DESTBLEND,         D3DBLEND_INVSRCALPHA);
						}

					// ---- turn alpha blending OFF
						else
						{
							//hr = d3d9_device->SetRenderState(D3DRS_ZENABLE,           TRUE);
							hr = d3d9_device->SetRenderState(D3DRS_ALPHABLENDENABLE,  FALSE);
						}

				}


				// 5.2.2.16 element on ? =======================================================

				if ( element_on )
				{

					// 5.2.2.16.1 cull ? ===========================================================

						switch ( element_context->GetCullMode() )
						{
							default:
							case ElmContext::CULL_NONE:
								hr = d3d9_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
								break;
							case ElmContext::CULL_CW:   hr = d3d9_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);   break;
							case ElmContext::CULL_CCW:  hr = d3d9_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);  break;
						}


					// 5.2.2.16.2 set colour vertex ================================================

						hr = d3d9_device->SetRenderState(
								D3DRS_COLORVERTEX,
									element->element_context.GetColourVertex());

					
					// 5.2.2.16.3 set fvf and stream source ========================================

						hr = d3d9_device->SetFVF( element_context->GetVertexFormat() );
						hr = d3d9_device->SetStreamSource(
								0,
								element->vertex_buffer,
								0,
								element_context->GetVertexSize()*sizeof(float)
							);
				

					// 5.2.2.16.4 render primitive =================================================

					// ---- version == TEXT ?
					if ( element_context->GetVersion() == vs_system::ElmContext::VERSION_TEXT )
					{

						// ---- outline ? -- draw non-indexed primitive 
							if ( element_context->GetTextOutline() )
							{
								hr = d3d9_device->DrawPrimitive(
										(D3DPRIMITIVETYPE)element_context->GetPrimitiveType(),
										0,
										element_context->GetPrimitiveCount()
									);
							}


						// ---- no font ? -- get
							if ( element->font == NULL )
							{
								std::string font_name = element_context->GetFontName();
								element->font = pi_props->gfx_fonts->GetFont(font_name);
							}


						// ---- set multi state index
							UINT multi_state_index =
								element_context->GetMultiActive() ?
									( switched_on_status ? 2 : (element->over ? 3 : 1) ) :
										element_context->GetMultiNull();


						// ---- draw
							D3DCOLOR d3d_color =
								element_context->GetUseRGBA() ?
									(D3DCOLOR)element_context->GetRGBA() :
										pi_props->colour[pi_props->TEXT + multi_state_index];

								if ( element_context->GetTextVertical() )
									element->DrawTextVertical(d3d_color);
								else
									element->DrawText(d3d_color);

					}

					// ---- version != TEXT ?
					else
					{

						// ---- indices ? -- draw indexed primitives
						UINT index_count = element_context->GetIndexCount();
						if ( index_count > 0 )
						{
							hr = d3d9_device->SetIndices(element->index_buffer);
							hr = d3d9_device->DrawIndexedPrimitive(
									(D3DPRIMITIVETYPE)element_context->GetPrimitiveType(),
									0, // Base vertex index
									0, // Min vertex index
									element_context->GetVertexCount(),
									0, // Start index
									element_context->GetPrimitiveCount()
								);
						}

						// ---- draw non-indexed primitives
						else
						{
							hr = d3d9_device->DrawPrimitive(
									(D3DPRIMITIVETYPE)element_context->GetPrimitiveType(),
									0,
									element_context->GetPrimitiveCount()
								);

						}
				
					} // ---- 5.2.2.16.4 render primitive

				} // ---- 5.2.2.16 element on ?


				// 5.2.2.17 disable shader ? ===================================================


					if ( element_context->GetShaded() )
					{
						#if VS_MAX_SHADERS == 1
							shader_index = 0;
						#else
							shader_index = element_context->GetShaderIndex();
						#endif
						hr = pi_props->gfx_shaders[shader_index]->SetBCSDisabled();
					}


				// 5.2.2.18 restore default world matrix ? =====================================

					if ( use_matrix )
					{  
						d3d9_device->SetTransform(D3DTS_WORLD, &pi_props->d3d_identity_matrix);
					}


			} // ---- 5.2.1 visible ?

			// ---- next
				element = element->next;

		} // ---- 5.2 process elements


		// 5.3 re-setup updated fonts ==================================================

			if ( updated_font )
			{
				INT cleanup_fonts = pi_props->gfx_fonts->Cleanup();
				INT setup_fonts = pi_props->gfx_fonts->Setup(d3d9_device);
			}


		// 5.4 pick panel ? ============================================================

		// ---- NOT picked an element AND render panel background ?
			//
			//    IF
			//       last display list AND
			//		 background solid AND
			//       element NOT picked
			//    THEN
			//       invoke handle_pick_panel function
			//
			if (
					dspl_list_last &&
						solid &&
							! picked_an_element					
				)
			{
				BOOL picked =
					over_viewport ?
						pi_props->isPickPanel(panel_context->GetHandle()) :
							FALSE;
			}

		return VS_OK;

	} // ---- 5. display list ?


	return VS_ERROR;
}


// ---------- RenderSetupForViewport ----------
/*!
\brief Render setup of viewport, projection, view and world matrices
\author Gareth Edwards
\return INT - ERROR # < VS_ERROR < VS_OK
*/
INT GfxInterface::RenderSetupForViewport(
		PanelContext *panel_context
	)
{

	// ---- local
		INT result = 0, hr = 0;
		GfxDevice *props_gfx_device = pi_props->gfx_device;
		IDirect3DDevice9 *d3d9_device = props_gfx_device->d3d9_device;


	// ---- get client adjusted viewport
		VsFloatRectangle viewport;
		hr = GetClientAdjustedViewport(&viewport);


	// ---- set viewport
		D3DVIEWPORT9 fvp;
		fvp.X      = (DWORD)(viewport.left+0.5);
		fvp.Y      = (DWORD)(viewport.top+0.5);
		fvp.Width  = (DWORD)(viewport.right - viewport.left + 0.5);
		fvp.Height = (DWORD)(viewport.bottom - viewport.top + 0.5);
		fvp.MinZ   = 0;
		fvp.MaxZ   = 1;
		hr = d3d9_device->SetViewport(&fvp);


	// ---- set drawing mode
		hr = d3d9_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


	// ---- set projection, view and world matrices
	switch ( INT type = panel_context->GetTypeId() )
	{

		// ---- perspective
			case VS_PERSPECTIVE:
			{

				// ---- view context
					CHAR  *view      = panel_context->GetView();
					FLOAT *rotate    = panel_context->GetRotate();
					FLOAT *translate = panel_context->GetTranslate();


				// ---- perspective context
					FLOAT *perspective = panel_context->GetPerspective();


				// ---- proportional zoom
					// e.g. for spherical pano, use new radii difference / original radii difference
					// e.g. for spherical pano, use new fov / original fov
					// FLOAT prop_zoom = 1.0;
					// FLOAT ratio = (FLOAT)fvp.Width / (FLOAT)fvp.Height * prop_zoom;


				// ---- set projection matrix
					FLOAT ratio = (FLOAT)fvp.Width / (FLOAT)fvp.Height;
					D3DXMATRIX projMatrix;
					D3DXMatrixPerspectiveFovLH(
							&projMatrix,
							perspective[0]*(FLOAT)(VS_RADIANS_PER_DEGREE),	// Vertical FOV
							ratio,											// Aspect Ratio
							perspective[1],									// Z Near
							perspective[2]									// Z Far
						);
					hr = d3d9_device->SetTransform(D3DTS_PROJECTION, &projMatrix);


				// ---- initialise "no roll" default view matrix zero
					D3DXMATRIX viewMatrix, rotY, rotX, tran;
					D3DXMatrixIdentity( &viewMatrix);
					D3DXMatrixRotationX( &rotX, rotate[0] * (FLOAT)(VS_RADIANS_PER_DEGREE));
					D3DXMatrixRotationY( &rotY, rotate[1] * (FLOAT)(VS_RADIANS_PER_DEGREE));
					D3DXMatrixTranslation(&tran, translate[0], translate[1], translate[2]);

					D3DXMatrixMultiply( &viewMatrix,  &tran,  &viewMatrix);
					D3DXMatrixMultiply( &viewMatrix,  &rotX,  &viewMatrix);
					D3DXMatrixMultiply( &viewMatrix,  &rotY,  &viewMatrix);
				

				// ---- store & set default view matrix zero
					pi_props->gfx_view_matrices->Set(0, &viewMatrix);
					result = SetViewMatrix(0);
					//hr = d3d9_device->SetTransform  ( D3DTS_VIEW, &viewMatrix );


				// ---- set identity world matrix
					D3DXMATRIX worldMatrix;
					D3DXMatrixIdentity( &worldMatrix);
					hr = d3d9_device->SetTransform( D3DTS_WORLD, &worldMatrix);


			}
			break;

		// ---- orthogonal 
			case VS_ORTHO_FRONT:
			case VS_ORTHO_BACK:
			case VS_ORTHO_TOP:
			case VS_ORTHO_BOTTOM:
			case VS_ORTHO_LEFT:
			case VS_ORTHO_RIGHT:
			{

				// ---- orthogonal context
					FLOAT *orthogonal = panel_context->GetOrthogonal();


				// ---- view context
					FLOAT *rotate    = panel_context->GetRotate();
					FLOAT *translate = panel_context->GetTranslate();


				// ---- set projection
					D3DXMATRIX projMatrix;
					FLOAT wby2   =  (FLOAT)fvp.Width  / 2;
					FLOAT hby2   =  (FLOAT)fvp.Height / 2;
					FLOAT left   = (-wby2 * orthogonal[2] + orthogonal[0] * orthogonal[2]);
					FLOAT right  = ( wby2 * orthogonal[2] + orthogonal[0] * orthogonal[2]);
					FLOAT bottom = (-hby2 * orthogonal[2] + orthogonal[1] * orthogonal[2]);
					FLOAT top    = ( hby2 * orthogonal[2] + orthogonal[1] * orthogonal[2]);
					D3DXMatrixOrthoOffCenterRH (&projMatrix,
							left, right, bottom, top,
							orthogonal[3],
							orthogonal[4]
						);
					hr = d3d9_device->SetTransform(D3DTS_PROJECTION, &projMatrix);


				// ----initialise orthogonal view rotation options 
					float r[6][2] = 
					{
						{   0,   0 }, // front
						{   0, 180 }, // back
						{  90,   0 }, // top
						{ -90,   0 }, // bottom
						{   0,  90 }, // left
						{   0, -90 }, // right
					};


				// ---- initialise default view matrices
					D3DXMATRIX rotX, rotY, tran;
					D3DXMatrixRotationX(
							&rotX,
							(r[type - 2][0] + rotate[0]) *
							(FLOAT)(VS_RADIANS_PER_DEGREE)
						);
					D3DXMatrixRotationY(
							&rotY,
							(r[type - 2][1] + rotate[1]) *
							(FLOAT)(VS_RADIANS_PER_DEGREE)
						);
					D3DXMatrixTranslation(&tran, translate[0], translate[1], translate[2]);


				// ---- set default view matrix
					D3DXMATRIX viewMatrix;
					D3DXMatrixIdentity( &viewMatrix);
					D3DXMatrixMultiply( &viewMatrix,  &tran,  &viewMatrix);
					D3DXMatrixMultiply( &viewMatrix,  &rotX,  &viewMatrix);
					D3DXMatrixMultiply( &viewMatrix,  &rotY,  &viewMatrix);
					

				// ---- store &  set default view matrix zero
					pi_props->gfx_view_matrices->Set(0, &viewMatrix);
					result = SetViewMatrix(0);
					//hr = d3d9_device->SetTransform(D3DTS_VIEW, &viewMatrix);


				// ---- set world matrix
					D3DXMATRIX worldMatrix;
					D3DXMatrixIdentity( &worldMatrix);
					hr = d3d9_device->SetTransform( D3DTS_WORLD, &worldMatrix);

			}
			break;


		// ---- huh???
			default:
				return VS_ERROR;
	}


	// ---- set viewport screden space pick ray
		//
		// note: very important - do this after SetTransform( D3DTS_WORLD, &worldMatrix);
		//
		result = CreateScreenSpacePickRay();


	return VS_OK;
}


// ---------- RenderEnd ----------
/*!
\brief End render invoking RenderViewrect and RenderViewport methods
\author Gareth Edwards
\return INT - ERROR # < VS_ERROR < VS_OK
\note Render initialised as per parent panel context
*/
INT GfxInterface::RenderEnd()
{

	// ---- local
	GfxDevice *props_gfx_device = pi_props->gfx_device;
	IDirect3DDevice9 *d3d9_device = props_gfx_device->d3d9_device;


	// ---- restore viewport
		d3d9_device->SetViewport(&props_gfx_device->d3d9_viewport);


	// ---- end
		d3d9_device->EndScene();
		d3d9_device->Present( NULL, NULL, NULL, NULL);


	// ----no picking
		SetCallbackPickPanel(0, 0);
		SetCallbackPickElement(0, 0);

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- PUBLIC GfxInterface - Render To Image Framework ----------


// ---------- RenderToImageBegin ----------
/*!
\brief Begin render invoking RenderToImageViewrect and RenderToImageViewport methods
\author Gareth Edwards
\return INT - ERROR # < VS_ERROR < VS_OK
*/
INT GfxInterface::RenderToImageBegin()
{

	//---- local
		HRESULT hr = NULL;


	// ---- local
		GfxDevice         *props_gfx_device = pi_props->gfx_device;
		IDirect3DDevice9  *d3d9_device      = props_gfx_device->d3d9_device;


	// ---- local CAV
		VsFloatRectangle  cav = pi_props->client_adjusted_viewport;
		INT width  = (INT)cav.right;
		INT height = (INT)cav.bottom;


	// ---- begin R2I
		INT result = pi_props->gfx_render_to_image->Begin(d3d9_device, width, height);

	return result;
}


// ---------- RenderToImageViewrect ----------
/*!
\brief Render Viewrect background and foreground (e.g. border, grid)
\author Gareth Edwards
\param PanelContext * - parent panel context
\return INT - ERROR # < VS_ERROR < VS_OK
*/
INT GfxInterface::RenderToImageViewrect(
		PanelContext *panel_context
	)
{

	// ---- local
		GfxDevice *props_gfx_device = pi_props->gfx_device;
		IDirect3DDevice9 *d3d9_device = props_gfx_device->d3d9_device;


	// ---- get R2I dimensions
		UINT width = 640;
		UINT height = 400;
		panel_context->GetRenderToImageDimensions(&width, &height);


	// ---- set R2I viewport
		D3DVIEWPORT9 fvp;
		fvp.X = 0;
		fvp.Y = 0;
		fvp.Width = (DWORD)width;
		fvp.Height = (DWORD)height;
		fvp.MinZ = 0;
		fvp.MaxZ = 1;


	// ---- calculate amd set client adjusted R2I viewrect
		VsFloatRectangle adjusted_viewrect =
		{
			0,
			0,
			(FLOAT)fvp.Width,
			(FLOAT)fvp.Height
		};
		pi_props->client_adjusted_viewrect = adjusted_viewrect;
		pi_props->client_adjusted_viewport = adjusted_viewrect;


	// ---- set R2I viewport
		d3d9_device->SetViewport(&fvp);


	// ---- display list
		UINT dspl_list_index = 0;
		panel_context->GetDisplayListIndex(&dspl_list_index);
		BOOL dspl_list_first = dspl_list_index == 0 ? TRUE : FALSE;


	// ---- background
		BOOL r2i = panel_context->GetRenderToImageStatus();

		if (!r2i && dspl_list_first && panel_context->GetBackground())
		{
			HRESULT hr = props_gfx_device->ClearViewportBackground(
				pi_props->colour[pi_props->VIEWRECT]
			);
		}

	return VS_OK;
}


// ---------- RenderToImageViewport ----------
/*!
\brief Render Viewport background and current display list
\author Gareth Edwards
\param INT - display list handle
\param PanelContext * - parent panel context
\return INT - ERROR # < VS_ERROR < VS_OK
*/
INT GfxInterface::RenderToImageViewport(
		INT dspl_list_handle,
		PanelContext *panel_context
	)
{
	INT result = RenderViewport(dspl_list_handle, panel_context);
	return result;
}


// ---------- RenderToImageEnd ----------
/*!
\brief End render invoking RenderToImageViewrect and RenderToImageViewport methods
\author Gareth Edwards
\return INT - ERROR # < VS_ERROR < VS_OK
\note Render to a target texture object initialised as per parent panel context
*/
INT GfxInterface::RenderToImageEnd()
{

	// ---- local
		HRESULT hr = NULL;
		INT result = NULL;


	// ---- local
		GfxDevice         *props_gfx_device = pi_props->gfx_device;
		IDirect3DDevice9  *d3d9_device      = props_gfx_device->d3d9_device;


	// ---- local render to image
		GfxRenderToImage  *render_to_image  = pi_props->gfx_render_to_image;


	// ---- surface description ok ?
		D3DSURFACE_DESC surface_desc;
		hr = render_to_image->target_texture->GetLevelDesc(0, &surface_desc);
		if ( SUCCEEDED(hr) )
		{
			vs_system::ImgBuffer ib;
			result = ib.Initialise(surface_desc.Width, surface_desc.Height, 3);
			result = ib.CreateBuffer();

			result = pi_props->gfx_render_to_image->End(
					d3d9_device,
					ib.GetWidth(),
					ib.GetHeight(),
					ib.GetBytes(),
					ib.GetBuffer()
				);

			result = ib.DestroyBuffer();
		}

	return VS_OK;
}


// ---------- RenderToImageBufferEnd ----------
/*!
\brief End render invoking RenderToImageViewrect and RenderToImageViewport methods
\author Gareth Edwards
\param ImgBuffer * - destination image buffer
\return INT - ERROR # < VS_ERROR < VS_OK
\note Render to an ImgBuffer object (re)initialised as per parent panel context
*/
INT GfxInterface::RenderToImageBufferEnd(
		ImgBuffer *image_buffer
	)
{

	// ---- local
		HRESULT hr = NULL;
		INT result = NULL;


	// ---- image buffer ok ?
	if ( image_buffer != NULL )
	{

		// ---- local
			GfxDevice         *props_gfx_device = pi_props->gfx_device;
			IDirect3DDevice9  *d3d9_device      = props_gfx_device->d3d9_device;


		// ---- local render to image
			GfxRenderToImage  *render_to_image  = pi_props->gfx_render_to_image;


		// ---- surface description ok ?
			D3DSURFACE_DESC surface_desc;
			hr = render_to_image->target_texture->GetLevelDesc(0, &surface_desc);
			if ( SUCCEEDED(hr) )
			{

				// ---- image buffer dimensions
					UINT ib_width  = image_buffer->GetWidth();
					UINT ib_height = image_buffer->GetHeight();
					UINT ib_bytes  = image_buffer->GetBytes();


				// ---- buffer size NOT equal surface size ?
					if ( ib_width  != surface_desc.Width ||
						  ib_height != surface_desc.Height )
					{
						result = image_buffer->DestroyBuffer();
						result = image_buffer->Initialise(
								surface_desc.Width,
								surface_desc.Height,
								ib_bytes
							);
						result = image_buffer->CreateBuffer();
						ib_width = surface_desc.Width;
						ib_height = surface_desc.Height;
					}


				// ---- render INTO image
					result = pi_props->gfx_render_to_image->End(
							d3d9_device,
							ib_width,
							ib_height,
							ib_bytes,
							image_buffer->GetBuffer()
						);
			}

		return VS_OK;
	}

	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- PUBLIC GfxInterface - Device Framework ----------


// ---------- Initialise ----------
/*!
\brief Initialise device.
\author Gareth Edwards
\param HWND - Windows Handle
\param BOOL - IF true THEN windowed
\param UINT - width
\param UINT - height
\return INT - ERROR # < VS_ERROR < VS_OK
*/
int GfxInterface::Initialise(
		HWND hwnd,
		BOOL windowed,
		UINT width,
		UINT height
	)
{

	// ---- local
		GfxDevice *props_gfx_device = pi_props->gfx_device;


	// ---- output to log file
		BOOL log = pi_props->sys_output != NULL ? pi_props->sys_output->GetLog() : false;


	// ---- direct3d initialisation
		IDirect3D9 * g_pD3D = props_gfx_device->d3d9_object;
		g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
		if ( !g_pD3D )
		{
			if (log)
				pi_props->sys_output->Message("Failed to create graphics D3D9 object!");
			return VS_ERROR;
		}


	// ---- display mode
		D3DDISPLAYMODE d3ddm;
		g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);


	// ---- set present parameters
		D3DPRESENT_PARAMETERS *pp = &props_gfx_device->d3d_presentation_parameters;
		pp->Windowed               = windowed;
		pp->SwapEffect             = D3DSWAPEFFECT_DISCARD;
		pp->BackBufferFormat       = d3ddm.Format;
		pp->EnableAutoDepthStencil = TRUE;
		pp->AutoDepthStencilFormat = D3DFMT_D16;
		pp->PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
		pp->MultiSampleType        = D3DMULTISAMPLE_NONE;
		pp->MultiSampleQuality     = 0;


	// ---- here we go
		BOOL succeeded_create_device = FALSE;

	
	// ---- does this adapter mode support a 24 or 16bit depth stencil pixel format, ...?
		D3DFORMAT depth_stencil_format[2] = { D3DFMT_D24S8, D3DFMT_D16};
		for ( int dsf=0; dsf<2; dsf++ )
		{		

			// ---- set and enable depth stencil pixel format
				pp->AutoDepthStencilFormat = depth_stencil_format[dsf];
				pp->EnableAutoDepthStencil = true;


			// ---- anti-aliasing ?
				D3DMULTISAMPLE_TYPE aaq = (D3DMULTISAMPLE_TYPE)pi_props->aaq;
				{

					// ---- format supports multi-sampling -- THEN
						D3DMULTISAMPLE_TYPE aaq = (D3DMULTISAMPLE_TYPE)4;
						DWORD AAQuality = 0;		
						if (SUCCEEDED(
								g_pD3D->CheckDeviceMultiSampleType(
										D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										depth_stencil_format[dsf],
										TRUE,
										aaq,
										&AAQuality
									)
								)
							)
						{
							pp->MultiSampleType = aaq;
							pp->MultiSampleQuality = AAQuality - 1;
						}
				}

		
			// ---- create the d3d device

				INT hr = g_pD3D->CreateDevice(
					D3DADAPTER_DEFAULT,
					D3DDEVTYPE_HAL,
						hwnd,
						D3DCREATE_SOFTWARE_VERTEXPROCESSING,
						pp,
						&props_gfx_device->d3d9_device
					);


			// ---- failed ?
				if (SUCCEEDED(hr))
				{
					succeeded_create_device = true;
					break;
				}
		}


	// 5. failed to create IDirect3DDevice9 ========================================

		if ( !succeeded_create_device )
		{
			props_gfx_device->d3d9_object->Release();
			if ( log )
				pi_props->sys_output->Message("Failed to create graphics D3D9 device!");
			return VS_ERROR;
		}


	// 6. clear window and buffer ==================================================

		INT hr = props_gfx_device->d3d9_device->Clear(
					0,
					NULL,
					D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
					D3DCOLOR_COLORVALUE(0, 0, 0, 1.0f), //D3DCOLOR_XRGB(0, 0, 0),
					1.0f,
					0
				);
		hr = props_gfx_device->d3d9_device->Present( 0, 0, 0, 0 );


	// 7. add provided fonts =======================================================

		hr = pi_props->gfx_fonts->AddProvidedFonts();


	// 8. setup shaders =======================================================
		CHAR *app_name = pi_props->sys_context->GetChar(vs_system::SysContext::APP_NAME);
		for (INT i = 0; i < VS_MAX_SHADERS; i++)
		{
			hr = pi_props->gfx_shaders[i]->Setup(app_name, i, props_gfx_device->d3d9_device);
			if (hr == VS_ERROR) return VS_ERROR;
		}

	return VS_OK;
}


// ---------- SetupGraphics ----------
/*!
\brief Setup device.
\author Gareth Edwards
\return INT - values returned by device reset
*/
INT GfxInterface::SetupGraphics()
{

	// ---- so, setup:
	//    page textures
	//    fonts
	//    render to image
	//    device status
		IDirect3DDevice9 *id3d9_device = pi_props->gfx_device->d3d9_device;


	// ---- setup
		INT setup_pages = pi_props->gfx_atlas->SetupPageTextures(id3d9_device);
		INT setup_fonts = pi_props->gfx_fonts->Setup(id3d9_device);
		INT setup_render_to_image = pi_props->gfx_render_to_image->Setup();


	// ---- status is now..
		pi_props->gfx_device_status = VS_OK;


	// ---- & return setup status
		return setup_pages == VS_OK && setup_fonts == VS_OK ? VS_OK : VS_ERROR; 
}


// ---------- CleanupGraphics ----------
/*!
\brief Release device.
\author Gareth Edwards
\return INT - ERROR # < VS_ERROR < VS_OK 
*/
INT GfxInterface::CleanupGraphics()
{

	// ---- local
		pi_props->gfx_device_status = VS_ERROR;
		BOOL log = pi_props->sys_output != NULL ? pi_props->sys_output->GetLog() : FALSE;

		# if defined(DISPLAY_LIST_DEBUG)
		CHAR text[VS_MAXCHARLEN];
		#endif

	// --- elements
		PI_Element *list = pi_display_list->first_child;
		while ( list )
		{
			PI_Element *element = list->first_child;

			# if defined(DISPLAY_LIST_DEBUG)
			if ( log )
			{
				sprintf(text, "Cleanup Display List %s ( %d )", list->element_context.GetName().c_str(), (INT)list);
				OutputDebugString(text);
			}
			#endif

			while ( element )
			{
				INT cleanup_element = element->Cleanup();

				# if defined(DISPLAY_LIST_DEBUG)
				if ( log )
				{
					sprintf(text, "                Element %s ( %d )", element->element_context.GetName().c_str(), (INT)element);
					OutputDebugString(text);
				}
				#endif

				element = element->next;
			}
			list = list->next;
		}


	//  ---- cleanup each page in the atlas
		INT cleanup_pages = pi_props->gfx_atlas->CleanupPageTextures();


	// ---- cleanup fonts
		INT cleanup_fonts = pi_props->gfx_fonts->Cleanup();


	// ---- cleanup render to image
		INT cleanup_render_to_image = pi_props->gfx_render_to_image->Cleanup();


	// ---- cleanup shaders
		// NA

	return cleanup_pages == VS_OK && cleanup_fonts == VS_OK ? VS_OK : VS_ERROR;
}


// ---------- ResetDevice ----------
/*!
\brief (RE)Setup device.
\author Gareth Edwards
\param UINT - w
\param UINT - h
\return INT - value returned by device reset
*/
INT GfxInterface::ResetDevice(
		UINT w,
		UINT h
	)
{

	// ---- local
		pi_props->gfx_device_status = VS_ERROR;


	// ---- local
		GfxDevice *props_gfx_device = pi_props->gfx_device;


	// ---- log
		if ( pi_props->sys_output ) pi_props->sys_output->Message("Reset Graphics Device");


	// ---- reset
		props_gfx_device->d3d_presentation_parameters.BackBufferWidth = w;
		props_gfx_device->d3d_presentation_parameters.BackBufferHeight = h;
		HRESULT hr = props_gfx_device->d3d9_device->Reset(&props_gfx_device->d3d_presentation_parameters);

	return hr;
}


// ---------- PRIVATE GfxInterface - Device Framework ----------


// ---------- ReleaseDevice ----------
/*!
\brief Release the device.
\author Gareth Edwards
\return INT - value returned by device release
*/
/*
INT GfxInterface::ReleaseDevice()
{

	// ---- local
	GfxDevice *props_gfx_device = pi_props->gfx_device;


	// ---- log
	if ( pi_props->sys_output ) pi_props->sys_output->Message("Release Graphics Device");


	// ---- release
	HRESULT hr = 0;
	if ( props_gfx_device->d3d9_device )
	{
		hr = props_gfx_device->d3d9_device->Release();
		props_gfx_device->d3d9_device = 0;
	}

	return hr;
}
*/


////////////////////////////////////////////////////////////////////////////////
