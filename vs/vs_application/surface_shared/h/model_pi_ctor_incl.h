
// ======== OBJECTS ========

	// ---- allocate and construct object

		config_files      = new vs_system::ConfigFiles();
		for (UINT i = 0; i < num_app_camera; i++)
		{
			app_kamera[i] = new vs_system::AppKamera();
		}
		matrix_stack      = new vs_system::VsMatrixStack();
		obj_config        = new vs_system::ObjConfig();
		img_props         = new vs_system::ImgProps();


// ======== TABBED INTERFACES ========

	// ---- Tint - required
		handle_dspl_tint_head = 0;
		handle_dspl_tint_body = 0;