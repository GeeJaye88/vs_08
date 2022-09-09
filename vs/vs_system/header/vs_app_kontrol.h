////////////////////////////////////////////////////////////////////////////////

// ---------- vs_app_kontrol.h ----------
/*!
\file vs_app_kontrol.h
\brief Interface for the App(lication)Kontrol class
\author Gareth Edwards
*/

#pragma once


// ---- vs include ----
#include "../header/vs_app_dependency.h"
#include "../header/vs_app_keyboard.h"
#include "../header/vs_app_view.h"


namespace vs_system
{

	// ---------- AppKontrol class----------
	class AppKontrol
	{

	public:

		// ---- cdtor
			AppKontrol();
			virtual ~AppKontrol();


		// ---- framework methods
			INT Setup(vs_system::AppDependency*);


		// ---- get
			AppKeyboard   *GetAppKeyboard();
			AppDependency* GetAppDependency();
			AppView       *GetAppView();
			INT            GetHandleKontrol();
			INT            GetHandleModel();


		// ---- set
			INT InvokeInputCallbacks();
			INT SetPickCallbackFromGfxInterface();
			INT SetView(AppView *);
			INT SetHandleKontrol(INT);
			INT SetHandleModel(INT);


		// --- callback from pick via gfx interface
			//
			// note: only interface to Update methods below
			//       and might be removed...
			//
			static INT CallbackPickPanelFromGfxInterface(INT, INT);
			static INT CallbackPickElementFromGfxInterface(INT, INT);


		// --- callback from gfx interface via callbacks above
			static INT UpdatePanelFromCallback(INT, INT);
			static INT UpdateElementFromCallback(INT, INT);
			static INT UpdateByMouseWheelFromPanelCallback(INT, INT);
			static INT UpdateByKeyboardCallback(INT, INT);


		// --- application callbacks from callbacks above
			INT SetCallbackUpdatePickPanel(INT(*)(INT, INT));
			INT SetCallbackUpdatePickElement(INT(*)(INT, INT));
			INT SetCallbackUpdateByMouseWheel(INT(*)(INT, INT));
			INT SetCallbackUpdateByKeyboard(INT(*)(INT, INT));

	private:

		// ---- private implementation
			class  PI_AppKontrol; PI_AppKontrol *pi_app_kontrol;

	};

}
