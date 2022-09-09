////////////////////////////////////////////////////////////////////////////////

// ---------- vs_lidar_ouster.cpp ----------
/*!
\file vs_lidar_ouster.cpp
\brief Implementation of the LidarOuster class.
\author Gareth Edwards
*/

// ---- Use preprocessor directive to turn OFF MS warnings ----
#define _CRT_SECURE_NO_WARNINGS


// ---- library include ----
#include "../header/vs_lidar_ouster.h"


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


//---------- private implmentation ----------
class LidarOuster::PI_LidarOuster
{

public:

	// ---- cdtor
		PI_LidarOuster();
		virtual ~PI_LidarOuster();

	// ---- properties
		UINT beams = 0;
		UINT columns = 0;

};


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
LidarOuster::PI_LidarOuster::PI_LidarOuster()
{
	;
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
LidarOuster::PI_LidarOuster::~PI_LidarOuster()
{
	;
}



////////////////////////////////////////////////////////////////////////////////



// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
LidarOuster::LidarOuster()
{
	pi_lidar_ouster = new PI_LidarOuster();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
LidarOuster::~LidarOuster()
{
	if ( pi_lidar_ouster != NULL ) { delete pi_lidar_ouster; pi_lidar_ouster = NULL; }
}


// ---- get
UINT LidarOuster::GetColumns()
{
	return pi_lidar_ouster->columns;
}

UINT LidarOuster::GetBeams()
{
	return pi_lidar_ouster->beams;
}


// ---- set
INT LidarOuster::SetColumns(UINT columns)
{
	pi_lidar_ouster->columns = columns;
	return VS_OK;
}

INT LidarOuster::SetBeams(UINT beams)
{
	pi_lidar_ouster->beams = beams;
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
