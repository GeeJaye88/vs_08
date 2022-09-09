////////////////////////////////////////////////////////////////////////////////

// ---------- s1_model_my_star.hpp ----------
/*!
\file s1_model_my_star.hpp
\brief Implementation of the my star model update methods
\author Gareth Edwards
*/


// ========================================================================== //


// ---------- SysObjectSelect ----------
/*!
\brief Setup panel viewrects
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Model::SysObjectSelect(
		INT group_id,
		INT select_id
	)
{

	// ---- local
		INT select_index = select_id;

	// ---- is spheres ?
		if (pi_model->handle_sphere[select_index] != NULL)
		{

			// ---- get sphere element context
				vs_system::ElmContext *elm_context = NULL;
				INT hr = pi_model->gfx->GetElementContext(
						pi_model->handle_sphere[select_index],
						&elm_context
					);


			// ---- report
				CHAR text[VS_MAXCHARLEN];
				sprintf(text, " +++ PICK - Element %s ( %d %d )\n",
						elm_context->GetName().c_str(),
						group_id,
						select_id
					);
				OutputDebugString(text);


			// ---- switch selected ON & everythjing else OFF
				for ( INT i = 0; i < pi_model->object_index; i++ )
				{

					// ---- is halo ?
						if ( pi_model->handle_halo[i] != 0 )
						{

							// ---- get halo element context
								INT hr = pi_model->gfx->GetElementContext(
										pi_model->handle_halo[i],
										&elm_context
									);

								//elm_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
								//elm_context->SetConfigured(FALSE);

							// ---- display ?
								if ( i == select_index )
								{
									elm_context->SetDisplay(TRUE);
								}
								else
								{
									elm_context->SetDisplay(FALSE);
								}
						}
				}
		}

	return VS_OK;
}



// ========================================================================== //