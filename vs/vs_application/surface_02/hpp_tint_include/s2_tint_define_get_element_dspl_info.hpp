
INT GetElementDsplInfo(INT handle_model, INT body_lid)
{

	// ---- on time init
	if ( element_dspl_count == 0 )
	{

		// ---- local
			INT result = VS_OK;
			Model *model = (Model *)handle_model;
			vs_system::Tint *tint = model->pi_model->tint_setng;

		// ---- init element display info
			element_dspl_count = tint->GetBodyElemDsplInfo(
					(UINT)body_lid,                 // [in]
					num_element_item_lid,           // [in]
					element_item_lid,               // [in]
					group_item_lid,                 // [in]
					element_dspl_info,              // [out]
					hash_element_lid_to_dspl_index  // [out]
				);
	}

	return VS_OK;
}
