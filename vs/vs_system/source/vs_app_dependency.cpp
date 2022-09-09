////////////////////////////////////////////////////////////////////////////////

// ---------- vs_app_dependency.cpp ----------
/*!
\file vs_app_dependency.cpp
\brief Implementation of the App(lication)Dependency class
\author Gareth Edwards
*/

// ---- include ----
#include "../../vs_application/header/vs_app_setup.h"
#include "../header/vs_app_dependency.h"


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_AppDependency class ----------

class AppDependency::PI_AppDependency
{

public:

	PI_AppDependency()
	{

		strcpy(app_name, "");
		app_config_data  = NULL;
		sys_config_data  = NULL;
		view_config_data = NULL;

#		ifdef VS_GFX_FLAG
		gfx_interface    = new GfxInterface();
		sys_input        = new SysInput();
#		endif

		sys_context      = new SysContext();
		sys_output       = new SysOutput();

	};

	~PI_AppDependency() { };
	CHAR          app_name[VS_MAXCHARNAME];
	VNode        *app_config_data;
	VNode        *sys_config_data;
	VNode        *view_config_data;

#	ifdef VS_GFX_FLAG
	GfxInterface *gfx_interface;
	SysInput     *sys_input;
#	endif

	SysContext   *sys_context;
	SysOutput    *sys_output;

	INT   argc;
	CHAR *argv[VS_MAXARGV];
};


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
AppDependency::AppDependency()
{
	pi_app_dependency = new PI_AppDependency;
}


// ---------- Destructor ----------
AppDependency::~AppDependency()
{
	if ( pi_app_dependency->app_config_data  ) pi_app_dependency->app_config_data->Delete();
	if ( pi_app_dependency->sys_config_data  ) pi_app_dependency->sys_config_data->Delete();
	if ( pi_app_dependency->view_config_data ) pi_app_dependency->view_config_data->Delete();


#	ifdef VS_GFX_FLAG
	if ( pi_app_dependency->gfx_interface != NULL)
	{
		delete pi_app_dependency->gfx_interface;
		pi_app_dependency->gfx_interface = NULL;
	}
	if ( pi_app_dependency->sys_input != NULL)
	{
		delete pi_app_dependency->sys_input;
		pi_app_dependency->sys_input = NULL;
	}
#	endif


	if (pi_app_dependency->sys_context != NULL)
	{
		delete pi_app_dependency->sys_context;
		pi_app_dependency->sys_context = NULL;
	}

	if (pi_app_dependency->sys_output != NULL)
	{
		delete pi_app_dependency->sys_output;
		pi_app_dependency->sys_output = NULL;
	}

	if (pi_app_dependency != NULL)
	{
		delete pi_app_dependency;
		pi_app_dependency = NULL;
	}

}


// ---------- GetAppConfigData ----------
/*!
\brief Return pointer to configuration Hieratchical Data List (HDL)
\author Gareth Edwards
\return VNode * - ERROR return NULL
*/

// --- NOT USED

VNode *AppDependency::GetAppConfigData()
{
	if ( pi_app_dependency->app_config_data )
		return pi_app_dependency->app_config_data;
	return NULL;
}


// ---------- GetAppName ----------
/*!
\brief Return application name
\author Gareth Edwards
\param CHAR * - application name
\return INT - ERROR <= 0 < VS_OK

\note invoked in:

review_02::Model::SetupPanels
review_02::Model::SetupButtons

// ---- used to create image path/filename ----
	vs_system::CamPic::Setup(vs_system::AppDependency *app_dep)

*/
INT AppDependency::GetAppName(CHAR *name)
{
	if ( pi_app_dependency->app_name[0] != 0 )
	{
		strcpy_s(name, VS_MAXCHARNAME, pi_app_dependency->app_name);
		return VS_OK;
	}
	return VS_ERROR;
}


// ---------- GetArgc ----------
/*!
\brief Return argument count
\author Gareth Edwards
\return CHAR * - pointer to argument value
*/
INT AppDependency::GetArgc()
{
	if ( pi_app_dependency->argc <= VS_MAXARGV )
	{
		return pi_app_dependency->argc;
	}
	return VS_ERROR;
}


// ---------- GetArgv ----------
/*!
\brief Return argument value
\author Gareth Edwards
\return CHAR * - pointer to argument value
*/
CHAR* AppDependency::GetArgv(UINT index)
{
	if ( index < VS_MAXARGV )
	{
		return pi_app_dependency->argv[index];
	}
	return VS_ERROR;
}


// ---------- GetSysConfigData ----------
/*!
\brief Return pointer to configuration Hieratchical Data List (HDL)
\author Gareth Edwards
\return VNode * - ERROR return NULL
*/
VNode *AppDependency::GetSysConfigData()
{
	if ( pi_app_dependency->sys_config_data )
		return pi_app_dependency->sys_config_data;
	return NULL;
}


// ---------- GetViewConfigData ----------
/*!
\brief Return pointer to parameter Hieratchical Data List (HDL)
\author Gareth Edwards
\return VNode * - ERROR return NULL
*/
VNode *AppDependency::GetViewConfigData()
{
	if ( pi_app_dependency->view_config_data )
		return pi_app_dependency->view_config_data;
	return NULL;
}


// ---------- GetGfxInterface ----------
/*!
\brief Return pointer to graphics interface
\author Gareth Edwards
\return GfxInterface * - graphics interface
*/
#ifdef VS_GFX_FLAG
GfxInterface *AppDependency::GetGfxInterface()
{
	return pi_app_dependency->gfx_interface;
}
#endif


// ---------- GetSysContext ----------
/*!
\brief Return pointer to system context
\author Gareth Edwards
\return GfxInterface * - graphics interface
*/
SysContext *AppDependency::GetSysContext()
{
	return pi_app_dependency->sys_context;
}


// ---------- GetSysInput ----------
/*!
\brief Return pointer to system input
\author Gareth Edwards
\return SysInput * - system input
*/
#ifdef VS_GFX_FLAG
SysInput *AppDependency::GetSysInput()
{
	return pi_app_dependency->sys_input;
}
#endif


// ---------- GetSysOutput ----------
/*!
\brief Return pointer to system report
\author Gareth Edwards
\return SysInput * - system input
*/
SysOutput *AppDependency::GetSysOutput()
{
	return pi_app_dependency->sys_output;
}


// ---------- ReadConfigFiles ----------
/*!
\brief Read configuration file
\author Gareth Edwards
\param CHAR * - application name
\return INT - ERROR <= 0 < VS_OK 
*/
int AppDependency::ReadConfigFiles (CHAR *application_name)
{

	// ---- local ----
	VNodeIO vio;
	CharProc cp;
	CHAR  current_directory_path[VS_MAXCHARPATHLEN];


	// ---- set config application name ----
	SysContext *sys_context = pi_app_dependency->sys_context;
	CHAR *app_name = sys_context->GetChar(SysContext::APP_NAME);
	CHAR *config_name = sys_context->GetChar(SysContext::CONFIG_NAME);
	if ( strcmp(app_name, config_name) == 0 )
		strcpy(pi_app_dependency->app_name, app_name);
	else
		strcpy(pi_app_dependency->app_name, config_name);


	// ---- get current directory path ----
	// TBD: ifdef OS_WINDOWS
	char *cwd = getcwd( current_directory_path, VS_MAXCHARPATHLEN);


	// ---- local - Path ----
	char *app_folder_path     = sys_context->GetChar(sys_context->APP_FOLDER_PATH);
	char *project_folder_path = sys_context->GetChar(sys_context->PROJECT_FOLDER_PATH);


	// ---- local - File ----
	char *app_config_file  = sys_context->GetChar(sys_context->APP_CONFIG_FILE);
	char *sys_config_file  = sys_context->GetChar(sys_context->SYS_CONFIG_FILE);
	char *view_config_file = sys_context->GetChar(sys_context->VIEW_CONFIG_FILE);


	// ---- get development environment (set in WinMain TBD) ----
	char *dev_env = sys_context->GetChar(vs_system::SysContext::DEV_ENV);


	// ---- verify app mode, data folder, and update current_directory_path ----
	#include "../lambda/vs_auto_folder.h"
	#include "../lambda/vs_auto_str.h"
	#include "../lambda/vs_auto_cdp.h"
	INT result = StrPrepCdp(current_directory_path);


	// ---- create application path ----
	sprintf_s(app_folder_path, VS_MAXCHARPATHLEN, "%s", current_directory_path);
	cp.Substitute('\\', '/', app_folder_path);
	strcat_s(app_folder_path, VS_MAXCHARPATHLEN, "/");


	// ---- get user directory path ----
#	ifdef OS_WINDOWS
	CHAR *userProfile;

	// 64 BIT CHANGE from unsigned int to size_t
	size_t userProfileLen;

	_dupenv_s( &userProfile,&userProfileLen,"USERPROFILE");
	//
	// TBD 'userProfile' could be '0':  this does not adhere to the
	// specification for the function 'strcpy_s'. 
	//
	// 19/01/22 - NPL Richard Dudley
	// Hi Gareth, camera up and running at our site, is it possible to change the
	// save location to a different drive other than c: ?
	//
	strcpy_s(project_folder_path, VS_MAXCHARPATHLEN, userProfile);
#	else OTHER OS
	strcpy_s( context->project_folder_path, VS_MAXCHARPATHLEN, context->app_folder_path);
#	endif OS_WINDOWS


	// ---- create application project path ----
	cp.Substitute('\\', '/', project_folder_path);
	strcat_s(project_folder_path, VS_MAXCHARPATHLEN, "/");


	// IF -- application configuration file found -- THEN -- read -- ELSE -- set NULL
	sprintf_s(
			app_config_file,
			VS_MAXCHARPATHLEN,
			"%sdata/%s/app_config.hdl",
			app_folder_path,
			pi_app_dependency->app_name
		); // TODO : error check
	if ( ! (GetFileAttributes(app_config_file) != INVALID_FILE_ATTRIBUTES) )
	{
		pi_app_dependency->app_config_data = NULL;
	}
	else
	{
		pi_app_dependency->app_config_data = vio.Read(app_config_file);
		if ((INT)pi_app_dependency->app_config_data <= VS_ERROR)
			pi_app_dependency->app_config_data = NULL;
	}


	// IF -- system configuration file found -- THEN read -- ELSE -- set NULL
	sprintf_s(
			sys_config_file,
			VS_MAXCHARPATHLEN,
			"%sdata/%s/sys_config.hdl",
			app_folder_path,
			pi_app_dependency->app_name
		); // TODO : error check
	if ( ! (GetFileAttributes(sys_config_file) != INVALID_FILE_ATTRIBUTES) )
	{
		pi_app_dependency->sys_config_data = NULL;
	}
	else
	{
		pi_app_dependency->sys_config_data = vio.Read(sys_config_file);
		if ((INT)pi_app_dependency->sys_config_data <= VS_ERROR)
			pi_app_dependency->sys_config_data = NULL;
	}


	// IF -- view configuration file found -- THEN -- read -- ELSE -- set NULL
	sprintf_s(
			view_config_file,
			VS_MAXCHARPATHLEN,
			"%sdata/%s/view_config.hdl",
			app_folder_path,
			pi_app_dependency->app_name
		); // TODO : error check
	if ( ! (GetFileAttributes(view_config_file) != INVALID_FILE_ATTRIBUTES) )
	{
		pi_app_dependency->view_config_data = NULL;
	}
	else
	{
		pi_app_dependency->view_config_data = vio.Read(view_config_file);
		if ((INT)pi_app_dependency->sys_config_data <= VS_ERROR)
			pi_app_dependency->sys_config_data = NULL;
	}


	// IF -- all config files have NOT been found -- THEN -- return error
	if (
			pi_app_dependency->app_config_data  == NULL ||
			pi_app_dependency->sys_config_data  == NULL ||
			pi_app_dependency->view_config_data == NULL
		)
	{
		return VS_ERROR;
	}

	return VS_OK;
}


// ---------- ReportConfig ----------
/*!
\brief Read configuration
\author Gareth Edwards
\param CHAR * - application name
\return INT - ERROR <= 0 < VS_OK
\note 2017
*/
int AppDependency::ReportConfig()
{
	// ---- local - acceleration ----
	SysContext *sys_context   = pi_app_dependency->sys_context;
	SysOutput  *sys_output    = pi_app_dependency->sys_output;
	

	// ---- local - paths ----
	char *app_folder_path     = sys_context->GetChar(sys_context->APP_FOLDER_PATH);
	char *project_folder_path = sys_context->GetChar(sys_context->PROJECT_FOLDER_PATH);


	// ---- local - report ----
	CHAR report[VS_MAXCHARLEN];


	// ---- banner ----
	sys_output->Message("*****************************************************************************");
	sys_output->Message("*                                                                           *");
	sys_output->Message("*                       Observant Innovations Limited                       *");
	sys_output->Message("*                                                                           *");
	sys_output->Message("*****************************************************************************");


	// ---- system ----
	sys_output->Message("System:");
	sprintf_s(
			report,
			VS_MAXCHARLEN,
			"\tVersion is %d.%d.%d.%d",
			sys_context->GetDWord(sys_context->SYS_VERSION_MAJOR),
			sys_context->GetDWord(sys_context->SYS_VERSION_MINOR),
			sys_context->GetDWord(sys_context->SYS_VERSION_PATCH),
			sys_context->GetDWord(sys_context->SYS_VERSION_BUILD)
		);
	sys_output->Message(report);
	sprintf_s(
			report,
			VS_MAXCHARLEN,
			"\tCreated at %s on %s",
			__TIME__,
			__DATE__
		);
	sys_output->Message(report);


	// ---- application ----
	sys_output->Message("Application:");
	CHAR *app_name = pi_app_dependency->app_name;
	sprintf_s(
			report,
			VS_MAXCHARLEN,
			"\tName is %s",
			app_name != NULL ? app_name : "NOT initialised!"
		);
	sys_output->Message(report);
	sprintf_s(
			report,
			VS_MAXCHARLEN,
			"\tVersion is %d.%d.%d.%d",
			sys_context->GetDWord(sys_context->APP_VERSION_MAJOR),
			sys_context->GetDWord(sys_context->APP_VERSION_MINOR),
			sys_context->GetDWord(sys_context->APP_VERSION_PATCH),
			sys_context->GetDWord(sys_context->APP_VERSION_BUILD)
		);
	sys_output->Message(report);


	// ---- folders ----
	sys_output->Message("Folder paths:");
	sprintf_s(
			report,
			VS_MAXCHARLEN,
			"\tApplication is %s",
			app_folder_path != NULL ? app_folder_path : "NOT initialised!"
		);
	sys_output->Message(report);
	sprintf_s(
			report,
			VS_MAXCHARLEN,
			"\tProject is %s",
			app_folder_path != NULL ? project_folder_path : "NOT initialised!"
		);
	sys_output->Message(report);


	// ---- install folders ----
	sys_output->Message("Application configuration files:");
	sprintf_s(
			report,
			VS_MAXCHARLEN,
			"\tThese have %s into [User]/[Config Location].",
			sys_context->GetBool(SysContext::INSTALL_DATA) ?
				"BEEN INSTALLED" : "+++ NOT BEEN INSTALLED! +++"
		);
	sys_output->Message(report);
	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
	sys_output->Message("\tNote: Config location is \"/AppData/Local/Observant Innovations/Monitor\"");
	#else
	sys_output->Message("\tNote: Config location is \"/AppData/Local/Observant Innovations/Patrol\"");
	#endif

	// ---- config ----
	sys_output->Message("Configuration files:");
	sprintf_s(
			report,
			VS_MAXCHARLEN,
			"\tApplication is %s",
			pi_app_dependency->app_config_data != NULL ? "found and parsed" : "NOT found!"
		);
	sys_output->Message(report);
	sprintf_s(
			report,
			VS_MAXCHARLEN,
			"\tSystem is %s",
			pi_app_dependency->sys_config_data != NULL ? "found and parsed" : "NOT found!"
		);
	sys_output->Message(report);
	sprintf_s(
			report,
			VS_MAXCHARLEN,
			"\tView is %s",
			pi_app_dependency->view_config_data != NULL ? "found and parsed" : "NOT found!"
		);
	sys_output->Message(report);


	// ---- ok ----
	return VS_OK;
}


// ---------- SetArgc ----------
/*!
\brief Set argument count
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT AppDependency::SetArgc(UINT argc)
{
	if ( argc <= VS_MAXARGV )
	{
		pi_app_dependency->argc = argc;
		return VS_OK;
	}
	return VS_ERROR;
}


// ---------- SetArgv ----------
/*!
\brief Set argument value
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT AppDependency::SetArgv(UINT index, CHAR *argv)
{
	if ( index < VS_MAXARGV )
	{
		pi_app_dependency->argv[index] = argv;
		return VS_OK;
	}
	return VS_ERROR;
}
