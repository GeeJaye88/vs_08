////////////////////////////////////////////////////////////////////////////////

// ---------- sca1_application.hpp ----------
/*!
\file sca1_application.hpp
\brief 
\author Gareth Edwards
*/


// ---------- NewLog ----------
/*!
\brief Scan
\return INT - ERROR <=0 < VS_OK
*/
INT application::NewLog()
{

	// --- runtime ?
		if ( pi_app->num_of_iteration == 0 )
		{

			// --- get time stamp
				CHAR ts[256];
				GetTimeStamp(ts);


			// ---- start file, and report command line stuff
				sprintf(pi_app->log_path, "%s\\scavenger.log", pi_app->supplied_path);
				std::ofstream log_file(pi_app->log_path, std::ios_base::out);
				if ( log_file.is_open() )
				{
					log_file << "Scavenger Log @ " << ts << std::endl;
					log_file << "Command Line Parameters" << std::endl;
					log_file << "   Path          : " << pi_app->supplied_path << std::endl;
					log_file << "   Minutes-Max   : " << (INT)pi_app->max_num_of_minutes << std::endl;
					log_file << "   Seconds-Pause : " << (INT)pi_app->scan_seconds_pause << std::endl;
					log_file << "   Log           : " << (pi_app->log == 0 ? "OFF" : "ON") << std::endl;
					log_file.close();
				}

		}

	return VS_OK;
}


// ---------- Command ----------
/*!
\brief Scan
\return INT - ERROR <=0 < VS_OK
*/
INT application::Command()
{
	
	// ---- dev ?
		if ( pi_app->sys_context->GetBool(pi_app->sys_context->DEVELOPMENT) )
		{
			pi_app->argc = 5;
			pi_app->argv[0] = "Scavenger.exe";
			if ( GetObservantPath(pi_app->supplied_path) == VS_ERROR )
				return VS_ERROR;
			pi_app->argv[1] = pi_app->supplied_path;
			pi_app->argv[2] = "10";
			pi_app->argv[3] = "20";
			pi_app->argv[4] = "1";
		}
		else
		{
			pi_app->argc = pi_app->app_dependency->GetArgc();
			for (INT i = 0; i < pi_app->argc; i++)
				pi_app->argv[i] = pi_app->app_dependency->GetArgv(i);
		}


	// ---- 1
		if ( pi_app->argc != 5 )
		{
			std::cout << "   - Error: five command line arguments required!" << std::endl;
			return VS_ERROR;
		}

	// ---- 2
		BOOL exists = DirectoryExists(pi_app->argv[1]);
		if ( !exists )
		{
			std::cout << std::endl;
			std::cout << "   - Error: Directory " << pi_app->argv[1] << " does not exist!" << std::endl;
			return VS_ERROR;
		}
		strcpy(pi_app->supplied_path, pi_app->argv[1]);


	// ---- 3
		INT minutes = atoi(pi_app->argv[2]);
		pi_app->max_num_of_minutes = minutes < 10 ? 10 : minutes;


	// ---- 4
		INT pause = atoi(pi_app->argv[3]);
		pi_app->scan_seconds_pause = pause < 10 ? 10 : pause;


	// ---- 5
		INT log = atoi(pi_app->argv[4]);
		pi_app->log = log == 0 ? FALSE : TRUE;

	return VS_OK;
}


// ---------- Scan ----------
/*!
\brief Scan
\return INT - ERROR <=0 < VS_OK
*/
INT application::Scan()
{

	// ---- local
		INT result = VS_OK;


	// ---- archive ?
		if ( GetArchivePath(pi_app->supplied_path, pi_app->archive_path) == VS_ERROR )
			return VS_ERROR;


	// --- create
		pi_app->timeline->SetCallbackOnFolder(NULL);
		pi_app->folders_created = pi_app->timeline->Create(pi_app->archive_path, FALSE, FALSE);


	// ----- ?
		pi_app->create_status = pi_app->timeline->GetCreateStatus();
		if ( pi_app->create_status == VS_OK )
		{

			// ---- init process
				pi_app->timeline->SetCallbackOnFolder(CallbackFolder);
				pi_app->index_of_current_folder = 0;

			// ---- count
				pi_app->num_of_minute_folders = 0;
				pi_app->callback_mode = pi_app->CallbackMode::COUNT_FOLDERS;
				result = pi_app->timeline->Process(pi_app->datetime_minute);

			// ---- remove
				pi_app->index_del_to_minute_folder =
					pi_app->num_of_minute_folders -
						pi_app->max_num_of_minutes;
				pi_app->callback_mode = pi_app->CallbackMode::REMOVE_FOLDERS;
				result = pi_app->timeline->Process(pi_app->datetime_minute);

		}


	// ---- clear
		INT ok = DeleteEmptyDirectories(pi_app->archive_path);
		pi_app->folders_deleted = pi_app->timeline->Clear();


	// ---- report & log
		result = Report();

	return VS_OK;
}


// ---------- Report ---------
INT application::Report()
{

	// --- get time stamp
		CHAR ts[256];
		GetTimeStamp(ts);


	// ---- next...
		++pi_app->num_of_iteration;


	// ---- output
		if ( pi_app->sys_context->GetBool(pi_app->sys_context->DEVELOPMENT) )
		{
			std::cout << pi_app->num_of_iteration << " @ "<< ts << std::endl;
			std::cout << "   folders created : " << pi_app->folders_created       << std::endl;
			if ( pi_app->create_status == VS_OK )
			std::cout << "   # minutes found : " << pi_app->num_of_minute_folders << std::endl;
			else
			std::cout << "   # minutes found : none - archive poorly formed!"     << std::endl;
			std::cout << "   folders deleted : " << pi_app->folders_deleted       << std::endl;
		}


	// ---- log
		LONG minutes_elapsed = GetMinutesElapsed();
		if ( pi_app->log )
		{

			// ---- interval ?
				if (
						minutes_elapsed / (DOUBLE)pi_app->minutes_per_report !=
							pi_app->minutes_elapsed / (DOUBLE)pi_app->minutes_per_report
					)
				{

					// ---- append
						std::ofstream log_file(pi_app->log_path, std::ios_base::out | std::ios_base::app);
						if (log_file.is_open())
						{
							log_file << pi_app->num_of_iteration << " @ " << ts                  << std::endl;
							log_file << "   folders created : " << pi_app->folders_created       << std::endl;
							if (pi_app->create_status == VS_OK)
							log_file << "   # minutes found : " << pi_app->num_of_minute_folders << std::endl;
							else
							log_file << "   # minutes found : none - archive poorly formed!"     << std::endl;
							log_file << "   folders deleted : " << pi_app->folders_deleted       << std::endl;
							log_file.close();
						}
				}

		}


	// ---- store
		pi_app->minutes_elapsed = minutes_elapsed;


	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
