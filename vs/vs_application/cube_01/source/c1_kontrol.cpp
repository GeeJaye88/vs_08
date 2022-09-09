////////////////////////////////////////////////////////////////////////////////

// ---------- e1_control.cpp ----------
/*!
\file e1_control.cpp
\brief Implementation of the Control class
\author Gareth Edwards
\note
*/

#include "../header/c1_kontrol.h"


////////////////////////////////////////////////////////////////////////////////


using namespace cube_01;


// ---------- Private Implementation of PI_Props class ----------
class Kontrol::PI_Kontrol
{

public:

	// ---- cdtor
		PI_Kontrol();
		virtual ~PI_Kontrol();


	// ---- managed by vs
		vs_system::AppDependency *app_dependency;
		vs_system::AppView       *app_view;


	// ---- managed by application
		cube_01::Model           *model;
		vs_system::AppKontrol    *app_kontrol;

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Kontrol::PI_Kontrol::PI_Kontrol()
{
	app_kontrol = NULL;
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
Kontrol::PI_Kontrol::~PI_Kontrol()
{
	if ( app_kontrol != NULL ) delete app_kontrol;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Kontrol::Kontrol()
{
	pi_kontrol = new PI_Kontrol();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
Kontrol::~Kontrol()
{
	if ( pi_kontrol != NULL ) { delete pi_kontrol; pi_kontrol = NULL; }
}


// ---------- Setup ----------
/*!
\brief Setup AppKontrol and AppKeyboard
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value

\note invoked from application::Setup

*/
INT Kontrol::Setup(
		vs_system::AppDependency *app_dependency,
		vs_system::AppView *app_view,
		Model *model
	)
{

	// ---- local
		INT result = 0;


	// ---- managed by application
		pi_kontrol->app_dependency = app_dependency;
		pi_kontrol->app_view       = app_view;
		pi_kontrol->model          = model;


	// ---- managed
		pi_kontrol->app_kontrol    = new vs_system::AppKontrol();
		vs_system::AppKontrol *app_kontrol = pi_kontrol->app_kontrol;


	// ---- setup, & inject MVC depend
		result = app_kontrol->Setup(app_dependency);
		result = app_kontrol->SetHandleModel((INT)model);
		result = app_kontrol->SetView(app_view);
		result = app_kontrol->SetHandleKontrol((INT)this);


	// ---- set callbacks
		result = app_kontrol->SetCallbackUpdatePickPanel(CallbackByPickPanel);
		result = app_kontrol->SetCallbackUpdatePickElement(CallbackByPickElement);
		result = app_kontrol->SetCallbackUpdateByMouseWheel(CallbackByMouseWheel);
		result = app_kontrol->SetCallbackUpdateByKeyboard(CallbackByKeyboard);

	return VS_ERROR;
}


// ---------- Update ----------
/*!
\brief Update AppKontrol and AppKeyboard
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value

\note invoked from application::Display

*/
INT Kontrol::Update()
{

	// ---- local
		INT result = 0;


	// ---- handle keyboard input
		if ( result = pi_kontrol->app_kontrol->InvokeInputCallbacks() >= VS_OK )
		{
			result = pi_kontrol->app_kontrol->SetPickCallbackFromGfxInterface();
		}

	return VS_ERROR;
}



// ---------- CallbackByPickPanel ----------
/*!
\brief Callback by pick panel event
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value

\note invoked from:
	GfxInterface::RenderViewPort ->
		GfxInterface::PI_Props::isPickPanel ->
			AppKontrol::CallbackPickPanelFromGfxInterface ->
				AppKontrol::UpdatePickPanelFromGfxInterface

\note
	Context:
		IF panel is NOT NULL THEN
			context == "immediate" panel context
		ELSE
			context == "persistant" panel context

*/
INT Kontrol::CallbackByPickPanel(
		INT handle_app_kontrol,
		INT handle_panel
	)
{

	// ---- local
		vs_system::AppKontrol    *app_kontrol    = (vs_system::AppKontrol *)handle_app_kontrol;
		vs_system::AppPanel      *app_panel      = (vs_system::AppPanel *)handle_panel;
		vs_system::AppDependency *app_dependency = app_kontrol->GetAppDependency();
		vs_system::SysInput      *sys_input      = app_dependency->GetSysInput();


	// ---- select view rects mode
		Model *model = (Model *)app_kontrol->GetHandleModel();
		INT result = 0;//model->SetPanelsLayoutMode();


	//	---- ok ?
		if ( result == VS_OK )
		{
			// ---- done...
			//
			//  Reset by AppKontrol::UpdatePanelFromCallback
			//
			return VS_OK;
		}

	return VS_OK;
}


// ---------- CallbackByPickElement ----------
/*!
\brief Callback by pick element event
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value

\note invoked from:
	GfxInterface::RenderViewPort ->
		GfxInterface::PI_Props::isPickPanel ->
			AppKontrol::CallbackPickElementFromGfxInterface ->
				AppKontrol::UpdatePickElementFromGfxInterface

*/
INT Kontrol::CallbackByPickElement(
		INT handle_app_kontrol,
		INT handle_element
	)
{

	// ---- local
		INT result = 0;
		BOOL reset_mouse = TRUE;
		vs_system::AppKontrol    *app_kontrol = (vs_system::AppKontrol *)handle_app_kontrol;
		vs_system::AppDependency *app_dependency = app_kontrol->GetAppDependency();


	// ---- has to be ok.... to get to here!!!
		vs_system::ElmContext *elem_context = NULL;
		result = app_dependency->GetGfxInterface()->GetElementContext(handle_element, &elem_context);
		if ( elem_context == NULL ) return VS_ERROR;


	// ---- Tint - required
		INT group_id = elem_context->GetGroupId();
		switch ( group_id )
		{

			case 1:
				break;

			default:
				reset_mouse = FALSE;
				break;
		}


	//	---- ok ?
		if ( reset_mouse )
		{
			// ---- done...
			//
			//  Reset by AppKontrol::UpdateElementFromCallback
			//
			return VS_OK;
		}


	return VS_OK;
}


// ---------- CallbackByMouseWheel ----------
/*!
\brief Update application by mouse wheel event
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK

\note invoked from:
	application::Display ->
		Kontrol::Update ->
			AppKontrol::SetKeyboard ->
				AppKontrol::PI_AppKontrol::SetKeyboard ->
					AppKeyBoard::SetKeyboard ->
						AppKeyBoard::PI_AppKeyBoard::SetKeyboard ->
							AppKontrol::UpdatePanelFromCallback
								AppKontrol::UpdateByMouseWheelFromPanelCallback

*/
INT Kontrol::CallbackByMouseWheel(
		INT handle_app_kontrol,
		INT handle_gfx_context
	)
{

	// ---- local
		vs_system::AppKontrol    *app_kontrol = (vs_system::AppKontrol *)handle_app_kontrol;
		vs_system::PanelContext  *gfx_context = (vs_system::PanelContext *)handle_gfx_context;
		vs_system::AppDependency *app_dependency = app_kontrol->GetAppDependency();


	// ---- ?
		if ( gfx_context == NULL )
		{

			// ---- accelerate
				vs_system::AppDependency *app_dependency = app_kontrol->GetAppDependency();
				vs_system::AppKeyboard   *app_keyboard = app_kontrol->GetAppKeyboard();


			// ---- wheel change ?
				//
				//    Note: already tested if invoked from
				//    AppKontrol->UpdateInput method )
				//
				vs_system::SysInput *sys_input = app_dependency->GetSysInput();
				BOOL wheel_change = sys_input->GetBool(sys_input->WHEEL_CHANGE);
				if ( wheel_change )
				{
					  
					// ---- context = "over" panel ?
						vs_system::GfxInterface  *gfx_interface = app_dependency->GetGfxInterface();
						INT result = gfx_interface->GetContextOverPanel(&gfx_context);
						if ( gfx_context )
						{

							// ---- get config name
								vs_system::SysContext *sys_context = app_dependency->GetSysContext();
								CHAR *config_name = sys_context->GetChar(vs_system::SysContext::CONFIG_NAME);


							// ---- do something
								//
								// ???
								//
								OutputDebugString(" +++ WHEELIE \n");


							// ---- camera
								FLOAT delta = (FLOAT)sys_context->GetDouble(vs_system::SysContext::TIME_MS_DELTA);
								INT wheel_clicks = sys_input->GetLong(sys_input->WHEEL_CLICKS);
								Model *model = (Model *)app_kontrol->GetHandleModel();
								INT msg = wheel_clicks > 0 ? '{' : '}';
								INT resul = model->SetMsgProc(msg, delta);


							//---- ok ?
								if ( result )
								{
									//
									//  Reset by AppKontrol::UpdatePanelFromCallback
									//
									return VS_OK;
								}
						}
				}
		}

	return VS_ERROR;
}


// ---------- CallbackByKeyboard ----------
/*!
\brief Update application by keyboard event
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK

\note invoked from:
	application::Display ->
		Kontrol::Update ->
			AppKontrol::SetKeyboard ->
				AppKontrol::PI_AppKontrol::SetKeyboard ->
					AppKeyBoard::SetKeyboard ->
						AppKeyBoard::PI_App_Keyboard::SetKeyboard ->
							AppKontrol::UpdateByKeyboardCallback

*/
INT Kontrol::CallbackByKeyboard(
		INT handle_app_kontrol,
		INT handle_kontrol
	)
{

	// ---- local
		Kontrol *kontrol = (Kontrol *)handle_kontrol;
		vs_system::AppKontrol    *app_kontrol = (vs_system::AppKontrol *)handle_app_kontrol;
		vs_system::AppDependency *app_dependency = app_kontrol->GetAppDependency();


	// ---- valid ascii code ?
		vs_system::SysInput  *sys_input = app_dependency->GetSysInput();
		INT msg = sys_input->GetInt(sys_input->ASCII_CODE);
		if ( msg != NULL )
		{

			// ---- local
				vs_system::SysContext*sys_context = app_dependency->GetSysContext();
				FLOAT delta = (FLOAT)sys_context->GetDouble(sys_context->TIME_MS_DELTA);


			// ---- report?
				BOOL report = FALSE;
				if ( report)
				{
					vs_system::SysOutput *sys_output = app_dependency->GetSysOutput();
					BOOL shift_key   = sys_input->GetBool(sys_input->SHIFT_KEY);
					BOOL control_key = sys_input->GetBool(sys_input->CONTROL_KEY);

					CHAR text[VS_MAXCHARLEN];
						sprintf(text, "Key pressed is (%c%c) %c with delta %f",
							shift_key ? 'S' : '_',
							control_key ? 'C' : '_',
							msg,
							delta
						);
					sys_output->Message(text);
				}


			// ---- model
				INT result = VS_OK;
				// Model *model = (Model *)app_kontrol->GetHandleModel();
				Model *model = kontrol->pi_kontrol->model;


			// ---- process input
				result = model->SetMsgProc(msg, delta);


			// ---- done ?
				//
				// note: .... then reset by AppKontrol::UpdateByKeyboardCallback
				//
				if ( result )
				{
					return VS_OK;
				}

		}

	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////

