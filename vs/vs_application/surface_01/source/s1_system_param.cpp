////////////////////////////////////////////////////////////////////////////////

// ---------- s1_system_object.cpp ----------
/*!
\file s1_system_object.cpp
\brief Implementation of the SystemObject class
\author Gareth Edwards
*/

#include "../header/s1_system_param.h"

using namespace surface_01;


////////////////////////////////////////////////////////////////////////////////


// ---------- private implementation of System Paremeters class ----------
class SystemParam::PI_SystemParam
{

public:

	// ---- cdtor
		PI_SystemParam();
		~PI_SystemParam();


	// ---- read
		CHAR solar_system_hdl_filename[VS_MAXCHARPATHLEN];
		vs_system::VNode *config_hdl = NULL;


	// ---- camera control
		struct CameraControl
		{
			UINT message = 0;
			BOOL persist = 0;
			BOOL status = FALSE;
		};
		CameraControl current_camera_control;


	// ---- camera (XYZ, VECTOR, PTZ or TARGET)
		UINT kamera_control_mode   = vs_system::AppKamera::Mode::XYZ;
		UINT target_object_id_prev = SystemParam::CameraTarget::NONE_RUNTIME;
		UINT target_object_id      = SystemParam::CameraTarget::NONE;


	// ---- modes
		UINT star_light_mode  = SystemParam::SystemLightMode::GRAPHIC_COLOUR;
		UINT system_view_mode = SystemParam::SystemViewMode::GRAPHIC_SIZE;
		BOOL system_view_update = FALSE;


	// --- pause
		INT    pause_mode          = 0;
		DOUBLE paused_start_time   = 0;
		DOUBLE paused_elapsed_time = 0;


	// ---- scale (geometry)
		DOUBLE scale_global = 0.00001f;  // was 0.00001f
		DOUBLE scale_object[3] = { 1,  50,  100 };
		   

	// ---- scalar (value)
		DOUBLE time_scalar   = 1;
		DOUBLE daylen_scalar = 1;


	// ---- time
		LONG   ms_offset = 0;

};


// ---------- ctor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SystemParam::PI_SystemParam::PI_SystemParam()
{
	;
}


// ---------- dtor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SystemParam::PI_SystemParam::~PI_SystemParam()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- ctor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SystemParam::SystemParam()
{
	pi_system_param = new PI_SystemParam();
}


// ---------- dtor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SystemParam::~SystemParam()
{
	if ( pi_system_param != NULL ) { delete pi_system_param;  pi_system_param = NULL; }
}


// ---------- Read ----------
/*!
\brief Read csolar system configuration hdl file
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT SystemParam::Read(CHAR * solar_system_hdl_filename)
{

	// ---- file exists ?
		if ( GetFileAttributes(solar_system_hdl_filename) != INVALID_FILE_ATTRIBUTES )
		{

			// ---- read
				vs_system::VNodeIO vio;
				vs_system::VNode *config_hdl = vio.Read(solar_system_hdl_filename);

				
			// ---- failed ? -- return
				if ( config_hdl == NULL )
				{
					return VS_ERROR;
				}


			// ---- ok ?

			// ---- scale objects name
				CHAR *scale_name[3] =
				{
					"Standard",
					"Large",
					"Graphic",
				};
				vs_system::VNode *d = NULL, *c = NULL;
				vs_system::VNode *v = NULL;


			// ---- display
				v = config_hdl->SearchThis("Display");
				if ( v != NULL )
				{
					if ( c = v->SearchName("Mode") )
					{
						INT system_view_mode = 
						pi_system_param->system_view_mode      = c->GetIntFromValue();
					}
				}


			// ---- camera
				v = config_hdl->SearchThis("Explore");
				if ( v != NULL )
				{

					// ---- 5 update modes
						if (c = v->SearchName("Update"))
						{
							INT kamera_control_mode = c->GetIntFromValue();
							pi_system_param->kamera_control_mode = kamera_control_mode;
						}

					// ---- 12 target objects
						if ( c = v->SearchName("Target") )
						{
							INT target_object_id = target_object_id = c->GetIntFromValue();
							target_object_id = target_object_id <= 1 ?
								target_object_id : (target_object_id > 12 ? 12 : target_object_id);
							pi_system_param->target_object_id = target_object_id;
						}

				}




			// ---- scale
				v = config_hdl->SearchThis("Scale");
				if ( v != NULL )
				{
					if ( c = v->SearchName("Global") ) pi_system_param->scale_global          = c->GetFloatFromValue();
					for (UINT index = 0; index < 3; index++)
					{
						d = c->SearchThis(scale_name[index]);
						if ( d != NULL )
						{
							pi_system_param->scale_object[index] = d->GetFloatFromValue();
						}
					}
				}

			// ---- scalar
				/*

					Day Length Scalar
					=================
					- DOUBLE GetDayLenScalar()
					- INT SetDayLenScalar(DOUBLE)

					- DOUBLE day_degrees = fabs(day_length_hours) < 0.000001 ? 0 :
											hours_elapsed / day_length_hours * 360 ;
						day_degrees += fabs(day_start_hours) < 0.000001 ? 0 :
											day_start_hours / day_length_hours * 360;
						day_degrees *= daylen_scalar;

					Time Scalar
					===========
						- DOUBLE GetTimeScalar()
						- INT SetTimeScalar(DOUBLE)

						- DOUBLE hours_elapsed =
								system_time_elapsed *
									system_param->GetTimeScalar() +
										system_param->GetMsOffset();

				*/
				v = config_hdl->SearchThis("Scalar");
				if ( v != NULL )
				{
					if ( c = v->SearchName("Day")    ) pi_system_param->daylen_scalar         = c->GetFloatFromValue();
					if ( c = v->SearchName("Time")   ) pi_system_param->time_scalar           = c->GetFloatFromValue();
				}


			// ---- time
				/*
				
				*/
				v = config_hdl->SearchThis("Time");
				if ( v != NULL )
				{
					if ( c = v->SearchName("Pause")  ) pi_system_param->pause_mode            = c->GetIntFromValue();
					if ( c = v->SearchName("Offset") ) pi_system_param->ms_offset             = c->GetIntFromValue();
				}


			// ---- report
				{
					CHAR msg[VS_MAXCHARPATHLEN];
					sprintf(msg, "+++ Read camera properties config file from:\n");
					OutputDebugString(msg);
					sprintf(msg, "   %s\n", solar_system_hdl_filename);
					OutputDebugString(msg);
				}


			// ---- store
				strcpy(pi_system_param->solar_system_hdl_filename, solar_system_hdl_filename);
				pi_system_param->config_hdl = config_hdl;


			// ---- yay!
				return VS_OK;

		}

	return VS_ERROR;
}


// ---------- Write ----------
/*!
\brief Write solar system configuration hdl file
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT SystemParam::Write()
{

	// ---- file exists ?
		if ( GetFileAttributes(pi_system_param->solar_system_hdl_filename) != INVALID_FILE_ATTRIBUTES )
		{

			// ---- ok ?

			// ---- scale objects name
				CHAR *scale_name[3] =
				{
					"Standard",
					"Large",
					"Graphic",
				};
				vs_system::VNode *d = NULL, *c = NULL;
				vs_system::VNode *v = NULL;

			// ---- camera
				v = pi_system_param->config_hdl->SearchThis("Explore");
				if ( v != NULL )
				{
					if ( c = v->SearchName("Update")   ) c->SetValue((INT)pi_system_param->kamera_control_mode);
					if ( c = v->SearchName("Target")   ) c->SetValue((INT)pi_system_param->target_object_id     );
				}

			// ---- display
				v = pi_system_param->config_hdl->SearchThis("Display");
				if ( v != NULL )
				{
					if ( c = v->SearchName("Mode")     ) c->SetValue((INT)pi_system_param->system_view_mode     );
				}

			// ---- scale
				v = pi_system_param->config_hdl->SearchThis("Scale");
				if ( v != NULL )
				{
					if ( c = v->SearchName("Global")   ) c->SetValue((FLOAT)pi_system_param->scale_global       );
					for (UINT index = 0; index < 3; index++)
					{
						d = c->SearchThis(scale_name[index]);
						if ( d != NULL )
						{
							d->SetValue((FLOAT)pi_system_param->scale_object[index]);
						}
					}

				}

			// ---- scalar
				v = pi_system_param->config_hdl->SearchThis("Scalar");
				if ( v != NULL )
				{
					if ( c = v->SearchName("Day")      ) c->SetValue((FLOAT)pi_system_param->daylen_scalar      );
					if ( c = v->SearchName("Time")     ) c->SetValue((FLOAT)pi_system_param->time_scalar        );
				}


			// ---- time
				v = pi_system_param->config_hdl->SearchThis("Time");
				if ( v != NULL )
				{
					if ( c = v->SearchName("Pause")    ) c->SetValue((INT)pi_system_param->pause_mode           );
					if ( c = v->SearchName("Offset")   ) c->SetValue((INT)pi_system_param->ms_offset            );
				}

			
			// ---- write config file
				vs_system::VNodeIO vio;
				INT result = vio.Write(
						pi_system_param->solar_system_hdl_filename,
						pi_system_param->config_hdl
					);


			// ---- report
				CHAR msg[VS_MAXCHARPATHLEN];
				if ( result == VS_OK )
				{
					sprintf(msg, "+++ SAVED camera properties config file to:\n");
					OutputDebugString(msg);
					sprintf(msg, "   %s\n", pi_system_param->solar_system_hdl_filename);
					OutputDebugString(msg);
				}
				else
				{
					sprintf(msg, "+++ FAILED to save camera properties config file to:\n");
					OutputDebugString(msg);
					sprintf(msg, "   %s\n", pi_system_param->solar_system_hdl_filename);
					OutputDebugString(msg);
				}


			// ---- successful ? -- return error
				if ( result == VS_ERROR ) return VS_ERROR;


			// ---- yay!
				return VS_OK;

	}

	return VS_ERROR;
}


// ---------- system light mode ----------
	UINT SystemParam::GetSystemLightMode()         { return pi_system_param->star_light_mode; }
	VOID SystemParam::SetSystemLightMode(UINT ldm) { pi_system_param->star_light_mode = ldm;  }

	UINT SystemParam::IncSystemLightMode()
	{
		pi_system_param->star_light_mode += 1;
		pi_system_param->star_light_mode =
			pi_system_param->star_light_mode > SystemParam::SystemLightMode::GRAPHIC_COLOUR ?
				SystemParam::SystemLightMode::NORMAL_COLOUR :
					pi_system_param->star_light_mode;
		return pi_system_param->star_light_mode;
	}


// ---------- system view mode ----------
	UINT SystemParam::GetSystemViewMode()              { return pi_system_param->system_view_mode; }
	VOID SystemParam::SetSystemViewMode(UINT vsm)      { pi_system_param->system_view_mode = vsm;  }
	BOOL SystemParam::GetSystemViewModeChanged()       { return pi_system_param->system_view_update; }
	VOID SystemParam::SetSystemViewModeChanged(BOOL u) { pi_system_param->system_view_update = u;    }

	UINT SystemParam::IncSystemViewMode()
	{
		pi_system_param->system_view_mode += 1;
		pi_system_param->system_view_mode =
			pi_system_param->system_view_mode > SystemParam::SystemViewMode::GRAPHIC_SIZE ?
				SystemParam::SystemViewMode::STANDARD_SIZE :
					pi_system_param->system_view_mode;
		return pi_system_param->system_view_mode;
	}


// ---------- gfx ----------
	BOOL SystemParam::GetGfxVersion()
	{
		UINT gfx_mode = GetSystemViewMode();
		BOOL gfx_version = FALSE;
		switch (gfx_mode)
		{
			case SystemParam::SystemViewMode::GRAPHIC_SIZE:
				gfx_version = TRUE;
				break;
			case SystemParam::SystemViewMode::STANDARD_SIZE:
			case SystemParam::SystemViewMode::LARGE_SIZE:
			default:
				break;
		}
		return gfx_version;
	}


// ---------- object ----------
	FLOAT SystemParam::GetObjectDiameterScalar(UINT type)
	{
		DOUBLE object_diameter_scalar = 1;
		switch ( type )
		{
			case SystemObject::Type::UnKnown:
				object_diameter_scalar = 0;
				break;
			case SystemObject::Type::Asteroid:
				object_diameter_scalar = GetObjectScale() * 60;
				break;
			case SystemObject::Type::AstRing:
			case SystemObject::Type::Meteroid:
				object_diameter_scalar = GetObjectScale() * 10;
				break;
			case SystemObject::Type::Starfield:
				object_diameter_scalar = GetObjectScale() * 1;
				break;
			default:
				object_diameter_scalar = GetObjectScale();
				break;
		}
		return (FLOAT)object_diameter_scalar;
	}


// ---------- pause ----------
	INT    SystemParam::GetPauseMode()                  { return pi_system_param->pause_mode;             }
	VOID   SystemParam::SetPauseMode(INT pm)            { pi_system_param->pause_mode = pm;               }
	DOUBLE SystemParam::GetPauseTimeStart()             { return pi_system_param->paused_start_time;      }
	VOID   SystemParam::SetPauseTimeStart(DOUBLE pst)   { pi_system_param->paused_start_time = pst;       }
	DOUBLE SystemParam::GetPauseTimeElapsed()           { return pi_system_param->paused_elapsed_time;    }
	VOID   SystemParam::SetPauseTimeElapsed(DOUBLE pet) { pi_system_param->paused_elapsed_time = pet;     }


// ---------- scale (geometry) ----------
	DOUBLE SystemParam::GetGlobalScale()                { return pi_system_param->scale_global;           }
	DOUBLE SystemParam::GetObjectScale()
	{
		return pi_system_param->scale_object[pi_system_param->system_view_mode];
	}

	INT    SystemParam::SetGlobalScale(DOUBLE s)        { pi_system_param->scale_global  = s; return VS_OK; }
	INT    SystemParam::SetObjectScale(DOUBLE s)
	{
		pi_system_param->scale_object[pi_system_param->system_view_mode] = s;
		return VS_OK;
	}


// ---------- scalar (value) ----------
	DOUBLE SystemParam::GetTimeScalar()                 { return pi_system_param->time_scalar;            }
	DOUBLE SystemParam::GetDayLenScalar()               { return pi_system_param->daylen_scalar;          }
	FLOAT  SystemParam::GetTargetDistScalar()
	{
		return (FLOAT)pi_system_param->scale_object[pi_system_param->system_view_mode] * 0.1f;
	}

	INT SystemParam::SetTimeScalar(DOUBLE s)            { pi_system_param->time_scalar   = s; return VS_OK; }
	INT SystemParam::SetDayLenScalar(DOUBLE s)          { pi_system_param->daylen_scalar = s; return VS_OK; }


// ---------- time ----------
	LONG   SystemParam::GetMsOffset()                   { return pi_system_param->ms_offset; }
	INT    SystemParam::SetMsOffset(LONG l)             { pi_system_param->ms_offset = l; return VS_OK; }


////////////////////////////////////////////////////////////////////////////////