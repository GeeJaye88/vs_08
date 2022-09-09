
// ---------- SetupONVIF ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::SetupONVIF()
{

	#ifdef VS_CAMERA_03_STREAMING


	// ---- local ----
	INT result = 0;
	CHAR message[VS_MAXCHARLEN];
	

	pi_props->sys_output->MessageSetup("Begin", "SetupONVIF");


	// ---- essential: wrap in try/catch ----
	try
	{
		// ---- initialize winsock. must do this ----
		Chordia::CWSInit wsi;


		// ---- create a handler ----
		pi_props->onvif_handler = std::make_shared<OnvifHandler>();


		// ---- maps MAC address to IP address
		std::vector<Chordia::MacInfo> macinfo;


		// ---- get adapter name, (1st) IP address and MAC ----
		if ( !Chordia::GetInterfaces(macinfo) )
		{
			pi_props->sys_output->Message("       +++ Cannot enumerate network interfaces. +++");
			throw "ONVIF: Cannot enumerate network interfaces";
		}


		// ---- set up handler parameters ----
		//
		//   Note: ONVIFHTTPHandler::Args handler_args;

		// set port on which server will listen
		CHAR *port_number = pi_props->onvif_port_number; // Initialised as "8001";
		sprintf(message, "ONVIF: Port on which server will listen: %s", port_number);
		pi_props->sys_output->Message(message);
		pi_props->onvif_handler_args.onvifport = _T(port_number);

		// Note There is also a new rtsp_port which defaults to 8554
		pi_props->onvif_handler_args.rtspport = _T("8554");
		
		// m_macros["$RTSP_PORT"] = Chordia::t2n(m_args.rtspport);


		// ---- get the Local IP Address ----


		/*
		Get the local machine’s IP addresses.
		Yes, “addresses, ” plural
		You can count on seeing at least two addresses on most machines:
		one for the loopback interface (127.0.0.1) and at least one for
		an external network interface.It is not at all uncommon for a
		single machine to have multiple network interfaces: a PC on a
		LAN with a modem connected to the Internet, for instance.
		The loopback interface lets two programs running on a single
		machine talk to each other without involving hardware drivers.
		*/
		CHAR ac[80];
		if ( gethostname(ac, sizeof(ac)) == SOCKET_ERROR )
		{
			sprintf(message, "       +++ Error %d when getting local host name +++", WSAGetLastError());
			pi_props->sys_output->Message(message);
			Sleep(1000);
			return VS_ERROR;
		}


		/*
		Structures returned by network data base library, taken from the
		BSD file netdb.h.  All addresses are supplied in host order, and
		returned in network order (suitable for use in system calls).
		*/
		struct hostent *phe = gethostbyname(ac);
		if ( phe == 0 ) {
			sprintf(message, "       +++ Error! Bad host lookup +++");
			pi_props->sys_output->Message(message);
			Sleep(1000);
			return VS_ERROR;
		}


		/*
		The loop near the bottom of the doit() function ensures that all
		of these interfaces are listed.
		If you want to programmatically pick one of these interfaces
		intelligently, you’re more or less on your own. Often you end
		up asking to the user to pick one.
		*/
		struct in_addr addr[4];
		for (INT i = 0; phe->h_addr_list[i] != 0; ++i)
		{
			memcpy(&addr[i], phe->h_addr_list[i], sizeof(struct in_addr));
			sprintf(message, "       Address (network order) %d is %s", i, inet_ntoa(addr[i]));
			pi_props->sys_output->Message(message);
		}


		// ---- check format of supplied network address ----


		/*
		   IF NOT a supplied ip address THEN use first network order ip address... 
		*/
		if ( !strcmp("0", pi_props->onvif_ip_address) ) // Initialised as "0";
		{
			// IP address of host machine
			// pi_props->onvif_handler_args.hostaddr = macinfo.begin()->ipaddress;
			sprintf(message, "       Using Address 0: %s", inet_ntoa(addr[0]));
			pi_props->sys_output->Message(message);
			pi_props->onvif_handler_args.hostaddr = _T(inet_ntoa(addr[0]));
		}
		// ELSE use supplied IP address
		else
		{
			sprintf(message, "       Using supplied ip address %s", pi_props->onvif_ip_address);
			pi_props->sys_output->Message(message);
			pi_props->onvif_handler_args.hostaddr = _T(pi_props->onvif_ip_address);
		}


		// MAC sextet
		pi_props->onvif_handler_args.macaddr = macinfo.begin()->macaddress;
		

		// ---- set full path to XML files for canned ONVIF responses ----


		// ---- 1. get user location ----
		CHAR user_folder[VS_MAXCHARPATHLEN];
		#ifdef OS_WINDOWS
		CHAR *userProfile;
		size_t userProfileLen;
		_dupenv_s(&userProfile, &userProfileLen, "USERPROFILE");
		//
		// TBD 'userProfile' could be '0':  this does not adhere to the
		// specification for the function 'strcpy_s'. 
		//
		strcpy_s(user_folder, VS_MAXCHARPATHLEN, userProfile);
		#else OTHER OS
		strcpy_s(user_folder, VS_MAXCHARPATHLEN, context->app_folder_path);
		#endif OS_WINDOWS


		// ---- 2. append appdata folder path ----
		CHAR mocks_folder[VS_MAXCHARPATHLEN];
		#if defined(VS_CAMERA_05) || defined(VS_REVIEW_04)
		sprintf(mocks_folder, "%s/AppData/Local/Observant Innovations/Monitor/mocks", user_folder);
		#else
		sprintf(mocks_folder, "%s/AppData/Local/Observant Innovations/Patrol/mocks", user_folder);
		#endif
		pi_props->onvif_handler_args.asset_folder = _T(mocks_folder);


		// ---- 3. run it ----
		pi_props->onvif_handler->Run(pi_props->onvif_handler_args);


		// ---- 4. set ONVIF WIAN PTZ ----
		pi_props->onvif_wian_ptz = pi_props->onvif_handler_args.ptz;


		// ---- 5. set args in model ----
		pi_props->model->SetStreamOnvifHandlerArgs(
				pi_props->onvif_handler_args.hostaddr,
				pi_props->onvif_handler_args.onvifport,
				pi_props->onvif_handler_args.rtspport,
				pi_props->onvif_handler_args.rtsp_stream
			);

	}
	catch (...)
	{
		// yak! problem!
		pi_props->sys_output->Message("     : +++ Caught problem in SetupONVIF +++");
		//return VS_ERROR;
	}


	pi_props->sys_output->MessageSetup("End", "SetupONVIF");

	#else

	pi_props->sys_output->MessageSetup("Setup", "+++ NO SetupONVIF +++");

	#endif

	return VS_OK;
}


// ---------- CleanupONVIF ----------
/*!
\brief
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::CleanupONVIF()
{

	#ifdef VS_CAMERA_03_STREAMING

	// ---- essential: wrap in try/catch ----
	try
	{
		// we are done
		pi_props->onvif_handler->Halt();

		// wait until shutdown
		while (pi_props->onvif_handler->IsRunning())
		{
			// windows specific ...
			SleepEx(50, FALSE);
		}

	}
	catch (...)
	{
		// yak! problem!
		;
	}

	#endif

	return VS_OK;
}

