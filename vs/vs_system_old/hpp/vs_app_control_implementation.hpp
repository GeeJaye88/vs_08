////////////////////////////////////////////////////////////////////////////////

// ---------- vs_app_control_implementation.hpp ----------
/*!
\file vs_app_control_implementation.hpp
\brief Control object "DO NOT ALTER" implementation
\author Gareth Edwards 
*/


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\note DO NOT ALTER THIS CODE
*/
Control::Control()
{
	app_control  = new vs_system::AppControl();
	INT result = Construct();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
\note DO NOT ALTER THIS CODE
*/

Control::~Control()
{
	if ( app_control != NULL ) { delete app_control; app_control = NULL; }
	INT result = Destruct();
}


////////////////////////////////////////////////////////////////////////////////


// ========== Get =========


// ---------- GetPicking ----------
/*!
\brief
\author Gareth Edwards
\return BOOL -
\note DO NOT ALTER THIS CODE
*/
BOOL Control::GetPicking()
{
	return app_control->picking;
}


// ---------- GetQuit ----------
/*!
\brief
\author Gareth Edwards
\return BOOL -
\note DO NOT ALTER THIS CODE
*/
BOOL Control::GetQuit()
{
	return app_control->quit;
}


////////////////////////////////////////////////////////////////////////////////


// ========== Set =========


// ---------- SetPicking ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
\note DO NOT ALTER THIS CODE
*/
INT Control::SetPicking(BOOL value)
{
	app_control->picking = value;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ========== Event ==========


// ---------- GetGroupEventID ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
\note DO NOT ALTER THIS CODE
*/
INT Control::GetGroupEventID()
{
	return app_control->group_id;
}


// ---------- GetElementEventID ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
\note DO NOT ALTER THIS CODE
*/
INT Control::GetElementEventID()
{
	return app_control->element_id;
}


// ---------- ResetEvents ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
\note DO NOT ALTER THIS CODE
*/
INT Control::ResetEvents()
{
	app_control->group_id = 0;
	app_control->element_id = 0;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ========== Handle ==========


// ---------- Update Input----------
/*!
\brief Update input from mouse and keyboard
\author Gareth Edwards
\return INT - no update <= NULL < VS_OK
\note DO NOT ALTER THIS CODE
*/
INT Control::UpdateInput()
{
	// Facade
	return app_control->UpdateInput();
}


// ---------- HandleMouseInput ----------
/*!
\brief Handle mouse input
\author Gareth Edwards
\return INT - no update <= NULL < VS_OK
\note DO NOT ALTER THIS CODEKeyboard
*/
INT Control::HandleMouseInput()
{
	// IMPORTANT CODE - DO NOT CHANGE
	//
	// The following code block "binds" all mouse and keyboard
	// events to specific callback functions....
	//
	{
		vs_system::GfxInterface *gfx = app_dependency->GetGfxInterface();
		gfx->SetCallbackPickPanel(CallbackPickPanel, (INT)this);
		gfx->SetCallbackPickElement(CallbackPickElement, (INT)this);
	}

	/*
	// IF mouse moved THEN
	if ( app_control->delta_change )
	{
		INT result = CursorMoved();
	}

	// IF wheel clicked THEN
	if ( app_control->wheel_clicks > 0 )
	{
		INT result = WheelClicked();
	}
	*/

	return VS_OK;
}


// ---------- HandleKeyboardInput ----------
/*!
\brief Handle keyboard input
\author Gareth Edwards
\return INT - no update <= NULL < VS_OK
\note DO NOT ALTER THIS CODE
*/
INT Control::HandleKeyboardInput()
{
	/*
	// Keyboard clicked
	if ( app_control->key_down )
	{
		INT result = KeyPressedDown();
	}
	*/

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ========== Callback ==========


// ---------- CallbackPickPanel ----------
/*!
\brief Callback pick viewport
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
\note Invokes UpdatePanel method
\note DO NOT ALTER THIS CODE
*/
INT Control::CallbackPickPanel(INT handle_control_obj, INT handle_panel_obj)
{
	INT result = UpdatePanel(handle_control_obj, handle_panel_obj);
	return VS_OK;
}


// ---------- CallbackPickElement ----------
/*!
\brief Callback pick item
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
\note DO NOT ALTER THIS CODE
*/
INT Control::CallbackPickElement(INT handle_control_obj, INT element_handle)
{
	INT result = UpdateElement(handle_control_obj, element_handle);
	return VS_OK;
}

////////////////////////////////////////////////////////////////////////////////
