////////////////////////////////////////////////////////////////////////////////

// ---------- e1_project.cpp ----------
/*!
\file e1_projects.cpp
\brief Implementation of a sub-application class
\author Gareth Edwards
\note Vanilla C++
*/

#include "../header/e1_project.h"

using namespace examples_01;


// ---------- private implementation of Portal class ----------
class Project::PI_Project
{

public:

	// ---- cdtor
		PI_Project();
		~PI_Project();


	// ---- dependencies managed by VS
		vs_system::AppDependency *app_dependency;
		vs_system::GfxInterface  *gfx;
		vs_system::SysContext    *sys_context;
		vs_system::SysOutput     *sys_output;


	// ---- dependencies managed by application
		examples_01::Model       *model;
		examples_01::Kontrol     *kontrol;
};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Project::PI_Project::PI_Project()
{

	// ----managed dependencies
		app_dependency = NULL;
		gfx            = NULL;
		sys_context    = NULL;
		sys_output     = NULL;


	// ---- dependencies managed by application
		model          = NULL;
		kontrol        = NULL;
}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Project::PI_Project::~PI_Project()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
Project::Project()
{
	pi_project = new PI_Project();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
Project::~Project()
{
	delete pi_project;
}


// ---------- framework ----------


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param AppDependency * - application dependency
\return INT - ERROR <=0 < VS_OK
*/
INT Project::Setup(vs_system::AppDependency *app_dep)
{

	// ---- local
		INT result = 0;


	// ---- store dependency pointers
		pi_project->app_dependency = app_dep;
		pi_project->gfx            = app_dep->GetGfxInterface();
		pi_project->sys_context    = app_dep->GetSysContext();
		pi_project->sys_output     = app_dep->GetSysOutput();


	return VS_OK;
}


// ---------- framework / Display ----------
/*!
\brief Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Project::Display()
{

	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Project::Cleanup()
{
	return VS_OK;
}

