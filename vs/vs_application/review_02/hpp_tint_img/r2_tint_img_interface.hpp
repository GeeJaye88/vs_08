////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_img_interface.hpp ----------
/*!
\file r2_tint_img_interface.hpp
\brief Interface for the Model class Tint methods and properties
\author Gareth Edwards
*/

// ---- r2_tint_img_update_body.hpp

	INT Tint_Img_UpdateBody(UINT);
	INT Tint_Img_UpdateBodyText(UINT, INT, INT);


// ---- r2_tint_img_callback.hpp

	// image pan, tilt and zoom
	INT Tint_ImgPtz_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_ImgPtz_BuildList(vs_system::TintItem[]);
	static INT Tint_ImgPtz_GetIconInfo(INT);
	static INT Tint_ImgPtz_UpdateModel(INT, INT);
	static INT Tint_ImgPtz_UpdateElement(INT, INT);
	static INT Tint_ImgPtz_AddActions(INT, INT, FLOAT *);
	static INT Tint_ImgPtz_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_ImgPtz_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);

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

	// config
	INT Tint_Fconfig_GetCallbacks(vs_system::TintCallback *);
	static INT Tint_Fconfig_BuildList(vs_system::TintItem[]);
	static INT Tint_Fconfig_GetIconInfo(INT);
	static INT Tint_Fconfig_UpdateModel(INT, INT);
	static INT Tint_Fconfig_UpdateElement(INT, INT);
	static INT Tint_Fconfig_AddText(INT, INT, BOOL *, BOOL *, CHAR *);
	static INT Tint_Fconfig_AddValue(INT, INT, BOOL *, FLOAT *, CHAR *);

	// functional and parameter stuff
	class Tint_Fconfig_Param;
	class Tint_ImgPtz_Param;
	class Tint_ImgFx_Param;
	class Tint_ImgLoc_Param;