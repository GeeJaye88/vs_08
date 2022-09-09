////////////////////////////////////////////////////////////////////////////////

// ---------- vs_sys_context.h ----------
/*!
\file vs_sys_context.h
\brief Interface for the Sys(tem)Context class.
\author Gareth Edwards
*/

#pragma once


// ---- vs system include
#include "../header/vs_inc_defs.h"
#include "../header/vs_sys_version.h"


// ---- vs library include
#include "../../vs_library/header/vs_vnode.h"


// ---------- namespace ----------
namespace vs_system
{


	// ---------- SysContext ----------
	/*!
	\brief Interface to the SysContext class.
	\author Gareth Edwards
	*/
	class SysContext
	{

	public:


		// ---- enumerated id's
		enum SysContextID
		{

			// ---- text

				// CHAR* - interface methods are Get/SetChar
				APP_TITLE              = 11, // Size == VS_MAXCHARNAME
				APP_NAME               = 12,
				CONFIG_NAME            = 19,
				DEV_ENV                = 18,
				APP_FOLDER_PATH        = 13, // Size == VS_MAXCHARPATHLEN
				PROJECT_FOLDER_PATH    = 14,
				APP_CONFIG_FILE        = 15,
				SYS_CONFIG_FILE        = 16,
				VIEW_CONFIG_FILE       = 17,


			// ---- config

				// BOOL - 
				CONFIG_MODE            = 21,


			// ---- flags

				// BOOL - interface methods are Get/SetBool
				DEVELOPMENT            = 23, // Initialised FALSE

				CONSOLE                = 31, // Initialised TRUE
				DEBUG                  = 32, // Initialised FALSE
				LOG                    = 33, // Initialised FALSE
				LOG_TIME               = 22, // Initialised FALSE

				INSTALL_DATA           = 34, // Initialised FALSE

				CURSOR                 = 35, // Initialised TRUE
				TOUCH                  = 36, // Initialised FALSE

				WINDOWED               = 37, // Initialised TRUE
				DESKTOP                = 38, // Initialised FALSE
				BORDER_STYLE           = 39, // Initialised FALSE
				MENU_STYLE             = 40, // Initialised FALSE
				OVERLAP_STYLE          = 41, // Initialised TRUE
			
				ACTIVE                 = 42,
				MAXIMISED              = 43,
				MINIMISED              = 44,
				MOVED                  = 45,
				MOVING                 = 46,
				SIZING                 = 47,
				BLOCKED                = 48,

				FORMAT                 = 50, // Initialised LANDSCAPE
				LANDSCAPE              = 51,
				PORTRAIT               = 52,


			// ---- files
				LOG_FILES              = 49, // Initialised 10


			// ---- time

				// DOUBLE - interface method is GetDouble (hence cannot be set via SetDouble)
				TIME_MS_DELTA         = 91, // Initialised 0 - set each frame
				TIME_MS_ELAPSED       = 92, // Initialised 0 - set each frame

			// ---- graphics

				// DWORD - interface methods are Get/SetDWord
				MSG                    = 51,
				AAQ                    = 52, // Initialised 4
				FPS                    = 53, // Initialised 60
				FPS_NOW                = 54, // == FPS

				// FLOAT - interface methods are Get/SetFloat
				BORDER_WIDTH           = 55, // Initialised 1 

				// FLOAT - interface methods are Get/SetFloat
				BG_RED                 = 56, // Initialised 0.0
				BG_GREEN               = 57, // Initialised 0.4
				BG_BLUE                = 58, // Initialised 0.6

				// BOOL - interface methods are Get/SetBool
				TEXTURES               = 59, // Initialised TRUE


			// ---- window

				// DWORD - interface methods are Get/SetDWord

				// window dimensions
				MIN_WIDTH              = 61, // Initialised 400
				MIN_HEIGHT             = 62, // Initialised 300
				CLIENT_WIDTH           = 63, // Initialised 4096
				CLIENT_HEIGHT          = 64, // Initialised 2048
				MAX_WIDTH              = 65,
				MAX_HEIGHT             = 66,

				// window move and resize
				PREVIOUS_WIDTH         = 71,
				PREVIOUS_HEIGHT        = 72,
				DESKTOP_WIDTH          = 73,
				DESKTOP_HEIGHT         = 74,
				WINDOW_WIDTH           = 75,
				WINDOW_HEIGHT          = 76,
				WINDOW_STYLE           = 77,

				// window misc
				X_OFFSET               = 81,
				Y_OFFSET               = 82,
				CX_BORDER              = 83,
				CX_TOP                 = 84,
				WINDOW_HANDLE          = 85,
				WINDOW_TRIGGER         = 89,
				MSG_WPARAM             = 86,
				MSG_LPARAM             = 87,
				MSG_USER_EVENT_COUNT   = 88,


			// ---- window

				// BOOL - interface method is Get/SetBool
				APP_LICENSED           = 90, // Initialised TRUE

				// DWORD - interface method is GetDWord

				// system version
				SYS_VERSION_MAJOR      = 91,
				SYS_VERSION_MINOR      = 92,
				SYS_VERSION_PATCH      = 93,
				SYS_VERSION_BUILD      = 94,

				// DWORD - interface methods are Get/SetDWord

				// application version
				APP_VERSION_MAJOR      = 95, // Initialised 0
				APP_VERSION_MINOR      = 96, // Initialised 0
				APP_VERSION_PATCH      = 97, // Initialised 0
				APP_VERSION_BUILD      = 98, // Initialised 0

		};

	public:

		// ---- cdtor
			SysContext();
			~SysContext();


		// ---- framework
			INT Configure(vs_system::VNode*);
			INT ConfigureWindow();


		// ---- is


		// ---- get
			void   GetClientRect(RECT*);
			DOUBLE GetTimeDelta();
			DOUBLE GetTimeElapsed();


		// ---- get by type
			INT    GetPiSysContextStatus();
			BOOL   GetBool(UINT);
			DOUBLE GetDouble(UINT);
			CHAR  *GetChar(UINT);
			DWORD  GetDWord(UINT);
			FLOAT  GetFloat(UINT);


		// ---- set
			INT SetMovingRect(RECT*);


		// ---- set by type
			INT SetBool(UINT, BOOL);
			INT SetDouble(UINT, DOUBLE);
			INT SetChar(UINT, CHAR *);
			INT SetDWord(UINT, DWORD);
			INT SetFloat(UINT, FLOAT);


		// ---- mu
			INT GetTimerProgress(DOUBLE *);
			INT StartTimer(DOUBLE);

	private:


		// ---- private implementation
			class PI_SysContext; PI_SysContext *pi_sys_context;

	};


}