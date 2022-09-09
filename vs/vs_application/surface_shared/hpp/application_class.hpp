////////////////////////////////////////////////////////////////////////////////

// ---------- s3_application.cpp ----------
/*!
\file s3_application.cpp
\brief Implementation of the application class
\author Gareth Edwards
\note
*/


// ---------- constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::PI_Application::PI_Application()
{
	;
}


// ---------- destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::PI_Application::~PI_Application()
{

	// ---- delete MVC objects
		if ( app_view != NULL ) { delete app_view; app_view = NULL; }
		if ( kontrol != NULL )  { delete kontrol;  kontrol  = NULL; }
		if ( model != NULL )    { delete model;    model    = NULL; }
}


////////////////////////////////////////////////////////////////////////////////


// application static assignments
application::PI_Application  *application::pi_application = NULL;


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::application()
{
	// allocate and construct private properties
	pi_application = new PI_Application();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
application::~application()
{
	// delete private properties
	delete pi_application;
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT application::Setup(vs_system::AppDependency *app_dep)
{

	// ---- local
		INT result = 0;


	// ---- store dependency pointers
		pi_application->app_dependency = app_dep;
		pi_application->gfx            = app_dep->GetGfxInterface();
		pi_application->sys_context    = app_dep->GetSysContext();
		pi_application->sys_output     = app_dep->GetSysOutput();


	// ---- allocate, construct, setup and configure ----

		// M
		pi_application->model = new Model();
		result = pi_application->model->Setup(app_dep);

		// V
		pi_application->app_view = new vs_system::AppView();
		result = pi_application->app_view->Setup(app_dep);

		// C
		pi_application->kontrol = new Kontrol();
		result = pi_application->kontrol->Setup(
				app_dep,
				pi_application->app_view,
				pi_application->model
			);

	return VS_OK;
}


// ---------- SetupGraphics ----------
/*!
\brief Setup device
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SetupGraphics()
{

	// ---- local
		INT result = VS_OK;


	// ---- M
		result = pi_application->model->SetPanelsLayoutDisplayStatus(
				pi_application->app_dependency,
				pi_application->app_view
			);
		result = pi_application->model->SetupGraphics();


	// ---- M - Toolbar - reuired to poistion using AppPane
		result = pi_application->model->ToolbarButtonsConfig();


	// ---- C - Tint - required
		result = pi_application->kontrol->TintSetDisplayListHandles();
		result = pi_application->kontrol->KpadSetDisplayListHandles();

	return VS_OK;
}


// ---------- CallbackDisplayFromView ----------
/*!
\brief Callback display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::CallbackDisplayFromView(
		INT app_cast_to_int,
		vs_system::AppPanel *panel
	)
{

	// ---- accelerate
		vs_system::PanelContext *gfx_context = panel->GetPanelContext();
		application *app = (application *)app_cast_to_int;


	// ---- M - render panel->display list
		app->pi_application->model->Display(panel->GetPanelContext(), NULL);

	return VS_OK;
}


// ---------- CleanupGraphics ----------
/*!
\brief Cleanup device
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::CleanupGraphics()
{

	// ---- local
		INT result = 0;

	// ---- M
		result = pi_application->model->CleanupGraphics();

	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::Cleanup()
{
	
	// ---- local
		INT result = 0;
	
	// ---- M
		result = pi_application->model->Cleanup();

	return VS_OK;
}
