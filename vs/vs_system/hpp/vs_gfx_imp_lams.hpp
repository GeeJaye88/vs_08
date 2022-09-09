////////////////////////////////////////////////////////////////////////////////


// ---------- SetAmbient ----------
/*!
\brief Set  gfx_interface scene ambient properties using VsAmbient
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetAmbient(VsAmbient *a)
{
	pi_props->gfx_ambient->Set(a);
	return VS_OK;
}


// ---------- SetFog ----------
/*!
\brief Set gfx_interface scene fog properties using VsFog
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetFog(VsFog *f)
{
	pi_props->gfx_fog->Set(f);
	return VS_OK;
}


// ---------- SetLight ----------
/*!
\brief Set a gfx_interface scene light properties using VsLight
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetLight(UINT index, VsLight *vs_light)
{
	INT result = pi_props->gfx_lights->Set(vs_light, index);
	return result;
}


// ---------- GetMaterial ----------
/*!
\brief Get a gfx_interface scene material properties using VsMaterial
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::GetMaterial(UINT index, VsMaterial *vs_material)
{
	INT result = pi_props->gfx_materials->Get(vs_material, index);
	return result;
}


// ---------- SetMaterial ----------
/*!
\brief Set a gfx_interface scene material properties using VsMaterial
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetMaterial(UINT index, VsMaterial *vs_material)
{
	INT result = pi_props->gfx_materials->Set(vs_material, index);
	return result;
}


// ---------- SetViewMatrix ----------
/*!
\brief Set gfx_interface scene view matrix selected
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetViewMatrix(UINT index, VsMatrix *vsm)
{

	if (index < VS_MAX_VIEW_MATRICES)
	{
		D3DXMATRIX VM;
		D3DXMATRIX *V = &VM;
		(*V)(0, 0) = vsm->m[0][0]; (*V)(0, 1) = vsm->m[0][1]; (*V)(0, 2) = vsm->m[0][2]; (*V)(0, 3) = 0.0f;
		(*V)(1, 0) = vsm->m[1][0]; (*V)(1, 1) = vsm->m[1][1]; (*V)(1, 2) = vsm->m[1][2]; (*V)(1, 3) = 0.0f;
		(*V)(2, 0) = vsm->m[2][0]; (*V)(2, 1) = vsm->m[2][1]; (*V)(2, 2) = vsm->m[2][2]; (*V)(2, 3) = 0.0f;
		(*V)(3, 0) = vsm->m[3][0]; (*V)(3, 1) = vsm->m[3][1]; (*V)(3, 2) = vsm->m[3][2]; (*V)(3, 3) = 1.0f;
 
		pi_props->gfx_view_matrices->_view_matrix[index] = *V;
	}
	else
	{
		return VS_ERROR;
	}

	return VS_OK;
}


// ---------- SetViewMatrix ----------
/*!
\brief Set gfx_interface scene view matrix selected
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetViewMatrix(UINT index, D3DXMATRIX *v)
{

	if (index < VS_MAX_VIEW_MATRICES)
	{
		pi_props->gfx_view_matrices->_view_matrix[index] = *v;
	}
	else
	{
		return VS_ERROR;
	}

	return VS_OK;
}


// ---------- SetAmbient ----------
/*!
\brief Set ambient light RGB levels 
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetAmbient(FLOAT red, FLOAT green, FLOAT blue)
{

	pi_props->gfx_device->d3d9_device->SetRenderState
			(
				D3DRS_AMBIENT,
				D3DCOLOR_COLORVALUE
					(
						red,
						green,
						blue,
						1.0
					)
			);

	return VS_OK;
}


// ---------- SetAmbientEnabled ----------
/*!
\brief Set ambient light enabled
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetAmbientEnabled(BOOL enabled)
{
	pi_props->gfx_ambient->ambient_enable = enabled;
	return VS_OK;
}


// ---------- SetFog ----------
/*!
\brief Set gfx_interface scene fog using previously set GfxFog
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetFog()
{

	// ---- local
		IDirect3DDevice9 *d3d9_device = pi_props->gfx_device->d3d9_device;
		GfxFog *gfx_fog = pi_props->gfx_fog;


	// ---- set the fog color.
		DWORD color = VS_D3DCOLOR_COLORVALUE(
				gfx_fog->color.r,
				gfx_fog->color.g,
				gfx_fog->color.b,
				gfx_fog->color.a
			);
		d3d9_device->SetRenderState(D3DRS_FOGCOLOR, color);


	// ---- set fog parameters.
		if ( gfx_fog->table_mode == D3DFOG_LINEAR )
		{
			d3d9_device->SetRenderState(D3DRS_FOGTABLEMODE,   gfx_fog->table_mode);
			d3d9_device->SetRenderState(D3DRS_FOGSTART,     *(DWORD *)(&gfx_fog->start));
			d3d9_device->SetRenderState(D3DRS_FOGEND,       *(DWORD *)((&gfx_fog->end)));
		}
		else
		{
			d3d9_device->SetRenderState(D3DRS_FOGTABLEMODE,   gfx_fog->table_mode);
			d3d9_device->SetRenderState(D3DRS_FOGDENSITY,   *(DWORD *)(&gfx_fog->density));
		}


	// --- only use D3DRS_RANGEFOGENABLE with vertex fog

	return VS_OK;
}


// ---------- SetFogEnabled ----------
/*!
\brief Set fog ON or OFF (TRUE or FALSE)
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetFogEnabled(BOOL f)
{
	pi_props->gfx_device->d3d9_device->SetRenderState(D3DRS_FOGENABLE, f);
	return VS_OK;
}


// ---------- SetLighting ----------
/*!
\brief Set lights ON or OFF (TRUE or FALSE)
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/ 
INT GfxInterface::SetLighting(BOOL l)
{
	pi_props->gfx_device->d3d9_device->SetRenderState(D3DRS_LIGHTING, l);
	pi_props->gfx_device->d3d9_device->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	return VS_OK;
}



// ---------- SetLightEnabled ----------
/*!
\brief Set light enabled
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetLightEnabled(UINT index, BOOL enable)
{
	pi_props->gfx_device->d3d9_device->SetLight(index, &pi_props->gfx_lights->light[index]);
	pi_props->gfx_device->d3d9_device->LightEnable(index, enable);
	return VS_OK;
}


// ---------- SetMaterial ----------
/*!
\brief Set material selected
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetMaterial(UINT index)
{
	pi_props->gfx_device->d3d9_device->SetMaterial(&pi_props->gfx_materials->material[index]);
	return VS_OK;
}


// ---------- SetViewMatrix ----------
/*!
\brief Set view matrix selected
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::SetViewMatrix(UINT index)
{
	pi_props->gfx_device->d3d9_device->SetTransform(D3DTS_VIEW, &pi_props->gfx_view_matrices->_view_matrix[index]);
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////

