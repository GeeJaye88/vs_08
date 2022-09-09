////////////////////////////////////////////////////////////////////////////////

// ---------- r2_model.cpp ----------
/*!
\file r2_model.cpp
\brief Implementation of the Model class.
\author Gareth Edwards 
*/


#include "../header/r2_model.h"
#include "../header/r2_buttons.h"

using namespace review_02;


////////////////////////////////////////////////////////////////////////////////


#include "../hpp_pi/r2_pi_model.hpp"


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Model::Model()
{

	// ---- private implementation
		pi_model = new PI_Model;
		s_pi_model = pi_model;

		pi_context = new PI_Context;


	// ---- texture atlas logical id
		pi_model->pano_texture_lid = 0;


	// ---- dependencies
		pi_model->app_dependency = NULL;
		gfx = NULL;


	// ---- camera
		pi_model->cam_graphs = new vs_system::CamGraphs();
		pi_model->cam_props  = new vs_system::CamProps();


	// ---- camera
		pi_model->img_props = new vs_system::ImgProps();


	// ---- graphic objects
		pi_model->obj_config = new vs_system::ObjConfig();


	// ---- inject camera properties
		pi_model->obj_config->SetCamProps(pi_model->cam_props);



}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
Model::~Model()
{
	if ( pi_model->cam_graphs != NULL ) { delete pi_model->cam_graphs; pi_model->cam_graphs = NULL; }
	if ( pi_model->cam_props  != NULL ) { delete pi_model->cam_props;  pi_model->cam_props  = NULL; }
	if ( pi_model->img_props  != NULL ) { delete pi_model->img_props;  pi_model->img_props  = NULL; }
	if ( pi_model->obj_config != NULL ) { delete pi_model->obj_config; pi_model->obj_config = NULL; }
	if ( pi_model             != NULL ) { delete pi_model;             pi_model             = NULL; }
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Setup(vs_system::AppDependency *app_dependency)
{

	// ---- local
		INT hr;


	// ---- store injected dependency
		pi_model->app_dependency = app_dependency;


	// ---- set shortcut to an injected dependency property
		gfx = app_dependency->GetGfxInterface();
		

	// ---- setup
		hr = SetupButtons();
		hr = SetupUIButtons();

		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
		hr = SetupImgExposureAoi();
		#else
		hr = SetupPanoExposureAoi();
		hr = SetupPanoFocusAoi();
		#endif

		hr = SetupPanoGeometry();
		hr = SetupImgGeometry();
		hr = SetupUIGeometry();

		  
	// ---- add an atlas page for pano texture
		pi_model->pano_texture_lid = gfx->AddPageToAtlas();


	// ---- set pano
		pi_model->obj_config->SetPanoAtlasPageLid(pi_model->pano_texture_lid);


	// ---- set provided FONTS
		INT total_fonts = sizeof(font_list)/sizeof(font_info);
		for ( INT i=0; i<total_fonts; i++)
		{
			INT font_ok = gfx->SetFont(
					i+1,
					font_list[i].name,
					font_list[i].type,
					font_list[i].height,
					font_list[i].weight
				);
		}

	return VS_OK;
}


// ---------- SetupGraphics ----------
/*!
\brief Setup graphics
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupGraphics()
{
	INT result = gfx->SetupGraphics();
	return result;
}


// ---------- Begin ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::DisplayBegin()
{
	return VS_OK;
}

 
// ---------- Display ----------
/*!
\brief Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Display(
		vs_system::PanelContext *panel_context,
		vs_system::ImgBuffer  *dest_img_buffer
	)
{

	// ---- local
		INT result = 0;


	// ---- get panel "name"
		CHAR panel_name[VS_MAXCHARLEN];
		result = panel_context->GetName(panel_name);
		if ( panel_name == NULL ) return VS_ERROR;


	// ---- initialise panel display list
		INT handle_display_list = InitPanelDisplayList(panel_context);
		if ( handle_display_list == NULL ) return VS_ERROR;


	// ---- get status
		BOOL panel_status = TRUE;
		result = panel_context->GetDisplayStatus(&panel_status);
		if ( panel_status == FALSE ) return VS_ERROR;

	 
	// ---- get id
		UINT panel_group_id = 0;
		result = panel_context->GetGroupId(&panel_group_id);


	// IF -- "stream" group -- AND NOT -- vptz control -- THEN --
	//    overide group id and use "spherical" instead.....
		if ( panel_group_id == GROUP_STREAM )
		{
			if ( ! pi_model->onvif_remote_control )
				panel_group_id = GROUP_ELEM_PANO_SPHERICAL;
		}


	// ---- timer ----
		BOOL timer_running = FALSE;
		DOUBLE timer_value = 0;
		result = pi_model->app_dependency->GetSysContext()->GetTimerProgress(&timer_value);
		if ( result == VS_OK )
		{
			timer_running = TRUE;
		}


	// ---- lidar - store
		#if VS_LIDAR
		FLOAT *tmp_perspective = panel_context->GetPerspective();
		FLOAT  tmp_fov = tmp_perspective[0];
		#endif

	
	// -------- PTF --------
		//
		// Pan, tilt and FOV are ObjConfig Pano properties.
		//
		// To update GFXContext XY rotation (pan and tilt) and
		// Perspective field of view (FOV) these properties must be
		// set.
		//
		// Required for all GROUP_ELEM_PANO_SPHERICAL panel except those that
		// have a non-zero id but are NOT the currently selected PTF id.
		//

	// ---- IF "spherical" group THEN --
		if ( panel_group_id == GROUP_ELEM_PANO_SPHERICAL )
		{

			// ---- get panel id
				UINT panel_id = 0;
				result = panel_context->GetId(&panel_id);


			// ---- get selected PTF index
				UINT ptf_index = 0;
				result = pi_model->obj_config->GetPanoPTFId(&ptf_index);
			

			// ---- lidar - overwrite pan
				#if VS_LIDAR
				{
					CHAR name[128];
					panel_context->GetName(name);
					if ( strcmp(name, "3D Full") == 0 )
					{
						if ( pi_model->lidar_pan )
						{
							FLOAT  pan = 0, tilt = 0, fov = 0;
							result = pi_model->obj_config->GetPanoPTF(&pan, &tilt, &fov);
							pan   += pi_model->lidar_pan_incr;
							result = pi_model->obj_config->SetPanoPTF(pan, tilt, fov);
						}
					}
				}
				#endif


			// ---- local context
				FLOAT *perspective = panel_context->GetPerspective();
				FLOAT *rotate = panel_context->GetRotate();
				FLOAT  pan = 0, tilt = 0, fov = 0;


			// IF -- selected panel -- THEN
				if ( panel_id == 0 || panel_id == ptf_index )
				{
					result = pi_model->obj_config->GetPanoPTF(&pan, &tilt, &fov);
					if ( timer_running )
					{
						FLOAT pan_last = 0, tilt_last = 0, fov_last = 0; 
						result = pi_model->obj_config->GetPanoPTFLast(&pan_last, &tilt_last, &fov_last);
						pan  = (1 - (FLOAT)timer_value)*pan_last  + (FLOAT)timer_value*pan;
						tilt = (1 - (FLOAT)timer_value)*tilt_last + (FLOAT)timer_value*tilt;
						fov  = (1 - (FLOAT)timer_value)*fov_last  + (FLOAT)timer_value*fov;
					}
				}
				else
				{
					result = pi_model->obj_config->GetPanoPTFById(panel_id, &pan, &tilt, &fov);
				}


			// ---- reset
				rotate[1]      = pan;
				rotate[0]      = tilt;
				perspective[0] = fov;


			// ---- lidar - restore
				#if VS_LIDAR
				{
					CHAR name[128];
					panel_context->GetName(name);
					if ( strcmp(name, "3D Full") == 0 )
					{
						FLOAT *translate = panel_context->GetTranslate();
						perspective[0] = fov / 2;
						translate[2] = 10;
					}
				}
				#endif


		}


	// ---- ELSE IF -- "stream" --
		else if ( panel_group_id == GROUP_STREAM )
		{

			// ---- transform
				FLOAT *perspective = panel_context->GetPerspective();
				FLOAT *rotate = panel_context->GetRotate();
				FLOAT  pan = 0, tilt = 0, fov = 0;


			// ---- get pano
				result = pi_model->obj_config->GetPanoPTFById(
								vs_system::ObjConfig::STREAM_PANO_PTF_ID,
								&pan,
								&tilt,
								&fov
							);
				rotate[1] = pan;
				rotate[0] = tilt;
				perspective[0] = fov;
		}


	// -------- RENDER --------


	// -- IF -- render to image -- THEN --
		if ( dest_img_buffer != NULL && panel_context->GetRenderToImageStatus() )
		{
			result = panel_context->SetRenderToImageFlag(TRUE);
			result = gfx->RenderToImageViewrect(panel_context);
			result = gfx->RenderToImageBegin();
			result = gfx->RenderToImageViewport(handle_display_list, panel_context);
			result = gfx->RenderToImageBufferEnd(dest_img_buffer);
			result = panel_context->SetRenderToImageFlag(FALSE);
		}


	// ---- ELSE -- render to viewport
		else
		{

			// ---- setup and render view rectangle background
				result = gfx->SetMargin(panel_context->GetMargin());
				result = gfx->SetViewrect(panel_context->GetViewrect());
				result = gfx->RenderViewrect(panel_context, FALSE);


			// ---- setup and render view port AND selected display list
				result = gfx->SetBorder(panel_context->GetBorderRect());
				result = gfx->SetViewport(panel_context->GetViewport());
				result = gfx->RenderViewport(handle_display_list, panel_context);


			// ---- setup and render view rectangle foreground
				result = gfx->RenderViewrect(panel_context, TRUE);

		}


	// ---- lidar 3D
		#if VS_LIDAR
		tmp_perspective[0] = tmp_fov;
		FLOAT *tmp_translate = panel_context->GetTranslate();
		tmp_translate[2] = 0;
		#endif

	return VS_ERROR;
}


// ---------- Display ----------
/*!
\brief Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Display(vs_system::PanelContext *panel_context)
{

	// ---- local
		INT result = 0;


	// ---- setup and render view rectangle background
		result = gfx->SetMargin(panel_context->GetMargin());
		result = gfx->SetViewrect(panel_context->GetViewrect());
		result = gfx->RenderViewrect(panel_context, FALSE);


	// ---- setup and render view rectangle foreground
		result = gfx->RenderViewrect(panel_context, TRUE);

	return VS_OK;
}


// ---------- End ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::DisplayEnd()
{

	BOOL capture_an_image = FALSE;

	if ( capture_an_image )
	{
		vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
		INT trigger_lid = sys_context->GetDWord(vs_system::SysContext::WINDOW_TRIGGER);
		if ( trigger_lid != 0 )
		{
			HWND hWnd = (HWND)sys_context->GetDWord(vs_system::SysContext::WINDOW_HANDLE);
			INT result = CaptureAnImage(hWnd);
			result = sys_context->SetDWord(vs_system::SysContext::WINDOW_TRIGGER, 0);
		}
	}

	/*
	vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
	HWND hWnd = (HWND)sys_context->GetDWord(vs_system::SysContext::WINDOW_HANDLE);
	INT result = CaptureAnImage(hWnd);

	int fps = sys_context->GetDWord(vs_system::SysContext::FPS);
	if (fps != pi_model->fps_current)
	{
		char msg[128];
		sprintf_s(msg, 128, "FPS %d\n", fps);
		OutputDebugString(msg);

		pi_model->fps_current = fps;
	}
	*/

	return VS_OK;
}


// ---------- CleanupGraphics ----------
/*!
\brief Cleanup device
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::CleanupGraphics()
{
	INT result = gfx->CleanupGraphics();
	return result;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::Cleanup()
{
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


INT Model::UpdateAutoPTF()
{
	// ---- local
		INT result = 0;
		vs_system::ObjConfig *obj_config = GetObjConfig();

	// ---- gate
		if (!obj_config->IsAutoPTFRunning()) return VS_OK;

	// ---- get handle_dspl_[ir/mv]_img_button switched on states
		INT h = pi_model->handle_dspl_ir_img_button[0];

	// ---- on/off/update
		if (obj_config->IsAutoPTFStart())
		{
			//OutputDebugString(" -> AutoPTF START\n");
			result = obj_config->SetAutoPTFRunning(TRUE);
		}
		else if (obj_config->IsAutoPTFStop())
		{
			//OutputDebugString(" -> AutoPTF STOP\n");
			result = obj_config->SetAutoPTFRunning(FALSE);
		}
		else
		{
			// ---- update ptf
				result = obj_config->AutoPTFUpdate();

			// ---- update panels
				UINT id = 0;
				result = obj_config->GetPanoPTFId(&id);
				result = Update_ImgPanel_Config(id);

		// ---- update txt
				vs_system::TintCallback dmc;
				result = Tint_ImgPtz_GetCallbacks(&dmc);
				dmc.callback_update_element(
						(INT)this,
						0
					);
				dmc.callback_update_element(
						(INT)this,
						17 /// hmmmmm... bad!!!
					);
		}

		return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---- misc
#include "../hpp/r2_capture_screen.hpp"
#include "../hpp/r2_old_button_geometry.hpp"
#include "../hpp/r2_pano_atlas.hpp"
#include "../hpp/r2_pano_atlas.hpp"


// ---- camera
#include "../hpp_cam/r2_cam_image.hpp"
#include "../hpp_cam/r2_cam_obj_config.hpp"
#include "../hpp_cam/r2_cam_focus_bar_graph.hpp"
#include "../hpp_cam/r2_cam_props.hpp"


// ---- camera c5
#include "../hpp_camera/c5_handle_elem_img_rect.hpp"


// ---- display list
#include "../hpp_display_list/r2_dsp_param_dashboard.hpp"
#include "../hpp_display_list/r2_dsp_about_dashboard.hpp"
#include "../hpp_display_list/r2_dsp_camera_dashboard.hpp"
#include "../hpp_display_list/r2_dsp_gps_dashboard.hpp"
#include "../hpp_display_list/r2_dsp_get_panel_handle.hpp"
#include "../hpp_display_list/r2_dsp_monitor.hpp"
#include "../hpp_display_list/r2_dsp_nest_button.hpp"
#include "../hpp_display_list/r2_dsp_progress_bar.hpp"
#include "../hpp_display_list/r2_dsp_stream.hpp"   
#include "../hpp_display_list/r2_dsp_lidar.hpp"   


// ---- gfx
#include "../hpp_gfx/r2_gfx_get_set.hpp"


// ---- img
#include "../hpp_img/r2_img_props.hpp"


// ---- img panels ----
#include "../hpp_img_panels/r2_img_panels.hpp"


// ---- ptf panels ----
#include "../hpp_ptf_panels/r2_ptf_frames.hpp"
#include "../hpp_ptf_panels/r2_ptf_panels.hpp"


// ---- review
#include "../hpp_review/r2_review_date_time.hpp"
#include "../hpp_review/r2_review_get_set.hpp"


// ---- setup
#include "../hpp_setup/r2_setup_buttons.hpp"
#include "../hpp_setup/r2_setup_img_geom.hpp"
#include "../hpp_setup/r2_setup_img_nav_but.hpp"
#include "../hpp_setup/r2_setup_panels.hpp"
#include "../hpp_setup/r2_setup_pano.hpp"
#include "../hpp_setup/r2_setup_stream.hpp"
#include "../hpp_setup/r2_setup_tint_cam.hpp"
#include "../hpp_setup/r2_setup_tint_img.hpp"
#include "../hpp_setup/r2_setup_tint_get_param.hpp"
#include "../hpp_setup/r2_setup_ui.hpp"


// ---- tabbed interface camera settings
#include "../hpp_tint_cam/r2_tint_cam_enum.h"
#include "../hpp_tint_cam/r2_tint_cam_callback.hpp"
#include "../hpp_tint_cam/r2_tint_cam_update_body.hpp"


// ---- tabbed interface image settings
#include "../hpp_tint_img/r2_tint_img_enum.h"
#include "../hpp_tint_img/r2_tint_img_callback.hpp"
#include "../hpp_tint_img/r2_tint_img_update_body.hpp"
