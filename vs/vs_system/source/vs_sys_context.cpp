////////////////////////////////////////////////////////////////////////////////

// ---------- vs_sys_context.cpp ----------
/*!
\file vs_sys_context.cpp
\brief Implementation of the Sys(tem)Context class
\author Gareth Edwards
*/

// ---- include ----
#include "../header/vs_sys_context.h"


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Device class ----------

class SysContext::PI_SysContext
{

public:

	PI_SysContext()
	{

		// ---- zero fill block of memory
			memset(this, 0, sizeof(this));


		// ---- WINDOWS : set when WinProc msg == WM_USER+[?] ----
			msg          = 0;
			msg_wparam   = 0;
			msg_lparam   = 0;
			msc_uec      = 0;


		// ---- Set in AppDependency::ReadConfigFiles method ----

			// app
			strcpy_s(app_title, VS_MAXCHARNAME, "");

			// dev
			strcpy_s(dev_env, VS_MAXCHARNAME, "");

			// path
			strcpy_s(app_folder_path,     VS_MAXCHARPATHLEN, "");
			strcpy_s(project_folder_path, VS_MAXCHARPATHLEN, "");

			// filename
			strcpy_s(app_config_file,  VS_MAXCHARPATHLEN, "");
			strcpy_s(sys_config_file,  VS_MAXCHARPATHLEN, "");
			strcpy_s(view_config_file, VS_MAXCHARPATHLEN, "");

			// data
			app_config_data  = NULL;
			sys_config_data  = NULL;
			view_config_data = NULL;


		// ---- Set in Configure method ----

			// BOOL
			development  = false;
			console      = true;
			debug        = false;
			log          = false;
			log_time     = false;
			install_data = false;
			cursor       = true;
			touch        = false;
			windowed     = true;
			desktop      = false;


			// BOOL: IF true AND NOT overlap_style THEN thin border
			border_style = false;

			// BOOL: IF true THEN menu enabled (if any)
			menu_style   = false;

			// BOOL: IF true THEN name & minimax with thick border
			overlap_style = true;

			// DWORD: Files
			log_files = 10;

			// DOUBLE: time
			time_delta   = 0;
			time_elapsed = 0;

			// DOUBLE: timer
			timer_start  = 0;
			timer_length = 2000;

			// DWORD: gfx
			aaq = 4;
			fps = 60;

			// FLOAT: graphics
			border_width = 1;

			// FLOAT: graphics
			background_red   = 0.0f;
			background_green = 0.4f;
			background_blue  = 0.6f;

			// BOOL: graphics
			textures = TRUE;

			// DWORD: dimension - client area min
			min_width = 400;
			min_height = 300;

			// ---- DWORD: dimension - client width and height ----
			client_width = 1024;
			client_height = 768;


		// ---- Set in ConfigureWindow method  ----

			// ---- DWORD: dimension - client area max ----
			max_width  = 4096;
			max_height = 2048;

			// etc....


		// ---- Set by WinMain function from "vs/vs_application/header/app_include.h" ----
			strcpy_s(dev_env, VS_MAXCHARNAME, "");
			strcpy_s(app_name, VS_MAXCHARNAME, "");


		// ---- set by WinInitialise function
			format = LANDSCAPE;


		// ---- Set by application ----

			// BOOL:
			app_licensed = TRUE;


		// ---- Set in application [app prefix]_version.h file  ----

			// ---- DWORD: application version ----
			app_version_major = 0;
			app_version_minor = 0;
			app_version_patch = 0;
			app_version_build = 0;


		// ---- Set by WinCreate function ----

			// ---- DWORD: handle ----
			hwnd = NULL;

			// ---- DWORD: trigger event logical id ----
			trigger = 0;

		// ---- Set by WinProc function

			// BOOL: application is active
			active = TRUE;

			// BOOL: application is blocked
			blocked = FALSE;

			maximised = FALSE;
			minimised = FALSE;
			moved     = FALSE;
			moving    = FALSE;
			sizing    = FALSE;

			// DWORD: Window location offset
			x_offset  = 0;
			y_offset  = 0;

			// RECT: Window move info
			RECT temp_rect = { 0, 0, 0, 0 };
			moving_rect = temp_rect;


		// ---- Used by StartTimer and GetTimerProgress ----

			// BOOL: timer running ----
			timer_running = FALSE;

	};


	~PI_SysContext()
	{
		;
	};


	// ---- WINDOWS : set when WinProc msg == WM_USER+[?] ----

		DWORD msg;
		DWORD msg_wparam;
		DWORD msg_lparam;
		DWORD msc_uec;


	// ---- set in AppDependency::ReadConfigFiles method ----

		// title
		CHAR app_title[VS_MAXCHARNAME];

		// path
		CHAR app_folder_path[VS_MAXCHARPATHLEN];
		CHAR project_folder_path[VS_MAXCHARPATHLEN];

		// filename
		CHAR app_config_file[VS_MAXCHARPATHLEN];
		CHAR sys_config_file[VS_MAXCHARPATHLEN];
		CHAR view_config_file[VS_MAXCHARPATHLEN];

		// data
		VNode *app_config_data;
		VNode *sys_config_data;
		VNode *view_config_data;


	// ---- Set in Configure method ----

		// BOOL
		BOOL development;
		BOOL console;
		BOOL cursor;
		BOOL debug;
		BOOL install_data;
		BOOL log;
		BOOL log_time;
		BOOL touch;
		BOOL windowed;
		BOOL desktop;
		BOOL border_style;
		BOOL menu_style;
		BOOL overlap_style;

		// DWORD: files
		DWORD log_files;

		// DOUBLE: time
		DOUBLE time_delta;              // Set in SetDouble
		DOUBLE time_elapsed;            // Set in SetDouble

		// DOUBLE: timer
		DOUBLE timer_start;             // Set in WinMessagePump
		DOUBLE timer_length;            // Set by application

		// DWORD: gfx
		DWORD aaq;
		DWORD fps;
		DWORD fps_now;

		// FLOAT: graphics
		FLOAT border_width;

		// FLOAT: graphics
		FLOAT background_red;
		FLOAT background_green;
		FLOAT background_blue;

		// BOOL: graphics
		BOOL textures;

		// DWORD: Client minimum dimensions
		DWORD min_width;
		DWORD min_height;

		// DWORD: Client width and height
		DWORD client_width;
		DWORD client_height;


	// ---- Set in ConfigureWindow method  ----

		// DWORD: Client maximum dimensions
		DWORD max_width;
		DWORD max_height;

		DWORD previous_width;
		DWORD previous_height;

		// DWORD: Desktop width and height
		DWORD desktop_width;
		DWORD desktop_height;

		// DWORD: Window width and height
		DWORD window_width;
		DWORD window_height;

		// DWORD: Window style
		DWORD window_style;

		// DWORD: Derived window metrics
		DWORD cx_border;
		DWORD cx_top;


	// ---- set by WinMain function from "vs/vs_application/header/app_include.h" ----

		// CHAR:
		//
		// Note: Initialised in "vs/vs_application/header/app_include.h"
		//
		CHAR  dev_env[VS_MAXCHARLEN];
		CHAR  app_name[VS_MAXCHARLEN];
		CHAR  config_name[VS_MAXCHARLEN];


	// ---- set by WinInitialise function
		BOOL format;


	// ---- Set by application ----
		BOOL app_licensed;
		

	// ---- set in system sys_version.h file  ----
		//
		// ---- system version info ----
		//
		// Defined in "sys_context.h", and included using:
		//    #include "../header/vs_sys_version.h"
		//
		//  Note: GET ONLY!!!
		//


	// ---- Set in application [app prefix]_version.h file  ----
		//
		// ---- application version info ----
		//
		// Defined in "[app prefix]_version.h", and included using:
		//    #include "[dev env]/vs_application/[app name]/header/[app prefix]_sys_version.h"
		//

		DWORD app_version_major;
		DWORD app_version_minor;
		DWORD app_version_patch;
		DWORD app_version_build;


	// ---- Set by WinCreate function ----

		DWORD hwnd;

	// ---- Set by any function ----

		DWORD trigger;

	// ---- Set by WinProc function ----

		// BOOL: Window status
		BOOL active;
		BOOL blocked;
		BOOL maximised;
		BOOL minimised;
		BOOL moved;
		BOOL moving;
		BOOL sizing;

		// DWORD: Window location offset
		DWORD x_offset;
		DWORD y_offset;

		// RECT: Window move info
		RECT moving_rect;


	// ---- Used by StartTimer and GetTimerProgress ----
		BOOL timer_running;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
SysContext::SysContext()
{
	pi_sys_context = new PI_SysContext;
}


// ---------- Destructor ----------
SysContext::~SysContext ()
{
	delete pi_sys_context;
	pi_sys_context = NULL;
}


// ---------- Configure ----------
/*!
\brief Configure
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT SysContext::Configure(VNode *sys_config_hdl)
{

	// IF -- data -- THEN -- update context
	if (sys_config_hdl != NULL)
	{

		// IF -- data -- THEN
		if ( VNode *system_hdl = sys_config_hdl->SearchName("main") )
		{

			// ---- shortcut ----
			PI_SysContext *psc = pi_sys_context;


			// ---- local ----
			VNode *p = NULL;


			// ---- main ----
			if ( p=system_hdl->SearchName("Title")        ) strcpy(psc->app_title, p->GetValue());
			if ( p=system_hdl->SearchName("Console")      ) psc->console      = p->GetBoolFromValue();
			if ( p=system_hdl->SearchName("Debug")        ) psc->debug        = p->GetBoolFromValue();
			if ( p=system_hdl->SearchName("Log")          ) psc->log          = p->GetBoolFromValue();
			if ( p=system_hdl->SearchName("Log-Time")     ) psc->log_time     = p->GetIntFromValue();
			if ( p=system_hdl->SearchName("Log-Files")    ) psc->log_files    = p->GetIntFromValue();
			if ( p=system_hdl->SearchName("Install-Data") ) psc->install_data = p->GetBoolFromValue();
			

			// IF -- graphics data -- THEN
			if ( VNode *graphics_hdl = system_hdl->SearchName("Config") )
			{
				if (p = graphics_hdl->SearchName("View-Config")) psc->aaq = p->GetIntFromValue();
			}


			// IF -- graphics data -- THEN
			if ( VNode *graphics_hdl = system_hdl->SearchName("Graphics") )
			{
				if ( p=graphics_hdl->SearchName("AAQ") ) psc->aaq = p->GetIntFromValue();
				if ( p=graphics_hdl->SearchName("FPS") ) psc->fps = p->GetIntFromValue();
				if ( p=graphics_hdl->SearchName("Border-Width")) psc->border_width = p->GetFloatFromValue();
				if ( VNode *background_hdl = graphics_hdl->SearchName("Background-Colour") )
				{
					if ( p=background_hdl->SearchName("Red")   ) psc->background_red   = p->GetFloatFromValue();
					if ( p=background_hdl->SearchName("Green") ) psc->background_green = p->GetFloatFromValue();
					if ( p=background_hdl->SearchName("Blue")  ) psc->background_blue  = p->GetFloatFromValue();
				}
			}


			// IF -- window data -- THEN
			if ( VNode *window_hdl = system_hdl->SearchName("Window") )
			{
				if ( p=system_hdl->SearchName("Cursor")   ) psc->cursor   = p->GetBoolFromValue();
				if ( p=system_hdl->SearchName("Touch")    ) psc->touch    = p->GetBoolFromValue();

				if ( p=system_hdl->SearchName("Desktop")  ) psc->desktop  = p->GetBoolFromValue();
				if ( p=system_hdl->SearchName("Windowed") ) psc->windowed = p->GetBoolFromValue();
				if (VNode *dimension_hdl = window_hdl->SearchName("Dimension") )
				{
					if ( p=dimension_hdl->SearchName("Min-Width")  ) psc->min_height    = p->GetIntFromValue();
					if ( p=dimension_hdl->SearchName("Min-Height") ) psc->min_height    = p->GetIntFromValue();
					if ( p=dimension_hdl->SearchName("Width")      ) psc->client_width  = p->GetIntFromValue();
					if ( p=dimension_hdl->SearchName("Height")     ) psc->client_height = p->GetIntFromValue();
				}
				if (VNode *style_hdl = window_hdl->SearchName("Style") )
				{
					if ( p=system_hdl->SearchName("Border")  ) psc->border_style  = p->GetBoolFromValue();
					if ( p=system_hdl->SearchName("Menu")    ) psc->menu_style    = p->GetBoolFromValue();
					if ( p=system_hdl->SearchName("Overlap") ) psc->overlap_style = p->GetBoolFromValue();
				}

			}

		}

	}

	ConfigureWindow();

	return VS_OK;
}


// ---------- ConfigureWindow ----------
/*!
\brief Configure window properties
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
int SysContext::ConfigureWindow ()
{
	// Shortcut
	PI_SysContext *psc = pi_sys_context;

	#	ifdef VS_GFX_FLAG
		
	#	ifdef OS_WINDOWS


	// --- 1 . desktop == primary surface ----

		RECT rect;
		HWND hdt = GetDesktopWindow();
		GetWindowRect(hdt,&rect);
		psc->desktop_width  = rect.right;
		psc->desktop_height = rect.bottom;
		INT xCentre = psc->desktop_width/2;
		INT yCentre = psc->desktop_height/2;


	// ---- 2. get metrics ----

		// system metrics
		DWORD cxBorder  = GetSystemMetrics(SM_CXBORDER);
		DWORD cyBorder  = GetSystemMetrics(SM_CYBORDER);
		DWORD cxEdge    = GetSystemMetrics(SM_CXEDGE);
		DWORD cyEdge    = GetSystemMetrics(SM_CYEDGE);
		DWORD cyCaption = GetSystemMetrics(SM_CYCAPTION);

		// derived system metrics
		psc->cx_border = (cxBorder + cxEdge + 1) * 2;
		psc->cx_top    = cyCaption + psc->cx_border; 

	// ---- Part 3 : set window dimensions ----

		// Note:	WS_OVERLAPPEDWINDOW create window with everything
		//			WS_OVERLAPPED Creates an overlapped window which usually has a caption and a border
		//			WS_BORDER adds a border to the window
		//			WS_CAPTION creates a window that has a title bar (includes the WS_BORDER style).
		//			WS_POPUP creates a pop-up window. This style cannot be used with the WS_CHILD style.
		//			WS_SYSMENU creates a window that has a window menu on its title but cannot be resized
		//			WS_VISIBLE creates a window that is initially visible

		// default style - note: WS_OVERLAPPEDWINDOW has everything!!!!
		psc->window_style = WS_OVERLAPPEDWINDOW;

		// IF -- not windowed -- THEN -- dimensions == fullscreen
		if ( !psc->windowed )
		{
			psc->window_style  = WS_POPUP | WS_VISIBLE;
			psc->client_width  = rect.right;
			psc->client_height = rect.bottom;
			psc->window_width  = psc->client_width;
			psc->window_height = psc->client_height;
		}
		// ELSE -- window is either specified or the dimensions of the desktop
		else
		{
			// IF -- desktop -- THEN -- no caption BUT detects taskbar
			if ( psc->desktop )
			{
				psc->window_style  = WS_POPUP | WS_VISIBLE;
				psc->client_width  = rect.right;
				psc->client_height = rect.bottom;
				psc->window_width  = psc->client_width;
				psc->window_height = psc->client_height;
			}
			// ELSE--  dimensions are as specified
			else
			{
				// select style
				if ( psc->overlap_style )
					psc->window_style = WS_OVERLAPPEDWINDOW;
				else
					if ( psc->menu_style )
						psc->window_style = WS_SYSMENU;
					else
						if ( psc->border_style )
							psc->window_style = WS_BORDER;
						else
							psc->window_style = WS_POPUP;

				// adjust window to fit client rect
				RECT wr = { 0, 0, (LONG)psc->client_width, (LONG)psc->client_height };
				AdjustWindowRectEx(&wr, psc->window_style, false, 0);
				psc->window_width = wr.right - wr.left;
				psc->window_height = wr.bottom - wr.top;
			}
		}

	#	else // OTHER OS

		psc->window_style  = 0;
		INT xCentre   = psc->desktop_width/2;
		INT yCentre   = psc->desktop_height/2;
		psc->window_width  = width;
		psc->window_height = height;

	#	endif // OS_WINDOWS


	// ---- Part 4: offset ----

		// offset
		psc->x_offset = 0;
		psc->y_offset = 0;

		// IF -- fullscreen -- THEN -- rect == rect 
		if ( !psc->windowed )
		{
			;
		}
		// ELSE IF desktop THEN rect == rect 
		else if ( psc->desktop )
		{
			;
		}
		// ELSE locate centred
		else
		{
			// TODO: test against screensize
			psc->x_offset = xCentre - (psc->window_width/2);
			psc->y_offset = yCentre - (psc->window_height/2);
		}

	#	endif // VS_GFX_FLAG

	return VS_OK;
}


// ---------- GetClientRect ----------
/*!
\brief get client rectangle
\author Gareth Edwards
\param RECT * - rectangle structure
*/
void SysContext::GetClientRect(RECT *rect)
{
	rect->left   = 0;
	rect->top    = 0;
	rect->right  = pi_sys_context->client_width;
	rect->bottom = pi_sys_context->client_height;
}


// ---------- GetTimeDelta ----------
/*!
\brief Get time elapsed
\author Gareth Edwards
\return DOUBLE - time elapsed between now and last frame
*/
DOUBLE SysContext::GetTimeDelta()
{
	return pi_sys_context->time_delta;
}


// ---------- GetTimeElapsed ----------
/*!
\brief Get time elapsed since start time initialised
\author Gareth Edwards
\return DOUBLE - time now
*/
DOUBLE SysContext::GetTimeElapsed()
{
	return pi_sys_context->time_elapsed;
}


// ---------- GetPiSysContextStatus ----------
/*!
\brief Get exit status
\author Gareth Edwards
\return INT - 
\note When VS exits, if MS dialogue still active then WindowProc
may still be trying to handle events.... this detects if the
private implmentation has been deleted via the SysContext dtor.
*/
INT SysContext::GetPiSysContextStatus()
{
	//
	// Note:: Default Visual Studio 2013 value after object deleted is 0xfeeefeee
	//        Default Visual Studio 2013 value after object deleted is 0xdddddddd
	//
	// I have explicitly set pi_sys_context object after delete to NULL... 
	//
	if (	(INT)pi_sys_context == 0xfeeefeee ||
			(INT)pi_sys_context == 0xdddddddd ||
			(INT)pi_sys_context == NULL
		)
	{
		pi_sys_context = NULL;
		return VS_EXIT;
	}
	return VS_OK;
}


// ---------- GetBool ----------
/*!
\brief Get BOOL value
\author Gareth Edwards
\param UINT - enumerated id
\return BOOL - IF id found THEN value ELSE FALSE
*/
BOOL SysContext::GetBool(UINT id)
{

	switch ( id )
	{
		case DEVELOPMENT   : return pi_sys_context->development;
		case CONSOLE       : return pi_sys_context->console;
		case DEBUG         : return pi_sys_context->debug;
		case LOG           : return pi_sys_context->log;
		case LOG_TIME      : return pi_sys_context->log_time;
		case INSTALL_DATA  : return pi_sys_context->install_data;

		case CURSOR        : return pi_sys_context->cursor;
		case TOUCH         : return pi_sys_context->touch;

		case WINDOWED      : return pi_sys_context->windowed;
		case DESKTOP       : return pi_sys_context->desktop;
		case BORDER_STYLE  : return pi_sys_context->border_style;
		case MENU_STYLE    : return pi_sys_context->menu_style;
		case OVERLAP_STYLE : return pi_sys_context->overlap_style;

		case TEXTURES      : return pi_sys_context->textures;

		case ACTIVE        : return pi_sys_context->active;
		case BLOCKED       : return pi_sys_context->blocked;
		case MAXIMISED     : return pi_sys_context->maximised;
		case MINIMISED     : return pi_sys_context->minimised;
		case MOVED         : return pi_sys_context->moved;
		case MOVING        : return pi_sys_context->moving;
		case SIZING        : return pi_sys_context->sizing;

		case APP_LICENSED  : return pi_sys_context->app_licensed;

		default            : return FALSE;
	}

}


// ---------- GetChar ----------
/*!
\brief Get CHAR * value
\author Gareth Edwards
\param UINT - enumerated id
\return CHAR * - IF id found THEN pointer to string ELSE NULL
*/
CHAR * SysContext::GetChar(UINT id)
{
	switch ( id )
	{
		case APP_TITLE           : return pi_sys_context->app_title;
		case APP_NAME            : return pi_sys_context->app_name;
		case CONFIG_NAME         : return pi_sys_context->config_name;
		case DEV_ENV             : return pi_sys_context->dev_env;
		case APP_FOLDER_PATH     : return pi_sys_context->app_folder_path;
		case PROJECT_FOLDER_PATH : return pi_sys_context->project_folder_path;
		case APP_CONFIG_FILE     : return pi_sys_context->app_config_file;
		case SYS_CONFIG_FILE     : return pi_sys_context->sys_config_file;
		case VIEW_CONFIG_FILE    : return pi_sys_context->view_config_file;
		default                  : return NULL;
	}
}


// ---------- GetDouble ----------
/*!
\brief Get DOUBLE
\author Gareth Edwards
\param UINT - enumerated id
\return DOUBLE - id found THEN value ELSE NULL
*/
DOUBLE SysContext::GetDouble(UINT id)
{
	switch ( id )
	{
		case TIME_MS_DELTA   : return pi_sys_context->time_delta;
		case TIME_MS_ELAPSED : return pi_sys_context->time_elapsed;
		default              : return NULL;
	}
}


// ---------- GetDWord ----------
/*!
\brief Get DWORD value
\author Gareth Edwards
\param UINT - enumerated id
\return DWORD - IF id found THEN value ELSE NULL
*/
DWORD SysContext::GetDWord(UINT id)
{
	switch ( id )
	{
		case MSG                  : return pi_sys_context->msg;
		case AAQ                  : return pi_sys_context->aaq;
		case FPS                  : return pi_sys_context->fps;
		case FPS_NOW              : return pi_sys_context->fps_now;
		case LOG_FILES            : return pi_sys_context->log_files;
		case MIN_WIDTH            : return pi_sys_context->min_width;
		case MIN_HEIGHT           : return pi_sys_context->min_height;
		case CLIENT_WIDTH         : return pi_sys_context->client_width;
		case CLIENT_HEIGHT        : return pi_sys_context->client_height;
		case FORMAT               : return pi_sys_context->format;
		case MAX_WIDTH            : return pi_sys_context->max_width;
		case MAX_HEIGHT           : return pi_sys_context->max_height;
		case PREVIOUS_WIDTH       : return pi_sys_context->previous_width;
		case PREVIOUS_HEIGHT      : return pi_sys_context->previous_height;
		case DESKTOP_WIDTH        : return pi_sys_context->desktop_width;
		case DESKTOP_HEIGHT       : return pi_sys_context->desktop_height;
		case WINDOW_WIDTH         : return pi_sys_context->window_width;
		case WINDOW_HEIGHT        : return pi_sys_context->window_height;
		case WINDOW_STYLE         : return pi_sys_context->window_style;
		case X_OFFSET             : return pi_sys_context->x_offset;
		case Y_OFFSET             : return pi_sys_context->y_offset;
		case CX_BORDER            : return pi_sys_context->cx_border;
		case CX_TOP               : return pi_sys_context->cx_top;
		case WINDOW_HANDLE        : return pi_sys_context->hwnd;
		case WINDOW_TRIGGER       : return pi_sys_context->trigger;
		case MSG_WPARAM           : return pi_sys_context->msg_wparam;
		case MSG_LPARAM           : return pi_sys_context->msg_lparam;
		case MSG_USER_EVENT_COUNT : return pi_sys_context->msc_uec;
		case SYS_VERSION_MAJOR    : return VS_SYS_VERSION_MAJOR;
		case SYS_VERSION_MINOR    : return VS_SYS_VERSION_MINOR;
		case SYS_VERSION_PATCH    : return VS_SYS_VERSION_PATCH;
		case SYS_VERSION_BUILD    : return VS_SYS_VERSION_BUILD;
		case APP_VERSION_MAJOR    : return pi_sys_context->app_version_major;
		case APP_VERSION_MINOR    : return pi_sys_context->app_version_minor;
		case APP_VERSION_PATCH    : return pi_sys_context->app_version_patch;
		case APP_VERSION_BUILD    : return pi_sys_context->app_version_build;
		default                   : return NULL;
	}
}


// ---------- GetFloat ----------
/*!
\brief Get FLOAT value
\author Gareth Edwards
\param UINT - enumerated id
\return DWORD - IF id found THEN value ELSE NULL
*/
FLOAT SysContext::GetFloat(UINT id)
{
	switch ( id )
	{
		case BORDER_WIDTH : return pi_sys_context->border_width;
		case BG_RED   : return pi_sys_context->background_red;
		case BG_GREEN : return pi_sys_context->background_green;
		case BG_BLUE  : return pi_sys_context->background_blue;
		default       : return NULL;
	}
}


// ---------- SetMovingRect ----------
/*!
\brief Set client rectangle
\author Gareth Edwards
\param RECT * - rectangle structure
\return INT - IF id not found THEN VS_ERROR ELSE VS_OK
*/
INT SysContext::SetMovingRect(RECT *r)
{
	if ( r == NULL ) return VS_ERROR;
	PI_SysContext *psc = pi_sys_context;
	psc->moving_rect.left   = r->left;
	psc->moving_rect.top    = r->top;
	psc->moving_rect.right  = r->right;
	psc->moving_rect.bottom = r->bottom;
	return VS_OK;
}


// ---------- SetBoolean ----------
/*!
\brief Set BOOL value
\author Gareth Edwards
\param UINT - enumerated id
\param BOOL - value
\return INT - IF id not found THEN VS_ERROR ELSE VS_OK
*/
INT SysContext::SetBool(UINT id, BOOL value)
{

	switch ( id )
	{
		case DEVELOPMENT   : pi_sys_context->development   = value; break;

		// case CONSOLE       : pi_sys_context->console       = value; break;

		case CURSOR        : pi_sys_context->cursor        = value; break;
		case TOUCH         : pi_sys_context->touch         = value; break;

		case WINDOWED      : pi_sys_context->windowed      = value; break;
		case DESKTOP       : pi_sys_context->desktop       = value; break;
		case BORDER_STYLE  : pi_sys_context->border_style  = value; break;
		case MENU_STYLE    : pi_sys_context->menu_style    = value; break;
		case OVERLAP_STYLE : pi_sys_context->overlap_style = value; break;

		case TEXTURES      : pi_sys_context->textures      = value; break;

		case ACTIVE        : pi_sys_context->active        = value; break;
		case BLOCKED       : pi_sys_context->blocked       = value; break;
		case MAXIMISED     : pi_sys_context->maximised     = value; break;
		case MINIMISED     : pi_sys_context->minimised     = value; break;
		case MOVED         : pi_sys_context->moved         = value; break;
		case MOVING        : pi_sys_context->moving        = value; break;
		case SIZING        : pi_sys_context->sizing        = value; break;

		case APP_LICENSED  : pi_sys_context->app_licensed  = value; break;

		default            : return VS_ERROR;
	}

	return VS_OK;
}


// ---------- SetChar ----------
/*!
\brief Set CHAR string value
\author Gareth Edwards
\param UINT - enumerated id
\param CHAR * - ptr
\return INT - IF id not found OR ptr == NULL THEN VS_ERROR ELSE VS_OK
*/
INT SysContext::SetChar(UINT id, CHAR *ptr)
{
	if (ptr == NULL) return VS_ERROR; 
	switch ( id )
	{
		case APP_TITLE           : strcpy_s(pi_sys_context->app_title,           VS_MAXCHARLEN,     ptr); break;
		case APP_NAME            : strcpy_s(pi_sys_context->app_name,            VS_MAXCHARLEN,     ptr); break;
		case CONFIG_NAME         : strcpy_s(pi_sys_context->config_name,         VS_MAXCHARLEN,     ptr); break;
		case DEV_ENV             : strcpy_s(pi_sys_context->dev_env,             VS_MAXCHARLEN,     ptr); break;
		case APP_FOLDER_PATH     : strcpy_s(pi_sys_context->app_folder_path,     VS_MAXCHARPATHLEN, ptr); break;
		case PROJECT_FOLDER_PATH : strcpy_s(pi_sys_context->project_folder_path, VS_MAXCHARPATHLEN, ptr); break;
		case APP_CONFIG_FILE     : strcpy_s(pi_sys_context->app_config_file,     VS_MAXCHARPATHLEN, ptr); break;
		case SYS_CONFIG_FILE     : strcpy_s(pi_sys_context->sys_config_file,     VS_MAXCHARPATHLEN, ptr); break;
		case VIEW_CONFIG_FILE    : strcpy_s(pi_sys_context->view_config_file,    VS_MAXCHARPATHLEN, ptr); break;
		default                  : return VS_ERROR;
	}
	return VS_OK;
}


// ---------- SetDouble ----------
/*!
\brief Set DOUBLE value
\author Gareth Edwards
\param UINT - enumerated id
\param DWORD - value
\return INT - IF id not found THEN VS_ERROR ELSE VS_OK
*/
INT SysContext::SetDouble(UINT id, DOUBLE value)
{ 
	switch ( id )
	{
		case TIME_MS_DELTA   : pi_sys_context->time_delta   = value; break;
		case TIME_MS_ELAPSED : pi_sys_context->time_elapsed = value; break;
		default : return VS_ERROR;
	}
	return VS_OK;
}


// ---------- SetDWord ----------
/*!
\brief Set DWORD value
\author Gareth Edwards
\param UINT - enumerated id
\param DWORD - value
\return INT - IF id not found THEN VS_ERROR ELSE VS_OK
*/
INT SysContext::SetDWord(UINT id, DWORD value)
{
	switch ( id )
	{
		case MSG                  : pi_sys_context->msg               = value; break;
		case AAQ                  : pi_sys_context->aaq               = value; break;
		case FPS                  : pi_sys_context->fps               = value; break;
		case FPS_NOW              : pi_sys_context->fps_now           = value; break;
		case LOG_FILES            : pi_sys_context->log_files         = value; break;
		case MIN_WIDTH            : pi_sys_context->min_width         = value; break;
		case MIN_HEIGHT           : pi_sys_context->min_height        = value; break;
		case CLIENT_WIDTH         : pi_sys_context->client_width      = value; break;
		case CLIENT_HEIGHT        : pi_sys_context->client_height     = value; break;
		case FORMAT               : pi_sys_context->format            = value; break;
		case MAX_WIDTH            : pi_sys_context->max_width         = value; break;
		case MAX_HEIGHT           : pi_sys_context->max_height        = value; break;
		case PREVIOUS_WIDTH       : pi_sys_context->previous_width    = value; break;
		case PREVIOUS_HEIGHT      : pi_sys_context->previous_height   = value; break;
		case DESKTOP_WIDTH        : pi_sys_context->desktop_width     = value; break;
		case DESKTOP_HEIGHT       : pi_sys_context->desktop_height    = value; break;
		case WINDOW_WIDTH         : pi_sys_context->window_width      = value; break;
		case WINDOW_HEIGHT        : pi_sys_context->window_height     = value; break;
		case WINDOW_STYLE         : pi_sys_context->window_style      = value; break;
		case X_OFFSET             : pi_sys_context->x_offset          = value; break;
		case Y_OFFSET             : pi_sys_context->y_offset          = value; break;
		case CX_BORDER            : pi_sys_context->cx_border         = value; break;
		case CX_TOP               : pi_sys_context->cx_top            = value; break;
		case WINDOW_HANDLE        : pi_sys_context->hwnd              = value; break;
		case WINDOW_TRIGGER       : pi_sys_context->trigger           = value; break;
		case MSG_WPARAM           : pi_sys_context->msg_wparam        = value; break;
		case MSG_LPARAM           : pi_sys_context->msg_lparam        = value; break;
		case MSG_USER_EVENT_COUNT : pi_sys_context->msc_uec           = value; break;
		case APP_VERSION_MAJOR    : pi_sys_context->app_version_major = value; break;
		case APP_VERSION_MINOR    : pi_sys_context->app_version_minor = value; break;
		case APP_VERSION_PATCH    : pi_sys_context->app_version_patch = value; break;
		case APP_VERSION_BUILD    : pi_sys_context->app_version_build = value; break;
		default                   : return VS_ERROR;
	}
	return VS_OK;
}


// ---------- SetFloat ----------
/*!
\brief Set FLOAT value
\author Gareth Edwards
\param UINT - enumerated id
\param DWORD - value
\return INT - IF id not found THEN VS_ERROR ELSE VS_OK
*/
INT SysContext::SetFloat(UINT id, FLOAT value)
{
	switch ( id )
	{
		case BORDER_WIDTH : pi_sys_context->border_width = value; break;
		case BG_RED   : pi_sys_context->background_red   = value; break;
		case BG_GREEN : pi_sys_context->background_green = value; break;
		case BG_BLUE  : pi_sys_context->background_blue  = value; break;
		default       : return VS_ERROR;
	}
	return VS_OK;
}


// ---------- GetTimerProgress ----------
/*!
\brief
\author Gareth Edwards
\param DOUBLE [out] 0 < timer running, else 1 > timer
\return INT - IF timer running then VS_OK else VS_ERROR
\note 2017
*/
INT SysContext::GetTimerProgress(DOUBLE *mu)
{
	if ( pi_sys_context->timer_running == FALSE )
	{
		*mu = 0.0f;
		return VS_ERROR;
	}
	DOUBLE timer_elapsed =
		pi_sys_context->time_elapsed -
		pi_sys_context->timer_start;
	if ( timer_elapsed > pi_sys_context->timer_length )
	{
		*mu = 1.0f;
		pi_sys_context->timer_running = FALSE;
	}
	else
	{
		*mu = timer_elapsed / pi_sys_context->timer_length;
	}
	return VS_OK;
}


// ---------- StartTimer ----------
/*!
\brief
\author Gareth Edwards
\return INT - IF timer stopped THEN VS_OK ELSE VS_ERROR 
\note 2017
*/
INT SysContext::StartTimer(DOUBLE length)
{
	BOOL timer_currently_running = pi_sys_context->timer_running;
	pi_sys_context->timer_start = pi_sys_context->time_elapsed;
	pi_sys_context->timer_length = length;
	pi_sys_context->timer_running = TRUE;
	return timer_currently_running ? VS_ERROR : VS_OK;
}

