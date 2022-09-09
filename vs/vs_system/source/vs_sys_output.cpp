////////////////////////////////////////////////////////////////////////////////

// ---------- vs_sys_output.cpp ----------
/*!
\file vs_sys_output.cpp
\brief Implementation of the Sys(tem)Output class
\brief Ouput to stdout (VS_DEBUG defined) or a logfile (VS_LOGFILE defined)
\author Gareth Edwards
*/

// ---- include ----
#include "../header/vs_sys_output.h"


#define VS_SYS_OUTPUT_ON

#ifdef VS_SYS_OUTPUT_ON
//#include <debugapi.h>
#include <fstream>
#include <iostream>		// for std::cout, etc, ...
#endif

 
// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private implementation Sytem Output class----------

class SysOutput::PI_SysOutput
{
public:

	// ---- constructor ----
	PI_SysOutput()
	{

		debug        = false;
		log          = false;
		log_time     = false;
		line_number  = 0;

		CHAR *text[] = 
		{
			"Are you sure you want to QUIT?",                             // 0
			"System Setup: Create",                                       // 1
			"System Setup: Initialise",                                   // 2
			"System Setup: Framework",                                    // 3
			"System Initialise: Read configuration files",                // 4
			"System Initialise: Configure application context",           // 5
			"System Initialise: Create window",                           // 6
			"System Initialise: Setup graphics",                          // 7
			"Application framework: Setup",                               // 8
			"Application framework: Setup graphics",                      // 9
			"Application messaging: Display",                             // 10
			"Application framework: Cleanup graphics",                    // 11
			"Application framework: Cleanup",                             // 12
			"Application framework: Cleanup graphics interface",          // 13
			"Windows create: Register class",                             // 14
			"Windows create: Create",                                     // 15
			"System ResizeBackBuffer: Reset device (D3DERR_INVALIDCALL)", // 16
		};

		for (INT i=0; i<17; i++)
		{
			strcpy(message_text[i], text[i]);
		}

	};


	// ---- destructor ----
	virtual ~PI_SysOutput(void)
	{
		;
	}


	// ---- methods ----
	INT  GetLogTime(CHAR*);
	INT  DeleteLogFiles(CHAR *, UINT);


	// ---- properties ----
	BOOL debug;
	BOOL log;
	BOOL log_time;
	unsigned int line_number;
	CHAR message_text[18][VS_MAXCHARLEN];
	CHAR path[VS_MAXCHARPATHLEN];
	CHAR tmp[VS_MAXCHARLEN];

};


// ---------- GetLogTime ----------
/*!
\brief Get log time stamp as a char string
\author Gareth Edwards
\param CHAR * - returned time stamp (No bounds checking!!!)
\return INT - ERROR <= VS_ERROR < VS_OK
*/
INT SysOutput::PI_SysOutput::GetLogTime(CHAR *time_stamp)
{

	// ---- initialise returned arg ----
	strcpy(time_stamp, "");


	// IF -- log time ON -- THEN
	if ( log_time )
	{

		// ---- get time NOW ----
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);


		//
		// GetSystemTime() returns the current time in UTC
		//


		// ---- format ----
		char tmp[VS_MAXCHARLEN];
		sprintf(
				tmp,
				"%4d/%2d/%2d-%2d:%2d:%2d:%3d : ",
				SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond,
				SystemTime.wMilliseconds
			);


		// ---- cleanup unwanted chars ----
		INT len_filename = (INT)strlen(tmp);
		for (INT i = 0; i < len_filename-3; i++) tmp[i] = tmp[i] == ' ' ? '0' : tmp[i];


		// ---- copy into returned arg ----
		strcpy(time_stamp, tmp);
	}

	return VS_OK;
}


// ---------- DeleteLogFiles ----------
/*!
\brief Delete log files
\author Gareth Edwards
\return INT - ERROR <= VS_ERROR < VS_OK
*/
INT SysOutput::PI_SysOutput::DeleteLogFiles(CHAR *folder_path, UINT retained_log_files)
{

	// ---- local ----
		HANDLE hFind = INVALID_HANDLE_VALUE;
		WIN32_FIND_DATA fdata;
		std::stack<std::string> folders;
		std::string full_path;


	// ---- counter ----
		INT file_counter = 0;


	// ---- two passes ----
		for (UINT pass = 0; pass<2; pass++)
		{

			// ---- current file counter ----
				INT current_file_counter = 0;

			// ---- store ----
				std::string current_path = folder_path;
				folders.push(current_path.c_str());

			// ---- recurse until no more folders ----
				while (!folders.empty())
				{

					// ---- init current path from folders stack top ----
						current_path = folders.top();

						// ---- init full directory path.. ----
						full_path = current_path + "\\" + "*";


						// ---- then pop folders stack ----
						folders.pop();


						// FOR -- all files in this folder
						hFind = FindFirstFile(full_path.c_str(), &fdata);
						if ( hFind != INVALID_HANDLE_VALUE )
						{

							do
							{

								if (strcmp(fdata.cFileName, ".") != 0 &&
									strcmp(fdata.cFileName, "..") != 0)
								{
									// IF -- folder -- THEN -- push current path onto foldersa stack
									if (fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
									{
										folders.push(current_path + "\\" + fdata.cFileName);
									}
									// ELSE -- files, which are listed AFTER folders
									else
									{
										// increment # file counter
										current_file_counter++;

										// IF -- first -- THEN -- count # of files 
										if (pass == 0)
										{
											file_counter++;
										}
										// ELSE --delete excess
										else
										{
											// IF -- file # less than # to be retained -- THEN -- delete
											if (current_file_counter <= file_counter - (INT)retained_log_files)
											{
												CHAR paf[VS_MAXCHARPATHLEN];
												sprintf(paf, "%s%s", current_path.c_str(), fdata.cFileName);
												BOOL result = DeleteFile(paf);
											}
										}
									}
								}
							} while (FindNextFile(hFind, &fdata) != 0);
						}


						// ---- check that this folder listing is complete ----
							if (GetLastError() != ERROR_NO_MORE_FILES)
							{
								FindClose(hFind);
								return VS_ERROR;
							}


						// ---- close this folder ----
							FindClose(hFind);
							hFind = INVALID_HANDLE_VALUE;

				}

		}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
SysOutput::SysOutput()
{
	pi_sys_output = new PI_SysOutput();
}


// ---------- Destructor ----------
SysOutput::~SysOutput(void)
{
	delete pi_sys_output;
	pi_sys_output = NULL;
}


// ---------- Message ----------
VOID SysOutput::Message(CHAR *text)
{

	#ifdef VS_SYS_OUTPUT_ON

	if ( pi_sys_output->debug || pi_sys_output->log )
	{
		sprintf(
				pi_sys_output->tmp,
				"+++ %4d %s\n",
				pi_sys_output->line_number,
				text
			);
		OutputDebugString(pi_sys_output->tmp);
	}


	if ( pi_sys_output->log )
	{
		CHAR log_time[VS_MAXCHARNAME];
		INT result = pi_sys_output->GetLogTime(log_time);
		std::ofstream log_file(pi_sys_output->path, std::ios_base::out | std::ios_base::app);
		log_file << log_time << pi_sys_output->line_number << " " << text << std::endl;
	}


	pi_sys_output->line_number++;

	#endif

}


// ---------- Message ----------
VOID SysOutput::Message(CHAR *text, INT value)
{

	#ifdef VS_SYS_OUTPUT_ON

	if ( pi_sys_output->debug || pi_sys_output->log )
	{
		sprintf(
				pi_sys_output->tmp,
				"+++ %4d %s %d\n",
				pi_sys_output->line_number,
				text,
				value
			);
		OutputDebugString(pi_sys_output->tmp);
	}

	if ( pi_sys_output->log )
	{
		CHAR log_time[VS_MAXCHARNAME];
		INT result = pi_sys_output->GetLogTime(log_time);
		std::ofstream log_file(pi_sys_output->path, std::ios_base::out | std::ios_base::app);
		log_file << log_time << pi_sys_output->line_number << " " << text << " " << value << std::endl;
	}

	pi_sys_output->line_number++;

	#endif

}


// ---------- Message ----------
VOID SysOutput::Message(CHAR *text, CHAR *format, FLOAT value)
{

	sprintf(
			pi_sys_output->tmp,
			"+++ %4d %s %6.4f\n",
			pi_sys_output->line_number,
			text,
			value
		);

	OutputDebugString(pi_sys_output->tmp);

}


// ---------- Message ----------
VOID SysOutput::Message(CHAR *txt, UINT num, FLOAT value[] , CHAR * format[])
{

	CHAR msg[256] = "";
	UINT msg_len = 0;

	UINT txt_len = strlen(txt);
	if (txt_len > 0)
	{
		strcpy(msg, txt);
		msg_len = txt_len;
	}

	for (UINT i = 0; i < num; i++)
	{
		CHAR val_txt[32] = "";
		sprintf(val_txt, "%f %s\n", value[i], format[i]);
		UINT val_txt_len = strlen(val_txt);
		if (msg_len + val_txt_len < 256)
		{
			strcpy(msg, val_txt);
			msg_len += val_txt_len;
		}
		else
			break;
	}

	OutputDebugString(msg);

}


// ---------- MessageToLog ----------
VOID SysOutput::MessageToLog(CHAR *text)
{

	#ifdef VS_SYS_OUTPUT_ON

	if ( pi_sys_output->log )
	{
		CHAR log_time[VS_MAXCHARNAME];
		INT result = pi_sys_output->GetLogTime(log_time);
		std::ofstream log_file(pi_sys_output->path, std::ios_base::out | std::ios_base::app);
		log_file << log_time << pi_sys_output->line_number << " " << text << std::endl;
	}

	pi_sys_output->line_number++;

	#endif

}


// ---------- MessageEscape ----------
INT SysOutput::MessageEscape(UINT id)
{

	#ifdef VS_SYS_OUTPUT_ON

	if ( pi_sys_output->debug || pi_sys_output->log )
	{
		sprintf(
				pi_sys_output->tmp,
				"+++ %4d %s\n",
				pi_sys_output->line_number,
				pi_sys_output->message_text[id]
			);
		OutputDebugString(pi_sys_output->tmp);

		// std::cout << pi_sys_output->line_number << " " << pi_sys_output->message_text[id] << std::endl;

	}
		
	
	if ( pi_sys_output->log )
	{
		CHAR log_time[VS_MAXCHARNAME];
		INT result = pi_sys_output->GetLogTime(log_time);
		std::ofstream log_file(pi_sys_output->path, std::ios_base::out | std::ios_base::app);
		log_file << log_time << pi_sys_output->line_number << " " << pi_sys_output->message_text[id] << std::endl;
	}


	UINT result = 0;
	#ifdef OS_WINDOWS
		result = ::MessageBoxA(
				NULL,
				pi_sys_output->message_text[id],
				"You pressed the ESCAPE key!",
				MB_YESNO | MB_ICONQUESTION
			);
	#endif

	pi_sys_output->line_number++;

	return result;

	#else

	return 0;

	#endif

}


// ---------- MessageError ----------
INT SysOutput::MessageError(CHAR *error, CHAR *name, CHAR *value)
{

#ifdef VS_SYS_OUTPUT_ON

	if ( pi_sys_output->debug || pi_sys_output->log )
	{
		sprintf(
				pi_sys_output->tmp,
				"+++ %4d Error: %s, %s: %s\n",
				pi_sys_output->line_number,
				error,
				name,
				value
			);
		OutputDebugString(pi_sys_output->tmp);

		// std::cout << pi_sys_output->line_number << "Error: " << error << ", " << name << ": " << value << std::endl;

	}


	if ( pi_sys_output->log )
	{
		CHAR log_time[VS_MAXCHARNAME];
		INT result = pi_sys_output->GetLogTime(log_time);
		std::ofstream log_file(pi_sys_output->path, std::ios_base::out | std::ios_base::app);
		log_file << log_time << pi_sys_output->line_number << "Error: " << error << ", " << name << ": " << value << std::endl;
	}

	CHAR text[VS_MAXCHARLEN];
	sprintf(text, "%s\n\n%s: %s\n\nClick OK to continue.", error, name, value);


	#ifdef OS_WINDOWS
	INT result = ::MessageBoxA(
			NULL,
			text,
			"VS ERROR",
			MB_OK | MB_ICONEXCLAMATION
		);
	#endif


	pi_sys_output->line_number++;
	return result;

	#else

	return 0;

	#endif

}


// ---------- MessageOk ----------
VOID SysOutput::MessageOk( INT error_id, INT error_code)
{

	// IF -- not a standard error code -- THEN -- return
	if ( error_code < VS_ERROR ) return;


	#ifdef VS_SYS_OUTPUT_ON

	char text[128];
	if ( error_code == VS_ERROR )
	{
		sprintf(
				text,
				"%s - failed with error code VS_ERROR",
				pi_sys_output->message_text[error_id]
			);
	}
	else
	{
		sprintf(
				text,
				"%s - failed with error code %d!",
				pi_sys_output->message_text[error_id],
				error_code
			);
	}


	if ( pi_sys_output->debug || pi_sys_output->log )
	{
		sprintf(
			pi_sys_output->tmp,
			"+++ %4d Error: %s\n",
			pi_sys_output->line_number,
			text
		);
		OutputDebugString(pi_sys_output->tmp);

		// std::cout << pi_sys_output->line_number << "Error: " << error << ", " << name << ": " << value << std::endl;

	}


	if ( pi_sys_output->log )
	{
		CHAR log_time[VS_MAXCHARNAME];
		INT result = pi_sys_output->GetLogTime(log_time);
		std::ofstream log_file(pi_sys_output->path, std::ios_base::out | std::ios_base::app);
		log_file << log_time << pi_sys_output->line_number << " " << text << std::endl;
	}


	#ifdef OS_WINDOWS
		::MessageBox(
			0,
			text,
			"VS ERROR",
			0
		);
	#endif


	pi_sys_output->line_number++;

	#endif

}


// ---------- MessageOk ----------
INT SysOutput::MessageOk(CHAR *comment)
{

	#ifdef VS_SYS_OUTPUT_ON

	if ( pi_sys_output->debug || pi_sys_output->log )
	{
		sprintf(
			pi_sys_output->tmp,
			"+++ %4d %s\n",
			pi_sys_output->line_number,
			comment
		);
		OutputDebugString(pi_sys_output->tmp);

		// std::cout << pi_sys_output->line_number << " " << comment << std::endl;

	}


	if ( pi_sys_output->log )
	{
		CHAR log_time[VS_MAXCHARNAME];
		INT result = pi_sys_output->GetLogTime(log_time);
		std::ofstream log_file(pi_sys_output->path, std::ios_base::out | std::ios_base::app);
		log_file << log_time << pi_sys_output->line_number << " " << comment << std::endl;
	}


	#ifdef OS_WINDOWS
	INT result = ::MessageBoxA(NULL,
			comment,
			"VS Message",
			MB_OK | MB_ICONEXCLAMATION
		);
	#endif


	pi_sys_output->line_number++;
	return result;

	#else

	return 0;

	#endif

}


// ---------- MessageQuery ----------
INT SysOutput::MessageQuery(CHAR *query)
{

	#ifdef VS_SYS_OUTPUT_ON

	if ( pi_sys_output->debug || pi_sys_output->log )
	{
		sprintf(
			pi_sys_output->tmp,
			"+++ %4d %s\n",
			pi_sys_output->line_number,
			query
		);
		OutputDebugString(pi_sys_output->tmp);

		// std::cout << pi_sys_output->line_number << " " << query << std::endl;

	}
	

	if ( pi_sys_output->log )
	{
		CHAR log_time[VS_MAXCHARNAME];
		INT result = pi_sys_output->GetLogTime(log_time);
		std::ofstream log_file(pi_sys_output->path, std::ios_base::out | std::ios_base::app);
		log_file << log_time << pi_sys_output->line_number << " " << query << std::endl;
	}		
	

	#ifdef OS_WINDOWS
		INT result = ::MessageBoxA( NULL,
			query,
			"VS Query",
			MB_YESNO | MB_ICONEXCLAMATION
		);
	#endif


	pi_sys_output->line_number++;
	return result;

	#else

	return 0;

	#endif

}


// ---------- MessageReport ----------
INT SysOutput::MessageReport(CHAR *report, CHAR *name, CHAR *value)
{

	#ifdef VS_SYS_OUTPUT_ON

	if ( pi_sys_output->debug || pi_sys_output->log )
	{
		sprintf(
			pi_sys_output->tmp,
			"+++ %4d Setup: %s, %s : %s\n",
			pi_sys_output->line_number,
			report,
			name,
			value
		);
		OutputDebugString(pi_sys_output->tmp);

		// std::cout << pi_sys_output->line_number << "Setup: " << report << ", " << name << ": " << value << std::endl;

	}


	if ( pi_sys_output->log )
	{
		CHAR log_time[VS_MAXCHARNAME];
		INT result = pi_sys_output->GetLogTime(log_time);
		std::ofstream log_file(pi_sys_output->path, std::ios_base::out | std::ios_base::app);
		log_file << log_time << pi_sys_output->line_number << "Report: " << report << ", " << name << ": " << value << std::endl;
	}


	CHAR text[VS_MAXCHARLEN];
	sprintf(text, "%s\n\n%s: %s\n\nClick OK to continue.", report, name, value);


	#ifdef OS_WINDOWS
	INT result = ::MessageBoxA(
			NULL,
			text,
			"VS Report",
			MB_OK | MB_ICONEXCLAMATION
		);
	#endif


	pi_sys_output->line_number++;

	return result;

	#else

	return 0;

	#endif

}


// ---------- MessageSetup ----------
INT SysOutput::MessageSetup(CHAR *status, CHAR *value)
{

	#ifdef VS_SYS_OUTPUT_ON

	CHAR msg[80];
	sprintf(msg, " %s: %s", status, value);
	INT msg_len = strlen(msg);
	for (INT i = msg_len; i < 75; i++) msg[i] = '-';
	msg[75] = 0;


	BOOL begin = strcmp("Begin", status) == 0;


	if ( pi_sys_output->debug || pi_sys_output->log )
	{
		sprintf(
			pi_sys_output->tmp,
			"%s+++ %4d%s\n",
			(begin ? "\n" : ""),
			pi_sys_output->line_number,
			msg
		);
		OutputDebugString(pi_sys_output->tmp);

		// std::cout << (begin ? "\n" : "") << pi_sys_output->line_number << msg << std::endl;

	}


	if ( pi_sys_output->log )
	{
		CHAR log_time[VS_MAXCHARNAME];
		INT result = pi_sys_output->GetLogTime(log_time);
		std::ofstream log_file(pi_sys_output->path, std::ios_base::out | std::ios_base::app);
		log_file << (begin ? "\n" : "") << log_time << pi_sys_output->line_number << msg << std::endl;
	}


	pi_sys_output->line_number++;

	return 1;

	#else

	return 0;

	#endif

}


// ---------- WriteFile ----------
INT SysOutput::WriteFile(CHAR *paf)
{

	// IF -- path NULL -- THEN -- error
	if ( paf == NULL) return VS_ERROR;


	// IF -- NOT log -- THEN -- ok
	if ( !pi_sys_output->log ) return VS_OK;


	// IF -- file does not exist -- THEN
	DWORD file_handle_path = GetFileAttributesA(paf);
	if ( file_handle_path != INVALID_FILE_ATTRIBUTES )
	{

		// IF -- file opens ok -- THEN
		std::ofstream log_file(pi_sys_output->path, std::ios_base::out | std::ios_base::app);
		std::ifstream myfile(paf);
		if ( myfile.is_open() )
		{

			// bookend
			log_file << "" << std::endl;
			log_file << "===========================================================================" << std::endl;
			log_file << "Begin:=====================================================================" << std::endl;

			log_file << "" << std::endl;
			log_file << "FILE: " << paf << std::endl;
			log_file << "" << std::endl;
			CHAR line[VS_MAXCHARLEN];
			while ( !myfile.getline(line, VS_MAXCHARLEN).eof() )
			{
				log_file << line << std::endl;
			}
			log_file << "" << std::endl;


			// bookend
			log_file << "End:-----------------------------------------------------------------------" << std::endl;
			log_file << "" << std::endl;
		}
		else
		{
			return VS_ERROR;
		}
	}
	else
	{
		return VS_ERROR;
	}

	return VS_OK;
}


// ---------- GetDebug ----------
BOOL SysOutput::GetDebug()
{
	return pi_sys_output->debug;
}


// ---------- GetLog ----------
BOOL SysOutput::GetLog()
{
	return pi_sys_output->log;
}


// ---------- GetLogTime ----------
BOOL SysOutput::GetLogTime()
{
	return pi_sys_output->log_time;
}


// ---------- SetDebug ----------
VOID SysOutput::SetDebug(BOOL debug)
{
	pi_sys_output->debug = debug;
}


// ---------- SetLog ----------
VOID SysOutput::SetLog(BOOL log)
{
	pi_sys_output->log = log;
}


// ---------- SetLogTime ----------
VOID SysOutput::SetLogTime(BOOL log_time)
{
	pi_sys_output->log_time = log_time;
}


// ---------- Initialise ----------
INT SysOutput::Initialise(UINT log_files)
{

	#ifdef VS_SYS_OUTPUT_ON


	// ---- local ----
	CHAR path[VS_MAXCHARPATHLEN];


	// IF -- NOT log -- THEN -- ok
	if ( !pi_sys_output->log ) return VS_OK;


	// ---- get user directory path ----

		#ifdef OS_WINDOWS

		CHAR *userProfile;

		// 64 BIT CHANGE from unsigned int to size_t
		size_t userProfileLen;
	
		_dupenv_s(&userProfile, &userProfileLen, "USERPROFILE");
		strcpy_s(path, VS_MAXCHARPATHLEN, userProfile);
		UINT len = strlen(path);
		for ( UINT i=0; i<len; i++ ) path[i] = path[i] == '\\' ? '/' : path[i];

		#else OTHER OS

		; // TODO

		#endif OS_WINDOWS


	// ---- append Appdata/local ----
	strcat(path, "/Appdata/Local/");


	// IF -- exists -- THEN
	if ( GetFileAttributes(path) != INVALID_FILE_ATTRIBUTES )
	{

		// Get System time
			SYSTEMTIME SystemTime;
			// GetSystemTime(&SystemTime);
			GetLocalTime(&SystemTime);

		// IF -- "Observant Innovations" folder does not exist -- THEN -- create 
			strcat(path, "Observant Innovations/");
			if ( GetFileAttributes(path) == INVALID_FILE_ATTRIBUTES) INT result = _mkdir(path);

		// IF -- "Logs" folder does not exist -- THEN -- create 
			strcat(path, "logs/");
			if ( GetFileAttributes(path) == INVALID_FILE_ATTRIBUTES ) INT result = _mkdir(path);

		// delete unwanted log files
			INT result = pi_sys_output->DeleteLogFiles(path, log_files);

		// catenate log
			char dt[VS_MAXCHARLEN];
			sprintf(dt, "%4d%2d%2d_%2d%2d%2d_log_file.txt",
				SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond);
			INT len = strlen(dt);
			for ( INT i=0; i<len; i++ ) dt[i] = dt[i] == ' ' ? '0' : dt[i];
			strcat(path, dt);

		// store
			strcpy(pi_sys_output->path, path);

		// create / Wipe
			std::ofstream log_file(pi_sys_output->path, std::ios_base::out);

		// DateTime
			std::ofstream app_log_file(pi_sys_output->path, std::ios_base::out | std::ios_base::app);
			app_log_file << "Observant Innovations VS Log File" << std::endl;

		// DateTime
			sprintf( dt, "Started on (YMD) %4d/%2d/%2d at (HMS) %2d:%2d:%2d and (MS) %3d",
				SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond,
				SystemTime.wMilliseconds);
			app_log_file << dt << std::endl;

	}

	// ELSE -- report AND exit
	else
	{
		INT result = ::MessageBoxA(NULL,
			"Could not find the User Documents folder!\n\nPlease create, then restart application.",
			"VS System Error",
			MB_OK | MB_ICONEXCLAMATION);
		exit(0);
	}

	return VS_OK;

	#else

	return 0;

	#endif

}


////////////////////////////////////////////////////////////////////////////////