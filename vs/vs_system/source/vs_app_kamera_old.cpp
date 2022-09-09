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


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_AppKamera class ----------
class AppKamera::PI_AppKamera
{

public:

	// ---- cdtor
		PI_AppKamera() { ; };
		PI_AppKamera(Type kamera_type) { _kamera_type = kamera_type; };
		~PI_AppKamera() { ; };


	// ---- properties
		Type _kamera_type = AppKamera::AIR;


	// --- target
		BOOL  _target_on           = FALSE;
		FLOAT _target_incr         = 1.0f;
		FLOAT _target_zoom         = 200.0;
		FLOAT _target_zoom_default = _target_zoom;
		VsVector3 _t = VsVector3(0, 0, 0);


	// --- pre target
		FLOAT _pre_target_on_pan   = 0;
		FLOAT _pre_target_on_tilt  = 0;
		VsVector3 _pre_target_on_v = VsVector3(0, 0, 0);


	// ---- increment
		FLOAT _scalar_incr   = 1.0f;
		FLOAT _pan_tilt_incr = 1.0f;
		FLOAT _position_incr = 1.0f;
		FLOAT _world_incr    = 1.0f;
		FLOAT _zoom_incr     = 1.0f;


	// ---- pan & tilt
		FLOAT _pan           = 0.0f;
		FLOAT _tilt          = 0.0f;
		FLOAT _pan_default   = _pan;
		FLOAT _tilt_default  = _tilt_default;


	// ---- vectors
		VsVector3 _v             = VsVector3(0, 0, 0);
		VsVector3 _v_default     = VsVector3(0, 0, 0);
		VsVector3 _look          = VsVector3(0, 0, 1);
		VsVector3 _right         = VsVector3(1, 0, 0);
		VsVector3 _up            = VsVector3(0, 1, 0);
		VsVector3 _world         = VsVector3(0, 0, 0);
		VsVector3 _world_default = VsVector3(0, 0, 0);


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


//------------------------------------------------------------------------------


// ---- TYPE ---- 

AppKamera::Type AppKamera::GetKameraType()
{
	return pi_app_kamera->_kamera_type;
}
void AppKamera::SetKameraType(Type kamera_type)
{
	pi_app_kamera->_kamera_type = kamera_type;
}


//------------------------------------------------------------------------------


// ---- TARGET ---- 

void  AppKamera::GetTargetPosition(VsVector3 *t)
{
	t->x = pi_app_kamera->_t.x;
	t->y = pi_app_kamera->_t.y;
	t->z = pi_app_kamera->_t.z;
};


void AppKamera::IncTargetPan(FLOAT v)
{

	pi_app_kamera->_target_on = FALSE;

	pi_app_kamera->_pan += pi_app_kamera->_pan_tilt_incr * pi_app_kamera->_scalar_incr * v;
	SetPanTiltAxis();

	pi_app_kamera->_target_on = TRUE;

}

void AppKamera::IncTargetTilt(FLOAT v)
{

	pi_app_kamera->_target_on = FALSE;

	pi_app_kamera->_tilt += pi_app_kamera->_pan_tilt_incr * pi_app_kamera->_scalar_incr * v;
	SetPanTiltAxis();

	pi_app_kamera->_target_on = TRUE;

}

void AppKamera::IncTargetZoom(FLOAT delta)
{
	pi_app_kamera->_target_zoom +=
		pi_app_kamera->_zoom_incr *
			( pi_app_kamera->_target_incr * delta );
};

FLOAT AppKamera::GetTargetZoom()
{
	return pi_app_kamera->_target_zoom;
};

void AppKamera::SetTargetZoom(FLOAT zoom)
{
	pi_app_kamera->_target_zoom = zoom;
};

void AppKamera::SetTargetZoomDefault(FLOAT zoom)
{
	pi_app_kamera->_target_zoom_default = zoom;
};

void AppKamera::SetTargetZoomDefault()
{
	pi_app_kamera->_target_zoom = pi_app_kamera->_target_zoom_default;
};


FLOAT AppKamera::GetTargetIncr()             { return pi_app_kamera->_target_incr;   };
void AppKamera::SetTargetIncr(FLOAT scalar)  { pi_app_kamera->_target_incr = scalar; };


void AppKamera::SetTargetPosition(VsVector3 *tp)
{

	// ---- no target ? -- store pre target pan, tilt and position
		if ( !pi_app_kamera->_target_on )
		{
			pi_app_kamera->_pre_target_on_pan  = pi_app_kamera->_pan;
			pi_app_kamera->_pre_target_on_tilt = pi_app_kamera->_tilt;
			pi_app_kamera->_pre_target_on_v    = pi_app_kamera->_v;
		}


	// ---- target ON
		pi_app_kamera->_target_on = TRUE;


	// ---- store
		pi_app_kamera->_t.x = tp->x;
		pi_app_kamera->_t.y = tp->y;
		pi_app_kamera->_t.z = tp->z;


		FLOAT scalar = pi_app_kamera->_target_zoom;
		VsVector3 *v = &pi_app_kamera->_v;
		VsVector3 *t = &pi_app_kamera->_t;
		VsVector3 *l = &pi_app_kamera->_look;

		v->x = t->x - l->x * scalar;
		v->y = t->y - l->y * scalar;
		v->z = t->z - l->z * scalar;

	// ---- flag
		pi_app_kamera->has_been_updated = TRUE;

};


void AppKamera::SetTargetStatus(BOOL s)
{

	// ---- false ? -- restore pre-target param
		if ( !s )
		{
			if ( pi_app_kamera->_target_on )
				SetPanTiltAxis();
		}
		else
			pi_app_kamera->_target_on = TRUE;
}

//------------------------------------------------------------------------------


// ---- INCREMENT ----

FLOAT AppKamera::GetScalarIncr()             { return pi_app_kamera->_scalar_incr;   };
FLOAT AppKamera::GetPositionIncr()           { return pi_app_kamera->_position_incr; };
FLOAT AppKamera::GetPanTiltIncr()            { return pi_app_kamera->_pan_tilt_incr; };
FLOAT AppKamera::GetWorldIncr()              { return pi_app_kamera->_world_incr;    };
FLOAT AppKamera::GetZoomIncr()               { return pi_app_kamera->_zoom_incr;     };

void  AppKamera::SetScalarIncr(FLOAT incr)   { pi_app_kamera->_scalar_incr   = incr; };
void  AppKamera::SetPositionIncr(FLOAT incr) { pi_app_kamera->_position_incr = incr; };
void  AppKamera::SetPanTiltIncr(FLOAT incr)  { pi_app_kamera->_pan_tilt_incr = incr; };
void  AppKamera::SetWorldIncr(FLOAT incr)    { pi_app_kamera->_world_incr    = incr; };
void  AppKamera::SetZoomIncr(FLOAT incr)     { pi_app_kamera->_zoom_incr     = incr; };


//------------------------------------------------------------------------------


// ---- PAN & TILT ---- 

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

void AppKamera::SetPanTiltDefault(FLOAT pan, FLOAT tilt)
{
	pi_app_kamera->_pan_default  = pan;
	pi_app_kamera->_tilt_default = tilt;
}

void AppKamera::SetPanTilt(FLOAT pan, FLOAT tilt)
{
	pi_app_kamera->_pan  = pan;
	pi_app_kamera->_tilt = tilt;
	SetPanTiltAxis();
}

void AppKamera::IncPan(FLOAT delta)
{
	pi_app_kamera->_pan += pi_app_kamera->_pan_tilt_incr * pi_app_kamera->_scalar_incr * delta;
	SetPanTiltAxis();
}

void AppKamera::IncTilt(FLOAT delta)
{
	pi_app_kamera->_tilt += pi_app_kamera->_pan_tilt_incr * pi_app_kamera->_scalar_incr * delta;
	SetPanTiltAxis();
}

void AppKamera::SetPanTiltAxis()
{

	// ---- restore pre target on pan, tilt and position ?
		if ( pi_app_kamera->_target_on )
		{
			pi_app_kamera->_target_on = FALSE;
			pi_app_kamera->_pan  = pi_app_kamera->_pre_target_on_pan;
			pi_app_kamera->_tilt = pi_app_kamera->_pre_target_on_tilt;
			pi_app_kamera->_v    = pi_app_kamera->_pre_target_on_v;
		}


	// ---- int
		FLOAT rad = (FLOAT)0.01745329252;
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


	// ---- for each view vector
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


//------------------------------------------------------------------------------


// ---- AXIS ---- 

void AppKamera::GetLook(VsVector3 *v)
{
	v->x = pi_app_kamera->_look.x;
	v->y = pi_app_kamera->_look.y;
	v->z = pi_app_kamera->_look.z;
}

void AppKamera::GetRight(VsVector3 *v)
{
	v->x = pi_app_kamera->_right.x;
	v->y = pi_app_kamera->_right.y;
	v->z = pi_app_kamera->_right.z;
}

void AppKamera::GetUp(VsVector3 *v)
{
	v->x = pi_app_kamera->_up.x;
	v->y = pi_app_kamera->_up.y;
	v->z = pi_app_kamera->_up.z;
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


//------------------------------------------------------------------------------


// ---- POSITION ---- 

void AppKamera::GetPosition(VsVector3 *v)
{
	v->x = pi_app_kamera->_v.x;
	v->y = pi_app_kamera->_v.y;
	v->z = pi_app_kamera->_v.z;
}

void AppKamera::SetPositionDefault()
{

	// ---- update
		pi_app_kamera->_v.x = pi_app_kamera->_v_default.x;
		pi_app_kamera->_v.y = pi_app_kamera->_v_default.y;
		pi_app_kamera->_v.z = pi_app_kamera->_v_default.z;

	// ---- flag
		pi_app_kamera->has_been_updated = TRUE;

}

void AppKamera::SetPositionDefault(FLOAT x, FLOAT y, FLOAT z)
{
	VsVector3 v = { x, y, z };
	SetPositionDefault(&v);
}

void AppKamera::SetPositionDefault(VsVector3 *v)
{
	pi_app_kamera->_v_default.x = v->x;
	pi_app_kamera->_v_default.y = v->y;
	pi_app_kamera->_v_default.z = v->z;
}

void AppKamera::SetPosition(FLOAT x, FLOAT y, FLOAT z)
{
	VsVector3 v = { x, y, z };
	SetPosition(&v);
}

void AppKamera::SetPosition(VsVector3 *v)
{

	// ---- update
		pi_app_kamera->_v.x = v->x;
		pi_app_kamera->_v.y = v->y;
		pi_app_kamera->_v.z = v->z;

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
	FLOAT s = pi_app_kamera->_scalar_incr * delta;
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


//------------------------------------------------------------------------------


// ---- WORLD ---- 

void AppKamera::GetWorld(VsVector3 *v)
{
	v->x = pi_app_kamera->_world.x;
	v->y = pi_app_kamera->_world.y;
	v->z = pi_app_kamera->_world.z;
}

void AppKamera::SetWorld(VsVector3 *v)
{

	// ---- update
	pi_app_kamera->_world.x = v->x;
	pi_app_kamera->_world.y = v->y;
	pi_app_kamera->_world.z = v->z;

	// ---- flag
	pi_app_kamera->has_been_updated = TRUE;

}

void AppKamera::SetWorld(FLOAT x, FLOAT y, FLOAT z)
{
	VsVector3 v = { x, y, z };
	SetWorld(&v);
}

void AppKamera::SetWorldDefault(VsVector3 *v)
{
	pi_app_kamera->_world_default.x = v->x;
	pi_app_kamera->_world_default.y = v->y;
	pi_app_kamera->_world_default.z = v->z;
}

void AppKamera::SetWorldDefault(FLOAT x, FLOAT y, FLOAT z)
{
	VsVector3 v = { x, y, z };
	SetWorldDefault(&v);
}

void AppKamera::SetWorldDefault()
{

	// ---- update
		pi_app_kamera->_world.x = pi_app_kamera->_world_default.x;
		pi_app_kamera->_world.y = pi_app_kamera->_world_default.y;
		pi_app_kamera->_world.z = pi_app_kamera->_world_default.z;

	// ---- flag
		pi_app_kamera->has_been_updated = TRUE;

}


INT AppKamera::IncWorld(Ordinate o, FLOAT delta)
{

	FLOAT *value = NULL;
	switch (o)
	{
		case Ordinate::X: value = &pi_app_kamera->_world.x; break;
		case Ordinate::Y: value = &pi_app_kamera->_world.y; break;
		case Ordinate::Z: value = &pi_app_kamera->_world.z; break;
		default: return VS_ERROR;
	}

	*value = *value + pi_app_kamera->_world_incr * pi_app_kamera->_scalar_incr * delta;

	pi_app_kamera->has_been_updated = TRUE;

	return VS_OK;
}


//------------------------------------------------------------------------------

// ---- MATRIX ---- 

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


	// ---- keep camera's axes orthogonal to eachother
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

}


//------------------------------------------------------------------------------

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