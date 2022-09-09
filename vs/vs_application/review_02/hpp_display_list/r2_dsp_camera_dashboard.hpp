////////////////////////////////////////////////////////////////////////////////

// ---------- r2_dl_camera_dashboard.hpp ----------
/*!
\file r2_dl_camera_dashboard.hpp
\brief 
\author Gareth Edwards
*/

#include "../header/r2_model.h"

using namespace review_02;


// ---- dial init const dim for arrays
	const UINT cam_dbz_dial_total           = 24;
	const UINT cam_dbz_dial_exposure_index  = 9;
	const UINT cam_dbz_dial_framerate_index = 13;
	const UINT cam_dbz_dial_gain_wb_index   = 18;


// ---------- SetupCameraDashboard ----------
/*!
\brief Setup (initialise) camera dashboard
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupCameraDashboard()
{

	// ---- format
		vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
		DWORD format = sys_context->GetDWord(vs_system::SysContext::FORMAT);


	// ---- format scaling
		db_param.global_scale = format == vs_system::SysContext::PORTRAIT ? 0.2f : 0.35f;


	// --- update state
		for (UINT i = 0; i < Dbz_Cam_Index::NUMBER_OF_PARAM; i++)
		{
			cam_dbz_param[i]  = NULL_VALUE;
			cam_dbz_update[i] = FALSE;
		}

	
	// ---- gps text
		for (UINT i = 0; i < 4; i++)
		{
			cam_dbz_str_text[i].assign(""); 
		}

	return VS_OK;
}


// ---------- SetupCameraDashboardDials ----------
/*!
\brief Setup record dials display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupCameraDashboardDials()
{

	// ---- local
		INT result = 0;


	// ---- format
		vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
		DWORD format = sys_context->GetDWord(vs_system::SysContext::FORMAT);


	// ---- bitmap shortcut
		UINT bm_shape    = vs_system::ObjConfig::DBZ_DIAL_CONFIG_BITMAP_SHAPE;
		UINT bm_edge_max = vs_system::ObjConfig::DBZ_DIAL_CONFIG_BITMAP_EDGE_MAX;
		UINT bm_bars     = vs_system::ObjConfig::DBZ_DIAL_CONFIG_BITMAP_BARS;


	// ---- bitmap param
		UINT bg_out  = bm_shape;
		UINT bg_in   = bm_shape;
		UINT fg_out  = bm_edge_max | bm_shape | bm_bars;
		UINT fg_in   = bm_edge_max | bm_shape | bm_bars;
		UINT fg_blip = bm_bars;


	// ---- init offset and scaling param
		FLOAT tx, ty_l, ty_h, scale;
		if ( format == vs_system::SysContext::PORTRAIT )
		{
			tx    = db_param.dial_tx_portrait ;
			ty_l  = db_param.dial_ty_l_portrait;
			ty_h  = db_param.dial_ty_h_portrait;
		}
		else
		{
			tx    = db_param.dial_tx_landscape;
			ty_l  = db_param.dial_ty_l_landscape;
			ty_h  = db_param.dial_ty_h_landscape;
		}
		scale = db_param.global_scale;

		// outer trim
		FLOAT rl_trim = 1.30f * scale;
		FLOAT rh_trim = 1.32f * scale;

		// outer (exp max, frm sec, rgb wb) 
		FLOAT rl_out  = 1.05f * scale;
		FLOAT rh_out  = 1.12f * scale;

		// inner (exp, frm, gain)
		FLOAT rl_in   = 0.80f * scale;
		FLOAT rh_in   = 1.00f * scale;

		// inner (fts)
		FLOAT rl_fr   = 0.70f * scale;
		FLOAT rh_fr   = 0.75f * scale;


	// ---- dashboard method param
		FLOAT edge_size = 0.0125f * scale;
		FLOAT rotate = 90.0f;


	// ---- dial init struct
		struct DbDial {
			UINT  _bitmask;
			FLOAT _trans_x;
			FLOAT _trans_y;
			FLOAT _rotate;
			FLOAT _radius_min;
			FLOAT _radius_max;
			FLOAT _angle_min;
			FLOAT _angle_max;
		};


	// ---- dial init lut
		DbDial dashboard_dial[cam_dbz_dial_total] =
		{
			{ bg_out, -tx, ty_l,   0, rl_trim, rh_trim, 0, 360 },  // 
			{ bg_out, -tx, ty_l,   0, rl_out,  rh_out,  0, 270 },  // exposure
			{ bg_in,  -tx, ty_l,   0, rl_in,   rh_in,   0, 270 },  // exposure + 2

			{ bg_out,   0, ty_h,   0, rl_trim, rh_trim, 0, 360 },  //
			{ bg_out,   0, ty_h,   0, rl_out,  rh_out,  0, 270 },  // framerate
			{ bg_in,    0, ty_h,   0, rl_in,   rh_in,   0, 270 },  // framerate + 2

			{ bg_out,  tx, ty_l,   0, rl_trim, rh_trim, 0, 360 },  // 
			{ bg_out,  tx, ty_l,   0, rl_out,  rh_out,  0, 270 },  // gain wb
			{ bg_in,   tx, ty_l,   0, rl_in,   rh_in,   0, 270 },  // gain wb + 4

			{ fg_out, -tx, ty_l,   0, rl_out,  rh_out,  0, 100 },  // expos - min / max
			{ fg_blip,-tx, ty_l,   0, rl_out,  rh_out,  0, 270 },
			{ fg_in,  -tx, ty_l,   0, rl_in,   rh_in,   0, 100 },  //       - val
			{ fg_blip,-tx, ty_l,   0, rl_in,   rh_in,   0, 270 },

			{ fg_out,   0, ty_h,   0, rl_out,  rh_out,  0, 100 },  // framerate - seconds
			{ fg_blip,  0, ty_h,   0, rl_out,  rh_out,  0, 270 },
			{ fg_in,    0, ty_h,   0, rl_in,   rh_in,   0, 100 },  // framerate - fls
			{ fg_blip,  0, ty_h,   0, rl_in,   rh_in,   0, 270 },
			{ fg_in,    0, ty_h,   0, rl_fr,   rh_fr,   0,  30 },  // framerate - fts

			{ fg_out,  tx, ty_l, 180, rl_out,  rh_out,  0,  60 },  // wb - red
			{ fg_out,  tx, ty_l,  90, rl_out,  rh_out,  0,  60 },  // wb - green
			{ fg_out,  tx, ty_l,   0, rl_out,  rh_out,  0,  60 },  // wb - blue
			{ fg_blip, tx, ty_l,   0, rl_out,  rh_out,  0, 270 },
			{ fg_in,   tx, ty_l,   0, rl_in,   rh_in,   0, 100 },  // gain master
			{ fg_blip, tx, ty_l,   0, rl_in,   rh_in,   0, 270 },

		};


	// ---- init dials display list
		for (UINT i = 0; i < cam_dbz_dial_total; i++)
		{

			// ---- add & get context
				pi_model->handle_dbz_cam_dial[i] = gfx->AddElement(
						pi_model->handle_dspl_dbz,
						"",
						0,
						0,
						&pi_model->obj_config->Dbz_Callback_Configure_Dial
					);
				vs_system::ElmContext *elm = NULL;
				result = gfx->GetElementContext(pi_model->handle_dbz_cam_dial[i], &elm);


			// ---- set default
				FLOAT *p = elm->GetFloatParameter();


			// ---- set db
				result = pi_model->obj_config->Dbz_SetDefault(p);
				result = pi_model->obj_config->Dbz_SetDialDefault(p);


			// ---- set param
				DbDial *dbz_dial = &dashboard_dial[i];
				result = pi_model->obj_config->Dbz_SetConfig(p, (FLOAT)dbz_dial->_bitmask);
				result = pi_model->obj_config->Dbz_SetScale(p, 1.0f, 1.0f);
				result = pi_model->obj_config->Dbz_SetRotate(p, rotate + dbz_dial->_rotate);
				result = pi_model->obj_config->Dbz_SetTranslate(p, dbz_dial->_trans_x, dbz_dial->_trans_y);

				result = pi_model->obj_config->Dbz_SetDialRadii(p, dbz_dial->_radius_min, dbz_dial->_radius_max);
				result = pi_model->obj_config->Dbz_SetDialAngle(p, dbz_dial->_angle_min, dbz_dial->_angle_max);


			// ---- set edge colour
				result = pi_model->obj_config->Dbz_SetDialEdgeRGBMin(p, 0.2f, 0.2f, 0.2f);
				result = pi_model->obj_config->Dbz_SetDialEdgeRGBMax(p, 0.8f, 0.8f, 0.8f);
				result = pi_model->obj_config->Dbz_SetDialShapeRGBMin(p, db_param.c_r_min, db_param.c_g_min, db_param.c_b_min);
				result = pi_model->obj_config->Dbz_SetDialShapeRGBMax(p, db_param.c_r_max, db_param.c_g_max, db_param.c_b_max);


			// ---- set edge param
				result = pi_model->obj_config->Dbz_SetDialEdgeSize(p, edge_size);
				result = pi_model->obj_config->Dbz_SetDialBlipEdgeSize(p, edge_size);


			// ---- set specific param
				switch (i)
				{

					// ---- bg ----
					case 0:
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMin(p, db_param.c_r_min, db_param.c_g_min, db_param.c_b_min);
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMax(p, db_param.c_r_min, db_param.c_g_min, db_param.c_b_min);
						break;

					case 1:
					case 2:

					case 3:
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMin(p, db_param.c_r_min, db_param.c_g_min, db_param.c_b_min);
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMax(p, db_param.c_r_min, db_param.c_g_min, db_param.c_b_min);
						break;
					case 4:
					case 5:

					case 6:
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMin(p, db_param.c_r_min, db_param.c_g_min, db_param.c_b_min);
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMax(p, db_param.c_r_min, db_param.c_g_min, db_param.c_b_min);
						break;

					case 7:
					case 8:
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMin(p, db_param.m_bg, db_param.m_bg, db_param.m_bg);
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMax(p, db_param.m_bg, db_param.m_bg, db_param.m_bg);
						break;


					case cam_dbz_dial_exposure_index:
						break;
					case cam_dbz_dial_exposure_index + 1:
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMin(p, db_param.m_blip, db_param.m_blip, db_param.m_blip);
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMax(p, db_param.m_fg, db_param.m_fg, db_param.m_fg);
						result = pi_model->obj_config->Dbz_SetDialBlipCount(p, 10);
						break;
					case cam_dbz_dial_exposure_index + 2:
						break;
					case cam_dbz_dial_exposure_index + 3:
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMin(p, db_param.m_blip, db_param.m_blip, db_param.m_blip);
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMax(p, db_param.m_fg, db_param.m_fg, db_param.m_fg);
						result = pi_model->obj_config->Dbz_SetDialBlipCount(p, 10);
						break;


					case cam_dbz_dial_framerate_index:
						break;
					case cam_dbz_dial_framerate_index + 1:
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMin(p, db_param.m_blip, db_param.m_blip, db_param.m_blip);
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMax(p, db_param.m_fg, db_param.m_fg, db_param.m_fg);
						result = pi_model->obj_config->Dbz_SetDialBlipCount(p, 60);
						break;
					case cam_dbz_dial_framerate_index + 2:
						break;
					case cam_dbz_dial_framerate_index + 3:
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMin(p, db_param.m_blip, db_param.m_blip, db_param.m_blip);
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMax(p, db_param.m_fg, db_param.m_fg, db_param.m_fg);
						result = pi_model->obj_config->Dbz_SetDialBlipCount(p, 9);
					case cam_dbz_dial_framerate_index + 4:
						break;


					case cam_dbz_dial_gain_wb_index:
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMin(p, 0.8f, 0, 0);
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMax(p, 1.0f, 0, 0);
						break;
					case cam_dbz_dial_gain_wb_index + 1:
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMin(p, 0, 0.8f, 0);
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMax(p, 0, 1.0f, 0);
						break;
					case cam_dbz_dial_gain_wb_index + 2:
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMin(p, 0, 0, 0.8f);
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMax(p, 0, 0, 1.0f);
						break;
					case cam_dbz_dial_gain_wb_index + 3:
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMin(p, db_param.m_blip, db_param.m_blip, db_param.m_blip);
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMax(p, db_param.m_fg, db_param.m_fg, db_param.m_fg);
						result = pi_model->obj_config->Dbz_SetDialBlipCount(p, 30);
						break;
					case cam_dbz_dial_gain_wb_index + 4:
						break;
					case cam_dbz_dial_gain_wb_index + 5:
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMin(p, db_param.m_blip, db_param.m_blip, db_param.m_blip);
						result = pi_model->obj_config->Dbz_SetDialShapeRGBMax(p, db_param.m_fg, db_param.m_fg, db_param.m_fg);
						result = pi_model->obj_config->Dbz_SetDialBlipCount(p, 10);
						break;

					default:
						break;
				}

		}

	return VS_OK;
}


// ---------- SetupCameraDashboardIcons ----------
/*!
\brief Setup record icons display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupCameraDashboardIcons()
{

	// ---- local
		INT result = 0;


	// ---- format
		vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
		DWORD format = sys_context->GetDWord(vs_system::SysContext::FORMAT);


	// ---- init struct
		struct DbIcon {
			FLOAT _trans_x;
			FLOAT _trans_y;
			FLOAT _size_x;
			FLOAT _size_y;
			FLOAT _text_row;
			FLOAT _text_col;
		};


	// ---- init offset and scaling param
		FLOAT tx, ty_l, ty_h, scale;
		if ( format == vs_system::SysContext::PORTRAIT )
		{
			tx    = db_param.dial_tx_portrait;
			ty_l  = db_param.dial_ty_l_portrait;
			ty_h  = db_param.dial_ty_h_portrait;
		}
		else
		{
			tx    = db_param.dial_tx_landscape;
			ty_l  = db_param.dial_ty_l_landscape;
			ty_h  = db_param.dial_ty_h_landscape;
		}
		scale = db_param.global_scale * 0.22f;


		FLOAT  dashboard_apg = 3;
		DbIcon dashboard_icon[3] =
			{
				{ -tx,   ty_l, scale, scale, 4, 4 },  // A
				{   0,   ty_h, scale, scale, 4, 4 },  // A
				{  tx,   ty_l, scale, scale, 4, 4 },  // A
			};


	// ---- init icon/base/needle
		for (UINT i = 0; i < 3; i++)
		{

			// ---- icon -- add
				pi_model->handle_dbz_cam_icon[i] = gfx->AddElement(
						pi_model->handle_dspl_dbz,
						"",
						0,
						0,
						&pi_model->obj_config->Dbz_Callback_Configure_Icon
					);
				vs_system::ElmContext *elm = NULL;
				result = gfx->GetElementContext(pi_model->handle_dbz_cam_icon[i], &elm);


			// ---- icon -- set
				DbIcon *dbz_icon = &dashboard_icon[i];
				FLOAT *p = elm->GetFloatParameter();
				result = pi_model->obj_config->Dbz_SetDefault(p);
				result = pi_model->obj_config->Dbz_SetScale(p, dbz_icon->_size_x, dbz_icon->_size_y);
				result = pi_model->obj_config->Dbz_SetTranslate(p, dbz_icon->_trans_x, dbz_icon->_trans_y);

				result = pi_model->obj_config->Dbz_SetIconTexture(
						p,
						dashboard_apg,
						dbz_icon->_text_row,
						dbz_icon->_text_col,
						1,
						1,
						0.001f
					);


			// ---- 2 x 180 base
				for (UINT ib = 0; ib < 2; ib++)
				{

					// ---- base -- add
						INT icon_base = gfx->AddElement(
								pi_model->handle_dspl_dbz,
								"",
								0,
								0,
								&pi_model->obj_config->Dbz_Callback_Configure_Dial
							);
						result = gfx->GetElementContext(icon_base, &elm);


					// ---- base
						p = elm->GetFloatParameter();
						result = pi_model->obj_config->Dbz_SetDefault(p);
						result = pi_model->obj_config->Dbz_SetConfig(p, (FLOAT)vs_system::ObjConfig::DBZ_DIAL_CONFIG_BITMAP_SHAPE);
						result = pi_model->obj_config->Dbz_SetTranslate(p, dbz_icon->_trans_x, dbz_icon->_trans_y);

						result = pi_model->obj_config->Dbz_SetDialRadii(p, dbz_icon->_size_x, dbz_icon->_size_x + 0.01f);

						if (ib == 0)
						{
							result = pi_model->obj_config->Dbz_SetDialAngle(p, 0, 180);
							result = pi_model->obj_config->Dbz_SetRotate(p, 135);
							result = pi_model->obj_config->Dbz_SetDialShapeRGBMax(p, db_param.c_r_max, db_param.c_g_max, db_param.c_b_max);
							result = pi_model->obj_config->Dbz_SetDialShapeRGBMin(p, db_param.c_r_min, db_param.c_g_min, db_param.c_b_min);
						}
						else
						{
							result = pi_model->obj_config->Dbz_SetDialAngle(p, 0, 180);
							result = pi_model->obj_config->Dbz_SetRotate(p, 315);
							result = pi_model->obj_config->Dbz_SetDialShapeRGBMin(p, db_param.c_r_max, db_param.c_g_max, db_param.c_b_max);
							result = pi_model->obj_config->Dbz_SetDialShapeRGBMax(p, db_param.c_r_min, db_param.c_g_min, db_param.c_b_min);
						}
				}


			// ---- needle -- add
				FLOAT r[3] = { 70, 270, 110 };
				FLOAT needle_y_scale = 0.786f * db_param.global_scale;
				{

					// ---- add
						pi_model->handle_dbz_cam_needle[i] = gfx->AddElement(
								pi_model->handle_dspl_dbz,
								"",
								0,
								0,
								&pi_model->obj_config->Dbz_Callback_Configure_Needle
							);
						result = gfx->GetElementContext(pi_model->handle_dbz_cam_needle[i], &elm);


					// ---- set db
						FLOAT *p = elm->GetFloatParameter();
						result = pi_model->obj_config->Dbz_SetDefault(p);
						result = pi_model->obj_config->Dbz_SetScale(p, 1.0f, needle_y_scale);
						result = pi_model->obj_config->Dbz_SetRotate(p, r[i] - 90);
						result = pi_model->obj_config->Dbz_SetTranslate(p, dbz_icon->_trans_x, dbz_icon->_trans_y);

						result = pi_model->obj_config->Dbz_SetNeedleRGBMax(p, db_param.c_r_max, db_param.c_g_max, db_param.c_b_max);
						result = pi_model->obj_config->Dbz_SetNeedleRGBMin(p, db_param.c_r_min, db_param.c_g_min, db_param.c_b_min);

				}
		}

	return VS_OK;
}


// ---------- SetupCameraDashboardText ----------
/*!
\brief Setup record text display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupCameraDashboardText()
{

	// ---- local
		INT result = 0;


	// ---- format
		vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
		DWORD format = sys_context->GetDWord(vs_system::SysContext::FORMAT);


	// ---- init struct
		struct DbText {
			FLOAT _trans_x;
			FLOAT _trans_y;
			FLOAT _size_x;
			FLOAT _size_y;
			FLOAT _config;
			CHAR  _text[16];
		};


	// ---- init offset and scaling param
		FLOAT tx, ty_l, ty_h, scale;
		if ( format == vs_system::SysContext::PORTRAIT )
		{
			tx    = db_param.dial_tx_portrait;
			ty_l  = db_param.dial_ty_l_portrait;
			ty_h  = db_param.dial_ty_h_portrait;
		}
		else
		{
			tx    = db_param.dial_tx_landscape;
			ty_l  = db_param.dial_ty_l_landscape;
			ty_h  = db_param.dial_ty_h_landscape;
		}
		scale = db_param.global_scale;


	// ---- init lut
		FLOAT x1, x2, x3, y1, y2, text_ys;
		if (format == vs_system::SysContext::PORTRAIT)
		{
			x1      = -0.290f;
			x2      =  0.025f;
			x3      =  0.320f;
			y1      =  0.3f;
			y2      = -0.18f;
			text_ys = db_param.portrait_y_text;
		}
		else
		{
			x1      = -0.975f;
			x2      =  0.025f;
			x3      =  1.025f;
			y1      =  0.55f;
			y2      = -0.31f;
			text_ys = db_param.landscape_y_text;
		}

		FLOAT cac = (FLOAT)pi_model->obj_config->DBZ_TEXT_LABEL_ALIGNED_CENTRE;
		FLOAT lal = (FLOAT)pi_model->obj_config->DBZ_TEXT_LABEL_ALIGNED_LEFT;
		DbText dashboard_text[6] =
		{
			{ -tx,  ty_l + y1, 0, text_ys, cac, "Exposure"  },
			{   0,  ty_h + y1, 0, text_ys, cac, "Framerate" },
			{  tx,  ty_l + y1, 0, text_ys, cac, "Gain & WB" },
			{  x1,  ty_l + y2, 0, text_ys, lal, "0 ms"      },
			{  x2,  ty_h + y2, 0, text_ys, lal, "0 fps"     },
			{  x3,  ty_l + y2, 0, text_ys, lal, "0%"        },
		};


	// ---- init icon/name
		for (UINT i = 0; i < 6; i++)
		{

			// ---- add
				pi_model->handle_dbz_cam_text[i] = gfx->AddElement(
						pi_model->handle_dspl_dbz,
						"",
						0,
						0,
						&pi_model->obj_config->Dbz_Callback_Configure_Text
					);

				vs_system::ElmContext *elm = NULL;
				result = gfx->GetElementContext(pi_model->handle_dbz_cam_text[i], &elm);


			// ---- set elm
				DbText *dbz_text = &dashboard_text[i];
				elm->SetText(dbz_text->_text);
				elm->SetRGBA(db_param.c_r_max, db_param.c_g_max, db_param.c_b_max, 1.0f);


			// ---- set db
				FLOAT *p = elm->GetFloatParameter();
				result = pi_model->obj_config->Dbz_SetDefault(p);
				result = pi_model->obj_config->Dbz_SetTranslate(p, dbz_text->_trans_x, dbz_text->_trans_y);
				result = pi_model->obj_config->Dbz_SetScale(p, dbz_text->_size_x, dbz_text->_size_y);
				result = pi_model->obj_config->Dbz_SetConfig(p, dbz_text->_config);

		}

	return VS_OK;
}



// ---------- SetupCameraDashboardGps ----------
/*!
\brief Setup record gps display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupCameraDashboardGps()
{

	// ---- local
		INT result = 0;


	// ---- format
		vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
		DWORD format = sys_context->GetDWord(vs_system::SysContext::FORMAT);


	// ---- init struct
		struct DbText {
			FLOAT _trans_x;
			FLOAT _trans_y;
			FLOAT _size_x;
			FLOAT _size_y;
			FLOAT _config;
			CHAR  _text[16];
		};


	// ---- init offset and scaling param
		FLOAT tx, ty_l, ty_h, scale;
		if ( format == vs_system::SysContext::PORTRAIT )
		{
			tx   = db_param.dial_tx_portrait;
			ty_l = db_param.dial_ty_l_portrait;
			ty_h = db_param.dial_ty_h_portrait;
		}
		else
		{
			tx   = db_param.dial_tx_landscape;
			ty_l = db_param.dial_ty_l_landscape;
			ty_h = db_param.dial_ty_h_landscape;
		}
		scale = db_param.global_scale;

		FLOAT xs, y1, y2 , y_loc, text_ys, e_width, e_height;
		if ( format == vs_system::SysContext::PORTRAIT )
		{
			xs       = scale * 3.0f;
			y1       = ty_l + 1.10f;
			y2       = y1   + 0.05f;
			y_loc    = (y1 + y2) / 2;
			text_ys  = db_param.portrait_y_text;
			e_width  = 2.8f * xs;
			e_height = 0.12f;
		}
		else
		{
			xs       = scale * 3.0f;
			y1       = ty_l + 0.95f;
			y2       = y1   + 0.10f;
			y_loc    = (y1 + y2) / 2;
			text_ys  = db_param.landscape_y_text;
			e_width  = 2.8f * xs;
			e_height = 0.25f;
		}


		FLOAT lac = (FLOAT)pi_model->obj_config->DBZ_TEXT_LABEL_ALIGNED_CENTRE;
		DbText dashboard_gps[8] =
		{
			{ -1.00f, y1, 0, text_ys, lac, "Latitude"        },
			{ -0.28f, y1, 0, text_ys, lac, "Longitude"       },
			{  0.40f, y1, 0, text_ys, lac, "Time"            },
			{  1.00f, y1, 0, text_ys, lac, "Date"            },
			{ -1.00f, y2, 0, text_ys, lac, "00° 00' 00.00 ?" },
			{ -0.28f, y2, 0, text_ys, lac, "00° 00' 00.00 ?" },
			{  0.40f, y2, 0, text_ys, lac, "00-00-00"        },
			{  1.00f, y2, 0, text_ys, lac, "00:00:0000"      }
		};


	// ---- name and value
		for (UINT i = 0; i < 8; i++)
		{

			// ---- add
				pi_model->handle_dbz_cam_gps[i] = gfx->AddElement(
						pi_model->handle_dspl_dbz,
						"",
						0,
						0,
						&pi_model->obj_config->Dbz_Callback_Configure_Text
					);

				vs_system::ElmContext *elm = NULL;
				result = gfx->GetElementContext(pi_model->handle_dbz_cam_gps[i], &elm);


			// ---- set elm
				DbText *dbz_gps = &dashboard_gps[i];
				elm->SetText(dbz_gps->_text);
				elm->SetRGBA(db_param.c_r_max, db_param.c_g_max, db_param.c_b_max, 1.0f);


			// ---- set db
				FLOAT *p = elm->GetFloatParameter();
				result = pi_model->obj_config->Dbz_SetDefault(p);
				result = pi_model->obj_config->Dbz_SetConfig(p, dbz_gps->_config);
				result = pi_model->obj_config->Dbz_SetVisible(p, 0);
				result = pi_model->obj_config->Dbz_SetScale(p, dbz_gps->_size_x, dbz_gps->_size_y);
				result = pi_model->obj_config->Dbz_SetTranslate(p, dbz_gps->_trans_x * xs, dbz_gps->_trans_y);

		}


	// ---- enclose
		result = SetupCameraDashboardEnclose(&pi_model->handle_dbz_cam_gps[8], y_loc, e_width, e_height);

	return VS_OK;
}


// ---------- SetupCameraDashboardSys ----------
/*!
\brief Setup sys display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupCameraDashboardSys()
{

	// ---- local
		INT result = 0;


	// ---- format
		vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
		DWORD format = sys_context->GetDWord(vs_system::SysContext::FORMAT);


	// ---- init gps init struct
		struct DbText {
			FLOAT _trans_x;
			FLOAT _trans_y;
			FLOAT _size_x;
			FLOAT _size_y;
			FLOAT _config;
			CHAR  _text[16];
		};


	// ---- init offset and scaling param
		FLOAT tx, ty_l, ty_h, scale;
		if ( format == vs_system::SysContext::PORTRAIT )
		{
			tx    = db_param.dial_tx_portrait;
			ty_l  = db_param.dial_ty_l_portrait;
			ty_h  = db_param.dial_ty_h_portrait;
		}
		else
		{
			tx    = db_param.dial_tx_landscape;
			ty_l  = db_param.dial_ty_l_landscape;
			ty_h  = db_param.dial_ty_h_landscape;
		}
		scale = db_param.global_scale;


		FLOAT xs, y1, y2 , y_loc, text_ys, e_width, e_height;
		if ( format == vs_system::SysContext::PORTRAIT )
		{
			xs       = scale * 3.0f;
			y1       = ty_l + 1.30f;
			y2       = y1   + 0.05f;
			y_loc    = (y1 + y2) / 2;
			text_ys  = db_param.portrait_y_text;
			e_width  = 1.6f * xs;
			e_height = 0.12f;
		}
		else
		{
			xs       = scale * 3.0f;
			y1       = ty_l + 0.95f;
			y2       = y1   + 0.10f;
			y_loc    = (y1 + y2) / 2;
			text_ys  = db_param.landscape_y_text;
			e_width  = 1.6f * xs;
			e_height = 0.25f;
		}


	// ---- init gps lut
		FLOAT lac = (FLOAT)pi_model->obj_config->DBZ_TEXT_LABEL_ALIGNED_CENTRE;
		DbText dashboard_sys[4] =
		{
			{ -0.35f, y1, 0, text_ys, lac, "Time"       },
			{  0.35f, y1, 0, text_ys, lac, "Date"       },
			{ -0.35f, y2, 0, text_ys, lac, "00-00-00"   },
			{  0.35f, y2, 0, text_ys, lac, "00:00:0000" }
		};


	// ---- name and value
		for (UINT i = 0; i < 4; i++)
		{

			// ---- add
				pi_model->handle_dbz_cam_sys[i] = gfx->AddElement(
						pi_model->handle_dspl_dbz,
						"",
						0,
						0,
						&pi_model->obj_config->Dbz_Callback_Configure_Text
					);

				vs_system::ElmContext *elm = NULL;
				result = gfx->GetElementContext(pi_model->handle_dbz_cam_sys[i], &elm);


			// ---- set elm text
				DbText *dbz_sys = &dashboard_sys[i];
				elm->SetText(dbz_sys->_text);
				elm->SetRGBA(1, 1, 1, 1);


			// ---- set db
				FLOAT *p = elm->GetFloatParameter();
				result = pi_model->obj_config->Dbz_SetDefault(p);
				result = pi_model->obj_config->Dbz_SetConfig(p, dbz_sys->_config);
				result = pi_model->obj_config->Dbz_SetVisible(p, 0);
				result = pi_model->obj_config->Dbz_SetScale(p, dbz_sys->_size_x, dbz_sys->_size_y);
				result = pi_model->obj_config->Dbz_SetTranslate(p, dbz_sys->_trans_x * xs, dbz_sys->_trans_y);

		}


	// ---- enclose
		result = SetupCameraDashboardEnclose(&pi_model->handle_dbz_cam_sys[4], y_loc, e_width, e_height);

	return VS_OK;
}


// ---------- SetupCameraDashboardEnclose ----------
/*!
\brief Setup an enclose display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::SetupCameraDashboardEnclose(
		INT   *handle_elm,
		FLOAT  y_loc,
		FLOAT  width,
		FLOAT  height
	)
{

	// ---- local
		INT   result = 0;
	

	// ---- format
		vs_system::SysContext *sys_context = pi_model->app_dependency->GetSysContext();
		DWORD format = sys_context->GetDWord(vs_system::SysContext::FORMAT);


	// ---- add
		*handle_elm = gfx->AddElement(
				pi_model->handle_dspl_dbz,
				"",
				0,
				0,
				&pi_model->obj_config->Dbz_Callback_Configure_Rect
			);

		vs_system::ElmContext *elm = NULL;
		result = gfx->GetElementContext(*handle_elm, &elm);


	// ---- set elm
		elm->SetRGBA(1, 1, 1, 0.25f);


	// ---- set db
		FLOAT *p = elm->GetFloatParameter();
		result = pi_model->obj_config->Dbz_SetDefault(p);
		result = pi_model->obj_config->Dbz_SetConfig(p, 0);
		result = pi_model->obj_config->Dbz_SetVisible(p, 0);
		result = pi_model->obj_config->Dbz_SetScale(p, 1.0f, 1.0f);
		result = pi_model->obj_config->Dbz_SetTranslate(p, 0, y_loc);

		result = pi_model->obj_config->Dbz_SetRectDimension(p, width, height);
		result = pi_model->obj_config->Dbz_SetRectQuad(p,
				60,
				db_param.nclose_radius + db_param.nclose_width,
				db_param.nclose_radius
			);
		
		result = pi_model->obj_config->Dbz_SetRectRGBMax(p, db_param.c_r_min, db_param.c_g_min, db_param.c_b_min);
		result = pi_model->obj_config->Dbz_SetRectRGBMin(p, db_param.c_r_min, db_param.c_g_min, db_param.c_b_min);

	return VS_OK;
}


// ---------- UpdateCameraDbz ----------
/*!
\brief Update camera dashboard display lists
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::UpdateCameraDbz(FLOAT val[])
{

	// --- local : required for framerate f(rames)t(his)s(second)
		INT   result = VS_OK;
		FLOAT     *p = NULL;
		vs_system::ElmContext *elm = NULL;
		INT dial_bitmask = VS_ELM_SETUP_CONFIG | VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_INDEX;


	// ---- update fts every frame
		//
		// note: uses val[ Dbz_Cam_Index::ZERO], to flag "not a new second!
		//
		{
			INT result = gfx->GetElementContext(pi_model->handle_dbz_cam_dial[cam_dbz_dial_framerate_index + 4], &elm);
				p = elm->GetFloatParameter();
				FLOAT fts_angle = 270.0f / val[Dbz_Cam_Index::FRAMERATE_FPS] * (val[Dbz_Cam_Index::FRAMERATE_FTS] - 1);
				result = pi_model->obj_config->Dbz_SetDialAngle(p, 0, 270.0f / val[Dbz_Cam_Index::FRAMERATE_FPS]);
				result = pi_model->obj_config->Dbz_SetRotate(p, fts_angle + 90);
				result = gfx->SetElementConfigured(pi_model->handle_dbz_cam_dial[cam_dbz_dial_framerate_index + 4], FALSE, dial_bitmask);
				if ( val[Dbz_Cam_Index::ZERO] > 0 ) return VS_OK;
		}


	// ---- icon modes
		vs_system::CamProps *cam_props = NULL;
		result = GetCamProps(&cam_props);
		result = cam_props->GetProperty(
				vs_system::CamProps::SETUP,
				0, vs_system::CamProps::EXPOSURE_INDEX,
				&val[ICON_EXPOSURE_MODE]
			);
		result = cam_props->GetProperty(
				vs_system::CamProps::SETUP,
				0,
				vs_system::CamProps::FRAMERATE_INDEX,
				&val[ICON_FRAMERATE_MODE]
			);
		result = cam_props->GetProperty(
				vs_system::CamProps::SETUP,
				0,
				vs_system::CamProps::GAIN_MASTER_INDEX,
				&val[ICON_GAIN_MASTER_MODE]
			);


	// ---- not auto wb ? - auto gain mode ? then gain user mode
		FLOAT wb_mode = 0;
		result = cam_props->GetProperty(
				vs_system::CamProps::SETUP,
				0,
				vs_system::CamProps::GAIN_COLOUR_INDEX,
				&wb_mode
			);
		if ( wb_mode != 0 )
		{
			if ( val[ICON_GAIN_MASTER_MODE] == 0 )
				val[ICON_GAIN_MASTER_MODE] = 4;
		}


	// ---- update gps
		{
			if (cam_dbz_param[GPS_FLAG] > 0) cam_dbz_update[GPS_FLAG] = TRUE;
			for (UINT i = 0; i < 4; i++)
			{
				if (gps_data_str_text[i].compare("") != 0)
				{
					cam_dbz_update[GPS_LAT + i] = FALSE;
					if (cam_dbz_str_text[i].compare(gps_data_str_text[i]) != 0)
					{
						cam_dbz_str_text[i].assign(gps_data_str_text[i]);
						cam_dbz_update[GPS_LAT + i] = TRUE;
					}
				}
			}
		}


	// --- no change ?
		//
		// THIS IS WHERE THE STATE IS SET - OR NOT
		//
		for (UINT i = 0; i < Dbz_Cam_Index::GPS_FLAG; i++)
		{
			cam_dbz_update[i] = fabs((FLOAT)(cam_dbz_param[i] - val[i])) < 0.01f ? FALSE : TRUE;
			cam_dbz_param[i]  = val[i];
		}


	// --- local : required for everything else
		INT count = 0;
		INT needle_bitmask = VS_ELM_SETUP_GEOMETRY;


	// ------------ exposure ------------

		if ( cam_dbz_update[EXPOSURE_VAL] )
		{

			result = gfx->GetElementContext(pi_model->handle_dbz_cam_needle[0], &elm);
				p = elm->GetFloatParameter();
				result = pi_model->obj_config->Dbz_SetRotate(
						p,
						cam_dbz_param[Dbz_Cam_Index::EXPOSURE_VAL] - 90.0f
					);
				result = gfx->SetElementConfigured(
						pi_model->handle_dbz_cam_needle[0],
						FALSE,
						needle_bitmask
					);

			result = gfx->GetElementContext(pi_model->handle_dbz_cam_text[3], &elm);
				CHAR text[32];
				sprintf(text, "%d ms", (UINT)(cam_dbz_param[Dbz_Cam_Index::EXPOSURE_VAL] + 0.5f));
				result = elm->SetText(text);

			count++;

		}
		if ( cam_dbz_update[EXPOSURE_MIN] || cam_dbz_param[EXPOSURE_MAX] )
		{
			result = gfx->GetElementContext(pi_model->handle_dbz_cam_dial[cam_dbz_dial_exposure_index], &elm);
				p = elm->GetFloatParameter();
				FLOAT max = cam_dbz_param[
					Dbz_Cam_Index::EXPOSURE_MAX] > 270 ?
						270 : 
							cam_dbz_param[Dbz_Cam_Index::EXPOSURE_MAX
					];
				result = pi_model->obj_config->Dbz_SetDialAngle(
						p,
						cam_dbz_param[Dbz_Cam_Index::EXPOSURE_MIN],
						max
					);
				result = gfx->SetElementConfigured(
						pi_model->handle_dbz_cam_dial[cam_dbz_dial_exposure_index],
						FALSE,
						dial_bitmask
					);
			count++;
		}
		if ( cam_dbz_update[EXPOSURE_VAL] )
		{
			result = gfx->GetElementContext(pi_model->handle_dbz_cam_dial[cam_dbz_dial_exposure_index + 2], &elm);
				p = elm->GetFloatParameter();
				result = pi_model->obj_config->Dbz_SetDialAngle(
						p,
						0,
						cam_dbz_param[Dbz_Cam_Index::EXPOSURE_VAL]
					);
				result = gfx->SetElementConfigured(
						pi_model->handle_dbz_cam_dial[cam_dbz_dial_exposure_index + 2],
						FALSE,
						dial_bitmask
					);
			count++;
		}


	// ------------ framerate ------------

		if ( cam_dbz_update[Dbz_Cam_Index::FRAMERATE_FLS] )
		{
			result = gfx->GetElementContext(pi_model->handle_dbz_cam_needle[1], &elm);
				p = elm->GetFloatParameter();
				FLOAT needle_angle = 270.0f /
					cam_dbz_param[Dbz_Cam_Index::FRAMERATE_FPS] *
						cam_dbz_param[Dbz_Cam_Index::FRAMERATE_FLS] - 90.0f;
				result = pi_model->obj_config->Dbz_SetRotate(p, needle_angle);
				result = gfx->SetElementConfigured(pi_model->handle_dbz_cam_needle[1], FALSE, needle_bitmask);

			result = gfx->GetElementContext(pi_model->handle_dbz_cam_dial[cam_dbz_dial_framerate_index + 2], &elm);
				p = elm->GetFloatParameter();
				FLOAT fps_angle = 270.0f /
					cam_dbz_param[Dbz_Cam_Index::FRAMERATE_FPS] *
						cam_dbz_param[Dbz_Cam_Index::FRAMERATE_FLS];
				fps_angle = fps_angle > 270 ? 270 : fps_angle;
				result = pi_model->obj_config->Dbz_SetDialAngle(p, 0, fps_angle);
				result = gfx->SetElementConfigured(pi_model->handle_dbz_cam_dial[cam_dbz_dial_framerate_index + 2], FALSE, dial_bitmask);

			result = gfx->GetElementContext(pi_model->handle_dbz_cam_dial[cam_dbz_dial_framerate_index + 3], &elm);
				p = elm->GetFloatParameter();
				result = pi_model->obj_config->Dbz_SetDialBlipCount(p,
						cam_dbz_param[Dbz_Cam_Index::FRAMERATE_FPS]);
				result = gfx->SetElementConfigured(pi_model->handle_dbz_cam_dial[cam_dbz_dial_framerate_index + 3], FALSE, dial_bitmask);

			result = gfx->GetElementContext(pi_model->handle_dbz_cam_text[4], &elm);
				CHAR text[32];
				sprintf(text, "%d fps", (UINT)(cam_dbz_param[Dbz_Cam_Index::FRAMERATE_FLS] + 0.5f));
				result = elm->SetText(text);

				count++;
		}
		if ( cam_dbz_update[Dbz_Cam_Index::FRAMERATE_SECOND] )
		{
			result = gfx->GetElementContext(pi_model->handle_dbz_cam_dial[cam_dbz_dial_framerate_index], &elm);
				p = elm->GetFloatParameter();
				FLOAT seconds_angle = 270.0f / 60.0f * cam_dbz_param[Dbz_Cam_Index::FRAMERATE_SECOND];
				result = pi_model->obj_config->Dbz_SetDialAngle(p, 0, seconds_angle);
				result = gfx->SetElementConfigured(pi_model->handle_dbz_cam_dial[cam_dbz_dial_framerate_index], FALSE, dial_bitmask);
				count++;
		}


	// ------------ gain master ------------

		if ( cam_dbz_update[Dbz_Cam_Index::GAIN_MASTER] )
		{

			result = gfx->GetElementContext(pi_model->handle_dbz_cam_needle[2], &elm);
				p = elm->GetFloatParameter();
				FLOAT master_angle = 270.0f * (cam_dbz_param[Dbz_Cam_Index::GAIN_MASTER] / 100.0f) - 90.0f;
				result = pi_model->obj_config->Dbz_SetRotate(p, master_angle);
				result = gfx->SetElementConfigured(pi_model->handle_dbz_cam_needle[2], FALSE, needle_bitmask);

			result = gfx->GetElementContext(pi_model->handle_dbz_cam_dial[cam_dbz_dial_gain_wb_index + 4], &elm);
				p = elm->GetFloatParameter();
				master_angle = 270.0f * (cam_dbz_param[Dbz_Cam_Index::GAIN_MASTER] / 100.0f);
				result = pi_model->obj_config->Dbz_SetDialAngle(p, 0, master_angle);
				result = gfx->SetElementConfigured(pi_model->handle_dbz_cam_dial[cam_dbz_dial_gain_wb_index + 4], FALSE, dial_bitmask);

			result = gfx->GetElementContext(pi_model->handle_dbz_cam_text[5], &elm);
				CHAR text[32];
				sprintf(text, "%d%c", (UINT)(cam_dbz_param[Dbz_Cam_Index::GAIN_MASTER] + 0.5f), '%');
				result = elm->SetText(text);

				count++;
		}


	// ------------ gain wb ------------

		if (cam_dbz_update[Dbz_Cam_Index::GAIN_WB_RED])
		{
			result = gfx->GetElementContext(pi_model->handle_dbz_cam_dial[cam_dbz_dial_gain_wb_index], &elm);
			p = elm->GetFloatParameter();
			FLOAT red_angle = 80.0f * (cam_dbz_param[Dbz_Cam_Index::GAIN_WB_RED] / 100.0f) + 10.0f;
			result = pi_model->obj_config->Dbz_SetDialAngle(p, 0, red_angle);
			result = pi_model->obj_config->Dbz_SetRotate(p, 270);
			result = gfx->SetElementConfigured(pi_model->handle_dbz_cam_dial[cam_dbz_dial_gain_wb_index], FALSE, dial_bitmask);
			count++;
		}
		if (cam_dbz_update[Dbz_Cam_Index::GAIN_WB_GREEN])
		{
			result = gfx->GetElementContext(pi_model->handle_dbz_cam_dial[cam_dbz_dial_gain_wb_index + 1], &elm);
			p = elm->GetFloatParameter();
			FLOAT green_angle = 80.0f * (cam_dbz_param[Dbz_Cam_Index::GAIN_WB_GREEN] / 100.0f) + 10.0f;
			result = pi_model->obj_config->Dbz_SetDialAngle(p, 0, green_angle);
			result = pi_model->obj_config->Dbz_SetRotate(p, 180);
			result = gfx->SetElementConfigured(pi_model->handle_dbz_cam_dial[cam_dbz_dial_gain_wb_index + 1], FALSE, dial_bitmask);
			count++;
		}
		if (cam_dbz_update[Dbz_Cam_Index::GAIN_WB_BLUE])
		{
			result = gfx->GetElementContext(pi_model->handle_dbz_cam_dial[cam_dbz_dial_gain_wb_index + 2], &elm);
			p = elm->GetFloatParameter();
			FLOAT blue_angle = 80.0f * (cam_dbz_param[Dbz_Cam_Index::GAIN_WB_BLUE] / 100.0f) + 10.0f;
			result = pi_model->obj_config->Dbz_SetDialAngle(p, 0, blue_angle);
			result = pi_model->obj_config->Dbz_SetRotate(p, 90);
			result = gfx->SetElementConfigured(pi_model->handle_dbz_cam_dial[cam_dbz_dial_gain_wb_index + 2], FALSE, dial_bitmask);
			count++;
		}


	// ------------ icons ------------ 

		INT icon_mode[] = 
			{
				ICON_EXPOSURE_MODE,
				ICON_FRAMERATE_MODE,
				ICON_GAIN_MASTER_MODE
			};

		// atlas page group, row and column
		FLOAT apg[6] = { 3, 1, 1, 1, 3, 0 };
		FLOAT apr[6] = { 6, 3, 3, 3, 6, 0 };
		FLOAT apc[6] = { 0, 5, 6, 7, 4, 0 };

		for ( UINT i = 0; i < 3; i++ )
		{
			if ( cam_dbz_update[icon_mode[i]] )
			{
				FLOAT g = apg[ (INT)cam_dbz_param[icon_mode[i]] ];
				FLOAT r = apr[ (INT)cam_dbz_param[icon_mode[i]] ];
				FLOAT c = apc[ (INT)cam_dbz_param[icon_mode[i]] ];

				result = gfx->GetElementContext(pi_model->handle_dbz_cam_icon[i], &elm);
					p = elm->GetFloatParameter();
					result = pi_model->obj_config->Dbz_SetIconTexture(
							p,
							g, r, c,
							1,
							1,
							0.001f
						);
					result = gfx->SetElementConfigured(
							pi_model->handle_dbz_cam_icon[i],
							FALSE,
							VS_ELM_SETUP_MAPPING
						);
					gfx->SetConfigure(FALSE, pi_model->handle_dbz_cam_icon[i]);
					count++;
			}
		}


	// ---- tidy up: ' ' & '0' and '-' with ' '
		auto charTidyUp = [](
				CHAR *str
			)
		{
			UINT len = (UINT)strlen(str);
			for (UINT i = 0; i < len; i++)
			{
				str[i] = str[i] == ' ' ? '0' :
					(str[i] == '_' ? ' ' : str[i]);
			}
			return VS_OK;
		};


	// ------------ gps ------------

		if ( cam_dbz_update[GPS_FLAG] )
		{
		
			db_param.gps_mode = TRUE;

			result = gfx->GetElementContext(pi_model->handle_dbz_cam_gps[8], &elm);
			result = elm->SetVisible(TRUE);

			char *gps_label[4] =
			{
				"Latitude",
				"Longitude",
				"Time",
				"Date",
			};
			CHAR text[32];
			for (UINT i = 0; i < 2; i++)
			{
				BOOL def = cam_dbz_update[GPS_LAT + i];
				{
					result = gfx->GetElementContext(pi_model->handle_dbz_cam_gps[i], &elm);
						result = elm->SetText(gps_label[i]);
						elm->SetRGBA(db_param.c_r_max, db_param.c_g_max, db_param.c_b_max, 1.0f);
						result = elm->SetVisible(TRUE);

					result = gfx->GetElementContext(pi_model->handle_dbz_cam_gps[i + 4], &elm);
						sprintf(text, "%s", cam_dbz_str_text[i].c_str());
						result = elm->SetText(text);
						elm->SetRGBA(1, 1, 1, 1);
						result = elm->SetVisible(TRUE);

					count++;
				}
			}

			// ---- timestamp
				SYSTEMTIME SystemTime;
				GetLocalTime(&SystemTime);

				CHAR time[32];
				sprintf(
					time,
						"%2d:%2d:%2d",
						(INT)SystemTime.wHour,
						(INT)SystemTime.wMinute,
						(INT)SystemTime.wSecond
					);
					charTidyUp(time);

				CHAR date[32];
				sprintf(
						date,
						"%2d-%2d-%4d",
						(INT)SystemTime.wDay,
						(INT)SystemTime.wMonth,
						(INT)SystemTime.wYear
					);
					charTidyUp(date);

				char *sys_label[4] =
					{
						"Time",
						"Date",
						time,
						date
					};

				for (UINT i = 2; i < 4; i++)
				{
					{
						result = gfx->GetElementContext(pi_model->handle_dbz_cam_gps[i], &elm);
							result = elm->SetText(sys_label[i-2]);
							elm->SetRGBA(db_param.c_r_max, db_param.c_g_max, db_param.c_b_max, 1.0f);
							result = elm->SetVisible(TRUE);

						result = gfx->GetElementContext(pi_model->handle_dbz_cam_gps[i + 4], &elm);
							result = elm->SetText(sys_label[i]);
							elm->SetRGBA(1, 1, 1, 1);
							result = elm->SetVisible(TRUE);

						count++;
					}
				}
		}

	
	// ------------ system ------------

		if ( db_param.gps_mode != TRUE )
		{
			 
				result = gfx->GetElementContext(pi_model->handle_dbz_cam_sys[4], &elm);
				result = elm->SetVisible(TRUE);


			// ---- timestamp
				SYSTEMTIME SystemTime;
				GetLocalTime(&SystemTime);

				CHAR time[32];
				sprintf(
					time,
						"%2d:%2d:%2d",
						(INT)SystemTime.wHour,
						(INT)SystemTime.wMinute,
						(INT)SystemTime.wSecond
					);
					charTidyUp(time);

				CHAR date[32];
				sprintf(
						date,
						"%2d-%2d-%4d",
						(INT)SystemTime.wDay,
						(INT)SystemTime.wMonth,
						(INT)SystemTime.wYear
					);
					charTidyUp(date);

				char *sys_label[4] =
					{
						"Time",
						"Date",
						time,
						date
					};

				for (UINT i = 0; i < 2; i++)
				{
					BOOL def = cam_dbz_update[GPS_LAT + i];
					{
						result = gfx->GetElementContext(pi_model->handle_dbz_cam_sys[i], &elm);
							result = elm->SetText(sys_label[i]);
							elm->SetRGBA(db_param.c_r_max, db_param.c_g_max, db_param.c_b_max, 1.0f);
							result = elm->SetVisible(TRUE);

						result = gfx->GetElementContext(pi_model->handle_dbz_cam_sys[i + 2], &elm);
							result = elm->SetText(sys_label[i+2]);
							elm->SetRGBA(1, 1, 1, 1);
							result = elm->SetVisible(TRUE);

						count++;
					}
				}
		}

	return VS_OK;
} 
