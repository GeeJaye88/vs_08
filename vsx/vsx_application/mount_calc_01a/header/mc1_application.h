////////////////////////////////////////////////////////////////////////////////

// ---------- mount_calc_01.h ----------
/*!
\file mount_calc_01.h
\brief Interface for the application class.
\author Gareth Edwards
\note Vanilla C++
*/

#pragma once


// ---- std ----
	#include <stdio.h>   // FILE, 
	#include <string.h>  // printf,
	#include <ctype.h>   // toupper, 

// ---- vs system
	#include <../vs_07/vs/vs_system/header/vs_inc_defs.h>
	#include <../vs_07/vs/vs_system/header/vs_inc_maths.h>

#ifndef TESTING_4_BUG

#include <../vs_07/vs/vs_system/header/vs_app_dependency.h>


// ---- vs library
	#include <../vs_07/vs/vs_library/header/vs_img_jpeglib.h>
	#include <../vs_07/vs/vs_library/header/vs_img_buffer.h>
	#include <../vs_07/vs/vs_library/header/vs_vnode.h>
// ---------- vsx ----------
	#include <../vs_07/vsx/vsx_system/header/vsx_sys_include.h>



// ---------- application
	#include <../vs_07/vsx/vsx_application/mount_calc_01/header/vs_dot_ini.h>

#endif

// ---------- namespace ----------
namespace mount_calc_01
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

	private:

		// ---- private implementation
		class PI_Application; PI_Application *pi_app;

	};

}

