////////////////////////////////////////////////////////////////////////////////

// ---------- vs_cam_graphs.cpp ----------
/*!
\file vs_cam_graphs.cpp
\brief Implementation of the CamGraphs class.
\author Gareth Edwards 
\note 2017
*/

#include "../header/vs_cam_graphs.h"


// ---- namespace ----
using namespace vs_system;


// ---------- Private Implementation of PI_CamGraphs class ----------

class CamGraphs::PI_CamGraphs
{

	// ---- structs ----

	struct Focus
	{
		UINT spokes     = 1;
		UINT samples    = 4;
		UINT columns    = 256;   // NUMBER OF BARS!!!
		FloatBuffer _buffer;
	};

public:


	// ---- constructor ----
	PI_CamGraphs()
	{
		_inj_app_dependency = NULL;
		INT result = Initialise();
	}

	// ---- destructor ----
	~PI_CamGraphs()
	{
		;
	}


	// ---- methods ----
	INT Initialise();


	// VS object pointers
	vs_system::AppDependency *_inj_app_dependency;


	// ---- properties ----
	Focus _focus;


};


// ---------- Initialise ----------
/*!
\brief Initialise
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT CamGraphs::PI_CamGraphs::Initialise()
{
	_focus._buffer.Initialise(_focus.columns, _focus.spokes, 1);
	_focus._buffer.CreateBuffer();
	return VS_OK;
}


// ---------- Implementation of CamGraphs class ----------


// ---------- Constructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
CamGraphs::CamGraphs()
{
	_pi_cam_graphs = new PI_CamGraphs();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
CamGraphs::~CamGraphs()
{
	if ( _pi_cam_graphs != NULL ) { delete _pi_cam_graphs; _pi_cam_graphs = NULL; }
}


// ---------- Setup ----------
/*!
\brief Setup
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT CamGraphs::Setup(vs_system::AppDependency *app_dependency)
{
	// ---- local ----
	INT result = VS_OK;

	// ---- store ----
	_pi_cam_graphs->_inj_app_dependency = app_dependency;

	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Setup
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT CamGraphs::Cleanup()
{
	return VS_OK;
}


// ---------- GetFloatBuffer ----------
/*!
\brief Get float buffer
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT CamGraphs::GetFloatBuffer(
		UINT group,
		FloatBuffer **buffer
	)
{

	switch (group)
	{
		case FOCUS:
			*buffer = &_pi_cam_graphs->_focus._buffer;
			return VS_OK;
			break;
		default:
			break;
	}
	return VS_ERROR;
}


// ---------- GetProperty ----------
/*!
\brief Get property
\author Gareth Edwards
\param
\return INT - ERROR <=0 < VS_OK
*/
INT CamGraphs::GetProperty(
		UINT   group,
		UINT   id,
		FLOAT *value
	)
{

	switch (group)
	{
		case FOCUS:
			{
				switch (id)
				{
					case SPOKES  : *value = (FLOAT)_pi_cam_graphs->_focus.spokes;  return VS_OK;
					case SAMPLES : *value = (FLOAT)_pi_cam_graphs->_focus.samples; return VS_OK;
					case COLUMNS : *value = (FLOAT)_pi_cam_graphs->_focus.columns; return VS_OK;
					default:
						;
				}
			}
			break;
		default:
			break;
	}
	return VS_ERROR;
}

