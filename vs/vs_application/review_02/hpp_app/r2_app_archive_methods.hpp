////////////////////////////////////////////////////////////////////////////////

// ---------- r2_empty.h ----------
/*!
\file r2_empty.h
\brief
\author Gareth Edwards
*/


#pragma once

#include "../header/r2_model.h"

using namespace review_02;


// ---------- ArchiveQueue ----------
/*!
\brief Queue an image to be read AND read an image
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::ArchiveQueue()
{

	// ---- queue "filename"
		#ifdef USE_IMG_ARKIVE
		int queue_result = img_arkive->Read(
				pi_props->archive_queue_index,
				pi_props->archive_queue_filename
			);
		#else
		int queue_result = img_archive->QueueImage(
				pi_props->archive_queue_index,
				pi_props->archive_queue_filename
			);
		#endif


	// ---- image queued ?
		if ( queue_result == VS_OK )
		{
			pi_props->archive_queue_index++;
		}  
		else
		{
			CHAR text[256];
			sprintf(text, "Archive: Retry %d!", pi_props->archive_queue_index);
			pi_props->sys_output->Message(text);
		}

	return queue_result;
}


// ---------- ArchiveRead ----------
/*!
\brief Queue an image to be read AND read an image
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::ArchiveRead()
{

	// ---- read ready status
		std::string filename;
		#ifdef USE_IMG_ARKIVE
			BOOL read_is_ready = img_arkive->Read_IsReady(
					pi_props->archive_read_index,
					filename,
					img_buffer
				);
		#else
			BOOL read_is_ready = img_archive->IsReadImageReady(
					pi_props->archive_read_index,
					filename,
					img_buffer
				);
		#endif


	// ---- ready ?
	if ( read_is_ready )
	{

		// ---- setup pano atlas page
			INT handle_of_atlas_page = model->PanoAtlasPageSetup(filename, img_buffer);


		// ---- atlas page ok ?
			if ( handle_of_atlas_page != VS_ERROR )
			{

				// ---- copy archive image to atlas cache
					INT lock_result = img_buffer->SetBuffer(
								model->PanoAtlasPageLock(handle_of_atlas_page)
							);
					#ifdef USE_IMG_ARKIVE
					INT copy_result = img_arkive->Read_AccessBuffer(
								pi_props->archive_read_index,
								img_buffer
								#if VS_LIDAR
								,
								csv_buffer
								#endif
							);
					#else
					INT copy_result = img_archive->ReadImage(
								pi_props->archive_read_index,
								img_buffer
							);
					#endif
					INT unlock_result = model->PanoAtlasPageUnlock(handle_of_atlas_page);


				// ---- image copied into atlas cache ?
					if ( copy_result == VS_OK )
					{

						// ---- local
							INT width  = img_buffer->GetWidth();
							INT height = img_buffer->GetHeight();


						// ---- update
							pi_props->archive_read_filename = filename;
							pi_props->archive_read_index++;


						// ---- create 2D pano image
							if ( pi_props->create_pano_image )
							{
								pi_panophoto_data.button_on = FALSE;

								INT pano_result = CreatePanophotoThread(img_buffer);
								pi_props->create_pano_image = FALSE;
							}


						// IF -- different dimension -- THEN -- set update config flag, and....
							if ( width != pi_props->archive_last_image_width ||
								 height != pi_props->archive_last_image_height )
							{
								pi_props->update_config_bitmask = TRUE;
								pi_props->sys_output->Message("Image: Detected change of aspect ratio.");
							}


						// ---- update dimension
							pi_props->archive_last_image_width = width;
							pi_props->archive_last_image_height = height;


						// ---- flag system
							INT result = pi_props->sys_context->SetDWord(vs_system::SysContext::WINDOW_TRIGGER, 1);


						// ---- update_config_bitmask ?
							if ( pi_props->update_config_bitmask )
							{

								// ---- set panoramic source image aspect ratio
									INT result = model->GetObjConfig()->SetPanoAspectRatio((FLOAT)height / (FLOAT)width);


								// ---- set flag FALSE
									pi_props->update_config_bitmask = FALSE;


								// ---- as source image parameters (such as aspect ratio) might differ
								// from camera default configuration re-create all texture mapping
									result = model->SetConfigureAllBitmask(VS_ELM_SETUP_MAPPING);


								// ---- set ALL elements to be configured
									result =
										pi_props->app_dependency->
											GetGfxInterface()->
												SetGlobalConfigureBitmask(
													VS_ELM_SETUP_GEOMETRY | VS_ELM_SETUP_MAPPING,
													""
												);

								// ---- reset ?
									if ( result == VS_OK )
									{

										//  ---- frameset panels reset ?
											result = model->SetupPanels(pi_props->app_dependency, app_view);
											if ( result )
											{
												// update and reset succeeded
												;
											}
									}
							}


						// ---- lidar ?
							#if VS_LIDAR
							model->Update_PanoLidar(csv_buffer);
							#endif

						return VS_OK;
					}
			}
	}

	return VS_ERROR;
}


// ---------- ArchiveUpdate ----------
/*!
\brief Update archive every minhute AND last minute every # recreate millisecond interval
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::ArchiveUpdate()
{

	// Set "IS"
	BOOL timeline_is_available    = img_timeline->IsTimelineAvailable();
	BOOL timeline_is_ready        = img_timeline->IsTimelineCreated() && timeline_is_available;
	BOOL this_minute_is_available = img_timeline->IsThisMinuteAvailable();

	// IF timeline NOT available THEN return ERROR
	if ( !timeline_is_ready )
	{
		pi_props->sys_output->Message("Archive: \"LIVE\" timeline NOT ready! Retry...");
		return VS_ERROR;
	}

	// IF "this" minute NOT available THEN return ERROR
	if ( !this_minute_is_available )
	{
		pi_props->sys_output->Message("Archive: \"LIVE\" \"this\" minute NOT available! Retry...");
		return VS_ERROR;
	}

	//  --------- Time ---------

	// Millisecond intervals
	DOUBLE archive_ms_time_interval = 400;
	DOUBLE archive_ms_time_recreated_interval = 59 * 1000 + 750;

	// Millisecond timeouts
	DOUBLE archive_ms_folder_time_out = 61 * 1000;
	DOUBLE archive_ms_image_time_out = 5 * 1000;

	// Now 
	pi_props->archive_ms_time_now = timeGetTime();

	// Elapsed since last scanned/remapped
	DOUBLE archive_ms_time_elapsed =
		pi_props->archive_ms_time_now -
		pi_props->archive_ms_time_last;

	// Elapsed since archive recreated - because folder count had changed
	DOUBLE archive_ms_time_since_last_recreated =
		pi_props->archive_ms_time_now -
		pi_props->archive_ms_time_folder_count_changed;

	// IF update starting THEN prevent requirement for comparison tolerance move 
	if ( ! pi_props->archive_update_start )
	{
		archive_ms_time_since_last_recreated = archive_ms_time_recreated_interval + 1;
	}

	// IF archive is updating THEN
	if ( pi_props->archive_updating )
	{

		// IF elapsed time > # millisecond interval THEN
		if ( archive_ms_time_elapsed > archive_ms_time_interval )
		{

			// --------- RECREATE "archive" ---------

			// IF elapsed time since recreated > # millisecond recreated interval THEN
			if ( archive_ms_time_since_last_recreated > archive_ms_time_recreated_interval )
			{
				// Blimey... wot a palaver
				CHAR temp[VS_MAXCHARPATHLEN];
				strcpy(temp, pi_props->archive_paf);
				strcat(temp, "archive/");

				// Get current # folders
				UINT current_folder_count;
				INT result = img_timeline->GetTotalFolders(&current_folder_count);

				// IF started THEN report scan archive to count # folders
				CHAR msg[VS_MAXCHARLEN];
				if ( pi_props->archive_update_start )
				{
					sprintf(
						msg,
						"Archive: \"LIVE\" scaning."
						);
					pi_props->sys_output->Message(msg);
				}
				else
				{
					// IF syncing THEN report ONCE
					if ( pi_props->archive_sync_message )
					{
						sprintf(
							msg,
							"Archive: \"LIVE\" syncing."
							);
						pi_props->sys_output->Message(msg);
						pi_props->archive_sync_message = FALSE;
					}
				}

				UINT scanned_folder_count = img_timeline->Scan(temp);

				// IF # scanned > # created THEN re-create
				if ( scanned_folder_count > current_folder_count )
				{
					// Report
					sprintf(
						msg,
						"Archive: \"LIVE\" refreshed (# folders:%d).",
						scanned_folder_count
						);
					pi_props->sys_output->Message(msg);

					// IF archive recreated THEN set time when folder count changed
					INT result = img_timeline->RefreshCreate(temp);
					if ( result )
					{
						// Set time when folder count changed
						pi_props->archive_ms_time_image_count_changed = 
							pi_props->archive_ms_time_folder_count_changed =
								pi_props->archive_ms_time_now;

						// Start (interval testing)
						pi_props->archive_update_start = TRUE;

					}

				}   // --- IF # scanned > # created THEN re-create
			
				// ELSE 
				else
				{
					// IF elapsed time > timeout since folder last changed THEN stop updating
					if (
						pi_props->archive_ms_time_now -
						pi_props->archive_ms_time_folder_count_changed >
						archive_ms_folder_time_out
						)
					{
						// STOP archive updating
						pi_props->archive_updating = FALSE;

						// Report timeout
						CHAR msg[VS_MAXCHARLEN];
						sprintf(
							msg,
							"Archive: \"LIVE\" folder timeout after %0.1f seconds.",
							(FLOAT)(archive_ms_folder_time_out / 1000)
							);
						pi_props->sys_output->Message(msg);
					}

				}   // --- ELSE IF elapsed time > timeout

			}   // --- IF elapsed time since recreated > # millisecond recreated interval THEN


			// --------- REFRESH "last" minute ---------

			// IF "last" minute THEN
			if ( img_timeline->IsLastMinute() )
			{
				// Local
				INT result = VS_OK;

				// Get current image count
				UINT current_image_count = 0;
				result = img_timeline->GetLastMinteTotalImages(&current_image_count);

				// Refresh last minute
				result = (UINT)img_timeline->RefreshLastMinute();

				// Get new image count
				UINT new_image_count = 0;
				result = img_timeline->GetLastMinteTotalImages(&new_image_count);

				// IF # new > # current THEN
				if ( new_image_count > current_image_count )
				{
					// Set time image count changed
					pi_props->archive_ms_time_image_count_changed = pi_props->archive_ms_time_now;

					// Set "seconds changed" flag
					pi_props->second_changed = TRUE;
				}

			}   // --- IF "last" minute THEN


			// --------- TIMEOUT ---------
			{
				// IF time elapsed > timeout since image count last changed THEN stop updating
				DOUBLE archive_ms_image_time_elapsed =
					pi_props->archive_ms_time_now -
					pi_props->archive_ms_time_image_count_changed;
				if ( archive_ms_image_time_elapsed > archive_ms_image_time_out )
				{
					// STOP archive updating
					pi_props->archive_updating = FALSE;

					// Report timeout
					CHAR msg[VS_MAXCHARLEN];
					sprintf(
						msg,
						"Archive: \"LIVE\" image timeout after %0.1f seconds.",
						(FLOAT)(archive_ms_image_time_out / 1000)
						);
					pi_props->sys_output->Message("Archive: \"LIVE\" image timeout.");

				}
			}


			// --------- Set time LAST == NOW ---------
			pi_props->archive_ms_time_last = pi_props->archive_ms_time_now;


		}   // --- IF elapsed time > # millisecond interval THEN

	}   // --- IF archive is updating THEN


	// --------- REFRESH "last" minute ---------

	// IF archive is NOT updating THEN
	if ( ! pi_props->archive_updating )
	{
		// IF elapsed time > # millisecond interval THEN
		if ( archive_ms_time_elapsed > archive_ms_time_interval )
		{

			// IF "last" minute THEN
			if ( img_timeline->IsLastMinute() )
			{
				// Local
				INT result = VS_OK;

				// Get current image count
				UINT current_image_count = 0;
				result = img_timeline->GetLastMinteTotalImages(&current_image_count);

				// Refresh last minute
				result = (UINT)img_timeline->RefreshLastMinute();

				// Get new image count
				UINT new_image_count = 0;
				result = img_timeline->GetLastMinteTotalImages(&new_image_count);

				// IF # new > # current THEN
				if (new_image_count > current_image_count)
				{
					// Reset archive time
					pi_props->archive_ms_time_begin =
						pi_props->archive_ms_time_folder_count_changed =
						pi_props->archive_ms_time_image_count_changed =
						pi_props->archive_ms_time_last =
						pi_props->archive_ms_time_now = pi_props->archive_ms_time_now;

					// Start archive updating again
					pi_props->archive_updating = TRUE;       // Do want to start "live" updating
					pi_props->archive_update_start = FALSE;  // Don't want to start interval testing
					pi_props->archive_sync_message = TRUE;   // Do want first sync message

					// Set "seconds changed" flag
					pi_props->second_changed = TRUE;

					// Report resyncing
					CHAR msg[VS_MAXCHARLEN];
					sprintf(
						msg,
						"Archive: \"LIVE\" resyncing (%f)!!!",
						(FLOAT)(archive_ms_image_time_out / 1000)
						);
					pi_props->sys_output->Message(msg);

				}
			}
		}
	}

	// Done
	return VS_OK;
}


// ---------- GetArchivePath ----------
/*!
\brief GetArchivePath
\author Gareth Edwards

\note: Filter examples:
ofn.lpstrFilter = TEXT("Playlists: m3u.\0*.m3u\0Music Files :mp1, mp2, mp3, wav, ogg.\0*.mp1;*.mp2;*.mp3;*.wav;*.ogg\0");
ofn.lpstrFilter = TEXT("All\0*.*\0Text\0*.TXT\0");
ofn.lpstrFilter = TEXT("Images:jpg\0");

\note:
GetWindowsDirectory(ipf,512);
HRESULT result = SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, NULL, SHGFP_TYPE_CURRENT, ipf); 

*/
INT application::GetArchivePath(CHAR* archive_path, UINT max_path_len)
{

	// ---- local
		vs_system::SysContext *sys_context = pi_props->sys_context;
		CHAR msg[VS_MAXCHARLEN];


	// ---- buffer for current directory path
		CHAR current_dir[VS_MAXCHARPATHLEN];
		GetCurrentDirectory(512,current_dir);


	// ---- buffer for selected path/filename and match filename
		CHAR selected_filename[VS_MAXCHARLEN];
		CHAR selected_path[VS_MAXCHARPATHLEN];


	// ---- setup common dialog box structure.
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize     = sizeof(ofn);
		//ofn.hwndOwner     = g_hWnd;
		ofn.lpstrFile       = selected_path;
		ofn.lpstrFile[0]    = '\0';
		ofn.nMaxFile        = sizeof(selected_path);
		ofn.nFilterIndex    = 1;
		ofn.lpstrFileTitle  = selected_filename;
		ofn.nMaxFileTitle   = sizeof(selected_filename);
		ofn.lpstrInitialDir = current_dir;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


	// ---- set specific browse parameters
		ofn.lpstrTitle = TEXT("Select archive configuration file");
		ofn.lpstrFilter = TEXT("Archive file: hdl.\0*.hdl;\0");


	// ---- block application
		sys_context->SetBool(sys_context->BLOCKED, TRUE);
		sprintf(msg, "BLOCKED (0): %d.", sys_context->GetBool(sys_context->BLOCKED) );
		pi_props->sys_output->Message(msg);


	// ---- dialog box success ?
		HANDLE handle_file = NULL;
		if ( GetOpenFileName(&ofn)==TRUE )
		{
			// Test file exists....
			handle_file = CreateFile(ofn.lpstrFile,
				GENERIC_READ,
				0,
				(LPSECURITY_ATTRIBUTES) NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				(HANDLE) NULL);
		}


	// ---- NOT selected ?
		if ( handle_file == NULL )
		{
			// unblock application
			sys_context->SetBool(sys_context->BLOCKED, FALSE);
			sprintf(msg, "BLOCKED (a): %d.", sys_context->GetBool(sys_context->BLOCKED));
			pi_props->sys_output->Message(msg);
			return VS_ERROR;
		}


	// ---- close handle
		CloseHandle(handle_file);


	// ---- selected_filename NOT "archive.hdl" ?
		//
		// TBD String 'selected_filename' might not be zero-terminated.
		// 
		if ( strcmp(selected_filename, "archive.hdl") != 0 )
		{
			// unblock application
			sys_context->SetBool(sys_context->BLOCKED, FALSE);
			sprintf(msg, "BLOCKED (b): %d.", sys_context->GetBool(sys_context->BLOCKED));
			pi_props->sys_output->Message(msg);
			return VS_ERROR;
		}


	// ---- replace '\' with '/'
		UINT len_paf = strlen(selected_path);
		for ( UINT i=0; i<len_paf; i++ )
		{
			selected_path[i] = selected_path[i] == '\\' ?
				 '/' :
				 selected_path[i];
		}


	// ---- snip off selected_filename
		//
		// TBD Buffer overrun while writing to 'selected_path':  the writable
		// size is '512' bytes, but '-11' bytes might be written.
		// 
		UINT len_ini = strlen("archive.hdl");
		selected_path[len_paf - len_ini] = 0;
		strcpy(archive_path, selected_path);


	// ---- unblock application
		sys_context->SetBool(sys_context->BLOCKED, FALSE);
		sprintf(msg, "BLOCKED (c): %d.", sys_context->GetBool(sys_context->BLOCKED));
		pi_props->sys_output->Message(msg);

	return VS_OK;
}


// ---------- GetRunTimeArchivePath ----------
/*!
\brief GetRunTimeArchivePath
\author Gareth Edwards
*/
INT application::GetRunTimeArchivePath(CHAR *archive_folder, UINT max_path_len)
{

	// ---- local
	CHAR setup_config_paf[VS_MAXCHARPATHLEN];     // Application "capture" config file path
	BOOL found_folder = FALSE;                    // Flag TRUE IF found default or "capture" folder


	// ---- config files ----
	ConfigFiles config_files;


	// ---- initialise application setup configuration file path
	sprintf(
			setup_config_paf,
			"%sdata/%s/%s",
			pi_props->sys_context->GetChar(pi_props->sys_context->APP_FOLDER_PATH),
			pi_props->sys_context->GetChar(pi_props->sys_context->CONFIG_NAME),
			"app_config.hdl"
		);


	// ---- IF -- [folder] application configuration file exists -- THEN
	//
	//    IF "Archive" THEN
	//       IF "Folder" THEN
	//          IF specified folder exists THEN
	//             use this folder
	//
	DWORD file_handle_path = GetFileAttributesA(setup_config_paf);
	if ( file_handle_path != INVALID_FILE_ATTRIBUTES )
	{

		vs_system::VNodeIO vio;
		vs_system::VNode *config_hdl = vio.Read(setup_config_paf);

		// IF -- source file -- NOT READ -- THEN -- report && return
		if ( config_hdl == NULL )
		{
			pi_props->sys_output->MessageError(
					"Archive: Failed to find\n"
					"application setup configuration file.",
					"Config File",
					setup_config_paf
				);
			return VS_ERROR;
		}
		// ELSE -- parse
		else
		{
			// ---- archive
			pi_props->sys_output->Message("Archive:");

			vs_system::VNode *p = NULL;
			vs_system::VNode *c = NULL;
			if ( c = config_hdl->SearchName("Archive") )
			{
				if ( p = c->SearchName("Folder") )
				{
					// IF -- "capture" folder path -- THEN
					strcpy(pi_props->app_archive_paf, p->GetValue());
					INT len = strlen(pi_props->app_archive_paf);
					if ( len > 2 ) // e.g. gt "k:", must be "k:/"
					{
						for (INT i = 0; i < len; i++)
						{
							pi_props->app_archive_paf[i] =
									pi_props->app_archive_paf[i] == '\\' ?
										'/' : pi_props->app_archive_paf[i];
						}
						DWORD file_handle_path = GetFileAttributesA(pi_props->app_archive_paf);
						if ( file_handle_path != INVALID_FILE_ATTRIBUTES )
						{
							found_folder = TRUE;
							pi_props->sys_output->Message("   Using specified Image Archive parent folder.");
						}
					}
				}
			}
		}
	}


	// IF -- NOT [folder] application configuration file--  THEN
	//
	//    Default to [user]/Documents/Observant folder
	//
	//    Note: Created by [folder] application at "runtime"!
	//
	if ( !found_folder )
	{

		// ---- initialise "default" path folder path
		sprintf(
				pi_props->app_archive_paf,
				"%sDocuments/Observant/",
				pi_props->sys_context->GetChar(pi_props->sys_context->PROJECT_FOLDER_PATH)
			);


		// IF -- "default" folder path -- THEN
		file_handle_path = GetFileAttributesA(pi_props->app_archive_paf);
		if ( file_handle_path != INVALID_FILE_ATTRIBUTES )
		{
			found_folder = TRUE;
			pi_props->sys_output->Message("   Using default Image Archive parent folder.");
		}
	}


	// IF -- found either defaut folder -- OR -- specified folder -- THEN
	//
	//    Load latest archive...
	//
	if ( found_folder )
	{

		// ---- set current directory
		BOOL set_directory = SetCurrentDirectory(pi_props->app_archive_paf);


		// ---- append "*.*"
		static CHAR capture_paf[VS_MAXCHARPATHLEN];
		sprintf(
				capture_paf,
				"%s*.*",
				pi_props->app_archive_paf
			);


		// ---- report
		pi_props->sys_output->Message("   Scan for last Image Archive folder...");


		// IF -- valid path -- AND -- at least ONE folder -- THEN -- loop through all folders
		HANDLE hFind;
		WIN32_FIND_DATA data;
		BOOL found_archive = FALSE;
		CHAR archive_paf[VS_MAXCHARPATHLEN];
		//static CHAR archive_folder[VS_MAXCHARPATHLEN];
		hFind = FindFirstFile(capture_paf, &data); // "c:\\*.*", etc.
		if ( hFind != INVALID_HANDLE_VALUE ) {
			do {

				// ---- append "archive.hdl"
				sprintf(
						archive_paf,
						"%s%s/archive.hdl",
						pi_props->app_archive_paf,
						data.cFileName
					);


				// IF -- "[folder path]archive.hdl" -- THEN
				file_handle_path = GetFileAttributesA(archive_paf);
				if ( file_handle_path != INVALID_FILE_ATTRIBUTES )
				{
					sprintf(
							archive_folder,
							"     -> %s",
							data.cFileName
						);
					pi_props->sys_output->Message(archive_folder);

					sprintf(
							archive_folder,
							"%s%s/",
							pi_props->app_archive_paf,
							data.cFileName
						);
					found_archive = TRUE;
				}

			} while ( FindNextFile(hFind, &data) );
			

			// ---- close & done
			FindClose(hFind);
			

			// IF -- found archive -- THEN
			if ( found_archive )
			{
				CHAR message[VS_MAXCHARPATHLEN];
				sprintf(
						message,
						"   Using Image Archive %s",
						archive_folder
					);
				pi_props->sys_output->Message(message);
				return VS_OK;
			}
		}
	}


	// ---- report failure
	{
		pi_props->sys_output->Message("Archive: Warning: Failed to find Image Archive parent folder!");

		INT result = pi_props->sys_output->MessageQuery(
				"Archive:\n\n"
				"Failed to find an Image Archive parent folder!\n\n"
				"When RUN the Review application attempts to load "
				"the last Image Archive created in parent folder:\n\n"
				"[USER]/Documents/Observant\n\n"
				"To load an Image Archive, select the button "
				"at bottom left to open a file explorer window, then"
				"navigate to a Image Archive folder and select"
				"the \"archive.hdl\" file.\n\n"
				"Do you want to continue?"
			);
		if ( result == IDNO ) exit(VS_ERROR);
	}


	return VS_ERROR;
}


