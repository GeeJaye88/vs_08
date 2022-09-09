////////////////////////////////////////////////////////////////////////////////

// ---------- sca1_tmp.hpp ----------
/*!
\file sca1_application.hpp
\brief 
\author Gareth Edwards
*/


////////////////////////////////////////////////////////////////////////////////

/*

	// ---- folder
	if (0)
	{
		// ---- generic - requires formatting
		int len = strlen(archive_path);
		for (int i = 0; i < len; i++)
			archive_path[i] = archive_path[i] == '\\' ? '/' : archive_path[i];
		strcat(archive_path, "/archive/");

		pi_app->folder->SetCallbackOnFile(CallbackFile);
		pi_app->folder->SetCallbackOnFolder(CallbackFolder);
		pi_app->folder->Scan(archive_path);

	}



INT application::CallbackFile(vs_system::FolderInfo *info)
{
	std::cout << "CallbackFolder " << info->file_name << std::endl;
	return VS_OK;
}


INT application::CallbackFolder(vs_system::FolderInfo *info)
{

	// ---- starts at top level directory
		if ( info->folder_depth == 6 )
		{
			std::cout << "CallbackFolder " << info->folder_name << std::endl;
		}
	
	return VS_OK;
}


*/

////////////////////////////////////////////////////////////////////////////////
