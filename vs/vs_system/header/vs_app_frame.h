////////////////////////////////////////////////////////////////////////////////

// ---------- app_frame.h ----------
/*!
\file app_frame.h
\brief Interface for the App(lication)Frame class.
\author Gareth Edwards
*/

#pragma once


// ---- vs include
#include "../header/vs_app_dependency.h"
#include "../header/vs_app_panel.h"


// ---------- namespace ----------
namespace vs_system
{

	// ---------- AppFrame class ----------
	class AppFrame
	{

	public:

		// ---- constructor /destructor
			AppFrame();
			virtual ~AppFrame();


		// ---- framework
			INT Cleanup();
			INT Display(INT handle_app_obj, INT(*app_callback_display)(INT, AppPanel *));
			INT Setup(AppDependency *);


		// ---- configure
			INT Configure(VNode *);
			INT AppendPanelToList(VNode *);


		// ---- get
			AppPanel *GetFirstPanel();
			BOOL      GetHome();
			CHAR     *GetName();
			AppFrame *GetNext();
			AppFrame *GetLast();


		// ---- set
			INT SetHome(BOOL);
			INT SetNext(AppFrame *);
			INT SetLast(AppFrame *);


		// ---- panels
			INT SelectPanel(CHAR *, BOOL);


		// ---- element
			INT SetElementStatus(INT group_id, INT element_id, INT op_id, BOOL value);


	private:

		// ---- private implementation
			class PI_AppFrame; PI_AppFrame *pi_app_frame;

	};


}