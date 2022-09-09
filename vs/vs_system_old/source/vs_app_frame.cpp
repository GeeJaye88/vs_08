////////////////////////////////////////////////////////////////////////////////

// ---------- app_frame.cpp ----------
/*!
\file app_frame.cpp
\brief Implementation of the App(lication)Frame class
\author Gareth Edwards 
*/

// ---- include
#include "../header/vs_app_frame.h"


// ---- namespace
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_AppFrame class ----------

class AppFrame::PI_AppFrame
{

public:

	PI_AppFrame() {

		// ---- stuff
			app_dependency = NULL;
			gfx_interface  = NULL;

			next        = NULL;
			last        = NULL;
			first_panel = NULL;
			last_panel  = NULL;
			home        = FALSE;
			strcpy(name, "");


		// ---- layout
			VsFloatRectangle box_default = { 0, 0, 0, 0 };
			border = box_default;


		// ---- colour
			background = FALSE;
			background_color = { 0, 0, 0, 0 };
			background_color_over = { 0, 0, 0, 0 };
			texture_border = FALSE;
			texture_border_color = { 0, 0, 0, 0 };
			texture_border_color_over = { 0, 0, 0, 0 };
	};

	~PI_AppFrame() { };


	// ---- local
		AppDependency *app_dependency;
		GfxInterface  *gfx_interface;


	// ---- hierarchy
		AppFrame *next;
		AppFrame *last;
		AppPanel *first_panel;
		AppPanel *last_panel;
		BOOL      home;
		CHAR      name[VS_MAXCHARLEN];


	// ---- layout
		VsFloatRectangle border;


	// ---- colour
		BOOL      background;
		VsColour background_color;
		VsColour background_color_over;
		BOOL      texture_border;
		VsColour texture_border_color;
		VsColour texture_border_color_over;
};


////////////////////////////////////////////////////////////////////////////////


// ---------- constructor ----------
/*!
\brief Constructor.
\author Gareth Edwards
*/
AppFrame::AppFrame()
{
	pi_app_frame = new PI_AppFrame;
}


// ---------- destructor ----------
/*!
\brief Destructor.
\author Gareth Edwards
*/
AppFrame::~AppFrame()
{
	AppPanel* panel = pi_app_frame->first_panel;
	while (panel)
	{
		AppPanel* next = panel->GetNext();
		delete panel;
		panel = next;
	}
	delete pi_app_frame;
	pi_app_frame = NULL;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup.
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT AppFrame::Cleanup()
{
	AppPanel* panel = pi_app_frame->first_panel;
	while (panel)
	{
		panel->Cleanup();
		panel = panel->GetNext();
	}
	return VS_OK;
}


// ---------- Display ----------
/*!
\brief Display.
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT AppFrame::Display(INT handle_app_obj, INT (*app_callback_display)(INT, AppPanel *))
{

	// ---- local
		UINT result = VS_OK;
		GfxInterface *gfx = pi_app_frame->gfx_interface;


	// ---- begin - set panel viewport (viewrect), render states, texure states, etc...
		result = gfx->RenderBegin();


	// ---- set the panel (viewport) background
		result = gfx->SetBackgroundColor(
				pi_app_frame->background,
				&pi_app_frame->background_color,
				&pi_app_frame->background_color_over
			);


	// ---- set the border colour of textures
		result = gfx->SetTextureBorderColor(
				pi_app_frame->texture_border,
				&pi_app_frame->texture_border_color,
				&pi_app_frame->texture_border_color_over
			);


	// ---- invoke application display callback method
		AppPanel* panel = pi_app_frame->first_panel;
		while ( panel )
		{
			AppPanel* master_panel = panel->GetMaster();
			master_panel = master_panel == NULL ? panel : master_panel;
			result = master_panel->Display(handle_app_obj, app_callback_display);
			panel = panel->GetNext();
		}


	// ---- end - restore viewport, end scene, end pick, etc...
		result = gfx->RenderEnd();


	// IF -- touch mode -- THEN -- reset mouse
		vs_system::SysContext *sys_context = pi_app_frame->app_dependency->GetSysContext();
		BOOL touch = sys_context->GetBool(sys_context->TOUCH);
		if (touch)
		{
			vs_system::SysInput *sys_input = pi_app_frame->app_dependency->GetSysInput();
			result = sys_input->ResetMouse();
		}

	return VS_OK;
}


// ---------- Setup ----------
/*!
\brief Setup.
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT AppFrame::Setup(AppDependency *ad)
{
	pi_app_frame->app_dependency = ad;
	pi_app_frame->gfx_interface = ad->GetGfxInterface();
	AppPanel* panel = pi_app_frame->first_panel;
	while (panel)
	{
		panel->Setup(ad);
		panel = panel->GetNext();
	}
	return VS_OK;
}


// ---------- Configure ----------
/*!
\brief Configure frame
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT AppFrame::Configure(VNode *frame_node)
{

	VNode *p = NULL;
	VNode *node = frame_node->GetFirst();
	while ( node )
	{
		if ( !strcmp(node->GetName(), "Panel") )
		{
			INT result=AppendPanelToList(node);
		}
		else if ( !strcmp(node->GetName(), "Home") )
		{
			if ( node->GetValue() != NULL )
				pi_app_frame->home = node->GetBoolFromValue();
		}
		else if ( !strcmp(node->GetName(), "Name") )
		{
			if ( node->GetValue() != NULL )
				strcpy(pi_app_frame->name, node->GetValue());
		}
		else if (p = node->SearchName("Border"))
		{
			if (p = node->SearchName("Left"))   pi_app_frame->border.left   = p->GetFloatFromValue();
			if (p = node->SearchName("Top"))    pi_app_frame->border.top    = p->GetFloatFromValue();
			if (p = node->SearchName("Right"))  pi_app_frame->border.right  = p->GetFloatFromValue();
			if (p = node->SearchName("Bottom")) pi_app_frame->border.bottom = p->GetFloatFromValue();
			if (p = node->SearchName("Width"))
			{
				FLOAT border_width = p->GetFloatFromValue();
				pi_app_frame->border.left   = border_width;
				pi_app_frame->border.top    = border_width;
				pi_app_frame->border.right  = border_width;
				pi_app_frame->border.bottom = border_width;
			}
		}
		else if (p = node->SearchName("Background-Colour"))
		{
			pi_app_frame->background = TRUE;
			if ( p = node->SearchName("Red")   ) pi_app_frame->background_color.red   = p->GetFloatFromValue();
			if ( p = node->SearchName("Green") ) pi_app_frame->background_color.green = p->GetFloatFromValue();
			if ( p = node->SearchName("Blue")  ) pi_app_frame->background_color.blue  = p->GetFloatFromValue();
			if ( p = node->SearchName("Alpha") ) pi_app_frame->background_color.alpha = p->GetFloatFromValue();
		}
		else if (p = node->SearchName("Background-Colour-Over"))
		{
			if ( p = node->SearchName("Red")   ) pi_app_frame->background_color_over.red   = p->GetFloatFromValue();
			if ( p = node->SearchName("Green") ) pi_app_frame->background_color_over.green = p->GetFloatFromValue();
			if ( p = node->SearchName("Blue")  ) pi_app_frame->background_color_over.blue  = p->GetFloatFromValue();
			if ( p = node->SearchName("Alpha") ) pi_app_frame->background_color_over.alpha = p->GetFloatFromValue();
		}
		else if ( p = node->SearchName("Texture-Border-Colour") )
		{
			pi_app_frame->texture_border = TRUE;
			if ( p = node->SearchName("Red")   ) pi_app_frame->texture_border_color.red   = p->GetFloatFromValue();
			if ( p = node->SearchName("Green") ) pi_app_frame->texture_border_color.green = p->GetFloatFromValue();
			if ( p = node->SearchName("Blue")  ) pi_app_frame->texture_border_color.blue  = p->GetFloatFromValue();
			if ( p = node->SearchName("Alpha") ) pi_app_frame->texture_border_color.alpha = p->GetFloatFromValue();
		}
		else if ( p = node->SearchName("Texture-Border-Colour-Over") )
		{
			if ( p = node->SearchName("Red")   ) pi_app_frame->texture_border_color_over.red   = p->GetFloatFromValue();
			if ( p = node->SearchName("Green") ) pi_app_frame->texture_border_color_over.green = p->GetFloatFromValue();
			if ( p = node->SearchName("Blue")  ) pi_app_frame->texture_border_color_over.blue  = p->GetFloatFromValue();
			if ( p = node->SearchName("Alpha") ) pi_app_frame->texture_border_color_over.alpha = p->GetFloatFromValue();
		}
		else if ( !strcmp(node->GetName(),"$lib") )
		{
			VNode *definition_node = node->GetListItem( "library", node->GetValue() );
			if (definition_node)
			{
				Configure(definition_node);
			}
		}

		node = node->GetNext();
	}

	return VS_OK;
}


// ---------- AppendPanelToList ----------
/*!
\brief Append panel to end of linked list
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT AppFrame::AppendPanelToList(VNode *node)
{

	AppPanel *panel = new AppPanel();
	if ( panel != NULL )
	{
		if ( pi_app_frame->first_panel == NULL )
			pi_app_frame->first_panel = pi_app_frame->last_panel = panel;
		else
		{
			pi_app_frame->last_panel->SetNext(panel);
			pi_app_frame->last_panel = panel;
		}
		panel->Configure(node);
		return VS_OK;
	}

	return VS_ERROR;
}


// ---------- GetFirstPanel ----------
/*!
\brief Get first panel
\author Gareth Edwards
*/
AppPanel *AppFrame::GetFirstPanel()
{
	return pi_app_frame->first_panel;
}


// ---------- GetHome ----------
/*!
\brief Get home frame status
\author Gareth Edwards
*/
BOOL AppFrame::GetHome()
{
	return pi_app_frame->home;
}


// ---------- GetName ----------
/*!
\brief Get name of frame
\author Gareth Edwards
*/
CHAR * AppFrame::GetName()
{
	return pi_app_frame->name;
}


// ---------- GetNext ----------
/*!
\brief Get next frame
\author Gareth Edwards
*/
AppFrame * AppFrame::GetNext()
{
	return pi_app_frame->next;
}


// ---------- GetLast ----------
/*!
\brief Get last frame
\author Gareth Edwards
*/
AppFrame * AppFrame::GetLast()
{
	return pi_app_frame->last;
}


// ---------- SetHome ----------
/*!
\brief Set home frame
\author Gareth Edwards
*/
INT AppFrame::SetHome(BOOL status)
{
	pi_app_frame->home = status;
	return VS_OK;
}


// ---------- SetNext ----------
/*!
\brief Set next frame
\author Gareth Edwards
*/
INT AppFrame::SetNext(AppFrame *n)
{
	pi_app_frame->next = n;
	return n == NULL ? VS_ERROR : VS_OK;
}


// ---------- SetLast ----------
/*!
\brief Set last frame
\author Gareth Edwards
*/
INT AppFrame::SetLast(AppFrame *n)
{
	pi_app_frame->last = n;
	return n == NULL ? VS_ERROR : VS_OK;
}


// ---------- SelectPanel ----------
/*!
\brief Set last frame
\author Gareth Edwards
*/
INT AppFrame::SelectPanel(
		CHAR *name,
		BOOL master
	)
{
	vs_system::AppPanel *panel = GetFirstPanel();
	vs_system::AppPanel *use_panel = NULL;
	while ( panel )
	{

		// ---- master or slave
			if ( master )
			{
				use_panel = panel->GetMaster();
				use_panel = use_panel == NULL ? panel : use_panel;
			}
			else
				use_panel = panel;


		// --- select ?
			CHAR use_name[VS_MAXCHARNAME];
			use_panel->GetPanelContext()->GetName(use_name);
			use_panel->GetPanelContext()->SetSelected(
					!strcmp(use_name, name)
				);

		panel = panel->GetNext();

	}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
