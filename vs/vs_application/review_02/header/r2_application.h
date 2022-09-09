////////////////////////////////////////////////////////////////////////////////

// ---------- r2_application.h ----------
/*!
\file r2_application.h
\brief Interface for the application class.
\author Gareth Edwards
*/


#ifndef REVIEW_02_APPLICATION
#define REVIEW_02_APPLICATION


////////////////////////////////////////////////////////////////////////////////


// ---- include
#include "r2_include.h"
#include "../../../vs_library/header/vs_arc_clip.h"



// ---- MVC
#include "r2_control.h"
#include "r2_model.h"
#include "../../../vs_system/header/vs_app_view.h"


// ---- image
#include "../../../vs_library/header/vs_img_buffer.h"
#include "../../../vs_library/header/vs_img_jpeglib.h"
#include "../../../vs_library/header/vs_img_process.h"
#include "../../../vs_library/header/vs_img_timeline.h"
#include "../../../vs_library/header/vs_lidar_ouster.h"


// ---- archive
#define USE_IMG_ARKIVE
#ifdef USE_IMG_ARKIVE
#include "../../../vs_library/header/vs_img_arkive.h"
#else
#include "../../../vs_library/header/vs_img_archive.h"
#endif


// ----namespace ----
namespace review_02
{


// ---------- application class ----------
class application
{

public:
	 

	// ---- c/d tor
		application();
		virtual ~application();


	// ---- framework
		INT Setup(vs_system::AppDependency*);
		INT SetupGraphics();
		INT Display();
		INT CleanupGraphics();
		INT Cleanup();


	// ---- callback
		static INT callbackDisplay(INT, vs_system::AppPanel *);


	// ---- archive export thread
		INT CreateArchiveExportThread();
		static DWORD WINAPI ArchiveExport(LPVOID);


	// ---- panophoto
		INT Panophoto(vs_system::ImgBuffer *);


	// ---- panophoto thread
		INT CreatePanophotoThread(vs_system::ImgBuffer *);
		static DWORD WINAPI CreatePanophoto(LPVOID);


	// ---- non private (!) implmentation
		class PI_Props; PI_Props *pi_props;


private:

	// ---- MVC object pointers
		vs_system::AppView       *app_view;
		Control                  *control;
		Model                    *model;


	// ---- VS image object pointers
		vs_system::ImgTimeline   *img_timeline;
		vs_system::ImgBuffer     *img_buffer;

		#ifdef USE_IMG_ARKIVE
		vs_system::ImgArkive     *img_arkive;
		#else
		vs_system::ImgArchive    *img_archive;
		#endif

		#if VS_LIDAR
		vs_system::LidarOuster   *lidar_ouster;
		vs_system::CsvObject     *csv_buffer;
		#endif


	// ---- application
		INT  GetArchivePath(CHAR *, UINT);
		INT  GetRunTimeArchivePath(CHAR *, UINT);
		INT  CreateTimeline(CHAR *);


	// ---- timeline
		INT  TimelineDisplay();
		INT  TimelineGoto(INT, INT);
		INT  TimelineClipGoto(INT);


	// ---- archive
		INT  ArchiveQueue();
		INT  ArchiveRead();
		INT  ArchiveUpdate();


}; // application class


} // review_02 namespace


#endif // REVIEW_02_APPLICATION


