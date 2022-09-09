////////////////////////////////////////////////////////////////////////////////

// ---------- vs_cam_props.h ----------
/*!
\file vs_cam_props.h
\brief Interface and implementation for the CamProps class.
\author Gareth Edwards
\note 2017
*/

#pragma once


// ---- system include ----
#include "../../vs_system/header/vs_inc_defs.h"
#include "../../vs_system/header/vs_app_dependency.h"


// ---------- namespace ----------
namespace vs_system
{

// ---------- CamProps class ----------
class CamProps
{

public:

	// ---- groups
		enum {
			ANNULAR_AOI,
			ANNULAR_FOCUS,
			AOI,
			AWB,
			COLOUR,
			CONFIG,
			EXPOSURE,
			FRAMERATE,
			GAIN_MASTER,
			GAIN_COLOUR,
			GAMMA,
			MEDIA,
			ROI,
			ROP,
			SENSOR,
			SETUP
		};


	// ---- properties
		enum {
			NONE,

			DISPLAY,           // ALL
			ZONE,
			MIN,
			MAX,
			RED,
			GREEN,
			BLUE,
			USER,

			ROTATE,            // ANN_FOCUS
			SECTOR,

			SATURATION,        // COLOUR

			AUTO_EXPOSURE,     // SETUP
			AUTO_FRAMERATE,
			AUTO_GAIN,
			AUTO_WB,

			COLOUR_INDEX,      // SETUP
			EXPOSURE_INDEX,
			FRAMERATE_INDEX,
			GAIN_MASTER_INDEX,
			GAIN_COLOUR_INDEX,
			GAMMA_INDEX,

			NUM_STD_CONFIGS         = 5, // ALMHU - NUM(ber)_(of)[...]_CONFIG(urations)
			NUM_COLOUR_CONFIGS      = 5, // ALMHU
			NUM_EXPOSURE_CONFIGS    = 5, // ALMHU
			NUM_FRAMERATE_CONFIGS   = 5, // ALMHU
			NUM_GAIN_MASTER_CONFIGS = 5, // ALMHU 
			NUM_GAIN_COLOUR_CONFIGS = 7, // A012345U
			NUM_GAMMA_CONFIGS       = 5, // ALMHU

			BRIGHT_REF,        // EXPOSURE
			TIME_EXP_DEF,
			TIME_EXP_MAX,
			TIME_EXP_MS,

			FPS,               // FRAMERATE
			FPS_DEF,
			FPS_MAX,

			BLACK_LEVEL,       // GAIN_MASTER
			GAIN,

			FACTOR,            // TYPE
			TYPE,              // MEDIA

			H_OFFSET,          // ROI
			V_OFFSET,
			WIDTH,
			HEIGHT,
			SAMPLING,

			LEFT,              // ROI INTERNAL
			RIGHT,
			TOP,
			BOTTOM,

			H_FLIP,            // ROP
			V_FLIP,

			PIXEL_CLOCK,       // SENSOR
			SEQ_BUFFER,
		};


	// ---- id
		enum {
			LOW_ID    = 1,
			MEDIUM_ID = 2,
			HIGH_ID   = 3,
		};


public:


	// ---- ctor 
		CamProps();
		virtual ~CamProps();


	// ---- framework
		INT Setup(vs_system::AppDependency *);
		INT Cleanup();


	// ---- file io
		INT Read(CHAR *);
		INT Write(CHAR *);


	// ---- value io
		INT GetProperty(UINT, UINT, UINT, INT *);     // INT facade
		INT GetProperty(UINT, UINT, UINT, FLOAT *);
		INT SetProperty(UINT, UINT, UINT, FLOAT);


	// ---- get & set flags
		BOOL GetEditConfig();
		INT  SetEditConfig(BOOL);
		

	// ---- get callback
		INT GetBlackLevelCallback(INT(**)());
		INT GetColourCallback(INT(**)());
		INT GetExposureCallback(INT(**)());
		INT GetFramerateCallback(INT(**)());
		INT GetGainColourCallback(INT(**)());
		INT GetGainMasterCallback(INT(**)());
		INT GetGammaCallback(INT(**)());
		INT GetRopCallback(INT(**)());
		INT GetSetupCallback(INT(**)());
		INT GetWriteIniFileCallback(INT(**)());


	// ---- set callback
		INT SetBlackLevelCallback(INT(*)());
		INT SetColourCallback(INT(*)());
		INT SetExposureCallback(INT(*)());
		INT SetFramerateCallback(INT(*)());
		INT SetGainColourCallback(INT(*)());
		INT SetGainMasterCallback(INT(*)());
		INT SetGammaCallback(INT(*)());
		INT SetRopCallback(INT(*)());
		INT SetSetupCallback(INT(*)());
		INT SetWriteIniFileCallback(INT(*)());


	// ---- reset callback
		INT ResetProperty(UINT);
		INT ResetCallbackProperties();


	// ---- action
		FLOAT GetActionValue(
			INT,    // As per "if ( elem_index < 0 )" 
			UINT,   // e.g. AUTO_EXPOSURE
			UINT,   // e.g. EXPOSURE_INDEX
			UINT,   // e.g. NUM_EXPOSURE_CONFIGS
			UINT    // e.g. user's tint_acquire_param.EXP_USER_INDEX
		);


private:

	// ---- private implementation
		class PI_CamProps; PI_CamProps *_pi_cam_props;


}; // CamProps class


} // namespace
