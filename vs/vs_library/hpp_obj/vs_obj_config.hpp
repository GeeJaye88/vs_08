////////////////////////////////////////////////////////////////////////////////

// ---------- vs_obj_config.hpp ----------
/*!
\file vs_obj_config.hpp
\brief
\author Gareth Edwards
*/


// ---- initialise static private implementation ----
ObjConfig::ObjVptf      * ObjConfig::pi_obj_pano  = NULL;
ObjConfig::ImageRoi     * ObjConfig::pi_image_roi = NULL;
vs_system::CamProps     * ObjConfig::cam_props    = NULL;


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
ObjConfig::ObjConfig()
{

	// ---- big objects
		pi_obj_pano   = new ObjConfig::ObjVptf();
		pi_image_roi  = new ObjConfig::ImageRoi();

}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
ObjConfig::~ObjConfig()
{

	if ( pi_obj_pano != NULL  ) { delete pi_obj_pano;  pi_obj_pano = NULL;  }
	if  (pi_image_roi != NULL ) { delete pi_image_roi; pi_image_roi = NULL; }

}


// ---------- Initialise ---------- // NOT USED AT PRESENT
/*!
\brief Initialise pano config
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::Initialise()
{

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- PanoRead ----------
/*!
\brief Read pano configuration file
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::ReadPanoConfig(CHAR *file_name)
{
	INT result = pi_obj_pano->Read(file_name);
	return result;
}


// ---------- WritePanoConfig ----------
/*!
\brief Write pano configuration file
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::WritePanoConfig()
{
	INT result = pi_obj_pano->Write();
	return result;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- CopyPanoPtf ----------
/*!
\brief Copy 
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::CopyPanoPTF(
		UINT dest_id,
		UINT src_id
	)
{

	if (
			src_id >= 1 && src_id <= MAX_PANO_PTF_LUT &&
			dest_id >= 1 && dest_id <= MAX_PANO_PTF_LUT
		)
	{
		UINT src_index  = src_id - 1;
		UINT dest_index = dest_id - 1;

		pi_obj_pano->pan =
			pi_obj_pano->pan_last = 
				pi_obj_pano->pan_lut[dest_index] =
					//pi_obj_pano->pan_lut_default[dest_index] =
						pi_obj_pano->pan_lut[src_index];

		pi_obj_pano->tilt =
			pi_obj_pano->tilt_last =
				pi_obj_pano->tilt_lut[dest_index] =
					//pi_obj_pano->tilt_lut_default[dest_index] =
						pi_obj_pano->tilt_lut[src_index];

		pi_obj_pano->FOV =
			pi_obj_pano->FOV_last = 
				pi_obj_pano->fov_lut[dest_index] =
					//pi_obj_pano->fov_lut_default[dest_index] =
						pi_obj_pano->fov_lut[src_index];

		/*
		CHAR msg[VS_MAXCHARLEN];
		sprintf(msg, "--- %6.4f   %6.4f   %6.4f \n",
						pi_obj_pano->pan,
						pi_obj_pano->tilt,
						pi_obj_pano->FOV
					);
		OutputDebugString(msg);
		*/

		return VS_OK;
	}

	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- CpyImagePanelActivePtfbyId ----------
/*!
\brief Copy 
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::CpyImagePanelActivePtfbyId(UINT dest_id)
{

	if ( dest_id >= 1 && dest_id <= MAX_PANO_PTF_LUT )
	{
		UINT src_index  = pi_obj_pano->ptf_active_id - 1;

		/*
		CHAR msg[64];
		sprintf(msg, "    Copy %d to %d\n", pi_obj_pano->ptf_active_id, dest_id);
		OutputDebugString(msg);
		*/

		UINT dest_index = dest_id - 1;

				pi_obj_pano->pan_lut[dest_index] =
					pi_obj_pano->pan_lut[src_index];

				pi_obj_pano->tilt_lut[dest_index] =
					pi_obj_pano->tilt_lut[src_index];

				pi_obj_pano->fov_lut[dest_index] =
					pi_obj_pano->fov_lut[src_index];

		return VS_OK;
	}

	return VS_ERROR;
}


// ---------- GetImagePanelActiveId ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetImagePanelActiveId(UINT *id)
{
	*id = pi_obj_pano->ptf_active_id;
	return VS_OK;
}


// ---------- GetImagePanelPreviousId ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetImagePanelPreviousId(UINT *id)
{
	*id = pi_obj_pano->ptf_previous_id;
	return VS_OK;
}


// ---------- GetImagePanelAspectRatio ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetImagePanelAspectRatio(FLOAT *ar)
{
	*ar = pi_obj_pano->ptf_aspect_ratio;
	return VS_OK;
}


// ---------- SetImagePanelActiveId ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetImagePanelActiveId(UINT id)
{

	UINT index = id - 1;
	if (
		index >= IMG_PTZ_FIRST_PANEL_INDEX &&
		index <= IMG_PTZ_LAST_PANEL_INDEX
		)
	{

		if ( pi_obj_pano->ptf_active_id != id )
		{
			pi_obj_pano->ptf_previous_id = pi_obj_pano->ptf_active_id;
			pi_obj_pano->ptf_active_id = id;
		}

	}

	return VS_OK;
}


// ---------- SetImagePanelPreviousId ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetImagePanelPreviousId(UINT id)
{
	pi_obj_pano->ptf_previous_id = id;
	return VS_OK;
}


// ---------- SetImagePanelAspectRatio ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetImagePanelAspectRatio(FLOAT ar)
{
	pi_obj_pano->ptf_aspect_ratio = ar;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- GetImagePanelOverId ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetImagePanelOverId(UINT *id)
{
	*id = pi_obj_pano->ptf_over_id;
	return VS_OK;
}


// ---------- SetImagePanelOverId ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetImagePanelOverId(UINT id)
{
	pi_obj_pano->ptf_over_id = id;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------------- Auto PTF states ----------------

	BOOL ObjConfig::IsAutoPTFRunning() { return pi_obj_pano->auto_vptf_running; }
	BOOL ObjConfig::IsAutoPTFStart()   { return pi_obj_pano->auto_vptf_start;   }
	BOOL ObjConfig::IsAutoPTFStop()    { return pi_obj_pano->auto_vptf_stop;    }
	BOOL ObjConfig::IsAutoPTFSosi()    { return pi_obj_pano->auto_vptf_sosi;    }
	BOOL ObjConfig::IsAutoPTFSlide()   { return pi_obj_pano->auto_vptf_slide;   }
	BOOL ObjConfig::IsAutoPTFSlow()    { return pi_obj_pano->auto_vptf_slow;    }
	BOOL ObjConfig::IsAutoPTFForward() { return pi_obj_pano->auto_vptf_forward; }
	BOOL ObjConfig::IsAutoPTFPause()   { return pi_obj_pano->auto_vptf_pause;   }
	BOOL ObjConfig::IsAutoPTFShuffle() { return pi_obj_pano->auto_vptf_shuffle; }

// ---------------- Auto PTF get ----------------

	INT ObjConfig::GetAutoPTFFromId(UINT *from_id, FLOAT *from_mu)
	{
		if (pi_obj_pano->auto_vptf_running)
		{
			if (pi_obj_pano->auto_vptf_forward)
			{
				*from_id = pi_obj_pano->auto_vptf_as_id - 1;
				*from_mu = pi_obj_pano->auto_vptf_from_mu;
			}
			else
			{
				*from_id = pi_obj_pano->auto_vptf_as_id - 2;
				*from_mu = 1 - pi_obj_pano->auto_vptf_from_mu;
			}
		}
		else
		{
			UINT ptf_id = 0;
			INT result = GetPanoPTFId(&ptf_id);
			*from_id = ptf_id - 1;
			*from_mu = 0;
		}

		return VS_OK;
	}

// ---------------- Auto PTF set ----------------

	INT ObjConfig::SetAutoPTFRunning(BOOL running)
	{
		if (running)
			pi_obj_pano->auto_vptf_start = FALSE;
		else
			pi_obj_pano->auto_vptf_stop = FALSE;

		pi_obj_pano->auto_vptf_running = running;

		return VS_OK;
	}

	INT ObjConfig::SetAutoPTFScalar(FLOAT ms_scalar)
	{
		pi_obj_pano->auto_vptf_ms_speed_scalar = ms_scalar;
		return VS_OK;
	}

	INT ObjConfig::SetAutoPTFSosi(BOOL sosi)
	{
		pi_obj_pano->auto_vptf_sosi = sosi;
		return VS_OK;
	}

	INT ObjConfig::SetAutoPTFSlide(BOOL slide)
	{
		pi_obj_pano->auto_vptf_slide = slide;
		return VS_OK;
	}  

	INT ObjConfig::SetAutoPTFSlow(BOOL slow)
	{
		pi_obj_pano->auto_vptf_slow = slow;
		return VS_OK;
	}

	INT ObjConfig::SetAutoPTFForward(BOOL forward)
	{
		pi_obj_pano->auto_vptf_forward = forward;
		return VS_OK;
	}

	INT ObjConfig::SetAutoPTFPause(BOOL pause)
	{
		pi_obj_pano->auto_vptf_pause = pause;

		if (pause)
		{
			pi_obj_pano->auto_vptf_ms_pause_elapsed = timeGetTime() - pi_obj_pano->auto_vptf_ms_start;
		}
		else
		{
			pi_obj_pano->auto_vptf_ms_start = timeGetTime() - pi_obj_pano->auto_vptf_ms_pause_elapsed;
		}

		return VS_OK;
	}

	INT ObjConfig::SetAutoPTFShuffle(BOOL shuffle)
	{
		pi_obj_pano->auto_vptf_shuffle = shuffle;
		return VS_OK;
	}

// ---------------- Auto PTF process ----------------

	INT ObjConfig::AutoPTFStart()
	{
		// ---- start state
			pi_obj_pano->auto_vptf_start = TRUE;
			pi_obj_pano->auto_vptf_stop = FALSE;

			// note: Model::UpdateAutoPTF() -> IsAutoPTFRunning "gates" start
			pi_obj_pano->auto_vptf_running = TRUE;

		// ---- store index of selected ptf
			INT  result = GetPanoPTFId(&pi_obj_pano->auto_vptf_id);
			pi_obj_pano->auto_vptf_id -= pi_obj_pano->auto_vptf_lut_offset;

		// ---- start the clock
			pi_obj_pano->auto_vptf_ms_start = timeGetTime();

		// ---- if pause, set
			if (IsAutoPTFPause())
				pi_obj_pano->auto_vptf_ms_pause_elapsed = timeGetTime() - pi_obj_pano->auto_vptf_ms_start;

		// ---- store ptf & calc distance between ptf
			// note: set pi_obj_pano->auto_vptf_from_id in...
			AutoPTFSnapshot();

		// ---- temp: default speed scalar 
			SetAutoPTFScalar(0.1f);

		return VS_OK;
	}

	INT ObjConfig::AutoPTFSnapshot()
	{

		// ---- status
			BOOL forward = pi_obj_pano->auto_vptf_forward;

		// ---- stack from bottom to top OR vice versa
			INT start = forward ? 0 :  8;
			INT end   = forward ? 9 : -1;
			INT inc   = forward ? 1 : -1;
			INT top   = 0;
			for (INT i = start; i != end; i += inc)
			{
				// ---- only if switched on ?
					if (pi_obj_pano->auto_ptf_snapshot[i].switched_on)
					{
						// ---- app ptf id
							UINT ptf_id = i + 2;

						// ---- get ptf
							ObjVptf::AutoSnapshot *as = &pi_obj_pano->auto_ptf_snapshot[top];
							INT result = GetPanoPTFById(ptf_id, &as->pan, &as->tilt, &as->fov);

						// ---- set originating id
							as->id = ptf_id;

						// ---- update from!
							if (ptf_id == pi_obj_pano->auto_vptf_id + pi_obj_pano->auto_vptf_lut_offset)
								pi_obj_pano->auto_vptf_from_id = top;

						// ---- push
							top++;
					}
			}

		// ---- random shuffle
			if (pi_obj_pano->auto_vptf_shuffle)
			{
				std::vector<ObjVptf::AutoSnapshot> as_shuffle;
				for (INT i = 0; i < top; i++)
					as_shuffle.push_back(pi_obj_pano->auto_ptf_snapshot[i]);
				std::random_device rd;
				std::default_random_engine rng(rd());
				shuffle(as_shuffle.begin(), as_shuffle.end(), rng);

				for (INT i = 0; i < top; i++)
					pi_obj_pano->auto_ptf_snapshot[i] = as_shuffle.at(i);
			}

		// ---- wraparound
			pi_obj_pano->auto_ptf_snapshot[top] = pi_obj_pano->auto_ptf_snapshot[0];
			
			// ---- store selected (restored in AutoPTFStop)
			ObjVptf::AutoSnapshot *as = &pi_obj_pano->auto_ptf_snapshot[top];
			pi_obj_pano->auto_ptf_store = *as;

		// ---- set from & top
			pi_obj_pano->auto_vptf_top = top - 1;

		// ---- calc distance
			ObjVptf::AutoSnapshot *here, *there;
			FLOAT pan, tilt;
			for (UINT i = 0; i <= pi_obj_pano->auto_vptf_top; i++)
			{
				here  = &pi_obj_pano->auto_ptf_snapshot[i];
				there = &pi_obj_pano->auto_ptf_snapshot[i+1];
				pan   = there->pan  - here->pan;
				tilt  = there->tilt - here->tilt;
				here->dist = sqrt(pan * pan + tilt * tilt);
			}

		return VS_OK;
	}

	INT ObjConfig::AutoPTFSwitchedOn(BOOL switched_on[])
	{
		for (UINT i = 0; i <= 8; i++)
		{
			ObjVptf::AutoSnapshot *as = &pi_obj_pano->auto_ptf_snapshot[i];
			as->switched_on = switched_on[i];
		}
		return VS_OK;
	}

	// required for cos function below
	#include <cmath>

	INT ObjConfig::AutoPTFUpdate()
	{
		// ---- set update state
			pi_obj_pano->auto_vptf_start = FALSE;
			pi_obj_pano->auto_vptf_stop = FALSE;

		// ---- time based interpolation
			if (pi_obj_pano->auto_vptf_pause)
			{
				pi_obj_pano->auto_vptf_ms_now = pi_obj_pano->auto_vptf_ms_pause_elapsed;
			}
			else
			{
				pi_obj_pano->auto_vptf_ms_now = timeGetTime() - pi_obj_pano->auto_vptf_ms_start;
			}

		// ---- calc interpolation value between "here and there"
			UINT ptf_id = pi_obj_pano->auto_vptf_from_id;
			ObjVptf::AutoSnapshot *here = &pi_obj_pano->auto_ptf_snapshot[ptf_id];
			ObjVptf::AutoSnapshot *there = &pi_obj_pano->auto_ptf_snapshot[ptf_id +1];

		// ---- slide
			FLOAT auto_vptf_slide_scalar = 1 / pi_obj_pano->auto_vptf_ms_speed_scalar;

		// ---- slow
			FLOAT auto_vptf_slow_scalar = 0.2f;

		// ---- speed
			FLOAT speed = pi_obj_pano->auto_vptf_slide ?
					pi_obj_pano->auto_vptf_ms_speed_scalar * auto_vptf_slide_scalar :
						pi_obj_pano->auto_vptf_ms_speed_scalar;

			      speed = pi_obj_pano->auto_vptf_slow ? speed * auto_vptf_slow_scalar : speed;

		 // ---- linear OR inversely proportional scalar
			FLOAT ms_scalar = pi_obj_pano->auto_vptf_slide ? 
					speed :
						1 / here->dist * speed;

		// ---- inversely proportional scalar
			FLOAT mu = (FLOAT)pi_obj_pano->auto_vptf_ms_now / 1000 * ms_scalar;

		// ---- so, if finished then next or wraparound & set ptf OR interpolate & set ptf
			INT result = 0;
			if (mu > 1)
			{
				pi_obj_pano->auto_vptf_from_id = ptf_id + 1 > pi_obj_pano->auto_vptf_top ? 0 : ptf_id + 1;
				pi_obj_pano->auto_vptf_ms_start = timeGetTime();
				result = SetPanoPTFById(
						pi_obj_pano->auto_vptf_id + pi_obj_pano->auto_vptf_lut_offset,
						there->pan, there->tilt, there->fov
					);
				pi_obj_pano->auto_vptf_as_id = there->id;
			}
			else
			{
				// ---- slide 
					if (pi_obj_pano->auto_vptf_slide)
						mu = 0.0;
					else
						mu = pi_obj_pano->auto_vptf_sosi ? -cos(mu * 180*0.01745f)*0.5f+0.5f : mu;

				// ---- inerpolate
					FLOAT pan  = (1 - mu) * here->pan  + mu * there->pan;
					FLOAT tilt = (1 - mu) * here->tilt + mu * there->tilt;
					FLOAT fov  = (1 - mu) * here->fov  + mu * there->fov;

				// ---- set
					result = SetPanoPTFById(
							pi_obj_pano->auto_vptf_id + pi_obj_pano->auto_vptf_lut_offset,
							pan, tilt, fov
						);
					pi_obj_pano->auto_vptf_as_id = here->id;
			}

			pi_obj_pano->auto_vptf_from_mu = mu;

		return VS_OK;
	}

	INT ObjConfig::AutoPTFStop()
	{
		// ---- stop state
			pi_obj_pano->auto_vptf_start = FALSE;
			pi_obj_pano->auto_vptf_stop = TRUE;

			// note: Model::UpdateAutoPTF() -> IsAutoPTFRunning does not gate stop
			// pi_obj_pano->auto_vptf_running = FALSE;

		// ---- zap
			pi_obj_pano->auto_vptf_ms_start = 0;
			pi_obj_pano->auto_vptf_from_mu = 0;

		// ---- restore selected ptf to default position
			ObjVptf::AutoSnapshot *as = &pi_obj_pano->auto_ptf_store;
			INT result = SetPanoPTFById(
					pi_obj_pano->auto_vptf_id + pi_obj_pano->auto_vptf_lut_offset,
					as->pan, as->tilt, as->fov
				);

		return VS_OK;
	}

	INT ObjConfig::AutoPTFSet()
	{

		UINT ptf_id = 0;
		INT result = GetPanoPTFId(&ptf_id);

		ObjVptf::AutoSnapshot *as = &pi_obj_pano->auto_ptf_store;
		result = GetPanoPTFById(ptf_id, &as->pan, &as->tilt, &as->fov);

		return VS_OK;
	}


////////////////////////////////////////////////////////////////////////////////


// ---------- GetPanoParamById ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoParamById(UINT id, UINT index, INT *v)
{
	if ( id >= 1 && id <= MAX_PANO_PTF_LUT )
	{
		UINT index = id - 1;
		*v = pi_obj_pano->param_lut[index][index - 1];
		return VS_OK;
	}
	return VS_ERROR;
}


// ---------- GetPanoPTF ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoPTF(FLOAT *pan, FLOAT *tilt, FLOAT *FOV)
{
	*pan  = pi_obj_pano->pan;
	*tilt = pi_obj_pano->tilt;
	*FOV  = pi_obj_pano->FOV;
	return VS_OK;
}


// ---------- GetPanoPTFLast ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoPTFLast(FLOAT *pan, FLOAT *tilt, FLOAT *FOV)
{
	*pan  = pi_obj_pano->pan_last;
	*tilt = pi_obj_pano->tilt_last;
	*FOV  = pi_obj_pano->FOV_last;
	return VS_OK;
}


// ---------- GetPanoPTFDerived ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoPTFDerived(FLOAT *pan, FLOAT *tilt, FLOAT *FOV)
{
	*pan  = pi_obj_pano->pan_derived;
	*tilt = pi_obj_pano->tilt_derived;
	*FOV  = pi_obj_pano->FOV_derived;
	return VS_OK;
}


// ---------- GetPanoPTFById ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoPTFById(UINT id, FLOAT *pan, FLOAT *tilt, FLOAT *FOV)
{
	if ( id >= 1 && id <= MAX_PANO_PTF_LUT )
	{
		UINT index = id - 1;
		*pan  = pi_obj_pano->pan_lut[index];
		*tilt = pi_obj_pano->tilt_lut[index];
		*FOV  = pi_obj_pano->fov_lut[index];
		return VS_OK;
	}
	return VS_ERROR;
}


// ---------- GetPanoPTFId ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoPTFId(UINT *id)
{
	*id = pi_obj_pano->ptf_selected_index + 1;
	return VS_OK;
}


// ---------- GetPanoPanOffset ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoPanOffset(FLOAT *pan_offset)
{
	*pan_offset = pi_obj_pano->pan_offset;
	return VS_OK;
}


// ---------- GetPanoAutoPan ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK

\note not used

*/
INT ObjConfig::GetPanoAutoPan(INT *auto_pan)
{
	*auto_pan = pi_obj_pano->auto_pan;
	return VS_OK;
}


// ---------- GetPanoAngles ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoAngles(FLOAT *min_angle, FLOAT *max_angle)
{
	*min_angle = pi_obj_pano->min_angle;
	*max_angle = pi_obj_pano->max_angle;
	return VS_OK;
}


// ---------- GetPanoAngleMax ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoAngleMax(FLOAT *max_angle)
{
	*max_angle = pi_obj_pano->max_angle;
	return VS_OK;
}


// ---------- GetPanoAngleAtUnitRadius ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoAngleAtUnitRadius(FLOAT *angle_at_unit_radius)
{
	*angle_at_unit_radius = pi_obj_pano->angle_at_unit_radius;
	return VS_OK;
}


// ---------- GetPanoAspectRatio ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoAspectRatio(FLOAT *ar)
{
	*ar = pi_obj_pano->aspect_ratio;
	return VS_OK;
}


// ---------- GetPanoCamHFlip ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoCamHFlip(BOOL *flip)
{
	*flip = pi_obj_pano->u_flip;
	return VS_OK;
}


// ---------- GetPanoCamVFlip ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoCamVFlip(BOOL *flip)
{
	*flip = pi_obj_pano->v_flip;
	return VS_OK;
}

// ---------- GetPanoCamHOffset ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoCamHOffset(INT *cam_hoffset)
{
	*cam_hoffset  = pi_obj_pano->cam_hoffset;
	return VS_OK;
}


// ---------- GetPanoCamVOffset ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoCamVOffset(INT *cam_voffset)
{
	*cam_voffset  = pi_obj_pano->cam_voffset;
	return VS_OK;
}


// ---------- GetPanoCamWidth ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoCamWidth(INT *cam_width)
{
	*cam_width  = pi_obj_pano->cam_width;
	return VS_OK;
}


// ---------- GetPanoCamHeight ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoCamHeight(INT *cam_height)
{
	*cam_height  = pi_obj_pano->cam_height;
	return VS_OK;
}

 
// ---------- GetPanoUpdated ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoUpdated(BOOL *updated)
{
	*updated = pi_obj_pano->updated;
	return VS_OK;
}


// ---------- GetPanoConfigPaf ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoConfigPaf(CHAR *file_name)
{
	strcpy(file_name, pi_obj_pano->config_paf_hdl);
	return VS_OK;
}


// ---------- GetPanoCylinderXRotate ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoCylinderXRotate(FLOAT *cylinder_x_rotate)
{
	*cylinder_x_rotate = pi_obj_pano->cylinder_x_rotate;
	return VS_OK;
}


// ---------- GetPanoCylinderScale ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoCylinderScale(FLOAT *cylinder_scale)
{
	*cylinder_scale = pi_obj_pano->cylinder_scale;
	return VS_OK;
}


// ---------- GetPanoFovMax ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoFovMax(FLOAT *fov_max)
{
	*fov_max = pi_obj_pano->max_fov;
	return VS_OK;
}


// ---------- GetPanoPanVertexBuffer ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoPanVertexBuffer(BOOL *pan)
{
	*pan = pi_obj_pano->pan_vertex_buffer;
	return VS_OK;
}


// ---------- GetPanoUVOffset ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoUVOffset(FLOAT *u_offset, FLOAT *v_offset)
{
	*u_offset = pi_obj_pano->u_offset;
	*v_offset = pi_obj_pano->v_offset;
	return VS_OK;
}


// ---------- GetPanoPicking ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoPicking(BOOL *p)
{
	*p = pi_obj_pano->picking;
	return VS_OK;
}


// ---------- GetPanoRadii ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoRadii(FLOAT *min_radius, FLOAT *max_radius)
{
	*min_radius = pi_obj_pano->min_radius;
	*max_radius = pi_obj_pano->max_radius;
	return VS_OK;
}


// ---------- GetPanoRadiusMax ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoRadiusMax(FLOAT *max_radius)
{
	*max_radius = pi_obj_pano->max_radius;
	return VS_OK;
}


// ---------- GetPanoRadiusMin ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoRadiusMin(FLOAT *min_radius)
{
	*min_radius = pi_obj_pano->min_radius;
	return VS_OK;
}


#ifdef GET_PANO_EXPOSURE

// ---------- GetPanoExposureAoi ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoExposureAoi(FLOAT *min_aoi, FLOAT *max_aoi)
{
	*min_aoi = pi_obj_pano->exposure_aoi_min;
	*max_aoi = pi_obj_pano->exposure_aoi_max;
	return VS_OK;
}


// ---------- GetPanoExposureAoiMax ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoExposureAoiMax(FLOAT *max_aoi)
{
	*max_aoi = pi_obj_pano->exposure_aoi_max;
	return VS_OK;
}


// ---------- GetPanoExposureAoiMin ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoExposureAoiMin(FLOAT *min_aoi)
{
	*min_aoi = pi_obj_pano->exposure_aoi_min;
	return VS_OK;
}


// ---------- GetPanoExposureAoiShape ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoExposureAoiShape(UINT *aoi_shape)
{
	*aoi_shape = pi_obj_pano->exposure_aoi_shape;
	return VS_OK;
}


// ---------- GetPanoExposureAoiZone ----------
/*!
\brief Get
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoExposureAoiZone(UINT *aoi_zone)
{
	*aoi_zone = pi_obj_pano->exposure_aoi_zone;
	return VS_OK;
}

#endif


#ifdef GET_PANO_FOCUS

// ---------- GetPanoFocusAoi ----------
/*!
\brief Get min aoi radii
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoFocusAoi(FLOAT *min_aoi, FLOAT *max_aoi)
{
	*min_aoi = pi_obj_pano->focus_aoi_min;
	*max_aoi = pi_obj_pano->focus_aoi_max;
	return VS_OK;
}


// ---------- GetPanoFocusAoiMin ----------
/*!
\brief Get min aoi radius
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoFocusAoiMin(FLOAT *min_aoi)
{
	*min_aoi = pi_obj_pano->focus_aoi_min;
	return VS_OK;
}


// ---------- GetPanoFocusAoiMax ----------
/*!
\brief Get max aoi radius
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::GetPanoFocusAoiMax(FLOAT *max_aoi)
{
	*max_aoi = pi_obj_pano->focus_aoi_max;
	return VS_OK;
}


// ---------- GetPanoFocusAoiRotate ----------
/*!
\brief Get AOI Angle
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ObjConfig::GetPanoFocusAoiRotate(FLOAT *angle)
{
	*angle = pi_obj_pano->focus_aoi_rotate;
	return VS_OK;
}


// ---------- GetPanoFocusAoiSector ----------
/*!
\brief Get AOI Sector
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ObjConfig::GetPanoFocusAoiSector(FLOAT *sector)
{
	*sector = pi_obj_pano->focus_aoi_sector;
	return VS_OK;
}

#endif


// ---------- SetPanoParamById ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoParamById(UINT id, UINT ref, INT p)
{
	if ( id >= 1 && id <= MAX_PANO_PTF_LUT )
	{
		UINT index = id - 1;
		pi_obj_pano->param_lut[ref][index - 1] = p;
		return VS_OK;
	}
	return VS_ERROR;
}


// ---------- SetPanoPTF ----------
/*!
\brief 
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoPTF(FLOAT new_pan, FLOAT new_tilt, FLOAT new_FOV)
{

	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

		;

	#else


		#if VS_LIDAR

		;

		#else

			// Accelerate
			FLOAT fov_default = pi_obj_pano->max_fov;
			FLOAT fov_default_by2 = fov_default / 5;   // Zoom limit
			FLOAT max_angle = pi_obj_pano->max_angle;
			FLOAT min_angle = pi_obj_pano->min_angle;

			// Limit FOV to 0.5 zoom
			new_FOV = new_FOV > fov_default ?
					fov_default :
						(
						new_FOV < fov_default_by2 ?
							fov_default_by2 :
							new_FOV
						);

			// Limit TILT
			FLOAT fov_by2 = new_FOV / 2;
			new_tilt = new_tilt + fov_by2 > max_angle ?
					max_angle - fov_by2 :
						(
							new_tilt - fov_by2 < min_angle ?
							min_angle + fov_by2 :
							new_tilt
						);

		#endif


	#endif

	// Set pan
	pi_obj_pano->pan =
		pi_obj_pano->pan_lut[pi_obj_pano->ptf_selected_index] =
			new_pan;

	// ... tilt, and
	pi_obj_pano->tilt =
		pi_obj_pano->tilt_lut[pi_obj_pano->ptf_selected_index] =
			new_tilt;

	// ... FOV.
	pi_obj_pano->FOV =
		pi_obj_pano->fov_lut[pi_obj_pano->ptf_selected_index] =
			new_FOV;

	return VS_OK;
}


// ---------- SetPanoPTFLast ---------- 
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoPTFLast()
{
	pi_obj_pano->pan_last  = pi_obj_pano->pan;
	pi_obj_pano->tilt_last = pi_obj_pano->tilt;
	pi_obj_pano->FOV_last  = pi_obj_pano->FOV;
	return VS_ERROR;
}


// ---------- SetPanoPTFLast ---------- 
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoPTFLast(FLOAT new_pan, FLOAT new_tilt, FLOAT new_FOV)
{
	pi_obj_pano->pan_last  = new_pan;
	pi_obj_pano->tilt_last = new_tilt;
	pi_obj_pano->FOV_last  = new_FOV;
	return VS_ERROR;
}


// ---------- SetPanoPTFByIndex ---------- 
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoPTFByIndex(UINT index, FLOAT pan, FLOAT tilt, FLOAT FOV)
{
	pi_obj_pano->pan_lut[index]  = pan;
	pi_obj_pano->tilt_lut[index] = tilt;
	pi_obj_pano->fov_lut[index]  = FOV;
	return VS_ERROR;
}


// ---------- SetPanoPTF ---------- 
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoPTFById(
		UINT  id,
		FLOAT new_pan,
		FLOAT new_tilt,
		FLOAT new_FOV
	)
{

	if ( id >= 1 && id <= MAX_PANO_PTF_LUT )
	{
		pi_obj_pano->ptf_selected_index = id - 1;
		INT result = SetPanoPTF(new_pan, new_tilt, new_FOV);
		return VS_OK;
	}

	return VS_ERROR;
}


// ---------- SetPanoPtfSelectedById ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoPtfSelectedById(UINT id)
{
	if ( id >= 1 && id <= MAX_PANO_PTF_LUT )
	{
		UINT selected = id - 1;
		pi_obj_pano->ptf_selected_index = selected;
		pi_obj_pano->pan  = pi_obj_pano->pan_lut[selected];
		pi_obj_pano->tilt = pi_obj_pano->tilt_lut[selected];
		pi_obj_pano->FOV  = pi_obj_pano->fov_lut[selected];
		return VS_OK;
	}
	return VS_ERROR;
}


// ---------- SetPanoAngles ----------
/*!
\brief Set parameter
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoAngles(FLOAT min_angle, FLOAT max_angle)
{
	// pi_obj_pano->min_angle = -90.0f + pi_obj_pano->min_radius * (max_angle + 90.0f);
	pi_obj_pano->max_angle = max_angle;
	pi_obj_pano->min_angle = min_angle;
	return VS_OK;
}


// ---------- SetPanoAngleMax ----------
/*!
\brief Set parameter
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoAngleMax(FLOAT max_angle)
{
	pi_obj_pano->DerivePanoSetup(max_angle);
	return VS_OK;
}


// ---------- SetPanoAspectRatio ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoAspectRatio(FLOAT aspect_ratio)
{
	pi_obj_pano->aspect_ratio = aspect_ratio;
	return VS_OK;
}


// ---------- SetPanoAspectRatio ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoAspectRatio(FLOAT width, FLOAT height)
{

	// local
	INT result = VS_OK;

	// IF square THEN
	if ( height == width )
	{
		result = SetPanoAspectRatio(1);
	}
	// ELSE IF exactly 2 x 1 THEN
	else if ( height == width / 2 )
	{
		result = SetPanoAspectRatio(1);
	}
	// ELSE
	else
	{
		result = SetPanoAspectRatio((FLOAT)height / (FLOAT)width);
	}

	return result;
}


// ---------- SetPanoAtlasPageLid ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoAtlasPageLid(INT lid)
{
	pi_obj_pano->pano_texture_lid = lid;
	return VS_OK;
}


// ---------- SetPanoAutoPan ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK

\note not used

*/
INT ObjConfig::SetPanoAutoPan(INT ap)
{
	pi_obj_pano->auto_pan = ap;
	return VS_OK;
}


// ---------- SetPanoCamHOffset ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoCamHOffset(INT cam_hoffset)
{
	pi_obj_pano->cam_hoffset = cam_hoffset;
	return VS_OK;
}


// ---------- SetPanoCamVOffset ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoCamVOffset(INT cam_voffset)
{
	pi_obj_pano->cam_voffset = cam_voffset;
	return VS_OK;
}


// ---------- SetPanoCamWidth ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoCamWidth(INT cam_width)
{
	pi_obj_pano->cam_width = cam_width;
	return VS_OK;
}


// ---------- SetPanoCamHeight ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoCamHeight(INT cam_height)
{
	pi_obj_pano->cam_height = cam_height;
	return VS_OK;
}


// ---------- SetPanoPanVertexBuffer ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoPanVertexBuffer(BOOL pan)
{
	pi_obj_pano->pan_vertex_buffer = pan;
	return VS_OK;
}


// ---------- SetPanoUpdated ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoUpdated(BOOL updated)
{
	pi_obj_pano->updated = updated;
	return VS_OK;
}


// ---------- SetPanoDefault ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoDefault()
{
	pi_obj_pano->auto_pan  = 0; // not used
	   
	pi_obj_pano->pan  = 
		pi_obj_pano->pan_lut[pi_obj_pano->ptf_selected_index] =
			pi_obj_pano->pan_lut_default[pi_obj_pano->ptf_selected_index];

	pi_obj_pano->tilt = 
		pi_obj_pano->tilt_lut[pi_obj_pano->ptf_selected_index] =
			pi_obj_pano->tilt_lut_default[pi_obj_pano->ptf_selected_index];

	pi_obj_pano->FOV =
		pi_obj_pano->fov_lut[pi_obj_pano->ptf_selected_index] =
			pi_obj_pano->fov_lut_default[pi_obj_pano->ptf_selected_index];

	return VS_OK;
}


// ---------- SetPanoDefault ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoDefault(BOOL PT)
{
	if ( PT )
	{
		pi_obj_pano->pan =
			pi_obj_pano->pan_lut[pi_obj_pano->ptf_selected_index] =
			pi_obj_pano->pan_lut_default[pi_obj_pano->ptf_selected_index];

		pi_obj_pano->tilt =
			pi_obj_pano->tilt_lut[pi_obj_pano->ptf_selected_index] =
			pi_obj_pano->tilt_lut_default[pi_obj_pano->ptf_selected_index];
	}
	else
	{
		pi_obj_pano->FOV =
			pi_obj_pano->fov_lut[pi_obj_pano->ptf_selected_index] =
			pi_obj_pano->fov_lut_default[pi_obj_pano->ptf_selected_index];
	}

	return VS_OK;
}


// ---------- SetPanoDefault ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoDefault(UINT index, BOOL PT)
{
	if ( PT )
	{
		pi_obj_pano->pan =
			pi_obj_pano->pan_lut[index] =
			pi_obj_pano->pan_lut_default[index];

		pi_obj_pano->tilt =
			pi_obj_pano->tilt_lut[index] =
			pi_obj_pano->tilt_lut_default[index];
	}
	else
	{
		pi_obj_pano->FOV =
			pi_obj_pano->fov_lut[index] =
			pi_obj_pano->fov_lut_default[index];
	}

	return VS_OK;
}


// ---------- SetPanoDefaultUpdate ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoDefaultUpdate()
{
	UINT index = pi_obj_pano->ptf_selected_index;

	pi_obj_pano->pan_lut_default[index] =
		pi_obj_pano->pan =
			pi_obj_pano->pan_lut[index];

	pi_obj_pano->tilt_lut_default[index] =
		pi_obj_pano->tilt =
			pi_obj_pano->tilt_lut[index];

	pi_obj_pano->fov_lut_default[index] =
		pi_obj_pano->FOV =
			pi_obj_pano->fov_lut[index];

	return VS_OK;
}


// ---------- SetPanoOffset ----------
/*!
\brief Set parameter
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoOffset(FLOAT u_offset, FLOAT v_offset)
{
	pi_obj_pano->u_offset = u_offset;
	pi_obj_pano->v_offset = v_offset;
	return VS_OK;
}


// ---------- SetPanoPicking ----------
/*!
\brief Set
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoPicking(BOOL p)
{
	pi_obj_pano->picking = p;
	return VS_OK;
}


// ---------- SetPanoRadii ----------
/*!
\brief Set parameter
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoRadii(FLOAT min_radius, FLOAT max_radius)
{
	pi_obj_pano->min_radius = min_radius;
	pi_obj_pano->max_radius = max_radius;
	return VS_OK;
}


// ---------- SetPanoRadiusMax ----------
/*!
\brief Set pano radius max
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoRadiusMax(FLOAT max_radius)
{
	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

		;

	#else

		// resize fov scalar
		FLOAT fov_scalar =
			( max_radius - pi_obj_pano->min_radius ) / 
			( pi_obj_pano->max_radius - pi_obj_pano->min_radius );

		// set LUT FOV
		for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
		{
			FLOAT new_fov = pi_obj_pano->fov_lut[i] * fov_scalar;
			pi_obj_pano->fov_lut[i] = new_fov;
			//printf("pi_obj_pano->fov_lut[i] = %10.4f\n", new_fov);
		}

		// set current FOV
		FLOAT new_fov = pi_obj_pano->FOV * fov_scalar;
		pi_obj_pano->FOV = new_fov;
		//printf("pi_obj_pano->FOV = %10.4f\n", new_fov);

		// set new maximum radius
		pi_obj_pano->max_radius = max_radius;

		// ---------- Derive ----------

		// derive min_angle / max_angle == min_radius / max_radius
		pi_obj_pano->min_angle = pi_obj_pano->max_angle - pi_obj_pano->FOV;
		//printf("pi_obj_pano->min_angle = %10.4f\n", pi_obj_pano->min_angle);

		// derive Tilt
		pi_obj_pano->tilt = pi_obj_pano->tilt_derived = (pi_obj_pano->max_angle + pi_obj_pano->min_angle) / 2;
		//printf("pi_obj_pano->tilt = %10.4f\n", pi_obj_pano->tilt);

		// derive FOV
		pi_obj_pano->FOV_derived = pi_obj_pano->max_fov = pi_obj_pano->FOV;

		// derive Pan LUT
		for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
		{
			// Derive Tilt
			pi_obj_pano->tilt_lut[i] = pi_obj_pano->tilt;
		}

		// check legal AOI
		pi_obj_pano->CheckAoiMinimax();

	#endif

	return VS_OK;
}


// ---------- SetPanoRadiusMin ----------
/*!
\brief Set pano radius min
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoRadiusMin(FLOAT min_radius)
{

	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

		;

	#else

		// resize fov scalar
		FLOAT fov_scalar =
			( pi_obj_pano->max_radius - min_radius ) /
			( pi_obj_pano->max_radius - pi_obj_pano->min_radius );

		// set LUT FOV
		for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
		{
			FLOAT new_fov = pi_obj_pano->fov_lut[i] * fov_scalar;
			pi_obj_pano->fov_lut[i] = new_fov;
		}

		// set current FOV
		FLOAT new_fov = pi_obj_pano->FOV * fov_scalar;
		pi_obj_pano->FOV = new_fov;

		// set new minmum radius
		pi_obj_pano->min_radius = min_radius;

		// ---------- Derive ----------

		// derive min_angle / max_angle == min_radius / max_radius
		pi_obj_pano->min_angle = pi_obj_pano->max_angle - pi_obj_pano->FOV;

		// derive Tilt
		pi_obj_pano->tilt = pi_obj_pano->tilt_derived = (pi_obj_pano->max_angle + pi_obj_pano->min_angle) / 2;

		// derive FOV
		pi_obj_pano->FOV_derived = pi_obj_pano->max_fov = pi_obj_pano->max_angle - pi_obj_pano->min_angle;

		// derive tilt lut
		for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
		{
			// derive Tilt
			pi_obj_pano->tilt_lut[i] = pi_obj_pano->tilt;
		}

		// check legal AOI
		pi_obj_pano->CheckAoiMinimax();

	#endif

	return VS_OK;
}


// ---------- SetPanoExposureAoi ----------
/*!
\brief Set parameter
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoExposureAoi(FLOAT min_aoi, FLOAT max_aoi)
{
	pi_obj_pano->exposure_aoi_min = min_aoi;
	pi_obj_pano->exposure_aoi_max = max_aoi;

	// Check legal AOI
	pi_obj_pano->CheckAoiMinimax();

	// Done
	return VS_OK;
}


// ---------- SetPanoExposureAoiMax ----------
/*!
\brief Set pano aoi max
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoExposureAoiMax(FLOAT max_aoi)
{
	pi_obj_pano->exposure_aoi_max = max_aoi;

	// Check legal AOI
	pi_obj_pano->CheckAoiMinimax();

	// Done
	return VS_OK;
}


// ---------- SetPanoExposureAoiMin ----------
/*!
\brief Set pano aoi min
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoExposureAoiMin(FLOAT min_aoi)
{
	pi_obj_pano->exposure_aoi_min = min_aoi;
	pi_obj_pano->CheckAoiMinimax();
	return VS_OK;
}


// ---------- SetPanoAoiZone ----------
/*!
\brief Set AOI Zone
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ObjConfig::SetPanoExposureAoiZone(UINT aoi_zone)
{
	INT result = pi_obj_pano->CheckAoiZone(aoi_zone);
	if (result == VS_ERROR)
		pi_obj_pano->exposure_aoi_zone = AOI_ALL;
	else
		pi_obj_pano->exposure_aoi_zone = aoi_zone;
	return result;
}


// ---------- SetPanoFocusAo ----------
/*!
\brief Set min aoi radius
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoFocusAoi(FLOAT min_aoi, FLOAT max_aoi)
{
	pi_obj_pano->focus_aoi_min = min_aoi;
	pi_obj_pano->focus_aoi_max = max_aoi;
	return VS_OK;
}


// ---------- SetPanoFocusAoiMin ----------
/*!
\brief Set min aoi radius
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoFocusAoiMin(FLOAT min_aoi)
{
	pi_obj_pano->focus_aoi_min = min_aoi;
	return VS_OK;
}


// ---------- SetPanoFocusAoiMax ----------
/*!
\brief Set max aoi radius
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetPanoFocusAoiMax(FLOAT max_aoi)
{
	pi_obj_pano->focus_aoi_max = max_aoi;
	return VS_OK;
}


// ---------- SetPanoFocusAoiRotate ----------
/*!
\brief Set AOI Angle
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ObjConfig::SetPanoFocusAoiRotate(FLOAT angle)
{
	pi_obj_pano->focus_aoi_rotate = angle;
	return VS_OK;
}


// ---------- SetPanoFocusAoiSector ----------
/*!
\brief Set AOI Sector
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ObjConfig::SetPanoFocusAoiSector(FLOAT sector)
{
	pi_obj_pano->focus_aoi_sector = sector;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- SetConfigureAllBitmask ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS _OK
*/
INT ObjConfig::SetConfigureAllBitmask(BYTE setup_bitmask)
{
	gfx->SetGlobalConfigureBitmask(
			setup_bitmask,
			""
		);
	return VS_OK;
}
