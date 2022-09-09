////////////////////////////////////////////////////////////////////////////////

// ---------- c3_application.h ----------
/*!
\file c3_application.h
\brief Interface for the application class.
\author Gareth Edwards
\note 2017
*/


#ifndef CAMERA_03_APPLICATION
#define CAMERA_03_APPLICATION


////////////////////////////////////////////////////////////////////////////////


//  ---- include ----
#include "../header/c3_include.h"


// ---- namespace ----
namespace camera_03
{


// ---------- application class ----------
class application
{

public:

	// ---- c/dtor ----
	application();
	virtual ~application();


	// ---------- framework ----------

	       INT Setup(vs_system::AppDependency*);
	static INT SetupGraphics();
	       INT Display();
	static INT CleanupGraphics();
	       INT Cleanup();


private:


	// ---------- setup ----------

	// ---- camera ----
	INT SetupProperties();
	INT SetupArchive();
	INT SetupImgLutProcess();
	INT SetupCamera();
	INT SetupScanFolder();

	// ---- report ----
	INT ReportAppConfig();

	// ---- media setup----
	INT SetupONVIF();
	INT SetupGPS();
	INT SetupIPC();
	INT SetupScavenger();
		INT SetScavengerLocation(CHAR *path);
		INT StartScavenger();
	INT SetupSharedMemory();
	INT SetupStreaming();


	// ---------- setup graphics ----------
	//
	// invoked in SetupGraphics,
	//            SwitchCameraMode, and
	//            SwitchCameraLoopHardStart
	static INT StartLiveCamera();


	// ---------- display ----------

	INT SwitchArchiveSave();
	INT SwitchCameraMode();
	INT SwitchCameraLoopHardStart();
	INT SwitchDashboardOnly();
	INT ProcessGpsReader();
	INT ProcessGpsStatus();
	INT UpdateMonitor();

	static INT CallbackDisplayFromView(INT, vs_system::AppPanel *);
	static INT DisplayStream(vs_system::AppPanel *);
	static INT DisplayShared(vs_system::AppPanel *);

	INT UpdateSecondCount();
	INT UpdateCameraExposure();
	INT UpdateCameraHOffset();
	INT UpdateStreamIcon();


	// ---------- image ----------

	// ---- media - manage miniH264 ----
	INT FindAndKillProcess(CHAR *);
	INT KillProcess(DWORD, UINT);
	INT StartProcess(CHAR*, CHAR *);

	// ---- callback when an image is "captured" ----
	static INT CallbackOnCaptureImage(
			WORD,
			WORD,
			WORD,
			BYTE *,
			INT []
		);
	static INT ReportCaptureImageInfo(INT *);

	//----  hierarchical path stack and filename -
	//      invoked by CallbackOnCaptureImage
	static 	INT CreateSavePathFilename(
			std::string(&path)[6],
			std::string &filename,
			INT timestamp[]
		);

	//---- process a "captured" image -
	//     invoked by CallbackOnCaptureImage
	static INT ProcessCapturedImage(
			std::string(&)[IMAGE_ARCHIVE_PATHTOP],
			std::string &,
			WORD,
			WORD,
			WORD,
			BYTE *
		);


	// ---------- cleanup ----------

	INT CleanupONVIF();
	INT CleanupGPS();
	INT CleanupIPC();
	INT CleanupScavenger();
	INT CleanupSharedMemory();
	INT CleanupStreaming();


}; // application class


} // camera_03 namespace


#endif // CAMERA_03_APPLICATION
