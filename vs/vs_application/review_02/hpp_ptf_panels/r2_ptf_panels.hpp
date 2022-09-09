////////////////////////////////////////////////////////////////////////////////

// ---------- r2_ptf_panels.hpp ----------
/*!
\file r2_ptf_panels.hpp
\brief
\author Gareth Edwards

\note

*/


#include "../header/r2_model.h"

using namespace review_02;


// ---------- Set_PanoPanel_FrameSelected ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < 

\note

Invoked in control element switch blocks:
	FRAMES_BUTTON_GROUP_ID and
	NEST_BUTTON_GROUP_ID

*/
INT Model::Set_PanoPanel_FrameSelected(UINT frame_id)
{

	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

		;

	#else


		// ---- either of the "main" frames ?
		if ( frame_id == 1 || frame_id == 2 )
		{

			// was deselect TBD why - 1?
			for (UINT i = 0; i < vs_system::ObjConfig::MAX_PANO_PTF_LUT; i++)
			{
				if ( pi_model->panel_context_ptf[i] != NULL )
					pi_model->panel_context_ptf[i]->SetSelected(FALSE);
			}


			if ( frame_id == 1 )
			{
				if ( pi_model->panel_context_ptf[0] != NULL )
				{
					pi_model->panel_context_ptf[0]->SetSelected(TRUE);
					pi_model->obj_config->SetPanoPtfSelectedById(1);
				}
			}
			else
			{
				UINT index = pi_model->active_panel_id;
				if ( pi_model->panel_context_ptf[index - 1] != NULL )
				{
					pi_model->panel_context_ptf[index - 1]->SetSelected(TRUE);
					pi_model->obj_config->SetPanoPtfSelectedById(index);
				}
			}

			// set all "2D" prefixed elements to be re-configured
					BOOL pan_2d_vertex_buffer;
					INT result = GetObjConfig()->GetPanoPanVertexBuffer(&pan_2d_vertex_buffer);
					if ( pan_2d_vertex_buffer )
						result = gfx->SetGlobalConfigureBitmask(
								VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING,
								"2D"
							);
		}

	#endif

	return VS_OK;
}


// ---------- Set_ImgOrPanoPanel_IconSelected ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\ note

	Invoked at runtime!!! from:
		Model::SetupButtons
		Model::Set_ImgOrPanoPanel_Selected

	Invoked from:
		Control::SetPanelActive

*/
INT Model::Set_ImgOrPanoPanel_IconSelected(UINT panel_id)
{

	// --- "Frames" button list from which the Frames elements are generated
		button_element_descriptor *list = button_list[FRAMES_BUTTON_INDEX];


	// IF -- no list -- THEN -- return error
		if (list == NULL) return VS_ERROR;


	// ---- local
		INT result = VS_OK;


	// ---- legal PTF index ? -- update selected icon

	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

		UINT id_first = vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX + 1;
		UINT id_last = vs_system::ObjConfig::IMG_PTZ_LAST_PANEL_INDEX + 1;

		// ---- "IMG_PTZ_[...]" ---- OK

		if ( panel_id >= id_first && panel_id <= id_last )

	#else

		if (panel_id >= 1 && panel_id <= 8)

	#endif

	{

		// ---- update active pano ptf stuff
			vs_system::ObjConfig *obj_config = GetObjConfig();
			UINT id = Get_ImgPanel_ActiveId();
			obj_config->SetImagePanelActiveId(id);


		// ---- set row/col

			#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

				// "CONFIG IMG PTZ" ---- TBD
				
				// "F2"
				(list + 1)->tex_icon_col = (panel_id + 1) % 3; 
				(list + 1)->tex_icon_row = 5;


				// "F3"
				(list + 2)->tex_icon_col = 5 + ( (panel_id + 1) % 3 );
				(list + 2)->tex_icon_row = 5 + ( (panel_id - 2) / 3 );

			#else

				// IF panel # 1 THEN must be frame 1
				if ( panel_id == 1 )
				{
					// set frame # 1 icon to denote that "PTF" panel with id==1 is active
					(list + 0)->tex_icon_col = 0;
					(list + 0)->tex_icon_row = 5;

					// set frame # 2 icon to denote that "PTF" panel with 2<=id<=7 are NOT active
					(list + 1)->tex_icon_col = 1;
					(list + 1)->tex_icon_row = 6;
				}
				// ELSE must be frame # 2
				else
				{
					// set frame # 1 icon to denote that "PTF" panel with id==1 is NOT active
					(list + 0)->tex_icon_col = 0;
					(list + 0)->tex_icon_row = 6;

					// set frame # 2 icon to denote that a nono "PTF" panel with 2<=id<=7 is active
					(list + 1)->tex_icon_col = panel_id - 1;
					(list + 1)->tex_icon_row = 5;
				}

			#endif


		// ---- set "F#" prefixed elements to be configured
			result = VS_OK;
			result = gfx->SetGlobalConfigureBitmask(
					VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING,
					"F1"
				);
			result = gfx->SetGlobalConfigureBitmask(
					VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING,
					"F2"
				);
			#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
			result = gfx->SetGlobalConfigureBitmask(
					VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING,
					"F3"
				);
			#endif

	}

	return VS_OK;
}


// ---------- Set_ImgOrPanoPanel_Selected ----------
/*!
\brief Select and setup a ptf panel
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note

	Invoked ONLY once for each display list !!!
	--- in Model::InitPanelDisplayList

*/

INT Model::Set_ImgOrPanoPanel_Selected(
		vs_system::PanelContext *panel_context,
		CHAR *panel_name
	)
{

	UINT panel_id = 0;
	panel_context->GetId(&panel_id);
	if ( panel_id != 0 )
	{

		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

			INT image_id = Get_ImgPanel_Id(panel_name);
			if ( panel_name[0] == 'I' && image_id != VS_ERROR )
			{

				// ---- "IMG_PTZ_[...]" ---- OK

				// ---- select ptz panel in overview ?
					// note: works @ runtime....
					//       -- but only if # active id

					vs_system::ObjConfig *obj_config = GetObjConfig();

					UINT id_second = vs_system::ObjConfig::IMG_PTZ_FIRST_PANEL_INDEX + 1;
					if ( image_id == id_second )
					{

						
						// ---- select
							panel_context->SetSelected(TRUE);
							INT result = obj_config->SetPanoPtfSelectedById(image_id);
								result = Set_ImgOrPanoPanel_IconSelected(image_id);


						// ---- copy to stream
							UINT id_hid = vs_system::ObjConfig::IMG_PTZ_HIDDEN_PANEL_INDEX + 1;
							UINT id_vis = vs_system::ObjConfig::IMG_PTZ_VISIBLE_PANEL_INDEX + 1;

							for ( UINT i = id_hid; i <= id_vis; i++ )
							{
								obj_config->CpyImagePanelActivePtfbyId(i);
							}

					}

				// ---- settings ?
					UINT id_settings = vs_system::ObjConfig::IMG_PTZ_SETTINGS_PANEL_INDEX + 1;
					if ( image_id == id_settings)
					{
						INT result = obj_config->SetPanoPTFById(image_id, 0, 0, 1);
					}

			}
		#else

			if ( panel_id == 1 || panel_id == 2 )
				panel_context->SetSelected(TRUE);
			else
				panel_context->SetSelected(FALSE);

		#endif

	}

	return VS_OK;
}

