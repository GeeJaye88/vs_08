////////////////////////////////////////////////////////////////////////////////

// ---------- r2_dsp_lidar.hpp ----------
/*!
\file r2_dsp_lidar.hpp
\brief
\author Gareth Edwards
*/


#include "../header/r2_model.h"

using namespace review_02;


INT Model::SetCsvObject(vs_system::CsvObject *csv_buffer)
{
	pi_model->csv_object = csv_buffer;
	return VS_OK;
}


INT Model::SetLidarOuster(vs_system::LidarOuster *lidar_ouster)
{
	pi_model->lidar_ouster = lidar_ouster;
	return VS_OK;
}


// ---------- Update_PanoLidar ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
\note UPDATE ONLY TEXT
*/
INT Model::Update_PanoLidar(vs_system::CsvObject *csv_buffer)
{
	#if VS_LIDAR

	// ---- local
		INT result = VS_OK;


	// --- context
		vs_system::ElmContext *elm_context = NULL;
		result = s_pi_model->gfx->GetElementContext(pi_model->handle_elem_pano_lidar, &elm_context);


	// --- get lidar csv buffer
		FLOAT *csv_fb      = NULL;
		result = csv_buffer->GetBuffer(&csv_fb);


	// ---- update lidar element
		LONG *l = elm_context->GetLongParameter();
		*(l + 0) = (LONG)csv_fb;
		*(l + 1) = (LONG)csv_buffer->GetRows();
		*(l + 2) = (LONG)csv_buffer->GetColumns();
		*(l + 3) = (LONG)csv_buffer->GetExtent();
		*(l + 4) = (LONG)csv_buffer->GetRowsPerRecord();
		*(l + 5) = (LONG)csv_buffer->GetRead();

	// ---- lighting and materials
		//hr = elm_context->SetLit(FALSE);
		//hr = elm_context->SetCullMode(vs_system::ElmContext::CULL_NONE);
		//hr = elm_context->SetMaterialIndex(0);


	#endif

	return VS_OK;
}


// ---------- Display_PanoLidar ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
\note UPDATE ONLY TEXT
*/
INT Model::Display_PanoLidar(UINT *lidar_visible_mode)
{

	#if VS_LIDAR

	// ---- local
		INT result = VS_OK;


	// --- get pano context & force reconfig
		vs_system::ElmContext *pano_elm_context = NULL;
		s_pi_model->gfx->GetElementContext(pi_model->handle_elem_pano_spherical, &pano_elm_context);
		pano_elm_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
		pano_elm_context->SetConfigured(FALSE);


	// --- get lidar context & force reconfig
		vs_system::ElmContext *lidar_elm_context = NULL;
		s_pi_model->gfx->GetElementContext(pi_model->handle_elem_pano_lidar, &lidar_elm_context);
		lidar_elm_context->SetBitmask(VS_ELM_SETUP_GEOMETRY);
		lidar_elm_context->SetConfigured(FALSE);


	// ---- flip
		switch ( *lidar_visible_mode)
		{
			case 1:
				{
					*lidar_visible_mode = 2;
					pano_elm_context->SetVisible(FALSE);
					lidar_elm_context->SetVisible(TRUE);
				}
				break;
			case 2:
				{
					*lidar_visible_mode = 3;
					pano_elm_context->SetVisible(TRUE);
					lidar_elm_context->SetVisible(FALSE);
				}
				break;
			case 3:
				{
					*lidar_visible_mode = 1;
					pano_elm_context->SetVisible(TRUE);
					lidar_elm_context->SetVisible(TRUE);
				}
				break;
		}

	// ---- update icon
		Set_FrameIcons(
			FRAMES_BUTTON_INDEX,
			"8",
			LIDAR_BUTTON_GROUP_ID,
			8,
			1, *lidar_visible_mode - 1
		);

	#endif

	return VS_OK;
}


// ---------- Rotate_PanoLidar ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
\note UPDATE ONLY TEXT
*/
INT Model::Rotate_PanoLidar(UINT *lidar_rotate_mode)
{

	#if VS_LIDAR

	// ---- change mode
		switch (*lidar_rotate_mode)
		{
			case 1:
				{
					*lidar_rotate_mode = 2;
					pi_model->lidar_pan = FALSE;
					pi_model->lidar_pan_incr = 0.0f;
				}
				break;
			case 2:
				{
					*lidar_rotate_mode = 3;
					pi_model->lidar_pan = TRUE;
					pi_model->lidar_pan_incr = -0.1f;

				}
				break;
			case 3:
				{
					*lidar_rotate_mode = 1;
					pi_model->lidar_pan = TRUE;
					pi_model->lidar_pan_incr = 0.1f;
				}
				break;
		}

	// ---- update icon
		Set_FrameIcons(
			FRAMES_BUTTON_INDEX,
			"9",
			LIDAR_BUTTON_GROUP_ID,
			9,
			3, *lidar_rotate_mode - 1
		);

	#endif

	return VS_OK;
}
