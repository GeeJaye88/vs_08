////////////////////////////////////////////////////////////////////////////////

// ---------- av2_application.cpp ----------
/*!
\file av2_application.cpp
\brief Implementation of the application class
\author Gareth Edwards
\note Vanilla C++
*/

#define _CRT_SECURE_NO_WARNINGS


// ---- include
#include "../header/av2_application.h"


// ---- namespace
using namespace app_version_02;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Application class ----------
class application::PI_Application
{

public:

	// ---- c/dtor
		PI_Application();
		~PI_Application();


	// ---- methods
		INT Setup(vs_system::AppDependency *);
		INT Usage(CHAR);


	//---- pointers
		vs_system::AppDependency *app_dependency;
		vs_system::SysContext    *sys_context;
		vs_system::SysOutput     *sys_output;


	// ---- folder
		vs_system::Folder        *folder;


	// ---- argument count and values
		CHAR argv[VS_MAXARGV][VS_MAXCHARLEN];
		UINT argc = 0;


	// ---- version
		CHAR *name  = "vs";
		CHAR *major = NULL;
		CHAR *minor = NULL;
		CHAR *patch = NULL;
		CHAR *build = NULL;
		CHAR *incr  = NULL;


	// ---- version filename
		CHAR filename[VS_MAXCHARLEN];


	// --- log
		CHAR *descrip = NULL;

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::PI_Application::PI_Application()
{

	// ---- application dependency pointers
		app_dependency = NULL;
		sys_context = NULL;
		sys_output = NULL;


	// ---- folder
		folder = NULL;
}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::PI_Application::~PI_Application()
{
	if ( folder != NULL) delete folder;
}


// ---------- (private) Setup ----------
/*!
\brief Initialise
\author Gareth Edwards
*/
INT application::PI_Application::Setup(
			vs_system::AppDependency *ad
		)
{

	// ---- inject application dependency
		app_dependency = ad;


	// ---- accelerate
		sys_context = ad->GetSysContext();
		sys_output = ad->GetSysOutput();


	// ---- catch overflow
		argc = app_dependency->GetArgc();
		if (argc >= VS_MAXARGV)
		{
			printf("Error: Exceeded maximum [%d] # of command line arguments.\n", VS_MAXARGV);
			printf("\n");
			return VS_ABORT;
		}


	// ---- development
		//#define TEST

		#ifdef TEST
			argc = 11;
			strcpy(argv[0], "set_version");
			strcpy(argv[1], "-n");
			strcpy(argv[2], "c3");
			strcpy(argv[3], "-M");
			strcpy(argv[4], "6");
			strcpy(argv[5], "-m");
			strcpy(argv[6], "5");
			strcpy(argv[7], "-p");
			strcpy(argv[8], "4");
			strcpy(argv[9], "-i");
			strcpy(argv[10], "2");
		#else
			for ( UINT i = 0; i < argc; i++ )
			{
				strcpy(argv[i], app_dependency->GetArgv(i));
			}
		#endif


	// ---- options
		try
		{
			UINT arg = 1;
			while ( arg < argc )
			{

				if ( !strcmp("-h", argv[arg]) )
				{
					Usage('h');
				}
				else
				{
					if ( arg + 1 < argc )
					{

						// ---- version
							if      ( !strcmp("-n", argv[arg]) ) { name  = argv[arg + 1]; arg += 2; }  // NAME
							else if ( !strcmp("-M", argv[arg]) ) { major = argv[arg + 1]; arg += 2; }  // MAJOR
							else if ( !strcmp("-m", argv[arg]) ) { minor = argv[arg + 1]; arg += 2; }  // MINOR
							else if ( !strcmp("-p", argv[arg]) ) { patch = argv[arg + 1]; arg += 2; }  // PATCH
							else if ( !strcmp("-b", argv[arg]) ) { build = argv[arg + 1]; arg += 2; }  // BUILD
							else if ( !strcmp("-i", argv[arg]) )                                       // BUILD INCREMENT
											{
												incr  = argv[arg + 1];
												arg += 2;
												INT i = atoi(incr);
												if ( i < 0 || i > 9 )
												{
													CHAR err_msg[128];
													sprintf(err_msg, "Illegal build increment value %s.", argv[arg]);
													throw (err_msg);
												}
											} 
							else if ( !strcmp("-d", argv[arg]) ) { descrip = argv[arg + 1]; arg += 2; } // LOG TEXT

						// ---- error
							else
							{
								CHAR err_msg[128];
								sprintf(err_msg, "Unrecognised command line option %s.", argv[arg]);
								throw (err_msg);
							}

					}
					else
					{
						throw ("[-command value] pair mismatch");
					}
				}
			}
		}
		catch (CHAR *err_msg)
		{
			printf("Error: %s\n\n", err_msg);
			return VS_ABORT;
		}


	// ---- format filename
		if ( !strcmp(name, "vs") )
		{
			strcpy(filename,"vs_sys_version.h");
		}
		else
		{
			sprintf(filename, "%s_version.h", name);
		}

	return VS_OK;
}


// ---------- (private) Usage ----------
/*!
\brief Usage
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Application::Usage(CHAR mode)
{

	printf("Usage: app_version [app prefix] [...]\n");
	printf("\n");

	printf("Arguments: \"app_version\" OR \"app_version.exe\"\n");
	printf("           followed by: [-command value] pairs \n");
	printf("\n");

	printf("Command line options:\n");
	printf("\n");
	printf("         -n [CHAR] set version name.\n");
	printf("         -M [#] set MAJOR version #.\n");
	printf("         -m [#] set MINOR version #.\n");
	printf("         -p [#] set PATCH version #.\n");
	printf("         -b [#] set BUILD version #.\n");
	printf("         -i [#] increment BUILD version by #.\n");
	printf("\n");

	return VS_COMPLETED;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Application declaration for static properties ----------
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
	if ( pi_app ) delete pi_app;
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


	// ---- store application dependency reference
		result = pi_app->Setup(app_dependency);
		if (result <= VS_ERROR)
		{
			return result;
		}


	// ---- allocate and create Folder
		pi_app->folder = new vs_system::Folder();


	// ---- initialise callbacks
		result = pi_app->folder->SetCallbackOnFile(CallbackOnFile);
		result = pi_app->folder->SetCallbackOnFolder(CallbackOnFolder);

	return VS_OK;
}


// ---------- Display ----------
/*!
\brief Process
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::Display()
{

	// ---- local
		INT result = VS_OK;


	// ---- get current directory path
		CHAR current_directory_path[VS_MAXCHARPATHLEN];
		char *cwd = getcwd(current_directory_path, VS_MAXCHARPATHLEN);


	// ---- scan all folders
		result = pi_app->folder->Scan(current_directory_path);

	return VS_COMPLETED;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::Cleanup()
{

	INT result = 0;

	return VS_OK;
}


// -------------------- Callback --------------------


// ---------- CallbackOnFile ----------
/*!
\brief CallbackOnFile
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::CallbackOnFile(vs_system::FolderInfo *info)
{

	if ( !strcmp(info->file_name.c_str(), pi_app->filename) )
	{
		INT result = UpdateVersionFile(info);
		return VS_ERROR;
	}

	return VS_OK;
}


// ---------- CallbackOnFolder ----------
/*!
\brief CallbackOnFolder
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::CallbackOnFolder(vs_system::FolderInfo *info)
{
	if ( *info->folder_name.c_str() == '.' || *info->folder_name.c_str() == 0 )
	{
		return VS_ERROR;
	}
	else
	{ 
		// ---- split path into folders stack
		std::string folders[64];
		INT top = info->folder_path.length() - 1;
		for (INT i = info->folder_depth; i >= 0;  i--)
		{
			INT bottom = info->folder_path.find_last_of('\\', top) + 1;
			folders[i] = info->folder_path.substr(bottom, top-bottom+1);
			top = bottom - 2;
		}

		/*
		for (INT i = 0; i <= info->folder_depth; i++)
		{
			printf("    - %d : %s\n", i, folders[i].c_str());
		}
		*/ 

		/*
		INT result = printf(
				"%3d - Folder: %s (%s)\n",
				info->folder_depth,
				info->folder_path.c_str(),
				info->folder_name.c_str()
			);
		*/

		return VS_OK;
	}

	return VS_OK;
}


// ---------- UpdateVersionFile ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/

//#include <string>       // std::string
//#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
//#include <sstream>
#include <iterator>
//#include <numeric>
//#include <algorithm>


INT application::UpdateVersionFile(
		vs_system::FolderInfo * info
	)
{
	// ---- lambda
		auto print_line = []()
		{
			printf("--------------------------------------------------------------------------------\n");
		};


	// ---- local
		CHAR text[4][VS_MAXCHARLEN] = { NULL, NULL, NULL, NULL };
		CHAR line[256][VS_MAXCHARLEN];
		UINT line_count = 0;
		CHAR *mode = !strcmp("vs", pi_app->name)  ? "SYS" : "APP";
		BOOL  doit[4] =
		{
			pi_app->major != NULL,
			pi_app->minor != NULL,
			pi_app->patch != NULL,
			pi_app->build != NULL || pi_app->incr != NULL,

		};


	// ---- file OK?
		std::string file_name = info->folder_path + "\\" + info->file_name;
		std::ifstream read_file(file_name);
		if ( read_file.is_open() )
		{

			// ---- read & store
				while
					(
						!read_file.getline(line[line_count++], VS_MAXCHARLEN).eof() &&
						line_count < 256
					)
				{
					;
				}
				read_file.close();


			// ---- update & write
				std::ofstream write_file(file_name);
				if (write_file.is_open())
				{

					UINT hash_count = 0;
					for (UINT i = 0; i < line_count; i++)
					{

						if ( line[i][0] == '#' )
						{

							if ( hash_count < 4 && doit[hash_count] )
							{
								CHAR *t = text[hash_count];
								switch ( hash_count )
								{
									case 0: { sprintf(t, "#define VS_%s_VERSION_MAJOR %s", mode, pi_app->major); } break;
									case 1: { sprintf(t, "#define VS_%s_VERSION_MINOR %s", mode, pi_app->minor); } break;
									case 2: { sprintf(t, "#define VS_%s_VERSION_PATCH %s", mode, pi_app->patch); } break;
									case 3:
										{
											if ( pi_app->build != NULL )
											{
												sprintf(t, "#define VS_%s_VERSION_BUILD %s", mode, pi_app->build);
											}
											else
											{

												// ---- split
													std::string str = line[i];
													std::istringstream iss(str);
													std::vector<std::string> results(
															std::istream_iterator<std::string>{iss},
															std::istream_iterator<std::string>()
														);

												// ---- re-assemble
													INT incr_build = atoi(results[2].c_str()) + atoi(pi_app->incr);
													sprintf(t, "#define VS_%s_VERSION_BUILD %d", mode, incr_build);

											}
										}
										break;
									default:
										break;
								}

								// ---- write & stamp
									SYSTEMTIME SystemTime;
									GetLocalTime(&SystemTime);
									write_file            <<
										t                 <<
										""                <<
										" // "            <<
										SystemTime.wYear  <<
										"/"               <<
										SystemTime.wMonth <<
										"/"               <<
										SystemTime.wDay   <<
										std::endl;
							}
							else
							{
								write_file << line[i] << std::endl;
							}

							hash_count++;

						}
						else
						{
							write_file << line[i] << std::endl;
						}

					}
					write_file.close();


				// ---- success
					INT result = printf("Updated %s :\n",file_name.c_str() );
					for ( UINT i = 0; i < 4; i++ )
					{
						if ( doit[i] != NULL )
						{
							INT result = printf("   %s\n", text[i]);
						}
					}
					print_line();


				// ---- log file OK?
					std::string log_file_name = "app_version.log";
					std::ofstream write_file(log_file_name, std::ios_base::app);
					if ( write_file.is_open() )
					{

						// ---- stamp
							SYSTEMTIME SystemTime;
							GetLocalTime(&SystemTime);
							write_file             <<
								"Update on "       <<
								SystemTime.wYear   <<
								"/"                <<
								SystemTime.wMonth  <<
								"/"                <<
								SystemTime.wDay    <<
								" at "             <<
								SystemTime.wHour   <<
								":"                <<
								SystemTime.wMinute <<
								":"                <<
								SystemTime.wSecond <<
								std::endl;

						// ---- description
							if (pi_app->descrip != NULL)
							{
								write_file             <<
									"   Description: " <<
									pi_app->descrip    <<
									std::endl;
							}

						// ---- file
							if (pi_app->descrip != NULL)
							{
								write_file            <<
									"   File: "       <<
									file_name.c_str() <<
									std::endl;
							}

						// ---- list version updates
							for (UINT i = 0; i < 4; i++)
							{
								if (doit[i] != NULL)
								{
									write_file << "   " << text[i] << std::endl;
								}
							}

						// ---- close
							write_file.close();

					}
					else
					{
						INT result = printf(
								"Write log file error:\n"
								"   - Failed to update %s.\n",
								log_file_name.c_str()
							);
					}


				return VS_OK;
			}


			// ---- write failed
				INT result = printf(
						"Write file error:\n"
						"   - Failed to update %s.\n", 
						file_name.c_str()
					);
				print_line();

			return VS_ERROR;
		}


	// ---- read failed
		INT result = printf(
				"Read file error:\n"
				"   - Failed to update %s.\n",
				file_name.c_str()
			);
		print_line();

	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////