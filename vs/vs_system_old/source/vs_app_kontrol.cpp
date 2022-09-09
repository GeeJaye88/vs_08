////////////////////////////////////////////////////////////////////////////////

// ---------- vs_app_control.cpp ----------
/*!
\file vs_app_control.cpp
\brief Implementation of the App(lication)Control class.
\author Gareth Edwards 

\note

In application Control object Setup method:

	AppKontrol object is created thus:

		pi_kontrol->app_kontrol = new vs_system::AppKontrol();
		vs_system::AppKontrol *app_kontrol = pi_kontrol->app_kontrol;


	AppKontrol setup, & inject MVC depend thus:

		result = app_kontrol->Setup(app_dependency);

		result = app_kontrol->SetHandleModel((INT)model);        // M
		result = app_kontrol->SetView(app_view);                 // V
		result = app_kontrol->SetHandleKontrol((INT)this);       // C


	AppKontrol set callbacks thus:

		result = app_kontrol->SetCallbackUpdatePickPanel(CallbackByPickPanel);
		result = app_kontrol->SetCallbackUpdatePickElement(CallbackByPickElement);
		result = app_kontrol->SetCallbackUpdateByMouseWheel(CallbackByMouseWheel);
		result = app_kontrol->SetCallbackUpdateByKeyboard(CallbackByKeyboard);



In application Application or Model objects invoke Cotrol object Update method:

	INT Kontrol::Update()
	{
		INT result = 0;
		if ( result = pi_kontrol->app_kontrol->InvokeInputCallbacks() >= VS_OK )
		{
			result = pi_kontrol->app_kontrol->SetPickCallbackFromGfxInterface();
		}
		return VS_ERROR;
	}

The above then passes the callbacks to the gfx interface, which then invoked the
callbacks as and when a panel, or element is being selected. Typically an element
callback takes prescedence over a panel callback.

*/

// ---- include ----
#include "../header/vs_app_kontrol.h"

using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Props class ----------
class AppKontrol::PI_AppKontrol
{

public:

	// ---- cdtor
		PI_AppKontrol();
		~PI_AppKontrol();


	// --- update stuff
		INT Setup(AppDependency *);
		INT InvokeInputCallbacks();


	// --- set stuff - injected by kontrol
		INT SetView(AppView *v)
				{
					app_view = v;
					return VS_OK;
				}


	// --- get stuff - accessed by application kontrol
		AppView *GetView()
				{
					return app_view;
				}
		AppKeyboard *GetKeyboard()
				{
					return app_keyboard;
				}
		AppDependency *GetAppDependency() { return app_dependency; }


	// ---- inject dependency pointers - managed by VS
		AppDependency *app_dependency;
		GfxInterface  *gfx;
		SysContext    *sys_context;
		SysOutput     *sys_output;
		SysInput      *sys_input;


	// ---- MVC- managed by application
		AppView       *app_view;
		INT            kontrol_handle;
		INT            model_handle;


	// ---- keyboard - managed by application kontrol
		AppKeyboard   *app_keyboard;


	// ---- callback
		INT (*callback_pick_panel)(INT, INT);
		INT (*callback_pick_element)(INT, INT);
		INT (*callback_scroll_mouse)(INT, INT);
		INT (*callback_keyboard)(INT, INT);
};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
AppKontrol::PI_AppKontrol::PI_AppKontrol()
{

	// ---- init injected depend
		app_dependency = NULL;
		gfx            = NULL;
		sys_context    = NULL;
		sys_output     = NULL;
		sys_input      = NULL;


	// ---- keyboard
		app_keyboard = new AppKeyboard();


	// ---- callback
		callback_pick_panel   = NULL;
		callback_pick_element = NULL;
		callback_scroll_mouse = NULL;
		callback_keyboard     = NULL;
}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
AppKontrol::PI_AppKontrol::~PI_AppKontrol()
{
	// ---- keyboard
		if ( app_keyboard == NULL ) { delete app_keyboard; app_keyboard = NULL; }
}



// ---------- Setup----------
/*!
\brief Setup 
\author Gareth Edwards
\return INT - no update <= NULL < VS_OK
*/
INT AppKontrol::PI_AppKontrol::Setup(vs_system::AppDependency *app_dep)
{

	// ---- inject depend
		app_dependency = app_dep;
		gfx            = app_dep->GetGfxInterface();
		sys_context    = app_dep->GetSysContext(); 
		sys_output     = app_dep->GetSysOutput();
		sys_input      = app_dep->GetSysInput();


	// ---- inject depend into keyboard
		app_keyboard->SetDependency(app_dep);

	return VS_OK;
}


// ---------- InvokeInputCallbacks----------
/*!
\brief Set update of mouse and keyboard
\author Gareth Edwards
\return INT - no update <= NULL < VS_OK 
*/
INT AppKontrol::PI_AppKontrol::InvokeInputCallbacks()
{
	return app_keyboard->InvokeInputCallbacks();
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
AppKontrol::AppKontrol()
{
	// allocate and construct private properties
	pi_app_kontrol = new PI_AppKontrol();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
AppKontrol::~AppKontrol()
{
	delete pi_app_kontrol;
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT AppKontrol::Setup(vs_system::AppDependency *app_dep)
{

	// ---- inject depend
		pi_app_kontrol->Setup(app_dep);


	// ---- set panel callback ( required for scroll wheel )
		pi_app_kontrol->app_keyboard->SetPickPanelCallback(
					UpdatePanelFromCallback,
					(INT)this
				);

	// ---- set panel callback ( required for scroll wheel )
		pi_app_kontrol->app_keyboard->SetKeyboardCallback(
					UpdateByKeyboardCallback,
					(INT)this
				);

	return VS_OK;
}


// ---------- GetAppKeyboard----------
/*!
\brief Get keyboard
\author Gareth Edwards
\return 
*/
AppKeyboard * AppKontrol::GetAppKeyboard()
{
	return pi_app_kontrol->GetKeyboard();
}


// ---------- GetAppDependency ----------
/*!
\brief Get application dependency
\author Gareth Edwards
\return 
*/
AppDependency * AppKontrol::GetAppDependency()
{
	return pi_app_kontrol->GetAppDependency();
}


// ---------- GetAppView ----------
/*!
\brief
\author Gareth Edwards
\return 
*/
AppView * AppKontrol::GetAppView()
{
	return pi_app_kontrol->app_view;
}


// ---------- GetHandleKontrol ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - no update <= NULL < VS_OK
*/
INT AppKontrol::GetHandleKontrol()
{
	return pi_app_kontrol->kontrol_handle;
}


// ---------- GetHandleModel ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - no update <= NULL < VS_OK
*/
INT AppKontrol::GetHandleModel()
{
	return pi_app_kontrol->model_handle;
}


// ---------- SetKeyboard----------
/*!
\brief Set update of mouse and keyboard
\author Gareth Edwards
\return INT - no update <= NULL < VS_OK
*/
INT AppKontrol::InvokeInputCallbacks()
{
	return pi_app_kontrol->InvokeInputCallbacks();
}


// ---------- SetPickCallbackFromGfxInterface ----------
/*!
\brief Set pick panel and element callbacks
\author Gareth Edwards
\return INT - no update <= NULL < VS_OK

\note binds all mouse and keyboard events that interact
      with panels and elements to callback functions

\note has to be done every frame, as render to texture events
      overwrite these specific callback functions

*/
INT AppKontrol::SetPickCallbackFromGfxInterface()
{

	vs_system::GfxInterface *gfx = pi_app_kontrol->app_dependency->GetGfxInterface();
	gfx->SetCallbackPickPanel(CallbackPickPanelFromGfxInterface, (INT)this);
	gfx->SetCallbackPickElement(CallbackPickElementFromGfxInterface, (INT)this);

	return VS_OK;
}


// ---------- SetView ----------
/*!
\brief Inject application view object
\author Gareth Edwards
\return INT - no update <= NULL < VS_OK
*/
INT AppKontrol::SetView(vs_system::AppView *v)
{
	pi_app_kontrol->SetView(v);
	return VS_OK;
}


INT AppKontrol::SetHandleKontrol(INT kh)
{
	pi_app_kontrol->kontrol_handle = kh;
	return VS_OK;
}


INT AppKontrol::SetHandleModel(INT mh)
{
	pi_app_kontrol->model_handle = mh;
	return VS_OK;
}


INT AppKontrol::SetCallbackUpdatePickPanel(INT(*callback)(INT, INT))
{
	pi_app_kontrol->callback_pick_panel = callback;
	return VS_OK;
}


INT AppKontrol::SetCallbackUpdatePickElement(INT(*callback)(INT, INT))
{
	pi_app_kontrol->callback_pick_element= callback;
	return VS_OK;
}


INT AppKontrol::SetCallbackUpdateByMouseWheel(INT(*callback)(INT, INT))
{
	pi_app_kontrol->callback_scroll_mouse = callback;
	return VS_OK;
}


INT AppKontrol::SetCallbackUpdateByKeyboard(INT(*callback)(INT, INT))
{
	pi_app_kontrol->callback_keyboard = callback;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- CallbackPickPanel ----------
/*!
\brief Callback pick viewport
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
\note Invokes UpdatePanel method
\note
*/
INT AppKontrol::CallbackPickPanelFromGfxInterface(INT handle_control_obj, INT handle_panel_obj)
{
	INT result = UpdatePanelFromCallback(handle_control_obj, handle_panel_obj);
	return VS_OK;
}


// ---------- CallbackPickElement ----------
/*!
\brief Callback pick item
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
\note
*/
INT AppKontrol::CallbackPickElementFromGfxInterface(INT handle_control_obj, INT element_handle)
{
	INT result = UpdateElementFromCallback(handle_control_obj, element_handle);
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- UpdatePanel ----------
INT AppKontrol::UpdatePanelFromCallback(
		INT handle_control_obj,
		INT handle_panel
	)
{

	if (handle_control_obj == NULL) return VS_ERROR;

	// --- local
		INT result = VS_OK;
		AppKontrol *app_kontrol = (AppKontrol *)handle_control_obj;
		vs_system::AppDependency *app_dependency =
			app_kontrol->pi_app_kontrol->app_dependency;


	// ---- log ?
		if ( app_dependency->GetSysOutput()->GetLog() )
		{
			vs_system::SysOutput *sys_output = app_dependency->GetSysOutput();
			sys_output->Message("UpdatePanelFromCallback:");
		}


	// ---- IF NO interaction THEN return
		if ( handle_panel != NULL )
		{
			BOOL interaction_status = TRUE;
			vs_system::AppView *app_view = app_kontrol->pi_app_kontrol->GetView();
			result = app_view->GetInteractionStatus(
						NULL,
						&interaction_status
					);
			if ( !interaction_status ) return VS_OK;
		}


	// ---- local
		vs_system::AppKeyboard *app_keyboard = app_kontrol->pi_app_kontrol->GetKeyboard();
		vs_system::AppPanel    *app_panel    = (vs_system::AppPanel *)handle_panel;


	// ---- set context
		vs_system::PanelContext *panel_context = NULL;
		if ( app_panel != NULL )
			panel_context = app_panel->GetPanelContext();
		else
			panel_context = app_keyboard->GetPersistantPanelPanelContext();


	// ---- invoke application keyboard & mouse wheel callback 
		result = UpdateByMouseWheelFromPanelCallback(
						handle_control_obj,
						(INT)panel_context
					);
			if ( result == VS_OK )
			{
				vs_system::AppKeyboard *app_keyboard = app_kontrol->pi_app_kontrol->GetKeyboard();
				result = app_dependency->GetSysInput()->ResetMouse();
				result = app_keyboard->SetPersistantPanelPanelContext(NULL);
				return VS_OK;
			}


	// ---- invoke application pick panel callback
		result = app_kontrol->pi_app_kontrol->callback_pick_panel
					(
						handle_control_obj,
						handle_panel
					);
			if ( result == VS_OK )
			{
				vs_system::AppKeyboard *app_keyboard = app_kontrol->pi_app_kontrol->GetKeyboard();
				result = app_dependency->GetSysInput()->ResetMouse();
				result = app_keyboard->SetPersistantPanelPanelContext(NULL);
				return VS_OK;
			}

	return VS_ERROR;
}


// ---------- UpdateElement ----------
INT AppKontrol::UpdateElementFromCallback(
		INT handle_control_obj,
		INT handle_element
	)
{

	// ---- local
		INT result = VS_OK;
		AppKontrol *app_kontrol = (AppKontrol *)handle_control_obj;
			vs_system::AppDependency *app_dependency =
				app_kontrol->pi_app_kontrol->app_dependency;


	// ---- log ?
		if ( app_dependency->GetSysOutput()->GetLog() )
		{
			vs_system::SysOutput *sys_output = app_dependency->GetSysOutput();
			sys_output->Message("UpdateElement:");
		}


	// ---- invoke application pick element callback
		result = app_kontrol->pi_app_kontrol->callback_pick_element
					(
						handle_control_obj,
						handle_element
					);
			if ( result == VS_OK )
			{
				vs_system::AppKeyboard *app_keyboard = app_kontrol->pi_app_kontrol->GetKeyboard();
				result = app_dependency->GetSysInput()->ResetMouse();
				result = app_keyboard->SetPersistantPanelPanelContext(NULL);
				return VS_OK;
			}

	return VS_ERROR;
}


// ---------- UpdateByMouseWheelFromPanelCallback ----------
INT AppKontrol::UpdateByMouseWheelFromPanelCallback(
		INT handle_control_obj,
		INT handle_panel_context
	)
{

	// ---- local
		INT result = VS_OK;
		AppKontrol *app_kontrol = (AppKontrol *)handle_control_obj;

		vs_system::AppDependency *app_dependency =
			app_kontrol->pi_app_kontrol->app_dependency;

		vs_system::PanelContext *panel_context =
			(vs_system::PanelContext *)handle_panel_context;


	// ---- wheel stuff
		SysInput *sys_input = app_dependency->GetSysInput();
		BOOL wheel_change = sys_input->GetBool(sys_input->WHEEL_CHANGE);
		LONG wheel_clicks = sys_input->GetLong(sys_input->WHEEL_CLICKS);


	// --- change ?
		if ( wheel_clicks )
		{

			// ---- log ?
				if ( app_dependency->GetSysOutput()->GetLog() )
				{
					vs_system::SysOutput *sys_output = app_dependency->GetSysOutput();
					sys_output->Message("UpdateByMouseWheelFromPanelCallback:");
				}

			// ---- invoke application Kontrol callback
				result = app_kontrol->pi_app_kontrol->callback_scroll_mouse
					(
						handle_control_obj,
						handle_panel_context
					);

			// ---- reset mouse and zap context (if any)
				if ( result == VS_OK )
				{
					//vs_system::AppKeyboard *app_keyboard = app_kontrol->pi_app_kontrol->GetKeyboard();
					//result = app_dependency->GetSysInput()->ResetMouse();
					//result = app_keyboard->SetPersistantPanelPanelContext(NULL);
					return VS_OK;
				}
		}

	return VS_ERROR;
}


// ---------- UpdateByKeyboardCallback ----------
INT AppKontrol::UpdateByKeyboardCallback(
	INT handle_control_obj,
	INT na
)
{

	// ---- local
		INT result = VS_OK;
		AppKontrol *app_kontrol = (AppKontrol *)handle_control_obj;


	// ---- invoke application Kontrol callback
		result = app_kontrol->pi_app_kontrol->callback_keyboard
			(
				handle_control_obj,
				app_kontrol->pi_app_kontrol->kontrol_handle
			);
		if ( result == VS_OK ) return VS_OK;;


	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////
