////////////////////////////////////////////////////////////////////////////////

// ---------- vs_lidar_ouster.h ----------
/*!
\file vs_lidar_ouster.h
\brief Interface for the LidarOuster class.
\author Gareth Edwards
\note 2017
*/

#pragma once


// ---- vs system ----
#include "../../vs_system/header/vs_inc_defs.h"


// ---- namespace ----
namespace vs_system
{


// ---- LidarOuster class ----
class LidarOuster
{

public:
	 
	// ---- cdtor
		LidarOuster();
		virtual ~LidarOuster();


	// ---- get
		UINT GetColumns();
		UINT GetBeams();


	// ---- set
		INT SetColumns(UINT columns);
		INT SetBeams(UINT rows);

private:

	// ---- private implementation
		class  PI_LidarOuster;
		PI_LidarOuster *pi_lidar_ouster;


}; // class


} // namespace

