////////////////////////////////////////////////////////////////////////////////

// ---------- r2_control.h ----------
/*!
\file r2_control.h
\brief Interface for the App(lication)Control class
\author Gareth Edwards
*/

#pragma once


// ---- vs include ----
#include "../header/vs_app_dependency.h"


// ---------- namespace ----------
namespace vs_system
{


	// ---------- framework class ----------
	class AppControl
	{

	public:
	 
		// ---- c/dtor ----
		AppControl();
		virtual ~AppControl();

		// ---- method`````````````````````````````````````````````````````````````````````s ---- 
		INT UpdateInput();

		// ---- Properties ----

		// ---- system ---- 
		vs_system::AppDependency *app_dependency;
		vs_system::SysContext    *sys_context;
		vs_system::SysInput      *sys_input;
		vs_system::SysOutput     *sys_output;

		// ---- callback ---- 
		INT(*handle_pick_panel)(INT, INT);
		INT   handle_pick_panel_control_obj;

		// ---- command ---- 
		UINT command_param_count;
		INT  command_param[100];

		// ---- cursor ---- 
		BOOL   delta_change;
		DOUBLE delta_time;
		POINT  delta;
		BOOL   dragging;
		BOOL   picking;

		// ---- events ---- 
		UINT   group_id;
		UINT   element_id;

		// ---- keyboard ---- 
		BOOL   shift_key;
		BOOL   control_key;
		INT    key_clicked;
		BOOL   key_down;

		// ---- mouse buttons ---- 
		INT    button_bit_mask;
		UINT   button_id[3];

		// ---- mouse wheel ---- 
		BOOL   wheel_change;
		LONG   wheel_clicks;

		// ---- runtime ---- 
		BOOL   runtime;

		// ---- system ---- 
		BOOL   quit;

		// ---- persistant PanelContext && Client Adjusted Viewport ---- 
		vs_system::PanelContext *persistant_panel_context;
		VsFloatRectangle persistant_cav;

	};


}
