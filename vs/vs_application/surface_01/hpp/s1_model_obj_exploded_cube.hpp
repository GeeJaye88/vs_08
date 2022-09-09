////////////////////////////////////////////////////////////////////////////////

// ---------- s1_model_obj_exploded_cube.hpp ----------
/*!
\file s1_model_obj_exploded_cube.hpp
\brief Implementation of the Object ExpolodedCube methods
\author Gareth Edwards
*/


// ========================================================================== //


INT Model::InitExplodedCube()
{
	return VS_OK;
}


// ========================================================================== //


INT Model::InitExplodedCubeScene()
{

	// ---- local
		vs_system::GfxInterface *gfx = pi_model->gfx;
		INT hr = 0;


	// ---- ambient
		VsColour amb_col = { 0.5f, 0.5f, 0.5f, 1.0f };
		vs_system::VsAmbient amb;
		hr = amb.SetColour(amb_col);
		hr = gfx->SetAmbient(&amb);


	// ---- directional main light
		VsColour sun_amb = {  0.2f, 0.2f, 0.2f, 1.0f };
		VsColour sun_dif = {  1.0f, 1.0f, 1.0f, 1.0f };
		VsColour sun_spc = {  1.0f, 1.0f, 1.0f, 1.0f };
		VsVector3 sun_dir = {  1.0f,-1.0f, 1.0f };
		vs_system::VsLight sun;
		hr = sun.SetType(vs_system::VS_LIGHT_DIRECTIONAL);
		hr = sun.SetAmbient(sun_amb);
		hr = sun.SetDiffuse(sun_dif);
		hr = sun.SetSpecular(sun_spc);
		hr = sun.SetDirection(sun_dir);
		hr = gfx->SetLight(0, &sun);
		hr = gfx->SetLightEnabled(0, TRUE);


	// ---- directional fill from right to back
		VsColour lgt_amb = { 0.2f, 0.2f, 0.2f, 1.0f };
		VsColour lgt_dif = { 0.5f, 0.5f, 0.5f, 1.0f };
		VsColour lgt_spc = { 1.0f, 1.0f, 1.0f, 1.0f };
		VsVector3 lgt_dir = {-1.0f,-1.0f,-1.0f };
		vs_system::VsLight lgt;
		hr = lgt.SetType(vs_system::VS_LIGHT_DIRECTIONAL);
		hr = lgt.SetAmbient(lgt_amb);
		hr = lgt.SetDiffuse(lgt_dif);
		hr = lgt.SetSpecular(lgt_spc);
		hr = lgt.SetDirection(lgt_dir);
		hr = gfx->SetLight(1, &lgt);
		hr = gfx->SetLightEnabled(1, FALSE);


	// ---- point radience from origin
		VsColour top_amb = { 0.2f, 0.2f, 0.2f, 1.0f };
		VsColour top_dif = { 0.5f, 0.5f, 0.5f, 1.0f };
		VsColour top_spc = { 1.0f, 1.0f, 1.0f, 1.0f };
		VsVector3 top_pos = { 0.0f, 0.0f, 0.0f };
		vs_system::VsLight top;
		hr = top.SetType(vs_system::VS_LIGHT_POINT);
		hr = top.SetAmbient(top_amb);
		hr = top.SetDiffuse(top_dif);
		hr = top.SetSpecular(top_spc);
		hr = top.SetPosition(top_pos);
		hr = top.SetRange(1000);
		hr = top.SetAttLinear(0.01f);
		hr = gfx->SetLight(2, &top);
		hr = gfx->SetLightEnabled(2, TRUE);


	// ---- spot left to front
		VsColour spot_amb = { 0.2f, 0.2f, 0.2f, 1.0f };
		VsColour spot_dif = { 1.0f, 1.0f, 1.0f, 1.0f };
		VsColour spot_spc = { 1.0f, 1.0f, 1.0f, 1.0f };
		VsVector3 spot_pos = {-800.0f, 800.0f,-800.0f };
		VsVector3 spot_dir = { 1.0f,-1.0f, 1.0f };
		vs_system::VsLight spot;
		hr = spot.SetType(vs_system::VS_LIGHT_SPOT);
		hr = spot.SetAmbient(spot_amb);
		hr = spot.SetDiffuse(spot_dif);
		hr = spot.SetSpecular(spot_spc);
		hr = spot.SetPosition(spot_pos);
		hr = spot.SetDirection(spot_dir);
		hr = spot.SetRange(2000);
		hr = spot.SetAttConstant(0);
		hr = spot.SetAttLinear(0.001f);
		hr = spot.SetAttQuadratic(0);
		hr = spot.SetOuter(20 * (float)VS_RADIANS_PER_DEGREE);
		hr = spot.SetInner(10* (float)VS_RADIANS_PER_DEGREE);
		hr = spot.SetFalloff(1.0f);
		hr = gfx->SetLight(3, &spot);
		hr = gfx->SetLightEnabled(3, TRUE);


	// ---- materials for each cube face
		VsColour c[12] =
		{
			{ 1.0f, 1.0f, 0.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f, 1.0f },
			{ 1.0f, 1.0f, 0.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 1.0f, 1.0f },
			{ 0.0f, 0.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f, 1.0f },
		};
		for (int i = 0; i < 12; i++)
		{
			VsColour obj_amb = { 0.2f, 0.2f, 0.2f, 1.0f };
			VsColour obj_dif = { c[i].red, c[i].green, c[i].blue, 1.0f };
			VsColour obj_spc = { 1.0f, 1.0f, 1.0f, 1.0f };
			vs_system::VsMaterial obj;
			hr = obj.SetAmbient(obj_amb);
			hr = obj.SetDiffuse(obj_dif);
			hr = obj.SetSpecular(obj_spc);
			hr = obj.SetPower(40);
			hr = gfx->SetMaterial(i, &obj);
		}


	// ---- kamera
		if ( pi_model->runtime )
		{
			for (INT i = 0; i < pi_model->num_app_camera; i++)
			{
				vs_system::AppKamera *kamera = pi_model->app_kamera[i];

				// ---- target
					kamera->SetTargetIncr(0.1f);
					kamera->SetTargetZoom(200.0f);
					kamera->SetTargetZoomMinimum(50);
					kamera->SetTargetZoomMaximum(250);
					kamera->SetTargetChangeZoomPower(1.5f);
					kamera->SetTargetChangeZoomDenom(20.0f);
					kamera->SetTargetChangeSpeed(2);

				// ---- increment
					kamera->SetScalarIncr(0.5f);
					kamera->SetScalarRotation(TRUE);
					kamera->SetScalarMode(TRUE);
					kamera->SetPositionIncr(0.5f);
					kamera->SetPanTiltIncr(0.01f);
					kamera->SetRotateIncr(0.02f);
					kamera->SetTranslateIncr(1.0f);
					kamera->SetZoomIncr(1.0f);

				// ---- set view
					kamera->SetPanTilt(0, 0);
					kamera->SetPosition(0, 0, -400);

				// ---- default & state
					kamera->SaveDefaults();
					kamera->SaveState();

			}

			INT result = SetMyKamera(pi_model->app_kamera_index);

		}


	return VS_OK;
}


// ========================================================================== //


INT Model::InitExplodedCubeObject()
{

	// ---- create display list
		vs_system::GfxInterface *gfx = pi_model->gfx;


	// ---- create display list ?
		if ( pi_model->runtime )
		{
			pi_model->handle_sphere_dsplst = gfx->AddDisplayList("Object", 1, 1);
		}


	// ---- for each face
		for (int i = 0; i < 12; i ++)
		{

			// ---- create quad ?
				if ( pi_model->runtime )
				{
					pi_model->handle_sphere[i] = gfx->AddElement(
							pi_model->handle_sphere_dsplst,
							"",
							1,
							1,
							pi_model->obj_config->Callback_BoxFace_Configure
						);
				}


			// ---- context
				vs_system::ElmContext *gfx_context = NULL;
				INT hr = gfx->GetElementContext(pi_model->handle_sphere[i], &gfx_context);


			// ---- force geometry setup
				gfx_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
				gfx_context->SetConfigured(FALSE);


			// ---- init sphere parameters
				FLOAT *param = gfx_context->GetFloatParameter();
				param[vs_system::ObjConfig::BOX_FACE_MODE]    = (FLOAT)i;
				param[vs_system::ObjConfig::BOX_FACE_TYPE]    = 2;
				param[vs_system::ObjConfig::BOX_FACE_AP_REF] =
					#if defined(S1_EXPLODED_CUBE)
						(FLOAT)pi_model->user_atlas_page_logical_id;
					#else
						0;
					#endif
				param[vs_system::ObjConfig::BOX_FACE_RED]     = 1;
				param[vs_system::ObjConfig::BOX_FACE_GREEN]   = 1;
				param[vs_system::ObjConfig::BOX_FACE_BLUE]    = 1;
				param[vs_system::ObjConfig::BOX_FACE_SCALE]   = 50.0f;
				param[vs_system::ObjConfig::BOX_FACE_EXPLODE] = 0.5f;
				param[vs_system::ObjConfig::BOX_FACE_WIDTH]   = 1.0f;
				param[vs_system::ObjConfig::BOX_FACE_HEIGHT]  = 1.0f;
				param[vs_system::ObjConfig::BOX_FACE_DEPTH]   = 1.0f;

			// ---- lighting and materials
				hr = gfx_context->SetLit(TRUE);
				hr = gfx_context->SetMaterialIndex(i);

				hr = gfx_context->SetCullMode(vs_system::ElmContext::CULL_CCW);

		}


	// ---- done
		pi_model->runtime = FALSE;

	return VS_OK;
}


// ========================================================================== //


INT Model::SetupExplodedCubeDisplay()
{

	// ---- kamera
		INT mki = pi_model->app_kamera_index;
		vs_system::AppKamera *kamera = pi_model->app_kamera[mki];


	// ---- setup
		{
			// ---- local
				INT result = 0;
				VsVector3 translate;
				FLOAT rx, ry, rz;

			// ---- setup matrix stack & transform values
				pi_model->recursive_depth_count = 0;
				pi_model->matrix_stack->Initialise();
				kamera->GetTranslate(&translate);
				kamera->GetRotate(&rx, &ry, &rz);

			// ---- push, transform, setup & pop
				pi_model->matrix_stack->Push();

				pi_model->matrix_stack->Rotate(rz, 'Z');
				pi_model->matrix_stack->Rotate(rx, 'X');
				pi_model->matrix_stack->Rotate(ry, 'Y');
				pi_model->matrix_stack->Translate(
								translate.x,
								translate.y,
								translate.z
							);

				pi_model->SetupExplodedCubeDisplay();
				pi_model->matrix_stack->Pop();

		}


	// ---- kamera
		if ( kamera->HasBeenUpdated() )
		{
			INT result = SetMyKamera(mki);
			kamera->SetHasBeenUpdated(FALSE);
		}
	
	return VS_OK;
}


// ========================================================================== //


INT Model::PI_Model::SetupExplodedCubeDisplay()
{

	for (int i = 0; i < 12; i++)
	{

		// ---- push
			matrix_stack->Push();


		// ---- time
			DOUBLE time_elapsed_ms =
				sys_context->GetDouble(vs_system::SysContext::TIME_MS_ELAPSED) +
					system_param->GetMsOffset();


		// ---- rotation
			DOUBLE rot_degrees = time_elapsed_ms / 100;


		// ---- matrix stuff
			VsMatrix *m = &matrix_stack->stack[matrix_stack->index];
			matrix_stack->Rotate(    (FLOAT)-rot_degrees, 'Y');
			matrix_stack->Translate( (FLOAT) 0, 0, 0);
			matrix_stack->Rotate(    (FLOAT) 0.0f, 'Y');


		// ---- sphere - world matrix
			vs_system::ElmContext *elm_context = NULL;
			INT hr = gfx->GetElementContext(handle_sphere[i], &elm_context);
			elm_context->SetUseMatrix(TRUE);
			elm_context->SetMatrix(m);
			elm_context->SetViewMatrix(gfx_view_matrix_index);


		// ---- pop
			matrix_stack->Pop();
		
	}

	
	// ---- locate
		UINT target_planet_index = GetTargetObjectId();
		if ( target_planet_index != SystemParam::CameraTarget::NONE )
		{
			VsMatrix  *sys_obj_matrix = &matrix_stack->stack[matrix_stack->index];
			VsVector3 sys_obj_vector = { sys_obj_matrix->m[3][0] , sys_obj_matrix->m[3][1] , sys_obj_matrix->m[3][2] };
			app_kamera[app_kamera_index]->SetTargetPosition(&sys_obj_vector, FALSE);
		}


	return VS_OK;
}


// ========================================================================== //
