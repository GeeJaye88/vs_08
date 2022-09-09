////////////////////////////////////////////////////////////////////////////////

// ---------- vs_auto_str.h ----------
/*!
\file vs_auto_str.h
\brief auto cdp utilities
\author Gareth Edwards
*/

	auto StrPrepCdp =
		[
			FolderExists,    // dependency: vs_auto_folder.h
			StrCatSlash,     // dependency: vs_auto_str.h
			StrDelFolder,    // dependency: vs_auto_str.h
			StrRepChar       // dependency: vs_auto_str.h
		]
		(
		char *current_directory_path
		)
	{
		CHAR temp[VS_MAXCHARPATHLEN];
		{
			strcpy(temp, current_directory_path);
			INT temp_len = strlen(temp);
			StrCatSlash(temp, &temp_len, '/');
			strcat(temp, "data/");
		}
		BOOL file_exists_so_app_mode = FolderExists(temp);
		if (file_exists_so_app_mode)
		{
			INT cdp_len = strlen(current_directory_path);
			StrCatSlash(current_directory_path, &cdp_len, '/');
		}
		else
		{
			{
				strcpy(temp, current_directory_path);
				INT temp_len = strlen(temp);
				StrDelFolder(temp, &temp_len, '/');
				strcat(temp, "data/");
			}
			BOOL file_exists_so_ide_mode = FolderExists(temp);
			if (file_exists_so_ide_mode)
			{
				INT cdp_len = strlen(current_directory_path);
				StrDelFolder(current_directory_path, &cdp_len, '/');
			}
			else
			{
				// runtime context has no data folder
				return VS_ERROR;   // dependency: vs_inc_defs.h
			}
		}
		INT cdp_len = strlen(current_directory_path);
		StrRepChar(current_directory_path, cdp_len, '\\', '/');
		return VS_OK;   // dependency: vs_inc_defs.h
	};
