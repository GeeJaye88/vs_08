////////////////////////////////////////////////////////////////////////////////

// ---------- vs_sys_winmain.cpp ----------
/*!
\file vs_sys_winmain.cpp
\brief windows "main" entry point and functions
\author Gareth Edwards
*/

// ---- defs, console, inpuit, output, gfx, app and vrtsrf ----
#include "../header/vs_sys_winmain.h"


// ---- resources ----
#include "../../vs_resources/resource.h"


// ---- JE: for winsock stuff ----
#if _MSC_VER
#pragma comment( lib, "ws2_32.lib")
#endif


// ---- function prototypes ----
INT WINAPI WinMain(HINSTANCE, HINSTANCE, PSTR, INT);
INT WinInitialise(HINSTANCE);
INT WinFramework();
INT WinMessagePump();
INT WinCreate(HINSTANCE);
LRESULT	CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
INT WinResetGraphicsDevice(UINT, UINT);
INT WinResources();


////////////////////////////////////////////////////////////////////////////////


// ---------- WinMain ----------
/*!
\brief Application entry point

\param HINSTANCE - Is a handle to the applications instance, where an
instance can be considered to be a single run of your application.
The instance is used by windows as a reference to your application
for event handling, message processing, and various other duties.
\param HINSTANCE - Is always NULL.
\param PSTR - Is a pointer string that is used to hold any command-line
       arguments that may have been specified when the application began.  
\param INT - Is the parameter that determines how the application's
       window will be displayed once it begins executing.
\return INT - ERROR <= 0 < VS_OK
\note For example, If the user opened the Run application and typed
      myapp.exe myparameter 1, then lpCmdLine would be myparameter 1.

*/

// ---------- JE: this is a quick fix ----------
//
// There is a Chordia class to do this in wsinit.h
// but I don't want to add an unnecessary .h include
// to your VS system. Hence this class which also handles shutdown.
//
class WSI
{

public:

	WSI()
	{
		#ifdef _MSC_VER

		// ---- initialize Winsock
			WSADATA wsaData;

			// ---- winsock 2.0 ----
			int result = WSAStartup(MAKEWORD(2, 0), &wsaData);
			if (result != 0) {
				OutputDebugString("WSAStartup failed.\n");
				throw std::exception("WSAStartup failed.\n");
			}

		#endif
	}

	~WSI()
	{
		#ifdef _MSC_VER
		WSACleanup();
		#endif
	}
};


// ---------- WinMain (continued...) ----------
INT WINAPI WinMain(
		HINSTANCE	hinstance,
		HINSTANCE	prevInstance,
		PSTR		cmdLine,
		INT			showCmd
	)
{

	// ---- local
		INT result = 0;


	// ---- JE: initialize Winsock right from the get-go
		WSI wsi;


	// ---- 1. set application dev env
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


	// ---- 2. set application name
		CHAR c_app_name[VS_MAXCHARNAME];
		{
			std::string s_app_name;
			virtual_surface.app_interface.GetAppName(&s_app_name);
			strcpy(c_app_name, s_app_name.c_str());
			result = virtual_surface.sys_context->SetChar(
					vs_system::SysContext::APP_NAME,
					c_app_name
				);
		}


	// ---- 3. set config name
		CHAR c_config_name[VS_MAXCHARNAME];
		{
			std::string s_config_name;
			virtual_surface.app_interface.GetConfigName(&s_config_name);
			strcpy(c_config_name, s_config_name.c_str());
			result = virtual_surface.sys_context->SetChar(
					vs_system::SysContext::CONFIG_NAME,
					c_config_name
				);
		}


	// ---- 4. set application version
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


	// ---- 5. development mode (cunning)
		if ( IsDebuggerPresent() )
		{
			result = virtual_surface.sys_context->SetBool(
					virtual_surface.sys_context->DEVELOPMENT,
					TRUE
				);
		}


	// ---- 6. development mode
		{
			CHAR *dev_env = virtual_surface.sys_context->GetChar(vs_system::SysContext::DEV_ENV);
			CHAR  current_directory_path[VS_MAXCHARPATHLEN];
			char *cwd       = getcwd(current_directory_path, VS_MAXCHARPATHLEN);
			INT path_len    = (INT)strlen(current_directory_path);
			INT dev_env_len = (INT)strlen(dev_env);
			if (!strcmp(&current_directory_path[path_len - dev_env_len], dev_env))
			{
				result = virtual_surface.sys_context->SetBool(
						virtual_surface.sys_context->DEVELOPMENT,
						TRUE
					);
			}
		}


	// ---- 7. TRY to launch application
		INT error_code = VS_OK;
		try
		{
			error_code = virtual_surface.app_interface.Setup();
				if ( error_code <= VS_ERROR )
					throw(1);

			if ( virtual_surface.sys_context->GetBool(virtual_surface.sys_context->APP_LICENSED) )
			{
				vs_system::SysLicense sys_license;
				result = sys_license.Valid();
				if (result == VS_ERROR) return VS_ERROR;
			}

			error_code = WinInitialise(hinstance);
				if ( error_code <= VS_ERROR )
					throw(2);

			error_code = WinFramework();
				if ( error_code <= VS_ERROR )
					throw(3);

			return VS_OK;
		}

	// ---- catch, report and return
		catch ( INT error_id )
		{
			virtual_surface.sys_output->MessageOk(error_id, error_code);
			exit(0);
		}

	return VS_OK;
}


// ---------- WinInitialise ----------
/*!
\brief Initialise invoked by WinMain
\author Gareth Edwards
\param HINSTANCE - handle
\return INT - ERROR <= VS_ERROR < VS_OK
*/
INT WinInitialise(HINSTANCE hinstance)
{
 
	// TRY to initialise
	INT result = VS_OK;
	INT error_code = VS_OK;
	try
	{

		// ---- 1. read config files 
			//
			// IF read config file ("[rel]data/[appname]/config.hdl")
			//    fails THEN throw excepetion
			//
			error_code=virtual_surface.app_dependency.ReadConfigFiles(
					virtual_surface.sys_context->GetChar(
						virtual_surface.sys_context->APP_NAME
					)
				);
				if ( error_code <= VS_ERROR )
					throw (4);


		// ---- 2. context
			error_code = virtual_surface.sys_context->Configure(
					virtual_surface.app_dependency.GetSysConfigData()
				);
				if (error_code <= VS_ERROR)
					throw (5);


		// ---- 3. output
			virtual_surface.sys_output->SetDebug(
					virtual_surface.sys_context->GetBool(virtual_surface.sys_context->DEBUG)
				);

			virtual_surface.sys_output->SetLog(
					virtual_surface.sys_context->GetBool(virtual_surface.sys_context->LOG)
				);

			virtual_surface.sys_output->SetLogTime(
				virtual_surface.sys_context->GetBool(virtual_surface.sys_context->LOG_TIME)
				);

			error_code = virtual_surface.sys_output->Initialise(
					(UINT)virtual_surface.sys_context->GetDWord(virtual_surface.sys_context->LOG_FILES)
				); // no test for error_code required


		// ---- 4. graphics
			error_code = virtual_surface.gfx_interface->SetAAQ(
					virtual_surface.sys_context->GetDWord(virtual_surface.sys_context->AAQ)
				);
			error_code = virtual_surface.gfx_interface->SetFPS(
					virtual_surface.sys_context->GetDWord(virtual_surface.sys_context->FPS)
				);
			VsColour c = {
					virtual_surface.sys_context->GetFloat(virtual_surface.sys_context->BG_RED),
					virtual_surface.sys_context->GetFloat(virtual_surface.sys_context->BG_GREEN),
					virtual_surface.sys_context->GetFloat(virtual_surface.sys_context->BG_BLUE),
					0
				};
			error_code = virtual_surface.gfx_interface->SetBackground(&c);
			error_code = virtual_surface.gfx_interface->SetSysContext(virtual_surface.sys_context);
			error_code = virtual_surface.gfx_interface->SetSysInput(virtual_surface.sys_input);
			error_code = virtual_surface.gfx_interface->SetSysOutput(virtual_surface.sys_output);


		// ---- 5. windows
			error_code = WinCreate(hinstance);
				if ( error_code <= VS_ERROR )
					throw (6);


		// ---- 6. console (deprecated from VS 2017)
			/*
			if ( virtual_surface.sys_context->GetBool(virtual_surface.sys_context->CONSOLE) )
				vs_system::SysConsole *sys_console = new vs_system::SysConsole();
			*/


		// ---- 7. graphics (again!)
			#if defined(TEST_GFX)
			error_code = virtual_surface.app_interface.Framework(
					virtual_surface.app_interface.SETUP,
					&virtual_surface.app_dependency
				);
				if ( error_code <= VS_ERROR )
					throw(8);
			#else

			// ---- init gfx
				error_code = virtual_surface.gfx_interface->Initialise(
						GetActiveWindow(),
						virtual_surface.sys_context->GetBool(virtual_surface.sys_context->WINDOWED),
						virtual_surface.sys_context->GetDWord(virtual_surface.sys_context->CLIENT_WIDTH),
						virtual_surface.sys_context->GetDWord(virtual_surface.sys_context->CLIENT_HEIGHT)
					);
					if (error_code <= VS_ERROR)
						throw (7);

			// ---- format
				DWORD width  = virtual_surface.sys_context->GetDWord(vs_system::SysContext::CLIENT_WIDTH);
				DWORD height = virtual_surface.sys_context->GetDWord(vs_system::SysContext::CLIENT_HEIGHT);
				if ( width < height )
				{
					virtual_surface.sys_context->SetDWord(
							vs_system::SysContext::FORMAT,
							vs_system::SysContext::PORTRAIT
						);
				}

			#endif


		// ---- 8. resources
			result = WinResources();

	}

	// ---- catch, report and return
		catch ( UINT error_id )
		{
			virtual_surface.sys_output->MessageOk(error_id, error_code);
			return VS_ERROR;
		}


	// ---- ... and finally - increase priority
		HANDLE handle = (HANDLE)virtual_surface.sys_context->GetDWord(
										virtual_surface.sys_context->WINDOW_HANDLE
									);
		BOOL ok = SetPriorityClass(handle, ABOVE_NORMAL_PRIORITY_CLASS);

	return VS_OK;
}


// ---------- WinFramework ----------
/*!
\brief Framework invoked by WndMain
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT WinFramework()
{

	// ---- local
		INT result = VS_OK;


	// ---- TRY framework
		INT error_code = VS_OK;
		try
		{
			#if defined(TEST_GFX)
			;
			#else
			error_code = virtual_surface.app_interface.Framework(
					virtual_surface.app_interface.SETUP,
					&virtual_surface.app_dependency
				);
				if ( error_code <= VS_ERROR )
					throw(8);
			#endif

			//result = virtual_surface.gfx_interface->SetDisplayListActiveState();

			error_code = virtual_surface.app_interface.Framework(
					virtual_surface.app_interface.SETUP_GRAPHICS,
					0
				);
				if ( error_code <= VS_ERROR )
					throw(9);

			error_code = WinMessagePump();

			error_code = virtual_surface.app_interface.Framework(
					virtual_surface.app_interface.CLEANUP_GRAPHICS,
					0
				);
				if ( error_code <= VS_ERROR )
					throw(11);

			error_code = virtual_surface.app_interface.Framework(
					virtual_surface.app_interface.CLEANUP,
					0
				);
				if ( error_code <= VS_ERROR )
					throw(12);

			error_code = virtual_surface.app_dependency.GetGfxInterface()->CleanupGraphics();
				if ( error_code <= VS_ERROR )
					throw(13);

		}

	// ---- catch, report and return
		catch ( INT error_id )
		{
			virtual_surface.sys_output->MessageOk(error_id, error_code);
			return error_code;
		}

	return VS_OK;
}


// ---------- WinMessagePump ----------
/*!
\brief Main message pump invoked by WinFramework 
\author Gareth Edwards
\return INT - ERROR <= 0 < msg.wParam
*/
INT WinMessagePump()
{

	// ---- fps
		#if defined(VS_DEBUG_FLAG)
		DOUBLE time_elapsed_ms_last = 0;
		UINT fps = 0;
		#endif


	// ---- Msg every Ten Second
		#if defined(VS_DEBUG_FLAG)
		CHAR mts[VS_MAXCHARLEN];
		sprintf(mts, " ins:%s  dev:%s",
				virtual_surface.sys_context->GetBool(virtual_surface.sys_context->INSTALL_DATA ) == 0 ? "OFF" : "ON",
				virtual_surface.sys_context->GetBool(virtual_surface.sys_context->DEVELOPMENT  ) == 0 ? "OFF" : "ON"
			);
		UINT tsc = 0; // Ten Sec Count
		#endif


	// ---- message
		MSG msg;
		::ZeroMemory(&msg, sizeof(MSG));

		 
	// ---- target time interval
		DOUBLE time_interval =
				1 /
					(double)( virtual_surface.sys_context->GetDWord(virtual_surface.sys_context->FPS ) + 1)
						* 1000;


	// ---- milliseconds
		DOUBLE start_time = (double)timeGetTime();
		DOUBLE time_now   = start_time;
		DOUBLE time_last  = start_time;
		DOUBLE next_time  = 0;


	// ---- loop until completed
		while( msg.message != WM_QUIT )
		{

			// ---- IF there is a message to process THEN translate and dispatch
				if ( ::PeekMessage( &msg, 0, 0, 0, PM_REMOVE) )
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}

				// ---- ELSE
				else
				{

					// ---- time
						DOUBLE time_elapsed_ms = (DOUBLE)timeGetTime() - start_time;
						virtual_surface.sys_context->SetDouble(
								virtual_surface.sys_context->TIME_MS_ELAPSED,
							time_elapsed_ms
							);


					// ---- IF current time exceeds next time THEN
						if ( time_elapsed_ms > next_time )
						{

							// ---- TRY to invoke display method
								INT error_code = VS_OK;
								try
								{

									// ---- delta
										time_last = time_now;
										time_now  = time_elapsed_ms;
										DOUBLE time_delta = time_now - time_last;
										virtual_surface.sys_context->SetDouble(
												virtual_surface.sys_context->TIME_MS_DELTA,
												time_delta
											);

										 
									// ---- display
										error_code = virtual_surface.app_interface.Framework(
												virtual_surface.app_interface.DISPLAY,
												0
											);
											if ( error_code<= VS_ERROR )
												throw (10);


									// ---- regulate
										DOUBLE time_to_display = (DOUBLE)timeGetTime() - time_elapsed_ms - start_time;
										INT time_to_sleep = (INT)(time_interval - time_to_display - 1);
										if ( time_to_sleep > 1 )
										{
											Sleep(time_to_sleep);
										}


									// ---- debug info
										#if defined(VS_DEBUG_FLAG)
										INT second = (INT)(time_elapsed_ms / 1000);
										INT second_last = (INT)( time_elapsed_ms_last / 1000);
										if ( second != second_last )
										{
											CHAR str[128];
											sprintf(str, " ---- %3d fps at %d s (d %6.3f ms) %s\n", (INT)fps, second, (float)time_delta, (++tsc % 10 == 0 ? mts : "") );
											OutputDebugString(str);
											fps = 0;
										}
										time_elapsed_ms_last = time_elapsed_ms;
										fps++;
										#endif


									// ---- update "next" time
										next_time = time_elapsed_ms + time_interval;

								}

							// ---- catch, report and return
								catch ( INT error_id )
								{
									virtual_surface.sys_output->MessageOk(error_id, error_code);
									return VS_ERROR;
								}

						}

				}

		}

    return (INT)msg.wParam;
}


// ---------- WinCreate ----------
/*!
\brief Create window.
\author Gareth Edwards
\param HINSTANCE - Handle to instance.
\return int - ERROR <= 0 < VS_OK

\note Useful window calls include:

GetWindowRect() will give you the area of a window
GetSystemMetrics() will give you sizes like border
SetWindowPos() or MoveWindow() will resize the window

\note WIN32 programming reading:
"Win32 programming" by Grent Rector and Joseph Newcomer

\note The "Bible" of Win32 programming...
"Programming Windows" by Charles Petzold

\note MFC reading:
"Programming MFC" by Jeff Prosise

\note Professional VC++ book from Wrox press
*/
INT WinCreate (HINSTANCE hInstance)
{

	// ---- local
		MSG        uMsg;
		ZeroMemory(&uMsg,sizeof(uMsg));


	// ---- create window class
		WNDCLASSEX winClass; 


	// ---- initialise window class
		winClass.lpszClassName = "VS";
		winClass.cbSize        = sizeof(WNDCLASSEX);
		winClass.style         = CS_HREDRAW | CS_VREDRAW;
		winClass.lpfnWndProc   = (WNDPROC)WindowProc;
		winClass.hInstance     = hInstance;
		winClass.hIcon	       = LoadIcon(0, IDI_APPLICATION);
		winClass.hIconSm	   = LoadIcon(0, IDI_APPLICATION);
		winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		winClass.lpszMenuName  = NULL;
		winClass.cbClsExtra    = 0;
		winClass.cbWndExtra    = 0;


	// ---- set background color
		winClass.hbrBackground = CreateSolidBrush(
				RGB(
					(DWORD)(virtual_surface.sys_context->GetFloat(virtual_surface.sys_context->BG_RED)*255),
					(DWORD)(virtual_surface.sys_context->GetFloat(virtual_surface.sys_context->BG_GREEN)*255),
					(DWORD)(virtual_surface.sys_context->GetFloat(virtual_surface.sys_context->BG_BLUE)*255)
				)
			);


	// ---- set icon Window
		winClass.hIcon = (HICON)LoadIcon( hInstance, MAKEINTRESOURCE(IDI_HICON));
		winClass.hIconSm = (HICON)LoadIcon( hInstance, MAKEINTRESOURCE(IDI_HICONSM));


	// ---- TRY to create window
	INT error_code = VS_OK;
	try
	{

		// ---- IF register class fails THEN report and return
			error_code = (INT)RegisterClassEx(&winClass);
				if ( error_code <= VS_ERROR )
					throw(14);


		// ---- set window name
			CHAR window_name[VS_MAXCHARLEN];
			sprintf_s(
					window_name,
					VS_MAXCHARLEN,
					"%s [ %s version:%d.%d.%d.%d]",
					virtual_surface.sys_context->GetChar(virtual_surface.sys_context->APP_TITLE),
					virtual_surface.sys_context->GetChar(virtual_surface.sys_context->APP_NAME), 
					VS_SYS_VERSION_MAJOR,
					VS_SYS_VERSION_MINOR,
					VS_SYS_VERSION_PATCH,
					VS_SYS_VERSION_BUILD
				);


		// ---- IF create window fails THEN throw exception
			HWND hwnd =
				CreateWindowEx(
					NULL,
					winClass.lpszClassName,
					window_name,
					virtual_surface.sys_context->GetDWord(virtual_surface.sys_context->WINDOW_STYLE),
					virtual_surface.sys_context->GetDWord(virtual_surface.sys_context->X_OFFSET),
					virtual_surface.sys_context->GetDWord(virtual_surface.sys_context->Y_OFFSET),
					virtual_surface.sys_context->GetDWord(virtual_surface.sys_context->CLIENT_WIDTH),
					virtual_surface.sys_context->GetDWord(virtual_surface.sys_context->CLIENT_HEIGHT),
					NULL,
					NULL,
					hInstance,
					NULL
				);
				if ( error_code=(INT)hwnd <=VS_ERROR )
					throw(15);


		// ---- store DWORD typecast window handle
			virtual_surface.sys_context->SetDWord(virtual_surface.sys_context->WINDOW_HANDLE, (DWORD)hwnd),


		// ---- show window ----
			ShowWindow(hwnd, SW_SHOW);


		// ---- register power setting events
			{
				HPOWERNOTIFY hpn;

				hpn = RegisterPowerSettingNotification(
						hwnd,
						&GUID_SLEEPBUTTON_ACTION,
						DEVICE_NOTIFY_WINDOW_HANDLE
					);

				hpn = RegisterPowerSettingNotification(
						hwnd,
						&GUID_POWERBUTTON_ACTION,
						DEVICE_NOTIFY_WINDOW_HANDLE
					);

				hpn = RegisterPowerSettingNotification(
						hwnd,
						&GUID_LIDCLOSE_ACTION,
						DEVICE_NOTIFY_WINDOW_HANDLE
					);

				hpn = RegisterPowerSettingNotification(
						hwnd,
						&GUID_LIDOPEN_POWERSTATE,
						DEVICE_NOTIFY_WINDOW_HANDLE
					);
			}

		
		// ---- tidy up
			UpdateWindow(hwnd);
			SetForegroundWindow(hwnd);
			SetFocus(hwnd);

		
		// ---- set cursor
			ShowCursor(virtual_surface.sys_context->GetBool(virtual_surface.sys_context->CURSOR));

	}

	// ---- catch, report and return
		catch ( INT error_id )
		{
			virtual_surface.sys_output->MessageOk(error_id, error_code);
			return VS_ERROR;
		}

	return VS_OK;
}


// ---------- Global Parameters class ----------
class GlobalParam
{

public:

	GlobalParam()
	{
		// seconds counter
		LONG seconds_elapsed_counter = 0;

		// message pump profiling
		returning_from_hibernation = FALSE;
		nc_activate = FALSE;
		activate = FALSE;
		activate_app = FALSE;
	};


	~GlobalParam()
	{
		;
	};


	// counters
	LONG seconds_elapsed_counter;


	// message profiling
	BOOL returning_from_hibernation;
	BOOL nc_activate;
	BOOL activate;
	BOOL activate_app;


} global_parameters;


// ---------- WindowProc WinMessageProcess ----------
/*!
\brief Process messages
\author Gareth Edwards
\param HWND - Unique window handle 
\param UINT - Message identifier that WndProc will be handling
\param WPARAM - Extensions of the message parameter
\param LPARAM
\return LRESULT - returned value = DefWindowProc(hwnd, msg, wParam, lParam);

\note Handle operating system messages identified in the Message structure.
\note Get mouse screen position and convert to client.
\note Update mouse left/middle/right button status.
\note Update mouse wheel value.
*/
LRESULT CALLBACK WindowProc( //WinMessageProcess(
		HWND hwnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam
	)
{

	// ---- zap message flag
		BOOL report_messages = FALSE;


	// ---- local
		vs_system::GfxInterface  *gfx_interface = virtual_surface.gfx_interface;
		vs_system::SysContext    *sys_context   = virtual_surface.sys_context;
		vs_system::SysInput      *sys_input     = virtual_surface.sys_input;
		vs_system::SysOutput     *sys_output    = virtual_surface.sys_output;


	// ---- IF pi_sys_context dtor'd THEN exit....
		INT exit_status = sys_context->GetPiSysContextStatus();
		if ( exit_status == VS_EXIT )
			return VS_ERROR;


	// ---- IF active AND NOT blocked THEN active
		BOOL active = sys_context->GetBool(sys_context->ACTIVE) &&
							!sys_context->GetBool(sys_context->BLOCKED);


	// ---- IF active THEN update cursor information
		if ( active )
		{
			// Get then convert cursor from screen to client
			POINT position;
			GetCursorPos( &position );
			ScreenToClient( hwnd, &position );

			// Set input client cursor position
			sys_input->SetClientCursorPosition(&position);

			// Set gfx client cursor position
			gfx_interface->SetClientCursorPosition(&position);
		}


	// ---- IF ( report_messages ) 
		if ( report_messages )  printf("msg %d\n", msg);
		CHAR map[VS_MAXCHARLEN];
		sprintf(
				map,
				" %s %s %s ",
				global_parameters.nc_activate  ? "T" : "F",
				global_parameters.activate     ? "T" : "F",
				global_parameters.activate_app ? "T" : "F"
			);


	// ---- part 1 : handle messages
	switch( msg )
	{

		case WM_SETFOCUS:  // 0x0007
			{
				if ( report_messages ) printf("WM_SETFOCUS (wParam == %d) %s\n", msg, map);

				// IF returning from hibernation THEN
				if ( global_parameters.returning_from_hibernation || global_parameters.activate_app )
				{

					// send WM_SYSCOMMAND message with SC_RESTORE
					/*
						if ( report_messages ) printf("SEND WM_SYSCOMMAND with wParam == SC_RESTORE\n");
						global_parameters.returning_from_hibernation = FALSE;
						global_parameters.activate_app = FALSE;
						SendMessage(
								(HWND)virtual_surface.sys_context->GetDWord(virtual_surface.sys_context->WINDOW_HANDLE),
								WM_SYSCOMMAND,
								SC_RESTORE,
								0
						);
					*/

					WinResetGraphicsDevice(
							(UINT)sys_context->GetDWord(sys_context->CLIENT_WIDTH),
							(UINT)sys_context->GetDWord(sys_context->CLIENT_HEIGHT)
						);

				}
			}
			break;


		case WM_KILLFOCUS:  // 0x0008
			{
				if ( report_messages ) printf("WM_KILLFOCUS (wParam == %d) %s\n", msg, map);
			}
			break;


		// event notifies applications that a power-management event has occurred
		//
		// note: Event registered via RegisterPowerSettingNotification
		//
		case WM_POWERBROADCAST:
			{
				// IF power event THEN
				if ( report_messages ) printf("WM_POWERBROADCAST %d\n", msg);
				if ( lParam == 0 )
				{

					// IF hibernating/shutting down THEN
					//   PBT_APMSUSPEND event
					//
					// Notifies applications that the computer is about to enter
					// a suspended state.This event is typically broadcast when
					// all applications and installable drivers have returned
					// TRUE to a previous PBT_APMQUERYSUSPEND event.
					//
					if ( wParam == 4 || wParam == 5 )
					{
						if (report_messages) printf("PBT_APMSUSPEND (wParam == %d)\n", wParam);
						global_parameters.returning_from_hibernation = FALSE;
					}

					// ELSE IF PBT_APMRESUMECRITICAL THEN
					//
					// Notifies applications that the system has resumed operation.
					//
					else if (wParam == 6)
					{
						if (report_messages) printf("PBT_APMRESUMECRITICAL (wParam == %d)\n", wParam);
					}

					// ELSE IF returning from hibernation THEN
					//
					// If the system wakes due to user activity, the system will
					// first broadcast the PBT_APMRESUMEAUTOMATIC (18) event 
					// followed by a PBT_APMRESUMESUSPEND (4) event.
					//
					else if ( wParam == 7 || wParam == 8 )
					{
						// When ready - the system sends the WM_SETFOCUS event,
						// This sends a WM_SYSCOMMAND message with SC_RESTORE
						// as it's parameter. This re-initialises the window
						// client dimensions (this event is used by others,
						// such as SC_MAXIMIZE) and then resizes the windows
						// backbuffer function, which in turn re-creates the
						// grpahics device.
						//
						// Note: See above...
						//
						if (report_messages) printf("PBT_APMRESUMESUSPEND (wParam == %d)\n", wParam);
						global_parameters.returning_from_hibernation = TRUE;
					}

					// ELSE IF PBT_APMPOWERSTATUSCHANGE THEN
					//
					// Notifies applications of a change in the power status
					// of the computer, such as a switch from battery power
					// to A/C.
					//
					else if (wParam == 10)
					{
						if (report_messages) printf("PBT_APMPOWERSTATUSCHANGE (wParam == %d)\n", wParam);
					}

					// ELSE IF PBT_APMRESUMEAUTOMATIC THEN
					//
					// When the system wakes automatically, it broadcasts the
					// PBT_APMRESUMEAUTOMATIC event to all applications.
					//
					else if ( wParam == 18 )
					{
						if (report_messages) printf("PBT_APMRESUMEAUTOMATIC (wParam == %d)\n", wParam);
					}

					// Report other events
					else
					{
						if (report_messages) printf("WM_POWERBROADCAST (wParam == %d)\n", wParam);
					}
				}
			}
			break;


		// event triggered ONLY at run-time
		case WM_CREATE:
			{
				if ( report_messages ) printf("WM_CREATE (wParam == %d) %s\n", msg, map);
			}
			break;


		case WM_NCACTIVATE: // 0x0086 (134)
			{
				global_parameters.nc_activate = TRUE;
				if (report_messages) printf("WM_NCACTIVATE (wParam == %d) %s\n", msg, map);
			}
			break;


		// event triggered at run-time AND when window is being activated or deactivated.
		case WM_ACTIVATE:  // 0x0006
			if ( LOWORD(wParam) != WA_INACTIVE )
			{
				sys_context->SetBool(sys_context->ACTIVE, true);
				if ( global_parameters.nc_activate )
				{
					global_parameters.nc_activate = FALSE;
					global_parameters.activate = TRUE;
				}
				if (report_messages) printf("WM_ACTIVATE (wParam == ACTIVE) %s\n", map);
			}
			else
			{
				if (report_messages) printf("WM_ACTIVATE (wParam == INACTIVE)\n");
				sys_context->SetBool(sys_context->ACTIVE, false);
			}
			break;


		case WM_ACTIVATEAPP: // 0x001C (28)
			{
				//global_parameters.nc_activate  = FALSE;
				//global_parameters.activate     = FALSE;
				//global_parameters.activate_app = TRUE;
				if (global_parameters.activate)
				{
					global_parameters.activate = FALSE;
					global_parameters.activate_app = TRUE;
				}
				if (report_messages) printf("WM_ACTIVATEAPP (wParam == %d) %s\n", msg, map);
			}
			break;


		// confirm that the application is to be closed
		case WM_CLOSE:
			{
				// Display windows message box
				INT result = virtual_surface.sys_output->MessageEscape(0);

				// IF user want to close THEN quit
				if ( result == IDYES )
				{
					// call default handler
					DestroyWindow(hwnd);
				}
				// ELSE throw message away 
				else
				{
					return 0;
				}
			}
			break;


		// event triggered at run-time AND when window moved or resized
		case WM_GETMINMAXINFO:
			{
				LPMINMAXINFO pMMI = (LPMINMAXINFO)lParam;
				pMMI->ptMinTrackSize.x = 
					sys_context->GetDWord(sys_context->MIN_WIDTH) +
					(sys_context->GetDWord(sys_context->CX_BORDER) * 2);
				pMMI->ptMinTrackSize.y = 
					sys_context->GetDWord(sys_context->MIN_HEIGHT) +
					sys_context->GetDWord(sys_context->CX_TOP) +
					sys_context->GetDWord(sys_context->CX_BORDER);
			}
			return 0;


		// event triggered when window moved or resized
		case WM_EXITSIZEMOVE:
			{
				// IF resizing THEN
				if ( sys_context->GetBool(sys_context->SIZING) )
				{
					sys_context->SetBool(sys_context->SIZING, false);
					return WinResetGraphicsDevice(
						(UINT)sys_context->GetDWord(sys_context->CLIENT_WIDTH),
						(UINT)sys_context->GetDWord(sys_context->CLIENT_HEIGHT) 
						);
				}
				// ELSE move
				{
					sys_context->SetBool(sys_context->MOVED, false);
					sys_context->SetBool(sys_context->MOVING, false);
				}
			}
			break;


		// event triggered AT run-time AND when window is being resized
		case WM_SIZE:

			// IF windowed THEN (was + && gfx_interface.GetDevice() != 0 )
			if ( sys_context->GetBool(sys_context->WINDOWED) )
			{

				// IF NOT maximised THEN store previous client area dimensions
				if ( !sys_context->GetBool(sys_context->MAXIMISED) )
				{
					sys_context->SetDWord(
						sys_context->PREVIOUS_WIDTH,
						sys_context->GetDWord(sys_context->CLIENT_WIDTH) );
					sys_context->SetDWord(
						sys_context->PREVIOUS_HEIGHT,
						sys_context->GetDWord(sys_context->CLIENT_HEIGHT) );
				}

				// update client area dimensions
				sys_context->SetDWord(sys_context->CLIENT_WIDTH, LOWORD(lParam) );
				sys_context->SetDWord(sys_context->CLIENT_HEIGHT, HIWORD(lParam) );
				sys_context->SetBool(sys_context->SIZING, true);

				return 0;

			}
			break;


		// window has been moved (note: was MOVED_TO_X/Y)
		case WM_MOVE:
			{
				sys_context->SetDWord(sys_context->X_OFFSET, LOWORD(lParam) );
				sys_context->SetDWord(sys_context->Y_OFFSET, HIWORD(lParam) );
				sys_context->SetBool(sys_context->MOVED, true);
			}
			return 0;


		// window is moved
		case WM_MOVING:
			{
				// Note: use DWORD var = wParam; to detect side or corner
				sys_context->SetMovingRect((LPRECT)lParam);
				sys_context->SetBool(sys_context->MOVING, true);
			}
			return 0;


		// application system commands - see later
		case WM_SYSCOMMAND:
			{
				if ( report_messages )  printf("WM_SYSCOMMAND\n");
				int f = (int)wParam;
				switch ( wParam )
				{
					case SC_MINIMIZE:
						if ( report_messages )  printf("SC_MINIMIZE\n");
						break;
					case SC_MAXIMIZE:
						if ( report_messages )  printf("SC_MAXIMIZE\n");
						break;
					case SC_RESTORE:
						{
							// ALT TAB DESKTOP: Catch zero width/height
							if ( report_messages )  printf("SC_RESTORE\n");
						}
						break;
					case SC_CLOSE:
						if ( report_messages )  printf("SC_CLOSE\n");
						PostQuitMessage(0);
						break;
					case SC_SCREENSAVE:
						if ( report_messages )  printf("SC_SCREENSAVE\n");
						break;
					case SC_MONITORPOWER:
						if ( report_messages )  printf("SC_MONITORPOWER\n");
						break;
					default:
						break;
				}
			}
			break;


		case WM_RBUTTONDOWN:   if ( active ) sys_input->SetBool(sys_input->RBUTTON_DOWN,   true); break;
		case WM_RBUTTONDBLCLK: if ( active ) sys_input->SetBool(sys_input->RBUTTON_DBLCLK, true); break;
		case WM_RBUTTONUP:     if ( active ) sys_input->SetBool(sys_input->RBUTTON_UP,     true); break;


		case WM_LBUTTONDOWN:
			if ( active )
			{
				//OutputDebugString("WM_LBUTTONDOWN\n");
				sys_input->SetBool(sys_input->LBUTTON_DOWN, true);
			}
			break;


		case WM_LBUTTONDBLCLK: if ( active ) sys_input->SetBool(sys_input->LBUTTON_DBLCLK, true); break;


		case WM_LBUTTONUP:
			if ( active )
			{
				// Note:
				//
				// The WinMessageProcess function allows this event BEFORE
				// it has invoked an applications Disply method, hence an
				// application will not "detect" mouse down, so:
				//
				//    IF NOT touch mode THEN left button up ELSE the
				//    application MUST consume mouse down event.
				//
				//    See AppControl::UpdateInput()
				//
				BOOL touch = sys_context->GetBool(sys_context->TOUCH);
				if ( !touch )
				{
					//OutputDebugString("WM_LBUTTONUP\n");
					sys_input->SetBool(sys_input->LBUTTON_UP, true);
				}
			}
			break;


		case WM_MBUTTONDOWN:   if ( active ) sys_input->SetBool(sys_input->MBUTTON_DOWN,   true); break;
		case WM_MBUTTONDBLCLK: if ( active ) sys_input->SetBool(sys_input->MBUTTON_DBLCLK, true); break;
		case WM_MBUTTONUP:     if ( active ) sys_input->SetBool(sys_input->MBUTTON_UP,     true); break;


		case WM_MOUSEWHEEL:
			{
				// Set system mouse wheel clicks
				if ( active ) sys_input->SetLong(sys_input->WHEEL_CLICKS, (LONG)wParam);
			}
			break;


		case WM_DESTROY:
			PostQuitMessage(0);
			break;


		case WM_CHAR:
			{
				if ( active )
				{
					// Set key context
					BOOL shift_key = GetAsyncKeyState(VK_SHIFT) & 0x8000f ? true : false;
					BOOL control_key = GetAsyncKeyState(VK_CONTROL) & 0x8000f ? true : false;
					sys_input->SetBool( sys_input->SHIFT_KEY, shift_key );
					sys_input->SetBool( sys_input->CONTROL_KEY, control_key );

					// Set ascii key code and key state vector
					sys_input->SetKeyboardChar((INT)wParam, (INT)lParam);
				}
			}
			break;


		case WM_KEYDOWN:

			if ( active ) 
			{

				// handle virtual keyboard
				switch ( wParam )
				{

					// include a message box that confirms that the application is to be closed
					case VK_ESCAPE :
						{
							// display windows message box
							int result = sys_output->MessageEscape(0);

							// IF user want to close THEN quit
							if ( result == IDYES )
							{
								// invoke default handler
								DestroyWindow(hwnd);
							}
							// ELSE throw message away 
							else
							{
								return 0;
							}
						}
						break;

					default:
						break;

				}

				// set key pressed down
				sys_input->SetBool(sys_input->KEY_DOWN, true);

			}
			break;


		case WM_KEYUP:
			{
				if ( active ) sys_input->SetBool(sys_input->KEY_UP, true);
			}
			break;


		case (WM_USER + 1) :
			{
				sys_context->SetBool(sys_context->MAXIMISED, false);
				sys_context->SetBool(sys_context->MINIMISED, false);

				return WinResetGraphicsDevice(
						(UINT)sys_context->GetDWord(sys_context->CLIENT_WIDTH),
						(UINT)sys_context->GetDWord(sys_context->CLIENT_HEIGHT) 
					);
			}
			break;


		case (WM_USER + 0x0100) :
			{
				// store
				sys_context->SetDWord(sys_context->MSG, msg);
				sys_context->SetDWord(sys_context->MSG_WPARAM, (DWORD)wParam);
				sys_context->SetDWord(sys_context->MSG_LPARAM, (DWORD)lParam);

				// increment User Event Counter
				DWORD euc = sys_context->GetDWord(sys_context->MSG_USER_EVENT_COUNT);
				sys_context->SetDWord(sys_context->MSG_USER_EVENT_COUNT, euc + 1);
			}
			break;
	}


	// ---- part 2 : handle default events/messages----


	HRESULT defResult = DefWindowProc(hwnd, msg, wParam, lParam);


	// ---- part 3 : handle maximised and minimised system commands ----


	switch( msg )
	{

		case WM_SYSCOMMAND:
		{

			int f = (int)wParam;
			switch ( f )
			{

				case SC_MINIMIZE:
					{
						if ( report_messages )  printf("SC_MINIMIZE\n");

						// minimised
						sys_context->SetBool(sys_context->MINIMISED, true);
					}
					break;


				case SC_MAXIMIZE:
					{
						if ( report_messages )  printf("SC_MAXIMIZE\n");

						// set NEW client dimensions within desktop
						sys_context->SetDWord(
								sys_context->CLIENT_WIDTH,
								sys_context->GetDWord(sys_context->DESKTOP_WIDTH)
							);
						sys_context->SetDWord(
								sys_context->CLIENT_HEIGHT,
								sys_context->GetDWord(sys_context->DESKTOP_HEIGHT) -
								sys_context->GetDWord(sys_context->CX_TOP) +
								sys_context->GetDWord(sys_context->CX_BORDER)
							);

						// to fit NEW windows dimensions around client area,
						//
						//    Window.wndWidth  = Window.width + (2 * Window.cBorder);
						//    Window.wndHeight = Window.height + Window.cTop + Window.cBorder;
						//
						// ... then move and resize window at new location,
						/*
						HRESULT retval = MoveWindow (hwnd,
							-(int)Window.cBorder,
							-(int)Window.cBorder,
							Window.wndWidth-1,
							Window.wndHeight-1,
							true);
						*/

						// maximised
						sys_context->SetBool(sys_context->MAXIMISED, true);

						// resize buffer
						return WinResetGraphicsDevice(
								(UINT)sys_context->GetDWord(sys_context->CLIENT_WIDTH),
								(UINT)sys_context->GetDWord(sys_context->CLIENT_HEIGHT) 
							);
					}
					break;


				case SC_RESTORE: 
					{
						if ( report_messages )  printf("SC_RESTORE\n");

						// IF NOT minimised THEN restore client area dimensions
						if ( ! sys_context->GetBool(sys_context->MINIMISED)  )
						{
							sys_context->SetDWord(
									sys_context->CLIENT_WIDTH,
									sys_context->GetDWord(sys_context->PREVIOUS_WIDTH)
								);
							sys_context->SetDWord(
									sys_context->CLIENT_HEIGHT,
									sys_context->GetDWord(sys_context->PREVIOUS_HEIGHT)
								);
						}
						sys_context->SetBool(sys_context->MAXIMISED, false);
						sys_context->SetBool(sys_context->MINIMISED, false);


						// ALT TAB DESKTOP: Catch zero width/height
						if (
							sys_context->GetDWord(sys_context->CLIENT_WIDTH) == 0 ||
							sys_context->GetDWord(sys_context->CLIENT_HEIGHT)  == 0
							)
						{
							sys_context->SetDWord(
									sys_context->CLIENT_WIDTH,
									sys_context->GetDWord(sys_context->WINDOW_WIDTH)
								);
							sys_context->SetDWord(
									sys_context->CLIENT_HEIGHT,
									sys_context->GetDWord(sys_context->WINDOW_HEIGHT)
								);
							sys_context->SetDWord(
									sys_context->PREVIOUS_WIDTH,
									sys_context->GetDWord(sys_context->CLIENT_WIDTH)
								);
							sys_context->SetDWord(
									sys_context->PREVIOUS_HEIGHT,
									sys_context->GetDWord(sys_context->CLIENT_HEIGHT)
								);
						}


						// resize buffer
						return WinResetGraphicsDevice(
								(UINT)sys_context->GetDWord(sys_context->CLIENT_WIDTH),
								(UINT)sys_context->GetDWord(sys_context->CLIENT_HEIGHT) 
							);
					}
					break;


				case SC_CLOSE:
					if ( report_messages )  printf("SC_CLOSE\n");
					break;


				case SC_SCREENSAVE:
					if ( report_messages )  printf("SC_SCREENSAVE\n");
					break;


				case SC_MONITORPOWER:
					if ( report_messages )  printf("SC_MONITORPOWER\n");
					break;


				default:
					if ( report_messages )  printf("default: %d\n", f);

					if ( wParam == 0xf012 )
					{
						if ( report_messages )  printf("SC_DRAGMOVE\n");
					}
					
					// Note:
					//
					//    0xf001 == drag left border
					//    0xf002 == drag right border
					//    0xf003 == drag top border
					//    0xf006 == drag bottom border
					//    Etc....

					break;

				}
			}
			break;


		// ELSE not handled
		default:
			break;

	}

	return defResult;
}


// ---------- WinResetGraphicsDevice ----------
/*!
\brief Reset graphics device (invoked from WinMessageProcess function)
\author Gareth Edwards
\param UINT - width of frame buffer
\param UINT - height of frame buffer
\return INT - ERROR <= 0 < VS_OK 
*/
INT WinResetGraphicsDevice(
		UINT width,
		UINT height
	)
{

	#if defined(TEST_GFX)

	;

	#else

	if ( virtual_surface.sys_context->GetBool(virtual_surface.sys_context->WINDOWED) )
	{

		INT error_code=VS_OK;
		try
		{

			if ( error_code=virtual_surface.app_interface.Framework(
					virtual_surface.app_interface.CLEANUP_GRAPHICS, 0) == VS_ERROR
				)
				throw(11);

			if ( error_code=virtual_surface.gfx_interface->ResetDevice(
					width,
					height
					) == D3DERR_INVALIDCALL
				)
				throw(16);

			if ( error_code=virtual_surface.app_interface.Framework(
					virtual_surface.app_interface.SETUP_GRAPHICS, 0) == VS_ERROR
				)
				throw(9);
		}

		catch ( INT error_id )
		{
			virtual_surface.sys_output->MessageOk(error_id, error_code);
			PostQuitMessage(0);
			return VS_ERROR;
		}

	}


	// ---- store dimensions
		INT result = 0;
		result = virtual_surface.sys_context->SetDWord(vs_system::SysContext::CLIENT_WIDTH, width);
		result = virtual_surface.sys_context->SetDWord(vs_system::SysContext::CLIENT_HEIGHT, height);


	// ---- update format
		if ( width < height )
		{
			virtual_surface.sys_context->SetDWord(
					vs_system::SysContext::FORMAT,
					vs_system::SysContext::PORTRAIT
				);
		}
		else
		{
			virtual_surface.sys_context->SetDWord(
				vs_system::SysContext::FORMAT,
				vs_system::SysContext::LANDSCAPE
			);
		}

	#endif

	return VS_OK;
}


// ---------- WinResources ----------
/*!
\brief Find and load resources
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT WinResources ()
{

	#if defined(TEST_GFX)

	;

	#else

	// ---- report config
		INT error = virtual_surface.app_dependency.ReportConfig();


	// ---- title
		virtual_surface.sys_output->Message("Load resources:");


	// ---- find and then load resources

		for (UINT idr=IDR_RCDATA00; idr<= IDR_RCDATA15; idr++)
		{

			// Determines the location of a resource with the 
			// specified type and name in the specified module
			HRSRC hRes = FindResource(NULL, MAKEINTRESOURCE(idr), RT_RCDATA);
			if( NULL != hRes )
			{

				// Retrieve a handle that can be used to obtain a pointer
				// to the first byte of the specified resource in memory
				HGLOBAL hData = LoadResource(NULL, hRes);
				if( NULL != hData )
				{
					// Retrieves the byte size of the specified resource
					DWORD dataSize = SizeofResource(0, hRes);
					unsigned char* data = (unsigned char*)LockResource(hData);


					// Note: To store in string -
					//    std::string result;
					//    result.assign(data, dataSize);


					// add page to atlas from tga format file in memory
					INT result = virtual_surface.gfx_interface->AddPageToAtlasFromTgaFileInMemory(idr, data);

					// log error
					if ( !result )
					{
						char msg[VS_MAXCHARLEN];
						sprintf(msg, "Add image id: %d failed!\n", (INT)idr);
						virtual_surface.sys_output->Message(msg);
					}
				}
				else
				{
					char msg[VS_MAXCHARLEN];
					sprintf(msg, "IMAGE RESOURCES: Failed to retrieve a handle for image %d!/n", (INT)idr);
					virtual_surface.sys_output->Message(msg);
					return VS_ERROR;
				}
			}
			else
			{
				char msg[VS_MAXCHARLEN];
				sprintf(msg, "IMAGE RESOURCES: Failed to determine the location for image %d!/n", (INT)idr);
				virtual_surface.sys_output->Message(msg);
				return VS_ERROR;
			}
		}

	#endif

	return VS_OK;
}