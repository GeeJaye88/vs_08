
// ---------- SwitchArchiveSave ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SwitchArchiveSave()
{

	// ---- save ON/OFF
		BOOL archive_save = pi_props->control->GetArchiveSave();
		if ( archive_save != pi_props->archive_save )
		{
			pi_props->archive_save = archive_save;

			INT result = pi_props->img_arkive->SetSave(pi_props->archive_save);
			if ( archive_save )
			{
				BOOL updated = FALSE;
				result = pi_props->model->GetObjConfig()->GetPanoUpdated(&updated);
				if ( updated )
				{
					result = pi_props->model->SaveObjectAndCameraConfig();
					result = pi_props->model->GetObjConfig()->SetPanoUpdated(FALSE);
				}
				pi_props->sys_output->Message("Archive: SAVE now ON");



				// ---- whatever... now ON
					if (!pi_props->scavenging)
					{
						INT result = StartScavenger();
						pi_props->sys_output->Message("Scavenge: ON");
					}


			}
			else
			{
				pi_props->sys_output->Message("Archive: SAVE now OFF");
			}
			pi_props->seconds_elapsed_recording = 0;
		}

	return VS_OK;
}


// ---------- SwitchCameraMode ----------
/*!
\brief Switch "live" camera mode ON (Start) or OFF (Stop)
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SwitchCameraMode()
{
	BOOL live = pi_props->control->GetCameraLive();
	if ( live != pi_props->camera_live )
	{
		pi_props->camera_live = live;
		if ( live )
		{
			INT result = StartLiveCamera();
		}
		else
		{
			INT result = pi_camera->Stop();
			pi_props->sys_output->Message("Camera: STOP");
		}
		pi_props->seconds_elapsed_recording = 0;
	}

	return VS_OK;
}


// ---------- SwitchCameraLoopHardStart ----------
/*!
\brief Force a "hard restart" of the camera.
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note

The control::GetCameraLoopHardStart method only returns TRUE whilst
camera is being used in Monitor mode - see MONITOR_BUTTON_GROUP_ID

*/
INT application::SwitchCameraLoopHardStart()
{

	// IF ---- switch loop hard start ---- THEN ---- reset ""from hard start" seconds counter ----
	BOOL loop_hard_start = pi_props->control->GetCameraLoopHardStart();
	if ( loop_hard_start != pi_props->loop_hard_start )
	{
		pi_props->loop_hard_start = loop_hard_start;
		if ( loop_hard_start )
		{
			pi_props->sys_output->Message("Camera: Start LoopHardStart");
			pi_props->loop_hard_start_second_count = timeGetTime() / 1000.0f;
		}
		else
		{
			pi_props->sys_output->Message("Camera: STOP LoopHardStart");
			pi_props->loop_hard_start_second_count = 0;
		}
	}


	// IF ---- loop hard start ---- THEN
	if ( loop_hard_start )
	{

		// IF ---- # of seconds changed ---- THEN
		DOUBLE now_sec = timeGetTime() / 1000.0f;
		if ( now_sec >= pi_props->loop_hard_start_second_count + pi_props->monitor_restart )
		{
			INT result = VS_OK;
			result = pi_camera->Stop();      pi_props->sys_output->Message("Camera: Stop");
			result = pi_camera->Cleanup();   pi_props->sys_output->Message("Camera: Cleanup");
			result = pi_camera->Setup();     pi_props->sys_output->Message("Camera: Setup");
			result = StartLiveCamera();
			pi_props->loop_hard_start_second_count = now_sec;
		}
	}


	return VS_OK;
}


// ---------- SwitchDashboardOnly ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SwitchDashboardOnly()
{
	pi_props->dashboard_only = pi_props->control->GetDashboardOnly();
	return VS_OK;
}
