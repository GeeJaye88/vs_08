////////////////////////////////////////////////////////////////////////////////

// ---------- r2_pano_atlas.hpp ----------
/*!
\file r2_pano_atlas.hpp
\brief
\author Gareth Edwards
*/


#ifndef REVIEW_02_PANO_ATLAS
#define REVIEW_02_PANO_ATLAS


// ---------- application include ----------
#include "../header/r2_model.h"


using namespace review_02;


// ---------- PanoAtlasPageSetup ----------
/*!
\brief Pano Atlas page setup
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::PanoAtlasPageSetup(
		std::string &filename,
		vs_system::ImgBuffer *img_buffer
	)
{
	INT result = gfx->PageSetup(
			pi_model->pano_texture_lid,
			filename,
			img_buffer
		);
	return result;
}


// ---------- PanoAtlasPageLock ----------
/*!
\brief Pano Atlas page lock
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
UCHAR * Model::PanoAtlasPageLock(INT page_handle)
{
	UCHAR *uc = gfx->PageLock(page_handle);
	return uc;
}


// ---------- PanoAtlasPageUnlock ----------
/*!
\brief Pano Atlas page unlock
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::PanoAtlasPageUnlock(INT page_handle)
{
	INT result = gfx->PageUnlock(page_handle);
	return result;
}


#endif // REVIEW_02_PANO_ATLAS


