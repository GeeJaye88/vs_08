
// ---------- SetupShared ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SetupSharedMemory()
{

	#ifdef VS_CAMERA_03_SHARED_MEMORY

	//pi_props->sys_output->MessageSetup("Begin", "SetupSharedMemeory");

	;

	//pi_props->sys_output->MessageSetup("End", "SetupSharedMemeory");

	#else

	pi_props->sys_output->MessageSetup("Setup", "+++ NO Shared Memeory +++");

	#endif

	return VS_OK;
}


// ---------- CleanupSharedMemory ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::CleanupSharedMemory()
{

	#ifdef VS_CAMERA_03_SHARED_MEMORY

	try
	{
		if (shared_memory_handle != 0)
		{
			INT ret = sm_term(&shared_memory_handle);
			if (ret != esmOK)
				throw "CleanupSharedMemory: bad sm_term";
		}
	}
	catch (const char* e)
	{
		const char* p = e;
		OutputDebugStringA(p);
		OutputDebugStringA("\n");
	}
	catch (...)
	{
		OutputDebugStringA("CleanupSharedMemory: Unknown exception\n");
	}

	#endif

	return VS_OK;
}

