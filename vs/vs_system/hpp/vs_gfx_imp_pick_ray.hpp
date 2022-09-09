////////////////////////////////////////////////////////////////////////////////

//
// NOTE:
//
//    The ray/object intersection calculations are
//    handled in the IsOver3DElement method.
//

// ---------- CreateScreenSpacePickRay ----------
/*!
\brief Initialise GfxInterface::pi_props screen space VsVector3 viewport_ray
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::CreateScreenSpacePickRay()
		{

	// ---- button down ?
		if (!pi_props->sys_input->GetBool(SysInput::LBUTTON_DOWN))
			return VS_ERROR;


	// --- mouse pos
		POINT *p = pi_props->sys_input->GetPosition();
		FLOAT x = (FLOAT)p->x;
		FLOAT y = (FLOAT)p->y;


	// ---- get client adjusted viewrect
		VsFloatRectangle *cav = &pi_props->client_adjusted_viewport;
		x -= cav->left;
			FLOAT w = cav->right - cav->left;
			if ( x < 0 || x > w ) return VS_ERROR;
		y -= cav->top;
			FLOAT h = cav->bottom - cav->top;
			if ( y < 0 || y > h ) return VS_ERROR;


	// ---- device
		GfxDevice *props_gfx_device = pi_props->gfx_device;
		IDirect3DDevice9 *d3d9_device = props_gfx_device->d3d9_device;


	// ---- calc ray
		D3DXMATRIX proj;
		d3d9_device->GetTransform(D3DTS_PROJECTION, &proj);

		D3DXVECTOR3 *vr = &pi_props->viewport_ray;
		vr->x = (((2.0f * x) / w) - 1.0f) / proj._11;
		vr->y = -(((2.0f * y) / h) - 1.0f) / proj._22;
		vr->z = 1.0f;


	// ---- (re-)initialise zed intersection distance
		pi_props->nearest_ray_zed = 999999999.0f;
		pi_props->nearest_ray_handle = 0;

	return VS_OK;
}


// ---------- CreateScreenSpacePickRay ----------
/*!
\brief Transform the GfxInterface::pi_props screen space VsVector3 viewport_ray
into world space VSVector3 directional_ray and origin_ray
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::CreateWorldSpacePickRay()
{

	// ---- button down ?
		if (!pi_props->sys_input->GetBool(SysInput::LBUTTON_DOWN))
			return VS_ERROR;


	// ---- device
		GfxDevice *props_gfx_device = pi_props->gfx_device;
		IDirect3DDevice9 *d3d9_device = props_gfx_device->d3d9_device;


	// ---- get inverse of view matrix
		D3DXMATRIX view;
		d3d9_device->GetTransform(D3DTS_VIEW, &view);
		D3DXMATRIX iv;
		D3DXMatrixInverse(&iv, NULL, &view);


	// ---- transform the screen space viewport pick ray (vr) into 3D space (dr & or)
		D3DXVECTOR3 *vr = &pi_props->viewport_ray;

		D3DXVECTOR3 *dr = &pi_props->direction_ray;
		dr->x = vr->x * iv._11 + vr->y * iv._21 + vr->z * iv._31;
		dr->y = vr->x * iv._12 + vr->y * iv._22 + vr->z * iv._32;
		dr->z = vr->x * iv._13 + vr->y * iv._23 + vr->z * iv._33;

		D3DXVECTOR3 * or = &pi_props->origin_ray;
		or ->x = iv._41;
		or ->y = iv._42;
		or ->z = iv._43;

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////

