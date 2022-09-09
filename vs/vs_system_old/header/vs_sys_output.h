////////////////////////////////////////////////////////////////////////////////

// ---------- vs_sys_output.h ----------
/*!
\file vs_sys_output.h
\brief Interface for the Sys(tem)Output class.
\author Gareth Edwards
\note WINDOWS DEPENDANT
*/

#pragma once


// ---- vs include ----
#include "../header/vs_inc_defs.h"
#include "../header/vs_inc_dems.h"


// ---- vs include ----
#include <vector>
#include <stack>


// ---------- namespace ----------
namespace vs_system
{ 


	// ---------- SysOutput ----------
	/*!
	\brief Ouput to stdout (console), log file (logfile.txt) and message box
	\author Gareth Edwards
	*/
	class SysOutput
	{

	public:

		SysOutput();
		virtual ~SysOutput(void);

		// ---- message
			VOID Message(CHAR *);
			VOID MessageToLog(CHAR *);
			INT  MessageError(CHAR *, CHAR*, CHAR*);
			INT  MessageEscape(UINT);
			VOID MessageOk(INT, INT);
			VOID MessageOk(UINT);
			INT  MessageOk(CHAR *);
			INT  MessageQuery(CHAR *);
			INT  MessageReport(CHAR *, CHAR *, CHAR *);
			INT  MessageSetup(CHAR *, CHAR *);

		// ---- log
			INT  WriteFile(CHAR *);

		// ---- get
			BOOL GetDebug();
			BOOL GetLog();
			BOOL GetLogTime();

		// ---- set
			VOID SetDebug(BOOL);
			VOID SetLog(BOOL);
			VOID SetLogTime(BOOL);
			INT  Initialise(UINT);

	private:

		// ---- private implementation
			class PI_SysOutput; PI_SysOutput *pi_sys_output;

	};


}