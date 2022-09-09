////////////////////////////////////////////////////////////////////////////////

// ---------- vs_app_control_interface.hpp ----------
/*!
\file vs_app_control_interface.hpp
\brief Control object "DO NOT ALTER" interface
\author Gareth Edwards 
*/


////////////////////////////////////////////////////////////////////////////////


public:

	// Constructor /destructor
	Control();
	virtual ~Control();

	// Get
	BOOL GetPicking();
	BOOL GetQuit();

	// Set
	INT  SetPicking(BOOL);

	// Event
	INT  GetGroupEventID();
	INT  GetElementEventID();
	INT  ResetEvents();

	// Handle
	INT UpdateInput();
	INT HandleMouseInput();
	INT HandleKeyboardInput();

	// Callback
	static INT CallbackPickPanel(INT, INT);
	static INT CallbackPickElement(INT, INT);

private:

	vs_system::AppControl *app_control;


////////////////////////////////////////////////////////////////////////////////
