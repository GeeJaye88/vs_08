////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_cam_interface.hpp ----------
/*!
\file r2_tint_cam_interface.h
\brief Interface for the Model class Tint methods and properties
\author Gareth Edwards
*/

// ---- r2_tint_cam_update_body.hpp

	INT Tint_Cam_UpdateBody(UINT);
	INT Tint_Cam_UpdateBodyText(UINT, INT, INT);
	INT Tint_Cam_UpdateParamName(CHAR*, INT, INT);


// ---- r2_tint_cam_callback.hpp

	// acquire
	INT Tint_Acquire_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_Acquire_BuildList(vs_system::TintItem []);
	static INT Tint_Acquire_GetIconInfo(INT);
	static INT Tint_Acquire_UpdateModel(INT, INT);
	static INT Tint_Acquire_UpdateElement(INT, INT);
	static INT Tint_Acquire_AddActions(INT, INT, FLOAT *);
	static INT Tint_Acquire_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_Acquire_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);


	// colour
	INT Tint_Colour_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_Colour_BuildList(vs_system::TintItem[]);
	static INT Tint_Colour_GetIconInfo(INT);
	static INT Tint_Colour_UpdateModel(INT, INT);
	static INT Tint_Colour_UpdateElement(INT, INT);
	static INT Tint_Colour_AddActions(INT, INT, FLOAT *);
	static INT Tint_Colour_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_Colour_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);


	// gain master
	INT Tint_GainMas_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_GainMas_BuildList(vs_system::TintItem[]);
	static INT Tint_GainMas_GetIconInfo(INT);
	static INT Tint_GainMas_UpdateModel(INT, INT);
	static INT Tint_GainMas_UpdateElement(INT, INT);
	static INT Tint_GainMas_AddActions(INT, INT, FLOAT *);
	static INT Tint_GainMas_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_GainMas_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);


	// gain colour
	INT Tint_GainCol_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_GainCol_BuildList(vs_system::TintItem[]);
	static INT Tint_GainCol_GetIconInfo(INT);
	static INT Tint_GainCol_UpdateModel(INT, INT);
	static INT Tint_GainCol_UpdateElement(INT, INT);
	static INT Tint_GainCol_AddActions(INT, INT, FLOAT *);
	static INT Tint_GainCol_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_GainCol_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);


	// settings
	INT Tint_Settings_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_Settings_BuildList(vs_system::TintItem[]);
	static INT Tint_Settings_GetIconInfo(INT);
	static INT Tint_Settings_UpdateModel(INT, INT);
	static INT Tint_Settings_UpdateElement(INT, INT);
	static INT Tint_Settings_AddActions(INT, INT, FLOAT *);
	static INT Tint_Settings_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_Settings_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);


	// pan
	INT Tint_Pan_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_Pan_BuildList(vs_system::TintItem []);
	static INT Tint_Pan_GetIconInfo(INT);
	static INT Tint_Pan_UpdateModel(INT, INT);
	static INT Tint_Pan_UpdateElement(INT, INT);
	static INT Tint_Pan_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_Pan_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);


	// aec
	INT Tint_Aec_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_Aec_BuildList(vs_system::TintItem[]);
	static INT Tint_Aec_GetIconInfo(INT);
	static INT Tint_Aec_UpdateModel(INT, INT);
	static INT Tint_Aec_UpdateElement(INT, INT);
	static INT Tint_Aec_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_Aec_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);


	// focus
	INT Tint_Focus_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_Focus_BuildList(vs_system::TintItem[]);
	static INT Tint_Focus_GetIconInfo(INT);
	static INT Tint_Focus_UpdateModel(INT, INT);
	static INT Tint_Focus_UpdateElement(INT, INT);
	static INT Tint_Focus_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_Focus_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);


	// rroi
	INT Tint_Rroi_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_Rroi_BuildList(vs_system::TintItem[]);
	static INT Tint_Rroi_GetIconInfo(INT);
	static INT Tint_Rroi_UpdateModel(INT, INT);
	static INT Tint_Rroi_UpdateElement(INT, INT);
	static INT Tint_Rroi_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_Rroi_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);


	// functional and parameter stuff
	class Tint_Acquire_Param;
	class Tint_Colour_Param;
	class Tint_GainCol_Param;
	class Tint_GainMas_Param;
	class Tint_Settings_Param;

	class Tint_Pan_Param;
	class Tint_Aec_Param;
	class Tint_Focus_Param;