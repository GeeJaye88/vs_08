////////////////////////////////////////////////////////////////////////////////

// ---------- vs_obj_vptf.hpp ----------
/*!
\file vs_obj_vptf.hpp
\brief
\author Gareth Edwards
*/


// ---------- ObjVptf definitions ----------

#define V_INSIDE_OUT ( pi_obj_pano->v_inside_out > 0 )
#define U_FLIP_LOGIC ( pi_obj_pano->u_flip ? -1.0f :  1.0f )
#define V_FLIP_LOGIC ( pi_obj_pano->v_flip ?  1.0f : -1.0f )


// ---------- ObjVptf class ----------
class ObjConfig::ObjVptf
{

public:

	// ---- c/dtor
		ObjVptf();
		virtual ~ObjVptf();


	// ---- methods
		INT Initialise();
		INT Reset();
		INT Read(CHAR *);
		INT Write();
		INT Configure(vs_system::VNode *);
		INT DerivePanoSetup(FLOAT);
		INT DefaultPanoSetup();
		INT CheckAoiMinimax();
		INT CheckAoiZone(UINT);
	

	// ---- properties ----


	// ---- updated/edited state flag
		BOOL   updated;


	// ---- active
		UINT  ptf_active_id = 2;
		UINT  ptf_previous_id = ptf_active_id;
		FLOAT ptf_aspect_ratio = 1;


	// ---- over
		UINT  ptf_over_id = 0;


	// ---- CONFIG file
		CHAR   config_paf_hdl[VS_MAXCHARPATHLEN];
		VNode* config_hdl;

	
	// ---- CONFIG set sphere parameters
		UINT   sphere_rows;
		UINT   sphere_columns;
		FLOAT  sphere_radius;


	// ---- CONFIG set cylinder parameters
		UINT   cylinder_rows;
		UINT   cylinder_columns;
		FLOAT  cylinder_x_rotate;
		FLOAT  cylinder_scale;


	// ---- CONFIG P parameters
		BOOL   pan_vertex_buffer;
		FLOAT  pan_offset;

	
	// ---- global PTF
		FLOAT  pan;
		FLOAT  tilt;
		FLOAT  FOV;

	
	// ---- last PTF set
		FLOAT  pan_last;
		FLOAT  tilt_last;
		FLOAT  FOV_last;


	// ---- PTF derived
		FLOAT  pan_derived;
		FLOAT  tilt_derived;
		FLOAT  FOV_derived;
		
		
	// ---- CONFIG PTF LUT
		UINT   ptf_selected_index;

		FLOAT  pan_default;
		FLOAT  pan_lut[MAX_PANO_PTF_LUT];
		FLOAT  pan_lut_default[MAX_PANO_PTF_LUT];

		FLOAT  tilt_default;
		FLOAT  tilt_lut[MAX_PANO_PTF_LUT];
		FLOAT  tilt_lut_default[MAX_PANO_PTF_LUT];

		FLOAT  FOV_default;
		FLOAT  fov_lut[MAX_PANO_PTF_LUT];
		FLOAT  fov_lut_default[MAX_PANO_PTF_LUT];


	// ---- PTF state
		BOOL   picking;
		INT    auto_pan; // not used


	// ---- AUTO VPTF

		// ---- housekeeeping
			UINT   auto_vptf_id         = 0;
			UINT   auto_vptf_lut_offset = 2;
			UINT   auto_vptf_top        = 0;
			UINT   auto_vptf_from_id    = 0;
			UINT   auto_vptf_too_id     = 0;
			UINT   auto_vptf_as_id      = 0;

		// ---- flags
			BOOL   auto_vptf_running = FALSE;
			BOOL   auto_vptf_start   = FALSE;
			BOOL   auto_vptf_stop    = FALSE;
			BOOL   auto_vptf_sosi    = FALSE;
			BOOL   auto_vptf_slide   = FALSE;
			BOOL   auto_vptf_slow    = FALSE;
			BOOL   auto_vptf_forward = TRUE;
			BOOL   auto_vptf_pause   = FALSE;
			BOOL   auto_vptf_shuffle = FALSE;

		// ---- store AutoPTFUpdate interpolant
			FLOAT  auto_vptf_from_mu   = 0;

		// ---- auto paremmeters
			FLOAT  auto_vptf_ms_speed_scalar = 0.25f;

		// ----- auto millisecond
			DWORD  auto_vptf_ms_start  = 0;
			DWORD  auto_vptf_ms_now    = 0;
			DWORD  auto_vptf_ms_pause_elapsed = 0;

		// ---- auto snapshot
			struct AutoSnapshot
			{
				FLOAT pan = 0, tilt = 0, fov = 0;
				FLOAT dist = 0;
				UINT  id = 0;
				BOOL  switched_on = TRUE;
			} auto_ptf_store, auto_ptf_snapshot[10]; // +1 too wraparound

			std::vector<AutoSnapshot> v_auto_ptf_store;
			std::vector<AutoSnapshot> v_auto_ptf_snapshots[10];


	// ---- CONFIG param 1
		INT    param_lut[20][MAX_PANO_PTF_LUT];        // was MAX_PANO_PARAM WILMA6
		INT    param_lut_default[20][MAX_PANO_PTF_LUT];


	// ----  camera image parameters
		INT    cam_hoffset;
		INT    cam_voffset;
		INT    cam_width;
		INT    cam_height;

	
	// ---- CONFIG Geometry
		FLOAT  max_angle;

	
	// ---- derived geometry
		FLOAT  min_angle;
		FLOAT  max_fov;
		FLOAT  angle_at_unit_radius;

	
	// ---- mapping
		BOOL   v_inside_out;
		BOOL   u_flip;
		BOOL   v_flip;
		FLOAT  aspect_ratio;
		FLOAT  min_radius;
		FLOAT  max_radius;
		FLOAT  u_offset;
		FLOAT  v_offset;


	// ---- exposure aoi
		FLOAT  exposure_aoi_min;
		FLOAT  exposure_aoi_max;
		UINT   exposure_aoi_zone;
		UINT   exposure_aoi_shape;


	// ---- focus aoi
		FLOAT  focus_aoi_min;
		FLOAT  focus_aoi_max;
		FLOAT  focus_aoi_rotate;
		FLOAT  focus_aoi_sector;


	// ... and GFX (graphics engine) atlas page (pano texture) logical id
		INT   pano_texture_lid;


}; // ObjVptfParam class


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
ObjConfig::ObjVptf::ObjVptf()
{
	INT result = Initialise();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
ObjConfig::ObjVptf::~ObjVptf()
{
	;
}


// ---------- Initialise ----------
/*!
\brief Initialise parameters
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT ObjConfig::ObjVptf::Initialise()
{

	// ---- config path filename
		strcpy(config_paf_hdl, "");
		config_hdl     = NULL;

	// ---- active
		ptf_active_id = 2;
		ptf_previous_id = ptf_active_id;
		ptf_aspect_ratio = 1;

	// ---- over
		ptf_over_id = 0;

	// ---- PTF state
		ptf_selected_index = 0;
		picking  = FALSE;

		auto_pan = 0; // not used

	//----  camera image parameters
		cam_hoffset = 0;
		cam_voffset = 0;
		cam_width   = 3684;
		cam_height  = 3684;

	//----  reset config properties
		Reset();

	//----  ... but not the atlas page (pano texture) logical id
		pano_texture_lid = 0;

	return VS_OK;
}


// ---------- Reset ----------
/*!
\brief Reset parameters
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT ObjConfig::ObjVptf::Reset()
{
	// updated/edited state flag
	updated = FALSE;

	
	// CONFIG - set sphere parameters
	sphere_rows    = 60;
	sphere_columns = 300;
	sphere_radius  = 1;

	
	// CONFIG - set cylinder parameters
	cylinder_rows     = 60;
	cylinder_columns  = 100;
	cylinder_x_rotate = 40;
	cylinder_scale    = 0.44f;


	// CONFIG - PTF parameters
	pan_vertex_buffer = FALSE;
	pan_offset = 0;

	
	// CONFIG - Global PTF
	pan_derived  = pan_default  = pan  = 0;
	tilt_derived = tilt_default = tilt = 0;
	FOV_derived  = FOV_default  = FOV  = 40;

	
	// CONFIG - PTF
	for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
	{
		pan_lut[i]  = pan;     
		tilt_lut[i] = tilt;
		fov_lut[i]  = FOV;
	}


	// CONFIG - param
	for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
	{
		for (UINT j = 0; j < MAX_PANO_PARAM; j++)
		{
			param_lut[j][i] = 0;
		}
	}


	// CONFIG - maximum look up angle for mapping
	max_angle      = 54.0f;

	
	// derived mapping
	max_fov        = 108;
	min_angle      = -54;
	angle_at_unit_radius = 0;

	
	// CONFIG - mapping
	v_inside_out = FALSE;
	u_flip       = FALSE;
	v_flip       = FALSE;
	aspect_ratio = 1.0f;
	min_radius   = 0.1f;
	max_radius   = 1.0f;
	u_offset     = 0.0f;
	v_offset     = 0.0f;


	// CONFIG - aoi
	exposure_aoi_shape = 0;
	exposure_aoi_zone  = 0;
	exposure_aoi_min   = min_radius;
	exposure_aoi_max   = max_radius;


	// Done
	return VS_OK;
}


// ---------- Read ----------
/*!
\brief Read parameters
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT ObjConfig::ObjVptf::Read(CHAR *paf)
{
	// Replace '\' with '/'
	UINT len_paf = strlen(paf);
	for (UINT i = 0; i<len_paf; i++) paf[i] = paf[i] == '\\' ? '/' : paf[i];

	// IF file exists THEN
	if (GetFileAttributes(paf) != INVALID_FILE_ATTRIBUTES)
	{
		// Read
		vs_system::VNodeIO vio;
		vs_system::VNode *root = vio.Read(paf);


		// IF read HDL ok THEN
		if ( root != NULL )
		{
			// local
			INT result = VS_OK;

			// cleanup old data
			if ( config_hdl != NULL )
				result = config_hdl->Delete();

			// reset
			result = Reset();

			// configure
			result = Configure(root);

			// store
			strcpy(config_paf_hdl, paf);

			// note: config_hdl set == root in Configure method

			// ---- TEMP REPORT ----
			{
				printf("Read config file from:\n");
				printf("   %s\n", config_paf_hdl);
			}

			// done
			return VS_OK;
		}

		// ELSE read HDL ok FAILED
		else
		{

			// ---- TEMP REPORT ----
			{
				printf("VS_VNODE_READFILEFAILED...\n\n");
				printf("FAILED to read config file from:\n");
				printf("   %s\n", config_paf_hdl);
			}

			// done
			return VS_VNODE_READFILEFAILED;
		}

	}

	// ELSE file does NOT exist
	else
	{

		// ---- TEMP REPORT ----
		{
			printf("VS_VNODE_OPENFILEFAILED...\n\n");
			printf("FAILED to read config file from:\n");
			printf("   %s\n", config_paf_hdl);
		}

		// Done
		return VS_VNODE_OPENFILEFAILED;
	}


	// ---- TEMP REPORT ----
	{
		printf("File does not exist...\n\n");
		printf("FAILED to read config file from:\n");
		printf("   %s\n", config_paf_hdl);
	}

	// Done
	return VS_ERROR;
}


// ---------- Write ----------
/*!
\brief Write parameters
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT ObjConfig::ObjVptf::Write()
{

	// ---- file exists ?
	if ( GetFileAttributes(config_paf_hdl) != INVALID_FILE_ATTRIBUTES )
	{

		if (config_hdl != NULL)
		{

			if (config_hdl->SearchName("Mapping"))
			{

				// --- map
					CHAR *name[] =
					{
						"V-Inside-Out",
						"H-Flip",     "V-Flip",
						"Max-FOV",    "Max-Angle",
						"Min-Radius", "Max-Radius",
						"U-Offset",   "V-Offset",
						"X-Offset",   "Y-Offset"
					};
					FLOAT f_v_inside_out = (FLOAT)v_inside_out;
					FLOAT f_u_flip = (FLOAT)u_flip;
					FLOAT f_v_flip = (FLOAT)v_flip;
					FLOAT *param[] =
					{
						&f_v_inside_out,
						&f_u_flip,    &f_v_flip,
						&max_fov,     &max_angle,
						&min_radius,  &max_radius,
						&u_offset,    &v_offset,
						&u_offset,    &u_offset
					};


				// ---- local scope
					vs_system::VNode *p = NULL;
					UINT num_param = sizeof(param) / sizeof(FLOAT *);
					CHAR value[VS_MAXCHARNAME];


				// ---- set
					for (UINT i = 0; i < num_param; i++)
					{
						p = config_hdl->SearchName(name[i]);
						if (p != NULL)
						{
							sprintf(value, "%f", *param[i]);
							p->SetValue(value);
						}
					}

			}


			// ---- local
				vs_system::VNode *c = NULL;


			// ---- PTF
				if ( c = config_hdl->SearchName("PTF") )
				{

					vs_system::VNode *p = NULL;
					CHAR name[VS_MAXCHARNAME];
					CHAR value[VS_MAXCHARNAME];

					if ( p = config_hdl->SearchName("Pan-Offset") )
					{
						sprintf(value, "%f", pan_offset);
						p->SetValue(value);
					}

					if ( p = config_hdl->SearchName("Pan-VB") )
					{
						sprintf(value, "%d", pan_vertex_buffer);
						p->SetValue(value);
					}

					for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
					{
						sprintf(name, "%d", i + 1);
						if ( p = c->SearchName(name) )
						{
							vs_system::VNode *q = NULL;
							if ( q = p->SearchName("P0")   ) { sprintf(value, "%d", param_lut[0][i]);   q->SetValue(value); }
							if ( q = p->SearchName("P1")   ) { sprintf(value, "%d", param_lut[1][i]);   q->SetValue(value); }
							if ( q = p->SearchName("Pan")  ) { sprintf(value, "%f", pan_lut[i] + pan_offset); q->SetValue(value); }
							if ( q = p->SearchName("Tilt") ) { sprintf(value, "%f", tilt_lut[i]); q->SetValue(value); }
							if ( q = p->SearchName("FOV")  ) { sprintf(value, "%f", fov_lut[i]);  q->SetValue(value); }
						}
					}
				}


			// ---- PTZ
				if ( c = config_hdl->SearchName("PTZ") )
				{

					vs_system::VNode *p = NULL;
					CHAR name[VS_MAXCHARNAME];
					CHAR value[VS_MAXCHARNAME];

					for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
					{
						sprintf(name, "%d", i + 1);
						if ( p = c->SearchName(name) )
						{
							vs_system::VNode *q = NULL;
							if ( q = p->SearchName("P0")   ) { sprintf(value, "%d", param_lut[0][i]);   q->SetValue(value); }
							if ( q = p->SearchName("P1")   ) { sprintf(value, "%d", param_lut[1][i]);   q->SetValue(value); }
							if ( q = p->SearchName("Pan")  ) { sprintf(value, "%f", pan_lut[i]);  q->SetValue(value); }
							if ( q = p->SearchName("Tilt") ) { sprintf(value, "%f", tilt_lut[i]); q->SetValue(value); }
							if ( q = p->SearchName("Zoom") ) { sprintf(value, "%f", fov_lut[i]);  q->SetValue(value); }
						}
					}
				}

		}


		// ---- write config file
			vs_system::VNodeIO vio;
			INT result = vio.Write(config_paf_hdl, config_hdl);


		// ---- report
			{
				CHAR msg[128];
				if ( result == VS_OK )
					sprintf(msg, "Saved obj config file to %s\n", config_paf_hdl);
				else
					sprintf(msg, "Failed to save  obj config file to %s\n", config_paf_hdl);
				OutputDebugString(msg);
			}


		// ---- write not successful ?
			if ( result != VS_OK ) return VS_ERROR;



		// ---- write successful

			// default PTF now set to PTF
			for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
			{
				pan_lut_default[i]  = pan_lut[i];
				tilt_lut_default[i] = tilt_lut[i];
				fov_lut_default[i]  = fov_lut[i];
			}


			// default param set to param
			for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
			{
				for (UINT j = 0; j < MAX_PANO_PARAM; j++)
				{
					param_lut_default[j][i] = param_lut[j][i];
					param_lut_default[j][i] = param_lut[j][i];
				}
			}

	}

	return VS_OK;
}


// ---------- Configure ----------
/*!
\brief Configure parameters
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ObjConfig::ObjVptf::Configure(vs_system::VNode *parameter_hdl)
{

	// ---- local
		INT result = VS_OK;


	// ---- config OK ?
		config_hdl = parameter_hdl;
		if (config_hdl != NULL)
		{

			if ( config_hdl->SearchName("PTF") )
			{
				vs_system::VNode *p = NULL;
				if ( p=config_hdl->SearchName("Pan-Offset") ) pan_offset = p->GetFloatFromValue();
				if ( p=config_hdl->SearchName("Pan-VB")     ) pan_vertex_buffer = p->GetBoolFromValue();
				CHAR name[VS_MAXCHARNAME];
				for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
				{
					sprintf(name, "%d", i + 1);
					if (p = config_hdl->SearchName(name))
					{
						vs_system::VNode *q = NULL;
						if ( q = p->SearchName("P0")   ) param_lut[0][i]   = q->GetIntFromValue();
						if ( q = p->SearchName("P1")   ) param_lut[1][i]   = q->GetIntFromValue();
						if ( q = p->SearchName("Pan")  ) pan_lut[i]  = q->GetFloatFromValue();
						if ( q = p->SearchName("Tilt") ) tilt_lut[i] = q->GetFloatFromValue();
						if ( q = p->SearchName("FOV")  ) fov_lut[i]  = q->GetFloatFromValue();
					}
				}
			}
		
			if ( config_hdl->SearchName("PTZ") )
			{
				vs_system::VNode *p = NULL;
				pan_offset        = 0;
				pan_vertex_buffer = FALSE;
				CHAR name[VS_MAXCHARNAME];
				for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
				{
					sprintf(name, "%d", i + 1);
					if (p = config_hdl->SearchName(name))
					{
						vs_system::VNode *q = NULL;
						if ( q = p->SearchName("P0")   ) param_lut[0][i]   = q->GetIntFromValue();
						if ( q = p->SearchName("P1")   ) param_lut[1][i]   = q->GetIntFromValue();
						if ( q = p->SearchName("Pan")  ) pan_lut[i]  = q->GetFloatFromValue();
						if ( q = p->SearchName("Tilt") ) tilt_lut[i] = q->GetFloatFromValue();
						if ( q = p->SearchName("Zoom") ) fov_lut[i]  = q->GetFloatFromValue();
					}
				}
			}

			if ( config_hdl->SearchName("Mapping") )
			{
				vs_system::VNode *p = NULL;
				if ( p=config_hdl->SearchName("V-Inside-Out")) v_inside_out = p->GetBoolFromValue();
				if ( p=config_hdl->SearchName("H-Flip")      ) u_flip       = p->GetBoolFromValue();
				if ( p=config_hdl->SearchName("V-Flip")      ) v_flip       = p->GetBoolFromValue();
				if ( p=config_hdl->SearchName("Max-FOV")     ) max_fov      = p->GetFloatFromValue();
				if ( p=config_hdl->SearchName("Max-Angle")   ) max_angle    = p->GetFloatFromValue();
				if ( p=config_hdl->SearchName("Min-Radius")  ) min_radius   = p->GetFloatFromValue();
				if ( p=config_hdl->SearchName("Max-Radius")  ) max_radius   = p->GetFloatFromValue();
				if ( p=config_hdl->SearchName("U-Offset")    ) u_offset     = p->GetFloatFromValue();
				if ( p=config_hdl->SearchName("V-Offset")    ) v_offset     = p->GetFloatFromValue();
				if ( p=config_hdl->SearchName("X-Offset")    ) u_offset     = p->GetFloatFromValue();
				if ( p=config_hdl->SearchName("Y-Offset")    ) v_offset     = p->GetFloatFromValue();
			}

		}

	
	// ---- setup
		result = DerivePanoSetup(max_angle);
		result = DefaultPanoSetup();

	return VS_OK;
}


// ---------- DerivePanoSetup ----------
/*!
\brief Derive pano setup
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
\note Based on max_angle and radii, derive min angle, tilt and FOV
*/
INT ObjConfig::ObjVptf::DerivePanoSetup(FLOAT max)
{

	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
		;
	#else

		// assign
		max_angle = max;


		// derive min_angle / max_angle == min_radius / max_radius
		min_angle = ( min_radius/max_radius * (max_angle+90.0f) ) - 90;


		// derive angle at unit radius
		angle_at_unit_radius = (1 / max_radius * (max_angle + 90.0f)) - 90;


		// derive Pan from default/set pan plus pan offset 
		pan = pan_derived -= pan_offset;


		// derive Tilt
		tilt = tilt_derived = (max_angle + min_angle) / 2;


		// derive FOV
		FOV = FOV_derived = max_fov = max_angle - min_angle;


		// derive Pan LUT
		for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
		{
			pan_lut[i] -= pan_offset;
		}

		// check 
		INT result = CheckAoiMinimax();


	#endif

	return VS_OK;
}


// ---------- DefaultPanoSetup ----------
/*!
\brief Default pano setup
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ObjConfig::ObjVptf::DefaultPanoSetup()
{

	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
		;
	#else

		for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
		{
			// limit FOV
			fov_lut[i] = fov_lut[i] > max_fov ? max_fov : fov_lut[i];

			// limit TILT - BUG: not working
			FLOAT fov_by2 = fov_lut[i] / 2;
			tilt_lut[i] = tilt_lut[i] + fov_by2 > max_angle ?
					max_angle - fov_by2 :
					(
						tilt_lut[i] - fov_by2 < min_angle ?
						min_angle + fov_by2 :
						tilt_lut[i]
				);

		}

	#endif


	// ---- set default PTF LUT
		for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
		{
			pan_lut_default[i]  = pan_lut[i];
			tilt_lut_default[i] = tilt_lut[i];
			fov_lut_default[i]  = fov_lut[i];
		}


	// ---- set default Param LUT
		for (UINT i = 0; i < MAX_PANO_PTF_LUT; i++)
		{
			for (UINT j = 0; j < MAX_PANO_PARAM; j++)
			{
				param_lut_default[j][i] = param_lut[j][i];
				param_lut_default[j][i] = param_lut[j][i];
			}
		}


	// ---- set PTF
		pan_default  = pan  = pan_last  = pan_lut[ptf_selected_index];
		tilt_default = tilt = tilt_last = tilt_lut[ptf_selected_index];
		FOV_default  = FOV  = FOV_last  = fov_lut[ptf_selected_index];


	return VS_OK;
}


// ---------- CheckAoiMinimax ----------
/*!
\brief Default pano setup
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ObjConfig::ObjVptf::CheckAoiMinimax()
{
	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

		exposure_aoi_max = exposure_aoi_max > 1 ? 1 : (exposure_aoi_max < 0 ? 0 : exposure_aoi_max);
		exposure_aoi_min = exposure_aoi_min > 1 ? 1 : (exposure_aoi_min < 0 ? 0 : exposure_aoi_min);
	
	#else

		exposure_aoi_min = exposure_aoi_min < min_radius ? min_radius : (exposure_aoi_min > max_radius ? max_radius : exposure_aoi_min);
		exposure_aoi_max = exposure_aoi_max < exposure_aoi_min ? exposure_aoi_min : exposure_aoi_max;
		exposure_aoi_max = exposure_aoi_max > max_radius ? max_radius : exposure_aoi_max;
	
	#endif

	return VS_OK;
}


// ---------- CheckAoiZone ----------
/*!
\brief Default pano setup
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT ObjConfig::ObjVptf::CheckAoiZone(UINT zone)
{

	#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)

		switch ( zone )
		{
			case AOI_ALL:
			case AOI_ANNULAR:
				break;
			default:
				exposure_aoi_zone = AOI_ALL;
				return VS_ERROR;
		}

	#else

		switch (zone)
		{
			case AOI_ALL:
			case AOI_ANNULAR:
			case AOI_TOP:
			case AOI_LEFT:
			case AOI_RIGHT:
			case AOI_BOTTOM:
				break;
			default:
				exposure_aoi_zone = AOI_ALL;
				return VS_ERROR;
		}

	#endif

	return VS_OK;
}
