////////////////////////////////////////////////////////////////////////////////

// ---------- s3_tint_enum.hpp ----------
/*!
\file s3_tint_enum.hpp
\brief Interface for Model class Tint enumerations
\author Gareth Edwards
*/

#pragma once


// ---- tint head logical id
	//
	// s3_tint_kontrol.hpp
	// s3_model_tint_setup.hpp
	//
	enum
	{
		// selected tab head elements
		TINT_HEAD_3DS_LID = 3000,

	};


// ---- tint body logical id
	//
	// s3_kontrol_tint.hp
	// s3_tint_callback_[...].hpp
	// s3_model_tint_setup.hpp
	//
	enum
	{
		TINT_BODY_ACQUIRE_LID              = 150,
		TINT_BODY_ACQUIRE_ARRAY_LID        = 152,
		TINT_BODY_ACQUIRE_PARAM_LID        = 153,

		TINT_BODY_CALIBRATE_LID            = 120,

		TINT_BODY_LIGHT_LID                = 130,
		TINT_BODY_LIGHT_PARAM_LID          = 131,

		TINT_BODY_EXPLORE_LID              = 110,
		TINT_BODY_EXPLORE_LID_GROUP_1      = 161,
		TINT_BODY_EXPLORE_LID_GROUP_2      = 162,

		TINT_BODY_MEASURE_LID              = 160,
		TINT_BODY_MEASURE_PARAM_LID        = 164,

		TINT_BODY_ENHANCE_LID              = 170,

		TINT_BODY_FUNCTION_LID             = 140,
		TINT_BODY_FUNCTION_PARAM_LID       = 141,

		TINT_BODY_SETTINGS_LID             = 180,


	};

