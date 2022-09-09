////////////////////////////////////////////////////////////////////////////////

// ---------- vs_cam_props.cpp ----------
/*!
\file vs_cam_props.cpp
\brief Implementation of the CamProps class.
\author Gareth Edwards 
\note 2017
*/

// ---- include ----
#include "../header/vs_cam_props.h"


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_CamProps class ----------

class CamProps::PI_CamProps
{

public:


	// ---- structs

	struct Annular_Aoi
	{
		BOOL  display = FALSE;
		UINT  zone    = 1;      // id: 0 to 5
		FLOAT min     = 0.5f;   // parametric radii
		FLOAT max     = 1.0f;
	};


	struct Annular_Focus
	{
		BOOL  display = FALSE;
		FLOAT min     = 0.5f;   // parametric radii
		FLOAT max     = 1.0f;
		FLOAT rotate  = 0;      // y up
		FLOAT sector  = 5;      // centred on y up
	};


	struct Aoi
	{
		BOOL  display = FALSE;
		UINT  zone    = 1;      // id: 0 or 1
		FLOAT width   = 1;      // parametric dimension
		FLOAT height  = 1;
		FLOAT left    = 0;      // parametric location
		FLOAT right   = 1;
		FLOAT bottom  = 1;
		FLOAT top     = 0;

	};


	struct Awb
	{
		FLOAT min = 0;          // not used
		FLOAT max = 1;          // not used
	};


	struct Colour
	{
		FLOAT saturation = 1;   // range 0 to 2
	};


	struct Exposure
	{
		FLOAT bright_reference = 1;
		FLOAT time_def = 50;
		FLOAT time_ms  = 50;
		FLOAT time_max = 1000;
	};


	struct Framerate
	{
		FLOAT fps     = 9;
		FLOAT fps_def = 9;
		FLOAT fps_max = 12;
	};


	struct GainColour
	{
		FLOAT gain  = 0;        // range 0 to 1
		FLOAT red   = 0;
		FLOAT green = 0;
		FLOAT blue  = 0;
	};


	struct GainMaster
	{
		FLOAT black_level = 0;  // range 0 to 1
		FLOAT gain = 0;         // range 0 to 1
	};
	

	struct Gamma
	{
		FLOAT factor = 1.0f;    // range 0 to 2
	};


	struct Media
	{
		UINT type = 12;         // usage as per manufacturer
	};


	struct Roi                  
	{
		INT width    = 100;     // pixel dimensions
		INT height   = 100;
		INT h_offset = 0;       // pixel offset
		INT v_offset = 0;
		INT sampling = 1;       // usage as per manufacturer
	}; 


	struct Rop
	{
		BOOL h_flip = FALSE;    // usage as per manufacturer
		BOOL v_flip = FALSE;
	};


	struct Sensor
	{
		UINT pixel_clock = 100; // usage as per manufacturer
		UINT seq_buffer  = 1;   // usage as per manufacturer
	};


	struct Setup
	{

		BOOL auto_exposure  = TRUE;
		BOOL auto_framerate = TRUE;
		BOOL auto_gain      = TRUE;
		BOOL auto_wb        = TRUE;

		UINT colour_index      = 0;
		UINT exposure_index    = 0;
		UINT framerate_index   = 0;
		UINT gain_colour_index = 0;
		UINT gain_master_index = 0;
		UINT gamma_index       = 0;

	};


	// ---- constructor
		PI_CamProps()
		{
			;
		}


	// ---- destructor
		~PI_CamProps()
		{
			;
		}


	// ---- VS object pointers
		vs_system::AppDependency *_inj_app_dependency = NULL;


	// ---- config file
		VNode* _config_hdl = NULL;


	// ---- global states
		BOOL  _edit_all = FALSE;
		BOOL  _read_callback_properties = FALSE;;


	// ---- properties
		Annular_Aoi   _annular_aoi;
		Annular_Focus _annular_focus;
		Aoi           _aoi;
		Awb           _awb;
		Colour        _colour[NUM_COLOUR_CONFIGS];
		Exposure      _exposure[NUM_EXPOSURE_CONFIGS];
		Framerate     _framerate[NUM_FRAMERATE_CONFIGS];
		GainMaster    _gain_master[NUM_GAIN_MASTER_CONFIGS];
		GainColour    _gain_colour[NUM_GAIN_COLOUR_CONFIGS];
		Gamma         _gamma[NUM_GAMMA_CONFIGS];
		Media         _media;
		Roi           _roi;
		Rop           _rop;
		Sensor        _sensor;
		Setup         _setup;


	// ---- reset properties
		Annular_Aoi   _reset_annular_aoi;
		Annular_Focus _reset_annular_focus;
		Aoi           _reset_aoi;
		Awb           _reset_awb;
		Colour        _reset_colour[NUM_COLOUR_CONFIGS];
		Exposure      _reset_exposure[NUM_EXPOSURE_CONFIGS];
		Framerate     _reset_framerate[NUM_FRAMERATE_CONFIGS];
		GainMaster    _reset_gain_master[NUM_GAIN_MASTER_CONFIGS];
		GainColour    _reset_gain_colour[NUM_GAIN_COLOUR_CONFIGS];
		Gamma         _reset_gamma[NUM_GAMMA_CONFIGS];
		Media         _reset_media;
		Roi           _reset_roi;
		Rop           _reset_rop;
		Sensor        _reset_sensor;
		Setup         _reset_setup;


	// ---- callbacks
		INT (*_black_level_callback)();
		INT (*_colour_callback)();
		INT (*_exposure_callback)();
		INT (*_framerate_callback)();
		INT (*_gain_master_callback)();
		INT (*_gain_colour_callback)();
		INT (*_gamma_callback)();
		INT (*_rop_callback)();
		INT (*_setup_callback)();
		INT (*_write_ini_file_callback)();


	// ---- adjust an index value to match a bool state, and vice versa

		INT AdjustSetupProperty(UINT id)
		{
			switch ( id )
			{
				case AUTO_EXPOSURE     : _setup.exposure_index    = (INT)(_setup.auto_exposure  ? 0 : 1); break;
				case AUTO_FRAMERATE    : _setup.framerate_index   = (INT)(_setup.auto_framerate ? 0 : 1); break;
				case AUTO_GAIN         : _setup.gain_master_index = (INT)(_setup.auto_gain      ? 0 : 1); break;
				case AUTO_WB           : _setup.gain_colour_index = (INT)(_setup.auto_wb        ? 0 : 1); break;

				case EXPOSURE_INDEX    : _setup.auto_exposure  = (BOOL)((INT)_setup.exposure_index    == 0 ? TRUE : FALSE); break;
				case FRAMERATE_INDEX   : _setup.auto_framerate = (BOOL)((INT)_setup.framerate_index   == 0 ? TRUE : FALSE); break;
				case GAIN_MASTER_INDEX : _setup.auto_gain      = (BOOL)((INT)_setup.gain_master_index == 0 ? TRUE : FALSE); break;
				case GAIN_COLOUR_INDEX : _setup.auto_wb        = (INT)(_setup.gain_colour_index       == 0 ? TRUE : FALSE); break;

				default :
					break;
			}

			return VS_OK;
		}


	// ---- limit low, medium and high buttons value to have ascending value

		INT LimitProperty(UINT group, UINT index)
		{

			// ---- lambda ------------------------------------------------------------
				auto limit_lmh = [](UINT index, FLOAT *p1, FLOAT *p2, FLOAT *p3)
				{
					switch ( index )
					{
						case NUM_COLOUR_CONFIGS - 4:
							*p1 = *p1 > *p2 ? *p2 : *p1;
							break;
						case NUM_COLOUR_CONFIGS - 3:
							*p2 = *p2 > *p3 ? *p3 : *p2;
							*p2 = *p2 > *p1 ? *p2 : *p1;
							break;
						case NUM_COLOUR_CONFIGS - 2:
							*p3 = *p3 > *p2 ? *p3 : *p2;
							break;
						default:
							break;
					}
				};


			// ---- jdi
				switch ( group )
				{
					case COLOUR :
						limit_lmh(index, &_colour[1].saturation, &_colour[2].saturation, &_colour[3].saturation);
						break;

					case EXPOSURE :
						limit_lmh(index, &_exposure[1].bright_reference, &_exposure[2].bright_reference, &_exposure[3].bright_reference);
						limit_lmh(index, &_exposure[1].time_ms, &_exposure[2].time_ms, &_exposure[3].time_ms);
						break;

					case FRAMERATE :
						limit_lmh(index, &_framerate[1].fps, &_framerate[2].fps, &_framerate[3].fps);
						break;

					case GAIN_COLOUR :
						//
						// NEW : ignore web limits
						//
						// limit_lmh(index, &_gain_colour[1].red,   &_gain_colour[2].red,   &_gain_colour[3].red);
						// limit_lmh(index, &_gain_colour[1].green, &_gain_colour[2].green, &_gain_colour[3].green);
						// limit_lmh(index, &_gain_colour[1].blue,  &_gain_colour[2].blue,  &_gain_colour[3].blue);
						//
						break;

					case GAIN_MASTER :
						limit_lmh(index, &_gain_master[1].gain, &_gain_master[2].gain, &_gain_master[3].gain);
						limit_lmh(index, &_gain_master[1].black_level, &_gain_master[2].black_level, &_gain_master[3].black_level);
						break;

					case GAMMA :
						limit_lmh(index, &_gamma[1].factor, &_gamma[2].factor, &_gamma[3].factor);
						break;

					default     : break;
				}

			return VS_OK;
		}


	// ---- set properties to the reset value (i.e. value stored for each when read) 

		INT ResetProperty(UINT group)
		{

			switch ( group )
			{
				case ANNULAR_AOI   : _annular_aoi   = _reset_annular_aoi;   break;
				case ANNULAR_FOCUS : _annular_focus = _reset_annular_focus; break;
				case AOI           : _aoi           = _reset_aoi; break;
				case AWB           : _awb           = _reset_awb; break;

				case COLOUR:
					for ( UINT i=0; i< NUM_COLOUR_CONFIGS; i++)
						_colour[i] = _reset_colour[i];
					break;

				case EXPOSURE:
					for (UINT i = 0; i< NUM_EXPOSURE_CONFIGS; i++)
						_exposure[i] = _reset_exposure[i];
					break;

				case FRAMERATE:
					for (UINT i = 0; i< NUM_FRAMERATE_CONFIGS; i++)
						_framerate[i] = _reset_framerate[i];
					break;

				case GAIN_COLOUR:
					for (UINT i = 0; i< NUM_GAIN_COLOUR_CONFIGS; i++)
						_gain_colour[i] = _reset_gain_colour[i];
					break;

				case GAIN_MASTER:
					for (UINT i = 0; i< NUM_GAIN_MASTER_CONFIGS; i++)
						_gain_master[i] = _reset_gain_master[i];
					break;

				case GAMMA:
					for (UINT i = 0; i< NUM_GAMMA_CONFIGS; i++)
						_gamma[i] = _reset_gamma[i];
					break;

				case MEDIA  :_media  = _reset_media; break;
				case ROI    :_roi    = _reset_roi;   break;
				case ROP    :_rop    = _reset_rop;   break;
				case SENSOR :_sensor = _reset_sensor;break;
				case SETUP  :_setup  = _reset_setup; break;
				default     : break;
			}

			return VS_OK;
		}


	// ---- store reset properties (at runtime when read)

		INT StoreProperties()
		{
			INT result = VS_OK;
			result = StoreProperty(ANNULAR_AOI);
			result = StoreProperty(ANNULAR_FOCUS);
			result = StoreProperty(AOI);
			result = StoreProperty(AWB);
			result = StoreProperty(COLOUR);
			result = StoreProperty(EXPOSURE);
			result = StoreProperty(FRAMERATE);
			result = StoreProperty(GAIN_COLOUR);
			result = StoreProperty(GAIN_MASTER);
			result = StoreProperty(GAMMA);
			result = StoreProperty(MEDIA);
			result = StoreProperty(ROI);
			result = StoreProperty(ROP);
			result = StoreProperty(SENSOR);
			result = StoreProperty(SETUP);
			return VS_OK;
		}

		INT StoreProperty(UINT group)
		{

			switch ( group )
			{
				case ANNULAR_AOI   : _reset_annular_aoi   = _annular_aoi;   break;
				case ANNULAR_FOCUS : _reset_annular_focus = _annular_focus; break;
				case AOI           : _reset_aoi           = _aoi; break;
				case AWB           : _reset_awb           = _awb; break;

				case COLOUR:
					for ( UINT i=0; i< NUM_COLOUR_CONFIGS; i++)
						_reset_colour[i] =_colour[i];
					break;

				case EXPOSURE:
					for (UINT i = 0; i< NUM_EXPOSURE_CONFIGS; i++)
						_reset_exposure[i] = _exposure[i];
					break;

				case FRAMERATE:
					for (UINT i = 0; i< NUM_FRAMERATE_CONFIGS; i++)
						_reset_framerate[i] = _framerate[i];
					break;

				case GAIN_COLOUR:
					for (UINT i = 0; i< NUM_GAIN_COLOUR_CONFIGS; i++)
						_reset_gain_colour[i] = _gain_colour[i];
					break;

				case GAIN_MASTER:
					for (UINT i = 0; i< NUM_GAIN_MASTER_CONFIGS; i++)
						_reset_gain_master[i] = _gain_master[i];
					break;

				case GAMMA:
					for (UINT i = 0; i< NUM_GAMMA_CONFIGS; i++)
						_reset_gamma[i] = _gamma[i];
					break;

				case MEDIA  : _reset_media  = _media; break;
				case ROI    : _reset_roi    = _roi;   break;
				case ROP    : _reset_rop    = _rop;   break;
				case SENSOR : _reset_sensor = _sensor;break;
				case SETUP  : _reset_setup  = _setup; break;
				default     : break;
			}

			return VS_OK;
		}

};


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
CamProps::CamProps()
{
	_pi_cam_props = new PI_CamProps();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
CamProps::~CamProps()
{
	if ( _pi_cam_props != NULL ) { delete _pi_cam_props; _pi_cam_props = NULL; }
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT CamProps::Setup(vs_system::AppDependency *app_dependency)
{

	_pi_cam_props->_inj_app_dependency = app_dependency;

	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Setup
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT CamProps::Cleanup()
{
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Read ----------
/*!
\brief Read camera properties configuration hdl file
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::Read(CHAR * hdl_paf)
{

	// ---- file exists ?
	if ( GetFileAttributes(hdl_paf) != INVALID_FILE_ATTRIBUTES )
	{

		// ---- READ config file
			vs_system::VNodeIO vio;
			VNode *config_hdl = _pi_cam_props->_config_hdl = vio.Read(hdl_paf);


		// ---- sensor config file NOT READ ? -- report && return
			if ( config_hdl == NULL )
			{
				vs_system::SysOutput *sys_output = _pi_cam_props->_inj_app_dependency->GetSysOutput();
				sys_output->MessageError(
						"FAILED to READ camera config file.",
						"File Error",
						hdl_paf
					);
				return VS_ERROR;
			}

		// ---- ELSE READ THEN --
			else
			{

				// ---- standard group name
					CHAR *std_name[NUM_STD_CONFIGS] =
					{
						"Auto",
						"Low",
						"Medium",
						"High",
						"User",
					};
					vs_system::VNode *c = NULL;
				  
				// ---- annular aoi
					VNode *v = config_hdl->SearchThis("Annular-Aoi");
					if ( v != NULL )
					{
						if (c = v->SearchName("Display")) _pi_cam_props->_annular_aoi.display = (BOOL)c->GetIntFromValue();
						if (c = v->SearchName("Zone")   ) _pi_cam_props->_annular_aoi.zone    = c->GetIntFromValue();
						if (c = v->SearchName("Min")    ) _pi_cam_props->_annular_aoi.min     = c->GetFloatFromValue();
						if (c = v->SearchName("Max")    ) _pi_cam_props->_annular_aoi.max     = c->GetFloatFromValue();
					}


				// ---- annular focus
					v = config_hdl->SearchThis("Annular-Focus");
					if ( v != NULL )
					{
						if (c = v->SearchName("Display")) _pi_cam_props->_annular_focus.display = (BOOL)c->GetIntFromValue();
						if (c = v->SearchName("Min")    ) _pi_cam_props->_annular_focus.min     = c->GetFloatFromValue();
						if (c = v->SearchName("Max")    ) _pi_cam_props->_annular_focus.max     = c->GetFloatFromValue();
						if (c = v->SearchName("Rotate") ) _pi_cam_props->_annular_focus.rotate  = c->GetFloatFromValue();
						if (c = v->SearchName("Sector") ) _pi_cam_props->_annular_focus.sector  = c->GetFloatFromValue();
					}


				// ---- aoi
					v = config_hdl->SearchThis("Aoi");
					if ( v != NULL )
					{
						if (c = v->SearchName("Display")) _pi_cam_props->_aoi.display = (BOOL)c->GetIntFromValue();
						if (c = v->SearchName("Zone")   ) _pi_cam_props->_aoi.zone    = c->GetIntFromValue();
						if (c = v->SearchName("Height") ) _pi_cam_props->_aoi.height  = c->GetFloatFromValue();
						if (c = v->SearchName("Width")  ) _pi_cam_props->_aoi.width   = c->GetFloatFromValue();
						if (c = v->SearchName("Left")   ) _pi_cam_props->_aoi.left    = c->GetFloatFromValue();
						if (c = v->SearchName("Top")    ) _pi_cam_props->_aoi.top     = c->GetFloatFromValue();
						if (c = v->SearchName("Right")  ) _pi_cam_props->_aoi.right   = c->GetFloatFromValue();
						if (c = v->SearchName("Bottom") ) _pi_cam_props->_aoi.bottom  = c->GetFloatFromValue();
					}


				// ---- awb
					v = config_hdl->SearchThis("Awb");
					if ( v != NULL )
					{
						if (c = v->SearchName("Min")) _pi_cam_props->_awb.min = c->GetFloatFromValue();
						if (c = v->SearchName("Max")) _pi_cam_props->_awb.max = c->GetFloatFromValue();
					}


				// ---- colour
					v = config_hdl->SearchThis("Colour");
					if ( v != NULL )
					{
						for (UINT index = 0; index < NUM_COLOUR_CONFIGS; index++)
						{
							c = v->SearchThis(std_name[index]);
							if ( c != NULL )
							{
								VNode *d = NULL;
								if (d = c->SearchName("Saturation")) _pi_cam_props->_colour[index].saturation= d->GetFloatFromValue();
							}
						}
					}


				// ---- exposure
					v = config_hdl->SearchThis("Exposure");
					if ( v != NULL )
					{
						for (UINT index = 0; index < NUM_EXPOSURE_CONFIGS; index++)
						{
							c = v->SearchThis(std_name[index]);
							if ( c != NULL )
							{
								VNode *d = NULL;
								if (d = c->SearchName("Bright-Reference")) _pi_cam_props->_exposure[index].bright_reference= d->GetFloatFromValue();
								if (d = c->SearchName("Time-Def")        ) _pi_cam_props->_exposure[index].time_def = d->GetFloatFromValue();
								if (d = c->SearchName("Time-Ms")         ) _pi_cam_props->_exposure[index].time_ms  = d->GetFloatFromValue();
								if (d = c->SearchName("Time-Max")        ) _pi_cam_props->_exposure[index].time_max = d->GetFloatFromValue();
							}
						}
					}


				// ---- framerate
					v = config_hdl->SearchThis("Framerate");
					if ( v != NULL )
					{
						for (UINT index = 0; index < NUM_FRAMERATE_CONFIGS; index++)
						{
							c = v->SearchThis(std_name[index]);
							if ( c != NULL )
							{
								VNode *d = NULL;
								if (d = c->SearchName("Fps")    ) _pi_cam_props->_framerate[index].fps     = d->GetFloatFromValue();
								if (d = c->SearchName("Fps-Def")) _pi_cam_props->_framerate[index].fps_def = d->GetFloatFromValue();
								if (d = c->SearchName("Fps-Max")) _pi_cam_props->_framerate[index].fps_max = d->GetFloatFromValue();
							}
						}
					}
			

				// ---- gain colour
					v = config_hdl->SearchThis("Gain-Colour");
					if ( v != NULL )
					{
						CHAR *gain_name[NUM_GAIN_COLOUR_CONFIGS] =
						{
							"Auto",
							"Sunshine",
							"Cloudy",
							"Overcast",
							"Bulb",
							"Fluorescent",
							"User"
						};
						for (UINT index = 0; index < NUM_GAIN_COLOUR_CONFIGS; index++)
						{
							c = v->SearchThis(gain_name[index]);
							if ( c != NULL )
							{
								VNode *d = NULL;
								if (d = c->SearchName("Gain") ) _pi_cam_props->_gain_colour[index].gain  = d->GetFloatFromValue();
								if (d = c->SearchName("Red")  ) _pi_cam_props->_gain_colour[index].red   = d->GetFloatFromValue();
								if (d = c->SearchName("Green")) _pi_cam_props->_gain_colour[index].green = d->GetFloatFromValue();
								if (d = c->SearchName("Blue") ) _pi_cam_props->_gain_colour[index].blue  = d->GetFloatFromValue();
							}
						}
					}


				// ---- gain master
					v = config_hdl->SearchThis("Gain-Master");
					if ( v != NULL )
					{
						for (UINT index = 0; index < NUM_GAIN_MASTER_CONFIGS; index++)
						{
							c = v->SearchThis(std_name[index]);
							if ( c != NULL )
							{
								VNode *d = NULL;
								if (d = c->SearchName("Black-Level")) _pi_cam_props->_gain_master[index].black_level= d->GetFloatFromValue();
								if (d = c->SearchName("Gain")       ) _pi_cam_props->_gain_master[index].gain = d->GetFloatFromValue();
							}
						}
					}


				// ---- gamma
					v = config_hdl->SearchThis("Gamma");
					if ( v != NULL )
					{
						for (UINT index = 0; index < NUM_GAMMA_CONFIGS; index++)
						{
							c = v->SearchThis(std_name[index]);
							if ( c != NULL )
							{
								VNode *d = NULL;
								if (d = c->SearchName("Factor")) _pi_cam_props->_gamma[index].factor = d->GetFloatFromValue();
							}
						}
					}


				// ---- media
					v = config_hdl->SearchThis("Media");
					if ( v != NULL && ! _pi_cam_props->_read_callback_properties )
					{
						if (c = v->SearchName("Type")) _pi_cam_props->_media.type = c->GetIntFromValue();
					}


				// ---- roi
					v = config_hdl->SearchThis("Roi");
					if ( v != NULL && ! _pi_cam_props->_read_callback_properties )
					{
						if (c = v->SearchName("H-Off")   ) _pi_cam_props->_roi.h_offset = c->GetIntFromValue();
						if (c = v->SearchName("V-Off")   ) _pi_cam_props->_roi.v_offset = c->GetIntFromValue();
						if (c = v->SearchName("Height")  ) _pi_cam_props->_roi.height   = c->GetIntFromValue();
						if (c = v->SearchName("Width")   ) _pi_cam_props->_roi.width    = c->GetIntFromValue();
						if (c = v->SearchName("Sampling")) _pi_cam_props->_roi.sampling = c->GetIntFromValue();
					}


				// ---- rop
					v = config_hdl->SearchThis("Rop");
					if ( v != NULL )
					{
						if (c = v->SearchName("H-Flip")) _pi_cam_props->_rop.h_flip = c->GetBoolFromValue();
						if (c = v->SearchName("V-Flip")) _pi_cam_props->_rop.v_flip = c->GetBoolFromValue();
					}


				// ---- sensor
					v = config_hdl->SearchThis("Sensor");
					if ( v != NULL && ! _pi_cam_props->_read_callback_properties )
					{
						if (c = v->SearchName("Pixel-Clock")) _pi_cam_props->_sensor.pixel_clock = c->GetIntFromValue();
						if (c = v->SearchName("Seq-Buffer") ) _pi_cam_props->_sensor.seq_buffer  = c->GetIntFromValue();
					}


				// ---- setup
					v = config_hdl->SearchThis("Setup");
					if ( v != NULL && ! _pi_cam_props->_read_callback_properties )
					{

						// ---- NEW : ignore read BOOL states
						//
						// if (c = v->SearchName("Auto-Exposure") ) _pi_cam_props->_setup.auto_exposure  = (BOOL)c->GetIntFromValue();
						// if (c = v->SearchName("Auto-Framerate")) _pi_cam_props->_setup.auto_framerate = (BOOL)c->GetIntFromValue();
						// if (c = v->SearchName("Auto-Gain")     ) _pi_cam_props->_setup.auto_gain      = (BOOL)c->GetIntFromValue();
						// if (c = v->SearchName("Auto-WB")       ) _pi_cam_props->_setup.auto_wb        = (BOOL)c->GetIntFromValue();

						if (c = v->SearchName("Colour")        ) _pi_cam_props->_setup.colour_index      = c->GetIntFromValue();
						if (c = v->SearchName("Exposure")      ) _pi_cam_props->_setup.exposure_index    = c->GetIntFromValue();
						if (c = v->SearchName("Framerate")     ) _pi_cam_props->_setup.framerate_index   = c->GetIntFromValue();
						if (c = v->SearchName("Gain-Colour")   ) _pi_cam_props->_setup.gain_colour_index = c->GetIntFromValue();
						if (c = v->SearchName("Gain-Master")   ) _pi_cam_props->_setup.gain_master_index = c->GetIntFromValue();
						if (c = v->SearchName("Gamma")         ) _pi_cam_props->_setup.gamma_index       = c->GetIntFromValue();
	
						// ---- NEW : adjust BOOL states to match indeces
						//
						_pi_cam_props->AdjustSetupProperty(EXPOSURE_INDEX);
						_pi_cam_props->AdjustSetupProperty(FRAMERATE_INDEX);
						_pi_cam_props->AdjustSetupProperty(GAIN_MASTER_INDEX);
						_pi_cam_props->AdjustSetupProperty(GAIN_COLOUR_INDEX);
					}

			
				// ---- report
					{
						CHAR msg[VS_MAXCHARLEN];
						sprintf(msg, "+++ Read camera properties config file from:\n");
						OutputDebugString(msg);
						sprintf(msg, "   %s\n", hdl_paf);
						OutputDebugString(msg);
					}


				// ---- store
					_pi_cam_props->StoreProperties();


				return VS_OK;
			}
		}


	// ---- REPORT
		{
			CHAR msg[VS_MAXCHARLEN];
			sprintf(msg, "+++ FAILED to read camera properties config file from:\n");
			OutputDebugString(msg);
			sprintf(msg, "   %s\n", hdl_paf);
			OutputDebugString(msg);
		}


	return VS_ERROR;
}


// ---------- Write ----------
/*!
\brief Write camera properties configuration hdl file
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::Write(CHAR * hdl_paf)
{

	// ---- IF file exists THEN --
	if ( _pi_cam_props->_config_hdl != NULL )
	{

		// ---- standard group name
			CHAR *std_name[NUM_STD_CONFIGS] =
			{
				"Auito",
				"Low",
				"Medium",
				"High",
				"User",
			};
			vs_system::VNode *c = NULL;
			VNode *config_hdl = _pi_cam_props->_config_hdl;


		// ---- annular aoi
			VNode *v = config_hdl->SearchThis("Annular-Aoi");
			if ( v != NULL )
			{
				if (c = v->SearchName("Display")) c->SetValue((INT)_pi_cam_props->_annular_aoi.display);
				if (c = v->SearchName("Zone")   ) c->SetValue((INT)_pi_cam_props->_annular_aoi.zone);
				if (c = v->SearchName("Min")    ) c->SetValue(_pi_cam_props->_annular_aoi.min);
				if (c = v->SearchName("Max")    ) c->SetValue(_pi_cam_props->_annular_aoi.max);
			}


		// ---- annular focus
			v = config_hdl->SearchThis("Annular-Focus");
			if ( v != NULL )
			{
				if (c = v->SearchName("Display")) c->SetValue((INT)_pi_cam_props->_annular_focus.display);
				if (c = v->SearchName("Min")    ) c->SetValue(_pi_cam_props->_annular_focus.min);
				if (c = v->SearchName("Max")    ) c->SetValue(_pi_cam_props->_annular_focus.max);
				if (c = v->SearchName("Rotate") ) c->SetValue(_pi_cam_props->_annular_focus.rotate);
				if (c = v->SearchName("Sector") ) c->SetValue(_pi_cam_props->_annular_focus.sector);
			}


		// ---- aoi
			v = config_hdl->SearchThis("Aoi");
			if ( v != NULL )
			{
				if (c = v->SearchName("Display")) c->SetValue((INT)_pi_cam_props->_aoi.display);
				if (c = v->SearchName("Zone")   ) c->SetValue((INT)_pi_cam_props->_aoi.zone);
				if (c = v->SearchName("Height") ) c->SetValue(_pi_cam_props->_aoi.height);
				if (c = v->SearchName("Width")  ) c->SetValue(_pi_cam_props->_aoi.width);
				if (c = v->SearchName("Left")   ) c->SetValue(_pi_cam_props->_aoi.left);
				if (c = v->SearchName("Top")    ) c->SetValue(_pi_cam_props->_aoi.top);
				if (c = v->SearchName("Right")  ) c->SetValue(_pi_cam_props->_aoi.right);
				if (c = v->SearchName("Bottom") ) c->SetValue(_pi_cam_props->_aoi.bottom);
			}


		// ---- awb
			v = config_hdl->SearchThis("Awb   ");
			if ( v != NULL )
			{
				if (c = v->SearchName("Min")    ) c->SetValue(_pi_cam_props->_awb.min);
				if (c = v->SearchName("Max")    ) c->SetValue(_pi_cam_props->_awb.max);
			}


		// ---- colour
			v = config_hdl->SearchThis("Colour");
			if ( v != NULL )
			{
				for (UINT index = 0; index < NUM_COLOUR_CONFIGS; index++)
				{
					c = v->SearchThis(std_name[index]);
					if ( c != NULL )
					{
						VNode *d = NULL;
						if (d = c->SearchName("Saturation")) d->SetValue(_pi_cam_props->_colour[index].saturation);
					}
				}
			}


		// ---- exposure
			v = config_hdl->SearchThis("Exposure");
			if ( v != NULL )
			{
				for (UINT index = 0; index  <NUM_EXPOSURE_CONFIGS; index++)
				{
					c = v->SearchThis(std_name[index]);
					if ( c != NULL )
					{
						VNode *d = NULL;
						if (d = c->SearchName("Bright_Reference")) d->SetValue(_pi_cam_props->_exposure[index].bright_reference);
						if (d = c->SearchName("Time-Def")) d->SetValue(_pi_cam_props->_exposure[index].time_def);
						if (d = c->SearchName("Time-Max")) d->SetValue(_pi_cam_props->_exposure[index].time_max);
						if (d = c->SearchName("Time-Ms") ) d->SetValue(_pi_cam_props->_exposure[index].time_ms);
					}
				}
			}


		// ---- framerate
			v = config_hdl->SearchThis("Framerate");
			if ( v != NULL )
			{
				for (UINT index = 0; index < NUM_FRAMERATE_CONFIGS; index++)
				{
					c = v->SearchThis(std_name[index]);
					if ( c != NULL )
					{
						VNode *d = NULL;
						if (d = c->SearchName("Fps")    ) d->SetValue(_pi_cam_props->_framerate[index].fps);
						if (d = c->SearchName("Fps-Def")) d->SetValue(_pi_cam_props->_framerate[index].fps_def);
						if (d = c->SearchName("Fps-Max")) d->SetValue(_pi_cam_props->_framerate[index].fps_max);
					}
				}
			}


		// ---- gain colour
			VNode *exposure = config_hdl->SearchThis("Gain-Colour");
			if ( exposure != NULL )
			{
				CHAR *gain_name[NUM_GAIN_COLOUR_CONFIGS] =
				{
					"Auto",
					"Sunshine",
					"Cloudy",
					"Overcast",
					"Bulb",
					"Fluorescent",
					"User"
				};
				for (UINT index = 0; index < NUM_GAIN_COLOUR_CONFIGS; index++)
				{
					VNode *c = exposure->SearchThis(gain_name[index]);
					if ( c != NULL )
					{
						VNode *d = NULL;
						if (d = c->SearchName("Gain") ) d->SetValue(_pi_cam_props->_gain_colour[index].gain);
						if (d = c->SearchName("Red")  ) d->SetValue(_pi_cam_props->_gain_colour[index].red);
						if (d = c->SearchName("Green")) d->SetValue(_pi_cam_props->_gain_colour[index].green);
						if (d = c->SearchName("Blue") ) d->SetValue(_pi_cam_props->_gain_colour[index].blue);
					}
				}
			}


		// ---- gain master
			v = config_hdl->SearchThis("Gain-Master");
			if ( v != NULL )
			{
				for (UINT index = 0; index < NUM_GAIN_MASTER_CONFIGS; index++)
				{
					c = v->SearchThis(std_name[index]);
					if ( c != NULL )
					{
						VNode *d = NULL;
						if (d = c->SearchName("Black-Level")) d->SetValue((FLOAT)_pi_cam_props->_gain_master[index].black_level);
						if (d = c->SearchName("Gain")       ) d->SetValue((FLOAT)_pi_cam_props->_gain_master[index].gain);
					}
				}
			}


		// ---- gamma
			v = config_hdl->SearchThis("Gamma");
			if ( v != NULL )
			{
				for (UINT index = 0; index < 5; index++)
				{
					c = v->SearchThis(std_name[index]);
					if ( c != NULL )
					{
						VNode *d = NULL;
						if (d = c->SearchName("Factor")) _pi_cam_props->_gamma[index].factor= d->GetFloatFromValue();
					}
				}
			}


		// ---- media
			VNode *media = config_hdl->SearchThis("Media");
			if ( media != NULL )
			{
				if (c = media->SearchName("Type")) c->SetValue((INT)_pi_cam_props->_media.type);
			}


		// ---- media
			v = config_hdl->SearchThis("Media");
			if ( v != NULL )
			{
				if (c = v->SearchName("Type") ) c->SetValue((INT)_pi_cam_props->_media.type);
			}


		// ---- roi
			v = config_hdl->SearchThis("Roi");
			if ( v != NULL )
			{
				if (c = v->SearchName("H-Off")   ) c->SetValue((INT)_pi_cam_props->_roi.h_offset);
				if (c = v->SearchName("V-Off")   ) c->SetValue((INT)_pi_cam_props->_roi.v_offset);
				if (c = v->SearchName("Height")  ) c->SetValue((INT)_pi_cam_props->_roi.height);
				if (c = v->SearchName("Width")   ) c->SetValue((INT)_pi_cam_props->_roi.width);
				if (c = v->SearchName("Sampling")) c->SetValue((INT)_pi_cam_props->_roi.sampling);
			}


		// ---- rop
			v = config_hdl->SearchThis("Rop");
			if ( v != NULL )
			{
				if (c = v->SearchName("H-Flip")) c->SetValue((INT)_pi_cam_props->_rop.h_flip);
				if (c = v->SearchName("V-Flip")) c->SetValue((INT)_pi_cam_props->_rop.v_flip);
			}


		// ---- sensor
			v = config_hdl->SearchThis("Sensor");
			if ( v != NULL )
			{
				if (c = v->SearchName("Pixel-Clock")) c->SetValue((INT)_pi_cam_props->_sensor.pixel_clock);
				if (c = v->SearchName("Seq-Buffer") ) c->SetValue((INT)_pi_cam_props->_sensor.seq_buffer);
			}


			// ---- setup
			v = config_hdl->SearchThis("Setup");
			if (v != NULL)
			{

				// ---- NEW : ignore write BOOL states
				//
				// if (c = v->SearchName("Auto-Exposure") ) c->SetValue((INT)_pi_cam_props->_setup.auto_exposure);
				// if (c = v->SearchName("Auto-Framerate")) c->SetValue((INT)_pi_cam_props->_setup.auto_framerate);
				// if (c = v->SearchName("Auto-Gain")     ) c->SetValue((INT)_pi_cam_props->_setup.auto_gain);
				// if (c = v->SearchName("Auto-WB")       ) c->SetValue((INT)_pi_cam_props->_setup.auto_wb);

				if (c = v->SearchName("Colour")        ) c->SetValue((INT)_pi_cam_props->_setup.colour_index);
				if (c = v->SearchName("Exposure")      ) c->SetValue((INT)_pi_cam_props->_setup.exposure_index);
				if (c = v->SearchName("Framerate")     ) c->SetValue((INT)_pi_cam_props->_setup.framerate_index);
				if (c = v->SearchName("Gain-Colour")   ) c->SetValue((INT)_pi_cam_props->_setup.gain_colour_index);
				if (c = v->SearchName("Gain-Master")   ) c->SetValue((INT)_pi_cam_props->_setup.gain_master_index);
				if (c = v->SearchName("Gamma")         ) c->SetValue((INT)_pi_cam_props->_setup.gamma_index);

			}


		// ---- write config file
			vs_system::VNodeIO vio;
			INT result = vio.Write(hdl_paf, _pi_cam_props->_config_hdl);


		// ---- REPORT
			CHAR msg[VS_MAXCHARLEN];
			if ( result == VS_OK )
			{
				sprintf(msg, "+++ SAVED camera properties config file to:\n");
				OutputDebugString(msg);
				sprintf(msg, "   %s\n", hdl_paf);
				OutputDebugString(msg);
			}
			else
			{
				sprintf(msg, "+++ FAILED to save camera properties config file to:\n");
				OutputDebugString(msg);
				sprintf(msg, "   %s\n", hdl_paf);
				OutputDebugString(msg);
			}


		// ----  NOT successful ? -- return error
			if (result == VS_ERROR) return VS_ERROR;

	}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- GetProperty ----------
INT CamProps::GetProperty(
		UINT group,
		UINT index,
		UINT id,
		INT *value
	)
{
	FLOAT f = 0;
	INT result = GetProperty
		(
			group,
			index,
			id,
			&f
		);
	*value = (INT)f;
	return VS_OK;
}


// ---------- GetProperty ----------
/*!
\brief Get property
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT CamProps::GetProperty(
		UINT group,
		UINT index,
		UINT id,
		FLOAT *value
	)
{

	switch ( group )
	{

		case ANNULAR_AOI:
			{
				switch (id)
				{
					case DISPLAY : *value = (FLOAT)_pi_cam_props->_annular_aoi.display; break;
					case ZONE    : *value = (FLOAT)_pi_cam_props->_annular_aoi.zone;    break;
					case MIN     : *value = (FLOAT)_pi_cam_props->_annular_aoi.min;     break;
					case MAX     : *value = (FLOAT)_pi_cam_props->_annular_aoi.max;     break;
					default      : return VS_ERROR;
				}
			}
			break;


		case ANNULAR_FOCUS:
			{
				switch (id)
				{
					case DISPLAY : *value = (FLOAT)_pi_cam_props->_annular_focus.display; break;
					case MIN     : *value = (FLOAT)_pi_cam_props->_annular_focus.min;     break;
					case MAX     : *value = (FLOAT)_pi_cam_props->_annular_focus.max;     break;
					case ROTATE  : *value = (FLOAT)_pi_cam_props->_annular_focus.sector;  break;
					case SECTOR  : *value = (FLOAT)_pi_cam_props->_annular_focus.rotate;  break;
					default      : return VS_ERROR;
				}
			}
			break;


		case AOI:
			{
				switch (id)
				{
					case DISPLAY : *value = (FLOAT)_pi_cam_props->_aoi.display; break;
					case ZONE    : *value = (FLOAT)_pi_cam_props->_aoi.zone;    break;
					case HEIGHT  : *value = (FLOAT)_pi_cam_props->_aoi.height;  break;
					case WIDTH   : *value = (FLOAT)_pi_cam_props->_aoi.width;   break;
					case LEFT    : *value = (FLOAT)_pi_cam_props->_aoi.left;    break;
					case TOP     : *value = (FLOAT)_pi_cam_props->_aoi.top;     break;
					case RIGHT   : *value = (FLOAT)_pi_cam_props->_aoi.right;   break;
					case BOTTOM  : *value = (FLOAT)_pi_cam_props->_aoi.bottom;  break;
					default      : return VS_ERROR;
				}
			}
			break;


		case AWB:
			{
				switch ( id )
				{
					case MIN : *value = _pi_cam_props->_awb.min; break;
					case MAX : *value = _pi_cam_props->_awb.max; break;
					default  : return VS_ERROR;
				}
			}
			break;


		case COLOUR:
			{
				if ( index >= 0 && index < NUM_COLOUR_CONFIGS )
				{
					switch ( id )
					{
						case SATURATION : *value = _pi_cam_props->_colour[index].saturation; break;
						default         : return VS_ERROR;
					}
				}
			}
			break;
		

		case EXPOSURE:
			{
				if ( index >= 0 && index < NUM_EXPOSURE_CONFIGS )
				{
					switch (id)
					{
						case BRIGHT_REF   : *value = _pi_cam_props->_exposure[index].bright_reference; break;
						case TIME_EXP_DEF : *value = _pi_cam_props->_exposure[index].time_def;         break;
						case TIME_EXP_MAX : *value = _pi_cam_props->_exposure[index].time_max;         break;
						case TIME_EXP_MS  : *value = _pi_cam_props->_exposure[index].time_ms;          break;
						default           : return VS_ERROR;
					}
				}
			}
			break;


		case FRAMERATE:
			{
				if ( index >= 0 && index < NUM_FRAMERATE_CONFIGS )
				{
					switch (id)
					{
						case FPS     : *value = _pi_cam_props->_framerate[index].fps;     break;
						case FPS_DEF : *value = _pi_cam_props->_framerate[index].fps_def; break;
						case FPS_MAX : *value = _pi_cam_props->_framerate[index].fps_max; break;
						default      : return VS_ERROR;
					}
				}
			}
			break;


		case GAIN_COLOUR:
			{
				if ( index >= 0 && index < NUM_GAIN_COLOUR_CONFIGS )
				{
					switch (id)
					{
						case GAIN  : *value = _pi_cam_props->_gain_colour[index].gain;  break;
						case RED   : *value = _pi_cam_props->_gain_colour[index].red;   break;
						case GREEN : *value = _pi_cam_props->_gain_colour[index].green; break;
						case BLUE  : *value = _pi_cam_props->_gain_colour[index].blue;  break;
						default    : return VS_ERROR;
					}
				}
			}
			break;


		case GAIN_MASTER:
			{
				if ( index >= 0 && index < NUM_GAIN_MASTER_CONFIGS )
				{
					switch (id)
					{
						case BLACK_LEVEL : *value = _pi_cam_props->_gain_master[index].black_level; break;
						case GAIN        : *value = _pi_cam_props->_gain_master[index].gain;        break;
						default          : return VS_ERROR;
					}
				}
			}
			break;


		case GAMMA:
			{
				if (index >= 0 && index < NUM_GAMMA_CONFIGS)
				{
					switch (id)
					{
						case FACTOR : *value = _pi_cam_props->_gamma[index].factor; break;
						default     : return VS_ERROR;
					}
				}
			}
			break;


		case MEDIA:
			{
				switch ( id )
				{
					case TYPE : *value = (FLOAT)_pi_cam_props->_media.type; break;
					default   : return VS_ERROR;
				}
			}
			break;


		case ROI:
			{
				switch ( id )
				{
					case H_OFFSET : *value = (FLOAT)_pi_cam_props->_roi.h_offset; break;
					case V_OFFSET : *value = (FLOAT)_pi_cam_props->_roi.v_offset; break;
					case WIDTH    : *value = (FLOAT)_pi_cam_props->_roi.width;    break;
					case HEIGHT   : *value = (FLOAT)_pi_cam_props->_roi.height;   break;
					case SAMPLING : *value = (FLOAT)_pi_cam_props->_roi.sampling; break;
					default       : return VS_ERROR;
				}
			}
			break;


		case ROP:
			{
				switch ( id )
				{
					case H_FLIP : *value = (FLOAT)_pi_cam_props->_rop.h_flip; break;
					case V_FLIP : *value = (FLOAT)_pi_cam_props->_rop.v_flip; break;
					default     : return VS_ERROR;
				}
			}
			break;
		

		case SENSOR:
			{
				switch ( id )
				{
					case PIXEL_CLOCK : *value = (FLOAT)_pi_cam_props->_sensor.pixel_clock; break;
					case SEQ_BUFFER  : *value = (FLOAT)_pi_cam_props->_sensor.seq_buffer;  break;
					default          : return VS_ERROR;
				}
			}
			break;


		case SETUP:
			{
				switch ( id )
				{
					// ---- NEW : get BOOL state set by UINT index AND vice versa
					//
					case AUTO_EXPOSURE     : *value = (FLOAT)((INT)_pi_cam_props->_setup.auto_exposure);  break;
					case AUTO_FRAMERATE    : *value = (FLOAT)((INT)_pi_cam_props->_setup.auto_framerate); break;
					case AUTO_GAIN         : *value = (FLOAT)((INT)_pi_cam_props->_setup.auto_gain);      break;
					case AUTO_WB           : *value = (FLOAT)((INT)_pi_cam_props->_setup.auto_wb);        break;

					case COLOUR_INDEX      : *value = (FLOAT)_pi_cam_props->_setup.colour_index;          break;
					case EXPOSURE_INDEX    : *value = (FLOAT)_pi_cam_props->_setup.exposure_index;        break;
					case FRAMERATE_INDEX   : *value = (FLOAT)_pi_cam_props->_setup.framerate_index;       break;
					case GAIN_COLOUR_INDEX : *value = (FLOAT)_pi_cam_props->_setup.gain_colour_index;     break;
					case GAIN_MASTER_INDEX : *value = (FLOAT)_pi_cam_props->_setup.gain_master_index;     break;
					case GAMMA_INDEX       : *value = (FLOAT)_pi_cam_props->_setup.gamma_index;           break;
					default                : return VS_ERROR;
				}
			}
			break;

		default:
			break;
	}

	return VS_OK;
}


// ---------- SetProperty ----------
/*!
\brief Set property
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT CamProps::SetProperty(
		UINT  group,
		UINT  index,
		UINT  id,
		FLOAT value
	)
{

	switch ( group )
	{

		case ANNULAR_AOI:
			{
				switch (id)
				{
					case DISPLAY : _pi_cam_props->_annular_aoi.display = (BOOL)((INT)value); break;
					case ZONE    : _pi_cam_props->_annular_aoi.zone    = (UINT)value; break;
					case MIN     : _pi_cam_props->_annular_aoi.min     = value; break;
					case MAX     : _pi_cam_props->_annular_aoi.max     = value; break;
					default      : return VS_ERROR;
				}
			}
			break;


		case ANNULAR_FOCUS:
			{
				switch (id)
				{
					case DISPLAY : _pi_cam_props->_annular_focus.display = (BOOL)((INT)value); break;
					case MIN     : _pi_cam_props->_annular_focus.min     = value; break;
					case MAX     : _pi_cam_props->_annular_focus.max     = value; break;
					case ROTATE  : _pi_cam_props->_annular_focus.sector  = value; break;
					case SECTOR  : _pi_cam_props->_annular_focus.rotate  = value; break;
					default      : return VS_ERROR;
				}
			}
			break;


		case AOI:
			{
				switch (id)
				{
					case DISPLAY : _pi_cam_props->_aoi.display = (BOOL)((INT)value); break;
					case ZONE    : _pi_cam_props->_aoi.zone    = (UINT)value; break;
					case HEIGHT  : _pi_cam_props->_aoi.height  = value; break;
					case WIDTH   : _pi_cam_props->_aoi.width   = value; break;
					case LEFT    : _pi_cam_props->_aoi.left    = value; break;
					case TOP     : _pi_cam_props->_aoi.top     = value; break;
					case RIGHT   : _pi_cam_props->_aoi.right   = value; break;
					case BOTTOM  : _pi_cam_props->_aoi.bottom  = value; break;
					default      : return VS_ERROR;
				}
			}
			break;


		case AWB:
			{
				switch ( id )
				{
					case MIN : _pi_cam_props->_awb.min = value; break;
					case MAX : _pi_cam_props->_awb.max = value; break;
					default  : return VS_ERROR;
				}
			}
			break;


		case COLOUR:
			{
				if ( index >= 0 && index < NUM_COLOUR_CONFIGS )
				{
					switch ( id )
					{
						case SATURATION : _pi_cam_props->_colour[index].saturation = value; break;
						default         : return VS_ERROR;
					}
					_pi_cam_props->LimitProperty(COLOUR, index);
				}
			}
			break;
		

		case EXPOSURE:
			{
				if ( index >= 0 && index < NUM_EXPOSURE_CONFIGS )
				{
					switch (id)
					{
						case BRIGHT_REF  : _pi_cam_props->_exposure[index].bright_reference = value; break;
						case TIME_EXP_MS : _pi_cam_props->_exposure[index].time_ms = value; break;
						default          : return VS_ERROR;
					}
					_pi_cam_props->LimitProperty(EXPOSURE, index);
				}
			}
			break;


		case FRAMERATE:
			{
				if ( index >= 0 && index < NUM_FRAMERATE_CONFIGS )
				{
					switch (id)
					{
						case FPS : _pi_cam_props->_framerate[index].fps = value; break;
						default  : return VS_ERROR;
					}
					_pi_cam_props->LimitProperty(FRAMERATE, index);
				}
			}
			break;


		case GAIN_COLOUR:
			{
				if ( index >= 0 && index < NUM_GAIN_COLOUR_CONFIGS )
				{
					switch (id)
					{
						case GAIN  : _pi_cam_props->_gain_colour[index].gain  = value; break;
						case RED   : _pi_cam_props->_gain_colour[index].red   = value; break;
						case GREEN : _pi_cam_props->_gain_colour[index].green = value; break;
						case BLUE  : _pi_cam_props->_gain_colour[index].blue  = value; break;
						default    : return VS_ERROR;
					}
					_pi_cam_props->LimitProperty(GAIN_COLOUR, index);
				}
			}
			break;


		case GAIN_MASTER:
			{
				if ( index >= 0 && index < NUM_GAIN_MASTER_CONFIGS )
				{
					switch (id)
					{
						case BLACK_LEVEL : _pi_cam_props->_gain_master[index].black_level = value; break;
						case GAIN        : _pi_cam_props->_gain_master[index].gain        = value; break;
						default          : return VS_ERROR;
					}
					_pi_cam_props->LimitProperty(GAIN_MASTER, index);
				}
			}
			break;


		case GAMMA:
			{
				if ( index >= 0 && index < NUM_GAMMA_CONFIGS )
				{
					switch (id)
					{
						case FACTOR : _pi_cam_props->_gamma[index].factor = value; break;
						default     : return VS_ERROR;
					}
					_pi_cam_props->LimitProperty(GAMMA, index);
				}
			}
			break;


		case MEDIA:
			{
				switch ( id )
				{
					case TYPE : _pi_cam_props->_media.type = (UINT)value; break;
					default   : return VS_ERROR;
				}
			}
			break;


		case ROI:
			{
				switch ( id )
				{
					case H_OFFSET : _pi_cam_props->_roi.h_offset = (INT)value; break;
					case V_OFFSET : _pi_cam_props->_roi.v_offset = (INT)value; break;
					case WIDTH    : _pi_cam_props->_roi.width    = (INT)value; break;
					case HEIGHT   : _pi_cam_props->_roi.height   = (INT)value; break;
					case SAMPLING : _pi_cam_props->_roi.sampling = (INT)value; break;
					default       : return VS_ERROR;
				}
			}
			break;


		case ROP:
			{
				switch ( id )
				{
					case H_FLIP : _pi_cam_props->_rop.h_flip = (BOOL)((INT)value); break;
					case V_FLIP : _pi_cam_props->_rop.v_flip = (BOOL)((INT)value); break;
					default     : return VS_ERROR;
				}
			}
			break;
		

		case SENSOR:
			{
				switch ( id )
				{
					case PIXEL_CLOCK : _pi_cam_props->_sensor.pixel_clock = (UINT)value; break;
					case SEQ_BUFFER  : _pi_cam_props->_sensor.seq_buffer  = (UINT)value; break;
					default          : return VS_ERROR;
				}
			}
			break;


		case SETUP:
			{
				switch ( id )
				{
					// ---- NEW : set BOOL state set by UINT index AND vice versa
					//
					case AUTO_EXPOSURE     : _pi_cam_props->_setup.auto_exposure     = (BOOL)((INT)value); break;
					case AUTO_FRAMERATE    : _pi_cam_props->_setup.auto_framerate    = (BOOL)((INT)value); break;
					case AUTO_GAIN         : _pi_cam_props->_setup.auto_gain         = (BOOL)((INT)value); break;
					case AUTO_WB           : _pi_cam_props->_setup.auto_wb           = (BOOL)((INT)value); break;
					case COLOUR_INDEX      : _pi_cam_props->_setup.colour_index      = (UINT)value;        break;
					case EXPOSURE_INDEX    : _pi_cam_props->_setup.exposure_index    = (UINT)value; break;
					case FRAMERATE_INDEX   : _pi_cam_props->_setup.framerate_index   = (UINT)value; break;
					case GAIN_MASTER_INDEX : _pi_cam_props->_setup.gain_master_index = (UINT)value; break;
					case GAIN_COLOUR_INDEX : _pi_cam_props->_setup.gain_colour_index = (UINT)value; break;
					case GAMMA_INDEX       : _pi_cam_props->_setup.gamma_index       = (UINT)value; break;
					default                : return VS_ERROR;
				}
				_pi_cam_props->AdjustSetupProperty(id);
			}
			break;

		default:
			break;
	}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- GetEditConfig ----------
/*!
\brief Get "edit all configurations" flag
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
BOOL CamProps::GetEditConfig()
{
	return _pi_cam_props->_edit_all;
}


// ---------- SetEditConfig ----------
/*!
\brief Get "edit all configurations" flag
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT CamProps::SetEditConfig(BOOL flag)
{
	_pi_cam_props->_edit_all = flag;
	return VS_OK;
}



////////////////////////////////////////////////////////////////////////////////


// ---------- GetBlackLevelCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::GetBlackLevelCallback(INT(**callback)())
{
	*callback = _pi_cam_props->_black_level_callback;
	return VS_OK;
}


// ---------- GetColourCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::GetColourCallback(INT(**callback)())
{
	*callback = _pi_cam_props->_colour_callback;
	return VS_OK;
}


// ---------- GetExposureCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::GetExposureCallback(INT(**callback)())
{
	*callback = _pi_cam_props->_exposure_callback;
	return VS_OK;
}


// ---------- GetFramerateCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::GetFramerateCallback(INT(**callback)())
{
	*callback = _pi_cam_props->_framerate_callback;
	return VS_OK;
}


// ---------- GetGainColourCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::GetGainColourCallback(INT(**callback)())
{
	*callback = _pi_cam_props->_gain_colour_callback;
	return VS_OK;
}

// ---------- GetGainMasterCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::GetGainMasterCallback(INT(**callback)())
{
	*callback = _pi_cam_props->_gain_master_callback;
	return VS_OK;
}


// ---------- GetGammaCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::GetGammaCallback(INT(**callback)())
{
	*callback = _pi_cam_props->_gamma_callback;
	return VS_OK;
}


// ---------- GetRopCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::GetRopCallback(INT(**callback)())
{
	*callback = _pi_cam_props->_rop_callback;
	return VS_OK;
}


// ---------- GetSetupCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::GetSetupCallback(INT(**callback)())
{
	*callback = _pi_cam_props->_setup_callback;
	return VS_OK;
}


// ---------- GetWriteIniFileCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::GetWriteIniFileCallback(INT(**callback)())
{
	*callback = _pi_cam_props->_write_ini_file_callback;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- SetBlackLevelCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::SetBlackLevelCallback(INT(*callback)())
{
	_pi_cam_props->_black_level_callback = callback;
	return VS_OK;
}


// ---------- SetColourCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::SetColourCallback(INT(*callback)())
{
	_pi_cam_props->_colour_callback = callback;
	return VS_OK;
}


// ---------- SetExposureCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::SetExposureCallback(INT(*callback)())
{
	_pi_cam_props->_exposure_callback = callback;
	return VS_OK;
}


// ---------- SetFramerateCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::SetFramerateCallback(INT(*callback)())
{
	_pi_cam_props->_framerate_callback = callback;
	return VS_OK;
}


// ---------- SetGainColourCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::SetGainColourCallback(INT(*callback)())
{
	_pi_cam_props->_gain_colour_callback = callback;
	return VS_OK;
}


// ---------- SetGainMasterCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::SetGainMasterCallback(INT(*callback)())
{
	_pi_cam_props->_gain_master_callback = callback;
	return VS_OK;
}


// ---------- SetGammaCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::SetGammaCallback(INT(*callback)())
{
	_pi_cam_props->_gamma_callback = callback;
	return VS_OK;
}


// ---------- SetRopCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::SetRopCallback(INT(*callback)())
{
	_pi_cam_props->_rop_callback = callback;
	return VS_OK;
}


// ---------- SetSetupCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::SetSetupCallback(INT(*callback)())
{
	_pi_cam_props->_setup_callback = callback;
	return VS_OK;
}


// ---------- SetWriteIniFileCallback ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT CamProps::SetWriteIniFileCallback(INT(*callback)())
{
	_pi_cam_props->_write_ini_file_callback = callback;
	return VS_OK;
}


// ---- action
FLOAT CamProps::GetActionValue(
		INT  elem_index,    // As per "if ( elem_index < 0 )" 
		UINT auto_lid,      // e.g. AUTO_EXPOSURE ( if any )
		UINT prop_lid,      // e.g. EXPOSURE_INDEX
		UINT num_config,    // e.g. NUM_EXPOSURE_CONFIGS
		UINT user_lid       // e.g. user's tint_acquire_param.EXP_USER_INDEX
	)
{

	// ---- if in auto mode then always OFF
		if ( auto_lid )
		{
			INT auto_flag = 0;
			GetProperty(
					CamProps::SETUP,
					0,
					auto_lid,
					&auto_flag
				);
			if ( auto_flag ) return 0;
		}


	// --- get index within this property group
		INT index_within_group = 0;
		GetProperty(
				CamProps::SETUP,
				0,
				prop_lid,
				&index_within_group
			);


	// ---- last item ?
		BOOL last_item = index_within_group == num_config - 1 ? 1 : 0;


	// ---- if runtime AND last item then ON
		if ( elem_index < 0 )
		{
			return (FLOAT)( last_item ? 1 : 0 );
		}


	// ---- if last item (either way) then ON else OFF
		return
			(FLOAT)
			(
				last_item || elem_index == user_lid ? 1 : 0
			);

};


////////////////////////////////////////////////////////////////////////////////


// ---------- ResetProperty ----------
/*!
\brief Reset a property
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT CamProps::ResetProperty(UINT group)
{

	// ---- local
	INT result = VS_OK;


	// ---- reset
		_pi_cam_props->ResetProperty(group);


	// ---- callback
		switch (group)
		{

			case ANNULAR_AOI: break;
			case ANNULAR_FOCUS: break;
			case AOI: break;
			case AWB: break;
			case MEDIA: break;
			case ROI: break;
			case ROP: break;
			case SENSOR: break;

			case COLOUR:
				{
					INT(*colour_callback)();
					result = GetColourCallback(&colour_callback);
					result = colour_callback();
				}
				break;

			case EXPOSURE:
				{
					INT(*exposure_callback)();
					result = GetExposureCallback(&exposure_callback);
					result = exposure_callback();
				}
				break;

			case FRAMERATE:
				{
					INT(*framerate_callback)();
					result = GetFramerateCallback(&framerate_callback);
					result = framerate_callback();
				}
				break;

			case GAIN_COLOUR:
				{
					INT(*gain_colour_callback)();
					result = GetGainColourCallback(&gain_colour_callback);
					result = gain_colour_callback();
				}
				break;

			case GAIN_MASTER:
				{
					INT(*master_gain_callback)();
					result = GetGainMasterCallback(&master_gain_callback);
					result = master_gain_callback();
				}
				break;

			case GAMMA:
				{
					INT(*gamma_callback)();
					result = GetGammaCallback(&gamma_callback);
					result = gamma_callback();
				}
				break;

			case SETUP:
				{
					INT(*setup_callback)();
					result = GetSetupCallback(&setup_callback);
					result = setup_callback();
				}
				break;

			default:
				break;
		}

	return VS_OK;
}


// ---------- ResetCallbackProperties ----------
/*!
\brief Reset only callback properties
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT CamProps::ResetCallbackProperties()
{

	// ---- local
		INT result = VS_OK;


	// ---- do it
		result = ResetProperty(COLOUR);
		result = ResetProperty(EXPOSURE);
		result = ResetProperty(FRAMERATE);
		result = ResetProperty(GAIN_COLOUR);
		result = ResetProperty(GAIN_MASTER);
		result = ResetProperty(GAMMA);

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////

