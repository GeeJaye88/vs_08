////////////////////////////////////////////////////////////////////////////////

// ---------- vs_sys_console.cpp ----------
/*!
\file vs_sys_console.cpp
\brief Implementation of the Sys(tem)Console class
\author Gareth Edwards
\note FUNCTIONALITY IS WINDOWS DEPENDANT
*/

// ---- include ----
#include "../header/vs_sys_console.h"


// ---------- Constructor ----------
/*!
\brief constructor
\author Gareth Edwards

Note:\n
This method redirects:\n
Unbuffered STDOUT to the text window.\n
Unbuffered STDIN to the text window.\n
Unbuffered STDERR to the text window.\n
Note:\n
Makes cout, wcout, cin, wcin, wcerr,
cerr, wclog and clog point to the text window as well.

*/


#include <iostream> 

vs_system::SysConsole::SysConsole ()
{

	#if defined(DEBUG)

	float fred = 2;

	#endif


	#ifdef OS_WINDOWS

	// ---- local ----
	int file_descriptor;


	// ---- allocate ----
		AllocConsole();
		SetConsoleTitle("CONSOLE");

		std::cout << "FRED" << std::endl;

	// ---- get the three handles ----
		handle_for_console_input  = GetStdHandle(STD_INPUT_HANDLE);
		handle_for_console_output = GetStdHandle(STD_OUTPUT_HANDLE);
		handle_for_console_error  = GetStdHandle(STD_ERROR_HANDLE);

	// ---- set output colour ----
		SetConsoleTextAttribute(handle_for_console_output, White);


	// ---- redirect unbuffered STDOUT to the console ----
		file_descriptor = _open_osfhandle ((LONG)handle_for_console_output, _O_TEXT);
		*stdout = *_fdopen (file_descriptor, "w");
		setvbuf (stdout, NULL, _IONBF, 0);


	// ---- redirect unbuffered STDIN to the console ----
		file_descriptor = _open_osfhandle ((LONG)handle_for_console_input, _O_TEXT);
		*stdin = *_fdopen (file_descriptor, "r");
		setvbuf (stdin, NULL, _IONBF, 0);


	// ---- redirect unbuffered STDERR to the console ----
		file_descriptor = _open_osfhandle ((LONG)handle_for_console_error, _O_TEXT);
		*stderr = *_fdopen (file_descriptor, "w");
		setvbuf (stderr, NULL, _IONBF, 0);


    // ---- make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog ----
    //      all point to console as well...
		std::ios_base::sync_with_stdio();


	// WriteConsole (handle_for_console_output, "TEST", 4, number_of_chars_written, &reserved);
	// printf("test 1 2 3\n");
	std::cout << "Hello" << std::endl;
	std::cerr << "Hello" << std::endl;
	std::clog << "Hello" << std::endl;


	#endif

}


// ---------- Destructor ----------
vs_system::SysConsole::~SysConsole()
{
	#ifdef OS_WINDOWS
	FreeConsole();
	#endif
}


// ---------- Write ----------
/*!
\brief write to the text window
\author Gareth Edwards
\param CHAR * - string to be written
\note Also use std::cout << [string] << std::endl

Example of usage:
\code

// write to the text window
console->write ("Hello World");

\endcode
*/
void vs_system::SysConsole::Write (CHAR *str)
{
	#ifdef OS_WINDOWS
	WriteConsole (
			handle_for_console_output,
			str,
			strlen(str),
			number_of_chars_written, &reserved
		);
	#endif
} 



