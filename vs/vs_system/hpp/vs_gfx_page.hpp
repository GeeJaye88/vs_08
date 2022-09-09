////////////////////////////////////////////////////////////////////////////////


// ---------- GfxPage class ----------

class GfxPage
{

public:

	// ---- page type ----
	enum page_type
	{
		UNKNOWN=0,
		DYNAMIC=1,
		RETAINED=2,
		FROMJPEG=3
	};


	GfxPage()
	{

		// ---- misc
			type         = UNKNOWN;
			logical_id   = 0;
			filename[0]  = 0;
			next         = NULL;
			is_texture_created = FALSE;


		// ---- source
			source_rgba_buffer = NULL;
			

		// ---- texture
			d3d9_texture = NULL;
			d3d9_buffer  = NULL;


		// ---- dimension
			dimension_source_image.Set(128, 128);
			dimension_texture.Set(128, 128);

	};

	~GfxPage()
	{ 

		if ( d3d9_texture )
		{
			d3d9_texture->Release();
			d3d9_texture = NULL;
			is_texture_created = FALSE;
		}


		if ( d3d9_buffer )
		{
			d3d9_buffer->Release();
			d3d9_buffer = NULL;
		}


		if ( source_rgba_buffer )
		{
			delete source_rgba_buffer;
			source_rgba_buffer = NULL;
		}

	};


	// ---- old code!!! ----
	INT Create(IDirect3DDevice9 *, CHAR *);
	

	// ---- add ----
	INT Add(UINT);
	INT AddFromJpeg(IDirect3DDevice9 *, UINT, CHAR*);    // invoked SetupFromJpegTexture
	INT UpdateFromJpeg(IDirect3DDevice9 *, CHAR *);      // ditto
	INT AddRetained(UINT, UINT, UINT, UINT, UCHAR *);    //


	// ---- buffer ----
	INT BufferFlipVertical();


	// ---- framework ----
	INT Setup(IDirect3DDevice9 *, std::string &, vs_system::ImgBuffer *);
	INT Setup(IDirect3DDevice9 *, vs_system::ImgBuffer *);
	INT SetupFromJpegTexture(IDirect3DDevice9 *);
	INT SetupRetainedTexture(IDirect3DDevice9 *);
	INT CleanupTexture();


	// ---- get ----
	INT GetTextureDimension(GfxPageDimension *);
	IDirect3DTexture9 *GetD3DTexture9();


	// ---- is ----
	BOOL IsD3DTexture9Created();


	// ---- properties -----
	UINT               type;
	UINT               logical_id;
	CHAR               filename[VS_MAXCHARPATHLEN];
	GfxPage           *next;
	BOOL               is_texture_created;


	// ---- texture ----
	IDirect3DTexture9 *d3d9_texture;
	IDirect3DTexture9 *d3d9_buffer;
	GfxPageDimension   dimension_source_image;
	GfxPageDimension   dimension_texture;   


	// ---- source ----
	UCHAR *source_rgba_buffer;

};


// ---------- Create ----------
/*!
\brief Create texture
\author Gareth Edwards
\param IDirect3DDevice9 * - D3D9 device
\param CHAR * - path and filename
\return HRESULT - ERROR <= 0 < S_OK
*/
INT GfxPage::Create(
		IDirect3DDevice9 *d3d9_device,
		CHAR *paf
	)
{

	#ifdef VS_TEMP_DEBUG
	std::cout << "GfxPage::Create" << std::endl;
	#endif


	// ---- release
		if ( d3d9_texture != NULL )
		{
			d3d9_texture->Release();
			d3d9_texture = NULL;
			is_texture_created = FALSE;
		}

		if ( d3d9_buffer != NULL )
		{
			d3d9_buffer->Release();
			d3d9_buffer = NULL;
			is_texture_created = FALSE;
		}


	// ---- local
		HRESULT hr;


	// ---- get image info
		D3DXIMAGE_INFO image_info;
			hr = D3DXGetImageInfoFromFile(paf, &image_info);
			if (FAILED(hr)) return VS_ERROR;


	// ---- create texture
		hr = D3DXCreateTextureFromFile( d3d9_device, paf, &d3d9_texture);
			if (FAILED(hr)) return VS_ERROR;
			is_texture_created = TRUE;


	// ----- note: create buffer later


	// ---- get surface description
		D3DSURFACE_DESC surface_desc;
			hr = d3d9_texture->GetLevelDesc(0, &surface_desc);
			if (FAILED(hr)) return VS_ERROR;


	// ---- store dimensions
		dimension_source_image.Set(image_info.Width, image_info.Height);
		dimension_texture.Set(surface_desc.Width,surface_desc.Height);

	return VS_OK;
}


// ---------- Add ----------
/*!
\brief Add page
\author Gareth Edwards
\return HRESULT - ERROR <= 0 < S_OK
*/
INT GfxPage::Add(UINT id)
{

	#ifdef VS_TEMP_DEBUG
	std::cout << "GfxPage::Add" << std::endl;
	#endif

	type = UNKNOWN;
	logical_id = id;

	return id;
}


// ---------- AddFromJpeg ----------
INT GfxPage::AddFromJpeg(
		IDirect3DDevice9 *d3d9_device,
		UINT id,
		CHAR *paf
	)
{

	#ifdef VS_TEMP_DEBUG
	std::cout << "GfxPage::AddFromJpeg" << std::endl;
	#endif


	// ---- initialise
		type = FROMJPEG;
		logical_id = id;
		strcpy (filename, paf);


	// ---- texture ----
		#ifdef VS_DEBUG
		std::cout << "AddFromJpeg texture logical_id " << logical_id << " ok " << std::endl;
		#endif
		SetupFromJpegTexture(d3d9_device);

	return VS_OK;
}


// ---------- UpdateFromJpeg ----------
INT GfxPage::UpdateFromJpeg(IDirect3DDevice9 *d3d9_device, CHAR *paf)
{

	#ifdef VS_TEMP_DEBUG
	std::cout << "GfxPage::UpdateFromJpeg" << std::endl;
	#endif


	// ---- initialise
		strcpy (filename, paf);


	// ---- texture
		#ifdef VS_DEBUG
		std::cout << "UpdateFromJpeg texture logical_id " << logical_id << " ok " << std::endl;
		#endif
		SetupFromJpegTexture(d3d9_device);

	return VS_OK;
}


// ---------- AddRetained ----------
/*!
\brief Add retained page
\author Gareth Edwards
\param UINT - width
\param UINT - height
\param UINT - number of bytes per pixel (3 or 4 - RGB or RGBA)
\param UCHAR * - pixel data
\return HRESULT - ERROR <= 0 < S_OK
*/
INT GfxPage::AddRetained(
		UINT   id,
		UINT   width,
		UINT   height,
		UINT   bytes,
		UCHAR *data
	)
{

	#ifdef VS_TEMP_DEBUG
	std::cout << "GfxPage::AddRetained" << std::endl;
	#endif


	// ---- initialise
		type = RETAINED;
		logical_id = id;


	// ---- create source buffer
		LONG size = width * height * 4;
		source_rgba_buffer = new UCHAR[size];


	// ---- 4 bytes per pixel ?
		if ( bytes == 4 )
		{
			memcpy(
				source_rgba_buffer,
				&data[0],
				sizeof(UCHAR) * width * height * 4
				);
		}

	// ---- copy ?
		else
		{
			UCHAR *p_rgb_data = data;
			UCHAR *p_source_rgba_buffer = source_rgba_buffer;
			for ( UINT y=0; y<height; y++ )
			{
				for ( UINT x=0; x<width; x++ )
				{
					p_source_rgba_buffer[0] = p_rgb_data[0];
					p_source_rgba_buffer[1] = p_rgb_data[1];
					p_source_rgba_buffer[2] = p_rgb_data[2];
					p_source_rgba_buffer[3] = 255;
					p_source_rgba_buffer += 4;
					p_rgb_data += 3; // 190914 was += 4 !
				}
			}
		}


	// ---- set dimensions
		dimension_source_image.Set(width, height);

	return VS_OK;
}


// ---------- BufferFlipVertical ----------
/*!
\brief Flip buffer verticaly
\author Gareth Edwards
\param UINT - width
\param UINT - height
\param UINT - number of bytes per pixel (3 or 4 - RGB or RGBA)
\param UCHAR * - pixel data
\return HRESULT - ERROR <= 0 < S_OK
*/
INT GfxPage::BufferFlipVertical()
{

	#ifdef VS_TEMP_DEBUG
	std::cout << "GfxPage::BufferFlipVertical" << std::endl;
	#endif


	// ---- test
		if ( source_rgba_buffer == NULL ) return VS_ERROR;


	// ---- flip
		UINT width  = dimension_source_image.width;
		UINT height = dimension_source_image.height;
		UINT hby2   = height / 2;
		UCHAR *p_rgba_bottom = source_rgba_buffer;
		UCHAR *p_rgba_top = source_rgba_buffer + (width * (height - 1) * 4);
		for ( UINT y=0; y<hby2; y++ )
		{
			for ( UINT x=0; x<width; x++ )
			{
				for ( UINT s=0; s<4; s++ )
				{
					UCHAR temp = p_rgba_bottom[s];
					p_rgba_bottom[s] = p_rgba_top[s];
					p_rgba_top[s] = temp;
				}
				p_rgba_bottom += 4;
				p_rgba_top += 4;
			}
			p_rgba_top -= width * 2 * 4;
		}

	return VS_OK;
}


// ---------- CleanupTexture ----------
/*!
\brief Cleanup texture
\author Gareth Edwards
\return HRESULT - ERROR <= 0 < S_OK
*/
INT GfxPage::CleanupTexture()
{

	#ifdef VS_TEMP_DEBUG
	std::cout << "GfxPage::CleanupTexture" << std::endl;
	#endif

	if ( d3d9_texture != NULL ) d3d9_texture->Release();
	d3d9_texture = NULL;

	if ( d3d9_buffer != NULL ) d3d9_buffer->Release();
	d3d9_buffer = NULL;


	is_texture_created = FALSE;



	return VS_OK;
}


// ---------- Setup ----------
/*!
\brief Setup page
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_ERROR
*/
INT GfxPage::Setup(
	IDirect3DDevice9 *d3d9_device,
	vs_system::ImgBuffer *img_buffer
	)

{

	std::string name = "";
	INT result = Setup(d3d9_device, name, img_buffer);

	return VS_OK;
}


// ---------- Setup ----------
/*!
\brief Setup page with filename
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_ERROR
\note D3DFMT_A8B8G8R8 available but not supported
*/
INT GfxPage::Setup(
		IDirect3DDevice9 *d3d9_device,
		std::string &name,
		vs_system::ImgBuffer *img_buffer
	)
{

	#ifdef VS_TEMP_DEBUG
	std::cout << "GfxPage::Setup" << std::endl;
	#endif


	// ---- local
		UINT width = img_buffer->GetWidth();
		UINT height = img_buffer->GetHeight();


	// ---- texture OK ?
		if ( d3d9_texture != NULL )
		{

			// ---- same dimension ?
				if (
						dimension_source_image.width == width &&
						dimension_source_image.height == height
					)
				{
					strcpy(filename, name.c_str());
					return VS_OK;
				}

			// ---- release texture
				d3d9_texture->Release();
				d3d9_texture = NULL;
				is_texture_created = FALSE;
		}

		OutputDebugString("########## CreateTexture D3DFMT_A8R8G8B8 ##########\n");

	// ---- create empty IDirect3DTexture9 texture
		HRESULT hr = d3d9_device->CreateTexture(
				width,
				height,
				1,
				0,
				D3DFMT_A8R8G8B8,
				D3DPOOL_MANAGED,
				&d3d9_texture,
				0
			);

		if (FAILED(hr))
		{
			#ifdef VS_TEMP_DEBUG
			std::cout << "########## CreateTexture FAILED ##########" << std::endl;
			#endif
			return VS_ERROR;
		}
		is_texture_created = TRUE;


	// ---- get surface description
		D3DSURFACE_DESC surface_desc;
		HRESULT ihr = d3d9_texture->GetLevelDesc(0, &surface_desc);
		if (FAILED(ihr)) return VS_ERROR;


	// ---- store dimensions
		//  note: do not assume that these are the same...
		dimension_source_image.Set(width, height);
		dimension_texture.Set(surface_desc.Width, surface_desc.Height);


	// ---- initialise
		strcpy(filename, name.c_str());
		type = name == "" ? DYNAMIC : FROMJPEG;
		source_rgba_buffer = NULL;

	return VS_OK;
}


// ---------- SetupFromJpegTexture ----------
/*!
\brief Setup from jpeg texture
\author Gareth Edwards
\param IDirect3DDevice9 * - D3D9 device
\param UINT - width
\param UINT - height
\param UINT - number of bytes per pixel (must be 4 for A8R8G8B8)
\param UCHAR * - pixel data
\return HRESULT - ERROR <= 0 < S_OK
*/
INT GfxPage::SetupFromJpegTexture(IDirect3DDevice9 *d3d9_device)
{

	#ifdef VS_TEMP_DEBUG
	std::cout << "GfxPage::SetupFromJpegTexture" << std::endl;
	#endif


	// ---- local
		INT result = VS_ERROR;


	// ---- jpeg file exists
		ImgBuffer  img_buffer;
		ImgJpegLib img_jpeg;
		result = img_jpeg.GetHeaderInfo(&img_buffer, filename);
		if ( result > 0 )
		{

			// ---- get dimensions
				UINT width  = img_buffer.GetWidth();
				UINT height = img_buffer.GetHeight();
				UINT bytes  = img_buffer.GetBytes();


			// ---- same dimension
				BOOL dimension_same = false;
				if (
						width == dimension_source_image.width &&
						height == dimension_source_image.height
					)
				{
					dimension_same = true;
				}
				else
				{
					if ( d3d9_texture != NULL ) d3d9_texture->Release();
					d3d9_texture = NULL;
					is_texture_created = NULL;
				}


			// ---- buffer
				#if defined(VS_USE_BIG_BUFFER)
					;
				#else
				if ( d3d9_buffer == NULL )
				{
					HRESULT hr = d3d9_device->CreateTexture(
							width,
							height,
							1,
							0,
							D3DFMT_A8R8G8B8,
							D3DPOOL_MANAGED,
							&d3d9_buffer,
							0
						);
					if (FAILED(hr))
					{
						#ifdef VS_DEBUG
						std::cout << "########## CreateTexture for BUFFER FAILED ##########" << std::endl;
						#endif
						return VS_ERROR;
					}
				}
				#endif


			// ---- texture
				if ( d3d9_texture == NULL )
				{
					HRESULT hr = d3d9_device->CreateTexture(
							width,
							height,
							1,
							0,
							D3DFMT_A8R8G8B8,
							D3DPOOL_MANAGED,
							&d3d9_texture,
							0
						);
					if (FAILED(hr))
					{
						#ifdef VS_DEBUG
						std::cout << "########## CreateTexture for IMAGE FAILED ##########" << std::endl;
						#endif
						return VS_ERROR;
					}
				}


			// ---- lock buffer
				#if defined(VS_USE_BIG_BUFFER)
				;
				#else
				D3DLOCKED_RECT rect_buffer;
				HRESULT bhr = d3d9_buffer->LockRect(0, &rect_buffer, 0, D3DLOCK_DISCARD);
				#endif


			// ---- read jpeg
				img_buffer.Initialise(width, height, 3);
				img_buffer.SetQuality(85);

				#if defined(VS_USE_BIG_BUFFER)
				img_buffer.SetBuffer(global_very_big_image_buffer);
				#else
				{
					UCHAR *src = static_cast<UCHAR*>(rect_buffer.pBits);
					img_buffer.SetBuffer(src);
				}
				#endif

				int jpeg_read_status = img_jpeg.Read(&img_buffer, filename);

				#ifdef VS_DEBUG
				if ( jpeg_read_status <= 0 )
				{
					std::cout << "########## JPEG READ FAILED ##########" << std::endl;
				}
				#endif


			// ---- get surface description
				D3DSURFACE_DESC isurface_desc;
				HRESULT ihr = d3d9_texture->GetLevelDesc(0, &isurface_desc);
				if (FAILED(ihr)) return VS_ERROR;


			// ---- lock and load
				D3DLOCKED_RECT rect;
				HRESULT hr = d3d9_texture->LockRect( 0, &rect, 0, D3DLOCK_DISCARD );
				if ( SUCCEEDED(hr) ) 
				{
					UCHAR *dest = static_cast<UCHAR*>(rect.pBits);

					#if defined(VS_USE_BIG_BUFFER)
					UCHAR *src = static_cast<UCHAR*>( global_very_big_image_buffer);
					#else
					UCHAR *src = static_cast<UCHAR*>(rect_buffer.pBits);
					#endif

					for ( unsigned int y=0; y<height; y++ )
					{
						for ( unsigned int x=0; x<width; x++ )
						{
							*(dest)   = *(src+2);
							*(dest+1) = *(src+1);
							*(dest+2) = *(src+0);
							*(dest+3) = bytes == 4 ? *(src+3) : 255;                     // TBD - BUG BUG - WAS 0
							dest += 4;
							src += bytes;
						}
					}
					hr = d3d9_texture->UnlockRect(0);
				}


			// ---- store dimension of source image
				dimension_source_image.Set(width, height);


			// ---- get surface description
				D3DSURFACE_DESC surface_desc;
				hr = d3d9_texture->GetLevelDesc(0, &surface_desc);
				if (FAILED(hr)) return VS_ERROR;


			// ---- store texture dimension
				dimension_texture.Set(surface_desc.Width, surface_desc.Height);

			return VS_OK;
		}

	return result;
}


// ---------- SetupRetainedTexture ----------
/*!
\brief Setup retained texture
\author Gareth Edwards
\param IDirect3DDevice9 * - D3D9 device
\return HRESULT - ERROR <= 0 < S_OK
*/
INT GfxPage::SetupRetainedTexture(IDirect3DDevice9 *d3d9_device)
{

	#ifdef VS_TEMP_DEBUG
	std::cout << "GfxPage::SetupRetainedTexture" << std::endl;
	#endif

	// ---- local
		HRESULT hr;


	// ---- create empty IDirect3DTexture9*
		hr = d3d9_device->CreateTexture(
				dimension_source_image.width,
				dimension_source_image.height,
				1,
				0, 
				D3DFMT_A8R8G8B8,
				D3DPOOL_MANAGED,
				&d3d9_texture,
				0
			);
		if (FAILED(hr)) return VS_ERROR;
		is_texture_created = TRUE;


	// ---- lock and load
		D3DLOCKED_RECT rect;
		hr = d3d9_texture->LockRect( 0, &rect, 0, D3DLOCK_DISCARD );
			UCHAR* dest = static_cast<UCHAR*>(rect.pBits);
			memcpy(
					dest,
					&source_rgba_buffer[0],
					sizeof(UCHAR) *
						dimension_source_image.width *
						dimension_source_image.height * 4
				);
		hr = d3d9_texture->UnlockRect(0);


	// ---- confirm
		// D3DXSaveTextureToFile("test.jpg", D3DXIFF_JPG, d3d9_texture, NULL);


	// ---- get surface description
		D3DSURFACE_DESC surface_desc;
		hr = d3d9_texture->GetLevelDesc(0, &surface_desc);
		if (FAILED(hr)) return VS_ERROR;


	// ---- store texture dimension
		dimension_texture.Set(surface_desc.Width,surface_desc.Height);

	return VS_OK;
}


// ---------- GetD3DTexture9 ----------
IDirect3DTexture9 * GfxPage::GetD3DTexture9()
{

	#ifdef VS_TEMP_DEBUG
	std::cout << "GfxPage::GetD3DTexture9" << std::endl;
	#endif

	return d3d9_texture;
};


// ---------- GetTextureDimension ----------
INT GfxPage::GetTextureDimension(GfxPageDimension *dimension)
{

	#ifdef VS_TEMP_DEBUG
	std::cout << "GfxPage::GetD3DTexture9" << std::endl;
	#endif

	*dimension = dimension_texture;

	return VS_OK;
};


// ---------- IsD3DTexture9Created ----------
BOOL GfxPage::IsD3DTexture9Created()
{
	#ifdef VS_TEMP_DEBUG
	std::cout << "GfxPage::GetTextureCreated" << std::endl;
	#endif

	return is_texture_created;
};


////////////////////////////////////////////////////////////////////////////////
