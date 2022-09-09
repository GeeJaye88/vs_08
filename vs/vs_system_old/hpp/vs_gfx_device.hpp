////////////////////////////////////////////////////////////////////////////////


// ---------- GfxDevice class ----------

class GfxDevice
{

public:

	// ---- c/dtor
	GfxDevice()
	{
		memset(this, 0, sizeof(GfxDevice)); 
		d3d9_devtype = D3DDEVTYPE_HAL;
	};

	~GfxDevice()
	{
		// Release, etc, done by GfxInterface 
	};


	// ---- viewport
		HRESULT ClearViewportBackground(D3DCOLOR);
		HRESULT DrawViewportBorder(VsFloatRectangle *, D3DCOLOR);
		HRESULT DrawViewportGrid(UINT, UINT, UINT, D3DCOLOR);


	// ---- D3D9 properties
		IDirect3D9       *d3d9_object;    //!< Object 
		IDirect3DDevice9 *d3d9_device;    //!< Device
		D3DDEVTYPE        d3d9_devtype;   //!< Device type, e.g. HAL or REF
		D3DVIEWPORT9      d3d9_viewport;  //!< Viewport


	// ---- D3D properties
		D3DPRESENT_PARAMETERS d3d_presentation_parameters; //!< Required to create device

};


// ---------- ClearViewportBackground ----------
/*!
\brief Clear viewport background.
\author Gareth Edwards
\param D3DCOLOR - clear color
\return HRESULT - ERROR <= 0 < S_OK
*/
HRESULT GfxDevice::ClearViewportBackground(D3DCOLOR color)
{
	DWORD flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;
	HRESULT hr = d3d9_device->Clear(0, NULL, flags, color, 1.0f, 0);
	return S_OK;
}


// ---------- DrawViewportBorder ----------
/*!
\brief Draw viewport border.
\author Gareth Edwards
\param D3DCOLOR - edge color
\return HRESULT - ERROR <= 0 < S_OK
*/
HRESULT GfxDevice::DrawViewportBorder(VsFloatRectangle *margin, D3DCOLOR color)
{
	
	// ---- local
		HRESULT hr = NULL;
		DWORD flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;


	// ---- local....
		DWORD l = (DWORD)(margin->left);
		DWORD r = (DWORD)(margin->right);
		DWORD b = (DWORD)(margin->bottom);
		DWORD t = (DWORD)(margin->top);


	// ---- store viewport ----
		D3DVIEWPORT9 vp;
		hr = d3d9_device->GetViewport(&vp);


	// ---- left margin
		D3DVIEWPORT9 border = vp;
		border.Width = l;
		d3d9_device->SetViewport(&border);
		hr = d3d9_device->Clear(0, NULL, flags, color, 1.0f, 0);


	// ---- right margin
		border = vp;
		border.X += vp.Width - r;
		border.Width = r;
		d3d9_device->SetViewport(&border);
		hr = d3d9_device->Clear(0, NULL, flags, color, 1.0f, 0);


	// ---- top margin
		border = vp;
		border.Height = b;
		d3d9_device->SetViewport(&border);
		hr = d3d9_device->Clear(0, NULL, flags, color, 1.0f, 0);


	// ---- bottom margin
		border = vp;
		border.Y += vp.Height - t;
		border.Height = t; 
		d3d9_device->SetViewport(&border);
		hr = d3d9_device->Clear(0, NULL, flags, color, 1.0f, 0);


	// ---- retore viewport
		d3d9_device->SetViewport(&vp);
   
	return S_OK;
}


// ---------- DrawViewportGrid ----------
/*!
\brief Draw viewport edge.
\author Gareth Edwards
\param D3DCOLOR - edge color
\return HRESULT - ERROR <= 0 < S_OK
*/
HRESULT GfxDevice::DrawViewportGrid(UINT rows, UINT cols, UINT size, D3DCOLOR color)
{

	// ---- ignore & return ?
		if ( size == 0 ) return S_OK;


	// ---- local
		HRESULT hr = NULL;
		DWORD flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;


	// ---- store viewport
		D3DVIEWPORT9 vp;
		hr = d3d9_device->GetViewport(&vp);


	// ---- increment
		FLOAT xinc = (FLOAT)(vp.Width-1)/(FLOAT)rows;
		FLOAT yinc = (FLOAT)(vp.Height-1)/(FLOAT)cols;


	// ---- offset
		INT xoff = vp.X;
		INT yoff = vp.Y;
		INT sby2 = size / 2;


	// ---- each row
		for ( UINT r=0; r<=rows; r++ )
		{
			INT x = (INT)((FLOAT)r*xinc + 0.5f) + xoff;

			INT xs = x - sby2;
			INT xh = size;

			if ( r == 0 )
			{
				xs = x;
				xh = size;
			}
			else if ( r == rows )
			{
				xs = x - size;
				xh = size;
			}

			// ---- each column
			for ( UINT c=0; c<=cols; c++)
			{
				INT y = (INT)((FLOAT)c*yinc + 0.5f) + yoff;

				INT ys = y - sby2;
				INT yh = size;

				if ( c == 0 )
				{
					ys = y;
					yh = size;
				}
				else if ( c == cols )
				{
					ys = y - size;
					yh = size;
				}

				D3DVIEWPORT9 down = { (DWORD)( r==rows ? x-1 : x ), (DWORD)ys, 2, (DWORD)yh };
				d3d9_device->SetViewport(&down);
				hr = d3d9_device->Clear(0, NULL, flags, color, 1.0f, 0);

				// right margin
				D3DVIEWPORT9 across ={ (DWORD)xs, (DWORD)(c==cols ? y-1 : y), (DWORD)xh, 2 };
				d3d9_device->SetViewport(&across);
				hr = d3d9_device->Clear(0, NULL, flags, color, 1.0f, 0);
			}
		}


	// ---- retore viewport
		d3d9_device->SetViewport(&vp);


	return S_OK;
}


////////////////////////////////////////////////////////////////////////////////