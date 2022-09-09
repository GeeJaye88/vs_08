
// ---------- SetupIPC ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SetupIPC()
{

	#ifdef VS_CAMERA_03_STREAMING

	pi_props->sys_output->MessageSetup("Begin", "SetupIPC");


	// ---- local ----
	INT result = 0;
	vs_system::SysOutput *sys_output = pi_props->app_dependency->GetSysOutput();


	// Create the IPC object. The name is set in apptypes.h to
	// Observant-Innovations.MiniStream. This means an application
	// can create multiple named instances if required the default 
	// transfer size is 64MB but this can be any suitable value. 


	// ---- wrap in try catch ----
	try
	{
		// create the IPC object
		;
	}
	catch (...)
	{
		// yak! problem!
		pi_props->sys_output->Message("     : +++ Caught problem in SetupIPC +++");
		return VS_ERROR;
	}

	pi_props->sys_output->MessageSetup("End", "SetupIPC");

	#else

	pi_props->sys_output->MessageSetup("Setup", "+++ NO SetupIPC +++");

	#endif

	return VS_OK;
}


// ---------- CleanupIPC ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::CleanupIPC()
{

	#ifdef VS_CAMERA_03_STREAMING

	;

	#endif

	return VS_OK;
}
