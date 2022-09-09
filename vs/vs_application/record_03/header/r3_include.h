////////////////////////////////////////////////////////////////////////////////

// ---------- r3_include.h ----------
/*!
\file r3_include.h
\brief application include files.
\author Gareth Edwards
\note Vanilla C++
*/

#ifndef RECORD_03_INCLUDE
#define RECORD_03_INCLUDE


// ---------- Use preprocessor directive to turn OFF MS deprecation warnings ----------
#define _CRT_SECURE_NO_DEPRECATE


// ---------- App define----------
#define TITLE_BAR_GROUP    1
#define TITLE_BAR_ABOUT    1

#define CAM_REPORT_GROUP   2
#define CAM_REPORT_RECORD  1
#define CAM_REPORT_EXIT    2

#define ABOUT_BUTTON_GROUP_ID 3
#define ABOUT_VIEW            1
#define ABOUT_CLOSE           2


// ---------- VS App define ----------
#include "../../../vs_application/header/vs_app_setup.h"


// ---------- VS APP include ----------
#include "../header/r3_configfiles.h"


// ---------- VS APP include ----------
#include "../../../vs_system/header/vs_app_dependency.h"
#include "../../../vs_system/header/vs_app_panel.h"
#include "../../../vs_system/header/vs_app_view.h"
#include "../../../vs_system/header/vs_sys_license_expire.h"


#endif // RECORD_03_INCLUDE


