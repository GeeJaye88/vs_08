////////////////////////////////////////////////////////////////////////////////

// ---------- vs_auto_folder.h ----------
/*!
\file vs_auto_folder.h
\brief auto str utilities
\author Gareth Edwards
*/

	// ---- lambda ------------------------------------------------------------
		auto FileExists = [](
			const char* fileName
			)
		{
			DWORD attribs = ::GetFileAttributesA(fileName);
			if (attribs & FILE_ATTRIBUTE_DIRECTORY) return FALSE;
			if (attribs != INVALID_FILE_ATTRIBUTES) return TRUE;
			return FALSE;
		};


	// ---- lambda ------------------------------------------------------------
		auto FolderExists = [](
			const char *dirName
			)
		{
			DWORD attribs = ::GetFileAttributesA(dirName);
			if (attribs == INVALID_FILE_ATTRIBUTES) return FALSE;
			BOOL is_folder = attribs & FILE_ATTRIBUTE_DIRECTORY;
			return is_folder;
		};


