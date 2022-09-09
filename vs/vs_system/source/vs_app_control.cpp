////////////////////////////////////////////////////////////////////////////////

// ---------- vs_app_control.cpp ----------
/*!
\file vs_app_control.cpp
\brief Implementation of the App(lication)Control class.
\author Gareth Edwards 
*/

// ---- include ----
#include "../header/vs_app_control.h"


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
AppControl::AppControl()
{

	// system
	app_dependency      = NULL;
	sys_context         = NULL;
	sys_input           = NULL;
	sys_output          = NULL;

	// callback
	handle_pick_panel = NULL;
	handle_pick_panel_control_obj = 0;

	// command
	command_param_count = 0;

	// cursor
	delta_change        = FALSE;
	delta_time          = 0;
	POINT p = { 0, 0 };
	delta               = p;
	dragging            = FALSE;
	picking             = FALSE;

	// events
	group_id            = 0;
	element_id          = 0;

	// keyboard
	shift_key           = FALSE;
	control_key         = FALSE;
	key_clicked         = 0;
	key_down            = 0;

	// mouse buttons
	button_bit_mask     = 0;
	button_id[0]        = vs_system::SysInput::LBUTTON_DOWN;
	button_id[1]        = vs_system::SysInput::MBUTTON_DOWN;
	button_id[2]        = vs_system::SysInput::RBUTTON_DOWN;

	// mouse wheel
	wheel_change        = FALSE;
	wheel_clicks        = 0;

	// runtime
	runtime             = TRUE;

	// system
	quit                = FALSE;

	// persistant panel
	persistant_panel_context = NULL;

	// Client Adjusted Viewport
	VsFloatRectangle temp = { 0, 0, 0, 0 };
	persistant_cav = temp;
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
AppControl::~AppControl()
{
	;
}


// ---------- UpdateInput----------
/*!
\brief Update input from mouse and keyboard
\author Gareth Edwards
\return INT - no update <= NULL < VS_OK 
*/
INT AppControl::UpdateInput()
{

	// IF -- NOT -- active -- THEN -- return
	if (!sys_context->GetBool(sys_context->ACTIVE)) return 0;


	// ----- ACCELERATE -----
	BOOL touch = sys_context->GetBool(sys_context->TOUCH);
	BOOL log = sys_output->GetLog();


	// ----- MOUSE BUTTONS ------

	// IF -- touch screen -- THEN
	if ( touch )
	{
		// left button down?
		button_bit_mask = 1;

		// always picking
		picking = TRUE;

		// never dragging
		dragging = FALSE;

		// set wheel parameters
		wheel_change = FALSE;
		wheel_clicks = 0;

		// set wheel parameters (for Miles)
		wheel_change = sys_input->GetBool(sys_input->WHEEL_CHANGE);
		wheel_clicks = sys_input->GetLong(sys_input->WHEEL_CLICKS);

	}
	// ELSE -- normal mouse
	else
	{

		// button(s) down ?
		button_bit_mask = sys_input->GetMouseButtonBitMask();
		if ( button_bit_mask > 0 )
		{
			// IF -- NOT picking -- THEN -- start dragging?
			if ( !picking )
				dragging = button_bit_mask & sys_input->LBUTTON ? TRUE : FALSE;
		}

		// set wheel parameters
		wheel_change = sys_input->GetBool(sys_input->WHEEL_CHANGE);
		wheel_clicks = sys_input->GetLong(sys_input->WHEEL_CLICKS);

	}


	// ----- MOUSE DELTA ------

	// IF -- delta (cursor moved) -- THEN
	delta_change = sys_input->GetBool(sys_input->DELTA_CHANGE);
	if ( delta_change )
	{
		// get delta
		INT result = sys_input->GetDelta(&delta);

		// reset
		sys_input->SetBool(sys_input->DELTA_CHANGE, FALSE);
	}


	// ----- INVOKE PANEL CALLBACK ------

	// IF -- persistant panel gfx context -- THEN -- invoke panel callback method
	if ( !picking && persistant_panel_context != NULL )
	{
		INT result = handle_pick_panel(handle_pick_panel_control_obj, 0);
	}

	// ELSE IF -- wheel_change -- THEN -- invoke panel callback method
	else if  ( wheel_change )
	{
		INT result = handle_pick_panel(handle_pick_panel_control_obj, 0);
	}

	// ELSE -- if touch mode
	//
	// Touch mode is NOT persistant within the scope of the current
	// VS architecture, so there CANNOT be a callback. 
	//
	// [L/M/R]BUTTON_DOWN events are consumed after a frame has
	// been rendered (see AppFrame::Display method).
	//
	else if ( touch )
	{
		// Nothing happens - and for a reason!!!
		;
	}

	// ELSE IF NOT DRAGGING THEN
	else
	{
		// ----- CONSUME BUTTON EVENTS -----
		if ( !dragging )
		{
			sys_input->ResetMouse();
		}
	}


	// ----- CONSUME DRAGGING ------

	// IF NOT picking AND dragging AND left button up THEN STOP dragging 
	if ( !picking && dragging )
	{
		if (sys_input->GetBool(sys_input->LBUTTON_UP))
		{
			dragging = FALSE;
			persistant_panel_context = NULL;
		}
	}
	else
	{
		persistant_panel_context = NULL;
	}


	// ----- CONSUME KEYBOARD ------

	//#define SPEC_KBD

	#if defined (SPEC_KBD)

	;

	#else

	// key down ?
	key_down = sys_input->GetBool(sys_input->KEY_DOWN);
	if ( key_down )
	{  
		// valid ascii code ?
		key_clicked = sys_input->GetInt(sys_input->ASCII_CODE);
		if ( key_clicked != NULL )
		{
			// get time delta
			printf ("Get time delta\n");
			vs_system::SysContext*sys_context = app_dependency->GetSysContext();
			delta_time = sys_context->GetDouble(sys_context->TIME_MS_DELTA);

			// shift ?
			shift_key = sys_input->GetBool(sys_input->SHIFT_KEY);

			// control ?
			control_key = sys_input->GetBool(sys_input->CONTROL_KEY);

			// report
			if ( log )
			{
				char text[VS_MAXCHARLEN];
				sprintf (text, "Key pressed is (%c%c) %c with delta %f",
					shift_key ? 'S' : '_',
					control_key ? 'C' : '_',
					key_clicked,
					delta_time 
					);
				vs_system::SysOutput *sys_output = app_dependency->GetSysOutput();
				sys_output->Message(text);
			}
		}
		// ELSE -- key down
		else  key_down = FALSE;

		// reset
		sys_input->SetKeyboardChar(NULL, NULL);
		sys_input->SetBool(sys_input->SHIFT_KEY, NULL);
		sys_input->SetBool(sys_input->CONTROL_KEY, NULL);

		// key up
		sys_input->SetBool(sys_input->KEY_UP, TRUE);
	}

	#endif


	// ----- RETURN UPDATED STATUS -----
	//
	// IF update status is 0 then the control::HandleMouseInput method
	// and control::HandleKeyboardInput are not invoked, in which:
	// 
	//    "The following code block "binds" all mouse and keyboard
	//     events to specific callback functions....""
	//
	// So BAD, as input update callback functions are NOT invoked.
	//    ===
	INT updated = 
		( button_bit_mask > NULL ? 1 : 0 ) + 
		( wheel_clicks != 0 ? 2 : 0 );
		// + 
		// ( delta_change ? 4 : 0 ) +
		// ( key_down ? 8 : 0);

	return updated;
}


////////////////////////////////////////////////////////////////////////////////
