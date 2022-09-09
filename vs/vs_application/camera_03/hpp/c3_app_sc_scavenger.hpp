
// ---------- SetupScavenger ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SetupScavenger()
{

	#ifdef VS_CAMERA_03_SCAVENGER

	pi_props->sys_output->MessageSetup("Begin", "SetupScavenger");


	// ---- local
		INT result = 0;
		vs_system::SysOutput *sys_output = pi_props->app_dependency->GetSysOutput();


	// ---- TBD: error check


	// ---- set scavenger_app_path
		CHAR *userProfile;
		size_t userProfileLen;
		_dupenv_s(&userProfile, &userProfileLen, "USERPROFILE");
		//
		// TBD 'userProfile' could be '0'
		//
		sprintf_s(
				pi_props->scavenger_app_path,
				VS_MAXCHARPATHLEN,
				#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
				"%s\\AppData\\Local\\Observant Innovations\\Monitor\\scavenger",
				#else
				"%s\\AppData\\Local\\Observant Innovations\\Patrol\\scavenger",
				#endif
				userProfile
			);


	// ---- check for "scavenger.exe"
		if ( strcmp(pi_props->scavenger_app_name, "") == 0 )
		{
			sys_output->MessageError(
					"     : +++ Scavenger application name not valid.",
					"Name",
					"undefined in config file +++"
				);
			return VS_ERROR;
		}


	// ---- already scavenging ? -- kill
		result = FindAndKillProcess(pi_props->scavenger_app_name);



	// ---- start
		if ( pi_props->scavenging )
		{
			result = StartScavenger();
		}


		pi_props->sys_output->MessageSetup("End", "SetupScavenger");

	#else

		pi_props->sys_output->MessageSetup("Setup", "+++ NO Scavenger +++");

	#endif

	return VS_OK;
}


// ---------- SetScavengerLocation  ----------
/*!
\brief Set path
\author Gareth Edwards
*/
INT application::SetScavengerLocation(CHAR *path)
{
	strcpy(pi_props->scavenger_loc_path, path);
	return VS_OK;
}


// ---------- StartScavenger  ----------
/*!
\brief 
\author Gareth Edwards
*/
INT application::StartScavenger()
{

	// ---- create command line
		CHAR cmd_line[VS_MAXCHARLEN];
		sprintf_s(
			cmd_line,
			VS_MAXCHARLEN,
				"\"%s\\%s \" %s %d %d %d",
				pi_props->scavenger_app_path,
				pi_props->scavenger_app_name,
				pi_props->scavenger_loc_path,
				(INT)pi_props->scavenger_minutes_max,
				(INT)pi_props->scavenger_seconds_pause,
				(INT)pi_props->scavenger_log
			);


	// ---- start
		INT result = StartProcess(
				NULL,
				cmd_line
			);

		pi_props->scavenging = TRUE;

	return result;
}


// ---------- CleanupScavenger ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::CleanupScavenger()
{

	#ifdef VS_CAMERA_03_SCAVENGER

	INT result = FindAndKillProcess(pi_props->scavenger_app_name);

	#endif

	return VS_OK;
}

