////////////////////////////////////////////////////////////////////////////////

// ---------- c3_include.h ----------
/*!
\file c3_include.h
\brief Include for c3 based applications.
\author Gareth Edwards
\note 2017
*/

#ifndef CAMERA_03_INCLUDE
#define CAMERA_03_INCLUDE


////////////////////////////////////////////////////////////////////////////////


// ---- Jerry ----

#define _CRT_SECURE_NO_WARNINGS


#ifdef VS_CAMERA_03_GPS
#include <gps_thrd.h>           // NMEA 'compliant' serial port GPS reader
#endif


#ifdef VS_CAMERA_03_SHARED_MEMORY
#include <shbuf.h>              // prototype interfce for shared memory buffer.
#include <gps_thrd.h>           // NMEA 'compliant' serial port GPS reader
#endif


#ifdef VS_CAMERA_03_STREAMING
#include <video\ipc.h>          // interprocess communication via shared memory
#include <apptypes.h>           // ministream ipc_name and PixelData struct
#include <onvif_handler.h>      // ONVIF HTTP/XML handler
#include <pix/pixtypes.h>       // pixel type mappings and converters
#endif


////////////////////////////////////////////////////////////////////////////////


// ---------- NOTE: Review_02 include ----------
//
// Note: These are the files included via r2_include.h.
//
// ---------- include application #define (e.g. "CAMERA_03") ----------
// #include "../../../vs_application/header/vs_app_setup.h"
//
// ---------- document modules enumeration ----------
// #include "../hpp_tint_cam/r2_tint_enum.h"
//
// ---------- review application defintions ----------
// #include "../header/r2_definitions.h"
//
// ----------  review application configuration file ----------
// #include "../header/r2_configfiles.h"
//
// ---------- library ----------
// #include "../../../vs_library/header/vs_obj_config.h"
// #include "../../../vs_library/header/vs_img_lut.h"
//
// ---------- system include ----------
// #include "../../../vs_system/header/vs_app_dependency.h"
// #include "../../../vs_system/header/vs_app_panel.h"
// #include "../../../vs_system/header/vs_sys_licenseinfo.h"
//

#include "../../review_02/header/r2_include.h"


////////////////////////////////////////////////////////////////////////////////


// ---------- Camera ----------


// ---- MVC ----
#include "../../review_02/header/r2_control.h"
#include "../../review_02/header/r2_model.h"


// ---- MVC: System ----
#include "../../../vs_system/header/vs_app_view.h"


// ---- Library ----
#include "../../../vs_library/header/vs_img_buffer.h"
#include "../../../vs_library/header/vs_img_jpeglib.h"
#include "../../../vs_library/header/vs_img_process.h"
#include "../../../vs_library/header/vs_img_timeline.h"
#include "../../../vs_include/vs_folder.h"
#include "../../../vs_library/header/vs_lidar_ouster.h"


// ---- Project ----
#include "../../../vs_library/header/vs_img_arkive.h"


// ---- Cameras ----
#include "../../../vs_library/header/vs_cam_ids.h"
#include "../../../vs_library/header/vs_cam_pic.h"


#endif // CAMERA_03_INCLUDE