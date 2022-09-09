////////////////////////////////////////////////////////////////////////////////

// ---------- sca1_application.cpp ----------
/*!
\file sca1_application.cpp
\brief Implementation of the application class
\author Gareth Edwards
*/

//#define _CRT_SECURE_NO_WARNINGS

#include "../header/sca1_application.h"

using namespace scavenger_01;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Application class ----------
class application::PI_Application
{

public:

	// ---- cdtor
		PI_Application();
		~PI_Application();


	// ---- methods
		INT Setup(vs_system::AppDependency *);


	// ---- injected
		vs_system::AppDependency *app_dependency = NULL;
		vs_system::SysContext    *sys_context = NULL;
		vs_system::SysOutput     *sys_output = NULL;


	// ---- paths
		CHAR kept_minute_folders[VS_MAXCHARPATHLEN];
		CHAR supplied_path[VS_MAXCHARPATHLEN];
		CHAR archive_path[VS_MAXCHARPATHLEN];
		CHAR log_path[VS_MAXCHARPATHLEN];


	// ---- command line arguments
		INT argc = 0;
		CHAR *argv[VS_MAXARGV];


	// ---- housekeeping
		INT num_of_iteration = 0;


	// ---- time
		DOUBLE minutes_elapsed = 0;
		INT    minutes_per_report = 1;


	// ---- timeline
		vs_system::Timeline *timeline = NULL;
		vs_system::TimelineInfo::DateTime datetime_minute =
			vs_system::TimelineInfo::DateTime::MINUTE;


	// ---- folder callback
		enum CallbackMode
		{
			UNKNOWN,
			COUNT_FOLDERS,
			REMOVE_FOLDERS
		};
		INT callback_mode = 0;


	// ---- folder report
		BOOL log = TRUE;
		INT  create_status   = 0;
		INT  folders_created = 0;
		INT  folders_deleted = 0;


	// ---- folder numbers
		INT num_of_minute_folders       = 0;
		INT max_num_of_minutes          = 10;
		INT scan_seconds_pause          = 30;
		INT index_del_to_minute_folder  = 0;
		INT index_of_current_folder     = 0;


};


// ---------- Constructor ----------
application::PI_Application::PI_Application()
{
	;
}


// ---------- Destructor ----------
application::PI_Application::~PI_Application()
{
	;
}


// ---------- Setup ----------
INT application::PI_Application::Setup(
		vs_system::AppDependency *ad
	)
{

	// ---- init
		app_dependency = ad;
		sys_context = ad->GetSysContext();
		sys_output = ad->GetSysOutput();


	// ---- timeline
		timeline = new vs_system::Timeline();
		timeline->SetCallbackOnFile(NULL);
		timeline->SetCallbackOnFolder(CallbackFolder);


	// ---- context
		sys_context->SetDWord(sys_context->FPS, 1);


	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


application::PI_Application  * application::pi_app = NULL;

// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::application()
{
	pi_app = new PI_Application();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
application::~application()
{
	if (pi_app) delete pi_app;
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

	// ---- pimp
		result = pi_app->Setup(app_dependency);
		if ( result <= VS_ERROR ) return result;


	// --- command line
		result = Command();
		if ( result <= VS_ERROR ) return result;


	// ---- start a new log file
		result = NewLog();

	return VS_OK;
}


// ---------- Display ----------
/*!
\brief Process
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note

	C++11:

	Using the sleep_for and sleep_until functions:

	#include <chrono>
	#include <thread>

	std::this_thread::sleep_for(std::chrono::milliseconds(10000));

	OR

	using namespace std::this_thread; // sleep_for, sleep_until
	using namespace std::chrono; // nanoseconds, system_clock, seconds
	sleep_for(nanoseconds(10));
	sleep_until(system_clock::now() + seconds(1));

*/
INT application::Display()
{

	// ---- slee-ee-ee-eep
		std::this_thread::sleep_for(std::chrono::milliseconds(pi_app->scan_seconds_pause * 1000));

	// ---- scan
		if (Scan() == VS_ERROR) return VS_COMPLETED;

	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::Cleanup()
{

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


#include "../hpp/sca1_scavenge.hpp"
#include "../hpp/sca1_callback.hpp"
#include "../hpp/sca1_directory.hpp"
#include "../hpp/sca1_get.hpp"


////////////////////////////////////////////////////////////////////////////////
