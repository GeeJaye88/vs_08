////////////////////////////////////////////////////////////////////////////////

// ---------- s1_config_files.h ----------
/*!
\file s1_config_files.h
\brief Interface for the ConfigFiles class.
\author Gareth Edwards
*/

#pragma once


// ---------- System include ----------
#include "../../../vs_system/header/vs_app_dependency.h"


// ---------- Library include ----------
#include "../../../vs_library/header/vs_cam_define.h"


// ---------- namespace ----------
namespace vs_system
{

class ConfigFiles
{

public:
	 
	// ---- ctor
		ConfigFiles();
		virtual ~ConfigFiles();


	// ---- framework
		INT Setup(vs_system::AppDependency *, CHAR *, CHAR *);


	// ---- get
		INT GetAppFilename(UINT, CHAR*);


	// ---- set
		INT SetSrcDirectory(UINT, CHAR*);
		INT SetSrcFilename(UINT, CHAR*);

private:

	class PI_ConfigFiles; PI_ConfigFiles *pi_config_files;

};


}


