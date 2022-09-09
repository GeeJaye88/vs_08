////////////////////////////////////////////////////////////////////////////////

// ---------- vs_sys_license.h ----------
/*!
\file vs_sys_license.h
\brief Interface for the Sys(tem)License class.
\author Gareth Edwards
*/

#pragma once


// ---- vs include ----
#include "vs_inc_defs.h"


// ---------- namespace ----------
namespace vs_system
{


	// ---------- class ----------
	class SysLicense
	{

	public:

		// Constructor /destructor
		SysLicense();
		virtual ~SysLicense();

		// Methods
		INT Valid();

	private:

		class PI_SysLicense; PI_SysLicense  *pi_sys_license;

	};


}