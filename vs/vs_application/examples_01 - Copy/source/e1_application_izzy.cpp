////////////////////////////////////////////////////////////////////////////////

// ---------- e1_application.cpp ----------
/*!
\file e1_application.cpp
\brief Implementation of the application class
\author Gareth Edwards
\note Vanilla C++
*/

#include "../header/e1_application.h"
#include "../header/e1_project.h"

using namespace examples_01;


class application::PI_Application
{

public:
	PI_Application();
	~PI_Application();
};


// ---------- ctor ----------
/*!
\brief Constructor
\author Isabelle Sutton
*/
application::PI_Application::PI_Application()
{
	;
}


// ---------- dtor ----------
/*!
\brief Destructor
\author Isabelle Sutton
*/
application::PI_Application::~PI_Application()
{
	;
}


// application static assignments
//application::PI_Application  *application::pi_application = NULL;


// ---------- ctor ----------
/*!
\brief Constructor
\author Isabelle Sutton
*/
application::application()
{
	pi_application = new PI_Application();
}


// ---------- dtor ----------
/*!
\brief Destructor
\author Isabelle Sutton
*/
application::~application()
{
	;
}


// ---------- Setup ----------
/*!
\brief Setup
\author Isabelle Sutton
*/
INT application::Setup(vs_system::AppDependency *app_dependency)
{
	return VS_OK;
}


// ---------- SetupGraphics ----------
/*!
\brief SetupGraphics
\author Isabelle Sutton
*/
INT application::SetupGraphics()
{
	return VS_OK;
}


// ---------- Display ----------
/*!
\brief Display
\author Isabelle Sutton
*/
INT application::Display()
{
	return VS_OK;
}


// ---------- CleanupGraphics ----------
/*!
\brief CleanupGraphics
\author Isabelle Sutton
*/
INT application::CleanupGraphics()
{
	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Isabelle Sutton
*/
INT application::Cleanup()
{
	return VS_OK;
}


