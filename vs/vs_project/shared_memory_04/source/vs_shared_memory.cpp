////////////////////////////////////////////////////////////////////////////////

// ---------- vs_shared_memory.cpp ----------
/*!
\file vs_shared_memory.cpp
\brief Implementation of the shared memory API functions.
\copyright Copyright 2015 Observant Technology Limited. All rights reserved.
\license This project is released under NDA.
\author Gareth Edwards

*/

// ---- definitions ----
#define _CRT_SECURE_NO_WARNINGS 1


// ---- include ----
#include "../header/vs_shared_memory.h"


// ---- namespace ----
using namespace vs_system;


////////////////////////////////////////////////////////////////////////////////


// Global: size of shared memory structs 
const INT size_of_SharedMemoryAccess = sizeof(SharedMemoryAccess);
const INT size_of_SharedMemoryControl = sizeof(SharedMemoryControl);
const INT size_of_VirtualCamera = sizeof(VirtualCamera);
const INT size_of_BMPHeader = sizeof(BMPHeader);

// SharedMemoryControl access union
union u_smc_and_byte
{
	SharedMemoryControl shared_memory_control;
	BYTE bytes[size_of_SharedMemoryControl];
};

// VirtualCamera access union
union u_vc_and_byte
{
	VirtualCamera virtual_camera;
	BYTE bytes[size_of_VirtualCamera];
};

// SharedMemoryAccess access union
union u_sma_and_byte
{
	SharedMemoryAccess shared_memory_access;
	BYTE bytes[size_of_SharedMemoryAccess];
};

// BMPHeader access union
union u_bmp_and_byte
{
	BMPHeader bmp_header;
	BYTE bytes[size_of_BMPHeader];
};

// Global:
//
// This instance of SharedMemoryAccess is used as a scratchpad during
// the initialisation of shared memeory. It contains the "current"
// instance of a SharedMemoryControl struct plus the VirtualCamera
// structs ( # == VS_MAX_LOGICAL_CAMERAS ). Once these have been
// intialised via functions VS_InitialiseSharedMemoryAccess and
// VS_InitialiseVirtualCamera, the contents are used to calculate
// the size of shared memory by the function
// VS_InitialiseGetSizeOfSharedMemory.
//
// The contents are then block copied into shared memeory by the
// function VS_InitialiseSharedMemory.
//
// Note: In future versions of this function library this instance of
// SharedMemoryAccess MAY be required to be externally instantiated
// and passed as an argument to the functions mentioned above. 
//
static SharedMemoryAccess initialise_scratchpad;


////////////////////////////////////////////////////////////////////////////////


// ============================== GET ==============================


// ---------- VS_GetControlAccessStatus ----------
/*!
\brief Get control access status
\author Gareth Edwards
\param CHAR * - [out] Current enumerated access status: NOT_READY, RELEASE, READY
\param BYTE * - [in] Pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_GetControlAccessStatus(USHORT *access, BYTE *map_buffer)
{
	// Check
	if ( access == NULL ) return VS_ERROR;
	if ( map_buffer == NULL ) return VS_ERROR;

	// Get
	u_smc_and_byte s;
	memcpy(s.bytes, map_buffer, size_of_SharedMemoryControl);
	*access = s.shared_memory_control.access_status;

	// Done
	return VS_OK;
}


// ---------- VS_GetControlFPS ----------
/*!
\brief Get control "source" fps
\author Gareth Edwards
\param UINT * - [out] Current fps, which MUST be set each iteration by "source"
\param BYTE * - [in] Pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_GetControlFPS(UINT *fps, BYTE *map_buffer)
{
	// Check
	if ( fps == NULL ) return VS_ERROR;
	if ( map_buffer == NULL ) return VS_ERROR;

	// Get
	u_smc_and_byte s;
	memcpy(s.bytes, map_buffer, size_of_SharedMemoryControl);
	*fps = s.shared_memory_control.fps;

	// Done
	return VS_OK;
}


// ---------- VS_GetControlOwnerHeartbeat ----------
/*!
\brief Get control "owner" heartbeat
\author Gareth Edwards
\param INT * - [out] Current heartbeat, which MUST be incremented each iteration by "owner"
\param BYTE * - [in] Pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_GetControlOwnerHeartbeat(INT *owner_heartbeat, BYTE *map_buffer)
{
	// Check
	if ( owner_heartbeat == NULL ) return VS_ERROR;
	if ( map_buffer == NULL ) return VS_ERROR;

	// Get
	u_smc_and_byte s;
	memcpy(s.bytes, map_buffer, size_of_SharedMemoryControl);
	*owner_heartbeat = s.shared_memory_control.owner_heartbeat;

	// Done
	return VS_OK;
}


// ---------- VS_GetControlCameraHeartbeat ----------
/*!
\brief Get control "camera" heartbeat
\author Gareth Edwards
\param INT * - [out] Current heartbeat, which MUST be incremented each iteration by "camera"
\param BYTE * - [in] Pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_GetControlCameraHeartbeat(INT *camera_heartbeat, BYTE *map_buffer)
{
	// Check
	if ( camera_heartbeat == NULL ) return VS_ERROR;
	if ( map_buffer == NULL ) return VS_ERROR;

	// Get
	u_smc_and_byte s;
	memcpy(s.bytes, map_buffer, size_of_SharedMemoryControl);
	*camera_heartbeat = s.shared_memory_control.camera_heartbeat;

	// Done
	return VS_OK;
}


// ---------- VS_GetControlMessage ----------
/*!
\brief Set control message
\author Gareth Edwards
\param CHAR * - [out] Message that was "sent"
\param BYTE * - [in] Pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_GetControlMessage(
	CHAR* message,
	BYTE *map_buffer
	)
{
	// Check
	if ( message == NULL ) return VS_ERROR;
	if ( map_buffer == NULL ) return VS_ERROR;

	// Get
	u_smc_and_byte s;
	memcpy(s.bytes, map_buffer, size_of_SharedMemoryControl);
	strcpy(message, s.shared_memory_control.message);

	// Done
	return VS_OK;
}


// ---------- VS_GetControlProcessStatus ----------
/*!
\brief Get control process status
\author Gareth Edwards
\param USHORT * - [out] Current enumerated process status: IDLE, ACTIVE, CLOSING_DOWN
\param BYTE * - [in] Pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_GetControlProcessStatus(USHORT *process_status, BYTE *map_buffer)
{
	// Check
	if ( process_status == NULL ) return VS_ERROR;
	if ( map_buffer == NULL ) return VS_ERROR;
	
	// Get
	u_smc_and_byte s;
	memcpy(s.bytes, map_buffer, size_of_SharedMemoryControl);
	*process_status = s.shared_memory_control.process_status;

	// Done
	return VS_OK;
}


// ---------- VS_GetControlTimeStamp ----------
/*!
\brief Get control "source" time stamp
\author Gareth Edwards
\param ULONG * - [out] "source" time stamp in milliseconds, as per MS timeGetTime function
\param BYTE * - [in] Pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_GetControlTimeStamp(ULONG *time_stamp, BYTE *map_buffer)
{
	// Check
	if ( time_stamp == NULL ) return VS_ERROR;
	if ( map_buffer == NULL ) return VS_ERROR;

	// Get
	u_smc_and_byte s;
	memcpy(s.bytes, map_buffer, size_of_SharedMemoryControl);
	*time_stamp = s.shared_memory_control.time_stamp;

	// Done
	return VS_OK;
}


// ---------- VS_GetSizeOfSharedMemory ----------
/*!
\brief Get size of shared memory
\author Gareth Edwards
\return Size (in bytes) of required shared memory
*/
INT vs_system::VS_GetSizeOfSharedMemory(BYTE *map_buffer)
{
	// Note: sizeof(access_shared_memory) =
	//    sizeof(control_shared_memory) +
	//    sizeof(logical_camera) * VS_MAX_LOGICAL_CAMERAS
	//
	UINT size = size_of_SharedMemoryAccess;

	// Add size of each virtual camera's bmp data
	for ( UINT i=0; i<VS_MAX_LOGICAL_CAMERAS; i++ )
	{

		// Calculate offset to virtual camera
		INT offset = sizeof(SharedMemoryControl) + i * sizeof(VirtualCamera);

		// Get virtual camera
		VirtualCamera camera;
		memcpy(&camera, map_buffer + offset, sizeof(VirtualCamera));

		// Get bmp header data
		BMPHeader bmp;
		memcpy(&bmp, map_buffer + camera.fb_byte_offset, sizeof(BMPHeader));

		size += sizeof(BMPHeader);
		size += bmp.size_bmp_data;
	}

	// Done
	return size;
}


// ---------- VS_GetVirtualCamera ----------
/*!
\brief Get a Virtual Camera
\author Gareth Edwards
\param VirtualCamera * - [out] Pointer to Virtual Camera
\param UINT - [in] Index ( 0 <= index < VS_MAX_LOGICAL_CAMERAS)
\param BYTE * - [in] Pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_GetVirtualCamera(VirtualCamera* camera, UINT index, BYTE *map_buffer)
{
	// Check
	if ( camera == NULL ) return VS_ERROR;
	if ( map_buffer == NULL ) return VS_ERROR;
	if ( index >= VS_MAX_LOGICAL_CAMERAS ) return VS_ERROR;

	// Offset
	INT offset = size_of_SharedMemoryControl + (index * size_of_VirtualCamera);

	// Get
	memcpy(camera, map_buffer+offset, size_of_VirtualCamera);

	// Done
	return VS_OK;
}


// ---------- VS_GetVirtualCameraBufferHeader ----------
/*!
\brief Get Virtual Camera buffer header
\author Gareth Edwards
\param BYTE * - [out] Pointer to Virtual Camera image buffer header
\param UINT - [in] Index ( 0 <= index < VS_MAX_LOGICAL_CAMERAS)
\param BYTE * - [in] Pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_GetVirtualCameraBufferHeader(
	BYTE *buffer_header,
	UINT index,
	BYTE *map_buffer
	)
{
	// Check
	if ( buffer_header == NULL ) return VS_ERROR;
	if ( map_buffer == NULL ) return VS_ERROR;
	if ( index >= VS_MAX_LOGICAL_CAMERAS ) return VS_ERROR;

	// Offset
	INT offset = size_of_SharedMemoryControl + (index * size_of_VirtualCamera);

	// Get camera
	VirtualCamera camera;
	memcpy(&camera, map_buffer+offset, size_of_VirtualCamera);

	// IF buffer header pointer is NULL THEN return error
	if ( camera.fb_byte_offset == 0 ) return VS_ERROR;

	// Get buffer header
	switch ( camera.buffer_format )
	{
	case BUFFER_BMP:
		memcpy(buffer_header, map_buffer+camera.fb_byte_offset, size_of_BMPHeader);
		break;
	default:
		return VS_ERROR;
		break;
	}

	// Done
	return VS_OK;
}


// ---------- VS_GetVirtualCameraStatus ----------
/*!
\brief Get virtual camera status
\author Gareth Edwards
\param USHORT * - [out] Current enumerated camera status: IDLE, ACTIVE, etc.
\param UINT - [in] Index ( 0 <= index < VS_MAX_LOGICAL_CAMERAS)
\param BYTE * - [in] Pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_GetVirtualCameraStatus(
		USHORT *camera_status,
		UINT index,
		BYTE *map_buffer
	)
{
	// Check
	if ( camera_status == NULL ) return VS_ERROR;
	if ( map_buffer == NULL ) return VS_ERROR;
	if ( index >= VS_MAX_LOGICAL_CAMERAS )  return VS_ERROR;

	// Offset
	INT offset = size_of_SharedMemoryControl + (index * size_of_VirtualCamera);

	// Get camera
	VirtualCamera camera;
	memcpy(&camera, map_buffer+offset, size_of_VirtualCamera);

	// Set
	*camera_status = camera.camera_status;

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ============================== INCREMENT ==============================


// ---------- VS_IncrementControlOwnerHeartbeat ----------
/*!
\brief Increment control "owner" heartbeat count
\author Gareth Edwards
\param BYTE * - [in] Pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_IncrementControlOwnerHeartbeat(BYTE *map_buffer)
{
	// Check
	if ( map_buffer == NULL ) return VS_ERROR;

	// Set
	u_smc_and_byte s;
	memcpy(s.bytes, map_buffer, size_of_SharedMemoryControl);
	s.shared_memory_control.owner_heartbeat++;
	memcpy(map_buffer, s.bytes, size_of_SharedMemoryControl);

	// Done
	return VS_OK;
}


// ---------- VS_IncrementControlCameraHeartbeat ----------
/*!
\brief Increment control "camera" heartbeat count
\author Gareth Edwards
\param BYTE * - [in] Pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_IncrementControlCameraHeartbeat(BYTE *map_buffer)
{
	// Check
	if ( map_buffer == NULL ) return VS_ERROR;

	// Set
	u_smc_and_byte s;
	memcpy(s.bytes, map_buffer, size_of_SharedMemoryControl);
	s.shared_memory_control.camera_heartbeat++;
	memcpy(map_buffer, s.bytes, size_of_SharedMemoryControl);

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ============================== Initialise ==============================


// ---------- VS_InitialiseSharedMemoryAccess ----------
/*!
\brief Initialise access to shared memory
\author Gareth Edwards
\param CHAR * - [in] Unique name of shared memory
\param ULONG  - [in] Used to identity "owner" of virtual camera
\param USHORT - [in] X maximum width of image
\param USHORT - [in] Y maximum height of image
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_InitialiseSharedMemoryAccess(
		CHAR *memory_name,
		ULONG process_id,
		USHORT max_width,
		USHORT max_height
	)
{
	// Initialise control
	SharedMemoryControl *control = &initialise_scratchpad.control;
	control->access_status    = ACCESS_NOT_READY;
	control->owner_heartbeat  = 0;
	control->camera_heartbeat = 0;
	control->process_id       = process_id;
	control->process_status   = PROCESS_IDLE;

	// These MUST be set by the application that
	// writes images data into shared memory.
	// 
	// ... and MUST NOT be set by "owner"
	//
	control->vc_index         = -1;
	control->fps              = 0;
	control->max_width        = max_width;
	control->max_height       = max_height;
	control->time_stamp       = 0;

	// Initialise text
	if ( memory_name[0] == NULL ) return VS_ERROR;
	strcpy(control->memory_name, memory_name);

	// Initialise virtual cameras
	for ( UINT i=0; i<VS_MAX_LOGICAL_CAMERAS; i++ )
	{
		VirtualCamera *camera = &initialise_scratchpad.camera[i];
		camera->access_status  = ACCESS_NOT_READY;
		camera->logical_id     = 0;
		camera->process_id     = 0;
		camera->camera_status  = CAMERA_IDLE;
		camera->x_lookat       = 0.5;
		camera->y_lookat       = 0.5;
		camera->vertical_fov   = 40.0;
		camera->width          = 400;
		camera->height         = 300;
		camera->buffer_format  = 0;
		camera->fb_byte_offset = 0;
		camera->bb_byte_offset = 0;
	}

	// OK
	return VS_OK;
}


// ---------- VS_InitialiseVirtualCamera ----------
/*!
\brief Initialise a virtual camera
\author Gareth Edwards
\param USHORT - [in] Index ( 0 <= logical_id < VS_MAX_LOGICAL_CAMERAS)
\param USHORT - [in] Identity, which must be non-zero and unique
\param USHORT - [in] Enumerated IDLE, ACTIVE, CLOSING_DOWN, etc.
\param ULONG  - [in] Used to identity "owner" of virtual camera
\param FLOAT  - [in] X lookat proportional to 2D panorama width ( 0.0 <= X <= 1.0 )
\param FLOAT  - [in] Y lookat proportional to 2D panorama height ( 0.0 <= Y <= 1.0 )
\param FLOAT  - [in] Vertical Field Of View (FOV)
\param USHORT - [in] X maximum width of image
\param USHORT - [in] Y maximum height of image
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_InitialiseVirtualCamera(
		USHORT index,
		USHORT logical_id,
		USHORT camera_status,
		ULONG  process_id,
		FLOAT  x_lookat,
		FLOAT  y_lookat,
		FLOAT  vertical_fov,
		USHORT width,
		USHORT height
	)
{
	// IF NOT legal index THEN
	if ( index >= VS_MAX_LOGICAL_CAMERAS ) return VS_ERROR;

	// IF NOT legal logical id THEN
	if ( logical_id == 0 ) return VS_ERROR;

	// IF NOT legal width THEN
	if ( width == 0 ) return VS_ERROR;

	// IF NOT legal height THEN
	if ( height == 0 ) return VS_ERROR;

	// Maximum size of ALL camera image data
	UINT max_width = initialise_scratchpad.control.max_width;
	UINT max_height = initialise_scratchpad.control.max_height;

	// Initialise
	VirtualCamera *camera = &initialise_scratchpad.camera[index];
	camera->access_status  = ACCESS_NOT_READY;
	camera->logical_id     = logical_id;
	camera->camera_status  = camera_status;
	camera->process_id     = process_id;
	camera->x_lookat       = x_lookat;
	camera->y_lookat       = y_lookat;
	camera->vertical_fov   = vertical_fov;
	camera->width          = width > max_width ? max_width : width;
	camera->height         = height > max_height ? max_height : height;
	camera->buffer_format  = BUFFER_BMP;
	camera->fb_byte_offset = 0;
	camera->bb_byte_offset = 0;

	// Done
	return VS_OK;
}


// ---------- VS_InitialiseGetSizeOfSharedMemory ----------
/*!
\brief Get initialisation size of shared memory
\author Gareth Edwards
\return Size (in bytes) of required shared memory
*/
INT vs_system::VS_InitialiseGetSizeOfSharedMemory()
{
	// Note: sizeof(access_shared_memory) =
	//    sizeof(control_shared_memory) +
	//    sizeof(logical_camera) * VS_MAX_LOGICAL_CAMERAS
	//
	UINT size = size_of_SharedMemoryAccess;

	// Maximum size of ALL camera image data
	UINT size_of_image_data =
		initialise_scratchpad.control.max_width *
		initialise_scratchpad.control.max_height *
		3;

	// Add size of each virtual camera's image data
	//
	// Note: Camera with a ZERO logical id have zero size!!!
	//
	for (UINT i = 0; i<VS_MAX_LOGICAL_CAMERAS; i++)
	{
		VirtualCamera *camera = &initialise_scratchpad.camera[i];
		if (camera->logical_id != 0)
		{
			size += size_of_BMPHeader;
			size += size_of_image_data;
		}
	}

	// Done
	return size;
}


// ---------- VS_InitialiseSharedMemory ----------
/*!
\brief Initialise shared memory
\author Gareth Edwards
\param BYTE * - [in] Pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_InitialiseSharedMemory(
		BYTE* buffer
	)
{
	// Local
	BYTE *buffer_ptr = buffer;

	// Increment pointer past shared memory access
	buffer_ptr += size_of_SharedMemoryAccess;

	// Maximum size of ALL camera image data
	UINT size_of_image_data =
		initialise_scratchpad.control.max_width *
		initialise_scratchpad.control.max_height *
		3;

	// Copy virtual camera bmp headers and image data
	for ( UINT i=0; i<VS_MAX_LOGICAL_CAMERAS; i++ )
	{
		// Accelerate virtual camera
		VirtualCamera *camera = &initialise_scratchpad.camera[i];

		// IF virtual camera exists (non-zero logical id)
		if ( camera->logical_id != 0 )
		{
			// BMP - TBD
			bmp_header bmp;
			bmp.type          = 0;
			bmp.size          = 0;
			bmp.reserved1     = 0;
			bmp.reserved2     = 0;
			bmp.offset_bits   = 0;

			// BMP - OK
			bmp.bytes_in_hdr  = size_of_BMPHeader;
			bmp.width         = camera->width;
			bmp.height        = camera->height;
			bmp.colour_planes = 3;
			bmp.bits_pixel    = 8;

			// BMP - TBD
			bmp.rgb_compress  = 0;
			bmp.size_bmp_data = camera->width * camera->height * 3;
			bmp.hor_res       = 0;
			bmp.vert_res      = 0;
			bmp.no_colours    = 0;
			bmp.colours_imp   = 0;

			// Store byte offset to header data
			camera->fb_byte_offset = (UINT)(buffer_ptr-buffer);
			camera->bb_byte_offset  = (UINT)(buffer_ptr-buffer);

			// Copy header data into shared memory
			memcpy(buffer_ptr, &bmp, size_of_BMPHeader);
			buffer_ptr += size_of_BMPHeader;

			// Copy image data into shared memory
			memset(buffer_ptr, '#', bmp.size_bmp_data);
			for ( UINT i=0; i<100; i++ ) *(buffer_ptr+i) = i + ' '; // For debug: writes ascii char data
			buffer_ptr += size_of_image_data; // WAS bmp.size_bmp_data before max width and height

		}
	}

	// NOW - Block copy access into shared memory WITH pointer values
	memcpy(buffer, &initialise_scratchpad, size_of_SharedMemoryAccess);

	// 

	// OK
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ============================== SET ==============================


// ---------- VS_SetControlAccessStatus ----------
/*!
\brief Set control access status
\author Gareth Edwards
\param USHORT - [in] Enumerated access status: ACCESS_NOT_READY, ACCESS_RELEASE, ACCESS_READY or ACCESS_DONE
\param BYTE * - [in] Pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_SetControlAccessStatus(
		USHORT access_status,
		BYTE *map_buffer
	)
{
	// Check
	if ( map_buffer == NULL ) return VS_ERROR;
	
	// Set
	switch ( access_status )
	{
		case ACCESS_NOT_READY:
		case ACCESS_RELEASE:
		case ACCESS_READY:
		case ACCESS_DONE:
			u_smc_and_byte s;
			memcpy(s.bytes, map_buffer, size_of_SharedMemoryControl);
			s.shared_memory_control.access_status = access_status;
			memcpy(map_buffer, s.bytes, size_of_SharedMemoryControl);
			break;

		default:
			return VS_ERROR;
			break;
		}

	// Done
	return VS_OK;
}


// ---------- VS_SetControlFPS ----------
/*!
\brief Set control "source" fps
\author Gareth Edwards
\param UINT - [in] Current fps, which MUST be set each iteration by "source"
\param BYTE * - [in] Pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_SetControlFPS(
		UINT fps,
		BYTE *map_buffer
	)
{
	// Check
	if ( map_buffer == NULL ) return VS_ERROR;

	// Set
	u_smc_and_byte s;
	memcpy(s.bytes, map_buffer, size_of_SharedMemoryControl);
	s.shared_memory_control.fps = fps;
	memcpy(map_buffer, s.bytes, size_of_SharedMemoryControl);

	// Done
	return VS_OK;
}


// ---------- VS_SetControlHeartbeat ----------
/*!
\brief Set control heartbeat count
\author Gareth Edwards
\param INT - [in] Heartbeat, which MUST be incremented each iteration by "owner"
\param BYTE * - [in] Pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_SetControlHeartbeat(
		INT heartbeat,
		BYTE *map_buffer
	)
{
	// Check
	if ( map_buffer == NULL ) return VS_ERROR;

	// Set
	u_smc_and_byte s;
	memcpy(s.bytes, map_buffer, size_of_SharedMemoryControl);
	s.shared_memory_control.owner_heartbeat = heartbeat;
	memcpy(map_buffer, s.bytes, size_of_SharedMemoryControl);

	// Done
	return VS_OK;
}


// ---------- VS_SetControlMessage ----------
/*!
\brief Set control message
\author Gareth Edwards
\param CHAR * - [in] text less than VS_MAX_TEXT_LEN characters
\param BYTE * - [in] pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_SetControlMessage(
		CHAR* message,
		BYTE *map_buffer
	)
{
	// Check
	if ( message[0] == NULL ) return VS_ERROR;
	if ( strlen(message) > VS_MAX_TEXT_LEN - 1 ) return VS_ERROR;
	if ( map_buffer == NULL ) return VS_ERROR;

	// Set
	u_smc_and_byte s;
	memcpy(s.bytes, map_buffer, size_of_SharedMemoryControl);
	strcpy(s.shared_memory_control.message, message);
	memcpy(map_buffer, s.bytes, size_of_SharedMemoryControl);

	// Done
	return VS_OK;
}


// ---------- VS_SetControlProcessStatus ----------
/*!
\brief Set control process status
\author Gareth Edwards
\param USHORT - [in] enumerated process status: PROCESS_IDLE, PROCESS_ACTIVE or PROCESS_CLOSING_DOWN
\param BYTE * - [in] pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_SetControlProcessStatus(
		USHORT process_status,
		BYTE *map_buffer
	)
{
	// Check
	if ( map_buffer == NULL ) return VS_ERROR;

	// Set
	switch ( process_status )
	{
		case PROCESS_IDLE:
		case PROCESS_ACTIVE:
		case PROCESS_CLOSING_DOWN:
			u_smc_and_byte s;
			memcpy(s.bytes, map_buffer, size_of_SharedMemoryControl);
			s.shared_memory_control.process_status = process_status;
			memcpy(map_buffer, s.bytes, size_of_SharedMemoryControl);
			break;

		default:
			return VS_ERROR;
			break;
	}

	// Done
	return VS_OK;
}


// ---------- VS_SetControlTimeStamp ----------
/*!
\brief Set control time stamp
\author Gareth Edwards
\param ULONG - [in]  Time stamp in milliseconds, as per MS timeGetTime function
\param BYTE * - [in] pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_SetControlTimeStamp(
		ULONG time_stamp,
		BYTE *map_buffer
	)
{
	// Check
	if ( map_buffer == NULL ) return VS_ERROR;

	// Set
	u_smc_and_byte s;
	memcpy(s.bytes, map_buffer, size_of_SharedMemoryControl);
	s.shared_memory_control.time_stamp = time_stamp;
	memcpy(map_buffer, s.bytes, size_of_SharedMemoryControl);

	// Done
	return VS_OK;
}


// ---------- VS_SetVirtualCameraXYF ----------
/*!
\brief Set a virtual camera XYF ( X and Y proportional to 2D panorama width and height, and FOV)
\author Gareth Edwards
\param INT - index
\param USHORT - [in] enumerated camera status: CAMERA_IDLE or CAMERA_ACTIVE
\param FLOAT - [in] X proportional to 2D panorama width (converted to pan)
\param FLOAT - [in] Y proportional to 2D panorama height (converted to tilt)
\param FLOAT - [in] Vertical Field Of View (FOV)
\param BYTE * - [in] pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_SetVirtualCameraXYF(
		INT    index,
		USHORT camera_status,
		FLOAT  x_lookat,
		FLOAT  y_lookat,
		FLOAT  vertical_fov,
		BYTE  *map_buffer
	)
{
	// Check
	if ( index >= VS_MAX_LOGICAL_CAMERAS ) return VS_ERROR;
	if ( map_buffer == NULL ) return VS_ERROR;

	// Calculate offset to virtual camera #
	INT offset = sizeof(SharedMemoryControl) + index * sizeof(VirtualCamera);

	// Get virtual camera
	VirtualCamera camera;
	memcpy(&camera, map_buffer+offset, sizeof(VirtualCamera));
	camera.camera_status = camera_status;
	camera.x_lookat = x_lookat;
	camera.y_lookat = y_lookat;
	camera.vertical_fov = vertical_fov;
	memcpy(map_buffer+offset, &camera, sizeof(VirtualCamera));

	// Done
	return VS_OK;
}


// ---------- VS_SetVirtualCameraStatus ----------
/*!
\brief Set a virtual camera status
\author Gareth Edwards
\param INT - index
\param USHORT - [in] enumerated camera status: CAMERA_IDLE or CAMERA_ACTIVE
\param BYTE * - [in] pointer to shared memory mapped buffer
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_SetVirtualCameraStatus(
		INT    index,
		USHORT camera_status,
		BYTE   *map_buffer
	)
{
	// Check
	if ( index >= VS_MAX_LOGICAL_CAMERAS ) return VS_ERROR;
	if ( map_buffer == NULL ) return VS_ERROR;

	// Set index
	SharedMemoryControl control;
	memcpy(&control, map_buffer, sizeof(SharedMemoryControl));
	control.vc_index = camera_status == CAMERA_ACTIVE ? index : -1;
	memcpy(map_buffer, &control, sizeof(SharedMemoryControl));

	// Calculate offset to virtual camera #
	INT offset = sizeof(SharedMemoryControl) + index * sizeof(VirtualCamera);

	// Get virtual camera
	VirtualCamera camera;
	memcpy(&camera, map_buffer + offset, sizeof(VirtualCamera));
	camera.camera_status = camera_status;
	memcpy(map_buffer + offset, &camera, sizeof(VirtualCamera));

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ============================== SESSION ==============================


// ---------- VS_BeginSharedMemory ----------
/*!
\brief Begin shared memory
\author Gareth Edwards
\param CHAR* - [in] Name of the file mapping object
\param DWORD* - [out] Size of shared memeory
\param BYTE** - [out] Pointer to starting address of the mapped view
\param HANDLE* - [out] Handle to the newly created file mapping object
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_BeginSharedMemory(
		CHAR*   name,
		DWORD*  buffer_size,
		BYTE**  buffer,
		HANDLE *handle_map_file
	)
{
	/* ---

	// Local
	INT result;

	// Get size of required shared memory
	*buffer_size = vs_system::VS_GetSizeOfSharedMemory();

	// IF open ok THEN report
	if ( result = vs_system::VS_OpenSharedMemory(
				name,
				*buffer_size,
				buffer,
				handle_map_file
			) == VS_OK
		)
	{
#		ifdef VS_SHARED_MEMORY_REPORT
		printf("OPEN succeeded with %d bytes of shared memory.\n", (INT)*buffer_size);
#		else
		;
#		endif
	}
	// ELSE try to create
	else
	{
		// If create ok THEN
		if ( result = vs_system::VS_CreateSharedMemory(
				name,
				*buffer_size,
				buffer,
				handle_map_file
				) == VS_OK
			)
		{
			// Initialise shared memory
			result = vs_system::VS_InitialiseSharedMemory(*buffer);

			// Report
#			ifdef VS_SHARED_MEMORY_REPORT
			printf("CREATE succeeded with %d bytes of shared memory.\n", (INT)*buffer_size);
#			endif
		}
		// ELSE if create fails THEN report and exit
		else
		{
#			ifdef VS_SHARED_MEMORY_REPORT
			printf("EXITING because OPEN AND CREATE both failed\n");
#			endif
			Sleep(1000);
			return VS_ERROR;
		}
	}
	
	--- */

	// OK
	return VS_OK;
}

// ---------- VS_CreateSharedMemory ----------
/*!
\brief Start shared memory
\author Gareth Edwards
\param CHAR* - [in] Name of the file mapping object
\param DWORD - [in] Size of shared memeory
\param BYTE** - [out] Pointer to starting address of the mapped view
\param HANDLE* - [out] Handle to the newly created file mapping object
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_CreateSharedMemory(
		CHAR* name,
		DWORD buffer_size,
		BYTE** buffer,
		HANDLE *handle_map_file
	)
{
	// Create
	*handle_map_file = CreateFileMapping(
			INVALID_HANDLE_VALUE,    // use paging file
			NULL,                    // default security
			PAGE_READWRITE,          // read/write access
			0,                       // maximum object size (high-order DWORD)
			buffer_size,             // maximum object size (low-order DWORD)
			name                     // name of mapping object
		);

	// IF create failed THEN report and return VS_ERROR
	if ( *handle_map_file == NULL )
	{
#		ifdef VS_SHARED_MEMORY_REPORT
		printf("INITIALISE could not create file mapping object (Error %d).\n", GetLastError());
#		endif
		Sleep(1000);
		return VS_ERROR;
	}

	// Map
	*buffer = (BYTE *)MapViewOfFile(
			*handle_map_file,        // handle to map object
			FILE_MAP_ALL_ACCESS,     // read/write permission
			0,
			0,
			buffer_size
		);

	// IF mapping failed THEN report and return VS_ERROR
	if ( *buffer == NULL )
	{
#		ifdef VS_SHARED_MEMORY_REPORT
		printf("INITIALISE could not map view of file (Error %d).\n", GetLastError());
#		endif
		CloseHandle(*handle_map_file);
		Sleep(1000);
		return VS_ERROR;
	}

	// Create and mapping succeeded
	return VS_OK;
}


// ---------- VS_OpenSharedMemory ----------
/*!
\brief Open shared memory
\author Gareth Edwards
\param CHAR* - [in] Name of the file mapping object
\param DWORD - [in] Size of shared memeory
\param BYTE** - [out] Pointer to starting address of the mapped view
\param HANDLE* - [out] Handle to the newly created file mapping object
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_OpenSharedMemory(
		CHAR* name,
		DWORD buffer_size,
		BYTE** buffer,
		HANDLE *handle_map_file
	)
{
	// Open
	*handle_map_file = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		name);                 // name of mapping object

	// IF open failed THEN report and return error
	if ( *handle_map_file == NULL )
	{
#     ifdef VS_SHARED_MEMORY_REPORT
		; // printf("OPEN could not open file mapping (Error %d).\n", GetLastError());
#		endif
		return VS_ERROR;
	}

	// Mapping
	*buffer = (BYTE *)MapViewOfFile(
		*handle_map_file,      // handle to map object
		FILE_MAP_ALL_ACCESS,   // read/write permission
		0,
		0,
		buffer_size);

	// IF mapping failed THEN report and return error
	if ( *buffer == NULL )
	{
#     ifdef VS_SHARED_MEMORY_REPORT
		printf("OPEN could not map view of file (Error %d).\n", GetLastError());
#		endif
		CloseHandle(*handle_map_file);
		return VS_ERROR;
	}

	// OK
	return VS_OK;
}


// ---------- VS_EndSharedMemory ----------
/*!
\brief Dismiss shared memory
\author Gareth Edwards
\param BYTE** - [in] Starting address of the mapped view
\param HANDLE* - [in] Handle to the newly created file mapping object
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_EndSharedMemory(
		BYTE** buffer,
		HANDLE *handle_map_file
	)
{
	UnmapViewOfFile(*buffer);
	
	// FAILS: CloseHandle(*handle_map_file);

	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ============================== SESSION ==============================


// ---------- VS_TestSharedMemory ----------
/*!
\brief Test shared memory
\author Gareth Edwards
\param BYTE* - [in] Pointer to starting address of the mapped view
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_TestSharedMemory(
		BYTE* buffer
	)
{
	// Local
	INT result;

	// TEST access to shared memory ==============================
	if ( 1 )
	{
		using namespace vs_system;

		// Calculate offset to virtual camera # 1
		UINT index = 1;
		INT offset = sizeof(SharedMemoryControl) + index * sizeof(VirtualCamera);

		// Get virtual camera
		VirtualCamera camera;
		memcpy(&camera, buffer + offset, sizeof(VirtualCamera));

		// Get bmp header data
		BMPHeader bmp;
		memcpy(&bmp, buffer + camera.fb_byte_offset, sizeof(BMPHeader));

		// Get bmp image data
		BYTE *rgb_buffer = new BYTE[bmp.size_bmp_data];
		memcpy(
				rgb_buffer,
				buffer+camera.fb_byte_offset+bmp.bytes_in_hdr,
				bmp.size_bmp_data
			);

		delete[] rgb_buffer;

	}  // --- if ( 1 )


	// TEST API access to shared memory ==============================
	if ( 1 )
	{
		using namespace vs_system;

		// Get virtual camera
		VirtualCamera camera;
		UINT index = 1;
		result = VS_GetVirtualCamera(&camera, index, buffer);

		// Set BYTE * to appropriate type of buffer header
		BMPHeader bmp;
		BYTE *buffer_header = NULL;
		switch ( camera.buffer_format )
		{
			case BUFFER_BMP:
				buffer_header = (BYTE *)(&bmp);
				break;
			default:
				return VS_ERROR;
				break;
		}

		// Get buffer header
		result = VS_GetVirtualCameraBufferHeader(buffer_header, index, buffer);

		// Set BYTE * to appropriate type of buffer data
		BYTE *rgb_buffer = NULL;
		switch ( camera.buffer_format )
		{
			case BUFFER_BMP:
				rgb_buffer = buffer+camera.fb_byte_offset+bmp.bytes_in_hdr;
				break;
			default:
				return VS_ERROR;
				break;
		}

	}

	// OK
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ============================== TIME ==============================
//
// Provided to allow a 'W'rite application to generate a specified
// number of Frames Per Second (FPS), and to allow the 'R'ead
// application to test at a specified FPS.
//

// ---------- TimeInfo Struct ----------
struct VS_TimeInfo
{
	// Milliseconds throttle
	DOUBLE start_time    = 0;
	DOUBLE last_time     = 0;
	DOUBLE next_time     = 0;
	DOUBLE current_time  = 0;

	// Target time interval
	DOUBLE time_interval = 0;

	// Frame counter
	LONG frame = 0;
};

// Global instance
static VS_TimeInfo vs_time_info;


////////////////////////////////////////////////////////////////////////////////


// ---------- VS_GetFrameCount ----------
/*!
\brief Get frames since time started
\author Gareth Edwards
\return LONG - current frame count
*/
LONG vs_system::VS_GetFrameCount()
{
	return vs_time_info.frame;
}


// ---------- VS_GetNextTime ----------
/*!
\brief Get current time in milliseconds
\author Gareth Edwards
\return DOUBLE - milliseconds
*/
DOUBLE vs_system::VS_GetCurrentTime()
{
	vs_time_info.current_time = (DOUBLE)timeGetTime() - vs_time_info.start_time;
	return vs_time_info.current_time;
}


// ---------- VS_GetNextTime ----------
/*!
\brief Get time at which current second ends in milliseconds
\author Gareth Edwards
\return DOUBLE - milliseconds
*/
DOUBLE vs_system::VS_GetNextTime()
{
	return vs_time_info.next_time;
}


// ---------- VS_IncrementTime ----------
/*!
\brief Increment time
\author Gareth Edwards
\note ONLY invoked AFTER VS_GetCurrentTime() > VS_GetNextTime()
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_IncrementTime()
{
	// Increment frame count
	++vs_time_info.frame;

	// Increment time
	vs_time_info.next_time += vs_time_info.time_interval;

	// IF next > elapsed THEN
	if ( vs_time_info.next_time < vs_time_info.current_time )
	{
		// Reset next to current plus interval
		vs_time_info.next_time = vs_time_info.current_time + vs_time_info.time_interval;
	}

	// Set last time to current time
	vs_time_info.last_time = vs_time_info.current_time;

	// Done
	return VS_OK;
}


// ---------- VS_StartTime ----------
/*!
\brief Start (initialise) time
\author Gareth Edwards
\return INT - Error, VS_ERROR <= 0 < VS_OK
*/
INT vs_system::VS_StartTime(UINT fps)
{
	// Throttle
	vs_time_info.start_time    = (DOUBLE)timeGetTime();
	vs_time_info.last_time     = 0;
	vs_time_info.next_time     = 0;
	vs_time_info.current_time  = 0;

	// Target time interval
	vs_time_info.time_interval = 1.0f/(FLOAT)fps * 1000.0f;

	// Frame counter
	vs_time_info.frame         = 0;

	// Done
	return VS_OK;
}


////////////////////////////////////////////////////////////////////////////////
