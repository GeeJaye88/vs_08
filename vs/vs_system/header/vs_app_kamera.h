////////////////////////////////////////////////////////////////////////////////

// ---------- vs_app_camera.h ----------
/*!
\file vs_app_camera.h
\brief Interface for the App(lication)Camera class
\author Gareth Edwards
*/

#pragma once


#include "vs_inc_defs.h"
#include "vs_inc_dems.h"
#include "vs_inc_maths.h"


namespace vs_system
{

	class AppKamera
	{

	public:

		// ---- enumerated types
			enum Mode
			{
				XYZ       = 0,
				VECTOR    = 1,
				PTZ       = 2,
				TRANSLATE = 3,
				ROTATE    = 4,
				TARGET    = 5
			};

			enum Direction {
				LOOK  = 0,
				RIGHT = 1,
				UP    = 2
			};

			enum Ordinate {
				X = 0,
				Y = 1,
				Z = 2
			};

			enum Type {
				LAND   = 0,
				AIR    = 1,
				LOOKAT = 2
			};


		// ---- cdtor
			AppKamera();
			AppKamera(Type);
			~AppKamera();


		// ---- type
			Type GetKameraType();
			void SetKameraType(Type);


		// ---- target
			
			// ---- pan
				void  IncTargetPan(FLOAT);
				FLOAT GetTargetPan();
				void  SetTargetPan(FLOAT);

			// ---- tilt
				void  IncTargetTilt(FLOAT);
				FLOAT GetTargetTilt();
				void  SetTargetTilt(FLOAT);

			// ---- zoom
				void  IncTargetZoom(FLOAT);
				void  SetTargetZoomMinMax();
				FLOAT GetTargetZoom();
				void  SetTargetZoom(FLOAT);

			// --- zoom param
				void   SetTargetZoomDefault();
				FLOAT  GetTargetZoomMinimum();
				void   SetTargetZoomMinimum(FLOAT);
				FLOAT  GetTargetZoomMaximum();
				void   SetTargetZoomMaximum(FLOAT);

			// ---- target change
				void   SetTargetChangeZoomPower(FLOAT);
				void   SetTargetChangeZoomDenom(FLOAT);
				void   SetTargetChangeSpeed(FLOAT);

			// ---- target increment
				FLOAT GetTargetIncr();
				void  SetTargetIncr(FLOAT);

			// ---- target position
				void  GetTargetPosition(VsVector3 *); 
				void  SetTargetPosition(VsVector3 *, BOOL);

			// ---- target state
				BOOL  GetTargetStatus();
				void  SetTargetStatus(BOOL);


		// ---- axis
			void GetLook(VsVector3 *);
			void GetRight(VsVector3 *);
			void GetUp(VsVector3 *);

			void SetLook(VsVector3 *);
			void SetRight(VsVector3 *);
			void SetUp(VsVector3 *);


		// ---- default
			void SaveDefaults();


		// ---- increment
			FLOAT GetScalarIncr();
			FLOAT GetPositionIncr();
			FLOAT GetPanTiltIncr();
			FLOAT GetRotateIncr();
			FLOAT GetTranslateIncr();
			FLOAT GetZoomIncr();

			void  SetScalarIncr(FLOAT);
			void  SetPositionIncr(FLOAT);
			void  SetPanTiltIncr(FLOAT);
			void  SetRotateIncr(FLOAT);
			void  SetTranslateIncr(FLOAT);
			void  SetZoomIncr(FLOAT);

		// ---- flags
			BOOL  GetScalarMode();
			void  SetScalarMode(BOOL);
			BOOL  GetScalarRotation();
			void  SetScalarRotation(BOOL);


		// --- matrix
			void GetViewMatrix(VsMatrix *);


		// ---- pan & tilt
			void GetPanTilt(FLOAT *, FLOAT*);
			void SetPanTiltDefault();
			void SetPanTilt(FLOAT, FLOAT);
			void IncPan(FLOAT);
			void IncTilt(FLOAT);
			void SetPanTiltAxis();


		// ---- position
			void GetPosition(VsVector3 *);
			void SetPositionDefault();
			void SetPosition(FLOAT, FLOAT, FLOAT);
			void SetPosition(VsVector3 *);
			INT  IncPosDir(Direction, FLOAT);
			INT  IncPosOrd(Ordinate, FLOAT);

		// ---- rotate
			void GetRotate(FLOAT *, FLOAT *, FLOAT *);
			void SetRotate(FLOAT, FLOAT, FLOAT);
			void SetDefaultRotate();
			INT  IncRotate(Ordinate, FLOAT);


		// ---- state
			void SaveState();
			void RestoreState();


		// ---- translate
			void GetTranslate(VsVector3 *);
			void SetTranslate(VsVector3 *);
			void SetTranslate(FLOAT, FLOAT, FLOAT);
			void SetDefaultTranslate();
			INT  IncTranslate(Ordinate, FLOAT);


		// ---- utility
			BOOL HasBeenUpdated();
			void SetHasBeenUpdated(BOOL);


	private:


		// ---- private implementation
			class  PI_AppKamera; PI_AppKamera *pi_app_kamera;

	};

}

