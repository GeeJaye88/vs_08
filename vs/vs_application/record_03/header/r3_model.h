////////////////////////////////////////////////////////////////////////////////

// ---------- r3_model.h ----------
/*!
\file r3_model.h
\brief Interface for the Model class.
\author Gareth Edwards
\note Vanilla C++
*/

#ifndef RECORD_03_MODEL
#define RECORD_03_MODEL


// ---------- application include ----------
#include "r3_include.h"

// ---------- library include ----------
#include "../../../vs_library/header/vs_cam_define.h"
#include "../../../vs_library/header/vs_cam_modes.h"
#include "../../../vs_library/header/vs_cam_props.h"
#include "../../../vs_library/header/vs_obj_config.h"
#include "../../../vs_library/header/vs_img_buffer.h"

// ---------- system include ----------
#include "../../../vs_system/header/vs_elm_context.h"
#include "../../../vs_system/header/vs_gfx_context.h"


// ---------- namespace ----------
namespace record_03
{


// ---------- Model class ----------
class Model
{

public:

	// ---- constructor /destructor ----
	Model();
	virtual ~Model();


	// ********** METHODS **********


	// ---- Framework ----
	INT Setup(vs_system::AppDependency*);
	INT SetupGraphics();
	INT DisplayBegin();
	INT Display(vs_system::GfxContext *, vs_system::ImgBuffer *);
	INT DisplayEnd();
	INT CleanupGraphics();
	INT Cleanup();

	// ---- TitleBar ----
	INT SetupTitleBar();
	INT SetupTitleBarDisplayList(INT);

	// ---- CamReport ----
	INT SetupCamReport();
	INT SetupCamReportDisplayList(INT);

	// ---- Camera liught modes gfx ----
	INT SetupCamModes();
	INT SetupCamModesDisplayList(INT);
	INT UpdateCamModesDisplayList(BOOL);

	// ---- Camera light modes ----
	INT GetCamModes(vs_system::CamModes **);
	INT GetCamModesPaf(CHAR *);
	INT ReadCamModes(CHAR *);
	INT UpdateCamModes(UINT, UINT);

	// ---- Camera properties ----
	INT GetCamProps(vs_system::CamProps **);
	INT GetPropsConfigPaf(CHAR *);
	INT ReadCamProps(CHAR *);

	// ---- About properties ----
	INT SetupAbout();
	INT SetupAboutDisplayList(INT);
	INT SetAboutDisplayStatus();

	// ---- Frameset ----
	INT SetupFrameset(vs_system::AppDependency *, vs_system::AppView *);

	// ----- Config Files -----
	INT GetConfigFiles(ConfigFiles **);

	// ---- Get ----
	INT GetDeviceStatus();

	// ---- Set ----
	INT SetConfigureAllBitmask(BYTE);

	// ---- Update report text elements ----
	INT UpdateRecordText(CHAR *);
	INT UpdateFrameRateText(UINT);

private:

	// Private implementation
	class PI_Model; PI_Model *pi_model;

}; // Model class


} // record_03 namespace


#endif // RECORD_03_MODEL


