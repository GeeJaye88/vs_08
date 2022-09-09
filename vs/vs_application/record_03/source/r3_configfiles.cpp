////////////////////////////////////////////////////////////////////////////////

// ---------- r3_configfiles.cpp ----------
/*!
\file r3_configfiles.cpp
\brief Implementation of the ConfigFiles class.
\author Gareth Edwards 
\note Vanilla C++
*/

// ---------- include ----------
#include "../header/r3_configfiles.h"

// ---------- namespace ----------
using namespace record_03;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_ConfigFiles class ----------

class ConfigFiles::PI_ConfigFiles
{

public:

	// Constructor/destructor
	PI_ConfigFiles();
	~PI_ConfigFiles();

	// Methods
	INT Initialise();

	// Objects
	vs_system::AppDependency *app_dependency;
	vs_system::SysContext *sys_context;
	vs_system::SysOutput *sys_output;

	// Files
	CHAR   application_config_hdl_filename[VS_MAXCHARPATHLEN];
	CHAR   camera_config_hdl_filename[VS_MAXCHARPATHLEN];
	CHAR   sensor_config_hdl_filename[VS_MAXCHARPATHLEN];
	CHAR   panoramic_config_hdl_filename[VS_MAXCHARPATHLEN];
	CHAR   image_config_jpg_filename[VS_MAXCHARPATHLEN];

};


// ========== PI_Files ==========


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
ConfigFiles::PI_ConfigFiles::PI_ConfigFiles()
{
	INT result = Initialise();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
ConfigFiles::PI_ConfigFiles::~PI_ConfigFiles()
{
	;
}


// ---------- Initialise ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
INT ConfigFiles::PI_ConfigFiles::Initialise()
{
	// Objects
	app_dependency = NULL;
	sys_context = NULL;
	sys_output = NULL;
	
	// Config path/file names
	strcpy(application_config_hdl_filename, "");
	strcpy(camera_config_hdl_filename, "");
	strcpy(sensor_config_hdl_filename, "");
	strcpy(panoramic_config_hdl_filename, "");
	strcpy(image_config_jpg_filename, "");

	// Done
	return VS_OK;
}

////////////////////////////////////////////////////////////////////////////////


// ========== ConfigFiles ==========


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
ConfigFiles::ConfigFiles()
{
	pi_config_files = new PI_ConfigFiles();

}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
ConfigFiles::~ConfigFiles()
{
	delete pi_config_files;
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT ConfigFiles::Setup(vs_system::AppDependency *app_dependency, CHAR *camera_app_name)
{
	// Local
	INT result = 0;
	CHAR msg[VS_MAXCHARPATHLEN];

	// Store
	pi_config_files->app_dependency = app_dependency;
	pi_config_files->sys_context = app_dependency->GetSysContext();
	pi_config_files->sys_output = app_dependency->GetSysOutput();

	// Shortcut
	vs_system::SysContext *sys_context = pi_config_files->sys_context;
	vs_system::SysOutput *sys_output = pi_config_files->sys_output;

	// Bookend
	sys_output->Message("Configuration Files:=======================================================");

	sys_output->Message("Note:");
	sys_output->Message("   At runtime Patrol applications specify a source application [name] whose");
	sys_output->Message("   configuration files will be copied - if they do not already exist - from");
	sys_output->Message("   a source application \"data/[name]\" folder to a destination");
	sys_output->Message("   \"[User]/Documents/Data/[name]\" folder.");

	// Target destination folder path and target folder
	CHAR *tp = "AppData/Local/Observant Innovations";
	CHAR *td = "/Patrol";

	// Paths
	CHAR app_path[VS_MAXCHARPATHLEN];
	strcpy(app_path, sys_context->GetChar(sys_context->APP_FOLDER_PATH));
	CHAR project_path[VS_MAXCHARPATHLEN];
	strcpy(project_path, sys_context->GetChar(sys_context->PROJECT_FOLDER_PATH));
	sys_output->Message("With:");
	sprintf(msg, "   Source path: %s", app_path);
	sys_output->Message(msg);
	sprintf(msg, "   Destination path: %s%s%s", project_path, tp, td);
	sys_output->Message(msg);

	// Camera application source folder name
	CHAR folder_name[VS_MAXCHARPATHLEN];
	//strcpy(folder_name, sys_context->GetChar(sys_context->APP_NAME));
	strcpy(folder_name, camera_app_name);

	// Sub folder name (typically for the sensor file)
	CHAR *sub_folder_name = "";
	CHAR *sub_folder_file = "";

#	ifdef VS_CAM_IDS_VIDEO_SDK
	sub_folder_name = "/ids";
	sub_folder_file = "ids_config.hdl";
#	endif

#	ifdef VS_CAM_JPLY_DVP_SDK
	sub_folder_name = "/jply";
	sub_folder_file = "jply_config.hdl";
#	endif

#	ifdef VS_CAM_PIC_BUFFER
	sub_folder_name = "/jply";
	sub_folder_file = "jply_config.hdl";
#	endif


	// ----- Part 1: Create Project Folders -----

	/*
	CHAR pp[5][VS_MAXCHARPATHLEN];
	sprintf(pp[0], "%s/Documents", project_path);
	sprintf(pp[1], "%s/Documents/Observant", project_path);
	sprintf(pp[2], "%s/Documents/Observant/Data", project_path);
	sprintf(pp[3], "%s/Documents/Observant/Data/%s", project_path, folder_name);
	sprintf(pp[4], "%s/Documents/Observant/Data/%s%s", project_path, folder_name, sub_folder_name);
	*/

	CHAR pp[5][VS_MAXCHARPATHLEN];
	sprintf(pp[0], "%s%s",             project_path, tp);
	sprintf(pp[1], "%s%s%s",           project_path, tp, td);
	sprintf(pp[2], "%s%s%s/data",      project_path, tp, td);
	sprintf(pp[3], "%s%s%s/data/%s",   project_path, tp, td, folder_name);
	sprintf(pp[4], "%s%s%s/data/%s%s", project_path, tp, td, folder_name, sub_folder_name);

	// For each directory path
	for ( UINT i=0; i<5; i++ )
	{
		// TRY directory path
		try
		{
			// IF folder handle INVALID THEN throw
			DWORD ftyp = GetFileAttributesA(pp[i]);
			if (ftyp == INVALID_FILE_ATTRIBUTES) throw(pp[i]);
			// ELSE folder exists
		}
		// Catch and create directory
		catch ( char* directory_path )
		{
			// EEXIST - Directory was not created - already exists
			// ENOENT - Path not found
			result = mkdir(directory_path);

			// Output Error Message
			if (result == EEXIST || result == ENOENT)
			{
				sys_output->MessageError(
						"FAILED to successfully make project directory.",
						"Directory",
						directory_path
					);
				return VS_ERROR;
			}
		}
	}

	// Source: app_path -> "data" folder
	strcat(app_path, "data");

	// Target: project_path -> Documents" folder -> "Observant" folder -> "Data" folder
	strcpy(project_path, pp[2]);


	// ----- Part 2: Files -----

	//
	// Application folder - app_path -> "data" folder
	// User folder - project_path -> Documents" folder -> "Observant" folder -> "Data" folder
	//

	// Local const
	const UINT number_of_files = 5;

	// Initialise lut
	CHAR *text[number_of_files][6] =
	{
		{ "%s/%s%s/%s",  app_path,  app_path,      folder_name,  "",              "app_config.hdl"  },
		{ "%s/%s%s/%s",  app_path,  project_path,  folder_name,  "",              "cam_config.hdl"  },
		{ "%s/%s%s/%s",  app_path,  project_path,  folder_name,  sub_folder_name,  sub_folder_file  },
		{ "%s/%s%s/%s",  app_path,  project_path,  folder_name,  "",              "pan_config.hdl"  },
		{ "%s/%s%s/%s",  app_path,  project_path,  folder_name,  "",              "img_config.jpg"  },
	};

	// Initialise path/filename pointers
	CHAR *target[number_of_files] =
	{
		pi_config_files->application_config_hdl_filename,
		pi_config_files->camera_config_hdl_filename,
		pi_config_files->sensor_config_hdl_filename,
		pi_config_files->panoramic_config_hdl_filename,
		pi_config_files->image_config_jpg_filename
	};

	// // For each file
	CHAR source[VS_MAXCHARPATHLEN];
	CHAR destination[VS_MAXCHARPATHLEN];
	for ( UINT i=0; i<number_of_files; i++ )
	{
		// Source
		sprintf(
				source,
				text[i][0],		// format
				text[i][1],		// path
				text[i][3],     // source folder
				text[i][4],     // sub-folder
				text[i][5]      // filename
			);

		// Target
		sprintf(
				destination,
				text[i][0],		// format
				text[i][2],		// path
				text[i][3],     // target folder
				text[i][4],     // sub-folder
				text[i][5]      // filename
			);

		// File status
		BOOL file_already_exists = TRUE;

		// Local
		INT len = 0;

		// MUST DO: replace '/' with '\'
		len = strlen(source);
		for ( INT l = 0; l < len; l++ ) if ( source[l] == '/' ) source[l] = '\\';
		len = strlen(destination);
		for ( INT l = 0; l < len; l++ ) if ( destination[l] == '/' ) destination[l] = '\\';

		// IF source and target path/filename are the same THEN don't copy source
		if ( !strcmp(source, destination) )
		{
			// IF source file does NOT exist THEN
			if ( GetFileAttributes(source) == INVALID_FILE_ATTRIBUTES )
			{
				sys_output->MessageError(
						"Source file does not exist.",
						"File",
						source
					);
				return VS_ERROR;
			}

			// Store
			strcpy(target[i], source);
		}
		else
		{
			// IF destination file does NOT exist THEN
			if ( GetFileAttributes(destination) == INVALID_FILE_ATTRIBUTES )
			{
				// Copy
				BOOL copyied =  CopyFile(
						source,
						destination,
						TRUE
					);

				// IF NOT copied THEN report && return error
				if ( !copyied )
				{
					sys_output->MessageError(
							"FAILED to successfully copy source file to Data directory.",
							"File",
							source
						);
					return VS_ERROR;
				}

				// File exists OK
				file_already_exists = FALSE;
			}

			// Destination is THE source
			strcpy(target[i], destination);
		}

		// Report
		if (i == 0)
		{
			sys_output->Message("Use or copy configuration files:");
		}
		if ( strcmp(source, destination) )
		{
			CHAR *file_already_copied = file_already_exists ? " - already been copied!" : " - copied.";

			if (i == 2)
			{
				sprintf(msg, "   Data/%s/%s/%s/%s %s", text[i][3], text[i][4], sub_folder_name, sub_folder_file, file_already_copied);
				sys_output->Message(msg);
			}
			else
			{
				sprintf(msg, "   Data/%s/%s/%s %s", text[i][3], text[i][4], text[i][5], file_already_copied);
				sys_output->Message(msg);
			}
		}

		// Tidy: Replace '\' with '/'
		len = strlen(source);
		for ( INT l = 0; l < len; l++ ) if ( source[l] == '\\' ) source[l] = '/';
		len = strlen(destination);
		for ( INT l = 0; l < len; l++ ) if ( destination[l] == '\\' ) destination[l] = '/';
	}

	// Bookend
	sys_output->Message("End:-----------------------------------------------------------------------");

	// Done
	return VS_OK;
}


// ---------- GetCameraApplicationName ----------
/*!
\brief Get camera application name
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ConfigFiles::GetCameraApplicationName(CHAR *name)
{
	if ( name != NULL )
	{
		strcpy(name, "camera_03");
		return VS_OK;
	}
	return VS_ERROR;
}


// ---------- GetCameraDevelopmentName ----------
/*!
\brief Get camera application development name
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ConfigFiles::GetCameraDevelopmentName(CHAR *name)
{
	if ( name != NULL )
	{
		strcpy(name, "camera_03");
		return VS_OK;
	}
	return VS_ERROR;
}


// ---------- GetReviewApplicationName ----------
/*!
\brief Get camera application development name
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ConfigFiles::GetReviewApplicationName(CHAR *name)
{
	if (name != NULL)
	{
		strcpy( name, "review_02" );
		return VS_OK;
	}
	return VS_ERROR;
}


// ---------- GetApplicationConfigHdlFilename ----------
/*!
\brief Get application HDL config filename
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ConfigFiles::GetApplicationConfigHdlFilename(CHAR *filename)
{
	strcpy(filename, pi_config_files->application_config_hdl_filename);
	return VS_OK;
}


// ---------- GetCameraConfigHdlFilename ----------
/*!
\brief Get camera HDL config filename
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ConfigFiles::GetCameraConfigHdlFilename(CHAR *filename)
{
	strcpy(filename, pi_config_files->camera_config_hdl_filename);
	return VS_OK;
}


// ---------- GetPropsConfigHdlFilename ----------
/*!
\brief Get sensor (e.g. "jply") HDL config filename
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ConfigFiles::GetPropsConfigHdlFilename(CHAR *filename)
{
	strcpy(filename, pi_config_files->sensor_config_hdl_filename);
	return VS_OK;
}


// ---------- GetPanoramicConfigHdlFilename ----------
/*!
\brief Get panoramic HDL config filename
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ConfigFiles::GetPanoramicConfigHdlFilename(CHAR *filename)
{
	strcpy(filename, pi_config_files->panoramic_config_hdl_filename);
	return VS_OK;
}


// ---------- GetImageConfigJpgFilename ----------
/*!
\brief Get configuration test image filename
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ConfigFiles::GetImageConfigJpgFilename(CHAR *filename)
{
	strcpy(filename, pi_config_files->image_config_jpg_filename);
	return VS_OK;
}



////////////////////////////////////////////////////////////////////////////////
