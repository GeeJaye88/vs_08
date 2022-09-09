////////////////////////////////////////////////////////////////////////////////

// ---------- r2_setup_panels.hpp ----------
/*!
\file r2_setup_panels.hpp
\brief
\author Gareth Edwards
*/


#pragma once

#include "../header/r2_model.h"

using namespace review_02;


// ---------- SetupPanels ----------
/*!
\brief Uppdate frameset
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Model::SetupPanels(
		vs_system::AppDependency *app_dependency,
		vs_system::AppView *app_view
	)
{

	// ---- local
		INT result = VS_OK;
		PanelProps pp; 


	// ---- local
		vs_system::AppFrame *selected_frame = app_view->GetSelectedFrame();


	// ---- init parameters
		result = InitialisePanelParam(app_dependency, &pp);


	// ---- init panel rects
		result = InitialisePanoPanelRects(&pp);
		result = InitialisePhotoPanelRects(&pp);
		result = InitialiseInterfacePanelRects(&pp);
		result = InitialiseQuad1PanelRects(&pp);
		result = InitialiseQuad2PanelRects(&pp);
		result = InitialiseImg1PanelRects(&pp); 
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
		result = InitialiseNinePanelImgRects(&pp);
		#else
		result = InitialiseNinePanelPanoRects(&pp);
		#endif
		result = InitialiseGuiPanelRects(&pp);
		result = InitialiseMediaPanelRects(&pp);
		result = InitialiseSixteenPanelRects(&pp);


	// ---- setup panel list


		// set # of panels
			#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
			const int total_panels = 80;
			#else
			const int total_panels = 77;
			#endif

		// panel table
			struct PanelTable {
				CHAR *name;
				UINT  lid;
				UINT  rid;
				VsFloatRectangle fr;
			};


		// initialise
		PanelTable pt[total_panels] =
		{

			// 6 x interface
			{ "Title",          0, 0, pp.title     },
			{ "Frames",         0, 0, pp.frames    },
			{ "Context",        0, 0, pp.context   },
			{ "Timeline",       0, 0, pp.timebar   },
			{ "Clipline",       0, 0, pp.clipbar   },
			{ "Navigation",     0, 0, pp.navig     },

			// 11 x pano
			{ "3D",             1, 0, pp.t3d_big   }, // see pi_model->panel_context_img_rect[0]
			{ "3D Full",        0, 0, pp.t3d_full  },
			{ "3D Small",       0, 0, pp.t3d_small },
			{ "2D",             0, 0, pp.t2d_all   },
			{ "2D Solo",        0, 0, pp.t2d_all   },
			{ "2D Nono",        0, 0, pp.t2d_all   },
			{ "Cylindrical",    0, 0, pp.t3d_big   },
			{ "2D Front",       0, 0, pp.t2d_front },
			{ "2D Back",        0, 0, pp.t2d_back  },
			{ "Annular",        0, 0, pp.ann_big   },
			{ "Annular Small",  0, 0, pp.ann_small },

			// 2 x photo
			{ "Photo",          1, 0, pp.photo_full   },
			{ "Photo Config",   0, 0, pp.photo_config },

			// 11 x GUI
			{ "Monitor",        0, 0, pp.monitor   },
			{ "Nest",           0, 0, pp.nest      },
			{ "GpsData",        0, 0, pp.gpsdata   },
			{ "Cam Head",       0, 0, pp.camhead   },
			{ "Cam Body",       0, 0, pp.cambody   },
			{ "Img Head",       0, 0, pp.imghead   },
			{ "Img Body",       0, 0, pp.imgbody   },
			{ "Img Nav",        0, 0, pp.imgnav    },
			{ "ProgBar",        0, 0, pp.progbar   },
			{ "Dashboard",      0, 0, pp.dashbrd   },
			{ "Spec",           0, 0, pp.specbrd   },

			// 1 x about
			{ "About",          0, 0, pp.about     },

			// 3 x stream
			{ "Stream Hidden",  0, 1, pp.strm_hid  },
			{ "Stream Visible", 0, 0, pp.strm_vis  },
			{ "Stream",         0, 0, pp.strm      },

			// 1 x shared
			{ "Shared Hidden",  0, 2, pp.shared    },


			#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
			// nine (of nine) ptf views (3 rows of 3)
			//
			// note: stored in model::panel_context_img_rect array
			//    max dimension ObjConfig::MAX_PANO_PTF_LUT
			//
			{ "N02",  2, 0, pp.nine[0] },
			{ "N03",  3, 0, pp.nine[1] },
			{ "N04",  4, 0, pp.nine[2] },
			{ "N05",  5, 0, pp.nine[3] },
			{ "N06",  6, 0, pp.nine[4] },
			{ "N07",  7, 0, pp.nine[5] },
			{ "N08",  8, 0, pp.nine[6] }, 
			{ "N09",  9, 0, pp.nine[7] },
			{ "N10", 10, 0, pp.nine[8] },
			#else
			// six (of nine) ptf views (2 rows of 3)
			//
			// note: stored in model::panel_context_img_rect array
			//       max dimension ObjConfig::MAX_PANO_PTF_LUT
			//
			{ "N1",   2, 0, pp.nine[0] },
			{ "N2",   3, 0, pp.nine[1] },
			{ "N3",   4, 0, pp.nine[2] },
			{ "N4",   5, 0, pp.nine[3] },
			{ "N5",   6, 0, pp.nine[4] },
			{ "N6",   7, 0, pp.nine[5] },
			#endif


			// view and navigation images
			//
			// note: see pi_model->panel_context_img_rect[]
			//       max dimension ObjConfig::MAX_PANO_PTF_LUT
			//
			{ "I01",  1, 0, pp.ptf_img[0]  }, // multiview
			{ "I02",  2, 0, pp.ptf_img[1]  }, // nav img's
			{ "I03",  3, 0, pp.ptf_img[2]  },
			{ "I04",  4, 0, pp.ptf_img[3]  },
			{ "I05",  5, 0, pp.ptf_img[4]  },
			{ "I06",  6, 0, pp.ptf_img[5]  },
			{ "I07",  7, 0, pp.ptf_img[6]  },
			{ "I08",  8, 0, pp.ptf_img[7]  },
			{ "I09",  9, 0, pp.ptf_img[8]  },
			{ "I10", 10, 0, pp.ptf_img[9]  },
			{ "I11", 11, 0, pp.ptf_img[10] },
			{ "I12", 12, 0, pp.ptf_img[11] }, // solo view
			{ "I13", 13, 0, pp.ann_big     }, // settings
			{ "I14", 14, 0, pp.ann_big     },
			{ "I15", 15, 0, pp.ann_big     },
			{ "I16", 16, 0, pp.ann_big     },


			// 4 x 2x2
			{ "Q1",  2, 0, pp.quad2[0] },
			{ "Q2",  3, 0, pp.quad2[1] },
			{ "Q3",  5, 0, pp.quad2[2] },
			{ "Q4",  6, 0, pp.quad2[3] },


			// 16 x 4x4
			{ "H1",  2, 0, pp.h1[0] },
			{ "H2",  3, 0, pp.h1[1] },
			{ "H3",  4, 0, pp.h1[2] },
			{ "H4",  5, 0, pp.h1[3] },
			{ "H5",  6, 0, pp.h1[4] },
			{ "H6",  7, 0, pp.h1[5] },
			{ "H7",  2, 0, pp.h1[6] },
			{ "H8",  3, 0, pp.h1[7] },
			{ "H9",  4, 0, pp.h1[8] },
			{ "H10", 5, 0, pp.h1[9] },
			{ "H11", 6, 0, pp.h1[10] },
			{ "H12", 7, 0, pp.h1[11] },
			{ "H13", 2, 0, pp.h1[12] },
			{ "H14", 3, 0, pp.h1[13] },
			{ "H15", 4, 0, pp.h1[14] },
			{ "H16", 5, 0, pp.h1[15] },

		};

		OutputDebugString("------- SetupPanels\n");


	//
	// ---- Note: fails if no name match!!
	//


	// ---- frames
		CHAR name[VS_MAXCHARNAME];
		vs_system::AppFrame *frame = app_view->GetFirstFrame();
		while ( frame )
		{

			// ---- panels
				vs_system::AppPanel *panel = frame->GetFirstPanel();
				while ( panel )
				{

					/// ---- context
						vs_system::PanelContext *panel_context = panel->GetPanelContext();


					// ---- NOT valid runtime source index ? find source index
						//
						// note: only once if master source panel
						//
						// note: there may be multiple panels with same name
						//       due to master source panel ...
						//
						INT source_index = panel->GetIndex();
						if ( source_index < 0 )
						{
							INT result = panel_context->GetName(name);
							for (UINT i = 0; i < total_panels; i++)
							{
								if ( pt[i].name != 0 && !strcmp(pt[i].name, name) )
								{
									source_index = i;
									panel->SetIndex(i); 
									break;
								}
							}
						}


					// ---- source INDEX ?
						//
						// note: ALL really important
						//
						if ( source_index >= 0 )
						{

							// ---- copy source rect
								VsFloatRectangle *vr = panel_context->GetViewrect();
								*vr = pt[source_index].fr;


							// ---- set source logical id
								INT lid = pt[source_index].lid;
								if ( lid != 0 )
								{
									panel_context->SetId(pt[source_index].lid);
								}


							// ---- set panel context
								if ( lid != 0 )
								{

									// img "I##"
									//
									// note: "live" update/set panel context for navigation requires
									//        this to be set in Model::Get_ImgPanel_HandleDisplayList
									//
									// note: see this method for more info...
									//
									if (
										strlen(pt[source_index].name) == 3 &&
											pt[source_index].name[0] == 'I' &&
												(
													pt[source_index].name[1] == '0' ||
													pt[source_index].name[1] == '1'
												)
										)
									{

										// note: pi_model->panel_context_img_rect[lid - 1] = panel_context;
										;
									}

									// nono "N#"
									else if (
										strlen( pt[source_index].name) == 2 ||
											pt[source_index].name[0] == 'N' &&
												(
													pt[source_index].name[1] >= '1' ||
													pt[source_index].name[1] <= '6'
												)
										)
									{
										pi_model->panel_context_ptf[lid - 1] = panel_context;
									}

									// nono "N##"
									else if (
										strlen( pt[source_index].name) == 3 ||
											pt[source_index].name[0] == 'N' &&
												(
													pt[source_index].name[1] == '0' ||
													pt[source_index].name[1] == '1'
												)
										)
									{
										pi_model->panel_context_ptf[lid - 1] = panel_context;
									}

									// #1 frame pano
									else if
										(
											!strcmp(pt[source_index].name, "3D")
										)
									{
										pi_model->panel_context_ptf[lid - 1] = panel_context;
									}

								}


							// ---- set render id
								//
								// note: used for stream and render to memory
								//
								INT rid = pt[source_index].rid;
								if ( rid != 0 )
								{
									panel_context->SetRenderToImageId(rid);
									panel_context->SetRenderToImageStatus(FALSE);
								}

						}

					panel = panel->GetNext();
				}

			frame = frame->GetNext();
		}

	return VS_OK;
}


// ---------- InitialisePanelParam ----------
/*!
\brief Initialise panel parameters
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Model::InitialisePanelParam(
	vs_system::AppDependency *app_dependency,
	PanelProps *pp
)
{

	// ---- local
		INT result = 0;
		vs_system::SysContext *sys_context = app_dependency->GetSysContext();


	// ---- image_aspect
		pp->image_aspect = 1.0;

		#if defined(VS_REVIEW_02) || defined(VS_CAMERA_03)

			pp->image_aspect = 1.33333f;

			FLOAT image_height = 1.0f;
			FLOAT image_width  = image_height * pp->image_aspect;

		#else

			FLOAT image_width = 1.0f;
			result = pi_model->cam_props->GetProperty(
					vs_system::CamProps::ROI,
					0,
					vs_system::CamProps::WIDTH,
					&image_width
				);
			FLOAT image_height = 1.0f;
			result = pi_model->cam_props->GetProperty(
					vs_system::CamProps::ROI,
					0,
					vs_system::CamProps::HEIGHT,
					&image_height
				);
			pp->image_aspect = image_width / image_height;

		#endif


	// ---- get application "name"
		CHAR app_name[VS_MAXCHARNAME];
		INT hr = app_dependency->GetAppName(app_name);


	// ---- pp->panels_[*] camera flags
		#if defined(VS_CAMERA_05)
			pp->panels_pano = FALSE;
		#endif


	// ----pp->panels_[*] review flags
		#if defined(VS_REVIEW_02) || defined(VS_REVIEW_04)
			pp->panels_timeline = TRUE;
			pp->panels_settings = FALSE;
		#else
			pp->panels_timeline = FALSE;
			pp->panels_settings = TRUE;
		#endif


	// ---- pano
		FLOAT v_radius_min = 0;
		FLOAT v_radius_max = 1;
		result = pi_model->obj_config->GetPanoRadii(&v_radius_min, &v_radius_max);


	// ---- frame_
		RECT client_rect;
		sys_context->GetClientRect(&client_rect);
		pp->frame_width = (FLOAT)(client_rect.right - client_rect.left);
		pp->frame_height = (FLOAT)(client_rect.bottom - client_rect.top);
		pp->frame_aspect = pp->frame_width / pp->frame_height;
		pp->frame_prop_border = sys_context->GetFloat(sys_context->BORDER_WIDTH);
		pp->frame_portrait = pp->frame_aspect < 1 ? TRUE : FALSE;


	// ---- y proportional
		pp->y_prop_space = 0.005f;
		pp->y_prop_border = pp->y_prop_space * pp->frame_prop_border;


	// -------- Y

		FLOAT y_bar_prop = 0.070f;
		FLOAT y_bar = pp->y_prop_space * 3;

		FLOAT y_title_scalar = 0.8f;
		FLOAT y_timeline_scalar = 0.8f;
		FLOAT y_navigation_scalar = 1.6f;
		if ( pp->frame_portrait )
		{
			y_title_scalar = 0.5f;
			y_timeline_scalar = 0.5f;
			y_navigation_scalar = 1.0f;
		}


	// ---- pp->y_title_ - only used in lut
		pp->y_title_top = pp->y_prop_border;
		pp->y_title_bottom = pp->y_title_top + y_bar_prop * y_title_scalar;

	// ---- pp->y_timeline_ - only used in lut
		pp->y_timeline_top = 1 - y_navigation_scalar * y_bar_prop - pp->y_prop_border - y_bar;
		pp->y_timeline_bottom = 1 - y_timeline_scalar * y_bar_prop - pp->y_prop_border - y_bar;

	// ---- pp->y_navigation_ - only used immediatly below and in lut
		pp->y_navigation_top = pp->y_timeline_bottom;
		pp->y_navigation_bottom = 1 - pp->y_prop_border - y_bar;


	// ---- pp->y_prop_content_
		pp->y_prop_content_top = pp->y_title_bottom + pp->y_prop_space * 3;
		if ( pp->frame_portrait )
		{
			// content height proportional to - frame height / image height
			/*
			FLOAT x_prop_space = pp->y_prop_space / pp->frame_aspect * pp->frame_prop_border * 2;
			FLOAT ratio = (pp->frame_height * (1 - x_prop_space)) / image_height;
			FLOAT f_w = 768, f_h = 1366;
			FLOAT f_r = f_h / f_w;
			FLOAT i_w = 4096, i_h = 2304;
			*/

			FLOAT i_r = image_height / image_width;
			FLOAT r   = i_r * pp->frame_aspect * 1.28f;

			pp->y_prop_content_bottom = pp->y_prop_content_top + r;


		}
		else
		{
			pp->y_prop_content_bottom = pp->panels_timeline ?
				pp->y_timeline_top - pp->y_prop_space - y_bar :
				1.0f - (pp->y_prop_border + y_bar);
		}
		pp->y_prop_content_height = pp->y_prop_content_bottom - pp->y_prop_content_top;


	// ---- y_prop_panorama
		FLOAT panorama_ratio = 6.2831853f / (1.75f * (v_radius_max - v_radius_min));
		FLOAT x_panorama_pixels = (pp->frame_width - (pp->y_prop_space * 2)) / panorama_ratio;
		pp->y_prop_panorama = x_panorama_pixels / pp->frame_height;


	// -------- X


	// ---- pp->x_prop_
		pp->x_prop_space = pp->y_prop_space / pp->frame_aspect;
		pp->x_prop_left  = pp->x_prop_space * pp->frame_prop_border;
		pp->x_prop_right = 1 - pp->x_prop_left;
		pp->x_prop_width = pp->x_prop_right - pp->x_prop_left;


	// ---- pp->x_prop_panel_asp_
		FLOAT x_pane_scalar = pp->y_prop_content_height / pp->frame_aspect * pp->image_aspect;
		pp->x_prop_panel_asp_width  = pp->x_prop_width  * x_pane_scalar;
		pp->x_prop_panel_asp_left   = 0.5f - pp->x_prop_panel_asp_width / 2;
		pp->x_prop_panel_asp_right  = 0.5f + pp->x_prop_panel_asp_width / 2;

	return VS_OK;
}


// ---------- InitialiseInterfacePanelRects ----------
/*!
\brief Initialise interface panel VsFloatRectangle
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Model::InitialiseInterfacePanelRects(PanelProps *pp)
{

	pp->title.left     = pp->x_prop_left;
	pp->title.right    = pp->x_prop_right;
	pp->title.top      = pp->y_title_top;
	pp->title.bottom   = pp->y_title_bottom;


	pp->frames.left    = pp->x_prop_left;
	pp->frames.right   = pp->x_prop_right;
	pp->frames.top     = pp->y_title_top;
	pp->frames.bottom  = pp->y_title_bottom;
	if ( pp->frame_portrait )
	{
		pp->frames.top    = pp->y_prop_content_bottom + pp->y_prop_space * 3;
		pp->frames.bottom = pp->frames.top + (pp->y_title_bottom - pp->y_title_top);
	}


	pp->context.left   = pp->x_prop_left;
	pp->context.right  = pp->x_prop_right;
	pp->context.top    = pp->y_timeline_top;
	pp->context.bottom = pp->y_timeline_bottom;


	pp->timebar.left   = pp->x_prop_left;
	pp->timebar.right  = pp->x_prop_right;
	pp->timebar.top    = pp->y_timeline_top;
	pp->timebar.bottom = pp->y_timeline_bottom;


	pp->clipbar.left   = pp->x_prop_left;
	pp->clipbar.right  = pp->x_prop_right;
	pp->clipbar.top    = pp->y_timeline_top;
	pp->clipbar.bottom = pp->y_timeline_bottom;


	pp->navig.left     = pp->x_prop_left;
	pp->navig.right    = pp->x_prop_right;
	pp->navig.top      = pp->y_navigation_top;
	pp->navig.bottom   = pp->y_navigation_bottom;

	return VS_OK;
}


// ---------- InitialisePanoPanelRects ----------
/*!
\brief Initialise pano panel VsFloatRectangle
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Model::InitialisePanoPanelRects(PanelProps *pp)
{

	// ---- annular big - located to left of panorama config panel
		// FLOAT x_panorama_prop = pp->y_prop_content_height / pp->frame_aspect;
		pp->ann_big.left   = pp->panels_timeline ? pp->x_prop_left : pp->x_prop_left;
		pp->ann_big.right  = pp->panels_timeline ? pp->x_prop_right : pp->x_prop_right - 0.3f;
		pp->ann_big.top    = pp->y_prop_content_top;
		pp->ann_big.bottom = pp->y_prop_content_bottom;


	// ---- annular small
		FLOAT y_prop_annular_small = pp->y_prop_content_height - pp->y_prop_panorama - pp->y_prop_space;
		FLOAT x_prop_annular_small = y_prop_annular_small * pp->frame_height / pp->frame_width;
		pp->ann_small.left   = pp->x_prop_right - x_prop_annular_small;
		pp->ann_small.right  = pp->x_prop_right;
		pp->ann_small.top    = pp->y_prop_content_top;
		pp->ann_small.bottom = pp->y_prop_content_top + y_prop_annular_small;


	// -------- 3D spherical --------

		// 3d big spherical
		pp->t3d_big.top    = pp->y_prop_content_top;
		pp->t3d_big.bottom = pp->y_prop_content_bottom - pp->y_prop_panorama - pp->y_prop_space;
		pp->t3d_big_height = pp->t3d_big.bottom - pp->t3d_big.top;
		pp->t3d_big_width  = pp->t3d_big_height / pp->frame_aspect * pp->image_aspect;
		pp->t3d_big.left   = 0.5f - pp->t3d_big_width / 2;
		pp->t3d_big.right  = 0.5f + pp->t3d_big_width / 2;

		// 3d full spsherical
		pp->t3d_full.left   = pp->x_prop_panel_asp_left  - 0.05f;    // Added -/+ 0.05f on 190827
		pp->t3d_full.right  = pp->x_prop_panel_asp_right + 0.05f;
		pp->t3d_full.top    = pp->y_prop_content_top;
		pp->t3d_full.bottom = pp->y_prop_content_bottom;

		// 3d small spherical
		pp->t3d_small.top    = pp->y_prop_content_top;
		pp->t3d_small.bottom = pp->y_prop_content_top + y_prop_annular_small;
		pp->t3d_small_height = pp->t3d_small.bottom - pp->t3d_small.top;
		pp->t3d_small_width  = pp->t3d_small_height * pp->image_aspect / pp->frame_aspect;

		// housekeeping
		FLOAT x_space_by2 = pp->x_prop_space / 2;
		FLOAT half = pp->x_prop_width / 2 - x_space_by2;

		// update
		pp->t3d_small_width = pp->t3d_small_width > half ? half - x_space_by2 : pp->t3d_small_width;

		// housekeeping
		FLOAT x_3D_offset = (pp->x_prop_width - (pp->t3d_small_width * 2 + pp->x_prop_space)) / 2;

		// small 3d spherical
		pp->t3d_small.left = pp->x_prop_left + x_3D_offset;
		pp->t3d_small.right = pp->t3d_small.left + pp->t3d_small_width;


	// ---- x adjust small annular to aluign with small 3D
		pp->ann_small.left  = pp->t3d_small.right + pp->x_prop_space;
		pp->ann_small.right = pp->ann_small.left + pp->t3d_small_width;


	// -------- 2D panorama --------

		// legwork....
		pp->t2d_all.left   = pp->x_prop_left;
		pp->t2d_all.right  = pp->x_prop_right;
		pp->t2d_all.top    = pp->y_prop_content_bottom - pp->y_prop_panorama;
		pp->t2d_all.bottom = pp->y_prop_content_bottom;

		// housekeeping - so start with twice the width and half the height....
		FLOAT y_2D_demi_prop = pp->y_prop_panorama * 2;
		FLOAT x_2D_demi = (pp->x_prop_right - pp->x_prop_left) / 2;

		// scaler to fit
		FLOAT y_2D_demi_total = y_2D_demi_prop * 2 + pp->y_prop_space;
		FLOAT y_2D_demi_max   = pp->y_prop_content_bottom - pp->y_prop_content_top;
		FLOAT scalar = 1.0;
		if (y_2D_demi_total > y_2D_demi_max)
			scalar = y_2D_demi_max / y_2D_demi_total;
		y_2D_demi_prop *= scalar;
		x_2D_demi      *= scalar;

		// front demi - located above Back
		pp->t2d_front.left   = 0.5f - x_2D_demi;
		pp->t2d_front.right  = 0.5f + x_2D_demi;
		pp->t2d_front.top    = pp->y_prop_content_top;
		pp->t2d_front.bottom = pp->y_prop_content_top + y_2D_demi_prop;

		// back demi - located below Front
		pp->t2d_back.left   = pp->t2d_front.left;
		pp->t2d_back.right  = pp->t2d_front.right;
		pp->t2d_back.top    = pp->t2d_front.bottom + pp->y_prop_space;
		pp->t2d_back.bottom = pp->t2d_back.top + y_2D_demi_prop;

	return VS_OK;
}


// ---------- InitialisePhotoPanelRects ----------
/*!
\brief Initialise photo panel VsFloatRectangle
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Model::InitialisePhotoPanelRects(PanelProps *pp)
{

	// ---- photo

		FLOAT x_panorama_prop  = pp->y_prop_content_height / pp->frame_aspect;

		// full
		pp->photo_full.left    = pp->x_prop_left;
		pp->photo_full.right   = pp->x_prop_right;
		pp->photo_full.top     = pp->y_prop_content_top;
		pp->photo_full.bottom  = pp->y_prop_content_bottom;

		// config
		pp->photo_config.left    = pp->panels_timeline ? pp->x_prop_left : pp->x_prop_left;
		pp->photo_config.right   = pp->panels_timeline ? pp->x_prop_right : pp->x_prop_right - 0.3f;
		pp->photo_config.top     = pp->y_prop_content_top;
		pp->photo_config.bottom  = pp->y_prop_content_bottom;

	return VS_OK;
}


// ---------- InitialiseGuiPanelRects ----------
/*!
\brief Initialise gui panel VsFloatRectangle
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Model::InitialiseGuiPanelRects(PanelProps *pp)
{

	// ---- y
		FLOAT top = pp->y_prop_content_top;
		FLOAT bottom = pp->y_prop_content_bottom;
		if ( pp->frame_portrait )
		{
			top    = pp->y_prop_content_top;
			bottom = pp->ptf_img[10].bottom;
		}


	// ---- width - proportional to height
		FLOAT x_s_width = (bottom - top) / pp->frame_aspect * pp->image_aspect;


	// ---- x
		FLOAT x_prop_left  = pp->x_prop_left;
		FLOAT x_prop_right = pp->x_prop_right;
		if ( pp->frame_portrait )
		{
			x_prop_left  = 0.5f - x_s_width / 2;
			x_prop_right = 0.5f + x_s_width / 2;
		}


	// ---- noise monitor
		pp->monitor.left    = pp->x_prop_left;
		pp->monitor.top     = pp->y_title_top;
		pp->monitor.right   = pp->x_prop_right;
		pp->monitor.bottom  = pp->y_prop_content_bottom;


	// ---- nest
		pp->nest.left       = pp->x_prop_left;
		pp->nest.top        = pp->y_title_top;
		pp->nest.right      = pp->x_prop_right;
		pp->nest.bottom     = pp->y_prop_content_bottom;


	// ---- gpsdata bar
		pp->gpsdata.left    = pp->t2d_all.left;
		pp->gpsdata.top     = pp->y_timeline_bottom;
		pp->gpsdata.right   = pp->t2d_all.right;
		pp->gpsdata.bottom  = pp->t2d_all.bottom;


	// ---- cam housekeeping
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
		FLOAT x_cam_left    = pp->imghead.left;
		FLOAT x_cam_right   = pp->imghead.right;
		FLOAT y_cam_top     = pp->imghead.top;
		FLOAT y_cam_bottom  = pp->imghead.bottom;
		#else
		FLOAT mu = 0.9f;
		FLOAT x_cam_left    = (1 - mu)*pp->ann_big.left + mu * pp->ann_big.right; // pp->ann_big.right;
		FLOAT x_cam_right   = pp->x_prop_right;
		FLOAT y_cam_top     = pp->ann_big.top;
		FLOAT y_cam_bottom  = pp->ann_big.bottom;
		#endif


	// ---- cam align annular big panel
		if ( pp->panels_settings )
		{
			pp->ann_big.left  = pp->x_prop_left;
			pp->ann_big.right = x_cam_left;
		}
		else
		{
			pp->ann_big.left  = pp->t3d_full.left;
			pp->ann_big.right = pp->t3d_full.right;
		}


	// ---- camera tabs
		pp->camhead.left   = x_cam_left;
		pp->camhead.top    = y_cam_top;
		pp->camhead.right  = x_cam_right;
		pp->camhead.bottom = y_cam_bottom;


	// ---- camera control
		pp->cambody.left   = x_cam_left;
		pp->cambody.top    = y_cam_top;
		pp->cambody.right  = x_cam_right;
		pp->cambody.bottom = y_cam_bottom;


	// ---- clip export progress bar
		pp->progbar.left   = pp->t2d_all.left;
		pp->progbar.top    = pp->y_timeline_top - (pp->y_prop_border*0.85f); // was 0.65
		pp->progbar.right  = pp->t2d_all.right;
		pp->progbar.bottom = pp->y_timeline_top - (pp->y_prop_border*0.2f);  // was 0.4


	// ---- camera dashboard
		pp->dashbrd.left   = pp->x_prop_left;
		pp->dashbrd.top    = pp->y_title_top;
		pp->dashbrd.right  = pp->x_prop_right;
		pp->dashbrd.bottom = pp->y_navigation_bottom;


	// ---- specification dashboard
		pp->specbrd.left   = pp->x_prop_left;
		pp->specbrd.top    = pp->y_title_top;
		pp->specbrd.right  = pp->x_prop_right;
		pp->specbrd.bottom = pp->y_navigation_bottom;

	return VS_OK;
}


// ---------- InitialiseMediaPanelRects ----------
/*!
\brief Initialise stream panel VsFloatRectangle
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Model::InitialiseMediaPanelRects(PanelProps *pp)
{

	// -------- STREAM --------

	// ---- y
		FLOAT top    = pp->y_prop_content_top;
		FLOAT bottom = pp->y_prop_content_bottom;


	// ---- portrait nine height as per overview
		if ( pp->frame_portrait )
		{
			bottom = pp->ptf_img[10].bottom;
		}


	// ---- width - proportional to height
		FLOAT x_s_width = (bottom - top) / pp->frame_aspect * pp->image_aspect;


	// ---- x
		FLOAT left, right;
		if ( pp->frame_portrait )
		{
			left  = pp->x_prop_left;
			right = pp->x_prop_right;
		}
		else
		{
			left  = 0.5f - x_s_width / 2;
			right = 0.5f + x_s_width / 2;
		}


		pp->strm_hid.left   = 0;     //left;
		pp->strm_hid.right  = 1;     //right;
		pp->strm_hid.top    = top;   //pp->ann_big.top;
		pp->strm_hid.bottom = bottom;

		pp->strm_vis.left   = left;
		pp->strm_vis.right  = right;
		pp->strm_vis.top    = top; //pp->ann_big.top;
		pp->strm_vis.bottom = bottom;

		pp->strm.left       = left;
		pp->strm.right      = right;
		pp->strm.top        = pp->y_title_top;
		pp->strm.bottom     = pp->y_prop_content_bottom;


	// ---- shared memory
		pp->shared.left     = left;
		pp->shared.right    = right;
		pp->shared.top      = pp->ann_big.top;
		pp->shared.bottom   = bottom;


	// ---- about (not used)
		pp->about.left      = left;
		pp->about.right     = right;
		pp->about.top       = pp->y_title_top;
		pp->about.bottom    = pp->y_prop_content_bottom;

	return VS_OK;
}


// ---------- InitialiseQuad1PanelRects ----------
/*!
\brief Initialise Q # 1 panel VsFloatRectangle
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Model::InitialiseQuad1PanelRects(PanelProps *pp)
{

	// ---- legwork...
		FLOAT ycen = (pp->y_prop_content_bottom + pp->y_prop_content_top) / 2;


	// ---- x layout
		FLOAT x1 = pp->x_prop_panel_asp_left;
		FLOAT x2 = 0.5f - pp->x_prop_space / 2;
		FLOAT x3 = 0.5f + pp->x_prop_space / 2;
		FLOAT x4 = pp->x_prop_panel_asp_right;


	// ---- y layout
		FLOAT y1 = pp->y_prop_content_top;
		FLOAT y2 = ycen - pp->y_prop_space / 2;
		FLOAT y3 = ycen + pp->y_prop_space / 2;
		FLOAT y4 = pp->y_prop_content_bottom;


	// ---- 4 x 2x2
		VsFloatRectangle quad[4] =
		{
			{ x1, y1, x2, y2 },
			{ x3, y1, x4, y2 },
			{ x1, y3, x2, y4 },
			{ x3, y3, x4, y4 },
		};
		for (INT i = 0; i < 4; i++) *(pp->quad1+i) = quad[i];

	return VS_OK;
}


// ---------- InitialiseQuad2PanelRects ----------
/*!
\brief Initialise Q # 2 panel VsFloatRectangle
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Model::InitialiseQuad2PanelRects(PanelProps *pp)
{

	// ---- x layout
		FLOAT x1 = pp->x_prop_left;
		FLOAT x2 = 0.5f - pp->x_prop_space / 2;
		FLOAT x3 = 0.5f + pp->x_prop_space / 2;
		FLOAT x4 = pp->x_prop_right;


	// ---- y layout
		FLOAT y1 = pp->y_prop_border;
		FLOAT y2 = 0.5f - pp->y_prop_space / 2;
		FLOAT y3 = 0.5f + pp->y_prop_space / 2;
		FLOAT y4 = 1 - pp->y_prop_border;


	// ---- 4 x 2x2
		VsFloatRectangle quad[4] =
		{
			{ x1, y1, x2, y2 },
			{ x3, y1, x4, y2 },
			{ x1, y3, x2, y4 },
			{ x3, y3, x4, y4 },
		};
		for (INT i = 0; i < 4; i++) *(pp->quad2+i) = quad[i];

	return VS_OK;
}


// ---------- InitialiseNinePanelImgRects ----------
/*!
\brief Initialise N # 1 panel VsFloatRectangle
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Model::InitialiseNinePanelImgRects(PanelProps *pp)
{

	// ---- housekeeping
		FLOAT top    = pp->y_prop_content_top; 
		FLOAT bottom = pp->ptf_img[10].bottom; // was pp->y_prop_content_bottom;

		FLOAT xsp    = pp->x_prop_space;
		FLOAT ysp    = pp->y_prop_space;
		FLOAT left   = pp->x_prop_left;
		FLOAT right  = pp->frame_portrait ? pp->x_prop_right : pp->imgbody.left - xsp; // was pp->x_prop_right;

		FLOAT xinc   = ( (right - left) - xsp * 2 ) / 3;
		FLOAT yinc   = ( (bottom - top) - ysp * 2 ) / 3;


	// ---- x layout
		FLOAT x1 = left;
		FLOAT x2 = x1 + xinc; 
		FLOAT x3 = x2 + xsp;
		FLOAT x4 = x3 + xinc;
		FLOAT x5 = x4 + xsp;
		FLOAT x6 = right;


	// ---- y layout
		FLOAT y1 = top;
		FLOAT y2 = y1 + yinc;
		FLOAT y3 = y2 + ysp;
		FLOAT y4 = y3 + yinc;
		FLOAT y5 = y4 + ysp;
		FLOAT y6 = bottom;


	// ---- "CONFIG IMG PTZ" ---- TBD

	// ---- vs_system::ObjConfig::MAX_PANO_PTF_LUT
		VsFloatRectangle p[9] =
		{

			{ x1, y1, x2, y2 }, //  2 - first group
			{ x3, y1, x4, y2 }, //  3
			{ x5, y1, x6, y2 }, //  4

			{ x1, y3, x2, y4 }, //  5 - second group
			{ x3, y3, x4, y4 }, //  6
			{ x5, y3, x6, y4 }, //  7

			{ x1, y5, x2, y6 }, //  8 - third group
			{ x3, y5, x4, y6 }, //  9
			{ x5, y5, x6, y6 }, // 10

		};


	// ---- copy
		for (INT i = 0; i < 9; i++) *(pp->nine+i) = p[i];


	// ---- set ptf aspect ratio ( parametric ratio * actual frame ratio )
		vs_system::ObjConfig *obj_config = GetObjConfig();
		obj_config->SetImagePanelAspectRatio( (x2 - x1) / (y2 - y1) * pp->frame_aspect );

	return VS_OK;
}


// ---------- InitialiseNinePanelPanoRects ----------
/*!
\brief Initialise N # 1 panel VsFloatRectangle
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Model::InitialiseNinePanelPanoRects(PanelProps *pp)
{

	// ---- legwork...
		FLOAT xi = pp->t3d_big_height * pp->image_aspect;
		FLOAT xi_by2 = xi / 2;
		xi /= 3;

		FLOAT xl = 0.5f - xi_by2;
		FLOAT xh = 0.5f + xi_by2;
		FLOAT yi = pp->t3d_big_height / 2;
		FLOAT yl = pp->y_prop_content_top;
		FLOAT yh = pp->y_prop_content_top + yi * 2;


	// ---- x layout
		FLOAT x1 = xl;
		FLOAT x2 = xl + xi - pp->x_prop_space / 2;
		FLOAT x3 = xl + xi + pp->x_prop_space / 2;
		FLOAT x4 = xh - xi - pp->x_prop_space / 2;
		FLOAT x5 = xh - xi + pp->x_prop_space / 2;
		FLOAT x6 = xh;


	// ---- y layout
		FLOAT y1 = yl;
		FLOAT y2 = yl + yi - pp->y_prop_space / 2;
		FLOAT y3 = yh - yi + pp->y_prop_space / 2;
		FLOAT y4 = yh;
		FLOAT y5 = 0;
		FLOAT y6 = 0;


	// ---- 6 x 3x3
		VsFloatRectangle n[6] =
		{
			{ x1, y1, x2, y2 },
			{ x3, y1, x4, y2 },
			{ x5, y1, x6, y2 },
			{ x1, y3, x2, y4 },
			{ x3, y3, x4, y4 },
			{ x5, y3, x6, y4 },
		};
		for (INT i = 0; i < 6; i++) *(pp->nine +i) = n[i];

	return VS_OK;
}


// ---------- InitialiseImg1PanelRects ----------
/*!
\brief Initialise N # 1 panel VsFloatRectangle
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Model::InitialiseImg1PanelRects(PanelProps *pp)
{

	// ---- housekeeping

		FLOAT top    = pp->y_prop_content_top;
		FLOAT bottom = pp->y_prop_content_bottom;


	// ---- portrait navigation offset beneath frames
		FLOAT portrait_y_offset = pp->frame_portrait ? pp->frames.bottom - pp->frames.top : 0;

		FLOAT left   = pp->x_prop_left;
		FLOAT right  = pp->x_prop_right;
		FLOAT xsp    = pp->x_prop_space;
		FLOAT ysp    = pp->y_prop_space;
		FLOAT xnav   = 0.02f;

		FLOAT xinc   = ( (right - left) - xsp * 5 - xnav * 2 ) / 4.15f;
		FLOAT yinc   = ( (bottom - top) - ysp * 4 ) / 4;

		if ( pp->frame_portrait )
		{
			xinc = ((right - left) - xsp * 4 - xnav * 2) / 3.0f;
			yinc = ((bottom - top) - ysp * 4) / 4;
		}


		// ---- x layout
		FLOAT x0 = left;
		FLOAT x1 = x0 + xnav;  // nav
		FLOAT x2 = x1 + xsp;
		FLOAT x3 = x2 + xinc;  // img
		FLOAT x4 = x3 + xsp;
		FLOAT x5 = x4 + xinc;  // img
		FLOAT x6 = x5 + xsp;
		FLOAT x7 = x6 + xinc;  // img
		FLOAT x8 = x7 + xsp;
		FLOAT x9 = x8 + xnav;  // nav
		FLOAT xA = x9 + xsp;
		FLOAT xB = pp->x_prop_right; // was xA + xinc;  // tint


	// ---- y layout
		FLOAT y1 = top;
		FLOAT y2 = y1 + yinc;
		FLOAT y3 = y2 + ysp;
		FLOAT y4 = y3 + yinc;
		FLOAT y5 = y4 + ysp; 
		FLOAT y6 = y5 + yinc;
		FLOAT y7 = y6 + ysp * 2;
		FLOAT y8 = bottom;


	// ---- img tab layout
		FLOAT img_left   = xA;
		FLOAT img_top    = y1;
		FLOAT img_right  = xB;
		FLOAT img_bottom = y8;
		if ( pp->frame_portrait )
		{
			img_left   = pp->x_prop_space;
			img_top    = pp->frames.bottom + pp->y_prop_space * 3;
			img_right  = pp->x_prop_right;
			img_bottom = img_top + 0.5f;
		}


	// ---- img head 
		pp->imghead.left   = img_left;    // xA;
		pp->imghead.top    = img_top;     // y1;
		pp->imghead.right  = img_right;   // xB;
		pp->imghead.bottom = img_bottom;  // y8;


	// ---- img body
		pp->imgbody.left   = img_left;    // xA;
		pp->imgbody.top    = img_top;     // y1;
		pp->imgbody.right  = img_right;   // xB;
		pp->imgbody.bottom = img_bottom;  // y8;


	// ---- img nav
		pp->imgnav.left    = x0;
		pp->imgnav.top     = y7 + portrait_y_offset;
		pp->imgnav.right   = x9;
		pp->imgnav.bottom  = y8 + portrait_y_offset;


	// ---- img ? --- then align cam tint...
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
			pp->camhead = pp->imghead;
			pp->cambody = pp->imgbody;
		#endif


	// ---- example height proportional to width
		// FLOAT y = (x6 - x1) * pp->frame_aspect * 1 / pp->image_aspect;
		// FLOAT y9 = y1 + y;


	// ---- "CONFIG IMG PTZ" ---- TBD


	// ---- vs_system::ObjConfig::MAX_PANO_PTF_LUT
		VsFloatRectangle p[vs_system::ObjConfig::MAX_PANO_PTF_LUT] =
		{
			{ x0, y1, x9, y6 }, //  1 - overview

			{ x2, y7, x3, y8 }, //  2 - first nav group
			{ x4, y7, x5, y8 }, //  3
			{ x6, y7, x7, y8 }, //  4

			{ x2, y7, x3, y8 }, //  5 - second nav group
			{ x4, y7, x5, y8 }, //  6
			{ x6, y7, x7, y8 }, //  7

			{ x2, y7, x3, y8 }, //  8 - third nav group
			{ x4, y7, x5, y8 }, //  9
			{ x6, y7, x7, y8 }, // 10

			{ x0, y1, x9, y6 }, // 11 - solo { x0, y1, xB, y8 },
			
			{ x0, y1, x9, y6 }, // 12 - settings { x0, y1, x9, y6 },


			{ x0, y1, xB, y8 }, // 13 - streaming hidden - not displayed
			{ x0, y1, xB, y8 }, // 14 - streaming visible - displayed

			{ x0, y1, xB, y8 }, // 15 - not used
			{ x0, y1, xB, y8 }, // 16 - not used

		};


	// ---- portrait
		if ( pp->frame_portrait )
		{

			// solo height as per overview
			p[10].bottom = y6;

			// frames under solo
			pp->frames.top = y6 + ysp;
			pp->frames.bottom = pp->frames.top + portrait_y_offset;

			// navigation under frames
			for ( UINT i = 1; i < 10; i++)
			{
				p[i].top += portrait_y_offset;
				p[i].bottom += portrait_y_offset;
			}

		}


	// ---- for procedural - copy p VsFloatRectangle into something else...


	// ---- copy
		for (INT i = 0; i < vs_system::ObjConfig::MAX_PANO_PTF_LUT; i++) *(pp->ptf_img +i) = p[i];


	// ---- set ptf aspect ratio ( parametric ratio * actual frame ratio )
		vs_system::ObjConfig *obj_config = GetObjConfig();
		obj_config->SetImagePanelAspectRatio( (x3 - x2) / (y8 - y7) * pp->frame_aspect );


	return VS_OK;
}


// ---------- InitialiseSixteenPanelRects ----------
/*!
\brief Initialise H # 1 panel VsFloatRectangle
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Model::InitialiseSixteenPanelRects(PanelProps *pp)
{

	// ---- legwork...
		FLOAT xi = (pp->x_prop_panel_asp_width - pp->x_prop_space * 3) / 4;
		FLOAT xl = pp->x_prop_panel_asp_left;
		FLOAT xh = pp->x_prop_panel_asp_right;
		FLOAT yi = (pp->y_prop_content_bottom - pp->y_prop_content_top - pp->y_prop_space * 3) / 4;
		FLOAT yl = pp->y_prop_content_top;
		FLOAT yh = pp->y_prop_content_bottom;


	// ---- x layout
		FLOAT x1 = xl;
		FLOAT x2 = xl + (xi * 1) - pp->x_prop_space / 2;
		FLOAT x3 = xl + (xi * 1) + pp->x_prop_space / 2;
		FLOAT x4 = xl + (xi * 2) - pp->x_prop_space / 2;
		FLOAT x5 = xl + (xi * 2) + pp->x_prop_space / 2;
		FLOAT x6 = xl + (xi * 3) - pp->x_prop_space / 2;
		FLOAT x7 = xl + (xi * 3) + pp->x_prop_space / 2;
		FLOAT x8 = xh;


	// ---- y layout
		FLOAT y1 = yl;
		FLOAT y2 = yl + (yi * 1) - pp->y_prop_space / 2;
		FLOAT y3 = yl + (yi * 1) + pp->y_prop_space / 2;
		FLOAT y4 = yl + (yi * 2) - pp->y_prop_space / 2;
		FLOAT y5 = yl + (yi * 2) + pp->y_prop_space / 2;
		FLOAT y6 = yl + (yi * 3) - pp->y_prop_space / 2;
		FLOAT y7 = yl + (yi * 3) + pp->y_prop_space / 2;
		FLOAT y8 = yh;


	// ---- 16 x 4x4
		VsFloatRectangle h[16] =
		{
			{ x1, y1, x2, y2 },
			{ x3, y1, x4, y2 },
			{ x5, y1, x6, y2 },
			{ x7, y1, x8, y2 },
			{ x1, y3, x2, y4 },
			{ x3, y3, x4, y4 },
			{ x5, y3, x6, y4 },
			{ x7, y3, x8, y4 },
			{ x1, y5, x2, y6 },
			{ x3, y5, x4, y6 },
			{ x5, y5, x6, y6 },
			{ x7, y5, x8, y6 },
			{ x1, y7, x2, y8 },
			{ x3, y7, x4, y8 },
			{ x5, y7, x6, y8 },
			{ x7, y7, x8, y8 },
		};
		for (INT i = 0; i < 16; i++) *(pp->h1+i) = h[i];

	return VS_OK;
}