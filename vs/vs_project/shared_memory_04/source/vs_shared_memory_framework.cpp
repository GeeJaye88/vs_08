////////////////////////////////////////////////////////////////////////////////

// ---------- vs_shared_framework.cpp ----------
/*!
\file vs_shared_framework.cpp
\brief Implementation of the shared memory API framework class.
\copyright Copyright 2015 Observant Technology Limited. All rights reserved.
\license This project is released under NDA.
\author Gareth Edwards

*/

// ---- definitions ----
#define _CRT_SECURE_NO_WARNINGS 1


// ---- include ----
#include "../header/vs_shared_memory_framework.h"


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Constructor ----------
/*!
\brief Constructor
\author Gareth Edwards
\note
*/
vs_system::Shared_Memory_Framework::Shared_Memory_Framework()
{
	// Application dependency pointers
	app_dependency        = NULL;
	sys_context           = NULL;
	sys_output            = NULL;

	// Application
	app_name[0]           = 0;

	// Virtual Camera logical id
	vrt_cam_logical_id    = 0;
	vrt_cam_index         = 0;
	vrt_cam_total_updated = 0;

	// Shared memory mapping
	mem_usage_mode        = 'S';
	strcpy(mem_name, "ObservantXXXXXX");
	mem_handle_map_file   = NULL;
	mem_buffer            = NULL;
	mem_buffer_size       = 0;
	mem_process_id        = 1;
	mem_camera_heartbeat  = 0;

	// Time
	ms_start_time         = (double)timeGetTime();
	ms_total_time         = 0;
	ss_now                = (ULONG)(ms_start_time / 1000);
	ss_ips                = 0;
}


// ---------- Destructor ----------
/*!
\brief Constructor
\author Gareth Edwards
*/
vs_system::Shared_Memory_Framework::~Shared_Memory_Framework()
{
	;
}


////////////////////////////////////////////////////////////////////////////////

// =========== Framework Shared Memory  ===========


// ---------- Setup ----------
/*!
\brief Setup shared memory
\author Gareth Edwards
\param INT [in] number of arguments
\param CHAR* [in] argument list
\return INT - ERROR <=0 < VS_OK
*/
INT Shared_Memory_Framework::Setup()
{
	// Local
	INT  result = 0;
	CHAR message[VS_MAXCHARLEN];
	BOOL opened = FALSE;


	// ============================== OPEN - ALREADY CREATED ==============================

	// IF open THEN exists SO....
	if ( result = vs_system::VS_OpenSharedMemory(
				mem_name,
				mem_buffer_size,
				&mem_buffer,
				&mem_handle_map_file
			) == VS_OK
		)
	{
		// Flag
		opened = TRUE;

		// Get size
		mem_buffer_size = vs_system::VS_GetSizeOfSharedMemory(mem_buffer);
		sprintf(
				message,
				"SHARED MEMORY: %s - OPEN succeeded with %d bytes of %s.",
				app_name,
				mem_buffer_size,
				mem_name
			);
			#ifdef NOT_VS
			printf(message);
			printf("\n");
			#else
			sys_output->Message(message);
			#endif
	}


	// ============================== CREATE ==============================

	else
	{
		// Note: The following functions must ONLY be invoked once
		// by the process initialising and then creating shared memory
		// as they share THE initialisation scratchpad. 
		//
		// VS_InitialiseSharedMemoryAccess
		// VS_InitialiseVirtualCamera
		// VS_InitialiseGetSizeOfSharedMemory
		// VS_InitialiseSharedMemory
		//

		// Initialise shared memory access
		/*
		\param CHAR * - [in] Unique name of shared memory
		\param ULONG  - [in] Used to identity "owner" of virtual camera
		*/
		result = vs_system::VS_InitialiseSharedMemoryAccess(
				mem_name,                   // unique
				mem_process_id,             // process id
				VS_MAX_IMAGE_WIDTH,         // maximum width
				VS_MAX_IMAGE_HEIGHT         // maximum height
			);

		// Initialise ALL virtual cameras
		/*
		\param USHORT - [in] Index (0 <= logical_id < VS_MAX_LOGICAL_CAMERAS)
		\param USHORT - [in] Identity, which must be non - zero and unique
		\param USHORT - [in] Enumerated IDLE, ACTIVE, CLOSING_DOWN, etc.
		\param ULONG  - [in] Used to identity "owner" of virtual camera
		\param FLOAT  - [in] X lookat proportional to 2D panorama width(0.0 <= X <= 1.0)
		\param FLOAT  - [in] Y lookat proportional to 2D panorama height(0.0 <= Y <= 1.0)
		\param FLOAT  - [in] Vertical Field Of View(FOV)
		\param USHORT - [in] X maximum width of image
		\param USHORT - [in] Y maximum height of image
		*/
		for ( UINT i = 0; i < VS_MAX_LOGICAL_CAMERAS; i++ )
		{
			result = vs_system::VS_InitialiseVirtualCamera(
					i,                          // index
					i + 1,                      // logical id
					vs_system::CAMERA_IDLE,     // camera_status
					mem_process_id,             // process_id
					0,                          // x_lookat
					0,                          // y_lookat
					40.0,                       // vertical_fov
					VS_MAX_IMAGE_WIDTH,         // width
					VS_MAX_IMAGE_HEIGHT         // height
				);
		}

		// Get size of required shared memory
		mem_buffer_size = vs_system::VS_InitialiseGetSizeOfSharedMemory();

		// If create ok THEN
		if ( result = vs_system::VS_CreateSharedMemory(
					mem_name,
					mem_buffer_size,
					&mem_buffer,
					&mem_handle_map_file
				) == VS_OK
			)
		{
			// Initialise shared memory
			result = vs_system::VS_InitialiseSharedMemory(mem_buffer);

			// Report
			sprintf(
					message,
					"SHARED MEMORY: %s - CREATE succeeded with %d bytes of %s named memory.",
					app_name,
					mem_buffer_size,
					mem_name
				);
				#ifdef NOT_VS
				printf(message);
				printf("\n");
				#else
				sys_output->Message(message);
				#endif
		}

		// ============================== OOOPS - FAILED ==============================

		// ELSE if create fails THEN report and exit
		else
		{
			// Report & return error
			sprintf(
					message,
					"SHARED MEMORY: %s - EXITING because OPEN AND CREATE both failed.",
					app_name
				);
				#ifdef NOT_VS
				printf(message);
				printf("\n");
				#else
				sys_output->Message(message);
				#endif
			Sleep(1000);
			return VS_ERROR;
		}
	}

	// Report ALL ok
	sprintf(
			message,
			"SHARED MEMORY: %s - SHARING succeeded with %s named memory.",
			app_name,
			mem_name
		);
		#ifdef NOT_VS
		printf(message);
		printf("\n");
		#else
		sys_output->Message(message);
		#endif


	// ============================== NOW HANDLE VIRTUAL CAMERA(S) ==============================

	// Initialise "this" streaming virtual camera
	if ( mem_usage_mode == 'S' )
	{
		// NOTE: IN streaming mode NO virtual cameras are ACTIVE
		;
	}
	else
	{
		// Report virtual cameras
		for (UINT i = 0; i < VS_MAX_LOGICAL_CAMERAS; i++)
		{
			// Get virtual camera status
			result = vs_system::VS_SetVirtualCameraStatus(
					i,
					i == 0 ? vs_system::CAMERA_ACTIVE : vs_system::CAMERA_IDLE,
					mem_buffer
				);

			// Report
			sprintf(
					message,
					"SHARED MEMORY: %s - Virtual Camera with logical id # %d is %s.",
					app_name,
					i + 1,
					i == 0 ? "ACTIVE" : "IDLE"
				);
				#ifdef NOT_VS
				printf(message);
				printf("\n");
				#else
				sys_output->Message(message);
				#endif
		}
	}

	// Seperate from setup
	#ifdef NOT_VS
	printf("****************************************************************************\n");
	#else
	sys_output->Message("****************************************************************************");
	#endif

	// Done
	return VS_OK;
}


// ---------- Cleanup ----------
/*!
\brief Cleanup shared memory
\author Gareth Edwards
\return INT - ERROR <=0 < VS_OK
*/
INT Shared_Memory_Framework::Cleanup()
{
	// Local
	INT result = 0;
	CHAR message[VS_MAXCHARLEN];

	// ============================== END ==============================

	// IF end fails THEN report ERROR
	if (result = vs_system::VS_EndSharedMemory(
				&mem_buffer,
				&mem_handle_map_file
			) == VS_ERROR)
		return VS_ERROR;

	// Report & return error
	sprintf(
			message,
			"SHARED MEMORY: %s - CLEANUP %s named memory.",
			app_name,
			mem_name
		);
		#ifdef NOT_VS
		printf(message);
		printf("\n");
		#else
		sys_output->Message(message);
		#endif

	// Seperate from setup
	#ifdef NOT_VS
	printf("****************************************************************************\n");
	#else
	sys_output->Message("****************************************************************************");
	#endif

	// Pause to allow user to see message
	Sleep(1000);

	// Done
	return VS_OK;
}



////////////////////////////////////////////////////////////////////////////////
