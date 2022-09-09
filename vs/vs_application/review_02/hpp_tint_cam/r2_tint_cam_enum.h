////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_cam_enum.hpp ----------
/*!
\file r2_tint_cam_enum.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---- tab elements group logical id
	//
	// r2_setup_tint_cam.hpp
	// r2_ctr_update_element.hpp
	//
	enum
	{
		// selected tab head elements
		ELEM_GROUP_CAM_HEAD_LID = 2000,

		// selected tab body elements
		ELEM_GROUP_ACQUIRE_LID  = 2300,
		ELEM_GROUP_GAINMAS_LID  = 2400,
		ELEM_GROUP_GAINCOL_LID  = 2500,
		ELEM_GROUP_COLOUR_LID   = 2600,
		ELEM_GROUP_SETTINGS_LID = 2950,
		ELEM_GROUP_PANO_LID     = 2700,
		ELEM_GROUP_AEC_LID      = 2800,
		ELEM_GROUP_FOCUS_LID    = 2900,
	};


// ---- tab body logical id
	//
	// r2_setup_tint_cam.hpp
	// r2_tint_cam_callback_[...].hpp
	//
	enum
	{
		TINT_HEAD_ACQUIRE_LID  = 54,
		TINT_HEAD_GAINMAS_LID  = 55,
		TINT_HEAD_GAINCOL_LID  = 56,
		TINT_HEAD_COLOUR_LID   = 57,
		TINT_HEAD_SETTINGS_LID = 58,
		TINT_HEAD_PANO_LID     = 1,
		TINT_HEAD_AEC_LID      = 23,
		TINT_HEAD_SYSTEM_LID   = 24,
	};
