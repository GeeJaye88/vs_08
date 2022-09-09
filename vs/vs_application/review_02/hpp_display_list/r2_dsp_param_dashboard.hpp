////////////////////////////////////////////////////////////////////////////////

// ---------- r2_dl_param_dashboard.hpp ----------
/*!
\file r2_dl_param_dashboard.hpp
\brief 
\author Gareth Edwards
*/

#include "../header/r2_model.h"

using namespace review_02;


// ---- dial init lut
	struct DashboardParam
	{

		FLOAT global_scale = 0.5f;

		FLOAT c_r_max = 0.15f;
		FLOAT c_g_max = 0.4f;
		FLOAT c_b_max = 1.0f;
		FLOAT c_r_min = c_r_max / 2;
		FLOAT c_g_min = c_g_max / 2;
		FLOAT c_b_min = c_b_max / 2;

		FLOAT m_fg    = 1.0f;
		FLOAT m_bg    = 0.2f;
		FLOAT m_blip  = 0.8f;

		FLOAT dial_tx_landscape    =  1.00f;
		FLOAT dial_ty_l_landscape  = -0.25f;
		FLOAT dial_ty_h_landscape  = -0.25f;

		FLOAT dial_tx_portrait     =  0.30f;
		FLOAT dial_ty_l_portrait   = -0.60f;
		FLOAT dial_ty_h_portrait   =  0.20f;

		FLOAT portrait_y_text      = 0.06f;
		FLOAT landscape_y_text     = 0.1f;

		FLOAT dial_scale           =  0.35f;

		FLOAT nclose_radius = 0.05f;
		FLOAT nclose_width  = 0.006f;

		BOOL  gps_mode = FALSE;

	} db_param;


