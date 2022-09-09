////////////////////////////////////////////////////////////////////////////////

// ---------- r2_ctr_pimp_cdtor.hpp ----------
/*!
\file r2_ctr_pimp_cdtor.hpp
\brief 
\author Gareth Edwards
*/


#include "../header/r2_model.h"

using namespace review_02;


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Construct - invoked by constructor
\author Gareth Edwards
*/
INT Control::Construct()
{
	pi_application = new PI_Application();
	pi_camera   = new PI_Camera();
	pi_context  = new PI_Context();
	pi_timeline = new PI_Timeline();
	return VS_OK;
}


// ---------- Destructor ----------
/*!
\brief Destruct - invoked by destructor
\author Gareth Edwards
*/
INT Control::Destruct()
{
	if ( pi_application != NULL ) { delete pi_application; pi_application = NULL; }
	if ( pi_camera      != NULL ) { delete pi_camera;      pi_camera = NULL; }
	if ( pi_context     != NULL ) { delete pi_context;     pi_context = NULL; }
	if ( pi_timeline    != NULL ) { delete pi_timeline;    pi_timeline = NULL; }
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


