////////////////////////////////////////////////////////////////////////////////

// ---------- e1_project_project.h ----------
/*!
\file e1_project_project.h
\brief Interface for the sub-application class.
\author Gareth Edwards
*/

#pragma once

#include "../header/e1_include.h"

namespace examples_01
{


// ---------- Project class ----------
class Project
{

public:

	// ---- cdtor
		Project();
		virtual ~Project();


	// ---- framework
		INT Setup(vs_system::AppDependency*);
		INT Display();
		INT Cleanup();


private:

	// ---- private implementation
		class  PI_Project;
		PI_Project *pi_project;

};


}