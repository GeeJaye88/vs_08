////////////////////////////////////////////////////////////////////////////////

// ---------- av2_application.h ----------
/*!
\file av2_application.h
\brief Interface for the application class.
\author Gareth Edwards
\note Vanilla C++
*/


#pragma once


// ---- include
#include <stdio.h>   // FILE, 
#include <string.h>  // printf,
#include <ctype.h>   // toupper, 
#include <iostream>


// ---- system
#include "../../../../vs/vs_include/vs_folder.h"
#include "../../../../vs/vs_system/header/vs_inc_defs.h"
#include "../../../../vs/vs_system/header/vs_app_dependency.h"


// ---------- namespace ----------
namespace bmp_utility_01
{

	// ---------- application class ----------
	class application
	{

	public:

		// ---- c/dtor
		application();
		virtual ~application();


		// ---- framework
		INT Setup(vs_system::AppDependency *);
		INT Display();
		INT Cleanup();


		// ---- callback
		static INT CallbackOnFile(vs_system::FolderInfo *);
		static INT CallbackOnFolder(vs_system::FolderInfo *);


		// ---- other
		static INT UpdateVersionFile(vs_system::FolderInfo *);


	private:

		// ----p rivate implementation
		class PI_Application; static PI_Application *pi_app;

	};


}