
// ---------- SetupGPS ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SetupGPS()
{

	#ifdef VS_CAMERA_03_GPS

	pi_props->sys_output->MessageSetup("Begin", "SetupGPS");


	try
	{
		pi_props->pGPSReader = new GPSReader(pi_props->s_gps_port, true);
		pi_props->pGPSReader->Run();

		// create a task to delay runtime status
		task<INT> failure_task = complete_after(2000).then([]
			{
				pi_props->gps_mode = PI_Props::GPS_TEST_RUNNING;
				return VS_OK;
			}
		);

	}
	catch (...)
	{
		pi_props->sys_output->Message("     : +++ Caught exception in SetupGPS! +++");
		DBMSG("Exception!");
		return VS_ERROR;
	}

	
	pi_props->sys_output->MessageSetup("End", "SetupGPS");

	#else

	pi_props->sys_output->Message("Setup: +++ NO SetupGPS +++");

	#endif

	return VS_OK;
}


// ---------- CleanupGPS ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::CleanupGPS()
{

	#ifdef VS_CAMERA_03_GPS

	if ( pi_props->pGPSReader != 0 )
	{
		pi_props->pGPSReader->Halt();
		delete pi_props->pGPSReader;
		pi_props->pGPSReader = NULL;
	}

	#endif

	return VS_OK;
}

