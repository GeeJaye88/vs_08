////////////////////////////////////////////////////////////////////////////////

// ---------- kpad_interface..hpp ----------
/*!
\file kpad_interface..hpp
\brief Interface for Model class Tint methods and properties
\author Gareth Edwards

\note included in s3_model.h

*/


// ---- s3_kpad_setup.hpp
	INT KpadSetup();


// ---- s3_kpad_get.hpp
	static FLOAT  KpadGetBuildParam(INT);
	INT KpadGetBodyDisplayListHandle();
	INT KpadGetHeadDisplayListHandle();


// ---- s3_model_tint_select.hpp
	INT KpadBodySelect(UINT);
	INT KpadBodyUpdateText(UINT, INT, INT);


// ---- s3_kpad_1_callback.hpp
	INT Kpad_Input_GetCallbacks(vs_system::TintCallback *);
	static INT Kpad_Input_BuildList(vs_system::TintItem[]);
	static INT Kpad_Input_GetIconInfo(INT);
	static INT Kpad_Input_UpdateModel(INT, INT);
	static INT Kpad_Input_UpdateElement(INT, INT);
	static INT Kpad_Input_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Kpad_Input_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);
	static INT Kpad_Update_SetElementList(INT, INT);

// ---- interface
	static INT Kpad_UpdateByParameter(INT, INT);
	static INT Kpad_UpdateByMessage(INT, INT);
	static INT Kpad_SetParameter(INT);


// ---- declare
	class Kpad_Input_Param;
	class Kpad_Input_Float;
