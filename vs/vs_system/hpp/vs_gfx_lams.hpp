////////////////////////////////////////////////////////////////////////////////


// ---------- GfxAmbient class ----------
class GfxAmbient
{

public:

	// ---- c/dtor ----
	GfxAmbient()
	{
		ZeroMemory(this, sizeof(GfxAmbient));
	}
	~GfxAmbient(){}


	// ---- properties
		D3DCOLORVALUE color = { 0, 0, 0, 0 };
		BOOL ambient_enable = TRUE;;

	// ---- methods

		INT Get(VsAmbient *vs_ambient)
		{
			vs_ambient->colour.red   = color.r;
			vs_ambient->colour.green = color.g;
			vs_ambient->colour.blue  = color.b;
			vs_ambient->colour.alpha = color.a;
			return VS_OK;
		}

		INT Set(VsAmbient *vs_ambient)
		{
			color.r = vs_ambient->colour.red;
			color.g = vs_ambient->colour.green;
			color.b = vs_ambient->colour.blue;
			color.a = vs_ambient->colour.alpha;
			return VS_OK;
		}

};


// ---------- GfxFog class ----------
class GfxFog
{

public:

	// ---- c/dtor ----
	GfxFog()
	{
		ZeroMemory(this, sizeof(GfxFog));
	}
	~GfxFog(){}


	// ---- properties
		D3DFOGMODE     table_mode;          //
		D3DCOLORVALUE  color;               //
		FLOAT          start;               // start (for both vertex and pixel)
		FLOAT          end;                 // end (ditto)
		FLOAT          density;             // density
		BOOL           fog_enable;          // on or off.....
		BOOL           range_enable;        // on or off.....


	// ---- methods
 		INT Set(VsFog *vs_fog)
		{

			switch (vs_fog->table_mode)
			{
				default :
				case VsFogType::VS_FOG_NONE:
					table_mode = D3DFOG_NONE;
					break;
				case VsFogType::VS_FOG_EXP:
					table_mode = D3DFOG_EXP;
					break;
				case VsFogType::VS_FOG_EXP2:
					table_mode = D3DFOG_EXP2;
					break;
				case VsFogType::VS_FOG_LINEAR:
					table_mode = D3DFOG_LINEAR;
					break;
			}
			
			color.r = vs_fog->colour.red;
			color.g = vs_fog->colour.green;
			color.b = vs_fog->colour.blue;
			color.a = vs_fog->colour.alpha;

			start   = vs_fog->start;
			end     = vs_fog->end;
			density = vs_fog->density;
			range_enable = vs_fog->range_enable;

			return VS_OK;
		}

};


// ---------- GfxLights class ----------
class GfxLights
{

public:

	// ---- c/dtor ----
	GfxLights()
	{
		for (INT i = 0; i < VS_MAX_LIGHTS; i++)
		{
			ZeroMemory(&light[i], sizeof(D3DMATERIAL9));
		}
	}

	~GfxLights(){}


	// ---- properties
		D3DLIGHT9 light[VS_MAX_LIGHTS];


	// ---- methods
		INT Set(VsLight *vs_light, UINT i)
		{

			if (i < 0 || i >= VS_MAX_LIGHTS) return VS_ERROR;

			switch (vs_light->type)
			{
				default :
				case VsLightType::VS_LIGHT_POINT:
					light[i].Type = D3DLIGHT_POINT;
					break;
				case VsLightType::VS_LIGHT_DIRECTIONAL:
					light[i].Type = D3DLIGHT_DIRECTIONAL;
					break;
				case VsLightType::VS_LIGHT_SPOT:
					light[i].Type = D3DLIGHT_SPOT;
					break;
			}
			
			light[i].Diffuse.r  = vs_light->diffuse.red;
			light[i].Diffuse.g  = vs_light->diffuse.green;
			light[i].Diffuse.b  = vs_light->diffuse.blue;
			light[i].Diffuse.a  = vs_light->diffuse.alpha;

			light[i].Ambient.r  = vs_light->ambient.red;
			light[i].Ambient.g  = vs_light->ambient.green;
			light[i].Ambient.b  = vs_light->ambient.blue;
			light[i].Ambient.a  = vs_light->ambient.alpha;

			light[i].Specular.r = vs_light->specular.red;
			light[i].Specular.g = vs_light->specular.green;
			light[i].Specular.b = vs_light->specular.blue;
			light[i].Specular.a = vs_light->specular.alpha;

			light[i].Position = D3DXVECTOR3(
						vs_light->position.x,
						vs_light->position.y,
						vs_light->position.z
					);

			light[i].Direction= D3DXVECTOR3(
						vs_light->direction.x,
						vs_light->direction.y,
						vs_light->direction.z
					);

			light[i].Range        = vs_light->range;
			light[i].Falloff      = vs_light->falloff;
			light[i].Attenuation0 = vs_light->attenuation0;
			light[i].Attenuation1 = vs_light->attenuation1;
			light[i].Attenuation2 = vs_light->attenuation2;
			light[i].Theta        = vs_light->theta;
			light[i].Phi          = vs_light->phi;

			return VS_OK;
		}

};


// ---------- GfxMaterials class ----------
class GfxMaterials
{

public:

	// ---- c/dtor ----
	GfxMaterials()
	{
		for (INT i = 0; i < VS_MAX_MATERIALS; i++)
		{
			ZeroMemory(&material[i], sizeof(D3DMATERIAL9));
		}
	}
	~GfxMaterials(){}


	// ---- properties
		D3DMATERIAL9  material[VS_MAX_MATERIALS];


	// ---- methods

		INT Get(VsMaterial *vs_material, UINT i)
		{
			if (i < 0 || i >= VS_MAX_MATERIALS) return VS_ERROR;

			vs_material->diffuse.red    = material[i].Diffuse.r;
			vs_material->diffuse.green  = material[i].Diffuse.g;
			vs_material->diffuse.blue   = material[i].Diffuse.b;
			vs_material->diffuse.alpha  = material[i].Diffuse.a;

			vs_material->ambient.red    = material[i].Ambient.r;
			vs_material->ambient.green  = material[i].Ambient.g;
			vs_material->ambient.blue   = material[i].Ambient.b;
			vs_material->ambient.alpha  = material[i].Ambient.a;

			vs_material->specular.red   = material[i].Specular.r;
			vs_material->specular.green = material[i].Specular.g;
			vs_material->specular.blue  = material[i].Specular.b;
			vs_material->specular.alpha = material[i].Specular.a;

			vs_material->emissive.red   = material[i].Emissive.r;
			vs_material->emissive.green = material[i].Emissive.g;
			vs_material->emissive.blue  = material[i].Emissive.b;
			vs_material->emissive.alpha = material[i].Emissive.a;

			vs_material->power          = material[i].Power;

			return VS_OK;
		}

		INT Set(VsMaterial *vs_material, UINT i)
		{
			if (i < 0 || i >= VS_MAX_MATERIALS) return VS_ERROR;

			material[i].Diffuse.r  = vs_material->diffuse.red;
			material[i].Diffuse.g  = vs_material->diffuse.green;
			material[i].Diffuse.b  = vs_material->diffuse.blue;
			material[i].Diffuse.a  = vs_material->diffuse.alpha;

			material[i].Ambient.r  = vs_material->ambient.red;
			material[i].Ambient.g  = vs_material->ambient.green;
			material[i].Ambient.b  = vs_material->ambient.blue;
			material[i].Ambient.a  = vs_material->ambient.alpha;

			material[i].Specular.r = vs_material->specular.red;
			material[i].Specular.g = vs_material->specular.green;
			material[i].Specular.b = vs_material->specular.blue;
			material[i].Specular.a = vs_material->specular.alpha;

			material[i].Emissive.r = vs_material->emissive.red;
			material[i].Emissive.g = vs_material->emissive.green;
			material[i].Emissive.b = vs_material->emissive.blue;
			material[i].Emissive.a = vs_material->emissive.alpha;

			material[i].Power      = vs_material->power;

			return VS_OK;
		}

};


// ---------- GfxViewMatrix class ----------
class GfxViewMatrix
{

public:

	// ---- c/dtor ----
	GfxViewMatrix()
	{
		ZeroMemory(this, sizeof(GfxViewMatrix));
		for (INT i = 0; i < VS_MAX_VIEW_MATRICES; i++)
			D3DXMatrixIdentity(&_view_matrix[i]);
	}
	~GfxViewMatrix(){}


	// ---- properties
		D3DXMATRIX _view_matrix[VS_MAX_VIEW_MATRICES];


	// ---- methods

		INT Get(UINT index, D3DXMATRIX *view_matrix)
		{
			if (index >= 0 && index < VS_MAX_VIEW_MATRICES)
			{
				*view_matrix = _view_matrix[index];
				return VS_OK;
			}
			return VS_ERROR;
		}

		INT Set(UINT index, D3DXMATRIX *view_matrix)
		{
			if (index >= 0 && index < VS_MAX_VIEW_MATRICES)
			{
				_view_matrix[index] = *view_matrix;
				return VS_OK;
			}
			return VS_ERROR;
		}

};


////////////////////////////////////////////////////////////////////////////////
