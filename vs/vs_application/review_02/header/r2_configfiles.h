////////////////////////////////////////////////////////////////////////////////

// ---------- r2_configfiles.h ----------
/*!
\file r2_files.h
\brief Interface for the ConfigFiles class.
\author Gareth Edwards
*/

#ifndef REVIEW_02_CONFIGFILES
#define REVIEW_02_CONFIGFILES


// ---------- System include ----------
#include "../../../vs_system/header/vs_app_dependency.h"


// ---------- Library include ----------
#include "../../../vs_library/header/vs_cam_define.h"


// ---------- namespace ----------
namespace review_02
{


// ---------- class ----------
class ConfigFiles
{

public:
	 
	// con/destructor
	ConfigFiles();
	virtual ~ConfigFiles();

	// framework
	INT Setup(vs_system::AppDependency *, CHAR *, UINT);

	// get HDL filenames
	INT GetApplicationConfigHdlFilename(CHAR*);
	INT GetImageConfigHdlFilename(CHAR*);
	INT GetPropsConfigHdlFilename(CHAR*);
	INT GetObjectConfigHdlFilename(CHAR*);
	INT GetImageConfigJpgFilename(CHAR*);

private:

	class PI_ConfigFiles; PI_ConfigFiles *pi_config_files;

}; // ConfigFiles class


} // review_02 namespace


#endif // REVIEW_02_CONFIGFILES


