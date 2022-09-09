////////////////////////////////////////////////////////////////////////////////

// ---------- r2_cam_obj_config.hpp ----------
/*!
\file r2_cam_obj_config.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- application include ----------
#include "../header/r2_model.h"


using namespace review_02;


////////////////////////////////////////////////////////////////////////////////


// ---------- GetObjConfig ----------
/*!
\brief Get object ObjConfig
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
vs_system::ObjConfig *Model::GetObjConfig()
{
	return pi_model->obj_config;
}


// ---------- ReportObjConfigPanoConfig ----------
/*!
\brief Report ObjConfig Pano Configuration
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::ReportObjConfigPanoConfig()
{

	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

		;

	#else

	// ---- local ----
	INT result = 0;


	// ---- local ----
	vs_system::AppDependency *app = pi_model->app_dependency;
	if (app == NULL) return VS_ERROR;
	vs_system::SysOutput *sys_output = app->GetSysOutput();
	if (sys_output == NULL) return VS_ERROR;


	// ---- get ----
	FLOAT min_angle, max_angle, angle_at_unit_radius;
	result = pi_model->obj_config->GetPanoAngles(&min_angle, &max_angle);
	result = pi_model->obj_config->GetPanoAngleAtUnitRadius(&angle_at_unit_radius);
	FLOAT pan_derived, tilt_derived, fov_derived;
	result = pi_model->obj_config->GetPanoPTFDerived(&pan_derived, &tilt_derived, &fov_derived);


	// ---- report ----
	CHAR msg[VS_MAXCHARLEN];
	{
		sys_output->Message("Pano PTF parameters:-");
			sys_output->Message("   At max radius:");
				sprintf(msg, "      - angle look up is %6.2f", max_angle);
				sys_output->Message(msg);
			sys_output->Message("   And derived:");
			sys_output->Message(msg);
				sprintf(msg, "      - angle look down is %6.2f", min_angle);
				sys_output->Message(msg);
				sprintf(msg, "      - tilt is %6.2f", tilt_derived);
				sys_output->Message(msg);
				sprintf(msg, "      - fov is %6.2f", fov_derived);
			sys_output->Message(msg);
			sys_output->Message("   At unit radius:");
				sprintf(msg, "      - angle is %6.2f", angle_at_unit_radius);
				sys_output->Message(msg);
				sprintf(msg, "      - tilt is %6.2f", (angle_at_unit_radius + min_angle) / 2);
				sys_output->Message(msg);
				sprintf(msg, "      - fov is %6.2f", angle_at_unit_radius - min_angle);
				sys_output->Message(msg);
	}

	#endif

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- GetConfigFiles ----------
/*!
\brief Set configuration path/files object
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::GetConfigFiles(ConfigFiles **cf)
{
	*cf = pi_model->config_files;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- SaveObjectAndCameraConfig ----------
/*!
\brief Save panoramic and camera properties configuration
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SaveObjectAndCameraConfig()
{

	// ---- local
		INT result = VS_OK;


	// ---- output
	vs_system::AppDependency *app = pi_model->app_dependency;
		if (app == NULL) return VS_ERROR;
	vs_system::SysOutput *sys_output = app->GetSysOutput();
		if (sys_output == NULL) return VS_ERROR;


	// ---- local path/filenames
		CHAR obj_config_paf[VS_MAXCHARPATHLEN];
		CHAR cam_props_config_paf[VS_MAXCHARPATHLEN];


	// ---- get config path filenames
		review_02::ConfigFiles *config_files = NULL;
		result = GetConfigFiles(&config_files);
		result = config_files->GetPropsConfigHdlFilename(cam_props_config_paf);
		result = config_files->GetObjectConfigHdlFilename(obj_config_paf);


	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

	;

	#else


	// ========== Update panoramic config file ==========
	{

		// get pano data
		FLOAT min_radius = 0, max_radius = 0;
		FLOAT min_aoi    = 0, max_aoi    = 0;
		FLOAT u_offset   = 0, v_offset   = 0;
		FLOAT max_angle  = 0;
		vs_system::ObjConfig *obj_config = GetObjConfig();
		result = obj_config->GetPanoRadii(&min_radius, &max_radius);
		#ifdef GET_PANO_EXPOSURE
		result = obj_config->GetPanoExposureAoi(&min_aoi, &max_aoi);
		#endif
		result = obj_config->GetPanoUVOffset(&u_offset, &v_offset);
		result = obj_config->GetPanoAngleMax(&max_angle);


		// read
		vs_system::VNodeIO vio;
		vs_system::VNode* config_pano_hdl = vio.Read(obj_config_paf);


		// IF pano config file NOT READ THEN report && return
		if (config_pano_hdl == NULL)
		{
			sys_output->MessageError(
					"FAILED to READ pano config file.",
					"File",
					obj_config_paf
				);
			return VS_ERROR;
		}

		// ELSE parse pano data
		else
		{
			// update
			vs_system::VNode *mapping = config_pano_hdl->SearchName("Mapping");
			if (mapping)
			{
				CHAR value[VS_MAXCHARLEN];
				vs_system::VNode *item = NULL;
				if (item = mapping->SearchName("Max-Angle") ) { sprintf(value, "%f", max_angle);  item->SetValue(value); };
				if (item = mapping->SearchName("Min-Radius")) { sprintf(value, "%f", min_radius); item->SetValue(value); };
				if (item = mapping->SearchName("Max-Radius")) { sprintf(value, "%f", max_radius); item->SetValue(value); };
				#ifdef GET_PANO_EXPOSURE
				if (item = mapping->SearchName("Min-Aoi")   ) { sprintf(value, "%f", min_aoi);    item->SetValue(value); };
				if (item = mapping->SearchName("Max-Aoi")   ) { sprintf(value, "%f", max_aoi);    item->SetValue(value); };
				#endif
				if (item = mapping->SearchName("U-Offset")  ) { sprintf(value, "%f", u_offset);   item->SetValue(value); };
				if (item = mapping->SearchName("V-Offset")  ) { sprintf(value, "%f", v_offset);   item->SetValue(value); };
			}


			// write
			INT result = vio.Write(obj_config_paf, config_pano_hdl);


			// delete
			config_pano_hdl->Delete();


			// IF pano config file NOT WRITTEN THEN report && return
			if (result != VS_OK)
			{
				sys_output->MessageError(
						"FAILED to WRITE pano config file.",
						"File",
						obj_config_paf
					);
				return VS_ERROR;
			}
			// ELSE report Pano configuration update at console
			else
			{
				printf("Updated pano config parameters:\n");
				printf("-------------------------------\n");
				printf("Radius Min is:%6.5f\n", min_radius);
				printf("Radius Max is:%6.5f\n", max_radius);
				printf("Aoi Min is:%6.5f\n", min_aoi);
				printf("Aoi Max is :%6.5f\n", max_aoi);
				printf("U Offset is:%6.5f\n", u_offset);
				printf("V Offset is:%6.5f\n", v_offset);
				printf("Max Angle is:%6.5f\n", max_angle);
				printf("-------------------------------\n");
				printf("\n");
			}

		}
	}


	#endif


	// ========== Update camera properties config path/file ==========
	{

		// get pano data
		INT h_offset = 0;
		result = GetObjConfig()->GetPanoCamHOffset(&h_offset);

		// read
		vs_system::VNodeIO vio;
		vs_system::VNode* config_props_hdl = vio.Read(cam_props_config_paf);

		// IF camera properties config file NOT READ THEN report && return
		if (config_props_hdl == NULL)
		{
			sys_output->MessageError(
					"FAILED to READ camera properties config file.",
					"File",
					cam_props_config_paf
				);
			return VS_ERROR;
		}
		// ELSE parse camera properties config data
		else
		{
			// update
			vs_system::VNode *roi = config_props_hdl->SearchName("ROI");
			if (roi)
			{
				CHAR value[VS_MAXCHARLEN];
				vs_system::VNode *item = NULL;
				if (item = roi->SearchName("H-Off"))
				{
					sprintf(value, "%f", (FLOAT)h_offset);  item->SetValue(value);
				};
			}


			// write
			INT result = vio.Write(cam_props_config_paf, config_props_hdl);


			// delete
			config_props_hdl->Delete();


			// IF camera properties config file NOT WRITTEN THEN report && return
			if (result != VS_OK)
			{
				sys_output->MessageError(
						"FAILED to WRITE camera properties config file.",
						"File",
						cam_props_config_paf
					);
				return VS_ERROR;
			}
			// ELSE report camera properties configuration update at console
			else
			{
				printf("Updated camera config properties parameter:\n");
				printf("-------------------------------------------\n");
				printf("H Off is:%d\n", h_offset);
				printf("-------------------------------------------\n");
				printf("\n");
			}
		}
	}

	return VS_OK;
}

