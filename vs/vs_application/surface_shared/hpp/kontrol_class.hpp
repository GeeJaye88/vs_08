////////////////////////////////////////////////////////////////////////////////

// ---------- kontrol_class.hpp ----------
/*!
\file model.hpp
\brief Implementation of Kontrol class methods
\author Gareth Edwards
*/


////////////////////////////////////////////////////////////////////////////////


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
	if (app_kontrol != NULL) delete app_kontrol;
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
	if (pi_kontrol != NULL) { delete pi_kontrol; pi_kontrol = NULL; }
}


////////////////////////////////////////////////////////////////////////////////

// ---------- s3_kontrol.cpp ----------
/*!
\file s3_kontrol.cpp
\brief Implementation of the Kontrol class
\author Gareth Edwards
\note
*/


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
		INT result = VS_OK;
		vs_system::AppKontrol *app_kontrol = (vs_system::AppKontrol *)handle_app_kontrol;
		vs_system::AppPanel   *app_panel   = (vs_system::AppPanel *)handle_panel;


	// ---- select panels layout ( surface_shared/hpp/model_panels_set_layout.hpp )
		Model *model = (Model *)app_kontrol->GetHandleModel();
		INT layout_changed = model->SetPanelsLayoutByCorner();


	//	---- ! layout unchanged... update camera
		if ( !layout_changed  )
		{

			// ---- local
				vs_system::AppDependency *app_dependency = app_kontrol->GetAppDependency();
				vs_system::SysContext    *sys_context    = app_dependency->GetSysContext();
				vs_system::SysInput      *sys_input      = app_dependency->GetSysInput();
				vs_system::GfxInterface  *gfx_interface  = app_dependency->GetGfxInterface();


			// ---- get client adjusted viewport
				VsFloatRectangle cav = { 0, 0, 0, 0 };
				result = gfx_interface->GetClientAdjustedViewport(&cav);


			// ---- pick within CAV
				POINT pick_point = { 0, 0 };
				pick_point = *sys_input->GetPosition();
				FLOAT x_pick = (FLOAT)pick_point.x - cav.left;
				FLOAT y_pick = (FLOAT)pick_point.y - cav.top;


			// ---- pick differences
				FLOAT x_diff = x_pick - (cav.right - cav.left ) / 2;
				FLOAT y_diff = y_pick - (cav.bottom - cav.top) / 2;


			// ---- pick messages
				CHAR x_msg = x_diff < 0 ? 'D' : 'A';
				CHAR y_msg = y_diff < 0 ? 'R' : 'F';


			// ---- store state ( s3.model_kamera.hpp )
				INT  message = model->GetCameraControlMessage();
				BOOL active = model->GetMsgCameraControlPersistantActive();


			// ---- process input ( s3.model_kamera.hpp )
				model->SetMsgCameraControlPersistantActive(FALSE);
				x_diff = abs(x_diff);
				result = model->SetMsgProc(x_msg, x_diff);
				y_diff = abs(y_diff);
				result = model->SetMsgProc(y_msg, y_diff);


			// ---- restore state ( s3.model_kamera.hpp )
				model->SetMsgCameraControlPersistantActive(active);
				model->SetCameraControlMessage(message);


			// ---- done...
			//
			//  Reset by AppKontrol::UpdatePanelFromCallback
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


							// ---- model
								Model *model = (Model *)app_kontrol->GetHandleModel();
	

							// ---- store state ( s3.model_kamera.hpp )
								INT  message = model->GetCameraControlMessage();
								BOOL active = model->GetMsgCameraControlPersistantActive();


							// ---- process input ( s3.model_kamera.hpp )
								model->SetMsgCameraControlPersistantActive(FALSE);
								FLOAT delta = (FLOAT)sys_context->GetDouble(vs_system::SysContext::TIME_MS_DELTA);
								INT wheel_clicks = sys_input->GetLong(sys_input->WHEEL_CLICKS);
								INT msg = wheel_clicks > 0 ? '{' : '}';
								FLOAT scalar = model->GetCameraControlZoomScalar();


							// ---- (s3.kamera.hpp)
								INT   result = model->SetMsgProc(msg, delta * scalar);


							// ---- restore state ( s3.model_kamera.hpp )
								model->SetMsgCameraControlPersistantActive(active);
								model->SetCameraControlMessage(message);


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


\note - return value is ignored!

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
		INT result = sys_input->SetInt(sys_input->ASCII_CODE_PREV, msg);
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

					CHAR ods[VS_MAXCHARLEN];
						sprintf(ods, "Key pressed is (%c%c) %c with delta %f \n",
							shift_key ? 'S' : '_',
							control_key ? 'C' : '_',
							msg,
							delta
						);
					OutputDebugString(ods);
				}


			// ---- model
				INT result = VS_OK;
				Model *model = kontrol->pi_kontrol->model;



				//model::Surface_03 *surface_03 = model->pi_model->surface_03;
				//UINT loi_index = surface_03->GetTargetLoiIndex();
				//char msg[16];
				//sprintf(msg, "%d\n", (INT)loi_index);
				//OutputDebugString(msg);

			// ---- process input ( s[#].model_kamera.hpp )
				result = model->SetMsgProc(msg, delta);


			// ---- update tint control ui ( s[#]_tint_interface.hpp ) 
				result = model->Tint_Explore_UpdateByMessage((INT)model, toupper(msg));


			// ---- done [ return value is ignored! ? ]
				//
				// note: .... then reset by AppKontrol::UpdateByKeyboardCallback
				//
				if ( result )
				{
					return VS_OK;
				}

		}

	return VS_ERROR; // return value is ignored!
}


////////////////////////////////////////////////////////////////////////////////

