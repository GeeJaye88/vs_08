////////////////////////////////////////////////////////////////////////////////

// ---------- r2_control.h ----------
/*!
\file r2_control.h
\brief Interface for the Control class.
\author Gareth Edwards
*/

#ifndef REVIEW_02_CONTROL
#define REVIEW_02_CONTROL


// ---- MUST include Control object interface----
#include "../../../vs_system/header/vs_app_control.h"


// ---- include ----
#include "r2_include.h"
#include "r2_model.h"


// ---------- namespace ----------
namespace review_02
{


// ---------- Control class ----------
class Control
{

	/////////////////////////////////////////////////////////////////////////////
	//
	// Include "DO NOT ALTER" Control object interface
	//
	#include "../../../vs_system/hpp/vs_app_control_interface.hpp"
	//
	/////////////////////////////////////////////////////////////////////////////


public:

	// ---- c/dtor 
		INT  Construct();
		INT  Destruct();


	// ---- framework - invoked from application
		INT Control::Setup(
				vs_system::AppDependency *ad,
				vs_system::AppView *w,
				review_02::Model *m
			);
		INT Control::Cleanup();


	// ---- application callbacks
		//INT SetCallbackToSetupGraphics(INT(*)());
		//INT SetCallbackToCleanupGraphics(INT(*)());


	// ---- input stubs
		INT KeyPressedDown() { return VS_OK; };
		INT CursorMoved()    { return VS_OK; };
		INT WheelClicked()   { return VS_OK; };


	// ---- camera
		BOOL GetCameraExposureMustBeUpdated();
		BOOL GetCameraUpdateOffset();
		BOOL GetCameraLive();
		BOOL GetCameraLoopHardStart();
		BOOL GetCameraPanophoto();


	// ---- archive ----
		BOOL GetArchiveSave();
		INT  SetArchiveSave(BOOL);


	// ---- dashboard ----
		BOOL GetDashboardOnly();
		INT  SetDashboardOnly(BOOL);


	// ---- fx ----
		UINT GetFXColourMode();
		UINT GetFXBoostMode();


	// ---- stream & remote control ----
		BOOL GetStreamFlag();
		BOOL GetRemoteControlFlag();
		BOOL SetStreamStatus(BOOL);
		BOOL SetRemoteControlStatus(BOOL);


	// ---- timeline ----
		UINT GetTimelineCommand(UINT *, INT []);
		BOOL GetTimelineLive();
		UINT GetTimelineSpeedMode();
		BOOL GetTimelinePlay();
		INT  SetTimelineLive(BOOL);
		INT  SetTimelinePlay(BOOL);


	// ---- panel callback ----

		static INT UpdateFrameGroup(INT);
		static INT UpdatePanel(INT, INT);


	// ---- invoked from UpdatePanel ----

		static INT SetPanelActive(
				Control *,
				vs_system::AppDependency *,
				vs_system::AppPanel *,
				vs_system::PanelContext *
			);

		static INT SetMouseWheelFov(
				Control *,
				vs_system::AppDependency *,
				vs_system::PanelContext *
			);


	// ---- invoked from UpdatePanel ----

		static INT UpdateElemImage(
				Control*,
				vs_system::AppControl *,
				vs_system::AppPanel *,
				vs_system::PanelContext *,
				INT
			);

		static INT UpdateElemPano(
				Control*,
				vs_system::AppControl *,
				vs_system::AppPanel *,
				vs_system::PanelContext *,
				INT
			);


	// ---- pano ----

		static INT UpdateElemPanoSphericalSoloPanTilt(
				Control *,
				vs_system::AppDependency *,
				vs_system::PanelContext *,
				VsFloatRectangle *,
				BOOL,
				BOOL
			);

		static INT UpdateElemPanoAnnularPanTilt(
				Control *,
				vs_system::AppDependency *,
				VsFloatRectangle *,
				UINT
			);

		static INT UpdateElemPanoPanoramaPanTilt(
				Control *,
				vs_system::AppDependency *,
				VsFloatRectangle *,
				UINT
			);

		static INT UpdateElemPanoSphericalPanTilt(
				Control *,
				vs_system::AppDependency *,
				VsFloatRectangle *,
				UINT
			);

		static INT UpdateElemPanoCylinderPanTilt(
				Control *,
				vs_system::AppDependency *,
				VsFloatRectangle *,
				UINT
			);


	// ---- image pt
		static INT UpdateElemImagePanTilt(
				Control *,
				vs_system::AppDependency *,
				vs_system::PanelContext *,
				VsFloatRectangle *
			);


	// ---- img panel
		static INT UpdateImgPtzPanelsNavButtons(Control*, UINT);


	// ---- tint
		static INT UpdateTintHead(Control*);
		static INT UpdateTintCamBody(Control*, INT, INT);
		static INT UpdateTintImgBody(Control*, INT, INT);
		static INT UpdateTintImgText(Control*);


	// ---- element callback
		static INT UpdateElement(INT, INT);
		static INT UpdateElementPTF(INT, UINT);


private:

	// ---- system
		vs_system::AppDependency *app_dependency;
		vs_system::AppView *app_view;


	// ---- application
		review_02::Model *model;


	// ---- private implmentations
		class PI_Application; PI_Application    *pi_application;
		class PI_Camera;      PI_Camera         *pi_camera;
		class PI_Timeline;    PI_Timeline       *pi_timeline;


public:

	// ---- public implmentations
		class PI_Context;     PI_Context        *pi_context;


};


class  Control::PI_Context;
static Control::PI_Context *s_pi_context;


} // review_02 namespace


#endif // REVIEW_02_CONTROL


