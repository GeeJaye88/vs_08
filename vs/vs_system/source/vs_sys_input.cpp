////////////////////////////////////////////////////////////////////////////////

// ---------- vs_sys_input.cpp ----------
/*!
\file vs_sys_input.cpp
\brief Implementation of the Sys(tem)Input class
\author Gareth Edwards
*/

// ---- include ----
#include "../header/vs_sys_input.h"
#include "stdio.h"


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_AppPanel class ----------

class SysInput::PI_SysInput
{

public:

	PI_SysInput()
	{

		// ---- BOOL - note: SetBool correctly sets sibling states
			mouse_button_down = FALSE;
			right_button_up  = SetBool(RBUTTON_UP, true);
			middle_button_up = SetBool(MBUTTON_UP, true);
			left_button_up   = SetBool(LBUTTON_UP, true);
			key_up           = SetBool(KEY_UP,     true);
			wheel_change     = false;
			delta_change     = false;
			shift_key        = false;
			control_key      = false;


		// ---- DOUBLE
			time_now   = 0;
			time_last  = 0;


		// ---- INT
			ascii_code = ascii_code_prev = 0;
			mouse_bm   = 0;
			mouse_bmo  = LBUTTON | MBUTTON | RBUTTON | WBUTTON | DBUTTON;
			key_state  = 0;


		// ---- LONG
			wheel_clicks = 0;


		// ---- Point
			POINT p = { 0, 0 };
			client_position = p;
			client_last     = p;
			client_delta    = p;

	};


	~PI_SysInput()
	{
		;
	};

	// ---- get
		BOOL GetBool(UINT);
		INT  GetInt(UINT);
		LONG GetLong(UINT);

	// ---- set
		INT  SetBool(UINT, BOOL);
		INT  SetInt(UINT, INT);
		INT  SetLong(UINT, LONG);
		INT  SetKeyboardChar(INT, INT);
		INT  SetClientCursorPosition(POINT *);

	// ---- BOOL: mouse events
		BOOL mouse_button_down;
		BOOL right_button_down;
		BOOL right_button_dbclk;
		BOOL right_button_up;
		BOOL middle_button_down;
		BOOL middle_button_dbclk;
		BOOL middle_button_up;
		BOOL left_button_down;
		BOOL left_button_dbclk;
		BOOL left_button_up;

	// ---- BOOL: mouse wheel
		BOOL wheel_change;

	// ---- BOOL: mouse delta non - zero
		BOOL delta_change;

	// ---- BOOL: keyboard
		BOOL shift_key;
		BOOL control_key;
		BOOL key_down;
		BOOL key_up;

	// ---- DOUBLE
		DOUBLE time_now;
		DOUBLE time_last;

	// ---- INT: Key
		INT ascii_code;
		INT ascii_code_prev;
		INT mouse_bm;
		INT mouse_bmo;
		INT key_state;

	// ---- LONG
		LONG wheel_clicks;

	// ---- POINT ----
		POINT client_position;
		POINT client_last;
		POINT client_delta;
};


// ---------- GetBool ----------
/*!
\brief Get BOOL value
\author Gareth Edwards
\param UINT - enumerated id
\return BOOL - IF id found THEN value ELSE FALSE
*/
BOOL SysInput::PI_SysInput::GetBool(UINT id)
{
	switch ( id )
	{
		case MOUSE_BUTTON_DOWN : return mouse_button_down;
		case RBUTTON_DOWN      : return right_button_down;
		case RBUTTON_DBLCLK    : return right_button_dbclk;
		case RBUTTON_UP        : return right_button_up;
		case MBUTTON_DOWN      : return middle_button_down;
		case MBUTTON_DBLCLK    : return middle_button_dbclk;
		case MBUTTON_UP        : return middle_button_up;
		case LBUTTON_DOWN      : return left_button_down;
		case LBUTTON_DBLCLK    : return left_button_dbclk;
		case LBUTTON_UP        : return left_button_up;
		case DELTA_CHANGE      : return delta_change;
		case SHIFT_KEY         : return shift_key;
		case CONTROL_KEY       : return control_key;
		case KEY_DOWN          : return key_down;
		case KEY_UP            : return key_up;
		case WHEEL_CHANGE      : return wheel_change;
		default                : return false;
	}
}


// ---------- GetInt ----------
/*!
\brief Get INT
\author Gareth Edwards
\param UINT - enumerated id
\return INT - id found THEN value ELSE VS_ERROR
*/
INT SysInput::PI_SysInput::GetInt(UINT id)
{
	switch ( id )
	{
		case ASCII_CODE      : return ascii_code;
		case ASCII_CODE_PREV : return ascii_code_prev;
		case MOUSE_BM        : 
		{
			mouse_bm =
				( left_button_down   ? LBUTTON : 0) |
				( middle_button_down ? MBUTTON : 0) |
				( right_button_down  ? RBUTTON : 0)
				;
			return mouse_bm;
		}
		break;
		case MOUSE_BMO       : return mouse_bmo;
		case KEY_STATE       : return key_state;
		default              : return VS_ERROR;
	}
}


// ---------- GetLong ----------
/*!
\brief Get LONG
\author Gareth Edwards
\param UINT - enumerated id
\return DWORD - id found THEN value ELSE VS_ERROR
*/
LONG SysInput::PI_SysInput::GetLong(UINT id)
{
	switch ( id )
	{
		case XPOSITION    : return client_position.x;
		case YPOSITION    : return client_position.y;
		case XDELTA       : return client_delta.x;
		case YDELTA       : return client_delta.y;
		case XLAST        : return client_last.x;
		case YLAST        : return client_last.y;
		case WHEEL_CLICKS : return wheel_clicks;
		default           : return (LONG)VS_ERROR;
	}
}


// ---------- SetBool ----------
/*!
\brief Set BOOL value
\author Gareth Edwards
\param UINT - enumerated id
\param BOOL - value
\return INT - IF id not found THEN VS_ERROR ELSE VS_OK
*/
INT SysInput::PI_SysInput::SetBool(UINT id, BOOL value)
{

	// note: mouse_bm NOT updated except when GetInt - as per:
	//    if ( mouse_bm ^ RBUTTON ) mouse_bm = mouse_bm | RBUTTON;
	//
	
	// ---- mouse buttons ----
	switch ( id )
	{
		case MOUSE_BUTTON_DOWN:
			{
				mouse_button_down = value;
			}
			return VS_OK;
		case RBUTTON_DOWN : 
			{
				if ( mouse_bmo & RBUTTON )
				{
					right_button_down   = value;
					right_button_dbclk  = false;
					right_button_up     = false;
					if ( value ) mouse_button_down = TRUE;
				}
			}
			return VS_OK;
		case RBUTTON_DBLCLK :
			{
				if ( mouse_bmo & RBUTTON )
				{
					right_button_down   = false;
					right_button_dbclk  = value;
					right_button_up     = false;
				}
			}
			return VS_OK;
		case RBUTTON_UP :
			{
				if ( mouse_bmo & RBUTTON )
				{
					right_button_down   = false;
					right_button_dbclk  = false;
					right_button_up     = value;
					if ( !value ) mouse_button_down = FALSE;
				}
			}
			return VS_OK;
		case MBUTTON_DOWN :
			{
				if ( mouse_bmo & MBUTTON )
				{
					middle_button_down  = value;
					middle_button_dbclk = false;
					middle_button_up    = false;
					if ( value ) {
						mouse_button_down = TRUE;
						wheel_change = TRUE;
					}
				}
			}
			return VS_OK;
		case MBUTTON_DBLCLK :
			{
				if ( mouse_bmo & MBUTTON )
				{
					middle_button_down  = false;
					middle_button_dbclk = value;
					middle_button_up    = false;
					if ( value ) {
						mouse_button_down = TRUE;
						wheel_change = TRUE;
					}
				}
			}
			return VS_OK;
		case MBUTTON_UP :
			{
				if ( mouse_bmo & MBUTTON )
				{
					middle_button_down  = false;
					middle_button_dbclk = false;
					middle_button_up    = value;
					if ( value ) {
						mouse_button_down = FALSE;
						wheel_change = FALSE;
					}
				}
			}
			return VS_OK;
		case LBUTTON_DOWN :
			{
				if ( mouse_bmo & LBUTTON )
				{
					left_button_down    = value;
					left_button_dbclk   = false;
					left_button_up      = false;
					if ( value ) mouse_button_down = TRUE;
				}
			}
			return VS_OK;
		case LBUTTON_DBLCLK :
			{
				if ( mouse_bmo & LBUTTON )
				{
					left_button_down    = false;
					left_button_dbclk   = value;
					left_button_up      = false;
				}
			}
			return VS_OK;
		case LBUTTON_UP :
			{
				if ( mouse_bmo & LBUTTON )
				{
					left_button_down    = false;
					left_button_dbclk   = false;
					left_button_up      = value;
					if ( !value ) mouse_button_down = TRUE;
				}
			}
			return VS_OK;
		case DELTA_CHANGE :
			{
				delta_change = value;
			}
			return VS_OK;
		case SHIFT_KEY :
			{
				shift_key = value;
			}
			return VS_OK;
		case CONTROL_KEY :
			{
				control_key = value;
			}
			return VS_OK;
		case KEY_DOWN :
			{
				key_down = value;
				key_up = false;
			}
			return VS_OK;
		case KEY_UP :
			{
				key_down = false;
				key_up = value;
			}
			return VS_OK;
		case WHEEL_CHANGE :
			{
				wheel_change = value;
				if (!wheel_change ) wheel_clicks = 0;
 			}
			return VS_OK;
		default: return VS_ERROR;
	}

	return VS_ERROR;
}


// ---------- SetInt ----------
/*!
\brief Set INT value
\author Gareth Edwards
\param UINT - enumerated id
\param INT - value
\return INT - IF id not found THEN VS_ERROR ELSE VS_OK
*/
INT SysInput::PI_SysInput::SetInt(UINT id, INT value)
{
	switch ( id )
	{
		case ASCII_CODE      : { ascii_code      = value; return VS_OK; }
		case ASCII_CODE_PREV : { ascii_code_prev = value; return VS_OK; }
		case MOUSE_BM        : { mouse_bm        = value; return VS_OK; }
		case MOUSE_BMO       : { mouse_bmo       = value; return VS_OK; }
		default:
			return VS_ERROR;
	}

	return VS_ERROR;
}


// ---------- SetLong ----------
/*!
\brief Set LONG value
\author Gareth Edwards
\param UINT - enumerated id
\param DWORD - value
\return INT - IF id not found THEN VS_ERROR ELSE VS_OK
*/
INT SysInput::PI_SysInput::SetLong(UINT id, LONG value)
{
	switch ( id )
	{
		case WHEEL_CLICKS :
			{
				wheel_change = TRUE;
				wheel_clicks =
					value < 0 ?
					-1 :
					( value == 0 ? 0 : 1);
			}
			return VS_OK;
		default: return VS_ERROR;
	}

	return VS_ERROR;
}


// ---------- SetClientCursorPosition ----------
/*!
\brief Update client cursor position
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT SysInput::PI_SysInput::SetClientCursorPosition(POINT *p)
{

	// IF -- no position chnage -- THEN -- return
	if (p->x == client_last.x || p->y == client_last.y) return VS_ERROR;


	// ---- update ----
	client_position.x = p->x;
	client_position.y = p->y;
	client_delta.x = client_position.x - client_last.x;
	client_delta.y = client_position.y - client_last.y;
	client_last.x  = client_position.x;
	client_last.y  = client_position.y;


	// ---- delta change?? ----
	delta_change = false;
	if ( abs(client_delta.x)>0 && abs(client_delta.x)<50 )
	{
		if ( abs(client_delta.y)>0 && abs(client_delta.y)<50 )
		{
			delta_change = true;
		}
	}

	return VS_OK;
}


// ---------- SetKeyboardChar ----------
/*!
\brief Update keyboard character
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT SysInput::PI_SysInput::SetKeyboardChar(INT ac, INT ks)
{

	if ( ac != NULL ) ascii_code_prev = ascii_code;

	ascii_code = ac;
	key_state = ks;

	return VS_OK;
}



////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
SysInput::SysInput()
{
	pi_sys_input = new PI_SysInput;
}


// ---------- Destructor ----------
SysInput::~SysInput()
{
	delete pi_sys_input;
	pi_sys_input = NULL;
}


// ---------- GetBool ----------
/*!
\brief Facade for PI_SysInput GetBool
*/
BOOL SysInput::GetBool(UINT id)
{
	return pi_sys_input->GetBool(id);
}


// ---------- GetMouseButtonBitMask ----------
/*!
\brief
*/
UINT SysInput::GetMouseButtonBitMask()
{
	return
		( pi_sys_input->GetBool(LBUTTON_DOWN) ? LBUTTON : 0 ) | 
		( pi_sys_input->GetBool(MBUTTON_DOWN) ? MBUTTON : 0 ) ;

		// TBD - To be fixed for applications invoking this method
		// 
		// So: Need to be able to swap between left and right for
		// different handedness, etc., and also to switch on and
		// off the various buttonms.
		//
		//( pi_sys_input->GetBool(RBUTTON_DOWN) ? RBUTTON : 0 );
}


// ---------- GetInt ----------
/*!
\brief Facade for PI_SysInput GetInt
*/
INT SysInput::GetInt(UINT id)
{
	return pi_sys_input->GetInt(id);
}


// ---------- GetLong ----------
/*!
\brief Facade for PI_SysInput GetLong
*/
LONG SysInput::GetLong(UINT id)
{
	return pi_sys_input->GetLong(id);
}


// ---------- GetPosition ----------
/*!
\brief Get cursor position
\author Gareth Edwards
\return POINT * - of cursor position
*/
POINT *SysInput::GetPosition()
{
	return &pi_sys_input->client_position;
}


// ---------- GetDelta ----------
/*!
\brief Get cursor delta
\author Gareth Edwards
\param POINT * client cursor delta
\return INT - VS_ERROR <=0 < VS_OK
*/
INT SysInput::GetDelta(POINT *p)
{
	if (p == NULL) return VS_ERROR;
	*p = pi_sys_input->client_delta;
	return VS_OK;
}


// ---------- GetLast ----------
/*!
\brief Get last cursor position
\author Gareth Edwards
\return POINT * - of last cursor position
*/
POINT *SysInput::GetLast()
{
	return &pi_sys_input->client_last;
}


// ---------- ResetKeyboard ----------
/*!
\brief Reset keybaord
\author Gareth Edwards
\return INT - 
*/
INT SysInput::ResetKeyboard()
{
	pi_sys_input->SetKeyboardChar(NULL, NULL);
	pi_sys_input->SetBool(SHIFT_KEY, NULL);
	pi_sys_input->SetBool(CONTROL_KEY, NULL);
	pi_sys_input->SetBool(KEY_DOWN, FALSE);
	pi_sys_input->SetBool(KEY_UP, TRUE);
	return VS_OK;
}


// ---------- ResetMouse ----------
/*!
\brief Reset mouse buttons and wheel
\author Gareth Edwards
\return INT - 
*/
INT SysInput::ResetMouse()
{
	pi_sys_input->SetBool(LBUTTON_UP, TRUE);
	pi_sys_input->SetBool(MBUTTON_UP, TRUE);
	pi_sys_input->SetBool(RBUTTON_UP, TRUE);
	pi_sys_input->SetBool(WHEEL_CHANGE, FALSE);
	return VS_OK;
}


// ---------- SetBool ----------
/*!
\brief Facade for PI_SysInput SetBool
*/
INT SysInput::SetBool(UINT id, BOOL value)
{
	return pi_sys_input->SetBool(id, value);
}


// ---------- SetInt ----------
/*!
\brief Facade for PI_SysInput SetInt
*/
INT SysInput::SetInt(UINT id, INT value)
{
	return pi_sys_input->SetInt(id, value);
}


// ---------- SetLong ----------
/*!
\brief Facade for PI_SysInput SetLong
*/
INT SysInput::SetLong(UINT id, LONG value)
{
	return pi_sys_input->SetLong(id, value);
}


// ---------- SetClientCursorPosition ----------
/*!
\brief Facade for PI_SysInput SetClientCursorPosition
*/
INT SysInput::SetClientCursorPosition(POINT *p)
{
	return pi_sys_input->SetClientCursorPosition(p);
}


// ---------- SetKeyboardChar ----------
/*!
\brief Facade for PI_SysInput SetKeyboardChar
*/
INT SysInput::SetKeyboardChar(INT ascii_code, INT key_state)
{
	return pi_sys_input->SetKeyboardChar(ascii_code, key_state);
}

