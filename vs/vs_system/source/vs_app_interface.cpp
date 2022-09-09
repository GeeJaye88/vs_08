////////////////////////////////////////////////////////////////////////////////

// ---------- vs_app_interface.cpp ----------
/*!
\file vs_app_interface.cpp
\brief Implementation of the App(lication)Interface class
\author Gareth Edwards
*/

// ---- include ----
#include "../header/vs_app_interface.h"


// ---- namespace ----
using namespace vs_system;


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\param CHAR * - name of application
*/
AppInterface::AppInterface ()
{

	// ---- set dev env and name ----
	#define VS_APPLICATION_APPNAME
	#include "../../vs_application/header/vs_app_setup.h"

	// ---- set version ----
	#define VS_APPLICATION_APPVERSION
	#include "../../vs_application/header/vs_app_setup.h"

};


// ---------- Destructor ----------
AppInterface::~AppInterface () { };


// ---------- Setup ----------
/*!
\brief Invokes the application interface framework method.
\author Gareth Edwards
\return int - VS_ERROR <= 0 < VS_OK
*/
INT AppInterface::Setup()
{

	// IF -- set framework succeeds -- THEN

	#define VS_APPLICATION_POINTER
	#include "../../vs_application/header/vs_app_setup.h"

	// ELSE -- fails, report and return error
	else
	{
		::MessageBox(
				0,
				"Error : AppInterface::AppInterface() - UNKNOWN",
				app_name.c_str(),
				MB_ICONERROR
			);
		return VS_ERROR;
	}

	return VS_OK;
}


// ---------- GetDevEnv ----------
/*!
\brief Get the development environment name
\author Gareth Edwards
\param std::string - name of selected application
\return int - VS_ERROR <= 0 < VS_OK
*/
INT AppInterface::GetDevEnv(std::string *de)
{
	de->assign(dev_env);
	return VS_OK;
}


// ---------- GetAppName ----------
/*!
\brief Get the application name
\author Gareth Edwards
\param std::string - name of selected application
\return int - VS_ERROR <= 0 < VS_OK
*/
INT AppInterface::GetAppName(std::string *n)
{
	n->assign(app_name);
	return VS_OK;
}


// ---------- GetConfigName ----------
/*!
\brief Get the config name
\author Gareth Edwards
\param std::string - name of selected application
\return int - VS_ERROR <= 0 < VS_OK
*/
INT AppInterface::GetConfigName(std::string *s)
{
	s->assign(config_name);
	return VS_OK;
}
 

// ---------- GetVersion ----------
/*!
\brief Get the application version
\author Gareth Edwards
\param UINT [] - version of selected application
\return int - VS_ERROR <= 0 < VS_OK
*/
INT AppInterface::GetVersion(UINT v[])
{
	v[0] = version[0];
	v[1] = version[1];
	v[2] = version[2];
	v[3] = version[3];
	return VS_OK;
}


// ---------- AppInterface framework interface ----------
/*!
\brief Invokes the application interface framework method.
\author Gareth Edwards
\param unsigned int - enumerated framework method
\param AppDependency * - application dependency
\return int - VS_ERROR <= 0 < VS_OK
*/
INT AppInterface::Framework (
		UINT method,
		AppDependency *app_dependency
	)
{
	int result = NULL;
	if ( ptr_to_framework != NULL )
	{
		result =
			(this->*ptr_to_framework)
				(method, app_dependency);
	}
	return result;
}


// ---------- AppInterface framework ----------
/*!
\brief Invoke an application framework method.
\author Gareth Edwards
\param unsigned int - enumerated framework method
\param AppDependency * - application dependency
\return int - VS_ERROR <= 0 < VS_OK
*/
int AppInterface::application (
		UINT method,
		AppDependency *app_dependency
	)
{
	int result = VS_OK; 
	switch ( method )
	{
		case SETUP :
			result = socket.application->Setup(app_dependency);
			break;
		case SETUP_GRAPHICS :
			result = socket.application->SetupGraphics();
			break;
		case DISPLAY :
			result = socket.application->Display();
			break;
		case CLEANUP_GRAPHICS :
			result = socket.application->CleanupGraphics();
			break;
		case CLEANUP :
			result = socket.application->Cleanup();
			break;
		default:
			result = VS_ERROR;
			break;
	}
	return result;
};


////////////////////////////////////////////////////////////////////////////////
