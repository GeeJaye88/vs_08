////////////////////////////////////////////////////////////////////////////////

// ---------- r2_tint_cam_callback_focus.hpp ----------
/*!
\file r2_tint_cam_callback_focus.hpp
\brief
\author Gareth Edwards
*/


using namespace review_02;


// -------- Functionality and Parameters for Tabbed Interface Focus

class Model::Tint_Focus_Param
{

	public:

	// ---- cdtor
		Tint_Focus_Param::Tint_Focus_Param(){;};
		Tint_Focus_Param::~Tint_Focus_Param(){;};


	// --- props
		BOOL _large_incr = TRUE;


	// --- indices
		enum
		{
			LINE_PAIR    = 1,
			INNER        = 2,
			INNER_LESS   = 1 + INNER,
			INNER_MORE   = 2 + INNER,
			OUTER        = 5, 
			OUTER_LESS   = 1 + OUTER,
			OUTER_MORE   = 2 + OUTER,
			ROTATE       = 8,
			ROTATE_LESS  = 1 + ROTATE,
			ROTATE_MORE  = 2 + ROTATE,
			SECTOR       = 11,
			SECTOR_LESS  = 1 + SECTOR,
			SECTOR_MORE  = 2 + SECTOR,
			DISPLAY      = 14,
			RESET        = 15,
			TOGGLEINC    = 16,
			DISPLAY_ON   = 18,
			DISPLAY_OFF  = 19
		};


} tint_focus_param;


// ----------------------------------------------------------------------
INT Model::Tint_Focus_GetCallbacks(vs_system::TintCallback *tint_callback)
{
	tint_callback->callback_build_list     = Tint_Focus_BuildList;
	tint_callback->callback_get_icon_info  = Tint_Focus_GetIconInfo;
	tint_callback->callback_update_model   = Tint_Focus_UpdateModel;
	tint_callback->callback_update_element = Tint_Focus_UpdateElement;
	tint_callback->callback_add_actions    = NULL;
	tint_callback->callback_add_text       = Tint_Focus_AddText;
	tint_callback->callback_add_value      = Tint_Focus_AddValue;
	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Focus_BuildList(
		vs_system::TintItem li[]
	)
{

	// ---- local
		INT number_of_items = 12;
		UINT gid = ELEM_GROUP_FOCUS_LID;
		using namespace vs_system;


	// ---- describe
		INT i = -1;
		li[++i].Set("Line Pairs", gid, tint_focus_param.LINE_PAIR);
		li[++i].Set("Radius Min", gid, tint_focus_param.INNER_LESS);
		li[++i].Set("",           gid, tint_focus_param.INNER_MORE);
		li[++i].Set("Radius Max", gid, tint_focus_param.OUTER_LESS);
		li[++i].Set("",           gid, tint_focus_param.OUTER_MORE);
		li[++i].Set("Rotate",     gid, tint_focus_param.ROTATE_LESS);
		li[++i].Set("",           gid, tint_focus_param.ROTATE_MORE);
		li[++i].Set("Spread",     gid, tint_focus_param.SECTOR_LESS);
		li[++i].Set("",           gid, tint_focus_param.SECTOR_MORE);
		li[++i].Set("Display",    gid, tint_focus_param.DISPLAY);
		li[++i].Set("Reset",      gid, tint_focus_param.RESET);
		li[++i].Set("Increment",  gid, tint_focus_param.TOGGLEINC);


	// ---- detail
		i = -1;
		li[++i].SetType(vs_system::Tint::ITEM_TEXT);   li[i].SetAtlas(0, 0, 0); li[i].SetLF(TRUE); // (0)
		li[++i].SetType(vs_system::Tint::ITEM_CLICK);  li[i].SetAtlas(3, 5, 0);
		li[++i].SetType(vs_system::Tint::ITEM_CLICK);  li[i].SetAtlas(3, 5, 2); li[i].SetXOff(1.1f);  li[i].SetLF(TRUE);
		li[++i].SetType(vs_system::Tint::ITEM_CLICK);  li[i].SetAtlas(3, 5, 0);
		li[++i].SetType(vs_system::Tint::ITEM_CLICK);  li[i].SetAtlas(3, 5, 2); li[i].SetXOff(1.1f);  li[i].SetLF(TRUE);
		li[++i].SetType(vs_system::Tint::ITEM_CLICK);  li[i].SetAtlas(3, 5, 0);
		li[++i].SetType(vs_system::Tint::ITEM_CLICK);  li[i].SetAtlas(3, 5, 2); li[i].SetXOff(1.1f);  li[i].SetLF(TRUE);
		li[++i].SetType(vs_system::Tint::ITEM_CLICK);  li[i].SetAtlas(3, 5, 0);
		li[++i].SetType(vs_system::Tint::ITEM_CLICK);  li[i].SetAtlas(3, 5, 2); li[i].SetXOff(1.1f);  li[i].SetLF(TRUE);
		li[++i].SetType(vs_system::Tint::ITEM_SWITCH); li[i].SetAtlas(1, 4, 6); li[i].SetLF(TRUE); // (1)
		li[++i].SetType(vs_system::Tint::ITEM_CLICK);  li[i].SetAtlas(3, 3, 4);
		li[++i].SetType(vs_system::Tint::ITEM_SWITCH); li[i].SetAtlas(3, 4, 3); li[i].SetXOff(1.5f); li[i].SetLF(TRUE);


	// ---- select and set display status
		using namespace vs_system;
		INT display;
		CamProps *cam_props = s_pi_model->cam_props;
		INT result = cam_props->GetProperty(
				CamProps::ANNULAR_FOCUS,
				0,
				CamProps::DISPLAY,
				&display
			);
		li[9].SetStatus(display);

	return number_of_items;
}


// ----------------------------------------------------------------------
INT Model::Tint_Focus_GetIconInfo(INT id)
{

	switch (id)
	{
		case vs_system::Tint::TAB_ATLAS_PAGE_ID  : return 1;
		case vs_system::Tint::TAB_ATLAS_PAGE_COL : return 5;
		case vs_system::Tint::TAB_ATLAS_PAGE_ROW : return 1;
		case vs_system::Tint::ICON_ATLAS_PAGE_ID : return 1;
		default:
			break;
	}

	return VS_ERROR;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Focus_UpdateModel(
		INT  handle_model,
		INT  element_id
	)
{

	// ---- local
		BYTE setup_bitmask = VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_INDEX | VS_ELM_SETUP_CONFIG;
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::ObjConfig *obj_config = model->GetObjConfig();
		vs_system::CamProps *cam_props = NULL;
		result = model->GetCamProps(&cam_props);


	// ---- get cam props param
		FLOAT focus_display_f = 0;
		result = cam_props->GetProperty(
					vs_system::CamProps::ANNULAR_FOCUS,
					0,
					vs_system::CamProps::DISPLAY,
					&focus_display_f
				);
		FLOAT focus_min = 0;
		result = cam_props->GetProperty(
					vs_system::CamProps::ANNULAR_FOCUS,
					0,
					vs_system::CamProps::MIN,
					&focus_min
				);
		FLOAT focus_max = 0;
		result = cam_props->GetProperty(
					vs_system::CamProps::ANNULAR_FOCUS,
					0,
					vs_system::CamProps::MAX,
					&focus_max
				);
		FLOAT focus_rotate = 0;
		result = cam_props->GetProperty(
					vs_system::CamProps::ANNULAR_FOCUS,
					0,
					vs_system::CamProps::ROTATE,
					&focus_rotate
				);
		FLOAT focus_sector = 0;
		result = cam_props->GetProperty(
					vs_system::CamProps::ANNULAR_FOCUS,
					0,
					vs_system::CamProps::SECTOR,
					&focus_sector
				);
		UINT focus_display = (UINT)focus_display_f;


	// ---- local
		BOOL focus_updated = FALSE;
		BOOL large_incr = tint_focus_param._large_incr;


	// ---- update
		switch ( element_id )
		{
			case tint_focus_param.INNER_LESS:
				{
					focus_min = focus_min - (large_incr ? 0.01f : 0.001f);
					focus_min = focus_min < 0.001f ? 0.001f : focus_min;
					focus_updated = TRUE;
				}
				break;

			case tint_focus_param.INNER_MORE:
				{
					focus_min = focus_min + (large_incr ? 0.01f : 0.001f);
					focus_min = focus_min > focus_max ? focus_max : focus_min;
					focus_updated = TRUE;
				}
				break;

			case tint_focus_param.OUTER_LESS:
				{
					focus_max = focus_max - (large_incr ? 0.01f : 0.001f);
					focus_max = focus_max < focus_min ? focus_min : focus_max;
					focus_updated = TRUE;
				}
				break;

			case tint_focus_param.OUTER_MORE:
				{
					focus_max = focus_max + (large_incr ? 0.01f : 0.001f);
					focus_max = focus_max > 1 ? 1 : focus_max;
					focus_updated = TRUE;
				}
				break;

			case tint_focus_param.ROTATE_LESS:
				{
					focus_rotate = focus_rotate - (large_incr ? 1 : 0.1f);
					focus_updated = TRUE;
				}
				break;

			case tint_focus_param.ROTATE_MORE:
				{
					focus_rotate = focus_rotate + (large_incr ? 1 : 0.1f);
					focus_updated = TRUE;
				}
				break;

			case tint_focus_param.SECTOR_LESS:
				{
					focus_sector = focus_sector - (large_incr ? 1 : 0.1f);
					focus_updated = TRUE;
				}
				break;

			case tint_focus_param.SECTOR_MORE:
				{
					focus_sector = focus_sector + (large_incr ? 1 : 0.1f);
					focus_updated = TRUE;
				}
				break;

			case tint_focus_param.DISPLAY_ON:
			case tint_focus_param.DISPLAY_OFF:
			case tint_focus_param.DISPLAY:
				{
					focus_display = focus_display ? 0 : 1;

					vs_system::PanelContext *panel_foc_aoi = (vs_system::PanelContext*)
								model->pi_model->handle_pan_gfx_con_pano_annu_foc_aoi;

					vs_system::PanelContext *panel_foc_grf = (vs_system::PanelContext*)
						model->pi_model->handle_pan_gfx_con_pano_annu_foc_grf;

					if ( focus_display )
					{
						panel_foc_aoi->SetDisplayStatus(TRUE);
						panel_foc_grf->SetDisplayStatus(TRUE);
					}
					else
					{
						panel_foc_aoi->SetDisplayStatus(FALSE);
						panel_foc_grf->SetDisplayStatus(FALSE);
					}
				}
				break;

			case tint_focus_param.RESET:
				{
					focus_min     = 0.65f;
					focus_max     = 0.92f;
					focus_rotate  = 0;
					focus_sector  = 5;
					focus_display = 1;
					focus_updated = TRUE;

					vs_system::PanelContext *panel_foc_aoi = (vs_system::PanelContext*)
						model->pi_model->handle_pan_gfx_con_pano_annu_foc_aoi;
					panel_foc_aoi->SetDisplayStatus(TRUE);

					vs_system::PanelContext *panel_foc_grf = (vs_system::PanelContext*)
						model->pi_model->handle_pan_gfx_con_pano_annu_foc_grf;
					panel_foc_grf->SetDisplayStatus(TRUE);
				}
				break;

			case tint_focus_param.TOGGLEINC:
				tint_focus_param._large_incr = tint_focus_param._large_incr ? FALSE : TRUE;
				break;

			default:
				setup_bitmask = VS_ELM_SETUP_NONE;
				break;
		}


	// ---- limit
		focus_rotate = focus_rotate > 360 ?
						focus_rotate - 360 :
							(focus_rotate < 0 ? 360 + focus_rotate : focus_rotate);
		focus_sector = focus_sector > 360 ?
							360 : (focus_sector < 1 ? 1 : focus_sector);


	// ---- set cam props param
		result = cam_props->SetProperty(
					vs_system::CamProps::ANNULAR_FOCUS,
					0,
					vs_system::CamProps::DISPLAY,
					(FLOAT)focus_display
				);
		result = cam_props->SetProperty(
				vs_system::CamProps::ANNULAR_FOCUS,
					0,
					vs_system::CamProps::MIN,
					focus_min
				);
		result = cam_props->SetProperty(
				vs_system::CamProps::ANNULAR_FOCUS,
					0,
					vs_system::CamProps::MAX,
					focus_max
				);
		result = cam_props->SetProperty(
				vs_system::CamProps::ANNULAR_FOCUS,
					0,
					vs_system::CamProps::ROTATE,
					(FLOAT)focus_rotate
				);
		result = cam_props->SetProperty(
				vs_system::CamProps::ANNULAR_FOCUS,
					0,
					vs_system::CamProps::SECTOR,
					(FLOAT)focus_sector
				);


	// ---- set model pano aoi
		result = obj_config->SetPanoFocusAoiMin(focus_min);
		result = obj_config->SetPanoFocusAoiMax(focus_max);
		result = obj_config->SetPanoFocusAoiRotate(focus_rotate);
		result = obj_config->SetPanoFocusAoiSector(focus_sector);


	// ---- set dependent config bitmasks
		if (setup_bitmask != VS_ELM_SETUP_NONE)
		{
			vs_system::AppDependency *app_dependency = model->pi_model->app_dependency;
			vs_system::GfxInterface  *gfx_interface = app_dependency->GetGfxInterface();
			result = gfx_interface->SetGlobalConfigureBitmask(
					setup_bitmask,
					"Annular Focus"
				);
		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Focus_UpdateElement(
		INT handle_model,
		INT element_id
	)
{

	// ---- local
		INT result = VS_OK;
		Model *model = (Model *)handle_model;
		vs_system::Tint *tint = model->pi_model->tint_cam_setng;


	// ---- set tint body logical id (invoked outside, e.g. model, the tint ecosystem)
		result = model->pi_model->tint_cam_setng->SetSelectedBody(TINT_HEAD_SYSTEM_LID);


	// ---- update
		switch ( element_id )
		{
			case tint_focus_param.INNER_LESS:
			case tint_focus_param.INNER_MORE:
				{
					result = model->pi_model->tint_cam_setng->UpdateText(NULL, tint_focus_param.INNER_LESS);
					result = model->pi_model->tint_cam_setng->UpdateText(NULL, tint_focus_param.LINE_PAIR);
				}
				break;
			case tint_focus_param.OUTER_LESS:
			case tint_focus_param.OUTER_MORE:
				{
					result = model->pi_model->tint_cam_setng->UpdateText(NULL, tint_focus_param.OUTER_LESS);
					result = model->pi_model->tint_cam_setng->UpdateText(NULL, tint_focus_param.LINE_PAIR);
				}
				break;
			case tint_focus_param.ROTATE_LESS:
			case tint_focus_param.ROTATE_MORE:
				{
					result = model->pi_model->tint_cam_setng->UpdateText(NULL,tint_focus_param.ROTATE_LESS);
				}
				break;
			case tint_focus_param.SECTOR_LESS:
			case tint_focus_param.SECTOR_MORE:
				{
					result = model->pi_model->tint_cam_setng->UpdateText(NULL,tint_focus_param.SECTOR_LESS);
				}
				break;
			case tint_focus_param.RESET:
				{
					result = model->pi_model->tint_cam_setng->UpdateText(NULL, tint_focus_param.INNER_LESS);
					result = model->pi_model->tint_cam_setng->UpdateText(NULL, tint_focus_param.OUTER_LESS);
					result = model->pi_model->tint_cam_setng->UpdateText(NULL, tint_focus_param.ROTATE_LESS);
					result = model->pi_model->tint_cam_setng->UpdateText(NULL, tint_focus_param.SECTOR_LESS);
					result = model->pi_model->tint_cam_setng->UpdateStatus(tint_focus_param.DISPLAY, TRUE);
				}
				break;
			default:
				break;
		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT  Model::Tint_Focus_AddText(
		const INT  handle_ass_obj,
		const INT  element_id,
		BOOL      *replace_text,
		BOOL      *add_text,
		CHAR      *text
	)
{

	// ---- local ----
		INT result = VS_OK;


	// ---- init ----
		*replace_text = TRUE;
		*add_text     = FALSE;


	// ---- init ----
		FLOAT focus_min = 0;
		FLOAT focus_max = 0;
		FLOAT hby2 = 0;
		FLOAT rows = 0;


	// ---- cast to get camera properties object
		vs_system::CamProps *cam_props = (vs_system::CamProps *)handle_ass_obj;


	// ---- lambda ------------------------------------------------------------
		auto fminimax = [&]()
		{
			FLOAT roi_height = 0;
			result = cam_props->GetProperty(
						vs_system::CamProps::ROI,
						0,
						vs_system::CamProps::HEIGHT,
						&roi_height
					);
			result = cam_props->GetProperty(
						vs_system::CamProps::ANNULAR_FOCUS,
						0,
						vs_system::CamProps::MIN,
						&focus_min
					);
			result = cam_props->GetProperty(
						vs_system::CamProps::ANNULAR_FOCUS,
						0,
						vs_system::CamProps::MAX,
						&focus_max
					);
			hby2 = roi_height / 2;
			rows = floor( (focus_max - focus_min) * hby2 );
		};


	// ---- set
		switch (element_id)
		{
			case tint_focus_param.LINE_PAIR:
				{
					fminimax();
					sprintf(text, "There are %d focus lines", (INT)rows);
				}
				break;
			default:
				break;
		}

	return VS_OK;
}


// ----------------------------------------------------------------------
INT Model::Tint_Focus_AddValue(
		const INT  handle_ass_obj,
		const INT  element_id,
		BOOL      *add_value,
		FLOAT     *value,
		CHAR      *suffix
	)
{

	// ---- local
		INT result = VS_OK;


	// ---- init
		*add_value = TRUE;
		*value     = 0;
		sprintf(suffix, "2");


	// ---- init
		FLOAT focus_min = 0;
		FLOAT focus_max = 0;
		FLOAT hby2 = 0;
		FLOAT rows = 0;


	// ---- cast to get camera properties object
		vs_system::CamProps *cam_props = (vs_system::CamProps *)handle_ass_obj;


	// ---- lambda ------------------------------------------------------------
		auto fminimax = [&]()
		{
			FLOAT roi_height = 0;
			result = cam_props->GetProperty(
						vs_system::CamProps::ROI,
						0,
						vs_system::CamProps::HEIGHT,
						&roi_height
					);
			result = cam_props->GetProperty(
						vs_system::CamProps::ANNULAR_FOCUS,
						0,
						vs_system::CamProps::MIN,
						&focus_min
					);
			result = cam_props->GetProperty(
						vs_system::CamProps::ANNULAR_FOCUS,
						0,
						vs_system::CamProps::MAX,
						&focus_max
					);
			hby2 = roi_height / 2;
			rows = floor( (focus_max - focus_min) * hby2 );
		};


	// ---- set ----
		switch (element_id)
		{
			case tint_focus_param.LINE_PAIR:
				{
					*add_value = FALSE;
				}
				break;
			case tint_focus_param.INNER_LESS:
				{
					fminimax();
					*value = focus_min;
				}
				break;
			case tint_focus_param.OUTER_LESS:
				{
					fminimax();
					*value = focus_max;
				}
				break;
			case tint_focus_param.ROTATE_LESS:
				{
					FLOAT focus_rotate = 0;
					result = cam_props->GetProperty(
							vs_system::CamProps::ANNULAR_FOCUS,
							0,
							vs_system::CamProps::ROTATE,
							&focus_rotate
						);
					*value = focus_rotate;
				}
				break;
			case tint_focus_param.SECTOR_LESS:
				{
					FLOAT focus_sector = 0;
						result = cam_props->GetProperty(
							vs_system::CamProps::ANNULAR_FOCUS,
							0,
							vs_system::CamProps::SECTOR,
							&focus_sector
						);
					*value = focus_sector;
				}
				break;
			default:
				*add_value = FALSE;
				break;
		}

	return VS_OK;
}

