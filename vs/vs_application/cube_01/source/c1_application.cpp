////////////////////////////////////////////////////////////////////////////////

// ---------- c1_application.cpp ----------
/*!
\file c1_application.cpp
\brief Implementation of the Application class
\author Gareth Edwards
\note 2017
*/

////////////////////////////////////////////////////////////////////////////////


// ---- application
#include "../header/c1_application.h"
#include "../header/c1_model.h"


// ---- namespace
using namespace cube_01;


// ---------- private implementation of PI_Props class ----------
class application::PI_Application
{

public:

	// ---- cdtor
		PI_Application();
		~PI_Application();


	// ---- dependency pointers managed by VS
		vs_system::AppDependency *app_dependency = NULL;
		vs_system::GfxInterface  *gfx            = NULL;
		vs_system::SysContext    *sys_context    = NULL;
		vs_system::SysOutput     *sys_output     = NULL;


	// ---- MVC objects pointers managed by app
		cube_01::Model           *model          = NULL;
		cube_01::Kontrol         *kontrol        = NULL;
		vs_system::AppView       *view           = NULL;


};


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

	// ---- delete MVC objects managed by app
		if ( kontrol != NULL )   { delete kontrol; kontrol     = NULL; }
		if ( view != NULL )      { delete view;    view        = NULL; }
		if ( model != NULL )     { delete model;   model       = NULL; }

}


////////////////////////////////////////////////////////////////////////////////


// application static assignments
application::PI_Application  *application::pi_application = NULL;


////////////////////////////////////////////////////////////////////////////////


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


////////////////////////////////////////////////////////////////////////////////


// ---------- Framework ----------


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT application::Setup(vs_system::AppDependency *app_dependency)
{

	// ---- local
		INT result = 0;


	// ---- store dependency pointers
		pi_application->app_dependency = app_dependency;
		pi_application->gfx = app_dependency->GetGfxInterface();
		pi_application->sys_context    = app_dependency->GetSysContext();
		pi_application->sys_output     = app_dependency->GetSysOutput();


	// ---- allocate, construct, setup and configure ----

		// M
		pi_application->model = new cube_01::Model();
		result = pi_application->model->Setup(app_dependency);

		// V
		pi_application->view = new vs_system::AppView();
		result = pi_application->view->Setup(pi_application->app_dependency);

		// C
		pi_application->kontrol = new cube_01::Kontrol();
		result = pi_application->kontrol->Setup(
				app_dependency,
				pi_application->view,
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

	// --- local
		INT result = VS_OK;


	// ---- model
		result = pi_application->model->SetupFrameset(
				pi_application->app_dependency,
				pi_application->view
			);

	// ---- M
		result = pi_application->model->SetupGraphics();

	return VS_OK ;
}


// ---------- Display ----------
/*!
\brief Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::Display()
{

	// ---- local
		INT result = 0;


	// ---- update control / input
		result = pi_application->kontrol->Update();


	// --- pre-animate
		result = pi_application->model->SetupDisplay();


	// ---- view
		result = pi_application->view->Display(
				(INT)this,
				&CallbackDisplayFromView
			);

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


	// ---- model
		INT result = app->pi_application->model->Display(panel->GetPanelContext(), NULL);

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

	INT result = 0;
	result = pi_application->model->CleanupGraphics();

	return VS_OK;
}


// ---------- Cleanup------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::Cleanup()
{

	INT result = 0;
	result = pi_application->model->Cleanup();

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
