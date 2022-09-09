////////////////////////////////////////////////////////////////////////////////

// ---------- model_kpad_enum.hpp ----------
/*!
\file model_kpad_enum.hpp
\brief Interface for Model class Tint enumerations
\author Gareth Edwards
*/

// ---- required pragma
//      included in application include ( e.g. s3_include.h) &
//      also in application Model include (e.g. s3_model.cpp )


// ---- kpad head logical id
	//
	// kpad_kontrol_class.hpp
	// model_kpad_setup.hpp
	//
	enum
	{
		// selected tab head elements
		KPAD_HEAD_LID = 2001,

	};


// ---- kpad body logical id
	//
	// model_kpad_kontrol.hp
	// model_kpad_[...]_callback_[...].hpp
	// model_kpad_setup.hpp
	//
	enum
	{
		KPAD_BODY_INPUT_LID = 100,
		KPAD_BODY_LID = 100,
	};

