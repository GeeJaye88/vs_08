////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_cam_callback_aec.hpp ----------
/*!
\file r2_tint_cam_callback_aec.hpp
\brief
\author Gareth Edwards
*/


using namespace review_02;


// -------- Functionality and Parameters for Tabbed Interface A(rea of) E(xposure) C(ontrol)

class Model::Tint_Aec_Param
{

	public:

	// ---- cdtor
		Tint_Aec_Param::Tint_Aec_Param(){;};
		Tint_Aec_Param::~Tint_Aec_Param(){;};


	// --- props
		BOOL _large_incr = TRUE;


	// --- indices
		enum
		{

			#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

			ALL         = 0,
			RECT        = 1,
			
			COPY        = 2,
			DISPLAY     = 3,

			AUTO_PTZ    = 4,

			WIDTH_LESS  = 5,
			WIDTH_MORE  = 6,

			HEIGHT_LESS = 7,
			HEIGHT_MORE = 8,

			LEFT_LESS   = 9,
			LEFT_MORE   = 10,

			TOP_LESS    = 11,
			TOP_MORE    = 12,

			PTZ         = 13,

			SAVE        = 22,
			RESET       = 23,
			TOGGLEINC   = 24

			#else

			INNER       = 0,
			INNER_LESS  = 1 + INNER,
			INNER_RESET = 2 + INNER,
			INNER_MORE  = 3 + INNER,
			OUTER       = 4, 
			OUTER_LESS  = 1 + OUTER,
			OUTER_RESET = 2 + OUTER,
			OUTER_MORE  = 3 + OUTER,
			ZONE        = 8,
			ALL         = 1 + ZONE,
			ANNULAR     = 2 + ZONE,
			TOP         = 3 + ZONE,
			LEFT        = 4 + ZONE,
			BOTTOM      = 5 + ZONE,
			RIGHT       = 6 + ZONE,
			DISPLAY     = 15,
			SAVE        = 16,
			RESET       = 17,
			TOGGLEINC   = 18

			#endif

	};


} tint_aec_param;


// ----------------------------------------------------------------------
INT Model::Tint_Aec_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_Aec_BuildList;
	tint_callback->callback_get_icon_info  = Tint_Aec_GetIconInfo;
	tint_callback->callback_update_model   = Tint_Aec_UpdateModel;
	tint_callback->callback_update_element = Tint_Aec_UpdateElement;
	tint_callback->callback_add_actions    = NULL;
	tint_callback->callback_add_text       = Tint_Aec_AddText;
	tint_callback->callback_add_value      = Tint_Aec_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Aec_BuildList(
		vs_system::TintItem li[]
	)
{

	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

	// ---- local
		FLOAT lincr  = 4;
		INT   result = 0;


	// ---- local for TINT_ macros
		UINT gid = ELEM_GROUP_AEC_LID;
		UINT noi = 0;


	// ---- build
		{

			// ---- scope tint def & enum
				using namespace vs_system;

			// ---- set
				TINT_SET_START_NOI()

				TINT_ADD_ITEM("Zone" ); // (1)
				TINT_SET_TARC(vs_system::Tint::ITEM_RADIO,    1, 3, 0);
				TINT_ADD_ICON(); // (1)
				TINT_SET_TARCX(vs_system::Tint::ITEM_RADIO,   1, 3, 1, 1.1f);
				TINT_ADD_ITEM("Copy");
				TINT_SET_TARCX(vs_system::Tint::ITEM_CLICK,   3, 3, 4, 2.4f);
				TINT_ADD_ITEM("Disp");
				TINT_SET_TARCX(vs_system::Tint::ITEM_SWITCH,  3, 4, 4, 3.7f);
				TINT_ADD_ITEM("Auto");
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_SWITCH, 3, 4, 4, 5.0f, TRUE, 12);

				TINT_ADD_ITEM("Width");
				TINT_SET_TARC(vs_system::Tint::ITEM_CLICK,    3, 5, 0);
				TINT_ADD_ICON();
				TINT_SET_TARCX(vs_system::Tint::ITEM_CLICK,   3, 5, 2, 1.1f);
				TINT_ADD_ITEM("Height");
				TINT_SET_TARCX(vs_system::Tint::ITEM_CLICK,   3, 5, 0, 2.5f);
				TINT_ADD_ICON();
				TINT_SET_TARCXL(vs_system::Tint::ITEM_CLICK,  3, 5, 2, 3.8f, TRUE);

				TINT_ADD_ITEM("Left");
				TINT_SET_TARC(vs_system::Tint::ITEM_CLICK,    3, 5, 0);
				TINT_ADD_ICON();
				TINT_SET_TARCX(vs_system::Tint::ITEM_CLICK,   3, 5, 2, 1.1f);
				TINT_ADD_ITEM("Top");
				TINT_SET_TARCX(vs_system::Tint::ITEM_CLICK,   3, 5, 0, 2.5f);
				TINT_ADD_ICON();
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_CLICK, 3, 5, 2, 3.6f, TRUE, 12);

				TINT_ADD_ITEM("Pan & Tilt");
				TINT_SET_TARC(vs_system::Tint::ITEM_CLICK,    3, 2, 0);
				TINT_ADD_ICON();
				TINT_SET_TARCX(vs_system::Tint::ITEM_CLICK,   3, 2, 1, 1.1f);
				TINT_ADD_ICON();
				TINT_SET_TARCXL(vs_system::Tint::ITEM_CLICK,  3, 2, 2, 2.2f, TRUE);
				TINT_ADD_ICON();
				TINT_SET_TARC(vs_system::Tint::ITEM_CLICK,    3, 3, 0);
				TINT_ADD_ICON();
				TINT_SET_TARCX(vs_system::Tint::ITEM_CLICK,   3, 3, 1, 1.1f);
				TINT_ADD_ICON();
				TINT_SET_TARCXL(vs_system::Tint::ITEM_CLICK,  3, 3, 2, 2.2f, TRUE);
				TINT_ADD_ICON();
				TINT_SET_TARC(vs_system::Tint::ITEM_CLICK,    3, 4, 0);
				TINT_ADD_ICON();
				TINT_SET_TARCX(vs_system::Tint::ITEM_CLICK,   3, 4, 1, 1.1f);
				TINT_ADD_ICON();
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_CLICK, 3, 4, 2, 2.2f, TRUE, 12);

				TINT_ADD_ITEM("Save");
				TINT_SET_TARC(vs_system::Tint::ITEM_CLICK,     3, 3, 3);
				TINT_ADD_ITEM("Reset");
				TINT_SET_TARCX(vs_system::Tint::ITEM_CLICK,    3, 3, 5, 1.5f);
				TINT_ADD_ITEM("Increment");
				TINT_SET_TARCXLI(vs_system::Tint::ITEM_SWITCH, 3, 4, 3, 3.0f, TRUE, 4);

				// ---- get 
				TINT_SET_TOTAL_NOI();

		}


	// ---- set by data

		using namespace vs_system;

		CamProps *cam_props = s_pi_model->cam_props;

		// 0 or 1
		INT zone_index = 0;
		result = cam_props->GetProperty(CamProps::AOI, 0, CamProps::ZONE, &zone_index);
		zone_index = zone_index == ObjConfig::AOI_ALL ? ObjConfig::AOI_ALL : ObjConfig::AOI_ANNULAR;
		li[tint_aec_param.ALL + zone_index].SetStatus(TRUE);

		INT display = 0;
		result = cam_props->GetProperty(CamProps::AOI, 0, CamProps::DISPLAY, &display);
		li[tint_aec_param.DISPLAY].SetStatus(display ? TRUE : FALSE);


	// ---- column LF
		li[Tint_Aec_Param::PTZ].SetColumnLF(TRUE);
		li[Tint_Aec_Param::SAVE].SetColumnLF(TRUE);


	#else
	

	// ---- local
		FLOAT lincr  = 4;
		INT   result = 0;


	// ---- local for TINT_ macros
		UINT gid = ELEM_GROUP_AEC_LID;
		UINT noi = 0;


	// ---- build
		INT zone_index = 0;
		INT display_index = 0;
		{

			// ---- scope tint def & enum
				using namespace vs_system;

			// ---- set
				TINT_SET_START_NOI()

			// ---- radius min
				TINT_ADD_ITEMI("Radius Min", tint_aec_param.INNER_LESS);
				TINT_SET_TARC(Tint::ITEM_CLICK, 3, 5, 0);
				TINT_ADD_ICONI(tint_aec_param.INNER_RESET);
				TINT_SET_TARCX(Tint::ITEM_CLICK, 3, 5, 1, 1.1f);
				TINT_ADD_ICONI(tint_aec_param.INNER_MORE);
				TINT_SET_TARCXLI(Tint::ITEM_CLICK, 3, 5, 2, 2.2f, TRUE, lincr);

			// ---- radius max
				TINT_ADD_ITEMI("Radius Min", tint_aec_param.OUTER_LESS);
				TINT_SET_TARC(Tint::ITEM_CLICK, 3, 5, 0);
				TINT_ADD_ICONI(tint_aec_param.OUTER_RESET);
				TINT_SET_TARCX(Tint::ITEM_CLICK, 3, 5, 1, 1.1f);
				TINT_ADD_ICONI(tint_aec_param.OUTER_MORE);
				TINT_SET_TARCXLI(Tint::ITEM_CLICK, 3, 5, 2, 2.2f, TRUE, lincr);

			// ---- zone (1)
				TINT_ADD_ITEMI("Zone", tint_aec_param.ALL);
				TINT_SET_TARC(Tint::ITEM_RADIO, 1, 4, 0);
				zone_index = noi;
				TINT_ADD_ICONI(tint_aec_param.ANNULAR);
				TINT_SET_TARCXLI(Tint::ITEM_RADIO, 1, 4, 1, 1.1f, TRUE, lincr);
				TINT_ADD_ICONI(tint_aec_param.TOP);
				TINT_SET_TARC(Tint::ITEM_RADIO, 1, 4, 2);
				TINT_ADD_ICONI(tint_aec_param.LEFT);
				TINT_SET_TARCX(Tint::ITEM_RADIO, 1, 4, 3, 1.1f);
				TINT_ADD_ICONI(tint_aec_param.BOTTOM);
				TINT_SET_TARCX(Tint::ITEM_RADIO, 1, 4, 4, 2.2f);
				TINT_ADD_ICONI(tint_aec_param.RIGHT);
				TINT_SET_TARCXLI(Tint::ITEM_RADIO, 1, 4, 5, 3.3f, TRUE, lincr);

			// ---- display (1)
				TINT_ADD_ITEMI("Display", tint_aec_param.DISPLAY);
				TINT_SET_TARCXLI(Tint::ITEM_SWITCH, 1, 4, 6, 0.0f, TRUE, lincr);
				display_index = noi;

			// ---- sri
				TINT_ADD_ITEMI("Save", tint_aec_param.SAVE);
				TINT_SET_TARC(Tint::ITEM_CLICK, 3, 3, 3);
				TINT_ADD_ITEMI("Reset", tint_aec_param.RESET);
				TINT_SET_TARCX(Tint::ITEM_CLICK, 3, 3, 4, 1.5f);
				TINT_ADD_ITEMI("Increment", tint_aec_param.TOGGLEINC);
				TINT_SET_TARCXLI(Tint::ITEM_SWITCH, 3, 4, 3, 3.0f, TRUE, lincr);

			// ---- get 
				TINT_SET_TOTAL_NOI();

		}


	// ---- set by data
		{

			using namespace vs_system;

			CamProps *cam_props = s_pi_model->cam_props;

			// 0 to 6
			INT zone = 0;
			result = cam_props->GetProperty(CamProps::ANNULAR_AOI, 0, CamProps::ZONE, &zone);
			li[zone_index + zone].SetStatus(TRUE);

			INT display;
			result = cam_props->GetProperty(CamProps::ANNULAR_AOI, 0, CamProps::DISPLAY, &display);
			li[display_index].SetStatus((BOOL)display);

		}


	// ---- column LF
		li[Tint_Aec_Param::ALL].SetColumnLF(TRUE);
		li[Tint_Aec_Param::SAVE].SetColumnLF(TRUE);

	#endif

	return noi;
}


// ----------------------------------------------------------------------
INT Model::Tint_Aec_GetIconInfo(INT id)
{

	switch (id)
	{
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
			case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 3;
			case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 7;
			case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
			case vs_system::Tint::ICON_ATLAS_PAGE_ID : return 3;
		#else
			case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 1;
			case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 4;
			case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
			case vs_system::Tint::ICON_ATLAS_PAGE_ID : return 1;
		#endif
		default:
			break;
	}

	return VS_ERROR;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Aec_UpdateModel(
		INT  handle_model,
		INT  element_index
	)
{

	// ---- setup bitmask
		BYTE setup_bitmask = VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING | VS_ELM_SETUP_INDEX;


	// ---- local
		INT   result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::ObjConfig *obj_config = model->GetObjConfig();

	// ---- switch namespace
		using namespace vs_system;


	// ---- cam props
		CamProps *cam_props = NULL;
		result = model->GetCamProps(&cam_props);


	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)


	// ---- get exposure param
		FLOAT exp_display_f = 0;
		FLOAT exp_zone_f    = 0;
		FLOAT exp_height    = 0;
		FLOAT exp_width     = 0;
		FLOAT exp_left      = 0;
		FLOAT exp_top       = 0;
		FLOAT exp_right     = 0;
		FLOAT exp_bottom    = 0;
		result = cam_props->GetProperty(CamProps::AOI, 0, CamProps::DISPLAY, &exp_display_f );
		result = cam_props->GetProperty(CamProps::AOI, 0, CamProps::ZONE,    &exp_zone_f    );
		result = cam_props->GetProperty(CamProps::AOI, 0, CamProps::HEIGHT,  &exp_height    );
		result = cam_props->GetProperty(CamProps::AOI, 0, CamProps::WIDTH,   &exp_width     );
		result = cam_props->GetProperty(CamProps::AOI, 0, CamProps::LEFT,    &exp_left      );
		result = cam_props->GetProperty(CamProps::AOI, 0, CamProps::TOP,     &exp_top       );
		result = cam_props->GetProperty(CamProps::AOI, 0, CamProps::RIGHT,   &exp_right     );
		result = cam_props->GetProperty(CamProps::AOI, 0, CamProps::BOTTOM,  &exp_bottom    );

		UINT exp_display = (UINT)exp_display_f;
		UINT exp_zone    = (UINT)exp_zone_f;


	#else


	// ---- get annular exposure param
		FLOAT ann_display_f = 0;
		FLOAT ann_zone_f   = 0;
		FLOAT ann_min_aoi  = 0;
		FLOAT ann_max_aoi  = 0;
		result = cam_props->GetProperty(CamProps::ANNULAR_AOI, 0, CamProps::DISPLAY, &ann_display_f);
		result = cam_props->GetProperty(CamProps::ANNULAR_AOI, 0, CamProps::ZONE,    &ann_zone_f);
		result = cam_props->GetProperty(CamProps::ANNULAR_AOI, 0, CamProps::MIN,     &ann_min_aoi);
		result = cam_props->GetProperty(CamProps::ANNULAR_AOI, 0, CamProps::MAX,     &ann_max_aoi);

		UINT ann_display = (UINT)ann_display_f;
		UINT ann_zone = (UINT)ann_zone_f;


	#endif


	// ---- switch namespace
		using namespace review_02;


	// ---- assume that...
		BOOL exposure_must_be_updated = TRUE;


	// ---- and that...
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
		FLOAT increment = tint_aec_param._large_incr ? 0.05f : 0.005f;
		#else
		FLOAT increment = tint_aec_param._large_incr ? 0.01f : 0.001f;
		#endif


	// ---- update

		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)


	// ---- minimax dimensions
		FLOAT min_dim = 0.01f;
		FLOAT max_dim = 0.99f;


	// ---- provide block switch to PTZ
		INT tmp_element_index = element_index;
		if (
			element_index >= tint_aec_param.PTZ &&
			element_index <  tint_aec_param.SAVE
			)
		{
			tmp_element_index = tint_aec_param.PTZ;
		}


	// ---- doit
		switch ( tmp_element_index )
		{
			
			case tint_aec_param.ALL:   // vs_system::ObjConfig::AOI_ALL      ( 0 )
			case tint_aec_param.RECT:  // vs_system::ObjConfig::AOI_ANNULAR  ( 1 )
				{
					exp_zone = (UINT)( element_index - tint_aec_param.ALL );
				}
				break;

			case tint_aec_param.COPY:
				{

					// ---- get previous id (cos we are in settings!)
						UINT  id = 0;
						result = obj_config->GetImagePanelActiveId(&id);


						/* was:
							result = obj_config->GetImagePanelPreviousId(&id);
						*/


						/*
							CHAR msg[128];
							sprintf(msg, " ---- %d\n", (INT)id);
							OutputDebugString(msg);
						*/


					// -- get previous PTZ
						FLOAT pan, tilt, zoom;
						result = obj_config->GetPanoPTFById(id, &pan, &tilt, &zoom);


					// ---- calculate parametric parameters
						FLOAT z = 1 + (zoom - 1) / 10;
						z = 1 / z;
						FLOAT w    = z;
						FLOAT wby2 = w / 2;
						FLOAT h    = z;
						FLOAT hby2 = h / 2;
						FLOAT xc   = 0.5f;
						FLOAT yc   = 0.5f;


					// ---- calculate parametric rect
						exp_left   =  pan + xc - wby2;
						exp_right  =  exp_left + w;
						exp_bottom = -tilt + yc - hby2;
						exp_top    =  exp_bottom + h;


					// ---- & dimensions
						exp_width  = exp_right - exp_left;
						exp_height = exp_top - exp_bottom;


					// ---- & flip reference
						exp_top = exp_bottom;

				}
				break;

			case tint_aec_param.DISPLAY:
				{
					exp_display = exp_display ? 0 : 1;

					vs_system::ElmContext *elm_context;
					result =  model->pi_model->gfx->GetElementContext(
							model->pi_model->handle_elem_ir_img_rect_view[
								vs_system::ObjConfig::IMG_PTZ_SETTINGS_PANEL_INDEX],
									&elm_context
							);

					if ( exp_display )
					{
						elm_context->SetDisplay(TRUE);
					}
					else
					{
						elm_context->SetDisplay(FALSE);
					}
				}
				break;

			case tint_aec_param.AUTO_PTZ:
				{
					OutputDebugString("tint_aec_param.AUTO_PTZ:\n");
				}
				break;

			case tint_aec_param.WIDTH_LESS:
				{
					exp_width = exp_width - increment;
					exp_width = exp_width < min_dim ? min_dim : exp_width;
				}
				break;

			case tint_aec_param.WIDTH_MORE:
				{
					exp_width = exp_width + increment;
					exp_width = exp_width > max_dim ? max_dim : exp_width;
				}
				break;

			case tint_aec_param.HEIGHT_LESS:
				{
					exp_height = exp_height - increment;
					exp_height = exp_height < min_dim ? min_dim : exp_height;
				}
				break;

			case tint_aec_param.HEIGHT_MORE:
				{
					exp_height = exp_height + increment;
					exp_height = exp_height > max_dim ? max_dim : exp_height;
				}
				break;

			case tint_aec_param.LEFT_LESS:
				{
					exp_left = exp_left - increment;
				}
				break;

			case tint_aec_param.LEFT_MORE:
				{
					exp_left = exp_left + increment;
				}
				break;

			case tint_aec_param.TOP_LESS:
				{
					exp_top = exp_top - increment;
				}
				break;

			case tint_aec_param.TOP_MORE:
				{
					exp_top = exp_top + increment;
				}
				break;

			case tint_aec_param.PTZ:
				{
					auto pan_and_tilt = [&](FLOAT pan, FLOAT tilt)
					{
						exp_left += pan;
						exp_left  = exp_left < 0 ? 0 : exp_left;
						exp_left  = exp_left > max_dim ? max_dim : exp_left;

						exp_top  += tilt;
						exp_top   = exp_top < 0 ? 0 : exp_top;
						exp_top   = exp_top > max_dim ? max_dim : exp_top;
					};

					switch (element_index)
					{
						case tint_aec_param.PTZ  : pan_and_tilt(-increment,-increment); break;
						case tint_aec_param.PTZ+1: pan_and_tilt(         0,-increment); break;
						case tint_aec_param.PTZ+2: pan_and_tilt( increment,-increment); break;
						case tint_aec_param.PTZ+3: pan_and_tilt(-increment,         0); break;
						case tint_aec_param.PTZ+4: pan_and_tilt(         0,         0); break;
						case tint_aec_param.PTZ+5: pan_and_tilt( increment,         0); break;
						case tint_aec_param.PTZ+6: pan_and_tilt(-increment, increment); break;
						case tint_aec_param.PTZ+7: pan_and_tilt(         0, increment); break;
						case tint_aec_param.PTZ+8: pan_and_tilt( increment, increment); break;
					}

					float fred = 2;
				}
				break;

			case tint_aec_param.SAVE:
				{
					#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
					result = model->SaveCamProps();
					#else
					result = model->SaveCamProps();
					result = model->SaveObjectAndCameraConfig();  // BUG - in method Hoff TBD
					#endif
					result = obj_config->SetPanoUpdated(FALSE);
				}
				break;

			case tint_aec_param.RESET:
				{

					// ---- reset aec param
						exp_display = 1;
						exp_zone    = 0;
						exp_height  = 1.0f;
						exp_width   = 1.0f;
						exp_left    = 0.0f;
						exp_top     = 0.0f;
						exp_right   = 1.0f;
						exp_bottom  = 1.0f;


					// ---- reset display
						vs_system::ElmContext *elm_context;
						result = model->pi_model->gfx->GetElementContext(
								model->pi_model->handle_elem_ir_img_rect_view[
									vs_system::ObjConfig::IMG_PTZ_SETTINGS_PANEL_INDEX],
										&elm_context
								);
						elm_context->SetDisplay(TRUE);


					// ---- update settings panel
						UINT id_settings = vs_system::ObjConfig::IMG_PTZ_SETTINGS_PANEL_INDEX + 1;
						result = obj_config->SetPanoPTFById(id_settings, 0, 0, 1);
						result = model->Update_ImgPanel_Config(id_settings);

				}
				break;

			case tint_aec_param.TOGGLEINC:
				{
					tint_aec_param._large_incr =
						tint_aec_param._large_incr ? FALSE : TRUE;
				}
				break;

			default:
				{
					setup_bitmask = VS_ELM_SETUP_NONE;
					exposure_must_be_updated = FALSE;
				}
				break;
		}


	// ---- limit
		{
			exp_left   = exp_left < 0 ? 0 : exp_left > max_dim ? max_dim : exp_left;
			exp_top    = exp_top  < 0 ? 0 : exp_left > max_dim ? max_dim : exp_top;

			exp_right = exp_left + exp_width;
			if ( exp_right > 1 )
			{
				exp_right = 1;
				exp_width = 1 - exp_left;
			}

			exp_bottom = exp_top + exp_height;
			if ( exp_bottom > 1 )
			{
				exp_bottom = 1;
				exp_height = 1 - exp_top;
			}

			if ( exp_left + exp_width > 1 )
			{
				exp_width = 1 - exp_left;
			}

			if ( exp_top + exp_height > 1 )
			{
				exp_height = 1 - exp_top;
			}
		}

		CHAR msg[256];
		sprintf(msg, " --- L:%6.4f   R:%6.4f   W:%6.4f\n", exp_left, exp_right, exp_width);
		OutputDebugString(msg);
		sprintf(msg, "     T:%6.4f   B:%6.4f   H:%6.4f\n", exp_top, exp_bottom, exp_height);
		OutputDebugString(msg);


	// ---- switch namespace
		using namespace vs_system;


	// ---- set cam props aoi
		result = cam_props->SetProperty(CamProps::AOI, 0, CamProps::DISPLAY, (FLOAT)exp_display);
		result = cam_props->SetProperty(CamProps::AOI, 0, CamProps::ZONE,    (FLOAT)exp_zone   );
		result = cam_props->SetProperty(CamProps::AOI, 0, CamProps::HEIGHT,  exp_height        );
		result = cam_props->SetProperty(CamProps::AOI, 0, CamProps::WIDTH,   exp_width         );
		result = cam_props->SetProperty(CamProps::AOI, 0, CamProps::LEFT,    exp_left          );
		result = cam_props->SetProperty(CamProps::AOI, 0, CamProps::TOP,     exp_top           );
		result = cam_props->SetProperty(CamProps::AOI, 0, CamProps::RIGHT,   exp_right         );
		result = cam_props->SetProperty(CamProps::AOI, 0, CamProps::BOTTOM,  exp_bottom        );


	// ---- set model pano aoi
		result = obj_config->SetPanoExposureAoiZone(exp_zone);
		result = obj_config->SetPanoUpdated(exposure_must_be_updated);


		#else


		switch (element_index)
		{
			case tint_aec_param.INNER_LESS:
				{
					ann_min_aoi = ann_min_aoi - increment;
				}
				break;
			case tint_aec_param.INNER_RESET:
				{
					result = obj_config->GetPanoRadiusMin(&ann_min_aoi);
				}
				break;
			case tint_aec_param.INNER_MORE:
				{
					ann_min_aoi = ann_min_aoi + increment;
				}
				break;

			case tint_aec_param.OUTER_LESS:
				{
					ann_max_aoi = ann_max_aoi - increment;
				}
				break;
			case tint_aec_param.OUTER_RESET:
				{
					result = obj_config->GetPanoRadiusMax(&ann_max_aoi);
				}
				break;
			case tint_aec_param.OUTER_MORE:
				{
					ann_max_aoi = ann_max_aoi + increment;
				}
				break;


			case tint_aec_param.ALL:     // vs_system::ObjConfig::AOI_ALL      ( 0 )
			case tint_aec_param.ANNULAR: // vs_system::ObjConfig::AOI_ANNULAR  ( 1 )
			case tint_aec_param.TOP:     // vs_system::ObjConfig::AOI_TOP      ( 2 )
			case tint_aec_param.LEFT:    // vs_system::ObjConfig::AOI_LEFT     ( 3 )
			case tint_aec_param.BOTTOM:  // vs_system::ObjConfig::AOI_BOTTOM   ( 4 )
			case tint_aec_param.RIGHT:   // vs_system::ObjConfig::AOI_RIGHT    ( 5 )
				{
					setup_bitmask = setup_bitmask | VS_ELM_SETUP_INDEX | VS_ELM_SETUP_CONFIG;
					ann_zone = (UINT)(element_index - tint_aec_param.ALL);
				}
				break;


			case tint_aec_param.DISPLAY:
				{
					ann_display = ann_display ? 0 : 1;

					vs_system::PanelContext *panel_context = (vs_system::PanelContext*)
						model->pi_model->handle_pan_gfx_con_pano_annu_exp_aoi;

					if (ann_display)
					{
						panel_context->SetDisplayStatus(TRUE);
					}
					else
					{
						panel_context->SetDisplayStatus(FALSE);
					}
				}
				break;

			case tint_aec_param.SAVE:
				{
					//result = model->SaveObjectAndCameraConfig();
					result = model->SaveCamProps();
					result = obj_config->SetPanoUpdated(FALSE);
				}
				break;

			case tint_aec_param.RESET:
				{
					ann_display = 1;
					ann_min_aoi = 0.65f;
					ann_max_aoi = 0.92f;
					ann_zone    = 0;

					vs_system::PanelContext *gfx_exp = (vs_system::PanelContext*)
						model->pi_model->handle_pan_gfx_con_pano_annu_exp_aoi;
					gfx_exp->SetDisplayStatus(TRUE);

				}
				break;

			case tint_aec_param.TOGGLEINC:
				{
					tint_aec_param._large_incr =
						tint_aec_param._large_incr ? FALSE : TRUE;
				}
				break;

			default:
				{
					setup_bitmask = VS_ELM_SETUP_NONE;
					exposure_must_be_updated = FALSE;
				}
				break;
		}


	// ---- limit
		ann_max_aoi = ann_max_aoi > 1 ? 1 : ( ann_max_aoi < 0 ? 0 : ann_max_aoi );
		ann_min_aoi = ann_min_aoi > ann_max_aoi ? ann_max_aoi : ( ann_min_aoi < 0 ? 0 : ann_min_aoi );


	// ---- switch namespace
		using namespace vs_system;


	// ---- set cam props aoi
		result = cam_props->SetProperty(CamProps::ANNULAR_AOI, 0, CamProps::DISPLAY, (FLOAT)ann_display);
		result = cam_props->SetProperty(CamProps::ANNULAR_AOI, 0, CamProps::ZONE,    (FLOAT)ann_zone);
		result = cam_props->SetProperty(CamProps::ANNULAR_AOI, 0, CamProps::MIN,     ann_min_aoi);
		result = cam_props->SetProperty(CamProps::ANNULAR_AOI, 0, CamProps::MAX,     ann_max_aoi);


	// ---- set model pano aoi
		result = obj_config->SetPanoExposureAoiMin(ann_min_aoi);
		result = obj_config->SetPanoExposureAoiMax(ann_max_aoi);
		result = obj_config->SetPanoExposureAoiZone(ann_zone);
		result = obj_config->SetPanoUpdated(exposure_must_be_updated);

		#endif


	// ---- update geometry
		if ( setup_bitmask != VS_ELM_SETUP_NONE )
		{

			#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

				result = model->gfx->SetGlobalConfigureBitmask(
						setup_bitmask,
						"IRV12"
					);

			#else

				vs_system::AppDependency *app_dependency = model->pi_model->app_dependency;
				vs_system::GfxInterface  *gfx_interface = app_dependency->GetGfxInterface();
				result = gfx_interface->SetGlobalConfigureBitmask(
						setup_bitmask,
						"Annular"
					);
				result = gfx_interface->SetGlobalConfigureBitmask(
						setup_bitmask,
						"Exposure"
					);
				result = gfx_interface->SetGlobalConfigureBitmask(
						setup_bitmask,
						"2D"
					);

			#endif

		}


	// ---- note:
	//      if ( exposure_must_be_updated )
	//         AOI exposure updated 

	return (INT)exposure_must_be_updated;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Aec_UpdateElement(
		INT handle_model,
		INT element_index
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_cam_setng;


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = tint->SetSelectedBody(TINT_HEAD_AEC_LID);


	// ---- select elem
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

		switch ( element_index )
		{

			case tint_aec_param.WIDTH_LESS:
			case tint_aec_param.WIDTH_MORE:
			case tint_aec_param.LEFT_LESS:
			case tint_aec_param.LEFT_MORE:
				{
					result = tint->UpdateText( NULL, tint_aec_param.WIDTH_LESS);
					result = tint->UpdateText(NULL, tint_aec_param.LEFT_LESS);
				}
				break;

			case tint_aec_param.HEIGHT_LESS:
			case tint_aec_param.HEIGHT_MORE:
			case tint_aec_param.TOP_LESS:
			case tint_aec_param.TOP_MORE:
				{
					result = tint->UpdateText( NULL, tint_aec_param.HEIGHT_LESS );
					result = tint->UpdateText( NULL, tint_aec_param.TOP_LESS);
				}
				break;

			case tint_aec_param.COPY:
				{
					result = tint->UpdateText(NULL, tint_aec_param.WIDTH_LESS);
					result = tint->UpdateText(NULL, tint_aec_param.HEIGHT_LESS);
					result = tint->UpdateText(NULL, tint_aec_param.LEFT_LESS);
					result = tint->UpdateText(NULL, tint_aec_param.TOP_LESS);
				}
				break;

			case tint_aec_param.RESET:
				{
					result = tint->UpdateRadio(tint_aec_param.ALL);
					result = tint->UpdateText(NULL, tint_aec_param.WIDTH_LESS);
					result = tint->UpdateText(NULL, tint_aec_param.HEIGHT_LESS);
					result = tint->UpdateText(NULL, tint_aec_param.LEFT_LESS);
					result = tint->UpdateText(NULL, tint_aec_param.TOP_LESS);
					result = tint->UpdateStatus(tint_aec_param.DISPLAY, TRUE);
				}
				break;

			default:
				break;
		}

		#else

		switch ( element_index )
		{

			case tint_aec_param.INNER_LESS:
			case tint_aec_param.INNER_RESET:
			case tint_aec_param.INNER_MORE:
				{
					result = tint->UpdateText(
							NULL,
							tint_aec_param.INNER_LESS
						);
				}
				break;

			case tint_aec_param.OUTER_LESS:
			case tint_aec_param.OUTER_RESET:
			case tint_aec_param.OUTER_MORE:
				{
					result = tint->UpdateText(
							NULL,
							tint_aec_param.OUTER_LESS
						);
				}
				break;

			case tint_aec_param.ALL:
			case tint_aec_param.TOP:
			case tint_aec_param.LEFT:
			case tint_aec_param.BOTTOM:
			case tint_aec_param.RIGHT:
			case tint_aec_param.ANNULAR:
			case tint_aec_param.DISPLAY:
				break;

			case tint_aec_param.RESET:
				{
					result = tint->UpdateRadio(tint_aec_param.ALL);
					result = tint->UpdateText(NULL, tint_aec_param.INNER_LESS);
					result = tint->UpdateText(NULL, tint_aec_param.OUTER_LESS);
					result = tint->UpdateStatus(tint_aec_param.DISPLAY, TRUE);
				}
				break;

			default:
				break;
		}
	
		#endif

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Aec_AddText(
		const INT  handle_obj_config,
		const INT  element_index,
		BOOL      *replace_text,
		BOOL      *add_text,
		CHAR      *text
	)
{
	*add_text     = FALSE;
	*replace_text = FALSE;
	*text         = NULL;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Aec_AddValue(
		const INT  handle_ass_obj,
		const INT  element_index,
		BOOL      *add_value,
		FLOAT     *value,
		CHAR      *suffix
	)
{

	// ---- local
		INT result = VS_OK;
		FLOAT p = 0, q = 0;
		INT   i = 0;


	// ---- init
		*add_value = TRUE;
		*value = 0;
		sprintf(suffix, "3");


	// ---- cast to get camera properties object
		vs_system::CamProps *cam_props = (vs_system::CamProps *)handle_ass_obj;


	// ---- change namespace
		using namespace vs_system;


	// ---- set
	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

		switch ( element_index )
		{

			case tint_aec_param.WIDTH_LESS:
				{
					FLOAT exp_max_aoi = 0;
					result = cam_props->GetProperty(CamProps::AOI, 0, CamProps::WIDTH, &exp_max_aoi);
					*value = exp_max_aoi;
				}
				break;

			case tint_aec_param.HEIGHT_LESS:
				{
					FLOAT exp_min_aoi = 0;
					result = cam_props->GetProperty(CamProps::AOI, 0, CamProps::HEIGHT, &exp_min_aoi);
					*value = exp_min_aoi;
				}
				break;

			case tint_aec_param.LEFT_LESS:
				{
					FLOAT exp_min_aoi = 0;
					result = cam_props->GetProperty(CamProps::AOI, 0, CamProps::LEFT, &exp_min_aoi);
					*value = exp_min_aoi;
				}
				break;

			case tint_aec_param.TOP_LESS:
				{
					FLOAT exp_max_aoi = 0;
					result = cam_props->GetProperty(CamProps::AOI, 0, CamProps::TOP, &exp_max_aoi);
					*value = exp_max_aoi;
				}
				break;

			default:
				*add_value = FALSE;
				break;
		}

	#else

		switch ( element_index )
		{
			case tint_aec_param.INNER_LESS:
				{
					FLOAT ann_min_aoi = 0;
					result = cam_props->GetProperty(CamProps::ANNULAR_AOI, 0, CamProps::MIN, &ann_min_aoi);
					*value = ann_min_aoi;
				}
				break;

			case tint_aec_param.OUTER_LESS:
				{
					FLOAT ann_max_aoi = 0;
					result = cam_props->GetProperty(CamProps::ANNULAR_AOI, 0, CamProps::MAX, &ann_max_aoi);
					*value = ann_max_aoi;
				}
				break;

			default:
				*add_value = FALSE;
				break;
		}
	
	#endif

	return VS_OK;
}
