////////////////////////////////////////////////////////////////////////////////

// ---------- vs_sys_input.h ----------
/*!
\file vs_sys_input.h
\brief Interface for the Sys(tem)Context class.
\author Gareth Edwards
*/

#pragma once

// ---- vs include ----
#include "../header/vs_inc_defs.h"


// ---------- namespace ----------
namespace vs_system
{


	// ---------- SysInput ----------
	/*!
	\brief Interface to the SysInput class.
	\author Gareth Edwards
	*/
	class SysInput
	{

	public:

		// Enumerated id's
		enum SysContextID
		{
			// MOUSE bitmask values
			LBUTTON           = 1,  // Left button
			MBUTTON           = 2,  // Middle button
			RBUTTON           = 4,  // Right button
			WBUTTON           = 8,  // Wheel clicks
			DBUTTON           = 16, // Delta update

			// BOOL
			MOUSE_BUTTON_DOWN = 20,

			RBUTTON_DOWN      = 21,
			RBUTTON_DBLCLK    = 22,
			RBUTTON_UP        = 23,

			LBUTTON_DOWN      = 24,
			LBUTTON_DBLCLK    = 25,
			LBUTTON_UP        = 26,

			MBUTTON_DOWN      = 27,
			MBUTTON_DBLCLK    = 28,
			MBUTTON_UP        = 29,

			MOUSE_BM          = 31, // Mouse bitmap 1 | 2 | 4
			MOUSE_BMO         = 32, // Mouse bitmap overlay

			DELTA_CHANGE      = 38,

			SHIFT_KEY         = 41,
			CONTROL_KEY       = 42,
			KEY_DOWN          = 43,
			KEY_UP            = 44,

			WHEEL_CHANGE      = 45,

			// INT
			ASCII_CODE        = 61,
			ASCII_CODE_PREV   = 62,
			KEY_STATE         = 63,

			// LONG
			XPOSITION         = 71,
			YPOSITION         = 72,
			XDELTA            = 73,
			YDELTA            = 74,
			XLAST             = 75,
			YLAST             = 76,
			WHEEL_CLICKS      = 77
		};

	public:

		// Constructor/destructor
		SysInput();
		~SysInput();

		// Get
		BOOL   GetBool(UINT);
		UINT   GetMouseButtonBitMask();
		INT    GetInt(UINT);
		LONG   GetLong(UINT);
		POINT *GetPosition();
		INT    GetDelta(POINT *);
		POINT *GetLast();

		// Reset
		INT ResetKeyboard();
		INT ResetMouse();

		// Set
		INT SetBool(UINT, BOOL);
		INT SetInt(UINT, INT);
		INT SetLong(UINT, LONG);

		INT SetKeyboardChar(INT, INT);
		INT SetMouseButtonBitMask(INT, INT);
		INT SetClientCursorPosition(POINT *);

	private:

		// Private implementation
		class PI_SysInput; PI_SysInput *pi_sys_input;

	};


}