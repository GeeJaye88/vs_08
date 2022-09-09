////////////////////////////////////////////////////////////////////////////////

// ---------- s1_config_files.cpp ----------
/*!
\file s1_config_files.cpp
\brief Implementation of the ConfigFiles class.
\author Gareth Edwards 
*/

// ---- include ----
#include "../header/s1_config_files.h"


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_ConfigFiles class ----------

class ConfigFiles::PI_ConfigFiles
{

public:

	// ---- ctor
		PI_ConfigFiles();
		~PI_ConfigFiles();


	// ---- methods
		INT Initialise();
		INT CreateDirectory(CHAR *);
		INT CreateDocumentsDirectory(CHAR *);
		INT CreateAppDataDirectory(CHAR *, CHAR *);

		INT InitLocalCopyPathFilename(CHAR *);
		INT CopyLocalFilesToDest();

		INT InitLocalDestPathFilename(CHAR *);

	// ---- objects
		vs_system::AppDependency *app_dependency;
		vs_system::SysContext    *sys_context;
		vs_system::SysOutput     *sys_output;


	// ---- #
		static const UINT _number_of_files = 5;


	// ---- path filenames
		CHAR _dest_path_filename[_number_of_files][VS_MAXCHARPATHLEN];
		CHAR _copy_path_filename[_number_of_files][VS_MAXCHARPATHLEN];


	// ---- source
		CHAR _src_directory[_number_of_files][VS_MAXCHARNAME];
		CHAR _src_filename[_number_of_files][VS_MAXCHARNAME];

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
INT ConfigFiles::PI_ConfigFiles::Initialise()
{

	// ---- objects
		app_dependency = NULL;
		sys_context = NULL;
		sys_output = NULL;
	

	// ---- path/filenames
		for (UINT i = 0; i < _number_of_files; i++)
		{
			strcpy(_copy_path_filename[i], "");
			strcpy(_dest_path_filename[i], "");
			strcpy(_src_directory[i], "");
			strcpy(_src_filename[i], "");
		}

	return VS_OK;
}


// ---------- CreateDirectory ----------
INT ConfigFiles::PI_ConfigFiles::CreateDirectory(
		CHAR *paf
	)
{

	// ---- local
		INT result = VS_OK;

	// ---- TRY to create
		try
		{
			// ---- folder handle INVALID ? -- throw
				DWORD ftyp = GetFileAttributesA(paf);
				if ( ftyp == INVALID_FILE_ATTRIBUTES )
					throw(paf);
		}

	// ---- CATCH and create
		catch ( char* paf )
		{
			//
			// EEXIST - Directory was not created - already exists - should never happen...
			// ENOENT - Path not found - should never happen..
			result = mkdir(paf);

			// error ?
			if ( result == EEXIST || result == ENOENT )
			{
				return VS_ERROR;
			}
		}

	return VS_OK;
}


// ---------- CreateDocumentsDirectory ----------
INT ConfigFiles::PI_ConfigFiles::CreateDocumentsDirectory(
		CHAR *brand_name
	)
{

	// ---- get [user]
		CHAR *user_profile;
		size_t user_profile_len;
		_dupenv_s(&user_profile, &user_profile_len, "USERPROFILE");


	// ---- build
		//
		// TBD 'userProfile' could be '0':  this does not adhere to the
		// specification for the function 'strcpy_s'. 
		//
		CHAR paf[VS_MAXCHARPATHLEN];
		sprintf(
				paf,
				"%s\\Documents\\%s",
				user_profile,
				brand_name
			);


	// ---- create 
		INT result = CreateDirectory(paf);

	return result;
}


// ---------- CreateAppDataDirectory ----------
INT ConfigFiles::PI_ConfigFiles::CreateAppDataDirectory(
		CHAR *brand_name,
		CHAR *app_name
	)
{

	// ---- local
		INT result = VS_OK;


	// ---- create brand directory
		CHAR *user_profile;
		size_t user_profile_len;
		_dupenv_s(&user_profile, &user_profile_len, "USERPROFILE");
		CHAR paf[VS_MAXCHARPATHLEN];
		sprintf(
				paf, // e.g. "C:\Users\garet"
				"%s\\AppData\\Local\\%s",    // 
				user_profile,
				brand_name
			);
		result = CreateDirectory(paf);


	// ---- create app directory
		sprintf(
				paf,
				"%s\\AppData\\Local\\%s\\%s",
				user_profile,
				brand_name,
				app_name
			);
		result = CreateDirectory(paf);


	// ---- create data directory
		sprintf(
				paf,
				"%s\\AppData\\Local\\%s\\%s\\data",
				user_profile,
				brand_name,
				app_name
			);
		result = CreateDirectory(paf);


	// ---- dev mode ?
		BOOL development_mode = sys_context->GetBool(sys_context->DEVELOPMENT);
		CHAR *app_src_path = NULL;
		if ( development_mode )
			app_src_path = "..\\";
		else
			app_src_path = "";

		 
	// ---- build destination path/filename
		for (UINT i = 0; i < _number_of_files; i++)
		{

			// ---- directory
				if ( _src_directory[i][0] != NULL )
				{
					sprintf(
							paf,
							"%s\\AppData\\Local\\%s\\%s\\data\\%s",
							user_profile,
							brand_name,
							app_name,
							_src_directory[i]
						);
					result = CreateDirectory(paf);
				}
				else
				{
					sprintf(
							paf,
							"%s\\AppData\\Local\\%s\\%s\\data",
							user_profile,
							brand_name,
							app_name
						);
					result = CreateDirectory(paf);
				}


			// ---- destination
				if ( _src_filename[i][0] != NULL )
				{

					sprintf(_dest_path_filename[i], "%s/%s", paf, _src_filename[i]);

					INT len = strlen(_dest_path_filename[i]);
					for (INT l = 0; l < len; l++)
					{
						CHAR *c = &_dest_path_filename[i][l];
						*c = *c == '\\' ? '/' : *c;
					}

				}

		}

	return VS_OK;
}


// ---------- InitLocalCopyPathFilename ----------
INT ConfigFiles::PI_ConfigFiles::InitLocalCopyPathFilename(
		CHAR *app_name
	)
{

	// ---- local
		INT result = VS_OK;


	// ---- dev mode
		BOOL development_mode = sys_context->GetBool(sys_context->DEVELOPMENT);
		CHAR *app_path = NULL;
		if ( development_mode )
			app_path = "..\\";
		else
			app_path = "";


	// ---- build destination path/filename
		for (UINT i = 0; i < _number_of_files; i++)
		{
			if (_src_filename[i][0] != NULL )
			{

				if ( _src_directory[i] != NULL )
				{
					sprintf(_copy_path_filename[i], "%sdata//%s/%s/%s", app_path, app_name, _src_directory[i], _src_filename[i]);
				}
				else
				{
					sprintf(_copy_path_filename[i], "%sdata//%s/%s", app_path, app_name, _src_filename[i]);
				}

				INT len = strlen(_copy_path_filename[i]);
				for (INT l = 0; l < len; l++)
				{
					CHAR *c = &_copy_path_filename[i][l];
					*c = *c == '\\' ? '/' : *c;
				}

			}
		}


	return VS_OK;
}


// ---------- CopyLocalFilesToDest ----------
INT ConfigFiles::PI_ConfigFiles::CopyLocalFilesToDest()
{

	// ---- local
		INT result = VS_OK;


	// ---- files
		for (UINT i = 0; i < _number_of_files; i++)
		{

			// ---- file ?
				if (_src_filename[i][0] != NULL)
				{

					// ---- copy
						if ( GetFileAttributes(_dest_path_filename[i]) == INVALID_FILE_ATTRIBUTES )
						{

							BOOL copyied = CopyFile(
									_copy_path_filename[i],
									_dest_path_filename[i],
									TRUE // TRUE : bail if it exists
								);

							DWORD err = GetLastError();

							if ( !copyied )
							{
								/*
								sys_output->MessageError(
									"FAILED to copy source file to Data directory.",
									"File",
									_copy_path_filename[i]
								);
								*/
								;
							}
						}

				}

		}

	return VS_OK;
}


// ---------- InitLocalDestPathFilename ----------
INT ConfigFiles::PI_ConfigFiles::InitLocalDestPathFilename(
		CHAR *app_name
	)
{

	// ---- local
		INT result = VS_OK;


	// ---- dev mode
		BOOL development_mode = sys_context->GetBool(sys_context->DEVELOPMENT);
		CHAR *app_path = NULL;
		if ( development_mode )
			app_path = "..\\";
		else
			app_path = "";


	// ---- build destination path/filename
		for (UINT i = 0; i < _number_of_files; i++)
		{
			if ( _src_filename[i][0] != NULL )
			{

				if ( _src_directory[i][0] != 0 )
				{
					sprintf(_dest_path_filename[i], "%sdata/%s/%s/%s", app_path, app_name, _src_directory[i], _src_filename[i]);
				}
				else
				{
					sprintf(_dest_path_filename[i], "%sdata/%s/%s", app_path, app_name, _src_filename[i]);
				}

				INT len = strlen(_dest_path_filename[i]);
				for (INT l = 0; l < len; l++)
				{
					CHAR *c = &_dest_path_filename[i][l];
					*c = *c == '\\' ? '/' : *c;
				}

			}
		}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


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
	if ( pi_config_files != NULL )
	{
		delete pi_config_files;
		pi_config_files = NULL;
	}
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT ConfigFiles::Setup(
		vs_system::AppDependency *app_dependency,
		CHAR *brand_name,
		CHAR *app_name
	)
{

	// ---- local
		INT result = 0;


	// ---- inject dependency 
		pi_config_files->app_dependency = app_dependency;
		pi_config_files->sys_context    = app_dependency->GetSysContext();
		pi_config_files->sys_output     = app_dependency->GetSysOutput();


	// ---- documents
		result = pi_config_files->CreateDocumentsDirectory(brand_name);
	

	// ---- init for AppData or local ?
		vs_system::SysContext *sys_context = pi_config_files->sys_context;
		BOOL install_data = sys_context->GetBool(sys_context->INSTALL_DATA);
		if ( install_data )
		{
			result = pi_config_files->CreateAppDataDirectory(brand_name, app_name);
			result = pi_config_files->InitLocalCopyPathFilename(app_name);
			result = pi_config_files->CopyLocalFilesToDest();
		}
		else
		{
			result = pi_config_files->InitLocalDestPathFilename(app_name);
		}

	return VS_OK;
}


// ---------- GetAppFilename ----------
/*!
\brief Get destination path/filename by identity
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ConfigFiles::GetAppFilename(UINT id, CHAR *filename)
{

	if ( pi_config_files->_dest_path_filename[id] == NULL )
		return VS_ERROR;

	strcpy(filename, pi_config_files->_dest_path_filename[id]);

	return VS_OK;
}


// ---------- SetSrcDirectory ----------
/*!
\brief Set source directory
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ConfigFiles::SetSrcDirectory(UINT index, CHAR *directory)
{

	if (directory == NULL)
		return VS_ERROR;

	if (directory[0] == 0)
		return VS_ERROR;

	strcpy(pi_config_files->_src_directory[index], directory);

	return VS_OK;
}


// ---------- SetSrcFilename ----------
/*!
\brief Set source filename
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ConfigFiles::SetSrcFilename(UINT index, CHAR *filename)
{

	if ( filename == NULL )
		return VS_ERROR;

	if ( filename[0] == 0 )
		return VS_ERROR;

	strcpy(pi_config_files->_src_filename[index], filename);

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
