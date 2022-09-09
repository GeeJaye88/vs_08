//////////////////////////////////////////////////////////////////////////////

// ---------- vs_app_interface.h ----------
/*!
\file vs_app_interface.h
\brief Interface for the VS App(lication)Interface class.
\author Gareth Edwards
*/

#pragma once


// ---- vs include ----
#include "../header/vs_inc_defs.h"


// ---- vs include application class interface header(s) ----
#define VS_APPLICATION_INCLUDE
#include "../../vs_application/header/vs_app_setup.h"


// ---------- namespace ----------
namespace vs_system
{


	// ---------- AppInterfaceSocket class----------
	/*!
	\brief Union of pointers to applications.
	\author Gareth Edwards
	*/
	union AppInterfaceSocket
	{
		// ---- include defined application socket declaration(s) ----
		#define VS_APPLICATION_SOCKET
		#include "../../vs_application/header/vs_app_setup.h"
	};


	// ---------- AppInterface ----------
	/*!
	\brief AppInterface.
	\author Gareth Edwards
	*/
	class AppInterface
	{

	public:

		AppInterface ();
		virtual ~AppInterface ();


		// ---- enumerated frameworks methods ----
		enum METHOD
		{
			UNKNOWN,
			CLEANUP,
			CLEANUP_GRAPHICS,
			DISPLAY,
			SETUP,
			SETUP_GRAPHICS,
			MAXIMUM
		};


		// ---- framework ----
		INT   Setup();
		INT   Framework(UINT, AppDependency *);


		// ---- get ----
		INT GetDevEnv(std::string *);
		INT GetAppName(std::string *);
		INT GetConfigName(std::string *);
		INT GetVersion(UINT[]);


	private:

		// ---- properties ----
		std::string         dev_env;
		std::string         app_name;
		std::string         config_name;
		UINT                version[4];
		AppInterfaceSocket  socket;
		INT (AppInterface::*ptr_to_framework)(UINT, AppDependency*);


		// ---- application framework(s) ----
		INT application(UINT, AppDependency*);

	};


}