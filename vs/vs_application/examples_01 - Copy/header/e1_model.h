////////////////////////////////////////////////////////////////////////////////

// ---------- e1_model.h ----------
/*!
\file e1_model.h
\brief Interface for the Model class.
\author Gareth Edwards
*/

#pragma once

#include "../header/e1_include.h"

namespace examples_01
{


// ---------- Model class ----------
class Model
{

public:

	// ---- cdtor
		Model();
		virtual ~Model();


	// ---- framework methods
		INT Setup(vs_system::AppDependency*);
		INT SetupGraphics();
		INT Display();
		INT Display(vs_system::PanelContext *, CHAR *);
		INT CleanupGraphics();
		INT Cleanup();


	// ---- planets
		INT InitPlanetsParameters();
		INT InitPlanetsScene();
		INT InitSystemObjects();
		INT SetupSystemObjectsForDisplay();


	// ---- cube
		INT InitCubeObjects();
		INT InitCubeScene();
		INT SetupCubeDisplay();


	// ---- set
		INT SetCamera(INT action, FLOAT delta);


	// ---- more sub-framework
		INT GetPanelDisplayListHandle(vs_system::PanelContext *);
		INT SetupFrameset(
				vs_system::AppDependency *,
				vs_system::AppView *
			);

private:

	// ---- private implementation
		class  PI_Model; PI_Model *pi_model;


};


}