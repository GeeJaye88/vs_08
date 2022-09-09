////////////////////////////////////////////////////////////////////////////////

// ---------- s1_include.h ----------
/*!
\file s1_include.h
\brief Include
\author Gareth Edwards

\note included in s1_model.cpp & s1_kontrol.cpp
as tint enumerations required in both

*/

#pragma once


// ---- VS system include
#include "../../../vs_system/header/vs_app_dependency.h"
#include "../../../vs_system/header/vs_app_camera.h"
#include "../../../vs_system/header/vs_app_kamera.h"
#include "../../../vs_system/header/vs_app_keyboard.h"
#include "../../../vs_system/header/vs_app_kontrol.h"
#include "../../../vs_system/header/vs_app_layout.h"
#include "../../../vs_system/header/vs_inc_maths.h"
#include "../../../vs_system/header/vs_app_view.h"
#include "../../../vs_system/header/vs_elm_matrix.h"
#include "../../../vs_system/header/vs_elm_lam.h"


// ---- library include
#include "../../../vs_library/header/vs_img_capture.h"
#include "../../../vs_library/header/vs_img_props.h"
#include "../../../vs_library/header/vs_tint.h"
#include "../../../vs_library/header/vs_obj_config.h"
#include "../../../vs_library/header/vs_parameter.h"
#include "../../../vs_library/header/vs_dot_obj_elem.h"


// ---- application (library candidates) include
#include "../header/s1_config_files.h"


// ---- application include
#include "../header/s1_system_object.h"
#include "../header/s1_system_param.h"
#include "../header/s1_system_io.h"
#include "../header/s1_model_enum.h"
#include "../header/s1_model.h"
#include "../header/s1_kontrol.h"


// ---- Toolbar - required for Kontrol class
#include "../../surface_shared/hpp_toolbar/model_toolbar_enum.hpp"


// ---- Tint - required for application class
#include "../hpp_tint/s1_tint_enum.hpp"


// ---- Kpad - required for application class
#include "../../surface_shared/hpp_kpad/model_kpad_enum.hpp"