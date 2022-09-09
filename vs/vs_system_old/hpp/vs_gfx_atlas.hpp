////////////////////////////////////////////////////////////////////////////////


// ----------GfxAtlas class ----------

class GfxAtlas
{

public:

	//  ---- cdtor ----
	GfxAtlas()
	{
		memset(this, 0, sizeof(GfxAtlas)); 
	};
	~GfxAtlas()
	{
		CleanupPageTextures();
	};


	// ---- jpeg ----
	INT  AddPageFromJpeg(IDirect3DDevice9 *, UINT, CHAR *);
	INT  UpdatePageFromJpeg(IDirect3DDevice9 *, GfxPage *, CHAR *);


	// ---- ImgBuffer ----
	INT  AddPage();
	INT  SetupPage(IDirect3DDevice9 *, UINT, std::string &, ImgBuffer *);


	// ---- retained ----
	INT  AddRetainedPage(UINT, UINT, UINT, UINT, unsigned char *, BOOL);


	// ---- get ----
	GfxPage * GetPageWithLogicalId(UINT);
	GfxPage * GetPageWithIndex(UINT);   // NOT USED
	UINT      GetPageLogicalId();
	IDirect3DTexture9 * GetTexture(UINT);
	INT       CleanupPageTexture(UINT);


	// ---- framework ----
	INT  SetupPageTextures(IDirect3DDevice9 *);
	INT  CleanupPageTextures();


	// ---- ring link ----
	GfxPage *first_page;
	GfxPage *last_page;
};


// ---------- AddPageFromJpeg ----------
/*!
\brief Add page to atlas from jpeg
\author Gareth Edwards
\return HRESULT - ERROR <= 0 < S_OK
*/
INT GfxAtlas::AddPageFromJpeg(IDirect3DDevice9 *d3d9_device, UINT id, CHAR * filename)
{

	// ---- create ----
	GfxPage *page = new GfxPage();


	// ---- link ----
	if ( first_page==NULL )
	{
		first_page = last_page = page;
	}
	else
	{
		last_page->next = page;
		last_page = page;
	}


	// ---- add ----
	INT result = VS_OK;
	if ( result = page->AddFromJpeg(d3d9_device, id, filename) == VS_OK )
	{
		;
	}

	return result;
}


// ---------- UpdatePageFromJpeg ----------
/*!
\brief Update page in atlas from jpeg
\author Gareth Edwards
\return HRESULT - ERROR <= 0 < S_OK
*/
INT GfxAtlas::UpdatePageFromJpeg(IDirect3DDevice9 *d3d9_device, GfxPage *page, CHAR *filename)
{
	INT result = VS_OK;
	if ( result = page->UpdateFromJpeg(d3d9_device, filename) == VS_OK )
	{
		;
	}

	return result;
}


// ---------- AddPage ----------
/*!
\brief Add page to atlas
\author Gareth Edwards
\return INT - ERROR < 0 <= logical id
*/
INT GfxAtlas::AddPage()
{

	// ---- create ----
	GfxPage *page = new GfxPage();


	// ---- link ----
	INT id = 0;
	if ( first_page==NULL )
	{
		first_page = last_page = page;
	}
	else
	{
		id = last_page->logical_id + 1;
		last_page->next = page;
		last_page = page;
	}


	// ---- add ----
	INT result = page->Add(id);

	return result;
}


// ---------- SetupPage ----------
/*!
\brief Setup page in atlas
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_ERROR
*/
INT GfxAtlas::SetupPage(
	IDirect3DDevice9 *d3d9_device,
	UINT id,
	std::string &filename,
	vs_system::ImgBuffer *img_buffer
	)
{

	GfxPage *page = GetPageWithLogicalId(id);
	if ( page != NULL)
	{
		INT result = page->Setup(d3d9_device, filename, img_buffer);
		if ( result == VS_OK)
			return (INT)page;
	}

	return VS_ERROR;
}


// ---------- AddRetainedPage ----------
/*!
\brief Add retained page to atlas
\author Gareth Edwards
\return HRESULT - ERROR <= 0 < S_OK
*/
INT GfxAtlas::AddRetainedPage(
		UINT id,
		UINT width,
		UINT height,
		UINT bytes,
		unsigned char *data,
		BOOL flip
	)
{

	// ---- create ----
	GfxPage *page = new GfxPage();


	// ---- link new texture to atlas ----
	if ( first_page==NULL )
	{
		first_page = last_page = page;
	}
	else
	{
		last_page->next = page;
		last_page = page;
	}


	// ---- setup and then flip page ----
	INT result = VS_OK;
	if ( result = page->AddRetained(id, width, height, bytes, data) == VS_OK )
	{
		if ( flip ) result = page->BufferFlipVertical();
	}

	return result;
}


// ---------- GetPageLogicalId ----------
/*!
\brief Get unused page logical id
\author Gareth Edwards
\return UINT - unused logical id
*/
UINT GfxAtlas::GetPageLogicalId()
{
	UINT unused_lid = 100;
	GfxPage *page = first_page;
	while ( page )
	{
		unused_lid = unused_lid <= page->logical_id ? page->logical_id + 1 : unused_lid;
		page = page->next;
	}

	return unused_lid;
}


// ---------- GetPageWithLogicalId ----------
/*!
\brief Get page with logical id
\author Gareth Edwards
\return HRESULT - ERROR <= 0 < S_OK
*/
GfxPage * GfxAtlas::GetPageWithLogicalId(UINT id)
{

	GfxPage *page = first_page;
	while ( page )
	{
		if ( page->logical_id == id )
		{
			return page;
		}
		page = page->next;
	}

	return NULL;
}


// ---------- GetPageWithIndex ----------
/*!
\brief Get page pointer
\author Gareth Edwards
\return HRESULT - ERROR <= 0 < S_OK
*/
GfxPage * GfxAtlas::GetPageWithIndex(UINT index)
{

	UINT page_index = 0;
	GfxPage *page = first_page;
	while ( page )
	{
		if ( page_index == index )
		{
			return page;
		}
		page_index++;
		page = page->next;
	}

	return NULL;
}


// ---------- GetTexture ----------
/*!
\brief Get page texture
\author Gareth Edwards
\return HRESULT - ERROR <= 0 < S_OK
*/
IDirect3DTexture9 * GfxAtlas::GetTexture(UINT id)
{

	GfxPage *page = first_page;
	while ( page )
	{
		if ( page->logical_id == id )
		{
			return page->GetD3DTexture9();
		}
		page = page->next;
	}

	return NULL;
}


// ---------- CleanupPageTextures ----------
/*!
\brief Cleanup page textures
\author Gareth Edwards
\return HRESULT - ERROR <= 0 < S_OK
*/
INT GfxAtlas::CleanupPageTextures()
{

	GfxPage *page = first_page;
	while ( page )
	{
		INT result = page->CleanupTexture();

#		ifdef VS_DEBUG
		std::cout << "Cleanup texture identity " << page->identity << " ok " << std::endl;
#		endif

		page = page->next;
	}

	return VS_OK;
}


// ---------- CleanupPageTexture ----------
/*!
\brief Cleanup a page texture
\author Gareth Edwards
\return HRESULT - ERROR <= 0 < S_OK
*/
INT GfxAtlas::CleanupPageTexture(UINT id)
{

	GfxPage *page = first_page;
	while (page)
	{
		if (page->logical_id == id)
		{
			INT result = page->CleanupTexture();
			#ifdef VS_DEBUG
			std::cout << "Cleanup texture identity " << page->identity << " [ logical id" << page->logical_id << "] ok " << std::endl;
			#endif
			return result;
		}

		page = page->next;
	}

	return VS_OK;
}


// ---------- SetupPageTextures ----------
/*!
\brief Setup page textures
\author Gareth Edwards
\return HRESULT - ERROR <= 0 < S_OK
*/
INT GfxAtlas::SetupPageTextures(IDirect3DDevice9 *d3d9_device)
{

	GfxPage *page = first_page;
	while ( page )
	{
		if ( page->type == page->FROMJPEG )
		{
			INT result = page->SetupFromJpegTexture(d3d9_device);

#			ifdef VS_DEBUG
			std::cout << "Setup from jpeg texture identity " << page->identity << " ok " << std::endl;
#			endif

			if ( page->source_rgba_buffer == NULL )
			{
#				ifdef VS_DEBUG
				std::cout << "RGBA data is NULL" << std::endl;
#				endif
			} 
		}
		else if ( page->type == page->RETAINED )
		{
			INT result = page->SetupRetainedTexture(d3d9_device);

#			ifdef VS_DEBUG
			std::cout << "Setup retained texture identity " << page->identity << " ok " << std::endl;
#			endif
		}
		else
		{
#			ifdef VS_DEBUG
			std::cout << "Setup texture identity " << page->identity << " failed " << std::endl;
#			endif

			return VS_ERROR;
		}

		page = page->next;
	}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////