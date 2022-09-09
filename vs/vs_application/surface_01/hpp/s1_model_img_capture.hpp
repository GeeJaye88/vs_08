////////////////////////////////////////////////////////////////////////////////

// ---------- s1_img_capture.hpp ----------
/*!
\file s1_img_capture.hpp
\brief Implementation of the image capture methods
\author Gareth Edwards
*/


// ========================================================================== //


INT Model::SetImgCaptureEventId(UINT event_id)
{
	pi_model->img_capture_event_id = event_id;
	return VS_OK;
}


INT Model::SetImgCaptureElapsedMs(DOUBLE elapsed_ms)
{
	pi_model->img_capture_elapsed_ms = elapsed_ms;
	return VS_OK;
}


// ========================================================================== //