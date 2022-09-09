////////////////////////////////////////////////////////////////////////////////

// ---------- sca1_application.h ----------
/*!
\file sca1_application.h
\brief Interface for the application class.
\author Gareth Edwards
*/

#pragma once


// ---- include
	#include <stdio.h>   // FILE, 
	#include <string.h>  // printf,
	#include <string>
	#include <ctype.h>   // toupper, 
	#include <iostream>

	#include <chrono>    // sleep_for
	#include <thread>    // std::this_thread::sleep_for


// ---- system

	#include "../../../../vs/vs_system/header/vs_inc_defs.h"
	#include "../../../../vs/vs_system/header/vs_app_dependency.h"
	#include "../../../../vs/vs_library/header/vs_char_proc.h"
	#include "../../../../vs/vs_library/header/vs_vnode.h"
	#include "../../../../vs/vs_include/vs_folder.h"
	#include "../../../../vs/vs_include/vs_timeline.h"


// ---------- namespace ----------
namespace scavenger_01
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


		// ---- sub_framework
			INT NewLog();
			INT Command();
			INT Scan();
			INT Report();


		// ---- directory
			static INT DeleteDirectory(
					const std::string &refcstrRootDirectory,
					BOOL delete_subdirectories
				);

			static INT DeleteEmptyDirectories(
					const std::string &refcstrRootDirectory
				);

			static BOOL DirectoryExists(
				const std::string &refcstrRootDirectory
			);


		// ---- get
			INT  GetObservantPath(CHAR *);
			INT  GetArchivePath(CHAR *, CHAR *);
			LONG GetMinutesElapsed();
			INT  GetTimeStamp(CHAR *);


		// ---- callbacks
			static INT CallbackFile(vs_system::TimelineInfo *info);
			static INT CallbackFolder(vs_system::TimelineInfo *info);

	private:

		// ---- pimp
			class PI_Application; static PI_Application *pi_app;

	};

}