
// ---------- StartLiveCamera ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note

Invoked from:
. SetupGraphics(),
. SwitchCameraMode(), and
. SwitchCameraLoopHardStart.

*/
INT application::StartLiveCamera()
{

	if ( pi_props->camera_live )
	{

		// ---- local
			INT result = VS_OK;


		// ---- (re)start 
			pi_camera->Start();
			pi_props->sys_output->Message("Camera: Live Start::=======================================================");


		// ---- horizontal parameter
			INT h_offset = pi_camera->GetHOffset();
			result = pi_props->model->GetObjConfig()->SetPanoCamHOffset(h_offset);


		// ---- exposure
			result = pi_camera->SetExposure();


		// ---- re-initialise # seconds that have been recorded
			pi_props->seconds_elapsed_recording = 0;


		// ---- flag
			pi_props->camera_started = TRUE;

	}

	return VS_OK;
}
