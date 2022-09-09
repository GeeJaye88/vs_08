////////////////////////////////////////////////////////////////////////////////

// ---------- r2_control_img_nav.hpp ----------
/*!
\file r2_control_img_nav.hpp
\brief 
\author Gareth Edwards
*/


#pragma once

using namespace review_02;


// ---------- UpdateImgPtzPanelsNavButtons ----------
/*!
\brief 
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Control::UpdateImgPtzPanelsNavButtons(
		Control* control,
		UINT element_id
	)
{

	// ---- note "update icons" in Set_ImgOrPanoPanel_IconSelected ----


	// ---- local
		INT result = VS_OK;
		Model* model = control->model;


	// ---- update panel group row
		UINT g_row = 0;
		model->Get_ImgPanel_GroupRow(&g_row);

			INT i_g_row = (INT)g_row;
			INT g_max  = vs_system::ObjConfig::IMG_PTZ_GROUP_SIZE - 1;
			g_row = element_id == 0 ? 
				(i_g_row - 1 < 0 ? g_max : i_g_row-1 ) :
					(i_g_row + 1 > g_max ? 0 : i_g_row + 1);

			char msg[50];
			sprintf(msg, "+++ %d %d %d \n", (INT)g_row, i_g_row, g_max);
			OutputDebugString(msg);

			/*
			g_row = element_id == 0 ?
						(i_g_row -1 < 0 ? 0 : i_g_row -1) :
							(i_g_row +1 > g_max ? g_max : i_g_row +1);
			*/

		result = model->Set_ImgPanel_GroupRow(g_row);


	// ---- update panel group display status
		result = control->app_view->SelectPanel(NULL);
		BOOL select_first_panel = TRUE;

		// this is suspect
		result = model->Set_ImgPanel_GroupDisplay(select_first_panel);


	// ---- update nav buttons ( - does nothing right now!!! )
		result = model->Set_ImgPanel_NavButtons(g_row);


	// ---- update frame icon
		vs_system::ObjConfig *obj_config = model->GetObjConfig();
		UINT id = 0;
		result = obj_config->GetImagePanelActiveId(&id);
		result = obj_config->SetPanoPtfSelectedById(id);
		result = model->Set_ImgOrPanoPanel_IconSelected(id);


	// --- update all dependent graphics
		result = model->Update_ImgPanel_Config(id);


	// ---- update tabbed interface image text and value
		result = UpdateTintImgText(control);

	return VS_OK;
}
