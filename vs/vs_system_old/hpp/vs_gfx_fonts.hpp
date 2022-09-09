////////////////////////////////////////////////////////////////////////////////


// ---------- GfxFonts class ----------

class GfxFonts
{

public:

	// ---- c/dtor ----
	GfxFonts()
	{
		first_font = NULL;
		last_font = NULL;
	}

	~GfxFonts() { Cleanup(); }


	// ---- properties ----
	GfxFont *first_font;
	GfxFont *last_font;


	// ---- methods ----
	INT  Setup(IDirect3DDevice9 *);
	INT  Cleanup();
	INT  AddFont(std::string &, std::string &, UINT, UINT);
	INT  AddProvidedFonts();
	GfxFont *GetFont(std::string &);
	GfxFont *GetFont(UINT);

};


// ---------- Setup ----------
INT GfxFonts::Setup(IDirect3DDevice9* device)
{

	// FOR -- each font
	GfxFont *font = first_font;
	while ( font )
	{
		INT result = font->Setup(device);

		#ifdef VS_DEBUG
		std::cout << "Setup font " << font->name << " ok " << std::endl;
		#endif

		font = font->next;
	}

	return VS_OK;
};


// ---------- Cleanup ----------
INT GfxFonts::Cleanup()
{

	// FOR -- each font
	GfxFont *font = first_font;
	while ( font )
	{
		INT result = font->Cleanup();

		#ifdef VS_DEBUG
		std::cout << "Cleanup font " << font->name << " ok " << std::endl;
		#endif

		font = font->next;
	}

	return VS_OK;
};


// ---------- AddFont ----------
/*!
\brief Add font
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxFonts::AddFont(
		std::string &name,
		std::string &type,
		UINT height,
		UINT weight
	)
{
	// Set permissable font type
	/*
	std::string font_types[17] =
	{
		"Arial",
		"Arial Black",
		"Calibri",
		"Cambria",
		"Comic Sans MS"
		"Courier New",     // monospaced
		"Georgia",
		"Geneva",
		"Impact",
		"Lucida Console",  // monospaced
		"MS-Sans Serif",
		"Soho Gothic Pro",
		"Technic",
		"Times New Roman",
		"Trebuchet",
		"Verdana",
		"Best Match"
	};
	std::string selected_type = "";
	for (  INT i=0; i<17; i++ )
	{
		if ( font_types[i].compare(type) == 0 )
		{
			selected_type = font_types[i];
			break;
		}
	}

	// IF no seclect font THEN default to ""Best Match"
	if ( selected_type.compare("") == 0 )
	selected_type = font_types[16];

	*/


	// ---- set permissable weight or thickness ----
	INT selected_weight = weight < 0 ? 0 : weight > 9 ? 900 : weight * 10;


	// ---- create new font ----
	GfxFont *font = new GfxFont();


	// ---- link new font ----
	if ( first_font==NULL )
	{
		first_font = last_font = font;
	}
	else
	{
		last_font->next = font;
		last_font = font;
	}

	// ---- initialise ----
	font->Add(name, type, height, weight);

	return VS_OK;
};


// ---------- AddProvidedFonts ----------
/*!
\brief Add fonts
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxFonts::AddProvidedFonts()
{

	// ----font structure ----
	struct font_info
	{
		std::string name;
		std::string type;
		UINT height;
		UINT weight;
	};


	// ---- button associated font list ----
	font_info font_list[16] =
	{
		{ "1",  "Courier New", 36, 1 },
		{ "2",  "Courier New", 36, 1 },
		{ "3",  "Courier New", 18, 1 },
		{ "4",  "Courier New", 18, 1 },
		{ "5",  "Courier New", 18, 1 },
		{ "6",  "Courier New", 36, 1 },
		{ "7",  "Courier New", 36, 1 },
		{ "8",  "Courier New", 36, 1 },
		{ "9",  "Courier New", 36, 1 },
		{ "10", "Courier New", 36, 1 },
		{ "11", "Courier New", 18, 1 },
		{ "12", "Courier New", 18, 1 },
		{ "13", "Courier New", 18, 1 },
		{ "14", "Courier New", 36, 1 },
		{ "15", "Courier New", 36, 1 },
		{ "16", "Courier New", 36, 1 }
	};


	// ---- add ----
	INT total_fonts = sizeof(font_list)/sizeof(font_info);
	for ( INT i=0; i<total_fonts; i++ )
	{
		INT result = AddFont(
			font_list[i].name,
			font_list[i].type,
			font_list[i].height,
			font_list[i].weight
		);
	}

	return VS_ERROR;
}


// ---------- GetFont ----------
GfxFont * GfxFonts::GetFont(std::string &name)
{

	if (name == "") return first_font;

	// FOR -- each font
	GfxFont *font = first_font;
	while ( font )
	{
		if ( font->name.compare(name) == 0)
			return font;
		font = font->next;
	}

	return NULL;
};


// ---------- GetFont ----------
GfxFont * GfxFonts::GetFont(UINT index)
{
	// FOR -- each font
	GfxFont *font = first_font;
	UINT i = 0;
	while ( font )
	{
		if ( ++i == index)
			return font;
		font = font->next;
	}

	return NULL;
};


////////////////////////////////////////////////////////////////////////////////
