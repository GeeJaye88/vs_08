   ////////////////////////////////////////////////////////////////////////////////

// ---------- vs_action_value_lambda.hpp ----------
/*!
\file vs_action_value_lambda.hpp
\brief
\author Gareth Edwards
*/


// ---- note: no scope requirements


// ---- lambda ------------------------------------------------------------
	//
	// So, complicated.... (:-)
	//
	// At runtime the index of the last picked element (elem_index) must
	// be initialised < 0 (e.g. -1).
	//
	// This unfortunately necessitates runtime handling logic (i.e. if
	// ( elem_index < 0 ) ), which in turn requires getting the currently
	// selected index of a camera property (CamProps) and testing that if
	// the selected index is == the max num of a camera property configs
	// (e.g. CamProps::NUM_EXPOSURE_CONFIGS) less one then the "< o >" buttons
	// are set ON.
	//
	// Note: the USER logical button id must always be the last in a group,
	// and be the base for the "< o >" buttons.
	//
	// The rest of the logic is pretty simple:
	//
	// IF there is a auto mode for this camera property THEN
	//    IF auto mode ON, THEN "< o >" buttons are set OFF
	//       return OFF
	//
	// IF currently selected element index < 0 THEN
	//    IF camera property config index == max num of a camera
	//        property configs less one THEN
	//           "< o >" buttons are set ON & done
	//              return ON
	//
	// IF currently selected element index == base USER lid
	//    (interchangeable with index) THEN
	//        return ON
	//    else
	//        return OFF
	//
	auto get_action_value = [] (
			INT  handle_obj,    // CamProps *cam_props = (CamProps *)handle_obj; 
			INT  elem_index,    // As per "if ( elem_index < 0 )" 
			UINT auto_lid,      // e.g. CamProps::AUTO_EXPOSURE
			UINT prop_lid,      // e.g. CamProps::EXPOSURE_INDEX
			UINT num_config,    // e.g. CamProps::NUM_EXPOSURE_CONFIGS
			UINT user_lid       // e.g. tint_acquire_param.EXP_USER_INDEX
		)
	{

		using namespace vs_system;
		CamProps *cam_props = (CamProps *)handle_obj;

		if ( auto_lid )
		{
			INT auto_flag = 0;
			cam_props->GetProperty(
					CamProps::SETUP,
					0,
					auto_lid,
					&auto_flag
				);
			if ( auto_flag ) return 0;
		}

		if ( elem_index < 0 )
		{

			INT index = 0;
			cam_props->GetProperty(
					CamProps::SETUP,
					0,
					prop_lid,
					&index
				);

			return ( index == num_config - 1 ? 1 : 0 );
		}

		return elem_index == user_lid ? 1 : 0;

	};

