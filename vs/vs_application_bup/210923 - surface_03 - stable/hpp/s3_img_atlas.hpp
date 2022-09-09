////////////////////////////////////////////////////////////////////////////////

// ---------- s3_img_atlas.hpp ----------
/*!
\file s3_img_atlas.hpp
\brief
\author Gareth Edwards
*/

#pragma once


#include "../header/s3_model.h"


using namespace surface_03;


// ---------- AtlasPageSetup ----------
/*!
\brief Atlas page setup
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::AtlasPageSetup(
		std::string &filename,
		vs_system::ImgBuffer *img_buffer
	)
{
	INT result = pi_model->gfx->PageSetup(
			pi_model->img_texture_lid,
			filename,
			img_buffer
		);
	return result;
}


// ---------- AtlasPageLock ----------
/*!
\brief Pano Atlas page lock
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
UCHAR * Model::AtlasPageLock(INT page_handle)
{
	UCHAR *uc = pi_model->gfx->PageLock(page_handle);
	return uc;
}


// ---------- AtlasPageUnlock ----------
/*!
\brief Pano Atlas page unlock
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT Model::AtlasPageUnlock(INT page_handle)
{
	INT result = pi_model->gfx->PageUnlock(page_handle);
	return result;
}
