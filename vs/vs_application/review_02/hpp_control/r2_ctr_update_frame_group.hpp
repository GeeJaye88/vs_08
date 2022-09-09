////////////////////////////////////////////////////////////////////////////////

// ---------- r2_control_update_frame_group.hpp ----------
/*!
\file r2_control_update_frame_group.hpp
\brief 
\author Gareth Edwards
*/


#include "../header/r2_model.h"

using namespace review_02;


////////////////////////////////////////////////////////////////////////////////



// ---------- UpdatePanel ----------
/*!
\brief Update panel
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK

\note

	Context:

		IF panel is NOT NULL THEN
		   context == "immediate" panel context
		ELSE
		   context == "persistant" panel context

*/
INT Control::UpdateFrameGroup(
		INT handle_control_obj
	)
{

	// ---- local
		INT result = VS_OK;
		Control   *control = (Control*)handle_control_obj;
		Model     *model = control->model;
		PI_Camera *pi_camera = control->pi_camera;


	// ---- get & set frame id
		vs_system::AppControl *app_control = control->app_control;
		UINT current_frame_id = model->Get_FrameId();
		UINT frame_id = app_control->element_id;
		model->Set_FrameId(frame_id);


	// ---- lambda ------------------------------------------------------------
		auto copy_ptf_frame_ptf_info = [&]()
		{
			result = model->Copy_ImgPanel_ByFrameId(frame_id,current_frame_id);
		};


	// ---- lambda ------------------------------------------------------------
		auto change_overview_group = [&]()
		{
			result = model->Update_ImgPanel_GroupRow();
		};


	// ---- lambda ------------------------------------------------------------
		auto select_overview_frame = [&]()
		{
			vs_system::AppFrame *frame = control->app_view->GetFrame("Overview");
			INT frame_id = model->Get_ImgPanel_ActiveId();
			result = model->Select_ImgPanel_ByFrameId(frame, frame_id, 'I');
		};


	// ---- lambda ------------------------------------------------------------
		auto select_multiview_frame = [&]()
		{
			vs_system::AppFrame *frame = control->app_view->GetFrame("MultiView");
			INT frame_id = model->Get_ImgPanel_ActiveId();
			result = model->Select_ImgPanel_ByFrameId(frame, frame_id, 'N');
		};


	// ---- lambda ------------------------------------------------------------
		auto select_soloview_frame = [&]()
		{
			vs_system::AppFrame *frame = control->app_view->GetFrame("Solo");
			INT frame_id = vs_system::ObjConfig::IMG_PTZ_SOLO_PANEL_INDEX + 1;
			result = model->Select_ImgPanel_ByFrameId(frame, frame_id, 'I');

			vs_system::ObjConfig *obj_config = model->GetObjConfig();
			result = obj_config->SetImagePanelActiveId(frame_id);
		};


	// ---- lambda ------------------------------------------------------------
		auto select_setings_frame = [&]()
		{
			vs_system::AppFrame *frame = control->app_view->GetFrame("Settings");
			INT frame_id = vs_system::ObjConfig::IMG_PTZ_SETTINGS_PANEL_INDEX + 1;
			result = model->Select_ImgPanel_ByFrameId(frame, frame_id, 'I');
		};


	// ---- handle frame selection setup
		switch ( frame_id )
		{
			case 1 :
			case 2 : // launch frame ( legacy id :-( )
			case 3 : 
			case 4 :
			case 5 :
			case 6 :
			case 7 : // settings
			case 8 : // dashboard
			case 9 : // info
				{

					#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
						copy_ptf_frame_ptf_info();
					#endif


					// ---- zap dashboard flag
						pi_camera->dashboard_only = FALSE;


					// ---- handle specific frame requirments

						if ( frame_id == 2 )
						{
							#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
								change_overview_group();
								select_overview_frame();
							#else
								;
							#endif
						}

						else if ( frame_id == 3 )
						{
							#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
								change_overview_group();
								select_multiview_frame();
							#else
								;
							#endif
						}

						else if ( frame_id == 5 )
						{
							#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
								change_overview_group();
								select_soloview_frame();
							#else
								;
							#endif
						}

						else if (frame_id == 7)
						{
							#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
								select_setings_frame();
							#else
								;
							#endif
						}

						else if ( frame_id == 8 )
						{
							// dashboard
							pi_camera->dashboard_only = TRUE;
						}

						else if ( frame_id == 9 )
						{
							// spec
							INT handle = model->GetDisplayListHandleByName("Frames");
							result = model->SetGroupSwitchedOnOff(handle, FRAMES_BUTTON_GROUP_ID);
						}


					// ---- select frame and panel
						INT result = control->app_view->SelectFrame(frame_id);


					// ---- select frame "active" PTF (if any) ----
						#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
							;
						#else
							result = model->Set_PanoPanel_FrameSelected(frame_id);
						#endif
				}
				break;

			default:
				result = VS_ERROR;
				break;
		}

	return result;
}


////////////////////////////////////////////////////////////////////////////////


