////////////////////////////////////////////////////////////////////////////////

// ---------- vs_sys_keyboard.h ----------
/*!
\file vs_sys_keyboard.h
\brief Interface for the Sys(tem)Keyboard class.
\author Gareth Edwards
*/

//#pragma once

// ---- vs include ----
#include "../header/vs_inc_defs.h"
#include "../header/vs_app_dependency.h"


// ---------- namespace ----------
namespace vs_system
{


	// ---------- SysKeyboard ----------
	/*!
	\brief Interface to the SysKeyboard class.
	\author Gareth Edwards
	*/
	class SysKeyboard
	{

	public:

		// ---- cdtor
			SysKeyboard();
			~SysKeyboard();


		// ---- framework
			INT Setup(vs_system::AppDependency *);

	private:

		// ---- private implementation
			class PI_SysKeyboard; PI_SysKeyboard *pi_sys_keyboard;

	};


}