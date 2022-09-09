////////////////////////////////////////////////////////////////////////////////

// ---------- e1_application.cpp ----------
/*!
\file e1_application.cpp
\brief Implementation of the application class
\author Gareth Edwards
\note
*/

#include "../header/s1_application.h"

using namespace surface_01;


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
	vs_system::AppView       *app_view       = NULL;


	// ---- objects managed by app
	Kontrol *kontrol = NULL;
	Model   *model   = NULL;

};


// ---------- Display ----------
/*!
\brief Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::Display()
{

	// ---- local
		INT result = VS_OK;
		INT handle_scene = 0;


	// ---- C - update control by touch/mouse/keyboard
		result = pi_application->kontrol->Update();


	// ---- M - setup / pre-animate
		result = pi_application->model->Display();


	// ---- V - render frame->panel(s)->display lists(s)
		result = pi_application->app_view->Display(
				(INT)this,
				&CallbackDisplayFromView
			);

	return VS_OK;
}


// ---------- Everthing else... ----------
#include "../../surface_shared/hpp/application_class.hpp"

