////////////////////////////////////////////////////////////////////////////////

// ---------- s1_model_img_props.hpp ----------
/*!
\file s1_model_img_props.hpp
\brief Implementation of the Camera methods
\author Gareth Edwards
*/


// ========================================================================== //


// ---------- GetGfxShaderParam ----------
/*!
\brief Get gfx shader parameters
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::GetGfxShaderParam(UINT index, FLOAT *param)
{
	vs_system::GfxInterface *gfx = pi_model->app_dependency->GetGfxInterface();
	gfx->GetGfxShaderParam(index, param);
	return VS_OK;
}


// ---------- SetGfxShaderParam ----------
/*!
\brief Set gfx shader parameters
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetGfxShaderParam(UINT index, FLOAT *param)
{
	vs_system::GfxInterface *gfx = pi_model->app_dependency->GetGfxInterface();
	gfx->SetGfxShaderParam(index, param);
	return VS_OK;
}


// ========================================================================== //


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
INT Model::ReadImgProps()
{

	vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
	CHAR *app_name = sys_context->GetChar(vs_system::SysContext::APP_NAME);

	CHAR  paf_name[VS_MAXCHARPATHLEN];
	CHAR  paf_prefix[4];
	CHAR *paf_format = "%sdata\\%s\\img_config.hdl";

	if ( IsDebuggerPresent()  )
	{
		strcpy(paf_prefix,"..\\");
	}
	else
	{
		strcpy(paf_prefix, "");
	}

	sprintf(
			paf_name,
			paf_format,
			paf_prefix,
			app_name
		);

	INT result = pi_model->img_props->Read(paf_name);

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

	vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
	CHAR *app_name = sys_context->GetChar(vs_system::SysContext::APP_NAME);

	CHAR  paf_name[VS_MAXCHARPATHLEN];
	CHAR  paf_prefix[4];
	CHAR *paf_format = "%sdata\\%s\\img_config.hdl";

	if ( IsDebuggerPresent()  )
	{
		strcpy(paf_prefix,"..\\");
	}
	else
	{
		strcpy(paf_prefix, "");
	}

	sprintf(
			paf_name,
			paf_format,
			paf_prefix,
			app_name
		);

	INT result = pi_model->img_props->Write(paf_name);

	return result;
}


// ========================================================================== //