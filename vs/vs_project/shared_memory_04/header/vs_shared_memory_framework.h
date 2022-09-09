////////////////////////////////////////////////////////////////////////////////

// ---------- vs_shared_framework.h ----------
/*!
\file vs_shared_framework.h
\brief Declaration of shared memory API framework class and definitions.
\copyright Copyright 2015 Observant Technology Limited. All rights reserved.
\license This project is released under NDA.
\author Gareth Edwards
*/

#ifndef VS_SYSTEM_SHARED_MEMORY_FRAMEWORK
#define VS_SYSTEM_SHARED_MEMORY_FRAMEWORK


// ---- system include ----
#include <stdio.h>   // FILE, 
#include <string.h>  // printf,
#include <ctype.h>   // toupper, 


// ---- vs include ----
#include "../../../vs_system/header/vs_inc_defs.h"
#include "../../../../vs/vs_system/header/vs_app_dependency.h"


// ---- vs library ----
#include "../../../../vs/vs_library/header/vs_img_buffer.h"
#include "../../../../vs/vs_library/header/vs_img_jpeglib.h"


// ---- shared memory include ----
#include "../header/vs_shared_memory.h"


// ---- circular image buffer ----
#	define VS_MAX_IMAGE_WIDTH  1920
#	define VS_MAX_IMAGE_HEIGHT 1080 


// ---------- namespace ----------
namespace vs_system
{


// ---------- Private Shared_Memory_Framework class ----------
class Shared_Memory_Framework
{

public:

	// Constructor/destructor
	Shared_Memory_Framework();
	~Shared_Memory_Framework();

	// ---------- Framework ----------
	INT Setup();
	INT Cleanup();

	// Application dependency pointers
	vs_system::AppDependency *app_dependency;
	vs_system::SysContext    *sys_context;
	vs_system::SysOutput     *sys_output;

	// Application
	CHAR app_name[VS_MAXCHARLEN];

	// Virtual Camera logical id ( 4 streaming only )
	UINT    vrt_cam_logical_id;
	UINT    vrt_cam_index;
	UINT    vrt_cam_total_updated;

	// Shared memory mapping
	BYTE    mem_usage_mode;           // *Either 'C'amera or 'S'tream
	CHAR    mem_name[VS_MAXCHARLEN];
	HANDLE  mem_handle_map_file;
	BYTE   *mem_buffer;
	DWORD   mem_buffer_size;
	ULONG   mem_process_id;
	ULONG   mem_camera_heartbeat;     // Process

	// Time
	DOUBLE  ms_start_time;
	DOUBLE  ms_end_time;
	DOUBLE  ms_total_time;
	ULONG   ss_now;
	USHORT  ss_ips;

}; //  class Shared_Memory_Framework


} // vs_system namespace


#endif // --- VS_SYSTEM_SHARED_MEMORY_FRAMEWORK