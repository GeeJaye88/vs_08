////////////////////////////////////////////////////////////////////////////////

// ---------- kpad_select.hpp ----------
/*!
\file kpad_select.hpp
\brief Interface for Model class Tint select methods
\author Gareth Edwards
*/


// ---------- KpadBodySelect ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::KpadBodySelect(
		UINT body_id
	)
{

	INT result = pi_model->kpad_setng->SetSelectedBody(body_id);

	pi_model->handle_dspl_kpad_body = pi_model->kpad_setng->GetBodyHandle(body_id);

	return VS_OK;
}


// ---------- KpadBodyUpdateText ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::KpadBodyUpdateText(
		UINT  body_id,
		INT   group_id,
		INT   elem_id
	)
{

	INT result = pi_model->kpad_setng->SetSelectedBody(body_id);

	result = pi_model->kpad_setng->UpdateText(
			group_id,
			elem_id
		);

	return result;
}