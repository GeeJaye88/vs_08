////////////////////////////////////////////////////////////////////////////////

// ---------- e1_application.h ----------
/*!
\file e1_application.h
\brief Interface for the application class.
\author Gareth Edwards
*/

#pragma once

#include "../header/e1_include.h"

namespace examples_01
{


// ---------- application class ----------
class application
{

public:

	// ---- cdtor
		application();
		virtual ~application();


	// ---- framework
		INT Setup(vs_system::AppDependency*);
		INT SetupGraphics();
		INT Display();
		INT CleanupGraphics();
		INT Cleanup();

private:

	// ---- callback display from view
		static INT CallbackDisplayFromView(INT, vs_system::AppPanel *);


	// ---- private implementation
		class PI_Application; static PI_Application *pi_application;

}; 


}
