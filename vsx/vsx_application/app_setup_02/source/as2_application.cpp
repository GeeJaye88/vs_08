////////////////////////////////////////////////////////////////////////////////

// ---------- s2_application.cpp ----------
/*!
\file s2_application.cpp
\brief Implementation of the application class
\author Gareth Edwards
\note Vanilla C++
*/

#define _CRT_SECURE_NO_WARNINGS


// ---------- include
#include "../header/as2_application.h"
#include "../hpp/as2_application.hpp"

#include <string>
#include <iostream>
//#include <sstream>

// ---------- namespace ----------
using namespace app_setup_02;


////////////////////////////////////////////////////////////////////////////////



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
	delete pi_app;
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
	INT result = pi_app->Setup(app_dependency);

	/*
	std::string str;
	std::getline(std::cin, str);

	printf(str.c_str());
	printf("\n");
	*/

	return result;
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
		INT result = VS_ERROR;


	// ---- begin
		printf("--------------------------------------------------------------------------------\n");


	// ---- write application interface setup file ----
		result = WriteAppInterfaceSetupFile();
		if ( result != VS_ERROR )
		{
			printf("Application setup with:\n");
			printf("   Environment      : %s\n", pi_app->dev_env);
			printf("   App name         : %s\n", pi_app->app_name);
			printf("   Config name      : %s\n", pi_app->config_name);
			printf("   Namespace        : %s\n", pi_app->app_namespace);
			printf("   App prefix       : %s\n", pi_app->app_prefix);
			printf("   Icon prefix      : %s\n", pi_app->rc_icon_prefix);
			printf("   Atlas postfix    : %s\n", pi_app->rc_texture_atlas_postfix);
			if (pi_app->sys_context->GetBool(pi_app->sys_context->DEVELOPMENT))
				printf("WARNING : In DEVELOPMENT mode!\n");
		}
		else
		{
			printf("\n");
			printf(
				"Error: Failed to open application setup file %s!",
				pi_app->wb_setup_filename
			);
			printf("\n");
			return VS_EXIT;
		}


	// ---- read version info
		if (GetVersionInfo("sys") == VS_ERROR) return VS_EXIT;
		if (GetVersionInfo("app") == VS_ERROR) return VS_EXIT;


	// ---- create
		printf("Updated:\n");
		printf("   Setup file       : \"%s\"\n", pi_app->wb_setup_filename);


	// ---- write application directive file
		{
			result = WriteAppDirectivesFile();
			if ( result != NULL )
			{
				printf(
					"   Directive file   : \"%s\"\n",
					pi_app->wb_define_filename
				);
			}
			else
			{
				printf("\n");
				printf(
					"Error: Failed to open application directive file %s!",
					pi_app->wb_define_filename
				);
				printf("\n");
				return VS_EXIT;
			}
		}


	// ---- write resource file
		{
			result = WriteVsRcFile();
			if ( result != VS_ERROR )
			{
				printf(
					"   Resource file    : \"%s\"\n",
					pi_app->wb_rc_filename
				);
			}
			else
			{
				printf("\n");
				printf(
					"Error: Failed to open application resource file %s!",
					pi_app->wb_rc_filename
				);
				printf("\n");
				return -1;
			}
		}


	// ---- show defintions
		result = ShowDefinitions("Variant");
		if (!strcmp("vs", pi_app->dev_env))
			result = ShowDefinitions("Camera");


	// ---- end
		printf("--------------------------------------------------------------------------------\n");

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


// ---------- GetVersionInfo ----------
/*!
\brief GetVersionInfo
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::GetVersionInfo(CHAR *type)
{

	// ---- local
		INT result = 0;


	// ---- set system flag
		BOOL is_sys = FALSE;
		if ( !strcmp(type, "sys") )
		{
			is_sys = TRUE;
		}
		else if (!strcmp(type, "app"))
		{
			;
		}
		else
		{
			return VS_EXIT;
		}


	// ---- system or application version info
		CHAR vs_version_filename[VS_MAXCHARLEN];
		if ( is_sys )
		{
			sprintf(
					vs_version_filename,
					"%s%s/%s_system/header/vs_sys_version.h",
					pi_app->rel_dev_directory,
					"vs",
					"vs"
				);
		}
		else
		{ 
			if ( pi_app->abs_folder_mode )
			{
				sprintf(
						vs_version_filename,
						"%s%s_application/%s/header/%s_version.h",
						pi_app->abs_folder_paf,
						pi_app->abs_folder,
						pi_app->app_name,
						pi_app->app_prefix
					);
			}
			else
			{
				result = sprintf(
						vs_version_filename,
						"%s%s/%s_application/%s/header/%s_version.h",
						pi_app->rel_dev_directory,
						pi_app->dev_env,
						pi_app->dev_env,
						pi_app->app_name,
						pi_app->app_prefix
					);
			}
		}


	// ---- include auto str lambdas
		#include <../vs_07/vs/vs_system/lambda/vs_auto_str.h>


	// ---- IF open THEN read
		CHAR line[512];
		std::ifstream myfile(vs_version_filename);
		if ( myfile.is_open() )
		{
			CHAR *word[32];
			INT lineCount = 0;
			while ( !myfile.getline(line, 512).eof() )
			{
				UINT num_words = 0;
				switch ( line[0] )
				{
					case '#' :
						num_words = StrSplitIntoWords(line, strlen(line), word);
						if (is_sys)
							vs_sys_ver_info.Set(word[1], word[2]);
						else
							vs_app_ver_info.Set(word[1], word[2]);
						break;
					default:
						break;
				}
			}
			myfile.close();
			if (is_sys)
				vs_sys_ver_info.Report();
			else
				vs_app_ver_info.Report();
		}
		else
		{
			printf("\n");
			printf("Error: Failed to open version file %s!", vs_version_filename);
			printf("\n");
			return -1;
		}

	return VS_OK;
}


// ---------- ShowDefinitions ----------
/*!
\brief ShowDefinitions
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::ShowDefinitions(CHAR *name)
{

	// ---- local
		INT result = 0;


	// ---- filename
		CHAR vs_filename[VS_MAXCHARLEN];
		if ( !strcmp("Variant", name) )
		{
			sprintf(
					vs_filename,
					"%s%s/vs_application/%s/header/%s_variant.h",
					pi_app->rel_dev_directory,
					pi_app->dev_env,
					pi_app->app_name,
					pi_app->app_prefix
				);
		}
		else if ( !strcmp("Camera", name) )
		{
			sprintf(
					vs_filename,
					"%svs/vs_library/header/vs_cam_define.h",
					pi_app->rel_dev_directory
				);
		}
		else
		{
			return VS_OK;
		}


	// ---- IF open THEN read ----
	INT hdef_count = 0;
	CHAR line[512];
	std::ifstream myfile(vs_filename);
	if ( myfile.is_open() )
	{
		BOOL first_def = TRUE;
		while ( !myfile.getline(line, 512).eof() )
		{
			INT sc = 0;
			while ( line[sc] == ' ' && sc < 16 ) sc++;
			if ( sc < 16 )
			{
				CHAR test_hdef0[5];
				for (INT i = 0; i < 5; i++) test_hdef0[i] = i == 4 ? 0 : line[i + sc];
				if ( !strcmp(test_hdef0, "#def") )
				{
					if ( hdef_count > 0 )
					{
						if ( first_def )
						{
							printf("%s defintions (found in \"%s\"):\n", name, vs_filename);
							first_def = FALSE;
						}
						printf("   %s\n", &line[sc]);
					}
					hdef_count++;
				}
			}
		}
		myfile.close();
	}

	return VS_OK;
}


// ---------- WriteAppInterfaceSetupFile ----------
INT application::WriteAppInterfaceSetupFile()
{
	INT result = VS_ERROR;

	if ( pi_app->abs_folder_mode )
	{
		result = sprintf(
				pi_app->wb_setup_filename,
				"%s%s/%s_application/header/%s_app_setup.h",
				pi_app->rel_dev_directory,
				"vs",
				"vs",
				"vs"
			);
	}
	else
	{
		result = sprintf(
				pi_app->wb_setup_filename,
				"%s%s/%s_application/header/%s_app_setup.h",
				pi_app->rel_dev_directory,
				pi_app->dev_env,
				pi_app->dev_env,
				pi_app->dev_env
			);
	}
	pi_app->output_file = fopen(pi_app->wb_setup_filename, "wb");
	if ( pi_app->output_file != NULL )
	{
		result = Step_0_Header();
		result = Step_1_DefineAppHashName();
		result = Step_2_InitialiseAppName();
		result = Step_3_IncludeAppVersion();
		result = Step_4_IncludeAppHeader();
		result = Step_5_InitialiseAppSocket();
		result = Step_6_InitialiseAppInterface();
		result = fclose(pi_app->output_file);
	}
	else
	{
		return VS_ERROR;
	}

	return VS_COMPLETED;
}


// ---------- Step 0: Write setup interface header ----------
INT application::Step_0_Header()
{

	fputs("////////////////////////////////////////////////////////////////////////////////\n", pi_app->output_file);
	fputs("// ---------- vs_app_setup.h ----------\n",    pi_app->output_file);
	fputs("/*!\n",                                        pi_app->output_file);
	fputs("\\file vs_app_setup.h\n",                      pi_app->output_file);
	fputs("\\brief Application interface setup\n",        pi_app->output_file);
	fputs("\\author Gareth Edwards\n",                    pi_app->output_file);
	fputs("\\note Generated by app_setup application.\n", pi_app->output_file);
	fputs("*/\n",                                         pi_app->output_file);
	fputs("\n",                                           pi_app->output_file);
	fputs("\n",                                           pi_app->output_file);

	return VS_OK;
}


// ---------- Step 1: Define application #[name] ----------
//
INT application::Step_1_DefineAppHashName()
{

	// ---- begin
		fputs("// ---------- #DEFINE ----------\n",      pi_app->output_file);
		fputs("//\n",                                    pi_app->output_file);
		fputs("// Step 1: Create application #define\n", pi_app->output_file);
		fputs("//\n",                                    pi_app->output_file);


	// ---- local
		char defintion[256];
	

	// ---- create #define
		strcpy(defintion, "#define ");
		strcat(defintion,  pi_app->dev_env_uc);
		strcat(defintion, "_");
		if ( !strcmp(pi_app->app_name_uc, pi_app->config_name_uc) )
			strcat(defintion,  pi_app->app_name_uc);
		else
			strcat(defintion, pi_app->config_name_uc);
		strcat(defintion, "\n");


	// ---- store
		fputs(defintion,                                 pi_app->output_file);


	// ---- end
		fputs("\n",                                      pi_app->output_file);
		fputs("\n",                                      pi_app->output_file);

	return VS_OK;
}


// ---------- Step 2: Initialise application name ----------
//
INT application::Step_2_InitialiseAppName()
{

	// ---- local
		CHAR text[VS_MAXCHARLEN];
		BOOL old_version = FALSE;


	// ---- begin
		fputs("// ---------- NAME ----------\n",                         pi_app->output_file);
		fputs("//\n",                                                    pi_app->output_file);
		sprintf(text, "//Included in %s_sys_windows.cpp\n",              pi_app->dev_env);
		fputs(text,                                                      pi_app->output_file);
		fputs("//\n",                                                    pi_app->output_file);
		fputs("// Step 2: Initialise application & development names\n", pi_app->output_file);
		fputs("//\n",                                                    pi_app->output_file);
		sprintf(text, "#ifdef %s_APPLICATION_APPNAME\n",                 pi_app->dev_env_uc);
		fputs(text,                                                      pi_app->output_file);


	// ---- set
		if ( old_version )
		{

			fputs("{\n",                                                     pi_app->output_file);

			fputs("   INT result = 0;\n",                                    pi_app->output_file);

				fputs("   result = virtual_surface.sys_context->SetChar(\n", pi_app->output_file);
				fputs("         virtual_surface.sys_context->DEV_ENV,\n",    pi_app->output_file);
				fputs("         \"",                                         pi_app->output_file);
				fputs(          pi_app->dev_env,                             pi_app->output_file);
				fputs("\"\n",                                                pi_app->output_file);
				fputs("      );\n",                                          pi_app->output_file);
		
				fputs("   result = virtual_surface.sys_context->SetChar(\n", pi_app->output_file);
				fputs("         virtual_surface.sys_context->APP_NAME,\n",   pi_app->output_file);
				fputs("         \"",                                         pi_app->output_file);
				fputs(          pi_app->app_name,                            pi_app->output_file);
				fputs("\"\n",                                                pi_app->output_file);
				fputs("      );\n",                                          pi_app->output_file);

			fputs("}\n",                                                     pi_app->output_file);
	
		}


	// ---- assign
		else
		{

			fputs("{\n",                                                     pi_app->output_file);

				fputs("dev_env.assign(",                                     pi_app->output_file);
				fputs("\"",                                                  pi_app->output_file);
				fputs(pi_app->dev_env,                                       pi_app->output_file);
				fputs("\"",                                                  pi_app->output_file);
				fputs(");\n",                                                pi_app->output_file);

				fputs("app_name.assign(",                                    pi_app->output_file);
				fputs("\"",                                                  pi_app->output_file);
				fputs(pi_app->app_name,                                      pi_app->output_file);
				fputs("\"",                                                  pi_app->output_file);

				fputs(");\n",                                                pi_app->output_file);
				fputs("config_name.assign(",                                 pi_app->output_file);
				fputs("\"",                                                  pi_app->output_file);
				fputs(pi_app->config_name,                                   pi_app->output_file);
				fputs("\"",                                                  pi_app->output_file);
				fputs(");\n",                                                pi_app->output_file);

			fputs("}\n",                                                     pi_app->output_file);

		}


	// ---- end
		sprintf(text, "#undef %s_APPLICATION_APPNAME\n",                 pi_app->dev_env_uc);
		fputs(text,                                                      pi_app->output_file);
		fputs("#endif\n",                                                pi_app->output_file);
		fputs("\n",                                                      pi_app->output_file);
		fputs("\n",                                                      pi_app->output_file);

	return VS_OK;
}


// ---------- Step 3: Include application version #include ----------
//
/*
// ---------- VERSION ----------
//
//Included in vs_sys_windows.cpp
//
// Step 3: Include application [Major].[Minor].[Patch].[Build] version
//
*/
INT application::Step_3_IncludeAppVersion()
{

	// ---- local
		CHAR text[VS_MAXCHARLEN];
		BOOL old_version = FALSE;


	// ---- begin
		fputs("// ---------- VERSION ----------\n",           pi_app->output_file);
		fputs("//\n",                                         pi_app->output_file);
		sprintf(text, "//Included in %s_sys_windows.cpp\n",   pi_app->dev_env);
		fputs(text,                                           pi_app->output_file);
		fputs("//\n",                                         pi_app->output_file);
		fputs("// Step 3: Include application version info\n",pi_app->output_file);
		fputs("//\n",                                         pi_app->output_file);
		sprintf(text, "#ifdef %s_APPLICATION_APPVERSION\n",   pi_app->dev_env_uc);
		fputs(text,                                           pi_app->output_file);


	// ---- push
		if ( !old_version )
		{
			fputs("{\n",                                      pi_app->output_file);
		}


	// ---- #include
		if ( pi_app->abs_folder_mode )
		{
			sprintf(
					text,
					"#include \"%s%s_application/%s/header/%s_version.h\"\n",
					pi_app->abs_folder_paf,
					pi_app->abs_folder,
					pi_app->app_name,
					pi_app->app_prefix
				);
		}
		else
		{
			sprintf(
					text,
					"#include \"../../%s_application/%s/header/%s_version.h\"\n",
					pi_app->dev_env,
					pi_app->app_name,
					pi_app->app_prefix
				);
		}
		fputs(text,                                           pi_app->output_file);


	// ---- store & pop
		if ( !old_version )
		{
			fputs("version[0] = VS_APP_VERSION_MAJOR;\n",     pi_app->output_file);
			fputs("version[1] = VS_APP_VERSION_MINOR;\n",     pi_app->output_file);
			fputs("version[2] = VS_APP_VERSION_PATCH;\n",     pi_app->output_file);
			fputs("version[3] = VS_APP_VERSION_BUILD;\n",     pi_app->output_file);
			fputs("}\n",                                      pi_app->output_file);
		}


	// ---- end
		sprintf(text, "#undef %s_APPLICATION_APPVERSION\n",   pi_app->dev_env_uc);
		fputs(text,                                           pi_app->output_file);
		fputs("#endif\n", pi_app->output_file);
		fputs("\n",                                           pi_app->output_file);
		fputs("\n",                                           pi_app->output_file);

	return VS_OK;
}


// ---------- Step 4: Include application header ----------
//
INT application::Step_4_IncludeAppHeader()
{

	// ---- local
		CHAR text[VS_MAXCHARLEN];


	// ---- begin
		fputs("// ---------- HEADER ----------\n",            pi_app->output_file);
		fputs("//\n",                                         pi_app->output_file);
		sprintf(text, "//Included in %s_app_interface.cpp\n", pi_app->dev_env);
		fputs(text,                                           pi_app->output_file);
		fputs("//\n",                                         pi_app->output_file);
		fputs("// Step 4: Include application header\n",      pi_app->output_file);
		fputs("//\n",                                         pi_app->output_file);
		sprintf(text, "#ifdef %s_APPLICATION_INCLUDE\n",      pi_app->dev_env_uc);
		fputs(text,                                           pi_app->output_file);


	// ---- #include
		CHAR vs_setup_folder[VS_MAXCHARLEN];
		if ( pi_app->abs_folder_mode )
		{
			sprintf(
					vs_setup_folder,
					"#include \"%s%s_application/%s/header/%s_application.h\"\n",
					pi_app->abs_folder_paf,
					pi_app->abs_folder,
					pi_app->app_name,
					pi_app->app_prefix
				);
		}
		else
		{
			sprintf(
					vs_setup_folder,
					"#include \"../../%s_application/%s/header/%s_application.h\"\n",
					pi_app->dev_env,
					pi_app->app_name,
					pi_app->app_prefix
				);
		}
		fputs(vs_setup_folder,                                pi_app->output_file);


	// ---- end
		sprintf(text, "#undef %s_APPLICATION_INCLUDE\n",      pi_app->dev_env_uc);
		fputs(text,                                           pi_app->output_file);
		fputs("#endif\n",                                     pi_app->output_file);
		fputs("\n",                                           pi_app->output_file);
		fputs("\n",                                           pi_app->output_file);

	return VS_OK;
}


// ---------- Step 5: Initialise application socket ----------
//
INT application::Step_5_InitialiseAppSocket()
{

	// ---- local
		CHAR text[VS_MAXCHARLEN];


	// ---- begin
		fputs("// ---------- SOCKET ----------\n",            pi_app->output_file);
		fputs("//\n",                                         pi_app->output_file);
		sprintf(text, "//Included in %s_app_interface.cpp\n", pi_app->dev_env);
		fputs(text,                                           pi_app->output_file);
		fputs("//\n",                                         pi_app->output_file);
		fputs("// Step 5: Initialise application socket\n",   pi_app->output_file);
		fputs("//\n",                                         pi_app->output_file);
		sprintf(text, "#ifdef %s_APPLICATION_SOCKET\n",       pi_app->dev_env_uc);
		fputs(text,                                           pi_app->output_file);


	// ---- socket
		char include[256];
		sprintf(include, "%s::application *application;\n",   pi_app->app_name);
		fputs(include,                                        pi_app->output_file);


	// ---- end
		sprintf(text, "#undef %s_APPLICATION_SOCKET\n",       pi_app->dev_env_uc);
		fputs(text,                                           pi_app->output_file);
		fputs("#endif\n",                                     pi_app->output_file);
		fputs("\n",                                           pi_app->output_file);
		fputs("\n",                                           pi_app->output_file);

	return VS_OK;
}


// ---------- Step 6: Initialise application interface  ----------
//
INT application::Step_6_InitialiseAppInterface()
{

	// ---- local
		CHAR text[VS_MAXCHARLEN];


	// ---- begin
		fputs("// ---------- POINTER ----------\n",            pi_app->output_file);
		fputs("//\n",  pi_app->output_file);
		sprintf(text, "//Included in %s_app_interface.cpp\n",  pi_app->dev_env);
		fputs(text,    pi_app->output_file);
		fputs("//\n",  pi_app->output_file);
		fputs("// Step 6: Initialise application interface\n", pi_app->output_file);
		fputs("//\n",  pi_app->output_file);
		sprintf(text, "#ifdef %s_APPLICATION_POINTER\n",       pi_app->dev_env_uc);
		fputs(text,    pi_app->output_file);


	// ---- interface
		sprintf(text, "if ( app_name.compare(\"%s\") == 0 )\n", pi_app->app_name);
		fputs(text,    pi_app->output_file);
		fputs("{\n",   pi_app->output_file);
		sprintf(text, "socket.application = new %s::application();\n", pi_app->app_name);
		fputs(text,    pi_app->output_file);
		sprintf(text, "ptr_to_framework = &AppInterface::application;;\n");
		fputs(text,    pi_app->output_file);
		fputs("}\n",   pi_app->output_file);


	// ---- end
		sprintf(text, "#undef %s_APPLICATION_POINTER\n", pi_app->dev_env_uc);
		fputs(text,    pi_app->output_file);
		fputs("#endif\n", pi_app->output_file);
		fputs("\n",    pi_app->output_file);
		fputs("\n",    pi_app->output_file);

	return VS_OK;
}


// ---------- WriteAppDirectivesFile ----------
/*!
\brief WriteAppDirectivesFile
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::WriteAppDirectivesFile()
{

	INT result = 0;


	if ( pi_app->abs_folder_mode )
	{
		sprintf(
				pi_app->wb_define_filename,
				"%s/%s_application/%s/header/%s_define.h",
				pi_app->abs_folder_paf,
				pi_app->abs_folder,
				pi_app->app_name,
				pi_app->app_prefix
			);
	}
	else
	{
		result = sprintf(
				pi_app->wb_define_filename,
				"%s%s/%s_application/%s/header/%s_define.h",
				pi_app->rel_dev_directory,
				pi_app->dev_env,
				pi_app->dev_env,
				pi_app->app_name,
				pi_app->app_prefix
			);
	}


	pi_app->output_file = fopen(pi_app->wb_define_filename, "wb");
	if ( pi_app->output_file != NULL )
	{
		result = AppDirectives();
		result = fclose(pi_app->output_file);
	}
	else
	{
		return VS_EXIT;
	}

	return VS_COMPLETED;
}


// ---------- AppDirectives ----------
// e.g.,
//    #define VS_MEMORY_SHARE
//    #define VS_CAMERA_03_MONITOR
//    #define VS_CAMERA_03_RECORD
//    #define VS_CAMERA_03_STREAMING
//
INT application::AppDirectives()
{

	fputs("////////////////////////////////////////////////////////////////////////////////\n", pi_app->output_file);
	fputs("\n", pi_app->output_file);

	fputs("// ---------- ",                  pi_app->output_file);
		fputs(pi_app->app_prefix,            pi_app->output_file);
		fputs("_defintions.h ----------\n",  pi_app->output_file);
		fputs("\n",                          pi_app->output_file);
	fputs("/*!\n",                           pi_app->output_file);

	fputs("\\file ",                         pi_app->output_file);
		fputs(pi_app->app_prefix,            pi_app->output_file);
		fputs("_defintions.h\n",             pi_app->output_file);

	fputs("\\brief Include application define directives.\n",             pi_app->output_file);
	
	fputs("\\note Created by app_setup, an application setup utility.\n", pi_app->output_file);

	fputs("*/\n",                            pi_app->output_file);

	fputs("\n",                              pi_app->output_file);


	fputs("#ifndef ",                        pi_app->output_file);
		fputs(pi_app->app_name_uc,           pi_app->output_file);  // toupper
		fputs("_DEFINE\n",                   pi_app->output_file);

	fputs("#define ",                        pi_app->output_file);
		fputs(pi_app->app_name_uc,           pi_app->output_file);  // toupper
		fputs("_DEFINE\n",                   pi_app->output_file);
	
	fputs("\n",                              pi_app->output_file);

	//printf("pi_app->prec %d\n", pi_app->prec);
	for (UINT i = 0; i < pi_app->prec; i++)
	{
		//printf("#define %s\n", pi_app->prev[i]);
		fputs("#define ",                    pi_app->output_file);
			fputs(pi_app->prev[i],           pi_app->output_file);
			fputs("\n",                      pi_app->output_file);
	}


	fputs("\n",                              pi_app->output_file);

	fputs("#endif // ",                      pi_app->output_file);
		fputs(pi_app->app_name_uc,           pi_app->output_file);
		fputs("_DEFINE\n",                   pi_app->output_file);

	fputs("\n",                              pi_app->output_file);

	fputs("////////////////////////////////////////////////////////////////////////////////\n", pi_app->output_file);

	return VS_OK;
}



// ---------- WriteVsRcFile ----------
/*!
\brief WriteVsRcFile
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::WriteVsRcFile()
{

	INT result = 0;

	sprintf(
		pi_app->wb_rc_filename,
			"%s%s/%s.rc",                                  // e.g. "[...]/rs.rc"
			pi_app->rel_dev_directory,                     // Set to "../" in DEVELOPMENT mode
			pi_app->dev_env,                               // e.g. "vs" or "vsx"
			pi_app->dev_env
		);

	pi_app->output_file = fopen(pi_app->wb_rc_filename, "wb");
	if ( pi_app->output_file != NULL )
	{
		if ( !strcmp("vsx", pi_app->dev_env) )
		{
			result = VsxRc();
		}
		else
		{
			result = VsRc();
		}
		result = fclose(pi_app->output_file);
	}
	else
	{
		return VS_EXIT;
	}

	return VS_COMPLETED;
}


// ---------- VsRc ----------
INT application::VsRc()
{

	fputs("////////////////////////////////////////////////////////////////////////////////\n", pi_app->output_file);

	fputs("// VSX C++ generated resource script.\n", pi_app->output_file);
	fputs("//\n",                                    pi_app->output_file);
	fputs("//\n\n",                                  pi_app->output_file);

	fputs("#include \"vs_resources\\resource.h\"\n", pi_app->output_file);
	fputs("\n\n",                                    pi_app->output_file);



	fputs("////////////////////////////////////////////////////////////////////////////////\n", pi_app->output_file);

	fputs(

		"\n"
		"\n"
		"#define APSTUDIO_READONLY_SYMBOLS\n"
		"////////////////////////////////////////////////////////////////////////////////\n"
		"\n"
		"// VSX generated (NOT from the TEXTINCLUDE 2 resource).\n"
		"\n"
		"#include \"windows.h\"\n"
		"////////////////////////////////////////////////////////////////////////////////\n"
		"#undef APSTUDIO_READONLY_SYMBOLS\n"
		"\n"

		"////////////////////////////////////////////////////////////////////////////////\n"
		"// English(U.K.) resources\n"
		"\n"

		"#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ENG)\n"

		"#ifdef _WIN32\n"
		"LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_UK\n"
		"#pragma code_page(1252)\n"
		"#endif //_WIN32\n"
		"\n"

		"#ifdef APSTUDIO_INVOKED\n"
		"////////////////////////////////////////////////////////////////////////////////\n"
		"//\n"
		"// TEXTINCLUDE\n"
		"//\n"
		"\n"

		"1 TEXTINCLUDE\n"
		"BEGIN\n"
		"   \"vs_resources/resource.h\\0\"\n"
		"END\n"
		"\n"

		"2 TEXTINCLUDE\n"
		"BEGIN\n"
		"   \"#include \"\"afxres.h\"\"\\r\\n\"\n"
		"   \"\\0\"\n"
		"END\n"
		"\n"

		"3 TEXTINCLUDE\n"
		"BEGIN\n"
		"   \"\\r\\n\"\n"
		"   \"\\0\"\n"
		"END\n"
		"\n"

		"#endif   // APSTUDIO_INVOKED\n"
		"\n",

		pi_app->output_file
	);



	fputs("////////////////////////////////////////////////////////////////////////////////\n", pi_app->output_file);


	vs_system_version_info *vsvi = &vs_sys_ver_info;
	vs_application_version_info *vavi = &vs_app_ver_info;


	fputs("//\n",                          pi_app->output_file);
	fputs("// Version\n",                  pi_app->output_file);
	fputs("//\n",                          pi_app->output_file);
	fputs("\n",                            pi_app->output_file);

	fputs("VS_VERSION_INFO VERSIONINFO\n", pi_app->output_file);

	fputs("   FILEVERSION ",               pi_app->output_file);
		fputs(vsvi->file_version.c_str(),  pi_app->output_file);
		fputs("\n",                        pi_app->output_file);

	fputs("   PRODUCTVERSION ",            pi_app->output_file);
		fputs(vavi->prod_version.c_str(),  pi_app->output_file);
		fputs("\n",                        pi_app->output_file);

	fputs("   FILEFLAGSMASK 0x3fL\n",      pi_app->output_file);
	fputs("#ifdef _DEBUG\n",               pi_app->output_file);
	fputs("   FILEFLAGS 0x1L\n",           pi_app->output_file);
	fputs("#else\n",                       pi_app->output_file);
	fputs("   FILEFLAGS 0x0L\n",           pi_app->output_file);
	fputs("#endif\n",                      pi_app->output_file);
	fputs("   FILEOS 0x40004L\n",          pi_app->output_file);
	fputs("   FILETYPE 0x1L\n",            pi_app->output_file);
	fputs("   FILESUBTYPE 0x0L\n",         pi_app->output_file);
	fputs("BEGIN\n",                       pi_app->output_file);
	fputs("   BLOCK \"StringFileInfo\"\n", pi_app->output_file);
	fputs("   BEGIN\n",                    pi_app->output_file);
	fputs("      BLOCK \"080904b0\"\n",    pi_app->output_file);
	fputs("      BEGIN\n",                 pi_app->output_file);

	fputs("         VALUE \"CompanyName\",  \"",     pi_app->output_file);
		fputs(vsvi->company_name.c_str(),            pi_app->output_file);
		fputs("\"\n",                                pi_app->output_file);

	fputs("         VALUE \"FileDescription\",  \"", pi_app->output_file);
		fputs(vavi->description.c_str(),             pi_app->output_file);
		fputs("\"\n",                                pi_app->output_file);

	fputs("         VALUE \"ProductVersion\",   \"", pi_app->output_file);
		std::string prod_version = ReplaceChar(vavi->prod_version, ',', '.');
		fputs(prod_version.c_str(),                  pi_app->output_file);
		fputs("\"\n",                                pi_app->output_file);
	
	fputs("         VALUE \"FileVersion\",      \"", pi_app->output_file);
		std::string file_version = ReplaceChar(vsvi->file_version, ',', '.');
		fputs(file_version.c_str(),                  pi_app->output_file);
		fputs("\"\n",                                pi_app->output_file);

	fputs("         VALUE \"InternalName\",     \"", pi_app->output_file);
		fputs(vavi->internal_name.c_str(),           pi_app->output_file);
		fputs("\"\n",                                pi_app->output_file);

	fputs("         VALUE \"OriginalFilename\", \"", pi_app->output_file);
		fputs(vavi->original_name.c_str(),           pi_app->output_file);
		fputs("\"\n",                                pi_app->output_file);

	fputs("         VALUE \"LegalCopyright\",   \"", pi_app->output_file);
		fputs(vsvi->copyright.c_str(),               pi_app->output_file);
		fputs(vsvi->copyright_year.c_str(),          pi_app->output_file);
		fputs("\"\n",                                pi_app->output_file);

	fputs("         VALUE \"ProductName\",      \"", pi_app->output_file);
		fputs(vavi->product_name.c_str(),            pi_app->output_file);
		fputs("\"\n",                                pi_app->output_file);

	fputs("      END\n",                   pi_app->output_file);
	fputs("   END\n",                      pi_app->output_file);
	fputs("   BLOCK \"VarFileInfo\"\n",    pi_app->output_file);
	fputs("   BEGIN\n",                    pi_app->output_file);
	fputs("      VALUE \"Translation\", 0x809, 1200\n",
		                                   pi_app->output_file);
	fputs("   END\n",                      pi_app->output_file);
	fputs("END\n",                         pi_app->output_file);

	fputs("\n", pi_app->output_file);

	
	// ---- include auto str lambdas
		#include <../vs_07/vs/vs_system/lambda/vs_auto_str.h>

	// ---- flip '/' back to '\'
		CHAR abs_folder_paf[VS_MAXCHARPATHLEN];
		strcpy(abs_folder_paf, pi_app->abs_folder_paf);
		StrRepChar(abs_folder_paf, 50, '/', '\\');


	// ---- icons header
	//
	// NOTE: This translates as "U.K. English, Unicode"
 	// VALUE "Translation", 0x809, 1200
	//
		fputs("////////////////////////////////////////////////////////////////////////////////\n", pi_app->output_file);
		fputs("//\n",       pi_app->output_file);
		fputs("// ICONS\n", pi_app->output_file);
		fputs("//\n",       pi_app->output_file);
		fputs("\n",         pi_app->output_file);


	// ---- icons
	//
	// note: icon with lowest ID value placed first to ensure
	// application icon remains consistent on all systems.
	//
		CHAR txt[128];
		if ( pi_app->abs_folder_mode )
		{
			INT result = sprintf(
					txt,
					"IDI_HICON        ICON        \"%s%s_resources\\\\",
					abs_folder_paf,
					pi_app->abs_folder
				);
				fputs(txt,                     pi_app->output_file);
				fputs("icons_",                pi_app->output_file);
				fputs(pi_app->rc_icon_prefix,  pi_app->output_file);
				fputs("\\\\48.ico\"\n",        pi_app->output_file);
			result = sprintf(
					txt,
					"IDI_HICONSM      ICON        \"%s%s_resources\\\\",
					abs_folder_paf,
					pi_app->abs_folder
				);
				fputs(txt,                     pi_app->output_file);
				fputs("icons_",                pi_app->output_file);
				fputs(pi_app->rc_icon_prefix,  pi_app->output_file);
				fputs("\\\\16.ico\"\n",        pi_app->output_file);
		}
		else
		{
			fputs("IDI_HICON        ICON      \"vs_resources\\\\", pi_app->output_file);
				fputs("icons_",                pi_app->output_file);
				fputs(pi_app->rc_icon_prefix,  pi_app->output_file);
				fputs("\\\\48.ico\"\n",        pi_app->output_file);

			fputs("IDI_HICONSM      ICON      \"vs_resources\\\\", pi_app->output_file);
				fputs("icons_",                pi_app->output_file);
				fputs(pi_app->rc_icon_prefix,  pi_app->output_file);
				fputs("\\\\16.ico\"\n",        pi_app->output_file);
		}
		fputs("\n",                    pi_app->output_file);


	// ---- images header
		fputs("////////////////////////////////////////////////////////////////////////////////\n", pi_app->output_file);
		fputs("//\n",                 pi_app->output_file);
		fputs("// IMAGES\n",          pi_app->output_file);
		fputs("//\n",                 pi_app->output_file);
		fputs("\n",                   pi_app->output_file);


	// ---- texture atlas postfix
		CHAR rc_tap[128];
		if ( pi_app->rc_texture_atlas_postfix == NULL )
		{
			strcpy(rc_tap, "");
		}
		else
		{
			sprintf(rc_tap, "_%s", pi_app->rc_texture_atlas_postfix);
		}


	// ---- images
		for (UINT i = 0; i < 20; i++)
		{

			CHAR pad_i[3] = "00";
			pad_i[1] = '0' + ( i > 9 ? i-10 : i );
			pad_i[0] = i > 9 ? '1' : '0';

			CHAR txt[128];
			if ( pi_app->abs_folder_mode )
			{
				INT result = sprintf(
						txt,
						"IDR_RCDATA%s      RCDATA      \"%s%s_resources\\\\atlas%s\\\\%s.tga\"\n",
						pad_i,
						abs_folder_paf,
						pi_app->abs_folder,
						rc_tap,
						pad_i
					);
			}
			else
			{
				INT result = sprintf(
						txt,
						"IDR_RCDATA%s      RCDATA      \"vs_resources\\\\atlas%s\\\\%s.tga\"\n",
						pad_i,
						rc_tap,
						pad_i
					);
			}
			fputs(txt, pi_app->output_file);
		}
		fputs("\n", pi_app->output_file);


	// ---- end
		fputs("#endif    // English (U.K.) resources\n",         pi_app->output_file);
		fputs("\n",                                              pi_app->output_file);

		fputs("////////////////////////////////////////////////////////////////////////////////\n", pi_app->output_file);

	return VS_OK;
}


// ---------- VsxRc ----------
INT application::VsxRc()
{
	fputs("1 ICON \"vsx_resources", pi_app->output_file);
	fputs("\\\\icons_",             pi_app->output_file);
	fputs(pi_app->rc_icon_prefix,   pi_app->output_file);
	fputs("\\\\48.ico\"\n",         pi_app->output_file);

	return VS_OK;
}
