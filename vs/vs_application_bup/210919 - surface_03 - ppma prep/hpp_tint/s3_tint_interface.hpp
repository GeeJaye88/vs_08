////////////////////////////////////////////////////////////////////////////////

// ---------- s3_tint_interface.hpp ----------
/*!
\file s3_tint_interface.hpp
\brief Interface for Tint methods and properties
\author Gareth Edwards

\note included in s3_model.h

*/


// ---- s3_model_tint_setup.hpp
	INT TintSetup();

	INT TintHeadUpdateAnalysisTabButtons();


// ---- s3_model_tint_get.hpp
	static FLOAT TintGetBuildParam(INT);
	INT TintGetBodyDisplayListHandle();
	INT TintGetHeadDisplayListHandle();


// ---- s3_model_tint_select.hpp
	INT TintBodySelect(UINT);
	INT TintBodyUpdateVisibleObjects(UINT);
	INT TintBodyUpdateText(UINT, INT, INT);


// ---- s3_model_tint_callback.hpp

	
	// ---- acquire
		INT Tint_Acquire_GetCallbacks(vs_system::TintCallback *);
		static INT Tint_Acquire_BuildList(vs_system::TintItem[]);
		static INT Tint_Acquire_GetIconInfo(INT);
		static INT Tint_Acquire_UpdateModel(INT, INT);
		static INT Tint_Acquire_AddActions(INT, INT, FLOAT[]);
		static INT Tint_Acquire_UpdateElement(INT, INT);
		static INT Tint_Acquire_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
		static INT Tint_Acquire_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);

		static INT Tint_Acquire_GetElemLidSourceSurf();


	// ---- calibrate
		INT Tint_Calibrate_GetCallbacks(vs_system::TintCallback *);
		static INT Tint_Calibrate_BuildList(vs_system::TintItem[]);
		static INT Tint_Calibrate_GetIconInfo(INT);
		static INT Tint_Calibrate_UpdateModel(INT, INT);
		static INT Tint_Calibrate_UpdateElement(INT, INT);
		static INT Tint_Calibrate_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
		static INT Tint_Calibrate_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);


	// ---- light
		INT Tint_Light_GetCallbacks(vs_system::TintCallback *);
		static INT Tint_Light_BuildList(vs_system::TintItem[]);
		static INT Tint_Light_GetIconInfo(INT);
		static INT Tint_Light_UpdateModel(INT, INT);
		static INT Tint_Light_UpdateElement(INT, INT);
		static INT Tint_Light_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
		static INT Tint_Light_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);


	// ---- explore
		INT Tint_Explore_GetCallbacks(vs_system::TintCallback *);
		static INT Tint_Explore_BuildList(vs_system::TintItem[]);
		static INT Tint_Explore_GetIconInfo(INT);
		static INT Tint_Explore_UpdateModel(INT, INT);
		static INT Tint_Explore_UpdateElement(INT, INT);
		static INT Tint_Explore_AddActions(INT, INT, FLOAT[]);
		static INT Tint_Explore_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
		static INT Tint_Explore_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);

		static INT Tint_Explore_UpdateSpeed();

		       INT Tint_Explore_Switch(INT, INT, INT, BOOL);
			   INT Tint_Explore_UpdateByMsg(INT, INT);
			   INT Tint_Explore_UpdateControlByMsg(INT, INT);
			   INT Tint_Explore_UpdateByMessage(INT, INT);


	// ---- measure
		INT Tint_Measure_GetCallbacks(vs_system::TintCallback *);
		static INT Tint_Measure_BuildList(vs_system::TintItem[]);
		static INT Tint_Measure_GetIconInfo(INT);
		static INT Tint_Measure_UpdateModel(INT, INT);
		static INT Tint_Measure_UpdateElement(INT, INT);
		static INT Tint_Measure_AddAction(INT, INT, FLOAT[]);
		static INT Tint_Measure_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
		static INT Tint_Measure_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);


	// ---- enhance
		INT Tint_Enhance_GetCallbacks(vs_system::TintCallback *);
		static INT Tint_Enhance_BuildList(vs_system::TintItem[]);
		static INT Tint_Enhance_GetIconInfo(INT);
		static INT Tint_Enhance_UpdateModel(INT, INT);
		static INT Tint_Enhance_UpdateElement(INT, INT);
		static INT Tint_Enhance_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
		static INT Tint_Enhance_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);

		static INT Tint_Enhance_SetSwitchedOn(INT, INT, BOOL);


	// ---- function
		INT Tint_Function_GetCallbacks(vs_system::TintCallback *);
		static INT Tint_Function_BuildList(vs_system::TintItem[]);
		static INT Tint_Function_GetIconInfo(INT);
		static INT Tint_Function_UpdateModel(INT, INT);
		static INT Tint_Function_UpdateElement(INT, INT);
		static INT Tint_Function_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
		static INT Tint_Function_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);


	// ---- settings
		INT Tint_Settings_GetCallbacks(vs_system::TintCallback *);
		static INT Tint_Settings_BuildList(vs_system::TintItem[]);
		static INT Tint_Settings_GetIconInfo(INT);
		static INT Tint_Settings_UpdateModel(INT, INT);
		static INT Tint_Settings_UpdateElement(INT, INT);
		static INT Tint_Settings_AddAction(INT, INT, FLOAT[]);
		static INT Tint_Settings_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
		static INT Tint_Settings_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);


	// ---- declarations

		class Tint_Acquire_Param;
		class Tint_Calibrate_Param;
		class Tint_Light_Param;
		class Tint_Explore_Param;
		class Tint_Measure_Param;
		class Tint_Enhance_Param;

		class Tint_Function_Param;
		class Tint_Function_Interface;
		class Tint_Function_Prototype;

		class Tint_Settings_Param;


	// ----- last line increment
		#define LAST_LINE_SPACING_0   0.0f
		#define LAST_LINE_SPACING_1   1.6f
		#define LAST_LINE_SPACING_2   3.2f
		#define LAST_LINE_SPACING_3   4.8f

