////////////////////////////////////////////////////////////////////////////////

// ---------- app_panel.cpp ----------
/*!
\file app_panel.cpp
\brief Implementation of the App(lication)Panel class
\author Gareth Edwards 
*/

// ---- Use preprocessor directive to turn OFF MS warnings
#define _CRT_SECURE_NO_WARNINGS


// ---- include
#include "../header/vs_app_panel.h"
#include "../header/vs_pan_context.h"


// ---- namespace
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_AppPanel class ----------

class AppPanel::PI_AppPanel
{
public:
	PI_AppPanel() {
		app_dependency = NULL;
		index    = -1;
		next     = NULL;
		original = NULL;
	};
	~PI_AppPanel() { };
	AppDependency *app_dependency;
	INT            index;
	AppPanel      *next;
	AppPanel      *original;
	PanelContext   context;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
AppPanel::AppPanel()
{
	pi_app_panel = new PI_AppPanel;
}


// ---------- destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
AppPanel::~AppPanel()
{
	if ( pi_app_panel != NULL )
	{
		delete pi_app_panel; pi_app_panel = NULL;
	}
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT AppPanel::Cleanup()
{
	return VS_OK;
}


// ---------- Display ----------
/*!
\brief Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT AppPanel::Display(
		INT handle_app_obj,
		INT (*app_callback_display)(INT, AppPanel *)
	)
{
	
	// ---- local
		INT result = 0;
		vs_system::PanelContext *panel_context = GetPanelContext();


	// ---- display ?
		//
		// BOOL display = TRUE;
		// result = panel_context->GetDisplayStatus(&display);
		// if ( !display ) return VS_ERROR;
		//
		// To be handled by application, as the above will
		// prevent initialisation of panel ref arrays


	// --- # display lists
		UINT display_list_count = 0;
		result = panel_context->GetNumberOfDisplayLists(&display_list_count);


	// ---- for each display list
		for (UINT display_list_index = 0;
				display_list_index < display_list_count;
					display_list_index++
			)
		{

			// ---- set index - to get across the vs | app barrier
				//
				// example Get:
				//    UINT handle_display_index;
				//    result = panel_context->GetDisplayListIndex(&handle_display_index);
				//
				panel_context->SetDisplayListIndex(display_list_index);
				 

			// ---- display ( e.g. application::CallbackDisplayFromView )
				//
				// note: app_callback_display typically invokes model->Display method
				//
				// example application->model->Display:
				//    vs_system::PanelContext *panel_context = panel->GetPanelContext();
				//    review_02::Model *model = pi_props->model;
				//    INT result = pi_props->model->Display(panel_context, NULL);
				//
				int g = (*app_callback_display)(
							handle_app_obj, 
							this // AppPanel *
						);

		}

	return VS_OK;
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT AppPanel::Setup(AppDependency *ad)
{
	pi_app_panel->app_dependency = ad;
	return VS_OK;
}


// ---------- Configure ----------
/*!
\brief Configure frame
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT AppPanel::Configure(VNode *panel_hdl)
{

	PanelContext *panel_context = &pi_app_panel->context;
	panel_context->Configure(panel_hdl);
	panel_context->SetHandle((INT)this);

	return VS_OK;
}


// ---------- GetPanelContext ----------
/*!
\brief Get panel gfx context
\author Gareth Edwards
*/
PanelContext* AppPanel::GetPanelContext()
{
	return &pi_app_panel->context;
}


// ---------- GetIndex ----------
/*!
\brief Get panel runtime index
\author Gareth Edwards
*/
INT AppPanel::GetIndex()
{
	return pi_app_panel->index;
}


// ---------- GetNext ----------
/*!
\brief Get next panel
\author Gareth Edwards
*/
AppPanel* AppPanel::GetNext()
{
	return pi_app_panel->next;
}


// ---------- GetMaster ----------
/*!
\brief Get original panel
\author Gareth Edwards
*/
AppPanel* AppPanel::GetMaster()
{
	return pi_app_panel->original;
}


// ---------- SetIndex ----------
/*!
\brief Get runtime index
\author Gareth Edwards
*/
INT AppPanel::SetIndex(INT i)
{
	pi_app_panel->index = i;
	return i < 0 ? VS_ERROR : VS_OK;
}


// ---------- SetNext ----------
/*!
\brief Get next panel
\author Gareth Edwards
*/
INT AppPanel::SetNext(AppPanel *n)
{
	pi_app_panel->next = n;
	return n == NULL ? VS_ERROR : VS_OK;
}


// ---------- SetMaster ----------
/*
\brief Set original panel
\author Gareth Edwards
*/
INT AppPanel::SetMaster(AppPanel *original)
{
	pi_app_panel->original = original;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
