////////////////////////////////////////////////////////////////////////////////

// ---------- c1_setup.hpp ----------
/*!
\file c1_setup.hpp
\brief Implementation of the Model setup methods
\author Gareth Edwards
*/


INT Model::SetupScene()
{

	// ---- local
		vs_system::GfxInterface *gfx = pi_model->gfx;
		INT hr = 0;


	// ---- ambient
		VsColour amb_col = { 0.1f, 0.1f, 0.1f, 1.0f };
		vs_system::VsAmbient amb;
		hr = amb.SetColour(amb_col);
		hr = gfx->SetAmbient(&amb);


	// ---- directional main light
		VsColour sun_amb = { 0.2f, 0.2f, 0.2f, 1.0f };
		VsColour sun_dif = { 1.0f, 1.0f, 1.0f, 1.0f };
		VsColour sun_spc = { 0.0f, 0.0f, 0.0f, 1.0f };
		VsVector3 sun_dir = {0.0f,-0.3f, -1.0f };
		vs_system::VsLight sun;
		hr = sun.SetType(vs_system::VS_LIGHT_DIRECTIONAL);
		hr = sun.SetAmbient(sun_amb);
		hr = sun.SetDiffuse(sun_dif);
		hr = sun.SetSpecular(sun_spc);
		hr = sun.SetDirection(sun_dir);
		hr = gfx->SetLight(0, &sun);
		hr = gfx->SetLightEnabled(0, TRUE);


	// ---- materials for each cube face
		VsColour c[12] =
		{
			{ 1.0f, 1.0f, 1.0f, 1.0f },
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

	return VS_OK;
}


INT Model::SetupCamera()
{
	
	// ---- local
		vs_system::GfxInterface *gfx = pi_model->gfx;
		FLOAT scene_dimension = pi_model->scene_dimension;
		INT hr = 0;


	// ---- camera
		vs_system::AppKamera *kamera = pi_model->kamera;

	// ---- target
		kamera->SetTargetIncr(0.5f);
		kamera->SetTargetZoom(10.0f);

	// ---- increment
		kamera->SetScalarIncr(0.5f);
		kamera->SetPositionIncr(0.1f);
		kamera->SetPanTiltIncr(0.01f);
		kamera->SetZoomIncr(0.1f);

	// ---- set view
		kamera->SetPanTilt(0, 0);
		kamera->SetPosition(0, 0, scene_dimension);

	// ---- view from....
		FLOAT view_ratio = 0.5;
		VsVector3 p = { 0, scene_dimension * view_ratio/2, -scene_dimension * view_ratio };
		kamera->SetPosition(&p);

	// ---- ....looking towards
		VsVector3 l = { 0.0f, -view_ratio/3, view_ratio };
		kamera->SetLook(&l);

	// ---- default & state
		kamera->SaveDefaults();
		kamera->SaveState();

	// ---- set view matrix
		VsMatrix V;
		kamera->GetViewMatrix(&V);
		gfx->SetViewMatrix(1, &V);

	return VS_OK;
}