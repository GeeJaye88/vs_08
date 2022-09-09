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

	class Camera
	{

	public:


		enum CameraType {
			LANDOBJECT = 0,
			AIRCRAFT = 1
		};


		// ---- cdtor
			Camera();
			Camera(CameraType);
			~Camera();


		// ---- type
			void GetCameraType(CameraType *cameraType);
			void SetCameraType(CameraType cameraType);


		// ---- units
			void Strafe(float units); // left/right
			void Fly(float units);    // up/down
			void Walk(float units);   // forward/backward


		// ---- angle
			void Pitch(float angle); // rotate on right vector
			void Yaw(float angle);   // rotate on up vector
			void Roll(float angle);  // rotate on look vector


		// ---- D3DXMATRIX
			void GetViewMatrix(D3DXMATRIX *V);


		// ---- D3DXVECTOR3
			void GetPosition(D3DXVECTOR3 *pos);
			void SetPosition(D3DXVECTOR3 *pos);
			void GetRight(D3DXVECTOR3 *right);
			void GetUp(D3DXVECTOR3 *up);


	private:

		CameraType  _cameraType;

		D3DXVECTOR3 _right;
		D3DXVECTOR3 _up;
		D3DXVECTOR3 _look;
		D3DXVECTOR3 _pos;
	};

}

