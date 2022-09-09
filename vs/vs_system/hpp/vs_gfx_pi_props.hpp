////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Props class ----------

class GfxInterface::PI_Props
{

public:

	// ---- ctor ----
	PI_Props()
	{

		// ---- allocate and create
			gfx_device_status   = VS_ERROR;
			gfx_device          = new GfxDevice;
			gfx_atlas           = new GfxAtlas;
			gfx_fonts           = new GfxFonts;
			for (UINT i=0; i<VS_MAX_SHADERS; i++)
				gfx_shaders[i]  = new GfxShaders;
			gfx_ambient         = new GfxAmbient;
			gfx_fog             = new GfxFog;
			gfx_lights          = new GfxLights;   
			gfx_materials       = new GfxMaterials;
			gfx_view_matrices   = new GfxViewMatrix;
			gfx_render_to_image = new GfxRenderToImage;


		// ---- dx fonts - lash up for BAE & RR presentations
			#if defined(CD3DXFONT)
			for (INT i = 0; i < 8; i++)
			{
				strcpy(dx_font_type[i], "Roboto");
				dx_font_size[i] = i * 10;
				dx_font[i] = NULL;
			}
			#endif


		// ---- set if over
			context_over_panel = NULL;


		// ---- matrices
			vop.Identity(&identity_matrix);
			vop.Identity(&world_matrix);


		// ---- d3d matrices
			memcpy(
					&d3d_identity_matrix.m[0][0],
					&identity_matrix.m[0][0],
					16 * sizeof(FLOAT)
				);
			memcpy(
					&d3d_world_matrix.m[0][0],
					&identity_matrix.m[0][0],
					16 * sizeof(FLOAT)
				);


		// ---- system IO
			sys_input = NULL;
			sys_output = NULL;


		// ---- element render flags
			elm_alpha_blending = FALSE;
			elm_colour_vertex  = FALSE;
			elm_multi_state    = FALSE;
			elm_solid          = TRUE;
			elm_texture        = TRUE;
			elm_visible        = TRUE;
			elm_overide        = FALSE;


		// ---- element render modes
			elm_cull_mode      = ElmContext::CULL_NONE;


		//  ---- grid
			grid_rows = 0;
			grid_cols = 0;
			grid_size = 9;


		// ---- misc
			aaq     = 4;
			fps     = 60;
			fps_now = fps;

		// ---- colour - TBD convert to VsColour
			D3DCOLOR c[24] = 
			{
				VS_D3DCOLOR_COLORVALUE( 0.20f,  0.20f,  0.20f,  1.00f),   // BG
				VS_D3DCOLOR_COLORVALUE( 0.20f,  0.20f,  0.20f,  1.00f),   // BG OFF
				VS_D3DCOLOR_COLORVALUE( 0.20f,  0.20f,  0.20f,  1.00f),   // BG ON
				VS_D3DCOLOR_COLORVALUE( 0.20f,  0.20f,  0.20f,  1.00f),   // BG OVER

				VS_D3DCOLOR_COLORVALUE( 0.61f,  0.65f,  0.69f,  1.00f),   // EDGE
				VS_D3DCOLOR_COLORVALUE( 0.61f,  0.65f,  0.69f,  1.00f),   // EDGE OFF
				VS_D3DCOLOR_COLORVALUE( 0.90f,  0.94f,  0.98f,  1.00f),   // EDGE ON
				VS_D3DCOLOR_COLORVALUE( 0.90f,  0.94f,  0.98f,  1.00f),   // EDGE OVER

				VS_D3DCOLOR_COLORVALUE( 0.61f,  0.65f,  0.69f,  1.00f),   // BORDER
				VS_D3DCOLOR_COLORVALUE( 0.61f,  0.65f,  0.69f,  1.00f),   // BORDER OFF
				VS_D3DCOLOR_COLORVALUE( 0.15f,  0.40f,  1.00f,  1.00f),   // BORDER ON
				VS_D3DCOLOR_COLORVALUE( 1.00f,  1.00f,  1.00f,  1.00f),   // BORDER OVER

				VS_D3DCOLOR_COLORVALUE( 0.20f,  0.20f,  0.20f,  1.00f),   // VIEWRECT
				VS_D3DCOLOR_COLORVALUE( 0.20f,  0.20f,  0.20f,  1.00f),   // VIEWRECT OFF
				VS_D3DCOLOR_COLORVALUE( 0.20f,  0.20f,  0.20f,  1.00f),   // VIEWRECT ON
				VS_D3DCOLOR_COLORVALUE( 0.20f,  0.20f,  0.20f,  1.00f),   // VIEWRECT OVER

				VS_D3DCOLOR_COLORVALUE( 0.20f,  0.20f,  0.20f,  1.00f),   // VIEWPORT
				VS_D3DCOLOR_COLORVALUE( 0.20f,  0.20f,  0.20f,  1.00f),   // VIEWPORT OFF
				VS_D3DCOLOR_COLORVALUE( 0.20f,  0.20f,  0.20f,  1.00f),   // VIEWPORT ON
				VS_D3DCOLOR_COLORVALUE( 0.30f,  0.30f,  0.30f,  1.00f),   // VIEWPORT OVER

				VS_D3DCOLOR_COLORVALUE( 0.30f,  0.30f,  0.30f,  1.00f),   // TEXT
				VS_D3DCOLOR_COLORVALUE( 0.80f,  0.80f,  0.80f,  1.00f),   // TEXT OFF
				VS_D3DCOLOR_COLORVALUE( 0.98f,  0.98f,  0.98f,  1.00f),   // TEXT ON
				VS_D3DCOLOR_COLORVALUE( 0.98f,  0.98f,  0.98f,  1.00f),   // TEXT OVER
			};
			for ( UINT i=0; i<24; i++ ) colour[i] = c[i];

		
		// ---- frame colors
			frame_background = FALSE;
			frame_background_color = VS_D3DCOLOR_COLORVALUE(0, 0, 0, 1);
			frame_background_color_over = VS_D3DCOLOR_COLORVALUE(0, 0, 0, 1);


		// ---- frame texture
			frame_texture_border = FALSE;
			frame_texture_border_color = VS_D3DCOLOR_COLORVALUE(0, 0, 0, 1);
			frame_texture_border_color_over = VS_D3DCOLOR_COLORVALUE(0, 0, 0, 1);


		// ---- supplied
			memset(supplied_gfx_float_buffer, 0, 1024*sizeof(FLOAT));


		// ---- cursor
			POINT p = { 0, 0 };
			cursor_position = p;


		// ---- R2I
			r2i = FALSE;


		// ---- pick panel
			pick_panel_flag = FALSE;
			handle_pick_panel = NULL;
			pick_panel_handle_app_obj = 0;


		// ---- pick element
			pick_element_flag = FALSE;
			pick_element_callback = NULL;
			pick_element_handle = 0;


		// ---- rectangles
			VsFloatRectangle r = { 0, 0, 0, 0 };
			margin      = r;
			border      = r;
			border_over = r;
			viewrect    = r;
			viewport    = r;
			client_adjusted_viewrect = r;
			client_adjusted_viewport = r;

	};


	// ---- dtor ----
	~PI_Props()
	{
		if ( gfx_device != NULL ) { delete gfx_device; gfx_device = NULL; }

		// TBD - add the rest
	};


	// ---- boolean tests
		BOOL isOverViewrect();
		BOOL isOverViewport();
		BOOL IsOver2DElement(PI_Element *);
		BOOL IsOver3DElement(PI_Element *);
		BOOL isPickPanel(INT);
		BOOL isPickElement(INT);


	// ---- callback
		INT  CallbackElement(INT);


	// ---- objects
		INT gfx_device_status;
		GfxDevice        *gfx_device;
		GfxAtlas         *gfx_atlas;
		GfxFonts         *gfx_fonts;
		GfxShaders       *gfx_shaders[VS_MAX_SHADERS];
		GfxAmbient       *gfx_ambient;
		GfxFog           *gfx_fog;
		GfxLights        *gfx_lights;
		GfxMaterials     *gfx_materials;
		GfxViewMatrix    *gfx_view_matrices;
		GfxRenderToImage *gfx_render_to_image;


	// ---- dx fonts
		#if defined(CD3DXFONT)
		CHAR dx_font_type[8][VS_MAXCHARNAME];
		INT  dx_font_size[8];
		CD3DFont *dx_font[8];
		#endif


	// ---- properties
		D3DMATERIAL9  material[VS_MAX_MATERIALS];


	// ---- panel "over" context
		PanelContext *context_over_panel;

	
	// ---- matrices
		VsMatrix   identity_matrix;
		VsMatrix   world_matrix;
		VsMatrixOp vop;


	// ---- d3d matrices
		D3DXMATRIX d3d_identity_matrix; // reset to this after using...
		D3DXMATRIX d3d_world_matrix;    // this....


	//  ---- system IO - injected dependency
		SysContext *sys_context;
		SysInput   *sys_input;
		SysOutput  *sys_output;


	// ---- element render flags
		BOOL elm_alpha_blending;
		BOOL elm_colour_vertex;
		BOOL elm_multi_state;
		BOOL elm_solid;
		BOOL elm_texture;
		BOOL elm_visible;
		BOOL elm_overide;


	// ---- element render modes
		UINT elm_cull_mode;


	// ---- panel grid
		UINT grid_rows;
		UINT grid_cols;
		UINT grid_size;


	// ---- misc
		UINT  aaq;
		UINT  fps;
		UINT  fps_now;


	// ---- colour
		enum Color_Index
		{
			BG       =  0,  BG_OFF       =  1,  BG_ON       =  2,  BG_OVER       =  3,
			EDGE     =  4,  EDGE_OFF     =  5,  EDGE_ON     =  6,  EDGE_OVER     =  7,
			BORDER   =  8,  BORDER_OFF   =  9,  BORDER_ON   = 10,  BORDER_OVER   = 11,
			VIEWRECT = 12,  VIEWRECT_OFF = 13,  VIEWRECT_ON = 14,  VIEWRECT_OVER = 15,
			VIEWPORT = 16,  VIEWPORT_OFF = 17,  VIEWPORT_ON = 18,  VIEWPORT_OVER = 19,
			TEXT     = 20,  TEXT_OFF     = 21,  TEXT_ON     = 22,  TEXT_OVER     = 23
		};
		D3DCOLOR colour[24];


	// ---- frame color
		BOOL     frame_background;
		D3DCOLOR frame_background_color;
		D3DCOLOR frame_background_color_over;
		BOOL     frame_texture_border;
		D3DCOLOR frame_texture_border_color;
		D3DCOLOR frame_texture_border_color_over;


	// ---- supplied element configuration data
		FLOAT supplied_gfx_float_parameter[VS_MAX_ELM_GFX_FLOAT_PARAMETER];
		FLOAT supplied_gfx_float_buffer[VS_MAX_ELM_GFX_FLOAT_BUFFER];


	// ---- cursor
		POINT cursor_position;


	// ---- R2I
		BOOL r2i;


	// ---- pick panel
		BOOL  pick_panel_flag;
		INT   (*handle_pick_panel)(INT, INT);
		INT   pick_panel_handle_app_obj;


	// ---- pick element
		BOOL  pick_element_flag;
		INT   (*pick_element_callback)(INT, INT);
		INT   pick_element_handle;


	// ---- pick ray
		D3DXVECTOR3 viewport_ray;
		D3DXVECTOR3 origin_ray;
		D3DXVECTOR3 direction_ray;
		FLOAT       nearest_ray_zed = 0;
		INT         nearest_ray_handle = 0;


	// ---- rectangles
		VsFloatRectangle margin;
		VsFloatRectangle border;
		VsFloatRectangle border_over;
		VsFloatRectangle viewrect;
		VsFloatRectangle viewport;
		VsFloatRectangle client_adjusted_viewrect;
		VsFloatRectangle client_adjusted_viewport;

};


// ---------- isOverViewrect ----------
/*!
\brief Test if over viewrect
\author Gareth Edwards
\return BOOL - IF over viewrect THEN true ELSE false
*/
BOOL GfxInterface::PI_Props::isOverViewrect()
{

	LONG cx = cursor_position.x;
	LONG cy = cursor_position.y;
	if ( cx >= client_adjusted_viewrect.left && cx <= client_adjusted_viewrect.right )
	{
		if ( cy >= client_adjusted_viewrect.top && cy <= client_adjusted_viewrect.bottom )
		{
			return true;
		}
	}

	return false;
}


// ---------- isOverViewport ----------
/*!
\brief Test if over viewport
\author Gareth Edwards
\return BOOL - IF over viewport THEN true ELSE false
*/
BOOL GfxInterface::PI_Props::isOverViewport()
{

	LONG cx = cursor_position.x;
	LONG cy = cursor_position.y;
	if (cx >= client_adjusted_viewport.left && cx <= client_adjusted_viewport.right)
	{
		if (cy >= client_adjusted_viewport.top && cy <= client_adjusted_viewport.bottom)
		{
			return true;
		}
	}

	return false;
}


// ---------- IsOver2DElement ----------
/*!
\brief Test if over 2D element
\author Gareth Edwards
\return BOOL - IF over element THEN true ELSE false
*/
BOOL GfxInterface::PI_Props::IsOver2DElement(PI_Element *element)
{

	VsFloatRectangle *rect = &element->rect_bounds;

	LONG cx = cursor_position.x;
	LONG cy = cursor_position.y;
	if ( cx >= rect->left && cx <= rect->right )
	{
		if ( cy >= rect->top && cy <= rect->bottom )
		{
			return TRUE;
		}
	}

	return FALSE;
}


// ---------- IsOver3DElement ----------
/*!
\brief Test if over 3D element
\author Gareth Edwards
\return BOOL - IF over element THEN true ELSE false
*/
BOOL GfxInterface::PI_Props::IsOver3DElement(PI_Element *element)
{

	if ( sys_input->GetBool(SysInput::LBUTTON_DOWN) )
	{

		// ---- object bounds
			VsFloatSphere *sp = &element->sphere;


		// ---- object location
			D3DXVECTOR3 loc3d = { 0, 0, 0 };


		// ---- transform location
			IDirect3DDevice9 *d3d9_device = gfx_device->d3d9_device;
			D3DXMATRIX world;
			d3d9_device->GetTransform(D3DTS_WORLD, &world);
			D3DXVec3TransformCoord(&loc3d, &loc3d, &world);


		// ---- get inverse of view matrix
			D3DXMATRIX iw;
			D3DXMatrixInverse(&iw, NULL, &world);


		// ---- transform the ray origin and the ray direction from view space to world space
			D3DXVECTOR3  rayOrigin, rayDirection;
			D3DXVec3TransformNormal(&rayDirection, &direction_ray, &iw);
			D3DXVec3TransformCoord(&rayOrigin, &origin_ray, &iw);


		// ---- normalise the ray direction
			D3DXVec3Normalize(&rayDirection, &rayDirection);


		// ---- calculate the a, b, and c coefficients
			FLOAT a = ( rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
			FLOAT b = ((rayDirection.x * rayOrigin.x   ) + (rayDirection.y * rayOrigin.y   ) + (rayDirection.z * rayOrigin.z)  ) * 2.0f;
			FLOAT c = ((rayOrigin.x    * rayOrigin.x   ) + (rayOrigin.y    * rayOrigin.y   ) + (rayOrigin.z    * rayOrigin.z)  ) - (sp->radius * sp->radius);


		// ---- find the discriminant
			FLOAT discriminant = (b * b) - (4 * a * c);


		// ---- if discriminant is negative the picking ray missed the sphere,

			if( discriminant < 0.0f )
			{
				return FALSE;
			}
			else
			{
				// ---- if a solution is >= 0, and the polynomial has two real roots
					// then there are intersections with the sphere, and use
					// the smallest as this is the closest intersect to the eye 
					discriminant = sqrtf(discriminant);
					float s0 = (-b + discriminant) / 2.0f;
					float s1 = (-b - discriminant) / 2.0f;
					if( s0 >= 0.0f || s1 >= 0.0f )
					{
						if ( s1 < nearest_ray_zed )
						{
							nearest_ray_handle = (INT)element;
							nearest_ray_zed = s1;
							return TRUE;
						}
					}
			}
	}

	return FALSE;
}


// ---------- isPickPanel ----------
/*!
\brief IF content picked then invoke element callback
\author Gareth Edwards
\return BOOL - IF pick element THEN true ELSE false
*/
BOOL GfxInterface::PI_Props::isPickPanel(INT handle_panel_obj)
{

	if ( pick_panel_flag && handle_pick_panel != NULL )
	{
		if ( sys_input != NULL && sys_input->GetBool(SysInput::LBUTTON_DOWN) )
		{
			INT result = handle_pick_panel(pick_panel_handle_app_obj, handle_panel_obj);
			pick_panel_flag = false;
			return true;
		}
	}

	return false;
}


// ---------- isPickElement ----------
/*!
\brief IF element picked then invoke element callback
\author Gareth Edwards
\return BOOL - IF pick element THEN true ELSE false
*/
BOOL GfxInterface::PI_Props::isPickElement(INT element_handle)
{

	if ( pick_element_flag && pick_element_callback != NULL )
	{
		if ( sys_input != NULL && sys_input->GetBool(SysInput::LBUTTON_DOWN) )
		{
			pick_element_flag = false;
			return true;
		}
	}

	return false;
}


// ---------- CallbackElement ----------
/*!
\brief Invoke element callback
\author Gareth Edwards
\return INT - VS_ERROR OR callback error value <= VS_OK
*/
INT GfxInterface::PI_Props::CallbackElement(INT element_handle)
{

	if ( pick_element_callback != NULL )
	{
		INT result = pick_element_callback(
				pick_element_handle,
				element_handle
			);
		//cursor_position.x = 0;
		//cursor_position.y = 0;
		return result;
	}

	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////
