////////////////////////////////////////////////////////////////////////////////


// ---------- CallbackFile ---------
INT application::CallbackFile(vs_system::TimelineInfo *info)
{
	return VS_OK;
}


// ---------- CallbackFolder ---------
INT application::CallbackFolder(vs_system::TimelineInfo *info)
{

	switch ( pi_app->callback_mode )
	{

		case pi_app->CallbackMode::COUNT_FOLDERS:
			pi_app->num_of_minute_folders++;
			break;

		case pi_app->CallbackMode::REMOVE_FOLDERS:
			{

					pi_app->index_of_current_folder++;

					std::string directory_path = info->archive_path;
					directory_path += info->folder_path;
					directory_path += "/";

					if ( info->folder_depth == vs_system::TimelineInfo::DateTime::MINUTE )
					{
							if ( pi_app->index_of_current_folder < pi_app->index_del_to_minute_folder )
							{

								INT iRC = DeleteDirectory(directory_path, FALSE);

								// ---- report
									if (pi_app->sys_context->GetBool(pi_app->sys_context->DEVELOPMENT))
										std::cout << "   - D " << info->folder_path << " " << std::endl;

								// ---- log
									if ( pi_app->log )
									{
										std::ofstream log_file(pi_app->log_path, std::ios_base::out | std::ios_base::app);
										if ( log_file.is_open() )
										{
											log_file << "   - D " << info->folder_path << " " << std::endl;
											log_file.close();
										}
									}
							}
					}

			}
			break;

		default:
			break;
	}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
