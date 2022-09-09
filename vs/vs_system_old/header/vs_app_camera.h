////////////////////////////////////////////////////////////////////////////////

// ---------- vs_app_camera.h ----------
/*!
\file vs_app_camera.h
\brief Interface for the App(lication)Camera class
\author Gareth Edwards
*/

#pragma once

#include <d3dx9.h>

namespace vs_system
{

	class AppCamera
	{

	public:

		// ---- type
			enum AppCameraType {
				LANDOBJECT = 0,
				AIRCRAFT = 1,
				LOOKAT = 2
			};


		// ---- cdtor
			AppCamera();
			AppCamera(AppCameraType);
			~AppCamera();


		// ---- type
			void GetCameraType(AppCameraType *cameraType);
			void SetCameraType(AppCameraType cameraType);


		// ---- scalar
			void SetAngleScalar(FLOAT angle_scalar);
			void SetUnitsScalar(FLOAT units_scalar);


		// ---- units
			void Strafe(FLOAT units); // left/right
			void Fly(FLOAT units);    // up/down
			void Walk(FLOAT units);   // forward/backward


		// ---- angle
			void Pitch(FLOAT angle); // rotate on right vector
			void Yaw(FLOAT angle);   // rotate on up vector
			void Roll(FLOAT angle);  // rotate on look vector


		// ---- D3DXMATRIX
			void GetViewMatrix(D3DXMATRIX *V);


		// ---- D3DXVECTOR3
			void SetLook(D3DXVECTOR3 *look, D3DXVECTOR3 *at);
			void SetLook(D3DXVECTOR3 *look);
			void SetPosition(D3DXVECTOR3 *pos);
			void SetRight(D3DXVECTOR3 *right);
			void SetUp(D3DXVECTOR3 *up);

			void GetLook(D3DXVECTOR3 *look);
			void GetPosition(D3DXVECTOR3 *pos);
			void GetRight(D3DXVECTOR3 *right);
			void GetUp(D3DXVECTOR3 *up);


	public:

		void SetPT(FLOAT, FLOAT);
		void GetPT(FLOAT *, FLOAT*);
		void GetPTLook(D3DXVECTOR3 *pt);


	private:

		AppCameraType _cameraType = LANDOBJECT;

		FLOAT _angle_scaler = 1.0f;
		FLOAT _units_scaler = 1.0f;

		D3DXVECTOR3 _right;
		D3DXVECTOR3 _up;
		D3DXVECTOR3 _look;
		D3DXVECTOR3 _pos;

		FLOAT _pan  = 0;
		FLOAT _tilt = 0;
	};

}

