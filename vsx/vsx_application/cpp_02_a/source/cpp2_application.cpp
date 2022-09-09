////////////////////////////////////////////////////////////////////////////////

// ---------- cpp2_application.cpp ----------
/*!
\file cpp2_application.cpp
\brief Implementation of the application class
\author Gareth Edwards
\note Vanilla C++
*/

#define _CRT_SECURE_NO_WARNINGS

#include "../header/cpp2_application.h"
#include "../hpp/cpp2_application.hpp"
#include "../hpp/cpp2_0_examples.hpp"


////////////////////////////////////////////////////////////////////////////////


// ---------- static properties ----------
application::PI_Application  * application::pi_app = NULL;


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::application()
{
	pi_app = new PI_Application();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
application::~application()
{
	if ( pi_app ) delete pi_app;
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param INT [in] number of arguments
\param CHAR* [in] argument list
\return INT - ERROR <=0 < VS_OK
*/
INT application::Setup(vs_system::AppDependency *app_dependency)
{

	INT result = VS_OK;
	result = pi_app->Setup(app_dependency);
	if (result <= VS_ERROR)
	{
		return result;
	}

	return VS_OK;
}


// ---------- Display ----------
/*!
\brief Process
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::Display()
{

	return VS_OK; // VS_COMPLETED;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::Cleanup()
{
	return VS_OK;
}

