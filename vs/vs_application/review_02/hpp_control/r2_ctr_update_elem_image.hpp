////////////////////////////////////////////////////////////////////////////////

// ---------- r2_control_update_elem_image.hpp ----------
/*!
\file r2_control_update_elem_image.hpp
\brief 
\author Gareth Edwards
*/


#include "../header/r2_model.h"

using namespace review_02;


// ---------- UpdateElemImage ----------
/*!
\brief Update image elements 
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Control::UpdateElemImage(
		Control               *control,
		vs_system::AppControl *app_control,
		vs_system::AppPanel   *app_panel,
		vs_system::PanelContext *panel_context,
		INT panel_group_id
	)
{

	// ---- local 
		INT result = VS_OK;
		vs_system::AppDependency *app_dependency = control->app_dependency;
		vs_system::SysInput      *sys_input      = app_dependency->GetSysInput();
		vs_system::GfxInterface  *gfx_interface  = app_dependency->GetGfxInterface();


	// ---- no config bitmask
		BOOL set_config_bitmask = FALSE;
		BYTE setup_bitmask = VS_ELM_SETUP_NONE;


	// ---- store last
		control->model->GetObjConfig()->SetPanoPTFLast();


	// ---- update
		switch ( panel_group_id )
		{
			case GROUP_ELEM_IMAGE:
				{
	
					// ---- get client adjusted viewport
						VsFloatRectangle cav = { 0, 0, 0, 0 };
						gfx_interface->GetClientAdjustedViewport(&cav);


					// ---- update pan & tilt
						BOOL setup_bitmask = (BYTE)UpdateElemImagePanTilt(
								control,
								app_dependency,
								panel_context,
								&cav
							);


					// ---- update tabbed interface image text and value
						UpdateTintImgText(control);


					// ---- reset
						sys_input->ResetMouse();
						app_control->persistant_panel_context = NULL;

			}
				break;

			default:
				break;
		}

	return VS_OK;
}


// ---------- UpdateElemImagePanTilt ----------
/*!
\brief Pick update image Pan & Tilt 
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Control::UpdateElemImagePanTilt(
		Control *control,
		vs_system::AppDependency *app_dependency,
		vs_system::PanelContext *panel_context,
		VsFloatRectangle *gfx_cav
	)
{

	// ---- local
		INT result = VS_OK;
		vs_system::AppControl *app_control = control->app_control;
		vs_system::SysInput   *sys_input   = app_dependency->GetSysInput();
		Model                 *model       = control->model;
		vs_system::ObjConfig  *obj_config  = model->GetObjConfig();


	// ---- get pano stuff - correct
		FLOAT aspect = 1.0f;
		result = obj_config->GetPanoAspectRatio(&aspect);


	// ---- get gfx id
		UINT gfx_id = 0;
		result = panel_context->GetId(&gfx_id);


	// ---- get pano id
		UINT ptf_id = 0;
		result = obj_config->GetPanoPTFId(&ptf_id);


	// ---- get PTF ( note: F == "zoom" )
		FLOAT pan = 0, tilt = 0, fov = 1;
		result = obj_config->GetPanoPTFById(ptf_id, &pan, &tilt, &fov);


	// ---- set XYZ
		FLOAT x_off = pan;
		FLOAT y_off = tilt;
		FLOAT zoom  = fov <= 1 ? 1 : fov;

	
	// ---- "zoom" calculation
		zoom = 1 + (zoom-1)/10;


	// ---- cav dim
		FLOAT cav_width  = gfx_cav->right - gfx_cav->left;
		FLOAT cav_height = gfx_cav->bottom - gfx_cav->top;


	// ---- calc image dim
		FLOAT img_width  = cav_height * ( ptf_id == 0 ? aspect : ( 1/aspect ) );
		FLOAT img_height = cav_height;
		FLOAT x_min      = ( cav_width - img_width ) / 2;
		FLOAT x_max      = x_min + img_width;


	// ---- pick within cav
		POINT pick_point = { 0, 0 };
		pick_point = *sys_input->GetPosition();
		FLOAT x_pick = (FLOAT)pick_point.x - gfx_cav->left;
		FLOAT y_pick = (FLOAT)pick_point.y - gfx_cav->top;


	//----- "magic":
		// If update from panel with id == 0 THEN
		//    new values NOT proportional to zoom
		// ELSE
		//    new values ARE proportional to zoom


	// ---- set x "magic" offset
		FLOAT new_x_off  = ( x_pick - x_min) / img_width - 0.5f;
		      new_x_off *= 1 / ( gfx_id == 1 ? 1 : zoom );


	// ---- set y "magic" offset
		FLOAT new_y_off  = y_pick / img_height - 0.5f;
		      new_y_off *= 1 / ( gfx_id == 1 ? 1 : zoom );


	// ---- set "magic" PTF parameters
		result = obj_config->SetPanoPTFById(
					ptf_id,
					gfx_id == 1 ?  new_x_off : x_off + new_x_off,
					gfx_id == 1 ? -new_y_off : y_off - new_y_off,
					fov
				);


	// ---- update
		result = model->Update_ImgPanel_Config(ptf_id);

		return VS_OK;
}

