////////////////////////////////////////////////////////////////////////////////

// ---------- s1_tint_interface.hpp ----------
/*!
\file s1_tint_interface.hpp
\brief Interface for Model class Tint methods and properties
\author Gareth Edwards

\note included in s1_model.h

*/


// ---- s1_model_tint_setup.hpp
	INT TintSetup();


// ---- s1_model_tint_get.hpp
	static FLOAT TintGetBuildParam(INT);
	INT TintGetBodyDisplayListHandle();
	INT TintGetHeadDisplayListHandle();


// ---- s1_model_tint_select.hpp
	INT TintBodySelect(UINT);
	INT TintBodyUpdateText(UINT, INT, INT);


// ---- s1_model_tint_callback.hpp

	// explore
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


	// image location
	INT Tint_ImgLoc_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_ImgLoc_BuildList(vs_system::TintItem[]);
	static INT Tint_ImgLoc_GetIconInfo(INT);
	static INT Tint_ImgLoc_UpdateModel(INT, INT);
	static INT Tint_ImgLoc_UpdateElement(INT, INT);
	static INT Tint_ImgLoc_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_ImgLoc_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);

	// image effects
	INT Tint_ImgFx_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_ImgFx_BuildList(vs_system::TintItem[]);
	static INT Tint_ImgFx_GetIconInfo(INT);
	static INT Tint_ImgFx_UpdateModel(INT, INT);
	static INT Tint_ImgFx_UpdateElement(INT, INT);
	static INT Tint_ImgFx_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_ImgFx_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);

	// system
	INT Tint_System_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_System_BuildList(vs_system::TintItem[]);
	static INT Tint_System_GetIconInfo(INT);
	static INT Tint_System_UpdateModel(INT, INT);
	static INT Tint_System_UpdateElement(INT, INT);
	static INT Tint_System_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_System_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);

	// objects
	INT Tint_Objects_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_Objects_BuildList(vs_system::TintItem[]);
	static INT Tint_Objects_GetIconInfo(INT);
	static INT Tint_Objects_UpdateModel(INT, INT);
	static INT Tint_Objects_UpdateElement(INT, INT);
	static INT Tint_Objects_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_Objects_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);

	// info
	INT Tint_Info_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_Info_BuildList(vs_system::TintItem[]);
	static INT Tint_Info_GetIconInfo(INT);
	static INT Tint_Info_UpdateModel(INT, INT);
	static INT Tint_Info_UpdateElement(INT, INT);
	static INT Tint_Info_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_Info_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);

	// sun
	INT Tint_Sun_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_Sun_BuildList(vs_system::TintItem[]);
	static INT Tint_Sun_GetIconInfo(INT);
	static INT Tint_Sun_UpdateModel(INT, INT);
	static INT Tint_Sun_UpdateElement(INT, INT);
	static INT Tint_Sun_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_Sun_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);

	// time
	INT Tint_Time_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_Time_BuildList(vs_system::TintItem[]);
	static INT Tint_Time_GetIconInfo(INT);
	static INT Tint_Time_UpdateModel(INT, INT);
	static INT Tint_Time_UpdateElement(INT, INT);
	static INT Tint_Time_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_Time_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);

	// colour
	INT Tint_Colour_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_Colour_BuildList(vs_system::TintItem[]);
	static INT Tint_Colour_GetIconInfo(INT);
	static INT Tint_Colour_UpdateModel(INT, INT);
	static INT Tint_Colour_UpdateElement(INT, INT);
	static INT Tint_Colour_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_Colour_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);

	// settings
	INT Tint_Settings_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_Settings_BuildList(vs_system::TintItem[]);
	static INT Tint_Settings_GetIconInfo(INT);
	static INT Tint_Settings_UpdateModel(INT, INT);
	static INT Tint_Settings_UpdateElement(INT, INT);
	static INT Tint_Settings_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_Settings_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);

	// functional and parameter stuff
	class Tint_Explore_Param;
	class Tint_ImgLoc_Param;
	class Tint_ImgFx_Param;