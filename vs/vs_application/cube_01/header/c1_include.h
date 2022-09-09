////////////////////////////////////////////////////////////////////////////////

// ---------- c1_include.h ----------
/*!
\file c1_include.h
\brief application include files.
\author Gareth Edwards
*/

#pragma once


// ---- vs system ----
#include "../../../vs_system/header/vs_inc_defs.h"
#include "../../../vs_system/header/vs_inc_maths.h"


// ---- vs graphics ----
#ifdef VS_GFX_FLAG
#include "../../../vs_system/header/vs_gfx_interface.h"
#include "../../../vs_system/header/vs_sys_input.h"
#endif


// ---- vs sys ----
#include "../../../vs_system/header/vs_app_dependency.h"
#include "../../../vs_system/header/vs_app_view.h"
#include "../../../vs_system/header/vs_pan_context.h"
#include "../../../vs_system/header/vs_app_kamera.h"
#include "../../../vs_system/header/vs_app_kontrol.h"


// ---- vs lib ----
#include "../../../vs_library/header/vs_obj_config.h"
#include "../../../vs_library/header/vs_vnode.h"


// ---- application include ----
#include "../header/c1_model.h"
#include "../header/c1_kontrol.h"
