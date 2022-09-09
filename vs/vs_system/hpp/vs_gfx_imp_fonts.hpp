////////////////////////////////////////////////////////////////////////////////


// ---------- AddFont ----------
/*!
\brief Add font
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT GfxInterface::AddFont(std::string &name, std::string &type, UINT height, UINT weight)
{

	// ---- test permissable weight or thickness
		INT selected_weight = weight < 0 ? 0 : weight > 9 ? 900 : weight * 10;

	// ---- add
		pi_props->gfx_fonts->AddFont(name, type, height, selected_weight);

	return VS_OK;
}


INT GfxInterface::GetFontName(UINT index, std::string &name)
{
	GfxFont *font = pi_props->gfx_fonts->GetFont(index);
	if (font == NULL) return VS_ERROR;
	name = font->name;
	return VS_OK;
}


INT GfxInterface::GetFontType(UINT index, std::string &type)
{
	GfxFont *font = pi_props->gfx_fonts->GetFont(index);
	if (font == NULL) return VS_ERROR;
	type = font->typeface;
	return VS_OK;
}


INT GfxInterface::GetFontHeight(UINT index, UINT *height)
{
	GfxFont *font = pi_props->gfx_fonts->GetFont(index);
	if (font == NULL) return VS_ERROR;
	*height = font->height;
	return VS_OK;
}


INT GfxInterface::GetFontWeight(UINT index, UINT *weight)
{
	GfxFont *font = pi_props->gfx_fonts->GetFont(index);
	if (font == NULL) return VS_ERROR;
	*weight = font->weight;
	return VS_OK;
}


// ---------- SetFont ----------
/*!
\brief Set font
\author Gareth Edwards
\return INT - VS_ERROR < 0 <= VS_OK
*/
INT  GfxInterface::SetFont(
		UINT index,
		std::string &name,
		std::string &type,
		UINT height,
		UINT weight
	)
{

	// ---- get font by index
		GfxFont *font = pi_props->gfx_fonts->GetFont(index);
		if ( font == NULL ) return VS_ERROR;


	// ---- test permissable weight or thickness
		INT selected_height = height < 1 ? 1 : height;
		INT selected_weight = weight < 0 ? 0 : weight > 9 ? 900 : weight * 10;


	// ---- set
		font->name     = name;
		font->typeface = type;
		font->height   = selected_height;
		font->weight   = selected_weight;

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////

