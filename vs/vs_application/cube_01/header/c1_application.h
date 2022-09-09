////////////////////////////////////////////////////////////////////////////////

// ---------- c1_application.h ----------
/*!
\file c1_application.h
\brief Interface for the application class.
\author Gareth Edwards
\note 2018
*/


#pragma once


////////////////////////////////////////////////////////////////////////////////


#include "c1_include.h"


// ---- namespace ----
namespace cube_01
{


// ---------- application class ----------
class application
{

public:

	// ---- c/dtor ----
		application();
		virtual ~application();


	// ---------- framework ----------
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

