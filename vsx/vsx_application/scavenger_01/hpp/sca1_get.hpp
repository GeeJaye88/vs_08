////////////////////////////////////////////////////////////////////////////////


// ---------- GetObservantPath ----------
/*!
\brief 
\return INT - ERROR <=0 < VS_OK
*/
INT application::GetObservantPath(CHAR *supplied_path)
{

	// ---- user?
		CHAR *user_profile;
		size_t user_profile_len;
		_dupenv_s(&user_profile, &user_profile_len, "USERPROFILE");


	// ---- observant ?
		sprintf(
				supplied_path, // e.g. "C:\Users\garet"
				"%s\\Documents\\Observant",
				user_profile
			);


	// ---- exists ?
		try
		{
			DWORD ftyp = GetFileAttributesA(supplied_path);
			if ( ftyp == INVALID_FILE_ATTRIBUTES ) throw(supplied_path);
		}
		// CATCH and create directory
		catch ( char* directory_path )
		{
			std::cout << "   - Error: Failed to find directory " << directory_path << std::endl;
			return VS_ERROR;
		}

	return VS_OK;
}


// ---------- GetArchivePath ----------
/*!
\brief GetRunTimeArchivePath
\author Gareth Edwards
*/
INT application::GetArchivePath(
		CHAR *supplied_path,
		CHAR *archive_folder
	)
{

	// ---- folder exists ?
		DWORD file_handle_path = GetFileAttributesA(supplied_path);
		if ( file_handle_path != INVALID_FILE_ATTRIBUTES )
		{

			// ---- set current directory
				BOOL set_directory = SetCurrentDirectory(supplied_path);


			// ---- append "*.*"
				static CHAR scan_paf[VS_MAXCHARPATHLEN];
				sprintf(
						scan_paf,
						"%s\\*.*",
						supplied_path
					);


			// ---- valid path ?
				//     at least ONE folder ?
				//        -- then..
				HANDLE hFind;
				WIN32_FIND_DATA data;
				BOOL found_archive = FALSE;
				CHAR archive_paf[VS_MAXCHARPATHLEN];
				hFind = FindFirstFile(scan_paf, &data); // "c:\\*.*", etc.
				if ( hFind != INVALID_HANDLE_VALUE )
				{

					// ---- loop through all folders
						do {

							// ---- archive_folder with "archive.hdl" ?
								sprintf(
									archive_paf,
									"%s\\%s\\archive.hdl",
									supplied_path,
									data.cFileName
								);
							file_handle_path = GetFileAttributesA(archive_paf);
								if ( file_handle_path != INVALID_FILE_ATTRIBUTES )
								{
									sprintf(
											archive_folder,
											"%s\\%s",
											supplied_path,
											data.cFileName
										);
									found_archive = TRUE;
								}

						} while ( FindNextFile(hFind, &data) );
			

					// ---- close
						FindClose(hFind);
			

					// ---- found ?
						if ( found_archive )
						{
							return VS_OK;
						}
				}
		}


	// ---- report failure
		std::cout << "   - Error: Failed to find archive directory " << archive_folder << std::endl;

		return VS_ERROR;
}


// ---------- GetMinutesElapsed ----------
/*!
\brief get minutes elapsed since scavenger application started
\author Gareth Edwards
*/
LONG application::GetMinutesElapsed()
{
	DOUBLE ms = pi_app->sys_context->GetDouble(vs_system::SysContext::TIME_MS_ELAPSED);
	return (INT)(ms/6000.0f);
}


// ---------- INT  GetTimeStamp(CHAR *); ----------
/*!
\brief Get time stamp as a char string
\author Gareth Edwards
\param CHAR * - returned time stamp (No bounds checking!!!)
\return INT - ERROR <= VS_ERROR < VS_OK
*/
INT application::GetTimeStamp(CHAR *time_stamp)
{

	// ---- initialise returned arg
	strcpy(time_stamp, "");

	// ---- get time NOW
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);

	//
	// GetSystemTime() returns the current time in UTC
	//

	// ---- format
		char tmp[VS_MAXCHARLEN];
		sprintf(
				tmp,
				"%4d/%2d/%2d-%2d:%2d:%2d",
				SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond
			);

	// ---- cleanup unwanted chars
		INT len_filename = (INT)strlen(tmp);
		for (INT i = 0; i < len_filename-3; i++) tmp[i] = tmp[i] == ' ' ? '0' : tmp[i];

	// ---- copy into returned arg
		strcpy(time_stamp, tmp);

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
