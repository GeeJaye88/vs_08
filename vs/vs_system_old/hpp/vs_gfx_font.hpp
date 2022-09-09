////////////////////////////////////////////////////////////////////////////////


// ---------- GfxFont class ----------

class GfxFont
{

public:

	//  ---- c/dtor ----
	GfxFont()
	{
		next = NULL;
		id3dx_font = NULL;
		name = "";
		typeface = "";
		height = 0;
		weight = 0;
	}

	~GfxFont()
	{
		Cleanup();
	}


	// ---- properties ----
	GfxFont *next;
	ID3DXFont *id3dx_font;
	std::string name;
	std::string typeface;
	UINT height;
	UINT weight;


	// ---- methods ----
	INT  Add(std::string &, std::string &, UINT, UINT);
	INT  Setup(IDirect3DDevice9*);
	INT  Cleanup();

};


// ---------- Add ----------
INT  GfxFont::Add(std::string &font_name, std::string &font_typeface, UINT font_height, UINT font_weight)
{
	name = font_name;
	typeface = font_typeface;
	height = font_height;
	weight = font_weight;
	return VS_OK;
}


// ---------- Setup ----------
/*!
\brief Add font
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK

pDevice - A pointer to your Direct3D device.

Height - The height (in pixels) of the font you want to create.

Width - The width (in pixels) of the font you want to create.

Set this to 0 for default width based on the height you gave above.

Weight - How thick the font is. Valid values range from 0-1,000.

Common flags and their numeric values are:
FW_DONTCARE     0
FW_THIN       100
FW_EXTRALIGHT 200
FW_ULTRALIGHT 200
FW_LIGHT      300
FW_NORMAL     400
FW_REGULAR    400
FW_MEDIUM     500
FW_SEMIBOLD   600
FW_DEMIBOLD   600
FW_BOLD       700
FW_EXTRABOLD  800
FW_ULTRABOLD  800
FW_HEAVY      900
FW_BLACK      900

MipLevels - Number of MipMaps to create. - 0 creates a full chain.
Used to show the font at different sizes more efficiently.

Since we will be scaling text, it is set to 1.

Italic - True/False, do you want Italics?

It is set to FALSE.

CharSet - Character Set. Used to specify international character
sets (Arabic, Greek, etc).

It is et to DEFAULT_CHARSET.

OutputPrecision - How precisely the output must match the font.
It can probably be left as OUT_DEFAULT_PRECIS.

It isd set to OUT_TT_ONLY_PRECIS

Quality - Some possible values are: ANTIALIASED_QUALITY,
DEFAULT_QUALITY, DRAFT_QUALITY, and PROOF_QUALITY.

It is set to CLEARTYPE_QUALITY.

PitchAndFamily - It can safely be set to DEFAULT_PITCH | FF_DONTCARE.

* Facename - The name of the font. It can be set this to an empty
string ("") and let it pick a font for you that matches the other parameters.

[ returned ] ID3DXFont ** pointer

*/
INT  GfxFont::Setup(IDirect3DDevice9* device)
{
	if ( id3dx_font != NULL ) Cleanup();
	HRESULT hr = D3DXCreateFontA(
			device,
			height,
			0,
			weight,
			0,
			FALSE,
			DEFAULT_CHARSET,
			OUT_TT_ONLY_PRECIS,
			PROOF_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			typeface.c_str(),
			&id3dx_font
		);
	return hr == S_OK ? VS_OK : VS_ERROR;
};


// ---------- Cleanup ----------
INT  GfxFont::Cleanup()
{
	ULONG hr = NULL;
	if ( id3dx_font != NULL ) hr = id3dx_font->Release();
	id3dx_font = NULL;
	return hr == S_OK ? VS_OK : VS_ERROR;
};


////////////////////////////////////////////////////////////////////////////////

