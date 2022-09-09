////////////////////////////////////////////////////////////////////////////////

// ---------- r2_setup_panel_props.hpp ----------
/*!
\file r2_setup_panel_props.hpp
\brief
\author Gareth Edwards
*/


#include "../header/r2_model.h"


struct PanelProps
{

	// ---- panel
		BOOL  panels_pano     = TRUE;
		BOOL  panels_timeline = TRUE;
		BOOL  panels_settings = TRUE;

	// ---- frame
		FLOAT frame_prop_border = 0;
		FLOAT frame_width       = 0;
		FLOAT frame_height      = 0;
		FLOAT frame_aspect      = 0;
		BOOL  frame_portrait    = FALSE;

	// ---- image
		FLOAT image_aspect      = 1.0f;

	// ---- Y
		FLOAT y_prop_space           = 0.005f;
		FLOAT y_prop_border          = y_prop_space * frame_prop_border;
		FLOAT y_title_top            = 0;
		FLOAT y_title_bottom         = 0;
		FLOAT y_timeline_top         = 0;
		FLOAT y_timeline_bottom      = 0;
		FLOAT y_navigation_top       = 0;
		FLOAT y_navigation_bottom    = 0;
		FLOAT y_prop_content_top     = 0;
		FLOAT y_prop_content_bottom  = 0;
		FLOAT y_prop_content_height  = 0;
		FLOAT y_prop_panorama        = 0;

	// ---- X
		FLOAT x_prop_space           = 0;
		FLOAT x_prop_left            = 0;
		FLOAT x_prop_right           = 0;
		FLOAT x_prop_width           = 0;
		FLOAT x_prop_panel_asp_width = 0; 
		FLOAT x_prop_panel_asp_left  = 0;
		FLOAT x_prop_panel_asp_right = 0;

	// ---- dimensions
		FLOAT t3d_big_height   = 0;
		FLOAT t3d_big_width    = 0;
		FLOAT t3d_small_height = 0;
		FLOAT t3d_small_width  = 0;

	// ---- 6 x interface
		VsFloatRectangle title;
		VsFloatRectangle frames;
		VsFloatRectangle context;
		VsFloatRectangle timebar;
		VsFloatRectangle clipbar;
		VsFloatRectangle navig;

	// ---- 11 ( 8 + multiple ) x pano with multiples
		VsFloatRectangle ann_big;
		VsFloatRectangle ann_small;
		VsFloatRectangle t3d_big;
		VsFloatRectangle t3d_full;
		VsFloatRectangle t3d_small;
		VsFloatRectangle t2d_all;
		VsFloatRectangle t2d_front;
		VsFloatRectangle t2d_back;

	// ---- 2 x photo
		VsFloatRectangle photo_full;
		VsFloatRectangle photo_config;

	// ---- 11 x gui
		VsFloatRectangle monitor;
		VsFloatRectangle nest;
		VsFloatRectangle gpsdata;
		VsFloatRectangle camhead;
		VsFloatRectangle cambody;
		VsFloatRectangle imghead;
		VsFloatRectangle imgbody;
		VsFloatRectangle imgnav;
		VsFloatRectangle progbar;
		VsFloatRectangle dashbrd;
		VsFloatRectangle specbrd;

	// ---- 1 x about
		VsFloatRectangle about;

	// ---- 3 x media
		VsFloatRectangle strm_hid;
		VsFloatRectangle strm_vis;
		VsFloatRectangle strm;

	// ---- 1 x shared
		VsFloatRectangle shared;

	// ---- "nest"
		VsFloatRectangle quad1[4];
		VsFloatRectangle quad2[4];
		VsFloatRectangle nine[9];
		VsFloatRectangle ptf_img[vs_system::ObjConfig::MAX_PANO_PTF_LUT];
		VsFloatRectangle h1[16];

};