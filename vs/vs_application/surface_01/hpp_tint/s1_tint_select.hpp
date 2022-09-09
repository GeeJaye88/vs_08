////////////////////////////////////////////////////////////////////////////////

// ---------- s1_tint_select.hpp ----------
/*!
\file s1_tint_select.hpp
\brief Interface for Model class Tint select methods
\author Gareth Edwards
*/

#pragma once

using namespace surface_01;


// ---------- TintBodySelect ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::TintBodySelect(
		UINT body_id
	)
{

	INT result = pi_model->tint_setng->SetSelectedBody(body_id);

	pi_model->handle_dspl_tint_body = pi_model->tint_setng->GetBodyHandle(body_id);

	return VS_OK;
}


// ---------- TintBodyUpdateText ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::TintBodyUpdateText(
		UINT  body_id,
		INT   group_id,
		INT   elem_id
	)
{

	INT result = pi_model->tint_setng->SetSelectedBody(body_id);

	result = pi_model->tint_setng->UpdateText(
			group_id,
			elem_id
		);

	return result;
}