////////////////////////////////////////////////////////////////////////////////

// ---------- r2_include.h ----------
/*!
\file r2_include.h
\brief application include files.
\author Gareth Edwards
*/

#ifndef REVIEW_02_INCLUDE
#define REVIEW_02_INCLUDE


// ---------- Use preprocessor directive to turn OFF MS deprecation warnings ----------
#define _CRT_SECURE_NO_DEPRECATE


// ---------- include application #define (e.g. "CAMERA_03") ----------
#include "../../../vs_application/header/vs_app_setup.h"


// ---------- document modules enumeration ----------
#include "../hpp_tint_cam/r2_tint_cam_enum.h"
#include "../hpp_tint_img/r2_tint_img_enum.h"


// ---------- review application defintions ----------
#include "../header/r2_definitions.h"


// ----------  review application configuration file ----------
#include "../header/r2_configfiles.h"


// ---------- library ----------
#include "../../../vs_library/header/vs_obj_config.h"
#include "../../../vs_library/header/vs_img_lut.h"


// ---------- system include ----------
#include "../../../vs_system/header/vs_app_dependency.h"
#include "../../../vs_system/header/vs_app_panel.h"
#include "../../../vs_system/header/vs_sys_licenseinfo.h"


#endif // REVIEW_02_INCLUDE


