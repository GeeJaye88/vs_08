////////////////////////////////////////////////////////////////////////////////

// ---------- c3_application.h ----------
/*!
\file c3_application.h
\brief Interface for the application class.
\author Gareth Edwards
\note Vanilla C++
*/

#ifndef RECORD_03_APPLICATION
#define RECORD_03_APPLICATION


// ---------- include ----------
#include "../../record_03/header/r3_include.h"

// ---------- MVC ----------
#include "../../record_03/header/r3_control.h"
#include "../../record_03/header/r3_model.h"
#include "../../../vs_system/header/vs_app_view.h"

// ---------- Library ----------
#include "../../../vs_library/header/vs_img_buffer.h"
#include "../../../vs_library/header/vs_img_archive.h"
#include "../../../vs_library/header/vs_img_jpeglib.h"

// ---------- Project ----------
#include "../../../vs_project/acquire_05/header/vs_image_archive.h"

// ---------- Camera Config ----------
#include "../../../vs_library/header/vs_cam_modes.h"

// ---------- Cameras ----------
#include "../../../vs_library/header/vs_cam_ids.h"
#include "../../../vs_library/header/vs_cam_jply.h"
#include "../../../vs_library/header/vs_cam_pic.h"


// ---------- namespace ----------
namespace record_03
{


// ---------- application class ----------
class application
{

public:

	// Constructor/Destructor
	application();
	virtual ~application();

	// Framework methods
	INT Setup(vs_system::AppDependency*);
	INT SetupGraphics();
	INT Display();
	INT CleanupGraphics();
	INT Cleanup();

	static 	INT CreateSavePathFilename(
			std::string(&path)[6],
			std::string &filename,
			INT *timestamp
			);
	static INT Report(INT *);

private:

	// Callback: Sub framework - from view to display
	static INT CallbackDisplayFromView(INT, vs_system::AppPanel *);

	// Setup: sub framework from setup
	INT SetupProperties();
	INT SetupArchive();
	INT SetupCamera();

	// Start
	INT StartLiveCamera();

	// Camera
	INT SwitchCameraLive();
	INT SwitchArchiveSave();


	INT ProcessCameraEvents();
	INT UpdateCameraConfig();

	// Callback on capture - pass image to....
	static INT CallbackOnCaptureImage(
			WORD,
			WORD,
			WORD,
			BYTE *,
			INT []
		);

	// - pass image to.... Process
	static INT ProcessCapturedImage(
			std::string(&)[IMAGE_ARCHIVE_PATHTOP],
			std::string &,
			WORD,
			WORD,
			WORD,
			BYTE *,
			INT []
		);

	// Private implementations
	class  PI_Camera; static PI_Camera *pi_camera;
	class  PI_Props; static PI_Props *pi_props;

}; // application class


} // record_03 namespace


#endif // RECORD_03_APPLICATION
