////////////////////////////////////////////////////////////////////////////////

// ---------- s1_system_param.h ----------
/*!
\file s1_system_param.h
\brief Interface for the SystemParam class.
\author Gareth Edwards
*/

#pragma once

// note - must include this here to pick up SystemObject

#include "../header/s1_include.h"


////////////////////////////////////////////////////////////////////////////////


namespace surface_01
{

	// ---------- system parameters ----------
	class SystemParam
	{

	public:

		// ---- target
			enum CameraTarget
			{
				NONE = 999,
				NONE_RUNTIME = 998
			};

		// ---- system light
			enum SystemLightMode
			{
				NORMAL_COLOUR  = 0,
				GRAPHIC_COLOUR = 1
			};

		// ---- system view
			enum SystemViewMode
			{
				STANDARD_SIZE = 0,
				LARGE_SIZE    = 1,
				GRAPHIC_SIZE  = 2
			};


		// ---- limit
			enum
			{
				MAX_SYS_OBJECT = 128
			};


		// ---- cdtor
			SystemParam();
			virtual ~SystemParam();


		// ---- io
			INT  Read(CHAR *);
			INT  Write();


		// ---- display
			UINT  GetSystemLightMode();
			UINT  IncSystemLightMode();
			VOID  SetSystemLightMode(UINT);
			UINT  GetSystemViewMode();
			UINT  IncSystemViewMode();
			VOID  SetSystemViewMode(UINT);
			BOOL  GetSystemViewModeChanged();
			VOID  SetSystemViewModeChanged(BOOL);


		// ---- gfx
			BOOL   GetGfxVersion();


		// ----- object
			FLOAT  GetObjectDiameterScalar(UINT type);


		// ---- pause mode
			INT    GetPauseMode();
			VOID   SetPauseMode(INT);
			DOUBLE GetPauseTimeStart();
			VOID   SetPauseTimeStart(DOUBLE);
			DOUBLE GetPauseTimeElapsed();
			VOID   SetPauseTimeElapsed(DOUBLE);


		// ---- scale (geometry)
			DOUBLE GetGlobalScale();
			DOUBLE GetObjectScale();
			INT    SetGlobalScale(DOUBLE);
			INT    SetObjectScale(DOUBLE);


		// ---- scalar (value)
			DOUBLE GetTimeScalar();
			DOUBLE GetDayLenScalar();
			INT    SetTimeScalar(DOUBLE);
			INT    SetDayLenScalar(DOUBLE);
			FLOAT  GetTargetDistScalar();


		// ---- time
			LONG   GetMsOffset();
			INT SetMsOffset(LONG);

	private:

		// ---- private implementation
			class  PI_SystemParam;
			PI_SystemParam *pi_system_param;

	};


}


////////////////////////////////////////////////////////////////////////////////
