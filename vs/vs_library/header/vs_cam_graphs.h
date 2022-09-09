////////////////////////////////////////////////////////////////////////////////

// ---------- vs_cam_graphs.h ----------
/*!
\file vs_cam_graphs.h
\brief Interface and implementation for the CamGraphs class.
\author Gareth Edwards
\note 2017
*/

#pragma once

#include "../../vs_system/header/vs_inc_defs.h"
#include "../../vs_system/header/vs_app_dependency.h"

#include "vs_flt_buffer.h"


// ---------- namespace ----------
namespace vs_system
{


// ---------- CamGraphs class ----------
class CamGraphs
{

public:

	// ---- graphs ----
	enum
	{
		FOCUS = 1
	};


	// ---- param ----
	enum
	{
		SPOKES  = 1,
		SAMPLES = 2,
		COLUMNS = 3
	};


	// ---- c/dtor ----
	CamGraphs();
	virtual ~CamGraphs();


	// ---- framework ----
	INT Setup(vs_system::AppDependency *);
	INT Cleanup();

	INT GetFloatBuffer(UINT, FloatBuffer **);
	INT GetProperty(UINT, UINT, FLOAT *);

private:

	// private implementation
	class PI_CamGraphs; PI_CamGraphs *_pi_cam_graphs;


};


}
