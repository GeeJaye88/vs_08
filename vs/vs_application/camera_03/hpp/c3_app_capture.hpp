
// # seconds to wait before recording
#define SECONDS_WAIT_TO_RECORD 1


#if defined(VS_CAM_JPLY_DVP_SDK) 
#undef SECONDS_WAIT_TO_RECORD
#define SECONDS_WAIT_TO_RECORD 3
#endif


// ---------- CallbackOnCaptureImage ----------
/*!
\brief 
\author Gareth Edwards
\returns void
*/
INT application::CallbackOnCaptureImage(
		WORD   width,
		WORD   height,
		WORD   bytes,
		BYTE  *byte_data,
		INT    timestamp[]
	)
{

	// ---- local
		INT result = VS_OK;
		INT create_save_path_result = VS_OK;


	// ---- create save path stack and filename
		std::string path[IMAGE_ARCHIVE_PATHTOP];
		std::string filename;
		create_save_path_result = CreateSavePathFilename(path, filename, timestamp);


	// ---- report
		result = ReportCaptureImageInfo(timestamp);


	// ---- don't continue conditions
		if ( !create_save_path_result ) return VS_OK;
		if ( pi_props->seconds_elapsed_recording < SECONDS_WAIT_TO_RECORD ) return VS_OK;


	// ---- monitor option
		#if defined(VS_CAM_MONITOR)
		if ( pi_props->monitor_on )
		{
			result = pi_props->MonitorBuffer(
							width,
							height,
							bytes,
							byte_data
						);
			result = pi_props->MonitorAlert();
			result = pi_props->model->UpdateMonitorDisplayList(
							pi_props->monitor_next_display_lut,
							pi_props->monitor_prev_display_lut
						);
		}
		#endif	


	// ---- focus option
		#if defined(VS_CAM_FOCUS)
		{
			vs_system::CamProps *cam_props = NULL;
			INT hr = pi_props->model->GetCamProps(&cam_props);

			INT fo = 0;
			cam_props->GetProperty
				(
					vs_system::CamProps::ANNULAR_FOCUS,
					0,
					vs_system::CamProps::DISPLAY,
					&fo
				);
			pi_props->focus_on = fo == 0 ? FALSE : TRUE;

			if ( pi_props->focus_on )
			{
				result = pi_props->UpdateFocusBarGraph(
								width,
								height,
								bytes,
								byte_data
							);
				pi_props->model->Dbz_UpdateFocusBarGraph();
			}
		}
		#endif


	// ---- process
		{

			// ---- increment IPS counter
				pi_props->images_saved_per_second++;

			// ---- ... and invoke application::ProcessCapturedImage 
				result = ProcessCapturedImage(
								path,
								filename,
								width,
								height,
								bytes,
								byte_data
							);
		}


	#if defined(VS_CAM_JPLY_DVP_SDK)
	// ---- JPLY trigger THEN
		;// pi_props->cam_jply->Trigger();
	#endif


	return VS_OK;
}


// ---------- ReportCaptureImageInfo ----------
/*!
\brief Report acquisition info
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT application::ReportCaptureImageInfo(INT *timestamp)
{

	// ---- local
		INT result = 0;


	// ---- increment counters
		pi_props->images_total_counter++;
		pi_props->images_counter++;


		#if defined (VS_CAM_IDS_VIDEO_SDK)
		DOUBLE now_sec = (DOUBLE)timestamp[5];
		#else
		DOUBLE now_ms = timeGetTime();
		ULONG  now_sec = (LONG)(now_ms / 1000);
		#endif


	// IF -- only dashboard -- THEN --
	if ( pi_props->dashboard_only )
	{

		// ---- temp param store
			FLOAT param[review_02::Model::Dbz_Cam_Index::NUMBER_OF_PARAM];


		// ---- IF total # of seconds changed THEN report
		if ( now_sec != pi_props->seconds_elapsed_to_now )
		{
			
			using namespace review_02;

			// ---- get exposure
				result = pi_camera->GetExposure(&param[Model::Dbz_Cam_Index::EXPOSURE_VAL]);
				result = pi_camera->GetExposureMin(&param[Model::Dbz_Cam_Index::EXPOSURE_MIN]);
				result = pi_camera->GetExposureMax(&param[Model::Dbz_Cam_Index::EXPOSURE_MAX]);
				 

			// ---- get framerate
				FLOAT fps = 0, mode = 0;
				vs_system::CamProps *cam_props = NULL;
				result = pi_props->model->GetCamProps(&cam_props);
				result = cam_props->GetProperty(
						vs_system::CamProps::SETUP,
						0,
						vs_system::CamProps::FRAMERATE_INDEX,
						&mode
					);
				result = cam_props->GetProperty(
						vs_system::CamProps::FRAMERATE,
						(INT)mode,
						vs_system::CamProps::FPS,
						&fps
					);
				param[Model::Dbz_Cam_Index::FRAMERATE_FPS] = fps;
				param[Model::Dbz_Cam_Index::FRAMERATE_FTS] = (FLOAT)pi_props->images_counter;
				param[Model::Dbz_Cam_Index::FRAMERATE_FLS] = (FLOAT)pi_props->images_per_second;
				param[Model::Dbz_Cam_Index::FRAMERATE_SECOND] = (FLOAT)(pi_props->seconds_elapsed_to_now % 60);


			// ---- get gain
				FLOAT gain[4];
				result = pi_camera->GetGain(gain);
				param[Model::Dbz_Cam_Index::GAIN_MASTER]   = gain[0];
				param[Model::Dbz_Cam_Index::GAIN_WB_RED]   = gain[1];
				param[Model::Dbz_Cam_Index::GAIN_WB_GREEN] = gain[2];
				param[Model::Dbz_Cam_Index::GAIN_WB_BLUE]  = gain[3];


			// ---- update
				param[Model::Dbz_Cam_Index::ZERO] = 0;
				result = pi_props->model->UpdateCameraDbz(param);

		}
		else
		{

			// ---- get speed fp and fts
				FLOAT fps = 0, mode = 0;
				vs_system::CamProps *cam_props = NULL;
				result = pi_props->model->GetCamProps(&cam_props);
				result = cam_props->GetProperty(
						vs_system::CamProps::SETUP,
						0,
						vs_system::CamProps::FRAMERATE_INDEX,
						&mode
					);
				result = cam_props->GetProperty(
						vs_system::CamProps::FRAMERATE,
						(INT)mode,
						vs_system::CamProps::FPS,
						&fps
					);

				using namespace review_02;

				param[Model::Dbz_Cam_Index::FRAMERATE_FPS] = fps;
				param[Model::Dbz_Cam_Index::FRAMERATE_FTS] = (FLOAT)pi_props->images_counter;


			// ---- update only dashboard "fts"
				param[Model::Dbz_Cam_Index::ZERO] = 1;
				result = pi_props->model->UpdateCameraDbz(param);
		}

	}


	// IF -- total # of seconds changed -- THEN -- report
	if ( now_sec != pi_props->seconds_elapsed_to_now )
	{

		// IF -- zero recorded -- THEN -- re-initialise
			if ( pi_props->seconds_elapsed_recording == 0 )
			{
				pi_props->images_missed_count = 0;
				pi_props->images_duplicated_count = 0;
			}


		// IF -- NOT waiting -- THEN --
		INT imc = 0;
		if ( pi_props->seconds_elapsed_recording >= SECONDS_WAIT_TO_RECORD )
		{

			// ---- re-initialise # of user event count (per second)
				vs_system::SysContext* sys_context = pi_props->app_dependency->GetSysContext();
				DWORD uec = sys_context->GetDWord(sys_context->MSG_USER_EVENT_COUNT);
				sys_context->SetDWord(sys_context->MSG_USER_EVENT_COUNT, 0);


			// IF -- user events count != ips -- THEN -- increment # images missed
				if ( uec != pi_props->images_counter )
					pi_props->images_missed_count++;


			// --- catch first recorded second fail
				imc = pi_props->images_missed_count - 1;
				imc = imc < 0 ? 0 : imc;


			// ---- report
				result = strcmp(
						pi_props->camera_ms_image_map_1,
						pi_props->camera_ms_image_map_0
					);
				if ( result )
				{
					CHAR msg[VS_MAXCHARLEN];
					sprintf(
							msg,
							"F:%d D:%d I:%d %s",
							(INT)pi_props->images_counter,
							imc ,
							pi_props->images_duplicated_count,
							pi_props->camera_ms_image_map_0
						);
					//pi_props->sys_output->Message(msg);
				}
		}

		// ----update
			pi_props->images_per_second = pi_props->images_counter;


		// ---- re-initialise
			pi_props->frames_per_second = 0;
			pi_props->images_counter = 0;
			pi_props->r2i_per_second = 0;
			pi_props->images_saved_per_second = 0;


		// ---- increment
			pi_props->seconds_elapsed_recording++;


		// ---- copy, then re-initialise ms image map
			memcpy(
					pi_props->camera_ms_image_map_1,
					pi_props->camera_ms_image_map_0,
					60
				);
			memset(pi_props->camera_ms_image_map_0, '-', 60);

	}


	// ---- update
		pi_props->seconds_elapsed_to_now =
			pi_props->seconds_playback_count = (ULONG)now_sec;

	return VS_OK;
}


// ---------- CreateSavePathFilename ----------
/*!
\brief CreateSavePathFilename
\author Gareth Edwards
\return INT - ERROR <= 0 < VS_OK
*/
INT application::CreateSavePathFilename(
		std::string(&path)[6],
		std::string &filename,
		INT timestamp[]
	)
{

	// ---- local
		INT result = VS_OK;


	// ---- path
		CHAR paf[IMAGE_ARCHIVE_PATHLEN];
		result = pi_props->img_arkive->GetFolderPath(paf);


	// ---- path components
		char path_component[IMAGE_ARCHIVE_PATHTOP][IMAGE_ARCHIVE_PATHLEN];
		sprintf(path_component[1], "%4d", timestamp[0]);
		sprintf(path_component[2], "%2d", timestamp[1]);
		sprintf(path_component[3], "%2d", timestamp[2]);
		sprintf(path_component[4], "%2d", timestamp[3]);
		sprintf(path_component[5], "%2d", timestamp[4]);


	// ---- initialise path 
		path[0].assign(paf);
		for (WORD i = 1; i<6; i++)
		{
			for (WORD j = 0; j < IMAGE_ARCHIVE_PATHLEN; j++)
			{
				path_component[i][j] = path_component[i][j] == ' ' ?
					'0' :
						path_component[i][j];
			}
			path[i].assign(path_component[i]);
		}
		path[0] += "/archive/";


	// ---- filename component
		char filename_components[IMAGE_ARCHIVE_MAXCHAR];
		sprintf(
			filename_components,
				"%4d%2d%2d_%2d%2d%2d_%3d.jpg",
				timestamp[0], timestamp[1], timestamp[2],
				timestamp[3], timestamp[4], timestamp[5],
				timestamp[6]
			);


	// ---- initialise filename 
		WORD len_filename = (WORD)strlen(filename_components);
		for (WORD i = 0; i < len_filename; i++)
		{
			filename_components[i] = filename_components[i] == ' ' ?
				'0' :
					filename_components[i];
		}
		filename.assign(filename_components);


	// ---- update image map
		INT i = (INT)((FLOAT)timestamp[6] / 16.66666f);
		if (i == 59) i--;
		pi_props->camera_ms_image_map_0[i]   = '[';
		pi_props->camera_ms_image_map_0[i+1] = ']';


	// ---- filename
		strcpy(pi_props->previous_filename, pi_props->archive_filename);
		strcpy(pi_props->archive_filename, filename.c_str());


	// IF -- duplicate -- THEN -- update image map
		if (!strcmp(pi_props->archive_filename, pi_props->previous_filename))
		{
			pi_props->camera_ms_image_map_0[i]   = '(';
			pi_props->camera_ms_image_map_0[i+1] = ')';
			pi_props->images_duplicated_count++;
			return VS_ERROR;
		}

	return VS_OK;
}


#if defined( VS_CAM_CHRONO )
//#define USE_CHRONO
#include <ctime>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;
#endif


// ---------- ProcessCapturedImage ----------
/*!
\brief .... AND format and then copy into texture atlas
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\notes

	Part 3 below:

	The destination (dst_img_buffer) buffer provides for the managment
	of the most recent image that has been captured.

	If image dimensions change then the external jpeg is updated.

	If image dimensions change then the atlas page is updated.

	The destination (dst_img_buffer) is also used as the target buffer
	for the reformatting of [in] byte_data into the GPU.

*/
INT application::ProcessCapturedImage(
		std::string(&path)[IMAGE_ARCHIVE_PATHTOP],
		std::string &filename,
		WORD w,
		WORD h,
		WORD bpp,
		BYTE *byte_data
	)
{

	// ---- image HAS been captured
		pi_props->camera_image_captured = TRUE;


	// ---- local
		INT result = 0;


	// ---- PART 1: write to image archive
		if ( pi_props->archive_save )
		{
			pi_props->img_arkive->SetQuality(
					pi_props->archive_jpg_compression
				);
			pi_props->img_arkive->Write(
					path,
					filename,
					w,
					h,
					bpp,
					byte_data
				);
		}


	// --- PART 2: dashboard only then don't continue
		if ( pi_props->dashboard_only ) return VS_OK;


	// ---- PART 3: update texture atlas (copy to GPU texture cache)
		{

			// ----  (if required) initialise destination buffer
				vs_system::ImgBuffer *dst_img_buffer = pi_props->dst_img_buffer;
				UINT width  = dst_img_buffer->GetWidth();
				UINT height = dst_img_buffer->GetHeight();
				UINT bytes  = dst_img_buffer->GetBytes();

				if ( width != w || height != h || bytes != 4 )
				{
					result = dst_img_buffer->Initialise(w, h, 4);
					// delete buffer // BUG: not deleted !!!!
					dst_img_buffer->SetBuffer(NULL);
				}



			// ----  (if required) initialise external jpg
				#if defined (VS_CAM_EXTERNAL_JPG)
				UCHAR *d = dst_img_buffer->GetBuffer();
				if ( d == NULL )
				{

					// ---- create destination buffer
						result = dst_img_buffer->CreateBuffer();

					// ---- borrow destination buffer to use as temp (3 bytes per pixel) buffer
						UCHAR *uc = dst_img_buffer->GetBuffer();
						memset((VOID *)uc, 0, w * h * 4);
						dst_img_buffer->Initialise(w, h, 3);
						UINT q = dst_img_buffer->GetQuality();
						dst_img_buffer->SetQuality(50);

					// ---- write temp buffer
						vs_system::ImgJpegLib img_jpeglib;
						result = img_jpeglib.WriteToDisk(
								dst_img_buffer,
								pi_props->ext_tex_map_image_paf
							);

					// ---- restore destination buffer
						dst_img_buffer->Initialise(w, h, 4);
						dst_img_buffer->SetQuality(q);
				}
				#endif


			// ---- (if required) initialise atlas page dimensions
				//   invoke -> Model::PanoAtlasPageSetup
				//         -> GfxInterface::PageSetup
				//            -> GfxAtlas::SetupPage
				//               -> GfxPage::Setup
				std::string temp = pi_props->ext_tex_map_image_paf;
				INT atlas_page_handle = pi_props->model->PanoAtlasPageSetup(
						temp,
						dst_img_buffer
					);


			// ---- (if ok) initialise atlas page
				if ( atlas_page_handle != VS_ERROR )
				{

					//  ---- IF LOCK atlas page succeeds THEN
						UCHAR *atlas_buffer = pi_props->model->PanoAtlasPageLock(atlas_page_handle);
						if ( atlas_buffer != NULL )
						{

							// ---- store destination buffer
								BYTE *dst_buffer = dst_img_buffer->GetBuffer();


							// ---- set atlas == destination
								result = dst_img_buffer->SetBuffer(atlas_buffer);


							// ---- clock (see #include <ctime> above)
								#if defined( VS_CAM_CHRONO )
									#if defined (USE_CHRONO)
										auto t1 = Clock::now();
									#else
										std::clock_t start;
										double duration;
										start = std::clock();
									#endif
								#endif


							// ---- process source buffer into destination (atlas) buffer 
								//#define OLD_PRE_PROCESS
								#if defined (OLD_PRE_PROCESS)
									result = pi_props->PreGPUProcess(w, h, bpp, byte_data);
								#else


								// ---- fastest
									UINT total = h * w;
									BYTE *src_data = byte_data;
									BYTE *dest_data = dst_img_buffer->GetBuffer();
									for (UINT t = 0; t<total; t++)
									{
										// note: memcpy(dest_data, src_data, 3); is slower
										*(dest_data)     = *(src_data);
										*(dest_data + 1) = *(src_data + 1);
										*(dest_data + 2) = *(src_data + 2);
										*(dest_data + 3) = 0;
										dest_data += 4;
										src_data  += 3;
									}

								#endif


							// ---- clock
								#if defined( VS_CAM_CHRONO )
									CHAR msg[64];
									#if defined(USE_CHRONO)
										auto t2 = Clock::now();
										auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
										sprintf(msg, "+++ duration %f nanoseconds\n", (FLOAT)duration);
									#else
										duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
										sprintf(msg,"+++ duration %6.4f seconds\n", duration);
									#endif
									OutputDebugString(msg);
								#endif


							// ---- UNLOCK atlas texture buffer
								result = pi_props->model->PanoAtlasPageUnlock(atlas_page_handle);


							// ---- restore destination (dst_img_buffer's) buffer
								result = dst_img_buffer->SetBuffer(dst_buffer);


							// IF -- camera has been started -- THEN --
							//
							//    NOTE: This might be removed!!!!
							//
								if ( pi_props->camera_started )
								{

									// ---- set aspect ratio ----
										pi_props->model->GetObjConfig()->SetPanoAspectRatio((FLOAT)w, (FLOAT)h);


									// ---- camera has NOT been started
										pi_props->camera_started = FALSE;


									// Note: as source image parameters (such as aspect ratio) might differ
									//       from camera default configuration re-create all texture mapping
										INT result = pi_props->model->SetConfigureAllBitmask(VS_ELM_SETUP_MAPPING);

										// ---- report
										pi_props->sys_output->Message("Gfx: Set CONFIG model bitmasks to update ALL mapping.");
										pi_props->sys_output->Message("End:-----------------------------------------------------------------------");

								}

					}

			}

	}

	return VS_ERROR;
}

