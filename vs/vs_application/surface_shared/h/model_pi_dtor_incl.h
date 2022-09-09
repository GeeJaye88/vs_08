
// ======== OBJECTS ========

	// ---- managed locally

		if ( config_files != NULL )
		{
			delete config_files;
			config_files = NULL;
		}

		for (UINT i = 0; i < num_app_camera; i++)
		{
			if ( app_kamera[i] != NULL )
			{
				delete app_kamera[i];
				app_kamera[i] = NULL;
			}
		}
		if ( matrix_stack!= NULL )
		{
			delete matrix_stack;
			matrix_stack = NULL;
		}

		if ( obj_config!= NULL )
		{
			delete obj_config;
			obj_config = NULL;
		}

		if ( img_props!= NULL )
		{
			delete img_props;
			img_props = NULL;
		}


// ======== TABBED INTERFACES ========

	// ---- managed locally

		if ( tint_setng != NULL ) delete tint_setng;
		handle_dspl_tint_head = 0;
		handle_dspl_tint_body = 0;

		if ( kpad_setng != NULL ) delete kpad_setng;
		handle_dspl_kpad_head = 0;
		handle_dspl_kpad_body = 0;
