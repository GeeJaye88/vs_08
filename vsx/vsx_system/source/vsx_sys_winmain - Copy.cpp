////////////////////////////////////////////////////////////////////////////////

// ---------- vsx_sys_winmain.cpp ----------
/*!
\file vsx_sys_winmain.cpp
\brief windows "main" entry point and functions
\author Gareth Edwards
\note Vanilla C++
*/

/*
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
*/

// ---- include ----
#include "../header/vsx_sys_winmain.h"


// ---- resources ----
//#include "../../vsx.rc"


// ---- VirtualSurface ----
#include "../../../vs/vs_system/header/vs_sys_vrtsrf.h"


////////////////////////////////////////////////////////////////////////////////


// ---------- MAIN ----------
int main(INT argc, CHAR* argv[])
{
	// ---- local ----
	INT result = VS_OK;


	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	// ---- license -----
	vs_system::SysLicense sys_license;
	result = sys_license.Valid();
	if ( result == VS_ERROR ) return VS_ERROR;


	// ---- framework ----
	result = Setup(argc, argv);
	if ( result == VS_OK )
	{
		result = Initialise();
		if ( result == VS_OK )
		{
			result = Display();
			result = Cleanup();
		}
	}

	//_CrtDumpMemoryLeaks();

	return result;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param INT [in] number of arguments
\param CHAR* [in] argument list
\return INT - ERROR <=0 < VS_OK
*/
INT Setup(INT argc, CHAR* argv[])
{

	// ---- local ----
	INT result = 0;


	// ---- set application dev env ----
	CHAR c_dev_env[VS_MAXCHARNAME];
	{
		std::string s_dev_env;
		virtual_surface.app_interface.GetDevEnv(&s_dev_env);
		strcpy(c_dev_env, s_dev_env.c_str());
		result = virtual_surface.sys_context->SetChar(
				vs_system::SysContext::DEV_ENV,
				c_dev_env
			);
	}


	// ---- set application name ----
	CHAR c_app_name[VS_MAXCHARNAME];
	{
		std::string s_app_name;
		virtual_surface.app_interface.GetName(&s_app_name);
		strcpy(c_app_name, s_app_name.c_str());
		result = virtual_surface.sys_context->SetChar(
				vs_system::SysContext::APP_NAME,
				c_app_name
			);
	}


	// ---- set application version ----
	{
		UINT app_version[4];
		result = virtual_surface.app_interface.GetVersion(app_version);
		result = virtual_surface.sys_context->SetDWord(
				virtual_surface.sys_context->APP_VERSION_MAJOR,
				app_version[0]
			);
		result = virtual_surface.sys_context->SetDWord(
				virtual_surface.sys_context->APP_VERSION_MINOR,
				app_version[1]
			);
		result = virtual_surface.sys_context->SetDWord(
				virtual_surface.sys_context->APP_VERSION_PATCH,
				app_version[2]
			);
		result = virtual_surface.sys_context->SetDWord(
				virtual_surface.sys_context->APP_VERSION_BUILD,
				app_version[3]
			);
	}


	// ---- development mode ----
	{
		char *dev_env = virtual_surface.sys_context->GetChar(vs_system::SysContext::DEV_ENV);
		CHAR  current_directory_path[VS_MAXCHARPATHLEN];
		char *cwd = getcwd(current_directory_path, VS_MAXCHARPATHLEN);
		INT path_len = strlen(current_directory_path);
		INT dev_env_len = strlen(dev_env);
		if (!strcmp(&current_directory_path[path_len - dev_env_len], dev_env))
		{
			INT result = virtual_surface.sys_context->SetBool(
					virtual_surface.sys_context->DEVELOPMENT,
					TRUE
				);
		}
	}


	// ---- store argument count and values ----
	virtual_surface.app_dependency.SetArgc(argc);
	for (INT i=0; i<argc; i++)
	{
		virtual_surface.app_dependency.SetArgv(i, argv[i]);
	}


	// ---- TRY to setup application ----
	INT error_code = VS_OK;
	try
	{
		error_code = virtual_surface.app_interface.Setup();
		if ( error_code <= VS_ERROR)
			throw((UINT)1);

		return VS_OK;
	}
	// catch, report and return
	catch ( UINT error_id )
	{
		virtual_surface.sys_output->MessageOk(error_id, error_code);
		return VS_ERROR;
	}


	return VS_OK;
}


// ---------- Initialise ----------
/*!
\brief Initialise
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Initialise()
{

	// ---- TRY to initialise ----
	INT error_code = VS_OK;
	try
	{

		// ---- set application name ----
		char *app_name = virtual_surface.sys_context->GetChar(vs_system::SysContext::APP_NAME);


		// ---- !!! CAMERA !!! ----
		//
		// BAD BAD BAD - TBD
		//
		BOOL read_config_files = FALSE;
		{
			// IF a "camera_" application THEN set app_name == "camera_03"
			CHAR camera_[VS_MAXCHARNAME];
			strcpy(camera_, app_name);
			camera_[7] = NULL;
			if ( !strcmp(camera_, "camera_") )
				app_name = "camera_03";

			// IF "camera_03" application THEN set read config files flag TRUE
			if ( !strcmp(app_name, "camera_03") )
			{
				read_config_files = TRUE;
			}
		}


		// ---- READ CONFIG FILES ----

		if ( read_config_files )
		{
			error_code = virtual_surface.app_dependency.ReadConfigFiles(
				virtual_surface.sys_context->GetChar(
					virtual_surface.sys_context->APP_NAME
					)
				);
				if ( error_code <= VS_ERROR )
					throw ((UINT)4);
		}


		// ---- CONTEXT ----

		error_code = virtual_surface.sys_context->Configure(
			virtual_surface.app_dependency.GetSysConfigData()
			);
			if (error_code <= VS_ERROR)
				throw ((UINT)5);


		// ---- OUTPUT ----

		virtual_surface.sys_output->SetDebug(
				virtual_surface.sys_context->GetBool(virtual_surface.sys_context->DEBUG)
			);

		virtual_surface.sys_output->SetLog(
				virtual_surface.sys_context->GetBool(virtual_surface.sys_context->LOG)
			);

		error_code = virtual_surface.sys_output->Initialise(
				(UINT)virtual_surface.sys_context->GetDWord(virtual_surface.sys_context->LOG_FILES)
			);

		error_code = virtual_surface.app_dependency.ReportConfig();

	}

	// catch, report and return
	catch ( UINT error_id )
	{
		virtual_surface.sys_output->MessageOk(error_id, error_code);
		return VS_ERROR;
	}


	// ... and finally - increase priority
	HANDLE handle = (HANDLE)virtual_surface.sys_context->GetDWord(
			virtual_surface.sys_context->WINDOW_HANDLE
		);
	BOOL ok = SetPriorityClass(handle, ABOVE_NORMAL_PRIORITY_CLASS);


	return VS_OK;
}


// ---------- ConsoleHandler ----------
/*!
\brief ConsoleHandler 
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
BOOL WINAPI ConsoleHandler(DWORD CEvent)
{

	if (SetConsoleCtrlHandler(
		(PHANDLER_ROUTINE)ConsoleHandler, TRUE) == FALSE)
	{
		// unable to install handler... 
		// display message to the user
		printf("Unable to install handler!\n");
		return -1;
	}

	switch (CEvent)
	{
		case CTRL_C_EVENT:
			//MessageBox(NULL,
			//	"CTRL+C received!", "CEvent", MB_OK);
			break;
		case CTRL_BREAK_EVENT:
			MessageBox(NULL,
				"CTRL+BREAK received!", "CEvent", MB_OK);
			break;
		case CTRL_CLOSE_EVENT:
			MessageBox(NULL,
				"Program being closed!", "CEvent", MB_OK);
			break;
		case CTRL_LOGOFF_EVENT:
			MessageBox(NULL,
				"User is logging off!", "CEvent", MB_OK);
			break;
		case CTRL_SHUTDOWN_EVENT:
			MessageBox(NULL,
				"User is logging off!", "CEvent", MB_OK);
			break;
	}

	return TRUE;
}


// ---------- Display ----------
/*!
\brief Setup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Display()
{
	// ---- local ----
	INT result = VS_OK;


	// ---- TRY framework ----
	INT error_code = VS_OK;
	try
	{

		// ---- setup fails THEN throw exception ----
		error_code = virtual_surface.app_interface.Framework(
				virtual_surface.app_interface.SETUP,
				&virtual_surface.app_dependency
			);
			if (error_code <= VS_ERROR)
				throw((UINT)8);


		// ---- loop application until "quit" ----
		error_code = DisplayLoop();


		// ---- IF application cleanup fails THEN throw exception ----
		error_code = virtual_surface.app_interface.Framework(
				virtual_surface.app_interface.CLEANUP,
				0
			);
			if (error_code <= VS_ERROR)
				throw((UINT)12);
	}

	// ---- catch, report and return ----
	catch ( UINT error_id )
	{
		virtual_surface.sys_output->MessageOk(error_id, error_code);
		return VS_ERROR;
	}

	return VS_OK;
}


// ---------- DisplayLoop ----------
/*!
\brief DisplayLoop
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT DisplayLoop()
{
	// ---- frame counters ----
	ULONG elapsed_seconds = 0;
	UINT  frames_per_second = 0;
	UINT  frame_number = 0;


	// ---- millisecond counters ----
	DOUBLE start_time = (double)timeGetTime();
	DOUBLE next_time  = 0;


	// ---- set target time interval ----
	double time_Interval =
			1 /
			(double)virtual_surface.sys_context->GetDWord(
				virtual_surface.sys_context->FPS
		)
		* 1000;


	// ---- set frame INTERVAL time ----
	virtual_surface.sys_context->SetDouble(
			virtual_surface.sys_context->TIME_MS_DELTA,
			time_Interval
		);


	// ---- loop ----
	UINT status = VS_OK;
	while ( status == VS_OK )
	{

		// ---- get current time ----
		DOUBLE curr_time  = (double)timeGetTime() - start_time;

		
		// ---- IF current time exceeds next time THEN invoke application display method
		if ( curr_time > next_time )
		{

			// ---- TRY to display ----
			INT error_code = VS_OK;
			try
			{

				// IF display fails THEN report
				error_code = virtual_surface.app_interface.Framework(
					virtual_surface.app_interface.DISPLAY, 0);
						if ( error_code <= VS_ERROR )
							throw ((UINT)12);


				// IF application completed THEN return (and finish)
				if ( error_code == VS_COMPLETED )
					return VS_OK;


				// update next time
				next_time += time_Interval;


				// increment frame number and fps
				frame_number++;
				frames_per_second++;


				// report
				ULONG now = (ULONG)( curr_time / 1000 );
				if ( now != elapsed_seconds )
				{
					/*
					printf("FPS: %d @ Frame # %d\n", frames_per_second, frame_number);
					*/
					elapsed_seconds = now;
					frames_per_second = 0;
				}
			}

			catch ( UINT error_id )
			{
				printf("Error: Application process returned error code %d!\n", error_id);
				return VS_ERROR;

			}

		}

	}

	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Cleanup()
{
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////