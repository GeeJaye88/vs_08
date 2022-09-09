////////////////////////////////////////////////////////////////////////////////

// ---------- s3_defintions.h ----------

/*!
\file s3_media.h
\brief GetMedia class interface.
\note
*/

#pragma once


// ---- vs system
#include "../../vs_system/header/vs_inc_defs.h"


// ---- windows
#include <ctime>
#include <fstream>
#include "resource.h"


namespace vs_system
{

	class Media
	{
		public:
			static BOOL GetMediaFile(char* file, char path[]);

		private:
			static BOOL SearchFolders(char* filename, char* exeFolder, char* exeName, char fullPath[]);
	};

}

////////////////////////////////////////////////////////////////////////////////
