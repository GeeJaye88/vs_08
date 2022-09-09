////////////////////////////////////////////////////////////////////////////////

// ---------- vs_folders.h ----------
/*!
\file vs_folders.h
\brief Interface for the Folders class.
\author Gareth Edwards
*/

#pragma once


// ---- include 
#include <stdio.h>        // 
#include <ctype.h>        // 
#include <string>         // std::string
#include <iostream>       // std::cout
#include <stack>          // std::stack
#include <vector>         // std::vector
#include <deque>          // std::deque


// ---- vs include ----
#include "../vs_system/header/vs_inc_defs.h"


// ---------- namespace ----------
namespace vs_system
{


// ---------- Folders Info Struct ----------
struct FolderInfo
{
	std::string file_name;
	std::string folder_name;
	std::string folder_path;
	INT folder_depth = 0;
};


// ---------- Folder Class ----------
class Folder
{

public:

	// ---------- ctor ----------
	/*!
	\brief Constructor
	\author Gareth Edwards
	*/
	Folder::Folder()
	{
		callback_on_file = NULL;
		callback_on_folder = NULL;
	}


	// ---------- dtor ----------
	/*!
	\brief Constructor
	\author Gareth Edwards
	*/
	Folder::~Folder()
	{
		;
	}

private:

	// ---------- properties ----------

	INT(*callback_on_file)(vs_system::FolderInfo *);
	INT(*callback_on_folder)(vs_system::FolderInfo *);


public:

	// ---------- methods ----------


	// ---------- Scan ----------
	/*!
	\brief Scan folder system
	\author Gareth Edwards
	*/
	INT Scan(std::string folder_path)
	{

		// ---- local
			HANDLE hFind = INVALID_HANDLE_VALUE;
			WIN32_FIND_DATA fdata;
			std::string fullpath;
			std::stack<std::string> folders;
			std::stack<INT> depth;
			std::string scan_mask = "*";

		// ---- push folder path
			std::string current_folder_path;
			folders.push(folder_path);

		// ---- push folder depth
			INT current_folder_depth = 0;
			depth.push(current_folder_depth);

		// ---- root - init folder info - and invoke folder callback
			vs_system::FolderInfo info;
			info.file_name    = "";
			info.folder_path  = folder_path;
			INT found         = (INT)folder_path.find_last_of('\\');
			info.folder_name  = folder_path.substr(found + 1);
			info.folder_depth = current_folder_depth;
			INT result = callback_on_folder(&info);


		// ---- until no more folders
			while ( !folders.empty() )
			{

				// ---- init "here"
					current_folder_path  = folders.top();
					current_folder_depth = depth.top();

				// ---- FindFirstFile path
					fullpath = current_folder_path + "\\" + scan_mask;

				// ---- pop
					folders.pop();
					depth.pop();

				// ---- push
					hFind = FindFirstFile(fullpath.c_str(), &fdata);
					if (hFind != INVALID_HANDLE_VALUE)
					{
						do
						{
							if ( strcmp(fdata.cFileName, ".") != 0 &&
								strcmp(fdata.cFileName, "..") != 0 )
							{
								if ( fdata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
								{

									// ---- push
										folders.push(current_folder_path + "\\" + fdata.cFileName);
										depth.push(current_folder_depth + 1);

									// ---- init folder info - and invoke folder callback
										info.file_name    = "";
										info.folder_path  = folders.top();
										info.folder_name  = fdata.cFileName;
										info.folder_depth = depth.top();

										if (callback_on_folder != NULL )
											INT result = callback_on_folder(&info);

								}
								else
								{

									// ---- file - init folder file info 
										info.file_name    = fdata.cFileName;
										info.folder_path  = current_folder_path;
										INT found         = (INT)current_folder_path.find_last_of('\\');
										info.folder_name  = current_folder_path.substr(found + 1);
										info.folder_depth = current_folder_depth;
										if (callback_on_file != NULL)
											INT result = callback_on_file(&info);

								}
							}
						} while ( FindNextFile(hFind, &fdata) != 0 );
					}

				// ---- check this folder
					if ( GetLastError() != ERROR_NO_MORE_FILES )
					{
						FindClose(hFind);
						return false;
					}

				// ---- close this folder
					FindClose(hFind);
					hFind = INVALID_HANDLE_VALUE;
			}

		return VS_OK;
	}


	// ---------- SetCallbackOnFile ----------
	/*!
	\brief SetCallbackOnFile
	\author Gareth Edwards
	\return INT - ERROR <=0 < VS_OK
	*/
	INT SetCallbackOnFile(
			INT(*callback)(vs_system::FolderInfo *info)
		)
	{
		callback_on_file = callback;
		return VS_OK;
	}


	// ---------- SetCallbackOnFolder ----------
	/*!
	\brief SetCallbackOnFolder
	\author Gareth Edwards
	\return INT - ERROR <=0 < VS_OK
	*/
	INT SetCallbackOnFolder(
			INT(*callback)(vs_system::FolderInfo *info)
		)
	{
		callback_on_folder = callback;
		return VS_OK;
	}


};


} // vs_system namespace



///////////////////////////////////////////////////////////////////////////////
