////////////////////////////////////////////////////////////////////////////////

// ---------- vs_shared_memeory_structs.h ----------
/*!
\file vs_shared_memeory_structs.h
\brief Declaration of shared memory API structures and definitions.
\copyright Copyright 2015 Observant Technology Limited. All rights reserved.
\license This project is released under NDA.
\author Gareth Edwards
\note

Note 1: Struct size is aligned to multiples of 4 unless
        block size is set via #pragma pack (2).

Note 2: Shared memory is structured thus:

   AccessSharedMemory which is comprised of a
      ControlSharedMemory and
      VirtualCamera * VS_MAX_LOGICAL_CAMERAS.

   Each image is comprised of an
	   image header and
	   uncompressed image RGB data

Note 3: Shared memory is initialised by:

   a. Initialising AccessSharedMemory's
	   - instance of ControlSharedMemory, and
		- VS_MAX_LOGICAL_CAMERAS * VirtualCamera

   b. Get size of the required shared memory

   c. Create shared memory of this size

   d. Block copy into shared memory:
      - AccessSharedMemory struct, and
      - VS_MAX_LOGICAL_CAMERAS * image header AND uncompressed image RGB data

*/

#ifndef VS_SYSTEM_SHARED_MEMORY_STRUCTS
#define VS_SYSTEM_SHARED_MEMORY_STRUCTS


// ---------- size definitions ----------
#define VS_MAX_LOGICAL_CAMERAS 4           // Maximum number of cameras
#define VS_MAX_TEXT_LEN        32          // Maximum text length


// ---------- type definitions ----------
/*
#define BOOL    bool
#define CHAR    char
#define BYTE    unsigned char
#define UCHAR   unsigned char
#define DWORD   unsigned long
#define ULONG   unsigned long
#define WORD    unsigned short
#define UINT    unsigned int
#define INT     int
#define SHORT   short
#define LONG    long
#define FLOAT   float
#define DOUBLE  double
#define VOID    void
*/


// ---------- Windows include ----------
#include <windows.h>


// ---------- namespace ----------
namespace vs_system
{


// ---------- Enumerate control access status values ----------
//
enum {
	ACCESS_NOT_READY = 0,                   // All access
	ACCESS_RELEASE = 1,                     // Not yet implemented!
	ACCESS_READY = 2,                       // All access
	ACCESS_DONE = 3,                        // All access
};


// ---------- Enumerate control process status values ----------
//
enum {
	PROCESS_IDLE = 0,                       // Owner access
	PROCESS_ACTIVE = 1,                     // Owner access
	PROCESS_CLOSING_DOWN = 2,               // Not yet implemented!
};


// ---------- Enumerate virtual camera status values ----------
//
enum {
	CAMERA_IDLE = 0,                        // Owner access
	CAMERA_ACTIVE = 1                       // Owner access
};


// ---------- Enumerate image buffer format values ----------
//
enum {
	BUFFER_IMAGE = 0,                       // Not yet implemented
	BUFFER_BMP = 1,                         // Only format available!
	BUFFER_TGA = 2                          // Not yet implemented
};


// ---------- Used to hold data for controling shared memory ----------
//
// Note: 92 bytes
//
typedef struct SharedMemoryControl
{
	// Housekeeping
	USHORT access_status;                   // Enumerated shared memory status, NOT_READY, RELEASE, READY, etc.
	INT    owner_heartbeat;                 // Incremented each iteration by "owner"
	INT    camera_heartbeat;                // Incremented each iteration by "camera"
	ULONG  process_id;                      // Used to identity "owner" of shared memory
	USHORT process_status;                  // Enumerated "owner" status, IDLE, ACTIVE, CLOSING_DOWN, etc.

	// These image parameters MUST be set by the "source" application
	INT    vc_index;                        // Index of active virtual camera
	USHORT fps;                             // Frames per second
	USHORT max_width;                       // Maximum X width of image
	USHORT max_height;                      // Maximum Y height of image
	ULONG  time_stamp;                      // Time stamp in milliseconds, as per MS timeGetTime function

	// Text
	CHAR   memory_name[VS_MAX_TEXT_LEN];    // e.g. "OBSERVANTxxxxxx", etc....
	CHAR   message[VS_MAX_TEXT_LEN];        // e.g. "Shutting Down", etc

} shared_memory_control;


// ---------- Used to hold data for every virtual camera defined ----------
//
// Note: 44 bytes
//
typedef struct VirtualCamera
{
	// Housekeeping
	USHORT access_status;                   // Enumerated status, NOT_READY, RELEASE, READY, etc.
	INT    heartbeat;                       // Incremented each iteration
	USHORT logical_id;                      // Identity, which must be non-zero and unique
	USHORT camera_status;                   // Enumerated status, IDLE, ACTIVE, etc.
	ULONG  process_id;                      // Used to identity "owner" of virtual camera
	
	// View
	FLOAT  x_lookat;                        // X lookat proportional to 2D panorama width ( 0.0 <= X <= 1.0 )
	FLOAT  y_lookat;                        // Y lookat proportional to 2D panorama height ( 0.0 <= X <= 1.0 )
	FLOAT  vertical_fov;                    // Vertical Field Of View (FOV)
	
	// Image
	USHORT width;                           // X width of image
	USHORT height;                          // Y height of image
	USHORT buffer_format;                   // Enumerated format of buffer, e.g. BMP, JPEG, IMG
	USHORT buffer_status;                   // Reserved
	UINT   fb_byte_offset;                  // Front buffer header data offset
	UINT   bb_byte_offset;                  // Back buffer header data offset - Not yet implmented!

} virtual_camera;


// ---------- Used to access shared memory ----------
//
// Note: 732 bytes
//
typedef struct SharedMemoryAccess
{
	SharedMemoryControl  control;
	VirtualCamera        camera[VS_MAX_LOGICAL_CAMERAS];

} shared_memory_access;


// ---------- BMP header ----------
//
// Note: 54 bytes
//
// Note: This is supplied by Videalert as the required header, and is taken
// from the Windows GDI ( Graphics Device Interface) header, however it does
// not appear to conform to the most recent BMP header definitions.
//
// Note: Observant does not need to reference RGB data via a BMP header!!!
//
#pragma pack (2)
typedef struct BMPHeader
{
	SHORT   type;
	INT     size;
	SHORT   reserved1;
	SHORT   reserved2;
	INT     offset_bits;
	INT     bytes_in_hdr;
	INT     width;
	INT     height;
	SHORT   colour_planes;
	SHORT   bits_pixel;
	INT     rgb_compress;
	INT     size_bmp_data;
	INT     hor_res;
	INT     vert_res;
	INT     no_colours;
	INT     colours_imp;
} bmp_header;
#pragma pack (8)

} // --- namespace vs_system


#endif // --- VS_SYSTEM_SHARED_MEMORY_STRUCTS