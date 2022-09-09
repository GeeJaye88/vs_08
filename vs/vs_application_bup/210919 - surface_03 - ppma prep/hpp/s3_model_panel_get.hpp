////////////////////////////////////////////////////////////////////////////////

// ---------- s3_model_get_panel.hpp ----------
/*!
\file s3_model_get_panel.hpp
\brief Implementation of the get panel methods
\author Gareth Edwards
*/


// ========================================================================== //


// ---------- GetPanelDisplayListHandle ----------
/*!
\brief Get a panels display list handle
\author Gareth Edwards
\return
*/
INT Model::GetPanelDisplayListHandle(
		vs_system::PanelContext *panel_gfx_context
	)
{

	// ---- local
		INT hr = 0;


	// ---- get current index of display list
		UINT index_display_list;
		hr = panel_gfx_context->GetDisplayListIndex(&index_display_list);


	// ---- get display list handle
		INT handle_display_list;
		hr = panel_gfx_context->GetDisplayListHandle(index_display_list, &handle_display_list);


	// ---- no display list handle ? - then set one -
		if ( handle_display_list <= 0 )
		{

			// ---- get display list name
				CHAR name_display_list[VS_MAXCHARLEN];
				hr = panel_gfx_context->GetDisplayListName(index_display_list, name_display_list);
				

			// ---- get display list handle, and id's...
				handle_display_list = pi_model->gfx->GetHandleDisplayList(name_display_list);


			// ---- empty panel ?
				if ( handle_display_list == 0 )
				{
					hr = panel_gfx_context->SetDisplayListHandle(
									index_display_list,
									VS_OK
								);
					return VS_OK;
				}


			// ---- id's
				UINT group_id = pi_model->gfx->GetElementGroupId(handle_display_list);
				UINT id       = pi_model->gfx->GetElementId(handle_display_list);


			// ---- get panel name
				CHAR name_panel[VS_MAXCHARNAME];
				hr = panel_gfx_context->GetName(name_panel);
				if ( name_panel == NULL ) strcpy(name_panel, "UNKNOWN");


			// ---- report
				CHAR msg[VS_MAXCHARLEN];
				sprintf(msg,
						"+++ set panel %s display list %s handle [%d] and gid %d\n",
						name_panel,
						name_display_list,
						handle_display_list,
						group_id
					);
				OutputDebugString(msg);


			// ---- set
				hr = panel_gfx_context->SetDisplayListHandle(
								index_display_list,
								handle_display_list
							);
				hr = panel_gfx_context->SetGroupId(group_id);

		}

	return handle_display_list;
}


// ========================================================================== //