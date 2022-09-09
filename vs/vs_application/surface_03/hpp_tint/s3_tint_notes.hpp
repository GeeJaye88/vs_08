////////////////////////////////////////////////////////////////////////////////

// ---------- s3_tint_notes.hpp ----------
/*!
\file s3_tint_notes.hpp
\brief Hints & Tips
\author Gareth Edwards
*/

/*

Binding funtionality of a new Tab
==================================

1) In the "s3_tint_enum.hpp" file:

	Add a primary enumerated value for the tab, e.g.,
		TINT_BODY_FUNCTION_LID             = 140,

	Add secondary enumerated value(s) for local radio widgets, e.g.,
		TINT_BODY_FUNCTION_PARAM_LID       = 141,


2) In the "s3_tint_kontrol.hpp" file:

	Add the above enumerations to the Kontrol::TintUpdateBody method,
	in the multiway switch statement, and also invoke te tab
	"get callback" method, e.g.,

	case TINT_BODY_MEASURE_LID:
	case TINT_BODY_MEASURE_PARAM_LID:
		model->Tint_Measure_GetCallbacks(&tint_callbacks);
		break;


3) In the "s3_tint_select.hpp" file:

	Add the primary enumerated value to the Model:TinBodySelect method,
	in the multiway switch statement, and also set the parameter group
	value (if any), e.g.,

	case TINT_BODY_MEASURE_LID:
		surface_03->SetTargetLoiMeasure(TRUE);
		param_group = PARAM_GROUP_FUNCTION;
		break;

	Also note the additional method invoked when this tab is selected.


4) In the "s3_model_kamera.hpp" file:

	Add the primary enumerated value to the Model:SetMsgProc method,
	in the multiway switch statement, if the SetMsgCameraControl method
	is also required to be invoked, e.g.,

	case TINT_BODY_MEASURE_LID:
		result = SetMsgCameraControl(uc_msg, delta, FALSE);
		break;


5) In the "s3_model_kpad_set_with_tint_update.hpp" file:

	Add the enumerated parameter group value to the Model:Kpad_SetParameter
	method, in the multiway switch statement, e.g.,

	case surface_03::Model::PARAM_GROUP_FUNCTION:
		model->Tint_Function_UpdateElement(handle_model, param_array_index + 'A');
		break;

	This handles the update of an element after the keypad has been used.


Adding a new Parameter Group
============================

1) In the "s3_surface_03.h" file:

	Declare the setup parameter group method, e.g.,
		INT SetupParamMeasure(INT *);

2) In the "s3_param_setup.hpp" file:

	Add a setup parameter group method, e.g.,
		INT Surface_03::SetupParamMeasure(INT *offset)
		{
			// .... see other methods for examples
		}

3) In the "s3_surface_03.cpp" file:

	Invoke the setup method, e.g.,
		result = SetupParamMeasure(&offset);

4) In the "s3_tint_select.hpp" file:

	Note: see above #3 in Binding funtionality of a new Tab

	Add the parameter group value (if any), e.g.,

	case TINT_BODY_MEASURE_LID:
		param_group = PARAM_GROUP_FUNCTION;
		break;


*/

