////////////////////////////////////////////////////////////////////////////////

// ---------- s2_tint_interface.hpp ----------
/*!
\file s2_tint_interface.hpp
\brief Interface for Tint methods and properties
\author Gareth Edwards

\note included in s2_model.h

*/


// ---- s2_model_tint_setup.hpp
	INT TintSetup();


// ---- s2_model_tint_get.hpp
	static FLOAT TintGetBuildParam(INT);
	INT TintGetBodyDisplayListHandle();
	INT TintGetHeadDisplayListHandle();


// ---- s2_model_tint_select.hpp
	INT TintBodySelect(UINT);
	INT TintBodyUpdate(UINT);


// ---- s2_model_tint_callback.hpp

	
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
		static INT Tint_Acquire_GetElemLidSourceFunction();
		static INT Tint_Acquire_GetElemLidLuminaFirst();
		static INT Tint_Acquire_GetElemLidLuminaLast();

	// ---- explore
		INT Tint_Explore_GetCallbacks(vs_system::TintCallback *);
		static INT Tint_Explore_BuildList(vs_system::TintItem[]);
		static INT Tint_Explore_GetIconInfo(INT);
		static INT Tint_Explore_UpdateModel(INT, INT);
		static INT Tint_Explore_UpdateElement(INT, INT);
		static INT Tint_Explore_AddActions(INT, INT, FLOAT[]);
		static INT Tint_Explore_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
		static INT Tint_Explore_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);
		       INT Tint_Explore_GetElemLidUpdateFirst();
		       INT Tint_Explore_GetElemLidTargetFirst();
			   
	// ---- explore kamera interface
		static INT Tint_Explore_UpdateSpeed();
		       INT Tint_Explore_Switch(INT, INT, INT, BOOL);
			   INT Tint_Explore_UpdateByMsg(INT, INT);
			   INT Tint_Explore_UpdateControlByMsg(INT, INT);
			   INT Tint_Explore_UpdateByMessage(INT, INT);


	// ---- function
		INT Tint_Function_GetCallbacks(vs_system::TintCallback *);
		static INT Tint_Function_BuildList(vs_system::TintItem[]);
		static INT Tint_Function_GetIconInfo(INT);
		static INT Tint_Function_UpdateModel(INT, INT);
		static INT Tint_Function_UpdateElement(INT, INT);
		static INT Tint_Function_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
		static INT Tint_Function_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);


	// ---- declarations

		class Tint_Acquire_Param_New_2;
		class Tint_Explore_Param;

		class Tint_Function_Param;
		class Tint_Function_Interface;
		class Tint_Function_Prototype;


	// ----- last line increment
		#define LAST_LINE_SPACING_0   0.0f
		#define LAST_LINE_SPACING_1   1.6f
		#define LAST_LINE_SPACING_2   3.2f
		#define LAST_LINE_SPACING_3   4.8f

