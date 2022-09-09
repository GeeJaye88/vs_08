////////////////////////////////////////////////////////////////////////////////

// ---------- vs_sys_license.cpp ----------
/*!
\file vs_sys_license.cpp
\brief Implementation of the Sys(tem)License class
\author Gareth Edwards 
*/

// ---- include ----
#include "../header/vs_sys_license.h"
#include "../header/vs_sys_licenseinfo.h"
#include <time.h>


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_License class ----------

class SysLicense::PI_SysLicense
{

public:

	// ---- constructor/destructor ----
	PI_SysLicense()
	{
		;
	};
	~PI_SysLicense(){ };

};


////////////////////////////////////////////////////////////////////////////////


// ========== SYSLICENSE ==========


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
SysLicense::SysLicense()
{
	pi_sys_license = new PI_SysLicense();
}


// ---------- Destructor ----------
/*!
\brief Destructor
\author Gareth Edwards
*/
SysLicense::~SysLicense()
{
	delete pi_sys_license;
	pi_sys_license = NULL;
}


// ---------- Setup ----------
/*!
\brief Setup controls
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK 
*/
INT SysLicense::Valid()
{

	// ---- local ----
		INT end_year  = VS_LICENSE_EXPIRE_YEAR;
		INT end_month = VS_LICENSE_EXPIRE_MONTH;
		INT end_day   = VS_LICENSE_EXPIRE_DAY;


	// IF -- end year == zero -- THEN -- license not required
		if ( end_year == 0 )return VS_OK;


	// ---- current date ----
		time_t  my_time = time(NULL);
		tm      my_usable_time;
		errno_t en;
		en = localtime_s(&my_usable_time, &my_time);
		INT year  = my_usable_time.tm_year+1900;
		INT month = my_usable_time.tm_mon + 1;
		INT day   = my_usable_time.tm_mday;


	// ---- expires in a future year, so... ----
		if ( year < end_year ) return VS_OK;


	// ---- but might be this year, so... ----
		if ( year == end_year )
		{
			// expires in a future month, so...
			if ( month < end_month ) return VS_OK;

			// but might be this month, so...
			if ( month == end_month )
			{
				// but not a day after expiry day.
				if ( day <= end_day ) return VS_OK;
			}

		}


	// ---- expired, so report:
	MessageBox(
			0,
			"YOUR LICENSE HAS EXPIRED:\n\n"
				"      Please contact Observant Innovations Limited.\n\n"
				"      Click OK to exit.",
			"Observant Innovations Limited",
			MB_ICONINFORMATION
		);

	return VS_ERROR;
}


////////////////////////////////////////////////////////////////////////////////
