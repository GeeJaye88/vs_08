////////////////////////////////////////////////////////////////////////////////

// ---------- e1_kontrol.h ----------
/*!
\file e1_kontrol.h
\brief Interface for the AppKontrol class.
\author Gareth Edwards
\note WINDOWS DEPENDANT
*/

#pragma once

#include "../header/s2_include.h"


////////////////////////////////////////////////////////////////////////////////


namespace surface_02
{

	class Model;

	// ---------- Kontrol class----------
	class Kontrol
	{

	public:

		// ---- cdtor
			Kontrol();
			virtual ~Kontrol();


		// --- set
			INT Setup
				(
					vs_system::AppDependency*,
					vs_system::AppView*,
					Model *
				);


		// --- set
			INT Update();


		// ---- callback
			static INT CallbackByPickPanel(INT, INT);
			static INT CallbackByPickElement(INT, INT);
			static INT CallbackByMouseWheel(INT, INT);
			static INT CallbackByKeyboard(INT, INT);


		// ---- Tint - required
			INT TintSetDisplayListHandles();
			static INT TintUpdateHead(vs_system::AppKontrol *, INT, INT);
			static INT TintUpdateBody(vs_system::AppKontrol *, INT, INT);
			static INT TintUpdateText(vs_system::AppKontrol *);


	private:

		// ---- private implementation
			class  PI_Kontrol; PI_Kontrol *pi_kontrol;

	};

}


////////////////////////////////////////////////////////////////////////////////
