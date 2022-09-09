////////////////////////////////////////////////////////////////////////////////

// ---------- r2_setup_stream.hpp ----------
/*!
\file r2_setup_stream.hpp
\brief
\author Gareth Edwards
*/


#pragma once

#include "../header/r2_model.h"

using namespace review_02;


// ---------- SetupStreamHiddenLists ----------
/*!
\brief Setup stream display lists
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupStreamHiddenLists()
{

	// ---- select
		#if defined(VS_CAMERA_05)
			SetupStreamHiddenListsForImage();
		#else
			SetupStreamHiddenListsForPano();
		#endif

	return VS_OK;
}


// ---------- SetupStreamHiddenListsForImage ----------
/*!
\brief Setup stream hidden display lists
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupStreamHiddenListsForImage()
{

	// ---- local
		INT hr = 0;


	// ---- "IMG_PTZ_[...]" ---- OK

		for (UINT i = 0; i < 4; i++)
		{
			CHAR name[6] = "IRQ10";
				name[4] = '0' + vs_system::ObjConfig::IMG_PTZ_HIDDEN_PANEL_INDEX + 1;

			pi_model->handle_elem_stream_hidden[i] = gfx->AddElement(
					pi_model->handle_dspl_stream_hidden,
					name,
					VS_OBJ_GEOMETRY,
					VS_OBJ_IMG,
					&pi_model->obj_config->Callback_Img_Rect_Configure
				);

			vs_system::ElmContext *elm_context;
			pi_model->gfx->GetElementContext(
					pi_model->handle_elem_stream_hidden[i],
					&elm_context
				);

			FLOAT *param = elm_context->GetFloatParameter();
			*(param + vs_system::ObjConfig::IMG_RECT_INDEX) = vs_system::ObjConfig::IMG_PTZ_HIDDEN_PANEL_INDEX;
			*(param + vs_system::ObjConfig::IMG_RECT_TYPE)  = vs_system::ObjConfig::IMG_RECT_TYPE_SOLID_PTZ_VIEW;
			*(param + vs_system::ObjConfig::IMG_RECT_MODE)  = vs_system::ObjConfig::IMG_RECT_MODE_PTF_VIEW;
		}

	return VS_OK;
}


// ---------- SetupStreamHiddenListsForPano ----------
/*!
\brief Setup stream hidden display lists
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupStreamHiddenListsForPano()
{

	// ---- local
		INT hr = 0;


	// ---- 1, 2, 3 & 4: "stream" geometry
		pi_model->handle_elem_stream_hidden[0] = gfx->AddElement(
				pi_model->handle_dspl_stream_hidden,
				"2D PanoRect",
				VS_OBJ_GEOMETRY,
				VS_OBJ_PANO_RECT_TOP,
				&pi_model->obj_config->Callback_PanoRect_Configure
			);
		pi_model->handle_elem_stream_hidden[1] = gfx->AddElement(
				pi_model->handle_dspl_stream_hidden,
				"2D PanoRect",
				VS_OBJ_GEOMETRY,
				VS_OBJ_PANO_RECT_BOTTOM,
				&pi_model->obj_config->Callback_PanoRect_Configure
			);
		pi_model->handle_elem_stream_hidden[2] = gfx->AddElement(
				pi_model->handle_dspl_stream_hidden,
				"3D Sphere",
				VS_OBJ_GEOMETRY,
				VS_OBJ_PANO_SPHERE,
				&pi_model->obj_config->Callback_PanoSphere_Configure
			);
		pi_model->handle_elem_stream_hidden[3] = gfx->AddElement(
				pi_model->handle_dspl_stream_hidden,
				"2D PanoRect",
				VS_OBJ_GEOMETRY,
				VS_OBJ_PANO_RECT_EQUI,
				&pi_model->obj_config->Callback_PanoRect_Configure
			);

	return VS_OK;
}


// ---------- SetupStreamVisibleLists ----------
/*!
\brief Setup stream display lists
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupStreamVisibleLists()
{

	// ---- select
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
			SetupStreamVisibleListsForImage();
		#else
			SetupStreamVisibleListsForPano();
		#endif

	return VS_OK;
}


// ---------- SetupStreamVisibleListsForImage ----------
/*!
\brief Setup stream visible display lists
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note 2017
*/
INT Model::SetupStreamVisibleListsForImage()
{

	// ---- local
		INT result = 0;


	// ---- "IMG_PTZ_[...]" ---- OK

		for (UINT i = 0; i < 4; i++)
		{

			CHAR name[6] = "IRQ10";
				name[4] = '0' + vs_system::ObjConfig::IMG_PTZ_VISIBLE_PANEL_INDEX + 1;

			pi_model->handle_elem_stream_visible[i] = gfx->AddElement(
					pi_model->handle_dspl_stream_visible,
					name,
					VS_OBJ_GEOMETRY,
					VS_OBJ_IMG,
					&pi_model->obj_config->Callback_Img_Rect_Configure
				);

			vs_system::ElmContext *elm_context;
			pi_model->gfx->GetElementContext(
					pi_model->handle_elem_stream_visible[i],
					&elm_context
				);

			FLOAT *param = elm_context->GetFloatParameter();
			*(param + vs_system::ObjConfig::IMG_RECT_INDEX) = vs_system::ObjConfig::IMG_PTZ_VISIBLE_PANEL_INDEX;
			*(param + vs_system::ObjConfig::IMG_RECT_TYPE)  = vs_system::ObjConfig::IMG_RECT_TYPE_SOLID_PTZ_VIEW;
			*(param + vs_system::ObjConfig::IMG_RECT_MODE)  = vs_system::ObjConfig::IMG_RECT_MODE_PTF_VIEW;

		}


	// ---- Y!!
		FLOAT y_location = 0.4f;

	// ---- 5 & 6: outline around [ 7 ]  
		FLOAT param[VS_MAX_ELM_FLOAT_PARAMETER];

		UINT v[2] = {
			vs_system::ElmContext::VERSION_BACKGROUND,
			vs_system::ElmContext::VERSION_BACKGROUND
		};

		for (UINT i = 0; i < 2; i++)
		{

			pi_model->handle_elem_stream_visible[4 + i] = gfx->AddElement(
					pi_model->handle_dspl_stream_visible,
					"",
					0,
					0,
					&pi_model->obj_config->Callback_Button_Configure
				);

			vs_system::ElmContext *elm = NULL;

			INT get_element_result = gfx->GetElementContext(
					pi_model->handle_elem_stream_visible[4 + i],
					&elm
				);

			if (get_element_result != NULL)
			{
				result = elm->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NONE);
				result = elm->SetVersion(v[i]);
				result = elm->SetRGBA(1.0f, 1.0f, 1.0f, 1.0f);

				result = pi_model->obj_config->ButtonSetDPT(
						param,
						vs_system::ElmContext::VIEWPORT_WIDTH |
							vs_system::ElmContext::VIEWPORT_CLIP
					);

				// ---- inside
					if ( i == 0 )
					{
						result = pi_model->obj_config->ButtonSetLocation(
								param,
								0.095f, y_location - 0.005f, 0.81f, 0.06f
							);
						result = pi_model->obj_config->ButtonSetTexture(
								param,
								VS_ATLAS_PAGE_GROUP_TWO,
								0, 0, 0, 0, 1, 1, 0.01f
							);
					}
				// ---- outside
					else
					{
						result = pi_model->obj_config->ButtonSetLocation(
								param,
								0.1f, y_location, 0.8f, 0.05f
								//-0.01f, 0.2f, 0.0f, 0.0f
							);
						result = pi_model->obj_config->ButtonSetTexture(
								param,
								VS_ATLAS_PAGE_GROUP_TWO,
								1, 0, 0, 0, 1, 1, 0.01f
							);
					}


				result = elm->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);
			}
		}


	// ---- 7: LAN rtsp

		pi_model->handle_elem_stream_visible[6] = gfx->AddElement(
				pi_model->handle_dspl_stream_visible,
				"",
				0,
				0,
				&pi_model->obj_config->Callback_Button_Configure
			);

		vs_system::ElmContext *elm = NULL;

		INT get_element_result = gfx->GetElementContext(
				pi_model->handle_elem_stream_visible[6],
				&elm
			);

		if ( get_element_result != NULL )
		{
			result = elm->SetFontName("RTSP");
			result = elm->SetAlign(VS_ALIGN_CENTRE_ON_TEXT);
			result = elm->SetFontAlignment(vs_system::ElmContext::ALIGN_LEFT);
			result = elm->SetText("LAN: \"rtsp://0.0.0.0:0/stream_name\"");
			result = elm->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NONE);
			result = elm->SetVersion(vs_system::ElmContext::VERSION_TEXT);
			result = elm->SetRGBA(1.0f, 1.0f, 1.0f, 1.0f);

			result = pi_model->obj_config->ButtonSetDPT(
					param,
					vs_system::ElmContext::VIEWPORT_WIDTH
				);
			result = pi_model->obj_config->ButtonSetLocation(
					param,
					0.2f, y_location, 0.5f, 0.05f
				);
			result = pi_model->obj_config->ButtonSetTexture(
					param,
					VS_ATLAS_PAGE_GROUP_TWO,
					0, 0, 0, 0, 1, 1, 0.01f
				);

			result = elm->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);
		}

	return VS_OK;
}


// ---------- SetupStreamVisibleListsForPano ----------
/*!
\brief Setup stream visible display lists
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note 2017
*/
INT Model::SetupStreamVisibleListsForPano()
{

	// ---- local
		INT result = 0;


	// ---- 1, 2, 3 & 4: "stream" geometry
		pi_model->handle_elem_stream_visible[0] = gfx->AddElement(
				pi_model->handle_dspl_stream_visible,
				"2D PanoRect",
				VS_OBJ_GEOMETRY,
				VS_OBJ_PANO_RECT_TOP,
				&pi_model->obj_config->Callback_PanoRect_Configure
			);
		pi_model->handle_elem_stream_visible[1] = gfx->AddElement(
				pi_model->handle_dspl_stream_visible,
				"2D PanoRect",
				VS_OBJ_GEOMETRY,
				VS_OBJ_PANO_RECT_BOTTOM,
				&pi_model->obj_config->Callback_PanoRect_Configure
			);
		pi_model->handle_elem_stream_visible[2] = gfx->AddElement(
				pi_model->handle_dspl_stream_visible,
				"3D Sphere",
				VS_OBJ_GEOMETRY,
				VS_OBJ_PANO_SPHERE,
				&pi_model->obj_config->Callback_PanoSphere_Configure
			);
		pi_model->handle_elem_stream_visible[3] = gfx->AddElement(
				pi_model->handle_dspl_stream_visible,
				"2D PanoRect",
				VS_OBJ_GEOMETRY,
				VS_OBJ_PANO_RECT_EQUI,
				&pi_model->obj_config->Callback_PanoRect_Configure
			);


	// ---- 5 & 6: outlines around [ 1, 2, 3 & 4 ] and [ 7 ]  
		FLOAT param[VS_MAX_ELM_FLOAT_PARAMETER];

		UINT v[2] = {
			vs_system::ElmContext::VERSION_OUTLINE,
			vs_system::ElmContext::VERSION_OUTLINE
		};

		for (UINT i = 0; i < 2; i++)
		{

			pi_model->handle_elem_stream_visible[4 + i] = gfx->AddElement(
					pi_model->handle_dspl_stream_visible,
					"",
					0,
					0,
					&pi_model->obj_config->Callback_Button_Configure
				);

			vs_system::ElmContext *elm = NULL;

			INT get_element_result = gfx->GetElementContext(
					pi_model->handle_elem_stream_visible[4 + i],
					&elm
				);

			if (get_element_result != NULL)
			{
				result = elm->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NONE);
				result = elm->SetVersion(v[i]);
				result = elm->SetRGBA(1.0f, 1.0f, 1.0f, 0.5f);


				result = pi_model->obj_config->ButtonSetDPT(
						param,
						vs_system::ElmContext::VIEWPORT_WIDTH |
							vs_system::ElmContext::VIEWPORT_CLIP
					);
				result = pi_model->obj_config->ButtonSetLocation(
						param,
						//-0.01f, 0.7f, 1.01f, 0.05f
						-0.01f, 0.7f, 0.0f, 0.0f
					);
				result = pi_model->obj_config->ButtonSetTexture(
						param,
						VS_ATLAS_PAGE_GROUP_TWO,
						0, 0, 0, 0, 1, 1, 0.01f
					);

				result = elm->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);
			}
		}


	// ---- 7: LAN rtsp

		pi_model->handle_elem_stream_visible[6] = gfx->AddElement(
				pi_model->handle_dspl_stream_visible,
				"",
				0,
				0,
				&pi_model->obj_config->Callback_Button_Configure
			);

		vs_system::ElmContext *elm = NULL;

		INT get_element_result = gfx->GetElementContext(
				pi_model->handle_elem_stream_visible[6],
				&elm
			);

		if (get_element_result != NULL)
		{
			result = elm->SetFontName("RTSP");
			result = elm->SetAlign(VS_ALIGN_CENTRE_ON_TEXT);
			result = elm->SetFontAlignment(vs_system::ElmContext::ALIGN_LEFT);
			result = elm->SetText("LAN: \"rtsp://0.0.0.0:0/stream_name\"");
			result = elm->SetBehaviour(vs_system::ElmContext::BEHAVIOUR_NONE);
			result = elm->SetVersion(vs_system::ElmContext::VERSION_TEXT);
			result = elm->SetRGBA(1.0f, 1.0f, 1.0f, 0.5f);

			result = pi_model->obj_config->ButtonSetDPT(
					param,
					vs_system::ElmContext::VIEWPORT_WIDTH
				);
			result = pi_model->obj_config->ButtonSetLocation(
					param,
					0.2f, 0.7f, 0.5f, 0.05f
				);
			result = pi_model->obj_config->ButtonSetTexture(
					param,
					VS_ATLAS_PAGE_GROUP_TWO,
					0, 0, 0, 0, 1, 1, 0.01f
				);

			result = elm->SetFloatParameter(VS_MAX_ELM_FLOAT_PARAMETER, param);
		}

	return VS_OK;
}
