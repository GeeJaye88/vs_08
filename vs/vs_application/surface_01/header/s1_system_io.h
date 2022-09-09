////////////////////////////////////////////////////////////////////////////////

// ---------- s1_system_io.h ----------
/*!
\file s1_system_io.h
\brief Interface for the SystemIO class.
\author Gareth Edwards
*/

#pragma once

#include "../header/s1_include.h"


////////////////////////////////////////////////////////////////////////////////


namespace surface_01
{

	// ---------- system object io ----------
	class SystemIO
	{

		public:

		// ---- cdtor
			SystemIO();
			virtual ~SystemIO();

		// ---- io
			INT Read(SystemObject *, CHAR *);
			INT Write();

		// ---- get
			INT    GetKeyCount();
			CHAR * GetKey(INT, CHAR *);
			CHAR * GetDescriptor(INT, CHAR *);
			CHAR * GetKeyDescriptor(CHAR *);

		private:

			class PI_SystemIO; PI_SystemIO *pi_system_io;

	};

}


////////////////////////////////////////////////////////////////////////////////
