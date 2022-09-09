////////////////////////////////////////////////////////////////////////////////

// ---------- r2_setup_pano.hpp ----------
/*!
\file r2_setup_pano.hpp
\brief
\author Gareth Edwards
*/


#pragma once

#include "../header/r2_model.h"

using namespace review_02;


// ---------- SetupPanoExposureAoi ----------
/*!
\brief Setup panoramic exposure AOI
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupPanoExposureAoi()
{

	// ---- local
		INT result;


	// ---- get camera properties
		vs_system::CamProps *cam_props = NULL;
		result = GetCamProps(&cam_props);


	// ---- get exposure param
		FLOAT exp_min_aoi = 0;
		result = cam_props->GetProperty(
				vs_system::CamProps::ANNULAR_AOI,
				0,
				vs_system::CamProps::MIN,
				&exp_min_aoi
			);
		FLOAT exp_max_aoi = 0;
		result = cam_props->GetProperty(
				vs_system::CamProps::ANNULAR_AOI,
				0,
				vs_system::CamProps::MAX,
				&exp_max_aoi
			);
		FLOAT exp_zone_f = 0;
		result = cam_props->GetProperty(
				vs_system::CamProps::ANNULAR_AOI,
				0,
				vs_system::CamProps::ZONE,
				&exp_zone_f
			);
		UINT exp_zone = (UINT)exp_zone_f;


	// ---- set exposure
		result = GetObjConfig()->SetPanoExposureAoi(exp_min_aoi, exp_max_aoi);
		result = GetObjConfig()->SetPanoExposureAoiZone(exp_zone);

	return VS_OK;
}


// ---------- SetupPanoFocusAoi ----------
/*!
\brief Setup panoramic focus AOI
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupPanoFocusAoi()
{

	// ---- local
	INT result;


	// ---- get camera properties
		vs_system::CamProps *cam_props = NULL;
		result = GetCamProps(&cam_props);


	// ---- get focus param
		FLOAT focus_min_aoi = 0;
		result = cam_props->GetProperty(
				vs_system::CamProps::ANNULAR_FOCUS,
				0,
				vs_system::CamProps::MIN,
				&focus_min_aoi
			);
		FLOAT focus_max_aoi = 0;
		result = cam_props->GetProperty(
				vs_system::CamProps::ANNULAR_FOCUS,
				0,
				vs_system::CamProps::MAX,
				&focus_max_aoi
			);
		FLOAT focus_rotate = 0;
		result = cam_props->GetProperty(
				vs_system::CamProps::ANNULAR_FOCUS,
				0,
				vs_system::CamProps::ROTATE,
				&focus_rotate
			);
		FLOAT focus_sector = 0;
		result = cam_props->GetProperty(
				vs_system::CamProps::ANNULAR_FOCUS,
				0,
				vs_system::CamProps::SECTOR,
				&focus_sector
			);


	// ---- set focus param
		result = GetObjConfig()->SetPanoFocusAoi(focus_min_aoi, focus_max_aoi);
		result = GetObjConfig()->SetPanoFocusAoiRotate(focus_rotate);
		result = GetObjConfig()->SetPanoFocusAoiSector(focus_sector);

	return VS_OK;
}


// ---------- SetupPanoGeometry ----------
/*!
\brief Setup panoramic geometry display lists
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupPanoGeometry()
{

	// ---- local
		INT result;
		INT hr;


	// ---- lambda ------------------------------------------------------------
		auto annular_elements = [&](INT handle)
		{
			hr = gfx->AddElement(
					handle,
					"2D PanoAnnular",
					VS_OBJ_GEOMETRY,
					VS_OBJ_PANO_ANNULAR,
					&pi_model->obj_config->Callback_PanoAnnular_Configure
				);
			hr = gfx->AddElement(
					handle,
					"Annular Aoi",
					VS_OBJ_GEOMETRY,
					VS_OBJ_PANO_ANNULAR_AOI,
					&pi_model->obj_config->Callback_PanoAnnularAoi_Configure
				);
			hr = gfx->AddElement(
					handle,
					"Annular Mask",
					VS_OBJ_GEOMETRY,
					VS_OBJ_PANO_ANNULAR_MASK,
					&pi_model->obj_config->Callback_PanoAnnularMask_Configure
				);
		};


	// ---- lambda ------------------------------------------------------------
		auto annular_exposure_elements = [&](INT handle)
		{
			hr = gfx->AddElement(
					handle,
					"Annular Exposure",
					VS_OBJ_GEOMETRY,
					VS_OBJ_PANO_ANNULAR_EXPOSURE_AOI,
					&pi_model->obj_config->Callback_PanoExposureAec_T_Configure
			);
		};


	// ---- lambda ------------------------------------------------------------
		auto annular_focus_aoi_elements = [&](INT handle)
		{
			hr = gfx->AddElement(
					handle,
					"Annular Focus",
					VS_OBJ_GEOMETRY,
					VS_OBJ_PANO_ANNULAR_FOCUS_AOI,
					&pi_model->obj_config->Callback_PanoExposureAec_T_Configure
				);
		};


	// ---- lambda ------------------------------------------------------------
		auto annular_focus_grf_elements = [&](INT handle)
		{

			INT num_bars = 256;

			// ---- camera graphs : float buffer
				vs_system::CamGraphs *cam_graphs;
				GetCamGraphs(&cam_graphs);

				vs_system::FloatBuffer *float_buffer = NULL;
				result = cam_graphs->GetFloatBuffer(
						vs_system::CamGraphs::FOCUS,
						&float_buffer
					);

				float_buffer->Initialise(num_bars, 1, 1);
				float_buffer->CreateBuffer();


			// ---- grf element
				hr = gfx->AddElement(
						handle,
						"Annular Graph",
						VS_OBJ_GEOMETRY,
						VS_OBJ_PANO_ANNULAR_FOCUS_GRF,
						&pi_model->obj_config->Dbz_Callback_Configure_BarGraph
					);

				vs_system::ElmContext *elm = NULL;
				result = gfx->GetElementContext(hr, &elm);


			// ---- initialise grf dashboard param
				vs_system::ObjConfig *oc = pi_model->obj_config;
				FLOAT *p = elm->GetFloatParameter();

				FLOAT config =
					vs_system::ObjConfig::DBZ_CONFIG_SHAPE +
						vs_system::ObjConfig::DBZ_CONFIG_AREA +
							vs_system::ObjConfig::DBZ_CONFIG_BG;

				result = oc->Dbz_SetDefault(p);
				result = oc->Dbz_SetConfig(p, config); 
				result = oc->Dbz_SetVisible(p, 1);
				result = oc->Dbz_SetScale(p, 1.0f, 1.0f);
				result = oc->Dbz_SetTranslate(p, -1.0f, -0.5f);
				result = oc->Dbz_SetBarGraphNum(p, (FLOAT)num_bars);
				result = oc->Dbz_SetBarGraphWidthEdge(p, 0.005f);
				result = oc->Dbz_SetBarGraphWidthSpace(p, 0.001f);
				result = oc->Dbz_SetBarGraphWidthMargin(p, 0.1f);
				result = oc->Dbz_SetBarGraphXArea(p, 0.1f, 0.9f);
				result = oc->Dbz_SetBarGraphYOffset(p, -0.05f);
				result = oc->Dbz_SetBarGraphDimension(p, 2.0f, 0.5f);

				FLOAT c[3] = { 0.15f, 0.4f, 1.0f };
				FLOAT s[4] = { 1.0f, 1.0f, 0.5f, 0.3f };
				FLOAT f[4] = { 0.7f, 0.7f, 0.5f, 0.3f };
				result = oc->Dbz_SetBarGraphEdgeRGBF  (p, c[0]*s[0], c[1]*s[0], c[2]*s[0], f[0]);
				result = oc->Dbz_SetBarGraphShapeRGBF (p, c[0]*s[1], c[1]*s[1], c[2]*s[1], f[1]);
				result = oc->Dbz_SetBarGraphAreaRGBF  (p, c[0]*s[2], c[1]*s[2], c[2]*s[2], f[2]);
				result = oc->Dbz_SetBarGraphBgRGBF    (p, c[0]*s[3], c[1]*s[3], c[2]*s[3], f[3]);

			return hr;
		};


	// ---- add ANNULAR list
		pi_model->handle_dspl_pano_annu = gfx->AddDisplayList("Annular");
			annular_elements(pi_model->handle_dspl_pano_annu);


	// ---- add ANNULAR EXPOSURE list
		pi_model->handle_dspl_pano_annu_exp_aoi = gfx->AddDisplayList("Annular Exposure");
			annular_exposure_elements(pi_model->handle_dspl_pano_annu_exp_aoi);


	// ---- add annular focus aoi list 
		pi_model->handle_dspl_pano_annu_foc_aoi = gfx->AddDisplayList("Annular Focus");
			annular_focus_aoi_elements(pi_model->handle_dspl_pano_annu_foc_aoi);


	// ---- add annular focus graph list
		pi_model->handle_dspl_pano_annu_foc_grf = gfx->AddDisplayList("Annular Graph");
		pi_model->handle_elem_pano_annu_foc_grf = 
			annular_focus_grf_elements(pi_model->handle_dspl_pano_annu_foc_grf);


	// ---- add SPHERICAL list
		pi_model->handle_dspl_pano_spherical = gfx->AddDisplayList("Spherical");
			pi_model->handle_elem_pano_spherical = gfx->AddElement(
					pi_model->handle_dspl_pano_spherical,
					"3D PanoSphere",
					VS_OBJ_GEOMETRY,
					VS_OBJ_PANO_SPHERE,
					&pi_model->obj_config->Callback_PanoSphere_Configure
				);


	// ---- add LIDAR list
		pi_model->handle_dspl_pano_lidar = gfx->AddDisplayList("Lidar");
		#if VS_LIDAR
			pi_model->handle_elem_pano_lidar = gfx->AddElement(
					pi_model->handle_dspl_pano_lidar,
					"3D PanoLidar",
					VS_OBJ_GEOMETRY,
					VS_OBJ_PANO_SPHERE,
					&pi_model->obj_config->Callback_PanoLidar_Configure
				);
		#endif


	// ---- add CYLINDRICAL list
		pi_model->handle_dspl_pano_cylindrical = gfx->AddDisplayList("Cylindrical");
		hr = gfx->AddElement(
				pi_model->handle_dspl_pano_cylindrical,
				"2D PanoCylinder",
				VS_OBJ_GEOMETRY,
				VS_OBJ_PANO_CYLINDER,
				&pi_model->obj_config->Callback_PanoCylinder_Configure
			);
		hr = gfx->AddElement(
				pi_model->handle_dspl_pano_cylindrical,
				"2D PanoCylinderTopRing",
				VS_OBJ_GEOMETRY,
				VS_OBJ_PANO_CYLINDER_TOP_RING,
				&pi_model->obj_config->Callback_PanoCylinderTopRing_Configure
			);


	// ---- add PANORAMA list
		pi_model->handle_dspl_pano_rect = gfx->AddDisplayList("Panorama");
			pi_model->handle_elem_pano_rect = gfx->AddElement(
					pi_model->handle_dspl_pano_rect,
					"2D PanoRect",
					VS_OBJ_GEOMETRY,
					VS_OBJ_PANO_RECT,
					&pi_model->obj_config->Callback_PanoRect_Configure
			);
			FLOAT f[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
			result = gfx->SetElementParameters(pi_model->handle_elem_pano_rect, 8, f);


	// ---- add PANO STREAM HIDDEN (underneath) list
		pi_model->handle_dspl_stream_hidden = gfx->AddDisplayList("Stream Hidden");
			hr = SetupStreamHiddenLists();


	// ---- add PANO STREAM VISIBLE (toggled via PanelContext status) list
		pi_model->handle_dspl_stream_visible = gfx->AddDisplayList("Stream Visible");
			hr = SetupStreamVisibleLists();


	// ---- add SHARED HIDDEN (underneath) list
		pi_model->handle_dspl_shared_hidden = gfx->AddDisplayList("Shared Hidden");
			hr = SetupSharedHiddenLists();


	// ---- add PANO FRONT list
		pi_model->handle_dspl_pano_front = gfx->AddDisplayList("Panorama Front");
			hr = gfx->AddElement(
					pi_model->handle_dspl_pano_front,
					"2D PanoRect",
					VS_OBJ_GEOMETRY,
					VS_OBJ_PANO_RECT_FRONT,
					&pi_model->obj_config->Callback_PanoRect_Configure
				);


	// ---- add PANO BACK list
		pi_model->handle_dspl_pano_back = gfx->AddDisplayList("Panorama Back");
			hr = gfx->AddElement(
					pi_model->handle_dspl_pano_back,
					"2D PanoRect",
					VS_OBJ_GEOMETRY,
					VS_OBJ_PANO_RECT_BACK,
					&pi_model->obj_config->Callback_PanoRect_Configure
				);

	return VS_OK;
}
