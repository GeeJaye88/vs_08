////////////////////////////////////////////////////////////////////////////////

// ---------- r2_control_update_elem_pano.hpp ----------
/*!
\file r2_control_update_elem_pano.hpp
\brief 
\author Gareth Edwards
*/


#include "../header/r2_model.h"

using namespace review_02;


// ---------- UpdateElemPano ----------
/*!
\brief Update panoramic panel
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < VS_OK
*/
INT Control::UpdateElemPano(
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
		Model                    *model          = control->model;
		vs_system::ObjConfig     *obj_config     = model->GetObjConfig();


	// ---- get client adjusted viewport
		VsFloatRectangle cav = { 0, 0, 0, 0 };
		gfx_interface->GetClientAdjustedViewport(&cav);


	// ---- no config bitmask
		BOOL set_config_bitmask = FALSE;
		BYTE setup_bitmask = VS_ELM_SETUP_NONE;


	// ---- store
		obj_config->SetPanoPTFLast();
		BOOL animate_pt = FALSE;


	// ---- select
	switch ( panel_group_id )
	{

		case GROUP_ELEM_PANO_FULL:
		case GROUP_ELEM_PANO_FRONT:
		case GROUP_ELEM_PANO_BACK:
			{
				setup_bitmask = (BYTE)UpdateElemPanoPanoramaPanTilt(
						control,
						app_dependency,
						&cav,
						panel_group_id
					);
				set_config_bitmask = TRUE;
			}
			break;

		case GROUP_ELEM_PANO_ANNULAR:
			{
				setup_bitmask = (BYTE)UpdateElemPanoAnnularPanTilt(
						control,
						app_dependency,
						&cav,
						panel_group_id
					);
				set_config_bitmask = TRUE;
			}
			break;

		// +++ linked ptf ++++
		case GROUP_ELEM_PANO_SPHERICAL:
			{
				setup_bitmask = (BYTE)UpdateElemPanoSphericalPanTilt(
						control,
						app_dependency,
						&cav,
						panel_group_id
					);
				set_config_bitmask = TRUE;
				animate_pt = TRUE;
			}
			break;

		case GROUP_ELEM_PANO_CYLIDRICAL:
			{
				setup_bitmask = (BYTE)UpdateElemPanoCylinderPanTilt(
						control,
						app_dependency,
						&cav,
						panel_group_id
					);
				set_config_bitmask = TRUE;
			}
			break;

		// ++++ non-linked PTF ++++
		case GROUP_ELEM_PANO_SPH_SOLO:
			{
				// Important:
				//
				// IF panel is NOT NULL THEN set persistant context ELSE use
				// persistant context, as subsequent calls will be invoked
				// from gfx interface, without panel context!!!
				//
				if ( app_panel != NULL )
				{
					app_control->persistant_panel_context = panel_context;
					INT result = gfx_interface->GetClientAdjustedViewport(&control->app_control->persistant_cav);
				}

				// update model pan tilt AND fov
				setup_bitmask = (BYTE)UpdateElemPanoSphericalSoloPanTilt(
						control,
						app_dependency,
						app_control->persistant_panel_context,
						&app_control->persistant_cav,
						FALSE,
						FALSE
					);
				animate_pt = TRUE;
			}
			break;

		default:
			break;
	}


	// ---- animate pan and tilt
		if ( animate_pt )
		{
			FLOAT pan_last, tilt_last, fov_last;
			FLOAT pan_now, tilt_now, fov_now;

			obj_config->GetPanoPTFLast(&pan_last, &tilt_last, &fov_last);
			obj_config->GetPanoPTF(&pan_now, &tilt_now, &fov_now);

			// catch "long way" animation
			FLOAT pan_total = pan_now - pan_last;
			if ( fabs(pan_total) > 180 )
			{
				if ( pan_now > 0 )
				{
					pan_last = 360 + pan_last;
				}
				else
				{
					pan_last = -360 + pan_last;
				}
				obj_config->SetPanoPTFLast(pan_last, tilt_last, fov_last);
			}

			// duration proportional to pt difference
			FLOAT pan_diff = pan_now - pan_last;
			FLOAT tilt_diff = tilt_now - tilt_last;
			FLOAT dist = sqrt(pan_diff * pan_diff + tilt_diff * tilt_diff);

			// start timer
			vs_system::SysContext *sys_context = app_dependency->GetSysContext();
			sys_context->StartTimer(dist * 20);

		}


	// ---- set config bitmask
		if ( set_config_bitmask )
		{

			// IF -- pan 2d vertex buffers AND setup bitmask NOT none -- THEN
			if ( setup_bitmask != VS_ELM_SETUP_NONE )
			{

				// IF -- pan vertex buffer -- THEN -- set all "2D" prefixed elements to be configured
				BOOL pan_2d_vertex_buffer;
				INT result = obj_config->GetPanoPanVertexBuffer(&pan_2d_vertex_buffer);
				if ( pan_2d_vertex_buffer )
				{
					result = gfx_interface->SetGlobalConfigureBitmask(
							setup_bitmask,
							"2D"
						);
				}
				// ELSE -- set only the "2D PanoCylinder"named element to be configured
				else
				{
					result = gfx_interface->SetGlobalConfigureBitmask(
							setup_bitmask,
							"2D PanoCylinder"
						);
				}

			}


		// ---- reset mouse and zap context (if any)
			sys_input->ResetMouse();
			app_control->persistant_panel_context = NULL;

	}

	return VS_OK;
}


// ---------- UpdateElemPanoSphericalSoloPanTilt ----------
/*!
\brief Mouse down on panel to update model panoramic Pan, Tilt and FOV values
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Control::UpdateElemPanoSphericalSoloPanTilt(
		Control *control,
		vs_system::AppDependency *app_dependency,
		vs_system::PanelContext *panel_context,
		VsFloatRectangle *gfx_cav,
		BOOL model_ptz,
		BOOL only_pan_and_reset
	)
{

	// ---- four steps:
	//   1. initialise local
	//   2. get either model or viepwort ptz parameters
	//   3. update ptz parameters
	//   4. set either model or viepwort ptz parameters


	// ---- step 1: Initialise local variables


	// ---- local
		vs_system::AppControl *app_control = control->app_control;
		vs_system::SysContext *sys_context = app_dependency->GetSysContext();
		vs_system::SysOutput  *sys_output  = app_dependency->GetSysOutput();
		vs_system::SysInput   *sys_input   = app_dependency->GetSysInput();


	// ---- setup bitmask
		BYTE setup_bitmask = VS_ELM_SETUP_NONE;
		BYTE setup_update = VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING;


	// ---- step 2: Get either model or viepwort ptz parameters


	// IF use model panoramic object PTF parameters THEN
		FLOAT *perspective = panel_context->GetPerspective();
		FLOAT *rotate = panel_context->GetRotate();
		FLOAT pan, tilt, zoom;
		pan  = rotate[1];
		tilt = rotate[0];
		zoom = perspective[0];


	// ---- delta
		FLOAT delta = 1.0f;


	// ---- ZOOM
		FLOAT zoom_min = 10;
		FLOAT zoom_max = 70;
		FLOAT zoom_scalar = (zoom - zoom_min) / (zoom_max - zoom_min) * 0.8f + 0.2f;


	// ----  step 3:  Update ptz parameters


	// ---- IF wheel clicked THEN
	LONG clicks = sys_input->GetLong(sys_input->WHEEL_CLICKS);
	if ( clicks != 0 && !only_pan_and_reset )
	{
		// ---- get interaction status
			BOOL interaction_status = TRUE;
			INT result = control->app_view->GetPanelInteractionStatus(NULL, &interaction_status);
			clicks = interaction_status ? clicks : 0;

		// ---- update FOV
			zoom += (FLOAT)app_control->wheel_clicks * delta;
			app_control->persistant_panel_context = NULL;
			sys_input->SetLong(sys_input->WHEEL_CLICKS, 0);
	}

	// ---- ELSE pick
	else
	{

		// ---- decide action
			POINT pick_point = { 0, 0 };
			pick_point = *sys_input->GetPosition();
			FLOAT x_pick = (FLOAT)pick_point.x - gfx_cav->left;
			FLOAT y_pick = (FLOAT)pick_point.y - gfx_cav->top;
			FLOAT width  = gfx_cav->right - gfx_cav->left;
			FLOAT height = gfx_cav->bottom - gfx_cav->top;

		// IF normal then ptz (9) actions OR only_pan_and_reset (3) actions
			UINT action = PTF_RESET;
			if ( only_pan_and_reset )
				action  = (UINT)( x_pick/(width/3) ) + 3;
			else
				action  = (UINT)( y_pick/(height/3) ) * 3 + (UINT)( x_pick/(width/3) );

		// ---- select
			switch ( action + 1)
			{
				case PTF_ZOOM_IN:
					{
						zoom -= delta;
					}
					break;
				case PTF_TILT_UP:
					{
						tilt += zoom_scalar;
					}
					break;
				case PTF_ZOOM_OUT:
					{
						zoom += delta;
					}
					break;
				case PTF_PAN_LEFT:
					{
						pan += zoom_scalar;
						setup_bitmask = setup_update;
					}
					break;
				case PTF_RESET:
					{
						if ( model_ptz )
						{
							control->model->GetObjConfig()->SetPanoDefault();
						}
						else
						{
							panel_context->DefaultPerspective(0);
							panel_context->DefaultRotate();
						}
						setup_bitmask = setup_update;
						return (INT)setup_bitmask;
					}
					break;
				case PTF_PAN_RIGHT:
					{
						pan -= zoom_scalar;
						setup_bitmask = setup_update;
					}
					break;
				case PTF_TILT_DOWN:
					{
						tilt -= zoom_scalar;
					}
					break;
				default:
					break;
			}

	}


	// ----  step 4: Set ptf parameters
		rotate[1] = pan;
		rotate[0] = tilt;
		perspective[0] = zoom;

	return (INT)setup_bitmask;
}


// ---------- UpdateElemPanoAnnularPanTilt ----------
/*!
\brief Pick on annular panel to update model panoramic Pan and Tilt values
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Control::UpdateElemPanoAnnularPanTilt(
		Control *control,
		vs_system::AppDependency *app_dependency,
		VsFloatRectangle *gfx_cav,
		UINT id
	)
{

	// ---- local
		vs_system::AppControl *app_control = control->app_control;
		vs_system::SysInput   *sys_input   = app_dependency->GetSysInput();
		Model                 *model       = control->model;
		vs_system::ObjConfig  *obj_config  = model->GetObjConfig();


	// ---- PT parameters
		FLOAT pan, tilt, zoom;
		obj_config->GetPanoPTF(&pan, &tilt, &zoom);


	// ---- angle parameters
		FLOAT min_angle, max_angle;
		obj_config->GetPanoAngles(&min_angle, &max_angle);


	// ---- radius parameters
		FLOAT min_radius, max_radius;
		obj_config->GetPanoRadii(&min_radius, &max_radius);


	// ---- pick with CAV
		POINT pick_point = { 0, 0 };
		pick_point = *sys_input->GetPosition();
		FLOAT x_pick = (FLOAT)pick_point.x - gfx_cav->left;
		FLOAT y_pick = (FLOAT)pick_point.y - gfx_cav->top;
		FLOAT width = gfx_cav->right - gfx_cav->left;
		FLOAT height = gfx_cav->bottom - gfx_cav->top;
		FLOAT x_cen = width / 2;
		FLOAT y_cen = height / 2;


	// ---- offset
		FLOAT t1x = x_pick - x_cen;
		FLOAT t1y = y_pick - y_cen;


	// ---- catch picks outside annular range
		FLOAT dist = sqrt(t1x*t1x + t1y*t1y) / y_cen;
		if (dist < min_radius) return VS_ELM_SETUP_NONE;
		if (dist > max_radius) return VS_ELM_SETUP_NONE;


	// ---- pan angle
		FLOAT pan_angle = (FLOAT)atan2(t1y, t1x) * (FLOAT)VS_RADIAN + 90.0f;


	// ---- pan
		FLOAT new_pan = pan_angle;


	// IF pan 2d vertex buffers AND setup bitmask NOT none THEN
		BOOL pan_2d_vertex_buffer;
		INT result = obj_config->GetPanoPanVertexBuffer(&pan_2d_vertex_buffer);

		if ( pan_2d_vertex_buffer )
		{
			new_pan = new_pan - pan;
			new_pan = new_pan < 0 ? new_pan + 360.0f : new_pan > 360.0f ? new_pan - 360.0f : new_pan;
		}
		else
		{
			FLOAT pan_offset;
			INT result = obj_config->GetPanoPanOffset(&pan_offset);
			new_pan += pan_offset;
		}


	// ---- pitch
		FLOAT new_tilt = float(sqrt(t1x*t1x+t1y*t1y)) / (height/2);
		new_tilt = (new_tilt - min_radius) / (max_radius - min_radius);
		new_tilt = new_tilt > 1 ? 1 : new_tilt < 0 ? 0 : new_tilt;


	// ---- map into angle
		new_tilt *= max_angle - min_angle;
		new_tilt += min_angle;


	// ---- set
		obj_config->SetPanoPTF(-new_pan, new_tilt, zoom);

	return (INT)VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING;
}


// ---------- UpdateElemPanoPanoramaPanTilt ----------
/*!
\brief Pick on panorama panel to update model panoramic Pan and Tilt values
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Control::UpdateElemPanoPanoramaPanTilt(
		Control *control,
		vs_system::AppDependency *app_dependency,
		VsFloatRectangle *gfx_cav,
		UINT id
	)
{

	// ---- local
		vs_system::AppControl *app_control = control->app_control;
		vs_system::SysInput   *sys_input   = app_dependency->GetSysInput();
		Model                 *model       = control->model;
		vs_system::ObjConfig  *obj_config  = model->GetObjConfig();


	// ---- PTF parameters
		FLOAT pan, tilt, fov;
		obj_config->GetPanoPTF(&pan, &tilt, &fov);


	// ---- angle parameters
		FLOAT min_angle, max_angle;
		obj_config->GetPanoAngles(&min_angle, &max_angle);


	// ---- pick with CAV
		POINT pick_point = { 0, 0 };
		pick_point = *sys_input->GetPosition();
		FLOAT x_pick = (FLOAT)pick_point.x - gfx_cav->left;
		FLOAT y_pick = (FLOAT)pick_point.y - gfx_cav->top;
		FLOAT width = gfx_cav->right - gfx_cav->left;
		FLOAT height = gfx_cav->bottom - gfx_cav->top;


	// ---- proportional x
		FLOAT prop_x = x_pick / width;
		switch (id)
		{
			case GROUP_ELEM_PANO_FRONT:
				prop_x = prop_x / 2 + 0.25f;
				break;
			case GROUP_ELEM_PANO_BACK:
				prop_x = prop_x / 2 + 0.75f;
				break;
			default:
				break;
		}


	// ---- new pan
		FLOAT new_pan = prop_x < 0.5f ? (0.5f - prop_x) * 360.0f : -(prop_x - 0.5f) * 360.0f;


	// ---- IF pan 2d vertex buffers AND setup bitmask NOT none THEN
		BOOL pan_2d_vertex_buffer;
		INT result = obj_config->GetPanoPanVertexBuffer(&pan_2d_vertex_buffer);

		if ( pan_2d_vertex_buffer )
		{
			new_pan += pan;
			new_pan = new_pan < 0 ? 360 + new_pan : new_pan;
		}
		else
		{
			FLOAT pan_offset;
			INT result = obj_config->GetPanoPanOffset(&pan_offset);
			new_pan -= pan_offset;
		}


	// ---- new tilt
		FLOAT new_tilt = 1 - (y_pick / height);
		new_tilt *= max_angle - min_angle;
		new_tilt += min_angle;


	// ---- set
		obj_config->SetPanoPTF(new_pan, new_tilt, fov);

	return (INT)VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING;
}


// ---------- UpdateElemPanoSphericalPanTilt ----------
/*!
\brief Pick on spherical panel to update model panoramic Pan and Tilt values
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Control::UpdateElemPanoSphericalPanTilt(
		Control *control,
		vs_system::AppDependency *app_dependency,
		VsFloatRectangle *gfx_cav,
		UINT id
	)
{

	// ---- local
		vs_system::AppControl *app_control = control->app_control;
		vs_system::SysInput   *sys_input   = app_dependency->GetSysInput();
		Model                 *model       = control->model;
		vs_system::ObjConfig  *obj_config  = model->GetObjConfig();


	// ---- PTF parameters
		FLOAT pan, tilt, fov;
		obj_config->GetPanoPTF(&pan, &tilt, &fov);


	// ---- angle parameters
		FLOAT min_angle, max_angle;
		obj_config->GetPanoAngles(&min_angle, &max_angle);


	// ---- pick with CAV
		POINT pick_point = { 0, 0 };
		pick_point = *sys_input->GetPosition();
		FLOAT x_pick = (FLOAT)pick_point.x - gfx_cav->left;
		FLOAT y_pick = (FLOAT)pick_point.y - gfx_cav->top;
		FLOAT width  = gfx_cav->right - gfx_cav->left;
		FLOAT height = gfx_cav->bottom - gfx_cav->top;
		FLOAT wby2   = width / 2;
		FLOAT hby2   = height / 2;
		FLOAT aspect = width / height;

	// ----  frustrum dimensions; x, y, z and radius
		FLOAT frus_z = 0.5;
		FLOAT frus_y = frus_z * tan(fov / 2 / 180 * (FLOAT)VS_PI);
		FLOAT frus_x = frus_y * aspect;
		FLOAT frus_r = sqrt(frus_x*frus_x + frus_y*frus_y + frus_z*frus_z);


	// ----  frustrum pick; x, y, z, radius and scalar
		FLOAT frus_x_p = (x_pick - wby2) / wby2 * frus_x;
		FLOAT frus_y_p = (y_pick - hby2) / hby2 * frus_y;
		FLOAT frus_z_p = frus_z;
		FLOAT frus_r_p = sqrt(frus_x_p*frus_x_p + frus_y_p*frus_y_p + frus_z_p*frus_z_p);
		FLOAT frus_s_p = frus_z / frus_r_p;


	// ----  frustrum spherical pick; x, y, z
		FLOAT frus_x_sp = frus_x_p * frus_s_p;
		FLOAT frus_y_sp = frus_y_p * frus_s_p;
		FLOAT frus_z_sp = frus_z_p * frus_s_p;


	// ---- frustrum PT
		FLOAT frus_pan = atan2(frus_x_sp, frus_z_sp);
		FLOAT frus_tilt = atan2(frus_y_sp, frus_z_sp);


	// ---- convert to degrees
		frus_pan *= (FLOAT)VS_RADIAN;
		frus_tilt *= (FLOAT)VS_RADIAN;


	// ---- adjust pan
		frus_pan = (-frus_pan ) + pan;
		frus_pan = frus_pan < 0 ? 360 + frus_pan : frus_pan;


	// ---- adjust tilt
		frus_tilt = (-frus_tilt) + tilt;


	// ---- set PTF
		obj_config->SetPanoPTF(frus_pan, frus_tilt, fov);

	return (INT)VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING;
}


// ---------- UpdateElemPanoCylinderPanTilt ----------
/*!
\brief Pick on cylidrical panel to update model panoramic Pan and Tilt values
\author Gareth Edwards
\return INT - VS_ERROR <= NULL < setup bitmask value
*/
INT Control::UpdateElemPanoCylinderPanTilt(
		Control *control,
		vs_system::AppDependency *app_dependency,
		VsFloatRectangle *gfx_cav,
		UINT id
	)
{

	// ---- local
		vs_system::AppControl *app_control = control->app_control;
		vs_system::SysInput   *sys_input   = app_dependency->GetSysInput();
		Model                  *model      = control->model;
		vs_system::ObjConfig   *obj_config = model->GetObjConfig();


	// ---- PTF parameters
		FLOAT pan, tilt, fov;
		obj_config->GetPanoPTF(&pan, &tilt, &fov);


	// ---- angle parameters
		FLOAT min_angle, max_angle;
		obj_config->GetPanoAngles(&min_angle, &max_angle);


	// ---- angle parameters
		FLOAT radius_min, radius_max;
		obj_config->GetPanoRadii(&radius_min, &radius_max);
		FLOAT radius_total = radius_max - radius_min;


	// ---- cylinder parameters
		FLOAT cylinder_scale, cylinder_x_rotate;
		obj_config->GetPanoCylinderXRotate(&cylinder_x_rotate);
		obj_config->GetPanoCylinderScale(&cylinder_scale);


	// ---- pick with CAV
		POINT pick_point = { 0, 0 };
		pick_point = *sys_input->GetPosition();
		FLOAT x_pick = (FLOAT)pick_point.x - gfx_cav->left;
		FLOAT y_pick = (FLOAT)pick_point.y - gfx_cav->top;
		FLOAT panel_width = gfx_cav->right - gfx_cav->left;
		FLOAT panel_height = gfx_cav->bottom - gfx_cav->top;
		FLOAT wby2 = panel_width / 2;
		FLOAT hby2 = panel_height / 2;
		FLOAT aspect = panel_width / panel_height;


	// ---- remapped pick point with y up
		x_pick = x_pick - wby2;
		y_pick = y_pick - hby2;
		y_pick *= -1;


	// ---- cylinder radius proportional to panel height
		FLOAT cylinder_radius = panel_height * cylinder_scale;


	// ========== IF NOT within x hull THEN don't update pan and tilt ==========
		if (x_pick > cylinder_radius || x_pick <= -cylinder_radius) return VS_ERROR;


	// ---- rotation cosf, sinf
		FLOAT x_rotation_radians = cylinder_x_rotate * (FLOAT)VS_RADIANS_PER_DEGREE;
		FLOAT x_cosf = cosf(x_rotation_radians);
		FLOAT x_sinf = sinf(x_rotation_radians);

		// ---- cylinder height proportional to cylinder_radius
		FLOAT cylinder_height = radius_total / radius_max * cylinder_radius * 2;
		FLOAT cylinder_hby2 = (cylinder_height / 2);


	// ---- create cylinder lut with y up
		FLOAT cyl_yz_lut[4][2] =
		{
			{  cylinder_hby2, -cylinder_radius },
			{  cylinder_hby2,  cylinder_radius },
			{ -cylinder_hby2,  cylinder_radius },
			{ -cylinder_hby2, -cylinder_radius }
		};


	// ---- rotate
		for (UINT i = 0; i < 4; i++)
		{
			FLOAT y = cyl_yz_lut[i][0];
			FLOAT z = cyl_yz_lut[i][1];
			cyl_yz_lut[i][0] = y*x_cosf - z*x_sinf; // y' = y*cos q - z*sin q
			cyl_yz_lut[i][1] = y*x_sinf + z*x_cosf; // z' = y*sin q + z*cos q
		}


	// ---- test
		FLOAT y_max = cyl_yz_lut[0][0];
		FLOAT y_min = cyl_yz_lut[2][0];


	// ========== IF NOT within y hull THEN don't update pan and tilt ==========
		if (y_pick > y_max || y_pick <= y_min) return VS_ERROR;

		//
		//          -------     So, scale orthographic cylinder 
		//         /       \    into isometric, then set flags
		//        /    A    \   for inside A, inside B and inside
		//        \         /   C ( B centre < > A centre ). 
		//        |\       /|
		//        | ------- |   Then,
		//        |    C    |   
		//        | ------- |   IF inside A AND NOT inside B THEN
		//        |/       \|     back surface
		//        /    B    \   ELSE
		//        \         /      IF inside B AND NOT inside A OR
		//         \       /          inside C AND NOT inside A THEN
		//          -------		  front surface
		//                       
		//                      Neat!
		//


	// ---- isometric scalar
		FLOAT y_height = cyl_yz_lut[0][0] - cyl_yz_lut[1][0];
		FLOAT y_scalar = cylinder_radius * 2 / y_height;


	// ---- scale y from orthographic to isometric
		for (UINT i = 0; i < 4; i++) cyl_yz_lut[i][0] *= y_scalar;
		y_pick *= y_scalar;


	// ---- A and B y centre
		FLOAT a_y_cen = (cyl_yz_lut[0][0] + cyl_yz_lut[1][0]) / 2;
		FLOAT b_y_cen = (cyl_yz_lut[2][0] + cyl_yz_lut[3][0]) / 2;


	// ---- r^2
		FLOAT r_x_2 = cylinder_radius*cylinder_radius;


	// ---- x_pick^2
		FLOAT x_x_2 = x_pick*x_pick;


	// ---- inside A
		FLOAT y = y_pick - a_y_cen;
		FLOAT a_dist = sqrt(x_x_2 + y*y);
		BOOL  a_inside = a_dist < cylinder_radius ? TRUE : FALSE;

	// ---- inside B
		y = y_pick - b_y_cen;
		FLOAT b_dist = sqrt(x_x_2 + y*y);
		BOOL  b_inside = b_dist < cylinder_radius ? TRUE : FALSE;


	// ---- inside C
		BOOL c_inside = y_pick > b_y_cen && y_pick < a_y_cen ? TRUE : FALSE;


	// ---- y = sqrt( r*r - x*x )
		FLOAT y_intersect = sqrt(r_x_2 - x_x_2);


	// ---- pan (not adjusted)
		FLOAT cyl_pan = atan2(x_pick, y_intersect) * 180 / (FLOAT)VS_PI;


	// ---- tilt
		FLOAT cyl_tilt = 0;


	// ---- IF inside A AND NOT inside B THEN
		if ( a_inside && !b_inside )
		{
			// adjust pan
			cyl_pan  = pan - cyl_pan;

			// calculate tilt interpolant
			cyl_tilt = ((a_y_cen + y_pick) - y_intersect) / y_height;

		}
	// ---- ELSE IF inside B AND NOT inside A OR inside C AND NOT inside A THEN
		else if ( b_inside && !a_inside || c_inside && !a_inside)
		{
			// adjust pan
			cyl_pan += pan + 180;

			// calculate tilt interpolant
			cyl_tilt = (y_intersect - (b_y_cen - y_pick)) / y_height;

		}
	// ---- ELSE elsewhere
		else
		{
			return VS_ELM_SETUP_NONE;
	}


	// ---- adjust pan to legit range
		cyl_pan = cyl_pan > 360 ? cyl_pan - 360 : cyl_pan < 0 ? cyl_pan + 360 : cyl_pan;


	// ---- adjust tilt to legit range
		cyl_tilt = cyl_tilt < 0 ? 0 : cyl_tilt > 1 ? 1 : cyl_tilt;


	// ---- interpolate tilt
		cyl_tilt = (1 - cyl_tilt)*min_angle + cyl_tilt*max_angle;


	// ---- set PTF
		obj_config->SetPanoPTF(cyl_pan, cyl_tilt, fov);


	return (INT)VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING;
}

