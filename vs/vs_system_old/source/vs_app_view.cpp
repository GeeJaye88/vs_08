////////////////////////////////////////////////////////////////////////////////

// ---------- app_view.cpp ----------
/*!
\file app_view.cpp
\brief Implementation of the App(lication)View class
\author Gareth Edwards 
*/

// ---- include
#include "../header/vs_app_view.h"


// ---- namespace
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_AppFrame class ----------

class AppView::PI_AppView
{
public:
	PI_AppView() {
		app_dependency = NULL;
		selected_frame = NULL;
		first_frame = NULL;
		last_frame = NULL;
	};
	~PI_AppView() { };
	AppFrame *AppendFrame();
	AppDependency *app_dependency;
	AppFrame *selected_frame;
	AppFrame *first_frame;
	AppFrame *last_frame;
};


// ---------- AppendFrame ----------
/*!
\brief Append frame 
\author Gareth Edwards
\return AppFrame * -
*/
AppFrame *AppView::PI_AppView::AppendFrame()
{
	AppFrame *frame = new AppFrame();
	if ( frame != NULL )
	{
		if ( first_frame == NULL )
		{
			first_frame = last_frame = frame;
		}
		else
		{
			last_frame->SetNext(frame);
			frame->SetLast(last_frame);
			last_frame = frame;
		}
		return frame;
	}
	return NULL;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- constructor ----------
/*!
\brief Constructor.
\author Gareth Edwards
*/
AppView::AppView()
{
	pi_app_view = new PI_AppView;
}


// ---------- destructor ----------
/*!
\brief Destructor.
\author Gareth Edwards
*/
AppView::~AppView()
{
	AppFrame* frame = pi_app_view->first_frame;
	while (frame)
	{
		AppFrame* next = frame->GetNext();
		delete frame;
		frame = next;
	}
	delete pi_app_view;
	pi_app_view = NULL;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup.
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT AppView::Cleanup()
{
	AppFrame* frame = pi_app_view->first_frame;
	while (frame)
	{
		frame->Cleanup();
		frame = frame->GetNext();
	}
	return VS_OK;
}


// ---------- Display ----------
/*!
\brief Display.
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT AppView::Display(
		INT handle_app_obj,
		INT (*app_callback_display)(INT, AppPanel *)
	)
 {

	pi_app_view->selected_frame->Display(handle_app_obj, app_callback_display);

	return VS_OK;
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT AppView::Setup(AppDependency *ad)
{

	// ---- configure
		pi_app_view->app_dependency = ad;

		VNode *v = pi_app_view->app_dependency->GetViewConfigData();
		INT result = Configure(v);


	// ---- select "home" frame - update below
		pi_app_view->selected_frame = pi_app_view->first_frame;


	// ---- setup
		AppFrame* frame = pi_app_view->first_frame;
		while (frame)
		{
			result = frame->Setup(pi_app_view->app_dependency);
			if ( frame->GetHome() )
				pi_app_view->selected_frame = frame;
			frame = frame->GetNext();
		}


	// ---- set master display versions of each panel
		result = SetMasterPanels();

	return VS_OK;
}


// ---------- Configure ----------
/*!
\brief Configure AppView
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT AppView::Configure(VNode *view_config_hdl)
{

	// ---- configure data ?
	if ( view_config_hdl != NULL )
	{

		// ---- main ?
		if ( VNode *main_hdl = view_config_hdl->SearchName("main") )
		{

			// ---- frameset ?
			if ( VNode *frameset_hdl=main_hdl->SearchName("Frameset") )
			{

				// ---- initialise first and last frame
					pi_app_view->first_frame = pi_app_view->last_frame =  NULL;


				// ---- for each frameset node...
					VNode *frame_hdl = frameset_hdl->GetFirst();
					while ( frame_hdl )
					{
						if ( !strcmp(frame_hdl->GetName(), "Frame") )
							INT result=AppendFrameToList(frame_hdl);
						frame_hdl = frame_hdl->GetNext();
					}


				// ---- note: panel selected


				// ---- no frames ?
					if ( pi_app_view->first_frame == NULL )
						return VS_ERROR;

			}

			// ---- no frameset ?
			else return VS_ERROR;

		}
	}

	return VS_OK;
}


// ---------- AppendFrameToList ----------
/*!
\brief Append frame to end of linked list
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT AppView::AppendFrameToList(VNode *frame_hdl)
{
	AppFrame *frame = pi_app_view->AppendFrame();
	if ( frame != NULL )
	{
		frame->Configure(frame_hdl);
		return VS_OK;
	}
	return VS_ERROR;
}


// ---------- SetMasterPanels ----------
/*!
\brief Set master display versions of each panel
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK

\note

FOR each frame
	FOR each panel

		FOR all other subsequent frames
			FOR all other panel

				IF other panel name == panel name THEN
					other panel * == panel * (it becomes the "master")
			END
		END

	END
END

\note Each panel has one or more display lists. 

\note Each panel also has a PanelContext object.

\note If a PanelContext pointer is set to the panel PanelContext, then, the
master panel PanelContext display can be set off or on.

\note Only one occurence of a master panel can be present in a frame set.

*/
INT AppView::SetMasterPanels()
{

	// ---- local
		INT result = 0;


	// ---- for each frame ...
		AppFrame *frame = pi_app_view->first_frame;
		while ( frame )
		{

			// ---- for each panel of current frame ...
			AppPanel* panel = frame->GetFirstPanel();
			while ( panel )
			{

				CHAR panel_name[VS_MAXCHARNAME];
				PanelContext *panel_context = panel->GetPanelContext();
				result = panel_context->GetName(panel_name);


				// ---- for all other subsequent frames ...
				AppFrame *other_frame = frame->GetNext();
				while ( other_frame )
				{

					// ---- for each subsequent panel ...
					AppPanel* other_panel = other_frame->GetFirstPanel();
					while ( other_panel )
					{

						// ---- panel does NOT have a master ?
						if ( other_panel->GetMaster() == NULL )
						{
							CHAR other_panel_name[VS_MAXCHARNAME];
							PanelContext *other_panel_context = other_panel->GetPanelContext();
							result = other_panel_context->GetName(other_panel_name);


							// ---- matched ?
							if ( !strcmp(panel_name, other_panel_name) )
							{
								other_panel->SetMaster(panel);
							}
						}

						other_panel = other_panel->GetNext();

					}
				
					other_frame = other_frame->GetNext();

				}

				panel = panel->GetNext();

			}

			frame = frame->GetNext();

		}

	return VS_OK;
}


// ---------- GetFirstFrame ----------
/*!
\brief Select first frame
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
AppFrame * AppView::GetFirstFrame()
{
	return pi_app_view->first_frame;
}


// ---------- GetFrame ----------
/*!
\brief Get frame by name
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
AppFrame * AppView::GetFrame(CHAR *name)
{

	if (name == NULL) return VS_ERROR;

	INT result = VS_ERROR;
	AppFrame *frame = pi_app_view->first_frame;
	while ( frame )
	{
		CHAR *frame_name = frame->GetName();
		if ( frame_name != NULL )
		{
			if ( !strcmp(frame_name, name) )
			{
				return frame;
			}
		}
		frame = frame->GetNext();
	}

	return NULL;
}


// ---------- GetSelectedFrame ----------
/*!
\brief Get selected frame
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
AppFrame * AppView::GetSelectedFrame()
{

	if ( pi_app_view->selected_frame != NULL )
		return pi_app_view->selected_frame;

	return VS_ERROR;
}


// ---------- GetDisplayStatus ----------
/*!
\brief Get display status
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT AppView::GetDisplayStatus(CHAR *name, BOOL *status)
{

	// ---- local
		INT result = 0;


	// ---- for each frame ....
		AppFrame *frame = pi_app_view->first_frame;
		while ( frame )
		{

			// ---- for each panel of current frame ....
			AppPanel* panel = frame->GetFirstPanel();
			while ( panel )
			{

				// ---- null name ?
				PanelContext *panel_context = panel->GetPanelContext();
				if ( name == NULL )
				{
					panel_context->GetDisplayStatus(status);
				}
				else
				{
					// ---- name matched ?
					CHAR panel_name[VS_MAXCHARNAME];
					result = panel_context->GetName(panel_name);
					if ( panel_name != NULL )
					{
						if (!strcmp(name, panel_name))
						{
							panel_context->GetDisplayStatus(status);
						}
					}
				}

				panel = panel->GetNext();

			}

			frame = frame->GetNext();

		}

	return VS_OK;
}


// ---------- SetDisplayStatus ----------
/*!
\brief Set display status
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT AppView::SetDisplayStatus(CHAR *name, BOOL status)
{

	// ---- local
		INT result = 0;


	// ---- for each frame ....
		AppFrame *frame = pi_app_view->first_frame;
		while ( frame )
		{

			// ---- for each panel of current frame ....
			AppPanel* panel = frame->GetFirstPanel();
			while ( panel )
			{

				// ---- null name ?
				PanelContext *panel_context = panel->GetPanelContext();
				if ( name == NULL )
				{
					panel_context->SetDisplayStatus(status);
				}
				else
				{
					// ---- name matched ?
					CHAR panel_name[VS_MAXCHARNAME];
					result = panel_context->GetName(panel_name);
					if ( panel_name != NULL )
					{
						if (!strcmp(name, panel_name))
						{
							panel_context->SetDisplayStatus(status);
						}
					}
				}

				panel = panel->GetNext();

			}

			frame = frame->GetNext();

		}

	return VS_OK;
}


// ---------- GetInteractionStatus ----------
/*!
\brief Get interaction status
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT AppView::GetInteractionStatus(CHAR *name, BOOL *status)
{

	// ---- local
		INT result = 0;


	// ---- for each frame ....
		AppFrame *frame = pi_app_view->first_frame;
		while ( frame )
		{

			// ---- for each panel of current frame ....
			AppPanel* panel = frame->GetFirstPanel();
			while ( panel )
			{

				// ---- null name ?
				PanelContext *panel_context = panel->GetPanelContext();
				if ( name == NULL )
				{
					panel_context->GetInteractionStatus(status);
				}
				else
				{
					// ---- name matched ?
					CHAR panel_name[VS_MAXCHARNAME];
					result = panel_context->GetName(panel_name);
					if ( panel_name != NULL )
					{
						if ( !strcmp(name, panel_name) )
						{
							panel_context->GetInteractionStatus(status);
						}
					}
				}

				panel = panel->GetNext();
	
			}

			frame = frame->GetNext();

		}

	return VS_OK;
}


// ---------- SetDisplayListHandle ----------
/*!
\brief Set the handle of display list "name" 
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK

\note if handle is set == 0, then this will force
    re-initialisation of a display list...

\note invoked from 
*/
INT AppView::SetDisplayListHandle(CHAR *name, INT handle)
{

	// ---- local
		INT result = 0;


	// ---- for each frame ....
		AppFrame *frame = pi_app_view->first_frame;
		while ( frame )
		{

			// ---- for each panel of current frame ....
			AppPanel* panel = frame->GetFirstPanel();
			while ( panel )
			{

				// ---- null name ?
				PanelContext *panel_context = panel->GetPanelContext();
				if ( name == NULL )
				{
					panel_context->SetDisplayListHandle(0, handle);
				}
				else
				{
					// ---- name matched ?
					CHAR panel_name[VS_MAXCHARNAME];
					result = panel_context->GetName(panel_name);
					if ( panel_name != NULL )
					{
						if ( !strcmp(name, panel_name) )
						{
							panel_context->SetDisplayListHandle(0, handle);
						}
					}
				}
				panel = panel->GetNext();

			}

			frame = frame->GetNext();

		}

	return VS_OK;
}


// ---------- SetInteractionStatus ----------
/*!
\brief Set interaction status
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT AppView::SetInteractionStatus(CHAR *name, BOOL status)
{

	// ---- local ----
		INT result = 0;

	// ---- for each frame ....
		AppFrame *frame = pi_app_view->first_frame;
		while ( frame )
		{

			// ---- for each panel of current frame ....
			AppPanel* panel = frame->GetFirstPanel();
			while ( panel )
			{

				// ---- null name ?
				PanelContext *panel_context = panel->GetPanelContext();
				if ( name == NULL )
				{
					panel_context->SetInteractionStatus(status);
				}
				else
				{
					// ---- name matched ?
					CHAR panel_name[VS_MAXCHARNAME];
					result = panel_context->GetName(panel_name);
					if ( panel_name != NULL )
					{
						if ( !strcmp(name, panel_name) )
						{
							panel_context->SetInteractionStatus(status);
						}
					}
				}

				panel = panel->GetNext();

			}

			frame = frame->GetNext();

		}

	return VS_OK;
}


// ---------- SelectFrame ----------
/*!
\brief Select frame by name
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT AppView::SelectFrame(CHAR *name)
{

	if ( name == NULL ) return VS_ERROR;

	INT result = VS_ERROR;
	AppFrame *frame = pi_app_view->first_frame;
	while (frame)
	{
		CHAR *this_frame_name = frame->GetName();
		if ( this_frame_name != NULL )
		{
			if ( !strcmp(this_frame_name, name) )
			{
				pi_app_view->selected_frame = frame;
				frame->SetHome(TRUE);
				result = VS_OK;
			}
			else
			{
				frame->SetHome(FALSE);
			}
		}
		frame = frame->GetNext();
	}

	return result;
}


// ---------- SelectFrame ----------
/*!
\brief Select "n'th" frame by index
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT AppView::SelectFrame(AppFrame* frame)
{

	if (frame != NULL)
	{
		pi_app_view->selected_frame = frame;
		return VS_OK;
	}

	return VS_ERROR;
}


// ---------- SelectFrame ----------
/*!
\brief Select "n'th" frame by index
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT AppView::SelectFrame(UINT index)
{

	AppFrame *frame = pi_app_view->first_frame;
	UINT frame_count = 1;
	while ( frame )
	{
		if ( frame_count == index )
		{
			pi_app_view->selected_frame = frame;
			return VS_OK;
		}
		frame_count++;
		frame = frame->GetNext();
	}

	return VS_ERROR;
}


// ---------- SelectFirstFrame ----------
/*!
\brief Select first frame
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT AppView::SelectFirstFrame()
{
	pi_app_view->selected_frame = pi_app_view->first_frame;
	return VS_OK;
}


// ---------- SelectNextFrame ----------
/*!
\brief Select next frame
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT AppView::SelectNextFrame()
{

	AppFrame *next_frame = pi_app_view->selected_frame->GetNext();
	if ( next_frame != NULL ) 
		pi_app_view->selected_frame = next_frame;
	else
		pi_app_view->selected_frame = pi_app_view->first_frame;

	return VS_OK;
}


// ---------- SelectLastFrame ----------
/*!
\brief Select last frame
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT AppView::SelectLastFrame()
{

	AppFrame *last_frame = pi_app_view->selected_frame->GetLast();
	if ( last_frame != NULL ) 
		pi_app_view->selected_frame = last_frame;
	else
		pi_app_view->selected_frame = pi_app_view->last_frame;

	return VS_OK;
}


// ---------- SetPanelViewrects ----------
/*!
\brief Setup panel viewrects from an array of AppPanes
\author Gareth Edwards
*/
INT AppView::SetPanelViewrects(
		UINT number_panes,
		AppPane *panes
	)
{

	// ---- find and assign panel, then set panel viewrects
		//
		// note: if no match defaults to pane 0
		//
		CHAR name[VS_MAXCHARNAME];
		vs_system::AppFrame *frame = GetFirstFrame();
		while (frame)
		{
			vs_system::AppPanel *panel = frame->GetFirstPanel();
			while (panel)
			{
				// set rect
				vs_system::PanelContext *gfx_context = panel->GetPanelContext();
				VsFloatRectangle *vr = gfx_context->GetViewrect();
				*vr = panes[0].rect;
				INT result = gfx_context->GetName(name);
				for (UINT i = 0; i<number_panes; i++)
				{
					if (
						panes[i].name != 0 &&
								!strcmp(panes[i].name, name)
						)
					{
						*vr = panes[i].rect;
						break;
					}
				}
				panel = panel->GetNext();
			}
			frame = frame->GetNext();
		}

	return VS_OK;
}


// ---------- SelectPanel ----------
/*!
\brief Select panel
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT AppView::SelectPanel(AppPanel *selected_panel)
{

	// ---- local
		PanelContext *panel_context = NULL;


	// ---- get graphic context
		if ( selected_panel != NULL )
		{
			panel_context = selected_panel->GetPanelContext();
			panel_context->SetSelected(TRUE);
		}


	// ---- for every frame ....
		AppFrame *frame = pi_app_view->first_frame;
		while ( frame )
		{
			AppPanel *panel = frame->GetFirstPanel();
			while ( panel )
			{
				if ( panel != selected_panel )
				{
					panel_context = panel->GetPanelContext();
					panel_context->SetSelected(FALSE);
				}
				panel = panel->GetNext();
			}
			frame = frame->GetNext();
		}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
