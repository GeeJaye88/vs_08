////////////////////////////////////////////////////////////////////////////////

// ---------- kontrol_class.h ----------
/*!
\file kontrol_class.h
\brief Interface for the Kontrol class.
\author Gareth Edwards
*/

#include "../../header/vs_app_setup.h"


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


	// ---- Kpad - required
		INT KpadSetDisplayListHandles();
		static INT KpadUpdateHead(vs_system::AppKontrol *, INT, INT);
		static INT KpadUpdateBody(vs_system::AppKontrol *, INT, INT);
		static INT KpadUpdateText(vs_system::AppKontrol *);


private:

	// ---- private implementation
		class  PI_Kontrol; PI_Kontrol *pi_kontrol;

};


////////////////////////////////////////////////////////////////////////////////
