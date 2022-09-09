////////////////////////////////////////////////////////////////////////////////

// ---------- vs_sys_keyboard.cpp ----------
/*!
\file vs_sys_keyboard.cpp
\brief Implementation of the Sys(tem)Keyboard class
\author Gareth Edwards
*/

// ---- include ----
#include "../header/vs_sys_keyboard.h"



// ---- namespace ----
using namespace vs_system;


// ---------- Private Implementation of PI_Props class ----------
class SysKeyboard::PI_SysKeyboard
{

public:

	// ---- cdtor
		PI_SysKeyboard();
		~PI_SysKeyboard();


	// --- update stuff
		INT SetupMouseAndKeybd(vs_system::AppDependency *);
		INT InputMouseAndKeybd();


	// ---- inject dependency pointers managed by VS
		vs_system::AppDependency *app_dependency;
		#ifdef VS_GFX_FLAG
		vs_system::GfxInterface  *gfx;
		vs_system::SysInput      *sys_input;
		#endif
		vs_system::SysContext    *sys_context;
		vs_system::SysOutput     *sys_output;


	// ---- callback 
		INT(*handle_pick_panel)(INT, INT);
		INT handle_pick_panel_control_obj;


	// ---- cursor 
		BOOL   delta_change = 0;
		DOUBLE delta_time   = 0;
		POINT  delta;
		BOOL   dragging     = 0;
		BOOL   picking      = 0;


	// ---- events 
		UINT   group_id    = 0;
		UINT   element_id  = 0;


	// ---- keyboard 
		BOOL   shift_key   = 0;
		BOOL   control_key = 0;
		INT    key_clicked = 0;
		BOOL   key_down    = 0;


	// ---- buffer 
		static CONST INT buffer_max = 128;
		INT    buffer_top = 0;
		CHAR   buffer[buffer_max];


	// ---- mouse buttons 
		INT    button_bit_mask = 0;
		UINT   button_id[3];


	// ---- mouse wheel 
		BOOL   wheel_change = 0;
		LONG   wheel_clicks = 0;


	// ---- runtime -- not used 
		BOOL   runtime = 0;


	// ---- system -- not used
		BOOL   quit = 0;


	// ---- persistant PanelContext && Client Adjusted Viewport 
		vs_system::PanelContext *persistant_panel_context = NULL;
		VsFloatRectangle persistant_cav;

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SysKeyboard::PI_SysKeyboard::PI_SysKeyboard()
{

	// ---- init injected depend
		app_dependency = NULL;
		#ifdef VS_GFX_FLAG
		sys_input      = NULL;
		gfx            = NULL;
		#endif
		sys_context    = NULL;
		sys_output     = NULL;



	// ---- init arrays and structs
		buffer[0] = 0;
		delta.x   = 0;
		delta.y   = 0;
		button_id[0] = 0;
		button_id[1] = 0;
		button_id[2] = 0;
		persistant_cav.left   = 0;
		persistant_cav.top    = 0;
		persistant_cav.bottom = 0;
		persistant_cav.right  = 0;
}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SysKeyboard::PI_SysKeyboard::~PI_SysKeyboard()
{
	;
}



// ---------- SetupMouseAndKeybd----------
/*!
\brief SetupMouseAndKeybd 
\author Gareth Edwards
\return INT - no update <= NULL < VS_OK
*/
INT SysKeyboard::PI_SysKeyboard::SetupMouseAndKeybd(vs_system::AppDependency *app_dep)
{

	app_dependency = app_dep;
	#ifdef VS_GFX_FLAG
	gfx            = app_dep->GetGfxInterface();
	sys_input      = app_dep->GetSysInput();
	#endif
	sys_context    = app_dep->GetSysContext();
	sys_output     = app_dep->GetSysOutput();

	return VS_OK;
}


// ---------- InputMouseAndKeybd----------
/*!
\brief Input from mouse and keyboard
\author Gareth Edwards
\return INT - no update <= NULL < VS_OK 
*/
INT SysKeyboard::PI_SysKeyboard::InputMouseAndKeybd()
{

	#ifdef VS_GFX_FLAG

	// ---- NOT active ?
		if ( !sys_context->GetBool(sys_context->ACTIVE) ) return 0;


	// ----- ACCELERATE
		BOOL touch = sys_context->GetBool(sys_context->TOUCH);
		BOOL log = sys_output->GetLog();


	// ----- MOUSE BUTTONS


	// ---- touch screen ?
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
	// ---- mouse ?
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


	// ----- MOUSE DELTA

		// delta (cursor moved) ?
		delta_change = sys_input->GetBool(sys_input->DELTA_CHANGE);
		if (delta_change)
		{
			// get delta
			INT result = sys_input->GetDelta(&delta);

			// reset
			sys_input->SetBool(sys_input->DELTA_CHANGE, FALSE);
		}


	// ----- INVOKE PANEL CALLBACK

		// ---- persistant panel gfx context ? -- invoke panel callback method
			if ( !picking && persistant_panel_context != NULL )
			{
				INT result = handle_pick_panel(handle_pick_panel_control_obj, 0);
			}

		// ---- wheel_change ? --invoke panel callback method
			else if ( wheel_change )
			{
				INT result = handle_pick_panel(handle_pick_panel_control_obj, 0);
			}


		// ---- touch mode ?
			//
			// Touch mode is NOT persistant within the scope of the current
			// VS architecture, so there CANNOT be a callback. 
			//
			// [L/M/R]BUTTON_DOWN events are consumed after a frame has
			// been rendered (see AppFrame::Display method).
			//
			else if (touch)
			{
				// nothing happens - and for a reason!!!
				;
			}


		// ---- NOT DRAGGING ?
			else
			{
				// ----- consume button events
					if ( !dragging )
					{
						sys_input->ResetMouse();
					}
			}


	// ----- CONSUME DRAGGING

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


	// ----- CONSUME KEYBOARD

		// ---- key down ?
			key_down = sys_input->GetBool(sys_input->KEY_DOWN);
			if ( key_down )
			{  

				// ---- valid ascii code ?
					key_clicked = sys_input->GetInt(sys_input->ASCII_CODE);
					if ( key_clicked != NULL )
					{

						// get time delta
							vs_system::SysContext*sys_context = app_dependency->GetSysContext();
							delta_time = sys_context->GetDouble(sys_context->TIME_MS_DELTA);

						// shift ?
							shift_key = sys_input->GetBool(sys_input->SHIFT_KEY);

						// control ?
							control_key = sys_input->GetBool(sys_input->CONTROL_KEY);

						// ---- report ?
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

				// ---- key down ? - NO
					else key_down = FALSE;

				// ---- reset
					sys_input->SetKeyboardChar(NULL, NULL);
					sys_input->SetBool(sys_input->SHIFT_KEY, NULL);
					sys_input->SetBool(sys_input->CONTROL_KEY, NULL);

				// ---- key up
					sys_input->SetBool(sys_input->KEY_UP, TRUE);
			}


	// ----- RETURN UPDATED STATUS
		//
		// IF update status is 0 then the control::HandleMouseInput method
		// and control::HandleKeyboardInput are not invoked, in which:
		// 
		//    "The following code block "binds" all mouse and keyboard
		//     events to specific callback functions....""
		//
		// So BAD, as input update callback functions are NOT invoked.
		//    ===
		//
		INT updated = 
			( button_bit_mask > NULL ? 1 : 0 ) + 
			( wheel_clicks != 0 ? 2 : 0 );
			// + 
			// ( delta_change ? 4 : 0 ) +
			// ( key_down ? 8 : 0);


	return updated;

	#else

	return VS_OK;

	#endif

}


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SysKeyboard::SysKeyboard()
{
	// allocate and construct private properties
	pi_sys_keyboard = new PI_SysKeyboard();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
SysKeyboard::~SysKeyboard()
{
	// delete private properties
	delete pi_sys_keyboard;
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT SysKeyboard::Setup(vs_system::AppDependency *app_dep)
{

	// ---- inject dependencies
		pi_sys_keyboard->SetupMouseAndKeybd(app_dep);

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
