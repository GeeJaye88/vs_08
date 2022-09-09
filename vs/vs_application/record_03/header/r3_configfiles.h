////////////////////////////////////////////////////////////////////////////////

// ---------- r3_configfiles.h ----------
/*!
\file r3_files.h
\brief Interface for the ConfigFiles class.
\author Gareth Edwards
\note Vanilla C++
*/

#ifndef RECORD_03_CONFIGFILES
#define RECORD_03_CONFIGFILES


// ---------- System include ----------
#include "../../../vs_system/header/vs_app_dependency.h"


// ---------- Library include ----------
#include "../../../vs_library/header/vs_cam_define.h"


// ---------- namespace ----------
namespace record_03
{


// ---------- class ----------
class ConfigFiles
{

public:
	 
	// Constructor /destructor
	ConfigFiles();
	virtual ~ConfigFiles();

	// Framework
	INT Setup(vs_system::AppDependency *, CHAR *);

	// Get application names
	INT GetCameraApplicationName(CHAR *);
	INT GetCameraDevelopmentName(CHAR *);
	INT GetReviewApplicationName(CHAR *);

	// Get HDL filenames
	INT GetApplicationConfigHdlFilename(CHAR*);
	INT GetCameraConfigHdlFilename(CHAR*);
	INT GetPropsConfigHdlFilename(CHAR*);
	INT GetPanoramicConfigHdlFilename(CHAR*);
	INT GetImageConfigJpgFilename(CHAR*);

private:

	class PI_ConfigFiles; PI_ConfigFiles *pi_config_files;

}; // ConfigFiles class


} // record_03 namespace


#endif // RECORD_03_CONFIGFILES


