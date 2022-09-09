////////////////////////////////////////////////////////////////////////////////

// ---------- app_layout.h ----------
/*!
\file app_layout.h
\brief Interface for the App(lication)Layout class.
\author Gareth Edwards
*/

#pragma once


// ---- vs include
#include "../header/vs_app_dependency.h"


// ---------- namespace ----------
namespace vs_system
{


	// ---------- AppLayout class ----------
	class AppLayout
	{

	public:

		enum Rect
		{
			ASPECT   = 0,
			BORDER_X = 1,
			BORDER_Y = 2,
			SPACE_X  = 3,
			SPACE_Y  = 4
		};


		// ---- constructor /destructor
			AppLayout();
			virtual ~AppLayout();


		// ---- setup
			INT Setup(AppDependency *);


		// ---- get
			AppDependency *GetAppDependency();
			INT GetBorderRect(VsFloatRectangle *);
			FLOAT GetValue(UINT);


			// --- set
			INT SetLayout();
			INT SetX();
			INT SetY();



	private:

		// ---- private implementation
			class PI_AppLayout; PI_AppLayout *pi_app_layout;

	};


}