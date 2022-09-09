////////////////////////////////////////////////////////////////////////////////

// ---------- vs_elm_context.cpp ----------
/*!
\file vs_elm_context.cpp
\brief Implementation of the El(e)m(ent)Context class
\author Gareth Edwards 
*/

// ---- include ----
#include "../header/vs_elm_context.h"
#include <locale> // std::locale, std::toupper


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_ElmContext class ----------

class ElmContext::PI_ElmContext
{

	public:


	// ---- ctor
		PI_ElmContext()
		{

			// ---- properties
				name           = "";
				text           = "";


			// ---- dynamic status
				actions        = 0;
				align          = ALIGN_LEFT;
				atlas_mode     = VS_ATLAS_PAGE_GROUP_ONE;
				behaviour      = BEHAVIOUR_NULL;
				sub_behaviour  = SUB_BEHAVIOUR_NULL;
				setup_bitmask  = VS_ELM_SETUP_ALL;
				state          = STATE_INACTIVE;

			// ---- font
				font_name      = "";
				font_alignment = ALIGN_NONE;
				font_height    = 0;


			// ---- logical id's
				group_id       = 0;
				id             = 0;


			// ---- indices
				material_index    = 0;
				shader_index      = 0;
				view_matrix_index = 0;


			// ---- creation status 
				version        = 0;
				report         = FALSE;
				runtime        = TRUE;
				display        = TRUE;


			// ---- colour
				use_rgba  = FALSE;
				red       = 0.25f;
				green     = 0.25f;
				blue      = 0.25f;
				alpha     = 0;
				d3d_color = 0;


			// ---- font colour - not used
				VsColour c[4] =
				{
					{ 0.30f, 0.30f, 0.30f, 1.00f },   // NULL
					{ 0.80f, 0.80f, 0.80f, 1.00f },   // OFF
					{ 0.98f, 0.98f, 0.98f, 1.00f },   // ON
					{ 0.98f, 0.98f, 0.98f, 1.00f },   // OVER
				};
				for ( UINT i=0; i<4; i++ ) font_colour[i] = c[i];


			// ---- float parameter
				for (UINT i = 0; i<VS_MAX_ELM_FLOAT_PARAMETER; i++) float_parameter[i] = 0;


			// ---- long parameter
				for (UINT i = 0; i<8; i++) long_parameter[i] = 0;


			// ---- data
				supplied_gfx_float_parameter  = NULL;
				supplied_gfx_float_buffer = NULL;


			// ---- atlas page
				for (UINT i = 0; i<VS_MAX_ELM_ATLAS_PAGE_REFERENCES; i++) atlas_page[i] = 0;


			// ---- render flags
				alpha_blending       = FALSE;
				colour_vertex        = FALSE;
				fog                  = FALSE;
				lit                  = FALSE;
				pick_on_radio_button = FALSE;
				pick_2D              = TRUE;
				pick_3D              = FALSE;
				shaded               = FALSE;
				switched_on          = FALSE;
				switched_on_prev     = FALSE;
				solid                = TRUE;
				text_outline         = TRUE;
				text_vertical        = FALSE;
				texture              = TRUE;
				texture_mirror       = FALSE;
				visible              = TRUE;
				overide              = FALSE;


			// ---- render modes
				cull_mode          = CULL_NONE;


			// ---- index buffer
				//index_count        = 0;
				//index_buffer       = NULL;


			// ---- multi-state texturing
				matrix_active      = FALSE;


			// ---- multi-state texturing
				multi_active       = TRUE;
				multi_null         = 0;
				multi_state        = FALSE;


			// ---- vertex buffer
				vertex_buffer      = NULL;
				vertex_count       = 0;
				vertex_format      = 0;
				vertex_size        = 0;


			// ---- primitive parameters
				primitive_bytes    = 0;
				primitive_count    = 0;
				primitive_type     = (VS_PrimitiveType)0;

		};


	// ---- dtor
		~PI_ElmContext()
		{
			;
		};


	// ---- initliased at runtime


	// ---- general text properties
		std::string name;
		std::string text;


	// ---- status
		UINT   actions;            // 0
		UINT   align;              // e.g. ALIGN_LEFT
		UINT   atlas_mode;         // e.g. VS_ATLAS_PAGE_GROUP_ONE
		UINT   behaviour;          // e.g. BEHAVIOUR_NONE
		UINT   sub_behaviour;      // e.g. SUB_BEHAVIOUR_NONE
		UINT   setup_bitmask;      // e.g. VS_ELM_SETUP_ALL
		UINT   state;              // e.g. STATE_INACTIVE, STATE_ACTIVE


	// ---- font
		std::string font_name;
		UINT   font_alignment;     // e.g. vs_system::ElmContext::ALIGN_NONE
		UINT   font_height;


	// ---- logical id's
		UINT   group_id;           // group logical id
		UINT   id;                 // logical id
		 

	// ---- indices
		INT    material_index;     // 0 >= material array index < VS_MAX_MATERIALS 
		INT    shader_index;       // 0 >= shader array index < VS_MAX_SHADERS 
		INT    view_matrix_index;  // 0 >= view matrix array index < VS_MAX_VIEW_MATRICES


	// ---- creation status 
		UINT   version;            // version #
		BOOL   report;             // set FALSE
		BOOL   runtime;            // set TRUE - at runtime
		BOOL   display;            // set TRUE - at runtime


	// ---- colour
		BOOL   use_rgba;           // set FALSE - at runtime
		FLOAT  red;
		FLOAT  green;
		FLOAT  blue;
		FLOAT  alpha;


	// ---- packed RGBA
		unsigned long d3d_color;


	// ---- null, off, on and over
		VsColour font_colour[4];


	// ---- float parameter
		UINT   float_parameter_size = VS_MAX_ELM_FLOAT_PARAMETER;
		FLOAT  float_parameter[VS_MAX_ELM_FLOAT_PARAMETER];


	// ---- long parameter
		UINT   long_parameter_size = 8;
		LONG   long_parameter[8];


	// ---- initliased in render


	// ---- gfx data
		//
		// Supplied system gfx float data such as panel viewport bounds
		// (within panel viewrect) and accessed by referenced offset,
		// e.g. UINT left == *(supplied_gfx_float_parameter+VS_PANEL_LEFT);
		//
		FLOAT *supplied_gfx_float_parameter;
		FLOAT *supplied_gfx_float_buffer;


	// ---- initliased in get data callback method


	// ---- atlas page
		UINT   atlas_page[VS_MAX_ELM_ATLAS_PAGE_REFERENCES]; // e.g. 0, 1, 2 and 3


	// ---- callback
		INT (*get_index_buffer)(ElmContext*);
		INT (*get_vertex_buffer)(ElmContext*);


	// ---- render flags
		BOOL   alpha_blending;                // alpha blending operations
		BOOL   colour_vertex;                 // requires vertex diffuse component
		BOOL   fog;                           // fog or not
		BOOL   lit;                           // lit or not
		BOOL   pick_on_radio_button;          // pick already selected radio (or channel) button
		BOOL   pick_2D;                       // pick by 2D screen space bounds
		BOOL   pick_3D;                       // pick by 3D world space pick ray
		BOOL   shaded;                        // shaded or not
		BOOL   switched_on;                   // selected on or off (true or false) 
		BOOL   switched_on_prev;              // element previously switched on?
		BOOL   solid;                         // solid OR wireframe
		BOOL   text_outline;                  // selected or not
		BOOL   text_vertical;                 // selected or not
		BOOL   texture;                       // requires valid atlas page index and texture
		BOOL   texture_mirror;                // selected on or off (true or false)
		BOOL   visible;                       // visible or hidden
		BOOL   overide;                       // overide or use config callback states and modes


	// ---- render modes
		UINT   cull_mode;


	// ---- index buffer
		UINT   index_count;                   // e.g. 4
		#if VS_INDEX_BUFFER_FORMAT == 32
		DWORD  *index_buffer;
		#else
		WORD  *index_buffer;
		#endif


	// ---- matrix
		BOOL     matrix_active;               // matrix flag initialised off
		VsMatrix matrix;                      // identity initialised 


	// ---- multi-state texturing
		BOOL   multi_active;                  // Four state atlas flag
		UINT   multi_null;                    // Non-active null index (0 to 3)
		BOOL   multi_state;                   // Four state atlas page texture (e.g. null, off, on and over)


	// ---- vertex
		FLOAT *vertex_buffer;
		UINT   vertex_count;                  // e.g. 4
		UINT   vertex_format;                 // e.g. VS_XYZRHW | VS_DIFFUSE
		UINT   vertex_size;                   // See macro VS_FORMATSIZE
		VS_PrimitiveType primitive_type;      // e.g. VS_LINESTRIP
	

	// ---- primitive
		UINT   primitive_bytes;               // vertex size x vertex count x sizeof(float)
		UINT   primitive_count;               // e.g. 4

};


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor.
\author Gareth Edwards
*/
ElmContext::ElmContext()
{

	// ---- create opaque pointer ----
		pi_elm_context = new PI_ElmContext;

	// ---- init matrix
		VsMatrixOp mop;
		mop.Identity(&pi_elm_context->matrix);

}


// ---------- Destructor ----------
/*!
\brief Destructor.
\author Gareth Edwards
*/
ElmContext::~ElmContext()
{

	// ---- delete opaque pointer ----
		if (pi_elm_context != NULL) { delete pi_elm_context; pi_elm_context = NULL; }

}


// ========== Configure ==========


// ---------- BeginConfig ----------
INT ElmContext::BeginConfig()
{
	return VS_OK;
}


// ---------- EndConfig ----------
INT ElmContext::EndConfig()
{
	return VS_OK;
}


// ========== Set ==========


// ---------- SetName ----------
INT ElmContext::SetName(std::string name)
{
	pi_elm_context->name = name;
	return VS_OK;
}


// ---------- SetText ----------
INT ElmContext::SetText(std::string text)
{
	pi_elm_context->text = text;
	return VS_OK;
}


// ---------- SetAlign ----------
INT ElmContext::SetAlign(UINT align)
{
	pi_elm_context->align = align;
	return VS_OK;
}


// ---------- SetActions ----------
INT ElmContext::SetActions(UINT actions)
{
	pi_elm_context->actions = actions;
	return VS_OK;
}


#ifndef D3DCOLOR_DEFINED
typedef DWORD D3DCOLOR;
#define D3DCOLOR_DEFINED
#endif


// ---------- SetAlpha ----------
INT ElmContext::SetAlpha(FLOAT alpha)
{

	pi_elm_context->alpha = alpha > 0 ? alpha :
		( pi_elm_context->alpha < 0 ? 0 : pi_elm_context->alpha );

	pi_elm_context->d3d_color = VS_D3DCOLOR_COLORVALUE(
			pi_elm_context->red,
			pi_elm_context->green,
			pi_elm_context->blue,
			pi_elm_context->alpha
		);

	return VS_OK;
}


// ---------- SetBehaviour ----------
INT ElmContext::SetBehaviour(UINT behaviour)
{
	pi_elm_context->behaviour = behaviour;

	if ( behaviour != BEHAVIOUR_NULL )
	{
		pi_elm_context->state = STATE_ACTIVE;
	}

	return VS_OK;
}


// ---------- SetBitmask ----------
INT ElmContext::SetBitmask(UINT bitmask)
{
	pi_elm_context->setup_bitmask = bitmask;
	return VS_OK;
}


// ---------- SetConfigured ----------
INT ElmContext::SetConfigured(BOOL config)
{
	//pi_elm_context->element_context_configured = config;
	return VS_OK;
}


// ---------- SetRGBA ----------
INT ElmContext::SetRGBA(FLOAT red, FLOAT green, FLOAT blue, FLOAT alpha)
{
	pi_elm_context->red   = red > 0 ? red   :
		( pi_elm_context->red < 0 ? 0 : pi_elm_context->red );

	pi_elm_context->green = green > 0 ? green :
		( pi_elm_context->green < 0 ? 0 : pi_elm_context->green );

	pi_elm_context->blue  = blue > 0 ? blue  :
		( pi_elm_context->blue < 0 ? 0 : pi_elm_context->blue );

	pi_elm_context->alpha = alpha > 0 ? alpha :
		( pi_elm_context->alpha < 0 ? 0 : pi_elm_context->alpha );

	pi_elm_context->d3d_color = VS_D3DCOLOR_COLORVALUE(
			pi_elm_context->red,
			pi_elm_context->green,
			pi_elm_context->blue,
			pi_elm_context->alpha
		);

	return VS_OK;
}


// ---------- SetFontName ----------
INT ElmContext::SetFontName(std::string font_name)
{
	pi_elm_context->font_name = font_name;
	return VS_OK;
}


// ---------- SetFontAlignment ----------
INT ElmContext::SetFontAlignment(UINT alignment)
{
	pi_elm_context->font_alignment = alignment;
	return VS_OK;
}


// ---------- SetFontHeight ----------
INT ElmContext::SetFontHeight(UINT height)
{
	pi_elm_context->font_height = height;
	return VS_OK;
}


// ---------- SetGroupId ----------
INT ElmContext::SetGroupId(UINT group_id)
{
	pi_elm_context->group_id = group_id;
	return VS_OK;
}


// ---------- SetId ----------
INT ElmContext::SetId(UINT id)
{
	pi_elm_context->id = id;
	return VS_OK;
}


// ---------- SetMaterialIndex ----------
INT ElmContext::SetMaterialIndex(UINT index)
{
	if ( index < VS_MAX_MATERIALS )
	{
		pi_elm_context->material_index = index;
	}
	else
	{
		return VS_ERROR;
	}
	return VS_OK;
}


// ---------- SetShaderIndex ----------
INT ElmContext::SetShaderIndex(UINT index)
{
	if ( index < VS_MAX_SHADERS )
	{
		pi_elm_context->shader_index = index;
	}
	else
	{
		return VS_ERROR;
	}
	return VS_OK;
}


// ---------- SetViewMatrix ----------
INT ElmContext::SetViewMatrix(UINT index)
{
	if ( index < VS_MAX_VIEW_MATRICES )
	{
		pi_elm_context->view_matrix_index = index;
	}
	else
	{
		return VS_ERROR;
	}
	return VS_OK;
}


// ---------- SetVersion ----------
INT ElmContext::SetVersion(UINT version)
{
	pi_elm_context->version = version;
	return VS_OK;
}


// ---------- SetReport ----------
INT ElmContext::SetReport(BOOL report)
{
	pi_elm_context->report = report;
	return VS_OK;
}


// ---------- SetRuntime ----------
INT ElmContext::SetRuntime(BOOL runtime)
{
	pi_elm_context->runtime = runtime;
	return VS_OK;
}


// ---------- SetState ----------
INT ElmContext::SetState(UINT state)
{
	pi_elm_context->state = state;
	return VS_OK;
}


// ---------- SetSubBehaviour ----------
INT ElmContext::SetSubBehaviour(UINT sub_behaviour)
{
	pi_elm_context->sub_behaviour = sub_behaviour;
	return VS_OK;
}


// ---------- SetDisplay ----------
INT ElmContext::SetDisplay(BOOL display)
{
	pi_elm_context->display = display;
	return VS_OK;
}


// ---------- SetUseRGBA ----------
INT ElmContext::SetUseRGBA(BOOL use_rgba)
{
	pi_elm_context->use_rgba = use_rgba;
	return VS_OK;
}


// ========== Set Atlas Pages ==========


// ---------- SetAtlasMode ----------
/*
INT ElmContext::SetAtlasMode(UINT multi_state_mode)
{
	switch (multi_state_mode)
	{
		case VS_ATLAS_PAGE_GROUP_ONE:
		case VS_ATLAS_PAGE_GROUP_TWO:
			pi_elm_context->atlas_mode = multi_state_mode;
			break;
		default:
			return VS_ERROR;
			break;
	}
	return VS_OK;
}
*/


// ---------- SetAtlasPage ----------
INT ElmContext::SetAtlasPages(UINT atlas_page_group)
{
	UINT atlas_base = 100;
	switch ( atlas_page_group )
	{
		case VS_ATLAS_PAGE_GROUP_ONE:
			atlas_base = 100;
			break;
		case VS_ATLAS_PAGE_GROUP_TWO:
			atlas_base = 104;
			break;
		case VS_ATLAS_PAGE_GROUP_THREE:
			atlas_base = 108;
			break;
		case VS_ATLAS_PAGE_GROUP_FOUR:
			atlas_base = 112;
			break;
		default:
			return VS_ERROR;
			break;
	}

	for (UINT i = 0; i<4; i++)
		pi_elm_context->atlas_page[i] = atlas_base + i;

	return VS_OK;
}


// ---------- SetAtlasPage ----------
INT ElmContext::SetAtlasPage(UINT index, UINT ap)
{
	if ( index < VS_MAX_ELM_ATLAS_PAGE_REFERENCES )
	{
		pi_elm_context->atlas_page[index] = ap;
		return VS_OK;
	}
	return FALSE;
}


// ========== Set Callback ==========


// ---------- SetIndexBufferCallback ----------
INT ElmContext::SetIndexBufferCallback(INT(*get_index_buffer)(ElmContext *))
{
	pi_elm_context->get_index_buffer = get_index_buffer;
	return VS_OK;
}


// ---------- SetVertexBufferCallback ----------
INT ElmContext::SetVertexBufferCallback(INT(*get_vertex_buffer)(ElmContext *))
{
	pi_elm_context->get_vertex_buffer = get_vertex_buffer;
	return VS_OK;
}


// ---------- IndexBufferCallback ----------
INT ElmContext::IndexBufferCallback(ElmContext *ec)
{
	if ( pi_elm_context->get_index_buffer != NULL )
	{
		if ( INT result=pi_elm_context->get_index_buffer(ec) != VS_OK )
			return result;
	}
	return VS_OK;
}


INT ElmContext::VertexBufferCallback(ElmContext *ec)
{
	if ( INT result=pi_elm_context->get_vertex_buffer(ec) != VS_OK ) return result;
	return VS_OK;
}


// ========== Set Parameter Data ==========


// ---------- SetFloatParameter ----------
INT ElmContext::SetFloatParameter(UINT param_count, FLOAT *param)
{
	pi_elm_context->float_parameter_size =
		param_count >= VS_MAX_ELM_FLOAT_PARAMETER ?
			VS_MAX_ELM_FLOAT_PARAMETER :
				param_count;
	for (UINT i=0; i<pi_elm_context->float_parameter_size; i++)
		pi_elm_context->float_parameter[i] = param[i];
	return VS_OK;
}


// ---------- SetGfxFloatParameter ----------
INT ElmContext::SetGfxFloatParameter(FLOAT *sd)
{
	pi_elm_context->supplied_gfx_float_parameter = sd;
	return VS_OK;
}


// ---------- SetGfxFloatBuffer----------
INT ElmContext::SetGfxFloatBuffer(FLOAT *sfb)
{
	pi_elm_context->supplied_gfx_float_buffer = sfb;
	return VS_OK;
}


// ---------- SetLongParameterSize ----------
INT ElmContext::SetLongParameterSize(UINT size)
{
	//pi_elm_context->long_parameter_size = size;
	//pi_elm_context->long_parameter = new LONG(sizeof(LONG)*size);
	return VS_OK;
}


// ---------- SetLongParameter ----------
INT ElmContext::SetLongParameter(LONG *p)
{
	//pi_elm_context->long_parameter = p;
	return VS_OK;
}


// ========== Set Render Flags ==========


// ---------- SetAlphaBlending ----------
INT ElmContext::SetAlphaBlending(BOOL ab)
{
	pi_elm_context->alpha_blending = ab;
	return VS_OK;
}


// ---------- SetColourVertex ----------
INT ElmContext::SetColourVertex(BOOL cv)
{
	pi_elm_context->colour_vertex = cv;
	return VS_OK;
}


// ---------- SetFog ----------
INT ElmContext::SetFog(BOOL f)
{
	pi_elm_context->fog = f;
	return VS_OK;
}


// ---------- SetLit ----------
INT ElmContext::SetLit(BOOL l)
{
	pi_elm_context->lit = l;
	return VS_OK;
}


// ---------- SetPick2D ----------
INT ElmContext::SetPick2D(BOOL p)
{
	pi_elm_context->pick_2D = p;
	return VS_OK;
}


// ---------- SetPick3D ----------
INT ElmContext::SetPick3D(BOOL p)
{
	pi_elm_context->pick_3D = p;
	return VS_OK;
}


// ---------- SetPickOnRadioButton ----------
INT ElmContext::SetPickOnRadioButton(BOOL p)
{
	pi_elm_context->pick_on_radio_button = p;
	return VS_OK;
}


// ---------- SetShaded ----------
INT ElmContext::SetShaded(BOOL s)
{
	pi_elm_context->shaded = s;
	return VS_OK;
}


// ---------- SetOveride ----------
INT ElmContext::SetOveride(BOOL ov)
{
	pi_elm_context->overide = ov;
	return VS_OK;
}


// ---------- SetSwitchedOn ----------
INT ElmContext::SetSwitchedOn(BOOL s)
{
	pi_elm_context->switched_on = s;

	if (pi_elm_context->group_id == 250)
	{
		if (pi_elm_context->id == 2)
		{
			float fred = 2;
		}
	}

	return VS_OK;
}


// ---------- SetSwitchedOnPreviously ----------
INT ElmContext::SetSwitchedOnPreviously(BOOL s)
{
	pi_elm_context->switched_on_prev = s;
	return VS_OK;
}


// ---------- SetSolid ----------
INT ElmContext::SetSolid(BOOL s)
{
	pi_elm_context->solid = s;
	return VS_OK;
}


// ---------- SetTextOutline ----------
INT ElmContext::SetTextOutline(BOOL to)
{
	pi_elm_context->text_outline = to;
	return VS_OK;
}


// ---------- SetTextVertical ----------
INT ElmContext::SetTextVertical(BOOL tv)
{
	pi_elm_context->text_vertical = tv;
	return VS_OK;
}


// ---------- SetTextured ----------
INT ElmContext::SetTextured(BOOL t)
{
	pi_elm_context->texture = t;
	return VS_OK;
}


// ---------- SetTextureMirror ----------
INT ElmContext::SetTextureMirror(BOOL w)
{
	pi_elm_context->texture_mirror = w;
	return VS_OK;
}


// ---------- SetVisible ----------
INT ElmContext::SetVisible(BOOL v)
{
	pi_elm_context->visible = v;
	return VS_OK;
}


// ========== Set Render Modes ==========


// ---------- SetCullMode ----------
INT ElmContext::SetCullMode(UINT cm)
{
	switch ( cm )
	{
		case CULL_NONE:
		case CULL_CW:
		case CULL_CCW:
			pi_elm_context->cull_mode = cm;
			break;
		default:
			return VS_ERROR;
	}
	return VS_OK;
}


// ========== Set Index Buffer ==========


// ---------- SetIndexBufferPtr ----------
#if VS_INDEX_BUFFER_FORMAT == 32
INT ElmContext::SetIndexBufferPtr(DWORD *ib)
#else
INT ElmContext::SetIndexBufferPtr(WORD *ib)
#endif
{
	pi_elm_context->index_buffer = ib;
	return VS_OK;
}


// ---------- SetIndexCount ----------
INT ElmContext::SetIndexCount(UINT ic)
{
	pi_elm_context->index_count = ic;
	return VS_OK;
}


// ========== Set Matrix stuff ==========


// ---------- SetUseMatrix ----------
INT ElmContext::SetUseMatrix(BOOL active)
{
	pi_elm_context->matrix_active = active;
	return VS_OK;
}


// ---------- SetMatrix ----------
INT ElmContext::SetMatrix(VsMatrix *vsm)
{
	VsMatrixOp mop;
	mop.Copy(&pi_elm_context->matrix, vsm);
	return VS_OK;
}


// ========== Set Multi-State texturing ==========


// ---------- SetMultiActive ----------
INT ElmContext::SetMultiActive(BOOL ma, UINT mn)
{
	pi_elm_context->multi_null = mn;
	pi_elm_context->multi_active = ma;
	return VS_OK;
}


// ---------- SetMultiState ----------
INT ElmContext::SetMultiState(BOOL ms)
{
	pi_elm_context->multi_state = ms;
	return VS_OK;
}



// ========== Set Vertex Buffer ==========


// ---------- SetVertexBufferPtr ----------
INT ElmContext::SetVertexBufferPtr(FLOAT *vertex_buffer)
{
	pi_elm_context->vertex_buffer = vertex_buffer;
	return VS_OK;
}


// ---------- SetVertexCount ----------
INT ElmContext::SetVertexCount(UINT vc)
{
	pi_elm_context->vertex_count = vc;
	return VS_OK;
}


// ---------- SetVertexFormat ----------
INT ElmContext::SetVertexFormat(UINT vf)
{
	pi_elm_context->vertex_format = vf;
	return VS_OK;
}


// ---------- SetVertexSize ----------
INT ElmContext::SetVertexSize(UINT vf)
{
	pi_elm_context->vertex_size =
	( (VS_VertexFormat)vf & VS_XYZ      ? 3 : 0) +
	( (VS_VertexFormat)vf & VS_XYZRHW   ? 4 : 0) +
	( (VS_VertexFormat)vf & VS_NORMAL   ? 3 : 0) +
	( (VS_VertexFormat)vf & VS_DIFFUSE  ? 1 : 0) +
	( (VS_VertexFormat)vf & VS_SPECULAR ? 1 : 0) +
	( (VS_VertexFormat)vf & VS_TEX1     ? 2 : 0);
	return VS_OK;
}


// ========== Set Primitive Parameters ==========


// ---------- SetPrimitiveBytes ----------
INT ElmContext::SetPrimitiveBytes(UINT pb)
{
	pi_elm_context->primitive_bytes = pb;
	return VS_OK;
}


// ---------- SetPrimitiveCount ----------
INT ElmContext::SetPrimitiveCount(UINT pc)
{
	pi_elm_context->primitive_count = pc;
	return VS_OK;
}


// ---------- SetPrimitiveType ----------
INT ElmContext::SetPrimitiveType(VS_PrimitiveType pt)
{
	pi_elm_context->primitive_type = pt;
	return VS_OK;
}


// ========== Get ==========


// ---------- GetName ----------
std::string ElmContext::GetName()
{
	return pi_elm_context->name;
}


// ---------- GetText ----------
std::string ElmContext::GetText()
{
	return pi_elm_context->text;
}


// ---------- GetAlign ----------
UINT ElmContext::GetAlign()
{
	return pi_elm_context->align;
}


// ---------- GetActions ----------
UINT ElmContext::GetActions()
{
	return pi_elm_context->actions;
}


// ---------- GetAtlasMode ----------
UINT ElmContext::GetAtlasMode()
{
	return pi_elm_context->atlas_mode;
}


// ---------- GetBehaviour ----------
UINT ElmContext::GetBehaviour()
{
	return pi_elm_context->behaviour;
}


// ---------- GetSetupBitmask ----------
UINT ElmContext::GetSetupBitmask()
{
	return pi_elm_context->setup_bitmask;
}


// ---------- GetRGBA ----------
INT ElmContext::GetRGBA(FLOAT *red, FLOAT *green, FLOAT *blue, FLOAT *alpha)
{
	*red   = pi_elm_context->red;
	*green = pi_elm_context->green;
	*blue  = pi_elm_context->blue;
	*alpha = pi_elm_context->alpha;
	return VS_OK;
}


// ---------- GetRGBA ----------
DWORD ElmContext::GetRGBA()
{
	return pi_elm_context->d3d_color;
}


// ---------- GetFontName ----------
std::string ElmContext::GetFontName()
{
	return pi_elm_context->font_name;
}


// ---------- GetFontAlignment ----------
UINT ElmContext::GetFontAlignment()
{
	return pi_elm_context->font_alignment;
}


// ---------- GetFontHeight ----------
UINT ElmContext::GetFontHeight()
{
	return pi_elm_context->font_height;
}


// ---------- GetGroupId ----------
UINT ElmContext::GetGroupId()
{
	return pi_elm_context->group_id;
}


// ---------- GetId ----------
UINT ElmContext::GetId()
{
	return pi_elm_context->id;
}


// ---------- GetMaterialIndex ----------
UINT ElmContext::GetMaterialIndex()
{
	return pi_elm_context->material_index;
}


// ---------- GetShaderIndex ----------
UINT ElmContext::GetShaderIndex()
{
	return pi_elm_context->shader_index;
}


// ---------- GetViewMatrixIndex ----------
UINT ElmContext::GetViewMatrixIndex()
{
	return pi_elm_context->view_matrix_index;
}


// ---------- GetVersion ----------
UINT ElmContext::GetVersion()
{
	return pi_elm_context->version;
}


// ---------- GetReport ----------
BOOL ElmContext::GetReport()
{
	return pi_elm_context->report;
}


// ---------- GetRuntime ----------
BOOL ElmContext::GetRuntime()
{
	return pi_elm_context->runtime;
}


// ---------- GetState ----------
UINT ElmContext::GetState()
{
	return pi_elm_context->state;
}


// ---------- GetSubBehaviour ----------
UINT ElmContext::GetSubBehaviour()
{
	return pi_elm_context->sub_behaviour;
}


// ---------- GetDisplay ----------
BOOL ElmContext::GetDisplay()
{
	return pi_elm_context->display;
}


// ---------- GetUseRGBA ----------
BOOL ElmContext::GetUseRGBA()
{
	return pi_elm_context->use_rgba;
}


// ========== Get Atlas Pages ==========


// ---------- GetAtlasPages ----------
INT ElmContext::GetAtlasPages(UINT *ap)
{
	for ( UINT i=0; i<VS_MAX_ELM_ATLAS_PAGE_REFERENCES; i++ )
		*(ap+i) = pi_elm_context->atlas_page[i];
	return VS_OK;
}


// ---------- GetAtlasPage ----------
INT ElmContext::GetAtlasPage(UINT index, UINT *ap)
{
	if ( index < VS_MAX_ELM_ATLAS_PAGE_REFERENCES )
	{
		*ap = pi_elm_context->atlas_page[index];
		return VS_OK;
	}
	return FALSE;
}


// ========== Get Data ==========


// ---------- GetFloatParameterSize ----------
UINT ElmContext::GetFloatParameterSize()
{
	return pi_elm_context->float_parameter_size;
}


// ---------- GetFloatParameter ----------
FLOAT * ElmContext::GetFloatParameter()
{
	return pi_elm_context->float_parameter;
}


// ---------- GetGfxFloatParameter ----------
FLOAT * ElmContext::GetGfxFloatParameter()
{
	return pi_elm_context->supplied_gfx_float_parameter;
}


// ---------- GetGfxFloatBuffer----------
FLOAT * ElmContext::GetGfxFloatBuffer()
{
	return pi_elm_context->supplied_gfx_float_buffer;
}


// ---------- GetLongParameterSize----------
UINT ElmContext::GetLongParameterSize()
{
	return pi_elm_context->long_parameter_size;
}


// ---------- GetLongParameterSize----------
LONG * ElmContext::GetLongParameter()
{
	return pi_elm_context->long_parameter;
}


// ========== Get Render Flags ==========


// ---------- GetAlphaBlending ----------
BOOL ElmContext::GetAlphaBlending()
{
	return pi_elm_context->alpha_blending;
}


// ---------- GetOveride ----------
BOOL ElmContext::GetOveride()
{
	return pi_elm_context->overide;
}


// ---------- GetColourVertex ----------
BOOL ElmContext::GetColourVertex()
{
	return pi_elm_context->colour_vertex;
}


// ---------- GetFog ----------
BOOL ElmContext::GetFog()
{
	return pi_elm_context->fog;
}


// ---------- GetLit ----------
BOOL ElmContext::GetLit()
{
	return pi_elm_context->lit;
}


// ---------- GetPick2D ----------
BOOL ElmContext::GetPick2D()
{
	return pi_elm_context->pick_2D;
}


// ---------- GetPick3D ----------
BOOL ElmContext::GetPick3D()
{
	return pi_elm_context->pick_3D;
}


// ---------- GetPickOnRadioButton ----------
BOOL ElmContext::GetPickOnRadioButton()
{
	return pi_elm_context->pick_on_radio_button;
}


// ---------- GetShaded ----------
BOOL ElmContext::GetShaded()
{
	return pi_elm_context->shaded;
}


// ---------- GetSwitchedOn ----------
BOOL ElmContext::GetSwitchedOn()
{
	return pi_elm_context->switched_on;
}


// ---------- GetSwitchedOnPreviously ----------
BOOL ElmContext::GetSwitchedOnPreviously()
{
	return pi_elm_context->switched_on_prev;
}


// ---------- GetSolid ----------
BOOL ElmContext::GetSolid()
{
	return pi_elm_context->solid;
}


// ---------- GetTextOutline ----------
BOOL ElmContext::GetTextOutline()
{
	return pi_elm_context->text_outline;
}


// ---------- GetTextVertical ----------
INT ElmContext::GetTextVertical()
{
	return pi_elm_context->text_vertical;
}


// ---------- GetTextured ----------
BOOL ElmContext::GetTextured()
{
	return pi_elm_context->texture;
}


// ---------- GetTextureMirror ----------
BOOL ElmContext::GetTextureMirror()
{
	return pi_elm_context->texture_mirror;
}


// ---------- GetVisible ----------
BOOL ElmContext::GetVisible()
{
	return pi_elm_context->visible;
}


// ========== Get Render Modes ==========


// ---------- GetCullMode ----------
UINT ElmContext::GetCullMode()
{
	return pi_elm_context->cull_mode;
}


// ========== Get Index Buffer ==========


// ---------- GetIndexBufferPtr ----------
#if VS_INDEX_BUFFER_FORMAT == 32
DWORD * ElmContext::GetIndexBufferPtr()
#else
WORD * ElmContext::GetIndexBufferPtr()
#endif
{
	return pi_elm_context->index_buffer;
}


// ---------- GetIndexCount ----------
UINT ElmContext::GetIndexCount()
{
	return pi_elm_context->index_count;
}


// ========== Get Matrix stuff ==========


// ---------- GetUseMatrix ----------
INT ElmContext::GetUseMatrix(BOOL *active)
{
	*active = pi_elm_context->matrix_active;
	return VS_OK;
}


// ---------- GetMatrix ----------
INT ElmContext::GetMatrix(VsMatrix *vsm)
{
	VsMatrixOp mop;
	mop.Copy(vsm, &pi_elm_context->matrix);
	return VS_OK;
}


// ========== Set Multi-State texturing ==========


// ---------- GetMultiActive ----------
BOOL ElmContext::GetMultiActive()
{
	return pi_elm_context->multi_active;
}


// ---------- GetMultiNull ----------
UINT ElmContext::GetMultiNull()
{
	return pi_elm_context->multi_null;
}


// ---------- GetMulti_State ----------
BOOL ElmContext::GetMultiState()
{
	return pi_elm_context->multi_state;
}


// ========== Get Vertex Buffer ==========


// ---------- GetVertexBufferPtr ----------
FLOAT * ElmContext::GetVertexBufferPtr()
{
	return pi_elm_context->vertex_buffer;
}


// ---------- GetVertexSize ----------
UINT ElmContext::GetVertexFormat()
{
	return pi_elm_context->vertex_format;
}


// ---------- GetVertexCount ----------
UINT ElmContext::GetVertexCount()
{
	return pi_elm_context->vertex_count;
}


// ---------- GetVertexSize ----------
UINT ElmContext::GetVertexSize()
{
	return pi_elm_context->vertex_size;
}


// ========== Get Primitive Parameters ==========


// ---------- GetPrimitiveBytes ----------
UINT ElmContext::GetPrimitiveBytes()
{
	return pi_elm_context->primitive_bytes;
}


// ---------- GetPrimitiveCount ----------
UINT ElmContext::GetPrimitiveCount()
{
	return pi_elm_context->primitive_count;
}


// ---------- GetPrimitiveType ----------
VS_PrimitiveType ElmContext::GetPrimitiveType()
{
	return pi_elm_context->primitive_type;
}


////////////////////////////////////////////////////////////////////////////////
