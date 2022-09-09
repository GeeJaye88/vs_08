////////////////////////////////////////////////////////////////////////////////


// ---------- r2_img_props.hpp ----------
/*!
\file r2_img_props.hpp
\brief application include files.
\author Gareth Edwards
*/


#pragma once


// ---------- application include ----------
#include "../header/r2_model.h"


using namespace review_02;


// ---------- GetImgProps ----------
/*!
\brief Get image props
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::GetImgProps(vs_system::ImgProps **img_props)
{
	*img_props = pi_model->img_props;
	return VS_OK;
}


// ---------- GetImgPropsConfigPaf ----------
/*!
\brief Get image properties config path filename
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::GetImgPropsConfigPaf(CHAR *file_name)
{
	strcpy(file_name, pi_model->img_props_config_paf);
	return VS_OK;
}


// ---------- SetImgPropsShaders ----------
/*!
\brief Set image properties shaders
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetImgPropsShaders()
{

	// ---- local
		INT hr = 0;


	// ---- BCS shader param
		FLOAT shader_param[8];
		FLOAT *b = &shader_param[0];
		FLOAT *c = &shader_param[1];
		FLOAT *s = &shader_param[2];


	using namespace vs_system;


	// ----get image properties BCS
		vs_system::ImgProps *img_props = NULL;
		hr = GetImgProps(&img_props);

		FLOAT value = 0;
		hr = img_props->GetProperty(ImgProps::GFX_SHADER, 0, ImgProps::MODE, &value);
		UINT config_mode = (UINT)value;
		UINT config_index = config_mode - 1;
		hr = img_props->GetProperty(ImgProps::GFX_SHADER, config_index, ImgProps::BRIGHTNESS, &value);
		*b = value;
		hr = img_props->GetProperty(ImgProps::GFX_SHADER, config_index, ImgProps::CONTRAST,   &value);
		*c = value;
		hr = img_props->GetProperty(ImgProps::GFX_SHADER, config_index, ImgProps::SATURATION, &value);
		*s = value;


	// ---- set shader BCS
		hr = SetGfxShaderParam(0, shader_param);

	return VS_OK;
}


// ---------- ReadImgProps ----------
/*!
\brief Read image properties
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::ReadImgProps(CHAR *file_name)
{
	if (strlen(file_name) <= 0) return VS_ERROR;
	strcpy(pi_model->img_props_config_paf, file_name);
	INT result = pi_model->img_props->Read(file_name);
	return result;
}


// ---------- SaveImgProps ----------
/*!
\brief Save image properties
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SaveImgProps()
{
	if (strlen(pi_model->img_props_config_paf) <= 0) return VS_ERROR;
	INT result = pi_model->img_props->Write(pi_model->img_props_config_paf);
	return VS_OK;
}


// ---------- SetupImgExposureAoi ----------
/*!
\brief Setup image exposure AOI
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupImgExposureAoi()
{

	// ---- local
		INT result;


	// ---- get camera properties
		vs_system::CamProps *cam_props = NULL;
		result = GetCamProps(&cam_props);


	// --- get aoi zone
		FLOAT aoi_zone_f = 0;
		result = cam_props->GetProperty(
				vs_system::CamProps::AOI,
				0,
				vs_system::CamProps::ZONE,
				&aoi_zone_f
			);
		UINT aoi_zone = (UINT)aoi_zone_f;


	// ---- set aoi zone
		result = GetObjConfig()->SetPanoExposureAoiZone(aoi_zone);

	return VS_OK;
}



