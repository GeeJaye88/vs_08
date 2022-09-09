////////////////////////////////////////////////////////////////////////////////

// ---------- vs_app_camera.cpp ----------
/*!
\file vs_app_camera.cpp
\brief Implementation of the App(lication)Camera class
\author Gareth Edwards
\note based on Frank Luna framework
*/

// ---- include ----
#include "../header/vs_app_camera.h"

using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- type ----------


AppCamera::AppCamera()
{
	_cameraType = AIRCRAFT;

	_pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

AppCamera::AppCamera(AppCameraType ct)
{
	_cameraType = ct;

	_pos   = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up    = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look  = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

AppCamera::~AppCamera()
{

}


// ---------- type ----------

void AppCamera::GetCameraType(AppCameraType *cameraType)
{
	*cameraType = _cameraType;
}

void AppCamera::SetCameraType(AppCameraType cameraType)
{
	_cameraType = cameraType;
}


// ---------- scalar ----------

void AppCamera::SetAngleScalar(FLOAT angle_scalar)
{
	_angle_scaler = angle_scalar;
}

void AppCamera::SetUnitsScalar(FLOAT units_scalar)
{
	_units_scaler = units_scalar;
}


// ---------- angle ----------

void AppCamera::Pitch(FLOAT angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_right, D3DXToRadian(angle));

	// rotate _up and _look around _right vector
	D3DXVec3TransformCoord(&_up, &_up, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);
}


void AppCamera::Yaw(FLOAT angle)
{
	D3DXMATRIX T;

	// rotate around world y (0, 1, 0) always for land object
	if (_cameraType == LANDOBJECT)
		D3DXMatrixRotationY(&T, D3DXToRadian(angle));

	// rotate around own up vector for aircraft
	if (_cameraType == AIRCRAFT)
		D3DXMatrixRotationAxis(&T, &_up, D3DXToRadian(angle));

	// rotate _right and _look around _up or y-axis
	D3DXVec3TransformCoord(&_right, &_right, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);
}


void AppCamera::Roll(FLOAT angle)
{

	// ---- only roll for aircraft type
		if (_cameraType == AIRCRAFT)
		{
			D3DXMATRIX T;
			D3DXMatrixRotationAxis(&T, &_look, D3DXToRadian(angle));

			// rotate _up and _right around _look vector
			D3DXVec3TransformCoord(&_right, &_right, &T);
			D3DXVec3TransformCoord(&_up, &_up, &T);
		}
}


// ---------- units ----------

void AppCamera::Walk(FLOAT units)
{
	// move only on xz plane for land object
	if (_cameraType == LANDOBJECT)
		_pos += D3DXVECTOR3(_look.x, 0.0f, _look.z) * units;

	if (_cameraType == AIRCRAFT)
		_pos += _look * units;
}


void AppCamera::Strafe(FLOAT units)
{
	// move only on xz plane for land object
	if (_cameraType == LANDOBJECT)
		_pos += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;

	if (_cameraType == AIRCRAFT)
		_pos += _right * units;
}


void AppCamera::Fly(FLOAT units)
{

	// move only on y-axis for land object
	if (_cameraType == LANDOBJECT)
		_pos.y += units;

	if (_cameraType == AIRCRAFT)
		_pos += _up * units;

}


// ---------- D3DXMATRIX ----------

void AppCamera::GetViewMatrix(D3DXMATRIX* V)
{

	// ---- keep camera's axes orthogonal to eachother
		D3DXVec3Normalize(&_look, &_look);

		D3DXVec3Cross(&_up, &_look, &_right);
		D3DXVec3Normalize(&_up, &_up);

		D3DXVec3Cross(&_right, &_up, &_look);
		D3DXVec3Normalize(&_right, &_right);


	// ---- build the view matrix:
		float x = -D3DXVec3Dot(&_right, &_pos);
		float y = -D3DXVec3Dot(&_up, &_pos);
		float z = -D3DXVec3Dot(&_look, &_pos);

		(*V)(0, 0) = _right.x; (*V)(0, 1) = _up.x; (*V)(0, 2) = _look.x; (*V)(0, 3) = 0.0f;
		(*V)(1, 0) = _right.y; (*V)(1, 1) = _up.y; (*V)(1, 2) = _look.y; (*V)(1, 3) = 0.0f;
		(*V)(2, 0) = _right.z; (*V)(2, 1) = _up.z; (*V)(2, 2) = _look.z; (*V)(2, 3) = 0.0f;
		(*V)(3, 0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;

}


// ---------- D3DXVECTOR3 ----------

void AppCamera::SetLook(D3DXVECTOR3* look, D3DXVECTOR3* at)
{
	_pos  = *look;
	_look = *at;

	D3DXVECTOR3 right = { 1, 0, 0 };
	_right = right;

	D3DXVECTOR3 up = { 0, 1, 0 };
	_up = up;
}

void AppCamera::SetLook(D3DXVECTOR3    *look) { _look  = *look;  }
void AppCamera::SetPosition(D3DXVECTOR3 *pos) { _pos   = *pos;   }
void AppCamera::SetRight(D3DXVECTOR3  *right) { _right = *right; }
void AppCamera::SetUp(D3DXVECTOR3        *up) { _up    = *up;    }

void AppCamera::GetLook(D3DXVECTOR3    *look) { *look  = _look;  }
void AppCamera::GetPosition(D3DXVECTOR3 *pos) { *pos   = _pos;   }
void AppCamera::GetRight(D3DXVECTOR3  *right) { *right = _right; }
void AppCamera::GetUp(D3DXVECTOR3        *up) { *up    = _up;    }


////////////////////////////////////////////////////////////////////////////////


// ---------- Pan & Tilt ----------

void AppCamera::SetPT(FLOAT p, FLOAT t)
{
	_pan  = p;
	_tilt = t;
}

void AppCamera::GetPT(FLOAT *p, FLOAT *t)
{
	*p = _pan;
	*t = _tilt;
}

void AppCamera::GetPTLook(D3DXVECTOR3 *cd)
{

	D3DXVECTOR3 dcd = { 0, 0, 1 };
	FLOAT rad = (FLOAT)0.01745329252;


	// ---- tilt around x axis
		// x' = 0
		// y' = y*cos q - z*sin q
		// z' = y*sin q + z*cos q
		FLOAT tilt_2_rad = _tilt * rad;
		FLOAT y = -sinf(tilt_2_rad);
		FLOAT z =  cosf(tilt_2_rad);


	// ---- pan around y axis
		// x' = z*sin q + x*cos q
		// y' = y
		// z' = z*cos q - x*sin q
		FLOAT pan_2_rad = _pan * rad;
		FLOAT x = z * sinf(pan_2_rad);
		      z = z * cosf(pan_2_rad);

	*cd = D3DXVECTOR3(x, y, z);

}


////////////////////////////////////////////////////////////////////////////////