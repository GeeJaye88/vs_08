
// ---------- SetupStreaming ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SetupStreaming()
{

	#ifdef VS_CAMERA_03_STREAMING

	pi_props->sys_output->MessageSetup("Begin", "SetupStream");


	// ---- local ----
	INT result = 0;
	CHAR  cmd_line[VS_MAXCHARLEN];
	vs_system::SysOutput *sys_output = pi_props->app_dependency->GetSysOutput();


	// TBD: error check


	// ---- set stream_app_path ----
	CHAR *userProfile;
	size_t userProfileLen;
	_dupenv_s(&userProfile, &userProfileLen, "USERPROFILE");
	//
	// TBD 'userProfile' could be '0':  this does not adhere to the
	// specification for the function 'strcpy_s'. 
	//
	sprintf_s(
			pi_props->stream_app_path,
			VS_MAXCHARPATHLEN,
			#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
			"%s\\AppData\\Local\\Observant Innovations\\Monitor\\ministream",
			#else
			"%s\\AppData\\Local\\Observant Innovations\\Patrol\\ministream",
			#endif
			userProfile
		);


	// ---- check for stream application name, e.g. CHAR *app_name = "MiniStream.exe"; ----
	if ( strcmp(pi_props->stream_app_name, "") == 0 )
	{
		sys_output->MessageError(
				"     : +++ Streaming application name not valid.",
				"Name",
				"undefined in config file +++"
			);
		return VS_ERROR;
	}


	// ---- IF streaming app still running THEN kill ----
	result = FindAndKillProcess(pi_props->stream_app_name);


	// ---- create MiniStream command line ----
	sprintf_s(
			cmd_line,
			VS_MAXCHARLEN,
			"%s\\%s -f %d -iw %d -ow %d -ih %d -oh %d",
			pi_props->stream_app_path,
			pi_props->stream_app_name,
			pi_props->stream_app_fps,
			pi_props->stream_image_width,
			pi_props->stream_image_width,
			pi_props->stream_image_height,
			pi_props->stream_image_height
		);


	// ---- start streaming app ----
	result = StartProcess(
			NULL,
			cmd_line
		);


	pi_props->sys_output->MessageSetup("End", "SetupStream");

	#else

	pi_props->sys_output->MessageSetup("Setup", "+++ NO SetupStream +++");

	#endif

	return VS_OK;
}


// ---------- CleanupStreaming ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::CleanupStreaming()
{

	#ifdef VS_CAMERA_03_STREAMING

	INT result = FindAndKillProcess(pi_props->stream_app_name);

	#endif

	return VS_OK;
}