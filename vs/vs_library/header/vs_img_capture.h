////////////////////////////////////////////////////////////////////////////////

// ---------- vs_img_analysis.h ----------
/*!
\file vs_img_analysis.h
\brief Interface for the ImgAnalysis class.
\author Gareth Edwards
*/

#pragma


#include <vs/vs_system/header/vs_inc_defs.h>
#include <vs/vs_system/header/vs_app_dependency.h>


namespace vs_system
{

	class ImgCapture
	{

	public:

		// ---- cdtor ----
			ImgCapture();
			~ImgCapture();

			INT Setup(
					vs_system::AppDependency *app_dependency,
					CHAR *directory,
					INT trigger
				);
			INT Capture();

		// ---- pimp ----
			class  PI_ImgCapture;
			PI_ImgCapture *pi_img_capture;

	};

}



////////////////////////////////////////////////////////////////////////////////
