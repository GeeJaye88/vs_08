////////////////////////////////////////////////////////////////////////////////

// ---------- r2_ptf_frames.hpp ----------
/*!
\file r2_ptf_frames.hpp
\brief
\author Gareth Edwards

\note

See: r2_setup_geometry.hpp for INT Model::SetupButtons()

In which you will find:

	// ---- set "HOME" frame
	UINT home_frame_selected = Get_HomeFrameSelected() - 1;

*/


#ifndef REVIEW_02_PTF_FRAMES
#define REVIEW_02_PTF_FRAMES


// ---------- application include ----------
#include "../header/r2_model.h"


using namespace review_02;


// ---------- Get_FrameId----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
UINT Model::Get_FrameId()
{
	return pi_model->frame_id;
}


// ---------- Set_FrameId----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT  Model::Set_FrameId(UINT id)
{
	pi_model->frame_id = id;
	return VS_OK;
}


// ---------- Get_HomeFrameSelected----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note
*/
UINT Model::Get_HomeFrameSelected()
{
	return pi_model->home_frame_selected;
}


// ---------- Set_HomeFrameSelected----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
\note invoked ONLY at runtime in application::SetupProperties()
\note also sets current frame_id (see Get/Set_FrameId)
*/
INT  Model::Set_HomeFrameSelected(UINT id)
{
	pi_model->home_frame_selected = id;
	pi_model->frame_id = id;
	return VS_OK;
}


// ---------- Set_FrameIcons ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Set_FrameIcons(
		UINT list_id,  // e.g. FRAMES_BUTTON_INDEX
		CHAR *name,
		UINT group_id,
		UINT id,
		UINT row,
		UINT col
	)
{

	// ---- "Frames" button list from which the Frames elements are generated ----
	button_element_descriptor *list = button_list[list_id];


	// IF -- no list -- THEN -- return error
	if (list == NULL ) return VS_ERROR;


	// ---- find and fix button ----
	UINT number_of_elements = button_list_size[list_id];
	for (UINT i = 0; i < number_of_elements; i++)
	{
		if (list[i].group_id == group_id && list[i].id == id)
		{
			list[i].tex_icon_col = col;
			list[i].tex_icon_row = row;
			INT result = gfx->SetGlobalConfigureBitmask(
					VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING,
					name
				);

		}
	}

	return VS_OK;
}


#endif // REVIEW_02_PTF_FRAMES


