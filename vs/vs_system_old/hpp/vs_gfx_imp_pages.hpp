////////////////////////////////////////////////////////////////////////////////


// ---------- AddPageToAtlasFromJpegFile ----------
/*!
\brief Add texture page to texture atlas from file
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK
*/
INT GfxInterface::AddPageToAtlasFromJpegFile(UINT id, CHAR *filename)
{

	// ---- local
	INT result = VS_ERROR;


	// ---- jpeg file exists ?
		ImgBuffer  img_buffer;
		ImgJpegLib img_jpeg;
		result = img_jpeg.GetHeaderInfo(&img_buffer, filename);
		if ( result > 0 )
		{

			// ---- get dimensions
				UINT width = img_buffer.GetWidth();
				UINT height = img_buffer.GetHeight();
				UINT bytes = img_buffer.GetBytes();
				UCHAR *data = NULL;


			// ---- page with this identity ?
				GfxPage* page = pi_props->gfx_atlas->GetPageWithLogicalId(id);
				if ( page != NULL )
				{

					// ---- update
					IDirect3DDevice9 *d3d9_device = pi_props->gfx_device->d3d9_device;
					result = pi_props->gfx_atlas->UpdatePageFromJpeg(d3d9_device, page, filename);
					if (result == VS_OK)
					{
						if ( pi_props->sys_output )
						{
							char msg[VS_MAXCHARLEN];
							sprintf(msg, "Updated Atlas page id %d (dimension %d x %d)", (INT)id, (INT)width, (INT)height);
							pi_props->sys_output->Message(msg);
						}
						return VS_OK;
					}
				}


				// ---- no page with this identity so add
				else
				{

					// ---- add
					IDirect3DDevice9 *d3d9_device = pi_props->gfx_device->d3d9_device;
					result = pi_props->gfx_atlas->AddPageFromJpeg(d3d9_device, id, filename);
					if ( result == VS_OK )
					{
						if (pi_props->sys_output)
						{
							char msg[VS_MAXCHARLEN];
							sprintf(msg, "Added Atlas page id %d (dimension %d x %d)", (INT)id, (INT)width, (INT)height);
							pi_props->sys_output->Message(msg);
						}
						return VS_OK;
					}
				}

		}


		// ---- fail
			if (pi_props->sys_output)
			{
				char msg[VS_MAXCHARLEN];
				sprintf(msg, "Failed to add OR update Atlas page id %d failed!", (INT)id);
				pi_props->sys_output->Message(msg);
			}

	return result;
}


// ---------- AddPageToAtlasFromTgaFileInMemory ----------
/*!
\brief Add texture page to texture atlas from tga format file in memory
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK
*/
INT GfxInterface::AddPageToAtlasFromTgaFileInMemory(UINT id, BYTE *data)
{

	// ---- TGA header struct
	struct tga_header
	{
		UCHAR   idlength;
		UCHAR 	colourmaptype;
		UCHAR 	datatypecode;
		USHORT 	colourmaporigin;
		UCHAR 	colourmaplength;
		UCHAR 	ColourMapDepth;
		USHORT 	x_origin;
		USHORT 	y_origin;
		USHORT 	width;
		USHORT 	height;
		UCHAR 	bits_per_pixel;
		UCHAR 	imagedescriptor;
	} header;


	// ---- Note : MUST be DATA TYPE 2 (Unmapped RGB)
	char uncompressed_header_content[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	for (UINT i = 0; i<12; i++)
		if (data[i] != uncompressed_header_content[i]) return VS_ERROR;


	// ---- mem copy first 18 bytes into header struct
	memcpy(&header, data, 18);


	// ---- extract dimension and bytes per pixel
	UINT width = header.width;
	UINT height = header.height;
	UINT bytes = header.bits_per_pixel / 8;


	// ---- add page
	INT result = VS_ERROR;
	if (result = pi_props->gfx_atlas->AddRetainedPage(id, width, height, bytes, data + 18, true))
	{
		if (pi_props->sys_output)
		{
			char msg[VS_MAXCHARLEN];
			sprintf(msg, "\tAdded Atlas page id %d (dimension %d x %d)", (INT)id, (INT)width, (INT)height);
			pi_props->sys_output->Message(msg);
		}
	}
	else
	{
		if (pi_props->sys_output)
		{
			char msg[VS_MAXCHARLEN];
			sprintf(msg, "\tFailed to add Atlas page id %d failed!", (INT)id);
			pi_props->sys_output->Message(msg);
		}
	}

	return result;
}


// ---------- AddPageToAtlasFromImgBuffer ----------
/*!
\brief Add texture page to texture atlas from image buffer
\author Gareth Edwards
\return INT - ERROR < VS_ERROR == 0 < VS_OK
*/
INT GfxInterface::AddPageToAtlasFromImgBuffer(
		UINT lid,
		vs_system::ImgBuffer *img,
		BOOL flip
	)
{

	// ---- get dimension and bytes per pixel
	UINT  width = img->GetWidth();
	UINT  height = img->GetHeight();
	UINT  bytes = img->GetBytes();
	BYTE *data = img->GetBuffer();


	// ---- add page
	INT result = VS_ERROR;
	if (result =
		pi_props->gfx_atlas->AddRetainedPage(
			lid,
			width,
			height,
			bytes,
			data,
			flip
		)
		)
	{
		if (pi_props->sys_output)
		{
			char msg[VS_MAXCHARLEN];
			sprintf(
				msg,
				"\tAdded Atlas page id %d (dimension %d x %d)",
				(INT)lid,
				(INT)width,
				(INT)height
			);
			pi_props->sys_output->Message(msg);
		}
	}
	else
	{
		if (pi_props->sys_output)
		{
			char msg[VS_MAXCHARLEN];
			sprintf(
				msg,
				"\tFailed to add Atlas page id %d failed!",
				(INT)lid
			);
			pi_props->sys_output->Message(msg);
		}
	}

	return result;
}


// ---------- AddPageToAtlas ----------
/*!
\brief Add texture page to texture atlas
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= identity
*/
INT GfxInterface::AddPageToAtlas()
{

	INT identity = pi_props->gfx_atlas->AddPage();
	if (identity >= 0)
	{
		if (pi_props->sys_output)
		{
			char msg[VS_MAXCHARLEN];
			sprintf(msg, "\tAdded Atlas page id %d (dynamic)", identity);
			pi_props->sys_output->Message(msg);
		}
	}
	else
	{
		if (pi_props->sys_output)
		{
			char msg[VS_MAXCHARLEN];
			sprintf(msg, "\tFailed to add Atlas page id %d (dynamic)!", identity);
			pi_props->sys_output->Message(msg);
		}
	}

	return identity;
}


// ---------- GetAtlasPageDimensions ----------
/*!
\brief Get handle of an atlas page
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= handle
*/
INT GfxInterface::GetAtlasPageTextureDimensions(UINT logical_id, UINT *width, UINT *height)
{

	GfxPage *page = pi_props->gfx_atlas->GetPageWithLogicalId(logical_id);
	if (page != NULL)
	{
		GfxPageDimension dimension;
		page->GetTextureDimension(&dimension);
		*width = dimension.width;
		*height = dimension.height;
	}

	return VS_OK;
}


// ---------- GetAtlasPageHandle ----------
/*!
\brief Get handle of an atlas page
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= handle
*/
INT GfxInterface::GetAtlasPageHandle(UINT logical_id)
{
	GfxPage *page = pi_props->gfx_atlas->GetPageWithLogicalId(logical_id);
	return (INT)page;
}


// ---------- GetAtlasPageLogicalId ----------
/*!
\brief Get handle of an atlas page
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= handle
*/
UINT GfxInterface::GetAtlasPageLogicalId()
{
	UINT unused_lid = pi_props->gfx_atlas->GetPageLogicalId();
	return unused_lid;
}


// ---------- PageSetup ----------
/*!
\brief (Re)Setup (re-intitialise) a page in Atlas
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= identity
*/
INT GfxInterface::PageSetup(
	UINT identity,
	std::string &filename,
	vs_system::ImgBuffer *img_buffer
)
{

	// ---- page added -- ? report success -- ---- report fail
	INT page_handle = pi_props->gfx_atlas->SetupPage(
		pi_props->gfx_device->d3d9_device,
		identity,
		filename,
		img_buffer
	);


	// ---- page added -- THEN
	if (page_handle >= 0)
	{
		if (pi_props->sys_output)
		{
			//char msg[VS_MAXCHARLEN];
			//sprintf(msg, "Setup Atlas page id: %d", identity);
			//pi_props->sys_output->Message(msg);
			;
		}
	}

	// ELSE - - report
	else
	{
		if (pi_props->sys_output)
		{
			char msg[VS_MAXCHARLEN];
			sprintf(msg, "Failed to setup Atlas page id: %d", identity);
			pi_props->sys_output->Message(msg);
		}
	}

	return page_handle;
}


// ---------- PageLock ----------
/*!
\brief Page lock
\author Gareth Edwards
\return UCHAR *
*/
UCHAR * GfxInterface::PageLock(INT page_handle)
{

	// ---- local
	GfxPage *page = (GfxPage *)page_handle;


	// ---- page -- THEN
	if (page != NULL && page->d3d9_texture != NULL)
	{

		// ---- lock and load
		D3DLOCKED_RECT locked;
		HRESULT hr = page->d3d9_texture->LockRect(0, &locked, NULL, 0);
		if (SUCCEEDED(hr))
		{
			// ---- good to go....!!!
			UCHAR * bytePointer = (UCHAR *)locked.pBits;
			//std::cout << "PageLock: " << (UCHAR *)locked.pBits << std::endl;
			return bytePointer;
		}
		else
		{
			char msg[VS_MAXCHARLEN];
			sprintf(msg, "PageLock: page->d3d9_texture->LockRect failed!");
			pi_props->sys_output->Message(msg);
		}
	}
	else
	{
		char msg[VS_MAXCHARLEN];
		sprintf(msg, "PageLock: page->d3d9_texture %d", (INT)page->d3d9_texture);
		pi_props->sys_output->Message(msg);
	}

	return NULL;
}


// ---------- PageUnlock ----------
/*!
\brief Page unlock
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT  GfxInterface::PageUnlock(INT page_handle)
{

	// ---- cast from INT to GfxPage
	GfxPage *page = (GfxPage *)page_handle;


	// ---- page ?
	if (page != NULL && page->d3d9_texture != NULL)
	{
		HRESULT hr = page->d3d9_texture->UnlockRect(0);
	}

	//D3DXSaveTextureToFile("test.jpg", D3DXIFF_JPG, page->d3d9_texture, NULL);

	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////

