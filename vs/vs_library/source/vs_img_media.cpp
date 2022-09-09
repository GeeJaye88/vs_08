////////////////////////////////////////////////////////////////////////////////

// ---------- vs_media.cpp ----------
/*!
\file vs_media.cpp
\brief implementation of the GetMedia class
\author Gareth Edwards
*/

#include <vs/vs_library/header/vs_img_media.h>


using namespace vs_system;


/*!
\brief: Searches through folders to find a media file.
\note: Adapted from the DirectX Sample Framework.
\parameter [in] file - Name of the file we're looking for.
\parameter [in/out] path - If file is found, filled with full path to the desired file.
\return    TRUE if the file was found, FALSE otherwise
*/
BOOL Media::GetMediaFile(
		char* file,
		char path[]
	)
{

	// ---- local
		char exeName[MAX_PATH] = { 0 };
		char exeFolder[MAX_PATH] = { 0 };

	// ---- get full executable path
		GetModuleFileName(NULL, exeFolder, MAX_PATH);
		exeFolder[MAX_PATH - 1] = 0;

	// ---- get pointer to beginning of executable file name which is after the last slash
		char* pCutPoint = NULL;
		for (int i = 0; i < MAX_PATH; i++)
		{
			if (exeFolder[i] == '\\')
			{
				pCutPoint = &exeFolder[i + 1];
			}
		}

	// ---- cutpoint
		if ( pCutPoint )
		{
			// ---- copy over the exe file name
				strcpy_s(exeName, MAX_PATH, pCutPoint);

			// ---- chop off the exe file name from the path so we just have the exe directory
				*pCutPoint = 0;

			// get pointer to start of the .exe extension 
				pCutPoint = NULL;
				for (int i = 0; i < MAX_PATH; i++)
				{
					if (exeName[i] == '.')
					{
						pCutPoint = &exeName[i];
					}
				}

			// ---- chop the .exe extension from the exe name
				if (pCutPoint)
				{
					*pCutPoint = 0;
				}

			// ---- add a slash
				strcat_s(exeName, MAX_PATH, "\\");
		}

	// ---- search all the folders in searchFolders
		if (SearchFolders(file, exeFolder, exeName, path))
		{
			return TRUE;
		}

	// ---- search all the folders in searchFolders with media\ appended to the end
		char mediaFile[MAX_PATH] = "media\\";
		strcat_s(mediaFile, MAX_PATH, file);
		if (SearchFolders(mediaFile, exeFolder, exeName, path))
		{
			return TRUE;
		}

	return FALSE;
}

/*!
\brief Searches through folders to find a media file.
\note adapted from the DirectX Sample Framework.
\parameter [in] filename - File we are looking for
\parameter [in] exeFolder - Folder of the executable
\parameter [in] exeName - Name of the executable
\parameter [in/out] fullPath - Returned path if file is found.
\return    TRUE if the file was found, FALSE otherwise.
*/
BOOL Media::SearchFolders(
		char* filename,
		char* exeFolder,
		char* exeName,
		char fullPath[]
	)
{
	char* searchFolders[] =
	{
		".\\", "..\\", "..\\..\\", "%s", "%s..\\", "%s..\\..\\", "%s..\\%s", "%s..\\..\\%s"
	};

	// ---- look through each folder to find the file
		char currentPath[MAX_PATH] = { 0 };
		for (int i = 0; i < 8; i++)
		{
			sprintf_s(currentPath, MAX_PATH, searchFolders[i], exeFolder, exeName);
			strcat_s(currentPath, MAX_PATH, filename);
			if (GetFileAttributes(currentPath) != INVALID_FILE_ATTRIBUTES)
			{
				strcpy_s(fullPath, MAX_PATH, currentPath);
				return TRUE;
			}
		}

	return FALSE;
}
