////////////////////////////////////////////////////////////////////////////////

// ---------- as2_application.h ----------
/*!
\file as2_application.h
\brief Interface for the application class.
\author Gareth Edwards
\note Vanilla C++
*/

#pragma once

// ---- std ----
	#include <stdio.h>   // FILE, 
	#include <string.h>  // printf,
	#include <ctype.h>   // toupper, 


// ---- VS ----
	#include "../../../../vs/vs_system/header/vs_inc_defs.h"
	#include "../../../../vs/vs_system/header/vs_app_dependency.h"


// ---------- VSX ----------
	#include "../../../vsx_system/header/vsx_sys_include.h"


// ---------- namespace ----------
namespace app_setup_02
{

	// ---- application class
	class application
	{

		public:

			// ---- c/dtor
				application();
				virtual ~application();


			// ---- framework methods
				INT Setup(vs_system::AppDependency *);
				INT Display();
				INT Cleanup();
				INT Usage();

				INT GetVersionInfo(CHAR *);
		
				INT ShowDefinitions(CHAR *);

				INT WriteAppInterfaceSetupFile();
					INT Step_0_Header();
					INT Step_1_DefineAppHashName();
					INT Step_2_InitialiseAppName();
					INT Step_3_IncludeAppVersion();
					INT Step_4_IncludeAppHeader();
					INT Step_5_InitialiseAppSocket();
					INT Step_6_InitialiseAppInterface();

				INT WriteAppDirectivesFile();
					INT AppDirectives();

				INT WriteVsRcFile();
					INT VsRc();
					INT VsxRc();

	private:

		// ---- private implementation
			class PI_Application; PI_Application *pi_app;

	};

}
