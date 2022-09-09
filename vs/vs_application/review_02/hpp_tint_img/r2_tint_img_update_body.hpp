////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_img_update_body.hpp ----------
/*!
\file r2_tint_img_update_body.hpp
\brief application include files.
\author Gareth Edwards
*/

#pragma once

using namespace review_02;


// ---------- Tint_Img_UpdateBody ----------
/*!
\brief facade for PI_Model method
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::Tint_Img_UpdateBody(
		UINT body_id
	)
{
	INT result = pi_model->tint_img_setng->SetSelectedBody(body_id);
	pi_model->handle_dspl_tint_img_body = pi_model->tint_img_setng->GetBodyHandle(body_id);
	return VS_OK;
}


// ---------- Tint_Img_UpdateBodyText ----------
/*!
\brief facade for PI_Model method
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::Tint_Img_UpdateBodyText(
		UINT  body_id,
		INT   group_id,
		INT   elem_id
	)
{
	INT result = pi_model->tint_img_setng->SetSelectedBody(body_id);

	result = pi_model->tint_img_setng->UpdateText(
			group_id,
			elem_id
		);

	return result;
}