////////////////////////////////////////////////////////////////////////////////

// ---------- c3_ptz_onvif.hpp ----------
/*!
\file c3_ptz_onvif.hpp
\brief 
\author Gareth Edwards
*/


// ---------- MapPtzToVptf ----------
/*!
\brief MAP onvif "where i am now" PTZ TO Virtual camera's PTF
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
\note

1. RM and CM values are deltas, but only the sign of these is
used, as the values are not consistant across VMS and OTT.

2. Incoming pan values are flipped so that observed pan
behaviour matches VMS and OTT events.

3. Outgoing, sent back to server, pan values are re-flipped.

4. Pan values are scaled to proportionaly match tilt values.

5. Pan values wraparound.

6. Tilt values are limited to minimum and maximum pano radii/
angles, and adjusted such that the top of the field of (FOV)
does exceed the viewable limits.

7. Zoom values are mapped to FOV abd are limited to be a
minum of 1/2 the maximum value FOV and the maximum FOV.

8. Pan and tilt values are scaled proportional to zoom,
such that the appear to be consistant in magnitude.

9. Relative move (RM) increments are scaled from a value
specified in the Camera application config file, e.g.
"RM-Increment:0.1;".

10. Continuous move (CM) increments are scaled from a value
specified in the Camera application config file, e.g.
"CM-Increment:0.1;".

11. Continuous move (CM) increments are scaled proportional
to the elapsed time between each move.

12. PTZ values are processed into a stashed "Where I Am Now"
(WIAN) set of PTZ values (see usage of ONVIF::PTZ class)
(see UpdateWianPtz method).

13. Stashed WIAN PTZ values are then mapped into panoramic
Virtual Camera PTF values (see method UpdateVptf).

*/
INT PI_Props::MapPtzToVptf()
{
	// ---- local ----
	INT result = 0;
	CHAR *move_name[6] = { "unknown", "eStop", "eContinuousMove", "eAbsoluteMove", "eRelativeMove", "eHomeMove" };
	CHAR  msg[128];


	// ---- parameters ----
	const FLOAT onvif_cm_tolr = 0.0001f;


	// ---- update continuous move time ----
	DWORD onvif_cm_time_now = timeGetTime();
	DWORD onvif_cm_ms_delta = onvif_cm_time_now - onvif_cm_time_last;
	onvif_cm_time_last = onvif_cm_time_now;


	// ---- Test: ensure we fully dequeue ----
	ONVIF::PTZ onvif_ptz; // = { 0, 0, 0, 0 };
	UINT index = 0;
	while ( onvif_handler->GetPTZ(onvif_ptz) )
	{
		// ---- report ----
		sprintf(
				msg,
				"ONVIF M:%s ____ P: %6.4f ____ T: %6.4f ____ Z: %6.4f\n",
				move_name[ onvif_ptz.m_motion < 0 ? 0 : ( onvif_ptz.m_motion > 5 ? 5 : onvif_ptz.m_motion ) ],
				onvif_ptz.m_p,
				onvif_ptz.m_t,
				onvif_ptz.m_z
			);
		sys_output->Message(msg);


		// ---- set sign of P, T and Z ----
		onvif_pan_sign = onvif_ptz.m_p < -onvif_cm_tolr ? -1 :
			( onvif_ptz.m_p > onvif_cm_tolr ? 1 : 0 );
		onvif_tilt_sign = onvif_ptz.m_t < -onvif_cm_tolr ? -1 :
			(onvif_ptz.m_t > onvif_cm_tolr ? 1 : 0);
		onvif_zoom_sign = onvif_ptz.m_z < -onvif_cm_tolr ? -1 :
			(onvif_ptz.m_z > onvif_cm_tolr ? 1 : 0);


		// ---- handle events ----
		switch ( onvif_ptz.m_motion )
		{
			case ONVIF::eHomeMove:                       // move to home
				break;

			case ONVIF::eAbsoluteMove:                   // absolute move (AM)
				onvif_ptz.m_p *= -1;                     // flip sign!!!
				onvif_wian_ptz = onvif_ptz;
				break;

			case ONVIF::eRelativeMove:                   // relative move (RM)
				{
					// Calculate pan proportional to tilt scalar
					FLOAT pano_min_angle, pano_max_angle;
					result = model->GetObjConfig()->GetPanoAngles(&pano_min_angle, &pano_max_angle);
					FLOAT total_angle = pano_max_angle - pano_min_angle;
					FLOAT pan_scalar  = total_angle / 360.0f;

					// Calculate RM pan and tilt
					FLOAT fov_scalar = (onvif_wian_ptz.m_z + 1) / 2;
					FLOAT magnitude  = onvif_rm_incr * fov_scalar;
					FLOAT vp = onvif_ptz.m_p < 0 ? -magnitude : magnitude;
					FLOAT vt = onvif_ptz.m_t < 0 ? -magnitude : magnitude;
					onvif_wian_ptz.m_p -= vp*pan_scalar; // Flip sign!!!
					onvif_wian_ptz.m_t += vt;
					
				}
				break;

			case ONVIF::eContinuousMove:                 // continuous move (CM)
				{

					if ( onvif_ptz.m_z != 1.0f && fabs(onvif_ptz.m_z) > onvif_cm_tolr )
					{
						onvif_cm_zoom_mode = TRUE;
						onvif_cm_on = TRUE;
						onvif_cm_zoom = onvif_cm_pan = onvif_cm_tilt = 0;

						onvif_cm_zoom = onvif_ptz.m_z < 0 ?
							-onvif_cm_incr :
								onvif_cm_incr;
					}
					else if ( fabs(onvif_ptz.m_p) > onvif_cm_tolr || fabs(onvif_ptz.m_t) > onvif_cm_tolr )
					{
						onvif_cm_zoom_mode = FALSE;
						onvif_cm_on = TRUE;
						onvif_cm_zoom = onvif_cm_pan = onvif_cm_tilt = 0;

						if (fabs(onvif_ptz.m_p) > onvif_cm_tolr)
							onvif_cm_pan  = onvif_ptz.m_p < 0 ?
								onvif_cm_incr :
									-onvif_cm_incr;      // flip sign!!!

						if ( fabs(onvif_ptz.m_t) > onvif_cm_tolr )
							onvif_cm_tilt = onvif_ptz.m_t < 0 ?
								-onvif_cm_incr :
									onvif_cm_incr;
					}
					else
					{
						onvif_cm_zoom_mode = FALSE;
						onvif_cm_on        = FALSE;
						onvif_cm_zoom      = onvif_cm_pan = onvif_cm_tilt = 0;
						onvif_ptz.m_motion = ONVIF::eStop;
					}
				}
				break;

			case ONVIF::eStop:                           // stop
				onvif_cm_on = TRUE;
				;
				break;

			default:
				break;
		}


		// ---- IF NOT continuous move THEN set continuous move OFF ----
		if ( onvif_ptz.m_motion != ONVIF::eContinuousMove )
			onvif_cm_on = FALSE;


		// ---- update "where i am now" motion ----
		onvif_wian_ptz.m_motion = onvif_ptz.m_motion;


		// ---- process ONVIF "Where Am I Now" PTZ ----
		result = UpdateWianPtz();


		// ---- update Virtual Camera PTF # ----
		result = UpdateVptf(vs_system::ObjConfig::STREAM_PANO_PTF_ID);


	}   // --- while ( onvif_handler->GetPTZ(onvif_ptz) )


	// ----- IF continuous move THEN ---
	if ( onvif_cm_on )
	{

		// ---- calculate move scalar proportional to elapsed time ----
		FLOAT onvif_cm_scalar   = onvif_cm_ms_delta / 1000.0f;
		FLOAT fov_scalar  = (onvif_wian_ptz.m_z + 1) / 2;
		FLOAT move_scalar = fov_scalar * onvif_cm_scalar;


		// ---- IF zoom only THEN --
		if ( onvif_cm_zoom_mode )
		{
			// Calculate zoom
			onvif_wian_ptz.m_z += move_scalar * onvif_cm_zoom;

			//printf("Z  %6.4f \n", onvif_cm_zoom);
		}
		// ELSE both pan and tilt
		else
		{
			// Calculate pan scalar proportional to annular angle aspect ratio
			FLOAT pano_min_angle, pano_max_angle;
			result = model->GetObjConfig()->GetPanoAngles(&pano_min_angle, &pano_max_angle);
			FLOAT total_angle = pano_max_angle - pano_min_angle;
			FLOAT pan_scalar = total_angle / 360.0f;

			// Calculate pan and tilt
			onvif_wian_ptz.m_p += move_scalar * onvif_cm_pan * pan_scalar;
			onvif_wian_ptz.m_t += move_scalar * onvif_cm_tilt;

			//printf("PT %6.4f %6.4f \n", onvif_cm_pan, onvif_cm_tilt);
		}

		//printf("CM %6.4f %6.4f %6.4f \n", onvif_wian_ptz.m_p, onvif_wian_ptz.m_t, onvif_wian_ptz.m_z);


		// ---- update "where i am now" motion ----
		onvif_wian_ptz.m_motion = ONVIF::eContinuousMove;


		// ---- process ONVIF "Where Am I Now" PTZ ----
		result = UpdateWianPtz();


		// ---- update Virtual Camera PTF # ----
		result = UpdateVptf(vs_system::ObjConfig::STREAM_PANO_PTF_ID);
	}

	return VS_OK;
}


// ---------- UpdateWianPtz ----------
/*!
\brief Update onvif "Where I Am Now" PTZ
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT PI_Props::UpdateWianPtz()
{

	// ---- parameters ----
	const FLOAT onvif_cm_tolr = 0.0001f;


	// ---- wraparound Pan ----
	onvif_wian_ptz.m_p = onvif_wian_ptz.m_p < -1 ? 2 + onvif_wian_ptz.m_p :
		(onvif_wian_ptz.m_p > 1 ? onvif_wian_ptz.m_p - 2 : onvif_wian_ptz.m_p);


	// ---- limit Tilt and Zoom to minimax ----
	onvif_wian_ptz.m_t = onvif_wian_ptz.m_t < -1 ? -1 : (onvif_wian_ptz.m_t > 1 ? 1 : onvif_wian_ptz.m_t);
	onvif_wian_ptz.m_z = onvif_wian_ptz.m_z <  0 ?  0 : (onvif_wian_ptz.m_z > 1 ? 1 : onvif_wian_ptz.m_z);


	// IF -- continuous move --- THEN ( stop move at minimax )
	if ( onvif_wian_ptz.m_motion == ONVIF::eContinuousMove )
	{

		// IF -- zoom only -- THEN
		if (onvif_cm_zoom_mode)
		{

			// IF -- less than or equal to min AND decrementing -- THEN -- stop
			if (onvif_wian_ptz.m_z <= 0 && onvif_zoom_sign == -1)
			{
				onvif_cm_on = FALSE;
				onvif_wian_ptz.m_motion = ONVIF::eStop;
				sys_output->Message("ONVIF Z: STOP at MIN.");
			}

			// ELSE IF -- greater than or equal to max -- AND -- incrementing -- THEN -- stop
			else if (onvif_wian_ptz.m_z >= 1 && onvif_zoom_sign == 1)
			{
				onvif_cm_on = FALSE;
				onvif_wian_ptz.m_motion = ONVIF::eStop;
				sys_output->Message("ONVIF Z: STOP at MAX.");
			}

		}
		// ELSE both pan and tilt
		else
		{

			// IF -- tilt less than or equal to min -- AND -- tilt decrementing -- THEN -- stop
			if (onvif_wian_ptz.m_t <= -1 && onvif_tilt_sign == -1)
			{
				onvif_cm_on = FALSE;
				onvif_wian_ptz.m_motion = ONVIF::eStop;
				sys_output->Message("ONVIF T: STOP at MIN.");

			}

			// ELSE IF -- tilt greater than or equal to max -- AND -- tilt incrementing -- THEN -- stop
			else if (onvif_wian_ptz.m_t >= 1 && onvif_tilt_sign == 1)
			{
				onvif_cm_on = FALSE;
				onvif_wian_ptz.m_motion = ONVIF::eStop;
				sys_output->Message("ONVIF T: STOP at MAX.");
			}
		}
	}


	// ---- REALLY important: Copy "Where I am", unflip pan - then...
	ONVIF::PTZ onvif_unflip_ptz = onvif_wian_ptz;
	onvif_unflip_ptz.m_p *= -1;


	// ---- REALLY important: Send unflipped PTZ back to the ONVIF handler
	onvif_handler->UpdatePTZ(onvif_unflip_ptz);

	return VS_OK;
}


// ---------- UpdateVptf ----------
/*!
\brief Update Virtual camera (# 8) PTF
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT PI_Props::UpdateVptf(UINT pano_ptf_id)
{

	// ---- MAP ONVIF PTZ to PTF ----


	// ---- get Pano parameters ----
	INT result = 0;
	FLOAT pano_fov_max = 0;
	FLOAT pano_min_angle = 0, pano_max_angle = 0;
	vs_system::ObjConfig *obj_config = pi_props->model->GetObjConfig();
	result = obj_config->GetPanoFovMax(&pano_fov_max);
	result = obj_config->GetPanoAngles(&pano_min_angle, &pano_max_angle);


	// ---- map pan ----
	FLOAT virtual_camera_pan = onvif_wian_ptz.m_p * 180.0f;


	// ---- map fov ----
	FLOAT rmu = onvif_wian_ptz.m_z;
	FLOAT virtual_camera_fov = (1 - rmu) * (pano_fov_max / 2.0f) + rmu * pano_fov_max;


	// ---- map tilt ----
	FLOAT total_angle = pano_max_angle - pano_min_angle;
	FLOAT mid_angle = pano_max_angle - (total_angle / 2.0f);
	FLOAT angle = (pano_fov_max - virtual_camera_fov) / 2.0f;
	FLOAT virtual_camera_tilt = onvif_wian_ptz.m_t * angle + mid_angle;


	// ---- get index of current Virtual Camera ----
	UINT current_id = 0;
	result = obj_config->GetPanoPTFId(&current_id);


	// ---- set Virtual Camera # 8 mapped PTF values
	//
	// Note:Check model->Display method for usage of Virtual Camera # 8
	// 
	result = obj_config->SetPanoPTFById(
				pano_ptf_id,
				virtual_camera_pan,
				virtual_camera_tilt,
				virtual_camera_fov
			);


	// ---- restore index of current Virtual Camera  ----
	result = obj_config->SetPanoPtfSelectedById(current_id);

	return VS_OK;
}