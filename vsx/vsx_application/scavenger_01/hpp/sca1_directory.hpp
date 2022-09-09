////////////////////////////////////////////////////////////////////////////////


// ---------- DeleteDirectory ----------
/*!
\brief recursively delete directories
\return INT - ERROR <=0 < VS_OK
*/
INT application::DeleteDirectory(
		const std::string &refcstrRootDirectory,
		BOOL delete_subdirectories
	)
{

	//#define DELETE_TEST

	bool            sub_directory = false;       // Flag, indicating whether
												 // subdirectories have been found

	HANDLE          hFile;                       // Handle to directory
	std::string     strFilePath;                 // Filepath
	std::string     strPattern;                  // Pattern
	WIN32_FIND_DATA FileInformation;             // File information


	strPattern = refcstrRootDirectory + "\\*.*";
	hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
	if (hFile != INVALID_HANDLE_VALUE)
	{
			INT num_files_found = 0;
			do
			{
					if ( FileInformation.cFileName[0] != '.' )
					{

							strFilePath.erase();
							strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;

							if ( FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
							{

									if ( delete_subdirectories )
									{
										// ---- delete subdirectory
											int iRC = DeleteDirectory(
													strFilePath,
													delete_subdirectories
												);
											if ( iRC )
												return iRC;
									}
									else
										sub_directory = true;

							}
							else
							{

								// ---- set file attributes
									if ( ::SetFileAttributes(strFilePath.c_str(),
											FILE_ATTRIBUTE_NORMAL) == FALSE)
										return ::GetLastError();

								// ---- delete file
									if (::DeleteFile(strFilePath.c_str()) == FALSE)
										return ::GetLastError();

									num_files_found++;
							}

					}

			} while ( ::FindNextFile(hFile, &FileInformation) == TRUE );


		// ---- close handle
			::FindClose(hFile);


		// ---- cleanup
			DWORD dwError = ::GetLastError();
			if ( dwError != ERROR_NO_MORE_FILES )
			{
				return dwError;
			}
			else
			{

				// ---- NOT ?
					if ( !sub_directory )
					{

						// ---- set directory attributes
							if ( ::SetFileAttributes(
									refcstrRootDirectory.c_str(),
										FILE_ATTRIBUTE_NORMAL
											) == FALSE
								)
								return ::GetLastError();


						// ---- delete directory
							if ( ::RemoveDirectory(
									refcstrRootDirectory.c_str()
										) == FALSE
								)
								return ::GetLastError();

					}
			}
	}

	return 0;
}


// ---------- DeleteEmptyDirectories ----------
/*!
\brief recursively delete ONLY empty directories
\return INT - ERROR <=0 < VS_OK
*/
INT application::DeleteEmptyDirectories(
		const std::string &refcstrRootDirectory
	)
{

	HANDLE          hFile;                       // Handle to directory
	std::string     strFilePath;                 // Filepath
	std::string     strPattern;                  // Pattern
	WIN32_FIND_DATA FileInformation;             // File information


	strPattern = refcstrRootDirectory + "\\*.*";
	hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
	if (hFile != INVALID_HANDLE_VALUE)
	{
			INT num_files_found = 0;
			INT num_directories_found = 0;
			do
			{
					if ( FileInformation.cFileName[0] != '.' )
					{

						strFilePath.erase();
						strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;

						if ( FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
						{
							int iRC = DeleteEmptyDirectories(strFilePath);
							if ( iRC ) return iRC;
							num_directories_found++;
						}
						else
						{
							num_files_found++;
						}

					}

			} while ( ::FindNextFile(hFile, &FileInformation) == TRUE );


		// ---- close handle
			::FindClose(hFile);


		// ---- cleanup
			DWORD dwError = ::GetLastError();
			if ( dwError != ERROR_NO_MORE_FILES )
			{
				return dwError;
			}
			else
			{

					if ( num_files_found == 0 && num_directories_found == 0 )
					{

						// ---- set directory attributes
							if ( ::SetFileAttributes(
									refcstrRootDirectory.c_str(),
										FILE_ATTRIBUTE_NORMAL
											) == FALSE
								)
								return ::GetLastError();


						// ---- delete directory
							if ( ::RemoveDirectory(
									refcstrRootDirectory.c_str()
										) == FALSE
								)
								return ::GetLastError();


						// ---- chop
							CHAR msg[256];
							strcpy(msg, refcstrRootDirectory.c_str());
							INT len = strlen(msg);
							CHAR *date_time = &msg[len - 13];
							for (INT i = len - 13; i < len; i++) msg[i] = msg[i] == '\\' ? '/' : msg[i];


						// ---- report
							if ( pi_app->sys_context->GetBool(pi_app->sys_context->DEVELOPMENT) )
								std::cout << "   - E " << date_time << " --- " << std::endl;


						// ---- log
							if ( pi_app->log )
							{
								std::ofstream log_file(pi_app->log_path, std::ios_base::out | std::ios_base::app);
								if ( log_file.is_open() )
								{
									log_file << "   - E " << date_time << " --- " << std::endl;
									log_file.close();
								}
							}

					}
			}
	}

	return 0;
}


// ---------- DirectoryExists ----------
/*!
\brief get directory status
\return INT - ERROR <=0 < VS_OK
*/
BOOL application::DirectoryExists(
		const std::string &refcstrRootDirectory
	)
{

	// ---- get attributes
		DWORD ftyp = GetFileAttributesA(refcstrRootDirectory.c_str());

	// ---- something is wrong with your path!
		if (ftyp == INVALID_FILE_ATTRIBUTES)
			return false;

	// ---- yay -- this is a directory!
		if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
			return true;

	// ---- not a directory!
	return false;

}


////////////////////////////////////////////////////////////////////////////////
