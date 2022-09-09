////////////////////////////////////////////////////////////////////////////////

// ---------- e1_kontrol.h ----------
/*!
\file e1_kontrol.h
\brief Interface for the AppKontrol class.
\author Gareth Edwards
\note WINDOWS DEPENDANT
*/

#pragma once

#include "../header/e1_include.h"


////////////////////////////////////////////////////////////////////////////////


namespace examples_01
{


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
					vs_system::AppView*
				);

			INT SetBill(INT);

		// --- set
			INT Update();
			static INT UpdatePickPanel(INT, INT);
			static INT UpdatePickElement(INT, INT);
			static INT UpdateByMouseWheel(INT, INT);
			static INT UpdateByKeyboard(INT, INT);


	private:

		// ---- private implementation
			class  PI_Kontrol; PI_Kontrol *pi_kontrol;

	};

}


////////////////////////////////////////////////////////////////////////////////
