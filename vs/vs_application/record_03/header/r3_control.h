////////////////////////////////////////////////////////////////////////////////

// ---------- r3_control.h ----------
/*!
\file r3_control.h
\brief Interface for the Control class.
\author Gareth Edwards
\note Vanilla C++
*/

#ifndef RECORD_03_CONTROL
#define RECORD_03_CONTROL


// ---------- include ----------

/////////////////////////////////////////////////////////////////////////////
//
//MUST include Control object interface
//
#include "../../../vs_system/header/vs_app_control.h"
//
/////////////////////////////////////////////////////////////////////////////


// ---------- include ----------
#include "r3_include.h"
#include "r3_model.h"


// ---------- namespace ----------
namespace record_03
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

	// Construct/destruct
	INT  Construct();
	INT  Destruct();

	// Framework - invoked from application
	INT Control::Setup(
			vs_system::AppDependency *ad,
			vs_system::AppView *w,
			record_03::Model *m
		);
	INT Control::Cleanup();

	// Input stubs
	INT KeyPressedDown() { return VS_OK; };
	INT CursorMoved()    { return VS_OK; };
	INT WheelClicked()   { return VS_OK; };

	// Camera
	BOOL GetCameraLive();
	BOOL GetCameraArchive();
	INT  SetCameraLive(BOOL);
	INT  SetCameraArchive(BOOL);

	// Panel - invoked from callback
	static INT UpdatePanel(INT, INT);

	// Element - invoked from callback
	static INT UpdateElement(INT, INT);

private:

	// System
	vs_system::AppDependency *app_dependency;
	vs_system::AppView *app_view;

	// Application
	record_03::Model *model;

	// Private implmentation
	class PI_Timeline; PI_Timeline *pi_timeline;
	class PI_Camera;   PI_Camera   *pi_camera;


}; // Control class


} // record_03 namespace


#endif // RECORD_03_CONTROL


