////////////////////////////////////////////////////////////////////////////////

// ---------- vs_app_keyboard.h ----------
/*!
\file vs_app_keyboard.h
\brief Interface for the App(lication)Keyboard class
\author Gareth Edwards
*/

#pragma once


// ---- vs include ----
#include "../header/vs_app_dependency.h"


namespace vs_system
{


	// ---------- AppKeyboard class----------
	class AppKeyboard
	{

	public:

		// ---- cdtor
			AppKeyboard();
			virtual ~AppKeyboard();


		// ---- set
			INT SetDependency(vs_system::AppDependency*);
			INT InvokeInputCallbacks();
			INT SetPickPanelCallback(INT(*)(INT, INT), INT);
			INT SetKeyboardCallback(INT(*)(INT, INT), INT);


		// ---- get/set persistant panel gfx gontext
			vs_system::PanelContext *GetPersistantPanelPanelContext();
			INT SetPersistantPanelPanelContext(vs_system::PanelContext *);


	private:

		// ---- private implementation
			class  PI_AppKeyboard; PI_AppKeyboard *pi_app_keyboard;

	};

}