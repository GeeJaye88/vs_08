////////////////////////////////////////////////////////////////////////////////

// ---------- model_camera_methods_incl.h ----------
/*!
\file model_camera_methods_incl.h
\brief include for the Model camera methods
\author Gareth Edwards
*/


// ---- camera kontrol ( [...]_model_kamera.hpp )
	INT  SetMyKamera(INT);


	//  invoked by Kontrol::CallbackByPickPanel
	//           & Kontrol::CallbackByMouseWheel
	//           & Kontrol::CallbackByKeyboard

	INT  SetMsgProc(INT, FLOAT);

	INT  SetMsgTime(INT, FLOAT);
	INT  SetMsgLayout(INT);
	INT  SetMsgKeyPad(INT, FLOAT);
	INT  SetMsgSystem(INT, FLOAT);

	INT  SetMsgCameraPersistantControl();

	INT  SetMsgCameraControl(INT, FLOAT, BOOL);


// ---- camera kontrol ( [...]_model_kamera.hpp )
		//  invoked by Kontrol::CallbackByPickPanel
		//           & Kontrol::CallbackByMouseWheel

	BOOL  GetMsgCameraControlPersistantActive();
	VOID  SetMsgCameraControlPersistantActive(BOOL);

	BOOL  GetMsgCameraControlPersistantOn();
	VOID  SetMsgCameraControlPersistantOn(BOOL);

	INT   GetCameraControlMessage();
	VOID  SetCameraControlMessage(INT);

	INT   GetMsgCameraControlCount();
	VOID  IncMsgCameraControlCount();
	VOID  SetMsgCameraControlCount(INT);

	FLOAT GetCameraControlZoomScalar();
	VOID  SetCameraControlZoomScalar(FLOAT);


	////////////////////////////////////////////////////////////////////////////////
