////////////////////////////////////////////////////////////////////////////////

// ---------- r2_pi_model.hpp ----------
/*!
\file r2_pi_model.hpp
\brief
\author Gareth Edwards
*/


// ---------- Private Implementation of PI_Model class ----------

class Model::PI_Model
{

public:

	static const INT maxptf = vs_system::ObjConfig::MAX_PANO_PTF_LUT;

	// ---- constructor
	PI_Model()
	{

		// ---- create, allocate and init, application objects
			tint_cam_setng = new vs_system::Tint();
			tint_img_setng = new vs_system::Tint();
			config_files   = new ConfigFiles();


		// ---- "PTF" panels
			for ( UINT i=0; i<maxptf; i++)
				panel_context_ptf[i] = NULL;


		// ---- init handle_dspl_ir_img_button & handle_dspl_mv_img_button

		// ---- overview img id buttons
			for (UINT i = 0; i < maxptf; i++) handle_dspl_ir_img_button[i] = 0;
			for (UINT i = 0; i < maxptf; i++) 
				for (UINT j = 0; j < 3; j++)
					handle_elem_ir_img_button[i][j] = 0;

			// multi-view img id buttons
			for (UINT i = 0; i < maxptf; i++) handle_dspl_mv_img_button[i] = 0;
			for (UINT i = 0; i < maxptf; i++)
				for (UINT j = 0; j < 3; j++)
					handle_elem_mv_img_button[i][j] = 0;

			// map view id buttons
			handle_dspl_vm_img_button = 0;
			for (UINT i = 0; i < maxptf; i++)
				for (UINT j = 0; j < 3; j++)
					handle_elem_vm_img_button[i][j] = 0;


		// ---- img icon
			dspl_img_nav_panel_group_row = 0;


		// ---- img multiview
			for (UINT i = 0; i < maxptf; i++) handle_dspl_mv_img_rect_quad[i] = 0;
			for (UINT i = 0; i < maxptf; i++) handle_elem_mv_img_rect_quad[i] = 0;
			for (UINT i = 0; i < maxptf; i++) handle_dspl_mv_img_rect_line[i] = 0;
			for (UINT i = 0; i < maxptf; i++) handle_elem_mv_img_rect_line[i] = 0;


		// ---- img navigation buttons
			handle_dspl_img_nav_but = 0;
			for (UINT i = 0; i < 6; i++) handle_elem_img_nav_but[i] = 0;;


		// ---- img overview
			for (UINT i = 0; i < maxptf; i++) panel_context_img_rect[i]        = 0;
			for (UINT i = 0; i < maxptf; i++) handle_dspl_ir_img_rect_quad[i]  = 0;
			for (UINT i = 0; i < maxptf; i++) handle_elem_ir_img_rect_quad[i]  = 0;
			for (UINT i = 0; i < maxptf; i++) handle_dspl_ir_img_rect_line[i]  = 0;
			for (UINT i = 0; i < maxptf; i++) handle_elem_ir_img_rect_line[i]  = 0;
			for (UINT i = 0; i < maxptf; i++) handle_dspl_ir_img_rect_view[i]  = 0;
			for (UINT i = 0; i < maxptf; i++) handle_elem_ir_img_rect_view[i]  = 0;
			handle_dspl_ir_img_rect_locate = 0;
			for (UINT i = 0; i < maxptf; i++) handle_elem_ir_img_rect_vmap[i]  = 0;


		// ---- stream
			for (UINT i = 0; i < 4; i++) handle_elem_stream_hidden[i]  = 0;
			for (UINT i = 0; i < 7; i++) handle_elem_stream_visible[i] = 0;


		// ---- dbz
			for (UINT i = 0; i < 32; i++) handle_dbz_cam_dial[i]   = 0;
			for (UINT i = 0; i <  3; i++) handle_dbz_cam_icon[i]   = 0;
			for (UINT i = 0; i <  3; i++) handle_dbz_cam_needle[i] = 0;
			for (UINT i = 0; i <  6; i++) handle_dbz_cam_text[i]   = 0;
			for (UINT i = 0; i <  9; i++) handle_dbz_cam_gps[i]    = 0;
			for (UINT i = 0; i <  5; i++) handle_dbz_cam_sys[i]    = 0;


		// ---- setup
			strcpy(app_setup_config_paf, "");


		// ---- props
			strcpy(cam_props_config_paf, "");
			strcpy(img_props_config_paf, "");

	};


	// ---- destructor
		~PI_Model()
		{
			if ( tint_cam_setng != NULL ) { delete tint_cam_setng; tint_cam_setng = NULL; }
			if ( tint_img_setng != NULL ) { delete tint_img_setng; tint_img_setng = NULL; }
			if ( config_files   != NULL ) { delete config_files;   config_files = NULL;   }
		};


	// ---- VS Objects
		vs_system::AppDependency *app_dependency = NULL;
		vs_system::CamGraphs     *cam_graphs     = NULL;
		vs_system::CamProps      *cam_props      = NULL;
		vs_system::ImgProps      *img_props      = NULL;
		vs_system::ObjConfig     *obj_config     = NULL;
		vs_system::GfxInterface  *gfx            = NULL;
		vs_system::Tint          *tint_cam_setng = NULL;
		vs_system::Tint          *tint_img_setng = NULL;
		vs_system::CsvObject     *csv_object     = NULL;
		vs_system::LidarOuster   *lidar_ouster   = NULL;


	// ---- application Objects
		ConfigFiles *config_files = NULL;


	// ---- frame (used in Control::UpdateFrameGroup)
		UINT frame_id = 0;


	// ---- PTF Panels
		vs_system::PanelContext *panel_context_ptf[maxptf];

		UINT home_frame_selected  = 2;
		UINT active_panel_id      = 1;
		UINT copyied_src_frame    = 0;
		BOOL copyied_panel_ptf    = FALSE;


	// ---- archive
		BOOL archive_save_on_at_runtime = FALSE;
		BOOL streaming_on_at_runtime    = FALSE;
		BOOL streaming_and_onvif_on_at_runtime = FALSE;
	

	// ---- capture image
		BOOL capture_image_runtime_flag = TRUE;
		BOOL capture_image_number       = 1;
		CHAR capture_image_dir_name[64];


	// ---- setup
		CHAR app_setup_config_paf[VS_MAXCHARPATHLEN];


	// ---- props
		CHAR cam_props_config_paf[VS_MAXCHARPATHLEN];
		CHAR img_props_config_paf[VS_MAXCHARPATHLEN];


	// ---- various context
		vs_system::PanelContext *gps_data_panel_context       = NULL;
		vs_system::PanelContext *progress_bar_panel_context   = NULL;
		vs_system::PanelContext *stream_bg_panel_context      = NULL;
		vs_system::PanelContext *stream_content_panel_context = NULL;
		vs_system::PanelContext *stream_control_panel_context = NULL;


	// ---- ONVIF
		BOOL onvif_remote_control         = FALSE;
		std::string hostaddr              = "not set";
		std::string onvifport             = "not set";
		std::string rtspport              = "not set";
		std::string rtsp_stream           = "not set";

	
	// ---- UI handles
		INT handle_dspl_title_button      = NULL;
		INT handle_dspl_frame_button      = NULL;
		INT handle_dspl_context_button    = NULL;
		INT handle_dspl_timeline_button   = NULL;
		INT handle_dspl_clipline_button   = NULL;
		INT handle_dspl_control_button    = NULL;


	// ---- pano geometry handles
		INT handle_dspl_pano_annu         = NULL;
		INT handle_dspl_pano_annu_exp_aoi = NULL;
		INT handle_dspl_pano_annu_foc_aoi = NULL;
		INT handle_dspl_pano_annu_foc_grf = NULL;
		INT handle_elem_pano_annu_foc_grf = NULL;

		// ---- spherical panorama
			INT handle_dspl_pano_spherical    = NULL;
			INT handle_elem_pano_spherical    = NULL;

		// ----- lidar - proportional
			INT handle_dspl_pano_lidar        = NULL;
			#if VS_LIDAR
			INT handle_elem_pano_lidar        = NULL;
			#endif
			INT handle_dspl_pano_cylindrical  = NULL;
			INT handle_dspl_pano_rect         = NULL;
			INT handle_elem_pano_rect         = NULL;
			INT handle_dspl_pano_front        = NULL;
			INT handle_dspl_pano_back         = NULL;


	// ---- declare handle_dspl_ir_img_button & handle_dspl_mv_img_button

	// ---- img buttons
		INT handle_dspl_ir_img_button[maxptf];
		INT handle_elem_ir_img_button[maxptf][3];

		INT handle_dspl_mv_img_button[maxptf];
		INT handle_elem_mv_img_button[maxptf][3];

		INT handle_dspl_vm_img_button;
		INT handle_elem_vm_img_button[maxptf][3];


	// ---- img icon
		INT dspl_img_nav_panel_group_row = 0;


	// ---- img multiview
		INT handle_dspl_mv_img_rect_quad[maxptf];
		INT handle_elem_mv_img_rect_quad[maxptf];

		INT handle_dspl_mv_img_rect_line[maxptf];
		INT handle_elem_mv_img_rect_line[maxptf];


	// ---- img navigation buttons
		INT handle_dspl_img_nav_but;
		INT handle_elem_img_nav_but[6];


	// ---- img review (navigation)
		INT handle_dspl_img = NULL;

		vs_system::PanelContext *panel_context_img_rect[maxptf];

		INT handle_dspl_ir_img_rect_quad[maxptf];
		INT handle_elem_ir_img_rect_quad[maxptf];

		INT handle_dspl_ir_img_rect_line[maxptf];
		INT handle_elem_ir_img_rect_line[maxptf];

		INT handle_dspl_ir_img_rect_view[maxptf];
		INT handle_elem_ir_img_rect_view[maxptf];

		INT handle_dspl_ir_img_rect_locate;
		INT handle_elem_ir_img_rect_vmap[maxptf];


	// ---- panel gfx contest handles
		INT handle_pan_gfx_con_pano_annu_exp_aoi = NULL;
		INT handle_pan_gfx_con_pano_annu_foc_aoi = NULL;
		INT handle_pan_gfx_con_pano_annu_foc_grf = NULL;


	// ---- "stream"
		INT handle_dspl_stream_hidden     = NULL;
		INT handle_dspl_stream_visible    = NULL;
		INT handle_elem_stream_hidden[4];
		INT handle_elem_stream_visible[7];


	// ---- shared
		INT handle_dspl_shared_hidden     = NULL;
		INT handle_elem_shared_hidden     = NULL;


	// ---- ui handles
		INT handle_dspl_gps_info          = NULL;

		INT handle_dspl_tint_cam_head     = NULL;
		INT handle_dspl_tint_cam_body     = NULL;

		INT handle_dspl_tint_img_head     = NULL;
		INT handle_dspl_tint_img_body     = NULL;

		INT handle_dspl_monitor           = NULL;
		INT handle_dspl_progress_bar      = NULL;
		INT handle_dspl_about             = NULL;
		INT handle_dspl_stream_ui         = NULL;
		INT handle_dspl_monitor_image     = NULL;
		INT handle_dspl_monitor_control   = NULL;
		INT handle_dspl_nest              = NULL;


	// ---- ui dbz
		INT handle_dspl_dbz               = NULL;
		INT handle_dbz_cam_dial[32];
		INT handle_dbz_cam_icon[3];
		INT handle_dbz_cam_needle[3];
		INT handle_dbz_cam_text[6];
		INT handle_dbz_cam_gps[9];
		INT handle_dbz_cam_sys[5];


	// ---- logical id of [atlas] pano texture
		INT pano_texture_lid              = 0;


	// ---- frames per second
		INT fps_current = 0;


	// ---- rotation
		#if VS_LIDAR
		BOOL  lidar_pan = FALSE;
		FLOAT lidar_pan_incr = 0.1f;
		#endif


};


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Context class ----------

class Model::PI_Context
{

public:

	PI_Context() {}
	~PI_Context() {}

	BOOL pano_mode = FALSE;

};



