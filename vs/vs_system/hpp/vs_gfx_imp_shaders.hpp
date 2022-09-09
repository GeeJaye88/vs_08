////////////////////////////////////////////////////////////////////////////////


// ---------- GetGfxShaders ---------
INT GfxInterface::GetGfxShaderParam(UINT index, FLOAT *param)
{
	pi_props->gfx_shaders[index]->GetParam(param);
	return VS_OK;
}


// ---------- SetGfxShaders ---------
INT GfxInterface::SetGfxShaderParam(UINT index, FLOAT *param)
{
	pi_props->gfx_shaders[index]->SetParam(param);
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////

