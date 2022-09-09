////////////////////////////////////////////////////////////////////////////////

// ---------- e1_kontrol.h ----------
/*!
\file e1_kontrol.h
\brief Interface for the AppKontrol class.
\author Gareth Edwards
\note WINDOWS DEPENDANT
*/

#pragma once


#include "../header/c1_include.h"


////////////////////////////////////////////////////////////////////////////////


namespace cube_01
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


	private:

		// ---- private implementation
			class  PI_Kontrol; PI_Kontrol *pi_kontrol;

	};

}


////////////////////////////////////////////////////////////////////////////////
