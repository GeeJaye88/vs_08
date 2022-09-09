////////////////////////////////////////////////////////////////////////////////

// ---------- model_camera_methods.hpp ----------
/*!
\file model_camera_methods.hpp
\brief Implementation of Model class camera methods
\author Gareth Edwards
*/


////////////////////////////////////////////////////////////////////////////////


BOOL Model::GetMsgCameraControlPersistantActive()
{
	return pi_model->camera_control_persist_active;
}
VOID Model::SetMsgCameraControlPersistantActive(BOOL active)
{
	pi_model->camera_control_persist_active = active;
}


BOOL Model::GetMsgCameraControlPersistantOn()
{
	return pi_model->camera_control_persist_switched_on;
}
VOID Model::SetMsgCameraControlPersistantOn(BOOL switched_on)
{
	pi_model->camera_control_persist_switched_on = switched_on;
}


INT Model::GetCameraControlMessage()
{
	return pi_model->camera_control_message;
}
VOID Model::SetCameraControlMessage(INT message)
{
	pi_model->camera_control_message = message;
}


/*

\note prevent unwanted control messages

	See methods:
	- SetMsgCameraPersistantControl
	- SetMsgCameraControl

  note: prevent "persistant same message" repeat increments
       resulting in animation double frame jumps 

*/
INT Model::GetMsgCameraControlCount()
{
	return pi_model->camera_control_message_count;
}
VOID Model::IncMsgCameraControlCount()
{
	pi_model->camera_control_message_count++;
}
VOID Model::SetMsgCameraControlCount(INT count)
{
	pi_model->camera_control_message_count = count;
}


FLOAT Model::GetCameraControlZoomScalar()
{
	return pi_model->camera_control_zoom_scalar;
}
VOID Model::SetCameraControlZoomScalar(FLOAT zoom_scalar)
{
	pi_model->camera_control_zoom_scalar = zoom_scalar;
}


////////////////////////////////////////////////////////////////////////////////