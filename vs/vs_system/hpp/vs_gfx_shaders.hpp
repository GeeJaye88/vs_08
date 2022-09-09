////////////////////////////////////////////////////////////////////////////////


// ---------- GfxShaders class ----------

class GfxShaders
{

public:

	enum {
		MAX_PARAM = 8
	};

	// ---- c/dtor ----
	GfxShaders()
	{
		for (UINT i = 0; i < MAX_PARAM; i++)
		{
			shader_param[i] = 0;
		}
		shader_param[0] = 1;
	}


	~GfxShaders() { Cleanup(); }


	// ---- framework methods
		INT  Setup(CHAR *, INT, IDirect3DDevice9 *);
		INT  Cleanup();


	// ---- temp process methods
		FLOAT shader_param[MAX_PARAM];
		INT GetParam(FLOAT *);
		INT SetParam(FLOAT *);

		INT SetBCS(FLOAT, FLOAT, FLOAT);
		INT SetBCSEnabled(D3DXMATRIX *mv);
		INT SetBCSDisabled();


	// ---- process methods
		INT SetMatrices();


	// ---- properties
		IDirect3DDevice9*               d3d9_device        = NULL;

		#if VS_GFX_SHADERS
		LPDIRECT3DVERTEXSHADER9         vertex_shader      = NULL;
		LPDIRECT3DPIXELSHADER9          pixel_shader       = NULL;
		LPDIRECT3DVERTEXDECLARATION9    vertex_declaration = NULL;
		LPD3DXCONSTANTTABLE             constant_tableVS   = NULL;
		LPD3DXCONSTANTTABLE             constant_tablePS   = NULL;
		#endif

		D3DXMATRIX  matrix_world;
		D3DXMATRIX  matrix_view;
		D3DXMATRIX  matrix_proj;

};


// ---------- Setup ----------
INT GfxShaders::Setup(CHAR *app_name, INT index, IDirect3DDevice9* device)
{

	// ---- store
	d3d9_device = device;


	#if VS_GFX_SHADERS == 1


	// ---- local
		INT hr = 0;
		LPD3DXBUFFER pCode;
		DWORD dwShaderFlags = 0;
		LPD3DXBUFFER pBufferErrors = NULL;


	// ---- vertex declaration
		D3DVERTEXELEMENT9 declaration[] = 
		{
			{ 0,  0, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0 },
			{ 0, 16, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};

		d3d9_device->CreateVertexDeclaration(declaration, &vertex_declaration);


	// ---------- assemble the vertex shader ----------
		CHAR vs_format[256];
		strcpy(vs_format, "data/%s/assets/vertex_shader_%1d.vsh");
		if ( IsDebuggerPresent() )
			{
				strcpy(vs_format, "../data/%s/assets/vertex_shader_%1d.vsh");
			}
		CHAR vs_filename[VS_MAXCHARLEN];
		sprintf(vs_filename, vs_format, app_name, index);

		hr = D3DXCompileShaderFromFile
			(
				vs_filename,
				NULL, 
				NULL, 
				"main",
				"vs_2_0", 
				dwShaderFlags, 
				&pCode,
				&pBufferErrors,  // ---- TBD: not functioing as per spec
				&constant_tableVS
			);
		if (FAILED(hr))
		{
			MessageBox
			(
				NULL, 
				"The shader file is (probably) missing...", 
				"Vertex Shader Compile Error",
				MB_OK | MB_ICONEXCLAMATION
			);
			return VS_ERROR;
		}


	// ---------- Create the vertex shader ----------
		hr = d3d9_device->CreateVertexShader
		(
			(DWORD*)pCode->GetBufferPointer(),
			&vertex_shader
		);
		pCode->Release();


	// ---------- Assemble the pixel shader ----------
		CHAR ps_format[256];
		strcpy(ps_format, "data/%s/assets/pixel_shader_%1d.psh");
		if ( IsDebuggerPresent() )
		{
			strcpy(ps_format, "../data/%s/assets/pixel_shader_%1d.psh");
		}
		CHAR ps_filename[VS_MAXCHARLEN];
		sprintf(ps_filename, ps_format, app_name, index);
		hr = 
		D3DXCompileShaderFromFile
		(
			ps_filename,
			NULL, 
			NULL, 
			"main",
			"ps_2_0", 
			dwShaderFlags, 
			&pCode,
			&pBufferErrors, // ---- TBD: not functioing as per spec
			&constant_tablePS
		);
		if (FAILED(hr))
		{
			MessageBox
			(
				NULL, 
				"The shader file is (probably) missing...",
				"Pixel Shader Compile Error",
				MB_OK | MB_ICONEXCLAMATION
			);
			return VS_ERROR;
		}


	// ---------- Create the pixel shader ----------
		hr = d3d9_device->CreatePixelShader
		(
			(DWORD*)pCode->GetBufferPointer(),
			&pixel_shader
		);
		pCode->Release();


	#endif

	return VS_OK;
};


// ---------- Cleanup ----------
INT GfxShaders::Cleanup()
{

		#if VS_GFX_SHADERS


		if ( vertex_shader != NULL )
		{
			vertex_shader->Release();
			vertex_shader = NULL;
		}

		if ( constant_tableVS != NULL )
		{
			constant_tableVS->Release();
			constant_tableVS = NULL;
		}

		if ( vertex_declaration != NULL )
		{
			vertex_declaration->Release();
			vertex_declaration = NULL;
		}

		if ( pixel_shader != NULL )
		{
			pixel_shader->Release();
			pixel_shader = NULL;
		}

		if ( constant_tablePS != NULL )
		{
			constant_tablePS->Release();
			constant_tablePS = NULL;
		}


		#endif

	return VS_OK;
};


// ---------- GetParam ----------
INT GfxShaders::GetParam(
		FLOAT *param
	)
{
	memcpy(param, shader_param, MAX_PARAM * sizeof(FLOAT));
	return VS_OK;
}


// ---------- SetParam ----------
INT GfxShaders::SetParam(
		FLOAT *param
	)
{
	memcpy(shader_param, param, MAX_PARAM * sizeof(FLOAT));
	return VS_OK;
}


// ---------- SetBCS ----------
INT GfxShaders::SetBCS(
		FLOAT brightness,
		FLOAT contrast,
		FLOAT saturation
	)
{

	#if VS_GFX_SHADERS

	constant_tablePS->SetFloat(d3d9_device, "Brightness", brightness);
	constant_tablePS->SetFloat(d3d9_device, "Contrast",   contrast);
	constant_tablePS->SetFloat(d3d9_device, "Saturation", saturation);
	
	#endif
	
	return VS_OK;
}


// ---------- SetBCSEnabled ----------
INT GfxShaders::SetBCSDisabled()
{
	d3d9_device->SetVertexShader(NULL);
	d3d9_device->SetPixelShader(NULL);
	return VS_OK;
}


// ---------- SetBCSEnabled ----------
INT GfxShaders::SetBCSEnabled(
		D3DXMATRIX *mw
	)
{

	#if VS_GFX_SHADERS
	
	D3DXMATRIX worldViewProjection = *mw * matrix_view * matrix_proj;
	constant_tableVS->SetMatrix(d3d9_device, "worldViewProj", &worldViewProjection);
	d3d9_device->SetVertexDeclaration(vertex_declaration);
	d3d9_device->SetVertexShader(NULL);// vertex_shader);
	d3d9_device->SetPixelShader(pixel_shader);
	
	#endif

	return VS_OK;
}


// ---------- SetMatrices ----------
INT GfxShaders::SetMatrices()
{
	#if defined(VS_GFX_SHADERS)

	//D3DXMatrixIdentity(&matrix_world);
	d3d9_device->GetTransform(D3DTS_WORLD, &matrix_world);
	d3d9_device->GetTransform(D3DTS_VIEW, &matrix_view);
	d3d9_device->GetTransform(D3DTS_PROJECTION, &matrix_proj);

	#endif

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
