////////////////////////////////////////////////////////////////////////////////


// ---------- r2_cam_props.h ----------
/*!
\file r2_cam_props.h
\brief application include files.
\author Gareth Edwards
*/


#ifndef REVIEW_02_CAM_PROPS
#define REVIEW_02_CAM_PROPS


// ---------- application include ----------
#include "../header/r2_model.h"


using namespace review_02;


INT Model::SetAppConfigPaf(CHAR *file_name)
{
	strcpy(pi_model->app_setup_config_paf, file_name);
	return VS_OK;
}


INT Model::GetAppConfigPaf(CHAR *file_name)
{
	strcpy(file_name, pi_model->app_setup_config_paf);
	return VS_OK;
}


INT Model::SetAppConfigCameraMode(INT mode)
{
	// ---- read application setup configuration file
	vs_system::VNodeIO vio;
	vs_system::VNode *config_hdl = vio.Read(s_pi_model->app_setup_config_paf);
	if (config_hdl == NULL) return VS_ERROR;


	// ---- get
	vs_system::VNode *p = NULL;
	vs_system::VNode *c = NULL;
	if (c = config_hdl->SearchName("Camera"))
	{
		if ( p = c->SearchName("Config-Mode") )
		{
			p->SetValue(mode);
			vio.Write(s_pi_model->app_setup_config_paf, config_hdl);
		}
	}

	return VS_OK;
}


INT Model::GetAppConfigCameraMode()
{

	// ---- read application setup configuration file
		vs_system::VNodeIO vio;
		vs_system::VNode *config_hdl = vio.Read(s_pi_model->app_setup_config_paf);
		if ( config_hdl == NULL ) return VS_ERROR;


	// ---- get
		vs_system::VNode *p = NULL;
		vs_system::VNode *c = NULL;
		if ( c = config_hdl->SearchName("Camera") )
		{
			if (p = c->SearchName("Config-Mode")) return p->GetIntFromValue();
		}

	return VS_ERROR;
}


// ---------- GetCamProps ----------
/*!
\brief Get camera props
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::GetCamProps(vs_system::CamProps **cam_props)
{
	*cam_props = pi_model->cam_props;
	return VS_OK;
}


// ---------- GetPropsConfigPaf ----------
/*!
\brief Get camera properties config path filename
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::GetPropsConfigPaf(CHAR *file_name)
{
	strcpy(file_name, pi_model->cam_props_config_paf);
	return VS_OK;
}


// ---------- ReadCamProps ----------
/*!
\brief Read camera properties
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::ReadCamProps(CHAR *file_name)
{
	strcpy(pi_model->cam_props_config_paf, file_name);
	INT result = pi_model->cam_props->Read(file_name);
	return result;
}


// ---------- SaveCamProps ----------
/*!
\brief Save camera properties
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SaveCamProps()
{
	INT result = pi_model->cam_props->Write(pi_model->cam_props_config_paf);
	return VS_OK;
}

INT Model::TestSaveCamProps()
{
	INT result = pi_model->cam_props->Write("test.hdl");
	return VS_OK;
}

#endif // REVIEW_02_CAM_PROPS


