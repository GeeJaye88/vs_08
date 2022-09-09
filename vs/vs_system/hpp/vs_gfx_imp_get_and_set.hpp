////////////////////////////////////////////////////////////////////////////////


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


////////////////////////////////////////////////////////////////////////////////


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
INT GfxInterface::GetClientAdjustedViewrect(VsFloatRectangle *viewrect)
{
	*viewrect = pi_props->client_adjusted_viewrect;
	return VS_OK;
}


// ---------- GetClientAdjustedViewport ----------
INT GfxInterface::GetClientAdjustedViewport(VsFloatRectangle *viewport)
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


// ---------- GetContextOverPanel ----------
/*!
\brief Get context "over" panel
\author Gareth Edwards
\param PanelContext* - over context
\return INT
*/
INT GfxInterface::GetContextOverPanel(
		PanelContext **over
	)
{
	*over = pi_props->context_over_panel;
	return VS_OK;
}


// ---------- GetGfxFloatBuffer ----------
FLOAT * GfxInterface::GetGfxFloatBuffer()
{
	return pi_props->supplied_gfx_float_buffer;
}


INT GfxInterface::GetGfxDevice(IDirect3DDevice9 **d3d9_device)
{
	GfxDevice *props_gfx_device = pi_props->gfx_device;
	*d3d9_device = props_gfx_device->d3d9_device;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- GetElementContext ----------
INT GfxInterface::GetElementContext(INT handle, ElmContext **element_context)
{
	PI_Element *element = (PI_Element *)handle;
	if ( element == NULL) return VS_ERROR;
	*element_context = &element->element_context;
	if ( element_context == NULL) return VS_ERROR;
	return VS_OK;
}


// ---------- GetElementContexts ----------
INT GfxInterface::GetElementContexts(
		INT element_count,
		INT *handles,
		ElmContext **element_context
	)
{
	for (INT e = 0; e < element_count; e++)
	{
		PI_Element *element = (PI_Element *)(*(handles+e));
			if ( element == NULL ) return VS_ERROR;
		*(element_context+e) = &element->element_context;
			if ( element_context+e == NULL ) return VS_ERROR;
	}
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


////////////////////////////////////////////////////////////////////////////////


// ---------- SetBackground ----------
INT GfxInterface::SetBackground(VsColour *c)
{
	pi_props->colour[pi_props->BG] = VS_D3DCOLOR_COLORVALUE(c->red, c->green, c->blue, c->alpha);
	return VS_OK;
}


// ---------- SetBackgroundColor ----------
INT GfxInterface::SetBackgroundColor(
		BOOL background,
		VsColour *background_color,
		VsColour *background_color_over
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
	VsColour *texture_border_color,
	VsColour *texture_border_color_over
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


////////////////////////////////////////////////////////////////////////////////


// ---------- SetBorder ----------
INT GfxInterface::SetBorder(VsFloatRectangle *border)
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

	// ---- length zero -- THEN
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

	// ---- compare length greater than zero 
		else
		{

			PI_Element *list = pi_display_list->first_child;
			while ( list )
			{

				PI_Element *element = list->first_child;
				while ( element )
				{

					// ---- valid element name -- THEN
						ElmContext *element_context = &element->element_context;
						std::string element_name = element_context->GetName();
						if ( element_name != "" )
						{

							// ---- first char matched -- THEN
								if ( element_name[0] == setup_prefix[0] )
								{

									// ---- prefix matched -- ? set setup bitmask
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
INT GfxInterface::SetBorderOver(VsFloatRectangle *border_over)
{
	pi_props->border_over = *border_over;
	return VS_OK;
}


// ---------- SetFPS ----------
INT GfxInterface::SetFPS(UINT fps)
{
	pi_props->fps = fps;
	pi_props->fps_now = fps;
	return VS_OK;
}


// ---------- SetFpsNow ----------
INT GfxInterface::SetFpsNow(UINT fps_now)
{
	pi_props->fps_now = fps_now;
	return VS_OK;
}


// ---------- SetMargin ----------
INT GfxInterface::SetMargin(VsFloatRectangle *margin)
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

\note

   9/9/20 - NOT USED

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


// ---------- SetSysContext ----------
INT GfxInterface::SetSysContext(SysContext *sys_context)
{
	pi_props->sys_context = sys_context;
	return VS_OK;
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
INT GfxInterface::SetViewport(VsFloatRectangle *viewport)
{

	// ---- copy rect into viewport
		pi_props->viewport = *viewport;


	// ---- local
		VsFloatRectangle *border  = &pi_props->border;
		VsFloatRectangle *client_adjusted_viewrect = &pi_props->client_adjusted_viewrect;


	// ---- calculate CAV (Client Adjusted Viewport)
		VsFloatRectangle adjusted_viewport =
		{
			(FLOAT)( (INT)( client_adjusted_viewrect->left   + border->left   + 0.5f ) ),
			(FLOAT)( (INT)( client_adjusted_viewrect->top    + border->top    + 0.5f ) ),
			(FLOAT)( (INT)( client_adjusted_viewrect->right  - border->right  + 0.5f ) ),
			(FLOAT)( (INT)( client_adjusted_viewrect->bottom - border->bottom + 0.5f ) )
		};
	
	// ---- store
		pi_props->client_adjusted_viewport = adjusted_viewport;

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
INT GfxInterface::SetViewrect(VsFloatRectangle *rect)
{

	// ---- local
		VsFloatRectangle *margin = &pi_props->margin;
		FLOAT width = (FLOAT)GetBackBufferWidth();
		FLOAT height = (FLOAT)GetBackBufferHeight();


	// ---- copy rect into viewrect
		pi_props->viewrect = *rect;


	// ---- calculate client adjusted viewrect
		VsFloatRectangle adjusted_viewrect =
		{
			(FLOAT)( (INT)( pi_props->viewrect.left   * width  + margin->left   + 0.5f ) ),
			(FLOAT)( (INT)( pi_props->viewrect.top    * height + margin->top    + 0.5f ) ),
			(FLOAT)( (INT)( pi_props->viewrect.right  * width  - margin->right  + 0.5f ) ),
			(FLOAT)( (INT)( pi_props->viewrect.bottom * height - margin->bottom + 0.5f ) )
		};


	// ---- store
		pi_props->client_adjusted_viewrect = adjusted_viewrect;
		pi_props->client_adjusted_viewport = adjusted_viewrect;

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- AddDisplayList ----------
/*!
\brief Add list
\author Gareth Edwards
\return int - ERROR <= 0 < VS_OK
*/
INT GfxInterface::AddDisplayList(CHAR *name)
{

	// ---- add
		PI_Element *list = pi_display_list->Add(name);

	return (INT)list;
}


// ---------- AddDisplayList ----------
/*!
\brief Add list
\author Gareth Edwards
\return int - ERROR <= 0 < VS_OK
*/
INT GfxInterface::AddDisplayList(
		CHAR *name,
		UINT  group_id,
		UINT  id
	)
{

	// ---- add
		PI_Element *list = pi_display_list->Add(name);
		list->element_context.SetGroupId(group_id);
		list->element_context.SetGroupId(id);

	return (INT)list;
}


// ---------- AddElement ----------
/*!
\brief Add an element to a list
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < element handle
*/
INT GfxInterface::AddElement(
		INT   list_handle,
		CHAR *name,
		UINT  group_id,
		UINT  element_id,
		INT (*callback_get_configure)(ElmContext*)
	)
{

	// ---- list ?
		PI_Element *list = (PI_Element*)list_handle;
		if ( list != NULL && list->type == PI_Element::LIST )
		{

			// ---- add element
				PI_Element *element = list->Add(
						name,
						group_id,
						element_id,
						0,
						callback_get_configure
					);


			// ---- element -- THEN
				if ( element != NULL )
				{

					ElmContext *elm_context = &element->element_context;


					// ---- set "default" element render flags
						elm_context->SetAlphaBlending(pi_props->elm_alpha_blending);
						elm_context->SetColourVertex(pi_props->elm_colour_vertex);
						elm_context->SetOveride(pi_props->elm_overide);
						elm_context->SetMultiState(pi_props->elm_multi_state);
						elm_context->SetSolid(pi_props->elm_solid);
						elm_context->SetTextured(pi_props->elm_texture);
						elm_context->SetVisible(pi_props->elm_visible);
			 

					// ---- set "default" element render modes
						elm_context->SetCullMode(pi_props->elm_cull_mode);


					// ---- debug and output -- ? report
						#if defined(DISPLAY_LIST_DEBUG)
						if ( pi_props->sys_output )
						{
							char text[VS_MAXCHARLEN];
							sprintf(
									text,
									"            Element: %s ( %d )",
									element->element_context.GetName().c_str(),
									(INT)element
								);
							pi_props->sys_output->Message(text);
						}
						#endif
				}

			return (INT)element;
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
		INT   list_handle,
		CHAR *name,
		UINT  group_id,
		UINT  element_id,
		UINT  version,
		INT (*callback_get_configure)(ElmContext*)
	)
{

	// ---- list ?
		PI_Element *list = (PI_Element*)list_handle;
		if ( list != NULL && list->type == PI_Element::LIST )
		{

			// ---- add element
				PI_Element *element = list->Add(
						name,
						group_id,
						element_id,
						version,
						callback_get_configure
					);


			// ---- element ?
				if ( element != NULL )
				{
			
					// ---- set "default" element render flags
						ElmContext *elm_context = &element->element_context;
						elm_context->SetAlphaBlending(pi_props->elm_alpha_blending);
						elm_context->SetColourVertex(pi_props->elm_colour_vertex);
						elm_context->SetOveride(pi_props->elm_overide);
						elm_context->SetMultiState(pi_props->elm_multi_state);
						elm_context->SetSolid(pi_props->elm_solid);
						elm_context->SetTextured(pi_props->elm_texture);
						elm_context->SetVisible(pi_props->elm_visible);

			
					// ---- set "default" element render modes
						elm_context->SetCullMode(pi_props->elm_cull_mode);


					// ---- debug and output -- ? report
						#if defined(DISPLAY_LIST_DEBUG)
						if (  pi_props->sys_output )
						{
							char text[VS_MAXCHARLEN];
							sprintf(
									text,
									"            Element: %s ( %d )",
									element->element_context.GetName().c_str(),
									(INT)element
								);
							pi_props->sys_output->Message(text);
						}
						#endif

				}

		return (INT)element;

	}

	return VS_ERROR;
}


// ---------- DeleteElement ----------
/*!
\brief Delete an element in a list
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < element handle
*/
INT GfxInterface::DeleteElement(
		INT list_handle,
		INT element_type
	)
{

	// ---- list ?
		PI_Element *list = (PI_Element*)list_handle;
		if ( list != NULL && list->type == PI_Element::LIST )
		{

			// ---- element ?
				PI_Element *element = list->first_child;
				while ( element )
				{
					PI_Element *next_element = element->next;

					element->Cleanup();
					delete element;

					element = next_element;
				}

				list->first_child = list->last_child = NULL;
		}

	return VS_ERROR;
}

////////////////////////////////////////////////////////////////////////////////


// ---------- SetElementConfigured ----------
/*!
\brief Set element parameters
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK
*/
INT GfxInterface::SetElementConfigured(
		INT    element_handle,
		BOOL   config,
		UINT   bitmask
	)
{

	PI_Element *element = (PI_Element*)element_handle;
	if ( element != NULL && element->type == PI_Element::ELEMENT )
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
		INT    element_handle,
		UINT   float_parameter_size,
		FLOAT *parameter
	)
{

	PI_Element *element = (PI_Element*)element_handle;
	if ( element != NULL && element->type == PI_Element::ELEMENT )
	{
		element->element_context.SetFloatParameter(float_parameter_size, parameter);
		return VS_OK;
	}

	return VS_ERROR;
}


// ---------- SetDisplayListActiveState ----------
/*!
\brief Set display list types
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK
*/
int GfxInterface::SetDisplayListActiveState()
{

	if ( pi_display_list )
	{

		ElmContext *display_list_context = NULL;
		ElmContext *element_context = NULL;
		PI_Element *display_list = pi_display_list->first_child;
		while ( display_list )
		{

			// ---- count num of active elements
				UINT num_active = 0;
				PI_Element *element = display_list->first_child;
				while ( element )
				{
					element_context = &element->element_context;
					num_active += element_context->GetState() == ElmContext::STATE_ACTIVE ? 1 : 0;
					element = element->next;
				}


			// ---- set active state
				display_list_context = &display_list->element_context;
				display_list_context->SetState(num_active > 0 ? ElmContext::STATE_ACTIVE : ElmContext::STATE_INACTIVE);


			display_list = display_list->next;
		}
	}

	return VS_ERROR;
}


// ---------- SetDisplayListConfigure ----------
/*!
\brief Update config bitmask of all elements in a display list
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK

\note element setup bitmask values:
		VS_ELM_SETUP_NONE        0
		VS_ELM_SETUP_CONFIG      1
		VS_ELM_SETUP_GEOMETRY    2
		VS_ELM_SETUP_MAPPING     4
		VS_ELM_SETUP_INDEX       8
		VS_ELM_SETUP_ALL       255

*/
int GfxInterface::SetDisplayListConfigure(
		INT  display_list_handle,
		UINT element_setup_bitmask
	)
{

	// ---- list ?
		PI_Element *display_list = (PI_Element *)display_list_handle;
		if ( display_list )
		{

			// ---- elements ?
				ElmContext *element_context = NULL;
				PI_Element *element = display_list->first_child;
				while ( element )
				{
					element_context = &element->element_context;
					element_context->SetBitmask(element_setup_bitmask);
					element->element_context_configured = FALSE;
					element = element->next;
				}

		}

	return VS_ERROR;
}


// ---------- SetElementStatus ----------
/*!
\brief Update config bitmask of all elements in a display list
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK

*/
int GfxInterface::SetElementStatus(
		INT  display_list_handle,
		INT  group_id,
		INT  element_id,
		INT  op_id,
		BOOL value
	)
{

	// ---- list ?
		PI_Element *display_list = (PI_Element *)display_list_handle;
		if ( display_list )
		{

			// ---- elements ?
				ElmContext *element_context = NULL;
				PI_Element *element = display_list->first_child;
				while ( element )
				{
					element_context = &element->element_context;
					if (element_context->GetGroupId() == group_id)
					{
						if (element_context->GetId() == element_id)
						{
							element_context->SetSwitchedOn(value);
						}
					}
					element = element->next;
				}

		}

	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////


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


// ---------- SetHandleDisplayList ----------
/*!
\brief Set 
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < child count
*/
INT GfxInterface::SetHandleDisplayList(
		CHAR *name,
		INT handle
	)
{

	PI_Element *elem = pi_display_list->first_child;
	while (elem)
	{
		ElmContext *elm_context = &elem->element_context;

		std::string elm_name = elm_context->GetName();
		if ( !strcmp(elm_name.c_str(), name) )
		{
			ElmContext *elm_context = (ElmContext *)handle;

			elem->element_context = *elm_context;
			//*(elem->element_context) = (ElmContext *)handle;
			return VS_OK;
		}
		elem = elem->next;
	}

	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- GetChildCount ----------
/*!
\brief Get element child count
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < child count
*/
INT GfxInterface::GetChildCount(
		INT list_handle
	)
{

	// ---- list ?
		PI_Element *list = (PI_Element*)list_handle;
		if ( list != NULL && list->type == PI_Element::LIST )
		{
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
		INT list_handle
	)
{

	// ---- lnot list ?
		PI_Element *list = (PI_Element*)list_handle;
		if ( list != NULL && list->type==PI_Element::LIST )
		{
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
		INT element_handle
	)
{
	// ---- element ?
		PI_Element *elem = (PI_Element*)element_handle;
		if ( elem != NULL && elem->type == PI_Element::ELEMENT )
		{
			return (INT) elem->next;
		}

	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////

