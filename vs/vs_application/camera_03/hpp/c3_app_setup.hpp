
// ---------- SetupProperties ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SetupProperties()
{

	// ---- local
		INT result = 0;
		pi_props->sys_context = pi_props->app_dependency->GetSysContext();
		pi_props->sys_output = pi_props->app_dependency->GetSysOutput();


	// ---- begin
		pi_props->sys_output->MessageSetup("Begin", "SetupProperties");


	// ---- MVC
		{
			// allocate, construct and setup application view object
			pi_props->app_view = new vs_system::AppView();
			result = pi_props->app_view->Setup(pi_props->app_dependency);

			// allocate and construct other MVC objects
			pi_props->control = new review_02::Control();
			pi_props->model   = new review_02::Model();

		}


	// ---- M
		vs_system::ObjConfig *obj_config = pi_props->model->GetObjConfig();


	// ---- application and CV required config filenames
		review_02::ConfigFiles *config_files = NULL;
		result = pi_props->model->GetConfigFiles(&config_files);
		{

			// setup config files
			result = config_files->Setup(
					pi_props->app_dependency,
					pi_props->sys_context->GetChar(vs_system::SysContext::CONFIG_NAME),
					pi_props->camera_mode
				);
			if (result == VS_ERROR)
				return VS_ERROR;

			// get config path filenames
			config_files->GetApplicationConfigHdlFilename(pi_props->app_setup_config_paf);
			config_files->GetPropsConfigHdlFilename(pi_props->cam_props_config_paf);
			config_files->GetImageConfigHdlFilename(pi_props->img_props_config_paf);
			config_files->GetObjectConfigHdlFilename(pi_props->obj_config_paf);
			config_files->GetImageConfigJpgFilename(pi_props->ext_tex_map_image_paf);
		}


	// ---- setup and configure application properties 
		{
			result = pi_props->Setup();
			result = pi_props->Configure();
		}


	// ---- re-setup and config with correct camera mode
		{
			result = config_files->Setup(
					pi_props->app_dependency,
					pi_props->sys_context->GetChar(vs_system::SysContext::CONFIG_NAME),
					pi_props->camera_mode
				);
			config_files->GetPropsConfigHdlFilename(pi_props->cam_props_config_paf);
		}


	// ---- set control callbacks to application setup methods
		/*
		{
			result = pi_props->control->SetCallbackToSetupGraphics(SetupGraphics);
			result = pi_props->control->SetCallbackToCleanupGraphics(CleanupGraphics);
		}
		*/

	//  ---- camera
		{
			result = pi_props->model->SetAppConfigPaf(
					pi_props->app_setup_config_paf
				);
		}

	//  ---- camera
		{
			// read camera properties
			result = pi_props->model->ReadCamProps(
							pi_props->cam_props_config_paf
						);

			// write to log
			pi_props->sys_output->WriteFile(
							pi_props->cam_props_config_paf
						);

			//result = pi_props->model->TestSaveCamProps();

			// read pano config BUT don't write to log UNTIL
			// after (if) pic overwrite
			result = obj_config->ReadPanoConfig(
							pi_props->obj_config_paf
						);
		}


	//  ---- image
		{
			// read image properties AND then then write to log
			result = pi_props->model->ReadImgProps(pi_props->img_props_config_paf);
			pi_props->sys_output->WriteFile(pi_props->img_props_config_paf);
		}


	// ---- lidar
		{
			pi_props->lidar_ouster->SetBeams(64);
			pi_props->lidar_ouster->SetColumns(1024);

		}


	//  ---- if test pic overwrite config pano parameters

		#if defined(VS_CAM_PIC_BUFFER)


		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
		{
			for (int i = 0; i < vs_system::ObjConfig::MAX_PANO_PTF_LUT; i++)
			{
				result = obj_config->SetPanoPTFById(i, 0, 0, 1);
				result = obj_config->SetPanoAngles(-90, 90);
				result = obj_config->SetPanoOffset(0, 0);
				result = obj_config->SetPanoDefault();
			}
		}
		#else
		{
			for (int i = 0; i < vs_system::ObjConfig::MAX_PANO_PTF_LUT; i++)
			{
				result = obj_config->SetPanoPTFById(i, (FLOAT)i*60.0f, 0, 40.0f);
				result = obj_config->SetPanoRadii(0.5f, 1.0f);
				result = obj_config->SetPanoExposureAoi(0.65f, 0.92f);
				result = obj_config->SetPanoAngles(-45.0f, 8.0f);
				result = obj_config->SetPanoOffset(0.0f, 0.0f);
				result = obj_config->SetPanoDefault();
				/* ---- old
					result = obj_config->SetPanoPTFById(i, (FLOAT)i*60.0f, 0, 40.0f);
					result = obj_config->SetPanoRadii(0.3f, 1.0f);
					result = obj_config->SetPanoExposureAoi(0.65f, 0.92f);
					result = obj_config->SetPanoAngles(-54.0f, 54.0f);
					result = obj_config->SetPanoOffset(0.0f, 0.0f);
					result = obj_config->SetPanoDefault();
				*/
			}
		}
		#endif

		#endif


	//  ---- log config pano parameters file
		{
			pi_props->sys_output->WriteFile(pi_props->obj_config_paf);
		}


	// ---- MC: setup and configure
		//
		// note: must preset runtime status to provide for the init of main buttons 
		//
		{
			result = pi_props->control->Setup(
					pi_props->app_dependency,
					pi_props->app_view,
					pi_props->model
				);

			result = pi_props->model->SetArchiveSaveStatus (pi_props->archive_save_on_at_runtime);
			result = pi_props->model->SetStreamingStatus   (pi_props->stream_on_at_runtime);
			result = pi_props->model->SetStreamingAndOnvifStatus(pi_props->stream_and_onvif_on_at_runtime);

			result = pi_props->model->Setup(pi_props->app_dependency);

		}


	// ---- no "home" frame set
	BOOL home_frame_set = FALSE;
	{

		// ----- "monitor" -----
			vs_system::AppFrame *frame_monitor  = pi_props->app_view->GetFrame("Monitor");
			vs_system::AppFrame *frame_settings = pi_props->app_view->GetFrame("Settings");
			vs_system::AppFrame *frame_record   = pi_props->app_view->GetFrame("Dashboard");


		#ifdef VS_CAMERA_03_STREAMING


		// ---- SET streaming & remote control ON ?
			if ( pi_props->stream_on_at_runtime || pi_props->stream_and_onvif_on_at_runtime )
			{

				// home
					frame_monitor->SetHome(FALSE);
					frame_record->SetHome(FALSE);
		
				// flag
					pi_props->archive_save_on_at_runtime = FALSE;
					pi_props->streaming = TRUE;

				// flip control status
					result = pi_props->control->SetStreamStatus(TRUE);
					if ( pi_props->stream_and_onvif_on_at_runtime )
						result = pi_props->control->SetRemoteControlStatus(TRUE);

				// flip model status
					if ( pi_props->stream_and_onvif_on_at_runtime )
						result = pi_props->model->SetRemoteControlStatus(TRUE);

				// select "stream 1"
					result = pi_props->model->SetStreamElementDisplay(STREAM_1);

				// set play onvif, and "stream 1" icons ON
					result = pi_props->model->UpdateStreamDisplayList(
							STREAM_BUTTON_GROUP_ID,
							STREAM_PLAY,
							TRUE
						);
					if (pi_props->stream_and_onvif_on_at_runtime)
					{
						result = pi_props->model->UpdateStreamDisplayList(
								STREAM_BUTTON_GROUP_ID,
								STREAM_ONVIF_VPTZ,
								TRUE
							);
					}
					result = pi_props->model->UpdateStreamDisplayList(
							STREAM_BUTTON_GROUP_ID,
							STREAM_1,
							TRUE
						);

			}

		// ---- SET stream
			else
			{

				// select "stream 2"
					result = pi_props->model->SetStreamElementDisplay(STREAM_2);

				// set "stream 2" icon ON
					result = pi_props->model->UpdateStreamDisplayList(
							STREAM_BUTTON_GROUP_ID,
							STREAM_2,
							TRUE
						);

			}


		#endif

		// ---- home
		{

			// ---- no "Monitor"
				pi_props->monitor_on = FALSE;


			// --- old buttons lash up ------------------------------------------------------------
				auto select_camera_frame_button = [](INT gid, INT id)
					{
						pi_props->model->SetupCameraFramesButtonsList(
								0,  // group id
								0,  // element id
								3,  // switched_on
								0   // value (OFF)
							);
						pi_props->model->SetupCameraFramesButtonsList(
								gid,
								id, // element frame id
								3,  // switched_on
								1   // value (ON)
							);
					};


			// ----- IF "archive" ON -- THEN select "Dashboard" home frame
				if ( pi_props->archive_save_on_at_runtime )
				{
					frame_monitor->SetHome(FALSE);
					frame_settings->SetHome(FALSE);
					frame_record->SetHome(TRUE);
				}


			// ----- IF "Monitor" ON -- THEN --
				if ( frame_monitor != NULL && frame_monitor->GetHome() )
				{
					result = pi_props->app_view->SelectFrame("Monitor");
					pi_props->monitor_on = TRUE;
					home_frame_set = TRUE;
					frame_settings->SetHome(FALSE);
					frame_record->SetHome(FALSE);
				}


			// ----- ELSE IF "Settings" ON -- THEN --
				else if ( frame_settings != NULL && frame_settings->GetHome() )
				{
					result = pi_props->app_view->SelectFrame("Settings");
					home_frame_set = TRUE;
					select_camera_frame_button(FRAMES_BUTTON_GROUP_ID, 7);
					frame_monitor->SetHome(FALSE);
					frame_record->SetHome(FALSE);
				}


			// ----- ELSE IF "Dashboard" ON -- THEN --
				else if ( frame_record != NULL &&  frame_record->GetHome() )
				{
					result = pi_props->app_view->SelectFrame("Dashboard");
					home_frame_set = TRUE;
					select_camera_frame_button(FRAMES_BUTTON_GROUP_ID, 8);
					pi_props->control->SetDashboardOnly(TRUE);
					frame_monitor->SetHome(FALSE);
					frame_settings->SetHome(FALSE);
				}


			// ----- ELSE set "home" to frame # containing panel #
				else
				{


					// ---- set "HOME" frame

						result = pi_props->model->Set_HomeFrameSelected(2);
						UINT home_frame_selected = pi_props->model->Get_HomeFrameSelected();

						vs_system::AppFrame *frame = pi_props->app_view->GetFirstFrame();
						UINT frame_index = 1;
						while (frame)
						{
							if ( frame_index == home_frame_selected )
							{
								frame->SetHome(TRUE);
								pi_props->app_view->SelectFrame(frame);
								home_frame_set = TRUE;
							}
							else
							{
								frame->SetHome(FALSE);
							}
							frame_index++;
							frame = frame->GetNext();
						}

				}
		}


		// ---- IF -- archive ON -- THEN --
			if ( pi_props->archive_save_on_at_runtime )
			{
				pi_props->model->SetupCameraFramesButtonsList(
						CAPTURE_BUTTON_GROUP_ID,
						CAPTURE_SAVE, // element id
						3, // property
						1  // value (ON)
					);
				pi_props->archive_save = FALSE; // TBD - no need for this property!
				result = pi_props->control->SetArchiveSave(TRUE);

				// ---- scavenger
					pi_props->sys_output->Message("Scavenge: ON");
					pi_props->scavenging = TRUE;
			}

	}


	// ----- report no "home" frame
		if ( !home_frame_set )
		{
			;
		}


	// ---- initialise "now" in seconds
		{
			#ifdef VS_CAM_IDS_VIDEO_SDK
			pi_props->seconds_elapsed_to_now = 0;
			#endif

			#if defined(VS_CAM_JPLY_DVP_SDK) || defined(VS_CAM_PIC_BUFFER) 
			pi_props->seconds_elapsed_to_now = (LONG)pi_props->sys_context->GetTimeElapsed() / 1000;
			#endif

			// initialise # seconds that have been recorded
			pi_props->seconds_elapsed_recording = 0;
		}


	//  ---- log config pano parameters
		{
			pi_props->model->ReportObjConfigPanoConfig();
		}


	// ---- end
		pi_props->sys_output->MessageSetup("End", "SetupProperties");


	return VS_OK;
}


// ---------- SetupCamera ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SetupCamera() 
{

	// ---- local ----
	INT result = 0;

	pi_props->sys_output->MessageSetup("Begin", "SetupCamera");

	{

		// ---- IF live at runtime THEN start
			if ( pi_props->camera_live_at_runtime )
			{
				pi_props->camera_live = TRUE;
			}


		// ---- required: initialise these before camera setup...
			pi_camera->app_dependency             = pi_props->app_dependency;
			pi_camera->sys_output                 = pi_props->sys_output;
			pi_camera->model                      = pi_props->model;
			pi_camera->callback_on_capture_image  = CallbackOnCaptureImage;
		

		// ---- required: initialise these flags, and camera properties path/filename
			pi_camera->camera_mode                = pi_props->camera_mode;
			pi_camera->camera_use_ini_file        = pi_props->camera_use_ini_file;
			pi_camera->camera_use_hdl_file        = pi_props->camera_use_hdl_file;
			strcpy(pi_camera->cam_props_config_paf, pi_props->cam_props_config_paf);


		// ---- allocate, construct and then initialise camera ----
		//
		//      - and intialise ObjConfig Pano Aoi parameters
		//
		result = pi_camera->Setup();
		if ( result != VS_OK ) 
		{
			pi_props->sys_output->Message("     : +++ Camera Setup failed +++.");
			return result;
		}

	}

	pi_props->sys_output->MessageSetup("End", "SetupCamera");

	return result;
}


// ---------- SetupArchive ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SetupArchive()
{

	// ---- local ----
		INT result = 0;


	// ---- begin
		pi_props->sys_output->MessageSetup("Begin", "SetupArchive");

	{

		// ---- local
			pi_props->img_arkive = new vs_system::ImgArkive();


		// ---- init image buffer ( & quickly grab memory )
			INT width = 0, height = 0;
			pi_camera->GetDimensions(&width, &height);
			pi_props->dst_img_buffer->Initialise(width, height, 4);
			pi_props->dst_img_buffer->CreateBuffer();


		// ---- init csv buffer
			#if VS_LIDAR
			UINT csv_columns = 5;
			UINT csv_rows_per_record = pi_props->lidar_ouster->GetBeams();
			UINT csv_rows    = csv_rows_per_record * pi_props->lidar_ouster->GetColumns();
			#endif

			pi_props->img_arkive->Setup_Buffers(

					#if VS_LIDAR
					csv_columns,
					csv_rows,
					csv_rows_per_record,
					#endif

					width,
					height,
					3
				);



		// ---- IF archive folder NOT NULL THEN use this,...
			CHAR paf[VS_MAXCHARPATHLEN];
			if (pi_props->archive_folder[0] != 0)
			{
				sprintf(paf, "%s", pi_props->archive_folder);
			}
			// ELSE use documents/observant folder
			else
			{
				sprintf(
					paf,
					"%sDocuments/Observant/",
					pi_props->sys_context->GetChar(pi_props->sys_context->PROJECT_FOLDER_PATH)
				);
			}


		// ---- set archive folder
			result = pi_props->img_arkive->SetFolderPath(paf);


		// ---- set scavenger folder
			result = SetScavengerLocation(paf);


		// ---- set archive parameters
			result = pi_props->img_arkive->SetMode(pi_props->archive_mode);
			result = pi_props->img_arkive->SetQuality(pi_props->archive_jpg_compression);
			result = pi_props->img_arkive->SetStackExtra(pi_props->archive_extra_stack);
			result = pi_props->img_arkive->SetConfigPath(pi_props->obj_config_paf);


		// ---- setup archive
			result = pi_props->img_arkive->WriteSetup();
			if (result == VS_ERROR)
			{
				pi_props->sys_output->Message("     : +++ Image Archive Setup failed +++.");
				return VS_ERROR;
			}

	}


	// ---- end
		pi_props->sys_output->MessageSetup("End", "SetupArchive");


	return VS_OK;
}


// ---------- SetupImgLutProcess ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SetupImgLutProcess()
{

	// ---- local ----
		INT result = 0;


	// ---- begin
		pi_props->sys_output->MessageSetup("Begin", "SetupImgLutProcess");


	// ---- local
		pi_props->img_col_process = new vs_system::ImgColProcess();


	// ---- end
		pi_props->sys_output->MessageSetup("End", "SetupImgLutProcess");


	return VS_OK;
}


// ---------- SetupScanFolder ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/ 
INT application::SetupScanFolder()
{

	pi_props->sys_output->MessageSetup("Begin", "SetupScanFolder");

	{
		CHAR archive_folder_path[VS_MAXCHARPATHLEN];
		INT result = pi_props->img_arkive->GetSubFolderPath(archive_folder_path);
	}

	pi_props->sys_output->MessageSetup("End", "SetupScanFolder");

	return VS_OK;
}


// ---------- ReportAppConfig ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\n 2017
*/
INT application::ReportAppConfig()
{

	pi_props->sys_output->MessageSetup("Begin", "ReportAppConfig");


	// ---- local ----
	INT result = 0;
	CHAR message[VS_MAXCHARLEN];
	vs_system::SysOutput *sys_output = pi_props->sys_output;


	// ---- CAMERA ----
	{

		sprintf(
				message,
				"Camera: %d size mode.",
				(INT)pi_props->camera_mode
			);
			sys_output->Message(message);

		sprintf(
				message,
				"Camera: %s sensor config file.",
				pi_props->camera_use_ini_file ?
				"USING" :
				"NOT USING"
			);
			sys_output->Message(message);

		sprintf(
				message,
				"        %s camera config file.",
				pi_props->camera_use_hdl_file ?
				"USING" :
				"NOT USING"
			);
			sys_output->Message(message);
	}


	// ---- FOCUS ----
	{
		sprintf(message, "Focus: %s.", pi_props->focus_on ? "ON" : "OFF");
		sys_output->Message(message);
	}


	// ---- MONITOR ----
	if ( pi_props->monitor_on )
	{
		sprintf(message, "Monitor: Threshold is %d.", (INT)pi_props->monitor_threshold);
		sys_output->Message(message);
		sprintf(message, "         Restart duration is %d.", (INT)pi_props->monitor_restart);
		sys_output->Message(message);
	}


	// ---- ARCHIVE ----
	{
		sprintf(message, "Archive: Compression is %d", pi_props->img_arkive->GetQuality());
		sys_output->Message(message);
		sprintf(message, "         Max Stack is %d", pi_props->img_arkive->GetMaxStack());
		sys_output->Message(message);
		sprintf(message, "         Mode is %d", pi_props->img_arkive->GetMode());
		sys_output->Message(message);
		sprintf(message, "         Save is %s", pi_props->img_arkive->GetSave() ? "ON" : "OFF");
		sys_output->Message(message);
	}


	// ---- ARCHIVE MODE ----
	{
		sys_output->Message("Archive Mode 1 - use SINGLE archive folder,");
		sys_output->Message("        Mode 2 - use DAILY archive folder, and");
		sys_output->Message("        Mode 3 - use NEW archive folder.");
		sys_output->Message("        Note: Default mode is 3.");
	}


	// ---- SCAVENGER ----
	{
		sprintf(message, "Scavenger: Is %s", pi_props->scavenging ? "ON" : "OFF");
		sys_output->Message(message);
		sprintf(message, "         Minutes Kept is %d", (INT)pi_props->scavenger_minutes_max);
		sys_output->Message(message);
		sprintf(message, "         Seconds Wait %d", (INT)pi_props->scavenger_seconds_pause);
		sys_output->Message(message);
	}


	// ---- GPS ----
	{
		sprintf(message, "GPS: Port is %s", pi_props->s_gps_port.c_str());
		sys_output->Message(message);
		sprintf(message, "     Delay is %d", pi_props->gps_delay);
		sys_output->Message(message);
	}


	// ---- STREAM ----
	{
		sprintf(message, "Stream: Logical ID is %d", pi_props->stream_logical_id);
		sys_output->Message(message);
		sprintf(message, "        Image width is %d", pi_props->stream_image_width);
		sys_output->Message(message);
		sprintf(message, "        Image height is %d", pi_props->stream_image_height);
		sys_output->Message(message);
		sprintf(message, "        Frame modulus is %d", pi_props->stream_frame_modulus);
		sys_output->Message(message);
	}


	// ---- STREAM FRAME ----
	{
		sys_output->Message("Note: Frame modulus == 0 - Stream ONLY newly captured images,");
		sys_output->Message("                       1 - Stream every frame, and");
		sys_output->Message("                       2+ -Stream when current frame % modulus value == 1.");
		sys_output->Message("      Default frame modulus is 0.");
	}


	// ---- ONVIF ----
	{
		sprintf(message, "ONVIF: Supplied IP Address is %s", pi_props->onvif_ip_address);
		sys_output->Message(message);
		sprintf(message, "       SuppliedPort Number is %s", pi_props->onvif_port_number);
		sys_output->Message(message);
		sprintf(message, "       PTZ RM increment is %6.4f", pi_props->onvif_rm_incr);
		sys_output->Message(message);
		sprintf(message, "       PTZ CM increment is %6.4f", pi_props->onvif_cm_incr);
		sys_output->Message(message);
	}


	pi_props->sys_output->MessageSetup("End", "ReportAppConfig");


	return VS_OK;
}