//////////////////////////////////////////////////////////////////////////////

// ---------- vs_app_interface.h ----------
/*!
\file vs_app_interface.h
\brief Interface for the VS AppInterface class.
\author Gareth Edwards
\note Vanilla C++
*/

#pragma once


// ---- VS include
	#include "../../../vs/vs_system/header/vs_inc_defs.h"


// ---- VSX include
	#include "../header/vsx_sys_include.h"


// ---- include application class interface header(s) ----
	#define VSX_APPLICATION_INCLUDE
	#include "../../vsx_application/header/vsx_app_setup.h"


// ---- namespace ----
namespace vsx_system
{


// ---------- AppInterfaceSocket ----------
/*!
\brief Union of pointers to applications.
\author Gareth Edwards
*/
union AppInterfaceSocket
{
	#define VSX_APPLICATION_SOCKET
	#include "../../vsx_application/header/vsx_app_setup.h"
};


// ---------- AppInterface ----------
/*!
\brief AppInterface.
\author Gareth Edwards
*/
class AppInterface
{

public:

	// ---- cdtor
		AppInterface();
		virtual ~AppInterface();


	// ---- enumerated frameworks methods
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


	// ---- framework
		INT   Setup();
		INT   Framework(UINT, vs_system::AppDependency *);


	// ---- get
		INT GetDevEnv(std::string *);
		INT GetName(std::string *);
		INT GetVersion(UINT[]);


private:

	// ---- properties
		std::string         dev_env;
		std::string         app_name;
		std::string         config_name;
		UINT                version[4];
		AppInterfaceSocket  socket;
		INT(AppInterface::*ptr_to_framework)(UINT, vs_system::AppDependency*);


	// ---- application framework(s)
		INT application(UINT, vs_system::AppDependency*);


};


}
