////////////////////////////////////////////////////////////////////////////////

// ---------- r2_gfx_get_set.hpp ----------
/*!
\file r2_gfx_get_set.hpp
\brief 
\author Gareth Edwards
*/


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

