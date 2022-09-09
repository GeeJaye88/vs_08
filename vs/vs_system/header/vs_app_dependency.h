////////////////////////////////////////////////////////////////////////////////

// ---------- vs_app_dependency.h ----------
/*!
\file vs_app_dependency.h
\brief Interface for the App(lication)Dependency class.
\author Gareth Edwards
*/

#pragma once


// ---- vs system ----
#include "../header/vs_inc_defs.h"


// ---- vs graphics ----
#ifdef VS_GFX_FLAG
#include "../header/vs_gfx_interface.h"
#include "../header/vs_sys_input.h"
#endif


// ---- vs sys ----
#include "../header/vs_sys_context.h"
#include "../header/vs_sys_output.h"
#include "../header/vs_sys_input.h"


// ---- vs lib ----
#include "../../vs_library/header/vs_vnode.h"


// ---------- namespace ----------
namespace vs_system
{


	// ---------- AppDependency class ----------
	/*!
	\brief AppDependency.
	\author Gareth Edwards
	*/
	class AppDependency
	{

	public:

		AppDependency();
		~AppDependency();
		VNode        *GetAppConfigData();   // --- NOT USED
		INT           GetAppName(CHAR *);
		INT           GetArgc();
		CHAR         *GetArgv(UINT);
		VNode        *GetSysConfigData();
		VNode        *GetViewConfigData();

		#ifdef VS_GFX_FLAG
		GfxInterface *GetGfxInterface();
		SysInput     *GetSysInput();
		#endif

		SysContext   *GetSysContext();
		SysOutput    *GetSysOutput();
		INT           ReadConfigFiles(CHAR*);
		INT           ReportConfig();
		INT           SetArgc(UINT);
		INT           SetArgv(UINT, CHAR *);

	private:

		class PI_AppDependency; PI_AppDependency *pi_app_dependency;

	};


}