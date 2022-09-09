////////////////////////////////////////////////////////////////////////////////

// ---------- as2_application.hpp ----------
/*!
\file as2_application.hpp
\brief
\author Gareth Edwards
\note Vanilla C++
*/

#define _CRT_SECURE_NO_WARNINGS


// ---------- Application include ----------
#include "../header/as2_application.h"


// ---------- namespace ----------
using namespace app_setup_02;


////////////////////////////////////////////////////////////////////////////////


// ---- lambda
	auto ReplaceChar = [](
			std::string str,
			char ch1,
			char ch2
		)
	{
		for (std::string::size_type i = 0; i < str.length(); ++i)
		{
			if (str[i] == ch1)
				str[i] = ch2;
		}
		return str;
	};


// ---- version
	struct vs_semantic_versioning
	{
		std::string major = "0";
		std::string minor = "0";
		std::string patch = "0";
		std::string build = "0";
	};


// ---- system version info
	class vs_system_version_info
	{

		public:

			// ---- properties

			vs_semantic_versioning vss;
			std::string company_name   = "Observant Innovations";
			std::string file_version   = "1,4,4,1";
			std::string copyright      =  "Copyright ©";
			std::string copyright_year = "2013 - 2020";


			// ---- methods ----

			VOID Set (
						CHAR *def, // [in]
						CHAR *exp  // [in]
					)
			{
				if ( !strcmp("VS_SYS_VERSION_MAJOR", def) )
				{
					vss.major = exp;
				}
				else if ( !strcmp("VS_SYS_VERSION_MINOR", def) )
				{
					vss.minor = exp;
				}
				else if ( !strcmp("VS_SYS_VERSION_PATCH", def) )
				{
					vss.patch = exp;
				}
				else if ( !strcmp("VS_SYS_VERSION_BUILD", def) )
				{
					vss.build = exp;
				}
				else if ( !strcmp("VS_SYS_COMPANY_NAME", def) )
				{
					company_name = exp;
				}
				else if ( !strcmp("VS_SYS_COPYRIGHT_YEAR", def) )
				{
					copyright_year = exp;
				}
			};


			VOID SetFileVersion()
			{
				file_version =
					vss.major + "," +
					vss.minor + "," +
					vss.patch + "," +
					vss.build;
			};


			VOID Report ()
			{
				SetFileVersion();
				printf("System Version Info:\n");
				printf("   CompanyName      : %s\n",    company_name.c_str() );
				printf("   FileVersion      : %s\n",    file_version.c_str());
				printf("   LegalCopyright   : %s %s\n", copyright.c_str(), copyright_year.c_str() );

			};

	} vs_sys_ver_info;



// ---- application version info
	class vs_application_version_info
	{

		public:

			// ---- properties ----

			vs_semantic_versioning vss;
			std::string prod_version   = "1,0,0,1";
			std::string description    = "PATROL camera application";
			std::string internal_name  = "camera_03.exe";
			std::string original_name  = "camera_03.exe";
			std::string product_name   = "Camera";


			// ---- methods ----
			VOID Set (
					CHAR *def, // [in]
					CHAR *exp  // [in]
				)
			{
				if (!strcmp("VS_APP_VERSION_MAJOR", def))
				{
					vss.major = exp;
				}
				else if (!strcmp("VS_APP_VERSION_MINOR", def))
				{
					vss.minor = exp;
				}
				else if (!strcmp("VS_APP_VERSION_PATCH", def))
				{
					vss.patch = exp;
				}
				else if (!strcmp("VS_APP_VERSION_BUILD", def))
				{
					vss.build = exp;
				}
				else if (!strcmp("VS_APP_FILE_DESCRIPTION", def))
				{
					description = exp;
				}
				else if (!strcmp("VS_APP_INTERNAL_NAME", def))
				{
					internal_name = exp;
				}
				else if (!strcmp("VS_APP_ORIGINAL_NAME", def))
				{
					original_name = exp;
				}
				else if (!strcmp("VS_APP_PRODUCT_NAME", def))
				{
					product_name = exp;
				}
			};


			VOID SetProdVersion()
			{
				prod_version =
					vss.major + "," +
					vss.minor + "," +
					vss.patch + "," +
					vss.build;
			};


			VOID Report ()
			{
				SetProdVersion();
				printf("Application Version Info:\n");
				printf("   FileDescription  : %s\n", description.c_str()   );
				printf("   InternalName     : %s\n", internal_name.c_str() );
				printf("   OriginalFilename : %s\n", original_name.c_str() );
				printf("   ProductName      : %s\n", product_name.c_str()  );
				printf("   ProductVersion   : %s\n", prod_version.c_str());

			};


	} vs_app_ver_info;


// ---------- Private Implementation of PI_Application class ----------
class application::PI_Application
{

	public:

		// constructor/destructor
		PI_Application();
		~PI_Application();


		// methods
		INT Setup(vs_system::AppDependency *);
		INT Configure();
		INT Environment();
		INT Usage(CHAR);


		// application dependency pointers
		vs_system::AppDependency *app_dependency = NULL;
		vs_system::SysContext    *sys_context    = NULL;
		vs_system::SysOutput     *sys_output     = NULL;


		// argument count and values
		CHAR argv[VS_MAXARGV][VS_MAXCHARLEN];
		INT  argc = 0;


		// required (e.g. app_setup vs camera_03 c3)
		CHAR *dev_env       = NULL;
		CHAR *app_name      = NULL;
		CHAR *app_prefix    = NULL;


		// set
		CHAR app_namespace[VS_MAXCHARLEN];
		CHAR config_name[VS_MAXCHARLEN];


		// optional: -d flag
		BOOL  abs_folder_mode = FALSE;

		// optional: -d derived (e.g. with -d "D:/Gareth/Code/VS/dl_01/dl", proj_folder == "dl"); 
		CHAR  abs_folder[VS_MAXCHARLEN];


		// optional: -d derived (e.g. with -d [abs_folder_paf]); 
		CHAR  abs_folder_paf[VS_MAXCHARLEN];


		// optional: -a [postfix]
		CHAR *rc_texture_atlas_postfix = NULL;


		// optional: -i [prefix]
		CHAR *rc_icon_prefix = NULL;


		// ouput filenames
		CHAR wb_setup_filename[VS_MAXCHARLEN];
		CHAR wb_define_filename[VS_MAXCHARLEN];
		CHAR wb_rc_filename[VS_MAXCHARLEN];


		// development
		CHAR rel_dev_directory[4];


		// upper case
		CHAR dev_env_uc[VS_MAXCHARLEN];
		CHAR app_name_uc[VS_MAXCHARLEN];
		CHAR config_name_uc[VS_MAXCHARLEN];

		// preprocessor directives
		CHAR prev[VS_MAXARGV][VS_MAXCHARLEN];
		UINT prec = 0;


		// current output file
		FILE * output_file  = NULL;

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::PI_Application::PI_Application()
{
	app_dependency = NULL;
	sys_context = NULL;
	sys_output = NULL;
}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
application::PI_Application::~PI_Application()
{
	;
}


// ---------- Setup ----------
/*!
\brief Initialise
\author Gareth Edwards
*/
INT application::PI_Application::Setup(vs_system::AppDependency *ad)
{
	// ---- local
		INT result = VS_ERROR;


	// ---- inject application dependency
		app_dependency = ad;


	// ---- accelerate
		sys_context = ad->GetSysContext();
		sys_output  = ad->GetSysOutput();


	// ---- overflow
		argc = app_dependency->GetArgc();
		if ( argc >= VS_MAXARGV )
		{
			printf("\n");
			printf("Error: Exceeded maximum [%d] # of command line arguments.", VS_MAXARGV);
			printf("\n");
			return VS_EXIT;
		} 


	// ---- development
		if ( sys_context->GetBool( sys_context->DEVELOPMENT) )
		{
			strcpy(rel_dev_directory, "../");

			INT dev_choice = 1;
			switch ( dev_choice )
			{
				case 1:
					{
						argc = 4;
						strcpy(argv[0], "vsx");
						strcpy(argv[1], "vsx");
						strcpy(argv[2], "app_setup_02");
						strcpy(argv[3], "as2");
					}
					break;

				case 2:
					{
						argc = 4;
						strcpy(argv[0], "vs");
						strcpy(argv[1], "vs");
						strcpy(argv[2], "camera_03");
						strcpy(argv[3], "c3");
					}
					break;

				case 3:
					{
						argc = 6;
						strcpy(argv[0], "vs");
						strcpy(argv[1], "vs");
						strcpy(argv[2], "darkland_02");
						strcpy(argv[3], "d2");
						strcpy(argv[4], "-d");
						strcpy(argv[5], "D:/Gareth/Code/VS/dl_01/dl");
					}
					break;

				case 4:
					{
						argc = 6;
						strcpy(argv[0], "vs");
						strcpy(argv[1], "vs");
						strcpy(argv[2], "star_01");
						strcpy(argv[3], "s1");
						strcpy(argv[4], "-a");
						strcpy(argv[5], "s1");
					}
					break;
			}

		}
		else
		{
			strcpy(rel_dev_directory, "");
			for ( INT i = 0; i < argc; i++ )
			{
				strcpy(argv[i], app_dependency->GetArgv(i));
			}
		}


	// ---- usage
		if ( argc == 2 )
		{
			if (!strcmp(argv[1], "-e"))
				result = Usage('e');
			else
				result = Usage('u');
			return VS_EXIT;
		}


	// ---- less than minimum args
		if ( argc < 4 )
		{
			printf("\n");
			printf("Error: %d - too few command line arguments.\n", argc);
			result = Usage('u');
			return VS_EXIT;
		}


	// ---- set required param
		dev_env       = argv[1];
		app_name      = argv[2];
		app_prefix    = argv[3];


	// ---- default other names
		strcpy(app_namespace, app_name);
		strcpy(config_name, app_name);


	// ---- convert dev env && namespace to upper case versions
		strcpy(dev_env_uc, dev_env);
		INT i = 0;
		while ( dev_env_uc[i] )
		{
			dev_env_uc[i] = toupper(dev_env_uc[i]);
			i++;
		}

		strcpy(app_name_uc, app_name);
		i = 0;
		while (app_name_uc[i])
		{
			app_name_uc[i] = toupper(app_name_uc[i]);
			i++;
		}
		strcpy(config_name_uc, app_name_uc);


	// ---- set optional default
		strcpy(abs_folder_paf, "");             // e.g. -d "D:\Gareth\Code\VS\dl_01\dl"
		strcpy(abs_folder, "");                 // e.g. "dl", derived from abs_folder_paf
		rc_icon_prefix = argv[3];               // e.g. -i


	// ---- init param
		prec = 0;
		INT arg = 4;
		while ( arg < argc )
		{

			if (arg + 1 < argc)
			{

				// ---- intialise absolute appplication param
				//
				// TBD: needs error check throughout 
				//
					if ( !strcmp("-d", argv[arg]) )
					{

						// ---- what a mess: tidy abs_folder_paf and extract abs_folder ---- 
						strcpy(abs_folder_paf, argv[arg + 1]);
						INT len = strlen(abs_folder_paf);
						if (abs_folder_paf[len] != '\\' && abs_folder_paf[len] != '/')
							strcat(abs_folder_paf, "/");
						INT start = len - 1;
						while (start > 0 && abs_folder_paf[start] != '\\' && abs_folder_paf[start] != '/') { start--; } start++;
						INT i = 0;
						while (start < len) { abs_folder[i++] = abs_folder_paf[start++]; } abs_folder[i] = 0;
						for (INT i = 0; i < len; i++) abs_folder_paf[i] = abs_folder_paf[i] == '\\' ? '/' : abs_folder_paf[i];
						arg += 2;

						// YES!
						abs_folder_mode = TRUE;

					}

				// ---- texture atlas postfix
					else if ( !strcmp("-a", argv[arg]) )
					{
						rc_texture_atlas_postfix = argv[arg + 1];
						arg += 2;
					}

				// ---- config name 
					else if ( !strcmp("-c", argv[arg]) )
					{

						strcpy(config_name, argv[arg + 1]);
						arg += 2;

						// to upper case
						strcpy(config_name_uc, config_name);
						i = 0;
						while (config_name_uc[i])
						{
							config_name_uc[i] = toupper(config_name_uc[i]);
							i++;
						}

					}

				// ---- icon prefix
					else if ( !strcmp("-i", argv[arg]) )
					{
						rc_icon_prefix = argv[arg + 1];
						arg += 2;
					}

				// ---- preprocessor directive
					else if ( !strcmp("-p", argv[arg]) )
					{
						strcpy(prev[prec++], argv[arg + 1]);
						arg += 2;
					}

				// ----
					else
					{
						printf("\n");
						printf("Error: Unrecognised command line option %s.", argv[arg]);
						printf("\n");
						return VS_EXIT;
					}

			}
			else
			{
				printf("\n");
				printf("Error: Unmatched command line option %s.", argv[arg]);
				printf("\n");
				return VS_EXIT;
			}
		}


	// ---- init output filenames
		strcpy(wb_setup_filename, "");
		strcpy(wb_define_filename, "");
		strcpy(wb_rc_filename, "");


	// ---- finally test folder and files
		result = Environment();
		if ( result == VS_EXIT ) return VS_EXIT;

	return VS_OK;
}


// ---------- Configure ----------
/*!
\brief Configure private implementation properties object
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT application::PI_Application::Configure()
{

	return VS_OK;
}


// ---------- Environment ----------
/*!
\brief Environment
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Application::Environment()
{

	// ---- local
		INT result = VS_ERROR;


	// ---- lambda

		auto FileExists = [](const char* fileName) {
			DWORD attribs = ::GetFileAttributesA(fileName);
			if (attribs & FILE_ATTRIBUTE_DIRECTORY) return FALSE;
			if (attribs != INVALID_FILE_ATTRIBUTES) return TRUE;
			return FALSE;
		};


		auto FolderExists = [](const char* dirName) {
			DWORD attribs = ::GetFileAttributesA(dirName);
			if (attribs == INVALID_FILE_ATTRIBUTES) return FALSE;
			BOOL is_folder = attribs & FILE_ATTRIBUTE_DIRECTORY;
			return is_folder;
		};


	// ---- test dev env (e.g. "../vs/vs_application", "../vsx/vsx_application" )
		CHAR devenv_folder[VS_MAXCHARLEN];
		result = sprintf(
				devenv_folder,
				"%s%s/%s_application",
				rel_dev_directory,
				dev_env,
				dev_env
			);
		BOOL devenv_folder_exists = FolderExists(devenv_folder);
		if ( !devenv_folder_exists )
		{
			printf("\n");
			printf("Error: Development environment %s not found.", devenv_folder);
			printf("\n");
			return VS_EXIT;
		}


	// ---- test app name and paf (e.g. "D:/Gareth/Code/VS/dl_01/dl//dl_application/darkland_02")
		CHAR app_folder[VS_MAXCHARLEN];
		if ( abs_folder_mode )
		{
			result = sprintf(
					app_folder,
					"%s/%s_application/%s",
					abs_folder_paf,
					abs_folder,
					app_name
				);
		}
		else
		{
			result = sprintf(
					app_folder,
					"%s%s/%s_application/%s",
					rel_dev_directory,
					dev_env,
					dev_env,
					app_name
				);
		}
		BOOL app_folder_exists = FolderExists(app_folder);
		if ( !app_folder_exists )
		{
			printf("\n");
			printf("Error: Application folder %s not found.", app_folder);
			printf("\n");
			return VS_EXIT;
		}


	// ---- test app prefix
		CHAR filename[VS_MAXCHARLEN];
		if ( abs_folder_mode )
		{
			result = sprintf(
					filename,
					"%s/%s_application/%s/header/%s_application.h",
					abs_folder_paf,
					abs_folder,
					app_name,
					app_prefix
				);
		}
		else
		{
			result = sprintf(
					filename,
					"%s%s/%s_application/%s/header/%s_application.h",
					rel_dev_directory,
					dev_env,
					dev_env,
					app_name,
					app_prefix
				);
		}
		BOOL file_exists = FileExists(filename);
		if ( !file_exists )
		{
			printf("\n");
			printf("Error: Application include header %s not found.", filename);
			printf("\n");
			return VS_EXIT;
		}

	return VS_OK;
}


// ---------- Usage ----------
/*!
\brief Usage
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::PI_Application::Usage(CHAR mode)
{

	printf("Usage: app_setup [dev env] [app name] [app prefix] [...]\n");
	printf("\n");

	printf("Argument 1: \"app_setup\" OR \"app_setup.exe\"\n");
	printf("         2: [dev(elopment) env(ironment)] e.g. \"vs\", \"vsx\".\n");
	printf("         3: [app name] initialise application name and namespace.\n");
	printf("         4: [app prefix] initialise application \".cpp\" & \".h\" file prefix.\n");
	printf("\n");

	printf("Command line options:\n");
	printf("\n");
	printf("         -a [atlas postfix] initialise the texture atlas postfixfix.\n");
	printf("            If not specified then NO postfix is used.\n");
	printf("         -c [config name] initialise the application config name.\n");
	printf("         -d [app path] initialise the [dev env] directory path.\n");
	printf("            If not specified then no folder path used.\n");
	printf("         -e List expanded usage (e.g., \"app_setup -e\").\n");
	printf("         -i [icon prefix] initialise the icon prefix.\n");
	printf("            If not specified then the app prefix is used.\n");
	printf("         -p [value] iinitialise a preprocessor #define directive.\n");
	printf("            Up to 8 of these can be set.\n");
	printf("         -u List usage (e.g., \"app_setup -u\").\n");

	printf("\n");

	if (mode != 'e') return VS_COMPLETED;

	printf("Note, \"app_setup\" generates:\n");
	printf("\n");
	printf("         1: \"[vs/vsx]_app_setup.h\" file which is included several times in, the\n");
	printf("            \"[vs/vsx]_app_interface.h\" and \"[vs/vsx]_sys_main.cpp\" files, to\n");
	printf("            setup an application.\n");
	printf("\n");
	printf("         2: \"[app prefix]_define.h\" file (see the -p option above) containing\n");
	printf("            preprocessor #define directives.\n");
	printf("\n");
	printf("         3: \"[vs/vsx].rc\" resource file, a non-executable data file used used\n");
	printf("            to hold statements for different files that are going to be compiled\n");
	printf("            into a binary resource file, such as bitmaps, icons and version info.\n");
	printf("\n");

	printf("Dependencies include:\n");
	printf("\n");
	printf("         1. VS folder structure and file naming conventions (see VS documentation).\n");
	printf("\n");
	printf("         2. System version file: \"[vs/vsx]\\[vs/vsx]_system\\vs_sys_version.h\".\n");
	printf("\n");
	printf("         3. Application version file: \"[dev env]\\[dev env]_application\\\n");
	printf("            \\[app name]\\header\\[app prefix]_version.h\".\n");
	printf("\n");

	printf("Examples:\n");
	printf("\n");
	printf("         1. app_setup vs review_02 r2\n");
	printf("\n");
	printf("            This creates a \"vs_setup.h\" file located in directory\n");
	printf("            \"vs\\vs_application\\header\", which includes an application\n");
	printf("            class interface file \"r2_application.h\", located in directory\n");
	printf("            \"vs\\vs_application\\review_02\\header\".\n");
	printf("            \n");
	printf("            The development environment is \"vs\", and the application name and\n");
	printf("            namespace is \"camera_03\".\n");
	printf("\n");
	printf("         2. app_setup vsx app_setup_02 as2\n");
	printf("\n");
	printf("            This creates a \"vsx_setup.h\" file located in directory\n");
	printf("            \"vsx\\vsx_application\\header\", which includes an application\n");
	printf("            class interface file \"as2_application.h\", located in directory\n");
	printf("            \"vsx\\vsx_application\\app_setup_02\\header\".\n");
	printf("            \n");
	printf("            The development environment is \"vsx\", and the application name and\n");
	printf("            namespace is \"app_setup_02\".\n");
	printf("\n");
	printf("         3. app_setup.exe vs camera_03 c3 -i c2 -p VS_CAMERA_03_RECORD\n");
	printf("\n");
	printf("            This creates a \"vs_setup.h\" file located in directory\n");
	printf("            \"vs\\vs_application\\header\", which includes an application\n");
	printf("            class interface file \"c3_application.h\", located in directory\n");
	printf("            \"vs\\vs_application\\camera_03\\header\".\n");
	printf("            \n");
	printf("            The development environment is \"vs\", and the application name and\n");
	printf("            namespace is \"camera_03\".\n");
	printf("\n");
	printf("         4. app_setup.exe aa aa_03 aa3 -d \"D:\\code\\aa_project\aa\" \n");
	printf("\n");
	printf("            This creates a \"vs_setup.h\" file located in directory\n");
	printf("            \"vs\\vs_application\\header\", which includes an application\n");
	printf("            class interface file \"aa3_application.h\", located in directory\n");
	printf("            \"D:\\code\\aa_project\\aa\\aa_application\\aa_03\\header\".\n");
	printf("            \n");
	printf("            The development environment is \"aa\", and the application name and\n");
	printf("            namespace is \"aa_03\".\n");
	printf("\n");

	return VS_COMPLETED;
}

