
// ---------- UpdateCameraExposure ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note update camera exposure if flagged

Call tree:

	INT application::Display()

		----> INT application::UpdateCameraExposure()

			----> INT PI_Camera::SetExposure()

				----> INT CamIds::SetExposure()

*/
INT application::UpdateCameraExposure()
{
	if ( pi_props->control->GetCameraExposureMustBeUpdated() )
	{
		pi_camera->SetExposure();
	}

	return VS_OK;
}


// ---------- UpdateCameraHOffset ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::UpdateCameraHOffset()
{
	INT result = VS_OK;
	BOOL update = pi_props->control->GetCameraUpdateOffset();

	if (update != 0 )
	{
		result = pi_camera->SetHOffset();
		INT h_offset = pi_camera->GetHOffset();
		pi_props->model->GetObjConfig()->SetPanoCamHOffset(h_offset);
	}

	return result;
}


// ---------- UpdateMonitor ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::UpdateMonitor()
{
	return VS_OK;
}


// ---------- UpdateSecondCount ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::UpdateSecondCount()
{
	pi_props->frames_total++;
	return VS_OK;
}


// ---------- UpdateStreamIcon ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::UpdateStreamIcon()
{
	INT result = VS_OK;

	BOOL streaming = pi_props->control->GetStreamFlag();


	if ( pi_props->streaming == streaming ) return result;
	pi_props->streaming = streaming;


	if (  pi_props->streaming )
	{
		printf("pi_props->streaming ON\n");
		result = pi_props->model->Set_FrameIcons(
				FRAMES_BUTTON_INDEX,
				"ST",
				STREAM_BUTTON_GROUP_ID,
				STREAM_VIEW,
				7, 2
			);
	}
	else
	{
		printf("pi_props->streaming OFF\n");
		result = pi_props->model->Set_FrameIcons(
				FRAMES_BUTTON_INDEX,
				"ST",
				STREAM_BUTTON_GROUP_ID,
				STREAM_VIEW,
				4, 1
			);
	}


	return result;
}
