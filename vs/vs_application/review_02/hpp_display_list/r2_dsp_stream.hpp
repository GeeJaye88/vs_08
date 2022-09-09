////////////////////////////////////////////////////////////////////////////////

// ---------- r2_dl_stream.hpp ----------
/*!
\file r2_dl_stream.hpp
\brief
\author Gareth Edwards
*/


#include "../header/r2_model.h"

using namespace review_02;


// ---------- UpdateCaptureDisplayList ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
\note UPDATE ONLY TEXT
*/
INT Model::UpdateCaptureDisplayList(UINT group_id, UINT id, BOOL mode)
{
	return VS_OK;
}


// ---------- SetStreamElementDisplay ----------
/*!
\brief Setup stream display lists
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetStreamElementDisplay(INT stream_id)
{

	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
		SetStreamElementDisplayForPano(stream_id);
	#else
		SetStreamElementDisplayForPano(stream_id);
	#endif

	return VS_OK;
}


// ---------- SetStreamElementDisplayForPano ----------
/*!
\brief Set stream element display flags
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetStreamElementDisplayForPano(INT stream_id)
{

	// ---- set "stream" RTSP

		if ( stream_id == STREAM_RTSP )
			{
				INT hr = 0;

				vs_system::ElmContext *element_context;
				hr = gfx->GetElementContext(pi_model->handle_elem_stream_visible[6], &element_context);

				std::string rtsp_address =
					"LAN: \"rtsp://" +
					pi_model->hostaddr + ":" +
					pi_model->rtspport + "/" +
					pi_model->rtsp_stream + "\"";
				hr = element_context->SetText(rtsp_address.c_str());
				hr = element_context->SetAlign(VS_ALIGN_CENTRE_ON_TEXT);
				hr = element_context->SetBitmask(VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING);

			}

	// ---- set "stream" visibility & config 
		else
		{

			// ---- local
				INT hr = 0;
				BOOL visibility[5] = { TRUE, TRUE, FALSE, FALSE, TRUE };


			// ---- select
				switch ( stream_id )
				{

					default:

					// ---- set "stream" visibility
					case STREAM_1:
						{
							visibility[0] = FALSE;
							visibility[1] = FALSE;
							visibility[2] = TRUE;
							visibility[3] = FALSE;
						}
						break;

					case STREAM_2:
						{
							visibility[0] = TRUE;
							visibility[1] = TRUE;
							visibility[2] = FALSE;
							visibility[3] = FALSE;
						}
						break;

					case STREAM_3:
						{
							visibility[0] = FALSE;
							visibility[1] = FALSE;
							visibility[2] = FALSE;
							visibility[3] = TRUE;
						}
						break;
				}


			// ---- set "stream" element_context->visibility
				for (INT i = 0; i < 4; i++)
				{
					vs_system::ElmContext *element_context;
					hr = gfx->GetElementContext(pi_model->handle_elem_stream_hidden[i], &element_context);
					hr = element_context->SetDisplay(visibility[i]);
				}


			// ---- set "stream" element_context->etc., ...bitmask
				for (INT i = 0; i < 7; i++)
				{
					vs_system::ElmContext *element_context;
					hr = gfx->GetElementContext(pi_model->handle_elem_stream_visible[i], &element_context);

					if (i == 6)
					{
						std::string rtsp_address =
							"LAN: \"rtsp://" +
							pi_model->hostaddr + ":" +
							pi_model->rtspport + "/" +
							pi_model->rtsp_stream + "\"";
						hr = element_context->SetText(rtsp_address.c_str());
						hr = element_context->SetAlign(VS_ALIGN_CENTRE_ON_TEXT);
						hr = element_context->SetBitmask(VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING);
					}

					hr = element_context->SetDisplay(visibility[i]);
				}

		}

	return VS_OK;
}


// ---------- SetupStreamDisplayList ----------
/*!
\brief Setup stream display lists
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupStreamDisplayList()
{

	// ---- local
		INT result = 0;


	// ---- initialise layout parameters
		//
		// See method SetupAboutDisplayList
		//
		FLOAT x_top         = 0.05f;
		FLOAT y_top         = 0.00f;
		FLOAT text_dim      = 0.05f;
		FLOAT text_space    = 0.01f;


	// ---- initialise location/layout parameters
		FLOAT button_total = text_dim + text_space;


	// ---- initialise column location
		FLOAT c[5] =
		{
			{ button_total * 0.0f + text_space },
			{ button_total * 1.0f + text_space },
			{ button_total * 2.5f + text_space },
			{ button_total * 3.5f + text_space },
			{ button_total * 4.5f + text_space }
		};


	//	-------- BUILD LUT --------


	// ---- system ----
		using namespace vs_system;


	// ---- initialise template luts
		vs_system::ElmContext::ElmButton temp =
		{
			"Stream", "", 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			VS_ATLAS_PAGE_GROUP_TWO, 0, 0, 4, 7, 1, 1
		};


	// ---- element versions
		UINT element_version_lut[3] =
		{
			vs_system::ElmContext::VERSION_BACKGROUND,
			vs_system::ElmContext::VERSION_ICON,
			vs_system::ElmContext::VERSION_OUTLINE
		};


	// ---- lut
		INT icon_col[6] = { 4, 7, 4, 5, 7, 0 };
		INT icon_row[6] = { 7, 7, 6, 6, 6, 0 };
		INT icon_be[6]  = {
							ElmContext::BEHAVIOUR_SWITCH,
							ElmContext::BEHAVIOUR_SWITCH,
							ElmContext::BEHAVIOUR_RADIO,
							ElmContext::BEHAVIOUR_RADIO,
							ElmContext::BEHAVIOUR_RADIO,
							0
						};
		INT icon_id[6]  = {
							STREAM_PLAY,
							STREAM_ONVIF_VPTZ,
							STREAM_1,
							STREAM_2,
							STREAM_3,
							0
						};


	// ---- initialise button lut
		vs_system::ElmContext::ElmButton b[6];
		for (INT i = 0; i < 5; i++)
		{
			b[i]           = temp;
			b[i].group_id  = STREAM_BUTTON_GROUP_ID;
			b[i].id        = icon_id[i];
			b[i].type      = 0;
			b[i].status    = FALSE;
			b[i].behaviour = icon_be[i];
			b[i].x         = c[i];
			b[i].width     = text_dim;
			b[i].y         = y_top / 2;
			b[i].height    = text_dim;
			b[i].icon_col  = (UINT)icon_col[i];
			b[i].icon_row  = (UINT)icon_row[i];
		}


		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
			;
		#else
			b[3].status = TRUE;
		#endif


	// ---- initialise exit button
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
			UINT i = 2;
		#else
			UINT i = 5;
		#endif
			b[i] = temp;
			b[i].group_id  = STREAM_BUTTON_GROUP_ID;
			b[i].id        = STREAM_CLOSE;
			b[i].type      = 0;
			b[i].status    = FALSE;
			b[i].behaviour = ElmContext::BEHAVIOUR_CLICK;
			b[i].x         = 1 - text_dim;
			b[i].width     = text_dim;
			b[i].y         = 0;
			b[i].height    = text_dim;
			b[i].icon_col  = 7;
			b[i].icon_row  = 1;


	//	-------- USE BUTTON LUT TO CREATE DISPLAY LIST  --------

		using namespace review_02;
		FLOAT param[VS_MAX_ELM_FLOAT_PARAMETER];
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
			UINT max_but = 3;
		#else
			UINT max_but = 6;
		#endif


	// ---- add buttons
		for (UINT but = 0; but<max_but; but++)
		{

			// ---- add element
				for (UINT i = 0; i<3; i++)
				{

					// ---- display list
						INT button_handle = gfx->AddElement(
								pi_model->handle_dspl_stream_ui,
								b[but].name,
								b[but].group_id,
								b[but].id,
								&pi_model->obj_config->Callback_Button_Configure
							);


					// ---- initialise
						vs_system::ElmContext *elm = NULL;
						INT get_element_result = gfx->GetElementContext(button_handle, &elm);
						if (get_element_result != NULL)
						{
							result = elm->SetFontName("Stream");
							result = elm->SetFontAlignment(vs_system::ElmContext::ALIGN_LEFT);
							result = elm->SetText(b[but].text);
							result = elm->SetBehaviour(b[but].behaviour);

							// note: to switch on here use "(i == 1 ? 0 : b[but].status)"
							result = elm->SetSwitchedOn(FALSE);

							result = elm->SetVersion(element_version_lut[i]);
							result = elm->SetRGBA(1.0f, 1.0f, 1.0f, 0.2f);

							result = pi_model->obj_config->ButtonSetDPT(
									param,
									vs_system::ElmContext::VIEWPORT_HEIGHT |
										( but == max_but - 1 ?
											ElmContext::VIEWPORT_WIDTH_XLOC : 0 )
								);
							result = pi_model->obj_config->ButtonSetLocation(
									param,
									b[but].x, b[but].y,
									b[but].width, b[but].height
								);
							result = pi_model->obj_config->ButtonSetTexture(
									param,
									VS_ATLAS_PAGE_GROUP_TWO,
									b[but].bg_col,   b[but].bg_row,
									b[but].icon_col, b[but].icon_row,
									1,  1,
									0.001f
								);

						result = elm->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);
					}
				}
		}

	return VS_OK;
}


// ---------- SetStreamDisplayStatus ----------
/*!
\brief Setup stream panel display status
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note This method ONLY works because there is only ONE master copy
of the various panels (see AppView::SetMasterPanels() method).

*/
INT Model::SetStreamDisplayStatus()
{
	// ---- local
		INT result = VS_ERROR;


	// ---- set stream content ON
		if ( pi_model->stream_content_panel_context != NULL )
		{
			// Get, flip status, set..
			BOOL panel_status = TRUE;
			pi_model->stream_content_panel_context->GetDisplayStatus(&panel_status);
			panel_status = panel_status == TRUE ? FALSE : TRUE;
			pi_model->stream_content_panel_context->SetDisplayStatus(panel_status);
			result = panel_status == TRUE ? VS_OK : VS_ERROR;
		}


	// ---- set stream control ON
		if ( pi_model->stream_control_panel_context != NULL )
		{
			// Get, flip status, set..
			BOOL panel_status = TRUE;
			pi_model->stream_control_panel_context->GetDisplayStatus(&panel_status);
			panel_status = panel_status == TRUE ? FALSE : TRUE;
			pi_model->stream_control_panel_context->SetDisplayStatus(panel_status);
			result = panel_status == TRUE ? VS_OK : VS_ERROR;
		}

	return result;
}


// ---------- UpdateStreamDisplayList ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
\note UPDATE ONLY TEXT
*/
INT Model::UpdateStreamDisplayList(UINT group_id, UINT id, BOOL mode)
{
	// ---- local
		INT result = 0;
		vs_system::ElmContext *element_context = NULL;


	// ---- number of elements in display list
		INT number_of_config_elements = gfx->GetChildCount(pi_model->handle_dspl_stream_ui);


	// ---- stream element
		INT elm = gfx->GetFirst(pi_model->handle_dspl_stream_ui);
		while (elm != 0)
		{

			// ---- context ?
				result = gfx->GetElementContext(elm, &element_context);
				if (result != NULL)
				{
					if ( id == STREAM_PLAY || id == STREAM_ONVIF_VPTZ )
					{
						UINT element_group_id = element_context->GetGroupId();
						UINT element_id = element_context->GetId();
						if (element_group_id == group_id && element_id == id)
						{
							result = element_context->SetSwitchedOn(mode);
						}
					}
					else
					{
						UINT element_group_id = element_context->GetGroupId();
						UINT element_id = element_context->GetId();
						if (element_group_id == group_id && element_id == id)
						{
							// only switch on VERSION_BACKGROUND & VERSION_OULINE not VERSION_ICON
							UINT version = element_context->GetVersion();
							if (version != vs_system::ElmContext::VERSION_ICON)
								result = element_context->SetSwitchedOn(mode);
						}
					}

				}

				elm = gfx->GetNext(elm);
		}


	// ---- set config bitmask so that stream elements are reconfigured.... 
		result = gfx->SetGlobalConfigureBitmask(
				VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING,
				"Stream"
			);

	return VS_OK;
}


// ---------- UpdateFrameDisplayList ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
\note UPDATE ONLY TEXT
*/
INT Model::UpdateFrameDisplayList(UINT group_id, UINT id, BOOL mode)
{

	// ---- local
		INT result = 0;
		vs_system::ElmContext *element_context = NULL;


	// ---- number of elements in display list
		INT number_of_config_elements = gfx->GetChildCount(pi_model->handle_dspl_frame_button);


	// ---- FOR each stream element
		INT elm = gfx->GetFirst(pi_model->handle_dspl_frame_button);
		while (elm != 0)
		{

			// ---- IF context THEN --
			result = gfx->GetElementContext(elm, &element_context);
			if (result != NULL)
			{

				UINT element_group_id = element_context->GetGroupId();
				UINT element_id = element_context->GetId();
				if ( element_group_id == group_id && element_id == id )
				{
					result = element_context->SetSwitchedOn(mode);
					FLOAT *p = element_context->GetFloatParameter();

				}
			}

			elm = gfx->GetNext(elm);
		}


	// ---- set config bitmask so that stream elements are reconfigured.... 
	//result = gfx->SetGlobalConfigureBitmask(VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING, "Stream");


	return VS_OK;
}


// ---------- SetStreamOnvifHandlerArgs ----------
/*!
\brief Setup stream onvif stream arg values
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetStreamOnvifHandlerArgs(
		const std::string hostaddr,
		const std::string onvifport,
		const std::string rtspport,
		const std::string rtsp_stream
	)
{

	pi_model->hostaddr    = hostaddr;
	pi_model->onvifport   = onvifport;
	pi_model->rtspport    = rtspport;
	pi_model->rtsp_stream = rtsp_stream;

	return VS_OK;
}


// ---------- SetupSharedHiddenLists ----------
/*!
\brief Setup shared hidden display lists
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupSharedHiddenLists()
{

	pi_model->handle_elem_shared_hidden = gfx->AddElement(
			pi_model->handle_dspl_shared_hidden,
			"2D PanoRectPart",
			VS_OBJ_GEOMETRY,
			VS_OBJ_PANO_RECT_PART,
			&pi_model->obj_config->Callback_PanoRectPart_Configure
		);

	return VS_OK;
}


// ---------- SetRemoteControlStatus ----------
/*!
\brief Setup onvif remote control flag
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetRemoteControlStatus(BOOL status)
{
	pi_model->onvif_remote_control = status;
	return VS_OK;
}
