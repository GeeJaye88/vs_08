////////////////////////////////////////////////////////////////////////////////

// ---------- vs_app_camera.cpp ----------
/*!
\file vs_app_camera.cpp
\brief Implementation of the App(lication)Camera class
\author Gareth Edwards
\note based on Frank Luna framework
*/

#include <stdio.h>

#include "../header/vs_app_kamera.h"

using namespace vs_system;


void output_si(CHAR *s, INT i)
{
	char msg[64];
	sprintf(msg, "%s %d \n", s, i);
	OutputDebugString(msg);
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_AppKamera class ----------
class AppKamera::PI_AppKamera
{

public:

	// ---- cdtor
		PI_AppKamera() { ; };
		PI_AppKamera(Type kamera_type) { _kamera_type = kamera_type; };
		~PI_AppKamera() { ; };


	// ---- PROPERTIES

	// ---- type
		/*
			enum Type {
					LAND   = 0,
					AIR    = 1, // -> default
					LOOKAT = 2
				};
		*/
		Type _kamera_type = AppKamera::AIR;


	// --- target
		BOOL  _target_on                = FALSE;
		FLOAT _target_zoom_incr         = 1;
		FLOAT _target_zoom              = 1; // this is THE zoom
		FLOAT _target_zoom_minimum      = 1;
		FLOAT _target_zoom_maximum      = 100;

	// ---- target zoom change
		FLOAT _target_change_zoom_power = 1;
		FLOAT _target_change_zoom_denom = 1;

	// ---- target location from & too
		VsVector3 _v_target_from        = VsVector3(0, 0, 0);
		VsVector3 _v_target_position    = VsVector3(0, 0, 0);

	// ---- target defaults
		FLOAT _target_zoom_incr_default = _target_zoom_incr;
		FLOAT _target_zoom_default      = _target_zoom;

	// ---- target radius
		VsVector3 _target_change_v = VsVector3(0, 0, 0);
		FLOAT _target_change_distance = 100;
		FLOAT _target_change_value = 0;

	// ---- target from -> too
		FLOAT _target_change_incr_speed = 1;
		INT   _target_change_incr_steps = 100;
		INT   _target_change_incr_count = _target_change_incr_steps;

	// --- target previous
		FLOAT _previous_target_on_pan   = 0;
		FLOAT _previous_target_on_tilt  = 0;
		VsVector3 _previous_target_on_v = VsVector3(0, 0, 0);


	// ---- increment (state)
		FLOAT _scalar_incr    = 1.0f;
		FLOAT _pan_tilt_incr  = 1.0f;
		FLOAT _position_incr  = 1.0f;
		FLOAT _rotate_incr    = 1.0f;
		FLOAT _translate_incr = 1.0f;

	// ---- flags
		BOOL  _scalar_mode     = TRUE;  // high (T) or low (F)
		BOOL  _scalar_rotation = TRUE;  // set/get & used only in IncRotate

		FLOAT _zaam_incr       = 1.0f;  // set/get, but not used


	// ---- pan & tilt (state)
		FLOAT _pan           = 0.0f;
		FLOAT _tilt          = 0.0f;

	// ---- pan & tilt defaults
		FLOAT _pan_default   = _pan;
		FLOAT _tilt_default  = _tilt;


	// ---- vector (state)
		VsVector3 _v              = VsVector3(0, 0, 0);
		VsVector3 _look           = VsVector3(0, 0, 1);
		VsVector3 _right          = VsVector3(1, 0, 0);
		VsVector3 _up             = VsVector3(0, 1, 0);
		VsVector3 _world          = VsVector3(0, 0, 0);
		VsVector3 _rotate         = VsVector3(0, 0, 0);

	// ---- vector defaults
		VsVector3 _v_default      = VsVector3(0, 0, 0);
		VsVector3 _world_default  = VsVector3(0, 0, 0);
		VsVector3 _rotate_default = VsVector3(0, 0, 0);


	// ---- state
		VsVector3 _state_vector[6] =
			{
				_v, _look, _right, _up, _world,  _rotate
			};
		FLOAT _state_float[8] =
			{
				_scalar_incr , _pan_tilt_incr , _position_incr , _rotate_incr,
				_translate_incr , _zaam_incr , _pan , _tilt
			};


	// ---- utility
		BOOL has_been_updated = FALSE;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief allocate and construct private properties
\author Gareth Edwards
*/
AppKamera::AppKamera()
{
	pi_app_kamera = new PI_AppKamera();
}

AppKamera::AppKamera(Type kamera_type)
{
	pi_app_kamera = new PI_AppKamera(kamera_type);
}


// ---------- Destructor ----------
/*!
\brief delete private properties
\author Gareth Edwards
*/
AppKamera::~AppKamera()
{
	if ( pi_app_kamera != NULL ) delete pi_app_kamera;
}


////////////////////////////////////////////////////////////////////////////////


// ---- TYPE ---- 
/*

	Type of Camera behaviour.

	Selected from:

		enum Type {
				LAND   = 0,
				AIR    = 1, // -> default
				LOOKAT = 2
			};

*/

AppKamera::Type AppKamera::GetKameraType()
{
	return pi_app_kamera->_kamera_type;
}
void AppKamera::SetKameraType(Type kamera_type)
{
	pi_app_kamera->_kamera_type = kamera_type;
}


////////////////////////////////////////////////////////////////////////////////


// ---- TARGET ---- 

/*
	
	Its's complicated!!!

	Initialise
	==========

		// ---- kamera proportional value
			FLOAT kpv = dist / 1000.0f;

		// ---- target
			kamera->SetTargetIncr(kpv);
			kamera->SetTargetZoom(dist/2);

		// ---- change
			kamera->SetTargetChangeZoomPower(1.5f);
			kamera->SetTargetChangeZoomDenom(20.0f);
			kamera->SetTargetChangeSpeed(2);

		// ---- change
			kamera->SetTargetZoomMinimum(dist / 4.0f);
			kamera->SetTargetZoomMaximum(dist * 2.5f);


	Pan, Tilt and Zoom around a target location
	===========================================

	Example:

		case AppKamera::Mode::TARGET :
			switch ( message )
			{
				case 'R': kamera->IncTargetTilt(-delta);   break;
				case 'F': kamera->IncTargetTilt( delta);   break;
				case 'A': kamera->IncTargetPan( delta);    break;
				case 'D': kamera->IncTargetPan(-delta);    break;
				case 'W': kamera->IncTargetZoom(-delta);   break;
				case 'S': kamera->IncTargetZoom(delta);    break;
				case 'J':
					{
						kamera->IncTargetZoom(delta/5);
						kamera->IncTargetTilt(delta);
						kamera->IncTargetPan(delta);
					}
					break;
				case 'K':
					{
						kamera->IncTargetTilt(delta);
						kamera->IncTargetPan(delta);
					}
					break;
				case 'L':
					{
						kamera->IncTargetZoom(-delta / 5);
						kamera->IncTargetTilt(delta);
						kamera->IncTargetPan(delta);
					}
					break;
				default :
					break;
			}
			break;


	New Target
	==========

	Example:

		if ( new_target )
		{

			// ---- zoom
				app_kamera[app_kamera_index]->SetTargetZoomMinimum(object_diameter *  0.3f);
				app_kamera[app_kamera_index]->SetTargetZoomMaximum(object_diameter *  3.0f);

			// ---- change
				app_kamera[app_kamera_index]->SetTargetChangeZoomPower(1.5f);
				app_kamera[app_kamera_index]->SetTargetChangeZoomDenom(20.0f);
				if ( target_id > 0 && target_id < 6 )
				{
					app_kamera[app_kamera_index]->SetTargetChangeSpeed(4);
				}
				else
				{
					app_kamera[app_kamera_index]->SetTargetChangeSpeed(2);
				}

			// ---- target
				SetTargetObjectId(target_id);

		}

*/

// ---- pan
	void AppKamera::IncTargetPan(FLOAT v)
	{

		pi_app_kamera->_target_on = FALSE;

		pi_app_kamera->_pan += pi_app_kamera->_pan_tilt_incr * v * pi_app_kamera->_scalar_incr; // rotate scaler_incr
		SetPanTiltAxis();

		pi_app_kamera->_target_on = TRUE;

	}

	FLOAT AppKamera::GetTargetPan()
	{
		return pi_app_kamera->_pan;
	}

	void AppKamera::SetTargetPan(FLOAT v)
	{
		pi_app_kamera->_pan = v;
	}


// ---- tilt

	void AppKamera::IncTargetTilt(FLOAT v)
	{
		pi_app_kamera->_target_on = FALSE;
		pi_app_kamera->_tilt += pi_app_kamera->_pan_tilt_incr * v * pi_app_kamera->_scalar_incr; // rotate scaler_incr
		SetPanTiltAxis();
		pi_app_kamera->_target_on = TRUE;
	}

	FLOAT AppKamera::GetTargetTilt()
	{
		return pi_app_kamera->_tilt;
	}

	void AppKamera::SetTargetTilt(FLOAT v)
	{
		pi_app_kamera->_target_on = FALSE;
		pi_app_kamera->_tilt = v;
		SetPanTiltAxis();
		pi_app_kamera->_target_on = TRUE;
	}


// ---- zoom

	void AppKamera::IncTargetZoom(FLOAT delta)
	{

		FLOAT target_zoom =
			pi_app_kamera->_target_zoom +
					( pi_app_kamera->_target_zoom_incr * delta);

		FLOAT target_minimum = pi_app_kamera->_target_zoom_minimum;
		FLOAT target_maximum = pi_app_kamera->_target_zoom_maximum;

		target_zoom = target_zoom < target_minimum ? target_minimum :
			( target_zoom > target_maximum ? target_maximum : target_zoom);

		pi_app_kamera->_target_zoom = target_zoom;

	};

	void AppKamera::SetTargetZoomMinMax()
	{

		FLOAT target_zoom    = pi_app_kamera->_target_zoom;
		FLOAT target_minimum = pi_app_kamera->_target_zoom_minimum;
		FLOAT target_maximum = pi_app_kamera->_target_zoom_maximum;

		target_zoom = target_zoom < target_minimum ? target_minimum :
			(target_zoom > target_maximum ? target_maximum : target_zoom);

		pi_app_kamera->_target_zoom = target_zoom;

	};

	FLOAT AppKamera::GetTargetZoom()
	{
		return pi_app_kamera->_target_zoom;
	};

	void AppKamera::SetTargetZoom(FLOAT zoom)
	{
		pi_app_kamera->_target_zoom = zoom;
	};

	void AppKamera::SetTargetZoomDefault()
	{
		pi_app_kamera->_target_zoom = pi_app_kamera->_target_zoom_default;
	};


// ---- target change zoom minimum

	FLOAT AppKamera::GetTargetZoomMinimum()
	{
		return pi_app_kamera->_target_zoom_minimum;
	};

	void AppKamera::SetTargetZoomMinimum(FLOAT minimum)
	{
		pi_app_kamera->_target_zoom_minimum = minimum;
	};

	FLOAT AppKamera::GetTargetZoomMaximum()
	{
		return pi_app_kamera->_target_zoom_maximum;
	};

	void AppKamera::SetTargetZoomMaximum(FLOAT maximum)
	{
		pi_app_kamera->_target_zoom_maximum = maximum;
	};

// ---- target change power function param

	void AppKamera::SetTargetChangeZoomPower(FLOAT power)
	{
		pi_app_kamera->_target_change_zoom_power = power;
	};

	void AppKamera::SetTargetChangeZoomDenom(FLOAT distance)
	{
		pi_app_kamera->_target_change_zoom_denom = distance;
	};

	void AppKamera::SetTargetChangeSpeed(FLOAT speed)
	{
		pi_app_kamera->_target_change_incr_speed = speed;
	};


// ---- target increment

	FLOAT AppKamera::GetTargetIncr()                    { return pi_app_kamera->_target_zoom_incr; };
	void  AppKamera::SetTargetIncr(FLOAT scalar)        { pi_app_kamera->_target_zoom_incr = scalar; };


// ---- target position

	void  AppKamera::GetTargetPosition(VsVector3 *t)
	{
		*t = pi_app_kamera->_v_target_position;
	};

	void AppKamera::SetTargetPosition(VsVector3 *v_target_position, BOOL new_target)
	{

		// ------------ setup ------------

		// ---- no target ? -- store pre target pan, tilt and position...
			if ( !pi_app_kamera->_target_on )
			{
				pi_app_kamera->_previous_target_on_pan  = pi_app_kamera->_pan;
				pi_app_kamera->_previous_target_on_tilt = pi_app_kamera->_tilt;
				pi_app_kamera->_previous_target_on_v    = pi_app_kamera->_v;
			}

		// ---- !! target always ON !!
			pi_app_kamera->_target_on = TRUE;
	
		// ---- shortcut from & too...
			VsVector3 *too  = &pi_app_kamera->_v;
			VsVector3 *from = &pi_app_kamera->_v_target_from;
			INT target_change_incr_count = pi_app_kamera->_target_change_incr_count;
			INT target_change_incr_steps = pi_app_kamera->_target_change_incr_steps;


		// ---- new target ?
			if ( new_target )
			{

				// ---- calculate distance between new and old targets
					FLOAT xd = v_target_position->x - pi_app_kamera->_previous_target_on_v.x;
					FLOAT yd = v_target_position->y - pi_app_kamera->_previous_target_on_v.y;
					FLOAT zd = v_target_position->z - pi_app_kamera->_previous_target_on_v.z;
					pi_app_kamera->_target_change_distance = (FLOAT)sqrt(xd*xd + yd * yd + zd * zd);

				// ---- store new target
					pi_app_kamera->_previous_target_on_v = *v_target_position;

					//{ v_target_position->x, v_target_position->y, v_target_position->z };

				// ---- set # of animation steps proportional to distance
					target_change_incr_count = 0;
					target_change_incr_steps = (INT)(pi_app_kamera->_target_change_distance)/2;
					target_change_incr_steps = target_change_incr_steps < 1 ? 1 : target_change_incr_steps;

				// ---- copy
					*from = *too;

			}


		// ---- set target position
			pi_app_kamera->_v_target_position = *v_target_position;


		// ---- shortcut zoom target scalar
			FLOAT target_zoom = pi_app_kamera->_target_zoom;


		// ------------ interpolation ------------

			FLOAT rmu = (FLOAT)target_change_incr_count / (FLOAT)target_change_incr_steps;

		// ---- zoom value / zoom_limit >= 0 && <= 1.0
			FLOAT zoom_limit = pi_app_kamera->_target_zoom_maximum -
				pi_app_kamera->_target_zoom_minimum;
			FLOAT zoom_value = target_zoom -
				pi_app_kamera->_target_zoom_minimum;
			zoom_value = zoom_value < 0 ? 0 :
				(zoom_value > zoom_limit ? zoom_limit : zoom_value);


		// ---- don't do this...
			//
			// e.g. zoom_value *= rmu; // added 5/9/20
			//
			// zoom is "fed" by zoom events being selected
			// or being persistently "fed" into the kamera
			//

		// ---- zoom interpolation factor between "zoom from & zoom too"
			//
			//  note: allows for zoom to be proportional to the size of
			//        the from and too objects (e.g. planets, moons)
			//
			BOOL _zoom_mode = 0;
			switch ( _zoom_mode )
			{

				case 0:
					{
						// ---- linear interpolation (no fair)
							target_zoom =
								pi_app_kamera->_target_zoom_minimum +
									zoom_value / zoom_limit * zoom_limit;
					}
					break;

				case 1:
					{
						// ---- sine interpolation (fair at start and end)
							FLOAT target_zoom = 
								((FLOAT)sin((FLOAT)VS_RADIANS_PER_DEGREE * (zoom_value / zoom_limit * 180 - 90)) + 1) / 2;
					}
					break;

				case 2:
					{
						// ---- power interpolation (fair at start only)
							target_zoom =
								pi_app_kamera->_target_zoom_minimum +
									(FLOAT)pow(zoom_value, pi_app_kamera->_target_change_zoom_power) /
										pi_app_kamera->_target_change_zoom_denom;
					}
					break;
			}

		
		// ------------ zoom------------


		// ---- set "too" - zoom along look direction vector
			VsVector3 *look_direction = &pi_app_kamera->_look;
			too->x = v_target_position->x - look_direction->x * target_zoom;
			too->y = v_target_position->y - look_direction->y * target_zoom;
			too->z = v_target_position->z - look_direction->z * target_zoom;


		// ------------ position ------------


		// ---- sine function interpolate between positions "from & too" over "incr_steps"
			rmu = ( (FLOAT)sin( (FLOAT)VS_RADIANS_PER_DEGREE * (rmu * 180 - 90) ) + 1) / 2;
			FLOAT one_minus_rmu = 1 - rmu;
			too->x = one_minus_rmu * from->x + rmu * too->x;
			too->y = one_minus_rmu * from->y + rmu * too->y;
			too->z = one_minus_rmu * from->z + rmu * too->z;


	// ------------ increment ------------


		// ---- increment count...
			target_change_incr_count++;

		// ---- ... limited by max # of increment steps
			target_change_incr_count =
					target_change_incr_count >= target_change_incr_steps ?
						target_change_incr_steps :
							target_change_incr_count;
			pi_app_kamera->_target_change_incr_count = target_change_incr_count;


		// ---- flag
			pi_app_kamera->has_been_updated = TRUE;

	};


// ---- target state

	BOOL AppKamera::GetTargetStatus() { return pi_app_kamera->_target_on;  }

	void AppKamera::SetTargetStatus(BOOL s)
	{

		// ---- false ? -- restore pre-target param
			if ( !s )
			{
				if ( pi_app_kamera->_target_on )
					SetPanTiltAxis();
			}
			else
			{
				pi_app_kamera->_target_on = TRUE;
				pi_app_kamera->_previous_target_on_pan  = pi_app_kamera->_pan;
				pi_app_kamera->_previous_target_on_tilt = pi_app_kamera->_tilt;
				pi_app_kamera->_previous_target_on_v    = pi_app_kamera->_v;

			}

	}


////////////////////////////////////////////////////////////////////////////////


// ---- AXIS ---- 

/*
	Note: often use in conjunction with Position.

	For example:

		// ---- view from....
			FLOAT view_ratio = 0.5;
			VsVector3 p = { 0, scene_dimension * view_ratio/2, -scene_dimension * view_ratio };
			kamera->SetPosition(&p);

		// ---- ....looking towards
			VsVector3 l = { 0.0f, -view_ratio/3, view_ratio };
			kamera->SetLook(&l);

*/

void AppKamera::GetLook(VsVector3 *v)
{
	*v = pi_app_kamera->_look;
}

void AppKamera::GetRight(VsVector3 *v)
{
	*v = pi_app_kamera->_right;
}

void AppKamera::GetUp(VsVector3 *v)
{
	*v = pi_app_kamera->_up;
}

void AppKamera::SetLook(VsVector3 *l)
{

	// ---- lambda
		auto cross_product = [](VsVector3 *a, VsVector3 *b, VsVector3 *c)
		{
			c->x = a->y * b->z - a->z * b->y;
			c->y = a->z * b->x - a->x * b->z;
			c->z = a->x * b->y - a->y * b->x;
		};


	// ---- normalise
		l->Normalise();
		pi_app_kamera->_look = *l;


	// ---- orthogonality
		cross_product(
			&pi_app_kamera->_look,
			&pi_app_kamera->_up,
			&pi_app_kamera->_right
		);

		cross_product(
			&pi_app_kamera->_right,
			&pi_app_kamera->_look,
			&pi_app_kamera->_up
		);

		float fred = 2;
}

void AppKamera::SetRight(VsVector3 *v)
{
	pi_app_kamera->_right = *v;
}

void AppKamera::SetUp(VsVector3 *v)
{
	pi_app_kamera->_up = *v;
}


////////////////////////////////////////////////////////////////////////////////


// ---- DEFAULT ---- 

/*

	Note: often use in conjunction with State.

	For example:

		kamera->SaveDefaults();
		kamera->SaveState();

*/

void AppKamera::SaveDefaults()
{

	// ---- target zoom
		pi_app_kamera->_target_zoom_incr_default = pi_app_kamera->_target_zoom_incr;
		pi_app_kamera->_target_zoom_default      = pi_app_kamera->_target_zoom;

	// ---- pan & tilt
		pi_app_kamera->_pan_default    = pi_app_kamera->_pan;
		pi_app_kamera->_tilt_default   = pi_app_kamera->_tilt;

	// ---- vectors
		pi_app_kamera->_v_default      = pi_app_kamera->_v;
		pi_app_kamera->_world_default  = pi_app_kamera->_world;
		pi_app_kamera->_rotate_default = pi_app_kamera->_rotate;
}


////////////////////////////////////////////////////////////////////////////////


// ---- INCREMENT ----

/*

	Example:

		// ---- overall increment & its state (e.g. large == TRUE)
			kamera->SetScalarIncr(kpv * 2);
			kamera->SetScalarRotation(TRUE);
			kamera->SetScalarMode(TRUE);

		// ---- increment
			kamera->SetPositionIncr(kpv / 2);
			kamera->SetTranslateIncr(kpv);
			kamera->SetZoomIncr(kpv / 4);

		// ---- increment not proportional
			kamera->SetPanTiltIncr(1.0);
			kamera->SetRotateIncr(1.0);


*/

FLOAT AppKamera::GetScalarIncr()              { return pi_app_kamera->_scalar_incr;    };
FLOAT AppKamera::GetPositionIncr()            { return pi_app_kamera->_position_incr;  };
FLOAT AppKamera::GetPanTiltIncr()             { return pi_app_kamera->_pan_tilt_incr;  };
FLOAT AppKamera::GetRotateIncr()              { return pi_app_kamera->_rotate_incr;    };
FLOAT AppKamera::GetTranslateIncr()           { return pi_app_kamera->_translate_incr; };
FLOAT AppKamera::GetZoomIncr()                { return pi_app_kamera->_zaam_incr;      };

void  AppKamera::SetScalarIncr(FLOAT incr)    { pi_app_kamera->_scalar_incr    = incr; };
void  AppKamera::SetPositionIncr(FLOAT incr)  { pi_app_kamera->_position_incr  = incr; };
void  AppKamera::SetPanTiltIncr(FLOAT incr)   { pi_app_kamera->_pan_tilt_incr  = incr; };
void  AppKamera::SetRotateIncr(FLOAT incr)    { pi_app_kamera->_rotate_incr    = incr; };
void  AppKamera::SetTranslateIncr(FLOAT incr) { pi_app_kamera->_translate_incr = incr; };
void  AppKamera::SetZoomIncr(FLOAT incr)      { pi_app_kamera->_zaam_incr      = incr; };

BOOL  AppKamera::GetScalarMode()              { return pi_app_kamera->_scalar_mode;     };
void  AppKamera::SetScalarMode(BOOL mode)     { pi_app_kamera->_scalar_mode = mode;     };

BOOL  AppKamera::GetScalarRotation()          { return pi_app_kamera->_scalar_rotation; };
void  AppKamera::SetScalarRotation(BOOL mode) { pi_app_kamera->_scalar_rotation = mode; };


////////////////////////////////////////////////////////////////////////////////


// ---- MATRIX ---- 

/*

	Example:

		INT Model::SetMyKamera(INT kamera_index)
		{

			vs_system::AppKamera *kamera = pi_model->app_kamera[kamera_index];
			VsMatrix V;
			kamera->GetViewMatrix(&V);
			pi_model->gfx->SetViewMatrix(1, &V);

			return VS_OK;
		}

*/

void AppKamera::GetViewMatrix(
		VsMatrix *vm
	)
{

	// ---- lambda
		auto dot_product = [](VsVector3 *a, VsVector3 *b)
		{
			FLOAT d = 0;
			d += a->x * b->x;
			d += a->y * b->y;
			d += a->z * b->z;
			return d;
		};


	// ---- assumes all vectors normalised


	// ---- keep camera's axes orthogonal to each other
		//
		// e.g. not required
		//
		// cross_product(&pi_app_kamera->_look, &pi_app_kamera->_right, &pi_app_kamera->_up);
		// cross_product(&pi_app_kamera->_up,   &pi_app_kamera->_look,  &pi_app_kamera->_right);


	// ---- build the view matrix
		FLOAT x = -dot_product(&pi_app_kamera->_right, &pi_app_kamera->_v);
		FLOAT y = -dot_product(&pi_app_kamera->_up,    &pi_app_kamera->_v);
		FLOAT z = -dot_product(&pi_app_kamera->_look,  &pi_app_kamera->_v);
		VsVector3 *r = &pi_app_kamera->_right;
		VsVector3 *u = &pi_app_kamera->_up;
		VsVector3 *l = &pi_app_kamera->_look;
		FLOAT *m = vm->m[0];
		m[0]  = r->x; m[1]  = u->x; m[2]  = l->x; m[3]  = 0.0f;
		m[4]  = r->y; m[5]  = u->y; m[6]  = l->y; m[7]  = 0.0f;
		m[8]  = r->z; m[9]  = u->z; m[10] = l->z; m[11] = 0.0f;
		m[12] = x;    m[13] = y;    m[14] = z;    m[15] = 1.0f;

	//char msg[128];
	//sprintf(msg, "-> pos %6.4f %6.4f %6.4f\n", x, y, z);
	//OutputDebugString(msg);
}


////////////////////////////////////////////////////////////////////////////////


// ---- PAN & TILT ---- 

/*

	Example:

		case AppKamera::Mode::TARGET :
			switch ( message )
			{
				case 'R': kamera->IncTargetTilt(-delta);   break;
				case 'F': kamera->IncTargetTilt( delta);   break;
				case 'A': kamera->IncTargetPan( delta);    break;
				case 'D': kamera->IncTargetPan(-delta);    break;
				case 'W': kamera->IncTargetZoom(-delta);   break;
				case 'S': kamera->IncTargetZoom(delta);    break;
				case 'J':
					{
						kamera->IncTargetZoom(delta/5);
						kamera->IncTargetTilt(delta);
						kamera->IncTargetPan(delta);
					}
					break;
				case 'K':
					{
						kamera->IncTargetTilt(delta);
						kamera->IncTargetPan(delta);
					}
					break;
				case 'L':
					{
						kamera->IncTargetZoom(-delta / 5);
						kamera->IncTargetTilt(delta);
						kamera->IncTargetPan(delta);
					}
					break;
				default :
					break;
			}
			break;

*/

void AppKamera::GetPanTilt(FLOAT *pan, FLOAT *tilt)
{
	*pan  = pi_app_kamera->_pan;
	*tilt = pi_app_kamera->_tilt;
}

void AppKamera::SetPanTiltDefault()
{
	pi_app_kamera->_pan  = pi_app_kamera->_pan_default;
	pi_app_kamera->_tilt = pi_app_kamera->_tilt_default;
	SetPanTiltAxis();
}

void AppKamera::SetPanTilt(FLOAT pan, FLOAT tilt)
{
	pi_app_kamera->_pan  = pan;
	pi_app_kamera->_tilt = tilt;
	SetPanTiltAxis();
}


void AppKamera::IncPan(FLOAT delta)
{
	pi_app_kamera->_pan += pi_app_kamera->_pan_tilt_incr * delta * pi_app_kamera->_scalar_incr; // rotate scaler_incr
	SetPanTiltAxis();
}

void AppKamera::IncTilt(FLOAT delta)
{
	pi_app_kamera->_tilt += pi_app_kamera->_pan_tilt_incr * delta * pi_app_kamera->_scalar_incr; // rotate scaler_incr
	SetPanTiltAxis();
}

void AppKamera::SetPanTiltAxis()
{

	// ---- restore pre target on pan, tilt and position ?
		if ( pi_app_kamera->_target_on )
		{
			pi_app_kamera->_target_on = FALSE;
			pi_app_kamera->_pan  = pi_app_kamera->_previous_target_on_pan;
			pi_app_kamera->_tilt = pi_app_kamera->_previous_target_on_tilt;
			pi_app_kamera->_v    = pi_app_kamera->_previous_target_on_v;
		}


	// ---- local
		FLOAT rad = (FLOAT)0.01745329252;


	// ---- for each view vector
		VsVector3 v[3] =
		{
			{ 0, 0, 1 }, // look
			{ 1, 0, 0 }, // right
			{ 0, 1, 0 }  // up
		};
		VsVector3 *r[3] =
		{
			&pi_app_kamera->_look,
			&pi_app_kamera->_right,
			&pi_app_kamera->_up
		};

		for (INT i = 0; i < 3; i++)
		{

			// ---- tilt around x axis
				// x' = 0
				// y' = y*cos q - z*sin q
				// z' = y*sin q + z*cos q
				FLOAT tilt_2_rad = pi_app_kamera->_tilt * rad;
				FLOAT xt = v[i].x;
				FLOAT yt = v[i].y * cosf(tilt_2_rad) - v[i].z * sinf(tilt_2_rad);
				FLOAT zt = v[i].y * sinf(tilt_2_rad) + v[i].z * cosf(tilt_2_rad);

			// ---- pan around y axis
				// x' = z*sin q + x*cos q
				// y' = y
				// z' = z*cos q - x*sin q
				FLOAT pan_2_rad = pi_app_kamera->_pan * rad;
				FLOAT xp = zt * sinf(pan_2_rad) + xt * cosf(pan_2_rad);
				FLOAT zp = zt * cosf(pan_2_rad) - xt * sinf(pan_2_rad);

			// ---- store
				*r[i] = VsVector3(xp, yt, zp);

		}


	// ---- flag
		pi_app_kamera->has_been_updated = TRUE;

}


////////////////////////////////////////////////////////////////////////////////


// ---- POSITION ---- 

/*

	Example:

		case AppKamera::Mode::VECTOR :
			switch ( message )
			{
				case 'R': kamera->IncPosDir(AppKamera::Direction::UP,    delta); break;
				case 'F': kamera->IncPosDir(AppKamera::Direction::UP,   -delta); break;
				case 'A': kamera->IncPosDir(AppKamera::Direction::RIGHT,-delta); break;
				case 'D': kamera->IncPosDir(AppKamera::Direction::RIGHT, delta); break;
				case 'W': kamera->IncPosDir(AppKamera::Direction::LOOK,  delta); break;
				case 'S': kamera->IncPosDir(AppKamera::Direction::LOOK, -delta); break;
				default:
					break;
			}
			break;

*/

void AppKamera::GetPosition(VsVector3 *v)
{
	*v = pi_app_kamera->_v;
}

void AppKamera::SetPositionDefault()
{

	// ---- update
		pi_app_kamera->_v = pi_app_kamera->_v_default;

	// ---- flag
		pi_app_kamera->has_been_updated = TRUE;

}

void AppKamera::SetPosition(FLOAT x, FLOAT y, FLOAT z)
{
	VsVector3 v = { x, y, z };
	SetPosition(&v);
}

void AppKamera::SetPosition(VsVector3 *v)
{

	// ---- update
		pi_app_kamera->_v = *v;

	// ---- flag
		pi_app_kamera->has_been_updated = TRUE;

}

INT AppKamera::IncPosDir(Direction d, FLOAT delta)
{

	VsVector3 *dv = NULL;
	switch ( d )
	{
		case Direction::LOOK:  dv = &pi_app_kamera->_look;  break;
		case Direction::RIGHT: dv = &pi_app_kamera->_right; break;
		case Direction::UP:    dv = &pi_app_kamera->_up;    break;
		default: return VS_ERROR;
	}

	VsVector3 *v = &pi_app_kamera->_v;
	FLOAT s = pi_app_kamera->_scalar_incr * pi_app_kamera->_position_incr * delta;
	v->x = v->x + dv->x * s;
	v->y = v->y + dv->y * s;
	v->z = v->z + dv->z * s;

	pi_app_kamera->has_been_updated = TRUE;

	return VS_OK;
}

INT AppKamera::IncPosOrd(Ordinate o, FLOAT delta)
{

	FLOAT *value = NULL;
	switch ( o )
	{
		case Ordinate::X: value = &pi_app_kamera->_v.x; break;
		case Ordinate::Y: value = &pi_app_kamera->_v.y; break;
		case Ordinate::Z: value = &pi_app_kamera->_v.z; break;
		default: return VS_ERROR;
	}

	*value = *value + pi_app_kamera->_position_incr * pi_app_kamera->_scalar_incr * delta;

	pi_app_kamera->has_been_updated = TRUE;

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---- STATE ---- 

/*

	Note: often use in conjunction with Defaults.

	For example:

		kamera->SaveDefaults();
		kamera->SaveState();

*/

void AppKamera::SaveState()
{
	VsVector3 v[6] =
	{
		pi_app_kamera->_v,
		pi_app_kamera->_look,
		pi_app_kamera->_right,
		pi_app_kamera->_up,
		pi_app_kamera->_world,
		pi_app_kamera->_rotate
	};
	for (UINT i = 0; i < 6; i++)
	{
		pi_app_kamera->_state_vector[i] = v[i];
	}

	FLOAT f[8] =
	{
		pi_app_kamera->_scalar_incr,
		pi_app_kamera->_pan_tilt_incr,
		pi_app_kamera->_position_incr,
		pi_app_kamera->_rotate_incr,
		pi_app_kamera->_translate_incr,
		pi_app_kamera->_zaam_incr,
		pi_app_kamera->_pan,
		pi_app_kamera->_tilt
	};
	for (UINT i = 0; i < 8; i++)
	{
		pi_app_kamera->_state_float[i] = f[i];
	}

}

void AppKamera::RestoreState()
{

	VsVector3 *v[6] =
	{
		&pi_app_kamera->_v,
		&pi_app_kamera->_look,
		&pi_app_kamera->_right,
		&pi_app_kamera->_up,
		&pi_app_kamera->_world,
		&pi_app_kamera->_rotate
	};
	for (UINT i = 0; i < 6; i++)
	{
		*v[i] = pi_app_kamera->_state_vector[i];
	}

	FLOAT *f[8] =
	{
		&pi_app_kamera->_scalar_incr,
		&pi_app_kamera->_pan_tilt_incr,
		&pi_app_kamera->_position_incr,
		&pi_app_kamera->_rotate_incr,
		&pi_app_kamera->_translate_incr,
		&pi_app_kamera->_zaam_incr,
		&pi_app_kamera->_pan,
		&pi_app_kamera->_tilt
	};
	for (UINT i = 0; i < 8; i++)
	{
		*f[i] = pi_app_kamera->_state_float[i];
	}
}


////////////////////////////////////////////////////////////////////////////////


// ---- USAGE for TRANSLATE and ROTATE ---- 
/*

	Note: the rotate & transform values are maintained by the AppKamera object,
	      but are used within the framework display or display->setup methods.

	For example (where kamera is a pointer to an AppKamera object):

	// ---- local
		INT result = 0;
		VsVector3 translate;
		FLOAT rx, ry, rz;

	// ---- setup matrix stack & transform values
		pi_model->recursive_depth_count = 0;
		pi_model->matrix_stack->Initialise();
		kamera->GetTranslate(&translate);
		kamera->GetRotate(&rx, &ry, &rz);

	// ---- push, transform, setup & pop
		pi_model->matrix_stack->Push();

		pi_model->matrix_stack->Rotate(rz, 'Z');
		pi_model->matrix_stack->Rotate(rx, 'X');
		pi_model->matrix_stack->Rotate(ry, 'Y');
		pi_model->matrix_stack->Translate(
						translate.x,
						translate.y,
						translate.z
					);

		pi_model->SetupStarSystemDisplay(pi_model->system_object);
		pi_model->matrix_stack->Pop();

*/


////////////////////////////////////////////////////////////////////////////////


// ---- ROTATE ---- 
/*

	note: see usage notes above 

*/

void AppKamera::GetRotate(FLOAT *x, FLOAT *y, FLOAT *z)
{
	*x = pi_app_kamera->_rotate.x;
	*y = pi_app_kamera->_rotate.y;
	*z = pi_app_kamera->_rotate.z;
}

void AppKamera::SetRotate(FLOAT x, FLOAT y, FLOAT z)
{
	// ---- update
		pi_app_kamera->_rotate.x = x;
		pi_app_kamera->_rotate.y = y;
		pi_app_kamera->_rotate.z = z;

	// ---- flag
		pi_app_kamera->has_been_updated = TRUE;
}

void AppKamera::SetDefaultRotate()
{

	// ---- update
		pi_app_kamera->_rotate = pi_app_kamera->_rotate_default;

	// ---- flag
		pi_app_kamera->has_been_updated = TRUE;

}

INT AppKamera::IncRotate(Ordinate o, FLOAT delta)
{

	FLOAT *value = NULL;
	switch (o)
	{
		case Ordinate::X: value = &pi_app_kamera->_rotate.x; break;
		case Ordinate::Y: value = &pi_app_kamera->_rotate.y; break;
		case Ordinate::Z: value = &pi_app_kamera->_rotate.z; break;
		default: return VS_ERROR;
	}

	//
	// 16/11/20 - change ensures that rotation may NOT be proportional to scalar_incr  
	//

	*value = *value + pi_app_kamera->_rotate_incr * delta * pi_app_kamera->_scalar_incr; // rotate scaler_incr

	// was					 ( pi_app_kamera->_scalar_rotation ? pi_app_kamera->_scalar_incr : 1 ) *

	pi_app_kamera->has_been_updated = TRUE;

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---- TRANSLATE ---- 
/*

	note: see usage notes above

*/

void AppKamera::GetTranslate(VsVector3 *v)
{
	*v = pi_app_kamera->_world;
}

void AppKamera::SetTranslate(VsVector3 *v)
{

	// ---- update
	pi_app_kamera->_world = *v;

	// ---- flag
	pi_app_kamera->has_been_updated = TRUE;

}

void AppKamera::SetTranslate(FLOAT x, FLOAT y, FLOAT z)
{
	VsVector3 v = { x, y, z };
	SetTranslate(&v);
}

void AppKamera::SetDefaultTranslate()
{

	// ---- update
		pi_app_kamera->_world = pi_app_kamera->_world_default;

	// ---- flag
		pi_app_kamera->has_been_updated = TRUE;

}

INT AppKamera::IncTranslate(Ordinate o, FLOAT delta)
{

	FLOAT *value = NULL;
	switch (o)
	{
		case Ordinate::X: value = &pi_app_kamera->_world.x; break;
		case Ordinate::Y: value = &pi_app_kamera->_world.y; break;
		case Ordinate::Z: value = &pi_app_kamera->_world.z; break;
		default: return VS_ERROR;
	}

	*value = *value + pi_app_kamera->_translate_incr * pi_app_kamera->_scalar_incr * delta;

	pi_app_kamera->has_been_updated = TRUE;

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---- UTILTY ---- 

BOOL AppKamera::HasBeenUpdated()
{
	return pi_app_kamera->has_been_updated;
}

void AppKamera::SetHasBeenUpdated(BOOL has_been_updated)
{
	pi_app_kamera->has_been_updated = has_been_updated;
}


////////////////////////////////////////////////////////////////////////////////