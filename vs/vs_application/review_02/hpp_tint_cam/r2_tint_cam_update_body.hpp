////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_cam_update_body.hpp ----------
/*!
\file r2_tint_cam_update_body.hpp
\brief application include files.
\author Gareth Edwards
*/


using namespace review_02;


// ---------- Tint_Cam_UpdateBody ----------
/*!
\brief facade for PI_Model method
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::Tint_Cam_UpdateBody(
		UINT body_id
	)
{
	INT result = pi_model->tint_cam_setng->SetSelectedBody(body_id);
	pi_model->handle_dspl_tint_cam_body = pi_model->tint_cam_setng->GetBodyHandle(body_id);
	return VS_OK;
}


// ---------- Tint_Cam_UpdateBodyText ----------
/*!
\brief facade for PI_Model method
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::Tint_Cam_UpdateBodyText(
	UINT  body_id,
	INT   group_id,
	INT   elem_id
	)
{
	INT result = pi_model->tint_cam_setng->SetSelectedBody(body_id);

	result = pi_model->tint_cam_setng->UpdateText(
			group_id,
			elem_id
		);

	return result;
}