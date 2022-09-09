////////////////////////////////////////////////////////////////////////////////

// ---------- e1_control.cpp ----------
/*!
\file e1_control.cpp
\brief Implementation of the Control class
\author Gareth Edwards
\note Vanilla C++
*/

#include "../header/e1_kontrol.h"


////////////////////////////////////////////////////////////////////////////////


using namespace examples_01;


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
		examples_01::Model       *model;
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
*/
INT Kontrol::Setup(
		vs_system::AppDependency *app_dependency,
		vs_system::AppView *app_view
	)
{

	// ---- local
		INT result = 0;


	// ---- managed by application
		pi_kontrol->app_dependency = app_dependency;
		pi_kontrol->app_view       = app_view;


	// ---- managed
		pi_kontrol->app_kontrol    = new vs_system::AppKontrol();


	// ---- inject depend
		result = pi_kontrol->app_kontrol->Setup(app_dependency);
		result = pi_kontrol->app_kontrol->SetView(app_view);
		result = pi_kontrol->app_kontrol->SetHandleKontrol((INT)this);


	// ---- set callbacks
		result = pi_kontrol->app_kontrol->SetCallbackUpdatePickPanel(UpdatePickPanel);
		result = pi_kontrol->app_kontrol->SetCallbackUpdatePickElement(UpdatePickElement);
		result = pi_kontrol->app_kontrol->SetCallbackUpdateByMouseWheel(UpdateByMouseWheel);
		result = pi_kontrol->app_kontrol->SetCallbackUpdateByKeyboard(UpdateByKeyboard);

	return VS_ERROR;
}


INT Kontrol::SetBill(INT bill)
{
	pi_kontrol->model = (Model *)bill;
	return VS_ERROR;
}


// ---------- Update ----------
/*!
\brief Update AppKontrol and AppKeyboard
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Kontrol::Update()
{

	// ---- local
	INT result = 0;


	// ---- update
	if ( result = pi_kontrol->app_kontrol->SetKeyboard() >= VS_OK )
		result = pi_kontrol->app_kontrol->SetPickCallback();


	return VS_ERROR;
}



// ---------- UpdatePickPanel ----------
/*!
\brief Update via pick panel
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value

\note
	Context:
		IF panel is NOT NULL THEN
			context == "immediate" panel context
		ELSE
			context == "persistant" panel context

*/
INT Kontrol::UpdatePickPanel(
		INT handle_app_control,
		INT handle_panel
	)
{

	// ---- local
		vs_system::AppKontrol    *app_control = (vs_system::AppKontrol *)handle_app_control;
		vs_system::AppPanel      *app_panel = (vs_system::AppPanel *)handle_panel;
		vs_system::AppDependency *app_dependency = app_control->GetAppDependency();


	//	---- reset
		vs_system::SysInput *sys_input = app_dependency->GetSysInput();
		sys_input->ResetMouse();

	return VS_OK;
}


// ---------- UpdatePickElement ----------
/*!
\brief Update via pick element
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Kontrol::UpdatePickElement(
		INT handle_app_control,
		INT handle_element
	)
{

	// ---- local
		vs_system::AppKontrol    *app_control = (vs_system::AppKontrol *)handle_app_control;
		vs_system::ElmContext    *elem_context = (vs_system::ElmContext *)handle_element;
		vs_system::AppDependency *app_dependency = app_control->GetAppDependency();


	//	---- reset
		vs_system::SysInput *sys_input = app_dependency->GetSysInput();
		sys_input->ResetMouse();

	return VS_OK;
}


// ---------- UpdateByMouseWheel ----------
/*!
\brief Update application by mouse wheel
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Kontrol::UpdateByMouseWheel(
		INT handle_app_control,
		INT handle_gfx_context
	)
{

	// ---- local
		vs_system::AppKontrol    *app_control = (vs_system::AppKontrol *)handle_app_control;
		vs_system::PanelContext  *gfx_context = (vs_system::PanelContext *)handle_gfx_context;
		vs_system::AppDependency *app_dependency = app_control->GetAppDependency();


	// ---- ?
		if ( gfx_context == NULL )
		{

			// ---- accelerate
				vs_system::AppDependency *app_dependency = app_control->GetAppDependency();
				vs_system::AppKeyboard   *app_keyboard = app_control->GetAppKeyboard();


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


							// ---- clicks
								INT wheel_clicks = sys_input->GetLong(sys_input->WHEEL_CLICKS);


							// ---- do something
								//
								// ???
								//

						}
				}

			// ---- reset mouse and zap context (if any)
				sys_input->ResetMouse();
				app_keyboard->SetPersistantPanelPanelContext(NULL);

			return VS_OK;
		}

	return VS_ERROR;
}


// ---------- UpdateByKeyboard ----------
/*!
\brief Update application by keyboard
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Kontrol::UpdateByKeyboard(
		INT handle_app_control,
		INT handle_kontrol
	)
{

	// ---- local
		Kontrol *kontrol = (Kontrol *)handle_kontrol;
		vs_system::AppKontrol    *app_control = (vs_system::AppKontrol *)handle_app_control;
		vs_system::AppDependency *app_dependency = app_control->GetAppDependency();


	// ---- valid ascii code ?
		vs_system::SysInput  *sys_input = app_dependency->GetSysInput();
		INT key_clicked = sys_input->GetInt(sys_input->ASCII_CODE);
		if ( key_clicked != NULL )
		{

			// ---- local
				vs_system::SysContext*sys_context = app_dependency->GetSysContext();
				FLOAT delta_time = (FLOAT)sys_context->GetDouble(sys_context->TIME_DELTA);

			// ---- report?
				BOOL report = FALSE;
				if ( report)
				{

					vs_system::SysOutput *sys_output = app_dependency->GetSysOutput();
					BOOL shift_key = sys_input->GetBool(sys_input->SHIFT_KEY);
					BOOL control_key = sys_input->GetBool(sys_input->CONTROL_KEY);

					CHAR text[VS_MAXCHARLEN];
						sprintf(text, "Key pressed is (%c%c) %c with delta %f",
							shift_key ? 'S' : '_',
							control_key ? 'C' : '_',
							key_clicked,
							delta_time
						);

					sys_output->Message(text);

				}

			// ---- camera
				kontrol->pi_kontrol->model->SetCamera(key_clicked, delta_time);

		}

	return VS_ERROR;
}



