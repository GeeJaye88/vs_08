////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_img_enum.hpp ----------
/*!
\file r2_tint_img_enum.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---- tab elements group logical id
	//
	// r2_setup_tint_img.hpp
	// r2_ctr_update_element.hpp
	//
	enum
	{
		// selected tab head elements
		ELEM_GROUP_IMG_HEAD_LID = 3000,

		// selected tab body elements
		ELEM_GROUP_IMGPTZ_LID    = 3100,
		ELEM_GROUP_IMGLOC_LID    = 3200,
		ELEM_GROUP_IMGGFX_LID    = 3300,
		ELEM_GROUP_IMGUSER01_LID = 3400,
		ELEM_GROUP_IMGUSER02_LID = 3500,
	};


// ---- tab body logical id
	//
	// r2_setup_tint_img.hpp
	// r2_tint_cam_callback_[...].hpp
	//
	enum
	{
		TINT_TAB_IMGPTZ_LID    = 33,
		TINT_TAB_IMGLOC_LID    = 34,
		TINT_TAB_IMGFX_LID     = 35,
		TINT_TAB_IMGUSER01_LID = 36,
		TINT_TAB_IMGUSER02_LID = 37,
	};

