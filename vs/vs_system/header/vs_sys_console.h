////////////////////////////////////////////////////////////////////////////////

// ---------- vs_sys_console.h ----------
/*!
\file vs_sys_console.h
\brief Interface for the Sys(tem)Console class.
\author Gareth Edwards
\note WINDOWS DEPENDANT
*/

#pragma once


// ---- vs include ----
#include "../header/vs_inc_defs.h"


// ---- windows ----
#ifdef OS_WINDOWS
#include <fcntl.h>
#include <fstream>
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#endif


// ---------- namespace ----------
namespace vs_system
{


	// ---------- Console ----------
	/*!
	\brief Console.
	\author Gareth Edwards
	*/
	class SysConsole
	{

	public:
		SysConsole(void);
		~SysConsole(void);

	private:

		#ifdef OS_WINDOWS

		// ---- enumerated console colour ----
		enum Color
		{
			Black       = 0,
			Grey        = FOREGROUND_INTENSITY,
			LightGrey   = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE,
			White       = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
			Blue        = FOREGROUND_BLUE,
			Green       = FOREGROUND_GREEN,
			Cyan        = FOREGROUND_GREEN | FOREGROUND_BLUE,
			Red         = FOREGROUND_RED,
			Purple      = FOREGROUND_RED   | FOREGROUND_BLUE,
			LightBlue   = FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
			LightGreen  = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
			LightCyan   = FOREGROUND_GREEN | FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
			LightRed    = FOREGROUND_RED   | FOREGROUND_INTENSITY,
			LightPurple = FOREGROUND_RED   | FOREGROUND_BLUE  | FOREGROUND_INTENSITY,
			Orange      = FOREGROUND_RED   | FOREGROUND_GREEN,
			Yellow      = FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		};

		HANDLE  handle_for_console_input;
		HANDLE  handle_for_console_output;
		HANDLE  handle_for_console_error;
		LPDWORD number_of_chars_written;
		LPVOID  reserved;

		void Write(CHAR*);

		#endif

	};


}