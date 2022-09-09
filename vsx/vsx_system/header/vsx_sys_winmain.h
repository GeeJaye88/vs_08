////////////////////////////////////////////////////////////////////////////////

// ---------- vsx_sys_winmain.h ----------
/*!
\file vsx_sys_winmain.h
\brief windows "main" entry point
\author Gareth Edwards
\note Vanilla C++
*/

#pragma once

#define _CRT_SECURE_NO_WARNINGS


// ---- VS
	#include "../../../vs/vs_system/header/vs_inc_defs.h"
	#include "../../../vs/vs_system/header/vs_app_dependency.h"
	#include "../../../vs/vs_system/header/vs_sys_output.h"
	#include "../../../vs/vs_system/header/vs_sys_context.h"
	#include "../../../vs/vs_system/header/vs_sys_license.h"


// ---- VSX
	#include "../header/vsx_app_interface.h"


// ---- framework functions
	INT Setup(INT , CHAR* []);
	INT Initialise();
	INT Display();
	INT DisplayLoop();
	INT Cleanup();
