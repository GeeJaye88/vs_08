
#include <tchar.h>
#include <iostream>
#include <string>
#include <process.h>
#include <tlhelp32.h>
#include <psapi.h>


// ---------- FindAndKillProcess ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::FindAndKillProcess(CHAR *process_name)
{
	// ---- local ----
	HANDLE hProcessSnap;
	//HANDLE hProcess;
	PROCESSENTRY32 pe32;
	//DWORD dwPriorityClass;


	// ---- take a snapshot of all processes in the system  ----
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return VS_ERROR;
	}


	// ---- set the size of the structure before using it ----
	pe32.dwSize = sizeof(PROCESSENTRY32);


	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		CloseHandle(hProcessSnap);  // clean the snapshot object
		return VS_ERROR;
	}


	// ---- walk the snapshot of processes ----
	do
	{
		std::string str(pe32.szExeFile);

		// IF ---- process name to be killed ---- THEN
		if ( str == process_name )
		{
			INT result = KillProcess(pe32.th32ProcessID, 1);
		}

	} while (Process32Next(hProcessSnap, &pe32));


	CloseHandle(hProcessSnap);

	return VS_OK;
}


// ---------- KillProcess ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::KillProcess(DWORD dwProcessId, UINT uExitCode)
{
	DWORD dwDesiredAccess = PROCESS_TERMINATE;
	BOOL  bInheritHandle = FALSE;
	HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
	if (hProcess == NULL)
		return VS_ERROR;


	INT result = (INT)TerminateProcess(hProcess, uExitCode);


	CloseHandle(hProcess);

	return result;
}


// ---------- StartProcess ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

Notes:

CreateProcess, see http://msdn.microsoft.com/en-us/library/ms682425%28VS.85%29.aspx.

The command line to be executed. The maximum length of this string
is 32,768 characters, including the Unicode terminating null character.
If lpApplicationName is NULL, the module name portion of lpCommandLine
is limited to MAX_PATH characters.

The lpCommandLine parameter can be NULL. In that case, the function
uses the string pointed to by lpApplicationName as the command line.

If both lpApplicationName and lpCommandLine are non-NULL, the
null-terminated string pointed to by lpApplicationName specifies the
module to execute, and the null-terminated string pointed to by
lpCommandLine specifies the command line. The new process can use
GetCommandLine to retrieve the entire command line.

Console processes written in C can use the argc and argv arguments
to parse the command line. Because argv[0] is the module name,
C programmers generally repeat the module name as the first token
in the command line.

*/
INT application::StartProcess(
		CHAR *application_name,
		CHAR* application_args
	)
{

	// ---- get logs context ----
		/*
		vs_system::SysContext* sys_context = pi_props->app_dependency->GetSysContext();
		BOOL development = sys_context->GetBool(vs_system::SysContext::DEVELOPMENT);
		*/


	// ---- additional information ----
	STARTUPINFO si;
	PROCESS_INFORMATION pi;


	// ---- child window status
		BOOL show_child_window = FALSE;


	// ---- get handles ----
		BOOL   showChildWindow = FALSE;
		HANDLE handle_input  = GetStdHandle(STD_INPUT_HANDLE);
		HANDLE handle_output = GetStdHandle(STD_OUTPUT_HANDLE);
		HANDLE handle_error  = GetStdHandle(STD_ERROR_HANDLE);


	// ---- set the size of the structures ----
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));


	// ---- stuff....
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags    = STARTF_USESTDHANDLES;
		si.dwFlags   |= ( showChildWindow ? STARTF_USESHOWWINDOW : 0 );
		si.hStdInput  = handle_input;
		si.hStdOutput = handle_output;
		si.hStdError  = handle_error;


	// ---- use this if you want to show the child.
		si.wShowWindow = show_child_window ? SW_SHOW : SW_HIDE;


	// ---- start the program up ----
	CreateProcess(	
			application_name, // Path\\to\\name.exe
			application_args, // Command line
			NULL,             // Process handle not inheritable
			NULL,             // Thread handle not inheritable
			FALSE,            // Set handle inheritance to FALSE
			show_child_window ? CREATE_NEW_CONSOLE : CREATE_NO_WINDOW, // 2/3/2018
			//development ? 0 : CREATE_NO_WINDOW,                      // ... was
			                  // IF development mode THEN create console window
			NULL,             // Use parent's environment block
			NULL,             // Use parent's starting directory 
			&si,              // Pointer to STARTUPINFO structure
			&pi               // Pointer to PROCESS_INFORMATION structure
		);


	// ---- close process and thread handles ----
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return VS_OK;
}

