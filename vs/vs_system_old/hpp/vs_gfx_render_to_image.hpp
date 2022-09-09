////////////////////////////////////////////////////////////////////////////////



// ---------- GfxRenderToImage class ----------

class GfxRenderToImage
{

public:

	// ---- c/dtor
	GfxRenderToImage()
	{
		Setup();
	}
	~GfxRenderToImage()
	{
		CleanupTargetTextureAndSurfaces();
	}


	// ---- framework ----
	INT  Setup();
	INT  Cleanup();
	INT  CleanupTargetTextureAndSurfaces();
	INT  CleanupTargetSurfaceAndBackBuffer();
	INT  CreateTargetTextureAndSurfaces(IDirect3DDevice9 *, UINT, UINT);


	// note: Transient target surface and backbuffer
	//    created in Begin and released in End
	INT  Begin(IDirect3DDevice9 *, UINT, UINT);
	INT  End(IDirect3DDevice9 *, UINT, UINT, UINT, BYTE *);


	// ---- persistent 
	IDirect3DTexture9 *target_texture;
	IDirect3DSurface9 *intermediate_target;
	IDirect3DSurface9* system_surface;
	

	// ---- transient
	IDirect3DSurface9 *back_buffer;
	IDirect3DSurface9 *target_surface;

};

// ---------- Begin ----------
INT GfxRenderToImage::Begin(
	IDirect3DDevice9 *d3d9_device,
	UINT width,
	UINT height
	)
{

	// ---- local ----
	HRESULT hr = NULL;


	// IF -- target_texture -- THEN
	if ( target_texture != NULL )
	{
		// IF -- dimensions of target_texture changed -- THEN -- release
		D3DSURFACE_DESC surface_desc;
		hr = target_texture->GetLevelDesc(0, &surface_desc);
		if ( FAILED(hr) ) return VS_ERROR;
		if ( surface_desc.Width != width || surface_desc.Height != height )
		{
			INT cleanup = CleanupTargetTextureAndSurfaces();
		}
	}


	// IF -- no target_texture -- THEN
	if ( target_texture == NULL )
	{
		INT create = CreateTargetTextureAndSurfaces(
				d3d9_device,
				width,
				height
			);
	}


	// ---- store pointer to the back buffer ----
	hr = d3d9_device->GetRenderTarget(
			0,
			&back_buffer
		);


	// ---- render target surface is first level of texture ----
	hr = target_texture->GetSurfaceLevel(
			0,
			&target_surface
		);


	// ---- set render surface ----
	hr  =d3d9_device->SetRenderTarget(
			0,
			target_surface
		);

	return VS_OK;
};


// ---------- End ----------
INT GfxRenderToImage::End(
	IDirect3DDevice9 *d3d9_device,
	UINT  dest_width,
	UINT  dest_height,
	UINT  dest_bytes,
	BYTE* dest_buffer
)
{
	// ---- local ----
	HRESULT hr = NULL;

	// IF -- target_texture -- THEN
	if ( target_texture != NULL )
	{

		// IF -- surface description --THEN
		D3DSURFACE_DESC surface_desc;
		hr = target_texture->GetLevelDesc(0, &surface_desc);
		if ( SUCCEEDED(hr) )
		{

			// ---- local target texture ----
			IDirect3DTexture9* d3dtex = (IDirect3DTexture9*)target_texture;

			// note: this intermediate surface is required because GetRenderTargetData(...)
			//    will fail for multi-sampled targets.  One must first down-sample to a
			//    non-multi-sample buffer, and then copy from that intermediate buffer
			//    to a main memory surface.

			IDirect3DSurface9 *temporary_surface;
			hr = d3dtex->GetSurfaceLevel(0, &temporary_surface);

			hr = d3d9_device->StretchRect(
					temporary_surface,
					NULL,
					intermediate_target,
					NULL,
					D3DTEXF_NONE
				);

			hr = d3d9_device->GetRenderTargetData(
					intermediate_target,
					system_surface
				);

			// ---- copy from "source" to "destination" buffers ----

			D3DLOCKED_RECT rect;
			hr = system_surface->LockRect(&rect, 0, 0);
			if ( SUCCEEDED(hr) )
			{
				UINT width = surface_desc.Width;
				UINT height = surface_desc.Height;
				BYTE *src = static_cast<unsigned char*>(rect.pBits);
				BYTE *dest = dest_buffer;
				for ( unsigned int y=0; y<height; y++ )
				{
					for ( unsigned int x=0; x<width; x++ )
					{
						*(dest)   = *(src+2);
						*(dest+1) = *(src+1);
						*(dest+2) = *(src+0);
						dest += dest_bytes;
						src += 4;
					}
				}
				hr = system_surface->UnlockRect();
			}

			//std::cout << "GRAB" << std::endl;

			/*
			hr = D3DXSaveSurfaceToFile(
				"test,png",
				D3DXIFF_PNG,
				system_surface,
				NULL,
				NULL
			);
			*/

			// ---- release temporary surface ----
			temporary_surface->Release();
		}
	}


	// ---- restore pointer to the back buffer ----
	hr = d3d9_device->SetRenderTarget(0, back_buffer);


	// ---- elease transient render to surface ----
	INT result = CleanupTargetSurfaceAndBackBuffer();

	return VS_OK;
};


// ---------- Cleanup ----------
INT GfxRenderToImage::Cleanup()
{
	INT result = 0;
	result = CleanupTargetTextureAndSurfaces();
	result = CleanupTargetSurfaceAndBackBuffer();
	return VS_OK;
};


// ---------- Setup ----------
INT GfxRenderToImage::Setup()
{
	// ---- persistent ----
	target_texture      = NULL;
	intermediate_target = NULL;
	system_surface      = NULL;

	// ---- transient ----
	back_buffer     = NULL;
	target_surface  = NULL;

	return VS_OK;
};


// ---------- CleanupTargetTextureAndSurfaces ----------
INT GfxRenderToImage::CleanupTargetTextureAndSurfaces()
{

	// ---- persistent ----
	if ( target_texture != NULL )
	{
		target_texture->Release();
		target_texture = NULL;
	}

	if ( intermediate_target != NULL )
	{
		intermediate_target->Release();
		intermediate_target = NULL;
	}

	if ( system_surface != NULL )
	{
		system_surface->Release();
		system_surface = NULL;
	}


	// ---- transient ----
	CleanupTargetSurfaceAndBackBuffer();

	return VS_OK;
};


// ---------- CleanupTargetSurfaceAndBackBuffer ----------
INT GfxRenderToImage::CleanupTargetSurfaceAndBackBuffer()
{
	if ( target_surface != NULL )
	{
		target_surface->Release();
		target_surface = NULL;
	}
	if ( back_buffer != NULL )
	{
		back_buffer->Release();
		back_buffer = NULL;
	}
	return VS_OK;
};


// ---------- CreateTargetTextureAndSurfaces ----------
INT GfxRenderToImage::CreateTargetTextureAndSurfaces(
		IDirect3DDevice9 *d3d9_device,
		UINT width,
		UINT height
	)
{

	// ---- local ----
	HRESULT hr = NULL;


	hr = d3d9_device->CreateTexture(
			width,
			height,
			1,	// for non-pyramidal surface
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&target_texture,
			NULL
		);

	D3DSURFACE_DESC surface_desc;
	hr = target_texture->GetLevelDesc(0, &surface_desc);

	hr = d3d9_device->CreateRenderTarget(
			surface_desc.Width,
			surface_desc.Height,
			surface_desc.Format,
			D3DMULTISAMPLE_NONE,
			0,
			FALSE,
			&intermediate_target,
			NULL
		);

	hr =  d3d9_device->CreateOffscreenPlainSurface(
			surface_desc.Width,
			surface_desc.Height,
			surface_desc.Format,
			D3DPOOL_SYSTEMMEM,
			&system_surface,
			NULL
		);

	return VS_OK;
};


////////////////////////////////////////////////////////////////////////////////

