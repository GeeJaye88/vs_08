////////////////////////////////////////////////////////////////////////////////

// ---------- model_pi_class_incl.h ----------
/*!
\file model_pi_class_incl.h
\brief Panel interface stuff for the PI_Model pimp class
\author Gareth Edwards
*/


// ----------  PROPERTIES ----------


// ---- objects managed by VS
	vs_system::AppDependency *app_dependency = NULL;
	vs_system::GfxInterface  *gfx            = NULL;
	vs_system::SysContext    *sys_context    = NULL;
	vs_system::SysOutput     *sys_output     = NULL;
	vs_system::AppView       *app_view       = NULL;


// ---- kamera
	INT app_kamera_index = 0;
	static const int num_app_camera = 3;


// ---- objects managed by this
	vs_system::ConfigFiles   *config_files   = NULL;
	vs_system::AppKamera     *app_kamera[num_app_camera];
	vs_system::ObjConfig     *obj_config     = NULL;
	vs_system::VsMatrixStack *matrix_stack   = NULL;
	vs_system::ImgProps      *img_props      = NULL;


// ---- camera kontrol
	UINT  camera_control_message             = 0;
	UINT  camera_control_message_count       = 0;
	BOOL  camera_control_persist_active      = 0;
	BOOL  camera_control_persist_switched_on = FALSE;
	FLOAT camera_control_zoom_scalar         = 4;


// ---- flags
	BOOL runtime = TRUE;
	BOOL interacted = FALSE;
	BOOL replace_object_type = FALSE;


// ======== Tint - REQUIRED ========

// ---- managed locally
	vs_system::Tint *tint_setng = NULL;
	vs_system::Tint *kpad_setng = NULL;


// ---- display lists
	INT handle_dspl_tint_head = 0;
	INT handle_dspl_tint_body = 0;
	INT handle_dspl_kpad_head = 0;
	INT handle_dspl_kpad_body = 0;


// ---- view - required for switching keypad ON/OFF
	VOID SetAppView(vs_system::AppView *av) { app_view = av; }
	vs_system::AppView *GetAppView() { return app_view; }


// ======== KeyPad - REQUIRED ========


// ---- keypad
	BOOL keypad_status = FALSE;
	VOID SetKeypadStatus(BOOL ks) { keypad_status = ks ? TRUE : FALSE; }
	BOOL GetKeypadStatus() { return keypad_status; }


// ======== Toolbar - REQUIRED ========


// ---- toolbar
	INT handle_dspl_toolbar = 0;
	INT handle_elem_toolbar[32];

// ---- preserve "Toolbar" rect for when "Main" is fullscreen
	VsFloatRectangle toolbar_rect = { 0, 1, 0, 1 };


// ======== Panels ========


// ---- layout bitmask
	enum
	{
		MAIN    = 1,
		TINT    = 2,
		KEYPAD  = 4,
		TOOLBAR = 8,
		MAIN_COMPLETE = MAIN + TOOLBAR,
		TINT_COMPLETE = MAIN + TINT + TOOLBAR,
		KEYPAD_COMPLETE = MAIN + TINT + KEYPAD + TOOLBAR
	};


// ---- visible bitmasks
	INT   panels_visible_previously = KEYPAD_COMPLETE;
	INT   panels_visible = TINT_COMPLETE;


// ---- mode
	BOOL  panels_main_fullscreen = FALSE;


// ---- toolbar pane
	vs_system::AppPane toolbar_pane;


// ---- aligned
	enum
	{
		TOP    = 1,
		BOTTOM = 2,
		LEFT   = 1,
		RIGHT  = 2,
	};
	INT panels_aligned = LEFT;


// ---- camera
	UINT  kamera_control_mode = vs_system::AppKamera::Mode::XYZ;
	FLOAT kamera_scalar_increment = 1.0f;


// ---- target
	UINT  target_object_id = 0;
	UINT  target_object_id_prev = 0;


// ---------- METHODS ----------

//
// An awkward group of methods that clash with the non-shared model config methods.
//

// ---------- camera - control----------
	UINT GetCameraControlMode()
	{
		return kamera_control_mode;
	}
	VOID SetCameraControlMode(UINT ccm)
	{
		kamera_control_mode = ccm;
	}


// ---------- camera - target ----------

	UINT GetTargetObjectId()
	{
		return target_object_id;
	}

	UINT GetTargetObjectIdPrev()
	{
		return target_object_id_prev;
	}

	VOID SetTargetObjectId(UINT toi)
	{
		target_object_id_prev = target_object_id;
		target_object_id = toi;
	}

	VOID SetTargetObjectIdPrev(UINT toi)
	{
		target_object_id_prev = toi;
	}

