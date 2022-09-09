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

			// ---- status lid's
				enum PanelPropertyLid
				{
					BACKGROUND  = 1,
					BORDER      = 2,
					DISPLAY     = 3,
					INTERACTION = 4,
					SELECTED    = 5
				};

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


			// ---- get frame
				AppFrame* GetFirstFrame();
				AppFrame* GetFrame(CHAR *);
				AppFrame* GetSelectedFrame();


			// ---- get panel
				INT GetPanelContext(CHAR *name, PanelContext **context);
				INT GetPanelStatus(CHAR *name, PanelPropertyLid lid, BOOL *value);
				INT GetPanelDisplayStatus(CHAR *, BOOL *);
				INT GetPanelInteractionStatus(CHAR *, BOOL *);


			// ---- set panel
				INT SetPanelStatus(CHAR *name, PanelPropertyLid lid, BOOL value);
				INT SetPanelDisplayStatus(CHAR *, BOOL);
				INT SetPanelDisplayListHandle(CHAR *, INT);
				INT SetPanelInteractionStatus(CHAR *, BOOL);
				INT SetPanelPane(UINT, AppPane *);


			// ---- select panel
				INT SelectPanel(AppPanel *);


			// ---- element
				INT SetElementStatus(INT group_id, INT element_id, INT op_id, BOOL value);


			// ---- select frames
				INT SelectFrame(CHAR *);
				INT SelectFrame(AppFrame *);
				INT SelectFrame(UINT);
				INT SelectFirstFrame();
				INT SelectNextFrame();
				INT SelectLastFrame();

		private:

			// ---- private implementation
				class PI_AppView; PI_AppView *pi_app_view;

		};


}