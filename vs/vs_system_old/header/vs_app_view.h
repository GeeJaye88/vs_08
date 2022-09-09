////////////////////////////////////////////////////////////////////////////////

// ---------- app_view.h ----------
/*!
\file app_view.h
\brief Interface for the App(lication)View class.
\author Gareth Edwards
*/

#pragma once


// ---- vs include
#include "../header/vs_app_dependency.h"
#include "../header/vs_app_frame.h"


// ---------- namespace ----------
namespace vs_system
{


	// ---------- AppPane struct ----------
		struct AppPane
		{
			CHAR name[VS_MAXCHARNAME];
			VsFloatRectangle rect;
		};


	// ---------- AppView class ----------
	class AppView
	{

	public:

		// ---- c/dtor
			AppView();
			virtual ~AppView();


		// ----  framework
			INT Cleanup();
			INT Display(INT handle_app_obj, INT(* app_callback_display)(INT, AppPanel *));
			INT Setup(AppDependency *);


		// ---- configure
			INT Configure(VNode *);
			INT	AppendFrameToList(VNode *);
			INT SetMasterPanels();


		// ---- get
			AppFrame* GetFirstFrame();
			AppFrame* GetFrame(CHAR *);
			AppFrame* GetSelectedFrame();
			INT GetDisplayStatus(CHAR *, BOOL *);
			INT GetInteractionStatus(CHAR *, BOOL *);


		// ---- set
			INT SetDisplayStatus(CHAR *, BOOL);
			INT SetDisplayListHandle(CHAR *, INT);
			INT SetInteractionStatus(CHAR *, BOOL);


		// ---- frames
			INT SelectFrame(CHAR *);
			INT SelectFrame(AppFrame *);
			INT SelectFrame(UINT);
			INT SelectFirstFrame();
			INT SelectNextFrame();
			INT SelectLastFrame();


		// ---- panel
			INT SetPanelViewrects(UINT, AppPane *);
			INT SelectPanel(AppPanel *);

	private:

		// ---- private implementation
			class PI_AppView; PI_AppView *pi_app_view;

	};


}