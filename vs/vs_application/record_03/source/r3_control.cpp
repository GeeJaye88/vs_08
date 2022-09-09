////////////////////////////////////////////////////////////////////////////////

// ---------- r3_control.cpp ----------
/*!
\file r3_control.cpp
\brief Implementation of the Control class.
\author Gareth Edwards 
\note Vanilla C++
*/

// ---------- include ----------
#include "../header/r3_control.h"


// ---------- namespace ----------
using namespace record_03;


////////////////////////////////////////////////////////////////////////////////
//
//
// Include "DO NOT ALTER" Control object implementation
// 
#include "../../../vs_system/hpp/vs_app_control_implementation.hpp"
//
////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Camera class ----------

class Control::PI_Camera
{

public:

	// Constructor/destructor
	PI_Camera()
	{
		live = FALSE;
		save = FALSE;
	};
	~PI_Camera(){ };

	// Properties
	BOOL  live;
	BOOL  save;
};


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Timeline class ----------

class Control::PI_Timeline
{

public:

	// Constructor/destructor
	PI_Timeline()
	{
		timeline_command   = FALSE;
		timeline_live_mode = FALSE;
		timeline_play_mode = FALSE;
		timeline_sequence_mode = FALSE;
	};
	~PI_Timeline(){ };

	UINT timeline_command;
	BOOL timeline_live_mode;
	BOOL timeline_play_mode;
	BOOL timeline_sequence_mode;
};


////////////////////////////////////////////////////////////////////////////////


// ========== ALLOCATE & INITIALISE ==========


// ---------- Construct ----------
/*!
\brief Construct - invoked by constructor
\author Gareth Edwards
*/
INT Control::Construct()
{
	pi_camera = new PI_Camera();
	return VS_OK;
}


// ---------- Destruct ----------
/*!
\brief Destruct - invoked by cestructor
\author Gareth Edwards
*/
INT Control::Destruct()
{
	delete pi_camera;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ========== Framework =========


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
	record_03::Model *m
	)
{
	// Store
	app_dependency = ad;
	app_view = w;
	model = m;

	// ... & Initialise
	app_control->app_dependency = ad;
	app_control->sys_context    = ad->GetSysContext();
	app_control->sys_input      = ad->GetSysInput();
	app_control->sys_output     = ad->GetSysOutput();
	app_control->pick_panel_callback = UpdatePanel;
	app_control->pick_panel_control_cast_to_int = (INT)this;

	// Done
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
	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ========== CAMERA ==========


// ---------- GetCameraLive ----------
/*!
\brief
\author Gareth Edwards
\return BOOL - TRUE or FALSE
*/
BOOL Control::GetCameraLive()
{
	return pi_camera->live;
}


// ---------- GetCameraArchive ----------
/*!
\brief
\author Gareth Edwards
\return BOOL - TRUE or FALSE
*/
BOOL Control::GetCameraArchive()
{
	return pi_camera->save;
}


// ---------- SetCameraLive ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT Control::SetCameraLive(BOOL live)
{
	pi_camera->live = live;
	return VS_OK;
}


// ---------- SetCameraArchive ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT Control::SetCameraArchive(BOOL save)
{
	pi_camera->save = save;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ========== PANEL ==========


// ---------- UpdatePanel ----------
/*!
\brief Update panel
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Control::UpdatePanel(INT this_cast_to_int, INT panel_cast_to_int)
{

	// ---- DONE ----
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ========== ELEMENT ==========


// ---------- UpdateElement ----------
/*!
\brief Update picked element
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Control::UpdateElement(INT this_cast_to_int, INT element_handle)
{
	// This instance
	Control* control = (Control*)this_cast_to_int;

	// PIMP's
	PI_Camera   *pi_camera   = control->pi_camera;

	// Accelerate
	vs_system::AppControl    *app_control    = control->app_control;
	vs_system::AppDependency *app_dependency = control->app_dependency;
	vs_system::GfxInterface  *gfx_interface  = app_dependency->GetGfxInterface();
	vs_system::SysOutput     *sys_output     = app_dependency->GetSysOutput();
	vs_system::SysInput      *sys_input      = app_dependency->GetSysInput();

	// Command parameters
	app_control->command_param_count = 0;

	// Element Id'd
	app_control->group_id   = gfx_interface->GetElementGroupId(element_handle);
	app_control->element_id = gfx_interface->GetElementId(element_handle);

	// Reset
	BOOL reset_ids = TRUE;
	INT  result = VS_OK;

	// Select event
	switch ( app_control->group_id )
	{
		case TITLE_BAR_GROUP:
			{
				switch (app_control->element_id)
				{
					case TITLE_BAR_ABOUT:
						break;
					default:
						break;
				}
			}
			break;

		case CAM_REPORT_GROUP:
			{
				switch (app_control->element_id)
				{
					case CAM_REPORT_RECORD:
						pi_camera->save = pi_camera->save == TRUE ? FALSE : TRUE;
						break;
					case CAM_REPORT_EXIT:
						PostQuitMessage(0);
						break;
					default:
						break;
				}
			}
			break;

		case ABOUT_BUTTON_GROUP_ID:
			switch ( app_control->element_id )
			{
				case ABOUT_CLOSE:
				case ABOUT_VIEW:
					{
						result = control->model->SetAboutDisplayStatus();
						if (result == VS_OK)
						{
							control->app_view->SetInteractionStatus(NULL, FALSE);
							control->app_view->SetInteractionStatus("About", TRUE);
						}
						else
						{
							control->app_view->SetInteractionStatus(NULL, TRUE);
						}
					}
					break;
				default:
					result = VS_ERROR;
					break;
			}
			break;

		case vs_system::ObjConfig::GROUP_PANO_CONFIG:                                
			{
				;
			}
			break;

		default:
			reset_ids = FALSE;
			break;
	}

	// Reset id's
	if ( reset_ids )
	{
		app_control->group_id   = 0;
		app_control->element_id = 0;
	}

	// Reset mouse
	sys_input->ResetMouse();

	// Done
	return result;
}


////////////////////////////////////////////////////////////////////////////////
