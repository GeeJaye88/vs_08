////////////////////////////////////////////////////////////////////////////////

// ---------- mount_calc_01.h ----------
/*!
\file mount_calc_01.h
\brief Interface for the application class.
\author Gareth Edwards
\note Vanilla C++
*/

#pragma once

// ---- std ----
	#include <stdio.h>   // FILE, 
	#include <string.h>  // printf,
	#include <ctype.h>   // toupper, 
	#include <iostream>


// ---- vs system
	#include <../vs_07/vs/vs_system/header/vs_inc_defs.h>

// ---- vs library
	#include <../vs_07/vs/vs_library/header/vs_vnode.h>


namespace vs_system
{

	// ---- iconfig class
	class DotIni
	{

		public:

			// ---- c/dtor
				DotIni();
				virtual ~DotIni();


			// ---- methods
				INT Initialise(CHAR *filename);
				INT Read();
				INT Write();
	
			// ---- get
				VNode * GetRoot();

		private:

			// ---- private implementation
				class PI_DotIni; PI_DotIni *pi_dotini;

	};

}
