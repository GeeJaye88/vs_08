
////////////////////////////////////////////////////////////////////////////////


// ---------- CallbackDisplayFromView ----------
/*!
\brief Callback display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK

\note invoked from vs AppPanel::Display method.

\note INT handle_app_obj,

    - no idea why this is passsed

	- usage: application *app = (application *)handle_app_obj;

*/
INT application::CallbackDisplayFromView(
		INT handle_app_obj, // unused
		vs_system::AppPanel *panel
	)
{




	// ---- local
		INT result = VS_OK;
		vs_system::PanelContext *panel_context = panel->GetPanelContext();


	// ---- display panel to image ?
		INT r2i_panel_logical_id = panel_context->GetRenderToImageId();
		if ( r2i_panel_logical_id > 0 )
		{
			switch ( r2i_panel_logical_id )
			{
				case 1:
					result = DisplayStream(panel);
					break;
				case 2:
					#ifdef VS_CAMERA_03_SHARED_MEMORY
					result = DisplayShared(panel);
					#endif
					break;

				default:
					break;
			}
		}


	// ---- display panel ?
		//
		// so - pass on ONLY panel gfx context
		//
		else
		{
			review_02::Model *model = pi_props->model;
			INT result = pi_props->model->Display(panel_context, NULL);
		}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// The name of the stream application is set in apptypes.h to
// Observant-Innovations.MiniStream.

#ifdef VS_CAMERA_03_STREAMING
IPC::Buffer<MiniStream::PixelData<>> ipc_buffer(MiniStream::ipc_name);
#endif


// ---------- DisplayStream ----------
/*!
\brief Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::DisplayStream(vs_system::AppPanel *panel)
{

	// ---- NOT streaming ? -- return
	if ( !pi_props->streaming ) return VS_OK;


	// ---- local
		vs_system::PanelContext *panel_context = panel->GetPanelContext();
		review_02::Model *model = pi_props->model;


	// ---- derive index from logical id
		INT r2i_panel_logical_id = panel_context->GetRenderToImageId();
		INT r2i_panel_index = r2i_panel_logical_id - 1;


	// ---- set r2i time/frame sampling status
		BOOL r2i_status = FALSE;
		switch ( pi_props->stream_frame_modulus )
		{
			case 0:
				r2i_status = pi_props->camera_image_captured;     // when an image has been captured
				break;
			case 1:
				r2i_status = TRUE;                                // always
				break;
			default:
				r2i_status =                                      // only when frame % val == 1
					(
						pi_props->frames_total %
						pi_props->stream_frame_modulus
					) == 1 ? TRUE : FALSE;
				break;
		}


	// ---- NOT render_to_image ? -- return
		if ( ! r2i_status ) return VS_OK;


	// ---- local
		INT  result = 0;
		char message[256] = "";


	// ---- legacy
		//
		// left over from the days of DR shared memory
		//
		// ---- enumerate virtual camera status values ----
		//
		// note: from "vs_shared_memory_structs.h"
		//
		enum {
			CAMERA_IDLE = 0,
			CAMERA_ACTIVE = 1
		};


	// ---- virtual camera active status
		USHORT camera_status =
			pi_props->stream_logical_id == r2i_panel_logical_id ?
				CAMERA_ACTIVE :
					CAMERA_IDLE;


	// ---- virtual camera is active ?
	if ( camera_status == CAMERA_ACTIVE )
	{


		#ifdef VS_CAMERA_03_STREAMING

		// ---- onvif OK -- AND -- remote control ON ?
			if ( pi_props->onvif_handler != NULL && pi_props->control->GetRemoteControlFlag() )
			{
				pi_props->MapPtzToVptf();
			}

		#endif


		// ---- initialise render_to_image buffer
			vs_system::ImgBuffer *r2i_buffer = pi_props->render_to_image_buffer[r2i_panel_index];


		// ---- NOT -- render_to_image panel status ?
			BOOL r2i_panel_status = panel_context->GetRenderToImageStatus();
			if ( !r2i_panel_status )
			{

				// ---- initialise r2i panel properties...
					panel_context->SetRenderToImageDimensions(
							pi_props->stream_image_width,
							pi_props->stream_image_height
						);
					panel_context->SetRenderToImageStatus(TRUE);


				// ---- initialise r2i image buffer...
					r2i_buffer->Initialise(
							pi_props->stream_image_width,
							pi_props->stream_image_height,
							3
						);
					r2i_buffer->CreateBuffer();
			}


		// ---- display render_to_image
			model->Display(panel_context, r2i_buffer);


		#ifdef VS_CAMERA_03_STREAMING

		// ---- streaming ?
			if ( pi_props->streaming )
			{

				// ---- wrap in try catch ----
				try
				{

					// ---- get a lock to the IPC buffer

						// note: a maximum timeout is specified for error reporting.
						MiniStream::PixelData<>* pixeldata = ipc_buffer.lock(100);
						if (pixeldata)
						{
							// ensure consumer knows dimensions and depth
							pixeldata->init(pi_props->stream_image_width, pi_props->stream_image_height, 3);

							// get pointer to IPC data
							uint8_t* pd = pixeldata->data();

							// copy image data into IPC system for consumer
							memcpy(pd, r2i_buffer->GetBuffer(), r2i_buffer->GetExtent());

							// Important: unlock the IPC buffer
							ipc_buffer.unlock(pixeldata);

							// signal consumer
							ipc_buffer.signal();
						}
				}
				catch (...)
				{
					;
				}

			}

		#endif


		// ---- increment r2i per second counter
			pi_props->r2i_per_second++;


		// ---- no image captured
			pi_props->camera_image_captured = FALSE;

	}

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- DisplayShared ----------
/*!
\brief Display
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT application::DisplayShared(vs_system::AppPanel *panel)
{

	// ---- NOT sharing ? -- return
		BOOL sharing_status = TRUE;
		if ( !sharing_status ) return VS_OK;


	// ---- NO new image ? -- return
		if ( !pi_props->camera_image_captured ) return VS_OK;


	// ---- local
		INT result = VS_OK;
		vs_system::PanelContext *panel_context = panel->GetPanelContext();
		review_02::Model *model = pi_props->model;


	// ---- report ----
		/*
		INT r2i_panel_logical_id = panel_context->GetRenderToImageId();
		CHAR message[VS_MAXCHARNAME];
		result = panel_context->GetName(message);
		printf("Share %s", message, r2i_panel_logical_id);
		*/


	// ---- shared memory
		vs_system::ImgBuffer *shared_image_buffer = pi_props->shared_image_buffer;


	// ---- one-time shared memeory initialisation
		BOOL r2i_buffers_initialised = panel_context->GetRenderToImageStatus();
		if ( !r2i_buffers_initialised )
		{
		
			pi_props->sys_output->MessageSetup("Begin", "SetupSharedMemeory");


			// ---- calculate pano aspect ratio
				INT   result = VS_OK;
				FLOAT pano_min_angle, pano_max_angle;
				result = model->GetObjConfig()->GetPanoAngles(&pano_min_angle, &pano_max_angle);

			// ---- set
				FLOAT aspect = (pano_max_angle - pano_min_angle) / 360.0f;
				pi_props->shared_image_width =
					(UINT)((FLOAT)pi_props->shared_image_height * aspect);

			// ---- block
				pi_props->shared_image_block_width  = pi_props->shared_image_width;
				pi_props->shared_image_block_height =
						pi_props->shared_image_height /
							pi_props->shared_image_blocks;
				pi_props->shared_image_block_extent =
						pi_props->shared_image_block_width *
							pi_props->shared_image_block_height *
								3;

			// ---- THEN:


			// --- report
				CHAR msg[VS_MAXCHARLEN];
				sprintf(msg, "   : Image Width  %d", (INT)pi_props->shared_image_width);
					pi_props->sys_output->Message(msg);
				sprintf(msg, "           Height %d", (INT)pi_props->shared_image_height);
					pi_props->sys_output->Message(msg);
				sprintf(msg, "   : Block Count  %d", (INT)pi_props->shared_image_blocks);
					pi_props->sys_output->Message(msg);
				sprintf(msg, "           Width  %d", (INT)pi_props->shared_image_block_width);
					pi_props->sys_output->Message(msg);
				sprintf(msg, "           Height %d", (INT)pi_props->shared_image_block_height);
					pi_props->sys_output->Message(msg);
				sprintf(msg, "           Extent %d", (INT)pi_props->shared_image_block_extent);
					pi_props->sys_output->Message(msg);


			// ---- THEN:


			// ---- initialise BIG shared image_buffer
				result = shared_image_buffer->Initialise(
						pi_props->shared_image_width,
						pi_props->shared_image_height,
						3
					);
				result = shared_image_buffer->SetQuality(90);
				result = shared_image_buffer->CreateBuffer();


			// ---- initialise SMALL r2i panel buffer
				panel_context->SetRenderToImageDimensions(
						pi_props->shared_image_block_width,
						pi_props->shared_image_block_height
					);


			// ---- initialise SMALL r2i image buffers
				for (UINT i = 0; i < VS_MAX_RENDER_TO_IMG_BUFFERS; i++)
				{
					result = pi_props->render_to_image_buffer[i]->Initialise(
							pi_props->shared_image_block_width,
							pi_props->shared_image_block_height,
							3
						);
					result = pi_props->render_to_image_buffer[i]->SetQuality(90);
				}


			// ---- zap one-time
				panel_context->SetRenderToImageStatus(TRUE);


			// ---- THEN:


			#ifdef VS_CAMERA_03_SHARED_MEMORY

				try
				{

					// ---- set up
						shared_memory_struct.struct_size =
							sizeof(shared_memory_struct);                // important that size is set
						shared_memory_struct.buffer_count = 1;           // currently unused

						// ---- dimensions, bytes per pixel and format ----
						shared_memory_struct.width  = pi_props->shared_image_width;
						shared_memory_struct.height = pi_props->shared_image_height;
						shared_memory_struct.bpp    = 3;
						shared_memory_struct.format = esmRGB;


					// ---- get a buffer handle
						shared_memory_handle = 0;
						INT ret = sm_init(shared_memory_struct, &shared_memory_handle);
						if (ret != esmOK)
							throw "DisplaySharedMemory: bad sm_init";

						if (!shared_memory_handle)
							throw "DisplaySharedMemory: bad sm_handle";

				}
				catch (const char* e)
				{
					const char* p = e;
					OutputDebugStringA(p);
					OutputDebugStringA("\n");
				}
				catch (...)
				{
					OutputDebugStringA("DisplaySharedMemory: Unknown exception\n");
				}

			#endif
		

			pi_props->sys_output->MessageSetup("End", "SetupSharedMemeory");


		}



	// ---- amplify
		for (UINT block = 0; block < pi_props->shared_image_blocks; block++)
		{

			// ---- reconfig pano rect part - nice
				{
					INT result = VS_OK;
					vs_system::GfxInterface * gfx = pi_props->app_dependency->GetGfxInterface();
					INT element_handle = pi_props->model->GetElementHandle("Shared Hidden");
					FLOAT param[3] = { // of VS_MAX_ELM_FLOAT_PARAMETER
						(FLOAT)block,
						(FLOAT)pi_props->shared_image_blocks,
						(FLOAT)pi_props->shared_pixel_mesh_dim
					};
					result = gfx->SetElementParameters(element_handle, 3, param);
					result = gfx->SetElementConfigured(element_handle, FALSE, VS_ELM_SETUP_GEOMETRY);
				}


			// ---- shortcut
				vs_system::ImgBuffer *r2i_buffer = pi_props->render_to_image_buffer[block];


			// ---- offset
				UCHAR *buffer = shared_image_buffer->GetBuffer();
				buffer += block * pi_props->shared_image_block_extent;
				r2i_buffer->SetBuffer(buffer);


			// ---- display render_to_image
				model->Display(panel_context, r2i_buffer);

		}


	// ---- test
		//#define VS_SHARED_MEMORY_JPEG_TEST
		#ifdef VS_SHARED_MEMORY_JPEG_TEST

		{

			CHAR name[VS_MAXCHARNAME];
				sprintf(name, "%4d.jpg", pi_props->shared_counter++);

			for (INT i = 0; i < 4; i++) name[i] = name[i] == ' ' ? '0' : name[i];

			vs_system::ImgJpegLib img_jpeg_lib;
			img_jpeg_lib.WriteToDisk(shared_image_buffer, name);
			float fred = 2;

		}

		#endif


	#ifdef VS_CAMERA_03_SHARED_MEMORY


	// ---- share
		try
		{
			INT ret = sm_signal(shared_memory_handle);
			if (ret != esmOK)
			{
				DBMSG("ret " << ret);
				throw "DisplaySharedMemory: bad sm_signal";
			}

			UCHAR *pb = shared_image_buffer->GetBuffer();

			int bytes =
				shared_memory_struct.width *
				shared_memory_struct.height *
				shared_memory_struct.bpp;

			int timeout = 10000;

			ret = sm_write(
					shared_memory_handle,
					shared_memory_struct,
					pb,
					bytes,
					timeout
				);
			if (ret < esmOK)
			{
				DBMSG("ret " << ret);
				throw "DisplaySharedMemory: bad sm_write";
			}

			ret = sm_clear(shared_memory_handle);
			if (ret != esmOK)
			{
				DBMSG("ret " << ret);
				throw "DisplaySharedMemory: bad sm_clear";
			}

		}
		catch (const char* e)
		{
			std::cout << (e ? e : "<nullptr>") << std::endl;
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		catch (...)
		{
			std::cout << "DisplaySharedMemory: Unknown exception ...\n";
		}

	#endif


	// ---- increment r2i per second counter 
		pi_props->r2i_per_second++;


	// ---- no image captured
		pi_props->camera_image_captured = FALSE;


	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
