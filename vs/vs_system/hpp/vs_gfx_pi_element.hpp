////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of Element class ----------

// TBD : Bullet proof this from user - especially get data and geometry callbacks

class GfxInterface::PI_Element
{

public:


	// ---- type
		enum PI_Element_Type
			{
				UNKNOWN = 0,
				ROOT    = (INT)'R',
				LIST    = (INT)'L',
				ELEMENT = (INT)'E'
			};


	// ---- c/dtor
		PI_Element()
		{

			// buffers
			index_buffer  = NULL;
			vertex_buffer = NULL;

			// flags
			element_context_configured = FALSE;
			over          = FALSE;
			picked        = FALSE;
			report_later  = FALSE;

			// font
			font          = NULL;

			// notes
			type          = UNKNOWN;
			dimension     = 2;
			children      = 0;

			// hierarchy
			parent        = NULL;
			next          = NULL;
			last          = NULL;
			first_child   = NULL;
			last_child    = NULL;

			// callback
			callback_get_configure = NULL;

		};


	~PI_Element()
	{
		if ( index_buffer  != 0 ) index_buffer->Release();
		if ( vertex_buffer != 0 ) vertex_buffer->Release();
	};


	// ---- buffers
		LPDIRECT3DINDEXBUFFER9  index_buffer;
		LPDIRECT3DVERTEXBUFFER9 vertex_buffer;


	// ---- flags
		BOOL element_context_configured;
		BOOL over;
		BOOL picked;
		BOOL report_later;


	// ---- font
		GfxFont *font;


	// ---- notes
		CHAR type;         // PI_Element_Type
		CHAR dimension;    // PI_Element_Dimension
		UINT children;     // increment by AddElement


	// ---- hierarchy
		PI_Element *parent;
		PI_Element *next;
		PI_Element *last;
		PI_Element *first_child;
		PI_Element *last_child;


	// ---- callback to configure element context
		INT (*callback_get_configure)(ElmContext*);


	// ---- data
		ElmContext element_context;
		VsFloatRectangle rect_bounds;
		VsFloatBox       box;
		VsFloatSphere    sphere;


	// ---- methods
		PI_Element* Add(CHAR *);
		PI_Element* Add(CHAR *, UINT, UINT, UINT, INT(*)(ElmContext*));
		INT Set(UINT, FLOAT*);
		INT SetConfigured(BOOL, UINT);
		INT Cleanup();
		INT Setup(IDirect3DDevice9 *);
		INT Bounds();


	// ---- draw text methods
		INT DrawText(UINT);
		INT DrawTextVertical(UINT);

};


// ---------- Add ----------
/*!
\brief Node link an element.
\author Gareth Edwards
\return :PI_Element *
*/
GfxInterface::PI_Element * GfxInterface::PI_Element::Add(CHAR *name)
{

	// ---- link
		PI_Element *child = new PI_Element();
		child->parent = this;
		if (last_child != NULL)
		{
			last_child->next = child;
			child->last = last_child;
			last_child = child;
		}
		else
		{
			first_child = child;
			last_child = child;
		}


	// ---- count
		this->children++;


	// ---- props
		std::string string_name = name;
		child->element_context.SetName(string_name);
		child->type = LIST;

	return child;
}


// ---------- Add ----------
/*!
\brief Node link an element.
\author Gareth Edwards
\return :PI_Element *
*/
GfxInterface::PI_Element * GfxInterface::PI_Element::Add(
	CHAR *name,
	UINT  group_id,
	UINT  element_id,
	UINT  version,
	INT(*callback_get_configure)(ElmContext*)
	)
{

	// ---- link
		PI_Element *child = new PI_Element();
		child->parent = this;
		if (last_child != NULL)
		{
			last_child->next = child;
			child->last = last_child;
			last_child = child;
		}
		else
		{
			first_child = child;
			last_child = child;
		}


	// ---- count
		this->children++;


	// ---- context
		std::string string_name = name;
		child->element_context.SetName(string_name);
		child->element_context.SetGroupId(group_id);
		child->element_context.SetId(element_id);
		child->element_context.SetVersion(version);


	// ---- prop
		child->type = ELEMENT;
		child->callback_get_configure = callback_get_configure;

	return child;
}


// ---------- Set ----------
/*!
\brief Set data parameters
\author Gareth Edwards
\return INT - ERROR # < VS_ERROR < VS_OK 
*/
INT GfxInterface::PI_Element::Set(
		UINT float_parameter_size,
		FLOAT *parameter
	)
{
	element_context.SetFloatParameter(float_parameter_size, parameter);
	return VS_OK;
}


// ---------- SetConfigured ----------
/*!
\brief Set data parameters
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK
*/
INT GfxInterface::PI_Element::SetConfigured(BOOL config, UINT bitmask)
{
	element_context_configured = config;
	element_context.SetBitmask(bitmask);
	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Recursively cleanup . 
\author Gareth Edwards
\return PI_Element *
*/
INT GfxInterface::PI_Element::Cleanup()
{

	if ( index_buffer != 0 )
	{
		index_buffer->Release();
		index_buffer = 0;
	}
	if ( vertex_buffer != 0 )
	{
		vertex_buffer->Release();
		vertex_buffer = 0;
	}
	element_context_configured = false;
	element_context.SetBitmask(VS_ELM_SETUP_ALL);

	return VS_OK;
}


// ---------- Setup ----------
/*!
\brief Set data parameters
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK
\Note Gosh blimey - "Lord, what fools these mortals be!"
*/
INT GfxInterface::PI_Element::Setup(IDirect3DDevice9 *d3d9_device)
{

	// ---- TRY to setup
		try
		{
			// ---- local
				INT result;
				HRESULT hr;


			// ---- bitmask NULL ? -- THEN force complete setup
				UINT setup_bitmask = element_context.GetSetupBitmask();
				if ( vertex_buffer == NULL ) setup_bitmask = VS_ELM_SETUP_ALL;


			// TTD: test that element has callbacks...


			// ---- IF -- setup -- THEN config & create
				if ( setup_bitmask & VS_ELM_SETUP_CONFIG )
				{

					// ---- configure object
						if ( result = callback_get_configure(&element_context) != VS_OK ) throw (result);


					// ---- create vertex buffer
						if ( vertex_buffer != NULL )
						{
							if (hr = vertex_buffer->Release() != S_OK) throw ((INT)hr);
							vertex_buffer = NULL;
						}
						if ( hr = d3d9_device->CreateVertexBuffer(
								element_context.GetPrimitiveBytes(),
								D3DUSAGE_WRITEONLY,
								element_context.GetVertexSize(),
								D3DPOOL_MANAGED,
								&vertex_buffer,
								NULL
							) != S_OK ) throw ( (INT)hr );


					// ---- create index buffer
						UINT index_count = element_context.GetIndexCount();
						if ( index_count > 0 )
						{
							if ( index_buffer != NULL )
							{
								if ( hr = index_buffer->Release() != S_OK ) throw ((INT)hr);
								index_buffer = NULL;
							}

							BOOL      dynamic = TRUE;

							#if VS_INDEX_BUFFER_FORMAT == 32
							D3DFORMAT format = D3DFMT_INDEX32;
							#else
							D3DFORMAT format = D3DFMT_INDEX16;
							#endif

							D3DPOOL pool  = dynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;
							UINT    size  = (format == D3DFMT_INDEX32) ? sizeof(UINT) : sizeof(USHORT);
							DWORD   usage = dynamic ? D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC : D3DUSAGE_WRITEONLY;

							if (
									hr = d3d9_device->CreateIndexBuffer(
											index_count*size,
											usage,
											format,
											pool,
											&index_buffer,
											NULL
										) != S_OK
								) throw ((INT)hr);

						}

					}


				// ---- setup vertex buffer ----
					if ( setup_bitmask & VS_ELM_SETUP_GEOMETRY || setup_bitmask & VS_ELM_SETUP_MAPPING )
					{
						FLOAT *f;
						if ( hr = vertex_buffer->Lock( 0, 0, (void**)&f, 0) != S_OK ) throw ( (INT)hr );
						{
							result = element_context.SetVertexBufferPtr((FLOAT*)f);             // set target
							result = element_context.VertexBufferCallback(&element_context);    // fill
							if ( !result )
							{
								hr = vertex_buffer->Unlock();
								throw (result);
							}
							if ( result = Bounds() != VS_OK )
							{
								hr = vertex_buffer->Unlock();
								throw (result);                 
							}

						}
						if ( hr = vertex_buffer->Unlock() != S_OK ) throw ( (INT)hr );
					}


				// ---- setup index Buffer ----
					if ( setup_bitmask & VS_ELM_SETUP_INDEX )
					{
						UINT index_count = element_context.GetIndexCount();
						if ( index_count > 0 )
						{
							#if VS_INDEX_BUFFER_FORMAT == 32
							DWORD *i;
							#else
							WORD *i;
							#endif
							if (hr = index_buffer->Lock(0, sizeof(i), (void**)&i, 0) != S_OK) throw ((INT)hr);
							{
								#if VS_INDEX_BUFFER_FORMAT == 32
								result = element_context.SetIndexBufferPtr((DWORD*)i);
								#else
								result = element_context.SetIndexBufferPtr((WORD*)i);
								#endif
								result = element_context.IndexBufferCallback(&element_context);
								if ( !result )
								{
									hr = index_buffer->Unlock();
									throw (result);
								}
							}
							if (hr = index_buffer->Unlock() != S_OK) throw ((INT)hr);
						}

					}
		}

		//  ---- CATCH error, report and return error id
			catch ( INT error_id )
			{
				printf("Setup element failed with error ID: %d", error_id);
				element_context_configured = TRUE;
				element_context.SetBitmask(VS_ELM_SETUP_ALL);
				return error_id;
			}


	// ---- ok
		element_context_configured = TRUE;
		element_context.SetBitmask(VS_ELM_SETUP_ALL);

	return VS_OK;
}


// ---------- Bounds ----------
/*!
\brief Set dimension and store 2D or 3D bounds
\author Gareth Edwards
\return INT - ERROR # < VS_ERROR < VS_OK 
*/
INT GfxInterface::PI_Element::Bounds()
{

	// ---- local
		FLOAT *f = element_context.GetVertexBufferPtr();
		DWORD vertex_count = (DWORD)element_context.GetVertexCount();
		DWORD vertex_size = (DWORD)element_context.GetVertexSize();


	// ---- dimension
		dimension = (DWORD)element_context.GetVertexFormat() & VS_XYZRHW ? 2 : 3;


	// ---- local minimax
		FLOAT big_number = (FLOAT)INT_MAX;
		FLOAT max[3];
		FLOAT min[3];
		for ( UINT j=0; j<(UINT)dimension; j++ ) { max[j] = -big_number; min[j] = big_number; }


	// ---- each vertex & ordinate
		for ( DWORD i=0; i<vertex_count; i++ )
		{
			for ( UINT j=0; j<(UINT)dimension; j++ )
			{
				FLOAT *v = f+j;
				max[j] = *v > max[j] ? *v : max[j];
				min[j] = *v < min[j] ? *v : min[j];
			}
			f += vertex_size;
		}


	// ---- initialise
		if ( dimension == 2 )
		{
			rect_bounds.left  = min[0]; rect_bounds.right  = max[0];
			rect_bounds.top   = min[1]; rect_bounds.bottom = max[1];
		}
		else
		{

			box.left  = min[0]; box.right  = max[0];
			box.top   = max[1]; box.bottom = min[1];
			box.back  = min[2]; box.front  = max[2];

			sphere.x = ( box.right + box.left ) / 2;
			sphere.y = ( box.top + box.bottom ) / 2;
			sphere.z = ( box.front + box.back ) / 2;

			FLOAT w = box.right - box.left;
			FLOAT h = box.top - box.bottom;
			FLOAT d = box.front - box.back;
			FLOAT m = w > h ? w : h;
			      m = d > m ? d : m;

			 sphere.radius = m / 2;

		}

	return VS_OK;
}


// ---------- DrawText ----------
/*!
\brief Render text 
\author Gareth Edwards
\return INT - ERROR # < VS_ERROR < VS_OK 
*/
INT GfxInterface::PI_Element::DrawText(UINT d3d_col)
{

	// ---- only 2D permissable ----
	if ( dimension != 2 ) return VS_ERROR;


	// IF -- NO font or text -- THEN -- return
	if ( font == NULL ) return VS_ERROR;
	if ( font->id3dx_font == NULL ) return VS_ERROR;
	if ( element_context.GetText() == "" ) return VS_ERROR;

	// TEMP
	//
	//char txt[128];
	//strcpy_s(txt, 128, element_context.GetText().c_str());
	//txt[4] = 0;
	//if (!strcmp(txt, "Text"))
	//{
	//	OutputDebugString(element_context.GetText().c_str());
	//	OutputDebugString("\n");
	//}

	// IF -- NO alignement -- THEN -- return;
	UINT font_alignement = element_context.GetFontAlignment();
	if ( font_alignement == vs_system::ElmContext::ALIGN_NONE ) return VS_ERROR;


	// ---- calc text dimensions ----
	RECT rect_text = { 0, 0, 0, 0 };
	HRESULT hr = font->id3dx_font->DrawText(
			0,
			element_context.GetText().c_str(),
			-1,
			&rect_text,
			DT_SINGLELINE + DT_CALCRECT,
			0
		);
	INT text_width  = (INT)rect_text.right;
	INT text_height = (INT)rect_text.bottom;


	// ---- align text within element ----
	RECT rect_copy = { 0, 0, 0, 0 };
	INT x_offset = 0;
	INT y_offset = 0;
	switch (font_alignement)
	{
		case vs_system::ElmContext::ALIGN_CENTRED_ON_TEXT:
			{
				INT cx = (INT)( (rect_bounds.left + rect_bounds.right ) / 2 + 0.5f);
				INT cy = (INT)( (rect_bounds.bottom + rect_bounds.top ) / 2 + 0.5f);
				RECT temp =
				{
					cx - text_width  / 2,
					cy - text_height / 2,
					cx + text_width  / 2 + 1,
					cy + text_height / 2 + 1,
				};
				rect_copy = temp;
			}
			break;

		case vs_system::ElmContext::ALIGN_CENTRE:
			{
				INT element_width  = (INT)(rect_bounds.right - rect_bounds.left + 0.5 );
				INT element_height = (INT)(rect_bounds.bottom - rect_bounds.top + 0.5 );
				x_offset = (element_width - text_width) / 2 + 1;
				y_offset = (element_height - text_height) / 2;
			}

		case vs_system::ElmContext::ALIGN_LEFT:
		default:
			{
				INT element_height = (INT)(rect_bounds.bottom - rect_bounds.top + 0.5 );
				y_offset = (element_height - text_height) / 2;
				RECT temp =
				{
					x_offset + (INT)rect_bounds.left,
					y_offset + (INT)rect_bounds.top,
					x_offset + (INT)rect_bounds.left + text_width + 1,
					y_offset + (INT)rect_bounds.top + text_height + 1
				};
				rect_copy = temp;
			}
			break;
	}


	// ---- draw ----
	hr = font->id3dx_font->DrawText(
			0,                                 // sprite
			element_context.GetText().c_str(), // string
			-1,                                // count the chars
			&rect_copy,                        // rectangular area
			0,                                 // flags
			d3d_col                            // colour
		);

	return VS_OK;
}


// ---------- DrawTextVertical ----------
/*!
\brief Render text verticaly
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK
*/
INT GfxInterface::PI_Element::DrawTextVertical(UINT d3d_col)
{

	// ---- only 2D permissable ----
	if ( dimension != 2 ) return VS_ERROR;


	// IF -- font or context NOT OK -- THEN -- return
	if ( font == NULL ) return VS_ERROR;
	if ( font->id3dx_font == NULL ) return VS_ERROR;
	if ( element_context.GetText() == "" ) return VS_ERROR;


	// IF -- no alignement -- THEN -- return;
	UINT font_alignement = element_context.GetFontAlignment();
	if ( font_alignement == vs_system::ElmContext::ALIGN_NONE ) return VS_ERROR;


	// ---- local ----
	HRESULT hr = 0;
	CHAR c[2] = "";
	RECT rect_text = { 0, 0, 0, 0 };


	// ----copy string into char ----
	CHAR tv[VS_MAXCHARNAME];
	strcpy(tv, element_context.GetText().c_str());
	UINT len = (UINT)strlen(tv);


	// ---- calculate vertical dimensions ----
	UINT text_width = 0;
	UINT text_height = 0;
	UINT th = 0;
	for (UINT i = 0; i < len; i++)
	{
		c[0] = tv[i];
		hr = font->id3dx_font->DrawText(
				0,
				c,
				-1,
				&rect_text,
				DT_SINGLELINE + DT_CALCRECT,
				0
			);

		if (hr <= 0)
		{
			float fred = 2;
		}

		text_width += (UINT)rect_text.right;
		th += (UINT)rect_text.bottom;
	}
	text_width = (INT)( (FLOAT)text_width / (FLOAT)len + 0.5f );


	// ---- store original text_height to ensure rect_align is "deep" enough ----
	UINT original_text_height = th;


	// ---- scale text height to compress vertical layouts
	text_height = (INT)((FLOAT)th * 0.8f);


	// ---- set FLOAT vertical decrement ----
	FLOAT f_text_decr = (FLOAT)text_height / (FLOAT)len + 0.5f;


	// ---- set element dimensions ----
	UINT element_width  = (UINT)(rect_bounds.right - rect_bounds.left + 0.5f );
	UINT element_height = (UINT)(rect_bounds.bottom - rect_bounds.top + 0.5f );


	// ---- align text within element
	INT x_offset = 0;
	INT y_offset = 0;
	switch (font_alignement)
	{
		case vs_system::ElmContext::ALIGN_CENTRE: // was VS_ALIGN_CENTRE:
			x_offset = (element_width - text_width) / 2 + 1;
			y_offset = (element_height - text_height) / 2;
			break;
		default:
			break;
	}


	// ---- draw each character
	for (UINT i = 0; i < len; i++)
	{
		INT ymin = y_offset + (INT)(f_text_decr * (FLOAT)i + 0.5) - 1;
		RECT rect_align =
		{
			(LONG)( x_offset + (INT)rect_bounds.left ),
			(LONG)( ymin     + (INT)rect_bounds.top ),
			(LONG)( x_offset + (INT)rect_bounds.left + text_width + 1 ),
			(LONG)( ymin     + (INT)rect_bounds.top + original_text_height + 1 )
		};
		c[0] = tv[i];
		hr = font->id3dx_font->DrawText(
				0,                                 // sprite
				c,                                 // string
				-1,                                // count the chars
				&rect_align,                       // rectangular area
				0,                                 // flags
				d3d_col                            // colour
			);
	}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
