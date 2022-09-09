////////////////////////////////////////////////////////////////////////////////

// ---------- c1_model.h ----------
/*!
\file c1_model.h
\brief Interface for the Model class.
\author Gareth Edwards
*/

#pragma once

#include "c1_include.h"


namespace cube_01
{


// ---------- Model class ----------
class Model
{

public:
	

	// ---- update
		enum CameraUpdate
		{
			XYZ    = 0,
			VECTOR = 1,
			PTZ    = 2,
			TARGET = 3
		};


	// ---- target
		enum CameraTarget
		{
			NONE = 999
		};


	// ---- cdtor
		Model();
		virtual ~Model();


	// ---- framework methods
		INT Setup(vs_system::AppDependency*);
		INT SetupGraphics();
		INT Display(vs_system::PanelContext *, CHAR *);
		INT CleanupGraphics();
		INT Cleanup();


	// ---- sub-framework
		INT SetupFrameset(
				vs_system::AppDependency *,
				vs_system::AppView *
			);
		INT SetupDisplay();


	// ---- get
		INT GetPanelDisplayListHandle(vs_system::PanelContext *);


	// ---- camera ( c1_model_camera.hpp )
		INT SetMyKamera(INT);
		INT SetMsgProc(INT, FLOAT);
		INT SetTime(INT, FLOAT);
		INT SetSystem(INT, FLOAT);
		INT SetPersistantCameraControl();
		INT SetCameraControl(INT, FLOAT, BOOL);


	// ---- scene
		INT SetupCamera();
		INT SetupScene();


	// ---- cube
		INT SetupCubeDisplay();
		INT SetupCubeObjects();


	// ---- mesh
		INT SetupMeshYArray();
		INT SetupMeshDisplay();
		INT SetupMeshObjects();


	// ---- surface
		INT SetupSurfKArray();
		INT SetupSurfKData(UINT);
		INT SetupSurfDisplay();
		INT SetupSurfObjects();

private:

	// ---- private implementation
		class  PI_Model; PI_Model *pi_model;


};


}