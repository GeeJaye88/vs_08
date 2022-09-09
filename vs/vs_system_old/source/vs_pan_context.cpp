////////////////////////////////////////////////////////////////////////////////

// ---------- vs_pan_context.cpp ----------
/*!
\file vs_pan_context.cpp
\brief Implementation of the Gfx(/raphics)Context class
\author Gareth Edwards 
*/

// ---- include d3d9 ----
#include <d3d9.h>
//#include <d3dx9.h>


// ---- include ----
#include "../header/vs_pan_context.h"


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Param ----------

class Param
{

public:

	// ---- constructor && destructor
		Param() {
			for ( UINT i=0; i<5; i++ )
			{
				v[i] = d[i] = 0;
			}
		}
		~Param() { }


	// ---- default
		VOID Default(UINT index)
		{
			v[index] = d[index];
		}
		VOID Default()
		{
			for ( UINT i=0; i<5; i++ )
			{
				v[i] = d[i];
			}
		}


	// ---- get
		FLOAT Get(UINT index)
		{
			return v[index];
		}
		FLOAT * Get()
		{
			return &v[0];
		}


	// ---- init
		VOID Init(UINT index, FLOAT value)
		{
			d[index] = v[index] = value;
		}
		VOID Init(FLOAT v0, FLOAT v1, FLOAT v2)
		{
			d[0] = v[0] = v0;
			d[1] = v[1] = v1;
			d[2] = v[2] = v2;
		}
		VOID Init(FLOAT v0, FLOAT v1, FLOAT v2, FLOAT v3, FLOAT v4)
		{
			d[0] = v[0] = v0;
			d[1] = v[1] = v1;
			d[2] = v[2] = v2;
			d[3] = v[3] = v3;
			d[4] = v[4] = v4;
		}


	// ---- set
		VOID Set(UINT index, FLOAT value)
		{
			v[index] = value;
		}
		VOID Set(FLOAT v0, FLOAT v1, FLOAT v2)
		{
			v[0] = v0;
			v[1] = v1;
			v[2] = v2;
		}
		VOID Set(FLOAT v0, FLOAT v1, FLOAT v2, FLOAT v3, FLOAT v4)
		{
			v[0] = v0;
			v[1] = v1;
			v[2] = v2;
			v[3] = v3;
			v[4] = v4;
		}


	// ---- value and default
		FLOAT v[5];
		FLOAT d[5];

};


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Props class ----------

class PanelContext::PI_PanelContext
{
public:

	// ---- constructor
		PI_PanelContext()
		{

			//---- panel
				panel_name[0] = 0;


			//---- display lists
				panel_display_list_top = 0;
				panel_display_list_index = 0;
				for (UINT i = 0; i < PANEL_DISPLAY_LIST_MAX; i++)
				{
					panel_display_list_names[i][0] = 0;
					panel_display_list_handle[i]   = 0;
					panel_display_list_state[i]    = 0;
				}


			// ---- misc
				panel_group_id       = 0;
				panel_id             = 0;
				panel_handle         = 0;
				panel_background     = TRUE;
				panel_outline        = FALSE;
				panel_selected       = FALSE;
				panel_echo_over      = FALSE;
				panel_display_status = TRUE;
				panel_over_status    = TRUE;
				panel_version        = 1;


			// ---- colour
				use_background_colour  = TRUE;
				background_colour      = VS_D3DCOLOR_COLORVALUE(0.1f, 0.2f, 0.4f, 1);
				background_colour_over = VS_D3DCOLOR_COLORVALUE(0.2f, 0.4f, 0.8f, 1);


			// ---- layout
				VsFloatRectangle view_default = { 0, 0, 1, 1 };
				VsFloatRectangle box_default  = { 0, 0, 0, 0 };
				VsFloatRectangle box_over     = { 2, 2, 2, 2 };


			// ---- box
				margin      = box_default;
				border      = box_default;
				border_over = box_over;


			// ---- view
				viewrect = view_default;
				viewport = view_default;


			// ---- type
				strcpy (type, "Perspective");
				type_id = VS_PERSPECTIVE;


			// ---- perspective
				perspective.Init(40, 0.001f, 1000);


			// ---- orthogonal
				orthogonal.Init(0, 0, 1, -100, 100);


			// ---- view
				view[0] = 'X';
				view[1] = 'Y';
				view[2] = 'Z';
				rotate.Init(0, 0, 0);
				translate.Init(0, 0, 0);


			// ---- render to image
				render_to_image_flag   = FALSE;
				render_to_image_begin  = 0;
				render_to_image_end    = 0;
				render_to_image_id     = 0;
				render_to_image_status = FALSE;
				render_to_image_width  = 800;
				render_to_image_height = 600;

		};


	// ---- destructor
		~PI_PanelContext() { };


	// ---- get enumerated projection type
		UINT GetTypeId(CHAR *); 


	// ---- get D3D_COLOR TBD
		INT GetD3dColor(VNode *, D3DCOLOR *);


	// ---- panel
		CHAR panel_name[VS_MAXCHARLEN];


	// ---- displayLists
		UINT panel_display_list_top    = 0;
		UINT panel_display_list_active = 0;
		UINT panel_display_list_index  = 0;
		CHAR panel_display_list_names[PANEL_DISPLAY_LIST_MAX][VS_MAXCHARNAME];
		INT  panel_display_list_handle[PANEL_DISPLAY_LIST_MAX];
		UINT panel_display_list_state[PANEL_DISPLAY_LIST_MAX];


	// ---- misc
		UINT panel_group_id;
		UINT panel_id;
		UINT panel_handle;      // Panel * cast to unsigned integer
		BOOL panel_background;  // If set TRUE then outline set FALSE
		BOOL panel_outline;     // If set TRUE then background set FALSE
		BOOL panel_selected;
		BOOL panel_echo_over;
		BOOL panel_display_status;
		BOOL panel_over_status;
		UINT panel_version;


	// ---- colour
		BOOL use_background_colour;
		D3DCOLOR background_colour;
		D3DCOLOR background_colour_over;


	// ----layout
		// note: location and dimensions of viewport
		//    (i.e. Frame->Panel->Margin->Viewrect->Border->Viewport)
		VsFloatRectangle margin;
		VsFloatRectangle viewrect;
		VsFloatRectangle border;
		VsFloatRectangle border_over;
		VsFloatRectangle viewport;


	// ---- projection

		// ---- type of projection (e.g. "Front")
			CHAR type[16];

		// ---- id of projection (e.g. FRONT)
			UINT type_id;

		// ---- perspective FOV, near and far clipping
			Param perspective;

		// ---- orthoganol X and Y offset, Scale, and Z minimum and maximum viewing volume
			Param orthogonal;


	// ---- view

		// ---- view (e.g. "XYZ")
			CHAR view[4];

		// ---- XYZ rotation
			Param rotate;

		// ---- XYZ translation
			Param translate;


	// ---- render to image
		BOOL render_to_image_flag;
		BOOL render_to_image_begin;
		BOOL render_to_image_end;
		UINT render_to_image_id;
		BOOL render_to_image_status;
		UINT render_to_image_width;
		UINT render_to_image_height;

};


// ----------GetTypeId ----------
UINT PanelContext::PI_PanelContext::GetTypeId(CHAR *t)
{
	if (t != NULL)
	{
		if (!strcmp(t, "Perspective"))
			return VS_PERSPECTIVE;
		else if (!strcmp(t, "Front"))
			return VS_ORTHO_FRONT;
		else if (!strcmp(t, "Back"))
			return VS_ORTHO_BACK;
		else if (!strcmp(t, "Top"))
			return VS_ORTHO_TOP;
		else if (!strcmp(t, "Bottom"))
			return VS_ORTHO_BOTTOM;
		else if (!strcmp(t, "Left"))
			return VS_ORTHO_LEFT;
		else if (!strcmp(t, "Right"))
			return VS_ORTHO_RIGHT;

	}
	return VS_ERROR;
}

// ----------GetD3dColor ----------
INT PanelContext::PI_PanelContext::GetD3dColor(VNode *parent_vnode, D3DCOLOR *d3d_color)
{

	// ---- check and return error ----
	if ( parent_vnode == NULL ) return VS_ERROR;
	if ( d3d_color == NULL ) return VS_ERROR;


	// ---- local ----
	CHAR * n[4] = { "Red", "Green", "Blue", "Alpha" };
	FLOAT c[4] = { 0, 0, 0, 1 };
	BOOL use_color = FALSE;
	

	// ---- initialise D3DCOLOR
	*d3d_color = 0;


	// ---- build RGBA ----
	for (INT i = 0; i < 4; i++)
	{
		VNode* vnode = parent_vnode->SearchName(n[i]);
		if ( vnode != NULL )
			c[i] = vnode->GetFloatFromValue();
	}

	// ---- set D3DCOLOR ----
	*d3d_color = VS_D3DCOLOR_COLORVALUE(
			c[0],
			c[1],
			c[2],
			c[3]
		);

	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////


// ----------DefaultPerspective ----------
/*!
\brief (Re)set all default perspective parameters
\author Gareth Edwards
\return VOID
\note Default values are set at runtime either via config file or by application
*/
VOID PanelContext::DefaultPerspective()
{
	pi_panel_context->perspective.Default();
}


// ----------DefaultPerspective ----------
/*!
\brief (Re)set a default perspective parameter
\author Gareth Edwards
\param UINT - [in] parameter index
\return VOID
\note Default values are set at runtime either via config file or by application
\note
Index == 0: Vertical FOV
         1: Near plane
		 2: Far plane
*/
VOID PanelContext::DefaultPerspective(UINT index)
{
	pi_panel_context->perspective.Default(index);
}


// ----------DefaultOrthogonal ----------
/*!
\brief (Re)set all default orthogonal parameters
\author Gareth Edwards
\param UINT - [in] property id
\return VOID
\note Default values are set at runtime either via config file or by application
*/
VOID PanelContext::DefaultOrthogonal()
{
	pi_panel_context->orthogonal.Default();
}


// ----------DefaultOrthogonal ----------
/*!
\brief (Re)set a default orthogonal parameter
\author Gareth Edwards
\param UINT - [in] parameter index
\return VOID
\note Default values are set at runtime either via config file or by application
\note
Index == 0: enumerated projection value, e.g. VS_ORTHO_FRONT, VS_ORTHO_BACK, etc.
         1: Left / Top
		 2: Right / Bottom
		 3: -Z
		 4: +Z
*/
VOID PanelContext::DefaultOrthogonal(UINT index)
{
	pi_panel_context->orthogonal.Default(index);
}


// ----------DefaultRotate ----------
/*!
\brief (Re)set default rotation parameters
\author Gareth Edwards
\return VOID
\note Default values are set at runtime either via config file or by application
*/
VOID PanelContext::DefaultRotate()
{
	pi_panel_context->rotate.Default();
}


// ----------DefaultTranslate ----------
/*!
\brief (Re)set default translation parameters
\author Gareth Edwards
\return VOID
\note Default values are set at runtime either via config file or by application
*/
VOID PanelContext::DefaultTranslate()
{
	pi_panel_context->translate.Default();
}


// ----------GetTypeId ----------
/*!
\brief Get type of projection
\author Gareth Edwards
\return INT - enumerated projection value
\note
Value = VS_PERSPECTIVE
        VS_ORTHO_FRONT
		VS_ORTHO_BACK
		VS_ORTHO_TOP
		VS_ORTHO_BOTTOM
		VS_ORTHO_LEFT
		VS_ORTHO_RIGHT
*/
INT PanelContext::GetTypeId() 
{
	return pi_panel_context->type_id;
}


// ---------- GetMargin ----------
/*!
\brief Get margin widths
\author Gareth Edwards
\return * VsFloatRectangle - storing left, top, right, bottom 
\note See definition of VsFloatRectangle
*/
VsFloatRectangle *PanelContext::GetMargin() 
{
	return &pi_panel_context->margin;
}


// ---------- GetViewrect ----------
/*!
\brief Get viewport rectangle
\author Gareth Edwards
\return * VsFloatRectangle - storing left, top, right, bottom
\note See definition of VsFloatRectangle
*/
VsFloatRectangle *PanelContext::GetViewrect() 
{
	return &pi_panel_context->viewrect;
}


// ---------- GetBorder ----------
/*!
\brief Get border rectangle
\author Gareth Edwards
\return * VsFloatRectangle - storing left, top, right, bottom
\note See definition of VsFloatRectangle
*/
VsFloatRectangle *PanelContext::GetBorder() 
{
	return &pi_panel_context->border;
}


// ---------- GetBorderOver ----------
/*!
\brief Get border over rectangle
\author Gareth Edwards
\return * VsFloatRectangle - storing left, top, right, bottom
\note See definition of VsFloatRectangle
*/
VsFloatRectangle *PanelContext::GetBorderOver()
{
	return &pi_panel_context->border_over;
}


// ---------- GetViewport ----------
/*!
\brief Get viewport
\author Gareth Edwards
\return * VsFloatRectangle - storing left, top, right, bottom
\note See definition of VsFloatRectangle
*/
VsFloatRectangle *PanelContext::GetViewport() 
{
	return &pi_panel_context->viewport;
}


// ---------- GetPerspective ----------
/*!
\brief Get perspective
\author Gareth Edwards
\return * FLOAT - pointer to array storing FOV, near plane, and fAR plane values
*/
FLOAT *PanelContext::GetPerspective() 
{
	return &pi_panel_context->perspective.v[0];
}


// ---------- GetOrthoganal ----------
/*!
\brief Get orthogonal
\author Gareth Edwards
\return * FLOAT - pointer to array storing projection, etc.
*/
FLOAT *PanelContext::GetOrthogonal() 
{
	return &pi_panel_context->orthogonal.v[0];
}


// ---------- GetZNear ----------
/*!
\brief Get Z Near value
\author Gareth Edwards
\return * FLOAT - pointer to Z near value
*/
FLOAT *PanelContext::GetZNear() 
{
	return &pi_panel_context->orthogonal.v[0];
}


// ---------- GetZFar ----------
/*!
\brief Get Z Far value
\author Gareth Edwards
\return * FLOAT - pointer to Z far value
*/
FLOAT *PanelContext::GetZFar() 
{
	return &pi_panel_context->orthogonal.v[0];
}


// ---------- GetView ----------
/*!
\brief Get View name
\author Gareth Edwards
\return * FLOAT - pointer to CHAR array storing name of current view
*/
CHAR *PanelContext::GetView() 
{
	return pi_panel_context->view;
}


// ---------- GetRotate ----------
/*!
\brief Get Rotation
\author Gareth Edwards
\return * FLOAT - pointer to array storing X, Y and Z rotation
*/
FLOAT *PanelContext::GetRotate() 
{
	return &pi_panel_context->rotate.v[0];
}


// ---------- GetTranslate ----------
/*!
\brief Get Translation
\author Gareth Edwards
\return * FLOAT - pointer to array storing X, Y and Z translation
*/
FLOAT *PanelContext::GetTranslate() 
{
	return &pi_panel_context->translate.v[0];
}


// ---------- GetName ----------
/*!
\brief Get context Name
\author Gareth Edwards
\param CHAR * - [in/out] CHAR array to store name
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::GetName(CHAR* name) 
{
	strcpy(name, pi_panel_context->panel_name);
	return VS_OK;
}


// ---------- GetDisplay ----------
/*!
\brief get display list name
\author Gareth Edwards
\param CHAR * - [in/out] CHAR array to store name
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::GetDisplayListName(UINT index, CHAR *display_list)
{
	if ( pi_panel_context->panel_display_list_names[index][0] != 0 )
	{
		strcpy(display_list, pi_panel_context->panel_display_list_names[index]);
		return VS_OK;
	}
	return VS_ERROR;
}


// ---------- GetDisplayListHandle ----------
/*!
\brief get display list handle
\author Gareth Edwards
\param INT * - [in/out] handle
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::GetDisplayListHandle(UINT index, INT *id)
{

	if ( index < PANEL_DISPLAY_LIST_MAX )
	{
		*id = pi_panel_context->panel_display_list_handle[index];
		return VS_OK;
	}
	*id = NULL;

	return VS_ERROR;
}


// ---------- GetDisplayListIndex ----------
/*!
\brief get display list index
\author Gareth Edwards
\param INT * - [in/out] handle
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::GetDisplayListIndex(UINT *index)
{
	*index = pi_panel_context->panel_display_list_index;
	return VS_ERROR;
}


// ---------- GetDisplayListTop ----------
/*!
\brief get display list top
\author Gareth Edwards
\param INT * - [in/out] handle
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::GetDisplayListTop(UINT *top)
{
	*top = pi_panel_context->panel_display_list_top;
	return VS_ERROR;
}

// ---------- GetGroupId ----------
/*!
\brief Get context Group Id
\author Gareth Edwards
\param UINT * - [in/out] group id
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::GetGroupId(UINT *gid)
{
	*gid = pi_panel_context->panel_group_id;
	return VS_OK;
}


// ---------- GetHandle ----------
/*!
\brief Get handle ( pointer to panel cast To unsigned integer )
\author Gareth Edwards
\return UINT - handle
*/
UINT PanelContext::GetHandle()
{
	return pi_panel_context->panel_handle;
}


// ---------- GetId ----------
/*!
\brief Get context Id
\author Gareth Edwards
\param UINT * - [in/out] id
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::GetId(UINT *id)
{
	*id = pi_panel_context->panel_id;
	return VS_OK;
}


// ---------- GetVersion ----------
/*!
\brief Get context version
\author Gareth Edwards
\param UINT * - [out] id
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::GetVersion(UINT *version)
{
	*version = pi_panel_context->panel_version;
	return VS_OK;
}


// ---------- GetBackground ----------
/*!
\brief Get status of Panel background
\author Gareth Edwards
\return BOOL - status
*/
BOOL PanelContext::GetBackground()
{
	return pi_panel_context->panel_background;
}


// ---------- GetOutline ----------
/*!
\brief Get status of Panel outline
\author Gareth Edwards
\return BOOL - status
*/
BOOL PanelContext::GetOutline()
{
	return pi_panel_context->panel_outline;
}


// ---------- GetEchoOver ----------
/*!
\brief Get status of Panel echo over
\author Gareth Edwards
\return BOOL - status
*/
BOOL PanelContext::GetEchoOver()
{
	return pi_panel_context->panel_echo_over;
}


// ---------- GetUseViewportColor ----------
/*!
\brief Get status of Panel Background Color
\author Gareth Edwards
\return BOOL - status
*/
BOOL PanelContext::GetUseViewportColor()
{
	return pi_panel_context->use_background_colour;
}


// ---------- GetViewportD3dColor ----------
/*!
\brief Get Panel Background D3D format Color
\author Gareth Edwards
\return DWORD - status
*/
DWORD PanelContext::GetViewportD3dColor()
{
	return pi_panel_context->background_colour;
}


// ---------- GetViewportOverD3dColor ----------
/*!
\brief Get Panel Background Over D3D format Color
\author Gareth Edwards
\return DWORD - status
*/
DWORD PanelContext::GetViewportOverD3dColor()
{
	return pi_panel_context->background_colour_over;
}


// ---------- GetSelected ----------
/*!
\brief Get selected status
\author Gareth Edwards
\return BOOL * - selected status
*/
INT PanelContext::GetSelected(BOOL *selected)
{
	*selected = pi_panel_context->panel_selected;
	return VS_OK;
}


// ---------- GetDisplayStatus ----------
/*!
\brief Get display status
\author Gareth Edwards
\return BOOL * - selected status
*/
INT PanelContext::GetDisplayStatus(BOOL *status)
{
	*status = pi_panel_context->panel_display_status;
	return VS_OK;
}


// ---------- GetInteractionStatus ----------
/*!
\brief Get user interaction status
\author Gareth Edwards
\return BOOL * - selected status
*/
INT PanelContext::GetInteractionStatus(BOOL *status)
{
	*status = pi_panel_context->panel_over_status;
	return VS_OK;
}


// ---------- SetDisplayListHandle ----------
/*!
\brief Set Display list handle
\author Gareth Edwards
\param INT - [in] handle
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::SetDisplayListHandle(UINT index, INT handle)
{

	if ( index < PANEL_DISPLAY_LIST_MAX )
	{
		pi_panel_context->panel_display_list_handle[index] = handle;
		return VS_OK;
	}
	pi_panel_context->panel_display_list_handle[index] = NULL;

	return VS_OK;
}


// ---------- SetDisplayListState ----------
/*!
\brief Set Display list state
\author Gareth Edwards
\param INT - [in] handle
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::SetDisplayListState(UINT index, INT state)
{

	if (index < PANEL_DISPLAY_LIST_MAX)
	{
		pi_panel_context->panel_display_list_state[index] = state;
		return VS_OK;
	}
	pi_panel_context->panel_display_list_state[index] = NULL;

	return VS_OK;
}


// ---------- SetDisplayListIndex ----------
/*!
\brief set display list index
\author Gareth Edwards
\param INT * - [in/out] handle
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::SetDisplayListIndex(UINT index)
{
	pi_panel_context->panel_display_list_index = index;
	return VS_ERROR;
}


// ---------- SetGroupId ----------
/*!
\brief Set context Group Id
\author Gareth Edwards
\param UINT - [in] group id
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::SetGroupId(UINT gid)
{
	pi_panel_context->panel_group_id = gid;
	return VS_OK;
}


// ---------- SetHandle ----------
/*!
\brief Set panel handle
\author Gareth Edwards
\param UINT - [in] id
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::SetHandle(UINT handle)
{
	pi_panel_context->panel_handle = handle;
	return VS_OK;
}


// ---------- SetId ----------
/*!
\brief Set context Id
\author Gareth Edwards
\param UINT - [in] id
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::SetId(UINT id)
{
	pi_panel_context->panel_id = id;
	return VS_OK;
}


// ---------- SetVersion ----------
/*!
\brief Set context version
\author Gareth Edwards
\param UINT - [in] id
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::SetVersion(UINT version)
{
	pi_panel_context->panel_version = version;
	return VS_OK;
}


// ---------- SetName ----------
/*!
\brief Set context Name
\author Gareth Edwards
\param CHAR * - [in] name
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::SetName(CHAR* name)
{
	strcpy(pi_panel_context->panel_name, name);
	return VS_OK;
}


// ---------- SetBackground ----------
/*!
\brief Set status of Panel background
\author Gareth Edwards
\return BOOL - status
*/
INT PanelContext::SetBackground(BOOL background)
{
	pi_panel_context->panel_background = background;
	if ( background )
		pi_panel_context->panel_outline = FALSE;
	return VS_OK;
}


// ---------- SetOutline ----------
/*!
\brief Set status of Panel outline
\author Gareth Edwards
\return BOOL - status
*/
INT PanelContext::SetOutline(BOOL outline)
{
	pi_panel_context->panel_outline = outline;
	if ( outline )
		pi_panel_context->panel_background = FALSE;
	return VS_OK;
}


// ---------- SetEchoOver ----------
/*!
\brief Set status of Panel echo over
\author Gareth Edwards
\return BOOL - status
*/
INT PanelContext::SetEchoOver(BOOL echo_over)
{
	pi_panel_context->panel_echo_over = echo_over;
	return VS_OK;
}


// ---------- SetSelected ----------
/*!
\brief Set selected status
\author Gareth Edwards
\return BOOL - selected status
*/
INT PanelContext::SetSelected(BOOL selected)
{
	pi_panel_context->panel_selected = selected;
	return VS_OK;
}


// ---------- SetDisplayStatus ----------
/*!
\brief Set display status
\author Gareth Edwards
\return BOOL * - selected status
*/
INT PanelContext::SetDisplayStatus(BOOL status)
{
	pi_panel_context->panel_display_status = status;
	return VS_OK;
}


// ---------- SetInteractionStatus ----------
/*!
\brief Set user interaction status
\author Gareth Edwards
\return BOOL * - selected status
*/
INT PanelContext::SetInteractionStatus(BOOL status)
{
	pi_panel_context->panel_over_status = status;
	return VS_OK;
}


// ---------- GetRenderToImageDimensions ----------
/*!
\brief Get Render To Image Dimensions
\author Gareth Edwards
\param UINT - [out] width
\param UINT - [out] height
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::GetRenderToImageDimensions(UINT *width, UINT *height)
{
	*width = pi_panel_context->render_to_image_width;
	*height = pi_panel_context->render_to_image_height;
	return VS_OK;
}


// ---------- GetRenderToImageBegin ----------
/*!
\brief Get Render To Image Begin value
\author Gareth Edwards
\return INT - begin value
*/
INT PanelContext::GetRenderToImageBegin()
{
	return pi_panel_context->render_to_image_begin;
}


// ---------- GetRenderToImageEnd ----------
/*!
\brief Get Render To Image End value
\author Gareth Edwards
\return INT - end value
*/
INT PanelContext::GetRenderToImageEnd()
{
	return pi_panel_context->render_to_image_end;
}


// ---------- GetRenderToImageFlag ----------
/*!
\brief Get Render To Image Flag status
\author Gareth Edwards
\return INT - flag status
*/
INT PanelContext::GetRenderToImageFlag(BOOL *status)
{
	*status = pi_panel_context->render_to_image_flag;
	return VS_OK;
}


// ---------- GetRenderToImageId ----------
/*!
\brief Get Render To Image Id value
\author Gareth Edwards
\return INT - id value
*/
INT PanelContext::GetRenderToImageId()
{
	return pi_panel_context->render_to_image_id;
}


// ---------- GetRenderToImageStatus ----------
/*!
\brief Get Render To Image Status
\author Gareth Edwards
\return BOOL - Status
*/
BOOL PanelContext::GetRenderToImageStatus()
{
	return pi_panel_context->render_to_image_status;
}


// ---------- SetRenderToImageDimensions ----------
/*!
\brief Set Render To Image Dimensions
\author Gareth Edwards
\param UINT - [in] width
\param UINT - [in] height
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::SetRenderToImageDimensions(UINT width, UINT height)
{
	pi_panel_context->render_to_image_width = width;
	pi_panel_context->render_to_image_height = height;
	return VS_OK;
}


// ---------- SetRenderToImageBegin ----------
/*!
\brief Set Render To Image Begin Flag
\author Gareth Edwards
\param BOOL - [in] status
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::SetRenderToImageBegin(BOOL flag)
{
	pi_panel_context->render_to_image_begin = flag;
	return VS_OK;
}


// ---------- SetRenderToImageEnd ----------
/*!
\brief Set Render To Image End Flag
\author Gareth Edwards
\param BOOL - [in] status
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::SetRenderToImageEnd(BOOL flag)
{
	pi_panel_context->render_to_image_end = flag;
	return VS_OK;
}


// ---------- SetRenderToImageFlag ----------
/*!
\brief Set Render To Image Flag
\author Gareth Edwards
\param BOOL - [in] status
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::SetRenderToImageFlag(BOOL flag)
{
	pi_panel_context->render_to_image_flag = flag;
	return VS_OK;
}


// ---------- SetRenderToImageId ----------
/*!
\brief Set Render To Image Id
\author Gareth Edwards
\param INT - [in] status
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::SetRenderToImageId(INT id)
{
	pi_panel_context->render_to_image_id = id;
	return VS_OK;
}


// ---------- SetRenderToImageStatus ----------
/*!
\brief Set Render To Image Status
\author Gareth Edwards
\param BOOL - [in] status
\return INT - ERROR <= 0 < VS_OK
*/
INT PanelContext::SetRenderToImageStatus(BOOL status)
{
	pi_panel_context->render_to_image_status = status;
	return (INT)pi_panel_context->render_to_image_id;
}


// ---------- Constructor ----------
/*!
\brief Constructor.
\author Gareth Edwards
*/
PanelContext::PanelContext()
{
	// Create opaque pointer
	pi_panel_context = new PI_PanelContext;
}


// ---------- Destructor ----------
/*!
\brief Destructor.
\author Gareth Edwards
*/
PanelContext::~PanelContext()
{
	// Delete opaque pointer
	if ( pi_panel_context != NULL ) { delete pi_panel_context; pi_panel_context = NULL; }
}


// ---------- Configure ----------
/*!
\brief Configure gfx_context
\author Gareth Edwards
\param VNode * - [in] HDL configuration data
\return INT - ERROR <= 0 < VS_OK
\note
{
	Parameters include:
	Name:[CHAR];
	Display:[CHAR];
	Group:[INT];
	Handle:[INT];
	Background:[BOOL];
	Background-Colour {
		Red:[FLOAT 0.0 to 1.0];
		Green:[FLOAT 0.0 to 1.0];
		Blue:[FLOAT 0.0 to 1.0];
		Alpha:[FLOAT 0.0 to 1.0];
	}
	Foreground-Colour{
		Red:[FLOAT 0.0 to 1.0];
		Green:[FLOAT 0.0 to 1.0];
		Blue:[FLOAT 0.0 to 1.0];
		Alpha:[FLOAT 0.0 to 1.0];
	}
	Viewrect{
		Left:[FLOAT 0.0 to 1.0];
		Top:[FLOAT 0.0 to 1.0];
		Right:[FLOAT 0.0 to 1.0];
		Bottom:[FLOAT 0.0 to 1.0];
	}
	Margin{
		Left:[FLOAT 0.0 to 1.0];
		Top:[FLOAT 0.0 to 1.0];
		Right:[FLOAT 0.0 to 1.0];
		Bottom:[FLOAT 0.0 to 1.0];
		Width:[FLOAT 0.0 to 1.0];
	}
	Border{
		Left:[FLOAT 0.0 to 1.0];
		Top:[FLOAT 0.0 to 1.0];
		Right:[FLOAT 0.0 to 1.0];
		Bottom:[FLOAT 0.0 to 1.0];
		Width:[FLOAT 0.0 to 1.0];
	}
	Projection{
		Type:"Perspective", "Front", etc....
		FOV:[FLOAT 0.0+ to 180.0];     //  Perspective
		Near:[FLOAT 0.0 to ?];
		Far:[FLOAT Near to ?];
		X_Offset:[FLOAT 0.0 to 1.0];   // Orthoganal
		Y_Offset:[FLOAT 0.0 to 1.0];
		Scale:[FLOAT 0.0+ to 1.0];
		Z Near:[FLOAT 0.0 to 1.0];     // Orthoganal Z viewing volume
		Z Far:[FLOAT Z Near to 1.0];
	}
	View{
		Type:"XYZ", "YZX", "ZXY", "XZY", "ZYX" or "YXZ";
		RX:[FLOAT];
		RY:[FLOAT];
		RZ:[FLOAT];
		TX:[FLOAT];
		TY:[FLOAT];
		TZ:[FLOAT];
	}
}
*/
INT PanelContext::Configure(VNode *panel_hdl)
{

	// ----no panel hdl data ? - return
		if (panel_hdl == NULL) return VS_ERROR;


	// ---- local
		VNode *p = NULL;
		VNode *q = NULL;


	// ---- local
		PI_PanelContext *pimp = pi_panel_context;


	// ---- data ?
		pimp->panel_display_list_top = 0;
		VNode *node = panel_hdl->GetFirst();
		while ( node != NULL )
		{

			// ---- panel
				if ( p=node->SearchName("Name") )
				{
					if ( p->GetValue() != NULL )
						strcpy(pimp->panel_name, p->GetValue());
				}
				else if ( p=node->SearchName("Display") )
				{
					if ( p->GetValue() != NULL )
					{
						strcpy(
								pimp->panel_display_list_names[pimp->panel_display_list_top],
								p->GetValue()
							);
						pimp->panel_display_list_top++;
					}
				}
				else if ( p=node->SearchName("Group-Id") )
				{
					if ( p->GetValue() != NULL )
						pimp->panel_group_id = p->GetIntFromValue();
				}
				else if ( p=node->SearchName("Id") )
				{
					if ( p->GetValue() != NULL )
						pimp->panel_id = p->GetIntFromValue();
				}
				else if ( p=node->SearchName("Handle") )
				{
					if ( p->GetValue() != NULL )
						pimp->panel_handle = p->GetIntFromValue();
				}
				else if ( p=node->SearchName("Background") )
				{
					if ( p->GetValue() != NULL )
						pimp->panel_background = p->GetBoolFromValue();
					if ( pimp->panel_background)
						pimp->panel_outline = FALSE;
				}
				else if ( p = node->SearchName("Outline") )
				{
					if ( p->GetValue() != NULL )
						pimp->panel_outline = p->GetBoolFromValue();
					if ( pimp->panel_outline )
						pimp->panel_background = FALSE;
				}
				else if ( p=node->SearchName("Selected") )
				{
					if ( p->GetValue() != NULL )
						pimp->panel_selected = p->GetBoolFromValue();
				}
				else if ( p=node->SearchName("Status") )
				{
					if ( p->GetValue() != NULL )
						pimp->panel_display_status = p->GetBoolFromValue();
				}


			// ---- colour ----
				else if ( p=node->SearchName("Background-Colour") )
				{
					INT result = pi_panel_context->GetD3dColor(p, &pimp->background_colour);
					pimp->use_background_colour = pimp->background_colour != 0 ? TRUE : FALSE;
				}
				else if ( p=node->SearchName("Background-Colour-Over") )
				{
					INT result = pi_panel_context->GetD3dColor(p, &pimp->background_colour_over);
				}


			// ---- layout  ----
				else if ( p=node->SearchName("Viewrect") )
				{
					if ( q=p->SearchName("Left")   ) pimp->viewrect.left   = q->GetFloatFromValue();
					if ( q=p->SearchName("Top")    ) pimp->viewrect.top    = q->GetFloatFromValue();
					if ( q=p->SearchName("Right")  ) pimp->viewrect.right  = q->GetFloatFromValue();
					if ( q=p->SearchName("Bottom") ) pimp->viewrect.bottom = q->GetFloatFromValue();
				}
				else if ( p=node->SearchName("Margin") )
				{
					if ( q=p->SearchName("Left")   ) pimp->margin.left   = q->GetFloatFromValue();
					if ( q=p->SearchName("Top")    ) pimp->margin.top    = q->GetFloatFromValue();
					if ( q=p->SearchName("Right")  ) pimp->margin.right  = q->GetFloatFromValue();
					if ( q=p->SearchName("Bottom") ) pimp->margin.bottom = q->GetFloatFromValue();
					if ( q=p->SearchName("Width") ) 
					{
						FLOAT margin_width = q->GetFloatFromValue();
						pimp->margin.left   = margin_width;
						pimp->margin.top    = margin_width;
						pimp->margin.right  = margin_width;
						pimp->margin.bottom = margin_width;
					}
				}
				else if ( p=node->SearchName("Border") )
				{
					if ( q=p->SearchName("Left")   ) pimp->border.left   = q->GetFloatFromValue();
					if ( q=p->SearchName("Top")    ) pimp->border.top    = q->GetFloatFromValue();
					if ( q=p->SearchName("Right")  ) pimp->border.right  = q->GetFloatFromValue();
					if ( q=p->SearchName("Bottom") ) pimp->border.bottom = q->GetFloatFromValue();
					if ( q=p->SearchName("Width")  ) 
					{
						FLOAT border_width = q->GetFloatFromValue();
						pimp->border.left   = border_width;
						pimp->border.top    = border_width;
						pimp->border.right  = border_width;
						pimp->border.bottom = border_width;
					}
					if ( q=p->SearchName("Over") )
					{
						FLOAT over_width  = q->GetFloatFromValue();
						pimp->border_over.left   = over_width;
						pimp->border_over.top    = over_width;
						pimp->border_over.right  = over_width;
						pimp->border_over.bottom = over_width;
					}
				}


			// ---- projection
				else if ( p=node->SearchName("Projection") )
				{
					// type
					if ( q=p->SearchName("Type") ) 
					{
						UINT type_id = pimp->GetTypeId(q->GetValue());
						if (type_id)
						{
							strcpy(pimp->type, q->GetValue());
							pimp->type_id = type_id;
						}
					}

					// perspective
					if ( q=p->SearchName("FOV")      ) pimp->perspective.Init(0, q->GetFloatFromValue());
					if ( q=p->SearchName("Near")     ) pimp->perspective.Init(1, q->GetFloatFromValue());
					if ( q=p->SearchName("Far")      ) pimp->perspective.Init(2, q->GetFloatFromValue());

					// orthoganal
					if ( q=p->SearchName("X_Offset") ) pimp->orthogonal.Init(0, q->GetFloatFromValue());
					if ( q=p->SearchName("Y_Offset") ) pimp->orthogonal.Init(1, q->GetFloatFromValue());
					if ( q=p->SearchName("Scale")    ) pimp->orthogonal.Init(2, q->GetFloatFromValue());
			
					// orthoganal Z viewing volume
					if ( q=p->SearchName("Z_Near")   ) pimp->orthogonal.Init(3, q->GetFloatFromValue());
					if ( q=p->SearchName("Z_Far")    ) pimp->orthogonal.Init(4, q->GetFloatFromValue());
				}


			// ---- orientation


			// ---- view
				else if ( p=node->SearchName("View") )
				{

					// ---- order
						if ( q=p->SearchName("Type") )
						{
							if (strlen(q->GetValue()) == 3 )
							{
								char *list[6] = { "XYZ", "YZX", "ZXY", "XZY", "ZYX", "YXZ" };
								for (UINT i=0; i<6; i++)
								{
									if (!strcmp(list[i], q->GetValue()) )
									{
										strcpy(pimp->type, q->GetValue());
										break;
									}
								}
							}
						}

					// ---- transform
						if ( q=p->SearchName("RX") ) pimp->rotate.Init(0, q->GetFloatFromValue());
						if ( q=p->SearchName("RY") ) pimp->rotate.Init(1, q->GetFloatFromValue());
						if ( q=p->SearchName("RZ") ) pimp->rotate.Init(2, q->GetFloatFromValue());
						if ( q=p->SearchName("TX") ) pimp->translate.Init(0, q->GetFloatFromValue());
						if ( q=p->SearchName("TY") ) pimp->translate.Init(1, q->GetFloatFromValue());
						if ( q=p->SearchName("TZ") ) pimp->translate.Init(2, q->GetFloatFromValue());

				}


			//  ---- render tib
				else if ( p=node->SearchName("Render") )
				{
					if ( q=p->SearchName("Begin") )  pimp->render_to_image_begin  = q->GetBoolFromValue();
					if ( q=p->SearchName("End") )    pimp->render_to_image_end    = q->GetBoolFromValue();
					if ( q=p->SearchName("ID") )     pimp->render_to_image_id     = (UINT)q->GetIntFromValue();
					if ( q=p->SearchName("Status") ) pimp->render_to_image_status = q->GetBoolFromValue();
					if ( q=p->SearchName("Height") ) pimp->render_to_image_height = (UINT)q->GetIntFromValue();
					if ( q=p->SearchName("Width")  ) pimp->render_to_image_width  = (UINT)q->GetIntFromValue();
				}


			// ---- library
				else if ( !strcmp(node->GetName(), "$lib") )
				{
					VNode *definition = node->GetListItem("library", node->GetValue());
					if ( definition )
					{
						Configure(definition);
					}
				}

			node = node->GetNext();
		}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
