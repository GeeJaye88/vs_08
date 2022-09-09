////////////////////////////////////////////////////////////////////////////////

// ---------- s3_model_project_framework.hpp ----------
/*!
\file s3_model_project_framework.hpp
\brief Implementation of the project methods
\author Gareth Edwards
*/


////////////////////////////////////////////////////////////////////////////////


// ---------- ProjectSetup ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::ProjectSetup()
{

	// ---- local
		INT result = VS_OK;


	// ---- allocate, construct, setup and configure
		pi_model->surface_03 = new model::Surface_03();
		model::Surface_03 *surface_03 = pi_model->surface_03;
		result = surface_03->Setup(pi_model->app_dependency);


	// ---- set object config
		result = surface_03->SetObjConfig(pi_model->obj_config);
		result = surface_03->SetImgProps(pi_model->img_props);


	// ---- set project dependant kamera stuff
		FLOAT acquire_width, acquire_height, acquire_depth;
		surface_03->GetAcquireSurfaceDimensions(
				&acquire_width,
				&acquire_height,
				&acquire_depth
			);
		pi_model->kamera_scalar_increment = acquire_depth / 1000.0f;

	return VS_OK;
}


// ---------- ProjectSetupGraphics ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::ProjectSetupGraphics()
{

	// ---- local
		INT result = 0;
		model::Surface_03 *surface_03 = pi_model->surface_03;


	// ---- lights, kamera, action....
		result = surface_03->SetupGraphics(pi_model->app_kamera[0]);
		result = surface_03->SetVisibleObjects(TINT_BODY_ACQUIRE_LID, surface_03);


	// ---- store camera scalar increment
		s_pi_model->kamera_scalar_increment = pi_model->app_kamera[0]->GetScalarIncr();


	// ---- only one scene
		INT handle_scene = 0;
		result = surface_03->GetDisplayListHandle(&handle_scene, "Scene");
		if ( handle_scene != 0 )
		{
			pi_model->handle_scene = handle_scene;
		}


	// ---- runtime ?
		if ( pi_model->runtime )
		{

			// ---- setup acquisition ( s3_tint_1_callback_acquire.hpp )
				result = AcquireSetup();

			// ---- initialise tint selected source surface ( s3_model_img_enhance.hpp )
				result = Tint_Acquire_UpdateModel(
						(INT)this,
						Tint_Acquire_GetElemLidSourceSurf()
					);

			// ---- zap
				pi_model->runtime = FALSE;

		}

	return VS_OK;
}


// ---------- ProjectSetupDisplay ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::ProjectSetupDisplay()
{

	// ---- local
		INT result = 0;
		model::Surface_03 *surface_03 = pi_model->surface_03;


	// ---- shortcut
		vs_system::AppKamera *kamera = pi_model->app_kamera[0];


	// ---- target LoI?
		UINT target_id = pi_model->GetTargetObjectId();
		if ( target_id != CameraTarget::NONE )
		{

			// ---- legit ?
				if ( target_id >= 0 && target_id <= 9 )
				{

					// ---- previous target
						INT  target_id_prev = pi_model->GetTargetObjectIdPrev();
						
					// ---- catch transition too target mode
						if ( target_id_prev == CameraTarget::NONE )
						{
							kamera->SaveState();
							VsVector3 t = { 0, 0, 0 };
							kamera->SetTranslate(&t);
							kamera->SetRotate(0, 0, 0);
						}

					// ---- catch runtime target
						BOOL new_target = target_id != target_id_prev;
						new_target = target_id_prev == CameraTarget::NONE_RUNTIME ? FALSE : new_target;

					// ---- new ? - set model target (accessed by kamera)
						if ( new_target )
						{
							pi_model->SetTargetObjectId(target_id);
						}

					// ---- set target loi
						surface_03->SetTargetLoiIndex(target_id);

					// ---- set kamera target
						VsVector3 v;
						surface_03->GetTargetLoi(target_id, &v);
						kamera->SetTargetPosition(&v, new_target);

				}

		}

	// ---- ! target
		else
		{

			// ---- catch transition from target mode
				if ( pi_model->GetTargetObjectIdPrev() != CameraTarget::NONE )
				{
					pi_model->SetTargetObjectIdPrev(CameraTarget::NONE);
					kamera->RestoreState();
				}

		}


	// ---- kamera updated ?
		if ( pi_model->app_kamera[0]->HasBeenUpdated() )
		{
			VsMatrix m;
			pi_model->app_kamera[0]->GetViewMatrix(&m);
			pi_model->gfx->SetViewMatrix(1, &m);
			pi_model->app_kamera[0]->SetHasBeenUpdated(FALSE);
		}


	// ---- time
		DOUBLE time_elapsed_ms = pi_model->sys_context->GetDouble(vs_system::SysContext::TIME_MS_ELAPSED);

		// ---- tick
			INT second  = (INT)(time_elapsed_ms / 1000);
			INT second_last = (INT)(pi_model->time_elapsed_ms_last / 1000);
			if ( second != second_last )
			{
				//CHAR str[128];
				//sprintf(str, "---- fps: %d --- %d %d \n", (INT)pi_model->fps, second, second_last);
				//OutputDebugString(str);
				pi_model->fps = 0;
			}

		pi_model->time_elapsed_ms_last = time_elapsed_ms;
		pi_model->fps++;


	// ---- setup
		result = surface_03->SetupDisplay(pi_model->app_kamera[0]);

	return VS_OK;
}


// ---------- ProjectCleanupGraphics ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::ProjectCleanupGraphics()
{

	// ---- local
		INT result = 0;
		model::Surface_03 *surface_03 = pi_model->surface_03;


	// ---- lights, kamera, action....
		result = surface_03->CleanupGraphics();


	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////

