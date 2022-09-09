////////////////////////////////////////////////////////////////////////////////

// ---------- app_panel.h ----------
/*!
\file app_panel.h
\brief Interface for the App(lication)Panel class.
\author Gareth Edwards
*/

#pragma once


// ---- vs include
#include "../header/vs_app_dependency.h"


// ---------- namespace ----------
namespace vs_system
{


	// ---------- AppPanel class ----------
	class AppPanel
	{

	public:

		// ---- c/dtor
			AppPanel();
			virtual ~AppPanel();


		// ---- framework
			INT Cleanup();
			INT Display(INT, INT(*)(INT, AppPanel *));
			INT Setup(AppDependency *);


		// ---- setup
			INT Configure(VNode *);


		// ---- get
			PanelContext *GetPanelContext();
			INT           GetIndex();
			AppPanel     *GetNext();
			AppPanel     *GetMaster();
	

		// ---- set
			INT SetIndex(INT);
			INT SetNext(AppPanel *);
			INT SetMaster(AppPanel *);

	private:

		// ---- private implementation
			class PI_AppPanel; PI_AppPanel *pi_app_panel;

	};


}