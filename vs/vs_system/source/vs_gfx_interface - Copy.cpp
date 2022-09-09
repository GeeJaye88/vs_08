////////////////////////////////////////////////////////////////////////////////

// ---------- vs_gfx_interface.cpp ----------
/*!
\file vs_gfx_interface.cpp
\brief Implementation of the Gfx(/raphics)Interface class
\author Gareth Edwards
\note TAINTED - REQUIRES COMPILE FLAGS
*/

// ---- include ----
#include "../header/vs_gfx_interface.h"


// ---- namespace ----
using namespace vs_system;



//#define NORMAL_GFX



////////////////////////////////////////////////////////////////////////////////


// ---------- definitions ----------
// #define VS_DEBUG
// #define VS_TEMP_DEBUG

#define ELEMENT_DEBUG FALSE


// ---------- global ( SetupFromJpegTexture ) ----------
//
// 4912 x 3684 x 3 = 54287424
// 4912 x 3684 x 4 = 72383232
// 50331648
//
UCHAR global_very_big_image_buffer[54287424];


////////////////////////////////////////////////////////////////////////////////


// ---------- Gfx classes
#include "../hpp/vs_gfx_page_dimension.hpp"
#include "../hpp/vs_gfx_page.hpp"
#include "../hpp/vs_gfx_atlas.hpp"
#include "../hpp/vs_gfx_device.hpp"
#include "../hpp/vs_gfx_font.hpp"
#include "../hpp/vs_gfx_fonts.hpp"
#include "../hpp/vs_gfx_lams.hpp"
#include "../hpp/vs_gfx_render_to_image.hpp"
#include "../hpp/vs_gfx_pi_props.hpp"
#include "../hpp/vs_gfx_pi_element.hpp"


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


// ---------- AddPageToAtlasFromJpegFile ----------
/*!
\brief Add texture page to texture atlas from file
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK 
*/
INT GfxInterface::AddPageToAtlasFromJpegFile(UINT id, CHAR *filename)
{

	// ---- local ----
	INT result = VS_ERROR;


	// IF -- jpeg file exists THEN --
	ImgBuffer  img_buffer;
	ImgJpegLib img_jpeg;
	result = img_jpeg.GetHeaderInfo(&img_buffer, filename);
	if ( result > 0 )
	{

		// ---- get dimensions ----
		UINT width  = img_buffer.GetWidth();
		UINT height = img_buffer.GetHeight();
		UINT bytes  = img_buffer.GetBytes();
		UCHAR *data = NULL;


		// IF --page with this identity --THEN
		GfxPage* page = pi_props->gfx_atlas->GetPageWithLogicalId(id);
		if ( page != NULL )
		{

			// ---- update ----
			IDirect3DDevice9 *d3d9_device = pi_props->gfx_device->d3d9_device;
			result = pi_props->gfx_atlas->UpdatePageFromJpeg(d3d9_device, page, filename);
			if ( result == VS_OK )
			{
				if ( pi_props->sys_output )
				{
					char msg[VS_MAXCHARLEN];
					sprintf(msg, "Updated Atlas page id %d (dimension %d x %d)", (INT)id,  (INT)width, (INT)height);
					pi_props->sys_output->Message(msg);
				}
				return VS_OK;
			}
		}

		// ELSE -- no page with this identity so add
		else
		{

			// ---- add ----
			IDirect3DDevice9 *d3d9_device = pi_props->gfx_device->d3d9_device;
			result = pi_props->gfx_atlas->AddPageFromJpeg(d3d9_device, id, filename);
			if ( result == VS_OK )
			{
				if ( pi_props->sys_output )
				{
					char msg[VS_MAXCHARLEN];
					sprintf(msg, "Added Atlas page id %d (dimension %d x %d)", (INT)id,  (INT)width, (INT)height);
					pi_props->sys_output->Message(msg);
				}
				return VS_OK;
			}
		}

	}

	// ---- fail ----
	if ( pi_props->sys_output )
	{
		char msg[VS_MAXCHARLEN];
		sprintf(msg, "Failed to add OR update Atlas page id %d failed!", (INT)id);
		pi_props->sys_output->Message(msg);
	}

	return result;
}


// ---------- AddPageToAtlasFromTgaFileInMemory ----------
/*!
\brief Add texture page to texture atlas from tga format file in memory
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK 
*/
INT GfxInterface::AddPageToAtlasFromTgaFileInMemory(UINT id, UCHAR *data)
{

	// ---- TGA header struct -----
	struct tga_header
	{
		UCHAR   idlength;
		UCHAR 	colourmaptype;
		UCHAR 	datatypecode;
		USHORT 	colourmaporigin;
		UCHAR 	colourmaplength;
		UCHAR 	ColourMapDepth;
		USHORT 	x_origin;
		USHORT 	y_origin;
		USHORT 	width;
		USHORT 	height;
		UCHAR 	bits_per_pixel;
		UCHAR 	imagedescriptor;
	} header;


	// ---- Note : MUST be DATA TYPE 2 (Unmapped RGB) ----
	char uncompressed_header_content[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	for ( UINT i=0; i<12; i++ )
		if (data[i] != uncompressed_header_content[i]) return VS_ERROR;


	// ---- mem copy first 18 bytes into header struct ----
	memcpy(&header, data, 18);


	// ---- extract dimension and bytes per pixel ----
	UINT width  = header.width;
	UINT height = header.height;
	UINT bytes  = header.bits_per_pixel / 8;


	// ---- add page ----
	INT result = VS_ERROR;
	if ( result = pi_props->gfx_atlas->AddRetainedPage(id, width, height, bytes, data+18, true) ) 
	{
		if ( pi_props->sys_output )
		{
			char msg[VS_MAXCHARLEN];
			sprintf(msg, "\tAdded Atlas page id %d (dimension %d x %d)", (INT)id,  (INT)width, (INT)height);
			pi_props->sys_output->Message(msg);
		}
	}
	else
	{
		if ( pi_props->sys_output )
		{
			char msg[VS_MAXCHARLEN];
			sprintf(msg, "\tFailed to add Atlas page id %d failed!", (INT)id);
			pi_props->sys_output->Message(msg);
		}
	}


	return result;
}


// ---------- AddPageToAtlasFromImgBuffer ----------
/*!
\brief Add texture page to texture atlas from image buffer
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK
*/
INT GfxInterface::AddPageToAtlasFromImgBuffer(
		UINT lid,
		vs_system::ImgBuffer *img
	)
{

	// ---- get dimension and bytes per pixel ----
	UINT  width  = img->GetWidth();
	UINT  height = img->GetHeight();
	UINT  bytes  = img->GetBytes();
	BYTE *data   = img->GetBuffer();


	// ---- add page ----
	INT result = VS_ERROR;
	if ( result =
		pi_props->gfx_atlas->AddRetainedPage(
				lid,
				width,
				height,
				bytes,
				data,
				true
			)
	)
	{
		if ( pi_props->sys_output )
		{
			char msg[VS_MAXCHARLEN];
			sprintf(
					msg,
					"\tAdded Atlas page id %d (dimension %d x %d)",
					(INT)lid,
					(INT)width,
					(INT)height
				);
			pi_props->sys_output->Message(msg);
		}
	}
	else
	{
		if ( pi_props->sys_output )
		{
			char msg[VS_MAXCHARLEN];
			sprintf(
					msg,
					"\tFailed to add Atlas page id %d failed!",
					(INT)lid
				);
			pi_props->sys_output->Message(msg);
		}
	}

	return result;
}


// ---------- AddPageToAtlas ----------
/*!
\brief Add texture page to texture atlas
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= identity
*/
INT GfxInterface::AddPageToAtlas()
{

	INT identity = pi_props->gfx_atlas->AddPage();
	if ( identity >= 0 )
	{
		if ( pi_props->sys_output )
		{
			char msg[VS_MAXCHARLEN];
			sprintf(msg, "\tAdded Atlas page id %d (dynamic)", identity);
			pi_props->sys_output->Message(msg);
		}
	}
	else
	{
		if ( pi_props->sys_output )
		{
			char msg[VS_MAXCHARLEN];
			sprintf(msg, "\tFailed to add Atlas page id %d (dynamic)!", identity);
			pi_props->sys_output->Message(msg);
		}
	}

	return identity;
}


// ---------- GetAtlasPageDimensions ----------
/*!
\brief Get handle of an atlas page
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= handle
*/
INT GfxInterface::GetAtlasPageTextureDimensions(UINT logical_id, UINT *width, UINT *height)
{

	GfxPage *page = pi_props->gfx_atlas->GetPageWithLogicalId(logical_id);
	if (page != NULL)
	{
		GfxPageDimension dimension;
		page->GetTextureDimension(&dimension);
		*width = dimension.width;
		*height = dimension.height;
	}

	return VS_OK;
}


// ---------- GetAtlasPageHandle ----------
/*!
\brief Get handle of an atlas page
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= handle
*/
INT GfxInterface::GetAtlasPageHandle(UINT logical_id)
{
	GfxPage *page = pi_props->gfx_atlas->GetPageWithLogicalId(logical_id);
	return (INT)page;
}


// ---------- PageSetup ----------
/*!
\brief (Re)Setup (re-intitialise) a page in Atlas
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= identity
*/
INT GfxInterface::PageSetup(
	UINT identity,
	std::string &filename,
	vs_system::ImgBuffer *img_buffer
	)
{

	// IF -- page added -- THEN -- report success -- ELSE -- report fail
	INT page_handle = pi_props->gfx_atlas->SetupPage(
			pi_props->gfx_device->d3d9_device,
			identity,
			filename,
			img_buffer
		);


	// IF -- page added -- THEN
	if ( page_handle >= 0 )
	{
		if ( pi_props->sys_output )
		{
			//char msg[VS_MAXCHARLEN];
			//sprintf(msg, "Setup Atlas page id: %d", identity);
			//pi_props->sys_output->Message(msg);
			;
		}
	}

	// ELSE - - report
	else
	{
		if ( pi_props->sys_output )
		{
			char msg[VS_MAXCHARLEN];
			sprintf(msg, "Failed to setup Atlas page id: %d", identity);
			pi_props->sys_output->Message(msg);
		}
	}

	return page_handle;
}


// ---------- PageLock ----------
/*!
\brief Page lock
\author Gareth Edwards
\return UCHAR *
*/
UCHAR * GfxInterface::PageLock(INT page_handle)
{

	// ---- local ----
	GfxPage *page = (GfxPage *)page_handle;


	// IF -- page -- THEN
	if (page != NULL && page->d3d9_texture != NULL)
	{

		// ---- lock and load ----
		D3DLOCKED_RECT locked;
		HRESULT hr = page->d3d9_texture->LockRect( 0, &locked, NULL, 0 );
		if ( SUCCEEDED(hr) )
		{
			// ---- good to go....!!! ----
			UCHAR * bytePointer = (UCHAR *)locked.pBits;
			//std::cout << "PageLock: " << (UCHAR *)locked.pBits << std::endl;
			return bytePointer;
		}
		else
		{
			char msg[VS_MAXCHARLEN];
			sprintf(msg, "PageLock: page->d3d9_texture->LockRect failed!");
			pi_props->sys_output->Message(msg);
		}
	}
	else
	{
		char msg[VS_MAXCHARLEN];
		sprintf(msg, "PageLock: page->d3d9_texture %d", (INT)page->d3d9_texture);
		pi_props->sys_output->Message(msg);
	}

	return NULL;
}


// ---------- PageUnlock ----------
/*!
\brief Page unlock
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT  GfxInterface::PageUnlock(INT page_handle)
{
	// ---- cast from INT to GfxPage ----
	GfxPage *page = (GfxPage *)page_handle;

	// IF -- page -- THEN
	if (page != NULL && page->d3d9_texture != NULL)
	{
		HRESULT hr = page->d3d9_texture->UnlockRect(0);
	}

	//D3DXSaveTextureToFile("test.jpg", D3DXIFF_JPG, page->d3d9_texture, NULL);

	return VS_ERROR;
}


// ---------- AddFont ----------
/*!
\brief Add font
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::AddFont(std::string &name, std::string &type, UINT height, UINT weight)
{

	// ---- test permissable weight or thickness ----
	INT selected_weight = weight < 0 ? 0 : weight > 9 ? 900 : weight * 10;

	// ---- add ----
	pi_props->gfx_fonts->AddFont(name, type, height, selected_weight);

	return VS_OK;
}


// ---------- SetFont ----------
/*!
\brief Set font
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT  GfxInterface::SetFont(
		UINT index,
		std::string &name,
		std::string &type,
		UINT height,
		UINT weight
	)
{

	// ---- get font by index
		GfxFont *font = pi_props->gfx_fonts->GetFont(index);
		if ( font == NULL ) return VS_ERROR;


	// ---- test permissable weight or thickness
		INT selected_height = height < 1 ? 1 : height;
		INT selected_weight = weight < 0 ? 0 : weight > 9 ? 900 : weight * 10;


	// ---- set
		font->name     = name;
		font->typeface = type;
		font->height   = selected_height;
		font->weight   = selected_weight;

	return VS_OK;
}


// ---------- SetAmbient ----------
/*!
\brief Set ambient properties using vs ambient object
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetVsAmbient(VsAmbient *a)
{
	pi_props->gfx_ambient->Set(a);
	return VS_OK;
}


// ---------- SetVsLight ----------
/*!
\brief Set a light properties using vs light object
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetVsLight(UINT index, VsLight *vs_light)
{
	pi_props->gfx_lights->Set(vs_light, index);
	return VS_OK;
}


// ---------- SetVsMaterial ----------
/*!
\brief Set a material properties using vs light object
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetVsMaterial(UINT index, VsMaterial *vs_material)
{
	pi_props->gfx_materials->Set(vs_material, index);
	return VS_OK;
}


// ---------- SetLighting ----------
/*!
\brief Set lights ON or OFF (TRUE or FALSE)
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetLighting(BOOL b)
{
	
	pi_props->gfx_device->d3d9_device->SetRenderState(D3DRS_LIGHTING, b);
	//pi_props->gfx_device->d3d9_device->SetRenderState(D3DRS_ZENABLE, TRUE);
	//pi_props->gfx_device->d3d9_device->SetRenderState(D3DRS_COLORVERTEX, FALSE);
	pi_props->gfx_device->d3d9_device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

	return VS_OK;
}


// ---------- SetLightAmbient ----------
/*!
\brief Set ambient light RGB levels 
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetLightAmbient(FLOAT red, FLOAT green, FLOAT blue)
{

	pi_props->gfx_device->d3d9_device->SetRenderState
			(
				D3DRS_AMBIENT,
				D3DCOLOR_COLORVALUE
					(
						red,
						green,
						blue,
						1.0
					)
			);

	return VS_OK;
}


// ---------- SetLightEnable ----------
/*!
\brief Set light enabled
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetLightEnable(UINT index, BOOL enable)
{
	pi_props->gfx_device->d3d9_device->SetLight(index, &pi_props->gfx_lights->light[index]);
	pi_props->gfx_device->d3d9_device->LightEnable(index, enable);
	return VS_OK;
}


// ---------- SetMaterialSelected ----------
/*!
\brief Set material selected
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetMaterial(UINT index)
{
	pi_props->gfx_device->d3d9_device->SetMaterial(&pi_props->gfx_materials->material[index]);
	return VS_OK;
}


// ---------- GetDeviceStatus ----------
/*!
\brief Get width of device backbuffer
\author Gareth Edwards
\return UINT - width
*/
UINT GfxInterface::GetDeviceStatus()
{
	return pi_props->gfx_device_status;
}


// ---------- GetBackBufferWidth ----------
/*!
\brief Get width of device backbuffer
\author Gareth Edwards
\return UINT - width
*/
UINT GfxInterface::GetBackBufferWidth()
{
	return pi_props->gfx_device->d3d_presentation_parameters.BackBufferWidth;
}


// ---------- GetBackBufferHeight ----------
/*!
\brief Get height of device backbuffer
\author Gareth Edwards
\return UINT - height
*/
UINT GfxInterface::GetBackBufferHeight()
{
	return pi_props->gfx_device->d3d_presentation_parameters.BackBufferHeight;
}


// ---------- GetClientAdjustedViewrect ----------
INT GfxInterface::GetClientAdjustedViewrect(VS_FloatRectangle *viewrect)
{
	*viewrect = pi_props->client_adjusted_viewrect;
	return VS_OK;
}


// ---------- GetClientAdjustedViewport ----------
INT GfxInterface::GetClientAdjustedViewport(VS_FloatRectangle *viewport)
{
	*viewport = pi_props->client_adjusted_viewport;
	return VS_OK;
}


// ---------- GetElementRenderFlag ----------
/*!
\brief Get element render flag
\author Gareth Edwards
\param UINT - flag id
\param BOOL * - returned flag state
\return INT
*/
INT GfxInterface::GetElementRenderFlag(
		UINT  flag_id,
		BOOL *flag_state
	)
{
	switch ( flag_id )
	{
		case ElmContext::ALPHA_BLENDING : *flag_state = pi_props->elm_alpha_blending; break;
		case ElmContext::COLOUR_VERTEX  : *flag_state = pi_props->elm_colour_vertex;  break;
		case ElmContext::MULTI_STATE    : *flag_state = pi_props->elm_multi_state;    break;
		case ElmContext::SOLID          : *flag_state = pi_props->elm_solid;          break;
		case ElmContext::TEXTURE        : *flag_state = pi_props->elm_texture;        break;
		case ElmContext::VISIBLE        : *flag_state = pi_props->elm_visible;        break;
		case ElmContext::FLAG_OVERIDE   : *flag_state = pi_props->elm_overide;        break;
		default: return VS_ERROR;
	}
	return VS_OK;
}


// ---------- GetElementRenderMode ----------
/*!
\brief Get element render mode
\author Gareth Edwards
\param UINT - flag id
\param UINT * - returned mode value
\return INT
*/
INT GfxInterface::GetElementRenderMode(
		UINT  mode_id,
		UINT *mode_value
	)
{
	switch ( mode_id )
	{
		case ElmContext::CULL_MODE : *mode_value = pi_props->elm_cull_mode; break;
		default: return VS_ERROR;
	}
	return VS_OK;
}


// ---------- GetOverPanelContext ----------
/*!
\brief Get "over panel" context
\author Gareth Edwards
\param GfxContext* - over context
\return INT
*/
INT GfxInterface::GetOverPanelContext(
		GfxContext **over
	)
{
	*over = pi_props->gfx_context_over_panel;
	return VS_OK;
}


// ---------- GetElementContext ----------
INT GfxInterface::GetElementContext(INT handle, ElmContext **element_context)
{
	PI_Element *element = (PI_Element *)handle;
	if ( element == NULL) return VS_ERROR;
	*element_context = &element->element_context;
	if ( element_context == NULL) return VS_ERROR;
	return VS_OK;
}


// ---------- GetElementGroupId ----------
UINT GfxInterface::GetElementGroupId(INT handle)
{
	PI_Element *element = (PI_Element *)handle;
	vs_system::ElmContext *context = &element->element_context;
	return element == NULL ? 0 : (INT)context->GetGroupId();
}


// ---------- GetElementId ----------
UINT GfxInterface::GetElementId(INT handle)
{
	PI_Element *element = (PI_Element *)handle;
	vs_system::ElmContext *context = &element->element_context;
	return element == NULL ? 0 : (INT)context->GetId();
}


// ---------- SetBackground ----------
INT GfxInterface::SetBackground(VS_Colour *c)
{
	pi_props->colour[pi_props->BG] = VS_D3DCOLOR_COLORVALUE(c->red, c->green, c->blue, c->alpha);
	return VS_OK;
}


// ---------- SetBackgroundColor ----------
INT GfxInterface::SetBackgroundColor(
		BOOL background,
		VS_Colour *background_color,
		VS_Colour *background_color_over
	)
{
	pi_props->frame_background = background;
	pi_props->frame_background_color = VS_D3DCOLOR_COLORVALUE(
			background_color->red,
			background_color->green,
			background_color->blue,
			background_color->alpha
		);
	pi_props->frame_background_color_over = VS_D3DCOLOR_COLORVALUE(
			background_color_over->red,
			background_color_over->green,
			background_color_over->blue,
			background_color_over->alpha
		);
	return VS_OK;
}


// ---------- SetTextureBorderColor ----------
INT GfxInterface::SetTextureBorderColor(
	BOOL texture_border,
	VS_Colour *texture_border_color,
	VS_Colour *texture_border_color_over
	)
{
	pi_props->frame_texture_border = texture_border;
	pi_props->frame_texture_border_color = VS_D3DCOLOR_COLORVALUE(
			texture_border_color->red,
			texture_border_color->green,
			texture_border_color->blue,
			texture_border_color->alpha
		);
	pi_props->frame_texture_border_color_over = VS_D3DCOLOR_COLORVALUE(
			texture_border_color_over->red,
			texture_border_color_over->green,
			texture_border_color_over->blue,
			texture_border_color_over->alpha
		);
	return VS_OK;
}


// ---------- SetAAQ ----------
INT GfxInterface::SetAAQ(UINT aaq)
{
	pi_props->aaq = aaq;
	return VS_OK;
}


// ---------- SetBorder ----------
INT GfxInterface::SetBorder(VS_FloatRectangle *border)
{
	pi_props->border = *border;
	return VS_OK;
}


// ---------- SetCallbackPickPanel ----------
INT GfxInterface::SetCallbackPickPanel(INT(*callback)(INT, INT), INT param)
{
	pi_props->pick_panel_flag = true;
	pi_props->handle_pick_panel = callback;
	pi_props->pick_panel_handle_app_obj = param;
	return VS_OK;
}


// ---------- SetCallbackPickElement ----------
INT GfxInterface::SetCallbackPickElement(INT(*callback)(INT, INT), INT param)
{
	pi_props->pick_element_flag = true;
	pi_props->pick_element_callback = callback;
	pi_props->pick_element_handle = param;
	return VS_OK;
}


// ---------- SetClientCursorPosition ----------
//
// Note: Only invoked from Function WinMessageProcess
//
INT GfxInterface::SetClientCursorPosition(POINT *position)
{
	pi_props->cursor_position = *position;
	return VS_OK;
}


// ---------- SetGlobalConfigureBitmask ----------
INT GfxInterface::SetGlobalConfigureBitmask(
		BYTE setup_bitmask,
		std::string setup_prefix
	)
{

	// IF -- length zero -- THEN
	UINT setup_prefix_length = (UINT)setup_prefix.length();
	if ( setup_prefix_length == 0 )
	{

		PI_Element *list = pi_display_list->first_child;
		while ( list )
		{
			PI_Element *element = list->first_child;
			while ( element )
			{
				if ( element->vertex_buffer != NULL )
				{
					ElmContext *element_context = &element->element_context;
					element->element_context_configured = FALSE;
					element_context->SetBitmask(setup_bitmask);
				}
				element = element->next;
			}
			list = list->next;
		}

	}
	// ELSE -- compare length greater than zero 
	else
	{

		PI_Element *list = pi_display_list->first_child;
		while ( list )
		{

			PI_Element *element = list->first_child;
			while ( element )
			{
				ElmContext *element_context = &element->element_context;

				// IF -- valid element name -- THEN
				std::string element_name = element_context->GetName();
				if ( element_name != "" )
				{

					// IF -- first char matched -- THEN
					if ( element_name[0] == setup_prefix[0] )
					{

						// IF -- prefix matched -- THEN -- set setup bitmask
						UINT element_name_length = (UINT)element_name.length();
						if ( setup_prefix.compare(
							0, setup_prefix_length, element_name,
							0, setup_prefix_length) == 0 )
						{
							element->element_context_configured = FALSE;
							element_context->SetBitmask(setup_bitmask);
						}
					}
				}

				element = element->next;

			}

			list = list->next;

		}
	}

	// Done
	return VS_OK;
}


// ---------- SetElementConfigureBitmask ----------
INT GfxInterface::SetElementConfigureBitmask(
		INT handle_element,
		BYTE setup_bitmask
	)
{
	PI_Element *element = (PI_Element *)handle_element;
	ElmContext *element_context = &element->element_context;
	element_context->SetBitmask(setup_bitmask);
	element->element_context_configured = FALSE;
	return VS_OK;
}


// ---------- SetConfigure ----------
INT GfxInterface::SetConfigure(
		BOOL config,
		INT handle_element
	)
{
	PI_Element *element = (PI_Element *)handle_element;
	element->element_context_configured = config;
	return VS_OK;
}


// ---------- SetConfigure ----------
INT GfxInterface::SetConfigure(INT handel_display_list)
{
	// IF handel NOT zero THEN
	if ( handel_display_list != 0 )
	{
		// FOR -- each element in list
		PI_Element *list = (PI_Element *)handel_display_list;
		PI_Element *element = list->first_child;
		while (element)
		{
			if ( element->vertex_buffer != NULL )
			{
				element->element_context_configured = FALSE;
			}
			element = element->next;
		}
	}

	return VS_OK;
}


// ---------- SetBorderOver ----------
INT GfxInterface::SetBorderOver(VS_FloatRectangle *border_over)
{
	pi_props->border_over = *border_over;
	return VS_OK;
}


// ---------- SetFPS ----------
INT GfxInterface::SetFPS(UINT fps)
{
	pi_props->fps = fps;
	return VS_OK;
}


// ---------- SetMargin ----------
INT GfxInterface::SetMargin(VS_FloatRectangle *margin)
{
	pi_props->margin = *margin;
	return VS_OK;
}


// ---------- SetElementFlag ----------
/*!
\brief Set element flag
\author Gareth Edwards
\param UINT - flag id
\param BOOL - flag state
\return INT - ERROR < VS_ERROR == 0 < VS_OK
\note
	ElmContext::ALPHA_BLENDING = transparency alpha blending
	ElmContext::COLOUR_VERTEX  = interpolated colour vertices
	ElmContext::MULT_STATE     = off/on/over/active multi states
	ElmContext::SOLID          = solid or wireframe rendering
	ElmContext::TEXTURE        = texture mapping
	ElmContext::VISIBLE        = visible or hidden
*/
INT GfxInterface::SetElementFlag(
		UINT  flag_id,
		BOOL  flag_state
	)
{
	switch ( flag_id )
	{
		case ElmContext::ALPHA_BLENDING : pi_props->elm_alpha_blending = flag_state; break;
		case ElmContext::COLOUR_VERTEX  : pi_props->elm_colour_vertex  = flag_state; break;
		case ElmContext::MULTI_STATE    : pi_props->elm_multi_state    = flag_state; break;
		case ElmContext::SOLID          : pi_props->elm_solid          = flag_state; break;
		case ElmContext::TEXTURE        : pi_props->elm_texture        = flag_state; break;
		case ElmContext::VISIBLE        : pi_props->elm_visible        = flag_state; break;
		case ElmContext::FLAG_OVERIDE   : pi_props->elm_overide        = flag_state; break;
		default                         : return VS_ERROR;
	}
	return VS_OK;
}


// ---------- SetElementSwitch ----------
INT GfxInterface::SetElementSwitch(UINT group_id, UINT id, BOOL value)
{

	PI_Element *list = pi_display_list->first_child;
	while ( list )
	{
		PI_Element *element = list->first_child;
		while ( element )
		{
			vs_system::ElmContext *element_context = &element->element_context;
			if ( element_context )
			{
				if ( element_context->GetGroupId() == group_id )
				{
					if ( element_context->GetId() == id )
					{ 
						element_context->SetSwitchedOn(value);
					}
				}
			}
			element = element->next;
		}
		list = list->next;
	}

	return VS_OK;
}


// ---------- SetGfxFloatBuffer ----------
INT GfxInterface::SetGfxFloatBuffer(FLOAT *buffer, INT size)
{
	INT copy_size = size > VS_MAX_ELM_GFX_FLOAT_BUFFER ? VS_MAX_ELM_GFX_FLOAT_BUFFER : size;
	memcpy(pi_props->supplied_gfx_float_buffer, buffer, copy_size*sizeof(FLOAT));
	return VS_OK;
}


// ---------- GetGfxFloatBuffer ----------
FLOAT * GfxInterface::GetGfxFloatBuffer()
{
	return pi_props->supplied_gfx_float_buffer;
}


// ---------- SetSysInput ----------
INT GfxInterface::SetSysInput(SysInput *sys_input)
{
	pi_props->sys_input = sys_input;
	return VS_OK;
}

// ---------- SetSysOutput ----------
INT GfxInterface::SetSysOutput(SysOutput *sys_output)
{
	pi_props->sys_output = sys_output;
	return VS_OK;
}


// ---------- SetViewport ----------
INT GfxInterface::SetViewport(VS_FloatRectangle *viewport)
{

	// ---- copy rect into viewport ----
	pi_props->viewport = *viewport;


	// ---- accelerate ----
	VS_FloatRectangle *border  = &pi_props->border;
	VS_FloatRectangle *client_adjusted_viewrect = &pi_props->client_adjusted_viewrect;


	// ---- calculate CAV (Client Adjusted Viewport) ----
	VS_FloatRectangle adjusted_viewport =
	{
		(FLOAT)( (INT)( client_adjusted_viewrect->left   + border->left   + 0.5f ) ),
		(FLOAT)( (INT)( client_adjusted_viewrect->top    + border->top    + 0.5f ) ),
		(FLOAT)( (INT)( client_adjusted_viewrect->right  - border->right  + 0.5f ) ),
		(FLOAT)( (INT)( client_adjusted_viewrect->bottom - border->bottom + 0.5f ) )
	};
	
	// ---- store ----
	pi_props->client_adjusted_viewport = adjusted_viewport;

	// ---- done ----
	return VS_OK;
}


// ---------- SetViewportMarkers ----------
INT GfxInterface::SetViewportMarkers(UINT rows, UINT cols, UINT size)
{
	pi_props->grid_rows = rows;
	pi_props->grid_cols = cols;
	pi_props->grid_size = size;
	return VS_OK;
}


// ---------- SetViewrect ----------
INT GfxInterface::SetViewrect(VS_FloatRectangle *rect)
{

	// ---- local ----
	VS_FloatRectangle *margin = &pi_props->margin;
	FLOAT width = (FLOAT)GetBackBufferWidth();
	FLOAT height = (FLOAT)GetBackBufferHeight();


	// ---- copy rect into viewrect ----
	pi_props->viewrect = *rect;


	// ---- calculate client adjusted viewrect ----
	VS_FloatRectangle adjusted_viewrect =
	{
		(FLOAT)( (INT)( pi_props->viewrect.left   * width  + margin->left   + 0.5f ) ),
		(FLOAT)( (INT)( pi_props->viewrect.top    * height + margin->top    + 0.5f ) ),
		(FLOAT)( (INT)( pi_props->viewrect.right  * width  - margin->right  + 0.5f ) ),
		(FLOAT)( (INT)( pi_props->viewrect.bottom * height - margin->bottom + 0.5f ) )
	};


	// ---- store ----
	pi_props->client_adjusted_viewrect = adjusted_viewrect;
	pi_props->client_adjusted_viewport = adjusted_viewrect;

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Add interface to PI_Element ----------


// ---------- AddDisplayList ----------
/*!
\brief Add list
\author Gareth Edwards
\return int - ERROR <= 0 < VS_OK
*/
INT GfxInterface::AddDisplayList(CHAR *name)
{
	PI_Element *list = pi_display_list->Add(name);
	if ( ELEMENT_DEBUG && pi_props->sys_output )
	{
		char text[VS_MAXCHARLEN];
		sprintf(text, "Add display List: %s ( %d )", list->element_context.GetName().c_str(), (INT)list);
		pi_props->sys_output->Message(text);
	}
	return (INT)list;
}


// ---------- AddDisplayList ----------
/*!
\brief Add list
\author Gareth Edwards
\return int - ERROR <= 0 < VS_OK
*/
INT GfxInterface::AddDisplayList(CHAR *name, UINT group_id, UINT id)
{
	PI_Element *list = pi_display_list->Add(name);
	list->element_context.SetGroupId(group_id);
	list->element_context.SetGroupId(id);
	if (ELEMENT_DEBUG && pi_props->sys_output)
	{
		char text[VS_MAXCHARLEN];
		sprintf(text, "Add display List: %s ( %d )", list->element_context.GetName().c_str(), (INT)list);
		pi_props->sys_output->Message(text);
	}
	return (INT)list;
}


// ---------- AddElement ----------
/*!
\brief Add an element to a list
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < element handle
*/
INT GfxInterface::AddElement(
		INT handle,
		CHAR *name,
		UINT  group_id,
		UINT  element_id,
		INT(*callback_get_configure)(ElmContext*)
	)
{

	// IF -- not -- NULL -- AND valid type -- THEN
	PI_Element *list = (PI_Element*)handle;
	if ( list != NULL && list->type==PI_Element::LIST )
	{

		// ---- increment child count ----
		list->children++;


		// ---- add element ----
		PI_Element *display_element = list->Add(
				name,
				group_id,
				element_id,
				0,
				callback_get_configure
			);


		// IF -- element -- THEN
		if ( display_element != NULL )
		{
			ElmContext *elm_context = &display_element->element_context;


			// ---- set "default" element render flags
			elm_context->SetAlphaBlending(pi_props->elm_alpha_blending);
			elm_context->SetColourVertex(pi_props->elm_colour_vertex);
			elm_context->SetOveride(pi_props->elm_overide);
			elm_context->SetMultiState(pi_props->elm_multi_state);
			elm_context->SetSolid(pi_props->elm_solid);
			elm_context->SetTexture(pi_props->elm_texture);
			elm_context->SetVisible(pi_props->elm_visible);
			 

			// ---- set "default" element render modes
			elm_context->SetCullMode(pi_props->elm_cull_mode);


			// IF -- debug and output -- THEN -- report
			if ( ELEMENT_DEBUG && pi_props->sys_output )
			{
				char text[VS_MAXCHARLEN];
				sprintf(
					text,
					"            Element: %s ( %d )",
					display_element->element_context.GetName().c_str(),
					(INT)display_element
					);
				pi_props->sys_output->Message(text);
			}

		}

		return (INT)display_element;
	}

	return VS_ERROR;
}


// ---------- AddElement ----------
/*!
\brief Add an element to a list
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < element handle
*/
INT GfxInterface::AddElement(
		INT   handle,
		CHAR *name,
		UINT  group_id,
		UINT  element_id,
		UINT  version,
		INT(*callback_get_configure)(ElmContext*)
	)
{

	// IF -- not NULL -- AND valid type -- THEN
	PI_Element *list = (PI_Element*)handle;
	if ( list != NULL && list->type==PI_Element::LIST )
	{

		// ---- add element ----
		PI_Element *display_element = list->Add(
				name,
				group_id,
				element_id,
				version,
				callback_get_configure
			);


		// IF -- element -- THEN
		if ( display_element != NULL )
		{
			ElmContext *elm_context = &display_element->element_context;
			
			// ---- set "default" element render flags ----
			elm_context->SetAlphaBlending(pi_props->elm_alpha_blending);
			elm_context->SetColourVertex(pi_props->elm_colour_vertex);
			elm_context->SetOveride(pi_props->elm_overide);
			elm_context->SetMultiState(pi_props->elm_multi_state);
			elm_context->SetSolid(pi_props->elm_solid);
			elm_context->SetTexture(pi_props->elm_texture);
			elm_context->SetVisible(pi_props->elm_visible);

			
			// ---- set "default" element render modes ----
			elm_context->SetCullMode(pi_props->elm_cull_mode);


			// IF -- debug and output -- THEN -- report
			if ( ELEMENT_DEBUG && pi_props->sys_output )
			{
				char text[VS_MAXCHARLEN];
				sprintf(
						text,
						"            Element: %s ( %d )",
						display_element->element_context.GetName().c_str(),
						(INT)display_element
					);
				pi_props->sys_output->Message(text);
			}

		}

		return (INT)display_element;

	}

	return VS_ERROR;
}


// ---------- GetHandleDisplayList ----------
/*!
\brief Get element child count
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < child count
*/
INT GfxInterface::GetHandleDisplayList(
		CHAR *name
	)
{

	PI_Element *elem = pi_display_list->first_child;
	while (elem)
	{
		ElmContext *elm_context = &elem->element_context;
		if (!strcmp(elm_context->GetName().c_str(), name))
		{
			return (INT)elem;
		}
		elem = elem->next;
	}

	return VS_ERROR;
}


// ---------- GetChildCount ----------
/*!
\brief Get element child count
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < child count
*/
INT GfxInterface::GetChildCount(
		INT handle
	)
{

	// IF -- not NULL AND valid type -- THEN
	PI_Element *list = (PI_Element*)handle;
	if ( list != NULL && list->type==PI_Element::LIST )
	{
		// ---- return # child count ----
		return list->children;
	}

	return VS_ERROR;
}


// ---------- GetFirst ----------
/*!
\brief Get first child element
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < child count
*/
INT GfxInterface::GetFirst(
		INT handle
	)
{

	// IF -- not NULL AND valid type --THEN
	PI_Element *list = (PI_Element*)handle;
	if ( list != NULL && list->type==PI_Element::LIST )
	{
		// ---- increment child count ----
		return (INT) list->first_child;
	}

	return VS_ERROR;
}


// ---------- GetNext ----------
/*!
\brief Get next element
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < child count
*/
INT GfxInterface::GetNext(
		INT handle
	)
{
	// IF -- not NULL AND valid type --THEN
	PI_Element *elem = (PI_Element*)handle;
	if ( elem != NULL && elem->type==PI_Element::ELEMENT )
	{
		// ---- increment child count ----
		return (INT) elem->next;
	}

	return VS_ERROR;
}


// ---------- SetElementConfigured ----------
/*!
\brief Set element parameters
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK
*/
INT GfxInterface::SetElementConfigured(
	INT    handle,
	BOOL   config,
	UINT   bitmask
	)
{
	PI_Element *element = (PI_Element*)handle;
	if (element != NULL && element->type == PI_Element::ELEMENT)
	{
		element->SetConfigured(config, bitmask);
		return VS_OK;
	}
	return VS_ERROR;
}


// ---------- SetElementParameters ----------
/*!
\brief Set element parameters
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK
*/
INT GfxInterface::SetElementParameters(
	INT    handle,
	UINT   float_parameter_size,
	FLOAT *parameter
	)
{
	PI_Element *element = (PI_Element*)handle;
	if ( element != NULL && element->type==PI_Element::ELEMENT )
	{
		element->element_context.SetFloatParameter(float_parameter_size, parameter);
		return VS_OK;
	}
	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- RenderBegin ----------
/*!
\brief Render begin.
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK 
*/
int GfxInterface::RenderBegin()
{

	// ---- accelerate
		GfxDevice *props_gfx_device = pi_props->gfx_device;
		IDirect3DDevice9 *d3d9_device = props_gfx_device->d3d9_device;


	// ---- local
		HRESULT hr;


	// ---- set NO texture
		hr = d3d9_device->SetTexture(0, 0);

		hr = d3d9_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		hr = d3d9_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);


	// ---- set render states
		hr = d3d9_device->SetRenderState(D3DRS_AMBIENT, RGB(255, 255, 255));
		hr = d3d9_device->SetRenderState(D3DRS_LIGHTING, FALSE);
		hr = d3d9_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		hr = d3d9_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
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


	// ---- set world matrix ----
		D3DXMATRIX matrix;
		D3DXMatrixIdentity(&matrix);
		hr = d3d9_device->SetTransform(D3DTS_WORLD, &matrix);


	// ---- store viewport ----
		d3d9_device->GetViewport(&props_gfx_device->d3d9_viewport);


	// ---- clear bg ----
		hr = props_gfx_device->ClearViewportBackground(pi_props->colour[pi_props->BG]);


	// ---- outline entire window with a line one pixel wide ----
		// VS_FloatRectangle margin = { 1, 1, 1, 1 };
		// hr = props_gfx_device->DrawViewportBorder( &margin, pi_props->colour[pi_props->EDGE]);


	// ---- set NULL "over panel" context ----
		pi_props->gfx_context_over_panel = NULL;


	// ---- begin
		hr = d3d9_device->BeginScene();

	return VS_OK;
}


// ---------- RenderViewrect ----------
/*!
\brief Render viewrect
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK 
*/
INT GfxInterface::RenderViewrect(GfxContext *gfx_context, BOOL border)
{

	// ---- accelerate
		GfxDevice *props_gfx_device = pi_props->gfx_device;
		IDirect3DDevice9 *d3d9_device = props_gfx_device->d3d9_device;


	// ---- get client adjusted viewrect
		VS_FloatRectangle viewrect;
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
		BOOL solid = gfx_context->GetPanelBackground();
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
			VS_FloatRectangle *widths =
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


// ---------- RenderViewport ----------
/*!
\brief Render viewport
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK 
*/
INT GfxInterface::RenderViewport(INT handle, GfxContext *gfx_context)
{

	/*
	vs_system::SysInput *sys_input = pi_props->sys_input;
	BOOL mbd = sys_input->GetBool(sys_input->MOUSE_BUTTON_DOWN);
	BOOL lbd = sys_input->GetBool(sys_input->LBUTTON_DOWN);
	printf("mbd %d : lbd %d\n", mbd, lbd);
	*/


	// 1. Setup ====================================================================


	// ---- catch illegal handle
		if ( handle <= 0 )
		{
			pi_props->sys_output->Message("Illegal display list handle!");
			return VS_ERROR;
		}


	// ---- local
		INT vs_result = 0;
		HRESULT hr = NULL;


	// ---- accelerate
		GfxDevice *props_gfx_device = pi_props->gfx_device;
		IDirect3DDevice9 *d3d9_device = props_gfx_device->d3d9_device;


	// ---- setup viewport, projection, view matrix and world matrix
		vs_result = RenderSetupForViewport(gfx_context);


	// ---- get user interaction status
		BOOL user_interaction_status = TRUE;
		vs_result = gfx_context->GetInteractionStatus(&user_interaction_status);


	// IF -- user interaction -- THEN -- test and set against viewport OR set FALSE
		BOOL over_viewport = user_interaction_status ? pi_props->isOverViewport() : FALSE;


	// ---- get render to image flag
		BOOL render_to_image_flag = TRUE;
		vs_result = gfx_context->GetRenderToImageFlag(&render_to_image_flag);


	// IF -- render to image -- THEN -- never OVER viewport, so always set FALSE
		over_viewport = over_viewport ? ( render_to_image_flag ? FALSE : TRUE ) : FALSE;


	// ---- display list
		UINT index = 0;
		gfx_context->GetDisplayListIndex(&index);
		BOOL first_panel = index == 0 ? TRUE : FALSE;


	// ---- background
		BOOL solid = gfx_context->GetPanelBackground();
		if ( first_panel && solid )
		{

			// ---- set color
				D3DCOLOR panel_background_color =
					over_viewport ?
						( gfx_context->GetUseViewportColor() ?
							gfx_context->GetViewportOverD3dColor() :
								pi_props->colour[pi_props->VIEWPORT_OVER]
						) :
						( gfx_context->GetUseViewportColor() ?
							gfx_context->GetViewportD3dColor() :
								pi_props->colour[pi_props->VIEWPORT]
						) ;


			// ---- over viewport ?
				//
				// THEN -- set gfx_contex "over" panel
				//
				// Note: Provides for focus that is neither immediate (e.g. pick
				// viewport, pick element) OR persistant (e.g. mouse down on
				// viewport, mouse down on element), such as mouse wheel click
				// (or roll-over) that requires over viewport gfx context.
				//
				if ( over_viewport )
					pi_props->gfx_context_over_panel = gfx_context;


			// ---- clear
				hr = props_gfx_device->ClearViewportBackground(panel_background_color);

		}


	// ---- frame texture_border ?
		D3DCOLOR frame_texture_border_color = 0x00000000;
		if ( pi_props->frame_texture_border )
		{
			// IF -- over viewport -- THEN -- colour over -- ELSE colour on
			frame_texture_border_color = over_viewport ?
				pi_props->frame_texture_border_color_over :
					pi_props->frame_texture_border_color;
		}


	// ---- output to log file ----
		BOOL log = pi_props->sys_output != NULL ? pi_props->sys_output->GetLog() : FALSE;


	// ---- NO updated font ----
		BOOL updated_font = FALSE;


	// ---- NO picked element ----
		BOOL picked_an_element = FALSE;


	// 2. List =====================================================================


	// IF -- display list THEN render ----
	PI_Element *list = (PI_Element*)handle;
	if ( list->type == PI_Element::LIST )
	{

		// ---- log text ----
		CHAR text[VS_MAXCHARLEN];


		// ---- log ? THEN -- pre-report LIST setup (if any)
			PI_Element *element = list->first_child;
			if ( ELEMENT_DEBUG && log )
			{
				while ( element )
				{
					if ( !element->element_context_configured )
					{
						sprintf(text, "Setup display List %s ( %d )", list->element_context.GetName().c_str(), (INT)list);
						pi_props->sys_output->Message(text);
						break;
					}
					element = element->next;
				}

			}


		// ---- initialise SUPPLIED client adjusted viewport bounds
			VS_FloatRectangle viewport;
			vs_result = GetClientAdjustedViewport(&viewport);
			pi_props->supplied_gfx_float_parameter[VS_PANEL_LEFT]      = viewport.left;
			pi_props->supplied_gfx_float_parameter[VS_PANEL_TOP]       = viewport.top;
			pi_props->supplied_gfx_float_parameter[VS_PANEL_RIGHT]     = viewport.right;
			pi_props->supplied_gfx_float_parameter[VS_PANEL_BOTTOM]    = viewport.bottom;
			pi_props->supplied_gfx_float_parameter[VS_BUFFER_WIDTH]    = (FLOAT)GetBackBufferWidth();
			pi_props->supplied_gfx_float_parameter[VS_BUFFER_HEIGHT]   = (FLOAT)GetBackBufferHeight();



		// 3. Element ===============================================================

		//
		// Painters algorithm: Render first to last so that in a 2d element stack -
		// such as in a button - only the first element (e.g. background) is picked
		// and only this event invokes a callback via PI_Props isPickElement method
		//

		// FOR -- every element --
		element = list->first_child;
		while ( element )
		{

			// ---- context
				ElmContext *element_context = &element->element_context;


			// ?. scene =========================================================

				BOOL lit = element_context->GetLit();
				if ( lit )
				{
					hr = SetLighting(TRUE);
					hr = SetLightAmbient(
							pi_props->gfx_ambient->color.r,
							pi_props->gfx_ambient->color.g,
							pi_props->gfx_ambient->color.b
						);
					hr = SetMaterial((INT)element_context->GetMaterial());

					hr = d3d9_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
					hr = d3d9_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
					hr = d3d9_device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
					hr = d3d9_device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

				}
				else
				{
					hr = SetLighting(FALSE);
				}


			// ?. matrices =========================================================

				BOOL matrix_active = FALSE;
				element_context->GetMatrixActive(&matrix_active);
				if ( matrix_active )
				{
					element_context->GetMatrix(&pi_props->world_matrix);
					memcpy(
							&pi_props->d3d_world_matrix.m[0][0],
							&pi_props->world_matrix.m[0][0],
							16 * sizeof(FLOAT)
						);
					d3d9_device->SetTransform(D3DTS_WORLD, &pi_props->d3d_world_matrix);
				}


			// 4. configured =========================================================

			// ---- element context NOT configured ? -- THEN -- setup
				if ( !element->element_context_configured )
				{

					// ---- set supplied data
						vs_result = element_context->SetGfxFloatParameter(pi_props->supplied_gfx_float_parameter);
						vs_result = element_context->SetGfxFloatBuffer(pi_props->supplied_gfx_float_buffer);


					// ---- (re)setup element
						vs_result = element->Setup(d3d9_device);
						if ( ELEMENT_DEBUG && log )
						{
							sprintf(text, "              Element %s ( %d )", element_context->GetName().c_str(), (INT)element);
							pi_props->sys_output->Message(text);
						}

					// ---- font height changed ? -- THEN -- update font height
						UINT font_height = element_context->GetFontHeight();
						if ( font_height != 0 )
						{
							GfxFont *font = pi_props->gfx_fonts->GetFont(element_context->GetFontName());
							if ( font != NULL) { font->height = font_height; updated_font=TRUE; }
							element_context->SetFontHeight(0);
						}

				}


			// 5. Visible ============================================================


			// IF -- visible -- THEN -- render
			if ( element->element_context.GetVisible() && element->element_context.GetDisplay() )
			{

				/*
				std::string name = element->element_context.GetName();
				if (!strcmp("3D Sphere", name.c_str()))
				{
					printf("Found %s\n", name.c_str());
				}
				*/


				// 6. Behaviour =======================================================


				// ---- behaviour
					UINT behaviour = element_context->GetBehaviour();


				// ---- over viewport and user interaction ?
					element->over = FALSE;
					if ( over_viewport )
					{
						// IF -- 2D -- THEN -- test over 2D -- ELSE -- test over 3D
						if (element->dimension == element->TWO)
						{
							if (behaviour != ElmContext::BEHAVIOUR_NONE)
								element->over = pi_props->isOverElement(&element->bounds_2d);
						}
						else if (element->dimension == element->THREE)
						{
							element->over = pi_props->isOverElement(&element->bounds_3d);
						}
					}


				// 7. Picked ==========================================================


				// ---- over viewport (and user interaction) ?
					element->picked = FALSE;
					if ( over_viewport )
					{
						// picked AND NOT already picked an element ? -- THEN -- set picked an element
						element->picked = element->over ? pi_props->isPickElement((INT)element) : FALSE;
						if (element->picked && !picked_an_element)
						{
							picked_an_element = TRUE;
						}
					}


				// 8. On ==============================================================


				// ---- already ON ?
					BOOL switched_on_status = element_context->GetSwitchedOn();
					BOOL switched_on_prev_status = switched_on_status;


				// 9. Behaviour =======================================================


				// ---- update determined by behaviour
					switch ( behaviour )
					{
						case ElmContext::BEHAVIOUR_NONE:
							if ( element->picked )
							{
								switched_on_status = TRUE;
								element_context->SetSwitchedOn(switched_on_status);
							}
							break;

						case ElmContext::BEHAVIOUR_CLUNK:
							if ( element->picked )
							{
								switched_on_status = TRUE;
								element_context->SetSwitchedOn(switched_on_status);
							}
							break;

						case ElmContext::BEHAVIOUR_UNCLUNK:
							{
								element_context->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_CLUNK);
								switched_on_status = FALSE;
								element_context->SetSwitchedOn(switched_on_status);
							}
							break;

						case ElmContext::BEHAVIOUR_RADIO:

							if ( element->picked )
							{
								if ( switched_on_status )
								{
									UINT counter = 0;
									UINT group_id = element_context->GetGroupId();
									UINT id = element_context->GetId();
									PI_Element *e = list->first_child;
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
									PI_Element *e = list->first_child;
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

						case ElmContext::BEHAVIOUR_SWITCH:
							if ( element->picked )
							{
								switched_on_status = switched_on_status ? FALSE : TRUE;
								UINT group_id = element_context->GetGroupId();
								UINT id = element_context->GetId();
								PI_Element *e = list->first_child;
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


				// 10. Callback Picked Element =========================================


				// ---- element picked ?
					//    IF callback THEN
					//       invoke callback
					//
					// note: after updating switched on status
					//
					if ( element->picked )
					{
						pi_props->CallbackElement((INT)element);

						// IF -- switched on previuosly -- THEN -- flip
						//
						// note: allows callback functions to test for a
						// second pick of an element...
						//
						if ( switched_on_prev_status )
							element_context->SetSwitchedOnPreviously(FALSE);
						else
							element_context->SetSwitchedOnPreviously(TRUE);
					}


				// ---- set solid OR wireframe
					hr = d3d9_device->SetRenderState(
							D3DRS_FILLMODE, element_context->GetSolid() ?
								D3DFILL_SOLID :
									  D3DFILL_WIREFRAME);


				// ---- set colour vertex TRUE OR FALSE
					hr = d3d9_device->SetRenderState(D3DRS_COLORVERTEX,
							element_context->GetColourVertex() );


				// 11. Texture ========================================================


				// ---- texture ON ?
				BOOL element_on = TRUE;
				BOOL texture_on = element_context->GetTexture();
				if ( texture_on )
				{


					// 12. Multi State =================================================


					// ---- multi-state texturing ?
						UINT texture_page_reference = VS_ERROR;
						if ( element_context->GetMultiState() )
						{

							// ---- get multi-state atlas page references ----
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
						// ELSE -- get single-state atlas page reference
						else
						{
							element_context->GetAtlasPage(0, &texture_page_reference);
						}


					// 13. Texture =====================================================


					// ---- get page texture from atlas ----
					IDirect3DTexture9 *d3d9_texture = pi_props->gfx_atlas->GetTexture(texture_page_reference);


					// ---- valid texture ?
					if ( d3d9_texture != NULL )
					{

						// ---- set texture stage 0:
							hr = d3d9_device->SetTexture (0, d3d9_texture);


						// ---- set Texture States for transparent borders AND NO repeat ----
							//
							//      Also see: D3DTADDRESS_BORDER, D3DTADDRESS_WRAP,
							//      D3DTADDRESS_CLAMP and D3DTADDRESS_MIRROR
							//
							hr = d3d9_device->SetSamplerState(0, D3DSAMP_ADDRESSU,    D3DTADDRESS_BORDER);
							hr = d3d9_device->SetSamplerState(0, D3DSAMP_ADDRESSV,    D3DTADDRESS_BORDER);
							hr = d3d9_device->SetSamplerState(0, D3DSAMP_BORDERCOLOR, frame_texture_border_color);


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

						// ---- set non-blended statestates
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

				// ELSE -- texture is OFF
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


				// 14. On =============================================================


				// IF element_on THEN
				if ( element_on )
				{

					// 15. Cull ========================================================

					// ---- cull ?
						switch ( element_context->GetCullMode() )
						{
							default:
							case ElmContext::CULL_NONE:
								hr = d3d9_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
								break;
							case ElmContext::CULL_CW:   hr = d3d9_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);   break;
							case ElmContext::CULL_CCW:  hr = d3d9_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);  break;
						}


					// 16. Colour Vertex ===============================================
						hr = d3d9_device->SetRenderState(
								D3DRS_COLORVERTEX,
									element->element_context.GetColourVertex());

					// 17. FVF =========================================================


					// ---- set FVF and Stream Source ----
						hr = d3d9_device->SetFVF( element_context->GetVertexFormat() );
						hr = d3d9_device->SetStreamSource(
								0,
								element->vertex_buffer,
								0,
								element_context->GetVertexSize()*sizeof(float)
							);
				

					// 18. Version =====================================================


					// IF -- version == TEXT -- THEN
					if ( element_context->GetVersion() == vs_system::ElmContext::VERSION_TEXT )
					{

						// ---- outline ? -- THEN -- draw non-indexed primitive 
							if ( element_context->GetTextOutline() )
							{
								hr = d3d9_device->DrawPrimitive(
										(D3DPRIMITIVETYPE)element_context->GetPrimitiveType(),
										0,
										element_context->GetPrimitiveCount()
									);
							}


						// ---- no font ? -- THEN -- get
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

					// ELSE -- version != TEXT
					else
					{

						// ---- indices ? -- THEN -- draw indexed primitives
						UINT index_count = element_context->GetIndexCount();
						if ( index_count > 0 )
						{

							/*
							BOOL report = FALSE;
							std::string name = element_context->GetName();
							if (strcmp(name.c_str(), "Exposure Aoi") == 0) report = TRUE;
							if (report)
							{
								WORD *i;
								hr = element->index_buffer->Lock(0, sizeof(i), (void**)&i, 0);
								{
									if (element->report_later)
									{
										printf("### draw %d ( %d ) with...\n", (INT)i, (INT)element_context->GetIndexBufferPtr());
										for (INT j = 0; j < 12; j++)
											printf("%d, ", (INT)(*(i + j)));
										printf("\n");
										element->report_later = FALSE;
									}
								}
								hr = element->index_buffer->Unlock();
							}
							*/

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

						// ELSE -- draw non-indexed primitives
						else
						{
							hr = d3d9_device->DrawPrimitive(
									(D3DPRIMITIVETYPE)element_context->GetPrimitiveType(),
									0,
									element_context->GetPrimitiveCount()
								);

						}
				
					}

				}

			}


			// ---- matrices
				if ( matrix_active )
				{
					d3d9_device->SetTransform(D3DTS_WORLD, &pi_props->d3d_identity_matrix);
				}


			// ---- next
				element = element->next;

		}


		// 20. Font =================================================================

		if ( updated_font )
		{
			INT cleanup_fonts = pi_props->gfx_fonts->Cleanup();
			INT setup_fonts = pi_props->gfx_fonts->Setup(d3d9_device);
		}


		// 21. Panel Picked =========================================================


		// ---- NOT picked an element AND render panel background ?
			//
			//    Note: If panel_background NOT TRUE AND no background - simples,
			//    it cannot be picked... , see above:
			//
			//    BOOL panel_background = gfx_context->GetBackground();
			//
			//    Note: Major fix in isPickPanel method... though not a bug... if the
			//    handle_pick_panel function or method does not detect that the mouse
			//    button is down - that there is ANY unconsumed mouse button event, then
			//    the logic below, if no element selected, is "always on" until after
			//    run-time there are mouse buttons events -
			//
			//    Note: pi_props->isPickPanel logic
			//
			//    IF pick_panel_flag AND handle_pick_panel != NULL THEN
			//       IF sys_input != NULL AND (ANY) sys_input->mouse_button_down THEN
			//          invoke handle_pick_panel
			//          pick_panel_flag
			//
			if ( !picked_an_element && solid )
			{
				BOOL picked =
					over_viewport ?
					pi_props->isPickPanel(gfx_context->GetHandle()) :
					FALSE;
			}

		return VS_OK;
	}

	return VS_ERROR;
}


// ---------- RenderSetupForViewport ----------
/*!
\brief Render setup for viewport of viewport, projection, view and world matrices
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK 
*/
INT GfxInterface::RenderSetupForViewport(GfxContext *gfx_context)
{

	// ---- accelerate
	GfxDevice *props_gfx_device = pi_props->gfx_device;
	IDirect3DDevice9 *d3d9_device = props_gfx_device->d3d9_device;


	// ---- get client adjusted viewport ----
	VS_FloatRectangle viewport;
	GetClientAdjustedViewport(&viewport);


	// ---- set viewport ----
	D3DVIEWPORT9 fvp;
	fvp.X      = (DWORD)(viewport.left+0.5);
	fvp.Y      = (DWORD)(viewport.top+0.5);
	fvp.Width  = (DWORD)(viewport.right - viewport.left + 0.5);
	fvp.Height = (DWORD)(viewport.bottom - viewport.top + 0.5);
	fvp.MinZ   = 0; // *gfx_context->GetZNear();
	fvp.MaxZ   = 1; // *gfx_context->GetZFar();
	d3d9_device->SetViewport(&fvp);


	// ---- set drawing mode ----
	d3d9_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


	// ---- set projection, view and world matrices ----
	switch ( INT type = gfx_context->GetTypeId() )
	{

		// ---- perspective ----
		case VS_PERSPECTIVE:
		{

			// ---- view context ----
			CHAR  *view      = gfx_context->GetView();
			FLOAT *rotate    = gfx_context->GetRotate();
			FLOAT *translate = gfx_context->GetTranslate();


			// ---- perspective context ----
			FLOAT *perspective = gfx_context->GetPerspective();


			// ---- proportional zoom ----
			// e.g. for spherical pano, use new radii difference / original radii difference
			// e.g. for spherical pano, use new fov / original fov
			// FLOAT prop_zoom = 1.0;
			// FLOAT ratio = (FLOAT)fvp.Width / (FLOAT)fvp.Height * prop_zoom;


			// ---- set projection matrix ----
			FLOAT ratio = (FLOAT)fvp.Width / (FLOAT)fvp.Height;
			D3DXMATRIX projMatrix;
			D3DXMatrixPerspectiveFovLH(
					&projMatrix,
					perspective[0]*(FLOAT)(VS_RADIANS_PER_DEGREE),	// Vertical FOV
					ratio,											// Aspect Ratio
					perspective[1],									// Z Near
					perspective[2]									// Z Far
				);
			d3d9_device->SetTransform(D3DTS_PROJECTION, &projMatrix);


			// ---- initialise view matrices ----
			D3DXMATRIX viewMatrix, rotZ, rotY, rotX, tran;
			D3DXMatrixIdentity( &viewMatrix);
			D3DXMatrixRotationX( &rotX, rotate[0] * (FLOAT)(VS_RADIANS_PER_DEGREE));
			D3DXMatrixRotationY( &rotY, rotate[1] * (FLOAT)(VS_RADIANS_PER_DEGREE));
			//D3DXMatrixRotationZ( &rotZ, rotate[2] * (FLOAT)(VS_RADIANS_PER_DEGREE));
			D3DXMatrixTranslation(&tran, translate[0], translate[1], translate[2]);


			// ---- set view matrix ----
			D3DXMatrixMultiply( &viewMatrix,  &tran,  &viewMatrix);
			D3DXMatrixMultiply( &viewMatrix,  &rotX,  &viewMatrix);
			D3DXMatrixMultiply( &viewMatrix,  &rotY,  &viewMatrix);
			//D3DXMatrixMultiply( &viewMatrix,  &rotZ,  &viewMatrix);
			d3d9_device->SetTransform  ( D3DTS_VIEW, &viewMatrix );


			// ---- set world matrix ----
			D3DXMATRIX worldMatrix;
			D3DXMatrixIdentity( &worldMatrix);
			d3d9_device->SetTransform( D3DTS_WORLD, &worldMatrix);


		}
		break;

		// ---- orthogonal ----
		case VS_ORTHO_FRONT:
		case VS_ORTHO_BACK:
		case VS_ORTHO_TOP:
		case VS_ORTHO_BOTTOM:
		case VS_ORTHO_LEFT:
		case VS_ORTHO_RIGHT:
		{

			// ---- orthogonal context ----
			FLOAT *orthogonal = gfx_context->GetOrthogonal();


			// ---- view context ----
			FLOAT *rotate    = gfx_context->GetRotate();
			FLOAT *translate = gfx_context->GetTranslate();


			// ---- set projection ----
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
			d3d9_device->SetTransform(D3DTS_PROJECTION, &projMatrix);


			// ----initialise orthogonal view rotation options  ----
			float r[6][2] = 
			{
				{   0,   0 }, // front
				{   0, 180 }, // back
				{  90,   0 }, // top
				{ -90,   0 }, // bottom
				{   0,  90 }, // left
				{   0, -90 }, // right
			};


			// ---- initialise view matrices ----
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


			// ---- set view matrix ----
			D3DXMATRIX viewMatrix;
			D3DXMatrixIdentity( &viewMatrix);
			D3DXMatrixMultiply( &viewMatrix,  &tran,  &viewMatrix);
			D3DXMatrixMultiply( &viewMatrix,  &rotX,  &viewMatrix);
			D3DXMatrixMultiply( &viewMatrix,  &rotY,  &viewMatrix);
			d3d9_device->SetTransform( D3DTS_VIEW, &viewMatrix);


			// ---- set world matrix ----
			D3DXMATRIX worldMatrix;
			D3DXMatrixIdentity( &worldMatrix);
			d3d9_device->SetTransform( D3DTS_WORLD, &worldMatrix);

		}
		break;

		// Huh???
		default:
			return VS_ERROR;
	}

	return VS_OK;
}


// ---------- RenderEnd ----------
/*!
\brief Render end.
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK 
*/
INT GfxInterface::RenderEnd()
{

	// ---- accelerate ----
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


// ---------- PUBLIC GfxInterface - Render 2 Image Framework ----------


// ---------- RenderToImageViewrect ----------
/*!
\brief Render viewrect to image
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK
*/
INT GfxInterface::RenderToImageViewrect(
		GfxContext *gfx_context
	)
{

	// ---- accelerate
		GfxDevice *props_gfx_device = pi_props->gfx_device;
		IDirect3DDevice9 *d3d9_device = props_gfx_device->d3d9_device;


	// ---- get R2I dimensions
		UINT width  = 640;
		UINT height = 400;
		gfx_context->GetRenderToImageDimensions(&width, &height);


	// ---- set R2I viewport
		D3DVIEWPORT9 fvp;
		fvp.X      = 0;
		fvp.Y      = 0;
		fvp.Width  = (DWORD)width;
		fvp.Height = (DWORD)height;
		fvp.MinZ   =  0;
		fvp.MaxZ   =  1;


	// ---- calculate amd set client adjusted R2I viewrect
		VS_FloatRectangle adjusted_viewrect =
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
		UINT index = 0;
		gfx_context->GetDisplayListIndex(&index);
		BOOL first_panel = index == 0 ? TRUE : FALSE;


	// ---- background
		BOOL r2i = gfx_context->GetRenderToImageStatus();

		if ( !r2i && first_panel && gfx_context->GetPanelBackground() )
		{
			HRESULT hr = props_gfx_device->ClearViewportBackground(
				pi_props->colour[pi_props->VIEWRECT]
			);
		}

	return VS_OK;
}


// ---------- RenderToImageBegin ----------
/*!
\brief Render end.
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK
*/
INT GfxInterface::RenderToImageBegin()
{

	//---- local ----
	HRESULT hr = NULL;


	// ---- accelerate ----
	GfxDevice         *props_gfx_device = pi_props->gfx_device;
	IDirect3DDevice9  *d3d9_device      = props_gfx_device->d3d9_device;


	// ---- accelerate CAV ----
	VS_FloatRectangle  cav = pi_props->client_adjusted_viewport;
	INT width  = (INT)cav.right;
	INT height = (INT)cav.bottom;


	// ---- begin R2I ----
	INT result = pi_props->gfx_render_to_image->Begin(d3d9_device, width, height);

	return result;
}


// ---------- RenderToImageViewport ----------
/*!
\brief Render viewport to image
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK
*/
INT GfxInterface::RenderToImageViewport(INT handle, GfxContext *gfx_context)
{
	INT result = RenderViewport(handle, gfx_context);
	return result;
}


// ---------- RenderToImageEnd ----------
/*!
\brief Render end.
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK
*/
INT GfxInterface::RenderToImageEnd()
{

	// ---- local ----
	HRESULT hr = NULL;
	INT result = NULL;


	// ---- accelerate ----
	GfxDevice         *props_gfx_device = pi_props->gfx_device;
	IDirect3DDevice9  *d3d9_device      = props_gfx_device->d3d9_device;


	// ---- accelerate render to image ----
	GfxRenderToImage  *render_to_image  = pi_props->gfx_render_to_image;


	// IF --surface description -- THEN
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


// ---------- RenderToImageEnd ----------
/*!
\brief Render end.
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK
*/
INT GfxInterface::RenderToImageEnd(vs_system::ImgBuffer *image_buffer)
{

	// ---- local ----
	HRESULT hr = NULL;
	INT result = NULL;


	// IF -- image buffer -- THEN
	if ( image_buffer != NULL )
	{

		// ---- accelerate ----
		GfxDevice         *props_gfx_device = pi_props->gfx_device;
		IDirect3DDevice9  *d3d9_device      = props_gfx_device->d3d9_device;


		// ---- accelerate render to image ----
		GfxRenderToImage  *render_to_image  = pi_props->gfx_render_to_image;


		// IF -- surface description -- THEN
		D3DSURFACE_DESC surface_desc;
		hr = render_to_image->target_texture->GetLevelDesc(0, &surface_desc);
		if ( SUCCEEDED(hr) )
		{

			// ---- image buffer dimensions
			UINT ib_width  = image_buffer->GetWidth();
			UINT ib_height = image_buffer->GetHeight();
			UINT ib_bytes  = image_buffer->GetBytes();


			// IF -- buffer size NOT equal surface size -- THEN -- re-create
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


			// ---- render INTO image ----
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
\return INT - ERROR < VS_ERROR == 0 < VS_OK 
*/
int GfxInterface::Initialise(HWND hwnd, BOOL windowed, UINT width, UINT height)
{

	//#define GFX_NORMAL


	// ---- accelerate ----
	GfxDevice *props_gfx_device = pi_props->gfx_device;


	// ---- output to log file ----
	BOOL log = pi_props->sys_output != NULL ? pi_props->sys_output->GetLog() : false;



	#if defined(GFX_NORMAL)

	// 1: create IDirect3D9 ========================================================

	HRESULT hr = 0;
	props_gfx_device->d3d9_object = Direct3DCreate9( D3D_SDK_VERSION);
    if ( !props_gfx_device->d3d9_object )
	{
		if ( log )
			pi_props->sys_output->Message("Failed to create graphics D3D9 object!");
		return VS_ERROR;
	}


	// 2. hardware capabilities ====================================================

	D3DCAPS9 caps;
	props_gfx_device->d3d9_object->GetDeviceCaps( D3DADAPTER_DEFAULT, props_gfx_device->d3d9_devtype, &caps);
	int vp = 0;
	if ( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ) {
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}


	// 3. presentation parameters ==================================================

	D3DPRESENT_PARAMETERS *pp = &props_gfx_device->d3d_presentation_parameters;
	pp->hDeviceWindow              = hwnd;
	pp->Windowed                   = windowed;
	pp->Flags                      = 0;                           // no flags set
	pp->FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;     // default refresh rate
	pp->PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT; // default presentation rate D3DPRESENT_INTERVAL_IMMEDIATE
	pp->SwapEffect                 = D3DSWAPEFFECT_DISCARD;       // throw away previous frames
	pp->BackBufferCount            = 1;                           // we only need a signel back buffer
	pp->MultiSampleType            = D3DMULTISAMPLE_NONE;
	pp->MultiSampleQuality         = 0;


	// IF -- NOT windowed -- THEN -- then set width & height
	if ( !windowed )
	{
		pp->BackBufferWidth  = width;
		pp->BackBufferHeight = height;
	}


	// 4. create IDirect3DDevice9 ==================================================

	// ---- get the adapter display mode ----
	BOOL succeeded_create_device = false;
	D3DDISPLAYMODE d3d_display_mode;
    props_gfx_device->d3d9_object->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3d_display_mode );


	// ---- does this adapter mode support a 32-bit RGB pixel format? ----
	if( d3d_display_mode.Format != D3DFMT_X8R8G8B8 )
	{
		CleanupGraphics();
		if ( log )
			pi_props->sys_output->Message("Graphics display format D3DFMT_X8R8G8B8 not supported!");
		return VS_ERROR;
	}


	// ---- set backbuffer format ----
	pp->BackBufferFormat = d3d_display_mode.Format; // == D3DFMT_X8R8G8B8; // D3DFMT_X8R8G8B8 - Not D3DFMT_A8R8G8B8, D3DFMT_UNKNOWN, etc.


	// ---- does this adapter mode support a 24 or 16bit depth stencil pixel format ----
	D3DFORMAT depth_stencil_format[2] = { D3DFMT_D24S8, D3DFMT_D16};
	for ( int dsf=0; dsf<2; dsf++ )
	{		

		// ---- set and enable depth stencil pixel format ----
		pp->AutoDepthStencilFormat = depth_stencil_format[dsf];
		pp->EnableAutoDepthStencil = true;


		// IF --anti-aliasing -- THEN -- try to set multi-sampling type and quality
		if ( pi_props->aaq != 0 )
		{

			// IF -- format supports multi-sampling -- THEN
			DWORD AAQuality = 0;		
			if (SUCCEEDED(
					props_gfx_device->d3d9_object->CheckDeviceMultiSampleType(
							D3DADAPTER_DEFAULT,
							D3DDEVTYPE_HAL,
							depth_stencil_format[dsf],
							windowed,
							(D3DMULTISAMPLE_TYPE)pi_props->aaq,
							&AAQuality
						)
					)
				)
			{
				pp->MultiSampleType    = (D3DMULTISAMPLE_TYPE)pi_props->aaq;
				pp->MultiSampleQuality = AAQuality-1;
			}
		}


		// ---- create device ----
		hr = props_gfx_device->d3d9_object->CreateDevice(
			D3DADAPTER_DEFAULT,              // primary adapter
			props_gfx_device->d3d9_devtype,  // device type
			hwnd,                            // window associated with device
			vp,                              // vertex processing
			pp,                              // present parameters
			&props_gfx_device->d3d9_device); // return created device


		// If -- failed -- THEN -- try again
		if ( SUCCEEDED(hr) )
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


	#else


	// ---- direct3d initialisation ----

	IDirect3D9 * g_pD3D = props_gfx_device->d3d9_object;
    g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );


	// ---- display mode ----
	D3DDISPLAYMODE d3ddm;
	g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);

	D3DPRESENT_PARAMETERS *pp = &props_gfx_device->d3d_presentation_parameters;
	ZeroMemory(pp, sizeof(g_d3dpp));


	// ---- set present parameters ----
	pp->Windowed = TRUE;
	pp->SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp->BackBufferFormat = d3ddm.Format;
	pp->EnableAutoDepthStencil = TRUE;
	pp->AutoDepthStencilFormat = D3DFMT_D16;
	pp->PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


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
			D3DMULTISAMPLE_TYPE aaq = (D3DMULTISAMPLE_TYPE)4;
			{

				// IF -- format supports multi-sampling -- THEN
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


	#endif


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

	pi_props->gfx_fonts->AddProvidedFonts();


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

	// so, setup:
	//    page textures
	//    fonts
	//    render to image
	//    device status

	IDirect3DDevice9 *id3d9_device = pi_props->gfx_device->d3d9_device;


	INT setup_pages = pi_props->gfx_atlas->SetupPageTextures(id3d9_device);
	INT setup_fonts = pi_props->gfx_fonts->Setup(id3d9_device);
	INT setup_render_to_image = pi_props->gfx_render_to_image->Setup();


	pi_props->gfx_device_status = VS_OK;


	return setup_pages == VS_OK && setup_fonts == VS_OK ? VS_OK : VS_ERROR; 
}


// ---------- CleanupGraphics ----------
/*!
\brief Release device.
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK 
*/
INT GfxInterface::CleanupGraphics()
{

	// ---- local ----
	pi_props->gfx_device_status = VS_ERROR;
	BOOL log = pi_props->sys_output != NULL ? pi_props->sys_output->GetLog() : false;
	char text[VS_MAXCHARLEN];


	// FOR -- each element in the list
	PI_Element *list = pi_display_list->first_child;
	while ( list )
	{
		PI_Element *element = list->first_child;
		if ( ELEMENT_DEBUG && log )
		{
			sprintf(text, "Cleanup Display List %s ( %d )", list->element_context.GetName().c_str(), (INT)list);
			pi_props->sys_output->Message(text);
		}
		while ( element )
		{
			INT cleanup_element = element->Cleanup();
			if ( ELEMENT_DEBUG && log )
			{
				sprintf(text, "                Element %s ( %d )", element->element_context.GetName().c_str(), (INT)element);
				pi_props->sys_output->Message(text);
			}
			element = element->next;
		}
		list = list->next;
	}


	//  ---- cleanup each page in the atlas ----
	INT cleanup_pages = pi_props->gfx_atlas->CleanupPageTextures();


	// ---- cleanup fonts ----
	INT cleanup_fonts = pi_props->gfx_fonts->Cleanup();


	// ---- cleanup render to image ----
	INT cleanup_render_to_image = pi_props->gfx_render_to_image->Cleanup();

	return cleanup_pages == VS_OK && cleanup_fonts == VS_OK ? VS_OK : VS_ERROR;
}


// ---------- ResetDevice ----------
/*!
\brief (RE)Setup device.
\author Gareth Edwards
\return INT - value returned by device reset
*/
INT GfxInterface::ResetDevice(UINT w, UINT h)
{

	// ---- local ----
	pi_props->gfx_device_status = VS_ERROR;


	// ---- accelerate ----
	GfxDevice *props_gfx_device = pi_props->gfx_device;


	// ---- log  ----
	if ( pi_props->sys_output ) pi_props->sys_output->Message("Reset Graphics Device");


	// ---- reset ----
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

	// ---- accelerate ----
	GfxDevice *props_gfx_device = pi_props->gfx_device;


	// ---- log ----
	if ( pi_props->sys_output ) pi_props->sys_output->Message("Release Graphics Device");


	// ---- release ----
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


/*
hr = d3d9_device->SetRenderState(D3DRS_LIGHTING, TRUE);
hr = d3d9_device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(0.2f, 0.2f, 0.2f, 1.0));
D3DLIGHT9 light;
ZeroMemory(&light, sizeof(light));
light.Type = D3DLIGHT_DIRECTIONAL;
light.Diffuse   = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
light.Direction = D3DXVECTOR3(-1.0f, -0.3f, -1.0f);
*/

/*
ZeroMemory(&light, sizeof(light));
light.Type = D3DLIGHT_POINT;
light.Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
light.Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
light.Range        = 10000.0f;
light.Attenuation0 = 0.0f;
light.Attenuation1 = 0.005f;    // only .125 inverse attenuation
light.Attenuation2 = 0.0f;
*/

/*
hr = d3d9_device->SetLight(0, &light);
hr = d3d9_device->LightEnable(0, TRUE);
*/


/*
D3DMATERIAL9 material;
ZeroMemory(&material, sizeof(D3DMATERIAL9));
material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
hr = d3d9_device->SetMaterial(&material);
*/

/*
hr = d3d9_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
hr = d3d9_device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);


hr = d3d9_device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
hr = d3d9_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
hr = d3d9_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
hr = d3d9_device->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
hr = d3d9_device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

hr = d3d9_device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
hr = d3d9_device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

//hr = d3d9_device->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
//hr = d3d9_device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
*/
