////////////////////////////////////////////////////////////////////////////////

// ---------- r2_dl_gps_dashboard.hpp ----------
/*!
\file r2_dl_gps_dashboard.hpp
\brief application include files.
\author Gareth Edwards
*/


#include "../header/r2_model.h"
using namespace review_02;


// ---------- UpdateGpsDataStr ----------
/*!
\brief Setup gps display list
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Model::UpdateGpsDataStr(CHAR *d[])
{

	// ---- store ----
	for (UINT i = 0; i < 4; i++)
	{
		if (strcmp(d[i], "") != 0)
		{
			gps_data_str_text[i].assign(d[i]);
		}
	}

	cam_dbz_param[Dbz_Cam_Index::GPS_FLAG] = TRUE;

	return VS_OK;
}

