////////////////////////////////////////////////////////////////////////////////

// ---------- mount_cal_01.cpp ----------
/*!
\file mount_cal_01.cpp
\brief Implementation of the application class
\author Gareth Edwards
\note Vanilla C++
*/

#define _CRT_SECURE_NO_WARNINGS

// ---------- include
#include "../header/mc1_application.h"
#include "../hpp/mc1_pi_application.hpp"


// ---------- namespace ----------
using namespace mount_calc_01;


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::application()
{
	#ifndef _DEBUG_TESTING
	pi_app = new PI_Application();
	#endif
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
application::~application()
{
	#ifndef _DEBUG_TESTING
	delete pi_app;
	#endif
}


/*
	std::string cdow = getCurrentDirectoryOnWindows() + "\\mcalc.ini";
*/
std::string getCurrentDirectoryOnWindows()
{
	const unsigned long maxDir = 260;
	char currentDir[maxDir];
	GetCurrentDirectory(maxDir, currentDir);
	return std::string(currentDir);
}


/*
	CHAR cdow[VS_MAXCHARLEN];
	getCurrentDirectoryOnWindows(cdow);
*/
VOID getCurrentDirectoryOnWindows(CHAR *cd)
{
	const unsigned long maxDir = 260;
	char currentDir[maxDir];
	GetCurrentDirectory(maxDir, currentDir);
	strcpy_s(cd, VS_MAXCHARLEN, currentDir);
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

	// ---- local
		INT result = VS_OK;

	// --- get working directory
		CHAR cdow[VS_MAXCHARLEN];
		getCurrentDirectoryOnWindows(cdow);

	// ---- trim "\vsx" in debug mode
		if ( IsDebuggerPresent() )
		{
			INT len = (INT)strlen(cdow);
			*(cdow + len - 4) = 0;
		}

	// --- dotini: initialise
		strcat_s(cdow, VS_MAXCHARLEN, "\\mcalc.ini");
		result = pi_app->DotIni_Initialise(cdow);
		if ( result == VS_ERROR ) return VS_EXIT;

	// ---- dotini: read data
		result = pi_app->DotIni_Read();

	// ---- application: modify data
		result = pi_app->Setup(app_dependency);
		if (result == VS_EXIT) return VS_EXIT;

	// ---- dotini: save data
		result = pi_app->DotIni_Write();

	return result;
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
		INT result = VS_OK;

	// ---- report & make mount
		result = pi_app->Report();
		result = pi_app->Mount();

	return VS_COMPLETED;
}


// ---------- Cleanup ----------
/*!
\brief Setup
\author Gareth Edwards
\param INT [in] number of arguments
\param CHAR* [in] argument list
\return INT - ERROR <=0 < VS_OK
*/
INT application::Cleanup()
{
	return VS_OK;
}