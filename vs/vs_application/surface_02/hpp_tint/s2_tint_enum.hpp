////////////////////////////////////////////////////////////////////////////////

// ---------- s2_tint_enum.hpp ----------
/*!
\file s2_tint_enum.hpp
\brief Interface for Model class Tint enumerations
\author Gareth Edwards
*/

#pragma once


// ---- tint head logical id
	//
	// s2_tint_kontrol.hpp
	// s2_model_tint_setup.hpp
	//
	enum
	{
		// selected tab head elements
		TINT_HEAD_2DS_LID = 3000,

	};


// ---- tint body logical id
	//
	// s2_kontrol_tint.hp
	// s2_tint_callback_[...].hpp
	// s2_model_tint_setup.hpp
	//
	enum
	{
		TINT_BODY_SETUP_GFX_LID            = 120,

		TINT_BODY_ACQUIRE_LID              = 150,
		TINT_BODY_ACQUIRE_ARRAY_LID        = 152,
		TINT_BODY_ACQUIRE_PARAM_LID        = 153,

		TINT_BODY_EXPLORE_LID              = 110,
		TINT_BODY_EXPLORE_LID_GROUP_1      = 161,
		TINT_BODY_EXPLORE_LID_GROUP_2      = 162,

		TINT_BODY_FUNCTION_LID             = 140,
		TINT_BODY_FUNCTION_PARAM_LID       = 141,
	};

