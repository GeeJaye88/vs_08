////////////////////////////////////////////////////////////////////////////////

// ---------- r2_configfiles.cpp ----------
/*!
\file r2_files.cpp
\brief Implementation of the ConfigFiles class.
\author Gareth Edwards 
*/

// ---- include ----
#include "../../../vs_application/header/vs_app_setup.h"
#include "../header/r2_configfiles.h"


// ---- namespace ----
using namespace review_02;


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
	CHAR   image_config_hdl_filename[VS_MAXCHARPATHLEN];
	CHAR   sensor_config_hdl_filename[VS_MAXCHARPATHLEN];
	CHAR   object_config_hdl_filename[VS_MAXCHARPATHLEN];
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

	// ---- objects ----
	app_dependency = NULL;
	sys_context = NULL;
	sys_output = NULL;
	

	// ---- config path/file names ----
	strcpy(application_config_hdl_filename, "");
	strcpy(image_config_hdl_filename,       "");
	strcpy(sensor_config_hdl_filename,      "");
	strcpy(object_config_hdl_filename,      "");
	strcpy(image_config_jpg_filename,       "");

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
	if (pi_config_files != NULL) { delete pi_config_files; pi_config_files = NULL; }
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT ConfigFiles::Setup(
		vs_system::AppDependency *app_dependency,
		CHAR *camera_app_name,
		UINT  camera_mode
	)
{

	// ==================== LOCAL ====================

	// ---- local
		INT result = 0;
		CHAR msg[VS_MAXCHARPATHLEN];

	// VS stuff
		pi_config_files->app_dependency = app_dependency;
		pi_config_files->sys_context = app_dependency->GetSysContext();
		pi_config_files->sys_output = app_dependency->GetSysOutput();

	// ---- accelerate
		vs_system::SysContext *sys_context = pi_config_files->sys_context;
		vs_system::SysOutput *sys_output = pi_config_files->sys_output;

	// ---- source folder
		CHAR app_path[VS_MAXCHARPATHLEN];
		strcpy(app_path, sys_context->GetChar(sys_context->APP_FOLDER_PATH));

	// ---- destination folder
		CHAR project_path[VS_MAXCHARPATHLEN];
		strcpy(project_path, sys_context->GetChar(sys_context->PROJECT_FOLDER_PATH));

	// ---- destination folder path and target folder
		CHAR *tp = "AppData/Local/Observant Innovations";
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
		CHAR *td = "/Monitor";
		#else
		CHAR *td = "/Patrol";
		#endif

	// ---- config files
		const UINT number_of_files = 5;
		CHAR *config_file[number_of_files] =
		{
			pi_config_files->application_config_hdl_filename,
			pi_config_files->image_config_hdl_filename,
			pi_config_files->sensor_config_hdl_filename,
			pi_config_files->object_config_hdl_filename,
			pi_config_files->image_config_jpg_filename
		};


	// ==================== USER "Documents/Observant" ====================

		CHAR *user_profile;
		size_t user_profile_len;
		_dupenv_s(&user_profile, &user_profile_len, "USERPROFILE");
		//
		// TBD 'userProfile' could be '0':  this does not adhere to the
		// specification for the function 'strcpy_s'. 
		//
		CHAR observant_path[VS_MAXCHARPATHLEN];
		sprintf(
				observant_path, // e.g. "C:\Users\garet"
				"%s/Documents/Observant",
				user_profile
			);

	// ---- TRY observant path
		try
		{
			// IF folder handle INVALID THEN throw
			DWORD ftyp = GetFileAttributesA(observant_path);
			if (ftyp == INVALID_FILE_ATTRIBUTES) throw(observant_path);
			// ELSE folder exists
		}
		// CATCH and create directory
		catch ( char* directory_path )
		{
			// EEXIST - Directory was not created - already exists - should never happen...
			// ENOENT - Path not found - shoukld never happen..
			result = mkdir(directory_path);

			// output Error Message
			if ( result == EEXIST || result == ENOENT )
			{
				sys_output->MessageError(
					"FAILED to make [user]/Documents/Observant directory.",
					"Directory",
					directory_path
					);
				return VS_ERROR;
			}

			// report
			sys_output->Message("Created User Observant Folder:=============================================");

		}


	// ==================== CAMERA ====================


	// ---- camera project folder name
		CHAR cam_proj_folder_name[VS_MAXCHARPATHLEN];
		strcpy(cam_proj_folder_name, sys_context->GetChar(sys_context->CONFIG_NAME));


	// ---- camera data folder name
		CHAR cam_data_folder_name[VS_MAXCHARPATHLEN];
		strcpy(cam_data_folder_name, camera_app_name);


	// ---- sub folder name (typically for the sensor file)
		CHAR *cam_data_sub_folder_name = "";
		CHAR *cam_data_sub_folder_file = "";


#		ifdef VS_CAM_IDS_VIDEO_SDK
		CHAR temp_name[VS_MAXCHARLEN];
		sprintf(temp_name, "/ids_%d", camera_mode);
		cam_data_sub_folder_name = temp_name;
		cam_data_sub_folder_file = "ids_config.hdl";
#		endif

#		ifdef VS_CAM_JPLY_DVP_SDK
		;
#		endif

#		ifdef VS_CAM_PIC_BUFFER
		cam_data_sub_folder_name = "/pic";
		cam_data_sub_folder_file = "pic_config.hdl";
#		endif


		if ( strcmp(cam_data_sub_folder_name, "") == 0 ||
			strcmp(cam_data_sub_folder_file, "") == 0 )
		{
			sys_output->Message("Configuration Files: No pre-processor directive for camera type!\n");
			return VS_ERROR;
		}


	// ==================== INSTALL ====================

	BOOL install_data = sys_context->GetBool(sys_context->INSTALL_DATA);
	BOOL development_mode = sys_context->GetBool(sys_context->DEVELOPMENT);

	// ---- do NOT install data files?
		//
		// note: source local to app!
		//
		if ( install_data == 0 )
		{

			if ( development_mode )
				strcpy(app_path, "../"); 
			else
				strcpy(app_path, "");

			sprintf( config_file[0], "%sdata/%s/app_config.hdl", app_path, camera_app_name);
			sprintf( config_file[1], "%sdata/%s/img_config.hdl", app_path, camera_app_name);
			sprintf( config_file[2], "%sdata/%s%s/%s",           app_path, camera_app_name,
																			cam_data_sub_folder_name,
																			cam_data_sub_folder_file);
			sprintf( config_file[3], "%sdata/%s/obj_config.hdl", app_path, camera_app_name);
			sprintf( config_file[4], "%sdata/%s/img_config.jpg", app_path, camera_app_name);

			for (UINT i = 0; i < number_of_files; i++)
			{
				INT len = strlen(config_file[i]);
				for (INT l = 0; l < len; l++)
					if (config_file[i][l] == '/') config_file[i][l] = '\\';
			}

			return VS_OK;
		}


	// ==================== REPORT ====================


		sys_output->Message("Configuration Files:=======================================================");

		sys_output->Message("Note:");
		sys_output->Message("   At runtime camera applications specify a source application [name] whose");
		sys_output->Message("   configuration files will be copied - if they do not already exist - from");
		sys_output->Message("   a source application \"data/[name]\" folder to a destination");
		sys_output->Message("   \"[User]/Documents/Data/[name]\" folder.");

		sys_output->Message("With:");

		sprintf(msg, "   Source path: %s", app_path);
			sys_output->Message(msg);

		sprintf(msg, "   Destination path: %s%s%s", project_path, tp, td);
			sys_output->Message(msg);


	// ==================== COMPARISON ====================


	// ---- initialise folder component lut
		CHAR *folder_comp_lut[number_of_files][6] =
		{
			{
				"%s/%s%s/%s",
				app_path,
				project_path, //app_path,
				cam_data_folder_name, // cam_proj_folder_name,
				"",
				"app_config.hdl"
			},
			{
				"%s/%s%s/%s",
				app_path,
				project_path,
				cam_data_folder_name,
				"",
				"img_config.hdl"
			},
			{
				"%s/%s%s/%s",
				app_path,
				project_path,
				cam_data_folder_name,
				cam_data_sub_folder_name,
				cam_data_sub_folder_file
			},
			{
				"%s/%s%s/%s",
				app_path,
				project_path,
				cam_data_folder_name,
				"",
				"obj_config.hdl"
			},
			{
				"%s/%s%s/%s",
				app_path,
				project_path,
				cam_data_folder_name,
				"",
				"img_config.jpg"
			},
		};


	// ==================== Part 1: Create Target Project Folders ====================

		CHAR copy_folder[5][VS_MAXCHARPATHLEN];
		sprintf(copy_folder[0], "%s%s", project_path, tp);
		sprintf(copy_folder[1], "%s%s%s", project_path, tp, td);
		sprintf(copy_folder[2], "%s%s%s/data", project_path, tp, td);
		sprintf(copy_folder[3], "%s%s%s/data/%s", project_path, tp, td, cam_data_folder_name);
		sprintf(copy_folder[4], "%s%s%s/data/%s%s", project_path, tp, td, cam_data_folder_name, cam_data_sub_folder_name);

		// ---- for each directory path
			for ( UINT i=0; i<5; i++ )
			{
				// TRY directory path
				try
				{
					// IF folder handle INVALID THEN throw
					DWORD ftyp = GetFileAttributesA(copy_folder[i]);
					if (ftyp == INVALID_FILE_ATTRIBUTES) throw(copy_folder[i]);
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
								"FAILED to make project directory.",
								"Directory",
								directory_path
							);
						return VS_ERROR;
					}
				}
			}


	// --- source: app_path -> "data" folder
		strcat(app_path, "data");


	// ---- target: project_path -> Documents" folder -> "Observant" folder -> "Data" folder
		strcpy(project_path, copy_folder[2]);


	// ==================== Part 2: Files ====================

	//
	// Application folder - app_path -> "data" folder
	// User folder - project_path -> Documents" folder -> "Observant" folder -> "Data" folder
	//

	// ---- for each file
		CHAR source[VS_MAXCHARPATHLEN];
		CHAR destination[VS_MAXCHARPATHLEN];
		for ( UINT i=0; i<number_of_files; i++ )
		{

			// ---- source
				sprintf(
						source,
						folder_comp_lut[i][0],   // format
						folder_comp_lut[i][1],   // path
						folder_comp_lut[i][3],   // source folder
						folder_comp_lut[i][4],   // sub-folder
						folder_comp_lut[i][5]    // filename
					);

			// ---- target
				sprintf(
						destination,
						folder_comp_lut[i][0],   // format
						folder_comp_lut[i][2],   // path
						folder_comp_lut[i][3],   // target folder
						folder_comp_lut[i][4],   // sub-folder
						folder_comp_lut[i][5]    // filename
					);


			// ---- local
				BOOL file_already_exists = TRUE;
				INT len = 0;


			// ---- MUST DO: replace '/' with '\'
				len = strlen(source);
				for ( INT l = 0; l < len; l++ ) if ( source[l] == '/' ) source[l] = '\\';
				len = strlen(destination);
				for ( INT l = 0; l < len; l++ ) if ( destination[l] == '/' ) destination[l] = '\\';


			// ---- source and target path/filename are the same ?
				if ( !strcmp(source, destination) )
				{
					if ( GetFileAttributes(source) == INVALID_FILE_ATTRIBUTES )
					{
						sys_output->MessageError(
								"Source file does not exist.",
								"File",
								source
							);
						return VS_ERROR;
					}
					strcpy(config_file[i], source);
				}

			// ---- else copy
				else
				{
					if ( GetFileAttributes(destination) == INVALID_FILE_ATTRIBUTES )
					{
						BOOL copyied =  CopyFile(
								source,
								destination,
								TRUE
							);
						if ( !copyied )
						{
							sys_output->MessageError(
									"FAILED to copy source file to Data directory.",
									"File",
									source
								);
							return VS_ERROR;
						}
						file_already_exists = FALSE;
					}

					// ---- destination is THE source
						strcpy(config_file[i], destination);

				}

			// ---- report
				if (i == 0)
				{
					sys_output->Message("Use or copy configuration files:");
				}

				if ( strcmp(source, destination) )
				{
					CHAR *file_already_copied = file_already_exists ? " - already been copied!" : " - copied.";

					if (i == 2)
					{
						sprintf(
								msg,
								"   Data/%s/%s/%s/%s %s",
								folder_comp_lut[i][3],
								folder_comp_lut[i][4],
								cam_data_sub_folder_name,
								cam_data_sub_folder_file,
								file_already_copied
							);
						sys_output->Message(msg);
					}
					else
					{
						sprintf(
								msg,
								"   Data/%s/%s/%s %s",
								folder_comp_lut[i][3],
								folder_comp_lut[i][4],
								folder_comp_lut[i][5],
								file_already_copied
							);
						sys_output->Message(msg);
					}
				}


			// ---- tidy
				//
				// note: replace '\' with '/'
				//
				len = strlen(source);
				for ( INT l = 0; l < len; l++ ) if ( source[l] == '\\' ) source[l] = '/';
				len = strlen(destination);
				for ( INT l = 0; l < len; l++ ) if ( destination[l] == '\\' ) destination[l] = '/';
		}


	// ---- bookend
		sys_output->Message("End:-----------------------------------------------------------------------");

	return VS_OK;
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


// ---------- GetImageConfigHdlFilename ----------
/*!
\brief Get camera HDL config filename
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ConfigFiles::GetImageConfigHdlFilename(CHAR *filename)
{
	strcpy(filename, pi_config_files->image_config_hdl_filename);
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


// ---------- GetObjectConfigHdlFilename ----------
/*!
\brief Get object HDL config filename
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ConfigFiles::GetObjectConfigHdlFilename(CHAR *filename)
{
	strcpy(filename, pi_config_files->object_config_hdl_filename);
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
