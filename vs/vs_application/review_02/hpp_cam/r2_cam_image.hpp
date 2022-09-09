////////////////////////////////////////////////////////////////////////////////

// ---------- r2_cam_image.hpp ----------
/*!
\file r2_cam_image.hpp
\brief
\author Gareth Edwards
*/


#pragma once


// ---------- application include ----------
#include "../header/r2_model.h"


using namespace review_02;


////////////////////////////////////////////////////////////////////////////////


// ---------- ReportImageConfig ----------
/*!
\brief Report Image Configuration
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::ReportImageConfig()
{

	// ---- local ----
	INT result = 0;


	// ---- local ----
	vs_system::AppDependency *app = pi_model->app_dependency;
	if (app == NULL) return VS_ERROR;
	vs_system::SysOutput *sys_output = app->GetSysOutput();
	if (sys_output == NULL) return VS_ERROR;


	// ---- report ----

	return VS_OK;
}



////////////////////////////////////////////////////////////////////////////////


// ---------- SaveImageConfig ----------
/*!
\brief Save image configuration
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SaveImageConfig()
{

	// ---- local ----
	INT result = VS_OK;


	// ---- output ----
	vs_system::AppDependency *app = pi_model->app_dependency;
	if (app == NULL) return VS_ERROR;
	vs_system::SysOutput *sys_output = app->GetSysOutput();
	if (sys_output == NULL) return VS_ERROR;


	// ---- local path/filenames ----
	CHAR image_config_paf[VS_MAXCHARPATHLEN];


	// ---- get config path filenames ----
	review_02::ConfigFiles *config_files = NULL;
	result = GetConfigFiles(&config_files);
	config_files->GetImageConfigHdlFilename(image_config_paf);


	// ========== Update panoramic config file ==========
	{

		// read
		vs_system::VNodeIO vio;
		vs_system::VNode* image_config_hdl = vio.Read(image_config_paf);


		// IF pano config file NOT READ THEN report && return
		if ( image_config_hdl == NULL )
		{
			sys_output->MessageError(
					"FAILED to READ image config file.",
					"File",
					image_config_paf
				);
			return VS_ERROR;
		}

		// ELSE parse image data
		else
		{
			// update
			vs_system::VNode *mapping = image_config_hdl->SearchName("Mapping");
			if (mapping)
			{
				CHAR value[VS_MAXCHARLEN];
				vs_system::VNode *item = NULL;
				FLOAT temp_value = 0;
				if (item = mapping->SearchName("Temp-Angle")) { sprintf(value, "%f", temp_value);  item->SetValue(value); };

			}


			// write
			INT result = vio.Write(image_config_paf, image_config_hdl);


			// delete
			image_config_hdl->Delete();


			// IF image config file NOT WRITTEN THEN report && return
			if (result != VS_OK)
			{
				sys_output->MessageError(
						"FAILED to WRITE image config file.",
						"File",
						image_config_paf
					);
				return VS_ERROR;
			}
			// ELSE report configuration update at console
			else
			{
				printf("Updated image config parameters:\n");
				printf("\n");
			}

		}
	}


	return VS_OK;
}

