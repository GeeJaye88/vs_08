////////////////////////////////////////////////////////////////////////////////

// ---------- r2_control_ctor.hpp ----------
/*!
\file r2_control_ctor.hpp
\brief 
\author Gareth Edwards
*/


#include "../header/r2_model.h"


using namespace review_02;


////////////////////////////////////////////////////////////////////////////////


// ---------- Setup ----------
/*!
\brief Setup controls
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
\note DO NOT ALTER THIS CODE
*/

INT Control::Setup(
		vs_system::AppDependency *ad,
		vs_system::AppView *w,
		review_02::Model *m
	)
{

	// ---- store
		app_dependency = ad;
		app_view = w;
		model = m;


	// ---- ... & initialise
		app_control->app_dependency = ad;
		app_control->sys_context    = ad->GetSysContext();
		app_control->sys_input      = ad->GetSysInput();
		app_control->sys_output     = ad->GetSysOutput();
		app_control->handle_pick_panel = UpdatePanel;
		app_control->handle_pick_panel_control_obj = (INT)this;


	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup.
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note DO NOT ALTER THIS CODE
*/
INT Control::Cleanup()
{
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ========== APPLICATION CALLBACK ==========


// ---------- SetCallbackToSetupGraphics ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
/*
INT Control::SetCallbackToSetupGraphics(
		INT(*callback_to_setup_graphics)()
	)
{
	pi_application->callback_to_setup_graphics = callback_to_setup_graphics;
	return VS_OK;
}
*/


// ---------- SetCallbackToCleanupGraphics ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
/*
INT Control::SetCallbackToCleanupGraphics(
		INT(*callback_to_cleanup_graphics)()
	)
{
	pi_application->callback_to_cleanup_graphics = callback_to_cleanup_graphics;
	return VS_OK;
}
*/



////////////////////////////////////////////////////////////////////////////////


